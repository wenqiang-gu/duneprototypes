// crpChannelRangeTests.h
//
// Test the CRP channel ranges.
// Return 0 for success.
//
// It is intended this be included is source files that test the expected
// CRP channel ranges are present and cover the expected ranges.

#include <string>
#include <set>
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include "dunecore/DuneInterface/Tool/IndexRangeGroupTool.h"
#include "dunecore/DuneCommon/Utility/StringManipulator.h"

using std::string;
using std::cout;
using std::endl;
using std::to_string;
using NameVector = std::vector<string>;
using NameSet = std::set<string>;
using Index = IndexRange::Index;

// Check a range.
Index checkran(const IndexRange& ran, string sran, Index bexp, Index nexp =0, bool chkvalid =true) {
  const string myname = "checkran: ";
  if ( ran.isValid() ) cout << ran << endl;
  if ( chkvalid && ! ran.isValid() ) {
    cout << myname << "Unable to find range " << sran << endl;
    assert(false);
  }
  if ( nexp ) {
    assert( ran.begin == bexp );
    assert( ran.size() == nexp );
  }
  return ran.size();
}

// Check a range in a range tool.
Index checkran(const IndexRangeTool& rt, string sran, Index bexp =99999, Index nexp =0, bool chkvalid =true) {
  const string myname = "checkran: ";
  IndexRange ran = rt.get(sran);
  return checkran(ran, sran, bexp, nexp, chkvalid);
}

// Check a range in a range group tool.
Index checkran(const IndexRangeGroupTool& gt, string sran, Index bexp =99999, Index nexp =0, bool chkvalid =true) {
  const string myname = "checkran: ";
  IndexRangeGroup grp = gt.get(sran);
  string emsg;
  if ( ! grp.isValid() ) emsg = "Invalid range group: " + sran;
  else if ( grp.size() != 1 ) emsg = "Range group " + sran + " has size " + to_string(grp.size()) + " != 1";
  if ( emsg.size() ) {
    cout << myname << "ERROR: " << emsg << endl;
    assert(false);
  }
  IndexRange ran = grp.ranges[0];
  return checkran(ran, sran, bexp, nexp, chkvalid);
}

// ****** Check FEMB ranges ******

IndexRange get_only_range(const IndexRange& ran) {
  return ran;
}

IndexRange get_only_range(const IndexRangeGroup& grp) {
  return grp.range(0);
}

IndexRangeGroup get_group(const IndexRangeTool& rt, string) {
  assert(false);
  return IndexRangeGroup();
}

IndexRangeGroup get_group(const IndexRangeGroupTool& rt, string gnam) {
  return rt.get(gnam);
}

template<class T>
void check_femb_ranges(const T& rt, const NameVector& svols, bool doGroups) {
  const string myname = "check_femb_ranges: ";
  Index nran = 0;
  for ( string svol : svols ) {
    for ( Index ifmb=1; ifmb<=24; ++ifmb ) {
      string sfmb = std::to_string(ifmb);
      while ( sfmb.size() < 2 ) sfmb = "0" + sfmb;
      cout << myname << " ================== FEMB " << svol << sfmb << " ===================" << endl;
      Index nch_femb = 0;
      Index nfran = 0;
      NameSet rrnams;
      for ( string svie : {"u", "v", "z"} ) {
        string sran = "femb" + svol + sfmb + svie;
        IndexRange ran = get_only_range(rt.get(sran));
        if ( ran.isValid() ) {
          cout << myname << "  " << ran << endl;
          Index nch_view = ran.size();
          nch_femb += nch_view;
          ++nran;
          ++nfran;
          rrnams.insert(ran.name);
        } else {
          cout << myname << "  " << sran << ": *** Not found ***" << endl;
        }
      }
      if ( doGroups ) {
        string gnam = "femb" + svol + sfmb;
        IndexRangeGroup grp = get_group(rt, gnam);
        if ( ! grp.isValid() ) {
          cout << myname << "Group " << gnam << " not found." << endl;
          assert(false);
        } else {
          cout << myname << "  " << grp << endl;
          assert( grp.size() == nfran );
          NameSet grnams;
          for ( Index iran=0; iran<grp.size(); ++iran ) {
            grnams.insert(grp.range(iran).name);
          }
          assert( grnams == rrnams );
        }
      }
      cout << myname << "FEMB " << svol << sfmb << " has " << nch_femb << " channels.";
      if ( nch_femb == 128 ) {
        cout << " Good." << endl;
      } else {
        cout << " ERROR: This should be 128 channels." << endl;
        assert(false);
      }
    }
  }
  cout << myname << " ===============================================" << endl;
  cout << myname << "Good range FEMB count is " << nran << endl;
}

// ****** Check all ranges ******

template<class T>
int checkChannelRanges(string callname, string sdet, const T& rt, string line, bool doGroups =false) {
  string myname = callname + "checkChannelRanges: ";
  std::vector<string> svals = StringManipulator(sdet).split(":");
  assert( svals.size() > 0 );
  assert( svals.size() < 3 );
  string detname = svals[0];
  bool check_fembs = detname == "pdvd";
  bool check_adas = detname == "pdvd";
  string qual = "";
  for ( Index ival =1; ival<svals.size(); ++ival ) {
    string qual = svals[ival];
    if        ( qual == "fembs" ) check_fembs = true;
    else if ( qual == "nofembs" ) check_fembs = false;
    else {
      cout << myname << "Unexpected detectpr qualifier " << qual << " in " << sdet << endl;
      assert(false);
    }
  }
  cout << myname << line << endl;
  cout << myname << "Check detector " << sdet << " (" << detname << ")." << endl;
  if ( detname == "cb2022" ) {
    checkran(rt, "all",     0, 3072);
    checkran(rt, "crdet",    0, 3072);
    checkran(rt,  "cruC",    0, 3072);
    checkran(rt, "cruCu",    0,  952);
    checkran(rt, "cruCv",  952,  952);
    checkran(rt, "cruCz", 1904, 1168);
    if ( check_fembs ) {
      NameVector svols = {"C"};
      check_femb_ranges(rt, svols, doGroups);
    }
  } else if ( detname == "pdvd" ) {
    checkran(rt, "crdet",    0, 2*6144);
    checkran(rt,   "crb",    0,   6144);
    checkran(rt,   "crt", 6144,   6144);
    checkran(rt,  "crbA",    0,   3072);
    checkran(rt,  "crbB", 3072,   3072);
    checkran(rt,  "crtA", 6144,   3072);
    checkran(rt,  "crtB", 9216,   3072);
    string nams[4] = {"crbA", "crbB", "crtA", "crtB"};
    Index  begs[4] = {     0,   3072,   6144,   9216};
    for ( Index icru=0; icru<4; ++icru ) {
      string nam = nams[icru];
      Index  beg = begs[icru];
      checkran(rt,  nam+"u",      beg,    952);
      checkran(rt,  nam+"v",  beg+952,    952);
      checkran(rt,  nam+"z", beg+1904,   1168);
    }
    if ( check_fembs ) {
      cout << myname << line << endl;
      cout << myname << "Check FEMB ranges." << endl;
      NameVector svols = {"A", "B"};
      check_femb_ranges(rt, svols, doGroups);
    }
    if ( check_adas ) {
      cout << myname << line << endl;
      cout << myname << "Check adapters. Not." << endl;
      cout << myname << line << endl;
      cout << myname << "Check kels. Not." << endl;
    }
  } else {
    cout << myname << "ERROR: Detector not known: " << detname << endl;
    assert(false);
  }

  return 0;
}
