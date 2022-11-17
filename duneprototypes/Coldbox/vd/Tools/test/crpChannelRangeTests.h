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

// Check a range in a rnage tool.
Index checkran(const IndexRangeTool& rt, string sran, Index bexp, Index nexp =0, bool chkvalid =true) {
  const string myname = "checkran: ";
  IndexRange ran = rt.get(sran);
  return checkran(ran, sran, bexp, nexp, chkvalid);
}

// Check a range in a range group tool.
Index checkran(const IndexRangeGroupTool& gt, string sran, Index bexp, Index nexp =0, bool chkvalid =true) {
  const string myname = "checkran: ";
  IndexRangeGroup grp = gt.get(sran);
  assert( grp.isValid() );
  assert( grp.size() == 1 );
  IndexRange ran = grp.ranges[0];
  return checkran(ran, sran, bexp, nexp, chkvalid);
}

template<class T>
int checkChannelRanges(string callname, string det, const T& rt, string line) {
  string myname = callname + "checkChannelRanges: ";
  cout << myname << line << endl;
  cout << myname << "Check detector." << endl;
  if ( det == "cb2022" ) {
    checkran(rt, "crdet",    0, 3072);
    checkran(rt,  "cruC",    0, 3072);
    checkran(rt, "cruCu",    0,  952);
    checkran(rt, "cruCv",  952,  952);
    checkran(rt, "cruCz", 1904, 1168);
  } else if ( det == "pdvd2022" ) {
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
    cout << myname << line << endl;
    cout << myname << "Check adapters. Not." << endl;
    cout << myname << line << endl;
    cout << myname << "Check kels. Not." << endl;
  } else {
    cout << myname << "ERROR: Detector not known: " << det << endl;
  }

  return 0;
}
