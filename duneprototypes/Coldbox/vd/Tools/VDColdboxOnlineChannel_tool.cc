// VDColdboxOnlineChannel.cxx

#include "VDColdboxOnlineChannel.h"
#include "dune/Coldbox/vd/ChannelMap/VDColdboxChannelMapService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using Index = VDColdboxOnlineChannel::Index;

//**********************************************************************

VDColdboxOnlineChannel::VDColdboxOnlineChannel(const fhicl::ParameterSet& pset) 
: m_LogLevel(pset.get<Index>("LogLevel")) {
  const string myname = "VDColdboxOnlineChannel::ctor: ";
  cout << myname << "  LogLevel: " << m_LogLevel << endl;
}
  
//**********************************************************************

Index VDColdboxOnlineChannel::get(Index chanOff) const {
  const string myname = "VDColdboxOnlineChannel::get: ";
  if ( chanOff < 1600 || chanOff >= 3392 ) {
    if ( m_LogLevel > 1 ) cout << myname << "Invalid offline channel: " << chanOff << endl;
    return badIndex();
  }
  art::ServiceHandle<dune::VDColdboxChannelMapService> pms;
  dune::VDColdboxChannelMapService::VDCBChanInfo info = pms->getChanInfoFromOfflChan(chanOff);
  return 128*(info.femb) + 16*(info.asic-1) + info.asicchan;
}

//**********************************************************************

DEFINE_ART_CLASS_TOOL(VDColdboxOnlineChannel)
