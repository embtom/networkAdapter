
#######################################################################################
#Build Settings

set (SOURCES TcpServer.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_TcpServer ${SOURCES})
set_target_properties(EXA_TcpServer PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(EXA_TcpServer PRIVATE networkingAdapter)

#######################################################################################
#Install rules

install(TARGETS EXA_TcpServer
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
