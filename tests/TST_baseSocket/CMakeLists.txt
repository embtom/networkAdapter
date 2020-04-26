
######################################################
# Sources
set (SOURCES main.cpp)

######################################################
# Build target

add_executable(TST_baseSocket ${SOURCES})

set_target_properties(TST_baseSocket PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_include_directories(TST_baseSocket
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(TST_baseSocket
    networkadapter
    GTest::GTest
    GTest::Main
)

######################################################
# add to ctest

add_test(NAME TST_baseSocket COMMAND TST_baseSocket)
