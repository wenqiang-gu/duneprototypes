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
#include "dunecore/DuneObj/DUNEHDF5FileInfo2.h"
#include "dunecore/HDF5Utils/HDF5RawFile2Service.h"

//#include "PDHDReadoutUtils.h"

#include "lardataobj/RawData/OpDetWaveform.h"
#include "lardataobj/RecoBase/OpHit.h"

#include <memory>
namespace pdhd {

using dunedaq::daqdataformats::SourceID;

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

void pdhd::DAPHNEReaderPDHD::produce(art::Event& evt) {
  using dunedaq::fddetdataformats::DAPHNEFrame;
  using dunedaq::daqdataformats::FragmentHeader;

  std::vector<raw::OpDetWaveform> opdet_waveforms;
  std::vector<recob::OpHit> optical_hits;

  auto infoHandle = evt.getHandle<raw::DUNEHDF5FileInfo2>(fFileInfoLabel);
  const std::string & file_name = infoHandle->GetFileName();
  uint32_t runno = infoHandle->GetRun();
  size_t   evtno = infoHandle->GetEvent();
  size_t   seqno = infoHandle->GetSequence();

  dunedaq::hdf5libs::HDF5RawDataFile::record_id_t record_id
      = std::make_pair(evtno, seqno);

  std::cout << file_name << " " << runno << " " << evtno << " " << seqno <<
               std::endl;

  art::ServiceHandle<dune::HDF5RawFile2Service> rawFileService;
  auto raw_file = rawFileService->GetPtr();
  auto source_ids = raw_file->get_source_ids(record_id);

  for (const auto & source_id : source_ids)  {
    // only want detector readout data (i.e. not trigger info)
    if (!CheckSourceIsDetector(source_id)) continue;
    std::cout << "Source: " << source_id << std::endl;
    auto geo_ids = raw_file->get_geo_ids_for_source_id(record_id, source_id);
    for (const auto &geo_id : geo_ids) {
      //TODO -- Wrap This
      dunedaq::detdataformats::DetID::Subdetector det_idenum
          = static_cast<dunedaq::detdataformats::DetID::Subdetector>(
              0xffff & geo_id);
      auto subdetector_string
          = dunedaq::detdataformats::DetID::subdetector_to_string(det_idenum);
      if (subdetector_string != fSubDetString) continue;

      uint16_t crate_from_geo = 0xffff & (geo_id >> 16);
      std::cout << subdetector_string << " " << crate_from_geo << std::endl;

      std::cout << "Getting fragment" << std::endl;
      auto frag = raw_file->get_frag_ptr(record_id, source_id);
      auto frag_size = frag->get_size();
      size_t frag_header_size = sizeof(FragmentHeader);

      // Too small to even have a header
      if (frag_size <= frag_header_size) continue;

      size_t n_frames = (frag_size - frag_header_size)/sizeof(DAPHNEFrame);
      std::cout << "NFrames: " << n_frames << std::endl;
      for (size_t i = 0; i < n_frames; ++i) {
        auto frame = reinterpret_cast<DAPHNEFrame*>(
            static_cast<uint8_t*>(frag->get_data()) + i*sizeof(DAPHNEFrame));

        std::cout << i << " " << frame->get_channel() << " " <<
                     frame->get_timestamp() << " " << frame->s_num_adcs <<
                     std::endl;
        raw::OpDetWaveform waveform(frame->get_timestamp(), i, frame->s_num_adcs);
        for (size_t j = 0; j < static_cast<size_t>(frame->s_num_adcs); ++j) {
          //std::cout << "\t" << frame->get_adc(j) << std::endl;
          waveform.push_back(frame->get_adc(j));
        }
        opdet_waveforms.emplace_back(waveform);
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
