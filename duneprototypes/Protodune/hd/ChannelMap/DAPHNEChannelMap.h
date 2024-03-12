#ifndef DAPHNEChannelMap_H
#define DAPHNEChannelMap_H

#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

namespace dune {
  class DAPHNEChannelMap;
}

class dune::DAPHNEChannelMap {

public:
  /*struct DAPHNEChanInfo {
    unsigned int offlchan;        // in gdml and channel sorting convention
    unsigned int crate;           // crate number
    unsigned int slot;            // slot number
    std::string APAName;          // Descriptive APA name
    unsigned int link;            // link identifier: 0, 1, 2, 3 
    unsigned int framechan;       // channel index in DAPHNE frame.
  }*/

  DAPHNEChannelMap();  // constructor
  DAPHNEChannelMap(bool ignore_links=false);  // constructor
  void ReadMapFromFile(std::string &fullname);
  unsigned int GetOfflineChannel(unsigned int slot, unsigned int link,
                                 unsigned int frame_chan) const;

private:
  //Number of channels:
  //    4 Channels/Module x 10 Modules/APA x 4 APAs/PDHD
  const unsigned int fNChans = 4*10*4;

  std::unordered_map<unsigned int,   // Slot/Endpoint
    std::unordered_map<unsigned int, // Link
    std::unordered_map<unsigned int, // Frame Channel
    unsigned int>>> fMapToOfflineChannel;

  void check_offline_channel(unsigned int offlineChannel) const {
    if (offlineChannel >= fNChans) {
      throw std::range_error(
        "DAPHNEChannelMap offline Channel out of range");
    }
  };

  bool fIgnoreLinks;
};
#endif
