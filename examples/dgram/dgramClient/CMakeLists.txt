project(EXA_dgramClient)

#######################################################################################
#Build Settings 
#######################################################################################
set (SOURCES dgramClient.cpp)

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
target_link_libraries(${PROJECT_NAME} networkingAdapter)
