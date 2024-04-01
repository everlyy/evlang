#include "program.h"
#include "evlang.h"
#include "evlang_assert.h"
#include "lexer.h"
#include "list.h"
#include "log.h"
#include "string_view.h"

static const cstr builtin_names[_BUILTIN_COUNT] = {
    [BUILTIN_DUMP]  = "dump"
};

static Builtin builtin_from_name(StringView name) {
    ASSERT(name.length >= 2);

    // Skip leading '#'
    name.data++;
    name.length--;

    for(u64 i = 0; i < ARRAY_LEN(builtin_names); i++) {
        if(builtin_names[i] && sv_eq_cstr(&name, builtin_names[i]))
            return i;
    }

    return BUILTIN_UNKNOWN;
}

static void parse_token_instruction(Instruction* instruction, const Token* token) {
    ASSERT(token->type == TOKEN_INSTRUCTION);

    switch(token->interpret.as_instruction) {
    case TI_ADD: {
        instruction->type = INSN_ADD;
        break;
    }

    case TI_DUPLICATE: {
        instruction->type = INSN_DUPLICATE;
        break;
    }

    case TI_SWAP: {
        instruction->type = INSN_SWAP;
        break;
    }

    case TI_JUMP: {
        instruction->type = INSN_JUMP;
        break;
    }

    case TI_ROTATE: {
        instruction->type = INSN_ROTATE;
        break;
    }

    default:
        ASSERT(false);
    }
}

Program program_from_token_list(const TokenList* tl) {
    Program program = {0};

    LIST_ITERATE(tl, Token, token, {
        Instruction instruction = {0};

        switch(token->type) {
        case TOKEN_COMMENT:
        case TOKEN_EOF:
            goto dont_add;

        case TOKEN_INTEGER: {
            instruction.type = INSN_PUSH_INT;
            instruction.operand.as_u64 = token->interpret.as_u64;
            break;
        }

        case TOKEN_CALL_BUILTIN: {
            instruction.type = INSN_CALL_BUILTIN;
            instruction.operand.as_builtin = builtin_from_name(token->text);
            break;
        }

        case TOKEN_INSTRUCTION: {
            parse_token_instruction(&instruction, token);
            break;
        }

        default:
            LOG_ERROR_LOC(
                &token->location,
                "Unknown token '%.*s' (%s)",
                (int)token->text.length, token->text.data,
                lexer_token_type_cstr(token->type)
            );
        }

        il_append(&program.instructions, &instruction);

        dont_add:
            continue;
    })

    Instruction halt_instruction = {
        .type = INSN_HALT,
        .operand.as_u64 = 0
    };
    il_append(&program.instructions, &halt_instruction);
    return program;
}

cstr instruction_type_mneumonic(InstructionType type) {
    switch(type) {
    #define X(insn_type, mneumonic, has_operand) case insn_type: return mneumonic;
        ENUM_INSTRUCTION_TYPES
    #undef X
    }

    return NULL;
}

bool instruction_type_has_operand(InstructionType type) {
    switch(type) {
    #define X(insn_type, mneumonic, has_operand) case insn_type: return has_operand;
        ENUM_INSTRUCTION_TYPES
    #undef X
    }

    return false;
}

cstr builtin_to_name(Builtin b) {
    return builtin_names[b];
}

void program_instruction_debug_header(void) {
    printf("\e[1m%-4s    %-4s %-18s    %s\e[0m\n", "ADDR", "CODE", "OPERAND", "DISASSEMBLY");
}

void program_instruction_debug(u64 addr, const Instruction* insn) {
    printf("%04llu    ", addr);
    printf("%04X 0x%016llX    ", insn->type, insn->operand.as_u64);
    printf("%-8s", instruction_type_mneumonic(insn->type));
    if(instruction_type_has_operand(insn->type)) {
        printf(" %lld", insn->operand.as_s64);
        if(insn->type == INSN_CALL_BUILTIN)
            printf(" [= %s]", builtin_to_name(insn->operand.as_builtin));
    }
    printf("\n");
}

void il_append(InstructionList* il, const Instruction* i) {
    LIST_APPEND(il, i);
}

void il_extend(InstructionList* il, const InstructionList* other_il) {
    LIST_EXTEND(il, other_il);
}

void il_free(InstructionList* il) {
    LIST_FREE(il);
}
