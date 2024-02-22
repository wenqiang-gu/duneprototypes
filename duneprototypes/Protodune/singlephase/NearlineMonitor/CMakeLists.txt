cet_build_plugin(SPLifetime art::module
              lardataobj::RecoBase
              larcorealg::Geometry
              larcore::Geometry_Geometry_service
              larsim::MCCheater_BackTrackerService_service
              larsim::MCCheater_ParticleInventoryService_service
              nusimdata::SimulationBase
              art::Framework_Core
              art::Framework_Principal
              art::Framework_Services_Registry
              art_root_io::tfile_support
	      ROOT::Core ROOT::Graf ROOT::Gpad
              art::Persistency_Provenance
              art::Persistency_Common
              art::Utilities
              canvas::canvas
              messagefacility::MF_MessageLogger
              cetlib::cetlib cetlib_except
              ROOT::MathCore
              ROOT::GenVector
              ROOT::Geom
              BASENAME_ONLY)

cet_build_plugin(PlotTrackT0 art::module
              lardataobj_RecoBase
              larcorealg_Geometry
              larcore_Geometry_Geometry_service
              larsim_MCCheater_BackTrackerService_service
              larsim_MCCheater_ParticleInventoryService_service
              nusimdata::SimulationBase
              art::Framework_Core
              art::Framework_Principal
              art_root_io::tfile_support
              ROOT::Core
              art::Persistency_Provenance
              art::Persistency_Common
              art::Utilities
              canvas::canvas
              messagefacility::MF_MessageLogger
              cetlib::cetlib cetlib_except
              ROOT::MathCore
              ROOT::GenVector
              ROOT::Geom
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

cet_build_plugin(PlotTrackDetails art::module
              lardataobj_RecoBase
              larcorealg_Geometry
              larcore_Geometry_Geometry_service
              larsim_MCCheater_BackTrackerService_service
              larsim_MCCheater_ParticleInventoryService_service
              nusimdata::SimulationBase
              art::Framework_Core
              art::Framework_Principal
              art_root_io::tfile_support
              ROOT::Core
              art::Persistency_Provenance
              art::Persistency_Common
              art::Utilities
              canvas::canvas
              messagefacility::MF_MessageLogger
              cetlib::cetlib cetlib_except
              ROOT::MathCore
              ROOT::GenVector
              ROOT::Geom
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

cet_build_plugin(PlotEventDetails art::module
      lardataobj_RecoBase
      larcorealg_Geometry
      larcore_Geometry_Geometry_service
      larsim_MCCheater_BackTrackerService_service
      larsim_MCCheater_ParticleInventoryService_service
      nusimdata::SimulationBase
      art::Framework_Core
      art::Framework_Principal
      art_root_io::tfile_support
      ROOT::Core
      art::Persistency_Provenance
      art::Persistency_Common
      art::Utilities 
      canvas::canvas
      messagefacility::MF_MessageLogger
      cetlib::cetlib cetlib_except
      ROOT::MathCore
      ROOT::GenVector
      ROOT::Geom
      ROOT::Core ROOT::Hist ROOT::Tree
      BASENAME_ONLY
)

cet_build_plugin(PlotOpticalDetails art::module
              lardataobj_RecoBase
              art::Framework_Core
              art::Framework_Principal
              art_root_io::tfile_support
              ROOT::Core
              art::Persistency_Provenance
              art::Persistency_Common
              art::Utilities
              canvas::canvas
              messagefacility::MF_MessageLogger
              cetlib::cetlib cetlib_except
              ROOT::MathCore
              ROOT::GenVector
              ROOT::Geom
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

cet_build_plugin(RawEventDisplay art::module
              larcorealg_Geometry
              larcore_Geometry_Geometry_service
              lardataalg_DetectorInfo
              lardataobj_RawData
              nusimdata::SimulationBase
              art::Framework_Core
              art::Framework_Principal
              art_root_io::tfile_support
              ROOT::Core
              messagefacility::MF_MessageLogger
              cetlib::cetlib
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

cet_build_plugin(TpcMonitor art::module
              larcorealg_Geometry
              larcore_Geometry_Geometry_service
              lardataalg_DetectorInfo
              lardataobj_RawData
              nusimdata::SimulationBase
              dunepdlegacy_Services_ChannelMap_PdspChannelMapService_service
              art::Framework_Core
              art::Framework_Principal
              art_root_io::tfile_support
              ROOT::Core
              art::Persistency_Provenance
              messagefacility::MF_MessageLogger
              cetlib::cetlib
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

cet_build_plugin(SSPMonitor art::module
              larcorealg_Geometry
              larcore_Geometry_Geometry_service
              lardataobj_RawData
              lardataobj_RecoBase
              nusimdata::SimulationBase
              art::Framework_Core
              art::Framework_Principal
              art_root_io::tfile_support
              ROOT::Core
              art::Persistency_Provenance
              messagefacility::MF_MessageLogger
              cetlib::cetlib
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

cet_build_plugin(PDSPNearlineHeader art::module
              lardataobj_RecoBase
              art::Framework_Core
              art::Framework_Principal
              art_root_io::tfile_support
              ROOT::Core
              art::Persistency_Common
              art::Utilities
              canvas::canvas
              messagefacility::MF_MessageLogger
              cetlib::cetlib cetlib_except
              ROOT::Core ROOT::Hist ROOT::Tree
              BASENAME_ONLY)

cet_build_plugin(PDSPHitMonitor art::module
              larcorealg_Geometry
              larcore_Geometry_Geometry_service
              lardataobj_RawData
              nusimdata::SimulationBase
              dunepdlegacy_Services_ChannelMap_PdspChannelMapService_service
              art::Framework_Core
              art::Framework_Principal
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
