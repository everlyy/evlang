#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <evlang/evlang.h>
#include <evlang/format/evpgm.h>
#include <stdio.h>
#include "lexer.h"

typedef LIST_DEFINE_STRUCT(ASM_Instruction) InstructionList;

typedef struct {
    StringView name;
    u64 address;
} Label;

typedef LIST_DEFINE_STRUCT(Label) LabelList;

typedef struct {
    InstructionList instructions;
    LabelList labels;
} Program;

void il_append(InstructionList* il, const ASM_Instruction* i);
void il_extend(InstructionList* il, const InstructionList* other_il);
void il_free(InstructionList* il);

void ll_append(LabelList* ll, const Label* l);
void ll_extend(LabelList* ll, const LabelList* other_ll);
void ll_free(LabelList* ll);

Program program_from_token_list(const TokenList* tl);

void program_disassemble(const Program* program);
void program_write_evpgm(const Program* program, FILE* file);

#endif // _PROGRAM_H
