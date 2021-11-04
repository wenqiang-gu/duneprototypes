
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
 			art_Persistency_Common canvas
			art_Persistency_Provenance canvas
			art_Utilities canvas
                        messagefacility::MF_MessageLogger
                        
                        cetlib cetlib_except
			FHICLCPP
                        ROOT_BASIC_LIB_LIST
			${Boost_SYSTEM_LIBRARY}
        )

#art_make_exec( NAME RunInfoDUNE
#               SOURCE RunInfoDUNE.cc                 
#               LIBRARIES
#			RunHistoryDUNE
                         #wda
			 # nuevdb_IFDatabase
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
