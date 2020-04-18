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

add_library(endianConversion INTERFACE)
add_library(EMBTOM::endianConversion ALIAS endianConversion)

target_include_directories(endianConversion INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/detail>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianConversion>
)

######################################################
# Install rules

install(TARGETS endianConversion EXPORT endianConversion-targets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianConversion"
)

install(FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/endianConversion"
)

install(EXPORT endianConversion-targets
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/endianConversion"
)

install(FILES "cmake/endianConversion-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/endianConversion"
)