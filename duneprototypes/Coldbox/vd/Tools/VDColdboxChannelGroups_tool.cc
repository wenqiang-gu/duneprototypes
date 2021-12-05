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
: m_LogLevel(ps.get<int>("LogLevel")) {
  const string myname = "VDColdboxChannelGroups::ctor: ";
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

IndexRangeGroup VDColdboxChannelGroups::get(std::string gnam) const {
  const string myname = "VDColdboxChannelGroups::get: ";
  if ( m_pcrt == nullptr ) {
    cout << myname << "ERROR: Channel range tool was not found." << endl;
    return IndexRangeGroup();
  }
  // Construct group if this is a known group name.
  const NameVector ufnams = {"femb01u", "femb02u", "femb03u", "femb04u", "femb05u", "femb06u",
                             "femb07u", "femb08u", "femb09u", "femb10u", "femb11u"};
  const NameVector yfnams = {"femb01y", "femb02y", "femb03y", "femb04y",
                             "femb11y", "femb12y", "femb13y"};
  const NameVector zfnams = {"femb04z", "femb05z", "femb06z", "femb07z",
                             "femb08z", "femb09z", "femb10z", "femb11z"};
  NameVector fnams = ufnams;
  fnams.insert(fnams.end(), yfnams.begin(), yfnams.end());
  fnams.insert(fnams.end(), zfnams.begin(), zfnams.end());
  if ( gnam == "fembviews" ) {
    return makeGroup(gnam, fnams, "FEMB views");
  } else if ( gnam == "fembu" ) {
    return makeGroup(gnam, ufnams, "U FEMBs");
  } else if ( gnam == "femby" ) {
    return makeGroup(gnam, yfnams, "Y FEMBs");
  } else if ( gnam == "fembz" ) {
    return makeGroup(gnam, zfnams, "Z FEMBs");
  }
  // No groups found. Try range instead.
  IndexRange ran = m_pcrt->get(gnam);
  if ( ! ran.isValid() ) {
    if ( m_LogLevel >= 2 ) cout << myname << "Range not found: " << gnam << endl;
    return IndexRangeGroup();
  }
  return IndexRangeGroup(ran);
}

//**********************************************************************

IndexRangeGroup VDColdboxChannelGroups::
makeGroup(Name nam, NameVector rnams, Name lab) const {
  IndexRangeGroup::RangeVector rans;
  for ( Name rnam : rnams ) {
    rans.push_back(m_pcrt->get(rnam));
  }
  NameVector labs = {lab};
  return IndexRangeGroup(nam, labs, rans);
}
  

//**********************************************************************

DEFINE_ART_CLASS_TOOL(VDColdboxChannelGroups)
