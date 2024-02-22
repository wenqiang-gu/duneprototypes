cet_build_plugin(VDColdboxChannelRanges  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(VDColdboxChannelGroups  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )


cet_build_plugin(VDColdboxOnlineChannel  art::tool
                duneprototypes_Coldbox_vd_ChannelMap_VDColdboxChannelMapService_service
                dunecore_ArtSupport
                art::Framework_Services_Registry
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(CrpChannelRanges  art::tool
                dunecore_ArtSupport
                dunecore_DuneCommon_Utility
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )

cet_build_plugin(CrpChannelGroups  art::tool
                dunecore_ArtSupport
                dunecore_DuneCommon_Utility
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT::Core ROOT::Hist ROOT::Tree
             )


install_fhicl()

add_subdirectory(test)
