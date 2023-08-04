// test_fcl_pdhdChannelFemb.cxx
//
// David Adams
// June 2023
//
// Test tool pdhdChannelFemb in pdhd_tools.fcl.

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexMapTool.h"

#undef NDEBUG
#include <cassert>

using std::string;
using std::cout;
using std::endl;
using std::ofstream;
using std::istringstream;
using std::ostringstream;
using std::setw;
using std::vector;
using Index = unsigned int;
using IndexVector = std::vector<Index>;

//**********************************************************************

int test_fcl_pdhdChannelFemb() {
  const string myname = "test_fcl_pdhdChannelFemb: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  string fclfile = "pdhd_tools.fcl";
 string toolname = "pdhdChannelFemb";

  cout << myname << line << endl;
  cout << myname << "Fetching tool manager." << endl;
  DuneToolManager* ptm = DuneToolManager::instance(fclfile);
  assert ( ptm != nullptr );
  DuneToolManager& tm = *ptm;
  tm.print();
  assert( tm.toolNames().size() > 0 );

  cout << myname << line << endl;
  cout << myname << "Fetching tool." << endl;
  auto ptoo = tm.getPrivate<IndexMapTool>(toolname);
  assert( ptoo != nullptr );

  cout << myname << line << endl;
  cout << myname << "Check channels." << endl;
  Index ncha = 10240;
  Index nchaPerApa = 2560;
  Index napa = 4;
  assert( napa*nchaPerApa == ncha );
  Index nfemb = 20;
  cout << myname << "FEMB count: " << nfemb << endl;
  Index nchaPerFemb = ncha/nfemb;
  cout << myname << "Channels/FEMB count: " << nchaPerFemb << endl;
  Index fmin = 999;
  Index fmax = 0;
  IndexVector fembChanCounts(nfemb+1, 0);
  std::vector<IndexVector> apaFembChanCounts(napa, fembChanCounts);
  for ( Index icha=0; icha<ncha; ++icha ) {
     Index kapa = icha/nchaPerApa;
     Index ifmb = ptoo->get(icha);
     assert( ifmb > 0 );
     assert( ifmb <= nfemb );
     if ( ifmb < fmin ) fmin = ifmb;
     if ( ifmb > fmax ) fmax = ifmb;
     ++fembChanCounts[ifmb];
     ++apaFembChanCounts[kapa][ifmb];
  }
  cout << myname << "FEMB range: [" << fmin << ", " << fmax << "]" << endl;
  assert( fmin == 1 );
  assert( fmax == nfemb );

  cout << myname << line << endl;
  cout << myname << "Check FEMB channel counts." << endl;
  cout << myname << setw(4) << "    -------------- # Channels/APA/FEMB ---------------" << endl;
  cout << myname << "FEMB       all     APA 1     APA 3     APA 2     APA 4" << endl;
  for ( Index ifmb=1; ifmb<=nfemb; ++ifmb ) {
    Index ncha = fembChanCounts[ifmb];
    cout << myname << setw(4) << ifmb << setw(10) << ncha;
    for ( Index kapa=0; kapa<napa; ++kapa ) {
      cout << setw(10) << apaFembChanCounts[kapa][ifmb];
    }
    cout << endl;
    assert( ncha == nchaPerFemb );
  }

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  if ( argc > 1 ) {
    cout << "Usage: " << argv[0] << endl;
  }
  return test_fcl_pdhdChannelFemb();
}

//**********************************************************************
