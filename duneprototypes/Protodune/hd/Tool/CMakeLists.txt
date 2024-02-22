# duneprototypes/Protodune/hd/Tool/CMakeLists.txt

# David Adams
# June 2023

# Add plugin for each tool.

cet_build_plugin(PdhdChannelRanges   art::tool
                 dunecore_ArtSupport
                 art::Utilities canvas
                 cetlib::cetlib cetlib_except
                 art::Framework_Services_Registry
                 ROOT::Core ROOT::Hist ROOT::Tree
                )

cet_build_plugin(PdhdChannelGroups   art::tool
                 dunecore_ArtSupport
                 art::Utilities canvas
                 cetlib::cetlib cetlib_except
                 art::Framework_Services_Registry
                 ROOT::Core ROOT::Hist ROOT::Tree
                )

add_subdirectory(test)
