// VDColdboxOnlineChannel.h
//
// David Adams
// November 2021
//
// Tool that converts a VD coldbox offline channel number
// to an online index following the FEMB convention:
//
//   chanOn = 128*KFMB + KFCH
//   KFCH 8*KASIC + KACH
//
//  KFMB = FEMB number (1-14 in VD coldbox 2021)
//  KFCH = Channel number in the FEMB (0-127)
//  KASIC = ASCIC number in the FEMB (0-7)
//  KACH = Channel number in the ASIC (0-15)
//
// Configuration parameters:
//   LogLevel - 0=silent, 1=init messages, 2=message every call

#ifndef VDColdboxOnlineChannel_H
#define VDColdboxOnlineChannel_H

#include "art/Utilities/ToolMacros.h"
#include "fhiclcpp/ParameterSet.h"
#include "dune/DuneInterface/Tool/IndexMapTool.h"

class VDColdboxOnlineChannel : public IndexMapTool {

public:

  using Name = std::string;

  VDColdboxOnlineChannel(const fhicl::ParameterSet& ps);

  Index get(Index chanOff) const override;

private:

  // Configuration parameters.
  Index m_LogLevel;

};


#endif
