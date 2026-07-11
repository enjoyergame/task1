/*
tests
Кучуков Ридаль Радикович
МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "replacer.h"


void create_test_file(const char *filename, const char *content, size_t length) {
    FILE *f = fopen(filename, "wb");
    if (f) {
        fwrite(content, 1, length, f);
        fclose(f);
    }
}

int check_file_content(const char *filename, const char *expected, size_t exp_len) {
    FILE *f = fopen(filename, "rb");
    if (!f) return 0;
    
    char buffer[2048] = {0};
    size_t read_bytes = fread(buffer, 1, sizeof(buffer), f);
    fclose(f);
    if (read_bytes != exp_len) return 0;
    return memcmp(buffer, expected, exp_len) == 0;
}

int main() {
    printf("Helper functions compiled successfully!\n");
    return 0;
}