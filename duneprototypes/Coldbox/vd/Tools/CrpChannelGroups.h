// CrpChannelGroups.h
//
// David Adams
// November 2022
//
// Returns groups for the vertical drift CRPs used in the late 2022
// coldbox tests (CRP2+), ProtoDUNE-VD and maybe FD-VD.
// All ranges from CrpChannelRanges are included and teh following are added:
//    fembTFF - FEMB FF for volume T bottom
// Indices ar as for CrpChannelRanges:
//   T = TPC label: C for coldbox, {A,B} for ProtoDUNE, presumably {00, 01, ..., ??} for DUNE FD2.
//  FF = FEMB number in range [00, 15]

#ifndef CrpChannelGroups_H
#define CrpChannelGroups_H

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "dunecore/DuneInterface/Tool/IndexRangeGroupTool.h"
#include <map>

class IndexRangeTool;

class CrpChannelGroups : public IndexRangeGroupTool {

public:

  // Ctor.
  CrpChannelGroups(fhicl::ParameterSet const& ps);

  // Dtor.
  ~CrpChannelGroups() override =default;

  // Return a range.
  IndexRangeGroup get(std::string nam) const override;

private:

  // Configuration parameters.
  int m_LogLevel;

  // Derived paramters.
  IndexRangeTool* m_pcrt =nullptr;
  using GroupMap = std::map<Name, IndexRangeGroup>;
  GroupMap m_grps;

  // Create a group from range names.
  using Name = std::string;
  using NameVector = std::vector<Name>;
  IndexRangeGroup makeGroup(Name nam, NameVector rnams, Name lab, bool ignoreBadRanges =false);

};


#endif
