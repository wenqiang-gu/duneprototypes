////////////////////////////////////////////////////////////////////////
// Class:       CRTVDSim
// File:        CRTVDSim_module.cc
//
// September 2023 by Thomas Kosc kosc.thomas@gmail.com
//  Adapted from protodune single phase module duneprototypes/Protodune/singlephase/CRT/CRTSim_module.cc
////////////////////////////////////////////////////////////////////////

//     An ART module to simulate how the ProtoDUNE-SP Cosmic Ray Tagger (CRT) system 
//responds to energy deposits. 
//Framework includes
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "art/Persistency/Common/PtrMaker.h"
#include "canvas/Persistency/Common/Assns.h"
#include "canvas/Persistency/Common/Ptr.h"
#include "lardata/Utilities/AssociationUtil.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "canvas/Persistency/Common/FindManyP.h"
//LArSoft includes

#include "larcore/Geometry/Geometry.h"
#include "lardataobj/Simulation/AuxDetHit.h"
#include "lardataobj/RecoBase/TrackingTypes.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_vectors.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
#include "nusimdata/SimulationBase/MCParticle.h"
#include "nug4/ParticleNavigation/ParticleList.h"

//local includes
#include "CRTVDTrigger.h"
//#include "duneprototypes/Protodune/singlephase/CRT/data/CRTTrigger.h"

//c++ includes
#include <memory>
#include <algorithm>
#include <string>
#include <map>
#include <unordered_map>

namespace CRT {
  class CRTVDSim;
}

class CRT::CRTVDSim : public art::EDProducer {
public:
  explicit CRTVDSim(fhicl::ParameterSet const & p);
  // The compiler-generated destructor is fine for non-base
  // classes without bare pointers or other resource use.

  // Plugins should not be copied or assigned.
  CRTVDSim(CRTVDSim const &) = delete;
  CRTVDSim(CRTVDSim &&) = delete;
  CRTVDSim & operator = (CRTVDSim const &) = delete;
  CRTVDSim & operator = (CRTVDSim &&) = delete;

  // Required functions.
  void produce(art::Event & e) override;


private:

  // -- message logger
  mf::LogInfo logInfo_;

  //The formats for time and ADC value I will use throughout this module.  If I want to change it later, I only have to change it in one place.  
  typedef long long int time;
  typedef unsigned short adc_t;

  // Member data
  art::InputTag fSimLabel; 
//  double fGeVToADC; //Conversion from GeV in detector to ADC counts output by MAROC2.   

  //Parameterization for algorithm that takes voltage entering MAROC2 and converts it to ADC hits.  If I ultimately decide I want to 
  //study impact of more detailed simulation, factor this out into an algorithm for simulating MAROC2 that probably takes continuous 
  //voltage signal as input anyway.  
  time fIntegrationWindow; //The time in ns over which charge is integrated in the CRT electronics before being sent to the DAC comparator

  time fSamplingTime; // sampling time of CRT channels in ns
                             
  // coincidence time window between top and bottom CRT modules is defined with respect to bottom CRT.
  time fDownwardWindow; // time window pointing to the future w.r.t bottom CRT module time (ns)
  time fUpwardWindow; // time window pointing to the past w.r.t bottom CRT module time (ns)
  time fDeadTime; //The dead time after readout during which no energy deposits are processed by CRT boards. (ns)
  double fEnergyThreshold; //  MeV integrated Energy deposited.that can trigger a single crt channel
//  adc_t fDACThreshold; //DAC threshold for triggering readout for any CRT strip.  
                       //In GeV for now, but needs to become ADC counts one day.  
                       //Should be replaced by either a lookup in a hardware database or 
                       //some constant value one day.  
};


