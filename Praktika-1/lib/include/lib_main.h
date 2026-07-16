/*
Библиотека для замены байтовых последовательностей

Куковенко Елена Евгеньевна
МК-101
*/
#ifndef LIB_MAIN_H
#define LIB_MAIN_H

#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t matched;          // Сколько уже совпало
    unsigned char* pattern;  // Что ищем
    size_t pattern_len;      // Длина
    size_t pending_count;    // Сколько байт ожидают записи
    unsigned char pending_data[4096]; // Временно храним неподтвержденные байты
    size_t pattern_pos;      // Текущая позиция в паттерне для частичного совпадения
} SearchState;

int is_valid_hex(const char* str);

unsigned char* hex_decode(const char* hex_str, size_t* out_len);

void process_block(unsigned char* buffer, size_t bytes_read,
    SearchState* state,
    unsigned char* replacement, size_t replacement_len,
    FILE* output);

void flush_remaining(SearchState* state, FILE* output);

#endif 
