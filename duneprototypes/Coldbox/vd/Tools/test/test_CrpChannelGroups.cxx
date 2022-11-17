// test_CrpChannelGroups.cxx
//
// David Adams
// May 2018
//
// Test CrpChannelGroups.

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexRangeGroupTool.h"
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

int test_CrpChannelGroups(string det) {
  const string myname = "test_CrpChannelGroups: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  string fclfile = "test_CrpChannelGroups.fcl";
  if ( true ) {
    cout << myname << "Creating top-level FCL." << endl;
    ofstream fout(fclfile.c_str());
    fout << "tools: {" << endl;
    fout << "  channelRanges: {" << endl;
    fout << "    tool_type: CrpChannelRanges" << endl;
    fout << "    LogLevel: 1" << endl;
    fout << "    Detector: \"" << det << "\"" << endl;
    fout << "  }" << endl;
    fout << "  mytool: {" << endl;
    fout << "    tool_type: CrpChannelGroups" << endl;
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

  checkChannelRanges(myname, det, *cma, line);

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  string det = "pdvd2022";
  if ( argc > 1 ) {
    string sarg(argv[1]);
    if ( sarg == "-h" ) {
      cout << "Usage: " << argv[0] << "[cb2022 | pdvd2022 | -h]" << endl;
      return 0;
    }
    det = sarg;
  }
  return test_CrpChannelGroups(det);
}

//**********************************************************************
