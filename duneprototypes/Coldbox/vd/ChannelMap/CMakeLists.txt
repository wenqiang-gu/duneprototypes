
# Tom Junk 2021

install_fhicl()
install_headers()
install_source()

file(GLOB channel_map_file *chanmap*.txt)
install_fw( LIST ${channel_map_file} )

# Add plugin for each service

if( DEFINED ENV{CANVAS_DIR} )
  set( persistency_lib art::Utilities canvas)
else()
  set( persistency_lib art::Utilities)
endif()

simple_plugin(VDColdboxChannelMapService "service"
              ${ART_PERSISTENCY_ROOTDB}
              art::Framework_Services_Registry
              ${persistency_lib}
              SQLite::SQLite3
              messagefacility::MF_MessageLogger
              ${CETLIB_LIBS}
              ROOT::Core ROOT::Hist ROOT::Tree
) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use cet_build_plugin() with explicit source lists and plugin base types

simple_plugin(VDColdboxTDEChannelMapService "service"
              ${ART_PERSISTENCY_ROOTDB}
              art::Framework_Services_Registry
              ${persistency_lib}
              SQLite::SQLite3
              messagefacility::MF_MessageLogger
              ${CETLIB_LIBS}
              ROOT::Core ROOT::Hist ROOT::Tree
) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - deprecated: use cet_build_plugin() with explicit source lists and plugin base types
