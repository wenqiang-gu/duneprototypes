// test_CrpChannelRanges.cxx
//
// David Adams
// May 2018
//
// Test CrpChannelRanges.

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include "TH1F.h"
#include "crpChannelRangeTests.h"

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

//**********************************************************************

int test_CrpChannelRanges(bool useExistingFcl, string sdet) {
  const string myname = "test_CrpChannelRanges: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  cout << myname << "Testing detector config " << sdet << endl;
  cout << myname << line << endl;
  string fclfile = "test_CrpChannelRanges_" + sdet + ".fcl";
  if ( useExistingFcl ) {
    cout << myname << "Using existing top-level FCL." << endl;
  } else {
    cout << myname << "Creating top-level FCL." << endl;
    ofstream fout(fclfile.c_str());
    fout << "#include \"vdcb2_tools.fcl\"" << endl;
    fout << "save: @local::tools.crpChannelFemb" << endl;
    fout << "tools: @erase" << endl;
    fout << "tools: {" << endl;
    fout << "  crpChannelFemb: @local::save" << endl;
    fout << "  channelRanges: {" << endl;
    fout << "    tool_type: CrpChannelRanges" << endl;
    fout << "    LogLevel: 1" << endl;
    fout << "    Detector: \"" << sdet << "\"" << endl;
    fout << "  }" << endl;
    fout << "}" << endl;
    fout.close();
  }

  cout << myname << line << endl;
  cout << myname << "Fetching tool manager for fcl file " << fclfile << "." << endl;
  DuneToolManager* ptm = DuneToolManager::instance(fclfile);
  ptm->print();
  assert( ptm->toolNames().size() == 2 );

  cout << myname << line << endl;
  cout << myname << "Fetching tool." << endl;
  auto cma = ptm->getPrivate<IndexRangeTool>("channelRanges");
  assert( cma != nullptr );

  checkChannelRanges(myname, sdet, *cma, line);

  cout << myname << line << endl;
  cout << myname << "Done." << endl;

  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  bool useExistingFcl = false;
  string ssdet = "cb2022:nofembs,pdvd:nofembs,cb2022,pdvd";
  if ( argc > 1 ) {
    string sarg(argv[1]);
    if ( sarg == "-h" ) {
      cout << "Usage: " << argv[0] << " [keepFCL] [DET]" << endl;
      cout << "  keepFCL [false]: If \"true\" or 1, existing FCL file is used." << endl;
      cout << "  DETS: Comma-separate list of detectors. [" << ssdet << "]" << endl;
      cout << "  If FEMBs are used, then pdvd2_tools.fcl is included." << endl;
      return 0;
    }
    useExistingFcl = sarg == "true" || sarg == "1";
    if ( argc > 2 ) {
      ssdet = argv[2];
    }
  }
  vector<string> sdets = StringManipulator(ssdet).split(",");
  if ( sdets.size() == 0 ) {
    cout << "Empty detector string." << endl;
    return 1;
  }
  if ( sdets.size() == 1 ) {
    return test_CrpChannelRanges(useExistingFcl, ssdet);
  }
  string line = "=======================================================";
  cout << line << endl;
  Index rc = 99;
  for ( string sdet : sdets ) {
    cout << "Testing detector " << sdet << endl;
    string com = string(argv[0]) + " " + std::to_string(useExistingFcl) + " " + sdet;
    cout << "Command: " << com << endl;
    rc = system(com.c_str());
    if ( rc ) return rc;
    cout << line << endl;
  }
  return rc;
}

//**********************************************************************
