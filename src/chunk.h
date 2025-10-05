//
// Created by rmichau on 9/28/25.
//

#ifndef CLOX_CHUNK_H
#define CLOX_CHUNK_H
#include "common.h"
#include "value.h"


typedef enum {
    // OP_RETURN
    OP_RETURN,
    // OP_CONSTANT, CONSTANT_ADDR
    OP_CONSTANT,
    // OP_CONSTANT, CONSTANT_ADDR
    OP_CONSTANT_LONG,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
} OpCode;

typedef struct {
    int count;
    int capacity;
    int *lines;
    uint8_t *code;
    ValueArray constants;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk *chunk, Value value);

void writeConstant(Chunk *chunk, Value value, int line);
#endif //CLOX_CHUNK_H
