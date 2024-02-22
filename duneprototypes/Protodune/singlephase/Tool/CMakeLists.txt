# dunetpc/dune/Protodune/singlephase/Tool/CMakeLists.txt

# David Adams
# June 2018

# Add plugin for each tool.

cet_build_plugin(PdspOnlineChannel   art::tool
                dunepdlegacy_Services_ChannelMap_PdspChannelMapService_service
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                art::Framework_Services_Registry
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(IcebergOnlineChannel   art::tool
                dunepdlegacy_Services_ChannelMap_IcebergChannelMapService_service
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                art::Framework_Services_Registry
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(ProtoduneOnlineChannel   art::tool
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                art::Framework_Services_Registry
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(ProtoDuneChannelRanges  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(ApaChannelRanges  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(HalfApaChannelRanges  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(ProtoDuneChannelGroups  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(ApaChannelGroups  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

add_subdirectory(test)
