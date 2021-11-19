// Prototype program to try out the VD coldbox channel mapping.  Components to be made
// into a service
// compile with clang++ --std=c++17 -o maptest maptest.C
// takes one argument on the command line:  chan offset -- 3200.  Put in -1 to test channel map,
// -2 to test specific channels

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

struct VDCBChanInfo {
  int offlchan;        // in gdml and channel sorting convention
  int wib;             // slot number +1:    1, 2, 3, or 4
  int wibconnector;    // which board on the WIB  1, 2, 3, or 4
  int cebchan;         // cold electronics channel on board:  0 to 127
  int femb;            // FEMB ID:  1 to 14
  int asic;            // ASIC:   1 to 8
  int asicchan;        // ASIC channel:  0 to 15
  int connector;       // detector connector
  std::string stripid;  // strip name, with plane and number.  e.g. U79
  bool valid;          // true if valid, false if not
};

//  map so we can look up channel info by offline channel
std::unordered_map<int,VDCBChanInfo> chantoinfomap;
std::unordered_map<int,std::unordered_map<int,std::unordered_map<int,int> > > infotochanmap;

// function prototypes

VDCBChanInfo getChanInfoFromOfflChan(int offlchan);

// this uses conventions from Nitish's spreadsheet.  WIB: 1-3, wibconnector: 1-4, cechan: 0-127
int getOfflChanFromWIBConnectorInfo(int wib, int wibconnector, int cechan);

// this uses conventions from the DAQ WIB header, with two FEMBs per fiber
// on FELIX readout:  slot: 0-2, fiber=1 or 2, cehcan: 0-255
int getOfflChanFromSlotFiberChan(int slot, int fiber, int chan);

