
FILE (GLOB src_files *.cxx)

art_make_library(  LIBRARY_NAME  BeamData
		             SOURCE ${src_files}
			     LIBRARIES			
	                wda::wda
			ifbeam::ifbeam
#			IFBeam_service
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
                        
                        cetlib::cetlib cetlib_except
			fhiclcpp::fhiclcpp
                        ROOT_BASIC_LIB_LIST
			
        )

#art_make_exec( NAME RunInfoDUNE
#               SOURCE RunInfoDUNE.cc                 
#               LIBRARIES
#			RunHistoryDUNE
                         #wda
			 # nuevdb::IFDatabase
#               )

#art_make_exec( NAME getDUNEPedestals
#               SOURCE getDUNEPedestals.cc                 
#               LIBRARIES
#			RunHistoryDUNE
#               )


install_headers()
install_fhicl()
install_source()

#add_subdirectory(test)
#add_subdirectory(ProtoDUNEBeamSpill)
