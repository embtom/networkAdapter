
#######################################################################################
#Build Settings

set (SOURCES UdpClient.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_UdpClient ${SOURCES})
set_target_properties(EXA_UdpClient PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(EXA_UdpClient
    EMBTOM::endianConversion
    EMBTOM::networkingAdapter
    Threads::Threads
    docopt
    ComProto
)

#######################################################################################
#Install rules

install(TARGETS EXA_UdpClient
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
