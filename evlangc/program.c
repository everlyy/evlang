#include "program.h"
#include <evlang/asm.h>
#include <evlang/format/evpgm.h>
#include <evlang/list.h>
#include <evlang/types.h>
#include "lexer.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>

static ASM_Builtin builtin_from_name(const StringView* symbol) {
    for(ASM_Builtin i = ASMBI_UNKNOWN; i < _ASMBI_COUNT; i++) {
        if(sv_eq_cstr(symbol, asm_builtin_name(i)))
            return i;
    }

    return ASMBI_UNKNOWN;
}

static void parse_token_instruction(ASM_Instruction* instruction, const Token* token) {
    ASSERT(token->type == TOKEN_INSTRUCTION);

    switch(token->interpret.as_instruction) {
    case TI_ADD: {
        instruction->type = ASMI_ADD;
        break;
    }

    case TI_DUPLICATE: {
        instruction->type = ASMI_DUPLICATE;
        break;
    }

    case TI_SWAP: {
        instruction->type = ASMI_SWAP;
        break;
    }

    case TI_JUMP: {
        instruction->type = ASMI_JUMP;
        break;
    }

    case TI_ROTATE: {
        instruction->type = ASMI_ROTATE;
        break;
    }

    default:
        ASSERT(false);
    }
}

static bool get_address_from_label_name(const LabelList* ll, const StringView* name, u64* address) {
    LIST_ITERATE(ll, Label, l, {
        if(sv_eq(&l->name, name)) {
            *address = l->address;
            return true;
        }
    })

    return false;
}

static bool get_label_name_from_address(const LabelList* ll, u64 address, StringView** name) {
    LIST_ITERATE(ll, Label, l, {
        if(l->address == address) {
            *name = &l->name;
            return true;
        }
    })

    return false;
}

Program program_from_token_list(const TokenList* tl) {
    Program program = {0};

    LIST_ITERATE(tl, Token, token, {
        ASM_Instruction instruction = {0};

        switch(token->type) {
        case TOKEN_COMMENT:
        case TOKEN_EOF:
            goto dont_add;

        case TOKEN_INTEGER: {
            instruction.type = ASMI_PUSH_INT;
            instruction.operand.as_u64 = token->interpret.as_u64;
            break;
        }

        case TOKEN_CALL_BUILTIN: {
            instruction.type = ASMI_CALL_BUILTIN;
            instruction.operand.as_builtin = builtin_from_name(&token->interpret.as_symbol);
            break;
        }

        case TOKEN_INSTRUCTION: {
            parse_token_instruction(&instruction, token);
            break;
        }

        case TOKEN_LABEL_DEF: {
            Label label = {0};
            label.name = token->interpret.as_symbol;
            label.address = program.instructions.count;
            ll_append(&program.labels, &label);
            goto dont_add;
        }

        case TOKEN_LABEL_REF: {
            u64 address;
            bool found = get_address_from_label_name(&program.labels, &token->interpret.as_symbol, &address);

            LOG_ERROR_IF(
                !found,
                "Couldn't find label with name '%.*s'",
                (int)token->interpret.as_symbol.length, token->interpret.as_symbol.data
            );

            instruction.type = ASMI_PUSH_LABEL;
            instruction.operand.as_u64 = address;
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

    ASM_Instruction halt_instruction = {
        .type = ASMI_HALT,
        .operand.as_u64 = 0
    };
    il_append(&program.instructions, &halt_instruction);
    return program;
}

void program_disassemble(const Program* program) {
    printf("\e[1m%-4s    %-4s %-18s    %s\e[0m\n", "ADDR", "CODE", "OPERAND", "DISASSEMBLY");

    for(u64 i = 0; i < program->instructions.count; i++) {
        StringView* name;
        bool found_label = get_label_name_from_address(&program->labels, i, &name);
        if(found_label) {
            printf("\n<label> %.*s:\n", (int)name->length, name->data);
        }

        const ASM_Instruction* insn = &program->instructions.items[i];

        printf("%04llu    ", i);
        printf("%04X 0x%016llX    ", insn->type, insn->operand.as_u64);
        printf("%-8s", asm_instruction_mneumonic(insn->type));
        if(asm_instruction_has_operand(insn->type)) {
            printf(" %lld", insn->operand.as_s64);

            if(insn->type == ASMI_CALL_BUILTIN)
                printf(" [= %s]", asm_builtin_name(insn->operand.as_builtin));

            if(insn->type == ASMI_PUSH_LABEL) {
                StringView* name;
                bool found = get_label_name_from_address(&program->labels, insn->operand.as_u64, &name);
                if(found)
                    printf(" [= %.*s]", (int)name->length, name->data);
                else
                    printf(" [= ???]");
            }
        }
        printf("\n");
    }
}

void program_write_evpgm(const Program* program, FILE* file) {
    u16 label_count = program->labels.count;
    EVPGM_Label* labels = calloc(label_count, sizeof(*labels));

    LIST_ITERATE(&program->labels, Label, l, ({
        labels[__index] = (EVPGM_Label) {
            .name = (u8*)l->name.data,
            .name_length = l->name.length,
            .address = l->address
        };
    }))

    u64 fixme_code_capacity = 8192;
    u64 code_length = 0;
    u8* code = calloc(fixme_code_capacity, sizeof(*code));

    LIST_ITERATE(&program->instructions, ASM_Instruction, insn, ({
        memcpy(&code[code_length], insn, sizeof(ASM_Instruction));
        code_length += sizeof(ASM_Instruction);
    }))

    evpgm_write_to_file(file, label_count, labels, code_length, code);
}

void il_append(InstructionList* il, const ASM_Instruction* i) {
    LIST_APPEND(il, i);
}

void il_extend(InstructionList* il, const InstructionList* other_il) {
    LIST_EXTEND(il, other_il);
}

void il_free(InstructionList* il) {
    LIST_FREE(il);
}

void ll_append(LabelList* ll, const Label* l) {
    LIST_APPEND(ll, l);
}

void ll_extend(LabelList* ll, const LabelList* other_ll) {
    LIST_EXTEND(ll, other_ll);
}

void ll_free(LabelList* ll) {
    LIST_FREE(ll);
}
