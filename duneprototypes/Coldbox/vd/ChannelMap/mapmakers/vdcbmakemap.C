#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

int main(int argc, char **argv)
{
  const bool debug=false;

  std::string stripid;
  int connector = 0;
  int pin = 0;
  int board = 0;
  int cebchannel = 0;
  int asic = 0;
  int asicchan = 0;

  int wib=0;
  int wibconnector=0;

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

  while (true)
    {
      std::cin >> stripid >> connector >> pin >> board >> cebchannel >> asic >> asicchan;
      if (feof(stdin)) break;
      
      if (debug) std::cout << stripid << " " << connector << " " << pin << " " << board << " " << cebchannel << " " << asic << " " << asicchan << std::endl;

      if (board < 1 || board > 14)
	{
	  std::cout << "invalid CE board: " << board << std::endl;
	  return 1;
	}
      wib = wibvec.at(board);
      wibconnector = wibconnectorvec.at(board);

      // parse the strip ID
      std::string sp = stripid.substr(0,1);
      if (debug) std::cout << " strip ID prefix: " << sp << std::endl;
      std::string sn = stripid.substr(1);
      std::stringstream sns;
      sns << sn;
      int stripidnum;
      sns >> stripidnum;
      if (debug) std::cout << " strip ID number: " << stripidnum << std::endl;

      int chan = -1;
      if (sp == "U")
	{
	  chan = stripidnum + 1600 - 1;
	  if (stripidnum > 256)
	    {
	      chan = stripidnum + 1856 - 257;
	    }
	}
      else if (sp == "Y")
	{
	  chan = stripidnum + 1984 - 1;
	  if (stripidnum > 320)
	    {
	      chan = stripidnum + 2304 - 321;
	    }
	}
      else if (sp == "Z")
	{
	  chan = stripidnum + 2624 - 1;
	  if (stripidnum > 288)
	    {
	      chan = stripidnum + 2912 - 289;
	    }
	}
      if (chan == -1)
	{
	  std::cout << "Could not assign channel.  Use the debug flag to get more info." << std::endl;
	}

      int streamchannel = felixCh[asic-1][asicchan];
      
      std::cout << std::setw(8) << chan << " " << std::setw(8) << wib << " " << std::setw(8) << wibconnector << " " << std::setw(8) << streamchannel << " " << std::setw(8) << board << " " << std::setw(8) << asic << " " << std::setw(8) << asicchan << " " << std::setw(8) << connector << " " << std::setw(8) << stripid << std::endl;
      
    }
  
  return 0;
}
