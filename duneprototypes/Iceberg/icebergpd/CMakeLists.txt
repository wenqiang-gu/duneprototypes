cet_build_plugin(ICEBERGPDSSPMonitor art::module
              larcorealg_Geometry
              larcore_Geometry_Geometry_service
              lardataobj_RawData
              nusimdata::SimulationBase
              dunepdlegacy_Services_ChannelMap_IcebergChannelMapService_service
              art::Framework_Core
              art::Framework_Principal
              art::Framework_Services_Registry
              art_root_io::tfile_support
              ROOT::Core
              
              art::Persistency_Provenance
              messagefacility::MF_MessageLogger
              cetlib::cetlib
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

install_headers()
install_fhicl()
install_source()
install_scripts()
