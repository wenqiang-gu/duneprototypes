// PdhdChannelRanges.h
//
// David Adams
// July 2023
//
// Tool to return channel ranges for ProtoDUNE-HD, the 2023 HD DUNE prototype.
// The geometry and channel numbering are described here:
//   https://wiki.dunescience.org/wiki/ProtoDUNE-HD_Geometry
// The following ranges are created:
//       all - all channels
//      tpsS - for TPC set S, e.g. tps0
//     tppSP - TPC plane or plane pair P in TPC set S, e.g. tps0z
//      apaA - APA, e.g. apa3
//  fembAFFV - FEMB AFF orientation x, eg femb302x
//
// In the above
//    S is the TPC set (offline APA) number in range [0,3]
//    A is the APA number in range [1,4]
//    P is the wire plane: u, v, z (TPC-side collection) or c (cryostat-side collection)
//      or the wire plane pair x for (z, c) or i for (u, v)
//   FF is the FEMB number in an APA in range [01,20]
//    V is a wire orientation: u, v or x (collection)
//
// Where relevant, the ranges are assigned a second label location as one of:
//   P01SU, P02SU, P01NL, P02NL
//
// Note there is the option to append another index range tool which can
// override or extend the above set of ranges.
//
// The TPC set numbering follows the convention:
//
//  --->   TPS1  TPS3
//  beam   TPS0  TPS2
//
// and APA numbering is
//
//  --->   APA3  APA4
//  beam   APA1  APA2
//
// both viewed from above.
//
// Parameters:
//   LogLevel - Message logging level
//              0 = none
//              1 = display config
//              2 = Display each channel range
//   ExtraRanges - Name of tool with additional ranges. Blank for none.

#ifndef PdhdChannelRanges_H
#define PdhdChannelRanges_H

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "dunecore/DuneInterface/Tool/IndexRangeTool.h"
#include <map>

class PdhdChannelRanges : public IndexRangeTool {

public:

  using Name = std::string;
  using Index = IndexRange::Index;
  using IndexRangeMap = std::map<Name, IndexRange>;

  // Ctor.
  PdhdChannelRanges(fhicl::ParameterSet const& ps);

  // Dtor.
  ~PdhdChannelRanges() override =default;

  // Return a range.
  IndexRange get(Name nam) const override;

private:

  // Configuration parameters.
  Index m_LogLevel;
  Name m_ExtraRanges;

  IndexRangeMap m_Ranges;
  const IndexRangeTool* m_pExtraRanges =nullptr;

  // Add an entry to the range map.
  void insertLen(Name nam, Index begin, Index len, Name lab, Name lab1 ="", Name lab2 ="");

};


#endif
