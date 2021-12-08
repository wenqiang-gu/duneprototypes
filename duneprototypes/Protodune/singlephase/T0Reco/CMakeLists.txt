
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
                           ART_FRAMEWORK_CORE
                           ART_FRAMEWORK_PRINCIPAL
                           ART_FRAMEWORK_SERVICES_REGISTRY
                           ART_ROOT_IO_TFILE_SUPPORT
			   ROOT::Core
                           ART_ROOT_IO_TFILESERVICE_SERVICE
                           ART_PERSISTENCY_PROVENANCE
                           art::Persistency_Common
                           art::Utilities canvas
                           messagefacility::MF_MessageLogger
                           fhiclcpp::fhiclcpp
                           cetlib::cetlib cetlib_except
                           ROOT_BASIC_LIB_LIST
	   		   #ProtoDUNEDataUtils
         )

install_headers()
install_fhicl()
install_source()

