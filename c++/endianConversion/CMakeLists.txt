project(endianConversion)

include(GNUInstallDirs)

######################################################
#Build target

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}>
)

######################################################
#Install rules
install(TARGETS ${PROJECT_NAME} EXPORT endianConversion-config
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}"
)
install(DIRECTORY include
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}"
)
install(EXPORT endianConversion-config
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)