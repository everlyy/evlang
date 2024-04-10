#ifndef _EVLANG_TYPES_H
#define _EVLANG_TYPES_H

typedef unsigned char u8;
typedef signed   char s8;

typedef unsigned short u16;
typedef signed   short s16;

typedef unsigned long u32;
typedef signed   long s32;

typedef unsigned long long u64;
typedef signed   long long s64;

typedef const char* cstr;

#ifndef NULL
#define NULL ((void*)0)
#endif // NULL

#define U8_MAX  (1 << 8)
#define U16_MAX (1 << 16)
#define U32_MAX (1 << 32)
#define U32_MAX (1 << 32)

#endif // _EVLANG_TYPES_H
