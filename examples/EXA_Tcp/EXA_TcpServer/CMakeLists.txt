
#######################################################################################
#Build Settings

set (SOURCES TcpServer.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_TcpServer ${SOURCES})
set_target_properties(EXA_TcpServer PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(EXA_TcpServer
    EMBTOM::endianconversion
    EMBTOM::networkadapter
    ComProto
)

#######################################################################################
#Install rules

install(TARGETS EXA_TcpServer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
