#include "VDColdboxChannelGroups.h"
#include "dune/ArtSupport/DuneToolManager.h"
#include "dune/DuneInterface/Tool/IndexRangeTool.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

//**********************************************************************

VDColdboxChannelGroups::VDColdboxChannelGroups(fhicl::ParameterSet const& ps)
: m_LogLevel(ps.get<int>("LogLevel")) { }

//**********************************************************************

IndexRangeGroup VDColdboxChannelGroups::get(std::string gnam) const {
  const string myname = "VDColdboxChannelGroups::get: ";
  // No groups found. Try range instead.
  DuneToolManager* ptm = DuneToolManager::instance();
  if ( ptm == nullptr ) {
    if ( m_LogLevel >= 1 ) cout << myname << "ERROR: Tool manager not found." << endl;
    return IndexRangeGroup();
  }
  string crtName = "channelRanges";
  IndexRangeTool* prt = ptm->getShared<IndexRangeTool>(crtName);
  if ( prt == nullptr ) {
    if ( m_LogLevel >= 1 ) cout << myname << "ERROR: Channel range tool not found: " << crtName << endl;
    return IndexRangeGroup();
  }
  IndexRange ran = prt->get(gnam);
  if ( ! ran.isValid() ) {
    if ( m_LogLevel >= 2 ) cout << myname << "Range not found: " << gnam << endl;
    return IndexRangeGroup();
  }
  return IndexRangeGroup(ran);
}

//**********************************************************************

DEFINE_ART_CLASS_TOOL(VDColdboxChannelGroups)
