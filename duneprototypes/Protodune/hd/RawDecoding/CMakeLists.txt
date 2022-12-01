cet_build_plugin(PDHDTPCReader art::module
                        lardataobj::RawData
			duneprototypes::Protodune_hd_RawDecoding_PDHDDataInterfaceWIB3_tool
			dunecore::HDF5Utils_HDF5RawFile2Service_service
			dunecore::dunedaqhdf5utils2
                        LIBHDF5
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        art::Persistency_Provenance
                        messagefacility::MF_MessageLogger
                        ROOT_BASIC_LIB_LIST
                        BASENAME_ONLY
)

cet_build_plugin(PDHDDataInterfaceWIB3   art::tool
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
			dunecore::HDF5Utils_HDF5RawFile2Service_service
			dunecore::dunedaqhdf5utils2
                        LIBHDF5
             )

add_subdirectory(fcl)
install_headers()
install_fhicl()
install_source()
install_scripts()
