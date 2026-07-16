add_executable(byte_replace src/main.c)
target_link_libraries(byte_replace byte_replace_lib)
target_include_directories(byte_replace PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)

set_target_properties(byte_replace PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build
)
