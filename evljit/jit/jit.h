#ifndef _JIT_H
#define _JIT_H

#include <evlang/list.h>
#include <evlang/format/evpgm.h>

#define JIT_INITIAL_CAPACITY 64

typedef struct {
    u8* code;
    u64 count;
    u64 capacity;
} JITProgram;

#if defined(__x86_64__) || defined(_M_X64)
#define jit_compiler jit_x86_64_compile
#else
#define jit_compiler jit_unknown_compile
#endif

JITProgram jit_compile(const EVPGM* pgm);
void jit_compiler(JITProgram* jit, const EVPGM* pgm);
void jit_run(const JITProgram* jit);

#endif // _JIT_H

#ifdef JIT_COMPILER
#ifndef _JIT_COMPILER
#define _JIT_COMPILER

void jit_code_append(JITProgram* jit, u8* bytes, u64 length);
void jit_code_append_u8(JITProgram* jit, u8 x);
void jit_code_append_u64(JITProgram* jit, u64 x);

#define jcappend(jit, ...) jit_code_append((jit), (u8[]) { __VA_ARGS__ }, sizeof((u8[]) { __VA_ARGS__ }))

#endif // _JIT_COMPILER
#endif // JIT_COMPILER
