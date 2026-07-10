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

    // Пропускаем пробелы находим начало данных
    char *ptr = input_str;
    while (isspace((unsigned char)*ptr)) {
        ptr++;
    }

    // Проверяем 0x или 0X
    if (ptr[0] == '0' && (ptr[1] == 'x' || ptr[1] == 'X')) {
        // Проверяем, есть ли что-то после 0x/0X
        if (ptr[2] == '\0') {
             replacer r = {NULL, 0, 0}; // если на входе только 0x
             return r;
        }
        // Если хекс strtobyte для строки без 0x
        return strtobyte(ptr + 2);
    }

    // обрабатываем дефолтный текст
    replacer r = {NULL, 0, 0};
    // Используем ptr, чтобы убрать пробелы
    size_t data_len = strlen(ptr); 
    
    //+1 для '\0' 
    r.data = (uint8_t *)malloc(data_len + 1);
    
    if (r.data == NULL) {
        r.length = 0;
        return r;
    }

    memcpy(r.data, ptr, data_len);
    r.data[data_len] = '\0';
    // не от r.data[data_len] потому что в нем /0
    r.length = data_len;
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
