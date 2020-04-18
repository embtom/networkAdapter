get_filename_component(endianConversion_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH "${endianConversion_CMAKE_DIR}/cmake")

find_dependency(utilsCpp REQUIRED)

if(NOT TARGET EMBTOM::endianConversion)
    include("${endianConversion_CMAKE_DIR}/endianConversion-targets.cmake")
endif()
