
#######################################################################################
#Settings

set (SOURCES TcpClient.cpp)

#######################################################################################
#Build target

add_executable(EXA_TcpClient ${SOURCES})
set_target_properties(EXA_TcpClient PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(EXA_TcpClient
    EMBTOM::endianConversion
    EMBTOM::networkingAdapter
    Threads::Threads
    docopt
    ComProto
)

#######################################################################################
#Install rules

install(TARGETS EXA_TcpClient
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
