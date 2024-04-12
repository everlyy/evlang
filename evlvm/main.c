#include <evlang/assert.h>
#include <evlang/types.h>
#include <evlang/format/evpgm.h>
#include <stdio.h>
#include "vm.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        fprintf(stderr, "Not enough arguments.\n");
        fprintf(stderr, "Usage: %s <file.evpgm>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "rb");
    ASSERT(file != NULL);

    EVPGM program = evpgm_read_from_file(file);

    VM vm = vm_create(program);
    vm_run(&vm);

    return 0;
}
