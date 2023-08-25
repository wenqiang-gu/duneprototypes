// CrpChannelRanges_tool.cc 

#include "CrpChannelRanges.h"
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneCommon/Utility/StringManipulator.h"
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
  const Index nsu = 952;
  const Index nsv = 952;
  const Index nsz = 1168;
  IndexVector nPlaneStrips = {nsu, nsv, nsz};
  const Index nsc = nsu + nsv + nsz;
  Index ncru = 0;
  Index npla = 3;
  NameVector plaLabs = {"u", "v", "z"};
  NameVector cruLabs;
  // Split detector name and options.
  NameVector vals = StringManipulator(m_Detector).split(":");
  string detname = vals[0];
  // Build the labels and set default fo usefembs.
  bool usefembs = false;
  if ( detname == "cb2022" ) {
    ncru = 1;
    cruLabs.push_back("C");
  } else if ( detname == "pdvd" ) {
    ncru = 4;
    cruLabs.push_back("A");
    cruLabs.push_back("B");
    cruLabs.push_back("A");
    cruLabs.push_back("B");
    usefembs = true;
  } else {
    cout << myname << "ERROR: Invalid detector name: " << detname << endl;
  }
  Index nsdet = nsc*ncru;
  // Override usefembs.
  for ( Index ival=1; ival<vals.size(); ++ival ) {
    Name val = vals[ival];
    if      ( val == "fembs" )   usefembs = true;
    else if ( val == "nofembs" ) usefembs = false;
    else {
      cout << myname << "WARNING: Ignoring invalid detector option " << val << endl;
    }
  }
  // Fetch the channel-FEMB mapping tool, if needed.
  const IndexMapTool* pcrpChannelFemb = nullptr;
  if ( usefembs ) {
    Name fctname = "crpChannelFemb";
    pcrpChannelFemb = DuneToolManager::instance()->getShared<IndexMapTool>(fctname);
    if ( pcrpChannelFemb == nullptr ) {
      cout << myname << "WARNING: Tool " << fctname
           << " not found. FEMB-view ranges will not be defined." << endl;
      usefembs = false;
    } else {
      cout << myname << "Found FEMB-channel mapping tool " << fctname << endl;
    }
  }
  // Build the channel ranges.
  assert( ncru > 0 );
  insert("all", 0, nsdet, "CRDET");
  insert("crdet", 0, nsdet, "CRDET");
  if ( ncru > 1 ) assert( ncru/2 == (ncru+1)/2 );
  assert ( cruLabs.size() == ncru );
  // Loop over CRUs
  Index its = 0;    // First strip in this CRU
  for ( Index icru=0; icru<ncru; ++icru ) {
    string crulab = cruLabs[icru];
    string scr = "cru";
    if ( ncru > 1 ) scr = icru < ncru/2 ? "crb" : "crt";
    string uscr = toupper(scr);
    // Detector ends: crt, crb
    if ( ncru > 1 && (icru == 0 || icru == ncru/2) ) {
      insert(scr, its, its+nsdet/2, uscr);
    }
    uscr += "-";
    Index jts = its + nsc;
    // CRUS: crtA, crtB, crbA, crbB
    insert(scr + crulab, its, jts, uscr + crulab);
    // CRU planes: crtAu, crtAv, ...
    Index ips = its;
    for ( Index ipla=0; ipla<npla; ++ipla ) {
      Index nps = nPlaneStrips[ipla];
      Index jps = ips + nps;
      string plalab = plaLabs[ipla];
      string uplalab = toupper(plalab);
      insert(scr + crulab + plalab, ips, jps, uscr + crulab + uplalab);
      // FEMB views.
      if ( usefembs && scr != "crt") {
        Index icha = ips;
        Index ifmb = pcrpChannelFemb->get(icha%nsc);
        for ( Index jcha=icha+1; jcha<=jps; ++jcha ) {
          Index jfmb = 999999;
          if ( jcha < jps ) jfmb = pcrpChannelFemb->get(jcha%nsc);
          if ( jfmb == ifmb ) continue;
          string fmblab = std::to_string(ifmb);
          while ( fmblab.size() < 2 ) fmblab = "0" + fmblab;
          string rnam = "femb" + crulab + fmblab + plalab;
          string rlab = "FEMB" + crulab + fmblab + uplalab;
          insert(rnam, icha, jcha, rlab);
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
