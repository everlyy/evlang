#ifndef _EVLANG_FORMAT_EVPGM_H
#define _EVLANG_FORMAT_EVPGM_H

#include "evlang/list.h"
#include <evlang/string_view.h>
#include <evlang/types.h>
#include <evlang/asm.h>
#include <stdio.h>

#define EVPGM_VERSION _ASMI_COUNT

static const u8 evpgm_signature[8] = { 'e', 'v', 'p', 'g', 'm', ':', '-', ')' };

typedef struct {
    StringView name;
    u64 address;
} EVPGM_Label;

LIST_DEFINE_ALL(EVPGM_Labels, EVPGM_Label, evpgm_labels);

typedef struct {
    u8 signature[8];
    u64 version;
    EVPGM_Labels labels;
    ASM_Instructions instructions;
} EVPGM;

void evpgm_free(EVPGM pgm);
EVPGM evpgm_read_from_file(FILE* file);
void evpgm_write_to_file(FILE* file, const EVPGM* pgm);

#endif // _EVLANG_FORMAT_EVPGM_H
