
art_make( BASENAME_ONLY
          MODULE_LIBRARIES lardataobj_RecoBase
                           lardataalg_DetectorInfo
                           lardataobj_AnalysisBase
                           larcorealg_Geometry
                           larcore_Geometry_Geometry_service
                           lardata_Utilities
                           larsim_Simulation
			   lardataobj_Simulation
                           larsim_MCCheater_BackTrackerService_service
                           larsim_MCCheater_ParticleInventoryService_service
                           nusimdata::SimulationBase
                           art::Framework_Core
                           art::Framework_Principal
                           art::Framework_Services_Registry
                           art_root_io::tfile_support
			   ROOT::Core
                           art::Persistency_Provenance
                           art::Persistency_Common
                           art::Utilities canvas
                           messagefacility::MF_MessageLogger
                           fhiclcpp::fhiclcpp
                           cetlib::cetlib cetlib_except
                           ROOT::Core ROOT::Hist ROOT::Tree
	   		   #ProtoDUNEDataUtils
         ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_fhicl()
install_source()

