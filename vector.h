#pragma once

#include <stdlib.h>

#define APPEND(vec, item) _append((void**)&((vec).root), &(item), sizeof(item), &(vec).len, &(vec).cap)

#define INIT(vec) do { \
    vec.root = malloc(sizeof(*vec.root)); \
    vec.cap = 1; \
    vec.len = 0; \
} while(0)

#define DESTROY(vec) free(vec.root)

#define DECL_VEC(type, name) typedef struct { \
    type* root; \
    int len, cap; \
} name;


void _append(void** vec, void* item, size_t size, int* currentLength, int* currentCapacity);

