//
// Created by rmichau on 10/4/25.
//

#include "vm.h"

#include <stdio.h>
#include "debug.h"
#include "common.h"
#include "compiler.h"

#define READ_BYTE() (*vm.ip++)

VM vm;

static int constantLongInstruction() {
    uint8_t addr1 = READ_BYTE();
    uint8_t addr2 = READ_BYTE();
    uint8_t addr3 = READ_BYTE();
    return (addr1 << 16) | (addr2 << 8) | addr3;
}

static void resetStack() {
    vm.stackTop = vm.stack;
}

static InterpretResult run() {
#define BINARY_OP(op) \
do { \
double b = pop(); \
double a = pop(); \
push(a op b); \
} while (false)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *slot = vm.stack; slot < vm.stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(vm.chunk,
                               (int) (vm.ip - vm.chunk->code));
#endif
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_NEGATE: push(-pop());
                break;
            case OP_ADD: BINARY_OP(+);
                break;
            case OP_SUBTRACT: BINARY_OP(-);
                break;
            case OP_MULTIPLY: BINARY_OP(*);
                break;
            case OP_DIVIDE: BINARY_OP(/);
                break;
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_CONSTANT_LONG: {
                Value constant = constantLongInstruction();
                push(constant);
                break;
            }
            case OP_RETURN: {
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef BINARY_OP
#undef READ_CONSTANT
}


void initVM() {
    resetStack();
}

void freeVM() {
}

InterpretResult interpret(const char* source) {
    // compile(source);
    // return INTERPRET_OK;
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();

    freeChunk(&chunk);
    return result;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}
