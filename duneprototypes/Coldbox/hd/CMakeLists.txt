
cet_build_plugin(HDColdboxDataInterface   art::tool
                                     art_utilities
                                     canvas
                                     cetlib
                                     cetlib_except
                        lardataobj_RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_PdspChannelMapService_service
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
             )


cet_build_plugin(HDColdboxDataInterfaceWIB3   art::tool
                        art_utilities
                        canvas::canvas
                        cetlib::cetlib
                        cetlib_except::cetlib_except
                        lardataobj::RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        duneprototypes_Protodune_hd_ChannelMap_PD2HDChannelMapService_service
			art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        art::Persistency_Provenance
                        messagefacility::MF_MessageLogger
                        ROOT_BASIC_LIB_LIST
                        dunecore::HDF5Utils
                        LIBHDF5
             )

cet_build_plugin(HDColdboxDAQWriter art::module
                        lardataobj_RawData
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        ART_PERSISTENCY_PROVENANCE
                        messagefacility::MF_MessageLogger
                        CETLIB
                        ROOT_BASIC_LIB_LIST
			LIBHDF5
                        duneprototypes_Protodune_hd_ChannelMap_PD2HDChannelMapService_service
                        BASENAME_ONLY
)


add_subdirectory(fcl)
add_subdirectory(ChannelMap)

install_headers()
install_fhicl()
install_source()
install_scripts()
