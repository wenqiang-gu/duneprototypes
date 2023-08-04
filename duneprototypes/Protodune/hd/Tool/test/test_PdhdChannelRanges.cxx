// test_PdhdChannelRanges.cxx
//
// David Adams
// July 2018
//
// Test PdhdChannelRanges.

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include "dunecore/DuneInterface/Tool/IndexMapTool.h"
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
using std::setfill;
using std::vector;
using fhicl::ParameterSet;
using Index = unsigned int;
using IndexVector = std::vector<Index>;

//**********************************************************************

int test_PdhdChannelRanges(bool useExistingFcl =false, int show =1) {
  const string myname = "test_PdhdChannelRanges: ";
#ifdef NDEBUG
  cout << myname << "NDEBUG must be off." << endl;
  abort();
#endif
  string line = "-----------------------------";

  cout << myname << line << endl;
  string fclfile = "test_PdhdChannelRanges.fcl";
  if ( ! useExistingFcl ) {
    cout << myname << "Creating top-level FCL." << endl;
    ofstream fout(fclfile.c_str());
    fout << "tools: {" << endl;
    fout << "  mytool: {" << endl;
    fout << "    tool_type: PdhdChannelRanges" << endl;
    fout << "    LogLevel: 2" << endl;
    fout << "    ExtraRanges: \"\"" << endl;
    fout << "  }" << endl;
    fout << "}" << endl;
    fout << endl;
    fout << "#include \"pdhd_chanmap_tools.fcl\"" << endl;
    fout.close();
  } else {
    cout << myname << "Using existing top-level FCL." << endl;
  }

  Index napa = 4;

  cout << myname << line << endl;
  cout << myname << "Fetching tool manager." << endl;
  DuneToolManager* ptm = DuneToolManager::instance(fclfile);
  assert ( ptm != nullptr );
  DuneToolManager& tm = *ptm;
  tm.print();
  assert( tm.toolNames().size() >= 1 );

  cout << myname << line << endl;
  cout << myname << "Fetching channel range tool." << endl;
  auto irt = tm.getPrivate<IndexRangeTool>("mytool");
  assert( irt != nullptr );

  cout << myname << line << endl;
  cout << myname << "Set TPS-APA mappings." << endl;
  vector<bool> isNorthTps = {false, true, false, true};
  vector<bool> isSouthTps = {true, false, true, false};
  vector<Index> tpsToApa = {1, 3, 2, 4};
  vector<Index> apaToTps = {99, 0, 2, 1, 3};
  Index w = 8;
  cout << myname << setw(w) << "TPS" << setw(w) << "APA" << endl;
  for ( Index itps=0; itps<napa; ++itps ) {
    Index iapa = tpsToApa[itps];
    cout << myname << setw(w) << itps << setw(w) << iapa << endl;
    assert( itps < napa );
    assert( iapa <= napa );
    assert( apaToTps[iapa] == itps );
  }

  cout << myname << line << endl;
  cout << myname << "Fetching TPC ranges." << endl;
  int nbad = 0;
  vector<string> namSufs = {   "",   "u",   "v",   "z",  "c",   "x",    "i"};
  vector<string> namPres(napa+1, "tpp");
  namPres[0] = "tps";
  vector<Index> firstTpsZChan(napa+1, 0);
  vector<Index> firstTpsCChan(napa+1, 0);
  for ( Index inam=0; inam<namPres.size(); ++inam ) {
    string svie = namSufs[inam];
    bool isZ = svie == "z";
    bool isC = svie == "c";
    for ( Index itps=0; itps<napa; ++itps ) {
      string stps = std::to_string(itps);
      string nam = namPres[inam] + stps + svie;
      IndexRange ir = irt->get(nam);
      if ( ! ir.isValid() ) {
        cout << myname << "Invalid range: " << nam << endl;
        ++nbad;
      } else {
        cout << myname << setw(10) << ir.name << setw(20) << ir.rangeString()
             << " " << ir.label();
        for ( Index ilab=1; ilab<ir.labels.size(); ++ilab ) cout << ", " << ir.label(ilab);
        cout << endl;
        assert( ir.name == nam );
      }
      if ( isZ ) firstTpsZChan[itps] = ir.first();
      if ( isC ) firstTpsCChan[itps] = ir.first();
    }
  }
  assert( nbad == 0 );

  cout << myname << line << endl;
  cout << myname << "Fetching APA ranges." << endl;
  nbad = 0;
  vector<string> namPresApa(napa+1, "apa");
  for ( Index inam=0; inam<namPres.size(); ++inam ) {
    for ( Index iapa=1; iapa<=napa; ++iapa ) {
      string stps = std::to_string(iapa);
      string nam = namPresApa[inam] + stps + namSufs[inam];
      IndexRange ir = irt->get(nam);
      if ( ! ir.isValid() ) {
        cout << myname << "Invalid range: " << nam << endl;
        ++nbad;
      } else {
        cout << myname << setw(10) << ir.name << setw(20) << ir.rangeString()
             << " " << ir.label();
        for ( Index ilab=1; ilab<ir.labels.size(); ++ilab ) cout << ", " << ir.label(ilab);
        cout << endl;
        assert( ir.name == nam );
      }
    }
  }
  assert( nbad == 0 );

  cout << myname << line << endl;
  cout << myname << "Check left/right." << endl;
  for ( Index itps=0; itps<napa; ++itps ) {
    Index zc = firstTpsZChan[itps];
    Index cc = firstTpsCChan[itps];
    cout << myname << "TPS " << itps << " Z, C: " << zc << ", " << cc << endl;
    assert( zc );
    assert( cc );
    if ( isNorthTps[itps] ) {
      assert( zc < cc );
    } else if ( isSouthTps[itps] ) {
      assert( zc > cc );
    } else {
      assert(false);
    }
  }

  bool showFembBlocks = show > 1;
  if ( showFembBlocks ) {

    cout << myname << line << endl;
    cout << myname << "Fetching channel-FEMB map tool." << endl;
    auto pcfmap = tm.getPrivate<IndexMapTool>("pdhdChannelFemb");
    assert( pcfmap != nullptr );

    cout << myname << line << endl;
    cout << myname << "Fetching FEMB block ranges." << endl;
    IndexVector chk(15360, 0);
    Index nbadRange = 0;
    Index nwrongFemb = 0;
    for ( Index iapa=1; iapa<=napa; ++iapa ) {
      for ( string view : {"u", "v", "x"} ) {
        for ( Index ifmb=1; ifmb<=20; ++ifmb ) {
          ostringstream ssnam;
          ssnam << "femb" << iapa << setfill('0') << setw(2) << ifmb << view;
          string basenam = ssnam.str();
          vector<string> nams;
          if ( irt->get(basenam).isValid() ) {
            nams.push_back(basenam);
          } else {
            nams.push_back(basenam + "1");
            nams.push_back(basenam + "2");
          }
          for ( string nam : nams ) {
            IndexRange ir = irt->get(nam);
            if ( ! ir.isValid() ) {
              cout << myname << "Invalid range: " << nam << endl;
              ++nbadRange;
            } else {
              cout << myname << setw(10) << ir.name << setw(20) << ir.rangeString()
                   << " " << ir.label();
              for ( Index ilab=1; ilab<ir.labels.size(); ++ilab ) cout << ", " << ir.label(ilab);
              cout << endl;
              for ( Index icha=ir.begin; icha<ir.end; ++icha ) {
                chk[icha] += 1;
                Index imapfmb = pcfmap->get(icha);
                if ( imapfmb != ifmb ) {
                  cout << myname << "ERROR: FEMB mismatch for channel " << icha << ": " << ifmb << " != " << imapfmb << endl;
                  ++nwrongFemb;
                  assert( nwrongFemb < 500 );
                }
              }
            }
          }
        }
      }
    }
    Index nerr = 0;
    for ( Index icha=0; icha<10240; ++icha ) {
      if ( chk[icha] != 1 ) {
        cout << myname << "ERROR: Check count for channel " << icha << " is " << chk[icha] << endl;
        ++nerr;
      }
      assert( nerr < 200 );
    }
    assert( nwrongFemb == 0 );
    assert( nerr == 0 );
  }

  cout << myname << line << endl;
  cout << "Fetch bad range" << endl;
  IndexRange irb = irt->get("rangebad");
  cout << irb.rangeString() << endl;
  assert( ! irb.isValid() );

  cout << myname << line << endl;
  cout << myname << "Done." << endl;
  return 0;
}

//**********************************************************************

int main(int argc, char* argv[]) {
  bool useExistingFcl = false;
  int show = 2;
  if ( argc > 1 ) {
    string sarg(argv[1]);
    if ( sarg == "-h" ) {
      cout << "Usage: " << argv[0] << " [keepFCL] [SHOW]" << endl;
      cout << "  If keepFCL = true, existing FCL file is used." << endl;
      cout << "  SHOW > 1 also shows the 480 FEMB blocks." << endl;
      return 0;
    }
    useExistingFcl = sarg == "true" || sarg == "1";
  }
  if ( argc > 2 ) {
    string sarg(argv[2]);
    show = std::stoi(sarg);
  }
  return test_PdhdChannelRanges(useExistingFcl, show);
}

//**********************************************************************
