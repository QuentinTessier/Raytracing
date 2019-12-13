#pragma once

#include <stdlib.h>

typedef struct hitlist {
    void *memory;
    size_t size;
} hitlist;

inline __attribute__((always_inline)) hitlist new_hitlist()
{
    return (hitlist){0, 0};
}

void record(hitlist *list, short int type, size_t type_size, void *tmp_data);
void destroy_hitlist(hitlist list);