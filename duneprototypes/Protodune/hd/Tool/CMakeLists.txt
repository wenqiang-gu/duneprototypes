# duneprototypes/Protodune/hd/Tool/CMakeLists.txt

# David Adams
# June 2023

# Add plugin for each tool.

cet_build_plugin(PdhdChannelRanges   art::tool
                 dunecore_ArtSupport
                 art::Utilities canvas
                 cetlib::cetlib cetlib_except
                 ART_FRAMEWORK_SERVICES_REGISTRY
                 ROOT_BASIC_LIB_LIST
                )

cet_build_plugin(PdhdChannelGroups   art::tool
                 dunecore_ArtSupport
                 art::Utilities canvas
                 cetlib::cetlib cetlib_except
                 ART_FRAMEWORK_SERVICES_REGISTRY
                 ROOT_BASIC_LIB_LIST
                )

add_subdirectory(test)
