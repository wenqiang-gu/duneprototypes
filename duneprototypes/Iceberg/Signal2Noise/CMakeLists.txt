art_make( 
  LIB_LIBRARIES 
  lardataobj_RecoBase
  larana_OpticalDetector_OpHitFinder
  larsim_MCSTReco
  lardataobj_MCBase
  larcorealg_Geometry
  larcore_Geometry_Geometry_service
  larsim_PhotonPropagation_PhotonVisibilityService_service
  larana_OpticalDetector_OpDigiProperties_service
  lardata_Utilities
  nusimdata::SimulationBase
  art::Framework_Core
  art::Framework_Principal
  art::Framework_Services_Registry
  art_root_io::tfile_support
  ROOT::Core
  
  
  art::Persistency_Common
  art::Utilities
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
  cetlib::cetlib cetlib_except
  ROOT::Core ROOT::Hist ROOT::Tree
  MODULE_LIBRARIES larana_OpticalDetector_OpDigiProperties_service
  larana_OpticalDetector_OpHitFinder
  larana_OpticalDetector
  larsim_PhotonPropagation_PhotonVisibilityService_service
  larsim_MCCheater_BackTrackerService_service
  larsim_MCCheater_ParticleInventoryService_service
  larevt_Filters
  lardata_RecoObjects
  lardataalg_DetectorInfo
  lardataobj_RecoBase
  lardataobj_MCBase
  larreco_RecoAlg
  larcorealg_Geometry
  larcore_Geometry_Geometry_service
  larsim_Simulation nug4::ParticleNavigation lardataobj_Simulation
  lardata_Utilities
  lardataobj_RawData
  larreco_Calorimetry
  lardataobj_AnalysisBase
  nurandom::RandomUtils_NuRandomService_service
  nusimdata::SimulationBase
  art::Framework_Core
  art::Framework_Principal
  art::Framework_Services_Registry
  art_root_io::tfile_support
  ROOT::Core
  
  
  art::Persistency_Provenance
  art::Persistency_Common
  art::Utilities
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
  cetlib::cetlib cetlib_except
  ROOT::Core ROOT::Hist ROOT::Tree
  SERVICE_LIBRARIES
  larcorealg_Geometry
  larcore_Geometry_Geometry_service
  art::Framework_Core
  art::Framework_Principal
  art::Framework_Services_Registry
  art_root_io::tfile_support
  ROOT::Core
  
  art::Persistency_Common
  art::Utilities
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
  cetlib::cetlib cetlib_except
  CLHEP::CLHEP
  ROOT::Core ROOT::Hist ROOT::Tree
  ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_fhicl()
install_source()
#install_fw( LIST toyWaveform.txt)
#SUBDIRNAME (OpticalDetector )
