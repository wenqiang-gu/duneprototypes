

cet_build_plugin(
           CRTGen art::module
           nusimdata::SimulationBase
           cetlib::cetlib
           cetlib_except::cetlib_except
           fhiclcpp::fhiclcpp
           CLHEP::CLHEP
           larcorealg_Geometry
           larcoreobj_SummaryData
           larsim_EventGenerator
           nurandom::RandomUtils_NuRandomService_service
           art::Framework_Services_Registry
           larsim_MCCheater_ParticleInventoryService 
           art_root_io::tfile_support
           CLHEP::CLHEP
           messagefacility::MF_MessageLogger
  	   ROOT::Core ROOT::Hist ROOT::Tree
           ROOT::Geom
           ROOT::XMLIO
           ROOT::Gdml
)
if(FALSE)
cet_build_plugin(
			Purity art::module
			dunesim_DetSim
                        lardataobj_RecoBase
                        larreco_Calorimetry
                        larcorealg_Geometry
                        larcore_Geometry_Geometry_service
                        lardata_ArtDataHelper
                        lardata_Utilities
                        lardataobj_Simulation
                        larsim_MCCheater_BackTrackerService
                        larsim_MCCheater_ParticleInventoryService_service
                        larsim_MCCheater_ParticleInventoryService 
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        
                        art::Persistency_Common canvas
                        art::Persistency_Provenance canvas
                        art::Utilities canvas
                        messagefacility::MF_MessageLogger
                        
                        cetlib::cetlib cetlib_except
                        ROOT::Core ROOT::Hist ROOT::Tree
                        BASENAME_ONLY
)
endif()
cet_build_plugin(
			DEdxdp art::module
			dunesim_DetSim
                        lardataobj_RecoBase
                        larreco_Calorimetry
                        larcorealg_Geometry
                        larcore_Geometry_Geometry_service
                        lardata_ArtDataHelper
                        lardata_Utilities
                        lardataobj_Simulation
                        larsim_MCCheater_BackTrackerService_service
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        
                        art::Persistency_Common canvas
                        art::Persistency_Provenance canvas
                        art::Utilities canvas
                        messagefacility::MF_MessageLogger
                        
                        cetlib::cetlib cetlib_except
                        ROOT::Core ROOT::Hist ROOT::Tree
                        BASENAME_ONLY
)

cet_build_plugin(
	  AnaRootParser art::module
	  larcore_Geometry_Geometry_service
	  larsim_Simulation nug4::ParticleNavigation lardataobj_Simulation
          larsim_MCCheater_ParticleInventoryService 
	  larsim_MCCheater_BackTrackerService
	  larsim_MCCheater_BackTrackerService_service
          larsim_MCCheater_ParticleInventoryService_service
	  lardata_Utilities
	  larevt_Filters
	  lardataobj_RawData
	  lardataobj_RecoBase
	  lardata_RecoObjects
	  lardata_ArtDataHelper
	  lardataalg_DetectorInfo
	  larcorealg_Geometry
	  larcorealg_GeoAlgo
	  larreco_RecoAlg
	  larpandora_LArPandoraInterface
	  dunecore::SignalShapingServiceDUNE10kt_service
	  dunecore::SignalShapingServiceDUNE35t_service
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
	  
	  fhiclcpp::fhiclcpp
	  cetlib::cetlib cetlib_except
          ROOT::Geom
          ROOT::XMLIO
          ROOT::Gdml
	  ROOT::Core ROOT::Hist ROOT::Tree
)



install_headers()
install_fhicl()
install_source()
install_scripts()

add_subdirectory(Light)
add_subdirectory(RawDecoding)
add_subdirectory(fcl)
#add_subdirectory(test)
