
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
    "include/BaseSocket.hpp"
    "include/IpAddress.hpp"
    "include/NetAdapter.hpp"
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

add_library(networkadapter SHARED
    ${SOURCES}
    ${HEADERS}
)
add_library(EMBTOM::networkadapter ALIAS networkadapter)

target_link_libraries(networkadapter
    PUBLIC  EMBTOM::endianconversion
    PUBLIC  EMBTOM::utilsCpp
    PRIVATE Threads::Threads
)

target_include_directories(networkadapter PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkadapter>
)

set_target_properties(networkadapter PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)

target_compile_definitions(networkadapter
    PUBLIC NETWORKING_ADAPTER_VERSION="${PROJECT_VERSION}"
)

######################################################
# Install rules
install(TARGETS networkadapter EXPORT networkadapter-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkadapter"
)

install(FILES ${HEADERS}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/networkadapter"
)

install(EXPORT networkadapter-targets
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/networkadapter"
)

install(FILES "cmake/networkadapter-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/networkadapter"
)