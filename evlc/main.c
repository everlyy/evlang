#include <evlang/evlang.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "log.h"
#include "lexer.h"
#include "program.h"

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

int main(int argc, char** argv) {
    if(argc < 2)
        LOG_ERROR("Not enough arguments. Usage: %s <program.evlang>", argv[0]);

    cstr filename = argv[1];
    StringView contents = read_file(filename);

    Lexer l = lexer_create(filename, contents);
    TokenList tl = lexer_token_list(&l);
    EVPGM program = program_from_token_list(&tl);

    program_disassemble(&program);

    FILE* file = fopen("a.evpgm", "wb");
    ASSERT(file != NULL);
    program_write_evpgm(&program, file);

    free((void*)contents.data);
    return 0;
}
