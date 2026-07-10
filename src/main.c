/*
мейн

Кучуков Ридаль Радикович
МК-101
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "replacer.h"

int main(int argc, char *argv[])
{
    // Проверяем аргв
    if (argc != 5)
    {
        fprintf(stderr, "Использование: %s <входной_файл> <выходной_файл> <шаблон_поиска> <шаблон_замены>\n", argv[0]);
        fprintf(stderr, "Пример: %s in.txt out.txt \"test\" \"0x31323334\"\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Ошибка: Не удалось открыть входной файл %s\n", argv[1]);
        fclose(in);
        return 1;
    }

    FILE *out = fopen(argv[2], "wb");
    if (out == NULL)
    {
        fprintf(stderr, "Ошибка: Не удалось создать выходной файл %s\n", argv[2]);
        fclose(in);
        return 1;
    }

    //аргументы поиска и замены
    replacer search = strtobytes_smart(argv[3]);
    replacer replace = strtobytes_smart(argv[4]);

    if (search.length == 0 || search.data == NULL)
    {
        fprintf(stderr, "Ошибка: Некорректный или пустой шаблон поиска.\n");
        free_replacer(&search);
        free_replacer(&replace);
        fclose(in);
        fclose(out);
        return 1;
    }

    int result = replace_stream(in, out, &search, &replace);

    if (result != 0)
    {
        fprintf(stderr, "Ошибка во время обработки потока.\n");
    }

    free_replacer(&search);
    free_replacer(&replace);

    fclose(in);
    fclose(out);

    return result == 0 ? 0 : 1;
}