int main(int argc, char **argv)
{
  const bool debug=false;

  // when using this program in test mode, turn testmainchans on.  Looks for duplicates and
  // looks up all the channels we put in
  
  bool testmainchans = false;

  // when running in add Disconnected chans mode, turn this switch on.  Will print out an addition
  // to the map corresponding to the disconnected channels
  
  bool addDisconnectedChans = true;
  int disconnectedChanOffset = 0; // 3200;
  bool testSpecificChans = false;
  
  if (argc > 1)
    {
      int iarg = atoi(argv[1]);
      //std::cout << "Input argument: " << iarg << std::endl;
      if (iarg > -1)
	{
	  addDisconnectedChans = true;
	  disconnectedChanOffset = iarg;
	}
      else
	{
	  testmainchans = true;
	  addDisconnectedChans = false;
	  if (iarg < -1)
	    {
	      testSpecificChans = true;
	    }
	}
    }
  
  std::vector<int> wibvec{0,1,1,1,1,2,2,2,2,3,3,3,3,4,4};
  std::vector<int> wibconnectorvec{0,1,2,3,4,1,2,3,4,2,1,3,4,2,1};

  // Implementation of the FELIX channel map: FEMB channel (chipNo & chipChannel) to FELIX fragment channel index.  The second 128 channels in the frame
  // have the same behavior but are offset by 128
  // This table was gotten from the FELIX channel-ID mode data taken for ProtoDUNE-SP
  
  int felixCh[8][16] = {
    {7, 6, 5, 4, 3, 2, 1, 0, 23, 22, 21, 20, 19, 18, 17, 16},
    {39, 38, 37, 36, 35, 34, 33, 32, 55, 54, 53, 52, 51, 50, 49, 48},
    {15, 14, 13, 12, 11, 10, 9, 8, 31, 30, 29, 28, 27, 26, 25, 24},
    {47, 46, 45, 44, 43, 42, 41, 40, 63, 62, 61, 60, 59, 58, 57, 56},
    {71, 70, 69, 68, 67, 66, 65, 64, 87, 86, 85, 84, 83, 82, 81, 80},
    {103, 102, 101, 100, 99, 98, 97, 96, 119, 118, 117, 116, 115, 114, 113, 112},
    {79, 78, 77, 76, 75, 74, 73, 72, 95, 94, 93, 92, 91, 90, 89, 88},
    {111, 110, 109, 108, 107, 106, 105, 104, 127, 126, 125, 124, 123, 122, 121, 120}
  };

  std::string fullname("vdcbce_chanmap_v1.txt");
  //std::string fullname("vdcbce_chanmap_v1_dcchan0.txt");
  std::ifstream inFile(fullname, std::ios::in);
  std::string line;
  int numchans = 0;
  while (std::getline(inFile,line)) {

    VDCBChanInfo chinfo;
    std::stringstream linestream(line);
    linestream >>
      chinfo.offlchan >>
      chinfo.wib >>
      chinfo.wibconnector >>
      chinfo.cebchan >>
      chinfo.femb >>
      chinfo.asic >>
      chinfo.asicchan >>
      chinfo.connector >>
      chinfo.stripid;
    chinfo.valid = true;

    // see if we have already entered this channel
    
    int otest = getOfflChanFromWIBConnectorInfo(chinfo.wib,chinfo.wibconnector,chinfo.cebchan);
    if (otest >= 0)
      {
	std::cout << "Duplicate info found: " << chinfo.wib << " " << chinfo.wibconnector << " " << chinfo.cebchan << std::endl;
	std::cout << chinfo.offlchan << " " << otest << std::endl;
      }
    
    // std::cout << chinfo.offlchan << std::endl;
    chantoinfomap[chinfo.offlchan] = chinfo;
    infotochanmap[chinfo.wib][chinfo.wibconnector][chinfo.cebchan] = chinfo.offlchan;
    ++numchans;
  }
  inFile.close();
  //std::cout << "num chans: " << numchans << std::endl;


  if (testSpecificChans)
    {
      VDCBChanInfo ciret = getChanInfoFromOfflChan(1671);
      std::cout << "looked up offline channel 1671: " << ciret.offlchan << " " << ciret.wib << " " << ciret.wibconnector << " " << ciret.cebchan << " " << ciret.femb << " " << ciret.asic << " " << ciret.asicchan << " "  << ciret.connector << " " << ciret.stripid << " " << ciret.valid << std::endl;

      ciret = getChanInfoFromOfflChan(1689);
      std::cout << "looked up offline channel 1689: " << ciret.offlchan << " " << ciret.wib << " " << ciret.wibconnector << " " << ciret.cebchan << " " << ciret.femb << " " << ciret.asic << " " << ciret.asicchan << " "  << ciret.connector << " " << ciret.stripid << " " << ciret.valid << std::endl;

      int ctest = getOfflChanFromWIBConnectorInfo(3, 3, 4);
      std::cout << "reverse lookup: " << ctest << std::endl;
    }
  
  if (testmainchans)
    {
      
      //int ctest2 = getOfflChanFromSlotFiberChan(3, 3, 4);
      //std::cout << "reverse lookup2: " << ctest2 << std::endl;

      // last channel in the actual detector is 3456.  Have 192 extras, possibly on the end
      
      for (int i=0;i<3648; ++i)
	{
	  VDCBChanInfo ciret2 = getChanInfoFromOfflChan(i);
	  if (ciret2.valid)
	    {
	      std::cout << "looked up offline channel: " << i << " " << ciret2.offlchan << " " << ciret2.wib << " " << ciret2.wibconnector << " " << ciret2.cebchan << " " << ciret2.femb << " " << ciret2.asic << " " << ciret2.asicchan << " "  << ciret2.connector << " " << ciret2.stripid << " " << ciret2.valid << std::endl;

	      int circ1 = getOfflChanFromWIBConnectorInfo(ciret2.wib, ciret2.wibconnector, ciret2.cebchan);
	      if (circ1 != i)
		{
		  std::cout << "circularity check 1 failed: " << circ1 << std::endl;
		}
	    }
       
	}
    }

  if (addDisconnectedChans)
    {
      int idc = 0;
      for (int ifemb=1;ifemb<15; ++ifemb)
	{
	  for (int ichan=0; ichan<128; ++ichan)
	    {
	      int wib=wibvec.at(ifemb);
	      int wibconnector=wibconnectorvec.at(ifemb);
	      int iofflchan = getOfflChanFromWIBConnectorInfo(wib,wibconnector,ichan);
	      int streamchannel = ichan;  // felixCh[asic-1][asicchan];  already in stream channel basis

	      int connector = 0;
	      if (iofflchan < 0)
		{

		  int asic = -1; //    fake number: 1 + ichan/16;
	          int asicchan = -1; // fake number: ichan % 16;

		  // loop over existing fembs to see if we can find asic and asicchans corresponding to the streamchannel

		  for (int ifemb2 = 1; ifemb2 <15; ++ifemb2)
		    {
	              int wib2=wibvec.at(ifemb2);
	              int wibconnector2=wibconnectorvec.at(ifemb2);
	              int iofflchan2 = getOfflChanFromWIBConnectorInfo(wib2,wibconnector2,ichan);
		      if (iofflchan2 < 0) continue;  // didn't find it on this FEMB, go to the next one
		      VDCBChanInfo ci = getChanInfoFromOfflChan(iofflchan2);
		      asic = ci.asic;
		      asicchan = ci.asicchan;
		      break;
		    }
		  
		  std::string stripid="D";
		  stripid += std::to_string(idc);
                  std::cout << std::setw(8) << idc+disconnectedChanOffset << " " << std::setw(8) << wib << " " << std::setw(8) << wibconnector << " " << std::setw(8) << streamchannel << " " << std::setw(8) << ifemb << " " << std::setw(8) << asic << " " << std::setw(8) << asicchan << " " << std::setw(8) << connector << " " << std::setw(8) << stripid << std::endl;
      
		  ++idc;  // to get ready for the next one
		}
	    }
	}
    }
  
  return 0;
}

// if not found in the map, return a chan info struct filled with -1's and set the valid flag to false.

VDCBChanInfo getChanInfoFromOfflChan(int offlchan)
{
  VDCBChanInfo r;
  auto fm = chantoinfomap.find(offlchan);
  if (fm == chantoinfomap.end())
    {
      r.offlchan = -1;
      r.wib = -1;
      r.wibconnector = -1;
      r.cebchan = -1;
      r.femb = -1;
      r.asic = -1;
      r.asicchan = -1;
      r.connector = -1;
      r.stripid = "INVALID";
      r.valid = false;
    }
  else
    {
      r = fm->second;
    }
  return r;
}

// returns -1 if information not found in the map

int getOfflChanFromWIBConnectorInfo(int wib, int wibconnector, int cechan)
{
  int r = -1;
  auto fm1 = infotochanmap.find(wib);
  if (fm1 == infotochanmap.end()) return r;
  auto m1 = fm1->second;
  auto fm2 = m1.find(wibconnector);
  if (fm2 == m1.end()) return r;
  auto m2 = fm2->second;
  auto fm3 = m2.find(cechan);
  if (fm3 == m2.end()) return r;
  r = fm3->second;  
  return r;
}

int getOfflChanFromSlotFiberChan(int slot, int fiber, int chan)
{
  int wc = fiber*2 - 1;
  if (chan>127)
    {
      chan -= 128;
      wc++;
    }
  return getOfflChanFromWIBConnectorInfo(slot+1,wc,chan);
}
