
#######################################################################################
#Build Settings

set (SOURCES UdpServer.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_UdpServer ${SOURCES})
set_target_properties(EXA_UdpServer PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(EXA_UdpServer networkingAdapter)

#######################################################################################
#Install rules

install(TARGETS EXA_UdpServer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
