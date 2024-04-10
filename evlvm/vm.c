#include "vm.h"
#include "evlang/format/evpgm.h"
#include <evlang/asm.h>
#include <stdio.h>

static void builtin_unknown(VM*) {
    ASSERT(false);
}

static void builtin_dump(VM* vm) {
    printf("\033[1mSTACK DUMP\033[0m\n");
    for(u64 i = vm->stack_index; i > 0; i--) {
        ASM_Operand item = vm->stack[i - 1];
        printf("  %03llu\t0x%016llX (%lld)\n", i - 1, item.as_u64, item.as_s64);
    }
}

static void setup_builtins(VM_Builtin builtins[_ASMBI_COUNT]) {
    builtins[ASMBI_UNKNOWN] = builtin_unknown;
    builtins[ASMBI_DUMP]    = builtin_dump;
}

static ASM_Operand vm_u64(u64 x) {
    return (ASM_Operand) {
        .as_u64 = x
    };
}

static void push(VM* vm, ASM_Operand value) {
    ASSERT(vm->stack_index < VM_STACK_SIZE);
    vm->stack[vm->stack_index] = value;
    vm->stack_index++;
}

static ASM_Operand pop(VM* vm) {
    ASSERT(vm->stack_index > 0);
    vm->stack_index--;
    return vm->stack[vm->stack_index];
}

VM vm_create(EVPGM program) {
    VM vm = { 0 };
    vm.program = program;
    setup_builtins(vm.builtins);
    return vm;
}

void vm_run(VM* vm) {
    while(!vm->halt) {
        const ASM_Instruction* i = &((ASM_Instruction*)vm->program.code)[vm->program_counter];
        vm->program_counter++;

        switch(i->type) {
        case ASMI_HALT: {
            vm->halt = true;
            break;
        }

        case ASMI_PUSH_INT: {
            push(vm, i->operand);
            break;
        }

        case ASMI_CALL_BUILTIN: {
            VM_Builtin builtin = vm->builtins[i->operand.as_builtin];
            ASSERT(builtin != NULL);
            builtin(vm);
            break;
        }

        case ASMI_DUPLICATE: {
            ASM_Operand a = pop(vm);
            push(vm, a);
            push(vm, a);
            break;
        }

        case ASMI_ROTATE: {
            ASM_Operand a = pop(vm);
            ASM_Operand b = pop(vm);
            ASM_Operand c = pop(vm);
            push(vm, a);
            push(vm, b);
            push(vm, c);
            break;
        }

        case ASMI_ADD: {
            u64 a = pop(vm).as_u64;
            u64 b = pop(vm).as_u64;
            push(vm, vm_u64(a + b));
            break;
        }

        case ASMI_PUSH_LABEL: {
            push(vm, i->operand);
            break;
        }

        case ASMI_JUMP: {
            vm->program_counter = i->operand.as_u64;
            break;
        }

        default:
            ASSERT(false);
        }
    }
}
