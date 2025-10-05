//
// Created by rmichau on 10/3/25.
//

#ifndef CLOX_VALUE_H
#define CLOX_VALUE_H

typedef double Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void initValueArray(ValueArray* array);
void writeValueArray(ValueArray* array, Value value);
void freeValueArray(ValueArray* array);
#endif //CLOX_VALUE_H