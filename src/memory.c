//
// Created by rmichau on 9/28/25.
//

#include "memory.h"

#include <string.h>


void* reallocate(void* pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);
    if (result == NULL) exit(1);
    return result;

}
