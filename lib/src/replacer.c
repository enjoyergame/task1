/*
Реализация замены

Кучуков Ридаль Радикович
МК-101
*/

#include "replacer.h"
#include "utils.h"

replacer strtobytes_smart(char *input_str)
{
    if (input_str == NULL)
    {
        replacer r = {NULL, 0, 0};
        return r;
    }

    // Пропускаем пробелы находим начало данных
    char *ptr = input_str;
    while (isspace((unsigned char)*ptr))
    {
        ptr++;
    }

    // Проверяем 0x или 0X
    if (ptr[0] == '0' && (ptr[1] == 'x' || ptr[1] == 'X'))
    {
        // Проверяем, есть ли что-то после 0x/0X
        if (ptr[2] == '\0')
        {
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

    if (r.data == NULL)
    {
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

void free_replacer(replacer *r)
{
    if (r == NULL)
        return;
    free(r->data);
    r->data = NULL;
    r->length = 0;
    r->match_idx = 0;
}

int replace_stream(FILE *in, FILE *out, replacer *search, replacer *replace)
{
    if (!in || !out || !search || !replace || search->length == 0)
    {
        return -1;
    }
    uint8_t buffer[N];
    size_t bytes_read;
    search->match_idx = 0;

    while ((bytes_read = fread(buffer, 1, N, in)) > 0)
    {
        for (size_t i = 0; i < bytes_read; i++)
        {
            if (buffer[i] == search->data[search->match_idx])
            {
                search->match_idx++;
                
                if (search->match_idx == search->length)
                {
                    fwrite(replace->data, 1, replace->length, out);
                    search->match_idx = 0;
                }
            }
            else
            {
                if (search->match_idx > 0)
                {
                    fwrite(search->data, 1, search->match_idx, out);
                    search->match_idx = 0;
                    
                    if (buffer[i] == search->data[0]) {
                        search->match_idx = 1;
                        continue;
                    }
                }
                fwrite(&buffer[i], 1, 1, out);
            }
        }
    }

    return 0;
}
