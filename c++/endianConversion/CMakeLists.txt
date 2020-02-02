project(endianConversion)

include(GNUInstallDirs)

######################################################
#Sources and Headers to build
set(HEADERS_EC
    "include/NetOrder.h"
    "include/HostOrder.h"
    "include/detail/EndianConvert.h"
    "include/detail/EndianConverter.h"
    "include/detail/EndianMembers.h"
    "include/detail/EndianMeta.h"
    "include/detail/EndianMeta.inl"
    "include/detail/MetaHolder.h"
    "include/detail/template_helpers.h"
    "include/detail/template_helpers.inl"
)

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

install(FILES ${HEADERS_EC}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}"
)

install(EXPORT endianConversion-config
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)