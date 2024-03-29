#ifndef _TOKEN_H
#define _TOKEN_H

#include "evlang.h"
#include "list.h"
#include "string_view.h"

typedef struct {
    cstr filename;
    u32 line;
    u32 column;
} Location;

#define LOCATION_FMT "%s:%lu:%lu"
#define LOCATION_ARGS(l) (l)->filename, (l)->line, (l)->column

typedef enum {
    TI_ADD = 0,
    TI_DUPLICATE,
    TI_SWAP,
    TI_JUMP,
    TI_ROTATE,

    _TI_COUNT
} TokenInstruction;

typedef enum {
    TOKEN_UNKNOWN = 0,
    TOKEN_EOF,
    TOKEN_INTEGER,
    TOKEN_COMMENT,
    TOKEN_LABEL_REF,
    TOKEN_LABEL_DEF,
    TOKEN_CALL_BUILTIN,
    TOKEN_INSTRUCTION,

    _TOKEN_COUNT
} TokenType;

typedef struct {
    StringView text;
    TokenType type;
    Location location;

    union _Interpret {
        u64 as_u64;
        TokenInstruction as_instruction;
    } interpret;
} Token;

typedef struct {
    cstr filename;
    StringView content;
    u64 cursor;
    u64 line;
    u64 line_start;
} Lexer;

typedef LIST_DEFINE_STRUCT(Token) TokenList;

void tl_append(TokenList* tl, const Token* token);
void tl_extend(TokenList* tl, const TokenList* other_tl);
void tl_free(TokenList* tl);

Lexer lexer_create(cstr filename, StringView content);
bool lexer_is_eof(const Lexer* l);

Token lexer_token_next(Lexer* l);
TokenList lexer_token_list(Lexer* l);
cstr lexer_token_type_cstr(TokenType type);

#endif // _TOKEN_H
