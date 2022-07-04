#include "array.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void _append(void** array, void* item, size_t size, int* currentLength, int* currentCapacity) {
    if (*currentLength == *currentCapacity) {
        *array = realloc(*array, *currentCapacity * size * 2);
        *currentCapacity *= 2;
    }
    memcpy(&((*(char**)array)[(*currentLength) * size]), item, size);
    *currentLength += 1;
}