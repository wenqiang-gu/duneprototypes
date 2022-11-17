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
using fhicl::ParameterSet;
using Index = IndexRange::Index;
using IndexVector = std::vector<Index>;

//**********************************************************************

int test_CrpChannelRanges(bool useExistingFcl, string det) {
  const string myname = "test_CrpChannelRanges: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  string fclfile = "test_CrpChannelRanges.fcl";
  if (useExistingFcl) {
    cout << myname << "Using existing top-level FCL." << endl;
  } else {
    cout << myname << "Creating top-level FCL." << endl;
    ofstream fout(fclfile.c_str());
    fout << "tools: {" << endl;
    fout << "  cb2022: {" << endl;
    fout << "    tool_type: CrpChannelRanges" << endl;
    fout << "    LogLevel: 1" << endl;
    fout << "    Detector: \"cb2022\"" << endl;
    fout << "  }" << endl;
    fout << "  pdvd2022: {" << endl;
    fout << "    tool_type: CrpChannelRanges" << endl;
    fout << "    LogLevel: 1" << endl;
    fout << "    Detector: \"pdvd2022\"" << endl;
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
  cout << myname << "Fetching tool " << det << "." << endl;
  auto cma = tm.getPrivate<IndexRangeTool>(det);
  assert( cma != nullptr );

  checkChannelRanges(myname, det, *cma, line);

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
  return test_CrpChannelRanges(useExistingFcl, "cb2022") + test_CrpChannelRanges(useExistingFcl, "pdvd2022");
}

//**********************************************************************
