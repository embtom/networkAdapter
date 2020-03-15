
#######################################################################################
# Sources

set (SOURCES main.cpp)

#######################################################################################
# Build settings

add_executable(TST_TcpConnection ${SOURCES})

set_target_properties(TST_TcpConnection PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(TST_TcpConnection
    networkingAdapter
    GTest::GTest
    GTest::Main
)

######################################################
# add to ctest

add_test(NAME TST_TcpConnection COMMAND TST_TcpConnection)
