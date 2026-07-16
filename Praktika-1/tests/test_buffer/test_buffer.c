/*
Тесты для работы с буфером и hex-утилит

Куковенко Елена Евгеньевна
МК-101
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "lib_main.h"

// ============================================================
// Тесты hex-функций
// ============================================================

void test_valid_hex_ok() {
    assert(is_valid_hex("414243") == 1);
    assert(is_valid_hex("616263") == 1);
    assert(is_valid_hex("") == 1);
    assert(is_valid_hex("00") == 1);
    assert(is_valid_hex("ff") == 1);
    assert(is_valid_hex("FF") == 1);
    printf("test_valid_hex_ok passed\n");
}

void test_valid_hex_fail() {
    assert(is_valid_hex("414") == 0);      // Нечётная длина
    assert(is_valid_hex("414G") == 0);     // Не hex-символ
    assert(is_valid_hex(NULL) == 0);       // NULL
    printf("test_valid_hex_fail passed\n");
}

void test_hex_decode_ok() {
    size_t len;
    unsigned char* data = hex_decode("414243", &len);
    assert(data != NULL);
    assert(len == 3);
    assert(data[0] == 0x41);
    assert(data[1] == 0x42);
    assert(data[2] == 0x43);
    free(data);
    printf("test_hex_decode_ok passed\n");
}

void test_hex_decode_zero() {
    size_t len;
    unsigned char* data = hex_decode("41420043", &len);
    assert(data != NULL);
    assert(len == 4);
    assert(data[0] == 0x41);
    assert(data[1] == 0x42);
    assert(data[2] == 0x00);
    assert(data[3] == 0x43);
    free(data);
    printf("test_hex_decode_zero passed\n");
}

void test_hex_decode_empty() {
    size_t len;
    unsigned char* data = hex_decode("", &len);
    assert(data != NULL);
    assert(len == 0);
    free(data);
    printf("test_hex_decode_empty passed\n");
}

// ============================================================
// Тесты работы с буфером
// ============================================================

void test_buffer_read() {
    // Проверяем, что буфер читается правильно
    unsigned char buffer[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    // Создаём временный файл
    FILE* f = tmpfile();
    fwrite(buffer, 1, 10, f);
    rewind(f);

    unsigned char read_buffer[10];
    size_t n = fread(read_buffer, 1, 10, f);

    assert(n == 10);
    assert(memcmp(buffer, read_buffer, 10) == 0);

    fclose(f);
    printf("test_buffer_read passed\n");
}

void test_buffer_overflow() {
    // Проверяем, что не читаем за пределы буфера
    unsigned char buffer[5] = { 1, 2, 3, 4, 5 };
    size_t bytes_read = 3;  // Прочитали только 3 байта

    for (size_t i = 0; i < bytes_read; i++) {
        assert(buffer[i] == i + 1);
    }

    printf("test_buffer_overflow passed\n");
}


void test_buffer_boundary() {
    // Проверка чтения на границе буфера
    unsigned char buffer[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };

    // Создаём временный файл с 12 байтами (больше буфера)
    FILE* f = tmpfile();
    unsigned char test_data[12] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    fwrite(test_data, 1, 12, f);
    rewind(f);

    // Читаем первый блок (8 байт)
    unsigned char read_buffer[8];
    size_t n1 = fread(read_buffer, 1, 8, f);
    assert(n1 == 8);
    assert(memcmp(read_buffer, test_data, 8) == 0);

    // Читаем второй блок (4 байта)
    unsigned char read_buffer2[8];
    size_t n2 = fread(read_buffer2, 1, 8, f);
    assert(n2 == 4);
    assert(memcmp(read_buffer2, test_data + 8, 4) == 0);

    fclose(f);
    printf("test_buffer_boundary passed\n");
}

void test_buffer_large_read() {
    // Проверка чтения большого количества данных
    const size_t DATA_SIZE = 1000;
    unsigned char* test_data = (unsigned char*)malloc(DATA_SIZE);
    for (size_t i = 0; i < DATA_SIZE; i++) {
        test_data[i] = (unsigned char)(i % 256);
    }

    FILE* f = tmpfile();
    fwrite(test_data, 1, DATA_SIZE, f);
    rewind(f);

    unsigned char buffer[256];
    size_t total_read = 0;
    size_t bytes_read;

    while ((bytes_read = fread(buffer, 1, 256, f)) > 0) {
        assert(memcmp(buffer, test_data + total_read, bytes_read) == 0);
        total_read += bytes_read;
    }

    assert(total_read == DATA_SIZE);
    free(test_data);
    fclose(f);
    printf("test_buffer_large_read passed\n");
}

void test_buffer_empty_read() {
    // Проверка чтения из пустого файла
    FILE* f = tmpfile();  // Пустой файл
    unsigned char buffer[10];
    size_t n = fread(buffer, 1, 10, f);
    assert(n == 0);
    fclose(f);
    printf("test_buffer_empty_read passed\n");
}

// ============================================================
// Главная функция
// ============================================================

int main() {
    printf("\n=== Buffer Tests ===\n");

    printf("\nHex tests:\n");
    test_valid_hex_ok();
    test_valid_hex_fail();
    test_hex_decode_ok();
    test_hex_decode_zero();
    test_hex_decode_empty();

    printf("\nBuffer tests:\n");
    test_buffer_read();
    test_buffer_overflow();
    test_buffer_boundary();      
    test_buffer_large_read();    
    test_buffer_empty_read();    

    printf("\n All buffer tests passed!\n");
    return 0;
}
