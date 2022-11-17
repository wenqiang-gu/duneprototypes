// VDColdboxChannelGroups.h
//
// Returns groups for the vertical drift coldbox test in 2021.
// At present, single ranges from channelranges are returned.

#ifndef VDColdboxChannelGroups_H
#define VDColdboxChannelGroups_H

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "dunecore/DuneInterface/Tool/IndexRangeGroupTool.h"
#include <map>

class IndexRangeTool;

class VDColdboxChannelGroups : public IndexRangeGroupTool {

public:

  // Ctor.
  VDColdboxChannelGroups(fhicl::ParameterSet const& ps);

  // Dtor.
  ~VDColdboxChannelGroups() override =default;

  // Return a range.
  IndexRangeGroup get(std::string nam) const override;

private:

  // Configuration parameters.
  int m_LogLevel;

  // Derived paramters.
  IndexRangeTool* m_pcrt =nullptr;

  // Create a group from range names.
  using Name = std::string;
  using NameVector = std::vector<Name>;
  IndexRangeGroup makeGroup(Name nam, NameVector rnams, Name lab) const;

};


#endif
