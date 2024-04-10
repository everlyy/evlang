#ifndef _VM_H
#define _VM_H

#include <evlang/evlang.h>
#include <evlang/asm.h>
#include <evlang/format/evpgm.h>

#define VM_STACK_SIZE 8

struct VM;
typedef void (*VM_Builtin)(struct VM*);

typedef struct VM {
    VM_Builtin builtins[_ASMBI_COUNT];
    EVPGM program;

    bool halt;
    ASM_Operand stack[VM_STACK_SIZE];
    u64 stack_index;
    u64 program_counter;
} VM;

VM vm_create(EVPGM program);
void vm_run(VM* vm);

#endif // _VM_H
