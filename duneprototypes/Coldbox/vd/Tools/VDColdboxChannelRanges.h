#ifndef VDColdboxChannelRanges_H
#define VDColdboxChannelRanges_H

// Define channel ranges for the November 2021 vertical drift test in the
// CERN cold box.
//
// cru = all channels
// crt, crb = bottom and top
// cr[b,t][u,y,z] for each view, e.g. crbz
// crbg for the ghost (unused) bottom channels
//
// Configuration:
//   LogLevel: Message level (0, none, 1 init, ...)
//   GhostRange: [] = no ghosts
//               [MIN, MAX] means channels MIN, MIN+1, ..., MAX

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "dune/DuneInterface/Tool/IndexRangeTool.h"
#include <map>

class VDColdboxChannelRanges : public IndexRangeTool {

public:

  using Index = IndexRange::Index;
  using IndexVector = std::vector<Index>;

  // Ctor.
  VDColdboxChannelRanges(fhicl::ParameterSet const& ps);

  // Dtor.
  ~VDColdboxChannelRanges() override =default;

  // Return a range.
  IndexRange get(std::string range_name) const override;

private:

  // Configuration parameters.
  int m_LogLevel;
  IndexVector m_GhostRange;

  // Derived parameters.
  Index m_glo;
  Index m_ghi;

  using RangeMap = std::map<Name, IndexRange>;
  RangeMap m_rans;

  void insert(Name sran, Index ich1, Index ich2, Name slab1);

  void assignFembRanges();

};
#endif
