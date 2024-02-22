cet_build_plugin(ImportSingle311Event art::module
			EventDecoder_service

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
			art_root_io::TFileService_service
 			art::Persistency_Common canvas
			art::Persistency_Provenance canvas
			art::Utilities canvas
			messagefacility::MF_MessageLogger
			ROOT::Core ROOT::Hist ROOT::Tree
			BASENAME_ONLY
)

install_headers()
install_fhicl()
install_source()
install_scripts()