CRT::CRTVDSim::CRTVDSim(fhicl::ParameterSet const & p): EDProducer{p}, 
                                                              logInfo_("CRTVDSim"),
                                                              fSimLabel(p.get<art::InputTag>("SimLabel")), 
                                                              /*fScintillationYield(p.get<double>("ScintillationYield")), 
                                                              fQuantumEff(p.get<double>("QuantumEff")), 
                                                              fDummyGain(p.get<double>("DummyGain")),*/
                                                              //fGeVToADC(p.get<double>("GeVToADC")),
                                                              fIntegrationWindow(p.get<time>("IntegrationWindow")), 
                                                              fSamplingTime(p.get<time>("SamplingTime")), 
                                                              fDownwardWindow(p.get<time>("DownwardWindow")), 
                                                              fUpwardWindow(p.get<time>("UpwardWindow")), 
                                                              //fReadoutWindowSize(p.get<size_t>("ReadoutWindowSize")), 
                                                              fDeadTime(p.get<size_t>("DeadTime")),
                                                              fEnergyThreshold(p.get<double>("EnergyThreshold"))
                                                              //fDACThreshold(p.get<adc_t>("DACThreshold"))
{
  produces<std::vector<CRTVD::Trigger>>();
//  produces<art::Assns<simb::MCParticle,CRTVD::Trigger>>(); 

}


