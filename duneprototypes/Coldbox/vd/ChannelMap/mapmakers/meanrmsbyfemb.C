#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "canvas/Utilities/InputTag.h"
#include "gallery/Event.h"

#include "TFile.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TStyle.h"
#include "TColor.h"
#include "TMath.h"
#include "TVectorT.h"
#include "TCanvas.h"
#include "lardataobj/RawData/RawDigit.h"
#include "TLegend.h"

using namespace art;
using namespace std;


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

void FillMap();

VDCBChanInfo getChanInfoFromOfflChan(int offlchan);

// this uses conventions from Nitish's spreadsheet.  WIB: 1-3, wibconnector: 1-4, cechan: 0-127
int getOfflChanFromWIBConnectorInfo(int wib, int wibconnector, int cechan);

// this uses conventions from the DAQ WIB header, with two FEMBs per fiber
// on FELIX readout:  slot: 0-2, fiber=1 or 2, cehcan: 0-255
int getOfflChanFromSlotFiberChan(int slot, int fiber, int chan);

//  ROOT script using gallery to make mean, rms, and drms plots using raw::RawDigits for the ievcount'th event in the file
// Tom Junk, Fermilab, Sep. 2017
// Oct 2017 -- update to use the channel number from the rawdigit and not just the index of the rawdigit
// Example invocation for a 3x1x1 imported rootfile, make a correlation plot for the first event in the file.
// root [0] .L meanrms.C++
// root [1] meanrms("/pnfs/dune/tape_backed/dunepro/test-data/dune/raw/01/85/12/09/wa105_r842_s32_1501156823.root");

// arguments:  filename -- input file, larsoft formatted
// ievcount:  which event to display the mean and RMS for.  This is the tree index in the file and not the event number
// tickmin, tickmax -- to truncate the part of the event run on.  Set to big and small numbers for no truncation.
// inputtag: use "daq" for MC and 3x1x1 imported data.  It's SplitterInput:TPC for split 35t data


