////////////////////////////////////////////////////////////////////////
// Class:       RecoEff
// Module Type: analyzer
// File:        RecoEff_module.cc
// Author:      D.Stefan and R.Sulej
//
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Framework/Services/Optional/TFileService.h"
#include "canvas/Utilities/InputTag.h"
#include "fhiclcpp/types/Atom.h"
#include "messagefacility/MessageLogger/MessageLogger.h"

#include "canvas/Persistency/Common/FindManyP.h"

//#include "larcorealg/Geometry/GeometryCore.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Track.h"
#include "larsim/MCCheater/BackTracker.h"
#include "nusimdata/SimulationBase/MCParticle.h"

#include "TH1.h"
#include "TEfficiency.h"
#include "TTree.h"

namespace pdune
{
	class RecoEff;
}

class pdune::RecoEff : public art::EDAnalyzer {
public:

  enum EFilterMode { kCosmic, kBeam, kPrimary, kSecondary };

  struct Config {
      using Name = fhicl::Name;
      using Comment = fhicl::Comment;

      fhicl::Atom<art::InputTag> SimulationLabel { Name("SimulationLabel"), Comment("tag of simulation producer") };

      fhicl::Atom<art::InputTag> HitModuleLabel { Name("HitModuleLabel"), Comment("tag of hits producer") };

      fhicl::Atom<art::InputTag> TrackModuleLabel { Name("TrackModuleLabel"), Comment("tag of track producer") };
    
      fhicl::Atom<size_t> MinHitsPerPlane { Name("MinHitsPerPlane"), Comment("min hits per plane for a reconstructable MC particle") };

      fhicl::Sequence<std::string> Filters { Name("Filters"), Comment("on which particles the efficiency is calculated") };

      fhicl::Sequence<int> Pdg { Name("Pdg"), Comment("which PDG code, or 0 for all particles") };
      
      fhicl::Atom<size_t> EffHitMax { Name("EffHitMax"), Comment("max hits per MC particle in the track efficiency") };
      fhicl::Atom<size_t> EffHitBins { Name("EffHitBins"), Comment("number of bins in the track efficiency") };
  };
  using Parameters = art::EDAnalyzer::Table<Config>;

  explicit RecoEff(Parameters const& config);

  RecoEff(RecoEff const &) = delete;
  RecoEff(RecoEff &&) = delete;
  RecoEff & operator = (RecoEff const &) = delete;
  RecoEff & operator = (RecoEff &&) = delete;

  void analyze(art::Event const & evt) override;
  void beginJob() override;
  void endJob() override;

private:
  void ResetVars();
  
  TH1D* fDenominatorHist;
  TH1D* fNominatorHist;

  TTree *fEventTree;
  TTree *fTrkTree;

  int fRun, fEvent;
  int fNRecoTracks;
  int fReconstructable;
  int fMatched;

  float fEnGen, fEkGen;
  float fT0;

  float fTrkPurityPerPlane[3], fTrkCompletnessPerPlane[3]; // fixed size, don't expect more than 3 planes, less is OK
  float fTrkPurity, fTrkCompletness; // all planes together
  int fTrkPid, fTrkSize, fTrkMatched;

  art::InputTag fSimulationLabel;
  art::InputTag fHitModuleLabel;
  art::InputTag fTrackModuleLabel;
  size_t fMinHitsPerPlane;

  std::vector< EFilterMode > fFilters;
  std::vector< int > fPdg;

  size_t fEffHitMax, fEffHitBins;

  //geo::GeometryCore const* fGeometry;
};

pdune::RecoEff::RecoEff(Parameters const& config) : EDAnalyzer(config),
    fSimulationLabel(config().SimulationLabel()),
    fHitModuleLabel(config().HitModuleLabel()),
    fTrackModuleLabel(config().TrackModuleLabel()),
    fMinHitsPerPlane(config().MinHitsPerPlane()),
    fPdg(config().Pdg()),
    fEffHitMax(config().EffHitMax()),
    fEffHitBins(config().EffHitBins())
    //fGeometry( &*(art::ServiceHandle<geo::Geometry>()) )
{
    auto flt = config().Filters();
    for (const auto & s : flt)
    {
        if (s == "cosmic")       { fFilters.push_back(pdune::RecoEff::kCosmic);  }
        else if (s == "beam")    { fFilters.push_back(pdune::RecoEff::kBeam);    }
        else if (s == "primary") { fFilters.push_back(pdune::RecoEff::kPrimary); }
        else if (s == "second")  { fFilters.push_back(pdune::RecoEff::kSecondary); }
        else { std::cout << "unsupported filter name: " << s << std::endl; }
    }
}

