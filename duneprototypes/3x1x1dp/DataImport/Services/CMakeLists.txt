cet_build_plugin(RawData311InputDriver art::service
			EventDecoder_service
			dlardaq_service

			lardataobj_RawData
			lardata_Utilities
			larcorealg_Geometry
			art::Framework_Core
		        art::Framework_IO_Sources	
			art::Framework_Principal
 			art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			messagefacility::MF_MessageLogger
			
			cetlib::cetlib
			ROOT::Core ROOT::Hist ROOT::Tree
			BASENAME_ONLY
)

cet_build_plugin(dlardaq art::service
			lardataobj_RawData
			lardataobj_RecoBase
			larreco_RecoAlg
			larreco_Calorimetry
			larreco_RecoAlg_PMAlg
			larcorealg_Geometry
			larcore_Geometry_Geometry_service
			lardata_Utilities
			larsim_Simulation nug4::ParticleNavigation lardataobj_Simulation
		        art::Framework_IO_Sources	
			
			nusimdata::SimulationBase
			art::Framework_Core
			art::Framework_Principal
			art::Framework_Services_Registry
			art_root_io::tfile_support
			ROOT::Core
			
 			art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			messagefacility::MF_MessageLogger
			
			cetlib::cetlib
			ROOT::Core ROOT::Hist ROOT::Tree
			BASENAME_ONLY
)


cet_build_plugin(EventDecoder art::service
			dlardaq_service
			HuffDataCompressor_service
			
			lardataobj_RawData
			lardataobj_RecoBase
			larreco_RecoAlg
			larreco_Calorimetry
			larreco_RecoAlg_PMAlg
			larcorealg_Geometry
			larcore_Geometry_Geometry_service
			lardata_Utilities
			larsim_Simulation nug4::ParticleNavigation lardataobj_Simulation
			
			nusimdata::SimulationBase
			art::Framework_Core
			art::Framework_Principal
			art::Framework_Services_Registry
			art_root_io::tfile_support
			ROOT::Core
			
		        art::Framework_IO_Sources	
 			art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			messagefacility::MF_MessageLogger
			
			cetlib::cetlib
			ROOT::Core ROOT::Hist ROOT::Tree
			BASENAME_ONLY
)


cet_build_plugin(HuffDataCompressor art::service
			dlardaq_service

			lardataobj_RawData
			lardataobj_RecoBase
			larreco_RecoAlg
			larreco_Calorimetry
			larreco_RecoAlg_PMAlg
			larcorealg_Geometry
			larcore_Geometry_Geometry_service
			lardata_Utilities
			larsim_Simulation nug4::ParticleNavigation lardataobj_Simulation
			
			nusimdata::SimulationBase
			art::Framework_Core
			art::Framework_Principal
			art::Framework_Services_Registry
			art_root_io::tfile_support
			ROOT::Core
			
 			art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			messagefacility::MF_MessageLogger
			
		        art::Framework_IO_Sources	
			cetlib::cetlib
			ROOT::Core ROOT::Hist ROOT::Tree
			BASENAME_ONLY
)




install_headers()
install_fhicl()
install_source()
install_scripts()
