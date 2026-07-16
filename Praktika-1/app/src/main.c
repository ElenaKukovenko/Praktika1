/*
Главная программа для замены байтовых последовательностей

Куковенко Елена Евгеньевна
МК-101
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib_main.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s <input> <output> <pattern_hex> <replacement_hex>\n", argv[0]);
        return 1;
    }

    // Проверяем hex-строки
    if (!is_valid_hex(argv[3])) {
        printf("Error: invalid hex pattern: %s\n", argv[3]);
        return 1;
    }
    if (!is_valid_hex(argv[4])) {
        printf("Error: invalid hex replacement: %s\n", argv[4]);
        return 1;
    }

    // Декодируем
    size_t pattern_len, replacement_len;
    unsigned char* pattern = hex_decode(argv[3], &pattern_len);
    unsigned char* replacement = hex_decode(argv[4], &replacement_len);

    if (!pattern || !replacement) {
        printf("Error: failed to decode hex strings\n");
        free(pattern);
        free(replacement);
        return 1;
    }

    // Открываем файлы
    FILE* input = fopen(argv[1], "rb");
    if (!input) {
        printf("Error: cannot open %s\n", argv[1]);
        free(pattern);
        free(replacement);
        return 1;
    }

    FILE* output = fopen(argv[2], "wb");
    if (!output) {
        printf("Error: cannot create %s\n", argv[2]);
        fclose(input);
        free(pattern);
        free(replacement);
        return 1;
    }

    SearchState state;
    state.matched = 0;
    state.pattern = pattern;
    state.pattern_len = pattern_len;
    state.pending_count = 0;      
    state.pattern_pos = 0;        

    // Обрабатываем файл блоками
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;

    if (pattern_len == 0) {
        printf("Warning: empty pattern, copying file without changes\n");
        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
            fwrite(buffer, 1, bytes_read, output);
        }
    }
    else {
        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input)) > 0) {
            process_block(buffer, bytes_read, &state,
                replacement, replacement_len, output);
        }

        // Записываем оставшиеся данные                     
        flush_remaining(&state, output);                   
    }

    // Закрываем и освобождаем
    fclose(input);
    fclose(output);
    free(pattern);
    free(replacement);

    printf("Result in %s\n", argv[2]);
    return 0;
}