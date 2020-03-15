
#######################################################################################
#Build Settings

set (SOURCES broadServer.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_broadServer ${SOURCES})
set_target_properties(EXA_broadServer PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(EXA_broadServer 
    networkingAdapter
)

#######################################################################################
#Install rules

install(TARGETS EXA_broadServer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
