#define JIT_COMPILER
#include "jit.h"

#include <evlang/assert.h>
#include <stdio.h>

void jit_unknown_compile(JITProgram*, const EVPGM*) {
    fprintf(stderr, "No JIT compiler for current platform.\n");
    ASSERT(false);
}
