
# Tom Junk 2022

install_fhicl()
install_headers()
install_source()

file(GLOB channel_map_file *ChannelMap*.txt)
install_fw( LIST ${channel_map_file} )

# Add plugin for each service

if( DEFINED ENV{CANVAS_DIR} )
  set( persistency_lib art::Utilities canvas)
else()
  set( persistency_lib art::Utilities)
endif()

simple_plugin(PD2HDChannelMapService "service"
              ${ART_PERSISTENCY_ROOTDB}
              art::Framework_Services_Registry
              ${persistency_lib}
              SQLite::SQLite3
              messagefacility::MF_MessageLogger
              ${CETLIB_LIBS}
              ROOT_BASIC_LIB_LIST
)

