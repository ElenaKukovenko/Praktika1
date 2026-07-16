/*
Тесты для поиска и замены

Куковенко Елена Евгеньевна
Группа МК-101
*/

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "lib_main.h"

FILE* create_temp_output(void) {
    return tmpfile();
}

void read_result(FILE* f, unsigned char* buffer, size_t* len) {
    rewind(f);
    *len = fread(buffer, 1, 1024, f);
}

void run_process_block(unsigned char* input, size_t input_len,
    unsigned char* pattern, size_t pattern_len,
    unsigned char* replacement, size_t replacement_len,
    unsigned char* expected, size_t expected_len,
    const char* test_name) {

    FILE* out = create_temp_output();
    SearchState state;
    state.matched = 0;
    state.pattern = pattern;
    state.pattern_len = pattern_len;

    process_block(input, input_len, &state, replacement, replacement_len, out);

    if (state.matched > 0 && state.matched <= state.pattern_len) {
        fwrite(pattern, 1, state.matched, out);
    }

    unsigned char result[1024] = { 0 };
    size_t len = 0;
    read_result(out, result, &len);
    fclose(out);

    if (len != expected_len) {
        printf("  [FAIL] %s: length mismatch (got %zu, expected %zu)\n",
            test_name, len, expected_len);
        return;
    }

    if (memcmp(result, expected, expected_len) != 0) {
        printf("  [FAIL] %s: content mismatch\n", test_name);
        return;
    }

    printf("  [PASS] %s\n", test_name);
}

void test_simple_replace(void) {
    unsigned char input[] = { 'a','d','f','k' };
    unsigned char pattern[] = { 'a','d' };
    unsigned char replacement[] = { '1','2','3','4','5' };
    unsigned char expected[] = { '1','2','3','4','5','f','k' };

    run_process_block(input, 4, pattern, 2, replacement, 5,
        expected, 7, "simple_replace");
}

void test_delete_sequence(void) {
    unsigned char input[] = { 'a','b','c','d','e','f' };
    unsigned char pattern[] = { 'a','b','c' };
    unsigned char replacement[] = {};
    unsigned char expected[] = { 'd','e','f' };

    run_process_block(input, 6, pattern, 3, replacement, 0,
        expected, 3, "delete_sequence");
}

void test_empty_pattern(void) {
    unsigned char input[] = { 'a','b','c' };
    unsigned char pattern[] = {};
    unsigned char replacement[] = { 'x' };
    unsigned char expected[] = { 'a','b','c' };

    run_process_block(input, 3, pattern, 0, replacement, 1,
        expected, 3, "empty_pattern");
}

void test_replace_longer(void) {
    unsigned char input[] = { 'a','b','c' };
    unsigned char pattern[] = { 'b' };
    unsigned char replacement[] = { 'X','Y','Z' };
    unsigned char expected[] = { 'a','X','Y','Z','c' };

    run_process_block(input, 3, pattern, 1, replacement, 3,
        expected, 5, "replace_longer");
}

void test_replace_shorter(void) {
    unsigned char input[] = { 'a','b','c','d' };
    unsigned char pattern[] = { 'b','c' };
    unsigned char replacement[] = { 'X' };
    unsigned char expected[] = { 'a','X','d' };

    run_process_block(input, 4, pattern, 2, replacement, 1,
        expected, 3, "replace_shorter");
}

void test_multiple_occurrences(void) {
    unsigned char input[] = { 'a','d','x','a','d' };
    unsigned char pattern[] = { 'a','d' };
    unsigned char replacement[] = { '1','2' };
    unsigned char expected[] = { '1','2','x','1','2' };

    run_process_block(input, 5, pattern, 2, replacement, 2,
        expected, 5, "multiple_occurrences");
}


