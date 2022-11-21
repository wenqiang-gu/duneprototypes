#include "CrpChannelRanges.h"
#include "dunecore/ArtSupport/DuneToolManager.h"

using std::string;
using std::cout;
using std::endl;

using Name = CrpChannelRanges::Name;
using Index = CrpChannelRanges::Index;
using NameVector = std::vector<Name>;
using IndexVector = std::vector<Index>;

namespace {
  string toupper(std::string sin) {
    string sout = sin;
    for ( char& ch : sout ) ch = std::toupper(ch);
    return sout;
  }
}

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
  if ( m_Detector == "cb2022" ) {
    ncru = 1;
    cruLabs.push_back("C");
  } else if ( m_Detector == "pdvd2022" ) {
    ncru = 4;
    cruLabs.push_back("A");
    cruLabs.push_back("B");
    cruLabs.push_back("A");
    cruLabs.push_back("B");
  } else {
    cout << myname << "ERROR: Invalid detector name: " << m_Detector << endl;
  }
  Index nsdet = nsc*ncru;
  insert("crdet", 0, nsdet, "CRDET");
  assert( ncru > 0 );
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
      insert(scr + crulab + plalab, ips, jps, uscr + crulab + toupper(plalab));
      ips = jps;
    }
    its = jts;
  }
}


IndexRange CrpChannelRanges::get(string sran) const {
  const string myname = "CrpChannelRanges::get: ";
  RangeMap::const_iterator iran = m_rans.find(sran);
  if ( iran  != m_rans.end() ) return iran->second;
  if ( m_LogLevel >= 2 ) {
    cout << myname << "Invalid channel range name: " << sran << endl;
  }
  return IndexRange(0, 0);
}

void CrpChannelRanges::insert(Name sran, Index ich1, Index ich2, Name slab1) {
  if ( sran.size() == 0 ) return;
  m_rans[sran] = IndexRange(sran, ich1, ich2, slab1);
}

DEFINE_ART_CLASS_TOOL(CrpChannelRanges)
