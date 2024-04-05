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

const Int_t kMaxFrames = 50;

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
  TTree * fWaveformTree;
  size_t b_slot, b_crate, b_link;
  bool b_is_stream;
  size_t b_channel_0, b_channel_1, b_channel_2, b_channel_3;

  bool fExportWaveformTree;
  //vars per event
  int _Run;
  int _SubRun;
  int _Event;
  int _TriggerNumber;
  long int _TimeStamp;
  long int _Window_end;
  long int _Window_begin;
  int _NFrames;
  int _Slot;
  int _Crate;
  int _DaphneChannel;
  int _OfflineChannel;
  long int _FrameTimestamp;
  short _adc_value[1024];
  int _TriggerSampleValue;
  int _Threshold;
  int _Baseline;
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

  if(fExportWaveformTree)
  {
    fWaveformTree = tfs->make<TTree>("WaveformTree","Waveforms Tree");
    fWaveformTree->Branch("Run"    , &_Run    , "Run/I"    );
    fWaveformTree->Branch("Event"     , &_Event     , "Event/I"     );
    fWaveformTree->Branch("TriggerNumber" , &_TriggerNumber , "TriggerNumber/I"    );
    fWaveformTree->Branch("TimeStamp" , &_TimeStamp , "TimeStamp/l"    );
    fWaveformTree->Branch("Window_begin" , &_Window_begin , "Window_begin/l"    );
    fWaveformTree->Branch("Window_end" , &_Window_end , "Window_end/l"    );

    fWaveformTree->Branch("Slot"      , &_Slot     , "Slot/I");
    fWaveformTree->Branch("Crate"      , &_Crate    , "Crate/I"     );
    fWaveformTree->Branch("DaphneChannel"  ,& _DaphneChannel     , "DaphneChannel/I"     );
    fWaveformTree->Branch("OfflineChannel" , &_OfflineChannel    , "OfflineChannel/I"    );
    fWaveformTree->Branch("FrameTimestamp" ,& _FrameTimestamp , "FrameTimestamp/l"    );
    fWaveformTree->Branch("adc_channel"    , _adc_value          , "adc_value[1024]/S");

    fWaveformTree->Branch("TriggerSampleValue"  , &_TriggerSampleValue, "TriggerSampleValue/I"     ); //only for self-trigger
    fWaveformTree->Branch("Threshold"  , &_Threshold  , "Threshold/I"     ); //only for self-trigger
    fWaveformTree->Branch("Baseline"  , &_Baseline   , "Baseline/I"     ); //only for self-trigger

  }

}

pdhd::DAPHNEReaderPDHD::DAPHNEReaderPDHD(fhicl::ParameterSet const& p)
  : EDProducer{p}, 
    fInputLabel(p.get<std::string>("InputLabel", "daq")),
    fOutputLabel(p.get<std::string>("OutputLabel", "daq")),
    fFileInfoLabel(p.get<std::string>("FileInfoLabel", "daq")),
    fSubDetString(p.get<std::string>("SubDetString","HD_PDS")),
    fExportWaveformTree(p.get<bool>("ExportWaveformTree",true))
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
//  std::cout << "Process Frame link, slot, channel : " << b_link << " "<< b_slot <<  " " << b_channel_0 << std::endl;
//std::cout << "NSAmples:  " << frame->s_num_adcs << std::endl;
  auto offline_channel=-1;
  try {
     offline_channel = fChannelMap->GetOfflineChannel(
        b_slot, b_link, b_channel_0);
  }
  catch (const std::range_error & err) {
    std::cout << "WARNING: Could not find offline channel for " <<
                 b_slot << " " << b_link << " " << b_channel_0 << std::endl;
  }
//       std::cout << frame->header.channel <<  " "<< frame->header.pds_reserved_1 <<  " "<< frame->header.trigger_sample_value 
//    <<"  " << frame->header.threshold <<" " << frame->header.baseline << std::endl;
//    word_t channel : 6, pds_reserved_1 : 10, trigger_sample_value : 16;
//    word_t threshold : 16, baseline : 16;

  _Slot=b_slot;
  _DaphneChannel=b_channel_0;
  _OfflineChannel=offline_channel;
  _FrameTimestamp=frame->get_timestamp();
  _TriggerSampleValue=frame->header.trigger_sample_value;
  _Threshold=frame->header.threshold;
  _Baseline=frame->header.baseline;

  auto & waveform = MakeWaveform(
      offline_channel,
      static_cast<size_t>(frame->s_num_adcs),
      frame->get_timestamp(),
      wf_map);
  for (size_t j = 0; j < static_cast<size_t>(frame->s_num_adcs); ++j) {
    waveform.push_back(frame->get_adc(j));
   _adc_value[j]=frame->get_adc(j);
  }
  if(fExportWaveformTree) fWaveformTree->Fill();

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


  auto & waveform = wf_map.at(offline_chan).back();
  //Reserve more adcs at once for efficiency
