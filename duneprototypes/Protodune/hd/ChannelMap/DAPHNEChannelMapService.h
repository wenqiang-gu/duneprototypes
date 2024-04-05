#ifndef DAPHNEChannelMapService_H
#define DAPHNEChannelMapService_H

#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Services/Registry/ServiceMacros.h"

#include "DAPHNEChannelMap.h"

namespace dune {
  class DAPHNEChannelMapService;
}

class dune::DAPHNEChannelMapService {

public:

  DAPHNEChannelMapService(fhicl::ParameterSet const& pset);
  DAPHNEChannelMapService(fhicl::ParameterSet const& pset, art::ActivityRegistry&);

  unsigned int GetOfflineChannel(
   unsigned int slot,
   unsigned int link,
   unsigned int daphne_channel);

private:

  dune::DAPHNEChannelMap fChannelMap;

};

DECLARE_ART_SERVICE(dune::DAPHNEChannelMapService, LEGACY)

#endif
