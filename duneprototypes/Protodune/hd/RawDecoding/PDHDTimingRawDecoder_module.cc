////////////////////////////////////////////////////////////////////////
// Class:       PDHDTimingRawDecoder
// Plugin Type: producer (Unknown Unknown)
// File:        PDHDTimingRawDecoder_module.cc
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
#include "art/Utilities/make_tool.h"

#include "TTree.h"
#include "art_root_io/TFileService.h"

#include <memory>
namespace pdhd {

class PDHDTimingRawDecoder;

class PDHDTimingRawDecoder : public art::EDProducer {
public:
  explicit PDHDTimingRawDecoder(fhicl::ParameterSet const& p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  PDHDTimingRawDecoder(PDHDTimingRawDecoder const&) = delete;
  PDHDTimingRawDecoder(PDHDTimingRawDecoder&&) = delete;
  PDHDTimingRawDecoder& operator=(PDHDTimingRawDecoder const&) = delete;
  PDHDTimingRawDecoder& operator=(PDHDTimingRawDecoder&&) = delete;

  // Required functions.
  void produce(art::Event& event) override;

private:
  std::string fOutputLabel;
};
}

pdhd::PDHDTimingRawDecoder::PDHDTimingRawDecoder(fhicl::ParameterSet const& p)
  : EDProducer{p},
    fOutputLabel(p.get<std::string>("OutputLabel")) {
  produces<std::vector<uint64_t>> (fOutputLabel);
}

void pdhd::PDHDTimingRawDecoder::produce(art::Event& event) {
  // Get the HDF5 file.
  art::ServiceHandle<dune::HDF5RawFile3Service> rawFileService;
  auto raw_file = rawFileService->GetPtr();
  std::vector<std::string> record_header_paths = raw_file->get_trigger_record_header_dataset_paths();

  // Run through each trigger record header and get the timestamp.
  std::vector<uint64_t> timestamps;
  for (std::string& header_path : record_header_paths) {
      auto trh_ptr = raw_file->get_trh_ptr(header_path);
      timestamps.push_back(trh_ptr->get_trigger_timestamp());
  }

  event.put(std::make_unique<std::vector<uint64_t>>(std::move(timestamps)), fOutputLabel);
}

DEFINE_ART_MODULE(pdhd::PDHDTimingRawDecoder)
