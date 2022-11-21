cet_build_plugin(VDColdboxChannelRanges  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )

cet_build_plugin(VDColdboxChannelGroups  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )


cet_build_plugin(VDColdboxOnlineChannel  art::tool
                duneprototypes_Coldbox_vd_ChannelMap_VDColdboxChannelMapService_service
                dunecore_ArtSupport
                ART_FRAMEWORK_SERVICES_REGISTRY
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )

cet_build_plugin(CrpChannelRanges  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )

cet_build_plugin(CrpChannelGroups  art::tool
                dunecore_ArtSupport
                art::Utilities canvas
                cetlib::cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )


install_fhicl()

add_subdirectory(test)
