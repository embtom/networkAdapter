
######################################################
# Sources

set(SOURCES
    "src/IpAddress.cpp"
    "src/BaseSocket.cpp"
    "src/Lookup/HostLookup.cpp"
    "src/Lookup/InterfacesLookup.cpp"
    "src/Tcp/TcpDataLink.cpp"
    "src/Tcp/TcpServer.cpp"
    "src/Tcp/TcpClient.cpp"
    "src/Udp/UdpClient.cpp"
    "src/Udp/UdpDataLink.cpp"
    "src/Udp/UdpServer.cpp"
)

set(HEADERS
    "include/IpAddress.hpp"
    "include/BaseSocket.hpp"
    "include/Lookup/HostLookup.hpp"
    "include/Lookup/InterfacesLookup.hpp"
    "include/Tcp/TcpDataLink.hpp"
    "include/Tcp/TcpServer.hpp"
    "include/Tcp/TcpClient.hpp"
    "include/Udp/UdpClient.hpp"
    "include/Udp/UdpDataLink.hpp"
    "include/Udp/UdpServer.hpp"
)

######################################################
# Build target

add_library(networkingAdapter SHARED
    ${SOURCES}
    ${HEADERS}
)

set_target_properties(networkingAdapter PROPERTIES
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)

target_link_libraries(networkingAdapter
    PUBLIC  EMBTOM::endianConversion
    PUBLIC  EMBTOM::utilsCpp
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

target_compile_definitions(networkingAdapter
    PUBLIC NETWORKING_ADAPTER_VERSION="${PROJECT_VERSION}"
)

######################################################
# Install rules
install(TARGETS networkingAdapter EXPORT networkingAdapter-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkingAdapter"
)

install(FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkingAdapter"
)

install(EXPORT networkingAdapter-targets
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/networkingAdapter"
)

install(FILES "cmake/networkingAdapter-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/networkingAdapter"
)