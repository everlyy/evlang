#ifndef _EVLANG_ASM_H
#define _EVLANG_ASM_H

#include <evlang/types.h>
#include <evlang/list.h>
#include <stdbool.h>

#define ASM_INSTRUCTIONS_ENUM             \
    X(ASMI_NOP,          "nop",    false) \
    X(ASMI_HALT,         "halt",   false) \
    X(ASMI_PUSH_INT,     "push",   true)  \
    X(ASMI_ADD,          "add",    false) \
    X(ASMI_CALL_BUILTIN, "callbi", true)  \
    X(ASMI_JUMP,         "jump",   false) \
    X(ASMI_DROP,         "drop",   false) \
    X(ASMI_DUPLICATE,    "dup",    false) \
    X(ASMI_SWAP,         "swap",   false) \
    X(ASMI_ROTATE,       "rot",    false) \
    X(ASMI_PUSH_LABEL,   "pushl",  true)  \

#define ASM_BUILTINS_ENUM       \
    X(ASMBI_UNKNOWN, "unknown") \
    X(ASMBI_DUMP,    "dump")    \

typedef enum {
    #define X(type, ...) type,
        ASM_INSTRUCTIONS_ENUM
    #undef X

    _ASMI_COUNT
} ASM_InstructionType;

typedef enum {
    #define X(type, ...) type,
        ASM_BUILTINS_ENUM
    #undef X

    _ASMBI_COUNT
} ASM_Builtin;

typedef union {
    u64 as_u64;
    s64 as_s64;
    ASM_Builtin as_builtin;
} ASM_Operand;

typedef struct {
    ASM_InstructionType type;
    ASM_Operand operand;
} ASM_Instruction;

LIST_DEFINE_ALL(ASM_Instructions, ASM_Instruction, asm_instructions);

cstr asm_instruction_mneumonic(ASM_InstructionType type);
bool asm_instruction_has_operand(ASM_InstructionType type);
cstr asm_builtin_name(ASM_Builtin builtin);

#endif // _EVLANG_ASM_H
