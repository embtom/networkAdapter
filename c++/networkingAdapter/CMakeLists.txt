
######################################################
#Sources and Headers to build
set(SOURCES
    "src/IpAddress.cpp"
    "src/BaseSocket.cpp"
    "src/HostLookup.cpp"
    "src/BaseDataLink.cpp"
    "src/stream/StreamDataLink.cpp"
    "src/stream/StreamServer.cpp"
    "src/stream/StreamClient.cpp"
    "src/dgram/DgramDataLink.cpp"
    "src/dgram/DgramServer.cpp"
    "src/dgram/DgramClient.cpp"
)

set(HEADERS
    "include/IpAddress.hpp"
    "include/BaseSocket.hpp"
    "include/HostLookup.h"
    "include/BaseDataLink.hpp"
    "include/stream/StreamDataLink.hpp"
    "include/stream/StreamServer.hpp"
    "include/stream/StreamClient.hpp"
    "include/dgram/DgramDataLink.hpp"
    "include/dgram/DgramServer.hpp"
    "include/dgram/DgramClient.hpp"
)

######################################################
#Build target
add_library(networkingAdapter SHARED
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(networkingAdapter PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(networkingAdapter PRIVATE
    PUBLIC EMBTOM::utilsCpp
    PRIVATE Threads::Threads
)

target_include_directories(networkingAdapter PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkingAdapter>
)

set_target_properties(networkingAdapter PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

######################################################
#Install rules
install(TARGETS networkingAdapter EXPORT networkingadapter-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkingAdapter"
)

install(FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkingAdapter"
)

install(EXPORT networkingadapter-targets
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/networkingAdapter"
)

install(FILES "cmake/networkingadapter-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/networkingAdapter"
)