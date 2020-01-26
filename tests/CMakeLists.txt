

add_subdirectory(TST_baseSocket)
add_subdirectory(TST_Common)
add_subdirectory(TST_DgramConnection)
add_subdirectory(TST_EndianConversion)
add_subdirectory(TST_StreamConnection)
add_subdirectory(TST_IpAddress)

add_test(NAME TST_EnumReflect COMMAND TST_EnumReflect)
add_test(NAME TST_FdSet COMMAND TST_FdSet)
add_test(NAME TST_MakeVector COMMAND TST_MakeVector)
                        
