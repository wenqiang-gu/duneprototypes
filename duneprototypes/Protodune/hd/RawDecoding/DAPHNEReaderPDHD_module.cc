////////////////////////////////////////////////////////////////////////
// Class:       DAPHNEReaderPDHD
// Plugin Type: producer (Unknown Unknown)
// File:        DAPHNEReaderPDHD_module.cc
//
// Generated at Fri Aug 18 12:13:50 2023 by Jacob Calcutt using cetskelgen
// from  version .
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "duneprototypes/Protodune/hd/ChannelMap/DAPHNEChannelMapService.h"



#include "detdataformats/daphne/DAPHNEFrame.hpp"
#include "detdataformats/daphne/DAPHNEStreamFrame.hpp"
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"
#include "dunecore/HDF5Utils/HDF5RawFile2Service.h"

#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/RecoBase/OpHit.h"
#include "TTree.h"
#include "art_root_io/TFileService.h"

#include <memory>
namespace pdhd {

using dunedaq::daqdataformats::SourceID;
using dunedaq::daqdataformats::Fragment;
using dunedaq::daqdataformats::FragmentType;
using DAPHNEStreamFrame = dunedaq::fddetdataformats::DAPHNEStreamFrame;
using DAPHNEFrame = dunedaq::fddetdataformats::DAPHNEFrame;

//For brevity 
using WaveformVector = std::vector<raw::OpDetWaveform>;

class DAPHNEReaderPDHD;


class DAPHNEReaderPDHD : public art::EDProducer {
public:
  explicit DAPHNEReaderPDHD(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  DAPHNEReaderPDHD(DAPHNEReaderPDHD const&) = delete;
  DAPHNEReaderPDHD(DAPHNEReaderPDHD&&) = delete;
  DAPHNEReaderPDHD& operator=(DAPHNEReaderPDHD const&) = delete;
  DAPHNEReaderPDHD& operator=(DAPHNEReaderPDHD&&) = delete;

  // Required functions.
  void produce(art::Event& e) override;
  void beginJob() override;

private:

  art::ServiceHandle<dune::DAPHNEChannelMapService> fChannelMap;
  std::string fInputLabel, fOutputLabel, fFileInfoLabel, fSubDetString;

  bool CheckSourceIsDetector(const SourceID & id);

  template <class T> size_t GetNFrames(size_t frag_size, size_t frag_header_size);

  void ProcessFrame(
      std::unique_ptr<Fragment> & frag, size_t frame_size, size_t i,
      std::unordered_map<unsigned int, WaveformVector> & wf_map);

  void ProcessStreamFrame(
      std::unique_ptr<Fragment> & frag, size_t frame_size, size_t i,
      std::unordered_map<unsigned int, WaveformVector> & wf_map);

  raw::OpDetWaveform & MakeWaveform(
      unsigned int offline_chan,
      size_t n_adcs,
      raw::TimeStamp_t timestamp,
      std::unordered_map<unsigned int, WaveformVector> & wf_map,
      bool is_stream=false);

  TTree * fTree;
  size_t b_slot, b_crate, b_link;
  bool b_is_stream;
  size_t b_channel_0, b_channel_1, b_channel_2, b_channel_3; 
};
}

void pdhd::DAPHNEReaderPDHD::beginJob() {
  art::ServiceHandle<art::TFileService> tfs;
  fTree = tfs->make<TTree>("beamana","beam analysis tree");

  fTree->Branch("slot", &b_slot);
  fTree->Branch("crate", &b_crate);
  fTree->Branch("link", &b_link);
  fTree->Branch("is_stream", &b_is_stream);
  fTree->Branch("channel_0", &b_channel_0);
  fTree->Branch("channel_1", &b_channel_1);
  fTree->Branch("channel_2", &b_channel_2);
  fTree->Branch("channel_3", &b_channel_3);

}

pdhd::DAPHNEReaderPDHD::DAPHNEReaderPDHD(fhicl::ParameterSet const& p)
  : EDProducer{p}, 
    fInputLabel(p.get<std::string>("InputLabel", "daq")),
    fOutputLabel(p.get<std::string>("OutputLabel", "daq")),
    fFileInfoLabel(p.get<std::string>("FileInfoLabel", "daq")),
    fSubDetString(p.get<std::string>("SubDetString","HD_PDS"))
{
  produces<std::vector<raw::OpDetWaveform>> (fOutputLabel);
  produces<std::vector<recob::OpHit>> (fOutputLabel);
}


bool pdhd::DAPHNEReaderPDHD::CheckSourceIsDetector(const SourceID & id) {
  return (id.subsystem == SourceID::Subsystem::kDetectorReadout);
}

template<typename T>
size_t pdhd::DAPHNEReaderPDHD::GetNFrames(size_t frag_size, size_t frag_header_size) {
  return (frag_size - frag_header_size)/sizeof(T);
}


void pdhd::DAPHNEReaderPDHD::ProcessFrame(
    std::unique_ptr<Fragment> & frag,
    size_t frame_size,
    size_t frame_number,
    std::unordered_map<unsigned int, WaveformVector> & wf_map) {
  auto frame
      = reinterpret_cast<DAPHNEFrame*>(
          static_cast<uint8_t*>(frag->get_data()) + frame_number*frame_size);
  b_is_stream = false;
  b_channel_1 = 0;
  b_channel_2 = 0;
  b_channel_3 = 0;
  b_channel_0 = frame->get_channel();
  b_link = frame->daq_header.link_id;
  b_slot = frame->daq_header.slot_id;

  auto offline_channel = fChannelMap->GetOfflineChannel(
      b_slot, b_link, b_channel_0);

  auto & waveform = MakeWaveform(
      offline_channel,
      static_cast<size_t>(frame->s_num_adcs),
      frame->get_timestamp(),
      wf_map);

  fTree->Fill();
  for (size_t j = 0; j < static_cast<size_t>(frame->s_num_adcs); ++j) {
    //std::cout << "\t" << frame->get_adc(j) << std::endl;
    waveform.push_back(frame->get_adc(j));
  }
}


raw::OpDetWaveform & pdhd::DAPHNEReaderPDHD::MakeWaveform(
    unsigned int offline_chan,
    size_t n_adcs,
    raw::TimeStamp_t timestamp,
    std::unordered_map<unsigned int, WaveformVector> & wf_map,
    bool is_stream) {

  //If needed, make a new element in the map
  if (wf_map.find(offline_chan) == wf_map.end()) {
    wf_map.emplace(offline_chan, WaveformVector());
  }

  //If is_stream, we just want to change the waveform
  //If not, or if this is the first time hitting this channel,
  //add a new waveform to the vector
  if (wf_map.at(offline_chan).size() == 0 || !is_stream) {
    wf_map.at(offline_chan).emplace_back(
        raw::OpDetWaveform(timestamp, offline_chan));
  }

  //std::cout << offline_chan << " " << std::setprecision(20) << timestamp << std::endl;

  auto & waveform = wf_map.at(offline_chan).back();

  //Reserve more adcs at once for efficiency
  waveform.reserve(waveform.size() + n_adcs);
  return waveform;
}

void pdhd::DAPHNEReaderPDHD::ProcessStreamFrame(
    std::unique_ptr<Fragment> & frag,
    size_t frame_size,
    size_t frame_number,
    std::unordered_map<unsigned int, WaveformVector> & wf_map) {
  auto frame
      = reinterpret_cast<DAPHNEStreamFrame*>(
          static_cast<uint8_t*>(frag->get_data()) + frame_number*frame_size);
  b_link = frame->daq_header.link_id;
  b_slot = frame->daq_header.slot_id;
  b_channel_0 = frame->header.channel_0;
  b_channel_1 = frame->header.channel_1;
  b_channel_2 = frame->header.channel_2;
  b_channel_3 = frame->header.channel_3;
  fTree->Fill();


  std::array<size_t, 4> frame_channels = {
    frame->header.channel_0,
    frame->header.channel_1,
    frame->header.channel_2,
    frame->header.channel_3};
  std::cout << "Processing stream frame " << frame_number << std::endl;
  // Loop over channels
  for (size_t i = 0; i < frame->s_channels_per_frame; ++i) {
    auto offline_channel = fChannelMap->GetOfflineChannel(
        b_slot, b_link, frame_channels[i]);
    std::cout << b_slot << " " << b_link << " " << frame_channels[i] << " " <<
                 offline_channel << std::endl;

    auto & waveform = MakeWaveform(
        offline_channel,
        frame->s_adcs_per_channel,
        frame->get_timestamp(),
        wf_map,
        true);

    // Loop over ADC values in the frame for channel i 
    std::cout << "\tChannel " << i << std::endl;
    for (size_t j = 0; j < static_cast<size_t>(frame->s_adcs_per_channel); ++j) {
      //std::cout << "\t" << frame->get_adc(j) << std::endl;
      waveform.push_back(frame->get_adc(j, i));
    }
  }
  std::cout << std::endl;
}

void pdhd::DAPHNEReaderPDHD::produce(art::Event& evt) {
  using dunedaq::daqdataformats::FragmentHeader;

  std::vector<raw::OpDetWaveform> opdet_waveforms;
  std::unordered_map<unsigned int, WaveformVector> wf_map;
  std::vector<recob::OpHit> optical_hits;

  //Get the HDF5 file to be opened
  auto infoHandle = evt.getHandle<raw::DUNEHDF5FileInfo2>(fFileInfoLabel);
  const std::string & file_name = infoHandle->GetFileName();
  uint32_t runno = infoHandle->GetRun();
  size_t   evtno = infoHandle->GetEvent();
  size_t   seqno = infoHandle->GetSequence();

  dunedaq::hdf5libs::HDF5RawDataFile::record_id_t record_id
      = std::make_pair(evtno, seqno);

  std::cout << file_name << " " << runno << " " << evtno << " " << seqno <<
               std::endl;

  //Open the HDF5 file and get source ids
  art::ServiceHandle<dune::HDF5RawFile2Service> rawFileService;
  auto raw_file = rawFileService->GetPtr();
  auto source_ids = raw_file->get_source_ids(record_id);

  //Loop over source ids
  for (const auto & source_id : source_ids)  {
    // only want detector readout data (i.e. not trigger info)
    if (!CheckSourceIsDetector(source_id)) continue;

    //Loop over geo ids
    std::cout << "Source: " << source_id << std::endl;
    auto geo_ids = raw_file->get_geo_ids_for_source_id(record_id, source_id);
    for (const auto &geo_id : geo_ids) {
      //TODO -- Wrap This
      dunedaq::detdataformats::DetID::Subdetector det_idenum
          = static_cast<dunedaq::detdataformats::DetID::Subdetector>(
              0xffff & geo_id);

      //Check that it's photon detectors
      auto subdetector_string
          = dunedaq::detdataformats::DetID::subdetector_to_string(det_idenum);
      if (subdetector_string != fSubDetString) continue;

      uint16_t crate_from_geo = 0xffff & (geo_id >> 16);
      std::cout << subdetector_string << " " << crate_from_geo << std::endl;
      b_crate = crate_from_geo;

      std::cout << "Getting fragment" << std::endl;
      auto frag = raw_file->get_frag_ptr(record_id, source_id);
      auto frag_size = frag->get_size();
      size_t frag_header_size = sizeof(FragmentHeader); // make this a const class member

      // Too small to even have a header
      if (frag_size <= frag_header_size) continue;

      std::cout << frag->get_header() << std::endl;

      //Checking which type of DAPHNE Frame to use
      auto frag_type = frag->get_fragment_type();
      /*if ((frag_type != FragmentType::kDAPHNE) &&
          (frag_type != FragmentType::kDAPHNEStream))
        throw cet::exception("DAPHNEReaderPDHD")
          << "Found bad fragment type " <<
             dunedaq::daqdataformats::fragment_type_to_string(frag_type);*/
      const bool use_stream_frame = (frag_type != FragmentType::kDAPHNE);
                                           
      size_t n_frames
          = (use_stream_frame ?
             GetNFrames<DAPHNEStreamFrame>(frag_size, frag_header_size) :
             GetNFrames<DAPHNEFrame>(frag_size, frag_header_size));
      auto frame_size = (use_stream_frame ?
                         sizeof(DAPHNEStreamFrame) : sizeof(DAPHNEFrame));
      std::cout << "NFrames: " << n_frames << " Headder TS: " <<
                   frag->get_header().trigger_timestamp << std::endl;
      for (size_t i = 0; i < n_frames; ++i) {
        if (use_stream_frame) {
          ProcessStreamFrame(frag, frame_size, i, wf_map);
        }
        else {
          ProcessFrame(frag, frame_size, i, wf_map);
        }
      }
    }
  }

  //Convert map to vector for output
  for (auto & chan_wf_vector : wf_map) {//Loop over channels
    std::cout << "Inserting " << chan_wf_vector.first << " " << chan_wf_vector.second.size() << std::endl;
    opdet_waveforms.insert(opdet_waveforms.end(),
                           chan_wf_vector.second.begin(),
                           chan_wf_vector.second.end());
    //Remove elements from wf_map to save memory
    chan_wf_vector.second.clear();
  }

  evt.put(
      std::make_unique<decltype(opdet_waveforms)>(std::move(opdet_waveforms)),
      fOutputLabel
  );

  evt.put(
      std::make_unique<decltype(optical_hits)>(std::move(optical_hits)),
      fOutputLabel
  );
}

DEFINE_ART_MODULE(pdhd::DAPHNEReaderPDHD)
