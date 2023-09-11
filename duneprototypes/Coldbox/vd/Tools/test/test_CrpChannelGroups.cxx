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

#undef NDEBUG
#include <cassert>
#include "crpChannelRangeTests.h"

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

int test_CrpChannelGroups(bool useExistingFcl, string sdet, Index loglev) {
  const string myname = "test_CrpChannelGroups: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  string fclfile = "test_CrpChannelGroups_" + sdet + ".fcl";
  if ( ! useExistingFcl ) {
    cout << myname << line << endl;
    if ( true ) {
      cout << myname << "Creating top-level FCL." << endl;
      ofstream fout(fclfile.c_str());
      fout << "#include \"vdcb2_tools.fcl\"" << endl;
      fout << "save: @local::tools.crpChannelFemb" << endl;
      fout << "tools: {" << endl;
      fout << "  crpChannelFemb: @local::save" << endl;
      fout << "  channelRanges: {" << endl;
      fout << "    tool_type: CrpChannelRanges" << endl;
      fout << "    LogLevel: 0" << endl;
      fout << "    Detector: \"" << sdet << "\"" << endl;
      fout << "  }" << endl;
      fout << "  mytool: {" << endl;
      fout << "    tool_type: CrpChannelGroups" << endl;
      fout << "    LogLevel: " << loglev << endl;
      fout << "  }" << endl;
      fout << "}" << endl;
      fout.close();
    }
  }

  cout << myname << line << endl;
  cout << myname << "Fetching tool manager." << endl;
  DuneToolManager* ptm = DuneToolManager::instance(fclfile);
  assert ( ptm != nullptr );
  DuneToolManager& tm = *ptm;
  tm.print();
  assert( tm.toolNames().size() == 3 );

  cout << myname << line << endl;
  cout << myname << "Fetching tool." << endl;
  auto cma = tm.getPrivate<IndexRangeGroupTool>("mytool");
  assert( cma != nullptr );

  checkChannelRanges(myname, sdet, *cma, line, true);

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  string ssdet = "cb2022:nofembs,pdvd:nofembs,cb2022,pdvd";
  bool useExistingFcl = false;
  string sloglev = "1";
  if ( argc > 1 ) {
    string sarg(argv[1]);
    if ( sarg == "-h" ) {
      cout << "Usage: " << argv[0] << "keepFcl [DETS [LOGLEV]]]" << endl;
      return 0;
    }
    ssdet = sarg;
    useExistingFcl = sarg == "true" || sarg == "1";
    if ( argc > 2 ) {
      ssdet = argv[2];
      if ( argc > 3 ) {
        sloglev = argv[3];
      }
    }
  }
  Index loglev = std::stoi(sloglev);
  vector<string> sdets = StringManipulator(ssdet).split(",");
  if ( sdets.size() == 0 ) {
    cout << "Empty detector string." << endl;
    return 1;
  }
  if ( sdets.size() == 1 ) {
    return test_CrpChannelGroups(useExistingFcl, ssdet, loglev);
  }
  string line = "=======================================================";
  cout << line << endl;
  Index rc = 99;
  for ( string sdet : sdets ) {
    cout << "Testing detector " << sdet << endl;
    string com = string(argv[0]) + " " + std::to_string(useExistingFcl) + " " + sdet + " " + sloglev;
    cout << "Command: " << com << endl;
    rc = system(com.c_str());
    if ( rc ) return rc;
    cout << line << endl;
  }
  return rc;
}

//**********************************************************************
