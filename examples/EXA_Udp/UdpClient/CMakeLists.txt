
#######################################################################################
#Build Settings

set (SOURCES UdpClient.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_UdpClient ${SOURCES})
set_target_properties(EXA_UdpClient PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(EXA_UdpClient networkingAdapter)

#######################################################################################
#Install rules

install(TARGETS EXA_UdpClient
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
