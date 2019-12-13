#include <string.h>
#include <stdio.h>
#include "hit_list.h"

void record(hitlist *list, short int type, size_t type_size, void *tmp_data)
{
    list->memory = realloc(list->memory, list->size + (sizeof(short int) + type_size));

    if (list->memory == 0)
        return;
    char *tmp = (char *)list->memory + list->size;
    memcpy(tmp, &type, sizeof(short int));
    memcpy(tmp + sizeof(short int), tmp_data, type_size);
    list->size += (sizeof(short int) + type_size);
}

void destroy_hitlist(hitlist list)
{
    free(list.memory);
    list.size = 0;
}