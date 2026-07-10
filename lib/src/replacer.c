/*
Реализация замены

Кучуков Ридаль Радикович
МК-101
*/

#include "replacer.h"

replacer strtobytes_smart(char *input_str) {
    if (input_str == NULL) {
        replacer r = {NULL, 0, 0};
        return r;
    }

    if (strncmp(input_str, "0x", 2) == 0) {
        if (strlen(input_str) <= 2) {
             replacer r = {NULL, 0, 0}; // если на входе только 0x
             return r;
        }
        // Если хекс strtobyte для строки без 0x
        return strtobyte(input_str + 2);
    }

    // обрабатываем дефолтный текст
    replacer r = {NULL, 0, 0};
    r.length = strlen(input_str);
    r.data = (uint8_t *)malloc(r.length);
    
    if (r.data == NULL) {
        r.length = 0;
        return r;
    }

    memcpy(r.data, input_str, r.length);
    r.match_idx = 0;
    return r;
}

replacer strtobyte(char *input_str)
{
    replacer r = {NULL, 0, 0};
    if (input_str == NULL)
        return r;

    size_t str_len = strlen(input_str);

    // строка не может быть нечетной
    if (str_len % 2 != 0)
    {
        return r;
    }

    r.length = str_len / 2;
    r.data = (uint8_t *)malloc(r.length);
    if (r.data == NULL)
    {
        r.length = 0;
        return r;
    }

    for (size_t i = 0; i < r.length; i++)
    {
        char buf[3] = {input_str[i * 2], input_str[i * 2 + 1], '\0'};

        // если не hex
        if (!isxdigit((unsigned char)buf[0]) || !isxdigit((unsigned char)buf[1]))
        {
            free_replacer(&r);
            return r;
        }

        char *endptr;
        long val = strtol(buf, &endptr, 16);

        // проверка strtol
        if (endptr == buf || *endptr != '\0')
        {
            free_replacer(&r);
            return r;
        }

        // пределы байта
        if (val < 0 || val > 0xFF)
        {
            free_replacer(&r);
            return r;
        }

        r.data[i] = (uint8_t)val;
    }
    return r;
}

void free_replacer(replacer *r)
{
    if (r == NULL)
        return;
    free(r->data);
    r->data = NULL;
    r->length = 0;
    r->match_idx = 0;
}
