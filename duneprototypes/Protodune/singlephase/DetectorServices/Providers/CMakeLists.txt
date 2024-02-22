cet_enable_asserts()
art_make( NO_PLUGINS
          LIBRARY_NAME  ProtoDUNEspDataProviders
          LIB_LIBRARIES
                        lardataalg_DetectorInfo
  	                wda::wda
	                nuevdb::IFDatabase
                        larcorealg_Geometry
                        art_Framework_Core
                        art::Framework_Principal
                        art_root_io::RootDB
                        art::Utilities
                        messagefacility::MF_MessageLogger
                        art::Framework_Services_Registry
                        cetlib::cetlib cetlib_except
                        canvas::canvas
                        ifdh::ifdh
                        fhiclcpp::fhiclcpp
                        ROOT::Core ROOT::Hist ROOT::Tree
                        
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types


install_headers()
install_fhicl()
install_source()
