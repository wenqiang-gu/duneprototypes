# ======================================================================
#  duneprototypes main build file
# ======================================================================

cmake_minimum_required (VERSION 3.20 FATAL_ERROR) ### MIGRATE-CHANGED (migrate-3.22.02) - VERSION 3.19 -> 3.20
# cmake_policy(VERSION 3.19) ### MIGRATE-ACTION-COMPATIBILITY (migrate-3.22.02) - Uncomment to preserve compatibility with older CMake versions


find_package(cetmodules REQUIRED) ### MIGRATE-ADDED (migrate-3.22.02) - find_package(cetmodules) must precede project()
project(duneprototypes LANGUAGES CXX)
set(${PROJECT_NAME}_CMAKE_PROJECT_VERSION_STRING 09.83.01d00)
set(${PROJECT_NAME}_FW_DIR "config_data")

list(APPEND CMAKE_MODULE_PATH $ENV{CANVAS_ROOT_IO_DIR}/Modules) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - prefer find_package() to find external CMake modules
list(APPEND CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules) ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - prefer find_package() to find external CMake modules


include(CetCMakeEnv)
cet_cmake_env()

cet_set_compiler_flags(DIAGS CAUTIOUS
  WERROR
  NO_UNDEFINED
  EXTRA_FLAGS -pedantic -Wno-unused-local-typedefs
              -Wno-unused-result
)

cet_report_compiler_flags() ### MIGRATE-ACTION-RECOMMENDED (migrate-3.22.02) - add args: REPORT_THRESHOLD VERBOSE

find_package( Boost REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(...) -> find_package(REQUIRED ...)
find_package( art REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(art ...) -> find_package(art REQUIRED ...)
find_package( art_root_io REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(art_root_io ...) -> find_package(art_root_io REQUIRED ...)
find_package( canvas_root_io REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(canvas_root_io ...) -> find_package(canvas_root_io REQUIRED ...)
find_package(ROOT REQUIRED) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(...) -> find_package(REQUIRED ...)
find_package( larcore REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(larcore ...) -> find_package(larcore REQUIRED ...)
find_package( larcorealg REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(larcorealg ...) -> find_package(larcorealg REQUIRED ...)
find_package( larcoreobj REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(larcoreobj ...) -> find_package(larcoreobj REQUIRED ...)
find_package( larpandora REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(larpandora ...) -> find_package(larpandora REQUIRED ...)
find_package( dunecore REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(dunecore ...) -> find_package(dunecore REQUIRED ...)
find_package( lardataobj REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(lardataobj ...) -> find_package(lardataobj REQUIRED ...)
find_package( lardataalg REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(lardataalg ...) -> find_package(lardataalg REQUIRED ...)
find_package( lardata REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(lardata ...) -> find_package(lardata REQUIRED ...)
find_package( larevt REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(larevt ...) -> find_package(larevt REQUIRED ...)
find_package( larsim REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(larsim ...) -> find_package(larsim REQUIRED ...)
find_package( duneopdet REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(duneopdet ...) -> find_package(duneopdet REQUIRED ...)
find_package( dunecalib REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(dunecalib ...) -> find_package(dunecalib REQUIRED ...)
find_package( dunesim REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(dunesim ...) -> find_package(dunesim REQUIRED ...)
find_package( nusimdata REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(nusimdata ...) -> find_package(nusimdata REQUIRED ...)
find_package( nug4 REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(nug4 ...) -> find_package(nug4 REQUIRED ...)
find_package( dunepdlegacy REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(dunepdlegacy ...) -> find_package(dunepdlegacy REQUIRED ...)
find_package( artdaq_core REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(artdaq_core ...) -> find_package(artdaq_core REQUIRED ...)
find_package( TRACE REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(TRACE ...) -> find_package(TRACE REQUIRED ...)
find_package( CLHEP REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(clhep ...) -> find_package(clhep REQUIRED ...)
find_package( dunedaqdataformats REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(dunedaqdataformats ...) -> find_package(dunedaqdataformats REQUIRED ...)
find_package( dunedetdataformats REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(dunedetdataformats ...) -> find_package(dunedetdataformats REQUIRED ...)
find_package( PostgreSQL REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(postgresql ...) -> find_package(postgresql REQUIRED ...)
find_package( libwda REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(libwda ...) -> find_package(libwda REQUIRED ...)
find_package( ifdh_art REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(ifdh_art ...) -> find_package(ifdh_art REQUIRED ...)
find_package( ifdhc REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(ifdhc ...) -> find_package(ifdhc REQUIRED ...)
find_package( ifbeam REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(ifbeam ...) -> find_package(ifbeam REQUIRED ...)
find_package( Geant4 REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(...) -> find_package(REQUIRED ...)
find_package( nlohmann_json REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(nlohmann_json ...) -> find_package(nlohmann_json REQUIRED ...)
#find_package( highfive REQUIRED ) ### MIGRATE-CHANGED (migrate-3.22.02) - find_ups_product(highfive ...) -> find_package(highfive REQUIRED ...)
find_package( HDF5 REQUIRED EXPORT )
find_package( nuevdb REQUIRED )
# macros for artdaq_dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

add_subdirectory(duneprototypes)

cet_cmake_config() ### MIGRATE-ADDED (migrate-3.22.02) - required cmd
