#ifndef _PROGRAM_H
#define _PROGRAM_H

#include <evlang/evlang.h>
#include <evlang/format/evpgm.h>
#include <stdio.h>
#include <evlang/asm.h>
#include "lexer.h"

EVPGM program_from_token_list(const TokenList* tl);

void program_disassemble(const EVPGM* program);
void program_write_evpgm(const EVPGM* program, FILE* file);

#endif // _PROGRAM_H
