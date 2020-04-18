
#######################################################################################
#Build Settings

set (SOURCES "broadClient.cpp")

#######################################################################################
#Build target settings

add_executable(EXA_Broadclient ${SOURCES})
set_target_properties(EXA_Broadclient PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(EXA_Broadclient
    PRIVATE networkingAdapter
    Threads::Threads
)

#######################################################################################
#Install rules

install(TARGETS EXA_Broadclient
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
