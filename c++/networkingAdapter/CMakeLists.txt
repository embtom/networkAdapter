project(networkingAdapter)

include(GNUInstallDirs)

######################################################
#Sources and Headers to build
set(SOURCES_NA  
    "src/IpAddress.cpp"
    "src/BaseSocket.cpp"
    "src/HostName.cpp"
    "src/BaseDataLink.cpp"
    "src/stream/StreamDataLink.cpp"
    "src/stream/StreamServer.cpp"
    "src/stream/StreamClient.cpp"
    "src/dgram/DgramDataLink.cpp"
    "src/dgram/DgramServer.cpp"
    "src/dgram/DgramClient.cpp"
)

set(HEADERS_NA  
    "include/IpAddress.hpp"
    "include/BaseSocket.hpp"
    "include/HostName.h"
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
add_library(${PROJECT_NAME} SHARED ${SOURCES_NA} ${HEADERS_NA})
set_target_properties(${PROJECT_NAME} PROPERTIES 
    DEBUG_POSTFIX  ${CMAKE_DEBUG_POSTFIX}
    CXX_STANDARD   ${CMAKE_CXX_STANDARD}
    CXX_EXTENSIONS ${CMAKE_CXX_EXTENSIONS}
)
target_link_libraries(${PROJECT_NAME} PRIVATE        
    PRIVATE EMBTOM::utilsCpp 
    PRIVATE Threads::Threads
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}
)

######################################################
#Install rules
install(TARGETS ${PROJECT_NAME} EXPORT networkingadapter-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}"
)

install(FILES ${HEADERS_NA}
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}"
)

install(EXPORT networkingadapter-targets
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)

install(FILES "cmake/networkingadapter-config.cmake"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)