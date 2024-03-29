#include "lexer.h"
#include "evlang.h"
#include "evlang_assert.h"
#include "string_view.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int peek(const Lexer* l) {
    if(lexer_is_eof(l))
        return -1;

    return l->content.data[l->cursor];
}

static int consume(Lexer* l) {
    if(lexer_is_eof(l))
        return -1;

    char c = l->content.data[l->cursor];
    l->cursor++;

    if(c == '\n') {
        l->line++;
        l->line_start = l->cursor;
    }

    return c;
}

static void skipn(Lexer* l, u64 n) {
    for(u64 i = 0; i < n && consume(l) >= 0; i++);
}

static void skip_whitespace(Lexer* l) {
    while(isspace(peek(l)))
        consume(l);
}

static bool is_number(const Lexer* l) {
    char c = peek(l);
    return c >= '0' && c <= '9';
}

static bool starts_with(const Lexer* l, cstr s) {
    u64 len = strlen(s);

    if(len == 0)
        return true;

    if(l->cursor + len >= l->content.length)
        return false;

    return memcmp(&l->content.data[l->cursor], s, len) == 0;
}

static bool is_symbol_char(const Lexer* l) {
    char c = peek(l);
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || is_number(l);
}

static bool symbol_type(Lexer* l, Token* token, char start_char, TokenType type) {
    if(peek(l) == start_char) {
        consume(l);
        token->text.length = 1;
        token->type = type;

        while(is_symbol_char(l)) {
            token->text.length++;
            consume(l);
        }

        return true;
    }

    return false;
}

// static bool literal_token(Lexer* l, Token* token, char c, TokenType type) {
//     if(peek(l) == c) {
//         consume(l);
//         token->type = type;
//         token->text.length = 1;
//         return true;
//     }

//     return false;
// }

static cstr instruction_types_cstr[_TI_COUNT] = {
    [TI_ADD]       = ".add",
    [TI_DUPLICATE] = ".dup",
    [TI_SWAP]      = ".swap",
    [TI_JUMP]      = ".jump",
    [TI_ROTATE]    = ".rot",
};

static bool parse_instruction(Token* token) {
    for(u64 i = 0; i < ARRAY_LEN(instruction_types_cstr); i++) {
        if(sv_eq_cstr(&token->text, instruction_types_cstr[i])) {
            token->interpret.as_instruction = i;
            return true;
        }
    }

    token->type = TOKEN_UNKNOWN;
    return false;
}

Lexer lexer_create(cstr filename, StringView content) {
    Lexer l = {0};
    l.filename = filename;
    l.content = content;
    return l;
}

bool lexer_is_eof(const Lexer* l) {
    return l->cursor >= l->content.length;
}

Token lexer_token_next(Lexer* l) {
    skip_whitespace(l);

    Token token = {0};
    token.text.data = &l->content.data[l->cursor];
    token.location = (Location) {
        .filename = l->filename,
        .line = l->line + 1,
        .column = l->cursor - l->line_start + 1
    };

    if(lexer_is_eof(l)) {
        token.type = TOKEN_EOF;
        return token;
    }

    if(starts_with(l, "//")) {
        skipn(l, 2);

        token.text.length = 2;
        token.type = TOKEN_COMMENT;

        while(peek(l) != '\n') {
            token.text.length++;
            consume(l);
        }

        return token;
    }

    if(is_number(l)) {
        consume(l);
        token.text.length = 1;
        token.type = TOKEN_INTEGER;

        while(is_number(l)) {
            token.text.length++;
            consume(l);
        }

        ASSERT(sv_parse_u64(&token.text, &token.interpret.as_u64));
        return token;
    }

    if(symbol_type(l, &token, '#', TOKEN_CALL_BUILTIN))
        return token;

    if(symbol_type(l, &token, '$', TOKEN_LABEL_REF))
        return token;

    if(symbol_type(l, &token, ':', TOKEN_LABEL_DEF))
        return token;

    if(symbol_type(l, &token, '.', TOKEN_INSTRUCTION) && parse_instruction(&token)) {
        return token;
    }

    consume(l);
    token.text.length = 1;
    return token;
}

TokenList lexer_token_list(Lexer* l) {
    TokenList tl = {0};

    Token token;
    while((token = lexer_token_next(l)).type != TOKEN_EOF) {
        tl_append(&tl, &token);
    }

    return tl;
}

static cstr types_to_cstr[_TOKEN_COUNT] = {
    [TOKEN_EOF]          = "End Of File",
    [TOKEN_UNKNOWN]      = "Unknown",
    [TOKEN_INTEGER]      = "Integer",
    [TOKEN_COMMENT]      = "Comment",
    [TOKEN_LABEL_REF]    = "Label Reference",
    [TOKEN_LABEL_DEF]    = "Label Definition",
    [TOKEN_CALL_BUILTIN] = "Call Builtin",
    [TOKEN_INSTRUCTION]  = "Instruction",
};

cstr lexer_token_type_cstr(TokenType type) {
    return types_to_cstr[type];
}

void tl_append(TokenList* tl, const Token* token) {
    LIST_APPEND(tl, token);
}

void tl_extend(TokenList* tl, const TokenList* other_tl) {
    LIST_EXTEND(tl, other_tl);
}

void tl_free(TokenList* tl) {
    LIST_FREE(tl);
}
