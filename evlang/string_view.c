#include <evlang/string_view.h>

#include <evlang/assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static char* sv_to_cstr(const StringView* sv) {
    u64 length = sv->length + 1;
    char* buf = malloc(length);
    memcpy(buf, sv->data, length);
    buf[length - 1] = 0;
    return buf;
}

StringView sv_from_buffer(cstr data, u64 length) {
    return (StringView) {
        .data = data,
        .length = length
    };
}

u64 sv_find(const StringView* sv, char c) {
    for(u64 i = 0; i < sv->length; i++) {
        if(sv->data[i] == c)
            return i;
    }

    return SV_LOST;
}

StringView sv_split_null(StringView* right, ...) {
    StringView left = {
        .data = right->data,
        .length = 0
    };

    u64 pos = right->length;
    bool found = false;

    va_list args;
    va_start(args, right);
    int c;
    while((c = va_arg(args, int))) {
        u64 i = sv_find(right, c);
        if(i == SV_LOST)
            continue;

        found = true;
        if(i < pos)
            pos = i;
    }
    va_end(args);

    if(!found) {
        left.data = right->data;
        left.length = right->length;
        right->length = 0;
    } else {
        left.length = pos;
        right->length = right->length - pos - 1;
        right->data = &right->data[pos + 1];
    }

    return left;
}

void sv_trim_left(StringView* sv) {
    u64 i = 0;
    for(; i < sv->length; i++) {
        if(!isspace(sv->data[i]))
            break;
    }

    sv->data = &sv->data[i];
    sv->length = sv->length - i;
}

void sv_trim_right(StringView* sv) {
    for(; sv->length > 0; sv->length--) {
        if(!isspace(sv->data[sv->length - 1]))
            break;
    }
}

void sv_trim(StringView* sv) {
    sv_trim_left(sv);
    sv_trim_right(sv);
}

bool sv_parse_u64(const StringView* sv, u64* out) {
    for(u64 i = 0; i < sv->length; i++) {
        char c = sv->data[i];
        if(!isdigit(c))
            return false;
    }

    char* tmp = sv_to_cstr(sv);
    *out = atoll(tmp);
    free(tmp);
    return true;
}

bool sv_eq(const StringView* left, const StringView* right) {
    if(left->length != right->length)
        return false;

    return memcmp(left->data, right->data, left->length) == 0;
}

bool sv_eq_cstr(const StringView* left, cstr right) {
    StringView right_sv = {
        .data = right,
        .length = strlen(right)
    };

    return sv_eq(left, &right_sv);
}

bool sv_eq_char(const StringView* sv, char c) {
    if(sv->length != 1)
        return false;

    return sv->data[0] == c;
}

StringViewList svl_get_lines(StringView sv) {
    StringViewList lines = { 0 };
    StringView line;

    while(sv.length > 0) {
        line = sv_split(&sv, '\n');
        svl_append(&lines, &line);
    }

    return lines;
}