void pdune::RecoEff::beginJob()
{
	art::ServiceHandle<art::TFileService> tfs;
	
	fDenominatorHist = tfs->make<TH1D>("Denominator", "all reconstructable particles", fEffHitBins, 0., fEffHitMax);
	fNominatorHist = tfs->make<TH1D>("Nominator", "reconstructed and matched tracks", fEffHitBins, 0., fEffHitMax);

	fEventTree = tfs->make<TTree>("events", "summary tree");
	fEventTree->Branch("fRun", &fRun, "fRun/I");
	fEventTree->Branch("fEvent", &fEvent, "fEvent/I");
	fEventTree->Branch("fEnGen", &fEnGen, "fEnGen/F");
	fEventTree->Branch("fEkGen", &fEkGen, "fEkGen/F");
	fEventTree->Branch("fT0", &fT0, "fT0/F");
	fEventTree->Branch("fNRecoTracks", &fNRecoTracks, "fNRecoTracks/I");
	fEventTree->Branch("fReconstructable", &fReconstructable, "fReconstructable/I");
	fEventTree->Branch("fMatched", &fMatched, "fMatched/I");

	fTrkTree = tfs->make<TTree>("tracks", "track metricks");
	fTrkTree->Branch("fTrkPurity", &fTrkPurity, "fTrkPurity/F");
	fTrkTree->Branch("fTrkCompletness", &fTrkCompletness, "fTrkCompletness/F");
	fTrkTree->Branch("fTrkPurityPerPlane", &fTrkPurityPerPlane, "fTrkPurityPerPlane[3]/F");
	fTrkTree->Branch("fTrkCompletnessPerPlane", &fTrkCompletnessPerPlane, "fTrkCompletnessPerPlane[3]/F");
	fTrkTree->Branch("fTrkPid", &fTrkPid, "fTrkPid/I");
	fTrkTree->Branch("fTrkSize", &fTrkSize, "fTrkSize/I");
	fTrkTree->Branch("fTrkMatched", &fTrkMatched, "fTrkMatched/I");
}

void pdune::RecoEff::endJob()
{
    for (int i = 0; i < fDenominatorHist->GetNbinsX(); ++i)
    {
        double nom = fNominatorHist->GetBinContent(i);
        double den = fDenominatorHist->GetBinContent(i);
        std::cout << "nhits: " << i << " nom:" << nom << " den:" << den << std::endl;
        if (nom > den) { fNominatorHist->SetBinContent(i, den); }
    }
    art::ServiceHandle<art::TFileService> tfs;
    TEfficiency* fEfficiency = tfs->makeAndRegister<TEfficiency>("Efficiency", "tracking efficiency", *fNominatorHist, *fDenominatorHist);
    std::cout << "Efficiency created: " << fEfficiency->GetTitle() << std::endl;
}

