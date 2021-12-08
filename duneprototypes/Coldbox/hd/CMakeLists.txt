
cet_build_plugin(HDColdboxDataInterface   art::tool
                                     art_utilities
                                     canvas
                                     cetlib
                                     cetlib_except
                        lardataobj_RawData
                        dune_raw_data::dune-raw-data_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq-core_Utilities
                        dune_raw_data::dune-raw-data_Services_ChannelMap_PdspChannelMapService_service
                        HDF5Utils
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
             )


add_subdirectory(fcl)


install_headers()
install_fhicl()
install_source()
install_scripts()
