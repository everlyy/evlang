#ifndef _EMULATOR_H
#define _EMULATOR_H

#include "evlang_types.h"
#include "program.h"

#define EMULATOR_STACK_SIZE 512

typedef union {
    u64 as_u64;
    s64 as_s64;
} EmulatorValue;

struct EmulatorState;
typedef void (*BuiltinFunction)(struct EmulatorState*);

typedef struct EmulatorState {
    EmulatorValue stack[EMULATOR_STACK_SIZE];
    u64 stack_index;
    u64 program_counter;
    bool halt;
    BuiltinFunction builtins[_BUILTIN_COUNT];
} EmulatorState;

EmulatorState emulator_state_create(void);
void emulator_emulate_instruction(EmulatorState* state, const Instruction* instruction, bool show_disassembly);
void emulator_emulate_program(EmulatorState* state, const Program* program, bool show_disassembly);

void emulator_push(EmulatorState* state, EmulatorValue value);
EmulatorValue emulator_pop(EmulatorState* state);

#endif // _EMULATOR_H
