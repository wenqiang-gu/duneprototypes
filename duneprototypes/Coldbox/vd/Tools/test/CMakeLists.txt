# dunetpc/dune/VDColdbox/Tools/test/CMakeLists.txt

# David Adams
# November 2021

# Build test for each tool.

include(CetTest)

cet_test(test_VDColdboxChannelRanges SOURCES test_VDColdboxChannelRanges.cxx
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

cet_test(test_VDColdboxChannelGroups SOURCES test_VDColdboxChannelGroups.cxx
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

cet_test(test_VDColdboxOnlineChannel SOURCES test_VDColdboxOnlineChannel.cxx
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

cet_test(test_CrpChannelRanges SOURCES test_CrpChannelRanges.cxx
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

cet_test(test_CrpChannelGroups SOURCES test_CrpChannelGroups.cxx
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

