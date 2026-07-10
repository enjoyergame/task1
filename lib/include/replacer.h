/*
Только функции замены

Кучуков Ридаль Радикович
МК-101
*/

#ifndef REPLACER_H
#define REPLACER_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    uint8_t *data; // Буфер
    size_t length; // Количество байтов в буфере
    size_t match_idx; // Внутренний индекс
} replacer;

replacer strtobytes_smart(char *input_str);
void free_replacer(replacer *r);
int replace_stream(FILE *in, FILE *out, replacer *search, replacer *replace);

#endif