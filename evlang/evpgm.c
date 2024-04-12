#include "evlang/asm.h"
#include "evlang/list.h"
#include "evlang/string_view.h"
#include <evlang/format/evpgm.h>
#include <evlang/evlang.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void read_buffer(FILE* file, void* buf, u64 size) {
    u32 nread = fread(buf, 1, size, file);
    ASSERT(nread == size);
}

static u64 read_u64(FILE* file) {
    u64 x;
    read_buffer(file, &x, sizeof(x));
    return x;
}

static void write_buffer(FILE* file, const void* buf, u64 size) {
    u32 nwritten = fwrite(buf, 1, size, file);
    ASSERT(nwritten == size);
}

static void write_u64(FILE* file, u64 x) {
    write_buffer(file, &x, sizeof(x));
}

void evpgm_free(EVPGM pgm) {
    for(u16 i = 0; i < pgm.labels.count; i++)
        free((void*)pgm.labels.items[i].name.data);

    LIST_FREE(&pgm.labels);
    LIST_FREE(&pgm.instructions);
}

EVPGM evpgm_read_from_file(FILE* file) {
    EVPGM pgm = { 0 };

    read_buffer(file, pgm.signature, sizeof(evpgm_signature));
    ASSERT(memcmp(pgm.signature, evpgm_signature, sizeof(evpgm_signature)) == 0);

    pgm.version = read_u64(file);
    ASSERT(pgm.version == EVPGM_VERSION);

    u64 label_count = read_u64(file);

    for(u64 i = 0; i < label_count; i++) {
        u64 length = read_u64(file);
        cstr buf = malloc(length);
        read_buffer(file, (void*)buf, length);

        StringView name = sv_from_buffer(buf, length);
        u64 address = read_u64(file);
        EVPGM_Label label = {
            .name = name,
            .address = address
        };
        LIST_APPEND(&pgm.labels, &label);
    }

    u64 instruction_count = read_u64(file);

    for(u64 i = 0; i < instruction_count; i++) {
        ASM_Instruction instruction;
        read_buffer(file, &instruction, sizeof(instruction));
        LIST_APPEND(&pgm.instructions, &instruction);
    }

    return pgm;
}

void evpgm_write_to_file(FILE* file, const EVPGM* pgm) {
    write_buffer(file, pgm->signature, sizeof(pgm->signature));
    write_u64(file, pgm->version);

    write_u64(file, pgm->labels.count);
    for(u64 i = 0; i < pgm->labels.count; i++) {
        const EVPGM_Label* label = &pgm->labels.items[i];
        const StringView* name = &label->name;

        write_u64(file, name->length);
        write_buffer(file, name->data, name->length);
        write_u64(file, label->address);
    }

    write_u64(file, pgm->instructions.count);
    for(u64 i = 0; i < pgm->instructions.count; i++) {
        const ASM_Instruction* instruction = &pgm->instructions.items[i];
        write_buffer(file, instruction, sizeof(*instruction));
    }
}

LIST_FUNCTIONS_IMPLEMENT(EVPGM_Labels, EVPGM_Label, evpgm_labels);
