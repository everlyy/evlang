#ifndef _EVLANG_FORMAT_EVPGM_H
#define _EVLANG_FORMAT_EVPGM_H

#include <evlang/types.h>
#include <evlang/asm.h>
#include <stdio.h>

#define EVPGM_VERSION _ASMI_COUNT

static const u8 evpgm_signature[8] = { 'e', 'v', 'p', 'g', 'm', ':', '-', ')' };

typedef struct {
    u16 name_length;
    u8* name;
    u64 address;
} EVPGM_Label;

typedef struct {
    u8 signature[8];
    u16 version;

    u16 label_count;
    EVPGM_Label* labels;

    u64 code_length;
    u8* code;
} EVPGM;

void evpgm_free(EVPGM pgm);
EVPGM evpgm_read_from_file(FILE* file);
void evpgm_write_to_file(FILE* file, u16 label_count, EVPGM_Label labels[label_count], u64 code_length, u8 code[code_length]);

#endif // _EVLANG_FORMAT_EVPGM_H
