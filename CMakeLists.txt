cmake_minimum_required(VERSION 3.10)

option (NETWORKINGADAPTER_BUILD_TEST "build tests of the networkingAdapter" ON)
set(UTILSCPP_BUILD_TEST ${NETWORKINGADAPTER_BUILD_TEST})
if (${NETWORKINGADAPTER_BUILD_TEST})
    message(STATUS "NETWORKINGADAPTER_BUILD_TEST")
endif() 


set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)
#use of standard install paths

######################################################
#Subdirectoris 
######################################################
add_subdirectory(c++)

if (${NETWORKINGADAPTER_BUILD_TEST})
    #add_subdirectory(test)
endif()

