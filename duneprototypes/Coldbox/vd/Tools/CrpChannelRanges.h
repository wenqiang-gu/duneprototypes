#ifndef CrpChannelRanges_H
#define CrpChannelRanges_H

// Define channel ranges for the vertical-drift CRUs used for late 2022
// coldbox testing (CRP2+) and expected for ProtoDUNE-2V.
// Also may be used for DUNE FD2?
//
// crdet = all channels
//   crE = bottom and top all TPCs
//  crET = bottom or top of each TPC
// crETP = each view in bottom or tofor each view, e.g. crbAz
//   E = detector end: b for bottom, t for top or u for coldbox
//   T = TPC label: C for coldbox, {A,B} for ProtoDUNE, presumably {00, 01, ..., ??} for DUNE FD2.
//   P = plane: u, v, z
//
// For now we hardwire the current (Nov 2022) DUNE layout and offline ordering:
//   U -  952 strips: channel number is offset + [   0,  951]
//   V -  952 strips: channel number is offset + [ 952, 1903]
//   Z - 1168 strips: channel number is offset + [1904, 3071]
// for both top and bottom.
//
// Configuration:
//   LogLevel: Message level (0, none, 1 init, ...)
//   Detector: cb2022 or pdvd
//   ....

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include <map>

class CrpChannelRanges : public IndexRangeTool {

public:

  using Index = IndexRange::Index;
  using IndexVector = std::vector<Index>;
  using Name = std::string;

  // Ctor.
  CrpChannelRanges(fhicl::ParameterSet const& ps);

  // Dtor.
  ~CrpChannelRanges() override =default;

  // Return a range.
  IndexRange get(std::string range_name) const override;

private:

  // Configuration parameters.
  int m_LogLevel;
  Name m_Detector;   // cb2022, pdvd

  using RangeMap = std::map<Name, IndexRange>;
  RangeMap m_rans;

  void insert(Name sran, Index ich1, Index ich2, Name slab1);

  void assignFembRanges();

};
#endif
