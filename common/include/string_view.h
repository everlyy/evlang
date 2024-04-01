#ifndef _STRING_VIEW_H
#define _STRING_VIEW_H

#include "evlang.h"
#include "list.h"

#include <stdbool.h>

#define SV_WHITESPACE ' ', '\t', '\n', '\v', '\f', '\r'
#define SV_LOST 0xFFFFFFFFFFFFFFFF

#define SV_ARGS "%.*s"
#define SV_FMT(sv) (int)(sv)->length, (sv)->data

typedef struct {
    cstr data;
    u64 length;
} StringView;

typedef LIST_DEFINE_STRUCT(StringView) StringViewList;

#define svl_append(svl, sv) LIST_APPEND(svl, sv)
#define svl_free(svl) LIST_FREE(svl)

StringView sv_from_buffer(cstr data, u64 length);
u64 sv_find(const StringView* sv, char c);
#define sv_split(right, ...) sv_split_null((right), __VA_ARGS__, NULL);
StringView sv_split_null(StringView* right, ...);
void sv_trim_left(StringView* sv);
void sv_trim_right(StringView* sv);
void sv_trim(StringView* sv);
bool sv_parse_u64(const StringView* sv, u64* out);
bool sv_eq(const StringView* left, const StringView* right);
bool sv_eq_cstr(const StringView* left, cstr right);
bool sv_eq_char(const StringView* sv, char c);
StringViewList svl_get_lines(StringView sv);

#endif // _STRING_VIEW_H