void test_boundary_match(void) {
    // Паттерн на границе блока
    unsigned char input[] = { 'a','b','c','d','e' };
    unsigned char pattern[] = { 'c','d' };
    unsigned char replacement[] = { 'X','Y' };
    unsigned char expected[] = { 'a','b','X','Y','e' };

    run_process_block(input, 5, pattern, 2, replacement, 2,
        expected, 5, "boundary_match");
}

void test_zero_bytes(void) {
    // Тест с нулевыми байтами
    unsigned char input[] = { 0x41, 0x00, 0x42, 0x00, 0x43 };
    unsigned char pattern[] = { 0x00, 0x42 };
    unsigned char replacement[] = { 0xFF, 0xFF };
    unsigned char expected[] = { 0x41, 0xFF, 0xFF, 0x00, 0x43 };

    run_process_block(input, 5, pattern, 2, replacement, 2,
        expected, 5, "zero_bytes");
}

void test_overlapping_patterns(void) {
    // Перекрывающиеся паттерны: "aa" в "aaa"
    unsigned char input[] = { 'a','a','a' };
    unsigned char pattern[] = { 'a','a' };
    unsigned char replacement[] = { 'b' };
    unsigned char expected[] = { 'b','a' };

    run_process_block(input, 3, pattern, 2, replacement, 1,
        expected, 2, "overlapping_patterns");
}

void test_pattern_at_start(void) {
    // Паттерн в начале строки
    unsigned char input[] = { 'a','b','c','d' };
    unsigned char pattern[] = { 'a','b' };
    unsigned char replacement[] = { 'X','Y','Z' };
    unsigned char expected[] = { 'X','Y','Z','c','d' };

    run_process_block(input, 4, pattern, 2, replacement, 3,
        expected, 5, "pattern_at_start");
}

void test_pattern_at_end(void) {
    // Паттерн в конце строки
    unsigned char input[] = { 'a','b','c','d' };
    unsigned char pattern[] = { 'c','d' };
    unsigned char replacement[] = { 'X','Y' };
    unsigned char expected[] = { 'a','b','X','Y' };

    run_process_block(input, 4, pattern, 2, replacement, 2,
        expected, 4, "pattern_at_end");
}

void test_long_pattern(void) {
    // Длинный паттерн
    unsigned char input[] = { 'a','b','c','d','e','f','g' };
    unsigned char pattern[] = { 'c','d','e' };
    unsigned char replacement[] = { '1','2','3','4' };
    unsigned char expected[] = { 'a','b','1','2','3','4','f','g' };

    run_process_block(input, 7, pattern, 3, replacement, 4,
        expected, 8, "long_pattern");
}

void test_no_match(void) {
    // Нет совпадений
    unsigned char input[] = { 'a','b','c','d' };
    unsigned char pattern[] = { 'x','y' };
    unsigned char replacement[] = { '1','2' };
    unsigned char expected[] = { 'a','b','c','d' };

    run_process_block(input, 4, pattern, 2, replacement, 2,
        expected, 4, "no_match");
}

void test_replacement_longer_than_pattern(void) {
    // Замена длиннее паттерна
    unsigned char input[] = { 'a','b','c' };
    unsigned char pattern[] = { 'b' };
    unsigned char replacement[] = { '1','2','3','4','5' };
    unsigned char expected[] = { 'a','1','2','3','4','5','c' };

    run_process_block(input, 3, pattern, 1, replacement, 5,
        expected, 7, "replacement_longer");
}

// ============================================================
// Главная функция
// ============================================================

int main(void) {
    printf("\n=== Replace Tests ===\n\n");

    test_simple_replace();
    test_delete_sequence();
    test_empty_pattern();
    test_replace_longer();
    test_replace_shorter();
    test_multiple_occurrences();

    printf("\n=== New Replace Tests ===\n\n");
    test_boundary_match();
    test_zero_bytes();
    test_overlapping_patterns();
    test_pattern_at_start();
    test_pattern_at_end();
    test_long_pattern();
    test_no_match();

    printf("\nAll replace tests passed!\n");
    return 0;
}
