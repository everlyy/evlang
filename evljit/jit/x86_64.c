#include <stdio.h>
#define JIT_COMPILER
#include "jit.h"

#include <evlang/list.h>
#include <evlang/asm.h>
#include <evlang/assert.h>

void jit_x86_64_compile(JITProgram* jit, const EVPGM* pgm) {
    LIST_ITERATE(&pgm->instructions, ASM_Instruction, instruction, ({
        switch(instruction->type) {
        case ASMI_PUSH_INT: {
            jcappend(jit, 0x49, 0xBF);
            jit_code_append_u64(jit, instruction->operand.as_u64);
            break;
        }

        case ASMI_ADD: {
            jcappend(jit, 0x41, 0x5F);       // pop r15
            jcappend(jit, 0x41, 0x5E);       // pop r14
            jcappend(jit, 0x4D, 0x01, 0xFE); // add r14, r15
            jcappend(jit, 0x41, 0x56);       // push r14
            break;
        }

        case ASMI_HALT: {
            jcappend(jit, 0xB8, 0x3C, 0x00, 0x00, 0x00); // mov eax, 60
            jcappend(jit, 0x48, 0x31, 0xF6);             // xor rsi, rsi
            jcappend(jit, 0x0F, 0x05);                   // syscall
            break;
        }

        default: {
            fprintf(stderr, "Unimplemented instruction: %s (%u)\n", asm_instruction_mneumonic(instruction->type), instruction->type);
            ASSERT(false);
        }
        }
    }))
}
