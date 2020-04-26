
add_library(ComProto INTERFACE)

target_include_directories (ComProto
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ComProto INTERFACE
    EMBTOM::endianconversion
)