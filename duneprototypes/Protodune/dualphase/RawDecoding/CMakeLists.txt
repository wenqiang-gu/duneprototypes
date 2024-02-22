cet_build_plugin(PDDPChannelMap art::service
                        art::Framework_Core
			art::Framework_IO_Sources
                        art::Framework_Principal
                        art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			fhiclcpp::fhiclcpp
                        messagefacility::MF_MessageLogger
                        cetlib::cetlib
                        ROOT::Core ROOT::Hist ROOT::Tree
                        BASENAME_ONLY
)

cet_build_plugin(PDDPRawInput art::source
			PDDPRawInputDriver_service
                        lardataobj_RawData
                        lardata_Utilities
                        art::Framework_Core
			art::Framework_IO_Sources
                        art::Framework_Principal
                        art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			fhiclcpp::fhiclcpp
                        messagefacility::MF_MessageLogger
                        cetlib::cetlib
                        ROOT::Core ROOT::Hist ROOT::Tree
                        BASENAME_ONLY
)

cet_build_plugin(PDDPRawInputDriver art::service
			PDDPChannelMap_service
			pthread
			lardataobj_RawData
                        lardata_Utilities
                        art::Framework_Core
			art::Framework_IO_Sources
                        art::Framework_Principal
                        art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			fhiclcpp::fhiclcpp
                        messagefacility::MF_MessageLogger
                        cetlib::cetlib
                        ROOT::Core ROOT::Hist ROOT::Tree
                        BASENAME_ONLY
)

cet_build_plugin(PDDPTPCRawDecoderTest art::module
			lardataobj_RawData
                        art::Framework_Core
			art::Framework_Principal
			art::Framework_Services_Registry
                        art_root_io::tfile_support
                        ROOT::Core
                        
 			art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			messagefacility::MF_MessageLogger
			
			cetlib::cetlib cetlib_except
			ROOT::Core ROOT::Hist ROOT::Tree
              		BASENAME_ONLY
)


install_headers()
install_fhicl()
install_source()
install_scripts()

