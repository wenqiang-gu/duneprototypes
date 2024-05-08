#ifndef DAPHNEInterfaceBase_h
#define DAPHNEInterfaceBase_h
#include "lardataobj/RawData/OpDetWaveform.h"
#include "art/Utilities/ToolMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "duneprototypes/Protodune/hd/ChannelMap/DAPHNEChannelMapService.h"
namespace daphne {
using WaveformVector = std::vector<raw::OpDetWaveform>;
class DAPHNEInterfaceBase {
 public:
  virtual void Process(
      art::Event &evt,
      std::string inputlabel,
      std::string subdet_label,
      std::unordered_map<unsigned int, WaveformVector> & wf_map) = 0;

 protected:
  art::ServiceHandle<dune::DAPHNEChannelMapService> fChannelMap;
};
}

#endif
