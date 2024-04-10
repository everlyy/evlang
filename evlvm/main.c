#include <evlang/assert.h>
#include <evlang/types.h>
#include <evlang/format/evpgm.h>
#include <stdio.h>
#include "vm.h"

int main(void) {
    cstr filename = "a.evpgm";
    FILE* file = fopen(filename, "rb");
    ASSERT(file != NULL);

    EVPGM program = evpgm_read_from_file(file);

    VM vm = vm_create(program);
    vm_run(&vm);

    return 0;
}