void CRT::CRTVDSim::produce(art::Event & e)
{
  // Get all AuxDetHits contained in the event
  auto const allSims = e.getMany<sim::AuxDetHitCollection>();
std::cout << "aux det hit size : " << allSims.size() << std::endl;

  // -- Get all MCParticles to do assns later
  const auto & mcp_handle = e.getValidHandle<std::vector<simb::MCParticle>>("largeant"); // -- TODO: make this an input tag. Tag is correct though
  art::PtrMaker<simb::MCParticle> makeMCParticlePtr{e,mcp_handle.id()};
//  art::ServiceHandle < cheat::ParticleInventoryService > partInventory; // seems useless
  auto const & mcparticles = *(mcp_handle); //dereference the handle
std::cout << "MCParticle size = " << mcparticles.size() << std::endl;

  // -- Construct map of trackId to MCParticle handle index to do assns later
  std::unordered_map<int, int> map_trackID_to_handle_index;
  for (size_t idx = 0; idx < mcparticles.size(); ++idx){
    int tid = mcparticles[idx].TrackId();
    map_trackID_to_handle_index.insert(std::make_pair(tid,idx));
  }
  
  // objects to produce
  auto trigCol = std::make_unique<std::vector<CRTVD::Trigger>>();
//  std::unique_ptr< art::Assns<simb::MCParticle, CRTVD::Trigger>> partToTrigger( new art::Assns<simb::MCParticle, CRTVD::Trigger>);
  art::PtrMaker<CRTVD::Trigger> makeTrigPtr(e);

  // Retrieve geometry service
  art::ServiceHandle<geo::Geometry> geom;

  // declare hits module map that we'll work with
  std::map<int, std::map<time, std::vector<std::pair<CRTVD::Hit, int>>>> crtHitsModuleMap;



  // start loop over AuxDetHit objects and store info into the map
  for(auto const& auxHits : allSims){
//auxHits type is const class art::Handle<std::vector<sim::AuxDetHit> >
// its size is equal to the number of art-root objects containing sim::AuxDetHit (depending on which geometry volumes are auxdet sensitive)
    // loop over sim::AuxDetHit 
    for(const auto & eDep: * auxHits)
    {
      float tAvg_fl = eDep.GetEntryT(); // ns
      time tAvg = static_cast<time>(tAvg_fl);
      geo::Point_t const midpoint = geo::Point_t( 0.5*(eDep.GetEntryX()+eDep.GetExitX()) , 0.5*(eDep.GetEntryY()+eDep.GetExitY()), 0.5*(eDep.GetEntryZ()+eDep.GetExitZ()));
      std::string volume = geom->VolumeName(midpoint);
//      int i_volume = VolumeIndex(volume);
//if (volume.find("CRTDPTOP") == volume.npos){ 
std::cout << "\nHit in volume " << volume << std::endl;
std::cout << "edep.GetID() = " << eDep.GetID() << std::endl;
std::cout << "CRT volume index = " << (eDep.GetID()-1)/8 << std::endl;
std::cout << "CRT channel = " << (eDep.GetID()-1)%8 << std::endl;
std::cout << "edep.GetTrackID() = " << eDep.GetTrackID() << std::endl;
std::cout << "tAvg_fl = " << tAvg_fl << "\t" << "tAvg_int = " << tAvg << std::endl;
std::cout << "Integration window = " << fIntegrationWindow << std::endl; 
std::cout << "energy deposited : = " << eDep.GetEnergyDeposited() << std::endl; 
//}

      crtHitsModuleMap[(eDep.GetID()-1)/8][tAvg/fSamplingTime].emplace_back(CRTVD::Hit((eDep.GetID()-1)%8, volume, eDep.GetEnergyDeposited()), eDep.GetTrackID());
//      crtHitsModuleMap[(eDep.GetID()-1)/8][tAvg/fIntegrationTime].emplace_back(CRTVD::Hit((eDep.GetID()-1)%8, volume, eDep.GetEnergyDeposited()*0.001f*fGeVToADC),eDep.GetTrackID());
//      crtHitsModuleMap[i_volume][tAvg/fIntegrationTime].emplace_back(CRTVD::Hit((eDep.GetID())%64, volume, eDep.GetEnergyDeposited()*0.001f*fGeVToADC),eDep.GetTrackID());
//      mf::LogDebug("TrueTimes") << "Assigned true hit at time " << tAvg << " to bin " << tAvg/fIntegrationTime << ".\n";
    }
  }

std::cout << "\n";

  // Coincidence research : using BOTTOM module as a reference

  // This set stores time bins where signal above threshold is seen by bottom CRT module
  std::map<int, std::set<uint32_t>> timeActiveRegions; //A std::set contains only unique elements.  
  // first integer convention : 0 = bottom only, 1 = top only, 2 = coincidence
  // declare hits module map that we will keep track for triggering
  std::map<int, std::map<time, std::vector<std::pair<CRTVD::Hit, int>>>> crtTrackedHitsModuleMap;
  // first integer convention : 0 = bottom only, 1 = top only, 2 = coincidence

  // retrieve bottom CRT hits (mapped by time) 
  const auto botCRThitsMappedByTime = crtHitsModuleMap[1]; // 0 = top module, 1 = bottom module

  // Objetcs to keep track of 
  std::map<time, std::vector<std::pair<CRTVD::Hit, int>>> bottomHitsInActiveRegions; // internal integer corresponds to trackID

  time dummy = -999999999;
  time prevWindow = dummy; // dumb init of current time window. Must be small enough.
  // Loop over hits in bottom crt module and look for regions with activity
  // hits are ascendingly sorted w.r.t time
  int count = -1;
  for (const auto& [bintime, hitPairs] : botCRThitsMappedByTime){
    count++;
    std::cout << "Investigating hit at t = " << bintime*fSamplingTime << std::endl;

    time prevWindowUpLimit  = prevWindow+(fIntegrationWindow+fDeadTime)/fSamplingTime;

std::cout << "prev time window = [ " << prevWindow*fSamplingTime << " ; " << prevWindowUpLimit*fSamplingTime << std::endl;

    if ( prevWindow!=dummy && (bintime >= prevWindow && bintime <= prevWindowUpLimit) ) continue; // reject hits not in current integration window

std::cout << "\t---> not in previous time integration window, let's continue !" << std::endl;

    // integrate energy deposited in time window
    float sigIntegrationWindow = 0;
    for (auto p : hitPairs) sigIntegrationWindow += p.first.Edep();
//    for (time t=bintime; t<bintime+fIntegrationWindow; t++) for (auto p : hitPairs) sigIntegrationWindow += p.first.Edep();

std::cout << "Total signal at time bin : " << bintime*fSamplingTime << " is Edep = " << sigIntegrationWindow << std::endl;
      // skip current bin time if associated energy deposited is below threshold
      if (sigIntegrationWindow < fEnergyThreshold) continue;
std::cout << "\t---> passed threshold detection !! " << std::endl;

        // if signal is above threshold, keep track of time bin index
        timeActiveRegions[0].insert(bintime); // keep it in a std::set for later purposes
          prevWindow = bintime; // keep it in varaible for newt iteration loop
          // also keep track of all the hits which are in integration window (not only the ones in current bin time)
          for (const auto& pairHitsInReadoutWindow : botCRThitsMappedByTime) 
            {
            time t = pairHitsInReadoutWindow.first;
            if ( t < prevWindow && t > prevWindowUpLimit) continue;
            for (auto v : pairHitsInReadoutWindow.second) bottomHitsInActiveRegions[bintime].emplace_back(v);
            } // end loop over secondary bottom hits
         //} // end if
    } // end primary loop over bottom module hits

std::cout << "\n------- END Bottom hit search -------" << std::endl;
// CHECK
std::cout << "\n--- Check ---" << std::endl;
for (auto pairHits : bottomHitsInActiveRegions){
std::cout << "\nFound active region at bin time " << pairHits.first*fSamplingTime << std::endl;
bool haselement = (timeActiveRegions[0].find(pairHits.first) == timeActiveRegions[0].end());
std::cout << "Is this time stored in std::set object ? --> " << haselement << std::endl;
}




  std::set<uint32_t> topModuleActiveRegion; //A std::set contains only unique elements.  
  std::set<uint32_t> coincTimes; //A std::set contains only unique elements.  
  // Retrieve hits in top crt module
  const auto topCRThitsMappedByTime = crtHitsModuleMap[0]; // 0 = top module, 1 = bottom module

  // Loop over top module hits 
  for (const auto& [topbintime, tophitPairs] : topCRThitsMappedByTime){

    // get total energy deposited in current time window
    float sig = 0.;
    for (auto p : tophitPairs) sig += p.first.Edep();
    if (sig<fEnergyThreshold) continue;

    int keeptrkidx = 1;
    int keeptracktime = topbintime;
    // First look for coincidences with bottom crt module
    // loop over active time regions of bottom CRT module
    for (const auto botbintime : timeActiveRegions[0]){

      // check if current time bin of top hit is any coincidence window w.r.t bottom module
      if ( topbintime>(botbintime-fUpwardWindow) && topbintime<(botbintime+fDownwardWindow) ){
        // if coincidence is found, check that signal in top module is above threshold
           timeActiveRegions[2].insert(botbintime); // add the time coinc into coinc set
           timeActiveRegions[0].erase(timeActiveRegions[0].find(botbintime)); // remove the time coinc from bottom crt module only trigger
           keeptrkidx = 2;
           keeptracktime = botbintime;
           break;
        } // end if is in coinc time window
    } // end for

    // keep track of time to trigger top crt module only (if no trigger) 
    if (keeptrkidx == 1) timeActiveRegions[1].insert(topbintime);

    // loop again over all top crct hits and keep track of the ones within integration window
    for (const auto& topHitsInReadoutWindow : topCRThitsMappedByTime){
        time t = topHitsInReadoutWindow.first;
        if ( t<topbintime && t>(topbintime+fIntegrationWindow/fSamplingTime)) continue;
        for (auto v : topHitsInReadoutWindow.second) crtTrackedHitsModuleMap[keeptrkidx][keeptracktime].emplace_back(v);
       } // end loop over secondary bottom hits

  } // end loop over top crt hits


std::cout << "\n--- Check coincidences ---\n";
for (time t : timeActiveRegions[2]){
  std::cout << "Found coinc time at bottom time = " << t << std::endl;
}
std::cout << "\n--- END Check coincidences ---\n\n";

  // store bottom crt module triggers
  for (time t : timeActiveRegions[0]){
  std::vector<CRTVD::Hit> hits;
    for (auto hp : bottomHitsInActiveRegions[t]) hits.push_back(hp.first);
    trigCol->emplace_back(0, t*fSamplingTime, std::move(hits)); // probably wrong and work to do here 
  }

//  std::map<time, std::vector<std::pair<CRTVD::Hit, int>>> hitsInActiveRegions; // internal integer corresponds to trackID

// int crtChannel = 0;              
//time timestamp = 12;
//  auto const mcptr = makeMCParticlePtr(index);
//  partToTrigger->addSingle(mcptr, makeTrigPtr(trigCol->size()-1));
//  trigCol->emplace_back(crtChannel, timestamp*fIntegrationTime, std::move(hits)); 
  // -- Put Triggers and Assns into the event
  std::cout << "CreateTrigger : putting " << trigCol->size() << " CRTVD::Triggers into the event at the end of analyze().\n";
//  mf::LogDebug("CreateTrigger") << "Putting " << trigCol->size() << " CRT::Triggers into the event at the end of analyze().\n";
  e.put(std::move(trigCol));
//  e.put(std::move(partToTrigger));


}


DEFINE_ART_MODULE(CRT::CRTVDSim)
