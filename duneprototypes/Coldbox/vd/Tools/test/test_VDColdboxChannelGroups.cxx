// test_VDColdboxChannelGroups.cxx
//
// David Adams
// May 2018
//
// Test VDColdboxChannelGroups.

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "dune/ArtSupport/DuneToolManager.h"
#include "dune/DuneInterface/Tool/IndexRangeGroupTool.h"
#include "TH1F.h"

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
using fhicl::ParameterSet;
using Index = IndexRange::Index;
using IndexVector = std::vector<Index>;

Index checkran(const IndexRangeGroupTool& rm, string sgrp, Index nexp =0, bool chkvalid =true) {
  const string myname = "checkran: ";
  IndexRangeGroup grp = rm.get(sgrp);
  if ( grp.size() != 1 ) {
    cout << myname << "Unable to find group " << sgrp << endl;
    assert(false);
  }
  IndexRange ran = grp.range(0);
  if ( ran.isValid() ) cout << ran << endl;
  if ( chkvalid && ! ran.isValid() ) {
    cout << myname << "Invalid range in  " << sgrp << endl;
    assert(false);
  }
  if ( nexp ) assert( ran.size() == nexp );
  return ran.size();
}
  
//**********************************************************************

int test_VDColdboxChannelGroups(bool useExistingFcl =false) {
  const string myname = "test_VDColdboxChannelGroups: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  string fclfile = "test_VDColdboxChannelGroups.fcl";
  if (useExistingFcl) {
    cout << myname << "Using existing top-level FCL." << endl;
  } else {
    cout << myname << "Creating top-level FCL." << endl;
    ofstream fout(fclfile.c_str());
    fout << "tools: {" << endl;
    fout << "  channelRanges: {" << endl;
    fout << "    tool_type: VDColdboxChannelRanges" << endl;
    fout << "    LogLevel: 1" << endl;
    fout << "    GhostRange: [3200, 3392]" << endl;
    fout << "  }" << endl;
    fout << "  mytool: {" << endl;
    fout << "    tool_type: VDColdboxChannelGroups" << endl;
    fout << "    LogLevel: 1" << endl;
    fout << "  }" << endl;
    fout << "}" << endl;
    fout.close();
  }

  cout << myname << line << endl;
  cout << myname << "Fetching tool manager." << endl;
  DuneToolManager* ptm = DuneToolManager::instance(fclfile);
  assert ( ptm != nullptr );
  DuneToolManager& tm = *ptm;
  tm.print();
  assert( tm.toolNames().size() == 2 );

  cout << myname << line << endl;
  cout << myname << "Fetching tool." << endl;
  auto cma = tm.getPrivate<IndexRangeGroupTool>("mytool");
  assert( cma != nullptr );

  cout << myname << line << endl;
  cout << myname << "Check detector." << endl;
  checkran(*cma, "cru", 3200);
  checkran(*cma, "crt", 1600);
  checkran(*cma, "crb", 1600);
  checkran(*cma, "crtu", 384);
  checkran(*cma, "crty", 640);
  checkran(*cma, "crtz", 576);
  Index nchu = checkran(*cma, "crbu", 384);
  assert(nchu);
  Index nchy = checkran(*cma, "crby", 640);
  assert(nchy);
  Index nchz = checkran(*cma, "crbz", 576);
  assert(nchz);

  cout << myname << line << endl;
  cout << myname << "Check U FEMBs." << endl;
  Index nchuf = 0;
  for ( string sran : { "femb01u", "femb02u", "femb03u", "femb04u", "femb05u",
                        "femb06u", "femb07u", "femb08u", "femb09u", "femb10u", "femb11u" } ) {
    nchuf += checkran(*cma, sran, 0);
  }
  cout << myname << "  Checking " << nchuf << " == " << nchu << endl;
  assert( nchuf == nchu );

  cout << myname << line << endl;
  cout << myname << "Check Y FEMBs." << endl;
  Index nchyf = 0;
  for ( string sran : { "femb01y", "femb02y", "femb03y", "femb04y",
                        "femb11y", "femb12y", "femb13y" } ) {
    nchyf += checkran(*cma, sran, 0);
  }
  cout << myname << "  Checking " << nchyf << " == " << nchy << endl;
  assert( nchyf == nchy );

  cout << myname << line << endl;
  cout << myname << "Check Z FEMBs." << endl;
  Index nchzf = 0;
  for ( string sran : { "femb04z", "femb05z", "femb06z", "femb07z",
                        "femb08z", "femb09z", "femb10z", "femb11z" } ) {
    nchzf += checkran(*cma, sran, 0);
  }
  cout << myname << "  Checking " << nchzf << " == " << nchz << endl;
  assert( nchzf == nchz );

  cout << myname << line << endl;
  cout << myname << "Check each FEMB." << endl;
  Index ntot = 0;
  vector<vector<string>> fmbPlanes(14);
  for ( Index ifmb=1; ifmb<=11; ++ifmb ) fmbPlanes[ifmb].push_back("u");
  for ( Index ifmb=1; ifmb<=4; ++ifmb ) fmbPlanes[ifmb].push_back("y");
  for ( Index ifmb=11; ifmb<=13; ++ifmb ) fmbPlanes[ifmb].push_back("y");
  for ( Index ifmb=4; ifmb<=11; ++ifmb ) fmbPlanes[ifmb].push_back("z");
    
  for ( Index ifmb=1; ifmb<=13; ++ifmb ) {
    string sbas = string("femb") + (ifmb<10 ? "0" : "") + std::to_string(ifmb);
    Index nchf = 0;
    for ( string spla : fmbPlanes[ifmb] ) {
      nchf += checkran(*cma, sbas + spla, 0, false);
    }
    cout << "*** " << sbas << " count is " << nchf << endl;
    assert( nchf <= 128 );
    ntot += nchf;
  }
  cout << myname << "*** FEMB total is " << ntot << endl;
  assert( ntot == 1600 );

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  bool useExistingFcl = false;
  if ( argc > 1 ) {
    string sarg(argv[1]);
    if ( sarg == "-h" ) {
      cout << "Usage: " << argv[0] << " [keepFCL] [NSHOW]" << endl;
      cout << "  keepFCL [false]: If true, existing FCL file is used." << endl;
      cout << "  NSHOW [64]: Every nshow'th channels will be displayed in log." << endl;
      return 0;
    }
    useExistingFcl = sarg == "true" || sarg == "1";
  }
  return test_VDColdboxChannelGroups(useExistingFcl);
}

//**********************************************************************
