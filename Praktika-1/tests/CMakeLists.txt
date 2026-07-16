# Тесты буфера
add_executable(test_buffer
    test_buffer/test_buffer.c
)
target_link_libraries(test_buffer byte_replace_lib)
target_include_directories(test_buffer PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)
add_test(NAME BufferTest COMMAND test_buffer)

# Тесты замены
add_executable(test_replace
    test_replace/test_replace.c
)
target_link_libraries(test_replace byte_replace_lib)
target_include_directories(test_replace PRIVATE ${CMAKE_SOURCE_DIR}/lib/include)
add_test(NAME ReplaceTest COMMAND test_replace)
