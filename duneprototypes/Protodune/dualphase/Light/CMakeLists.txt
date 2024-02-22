art_make( BASENAME_ONLY MODULE_LIBRARIES
  larcore_Geometry_Geometry_service
  larcorealg_Geometry
  lardataobj_RawData
  larsim_MCCheater_ParticleInventoryService
  larsim::MCCheater_BackTrackerService_service
  larsim::MCCheater_PhotonBackTrackerService_service
  larsim::MCCheater_ParticleInventoryService_service
  larsim_Simulation
  nug4::ParticleNavigation
  lardataobj_Simulation
  nusimdata::SimulationBase	
  art::Framework_Core
  art::Framework_Principal
  art::Framework_Services_Registry
  art_root_io::tfile_support
  ROOT::Core
  art::Persistency_Common canvas
  art::Persistency_Provenance canvas
  art::Utilities canvas
  messagefacility::MF_MessageLogger
  ROOT::Geom
  cetlib::cetlib cetlib_except
  ROOT::Core ROOT::Hist ROOT::Tree
  ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_fhicl()
install_headers()
install_source()
