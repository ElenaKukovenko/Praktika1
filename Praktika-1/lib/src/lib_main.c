/*
Реализация библиотеки для замены байтовых последовательностей

Куковенко Елена Евгеньевна
Группа МК-101
*/

#include "lib_main.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// ============================================================
// HEX-ФУНКЦИИ
// ============================================================

int is_valid_hex(const char* str) {
    if (!str) return 0;
    size_t len = strlen(str);
    if (len % 2 != 0) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isxdigit((unsigned char)str[i])) return 0;
    }
    return 1;
}

unsigned char* hex_decode(const char* hex_str, size_t* out_len) {
    if (!hex_str || !out_len) return NULL;
    size_t len = strlen(hex_str);
    if (len % 2 != 0) return NULL;

    *out_len = len / 2;

    if (*out_len == 0) {
        unsigned char* result = malloc(1);
        if (result) *result = 0;
        return result;
    }

    unsigned char* result = (unsigned char*)malloc(*out_len);
    if (!result) return NULL;

    for (size_t i = 0; i < *out_len; i++) {
        char hex_byte[3] = { hex_str[2 * i], hex_str[2 * i + 1], '\0' };
        char* endptr;
        long value = strtol(hex_byte, &endptr, 16);
        if (*endptr != '\0' || value < 0 || value > 255) {
            free(result);
            return NULL;
        }
        result[i] = (unsigned char)value;
    }
    return result;
}
