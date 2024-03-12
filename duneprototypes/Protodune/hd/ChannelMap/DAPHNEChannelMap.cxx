#include "DAPHNEChannelMap.h"
#include <iostream>
#include <fstream>
#include <sstream>

dune::DAPHNEChannelMap::DAPHNEChannelMap() : fIgnoreLinks(false) {}
dune::DAPHNEChannelMap::DAPHNEChannelMap(bool ignore_links)
  : fIgnoreLinks(ignore_links) {}

void dune::DAPHNEChannelMap::ReadMapFromFile(std::string &fullname) {
  std::ifstream inFile(fullname, std::ios::in);
  std::string line;

  while (std::getline(inFile,line)) {
    std::stringstream linestream(line);

    unsigned int slot, link, daphne_channel, offline_channel;
    linestream 
      >> slot 
      >> link 
      >> daphne_channel 
      >> offline_channel;

    // fill maps.
    check_offline_channel(offline_channel);
    fMapToOfflineChannel[slot][link][daphne_channel] = offline_channel;
    //OfflToChanInfo[chanInfo.offlchan] = chanInfo;

  }
  inFile.close();
}

unsigned int dune::DAPHNEChannelMap::GetOfflineChannel(
    unsigned int slot, unsigned int link, unsigned int daphne_channel) const {

  if (fIgnoreLinks) link = 0;
  std::string err = "DAPHNEChannelMap -- Could not find ";
  if (fMapToOfflineChannel.find(slot) == fMapToOfflineChannel.end()) {
    err += "slot " + std::to_string(slot);
    throw std::range_error(err);
  }

  auto link_map = fMapToOfflineChannel.at(slot);
  if (link_map.find(link) == link_map.end()) {
    err += "link " + std::to_string(link);
    throw std::range_error(err);
  }

  auto daphne_map = link_map.at(link);
  if (daphne_map.find(daphne_channel) == daphne_map.end()) {
    err += "daphne_channel " + std::to_string(daphne_channel);
    err += ("\nfor link " + std::to_string(link) + " and slot " +
            std::to_string(slot));
    throw std::range_error(err);
  }

  return daphne_map.at(daphne_channel);
}
