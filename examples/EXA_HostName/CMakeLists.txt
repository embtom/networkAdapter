
#######################################################################################
#Build Settings

set (SOURCES main.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_HostName ${SOURCES})
set_target_properties(EXA_HostName PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(EXA_HostName
    networkingAdapter
    docopt
)

#######################################################################################
#Install rules

install(TARGETS EXA_HostName
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)