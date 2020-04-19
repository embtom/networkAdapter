
#######################################################################################
#Build Settings

set (SOURCES main.cpp)

#######################################################################################
#Build target settings

add_executable(EXA_InterfaceLookup ${SOURCES})
set_target_properties(EXA_InterfaceLookup PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(EXA_InterfaceLookup
    EMBTOM::networkingAdapter
)

#######################################################################################
#Install rules

install(TARGETS EXA_InterfaceLookup
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)