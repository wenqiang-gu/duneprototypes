cet_build_plugin(VDColdboxChannelRanges  art::tool
                dune_ArtSupport
                art_Utilities canvas
                cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )

cet_build_plugin(VDColdboxChannelGroups  art::tool
                dune_ArtSupport
                art_Utilities canvas
                cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )


cet_build_plugin(VDColdboxOnlineChannel  art::tool
                dune_Coldbox_vd_ChannelMap_VDColdboxChannelMapService_service
                dune_ArtSupport
                ART_FRAMEWORK_SERVICES_REGISTRY
                art_Utilities canvas
                cetlib cetlib_except
                ROOT_BASIC_LIB_LIST
             )


install_fhicl()

add_subdirectory(test)
