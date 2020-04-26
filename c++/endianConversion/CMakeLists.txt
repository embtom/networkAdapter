######################################################
# Sources

set(HEADERS
    "include/NetOrder.h"
    "include/HostOrder.h"
    "include/detail/EndianConvert.h"
    "include/detail/EndianConverter.h"
    "include/detail/EndianMembers.h"
    "include/detail/EndianMeta.h"
    "include/detail/EndianMeta.inl"
    "include/detail/MetaHolder.h"
)

######################################################
# Build target

add_library(endianconversion INTERFACE)
add_library(EMBTOM::endianconversion ALIAS endianconversion)

target_include_directories(endianconversion INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/detail>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianconversion>
)

######################################################
# Install rules

install(TARGETS endianconversion EXPORT endianconversion-targets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianconversion"
)

install(FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianconversion"
)

install(EXPORT endianconversion-targets
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/endianconversion"
)

install(FILES "cmake/endianconversion-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/endianconversion"
)