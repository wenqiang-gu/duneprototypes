
# Tom Junk 2022

install_fhicl()
install_headers()
install_source()

file(GLOB channel_map_file *ChannelMap*.txt)
install_fw( LIST ${channel_map_file} )

art_make(SERVICE_LIBRARIES 
                           art::Framework_Services_Registry
                           art::Framework_Principal
                           art_Framework_Core
                           art::Persistency_Provenance
                           messagefacility::MF_MessageLogger
                           ROOT::Core
 	                   duneprototypes::Coldbox_hd_ChannelMap
)


