cmake_minimum_required(VERSION 3.10)
project(networkingAdapter
    LANGUAGES   CXX
    VERSION     0.9.1
)

######################################################
#Dependencies
find_package(Threads REQUIRED)
find_package(utilsCpp REQUIRED)
include(GNUInstallDirs)

######################################################
#Build Config
set(CMAKE_CXX_STANDARD  17)
set(CMAKE_DEBUG_POSTFIX _g)
set(CMAKE_CXX_EXTENSIONS yes)

######################################################
#Build Options
option (NETWORKINGADAPTER_BUILD_TEST "build tests of the networkingAdapter" ON)
option (NETWORKINGADAPTER_BUILD_EXAMPLES "build examples of the networkingAdapter" ON)

######################################################
#Subdirectoris
add_subdirectory(c++)

include(CTest)
if (${BUILD_TESTING} AND ${NETWORKINGADAPTER_BUILD_TEST})
    find_package(GTest REQUIRED)
    message(STATUS "Build with networkingAdapter Unit Tests")
    enable_testing()
    add_subdirectory(tests)
endif()

if (${NETWORKINGADAPTER_BUILD_EXAMPLES})
    message(STATUS "Build with networkingAdapter Examples")
    add_subdirectory(examples)
endif()
