#include "emulator.h"

#include "evlang_assert.h"
#include "list.h"
#include "program.h"

#define push emulator_push
#define pop emulator_pop

static EmulatorValue ev_u64(u64 x) {
    return (EmulatorValue) {
        .as_u64 = x
    };
}

static void builtin_unknown(EmulatorState* state) {
    state->halt = true;
}

static void builtin_dump(EmulatorState* state) {
    printf("\n\e[1m%-4s %-18s %s\e[0m", "IDX", "VALUE (HEX)", "VALUE (DEC)\n");
    for(u64 i = state->stack_index; i > 0; i--) {
        EmulatorValue value = state->stack[i - 1];
        printf("%-4llu 0x%016llX %lld\n", i - 1, value.as_u64, value.as_s64);
    }
}

EmulatorState emulator_state_create(void) {
    EmulatorState state = {0};

    state.builtins[BUILTIN_UNKNOWN] = builtin_unknown;
    state.builtins[BUILTIN_DUMP] = builtin_dump;

    return state;
}

void emulator_emulate_instruction(EmulatorState* state, const Instruction* instruction) {
    switch(instruction->type) {
    case INSN_NOP:
        break;

    case INSN_HALT: {
        state->halt = true;
        break;
    }

    case INSN_PUSH_INT: {
        push(state, ev_u64(instruction->operand.as_u64));
        break;
    }

    case INSN_ADD: {
        u64 a = pop(state).as_u64;
        u64 b = pop(state).as_u64;
        push(state, ev_u64(a + b));
        break;
    }

    case INSN_CALL_BUILTIN: {
        state->builtins[instruction->operand.as_builtin](state);
        break;
    }

    case INSN_JUMP: {
        state->program_counter = pop(state).as_u64;
        goto after_inc_pc;
    }

    case INSN_DROP: {
        pop(state);
        break;
    }

    case INSN_DUPLICATE: {
        EmulatorValue value = pop(state);
        push(state, value);
        push(state, value);
        break;
    }

    case INSN_SWAP: {
        EmulatorValue a = pop(state);
        EmulatorValue b = pop(state);
        push(state, a);
        push(state, b);
        break;
    }

    case INSN_ROTATE: {
        EmulatorValue a = pop(state);
        EmulatorValue b = pop(state);
        EmulatorValue c = pop(state);
        push(state, a);
        push(state, c);
        push(state, b);
        break;
    }

    default:
        ASSERT(false);
    }

    state->program_counter++;
    after_inc_pc:
        return;
}

void emulator_emulate_program(EmulatorState* state, const Program* program) {
    while(!state->halt)
        emulator_emulate_instruction(state, &program->instructions.items[state->program_counter]);
}

void emulator_push(EmulatorState* state, EmulatorValue value) {
    ASSERT(state->stack_index < EMULATOR_STACK_SIZE);
    state->stack[state->stack_index++] = value;
}

EmulatorValue emulator_pop(EmulatorState* state) {
    ASSERT(state->stack_index > 0);
    return state->stack[--state->stack_index];
}
