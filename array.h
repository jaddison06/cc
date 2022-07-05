#pragma once

#include <stdlib.h>

#define APPEND(array, item, currentLength, currentCapacity) _append((void**)&((array).root), &(item), sizeof(item), &(array).len, &(array).cap)

#define INIT(array, type) do { \
    array.root = malloc(sizeof(type)); \
    array.cap = 1; \
    array.len = 0; \
} while(0)

#define ARRAYTYPE(type) typedef struct { \
    type* root; \
    int len, cap; \
} ##type##_List;

void _append(void** array, void* item, size_t size, int* currentLength, int* currentCapacity);