void meanrmsbyfemb(std::string const& filename="tmpdecode.root", 
             size_t ievcount=0, 
             size_t tickmin=0, 
             size_t tickmax=8192,  
             std::string const& inputtag="caldata:dataprep")
{

  FillMap();

  gStyle->SetOptStat(0);

  size_t evcounter=0;

  double s2i = 1.0/TMath::Sqrt(2.0);

  InputTag rawdigit_tag{ inputtag };
  vector<string> filenames(1, filename);

  for (gallery::Event ev(filenames); !ev.atEnd(); ev.next()) {
    if (evcounter == ievcount)
      {
	auto const& rawdigits = *ev.getValidHandle<vector<raw::RawDigit>>(rawdigit_tag);
	if (!rawdigits.empty())
	  {
	    const size_t nrawdigits = rawdigits.size();
	    size_t nchans=1792;

	    size_t tlow = TMath::Max(tickmin, (size_t) 0);
	    size_t thigh = TMath::Min(tickmax, (size_t) rawdigits[0].Samples()-1); // assume uncompressed; all channels have the same number of samples
	    size_t nticks = thigh - tlow + 1;

	    TVectorT<double> x(nchans);
	    TVectorT<double> dx(nchans);
	    TVectorT<double> sumx(nchans);
	    TVectorT<double> sumxx(nchans);
	    TVectorT<double> sumdx(nchans);
	    TVectorT<double> sumdxx(nchans);

	    size_t firstchan = 1600;

	    TH1D *mean = (TH1D*) new TH1D("mean","Mean",nchans,firstchan-0.5,firstchan+nchans-0.5);
	    TH1D *mean2 = (TH1D*) new TH1D("mean2","mean2",nchans,firstchan-0.5,firstchan+nchans-0.5); // declare with nchans so it can be overplotted
                                                                                 // even though we have one fewer channel
	    TH1D *rmshist = (TH1D*) new TH1D("rmshist","RMS",nchans,firstchan-0.5,firstchan+nchans-0.5);
	    TH1D *drmshist = (TH1D*) new TH1D("drmshist","dRMS",nchans,firstchan-0.5,firstchan+nchans-0.5);

	    TH1D *rmbyfemb[14];
	    for (int i=0; i<14; ++i)
	      {
		TString hn="RMSFEMB";
		hn += (i+1);
		rmbyfemb[i] = (TH1D*) new TH1D(hn,hn,nchans,firstchan-0.5,firstchan+nchans-0.5);
		rmbyfemb[i]->SetDirectory(0);
		rmbyfemb[i]->SetLineColor(1+i);
		rmbyfemb[i]->SetMarkerColor(1+i);
		if (i>8) rmbyfemb[i]->SetMarkerColor(31+i);
		rmbyfemb[i]->SetMarkerStyle(20);
	      }

            for (size_t itick=tlow;itick<=thigh;++itick)
	      {
		for (size_t ichan=0;ichan<nrawdigits; ++ichan) 
		  { 
		    size_t ic = rawdigits[ichan].Channel() - firstchan;
		    x[ic] = rawdigits[ichan].ADC(itick);
		  } 
		 for (size_t ichan=1;ichan<nchans; ++ichan) 
		   {
		     dx[ichan] = (x[ichan]-x[ichan-1])*s2i;
		   }
		 dx[0] = 0;
		 sumx += x;
		 sumdx += dx;
		 sumxx += x.Sqr();  // nb -- this overwrites x with its square so do it last
		 sumdxx += dx.Sqr(); // nb -- this overwrites dx with its square so do it last
	      }
	    
	    cout << "Finished filling vectors" << endl;

	    for (size_t ichan=0;ichan<nchans;++ichan)
	      {
		  {
		    double avg = sumx[ichan]/nticks;
		    double avgsquare = sumxx[ichan]/nticks;
		    mean->SetBinContent(ichan+1,avg);
		    mean2->SetBinContent(ichan+1,avg);
		    double rms = 0;
		    double tval = avgsquare - avg*avg;
		    if (tval>0) rms = TMath::Sqrt(tval);
		    mean->SetBinError(ichan+1,rms);
		    rmshist->SetBinContent(ichan+1,rms);

		    auto chinfo = getChanInfoFromOfflChan(ichan+1600);
		    int ifemb = chinfo.femb - 1;
		    //std::cout << "ichan, ifemb: " << ichan << " " << ifemb << std::endl;
		    rmbyfemb[ifemb]->SetBinContent(ichan+1,rms);

		    avg = sumdx[ichan]/nticks;
		    avgsquare = sumdxx[ichan]/nticks;
		    tval = avgsquare - avg*avg;
		    rms = 0;
		    if (tval>0) rms = TMath::Sqrt(tval);
		    mean2->SetBinError(ichan+1,avg);
		    drmshist->SetBinContent(ichan+1,rms);

		  }
	      }

	    TCanvas *mycanvas = new TCanvas("c","c",700,600);
	    mycanvas->Divide(1,2);
            mycanvas->cd(1);
	    mean->SetDirectory(0);
	    mean2->SetDirectory(0);
	    mean->GetXaxis()->SetTitle("Channel");
	    mean->GetYaxis()->SetTitle("Mean ADC");
	    mean->Draw("HIST");

	    mycanvas->cd(2);
	    rmshist->SetDirectory(0);
	    rmshist->GetXaxis()->SetTitle("Channel");
	    rmshist->GetYaxis()->SetTitle("RMS and dRMS");
	    drmshist->SetDirectory(0);
	    rmshist->SetLineColor(kBlack);
	    drmshist->SetLineColor(kGreen);
	    rmshist->Draw("HIST");
	    drmshist->Draw("SAME,HIST");
            mycanvas->Print("meanrms.png");
	    TFile outfile("meanrms.root","RECREATE");
	    mean->Write();
	    rmshist->Write();
	    drmshist->Write();
	    outfile.Close();

	    TCanvas *mycanvas2 = new TCanvas("c2","c2",700,600);
	    rmbyfemb[0]->SetMaximum(100);
	    rmbyfemb[0]->Draw("P");
	    for (int ihist=0;ihist<14; ++ihist)
	      {
		rmbyfemb[ihist]->Draw("P,SAME");
	      }
	    auto legend = new TLegend(0.8,0.5,0.9,0.9);
	    for (int ihist=0; ihist<14; ++ihist)
	      {
		TString ls = "FEMB";
		ls += (ihist+1);
		legend->AddEntry(rmbyfemb[ihist],ls,"P");
	      }
	    legend->Draw();

	    mycanvas2->Print("meanrmsbyfemb.png");
	  }
      }
    ++evcounter;
  }
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


void FillMap()
{
  std::string fullname("vdcbce_chanmap_v1_dcchan3200.txt");
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
}
