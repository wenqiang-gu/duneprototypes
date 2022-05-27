////////////////////////////////////////////////////////////////////////
//
// ProtoDUNEUnstableHV:Filter class
// 
// authors: Owen Goodwin, Lino Gerlach
// emails: owen.goodwin@manchester.ac.uk, lino.oscar.gerlach@cern.ch
//
// - A filter to reject events between given start and end times of unstable HV periods.
//   Using raw decoder timestamp.
// 
//    - All dates and times should be in unix time (UTC)
//
////////////////////////////////////////////////////////////////////////
// C++
extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
}
#include <math.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <typeinfo>
// ROOT
#include "TMath.h"
#include "TTimeStamp.h"

#include "TH1.h"
#include "TFile.h"
/// Framework 
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Principal/Event.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Handle.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "art_root_io/TFileService.h"
#include "art_root_io/TFileDirectory.h"
#include "messagefacility/MessageLogger/MessageLogger.h" 

#include "lardataobj/RawData/RDTimeStamp.h"
#include "dunecore/DuneObj/ProtoDUNETimeStamp.h"

#include <chrono>

namespace ProtoDUNEUnstableHV{

class UnstablePeriod {
    public:
        void setTimePair(std::pair<UInt_t,UInt_t> timePair);
        bool containsTimeStamp(const TTimeStamp *ts);
    private:
        void checkInputValid(UInt_t begin, UInt_t end);
        TTimeStamp beginTs;
        TTimeStamp endTs;
};
class UnstablePeriods {
    public:
        void setTimePairs(std::vector <std::pair<UInt_t,UInt_t >> timePairs);
        bool containsTimeStamp(const TTimeStamp *ts);
    private:
        UnstablePeriod *subPeriods;
        int n_periods;
};
class DecoderHandler {
    public:
        double getTimeStamp(art::Event & e);
    private:
        art::InputTag iTag = art::InputTag("timingrawdecoder", "daq");
        art::Handle< std::vector<raw::RDTimeStamp> > handle;
        void setHandle(art::Event& evt);
        raw::RDTimeStamp getRawDigitTimeStamp(art::Event& evt);
        double getSecsFrom20nsTicks(uint64_t rawTime);
};
class Filter : public art::EDFilter  {
    public:
        explicit Filter(fhicl::ParameterSet const& );
        virtual ~Filter();
        bool filter(art::Event& evt);
        void beginJob();
        void endJob();
        void reconfigure(fhicl::ParameterSet const& p);
    private:
        UnstablePeriods *unstablePeriods = new UnstablePeriods;
        DecoderHandler *decoderHandler = new DecoderHandler;
        bool keepEvent(art::Event& evt);
        bool fDebug;
        TH1D* fSelectedEvents;
        TH1D* fTotalEvents;
};

void UnstablePeriod::setTimePair(std::pair<UInt_t,UInt_t> timePair){
    checkInputValid(timePair.first, timePair.second);
    beginTs = TTimeStamp(timePair.first);
    endTs = TTimeStamp(timePair.second);
}

bool UnstablePeriod::containsTimeStamp(const TTimeStamp *ts){
    return ts->GetSec() > beginTs.GetSec() && ts->GetSec() < endTs.GetSec();
}

void UnstablePeriod::checkInputValid(UInt_t begin, UInt_t end){
    if (begin<1000000000 || end < 1000000000 || begin>end){
        throw std::logic_error("Unstable period not valid!");
    }
}

void UnstablePeriods::setTimePairs(std::vector <std::pair<UInt_t,UInt_t >> timePairs){
    n_periods = timePairs.size();
    subPeriods = new UnstablePeriod[n_periods];
    for (int n=0; n<n_periods; n++){
        subPeriods[n].setTimePair(timePairs.at(n));
    }
}

bool UnstablePeriods::containsTimeStamp(const TTimeStamp *ts){
    for (int n=0; n<n_periods; n++){
        if (subPeriods[n].containsTimeStamp(ts)) return true;
    }
    return false;
}

void DecoderHandler::setHandle(art::Event & e) {
    handle = e.getHandle< std::vector<raw::RDTimeStamp> >(iTag);
    MF_LOG_INFO("BeamEvent") << "Handle valid? " << handle.isValid() << "\n";
}

raw::RDTimeStamp DecoderHandler::getRawDigitTimeStamp(art::Event & e) {
    setHandle(e);
    for (auto const & rawTs : *handle){
        return rawTs;
    }return -1;
}

double DecoderHandler::getSecsFrom20nsTicks(uint64_t raw20nsTicks){
    long long ticks = (raw20nsTicks * 2) / (int)(TMath::Power(10,8));
    ticks = ticks * (int)(TMath::Power(10,8)) / 2;
    double seconds = 20.e-9 * ticks;
    return seconds;
}

double DecoderHandler::getTimeStamp(art::Event & e) {
    MF_LOG_INFO("BeamEvent") << "Getting Raw Decoder Info" << "\n";
    raw::RDTimeStamp rDtS = getRawDigitTimeStamp(e);
    MF_LOG_INFO("BeamEvent") << "Trigger: " << rDtS.GetFlags() << "\n";
    uint64_t raw20nsTicks = rDtS.GetTimeStamp();
    return getSecsFrom20nsTicks(raw20nsTicks);
}

void Filter::beginJob() {
    art::ServiceHandle<art::TFileService> tfs;
    fSelectedEvents = tfs->make<TH1D>("fSelectedEvents", "Number of Selected Events", 3, 0, 3); //counts the number of selected events 
    fTotalEvents = tfs->make<TH1D>("fTotalEvents", "Total Events", 3, 0, 3); //counts the initial number of events in the unfiltered root input file
}

void Filter::endJob() { }

void Filter::reconfigure(fhicl::ParameterSet const& p) {
    unstablePeriods->setTimePairs(p.get<std::vector<std::pair<UInt_t,UInt_t>>>("TimeRanges"));
    fDebug = p.get<int>("Debug");
}

Filter::Filter(fhicl::ParameterSet const& pset) : EDFilter(pset) {
    this->reconfigure(pset);
}

Filter::~Filter() { }

bool Filter::keepEvent(art::Event &evt) {
    if (!evt.isRealData()) return true;
    TTimeStamp *evtTTS = new TTimeStamp(decoderHandler->getTimeStamp(evt));
    return !unstablePeriods->containsTimeStamp(evtTTS);
}

bool Filter::filter(art::Event &evt) {
    std::cout<<"filter()"<<std::endl;
    bool keep = false;
    fTotalEvents->Fill(1);
    if (keepEvent(evt)) {
        fSelectedEvents->Fill(1);
        keep = true;
    }
    MF_LOG_INFO("BeamEvent") << "GypsyTest: id=" << evt.event() << ", keep=" << keep << "\n";
    return keep;
}
}
DEFINE_ART_MODULE(ProtoDUNEUnstableHV::Filter)
