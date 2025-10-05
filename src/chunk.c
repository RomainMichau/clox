//
// Created by rmichau on 9/28/25.
//

#include <stdlib.h>

#include "chunk.h"

#include <stdio.h>

#include "memory.h"

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(uint8_t, chunk->lines, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}


void writeConstant(Chunk *chunk, Value value, int line) {
    int addr = addConstant(chunk, value);
    if (addr <= UINT8_MAX) {
        writeChunk(chunk, OP_CONSTANT, line);
        writeChunk(chunk, addr, line);
    } else {
        writeChunk(chunk, OP_CONSTANT_LONG, line);
        uint8_t addr1 = (addr >> 16) & 0xFF; // highest 8 bits
        uint8_t addr2 = (addr >> 8) & 0xFF; // middle 8 bits
        uint8_t addr3 = addr & 0xFF; // lowest 8 bits
        writeChunk(chunk, addr1, line);
        writeChunk(chunk, addr2, line);
        writeChunk(chunk, addr3, line);
    }
}


int addConstant(Chunk *chunk, Value value) {
    writeValueArray(&chunk->constants, value);
    return chunk->constants.count - 1;
}
