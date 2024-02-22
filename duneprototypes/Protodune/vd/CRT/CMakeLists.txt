include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

art_make( MODULE_LIBRARIES
                        lardataobj_Simulation
                        nusimdata::SimulationBase
			art::Framework_Core
			art::Framework_Principal
			art::Framework_Services_Registry
			art_root_io::tfile_support
			ROOT::Core
 			art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			cetlib::cetlib cetlib_except
			lardataobj_AnalysisBase
			larcorealg_Geometry
			ROOT::Core ROOT::Hist ROOT::Tree
                        larcoreobj_SimpleTypesAndConstants
) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_source()
install_fhicl()
