
cet_build_plugin(VDColdboxDataInterface   art::tool
                                     canvas::canvas
                                     cetlib::cetlib
                                     cetlib_except::cetlib_except
                        lardataobj_RawData
                        dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        artdaq_core::artdaq-core_Utilities
                        dunepdlegacy_Services_ChannelMap_PdspChannelMapService_service
                        dunecore::HDF5Utils
                        HDF5::HDF5
                        duneprototypes_Coldbox_vd_ChannelMap_VDColdboxChannelMapService_service
                        art::Framework_Core
                        art::Framework_Principal
                        art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        
                        art::Persistency_Provenance
                        messagefacility::MF_MessageLogger
                        cetlib::cetlib
                        ROOT::Core ROOT::Hist ROOT::Tree
             )


simple_plugin(VDColdboxTDERawInput "source"
  duneprototypes_Coldbox_vd_ChannelMap_VDColdboxTDEChannelMapService_service
  art::Framework_Services_Registry
  canvas::canvas
  cetlib::cetlib
  cetlib_except::cetlib_except
  lardataobj_RawData
  art::Framework_Core
  art::Framework_Principal
  art::Framework_Services_Registry
  art_root_io::tfile_support
  ROOT::Core
  
  art::Persistency_Provenance
  messagefacility::MF_MessageLogger
  ROOT::Core ROOT::Hist ROOT::Tree
) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use cet_build_plugin() with explicit source lists and plugin base types


cet_build_plugin(VDColdboxPDSDecoder art::module
                 lardataobj_RawData
                 lardataobj_RecoBase
                 dunecore::HDF5Utils
                 HDF5::HDF5
                 art::Framework_Core
                 art::Framework_Principal
                 art::Framework_Services_Registry
                 art_root_io::tfile_support
                 ROOT::Core
                 
                 art::Persistency_Provenance
                 messagefacility::MF_MessageLogger
                 cetlib::cetlib
                 ROOT::Core ROOT::Hist ROOT::Tree
                 BASENAME_ONLY
)



add_subdirectory(ChannelMap)
add_subdirectory(fcl)
add_subdirectory(Tools)

install_headers()
install_fhicl()
install_source()
install_scripts()
