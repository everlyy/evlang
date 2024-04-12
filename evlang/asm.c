#include <evlang/list.h>
#include <evlang/asm.h>

cstr asm_instruction_mneumonic(ASM_InstructionType type) {
    switch(type) {
        #define X(type, mneumonic, _) case type: return mneumonic;
            ASM_INSTRUCTIONS_ENUM
        #undef X
    default:
        return NULL;
    }
}

bool asm_instruction_has_operand(ASM_InstructionType type) {
    switch(type) {
        #define X(type, _, has_operand) case type: return has_operand;
            ASM_INSTRUCTIONS_ENUM
        #undef X
    default:
        return false;
    }
}

cstr asm_builtin_name(ASM_Builtin builtin) {
    switch(builtin) {
        #define X(builtin, name) case builtin: return name;
            ASM_BUILTINS_ENUM
        #undef X
    default:
        return NULL;
    }
}

LIST_FUNCTIONS_IMPLEMENT(ASM_Instructions, ASM_Instruction, asm_instructions)