//  waveform.reserve(waveform.size() + n_adcs);
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
//  std::cout << "Processing stream frame " << frame_number << " containing " <<frame->s_channels_per_frame << " channels." << std::endl;
  // Loop over channels
  for (size_t i = 0; i < frame->s_channels_per_frame; ++i) {
//    std::cout << "Processing slot, link, channel: " << b_slot << " " << b_link << " " << frame_channels[i] << ", NSamples : "<< frame->s_adcs_per_channel <<std::endl;
    auto offline_channel = -1;

    try {
      offline_channel = fChannelMap->GetOfflineChannel(
        b_slot, b_link, frame_channels[i]);
    }
    catch (const std::range_error & err) {
      std::cout << "WARNING: Could not find offline channel for " <<
                   b_slot << " " << b_link << " " << b_channel_0 << std::endl;
    }

    _Slot=b_slot;
    _DaphneChannel=frame_channels[i];
    _OfflineChannel=offline_channel;
    _FrameTimestamp=frame->get_timestamp();
    _TriggerSampleValue=0;
    _Threshold=0;
    _Baseline=0;


    auto & waveform = MakeWaveform(
          offline_channel,
          frame->s_adcs_per_channel,
          frame->get_timestamp(),
          wf_map,
          true);

    // Loop over ADC values in the frame for channel i 
    //std::cout << "\tChannel " << i << std::endl;
    for (size_t j = 0; j < static_cast<size_t>(frame->s_adcs_per_channel); ++j) {
      //std::cout << "\t" << frame->get_adc(j) << std::endl;
      waveform.push_back(frame->get_adc(j, i));
      _adc_value[j]=frame->get_adc(j, i);
//      _adc_valueV[_NFrames][j]=frame->get_adc(j, i);
    }

    if(fExportWaveformTree) fWaveformTree->Fill();

  }
  //std::cout << std::endl;
}

void pdhd::DAPHNEReaderPDHD::produce(art::Event& evt) {

//std::cout << "RUNNIN NEW EVENT ==================" << std::endl;
  using dunedaq::daqdataformats::FragmentHeader;

  std::vector<raw::OpDetWaveform> opdet_waveforms;
  std::unordered_map<unsigned int, WaveformVector> wf_map;
  std::vector<recob::OpHit> optical_hits;

  //Get the HDF5 file to be opened
  auto infoHandle = evt.getHandle<raw::DUNEHDF5FileInfo2>(fFileInfoLabel);
  size_t   evtno = infoHandle->GetEvent();
  size_t   seqno = infoHandle->GetSequence();

  _Run=infoHandle->GetRun();
  _Event=infoHandle->GetEvent();
  _TriggerNumber=0;
  _TimeStamp=0;
  _NFrames=0;

  dunedaq::hdf5libs::HDF5RawDataFile::record_id_t record_id
      = std::make_pair(evtno, seqno);

  //uint32_t runno = infoHandle->GetRun();
  //const std::string & file_name = infoHandle->GetFileName();
  //std::cout << file_name << " " << runno << " " << evtno << " " << seqno <<
  //           std::endl;

  //Open the HDF5 file and get source ids
  art::ServiceHandle<dune::HDF5RawFile2Service> rawFileService;
  auto raw_file = rawFileService->GetPtr();
  auto source_ids = raw_file->get_source_ids(record_id);

  //Loop over source ids
  for (const auto & source_id : source_ids)  {
    // only want detector readout data (i.e. not trigger info)
    if (!CheckSourceIsDetector(source_id)) continue;

    //Loop over geo ids
    //std::cout << "Source: " << source_id << std::endl;
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
//      std::cout << "Substring: " << subdetector_string << " crate:" << crate_from_geo << std::endl;
      b_crate = crate_from_geo;
      _Crate=b_crate;

      auto frag = raw_file->get_frag_ptr(record_id, source_id);
      auto frag_size = frag->get_size();
      size_t frag_header_size = sizeof(FragmentHeader); // make this a const class member
//      std::cout << "Getting fragment, FRAG SIZE" << frag_size << std::endl;

      // Too small to even have a header
      if (frag_size <= frag_header_size) continue;

/*
Daphe Fragment header format:
Frag Header: check_word: 11112222, version: 5, size: 21864, trigger_number: 2522, run_number: 24496, trigger_timestamp: 106886658719868713, window_begin: 106886658719866569, window_end: 106886658720128713, error_bits: 0, fragment_type: 3, sequence_number: 0, detector_id: 2, element_id: subsystem: Detector_Readout id: 14
*/
      _TriggerNumber=frag->get_header().trigger_number;
//      _Fragment=frag->get_header().fragment_type;
      _TimeStamp=frag->get_header().trigger_timestamp;
     _Window_begin=frag->get_header().window_begin;
     _Window_end=frag->get_header().window_end;


//      std::cout << "Frag Header: " <<frag->get_header() << std::endl;

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
//      std::cout << "NFrames: " << n_frames << " Headder TS: " <<
//                 frag->get_header().trigger_timestamp << std::endl;
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
    //std::cout << "Inserting " << chan_wf_vector.first << " " << chan_wf_vector.second.size() << std::endl;
    opdet_waveforms.insert(opdet_waveforms.end(),
                           chan_wf_vector.second.begin(),
                           chan_wf_vector.second.end());
    //Remove elements from wf_map to save memory
    chan_wf_vector.second.clear();
  }
  std::cout << "EventNumber " << _Event << std::endl;
//  if(static_cast<std::vector<int>::size_type>(_NFrames)!=_OfflineChannelV.size()) std::cout << "WARNING ERROR" << std::endl;
  
//  if(fExportWaveformTree) fWaveformTree->Fill();

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
