# ============================================================
# Статическая библиотека
# ============================================================
add_library(byte_replace_lib STATIC
    src/lib_main.c
)

target_include_directories(byte_replace_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================
# Динамическая библиотека
# ============================================================
add_library(byte_replace_lib_shared SHARED
    src/lib_main.c
)

target_include_directories(byte_replace_lib_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================
# Настройка свойств библиотек
# ============================================================

# Статическая библиотека
set_target_properties(byte_replace_lib PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build
    OUTPUT_NAME "byte_replace_lib"
)

# Динамическая библиотека
set_target_properties(byte_replace_lib_shared PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build      # Linux .so
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build      # Windows .dll
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/out/build      # Windows .lib (импорт)
    OUTPUT_NAME "byte_replace_lib"
)

# ============================================================
# Установка флагов компиляции
# ============================================================

# Для статической библиотеки
target_compile_definitions(byte_replace_lib PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

# Для динамической библиотеки
target_compile_definitions(byte_replace_lib_shared PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)

# ============================================================
# Если нужно экспортировать символы в Windows
# ============================================================

if(WIN32)
    target_compile_definitions(byte_replace_lib_shared PRIVATE
        BUILDING_BYTE_REPLACE_LIB
    )
    target_compile_definitions(byte_replace_lib_shared PUBLIC
        USING_BYTE_REPLACE_LIB
    )
endif()

# ============================================================
# Позиционно-независимый код (для динамической библиотеки)
# ============================================================

set_target_properties(byte_replace_lib_shared PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

# ============================================================
# Сообщение о сборке
# ============================================================

message(STATUS "Configuring byte_replace_lib (static)...")
message(STATUS "Configuring byte_replace_lib_shared (shared)...")
