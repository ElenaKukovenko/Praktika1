cmake_minimum_required(VERSION 3.10)
project(Praktika1 C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Размер буфера
if(NOT DEFINED BUFFER_SIZE)
    set(BUFFER_SIZE 4096)
endif()
add_compile_definitions(BUFFER_SIZE=${BUFFER_SIZE})

# Поддиректории
add_subdirectory(lib)
add_subdirectory(app)

# Выходные директории
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build)

# Тесты
enable_testing()
add_subdirectory(tests)
