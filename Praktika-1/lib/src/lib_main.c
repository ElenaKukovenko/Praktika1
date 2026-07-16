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
