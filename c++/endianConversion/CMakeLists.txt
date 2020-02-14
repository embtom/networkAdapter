######################################################
#Sources and Headers to build
set(HEADERS
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

add_library(endianConversion INTERFACE)

target_include_directories(endianConversion INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianConversion>
)

######################################################
#Install rules
install(TARGETS endianConversion EXPORT endianConversion-config
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianConversion"
)

install(FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianConversion"
)

install(EXPORT endianConversion-config
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/endianConversion"
)