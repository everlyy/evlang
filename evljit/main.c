#include "jit/jit.h"
#include <stdio.h>
#include <evlang/assert.h>
#include <evlang/format/evpgm.h>

int main(void) {
    FILE* file = fopen("a.evpgm", "rb");
    ASSERT(file != NULL);

    EVPGM program = evpgm_read_from_file(file);
    JITProgram jit_program = jit_compile(&program);
    jit_run(&jit_program);
    return 0;
}
