# dunetpc/dune/VDColdbox/Tools/test/CMakeLists.txt

# David Adams
# November 2021

# Build test for each tool.

include(CetTest)

cet_test(test_VDColdboxChannelRanges SOURCES test_VDColdboxChannelRanges.cxx
  LIBRARIES
    dune_ArtSupport
    art_Utilities
    canvas
    fhiclcpp
    cetlib cetlib_except
    cetlib_except
    ROOT_BASIC_LIB_LIST
    SQLITE3
    ${Boost_FILESYSTEM_LIBRARY}
)

cet_test(test_VDColdboxChannelGroups SOURCES test_VDColdboxChannelGroups.cxx
  LIBRARIES
    dune_ArtSupport
    art_Utilities
    canvas
    fhiclcpp
    cetlib cetlib_except
    cetlib_except
    ROOT_BASIC_LIB_LIST
    SQLITE3
    ${Boost_FILESYSTEM_LIBRARY}
)

cet_test(test_VDColdboxOnlineChannel SOURCES test_VDColdboxOnlineChannel.cxx
  LIBRARIES
    dune_ArtSupport
    art_Utilities
    canvas
    fhiclcpp
    cetlib cetlib_except
    cetlib_except
    ROOT_BASIC_LIB_LIST
    SQLITE3
    ${Boost_FILESYSTEM_LIBRARY}
)

