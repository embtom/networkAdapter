project(TST_StreamConnection)

#######################################################################################
#Build Settings
#######################################################################################
find_package(GTest REQUIRED)

set (SOURCES main.cpp)

#######################################################################################
#Build target settings
#######################################################################################
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)
target_include_directories(${PROJECT_NAME} PUBLIC ./)
target_link_libraries(${PROJECT_NAME}  networkingAdapter
                                       GTest::GTest
                                       GTest::Main)

add_test(NAME TST_StreamConnection COMMAND ${PROJECT_NAME})
