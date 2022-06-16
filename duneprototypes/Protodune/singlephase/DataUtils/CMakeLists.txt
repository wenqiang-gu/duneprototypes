
art_make( LIBRARY_NAME ProtoDUNEDataUtils
	  LIB_LIBRARIES
                        dunecore_ArtSupport
                        ifbeam::ifbeam
                        ifdh_art::IFBeam_service
                        lardataobj_RawData
                        lardataobj_RecoBase
                        larreco_RecoAlg
                        larreco_Calorimetry
                        larreco_RecoAlg_PMAlg
                        larcorealg_Geometry
                        larcore_Geometry_Geometry_service
                        lardata_ArtDataHelper
                        lardata_Utilities
                        larsim_Simulation 
                        lardataobj_Simulation
                        dunecore_DuneObj
                        dunepdlegacy_Services_ChannelMap_PdspChannelMapService_service
                        larsim_MCCheater_PhotonBackTrackerService_service
                        larsim_MCCheater_BackTrackerService_service
                        larsim_MCCheater_ParticleInventoryService_service
                        nug4::ParticleNavigation 
                        nusimdata::SimulationBase
                        ART_FRAMEWORK_CORE
                        ART_FRAMEWORK_PRINCIPAL
                        ART_FRAMEWORK_SERVICES_REGISTRY
                        ART_ROOT_IO_TFILE_SUPPORT
                        ROOT::Core
                        ART_ROOT_IO_TFILESERVICE_SERVICE
                        art::Persistency_Common canvas
                        art::Persistency_Provenance canvas
                        art::Utilities canvas
                        messagefacility::MF_MessageLogger
                        fhiclcpp::fhiclcpp
                        cetlib::cetlib cetlib_except
                        ROOT_BASIC_LIB_LIST
	  MODULE_LIBRARIES
	  ProtoDUNEDataUtils
)

install_headers()
install_fhicl()
install_source()
install_scripts()
