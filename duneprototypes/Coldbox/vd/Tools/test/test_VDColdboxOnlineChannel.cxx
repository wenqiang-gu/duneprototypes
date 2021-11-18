// test_VDColdboxOnlineChannel.cxx
//
// David Adams
// May 2018
//
// Test VDColdboxOnlineChannel.

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "dune/ArtSupport/DuneToolManager.h"
#include "dune/DuneInterface/Tool/IndexMapTool.h"
#include "dune/ArtSupport/ArtServiceHelper.h"
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
using fhicl::ParameterSet;
using Index = IndexMapTool::Index;
using IndexVector = std::vector<Index>;

string sonline(Index ichaOn) {
  ostringstream ssout;
  Index irem = ichaOn;
  Index ifmb = irem/128;
  irem = irem%128;
  Index iasc = irem/16 + 1;
  Index iach = irem%16;
  ssout << (ifmb<10 ? "0" : "") << ifmb << "-" << iasc << "-" << (iach<10 ? "0" : "") << iach;
  return ssout.str();
}

//**********************************************************************

int test_VDColdboxOnlineChannel(bool useExistingFcl =false, Index nshow =64) {
  const string myname = "test_VDColdboxOnlineChannel: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  string fclfile = "test_VDColdboxOnlineChannel.fcl";
  if (useExistingFcl) {
    cout << myname << "Using existing top-level FCL." << endl;
  } else {
    cout << myname << "Creating top-level FCL." << endl;
    ofstream fout(fclfile.c_str());
    fout << "#include \"VDColdboxChannelMapService.fcl\"" << endl;
    fout << "services: { VDColdboxChannelMapService: @local::vdcoldboxchannelmap }" << endl;
    fout << "tools: {" << endl;
    fout << "  mytool: {" << endl;
    fout << "    tool_type: VDColdboxOnlineChannel" << endl;
    fout << "     LogLevel: 1" << endl;
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
  assert( tm.toolNames().size() == 1 );

  std::ifstream config{fclfile};
  ArtServiceHelper::load_services(config);

  cout << myname << line << endl;
  cout << myname << "Fetching tool." << endl;
  auto cma = tm.getPrivate<IndexMapTool>("mytool");
  assert( cma != nullptr );

  Index badIndex = IndexMapTool::badIndex();

  cout << myname << line << endl;
  cout << myname << "Check some good values." << endl;
  for ( Index ichaOff : { 1600, 1700, 3199, 3200, 3391 } ) {
    Index ichaOn = cma->get(ichaOff);
    cout << myname << setw(5) << ichaOff << " --> " << setw(5) << ichaOn << endl;
    assert( ichaOff != badIndex );
  }

  cout << myname << line << endl;
  cout << myname << "Check some bad values." << endl;
  for ( Index ichaOff : { -1, 0, 100, 1599, 3392, 4000 } ) {
    Index ichaOn = cma->get(ichaOff);
    cout << myname << ichaOff << " --> " << ichaOn << endl;
    assert( ichaOn == badIndex );
  }

  cout << myname << line << endl;
  cout << myname << "Check each online index appears exactly once." << endl;
  const Index nchaOn = 14*128;
  const Index ichaOn1 = 128;
  const Index ichaOn2 = ichaOn1 + nchaOn;
  const Index ichaOff1 = 1600;
  const Index ichaOff2 = ichaOff1 + nchaOn;
  IndexVector offlineChannel(ichaOn2, badIndex);
  IndexVector onlineCounts(ichaOn2, 0);
  Index ndup = 0;
  for ( Index ichaOff=ichaOff1; ichaOff<ichaOff2; ++ichaOff ) {
    Index ichaOn = cma->get(ichaOff);
    if ( nshow*(ichaOff/nshow) == ichaOff ) {
      cout <<  myname << " " << setw(4) << ichaOff << " --> " << setw(4) << ichaOn
           << " (" << sonline(ichaOn) << ")" << endl;
    }
    assert( ichaOn >= ichaOn1 );
    assert( ichaOn < ichaOn2 );
    if ( offlineChannel[ichaOn] != badIndex ) {
      cout << myname << "ERROR: Online channel " << setw(4) << ichaOn
           << " (" << sonline(ichaOn) << ")"
           << " is mapped to two offline channels: "
           << offlineChannel[ichaOn]
           << "  " << ichaOff << endl;
      ++ndup;
    }
    onlineCounts[ichaOn] += 1;
    offlineChannel[ichaOn] = ichaOff;
  }
  Index nbadOn = 0;
  Index nbadOff = 0;
  for ( Index ichaOn=ichaOn1; ichaOn<ichaOn2; ++ichaOn ) {
    if ( onlineCounts[ichaOn] != 1 ) {
      cout << "ERROR: Map count for online channel " << ichaOn << " is " << onlineCounts[ichaOn] << endl;;
      ++nbadOn;
    } else if ( offlineChannel[ichaOn] == badIndex ) {
      cout << "ERROR: Online channel " << ichaOn << " is not mapped." << endl;
    }
  }
  assert( ndup + nbadOn + nbadOff == 0 );

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  bool useExistingFcl = false;
  Index nshow = 1;  // Every nshow'th value is displayed in log.
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
  if ( argc > 2 ) {
    string sarg(argv[2]);
    nshow = std::stoi(sarg);
  }
  return test_VDColdboxOnlineChannel(useExistingFcl, nshow);
}

//**********************************************************************
