// PdhdChannelGroups.h
//
// David Adams
// August 2023
//
// Tool to return channel range groups for the 2023 DUNE horizontal-drift prototype
// aka ProtoDUNE-HD and pdhd.
//
// In addition to all the single ranges provided by PdhdChannelRanges, this tool adds
//   tppVs - All TPC planes with orientation V = {"z", "c", "x", "u", "v", "i"}
//   apaVs - All APA planes with orientation V
//   fembAFFV - FEMB AFF orientation V for split FEMB-ranges, eg femb211u.
//   fembAFF - FEMB AFF
//
// Configuration parameters:
//   LogLevel: Logging level (0=none, 1=ctor, 2=every call)
//   IndexRangeTool: Tool that maps names here to channel ranges.

#ifndef PdhdChannelGroups_H
#define PdhdChannelGroups_H

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "dunecore/DuneInterface/Tool/IndexRangeGroupTool.h"
#include <map>

class IndexRangeTool;

class PdhdChannelGroups : public IndexRangeGroupTool {

public:

  using Name = std::string;
  using NameVector = std::vector<Name>;
  using Index = IndexRangeGroup::Index;
  using GroupMap = std::map<Name, NameVector>;

  // Ctor.
  PdhdChannelGroups(fhicl::ParameterSet const& ps);

  // Dtor.
  ~PdhdChannelGroups() override =default;

  // Return a range.
  IndexRangeGroup get(Name nam) const override;

private:

  // Configuration parameters.
  Index m_LogLevel;
  Name m_IndexRangeTool;

  // Derived from configuration.
  const IndexRangeTool* m_pIndexRangeTool =nullptr;
  GroupMap m_groups;
  GroupMap m_labels;

};


#endif
