
cet_build_plugin(VDColdboxDataInterface   art::tool
                                     art_utilities
                                     canvas::canvas
                                     cetlib::cetlib
                                     cetlib_except::cetlib_except
                        lardataobj_RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_PdspChannelMapService_service
                        dunecore::HDF5Utils
                        LIBHDF5
                        duneprototypes_Coldbox_vd_ChannelMap_VDColdboxChannelMapService_service
                        ART_FRAMEWORK_CORE
                        ART_FRAMEWORK_PRINCIPAL
                        ART_FRAMEWORK_SERVICES_REGISTRY
                        ART_ROOT_IO_TFILE_SUPPORT
                        ROOT::Core
                        ART_ROOT_IO_TFILESERVICE_SERVICE
                        ART_PERSISTENCY_PROVENANCE
                        messagefacility::MF_MessageLogger
                        CETLIB
                        ROOT_BASIC_LIB_LIST
             )


simple_plugin(VDColdboxTDERawInput "source"
  duneprototypes_Coldbox_vd_ChannelMap_VDColdboxTDEChannelMapService_service
  art::Framework_Services_Registry
  art_utilities
  canvas::canvas
  cetlib::cetlib
  cetlib_except::cetlib_except
  lardataobj_RawData
  ART_FRAMEWORK_CORE
  ART_FRAMEWORK_PRINCIPAL
  ART_FRAMEWORK_SERVICES_REGISTRY
  ART_ROOT_IO_TFILE_SUPPORT
  ROOT::Core
  ART_ROOT_IO_TFILESERVICE_SERVICE
  ART_PERSISTENCY_PROVENANCE
  messagefacility::MF_MessageLogger
  ROOT_BASIC_LIB_LIST
)


cet_build_plugin(VDColdboxPDSDecoder art::module
                 lardataobj_RawData
                 lardataobj_RecoBase
                 dunecore::HDF5Utils
                 LIBHDF5
                 ART_FRAMEWORK_CORE
                 ART_FRAMEWORK_PRINCIPAL
                 ART_FRAMEWORK_SERVICES_REGISTRY
                 ART_ROOT_IO_TFILE_SUPPORT
                 ROOT::Core
                 ART_ROOT_IO_TFILESERVICE_SERVICE
                 ART_PERSISTENCY_PROVENANCE
                 messagefacility::MF_MessageLogger
                 CETLIB
                 ROOT_BASIC_LIB_LIST
                 BASENAME_ONLY
)



add_subdirectory(ChannelMap)
add_subdirectory(fcl)
add_subdirectory(Tools)

install_headers()
install_fhicl()
install_source()
install_scripts()