void pdune::RecoEff::analyze(art::Event const & evt)
{
  ResetVars();
  
  fRun = evt.run();
  fEvent = evt.id().event();

  art::ServiceHandle<cheat::BackTracker> bt;
  
  // we are going to look only for these MC truth particles, which contributed to hits
  // and normalize efficiency to things which indeed generated activity and hits in TPC's:
  // - need a vector of hits associated to these MC particles,
  // - need the energy deposit corresponding to the particle part seen in these hits.
  std::unordered_map<int, std::vector<recob::Hit> > mapTrackIDtoHits;
  std::unordered_map<int, double> mapTrackIDtoHitsEnergyPerPlane[3];
  std::unordered_map<int, double> mapTrackIDtoHitsEnergy;

  auto const & hitListHandle = *evt.getValidHandle< std::vector<recob::Hit> >(fHitModuleLabel);
  for (auto const & h : hitListHandle)
  {
    std::unordered_map<int, double> particleID_E;
    for (auto const & id : bt->HitToTrackID(h)) // loop over std::vector< sim::TrackIDE > contributing to hit h
	{
	    // select only hadronic and muon track, skip EM activity (electron's pdg, negative track id)
        if ((id.trackID > 0) && (abs((bt->TrackIDToParticle(id.trackID))->PdgCode()) != 11))
        {
            particleID_E[id.trackID] += id.energy;
        }
    }

    int best_id = 0;
    double max_e = 0;
    for (auto const & contrib : particleID_E)
    {
        if (contrib.second > max_e) // find particle TrackID corresponding to max energy contribution
        {
            max_e = contrib.second;
            best_id = contrib.first;
        }
    }

    if (max_e > 0)
    {
        mapTrackIDtoHits[best_id].push_back(h);
        mapTrackIDtoHitsEnergyPerPlane[h.WireID().Plane][best_id] += max_e;
        mapTrackIDtoHitsEnergy[best_id] += max_e;
    }
  }
  // ---------------------------------------------------------------------------------------


  // now lets map particles to their associated hits, but with some filtering of things
  // which have a minimal chance to be reconstructed: so at least some hits in two views
  std::unordered_map<int, std::vector< recob::Hit >> mapTrackIDtoHits_filtered;
  for (auto const & p : mapTrackIDtoHits)
  {
    bool skip = false;
    auto origin = bt->TrackIDToMCTruth(p.first)->Origin();
    for (auto f : fFilters)
    {
        switch (f)
        {
            case pdune::RecoEff::kCosmic:  if (origin != simb::kCosmicRay) { skip = true; } break;
            case pdune::RecoEff::kBeam:    if (origin != simb::kSingleParticle) { skip = true; } break;

            case pdune::RecoEff::kPrimary:
                if (bt->TrackIDToParticle(p.first)->Process() != "primary") { skip = true; }
                break;

            case pdune::RecoEff::kSecondary:
                {
                    int mId = bt->TrackIDToParticle(p.first)->Mother();
                    const simb::MCParticle * mother = bt->TrackIDToParticle(mId);
                    if ((mother == 0) || (mother->Process() != "primary")) { skip = true; }
                    break;
                }

            default: break;
        }
    }
    if (skip) { continue; } // skip if any condition failed

    if (!fPdg.empty())
    {
        skip = true;
        for (int code : fPdg) { if (bt->TrackIDToParticle(p.first)->PdgCode() == code) { skip = false; break; } }
        if (skip) { continue; } // skip only if no PDG is matching
    }

    std::unordered_map<geo::View_t, size_t> hit_count;
    for (auto const & h : p.second) { hit_count[h.View()]++; }

    size_t nviews = 0;
    for (auto const & n : hit_count) {  if (n.second > fMinHitsPerPlane) { nviews++; } }
    if (nviews >= 2)
    {
        // passed all conditions, move to filtered maps
        mapTrackIDtoHits_filtered.emplace(p);
    }
  }
  fReconstructable = mapTrackIDtoHits_filtered.size();

  std::cout << "------------ event: " << fEvent << " has reconstructable: " << fReconstructable << std::endl;
  for (auto const &p : mapTrackIDtoHits_filtered)
  {
    std::cout << " : id " << p.first << " size " << p.second.size() << " en: " << mapTrackIDtoHitsEnergy[p.first] << std::endl;
    fDenominatorHist->Fill(p.second.size());
  }
  // ---------------------------------------------------------------------------------------


  // match reconstructed tracks to MC particles
  const auto trkHandle = evt.getValidHandle< std::vector<recob::Track> >(fTrackModuleLabel);
  art::FindManyP< recob::Hit > hitsFromTracks(trkHandle, evt, fTrackModuleLabel);

  fNRecoTracks = trkHandle->size();
  for (size_t t = 0; t < trkHandle->size(); ++t)     // loop over tracks
  {
    // *** here you should select if the reconstructed track is interesting, eg:
    // - associated PFParticle has particlular PDG code
    // - or just the recob::Track has interesting ParticleID

    std::unordered_map<int, double> trkID_E_perPlane[3];
    std::unordered_map<int, double> trkID_E;         // map MC particles to their energy contributed to this track t
    const auto & hits = hitsFromTracks.at(t);
    for (const auto & h : hits)                      // loop over hits assigned to track t
    {
        size_t plane = h->WireID().Plane;
        for (auto const & ide : bt->HitToTrackID(h)) // loop over std::vector< sim::TrackIDE >, for a hit h
        {
            if (mapTrackIDtoHits_filtered.find(ide.trackID) != mapTrackIDtoHits_filtered.end())
            {
                trkID_E_perPlane[plane][ide.trackID] += ide.energy; // ...and sum energies contributed to this reco track
                trkID_E[ide.trackID] += ide.energy;                 // by MC particles which we considered as reconstructable
            }
        }
    }

    // find MC particle which cotributed maximum energy to track t
    int best_id = 0;
    double max_e = 0, tot_e = 0;
    double e_inPlane[3], tot_e_inPlane[3];
    for (auto const & entry : trkID_E)
	{
        tot_e += entry.second;    // sum total energy in the hits in track t
        for (size_t i = 0; i < 3; ++i)
        {
            tot_e_inPlane[i] += trkID_E_perPlane[i][entry.first];
        }

        if (entry.second > max_e) // find track ID corresponding to max energy
        {
            max_e = entry.second;
            best_id = entry.first;

            for (size_t i = 0; i < 3; ++i)
            {
                e_inPlane[i] = trkID_E_perPlane[i][entry.first];
            }
        }
    }

    // check if reco track is matching to MC particle:
    if ((max_e > 0.5 * tot_e) &&                         // MC particle has more than 50% energy contribution to the track
        (max_e > 0.5 * mapTrackIDtoHitsEnergy[best_id])) // track covers more than 50% of energy deposited by MC particle in hits
    {
        fNominatorHist->Fill(mapTrackIDtoHits_filtered[best_id].size());
        fTrkMatched = 1; fMatched++;
    }
    else { fTrkMatched = 0; }

    if (tot_e > 0)
    {
        fTrkPurity = max_e / tot_e;
        fTrkCompletness = max_e /  mapTrackIDtoHitsEnergy[best_id];
        for (size_t i = 0; i < 3; ++i)
        {
            if (tot_e_inPlane[i] > 0) { fTrkPurityPerPlane[i] = e_inPlane[i] / tot_e_inPlane[i]; }
            if (mapTrackIDtoHitsEnergyPerPlane[i][best_id] > 0) { fTrkCompletnessPerPlane[i] = e_inPlane[i] / mapTrackIDtoHitsEnergyPerPlane[i][best_id]; }
        }

        fTrkPid = (*trkHandle)[t].ParticleId();
        fTrkSize = hits.size();
        fTrkTree->Fill();
    }
  }

  if (fMatched > fReconstructable)
  {
    std::cout << "WARNING: matched more reco tracks than selected MC particles: " << fMatched << " > " << fReconstructable << std::endl;
  }

  std::cout << "------------ reconstructed: " << fNRecoTracks << " and then matched to MC particles: " << fMatched
    << " (reconstructable: " << fReconstructable << ")" << std::endl;
  fEventTree->Fill();
}

void pdune::RecoEff::ResetVars()
{
	fRun = 0; fEvent = 0;

	fEnGen = 0; fEkGen = 0;
	fT0 = 0;

    for (size_t i = 0; i < 3; ++i)
    {
        fTrkPurityPerPlane[i] = 0;
        fTrkCompletnessPerPlane[i] = 0;
    }

	fTrkPurity = 0; fTrkCompletness = 0;
	fTrkPid = 0; fTrkSize = 0; fTrkMatched = 0;

	fMatched = 0;
	fNRecoTracks = 0;
	fReconstructable = 0;
}

DEFINE_ART_MODULE(pdune::RecoEff)
