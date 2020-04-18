
######################################################
# Sources

set (SOURCES main.cpp)

######################################################
# Build target
add_executable(TST_IpAddress ${SOURCES})

set_target_properties(TST_IpAddress PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(TST_IpAddress
    networkingAdapter
    GTest::GTest
    GTest::Main
)

######################################################
# add to ctest

add_test(NAME TST_IpAddress COMMAND TST_IpAddress)