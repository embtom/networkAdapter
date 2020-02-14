get_filename_component(networkingAdapter_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(CMakeFindDependencyMacro)

list(APPEND CMAKE_MODULE_PATH "${networkingAdapter_CMAKE_DIR}/cmake")

find_dependency(utilsCpp REQUIRED)

if(NOT TARGET EMBTOM::networkingAdapter)
    include("${networkingAdapter_CMAKE_DIR}/networkingadapter-targets.cmake")
endif()
