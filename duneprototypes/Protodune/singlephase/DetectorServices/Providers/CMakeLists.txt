cet_enable_asserts()
art_make( NO_PLUGINS
          LIBRARY_NAME  ProtoDUNEspDataProviders
          LIB_LIBRARIES
                        lardataalg_DetectorInfo
  	                wda
	                nuevdb::IFDatabase
                        larcorealg_Geometry
                        art_Framework_Core
                        art::Framework_Principal
                        ART_ROOT_IO_ROOTDB
                        art::Utilities
                        messagefacility::MF_MessageLogger
                        ART_FRAMEWORK_SERVICES_REGISTRY
                        cetlib::cetlib cetlib_except
                        canvas::canvas
                        ifdh::ifdh
                        fhiclcpp::fhiclcpp
                        ROOT_BASIC_LIB_LIST
                        
        )


install_headers()
install_fhicl()
install_source()
