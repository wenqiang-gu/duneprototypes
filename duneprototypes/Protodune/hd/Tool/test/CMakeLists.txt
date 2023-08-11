# dunetpc/dune/Protodune/singlephase/Tool/test/CMakeLists.txt

# David Adams
# June 2021

# Build test for each tool.

cet_test(test_PdhdChannelRanges SOURCES test_PdhdChannelRanges.cxx
  LIBRARIES
    dunecore_ArtSupport
    art::Utilities
    canvas::canvas
    fhiclcpp
    cetlib::cetlib cetlib_except
    cetlib_except::cetlib_except
    ROOT_BASIC_LIB_LIST
    SQLITE3
    Boost::filesystem
)

cet_test(test_PdhdChannelGroups SOURCES test_PdhdChannelGroups.cxx
  LIBRARIES
    dunecore_ArtSupport
    art::Utilities
    canvas::canvas
    fhiclcpp
    cetlib::cetlib cetlib_except
    cetlib_except::cetlib_except
    ROOT_BASIC_LIB_LIST
    SQLITE3
    Boost::filesystem
)
include(CetTest)
