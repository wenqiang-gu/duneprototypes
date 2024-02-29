# dunetpc/dune/Protodune/singlephase/Utility/CMakeLists.txt

art_make(BASENAME_ONLY
         LIB_LIBRARIES
         dunecore::ArtSupport
         dunecore::DuneCommon_Utility
         art::Utilities
         canvas::canvas
         cetlib::cetlib
         ROOT::Core ROOT::Hist ROOT::Tree
         NO_PLUGINS
        ) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use art_make_library(), art_dictonary(), and cet_build_plugin() with explicit source lists and plugin base types

# Install the Root dictionary in the build and product lib directories.
#install(FILES TickModTreeData_Dict_rdict.pcm DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
#install(FILES TickModTreeData_Dict_rdict.pcm DESTINATION ${CMAKE_INSTALL_PREFIX}/${${product}_lib_dir})

install_source()
install_headers()

add_subdirectory(test)
