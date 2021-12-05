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
  ART_FRAMEWORK_CORE
  ART_FRAMEWORK_PRINCIPAL
  ART_FRAMEWORK_SERVICES_REGISTRY
  ART_ROOT_IO_TFILE_SUPPORT
  ROOT::Core
  ART_ROOT_IO_TFILESERVICE_SERVICE
  ART_FRAMEWORK_SERVICES_OPTIONAL_RANDOMNUMBERGENERATOR_SERVICE
  art::Persistency_Common
  art::Utilities
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
  cetlib::cetlib cetlib_except
  ROOT_BASIC_LIB_LIST
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
  ART_FRAMEWORK_CORE
  ART_FRAMEWORK_PRINCIPAL
  ART_FRAMEWORK_SERVICES_REGISTRY
  ART_ROOT_IO_TFILE_SUPPORT
  ROOT::Core
  ART_ROOT_IO_TFILESERVICE_SERVICE
  ART_FRAMEWORK_SERVICES_OPTIONAL_RANDOMNUMBERGENERATOR_SERVICE
  ART_PERSISTENCY_PROVENANCE
  art::Persistency_Common
  art::Utilities
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
  cetlib::cetlib cetlib_except
  ROOT_BASIC_LIB_LIST
  SERVICE_LIBRARIES
  larcorealg_Geometry
  larcore_Geometry_Geometry_service
  ART_FRAMEWORK_CORE
  ART_FRAMEWORK_PRINCIPAL
  ART_FRAMEWORK_SERVICES_REGISTRY
  ART_ROOT_IO_TFILE_SUPPORT
  ROOT::Core
  ART_ROOT_IO_TFILESERVICE_SERVICE
  art::Persistency_Common
  art::Utilities
  canvas::canvas
  messagefacility::MF_MessageLogger
  fhiclcpp::fhiclcpp
  cetlib::cetlib cetlib_except
  CLHEP
  ROOT_BASIC_LIB_LIST
  )

install_headers()
install_fhicl()
install_source()
#install_fw( LIST toyWaveform.txt)
#SUBDIRNAME (OpticalDetector )
