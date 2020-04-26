
######################################################
# Sources
set (SOURCES main.cpp)

######################################################
# Build target

add_executable(TST_EndianConversion ${SOURCES})

set_target_properties(TST_EndianConversion PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(TST_EndianConversion
    networkadapter
    EMBTOM::endianconversion
    GTest::GTest
    GTest::Main
)

######################################################
# add to ctest

add_test(NAME TST_EndianConversion COMMAND TST_EndianConversion)