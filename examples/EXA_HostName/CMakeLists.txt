
#######################################################################################
#Build Settings

set (SOURCES main.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_HostName ${SOURCES})
set_target_properties(EXA_HostName PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(EXA_HostName
    EMBTOM::networkingAdapter
    docopt
)

#######################################################################################
#Install rules

install(TARGETS EXA_HostName
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)