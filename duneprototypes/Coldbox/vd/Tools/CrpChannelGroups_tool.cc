#include "CrpChannelGroups.h"
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

//**********************************************************************

CrpChannelGroups::CrpChannelGroups(fhicl::ParameterSet const& ps)
: m_LogLevel(ps.get<int>("LogLevel")) {
  const string myname = "CrpChannelGroups::ctor: ";
  // Fetch the IndexRange tool.
  DuneToolManager* ptm = DuneToolManager::instance();
  if ( ptm == nullptr ) {
    if ( m_LogLevel >= 1 ) cout << myname << "ERROR: Tool manager not found." << endl;
  } else {
    string crtName = "channelRanges";
    m_pcrt = ptm->getShared<IndexRangeTool>(crtName);
    if ( m_pcrt == nullptr ) {
      if ( m_LogLevel >= 1 ) cout << myname << "ERROR: Channel range tool not found: " << crtName << endl;
    }
  }
  cout << myname << "    LogLevel: " << m_LogLevel << endl;
}

//**********************************************************************

IndexRangeGroup CrpChannelGroups::get(std::string gnam) const {
  const string myname = "CrpChannelGroups::get: ";
  if ( m_pcrt == nullptr ) {
    cout << myname << "ERROR: Channel range tool was not found." << endl;
    return IndexRangeGroup();
  }
  // No groups yet defined. Try range instead.
  IndexRange ran = m_pcrt->get(gnam);
  if ( ! ran.isValid() ) {
    if ( m_LogLevel >= 2 ) cout << myname << "Range not found: " << gnam << endl;
    return IndexRangeGroup();
  }
  return IndexRangeGroup(ran);
}

//**********************************************************************

IndexRangeGroup CrpChannelGroups::
makeGroup(Name nam, NameVector rnams, Name lab) const {
  IndexRangeGroup::RangeVector rans;
  for ( Name rnam : rnams ) {
    rans.push_back(m_pcrt->get(rnam));
  }
  NameVector labs = {lab};
  return IndexRangeGroup(nam, labs, rans);
}
  

//**********************************************************************

DEFINE_ART_CLASS_TOOL(CrpChannelGroups)
