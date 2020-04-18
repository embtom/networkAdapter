get_filename_component(networkingAdapter_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH "${networkingAdapter_CMAKE_DIR}/cmake")

find_dependency(utilsCpp REQUIRED)
find_dependency(endianConversion REQUIRED)

if(NOT TARGET EMBTOM::networkingAdapter)
    include("${networkingAdapter_CMAKE_DIR}/networkingAdapter-targets.cmake")
endif()
