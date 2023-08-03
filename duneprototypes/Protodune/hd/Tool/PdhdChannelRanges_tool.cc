// PdhdChannelRanges_tool.cc

#include "PdhdChannelRanges.h"
#include "dunecore/ArtSupport/DuneToolManager.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using std::cout;
using std::endl;
using std::string;
using std::ostringstream;
using std::istringstream;
using std::setw;
using std::setfill;
using Name = PdhdChannelRanges::Name;
using Index = PdhdChannelRanges::Index;
using IndexVector = std::vector<Index>;

//**********************************************************************

PdhdChannelRanges::PdhdChannelRanges(fhicl::ParameterSet const& ps)
: m_LogLevel(ps.get<Index>("LogLevel")),
  m_ExtraRanges(ps.get<Name>("ExtraRanges")) {
  const Name myname = "PdhdChannelRanges::ctor: ";
  const Index ntps = 4;
  Index nchaApa = 2560;
  Index nfchau = 40;
  Index nfchav = 40;
  Index nfchax = 48;
  Index nchau = 800;
  Index nchav = 800;
  Index nchaz = 480;
  Index nchax = 2*nchaz;
  Index nchai = nchau + nchav;
  Index apaIdx[ntps] = {1, 3, 2, 4};  // Installation order.
  Index apaIsUpper[ntps] = {1, 0, 1, 0};
  string slocs[ntps] = {"P02SU", "P02NL", "P01SU", "P01NL"};
  insertLen("all", 0, ntps*nchaApa, "All", "", "");
  for ( Index itps=0; itps<ntps; ++itps ) {
    string sitps = std::to_string(itps);
    string siapa = std::to_string(apaIdx[itps]);
    string labTps = "TPC set " + sitps;
    string labApa = "APA " + siapa;
    string sloc = slocs[itps];
    Index ch0 = itps*nchaApa;
    string stps = "tps" + sitps;
    string sapa = "apa" + siapa;
    insertLen(      stps, ch0, nchaApa, labTps, sloc, labApa);
    insertLen(      sapa, ch0, nchaApa, labApa, sloc);
    string stpp = "tpp" + sitps;
    Index chu0 = ch0;
    Index chv0 = chu0 + nchau;
    Index chx10 = chv0 + nchav;
    Index chx20 = chx10 + nchaz;
    Index chx0 = chx10;
    bool beamRight = 2*(itps/2) == itps;
    Index chz0 = beamRight ? chx20 : chx10;
    Index chc0 = beamRight ? chx10 : chx20;
    insertLen(stpp + "u", chu0, nchau, "TPC plane " + sitps + "u", sloc + "-U", labApa + "u");
    insertLen(sapa + "u", chu0, nchau, "APA plane " + siapa + "u", sloc + "-U");
    insertLen(stpp + "v", chv0, nchav, "TPC plane " + sitps + "v", sloc + "-V", labApa + "v");
    insertLen(sapa + "v", chv0, nchav, "APA plane " + siapa + "v", sloc + "-V");
    insertLen(stpp + "c", chc0, nchaz, "TPC plane " + sitps + "c", sloc + "-C", labApa + "c");
    insertLen(sapa + "c", chc0, nchaz, "APA plane " + siapa + "c", sloc + "-C");
    insertLen(stpp + "z", chz0, nchaz, "TPC plane " + sitps + "z", sloc + "-Z", labApa + "z");
    insertLen(sapa + "z", chz0, nchaz, "APA plane " + siapa + "z", sloc + "-Z");
    insertLen(stpp + "i", chu0, nchai, "TPC plane " + sitps + "i", sloc + "-I", labApa + "i");
    insertLen(sapa + "i", chu0, nchai, "APA plane " + siapa + "i", sloc + "-I");
    insertLen(stpp + "x", chx0, nchax, "TPC plane " + sitps + "x", sloc + "-X", labApa + "x");
    insertLen(sapa + "x", chx0, nchax, "APA plane " + siapa + "x", sloc + "-X");
    // Loop over FEMBS in offline order.
    // See https://wiki.dunescience.org/wiki/ProtoDUNE-HD_Geometry.
    bool isUpper = apaIsUpper[itps];
    bool isLower = ! isUpper;
    Index fchu0 = chu0 + 3;
    Index fchv0 = chv0 + 3;
    Index fchx0 = chx10;
    Index ifmbu = 10;
    Index ifmbv =  1;
    Index ifmbx =  1;
    Index chumax = chu0 + nchau;
    Index chvmax = chv0 + nchav;
    if ( isLower ) {
      fchu0 = chu0 + 745;
      fchv0 = chv0 + 745;
      ifmbu = 11;
      ifmbv = 20;
      ifmbx = 20;
    }
    for ( Index ifmbOff=0; ifmbOff<20; ++ifmbOff ) {
      // One FEMB in each of u and v has twu channel ranges.
      // // U plane
      bool splitu = fchu0 + nfchau > chumax;
      ostringstream ssnamu;
      ssnamu << siapa << setfill('0') << setw(2) << ifmbu << "u";
      string namu = ssnamu.str();
      ostringstream ssnamu2;
      ssnamu2 << ifmbu << "u";
      string namu2 = ssnamu2.str();
      string flab = "FEMB-view ";
      string lab1 = flab + namu;
      string lab2 = sloc + " " + flab + namu2;
      string suf1 = "1";
      string suf2 = "2";
      if ( splitu ) {
        insertLen("femb" + namu + suf1, fchu0, chumax-fchu0, lab1 + suf1, lab2 + suf1);
        fchu0 += nfchau;
        fchu0 -= nchau;
        insertLen("femb" + namu + suf2, chu0, fchu0-chu0,    lab1 + suf2, lab2 + suf2);
      } else {
        insertLen("femb" + namu, fchu0, nfchau, lab1, lab2);
        fchu0 += nfchau;
      }
      // V plane
      bool splitv = fchv0 + nfchav > chvmax;
      ostringstream ssnamv;
      ssnamv << siapa << setfill('0') << setw(2) << ifmbv << "v";
      string namv = ssnamv.str();
      ostringstream ssnamv2;
      ssnamv2 << ifmbv << "v";
      string namv2 = ssnamv2.str();
      lab1 = flab + namv;
      lab2 = sloc + " " + flab + namv2;
      if ( splitv ) {
        string suf = "1";
        insertLen("femb" + namv + suf1, fchv0, chvmax-fchv0, lab1 + suf1, lab2 + suf1);
        fchv0 += nfchav;
        fchv0 -= nchav;
        suf = "2";
        insertLen("femb" + namv + suf2, chv0, fchv0-chv0,    lab1 + suf2, lab2 + suf2);
      } else {
        insertLen("femb" + namv, fchv0, nfchav, flab + namv, sloc + " " + flab + namv2);
        fchv0 += nfchav;
      }
      // X plane
      ostringstream ssnamx;
      ssnamx << siapa << setfill('0') << setw(2) << ifmbx << "x";
      string namx = ssnamx.str();
      ostringstream ssnamx2;
      ssnamx2 << ifmbu << "x";
      string namx2 = ssnamx2.str();
      insertLen("femb" + namx, fchx0, nfchax, flab + namx, sloc + " " + flab + namx2);
      fchx0 += nfchax;
      if ( isUpper ) {
        ifmbu -= 1; 
        if ( ifmbu == 0 ) ifmbu = 20;
        ifmbv += 1;
        if ( ifmbOff < 9 )       ifmbx += 1;
        else if ( ifmbOff == 9 ) ifmbx = 20;
        else                     ifmbx -= 1;
      } else {
        ifmbu += 1; 
        if ( ifmbu > 20 ) ifmbu = 1;
        ifmbv -= 1;
        if ( ifmbOff < 9 )       ifmbx -= 1;
        else if ( ifmbOff == 9 ) ifmbx = 1;
        else                     ifmbx += 1;
      }
    }
  }
  if ( m_ExtraRanges.size() ) {
    DuneToolManager* ptm = DuneToolManager::instance();
    m_pExtraRanges = ptm->getShared<IndexRangeTool>(m_ExtraRanges);
    if ( m_pExtraRanges == nullptr ) {
      cout << myname << "WARNING: Extra range tool not found: " << m_ExtraRanges << endl;
    }
  }
  if ( m_LogLevel >= 1 ) {
    cout << myname << "          LogLevel: " << m_LogLevel << endl;
    cout << myname << "  Extra range tool: " << m_ExtraRanges << endl;
  }
}

//**********************************************************************

IndexRange PdhdChannelRanges::get(Name nam) const {
  const Name myname = "PdhdChannelRanges::runData: ";
  if ( m_pExtraRanges != nullptr ) {
    IndexRange rout = m_pExtraRanges->get(nam);
    if ( rout.isValid() ) return rout;
  }
  IndexRangeMap::const_iterator iran = m_Ranges.find(nam);
  if ( iran == m_Ranges.end() ) return IndexRange();
  return iran->second;
}

//**********************************************************************

void PdhdChannelRanges::
insertLen(Name nam, Index begin, Index len, Name lab, Name lab1, Name lab2) {
  string myname = "PdhdChannelRanges::insertLen: ";
  m_Ranges[nam] = IndexRange(nam, begin, begin+len, lab, lab1, lab2);
  if ( m_LogLevel >= 2 ) cout << myname << m_Ranges[nam] << endl;
}

//**********************************************************************

DEFINE_ART_CLASS_TOOL(PdhdChannelRanges)
