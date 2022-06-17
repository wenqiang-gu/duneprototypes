cet_build_plugin(PDSPmatch art::module
                        lardataobj_RawData
                        lardataobj_RecoBase
                        dunepdlegacy::dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        dunecore_Geometry
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
                        dunepdlegacy::dunepdlegacy_rce_dataaccess
                        BASENAME_ONLY
)

cet_build_plugin(PDSPmatchmc art::module
                        lardataobj_RawData
                        lardataobj_RecoBase
                        dunepdlegacy::dunepdlegacy_Overlays
                        artdaq_core::artdaq-core_Data
                        dunecore_Geometry
                        nusimdata::SimulationBase
                        larsim_MCCheater_PhotonBackTrackerService_service
                        larsim_MCCheater_ParticleInventoryService_service
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
                        dunepdlegacy::dunepdlegacy_rce_dataaccess
                        BASENAME_ONLY
)


install_fhicl()
install_source()
install_scripts()
