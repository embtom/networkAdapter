
######################################################
# Sources
set (SOURCES main.cpp)

######################################################
# Build target

add_executable(TST_UdpDataLink ${SOURCES})

set_target_properties(TST_UdpDataLink PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(TST_UdpDataLink
    networkadapter
    GTest::GTest
    GTest::Main
)

######################################################
# add to ctest

add_test(NAME TST_UdpDataLink COMMAND TST_UdpDataLink)