#include <evlang/format/evpgm.h>
#include <evlang/evlang.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void read_buffer(FILE* file, void* buf, u64 size) {
    u32 nread = fread(buf, 1, size, file);
    ASSERT(nread == size);
}

static u16 read_u16(FILE* file) {
    u16 x;
    read_buffer(file, &x, sizeof(x));
    return x;
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

static void write_u16(FILE* file, u16 x) {
    write_buffer(file, &x, sizeof(x));
}

static void write_u64(FILE* file, u64 x) {
    write_buffer(file, &x, sizeof(x));
}

void evpgm_free(EVPGM pgm) {
    for(u16 i = 0; i < pgm.label_count; i++)
        free(pgm.labels[i].name);

    free(pgm.labels);
    free(pgm.code);
}

EVPGM evpgm_read_from_file(cstr filename) {
    FILE* file = fopen(filename, "rb");
    ASSERT(file != NULL);

    EVPGM pgm = { 0 };

    read_buffer(file, pgm.signature, sizeof(evpgm_signature));
    ASSERT(memcmp(pgm.signature, evpgm_signature, sizeof(evpgm_signature)) == 0);

    pgm.version = read_u16(file);
    ASSERT(pgm.version == EVPGM_VERSION);

    pgm.label_count = read_u16(file);
    pgm.labels = calloc(pgm.label_count, sizeof(*pgm.labels));

    for(u16 i = 0; i < pgm.label_count; i++) {
        EVPGM_Label* l = &pgm.labels[i];
        l->name_length = read_u16(file);
        l->name = malloc(l->name_length);
        l->address = read_u64(file);
    }

    pgm.code_length = read_u64(file);
    pgm.code = malloc(pgm.code_length);
    read_buffer(file, pgm.code, pgm.code_length);

    return pgm;
}

void evpgm_write_to_file(FILE* file, u16 label_count, EVPGM_Label labels[label_count], u64 code_length, u8 code[code_length]) {
    write_buffer(file, evpgm_signature, sizeof(evpgm_signature));
    write_u16(file, EVPGM_VERSION);

    write_u16(file, label_count);
    for(u16 i = 0; i < label_count; i++) {
        write_u16(file, labels[i].name_length);
        write_buffer(file, labels[i].name, labels[i].name_length);
        write_u64(file, labels[i].address);
    }

    write_u64(file, code_length);
    write_buffer(file, code, code_length);
}
