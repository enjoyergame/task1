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

int test_hex_parsing() {
    printf("Test 1 (HEX Parsing): ");
    replacer r = strtobytes_smart("0x610062"); 

    if (r.length == 3 && r.data[0] == 0x61 && r.data[1] == 0x00 && r.data[2] == 0x62) {
        printf("PASSED\n");
        free_replacer(&r);
        return 1;
    }
    printf("FAILED\n");
    free_replacer(&r);
    return 0;
}


int test_text_parsing() {
    printf("Test 2 (Text Parsing): ");
    replacer r = strtobytes_smart("abc");
    
    if (r.length == 3 && r.data[0] == 'a' && r.data[1] == 'b' && r.data[2] == 'c') {
        printf("PASSED\n");
        free_replacer(&r);
        return 1;
    }
    printf("FAILED\n");
    free_replacer(&r);
    return 0;
}


int test_invalid_hex() {
    printf("Test 3 (Invalid HEX): ");
    replacer r = strtobytes_smart("0x123"); 

    if (r.length == 0 && r.data == NULL) {
        printf("PASSED\n");
        free_replacer(&r);
        return 1;
    }
    printf("FAILED\n");
    free_replacer(&r);
    return 0;
}

int test_basic_replace() {
    printf("Test 4 (Basic Replace): ");
    create_test_file("in_test4.bin", "A", 1);
    
    FILE *in = fopen("in_test4.bin", "rb");
    FILE *out = fopen("out_test4.bin", "wb");
    
    replacer s = strtobytes_smart("0x41");
    replacer r = strtobytes_smart("0x42");
    
    replace_stream(in, out, &s, &r);
    
    fclose(in); 
    fclose(out);

    int passed = check_file_content("out_test4.bin", "B", 1);
    printf(passed ? "PASSED\n" : "FAILED\n");
    
    free_replacer(&s); 
    free_replacer(&r);
    return passed;
}

int test_word_replace() {
    printf("Test 5 (Word Replace): ");
    create_test_file("in_test5.bin", "hello_test_world", 16);
    
    FILE *in = fopen("in_test5.bin", "rb");
    FILE *out = fopen("out_test5.bin", "wb");
    
    replacer s = strtobytes_smart("test"); 
    replacer r = strtobytes_smart("1234"); 
    
    replace_stream(in, out, &s, &r);
    
    fclose(in); 
    fclose(out);
    
    int passed = check_file_content("out_test5.bin", "hello_1234_world", 16);
    printf(passed ? "PASSED\n" : "FAILED\n");
    
    free_replacer(&s); 
    free_replacer(&r);
    return passed;
}

int main() {
    printf("--- Running Replacer Tests ---\n");
    int passed_tests = 0;
    
    passed_tests += test_hex_parsing();
    passed_tests += test_text_parsing();
    passed_tests += test_invalid_hex();
    passed_tests += test_basic_replace();
    passed_tests += test_word_replace();
    
    printf("------------------------------\n");
    printf("Total passed: %d/5\n", passed_tests);
    

    if (passed_tests < 5) {
        return 1; 
    }
    return 0;
}