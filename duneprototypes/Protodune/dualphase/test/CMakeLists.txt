cet_build_plugin(		recobWireCheck art::module
			lardataobj_RecoBase
                        lardata_ArtDataHelper
                        lardata_Utilities
                        ART_FRAMEWORK_CORE
                        ART_FRAMEWORK_PRINCIPAL
                        ART_FRAMEWORK_SERVICES_REGISTRY
                        ART_ROOT_IO_TFILE_SUPPORT
                        ROOT::Core
                        ART_ROOT_IO_TFILESERVICE_SERVICE
                        art::Persistency_Common canvas
                        art::Persistency_Provenance canvas
                        art::Utilities canvas
                        messagefacility::MF_MessageLogger
                        
                        cetlib::cetlib cetlib_except
                        ROOT_BASIC_LIB_LIST
                        BASENAME_ONLY
)

install_fhicl()
install_source()
