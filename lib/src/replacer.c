/*
Реализация замены

Кучуков Ридаль Радикович
МК-101
*/

#include "replacer.h"

replacer strtobyte(char* input_str) {
    replacer r = {NULL, 0, 0};
    if(input_str == NULL) return r;
    size_t str_len = strlen(input_str);
    r.length = str_len / 2;
    r.data = (uint8_t*)malloc(r.length);
    if (r.data == NULL) {
    r.length = 0;
    return r;     
}
    for (size_t i = 0; i < (r.length); i++){
            char buf[3] = {input_str[i * 2], input_str[i * 2 + 1], '\0'};
            r.data[i] = (uint8_t)strtol(buf, NULL, 16);
    }
    return r;
}

void free_replacer(replacer* r) {
    if (r == NULL) return;

    free(r->data);
    r->data = NULL;
    r->length = 0;
    r->match_idx = 0;
}