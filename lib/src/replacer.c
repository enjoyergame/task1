/*
Реализация замены

Кучуков Ридаль Радикович
МК-101
*/

#include "replacer.h"
#include "utils.h"

replacer strtobytes_smart(char *input_str)
{
    //проверка
    if (input_str == NULL)
    {
        replacer r = {NULL, 0, 0};
        return r;
    }

    // Пропускаем пробелы находим начало данных
    char *ptr = input_str;
    while (isspace((unsigned char)*ptr))//isspace тру если пробелы табы переносы и тд ансайнг чар что бы не сломать её отрицательными кодами
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
    // Используем ptr т.к там нет пробелов
    size_t data_len = strlen(ptr);

    //память +1 для '\0'
    r.data = (uint8_t *)malloc(data_len + 1);

    if (r.data == NULL)
    {
        r.length = 0;
        return r;
    }
    //копируем в r.data из птр
    memcpy(r.data, ptr, data_len); // копируем символы в память как байты
    r.data[data_len] = '\0';
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

//
static void feed_byte(FILE *out, replacer *search, replacer *replace, uint8_t c)
{
    // Сравниваем текущий байт файла с тем, что мы ожидали по индексу match_idx
    if (c == search->data[search->match_idx])
    {
        search->match_idx++;
        // Если собрали полное совпадение пишем в файл
        if (search->match_idx == search->length)
        {
            fwrite(replace->data, 1, replace->length, out);
            search->match_idx = 0;
        }
        return;
    }

    // Если совпадение сорвалось
    if (search->match_idx == 0)//если метч индекс остался 0 в прошлом ифе просто пишем байт
    {
        fwrite(&c, 1, 1, out);
    }
    else//если метч индекс был больше нуля но сейчас  не совпало
    {
        fwrite(&search->data[0], 1, 1, out);//пишем первый байтик (логично что он не подходит)

        size_t failed_match_len = search->match_idx;
        search->match_idx = 0;

        for (size_t j = 1; j < failed_match_len; j++)//реурсивно проверяем все что мы запомнили
        {
            feed_byte(out, search, replace, search->data[j]);
        }

        feed_byte(out, search, replace, c);//тот самый байтик который вызвал срыв
    }
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

    while ((bytes_read = fread(buffer, 1, N, in)) > 0)//читаем в буффер по 1 байтику N элементов (байтов) из ин
    {
        for (size_t i = 0; i < bytes_read; i++)//байтс рид равен N
        {
            feed_byte(out, search, replace, buffer[i]);//передаем в фид байт по байтику
        }
    }
    //записываем хвост (все что мы запомнили а файл кончился)
    if (search->match_idx > 0)
    {
        fwrite(search->data, 1, search->match_idx, out);
        search->match_idx = 0;
    }

    return 0;
}
