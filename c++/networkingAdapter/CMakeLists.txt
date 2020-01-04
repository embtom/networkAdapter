project(networkingAdapter)

include(GNUInstallDirs)
#######################################################################################
# settings 
#######################################################################################
set(SOURCES_NA  src/IpAddress.cpp
                src/BaseSocket.cpp
                src/HostName.cpp
                src/stream/StreamDataLink.cpp
                src/stream/StreamServer.cpp
                src/stream/StreamClient.cpp
                src/dgram/DgramServer.cpp
                src/dgram/DgramClient.cpp)

set (HEADERS_NA include/IpAddress.hpp
                include/BaseSocket.hpp
                include/HostName.h
                include/stream/StreamDataLink.hpp
                include/stream/StreamServer.hpp
                include/stream/StreamClient.hpp
                include/dgram/DgramServer.hpp
                include/dgram/DgramClient.hpp)
                
#######################################################################################
#Build target settings
#######################################################################################
add_library(${PROJECT_NAME} SHARED ${SOURCES_NA} ${HEADERS_NA})
set_target_properties(${PROJECT_NAME} PROPERTIES 
    DEBUG_POSTFIX  "_g"
    CXX_STANDARD   17
    CXX_EXTENSIONS yes
)
target_link_libraries(${PROJECT_NAME} PRIVATE utilsCpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}>
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    SOVERSION ${VERSION_MAJOR}
)

######################################################
#Install
######################################################

install(TARGETS ${PROJECT_NAME} EXPORT networkingAdapter-config
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}"
)
install(DIRECTORY include
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/embtom/${PROJECT_NAME}"
)
install(EXPORT networkingAdapter-config
    NAMESPACE
        EMBTOM::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
)
