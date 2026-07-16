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

    if (!is_valid_hex(argv[3])) {
        printf("Error: invalid hex pattern: %s\n", argv[3]);
        return 1;
    }
    if (!is_valid_hex(argv[4])) {
        printf("Error: invalid hex replacement: %s\n", argv[4]);
        return 1;
    }

    size_t pattern_len, replacement_len;
    unsigned char* pattern = hex_decode(argv[3], &pattern_len);
    unsigned char* replacement = hex_decode(argv[4], &replacement_len);

    if (!pattern || !replacement) {
        printf("Error: failed to decode hex strings\n");
        free(pattern);
        free(replacement);
        return 1;
    }

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

    void process_block(unsigned char* buffer, size_t bytes_read,
    SearchState* state,
    unsigned char* replacement, size_t replacement_len,
    FILE* output) {

    if (!state || !buffer || !output) return;
    if (state->pattern_len == 0) {
        fwrite(buffer, 1, bytes_read, output);
        return;
    }

    size_t i = 0;
    unsigned char* pattern = state->pattern;
    size_t pattern_len = state->pattern_len;

    while (i < bytes_read) {
        int match = 1;
        for (size_t j = 0; j < pattern_len; j++) {
            if (i + j >= bytes_read || buffer[i + j] != pattern[j]) {
                match = 0;
                break;
            }
        }

        if (match) {
            if (replacement_len > 0) {
                fwrite(replacement, 1, replacement_len, output);
            }
            i += pattern_len;
        } else {
            fwrite(&buffer[i], 1, 1, output);
            i++;
        }
    }
    fclose(input);
    fclose(output);
    free(pattern);
    free(replacement);

    printf("Done! Result in %s\n", argv[2]);
    return 0;
}
