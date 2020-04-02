
#######################################################################################
#Settings

set (SOURCES TcpClient.cpp)

#######################################################################################
#Build target

add_executable(EXA_TcpClient ${SOURCES})
set_target_properties(EXA_TcpClient PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(EXA_TcpClient
    endianConversion
    networkingAdapter
    Threads::Threads
    docopt
)

#######################################################################################
#Install rules

install(TARGETS EXA_TcpClient
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
