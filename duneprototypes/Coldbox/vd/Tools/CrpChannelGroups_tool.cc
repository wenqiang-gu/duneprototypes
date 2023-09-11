#include "CrpChannelGroups.h"
#include "CrpChannelHelper.h"
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
using Index = IndexRange::Index;

//**********************************************************************

CrpChannelGroups::CrpChannelGroups(fhicl::ParameterSet const& ps)
: m_LogLevel(ps.get<int>("LogLevel")) {
  const string myname = "CrpChannelGroups::ctor: ";
  if ( m_LogLevel ) cout << myname << "    LogLevel: " << m_LogLevel << endl;
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
    IndexRange ran = m_pcrt->get("crdet");
    if ( ! ran.isValid() ) {
      cout << myname << "ERROR: Channel range tool is not CRP." << endl;
      m_pcrt = nullptr;
      return;
    }
    Name sdet = ran.label();
    if ( m_LogLevel ) cout << myname << " Range detector: " << sdet << endl;
    CrpChannelHelper cch(sdet);
    if ( ! cch.isValid() ) {
      cout << myname << "ERROR: Invalid range detector name: " << sdet << endl;
      return;
    }
    if ( cch.usefembs ) {
      for ( Index icru=0; icru<cch.ncru; ++icru ) {
        if ( ! cch.cruHasFembs(icru) ) break;
        for ( Index ifmb=1; ifmb<=cch.nfc; ++ifmb ) {
          NameVector rnams;
          for ( Index ipla=0; ipla<3; ++ipla ) {
            string rnam = cch.fembPlaneName(icru, ifmb, ipla);
            rnams.push_back(rnam);
          }
          string gnam = cch.fembName(icru, ifmb);
          string glab = cch.fembLabel(icru, ifmb);
          IndexRangeGroup grp = makeGroup(gnam, rnams, glab, true);
        }
      }  
    }
  }
}

//**********************************************************************

IndexRangeGroup CrpChannelGroups::get(std::string gnam) const {
  const string myname = "CrpChannelGroups::get: ";
  if ( m_pcrt == nullptr ) {
    cout << myname << "ERROR: Channel range tool was not found." << endl;
    return IndexRangeGroup();
  }
  // First try local groups.
  GroupMap::const_iterator igrp = m_grps.find(gnam);
  if ( igrp != m_grps.end() ) {
   return igrp->second;
  }
  // Next see if there is range with the name.
  IndexRange ran = m_pcrt->get(gnam);
  if ( ran.isValid() ) {
    return IndexRangeGroup(ran);
  }
  if ( m_LogLevel >= 2 ) cout << myname << "Group not found: " << gnam << endl;
  return IndexRangeGroup();
}

//**********************************************************************

IndexRangeGroup CrpChannelGroups::makeGroup(Name nam, NameVector rnams, Name lab, bool ignoreBadRanges) {
  string myname = "CrpChannelGroups::makeGroup: ";
  IndexRangeGroup::RangeVector rans;
  for ( Name rnam : rnams ) {
    IndexRange ran = m_pcrt->get(rnam);
    if ( ran.isValid() ) {
      rans.push_back(ran);
    } else {
      if ( ! ignoreBadRanges ) {
        cout << myname << "WARNING: Invalid range " << rnam << " requested for group " << nam << endl;
      }
    }
  }
  NameVector labs = {lab};
  IndexRangeGroup grp(nam, labs, rans);
  if ( m_LogLevel >= 2 ) {
    cout << myname << "Adding group " << grp << endl;
  }
  m_grps[nam] = grp;
  return grp;
}
  

//**********************************************************************

DEFINE_ART_CLASS_TOOL(CrpChannelGroups)
