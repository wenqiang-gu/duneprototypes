
#include <iostream>
#include <TString.h>

/*************************************************************************
    > File Name: MakePD2HDChannelMap_v1.C
    > Author: Tom Junk
 ************************************************************************/

using namespace std;

// Implementation of Table 5 in DocDB 4064: FEMB channel (asicNo & asicChannel) to plane type
int plane[8][16] = {
										 {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2},
										 {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2}, 
										 {2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
										 {2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
										 {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2},
										 {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2},
										 {2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
										 {2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0}
	                 };

// Implementation of Table 5 in DocDB 4064: FEMB channel (asicNo & asicChannel) to plane channel number                 
int planeCh[8][16] = {
										 {19, 17, 15, 13, 11, 19, 17, 15, 13, 11, 23, 21, 19, 17, 15, 13},
										 {9,  7,  5,  3,  1,  9,  7,  5,  3,  1,  11, 9,  7,  5,  3,  1}, 
										 {14, 16, 18, 20, 22, 24, 12, 14, 16, 18, 20, 12, 14, 16, 18, 20},
										 {2,  4,  6,  8,  10, 12, 2,  4,  6,  8,  10, 2,  4,  6,  8,  10},
										 {29, 27, 25, 23, 21, 29, 27, 25, 23, 21, 35, 33, 31, 29, 27, 25},
										 {39, 37, 35, 33, 31, 39, 37, 35, 33, 31, 47, 45, 43, 41, 39, 37},
										 {26, 28, 30, 32, 34, 36, 22, 24, 26, 28, 30, 22, 24, 26, 28, 30},
										 {38, 40 ,42, 44, 46, 48, 32, 34, 36, 38, 40, 32, 34, 36, 38, 40}
	                 };

// Implmentation of WIB number from Cheng-Ju's squash-plate map.  FEMB is numbered here from 0 to 19

int WIBFromFEMB[20]         = {1,1,2,2,1,1,2,2,3,3,4,4,3,3,4,4,5,5,5,5};
int LinkFromFEMB[20]        = {1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,1,1,0,0};
int FEMBOnLinkFromFEMB[20]  = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0};

int calc_cebchan(int iplane, int ichan_in_plane);
int calc_asic(int iplane, int ichan_in_plane);
int calc_asicchan(int iplane, int ichan_in_plane);

// work in offline order:  upstream first, then downstream.  Within upstream, do
// beam-right side first, low z to high z, then beam left low z to high z.  Then do
// downstream side.

int cratelist[4] = {2,4,1,3};
TString APANames[4] = {"APA_P02SU","APA_P02NL","APA_P01SU","APA_P01NL"};

void MakePD2HDChannelMap_v1() {
 
  ofstream fmapfelix;
  fmapfelix.open ("PD2HDChannelMap_v1.txt");
  int asicsperfemb = 8;
  int ncrates = 4;
  int nfemb = 20;
  int nupcrates = 2;
  int ndowncrates = 2;
  int nslots = 5;
  int nichans = 40; // induction-plane channels
  int ncchans = 48; // collection-plane channels

  // variables beginning with "o" are for output

  for (int icrate=0;icrate<ncrates;++icrate)
    {
      bool udown = (icrate == 1 || icrate == 3);  // upside-down flag
      int offlchan = 0;
      int ocrate = cratelist[icrate];
      TString oAPAName = APANames[icrate];
      int oplane = 0;

      for (int ifemb=0; ifemb<nfemb; ++ifemb)
	{
	  int ofemb = ifemb + 1;  // for output
	  int owib = WIBFromFEMB[ifemb];
	  int olink = LinkFromFEMB[ifemb];
	  int ofemb_on_link = FEMBOnLinkFromFEMB[ifemb];

	  for (int iuchan = 0; iuchan<nichans; ++iuchan)
	    {
	      oplane = 0;
	      if (udown)  // upside-down
		{
		  if (ifemb<10)
		    {
		      offlchan = 2560*icrate + 348 + nichans*ifemb - iuchan + nichans - 1;
		    }
		  else
		    {
		      int tmpchan = 347 + nichans*(ifemb - 19) - iuchan;
		      if (tmpchan < 0) tmpchan += 800;
                      offlchan = tmpchan + 2560*icrate;
		    }
		}
	      else
		{
		  if (ifemb<10)
		    {
		      offlchan = 2560*icrate + 399 - nichans*ifemb - nichans + iuchan + 1;
		    }
		  else
		    {
		      offlchan = 2560*icrate + 400 + nichans*(19-ifemb)  + iuchan;
		    }
		}

  	      int ocebchan  = calc_cebchan(oplane,iuchan);
  	      int oasic     = calc_asic(oplane,iuchan);
  	      int oasicchan = calc_asicchan(oplane,iuchan);
	      fmapfelix << offlchan << "\t" 
			<< ocrate << "\t"
			<< oAPAName << "\t"
			<< owib << "\t"
			<< olink << "\t"
			<< ofemb_on_link << "\t"
			<< ocebchan << "\t"
			<< oplane << "\t"
			<< iuchan << "\t"
			<< ofemb << "\t"
 			<< oasic << "\t"
			<< oasicchan << "\t" 
			<< std::endl;

	    }
	  for (int ivchan = 0; ivchan<nichans; ++ivchan)
	    {
	      oplane = 1;
	      if (udown) // upside-down
		{
		  if (ifemb<10)
		    {
		      offlchan = 2560*icrate + 1547 - nichans*ifemb + ivchan - nichans + 1;
		    }
		  else
		    {
		      int tmpchan = 1548 - nichans*(ifemb-19) + ivchan;
		      if (tmpchan > 1599) tmpchan -= 800;
		      offlchan = tmpchan + 2560*icrate;
		    }
		}
	      else
		{
		  if (ifemb<10)
		    {
		      offlchan = 2560*icrate + 800 + nichans*ifemb + nichans - ivchan - 1;
		    }
		  else
		    {
		      offlchan = 2560*icrate + 1599 + nichans*(ifemb-19) - ivchan;
		    }
		}
  	      int ocebchan  = calc_cebchan(oplane,ivchan);
  	      int oasic     = calc_asic(oplane,ivchan);
  	      int oasicchan = calc_asicchan(oplane,ivchan);
	      fmapfelix << offlchan << "\t" 
			<< ocrate << "\t"
			<< oAPAName << "\t"
			<< owib << "\t"
			<< olink << "\t"
			<< ofemb_on_link << "\t"
			<< ocebchan << "\t"
			<< oplane << "\t"
			<< ivchan << "\t"
			<< ofemb << "\t"
 			<< oasic << "\t"
			<< oasicchan << "\t"
			<< std::endl;
	    }
	  for (int ixchan = 0; ixchan<ncchans; ++ixchan)
	    {
	      oplane = 2;
	      if (udown) // upside-down
		{
		  if (ifemb<10)
		    {
		      offlchan = 2560*icrate + 2080 + ncchans*ifemb + ncchans - ixchan - 1;
		    }
		  else
		    {
		      offlchan = 2560*icrate + 1600 + ncchans*(19-ifemb) + ixchan;
		    }
		}
	      else
		{
		  if (ifemb<10)
		    {
		      offlchan = 2560*icrate + 1600 + ncchans*ifemb + ncchans - ixchan - 1;
		    }
		  else
		    {
		      offlchan = 2560*icrate + 2080 + ncchans*(19-ifemb) + ixchan;
		    }
		}
  	      int ocebchan  = calc_cebchan(oplane,ixchan);
  	      int oasic     = calc_asic(oplane,ixchan);
  	      int oasicchan = calc_asicchan(oplane,ixchan);
	      fmapfelix << offlchan << "\t" 
			<< ocrate << "\t"
			<< oAPAName << "\t"
			<< owib << "\t"
			<< olink << "\t"
			<< ofemb_on_link << "\t"
			<< ocebchan << "\t"
			<< oplane << "\t"
			<< ixchan << "\t"
			<< ofemb << "\t"
 			<< oasic << "\t"
			<< oasicchan << "\t"
			<< std::endl;
	    }
	}
    }

  fmapfelix.close();  
}


int calc_cebchan(int iplane, int ichan_in_plane)
{
  return 16*calc_asic(iplane,ichan_in_plane) + calc_asicchan(iplane,ichan_in_plane);
}

int calc_asic(int iplane, int ichan_in_plane)
{
  int icp1 = ichan_in_plane + 1;
  for (int iasic=0; iasic<8; ++iasic)
    {
      for (int ichan=0; ichan<16; ++ichan)
	{
	  if (plane[iasic][ichan] == iplane && planeCh[iasic][ichan] == icp1)
	    {
	      return iasic;
	    }
	}
    }
  return -1;
}

int calc_asicchan(int iplane, int ichan_in_plane)
{
  int icp1 = ichan_in_plane + 1;
  for (int iasic=0; iasic<8; ++iasic)
    {
      for (int ichan=0; ichan<16; ++ichan)
	{
	  if (plane[iasic][ichan] == iplane && planeCh[iasic][ichan] == icp1)
	    {
	      return ichan;
	    }
	}
    }
  return -1; 
}
