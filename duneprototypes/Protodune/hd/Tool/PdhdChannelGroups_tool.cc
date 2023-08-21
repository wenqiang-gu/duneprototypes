// PdhdChannelGroups_tool.cc

#include "PdhdChannelGroups.h"
#include "dunecore/ArtSupport/DuneToolManager.h"
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
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
using Name = PdhdChannelGroups::Name;
using NameVector = std::vector<Name>;
using Index = PdhdChannelGroups::Index;
using IndexVector = std::vector<Index>;

//**********************************************************************

PdhdChannelGroups::PdhdChannelGroups(fhicl::ParameterSet const& ps)
: m_LogLevel(ps.get<Index>("LogLevel")),
  m_IndexRangeTool(ps.get<Name>("IndexRangeTool")) {
  const Name myname = "PdhdChannelGroups::ctor: ";
  if ( m_IndexRangeTool.size() ) {
    DuneToolManager* ptm = DuneToolManager::instance();
    m_pIndexRangeTool = ptm->getShared<IndexRangeTool>(m_IndexRangeTool);
    if ( m_pIndexRangeTool == nullptr ) {
      cout << myname << "WARNING: Index range tool not found: " << m_IndexRangeTool << endl;
    }
  } else {
    cout << myname << "WARNING: No Index range tool name." << endl;
  }
  Index napa = 4;
  m_labels["tpss"].push_back("TPC sets");
  m_labels["apas"].push_back("APAs");
  NameVector soris = {"z", "c", "x", "u", "v", "i"};
  for ( Name sori : soris ) {
    Name soriup = sori;
    soriup[0] = std::toupper(soriup[0]);
    m_labels["tpp" + sori + "s"].push_back(soriup + " planes");
    m_labels["apa" + sori + "s"].push_back(soriup + " planes");
  }
  for ( Index itps=0; itps<napa; ++itps ) {
    ostringstream sstps;
    sstps << itps;
    Name stps = sstps.str();
    Index iapa = itps + 1;
    ostringstream ssapa;
    ssapa << iapa;
    Name sapa = ssapa.str();
    m_groups["tpss"].push_back("tps" + stps);
    m_groups["apas"].push_back("apa" + sapa);
    for ( Name sori : soris ) {
      m_groups["tpp" + sori + "s"].push_back("tpp" + stps + sori);
      m_groups["apa" + sori + "s"].push_back("apa" + sapa + sori);
    }
  }
  Index nfmb = 20;
  for ( Index iapa=1; iapa<=napa; ++iapa ) {
    for ( Index ifmb=1; ifmb<=nfmb; ++ifmb ) {
      ostringstream ssgrp;
      ssgrp << "femb" << iapa;
      if ( ifmb < 10 ) ssgrp << "0";
      ssgrp << ifmb;
      Name sgrp = ssgrp.str();
      for ( Name sori : {"u", "v", "x"} ) {
        Name sran = sgrp + sori;
        IndexRange ran = m_pIndexRangeTool->get(sran);
        if ( ran.isValid() ) {
          m_groups[sgrp].push_back(sran);
        } else {
          // This must be a split FEMB: sran -> {sra1, sran2}
          NameVector sranxs({sran + "1", sran + "2"});
          NameVector split_labs;
          Index nerr = 0;
          for ( Name sranx : sranxs ) {
            IndexRange ranx = m_pIndexRangeTool->get(sranx);
            if ( ! ranx.isValid() ) {
              cout << myname << "ERROR: Unable to find split range " << sranx << "." << endl;
              ++nerr;
            }
            if ( split_labs.size() == 0 ) {
              for ( Name slab : ranx.labels ) {
                split_labs.push_back(slab.substr(0, slab.size()-1));
              }
            }
          }
          if ( nerr ) continue;
          m_groups[sran] = sranxs;
	  m_labels[sran] = split_labs;
          m_groups[sgrp].insert(m_groups[sgrp].end(), sranxs.begin(), sranxs.end());
        }
      }
      Name slab = "FEMB " + sgrp.substr(4);
      m_labels[sgrp].push_back(slab);
    }
  }
  if ( m_LogLevel >= 1 ) {
    cout << myname << "           LogLevel: " << m_LogLevel << endl;
    cout << myname << "     IndexRangeTool: " << m_LogLevel << endl;
  }
  if ( m_LogLevel >= 2 ) {
    cout << myname << "There are " << m_groups.size() << " channel groups:" << endl;
    for ( const GroupMap::value_type& igrp : m_groups ) {
      if ( true ) {
        Name sgrp = igrp.first;
        IndexRangeGroup grp = get(sgrp);
        if ( grp.isValid() ) cout << myname << grp << endl;
        else                 cout << myname << "ERROR: Group " << sgrp << " is not defined." << endl;
      } else {
        cout << myname << setw(10) << igrp.first << ":";
        for ( string sgrp : igrp.second ) {
          cout << setw(10) << sgrp;
        }
        cout << endl;
      }
    }
  }
}

//**********************************************************************

IndexRangeGroup PdhdChannelGroups::get(Name nam) const {
  const Name myname = "PdhdChannelGroups::get: ";
  if ( m_pIndexRangeTool == nullptr ) {
    if ( m_LogLevel >= 2 ) cout << myname << "No IndexRangeTool." << endl;
    return IndexRangeGroup();
  }
  GroupMap::const_iterator igrp = m_groups.find(nam);
  if ( igrp == m_groups.end() ) {
    if ( m_LogLevel >= 2 ) cout << myname << "Invalid group name: " << nam << endl;
    return IndexRangeGroup();
  }
  IndexRangeGroup::RangeVector rans;
  for ( Name rnam : igrp->second ) {
    IndexRange ran = m_pIndexRangeTool->get(rnam);
    if ( ran.isValid() ) {
      rans.push_back(ran);
    } else {
      cout << myname << "WARNING: Unable to find range " << rnam << " for group " << nam << "." << endl;
    }
  }
  NameVector labs;
  GroupMap::const_iterator ilabs = m_labels.find(nam);
  if ( ilabs == m_labels.end() ) return IndexRangeGroup(nam, rans);
  return IndexRangeGroup(nam, ilabs->second, rans);
}

//**********************************************************************

DEFINE_ART_CLASS_TOOL(PdhdChannelGroups)
