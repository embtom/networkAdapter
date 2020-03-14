project(EXA_TcpClient)

#######################################################################################
#Build Settings
#######################################################################################
set (SOURCES TcpClient.cpp)

#######################################################################################
#Build target settings
#######################################################################################
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(${PROJECT_NAME} networkingAdapter)
