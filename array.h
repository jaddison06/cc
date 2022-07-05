#pragma once

#include <stdlib.h>

#define APPEND(array, item, currentLength, currentCapacity) _append((void**)&(array), &(item), sizeof(item), &(currentLength), &(currentCapacity))

void _append(void** array, void* item, size_t size, int* currentLength, int* currentCapacity);

