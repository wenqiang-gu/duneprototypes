# dunetpc/dune/Coldbox/hd/fcl/fcldirs/CMakeLists.txt

# David Adams
# October 2022

# Install all fcl files in rundata in the rundata directory of the product fcl area.
install(
  DIRECTORY rundata
  DESTINATION ${duneprototypes_FHICL_DIR}
  FILES_MATCHING PATTERN *.fcl
)
