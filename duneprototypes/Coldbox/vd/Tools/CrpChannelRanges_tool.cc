// CrpChannelRanges_tool.cc 

#include "CrpChannelRanges.h"
#include "CrpChannelHelper.h"
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexMapTool.h"

using std::string;
using std::cout;
using std::endl;

using Name = CrpChannelRanges::Name;
using Index = CrpChannelRanges::Index;
using NameVector = std::vector<Name>;
using IndexVector = std::vector<Index>;

//**********************************************************************

namespace {
  string toupper(std::string sin) {
    string sout = sin;
    for ( char& ch : sout ) ch = std::toupper(ch);
    return sout;
  }
}

//**********************************************************************

CrpChannelRanges::CrpChannelRanges(fhicl::ParameterSet const& ps)
: m_LogLevel(ps.get<int>("LogLevel")),
  m_Detector(ps.get<Name>("Detector")) {
  const string myname = "CrpChannelRanges::ctor: ";
  if ( m_LogLevel >= 1 ) {
    cout << myname << "     LogLevel: " << m_LogLevel << endl;
    cout << myname << "     Detector: " << m_Detector << endl;
  }
  CrpChannelHelper cch(m_Detector);
  if ( ! cch.isValid() ) {
    cout << myname << "ERROR: Invalid range detector name: " << m_Detector << endl;
    return;
  }
  // Fetch the channel-FEMB mapping tool, if needed.
  const IndexMapTool* pcrpChannelFemb = nullptr;
  if ( cch.usefembs ) {
    Name fctname = "crpChannelFemb";
    pcrpChannelFemb = DuneToolManager::instance()->getShared<IndexMapTool>(fctname);
    if ( pcrpChannelFemb == nullptr ) {
      cout << myname << "WARNING: Tool " << fctname
           << " not found. FEMB-view ranges will not be defined." << endl;
      cch.usefembs = false;
    } else {
      if ( m_LogLevel ) cout << myname << "Found FEMB-channel mapping tool " << fctname << endl;
    }
  }
  // Build the channel ranges.
  assert( cch.ncru > 0 );
  insert("all", 0, cch.nsdet, m_Detector);
  insert("crdet", 0, cch.nsdet, m_Detector);
  if ( cch.ncru > 1 ) assert( cch.ncru/2 == (cch.ncru+1)/2 );
  assert ( cch.cruLabs.size() == cch.ncru );
  // Add each end.
  Index nend = cch.ncru == 1 ? cch.nsdet : cch.nsdet/2;
  Name enam = cch.cruEndName(0);
  insert(enam, 0, nend, toupper(enam));
  if ( nend < cch.nsdet ) {
    enam = cch.cruEndName(cch.ncru/2);
    insert(enam, nend, cch.nsdet, toupper(enam));
  }
  // Loop over CRUs
  Index its = 0;    // First strip in this CRU
  for ( Index icru=0; icru<cch.ncru; ++icru ) {
    Name crunam = cch.cruName(icru);
    Name crulab = cch.cruLabel(icru);
    // CRUS: crtA, crtB, crbA, crbB
    Index jts = its + cch.nsc;
    insert(crunam, its, jts, crulab);
    // CRU planes: crtAu, crtAv, ...
    Index ips = its;
    for ( Index ipla=0; ipla<cch.npla; ++ipla ) {
      Index nps = cch.nPlaneStrips[ipla];
      Index jps = ips + nps;
      insert(cch.cruPlaneName(icru, ipla), ips, jps, cch.cruPlaneLabel(icru, ipla));
      // FEMB views.
      if ( cch.cruHasFembs(icru) ) {
        Index icha = ips;
        Index ifmb = pcrpChannelFemb->get(icha%cch.nsc);
        for ( Index jcha=icha+1; jcha<=jps; ++jcha ) {
          Index jfmb = 999999;
          if ( jcha < jps ) jfmb = pcrpChannelFemb->get(jcha%cch.nsc);
          if ( jfmb == ifmb ) continue;
          insert(cch.fembPlaneName(icru, ifmb, ipla), icha, jcha, cch.fembPlaneLabel(icru, ifmb, ipla));
          ifmb = jfmb;
          icha = jcha;
          if ( icha >= jps ) break;
        }
      }
      ips = jps;
    }
    its = jts;
  }
}

//**********************************************************************

IndexRange CrpChannelRanges::get(string sran) const {
  const string myname = "CrpChannelRanges::get: ";
  RangeMap::const_iterator iran = m_rans.find(sran);
  if ( iran  != m_rans.end() ) return iran->second;
  if ( m_LogLevel >= 3 ) {
    cout << myname << "Invalid channel range name: " << sran << endl;
  }
  return IndexRange(0, 0);
}

//**********************************************************************

void CrpChannelRanges::insert(Name sran, Index ich1, Index ich2, Name slab1) {
  const string myname = "CrpChannelRanges::insert: ";
  if ( sran.size() == 0 ) return;
  if ( m_rans.count(sran) ) {
    cout << myname << "ERROR: Ignaoring duplicate range " << sran
         << " at " << IndexRange(ich1, ich2)
         << " is already mapped to " << get(sran) << endl;
  } else {
    if ( m_LogLevel >= 2 ) {
      cout << myname << "Adding range " << sran << ": [" << ich1 << ", "
           << ich2 << "), " << ich2 - ich1 << " channels" << endl;
    }
    m_rans[sran] = IndexRange(sran, ich1, ich2, slab1);
  }
}

//**********************************************************************

DEFINE_ART_CLASS_TOOL(CrpChannelRanges)
