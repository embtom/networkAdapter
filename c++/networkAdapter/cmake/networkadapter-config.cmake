get_filename_component(networkadapter_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH "${networkadapter_CMAKE_DIR}/cmake")

find_dependency(utilsCpp REQUIRED)
find_dependency(endianconversion REQUIRED)

if(NOT TARGET EMBTOM::networkadapter)
    include("${networkadapter_CMAKE_DIR}/networkadapter-targets.cmake")
endif()
