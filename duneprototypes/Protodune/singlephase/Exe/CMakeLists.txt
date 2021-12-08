# dunetpc/dune/Protodune/singlephase/Exe
#
# David Adams
# October 2018
#
# Instructions to build and install fcldump.

cet_make_exec(pdChannelRange
  SOURCE pdChannelRange.cxx
  LIBRARIES
    dune_ArtSupport
    ART_FRAMEWORK_CORE
    art::Utilities
    art::Persistency_Common
    fhiclcpp
    canvas::canvas
    cetlib::cetlib 
    cetlib_except::cetlib_except
)

install_source()
