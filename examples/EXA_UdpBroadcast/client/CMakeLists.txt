
#######################################################################################
#Build Settings

set (SOURCES "broadClient.cpp")

#######################################################################################
#Build target settings

add_executable(EXA_Broadclient ${SOURCES})
set_target_properties(EXA_Broadclient PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
)

target_link_libraries(EXA_Broadclient
    EMBTOM::endianconversion
    EMBTOM::networkadapter
    Threads::Threads
    ComProto
)

#######################################################################################
#Install rules

install(TARGETS EXA_Broadclient
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
