# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is **clox**, a bytecode virtual machine interpreter for the Lox programming language, written in C. The project follows the architecture from "Crafting Interpreters" by Robert Nystrom.

## Build and Run

```bash
# Build the project
cmake -B build && cmake --build build

# Run the interpreter
./build/clox
```

## Architecture

### Core Components

- **Chunk**: The bytecode container that stores instructions and constant values
  - `Chunk` struct holds bytecode (`code`), line numbers (`lines`), and constant pool (`constants`)
  - Dynamic arrays that grow as needed using the memory management macros
  - Each chunk has its own constant pool (`ValueArray`)

- **Value System**: Currently represents values as doubles
  - `Value` is typedef'd to `double`
  - `ValueArray` is a dynamic array of values used for the constant pool
  - Will likely expand to support additional types (strings, objects) later

- **Memory Management**: Custom allocator with growth strategy
  - `reallocate()` is the single point of allocation/deallocation
  - `GROW_CAPACITY` macro: starts at 8, then doubles
  - `GROW_ARRAY` and `FREE_ARRAY` macros wrap `reallocate()` for type safety
  - All dynamic arrays follow this pattern

- **Opcodes**: Bytecode instruction set (currently minimal)
  - `OP_CONSTANT`: Load a constant value (followed by constant pool index)
  - `OP_RETURN`: Return from current function
  - Each opcode has specific encoding (some take operands, some don't)

- **Disassembler**: Debug output for bytecode inspection
  - `disassembleChunk()`: prints entire chunk
  - `disassembleInstruction()`: prints single instruction with line number
  - Shows line numbers with `|` for consecutive instructions on same line
  - Critical for debugging the VM

### Key Patterns

- **Initialization pattern**: All data structures have `init*()` functions that zero out fields
- **Cleanup pattern**: All data structures have `free*()` functions that deallocate and reinitialize
- **Write pattern**: Dynamic arrays use `write*()` functions that handle capacity growth automatically
- **Instruction encoding**: Multi-byte instructions store operands in subsequent bytes (e.g., `OP_CONSTANT` followed by constant index)

## Common Issues

- `initChunk()` must be called before any chunk operations, otherwise you'll get null pointer dereferences
- The constant pool index in `addConstant()` must fit in a `uint8_t` (max 256 constants per chunk)
- Line numbers are stored parallel to bytecode - they must stay synchronized