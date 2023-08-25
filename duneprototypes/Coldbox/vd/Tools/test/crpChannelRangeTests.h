// crpChannelRangeTests.h
//
// Test the CRP channel ranges.
// Return 0 for success.
//
// It is intended this be included is source files that test the expected
// CRP channel ranges are present and cover the expected ranges.

#include <string>
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include "dunecore/DuneInterface/Tool/IndexRangeGroupTool.h"
#include "dunecore/DuneCommon/Utility/StringManipulator.h"

using std::string;
using std::cout;
using std::endl;
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
  assert( grp.isValid() );
  assert( grp.size() == 1 );
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

template<class T>
void check_femb_ranges(const T& rt) {
  const string myname = "check_femb_ranges: ";
  Index nran = 0;
  for ( string stpc : {"A", "B"} ) {
    for ( Index ifmb=1; ifmb<=16; ++ifmb ) {
      Index nch_femb = 0;
      string sfmb = std::to_string(ifmb);
      while ( sfmb.size() < 2 ) sfmb = "0" + sfmb;
      for ( string svie : {"u", "v", "z"} ) {
        string sran = "femb" + stpc + sfmb + svie;
        IndexRange ran = get_only_range(rt.get(sran));
        if ( ran.isValid() ) {
          cout << myname << "  " << ran << endl;
          Index nch_view = ran.size();
          nch_femb += nch_view;
          ++nran;
        } else {
          cout << myname << "  Range " << sran << " not found." << endl;
        }
      }
      cout << myname << "FEMB " << stpc << sfmb << " has " << nch_femb << " channels.";
      if ( nch_femb == 128 ) {
        cout << " Good." << endl;
      } else {
        cout << " ERROR: This should be 128 channels." << endl;
        assert(false);
      }
    }
  }
  cout << myname << "Good range FEMB count is " << nran << endl;
}

//void check_femb_ranges(const IndexRangeGroupTool& rt) {
//  const string myname = "check_femb_ranges: ";
//  cout << myname << "Test is disabled for IndexRangeGroupTool." << endl;
//  assert(false); //for now
//}

// ****** Check all ranges ******

template<class T>
int checkChannelRanges(string callname, string sdet, const T& rt, string line) {
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
    checkran(rt, "crdet",    0, 3072);
    checkran(rt,  "cruC",    0, 3072);
    checkran(rt, "cruCu",    0,  952);
    checkran(rt, "cruCv",  952,  952);
    checkran(rt, "cruCz", 1904, 1168);
    if ( check_fembs ) {
      cout << myname << "Checking FEMB ranges. Sorta." << endl;
      for ( Index ifmb=0; ifmb<=16; ++ifmb ) {
        string sfmb = std::to_string(ifmb);
        if ( sfmb.size() < 2 ) sfmb = "0" + sfmb;
        for ( string sori : {"u", "v", "z"} ) {
          string sran = "fembC" + sfmb + sori;
          // IndexRange or IndexRangeGroup
          auto ran = rt.get(sran);
          cout << "  " << sran << ": " << ran << endl;
        }
      }
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
      check_femb_ranges(rt);
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
