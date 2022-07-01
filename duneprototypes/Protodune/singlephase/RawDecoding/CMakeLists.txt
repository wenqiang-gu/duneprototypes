
cet_build_plugin(PDSPTPCDataInterface   art::tool
				     art_utilities
				     canvas::canvas
				     cetlib::cetlib
				     cetlib_except::cetlib_except
                        lardataobj_RawData
                        dunepdlegacy::Overlays
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
                        dunepdlegacy::Services_ChannelMap_PdspChannelMapService_service
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
                        dunepdlegacy::rce_dataaccess
                        z
             )


cet_build_plugin(TimingRawDecoder art::module
                        lardataobj_RawData
                        dunepdlegacy::Overlays
                        dunecore_DuneObj
			artdaq_core::artdaq-core_Utilities
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
                        BASENAME_ONLY
)

cet_build_plugin(SSPRawDecoder art::module
                        lardataobj_RawData
                        lardataobj_RecoBase
                        dunepdlegacy::Overlays
                        dunepdlegacy::Services_ChannelMap_PdspChannelMapService_service
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
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
                        BASENAME_ONLY
)

#[[
cet_build_plugin(PDWaveform art::module
                         lardataobj_RawData
                         lardataobj_RecoBase
                         dunepdlegacy::Overlays
                         dunepdlegacy::Services_ChannelMap_PdspChannelMapService_service
 			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
                         art::Framework_Core
                         art::Framework_Principal
                         art::Framework_Services_Registry
                         art_root_io::tfile_support
                         ROOT::Core
                         art_root_io::TFileService_service
                         messagefacility::MF_MessageLogger
			 dunecore_Geometry
			 CETLIB
                         ROOT_BASIC_LIB_LIST
			 ROOT_SPECTRUM
                         BASENAME_ONLY
)
]]


cet_build_plugin(RceRawDecoder art::module
                        lardataobj_RawData
                        dunepdlegacy::Overlays
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
                        dunepdlegacy::Services_ChannelMap_PdspChannelMapService_service
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
                        dunepdlegacy::rce_dataaccess
                        BASENAME_ONLY
)

cet_build_plugin(FelixRawDecoder art::module
                        lardataobj_RawData
                        dunepdlegacy::Overlays
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
                        dunepdlegacy::Services_ChannelMap_PdspChannelMapService_service
			dunecore_Geometry
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
                        dunepdlegacy::rce_dataaccess
                        z
                        BASENAME_ONLY
)

cet_build_plugin(FelixIntegrityTest art::module
                        lardataobj_RawData
                        dunepdlegacy::Overlays
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
                        dunepdlegacy::Services_ChannelMap_PdspChannelMapService_service
			dunecore_Geometry
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
                        dunepdlegacy::rce_dataaccess
                        z
                        BASENAME_ONLY
)

cet_build_plugin(PDSPTPCRawDecoder art::module
                        lardataobj_RawData
                        dunepdlegacy::Overlays
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
                        dunepdlegacy::Services_ChannelMap_PdspChannelMapService_service
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
                        dunepdlegacy::rce_dataaccess
                        z
                        BASENAME_ONLY
)

cet_build_plugin(CRTRawDecoder art::module
                        dunepdlegacy::Overlays
                        dunecore_Geometry
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
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
                        dunepdlegacy::rce_dataaccess
                        BASENAME_ONLY
)

cet_build_plugin(PDSPCTBRawDecoder art::module
                        dunepdlegacy::Overlays
			artdaq_core::artdaq-core_Data
			artdaq_core::artdaq-core_Utilities
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        art_root_io::TFileService_service
                        ART_PERSISTENCY_PROVENANCE
                        messagefacility::MF_MessageLogger
                        messagefacility::MF_MessageLogger
                        CETLIB
                        ROOT_BASIC_LIB_LIST
                        dunepdlegacy::rce_dataaccess
                        BASENAME_ONLY
)

art_make_library( LIBRARY_NAME dunetpc_protodune_RawDecodingUtils
                  SOURCE
                  RawDecodingUtils.cc
                  LIBRARIES
                  lardataobj_RawData
                  dunepdlegacy::Overlays
                  artdaq_core::artdaq-core_Data
		  artdaq_core::artdaq-core_Utilities
                  art::Framework_Core
                  art::Framework_Principal
                  art::Framework_Services_Registry
                  art_root_io::tfile_support
                  ROOT::Core
                  art_root_io::TFileService_service
 #                 art::Persistency_Common canvas_Persistency_Common
 #                 art::Persistency_Provenance canvas_Persistency_Provenance
 #                 art::Utilities canvas_Utilities
                  messagefacility::MF_MessageLogger
                  CETLIB
                  ROOT_BASIC_LIB_LIST
                  dunepdlegacy::rce_dataaccess
)


install_headers()
install_fhicl()
install_source()
install_scripts()
