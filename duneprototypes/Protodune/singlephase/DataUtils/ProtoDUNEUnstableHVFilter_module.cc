////////////////////////////////////////////////////////////////////////
//
// ProtoDUNEUnstableHV:Filter class
// 
// authors: Owen Goodwin, Lino Gerlach
// emails: owen.goodwin@manchester.ac.uk, lino.oscar.gerlach@cern.ch
//
// - A filter to reject events between given start and end times of unstable HV periods.
//   Using raw decoder timestamp and three custom helper classes
//
//    - All dates and times should be in unix time (UTC)
//
////////////////////////////////////////////////////////////////////////
// ROOT
#include "TMath.h"
#include "TTimeStamp.h"
#include "TH1.h"
/// Framework
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Principal/Event.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Handle.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art_root_io/TFileService.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "lardataobj/RawData/RDTimeStamp.h"


namespace ProtoDUNEUnstableHV{

class UnstablePeriod {
    public:
        void setTimePair(std::pair<UInt_t,UInt_t> timePair){
            checkInputValid(timePair.first, timePair.second);
            beginTs = TTimeStamp(timePair.first);
            endTs = TTimeStamp(timePair.second);
        }
        bool containsTimeStamp(const TTimeStamp *ts){
            return ts->GetSec() > beginTs.GetSec() && ts->GetSec() < endTs.GetSec();
        }
    private:
        TTimeStamp beginTs;
        TTimeStamp endTs;
        void checkInputValid(UInt_t begin, UInt_t end){
            if (begin<1000000000 || end < 1000000000 || begin>end){
                throw std::runtime_error("Unstable period not valid!");
            }
        }
};

class UnstablePeriodCollection {
    public:
        void setTimePairs(std::vector <std::pair<UInt_t,UInt_t >> timePairs){
            n_periods = timePairs.size();
            subPeriods = new UnstablePeriod[n_periods];
            for (int n=0; n<n_periods; n++){
                subPeriods[n].setTimePair(timePairs.at(n));
            }
        }
        bool containsTimeStamp(const TTimeStamp *ts){
            for (int n=0; n<n_periods; n++){
                if (subPeriods[n].containsTimeStamp(ts)) return true;
            }
            return false;
        }
    private:
        UnstablePeriod *subPeriods;
        int n_periods;
};

class DecoderHandler {
    public:
        double getTimeStamp() {
            MF_LOG_INFO("BeamEvent") << "Getting Raw Decoder Info" << "\n";
            for (auto const & rDtS : *handle){
                return getSecsFrom20nsTicks(rDtS.GetTimeStamp());
            }
            throw std::runtime_error("Handle to RDTimeStamp vector is empty!");
            return -1;
        }
        void setHandle(art::Event & e) {
            handle = e.getHandle< std::vector<raw::RDTimeStamp> >(iTag);
            MF_LOG_INFO("BeamEvent") << "Handle valid? " << handle.isValid() << "\n";
        }
    private:
        art::InputTag iTag = art::InputTag("timingrawdecoder", "daq");
        art::Handle< std::vector<raw::RDTimeStamp> > handle;
        double getSecsFrom20nsTicks(uint64_t raw20nsTicks){
            long long ticks = (raw20nsTicks * 2) / (int)(TMath::Power(10,8));
            ticks = ticks * (int)(TMath::Power(10,8)) / 2;
            return 20.e-9 * ticks;
        }
};

class Filter : public art::EDFilter  {
    public:
        explicit Filter(fhicl::ParameterSet const& );
        bool filter(art::Event& evt);
        void beginJob();
    private:
        UnstablePeriodCollection *unstablePeriodCollection = new UnstablePeriodCollection;
        DecoderHandler *decoderHandler = new DecoderHandler;
        bool _filter(art::Event& evt);
        bool fDebug;
        TH1D* fSelectedEvents;
        TH1D* fTotalEvents;
};
Filter::Filter(fhicl::ParameterSet const& p) : EDFilter(p) {
    unstablePeriodCollection->setTimePairs(p.get<std::vector<std::pair<UInt_t,UInt_t>>>("TimeRanges"));
    fDebug = p.get<int>("Debug");
}
void Filter::beginJob() {
    art::ServiceHandle<art::TFileService> tfs;
    fSelectedEvents = tfs->make<TH1D>("fSelectedEvents", "Number of Selected Events", 3, 0, 3);
    fTotalEvents = tfs->make<TH1D>("fTotalEvents", "Total Events", 3, 0, 3);
}
bool Filter::_filter(art::Event &evt) {
    if (!evt.isRealData()) return true;
    decoderHandler->setHandle(evt);
    TTimeStamp *evtTTS = new TTimeStamp(decoderHandler->getTimeStamp());
    return !unstablePeriodCollection->containsTimeStamp(evtTTS);
}
bool Filter::filter(art::Event &evt) {
    fTotalEvents->Fill(1);
    if (_filter(evt)) {
        fSelectedEvents->Fill(1);
        return true;
    }
    return false;
}
}
DEFINE_ART_MODULE(ProtoDUNEUnstableHV::Filter)