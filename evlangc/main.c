#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "emulator.h"
#include "evlang.h"
#include "evlang_assert.h"
#include "list.h"
#include "log.h"
#include "program.h"
#include "string_view.h"
#include "lexer.h"

StringView read_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    LOG_ERROR_IF(!file, "Couldn't open file %s: %s", filename, strerror(errno));

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    LOG_ERROR_IF(length < 0, "Couldn't determine size of file %s: %s", filename, strerror(errno));
    fseek(file, 0, SEEK_SET);

    char* data = malloc(length);
    size_t n = fread(data, 1, length, file);
    LOG_ERROR_IF(n != (size_t)length, "Couldn't read from file %s", filename);

    fclose(file);
    return sv_from_buffer(data, length);
}

void print_token_debug_header() {
    printf("\e[1m%-20s %-20s %-18s %s\e[0m\n", "TEXT", "TYPE", "VALUE", "LOCATION");
}

void print_token_debug(const Token* token) {
    const int ttml = 20;
    int ttl = 0;
    char token_text[ttml];

    if(token->text.length > (u64)ttml) {
        memcpy(token_text, token->text.data, ttml - 3);
        memcpy(token_text + ttml - 3, "...", 3);
        ttl = ttml;
    } else {
        memcpy(token_text, token->text.data, token->text.length);
        ttl = token->text.length;
    }

    printf("%-*.*s ", ttml, ttl, token_text);
    printf("%-20s ", lexer_token_type_cstr(token->type));
    printf("0x%016llX ", token->interpret.as_u64);
    printf(LOCATION_FMT" ", LOCATION_ARGS(&token->location));
    printf("\n");
}

static void dump(EmulatorState* state) {
    EmulatorValue value = emulator_pop(state);
    printf("%lld\n", value.as_u64);
    emulator_push(state, value);
}

int main(int argc, char** argv) {
    printf("evlangc - compiled on %s\n", __TIMESTAMP__);

    if(argc < 2) {
        fprintf(stderr, "Not enough arguments\n");
        fprintf(stderr, "Usage: %s program.evlang\n", argv[0]);
        return 1;
    }

    cstr filename = argv[1];
    StringView contents = read_file(filename);

    Lexer l = lexer_create(filename, contents);
    TokenList tl = lexer_token_list(&l);
    Program program = program_from_token_list(&tl);

    print_token_debug_header();
    LIST_ITERATE(&tl, Token, t, {
        print_token_debug(t);
    })

    printf("\n");
    program_instruction_debug_header();
    LIST_ITERATE(&program.instructions, Instruction, insn, {
        program_instruction_debug(__index, insn);
    })

    printf("\n\e[1mEMULATION OUTPUT\e[0m\n");
    EmulatorState state = emulator_state_create();
    state.builtins[BUILTIN_DUMP] = dump;

    while(!state.halt) {
        emulator_emulate_instruction(&state, &program.instructions.items[state.program_counter], true);
        sleep(1);
    }

    free((void*)contents.data);
    return 0;
}
