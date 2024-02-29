include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - avoid directory-scope functions: use target_link_libraries() with target semantics or target_include_directories() whenever possible

art_make( LIB_LIBRARIES
                        lardataobj::Simulation
                        nusimdata::SimulationBase
			art::Framework_Core
			art::Framework_Principal
			art::Framework_Services_Registry
			art_root_io_tfile_support
			ROOT::Core
 			art::Persistency_Common
			art::Persistency_Provenance
			art::Utilities canvas::canvas
			cetlib::cetlib cetlib_except::cetlib_except
			lardataobj::AnalysisBase
            DICT_LIBRARIES
                        lardataobj::Simulation
                        nusimdata::SimulationBase

) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

install_headers()
install_source()
