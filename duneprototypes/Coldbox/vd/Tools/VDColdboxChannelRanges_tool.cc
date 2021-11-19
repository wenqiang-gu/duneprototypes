#include "VDColdboxChannelRanges.h"
#include "dune/ArtSupport/DuneToolManager.h"

using std::string;
using std::cout;
using std::endl;

using Name = VDColdboxChannelRanges::Name;
using Index = VDColdboxChannelRanges::Index;

VDColdboxChannelRanges::VDColdboxChannelRanges(fhicl::ParameterSet const& ps)
: m_LogLevel(ps.get<int>("LogLevel")),
  m_GhostRange(ps.get<IndexVector>("GhostRange")),
  m_glo(0), m_ghi(0) {
  const string myname = "VDColdboxChannelRanges::ctor: ";
  const Index nu = 384;
  const Index ny = 640;
  const Index ntot = 3200;
  const Index nhaf = ntot/2;
  Index nfmb = 13;
  const IndexVector fuEdges = {1600, 1632, 1664, 1700, 1738, 1778, 1817, 1856, 1896, 1936, 1975, 1984, 1984, 1984};
  const IndexVector fyEdges = {1984, 2064, 2144, 2236, 2304,    0,    0,    0,    0,    0, 2624, 2560, 2432, 2304};
  const IndexVector fzEdges = {2624, 2624, 2624, 2624, 2646, 2734, 2823, 2912, 3000, 3088, 3177, 3200, 3200, 3200};
  if ( m_GhostRange.size() == 2 && m_GhostRange[1] >= m_GhostRange[0] ) {
    m_glo = m_GhostRange[0];
    m_ghi = m_GhostRange[1] + 1;
  } else if ( m_GhostRange.size() ) {
    cout << myname << "WARNING: " << "Ignoring invalid ghost range." << endl;
  }
  if ( m_LogLevel >= 1 ) {
    cout << myname << "     LogLevel: " << m_LogLevel << endl;
    cout << myname << "  Ghost range: [";
    if ( m_ghi > m_glo ) cout << m_glo << ", " << m_ghi-1;
    cout << "]" << endl;
  }
  // Build entries for detector, haves and planes.
  insert("cru",           0,       ntot, "CRU");
  insert("crt",           0,       nhaf, "CRT");
  insert("crb",        1600,       ntot, "CRB");
  insert("crtu",          0,         nu, "CRTu");
  insert("crty",         nu,      nu+ny, "CRTy");
  insert("crtz",      nu+ny,       nhaf, "CRTz");
  insert("crbu",       nhaf,    nhaf+nu, "CRBu");
  insert("crby",    nhaf+nu, nhaf+nu+ny, "CRBy");
  insert("crbz", nhaf+nu+ny,       ntot, "CRBz");
  insert("crbg",      m_glo,      m_ghi, "CRBghost");
  // Build the FEMB-view entries.
  for ( Index kfmb=0; kfmb<nfmb; ++kfmb ) {
    Index ifmb = kfmb + 1;
    Name sfmb = std::to_string(ifmb);
    if ( sfmb.size() < 2 ) sfmb = "0" + sfmb;
    Index ich1 = fuEdges[kfmb];
    Index ich2 = fuEdges[kfmb+1];
    if ( ich1 && ich2 && ich1 != ich2 ) {
      if ( ich1 > ich2 ) std::swap(ich1, ich2);
      insert("femb" + sfmb + "u", ich1, ich2, "FEMB " + sfmb + "U");
    }
    ich1 = fyEdges[kfmb];
    ich2 = fyEdges[kfmb+1];
    if ( ich1 && ich2 && ich1 != ich2 ) {
      if ( ich1 > ich2 ) std::swap(ich1, ich2);
      insert("femb" + sfmb + "y", ich1, ich2, "FEMB " + sfmb + "Y");
    }
    ich1 = fzEdges[kfmb];
    ich2 = fzEdges[kfmb+1];
    if ( ich1 && ich2 && ich1 != ich2 ) {
      if ( ich1 > ich2 ) std::swap(ich1, ich2);
      insert("femb" + sfmb + "z", ich1, ich2, "FEMB " + sfmb + "Z");
    }
  }
}


IndexRange VDColdboxChannelRanges::get(string sran) const {
  const string myname = "VDColdboxChannelRanges::get: ";
  RangeMap::const_iterator iran = m_rans.find(sran);
  if ( iran  != m_rans.end() ) return iran->second;
  if ( m_LogLevel >= 2 ) {
    cout << myname << "Invalid channel range name: " << sran << endl;
  }
  return IndexRange(0, 0);
}

void VDColdboxChannelRanges::insert(Name sran, Index ich1, Index ich2, Name slab1) {
  m_rans[sran] = IndexRange(sran, ich1, ich2, slab1);
}

DEFINE_ART_CLASS_TOOL(VDColdboxChannelRanges)
