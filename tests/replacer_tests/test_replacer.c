/*
tests

Кучуков Ридаль Радикович
МК-101
*/

#include "unity.h"
#include "replacer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void create_test_file(const char *filename, const char *content, size_t length)
{
    FILE *f = fopen(filename, "wb");
    if (f)
    {
        fwrite(content, 1, length, f);
        fclose(f);
    }
}

int check_file_content(const char *filename, const char *expected, size_t exp_len)
{
    FILE *f = fopen(filename, "rb");
    if (!f)
        return 0;

    char buffer[2048] = {0};
    size_t read_bytes = fread(buffer, 1, sizeof(buffer), f);
    fclose(f);

    if (read_bytes != exp_len)
        return 0;
    return memcmp(buffer, expected, exp_len) == 0;
}

void test_hex_parsing(void)
{
    replacer r = strtobytes_smart("0x610062");

    TEST_ASSERT_EQUAL_INT(3, r.length);
    TEST_ASSERT_NOT_NULL(r.data);
    TEST_ASSERT_EQUAL_HEX8(0x61, r.data[0]);
    TEST_ASSERT_EQUAL_HEX8(0x00, r.data[1]);
    TEST_ASSERT_EQUAL_HEX8(0x62, r.data[2]);

    free_replacer(&r);
}

void test_text_parsing(void)
{
    replacer r = strtobytes_smart("abc");

    TEST_ASSERT_EQUAL_INT(3, r.length);
    TEST_ASSERT_NOT_NULL(r.data);
    TEST_ASSERT_EQUAL_HEX8('a', r.data[0]);
    TEST_ASSERT_EQUAL_HEX8('b', r.data[1]);
    TEST_ASSERT_EQUAL_HEX8('c', r.data[2]);

    free_replacer(&r);
}

void test_invalid_hex(void)
{
    replacer r = strtobytes_smart("0x123");

    TEST_ASSERT_EQUAL_INT(0, r.length);
    TEST_ASSERT_NULL(r.data);

    free_replacer(&r);
}

void test_basic_replace(void)
{
    create_test_file("in_test4.bin", "A", 1);

    FILE *in = fopen("in_test4.bin", "rb");
    FILE *out = fopen("out_test4.bin", "wb");

    replacer s = strtobytes_smart("0x41");
    replacer r = strtobytes_smart("0x42");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test4.bin", "B", 1));

    free_replacer(&s);
    free_replacer(&r);
}

void test_word_replace(void)
{
    create_test_file("in_test5.bin", "hello_test_world", 16);

    FILE *in = fopen("in_test5.bin", "rb");
    FILE *out = fopen("out_test5.bin", "wb");

    replacer s = strtobytes_smart("test");
    replacer r = strtobytes_smart("1234");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test5.bin", "hello_1234_world", 16));

    free_replacer(&s);
    free_replacer(&r);
}

void test_overlap_replace(void)
{
    create_test_file("in_test6.bin", "aaab", 4);

    FILE *in = fopen("in_test6.bin", "rb");
    FILE *out = fopen("out_test6.bin", "wb");

    replacer s = strtobytes_smart("aab");
    replacer r = strtobytes_smart("X");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test6.bin", "aX", 2));

    free_replacer(&s);
    free_replacer(&r);
}

void test_eof_partial_match(void)
{
    create_test_file("in_test7.bin", "hello_te", 8);

    FILE *in = fopen("in_test7.bin", "rb");
    FILE *out = fopen("out_test7.bin", "wb");

    replacer s = strtobytes_smart("test");
    replacer r = strtobytes_smart("X");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test7.bin", "hello_te", 8));

    free_replacer(&s);
    free_replacer(&r);
}

void test_boundary_split(void)
{
    size_t file_size = 1026;
    char *content = malloc(file_size);
    TEST_ASSERT_NOT_NULL(content);
    memset(content, 'A', file_size);

    content[1022] = 't';
    content[1023] = 'e';
    content[1024] = 's';
    content[1025] = 't';

    create_test_file("in_test8.bin", content, file_size);
    free(content);

    FILE *in = fopen("in_test8.bin", "rb");
    FILE *out = fopen("out_test8.bin", "wb");

    replacer s = strtobytes_smart("test");
    replacer r = strtobytes_smart("X");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    char *expected = malloc(1023);
    TEST_ASSERT_NOT_NULL(expected);
    memset(expected, 'A', 1022);
    expected[1022] = 'X';

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test8.bin", expected, 1023));

    free(expected);
    free_replacer(&s);
    free_replacer(&r);
}

void test_empty_file(void)
{
    create_test_file("in_test9.bin", "", 0);

    FILE *in = fopen("in_test9.bin", "rb");
    FILE *out = fopen("out_test9.bin", "wb");

    replacer s = strtobytes_smart("test");
    replacer r = strtobytes_smart("X");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test9.bin", "", 0));

    free_replacer(&s);
    free_replacer(&r);
}

void test_deletion(void)
{
    create_test_file("in_test10.bin", "hello_test_world", 16);

    FILE *in = fopen("in_test10.bin", "rb");
    FILE *out = fopen("out_test10.bin", "wb");

    replacer s = strtobytes_smart("test");
    replacer r = strtobytes_smart("");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test10.bin", "hello__world", 12));

    free_replacer(&s);
    free_replacer(&r);
}

void test_large_pattern(void)
{
    size_t pat_size = 1100;
    char *search_str = malloc(pat_size + 1);
    TEST_ASSERT_NOT_NULL(search_str);
    memset(search_str, 'A', pat_size);
    search_str[pat_size] = '\0';

    char *content = malloc(pat_size + 2);
    TEST_ASSERT_NOT_NULL(content);
    memset(content, 'A', pat_size);
    content[pat_size] = 'B';
    content[pat_size + 1] = '\0';

    create_test_file("in_test11.bin", content, pat_size + 1);

    FILE *in = fopen("in_test11.bin", "rb");
    FILE *out = fopen("out_test11.bin", "wb");

    replacer s = strtobytes_smart(search_str);
    replacer r = strtobytes_smart("X");

    int result = replace_stream(in, out, &s, &r);
    fclose(in);
    fclose(out);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(check_file_content("out_test11.bin", "XB", 2));

    free(search_str);
    free(content);
    free_replacer(&s);
    free_replacer(&r);
}

// --- ТОЧКА ВХОДА UNITY ---

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_hex_parsing);
    RUN_TEST(test_text_parsing);
    RUN_TEST(test_invalid_hex);
    RUN_TEST(test_basic_replace);
    RUN_TEST(test_word_replace);
    RUN_TEST(test_overlap_replace);
    RUN_TEST(test_eof_partial_match);
    RUN_TEST(test_boundary_split);
    RUN_TEST(test_empty_file);
    RUN_TEST(test_deletion);
    RUN_TEST(test_large_pattern);

    return UNITY_END();
}