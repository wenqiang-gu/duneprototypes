
#include_directories( "$ENV{DUNEPDSPRCE_INC}" ) 
#cet_find_library( RCEDAMLIB NAMES protodune-dam PATHS ENV DUNEPDSPRCE_LIB NO_DEFAULT_PATH )

cet_build_plugin(IcebergTPCRawDecoder art::module
                        lardataobj::RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_IcebergChannelMapService_service
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        art::Persistency_Provenance
                        messagefacility::MF_MessageLogger
                        ROOT_BASIC_LIB_LIST
                        dunepdlegacy_rce_dataaccess
                        z
                        BASENAME_ONLY
)


cet_build_plugin(IcebergFELIXBufferDecoderMarch2021 art::module
                        lardataobj::RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_IcebergChannelMapService_service
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        art::Persistency_Provenance
                        messagefacility::MF_MessageLogger
                        ROOT_BASIC_LIB_LIST
                        dunepdlegacy_rce_dataaccess
                        z
                        cetlib::cetlib
                        BASENAME_ONLY
)

cet_build_plugin(IcebergDataInterface   art::tool
                                     art_utilities
                                     canvas::canvas
                                     cetlib::cetlib
                                     cetlib_except::cetlib_except
                        lardataobj::RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_IcebergChannelMapService_service
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        art::Persistency_Provenance
                        messagefacility::MF_MessageLogger
                        ROOT_BASIC_LIB_LIST
                        dunepdlegacy_rce_dataaccess
                        z
             )


cet_build_plugin(IcebergDataInterfaceFELIXBufferMarch2021   art::tool
                                     art_utilities
                                     canvas::canvas
                                     cetlib::cetlib
                                     cetlib_except::cetlib_except
                        lardataobj::RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_IcebergChannelMapService_service
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        art::Persistency_Provenance
                        messagefacility::MF_MessageLogger
                        ROOT_BASIC_LIB_LIST
                        z
             )


cet_build_plugin(IcebergHDF5DataInterface   art::tool
                        art_utilities
                        canvas::canvas
                        cetlib::cetlib
                        cetlib_except::cetlib_except
                        lardataobj::RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_IcebergChannelMapService_service
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


install_headers()
install_fhicl()
install_source()
install_scripts()
