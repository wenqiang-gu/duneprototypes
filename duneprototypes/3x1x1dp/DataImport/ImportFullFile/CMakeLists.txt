cet_build_plugin(ImportFull311File art::source
			RawData311InputDriver_service

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
			ROOT::Core ROOT::Hist ROOT::Tree
			BASENAME_ONLY
)


install_headers()
install_fhicl()
install_source()
install_scripts()
