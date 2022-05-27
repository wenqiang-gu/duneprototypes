#include<unordered_map>
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>

// wibframechan = cebchan + 128*femb_on_link

 typedef struct HDChanInfo {
   unsigned int offlchan;        // in gdml and channel sorting convention
   unsigned int crate;           // crate number
   std::string APAName;          // Descriptive APA name
   unsigned int wib;             // 1, 2, 3, 4 or 5  (slot number +1?)
   unsigned int link;            // link identifier: 0 or 1
   unsigned int femb_on_link;    // which of two FEMBs in the WIB frame this FEMB is:  0 or 1
   unsigned int cebchan;         // cold electronics channel on FEMB:  0 to 127
   unsigned int plane;           // 0: U,  1: V,  2: X
   unsigned int chan_in_plane;   // which channel this is in the plane in the FEMB:  0:39 for U and V, 0:47 for X
   unsigned int femb;            // which FEMB on an APA -- 1 to 20
   unsigned int asic;            // ASIC:   1 to 8
   unsigned int asicchan;        // ASIC channel:  0 to 15
   unsigned int wibframechan;    // channel index in WIB frame.  0:255
   bool valid;          // true if valid, false if not
 } HDChanInfo_t;

  const int fNChans = 2560*4;

  // map of crate, slot, link, femb_on_link, plane, chan to channel info struct

  std::unordered_map<unsigned int,   // crate
    std::unordered_map<unsigned int, // wib
    std::unordered_map<unsigned int, // link
    std::unordered_map<unsigned int, // wibframechan
    HDChanInfo_t > > > > DetToChanInfo;

  // map of chan info indexed by offline channel number

  std::unordered_map<unsigned int, HDChanInfo_t> OfflToChanInfo;

   HDChanInfo_t GetChanInfoFromOfflChan(unsigned int offlchan);

 HDChanInfo_t GetChanInfoFromDetectorElements(unsigned int crate, unsigned int slot, unsigned int link, unsigned int femb_on_link, unsigned int plane, unsigned int chan_in_plane);

 HDChanInfo_t GetChanInfoFromWIBElements(unsigned int crate, unsigned int slot, unsigned int link, unsigned int wibframechan);

int main(int argc, char **argv)
{
  std::string channelMapFile("PD2HDChannelMap_v1.txt");

  std::ifstream inFile(channelMapFile, std::ios::in);
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

    // calculate this as it wasn't in the original spec

    chanInfo.wibframechan = chanInfo.chan_in_plane + 128*chanInfo.femb_on_link;
    if (chanInfo.plane == 1) chanInfo.wibframechan += 40;
    else if (chanInfo.plane == 2) chanInfo.wibframechan += 80;
    else if (chanInfo.plane != 0)
      {
	std::cout << "bad plane: " << chanInfo.plane << std::endl;
	return 1;
      }
    chanInfo.valid = true;

    // fill maps.

    if (chanInfo.offlchan >= fNChans)
      {
        std::cout << "Ununderstood Offline Channel: " << chanInfo.offlchan << "\n";
        return 1;
      }


    DetToChanInfo[chanInfo.crate][chanInfo.wib][chanInfo.link][chanInfo.wibframechan] = chanInfo;
    OfflToChanInfo[chanInfo.offlchan] = chanInfo;

  }
  inFile.close();

  // do circularity test

  for (unsigned int ochan=0; ochan<fNChans; ++ochan)
    {
      HDChanInfo_t ci = GetChanInfoFromOfflChan(ochan);
      HDChanInfo_t ci2 = GetChanInfoFromDetectorElements(ci.crate, ci.wib-1, ci.link, ci.femb_on_link, ci.plane, ci.chan_in_plane);
      if (ci.offlchan != ci2.offlchan || ci.offlchan != ochan || !ci.valid || !ci2.valid)
        {
          std::cout << "chan compare: " << ochan << " " << ci.offlchan << " " << ci2.offlchan << " " << ci.valid << " " << ci2.valid << std::endl;
        }
    }
}


HDChanInfo_t GetChanInfoFromDetectorElements(unsigned int crate, unsigned int slot, unsigned int link, unsigned int femb_on_link, unsigned int plane, unsigned int chan_in_plane ) {

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

HDChanInfo_t GetChanInfoFromWIBElements(unsigned int crate, unsigned int slot, unsigned int link, unsigned int wibframechan ) {

  HDChanInfo_t badInfo = {};
  badInfo.valid = false;

  unsigned int wib = slot + 1;

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

HDChanInfo_t GetChanInfoFromOfflChan(unsigned int offlineChannel) {
  auto ci = OfflToChanInfo.find(offlineChannel);
  if (ci == OfflToChanInfo.end()) 
    {
      HDChanInfo_t badInfo = {};
      badInfo.valid = false;
      return badInfo;
    }
  return ci->second;
}
