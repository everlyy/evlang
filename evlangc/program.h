#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "lexer.h"
#include "list.h"

typedef enum {
    BUILTIN_UNKNOWN = 0,
    BUILTIN_DUMP,

    _BUILTIN_COUNT
} Builtin;

#define ENUM_INSTRUCTION_TYPES            \
    X(INSN_NOP,          "nop",    false) \
    X(INSN_HALT,         "halt",   false) \
    X(INSN_PUSH_INT,     "push",   true)  \
    X(INSN_ADD,          "add",    false) \
    X(INSN_CALL_BUILTIN, "callbi", true)  \
    X(INSN_JUMP,         "jump",   false) \
    X(INSN_DROP,         "drop",   false) \
    X(INSN_DUPLICATE,    "dup",    false) \
    X(INSN_SWAP,         "swap",   false) \
    X(INSN_ROTATE,       "rot",    false) \

typedef enum {
    #define X(insn_type, mneumonic, has_operand) insn_type,
        ENUM_INSTRUCTION_TYPES
    #undef X
} InstructionType;

typedef struct {
    InstructionType type;

    union _Operand {
        u64 as_u64;
        s64 as_s64;
        Builtin as_builtin;
    } operand;
} Instruction;

typedef LIST_DEFINE_STRUCT(Instruction) InstructionList;

typedef struct {
    InstructionList instructions;
} Program;

void il_append(InstructionList* il, const Instruction* i);
void il_extend(InstructionList* il, const InstructionList* other_il);
void il_free(InstructionList* il);

Program program_from_token_list(const TokenList* tl);

cstr instruction_type_mneumonic(InstructionType type);
bool instruction_type_has_operand(InstructionType type);

cstr builtin_to_name(Builtin b);

void program_instruction_debug_header(void);
void program_instruction_debug(u64 addr, const Instruction* insn);

#endif // _PROGRAM_H
