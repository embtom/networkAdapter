
#######################################################################################
#Build Settings

set (SOURCES UdpServer.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_UdpServer ${SOURCES})
set_target_properties(EXA_UdpServer PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(EXA_UdpServer
    EMBTOM::endianconversion
    EMBTOM::networkadapter
    ComProto
)

#######################################################################################
#Install rules

install(TARGETS EXA_UdpServer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
