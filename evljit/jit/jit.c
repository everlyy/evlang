#define JIT_COMPILER
#include "jit.h"

#include <stdio.h>
#include <evlang/assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

JITProgram jit_compile(const EVPGM* pgm) {
    JITProgram jit = {
        .code = malloc(JIT_INITIAL_CAPACITY),
        .capacity = JIT_INITIAL_CAPACITY,
        .count = 0
    };

    ASSERT(jit.code != NULL);

    // Calls the architecture-specific JIT compiler
    jit_compiler(&jit, pgm);

    return jit;
}

void jit_run(const JITProgram* jit) {
    void* code = mmap(NULL, jit->count, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    ASSERT(code != NULL);
    memcpy(code, jit->code, jit->count);
    mprotect(code, jit->count, PROT_READ | PROT_EXEC);
    ((void(*)(void))code)();
}

void jit_code_append(JITProgram* jit, u8* bytes, u64 length) {
    ASSERT(jit->code != NULL);

    if(jit->count + length >= jit->capacity) {
        jit->capacity *= 2;
        jit->code = realloc(jit->code, jit->capacity);
    }

    memcpy(&jit->code[jit->count], bytes, length);
    jit->count += length;
}

void jit_code_append_u8(JITProgram* jit, u8 x) {
    jit_code_append(jit, (u8*)&x, sizeof(x));
}

void jit_code_append_u64(JITProgram* jit, u64 x) {
    jit_code_append(jit, (u8*)&x, sizeof(x));
}
