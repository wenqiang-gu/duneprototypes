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
#include "duneprototypes/Protodune/hd/ChannelMap/DAPHNEChannelMapService.h"
#include "art/Utilities/make_tool.h" 

#include "DAPHNEInterfaceBase.h"
#include "DAPHNEUtils.h"

#include "lardataobj/RawData/OpDetWaveform.h"
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
  void produce(art::Event& e) override;
  //void beginJob() override;
  //void endJob() override;

private:
  std::string fOutputLabel;
};
}

//void pdhd::PDHDTimingRawDecoder::beginJob() {
//}

pdhd::PDHDTimingRawDecoder::PDHDTimingRawDecoder(fhicl::ParameterSet const& p)
  : EDProducer{p},
    fOutputLabel(p.get<std::string>("OutputLabel")) {
  //
  //TODO-- replace with the correct class
  //produces<std::vector<raw::OpDetWaveform>> (fOutputLabel);
}

//Actually place functionality here
void pdhd::PDHDTimingRawDecoder::produce(art::Event& evt) {

  //Change this to w/e class
  evt.put(
      std::make_unique<decltype(opdet_waveforms)>(std::move(opdet_waveforms)),
      fOutputLabel
  );

}

//void pdhd::PDHDTimingRawDecoder::endJob() {
//}

DEFINE_ART_MODULE(pdhd::PDHDTimingRawDecoder)
