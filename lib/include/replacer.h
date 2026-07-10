/*
Только функции замены

Кучуков Ридаль Радикович
МК-101
*/

#ifndef REPLACER_H
#define REPLACER_H
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint8_t *data;
    size_t length;
    size_t match_idx;
} replacer;

replacer strtobyte(char *input_str);
replacer strtobytes_smart(char *input_str);
void free_replacer(replacer *r);

#endif