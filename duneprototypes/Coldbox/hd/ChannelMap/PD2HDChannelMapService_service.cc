///////////////////////////////////////////////////////////////////////////////////////////////////
// Class:       PD2HDChannelMapService
// Module type: service
// File:        PD2HDChannelMapService.h
// Author:      Tom Junk, May 2022
//
// Implementation of hardware-offline channel mapping reading from a file.
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "PD2HDChannelMapService.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

dune::PD2HDChannelMapService::PD2HDChannelMapService(fhicl::ParameterSet const& pset) {

  std::string channelMapFile = pset.get<std::string>("FileName");

  std::string fullname;
  cet::search_path sp("FW_SEARCH_PATH");
  sp.find_file(channelMapFile, fullname);

  if (fullname.empty()) {
    std::cout << "Input file " << channelMapFile << " not found" << std::endl;
    throw cet::exception("File not found");
  }
  else
    std::cout << "PD2HD Channel Map: Building TPC wiremap from file " << channelMapFile << std::endl;

  std::ifstream inFile(fullname, std::ios::in);
  std::string line;

  while (std::getline(inFile,line)) {
    std::stringstream linestream(line);

    HDChanInfo_t chanInfo;
    linestream 
      >> chanInfo.offlchan 
      >> chanInfo.crate 
      >> chanInfo.APAName
      >> chanInfo.wib 
      >> chanInfo.link 
      >> chanInfo.femb_on_link 
      >> chanInfo.cebchan 
      >> chanInfo.plane 
      >> chanInfo.chan_in_plane 
      >> chanInfo.femb 
      >> chanInfo.asic 
      >> chanInfo.asicchan; 

    // calculate wibframechan as it wasn't in the original spec

    chanInfo.wibframechan = chanInfo.chan_in_plane + 128*chanInfo.femb_on_link;
    if (chanInfo.plane == 1) chanInfo.wibframechan += 40;
    else if (chanInfo.plane == 2) chanInfo.wibframechan += 80;
    else if (chanInfo.plane != 0)
      {
	throw cet::exception("PD2HDChannelMapService") << "Bad plane ID in input file: " << chanInfo.plane << std::endl;
      }
    chanInfo.valid = true;

    // fill maps.

    check_offline_channel(chanInfo.offlchan);

    DetToChanInfo[chanInfo.crate][chanInfo.wib][chanInfo.link][chanInfo.wibframechan] = chanInfo;
    OfflToChanInfo[chanInfo.offlchan] = chanInfo;

  }
  inFile.close();

}

dune::PD2HDChannelMapService::PD2HDChannelMapService(fhicl::ParameterSet const& pset, art::ActivityRegistry&) : PD2HDChannelMapService(pset) {
}


dune::PD2HDChannelMapService::HDChanInfo_t dune::PD2HDChannelMapService::GetChanInfoFromDetectorElements(
    unsigned int crate,
    unsigned int slot,
    unsigned int link,
    unsigned int femb_on_link,
    unsigned int plane,
    unsigned int chan_in_plane ) const {

  unsigned int wibframechan = 128*femb_on_link + chan_in_plane;
  if (plane == 1) wibframechan += 40;
  else if (plane == 2) wibframechan += 80;
  else if (plane != 0)
    {
      HDChanInfo_t badInfo = {};
      badInfo.valid = false;
      return badInfo;
    }

  return GetChanInfoFromWIBElements(crate,slot,link,wibframechan);
}

dune::PD2HDChannelMapService::HDChanInfo_t dune::PD2HDChannelMapService::GetChanInfoFromWIBElements(
    unsigned int crate,
    unsigned int slot,
    unsigned int link,
    unsigned int wibframechan ) const {

  unsigned int wib = slot + 1;

  HDChanInfo_t badInfo = {};
  badInfo.valid = false;

  auto fm1 = DetToChanInfo.find(crate);
  if (fm1 == DetToChanInfo.end()) return badInfo;
  auto& m1 = fm1->second;

  auto fm2 = m1.find(wib);
  if (fm2 == m1.end()) return badInfo;
  auto& m2 = fm2->second;

  auto fm3 = m2.find(link);
  if (fm3 == m2.end()) return badInfo;
  auto& m3 = fm3->second;

  auto fm4 = m3.find(wibframechan);
  if (fm4 == m3.end()) return badInfo;
  return fm4->second;
}


dune::PD2HDChannelMapService::HDChanInfo_t dune::PD2HDChannelMapService::GetChanInfoFromOfflChan(unsigned int offlineChannel) const {
  auto ci = OfflToChanInfo.find(offlineChannel);
  if (ci == OfflToChanInfo.end()) 
    {
      HDChanInfo_t badInfo = {};
      badInfo.valid = false;
      return badInfo;
    }
  return ci->second;

}


DEFINE_ART_SERVICE(dune::PD2HDChannelMapService)
