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



#include "detdataformats/daphne/DAPHNEFrame.hpp"
#include "detdataformats/daphne/DAPHNEStreamFrame.hpp"
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"
#include "dunecore/HDF5Utils/HDF5RawFile2Service.h"

//#include "PDHDReadoutUtils.h"

#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/RecoBase/OpHit.h"

#include <memory>
namespace pdhd {

using dunedaq::daqdataformats::SourceID;
using dunedaq::daqdataformats::Fragment;
using DAPHNEStreamFrame = dunedaq::fddetdataformats::DAPHNEStreamFrame;
using DAPHNEFrame = dunedaq::fddetdataformats::DAPHNEFrame;

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

private:

  std::string fInputLabel, fOutputLabel, fFileInfoLabel, fSubDetString;

  bool CheckSourceIsDetector(const SourceID & id);
  template <class T> size_t GetNFrames(size_t frag_size, size_t frag_header_size);
  void ProcessFrame(std::unique_ptr<Fragment> & frag, size_t frame_size, size_t i, std::vector<raw::OpDetWaveform> & opdet_waveforms);
  void ProcessStreamFrame(std::unique_ptr<Fragment> & frag, size_t frame_size, size_t i, std::vector<raw::OpDetWaveform> & opdet_waveforms);
};
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
    size_t i,
    std::vector<raw::OpDetWaveform> & opdet_waveforms) {
  auto frame
      = reinterpret_cast<DAPHNEFrame*>(
          static_cast<uint8_t*>(frag->get_data()) + i*frame_size);
  raw::OpDetWaveform waveform(frame->get_timestamp(), i, frame->s_num_adcs);
  for (size_t j = 0; j < static_cast<size_t>(frame->s_num_adcs); ++j) {
    //std::cout << "\t" << frame->get_adc(j) << std::endl;
    waveform.push_back(frame->get_adc(j));
  }
  opdet_waveforms.emplace_back(waveform);
}

void pdhd::DAPHNEReaderPDHD::ProcessStreamFrame(
    std::unique_ptr<Fragment> & frag,
    size_t frame_size,
    size_t i,
    std::vector<raw::OpDetWaveform> & opdet_waveforms) {
  auto frame
      = reinterpret_cast<DAPHNEStreamFrame*>(
          static_cast<uint8_t*>(frag->get_data()) + i*frame_size);
  //raw::OpDetWaveform waveform(frame->get_timestamp(), i, frame->s_num_adcs);
  //for (size_t j = 0; j < static_cast<size_t>(frame->s_num_adcs); ++j) {
    //std::cout << "\t" << frame->get_adc(j) << std::endl;
    //waveform.push_back(frame->get_adc(j));
  //}
  std::cout << frame->header.channel_0 << " " <<
               frame->header.channel_1 << " " <<
               frame->header.channel_2 << " " <<
               frame->header.channel_3 << std::endl;
  //opdet_waveforms.emplace_back(waveform);
}
void pdhd::DAPHNEReaderPDHD::produce(art::Event& evt) {
  using dunedaq::daqdataformats::FragmentHeader;

  std::vector<raw::OpDetWaveform> opdet_waveforms;
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

      std::cout << "Getting fragment" << std::endl;
      auto frag = raw_file->get_frag_ptr(record_id, source_id);
      auto frag_size = frag->get_size();
      size_t frag_header_size = sizeof(FragmentHeader); // make this a const class member

      // Too small to even have a header
      if (frag_size <= frag_header_size) continue;


      //Checking which type of DAPHNE Frame to use
      //const bool use_stream_frame = (crate_from_geo == 1);
      if (crate_from_geo != 2 && crate_from_geo != 1) 
        throw cet::exception("DAPHNEReaderPDHD")
          << "Found bad crate number: " << crate_from_geo;
          
                                           
      //size_t n_frames = (frag_size - frag_header_size)/sizeof(DAPHNEFrame);
      size_t n_frames
          = (crate_from_geo == 2 ?
             GetNFrames<DAPHNEFrame>(frag_size, frag_header_size) :
             GetNFrames<DAPHNEStreamFrame>(frag_size, frag_header_size));
      auto frame_size = (crate_from_geo == 2 ?
                         sizeof(DAPHNEFrame) : sizeof(DAPHNEStreamFrame));
      std::cout << "NFrames: " << n_frames << " Headder TS: " <<
                   frag->get_header().trigger_timestamp << std::endl;
      for (size_t i = 0; i < n_frames; ++i) {
        if (crate_from_geo == 2) {
          ProcessFrame(frag, frame_size, i, opdet_waveforms);
        }
        else {
          ProcessStreamFrame(frag, frame_size, i, opdet_waveforms);
        }

        //std::cout << i << " " <<
        //             frame->get_timestamp() << " " << frame->s_num_adcs <<
        //             //" " << frame->daq_header.slot_id <<
        //             std::endl;
        //std::cout << frame->daq_header << std::endl;
        //int channel = frame->get_channel();
        //std::cout  << "\tChannel: " << channel << " done" << std::endl;
        /*raw::OpDetWaveform waveform(frame->get_timestamp(), i, frame->s_num_adcs);
        for (size_t j = 0; j < static_cast<size_t>(frame->s_num_adcs); ++j) {
          //std::cout << "\t" << frame->get_adc(j) << std::endl;
          waveform.push_back(frame->get_adc(j));
        }
        opdet_waveforms.emplace_back(waveform);*/
      }
    }
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
