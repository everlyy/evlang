#ifndef _LOG_H
#define _LOG_H

#include "evlang.h"
#include "lexer.h"
#include <stdio.h>

#define PREFIX_ERROR   "\e[31mERROR\e[0m"
#define PREFIX_WARNING "\e[33mWARNING\e[0m"
#define PREFIX_INFO    "\e[36mINFO\e[0m"

#define COMPILER_ERROR(fmt, ...)             \
    do {                                     \
        compiler_printf(                     \
            stderr,                          \
            __FILE__, __LINE__,              \
            PREFIX_ERROR, fmt,               \
            ##__VA_ARGS__                    \
        );                                   \
        exit(1);                             \
    } while(0)

#define COMPILER_ERROR_IF(cond, fmt, ...) if(cond) COMPILER_ERROR(fmt, ##__VA_ARGS__)

#define COMPILER_ERROR_LOC(loc, fmt, ...) COMPILER_ERROR(LOCATION_FMT ": " fmt, LOCATION_ARGS(loc), ##__VA_ARGS__)

#define COMPILER_WARN(fmt, ...) \
    do {                        \
        compiler_printf(        \
            stdout,             \
            __FILE__, __LINE__, \
            PREFIX_WARNING,     \
            fmt, ##__VA_ARGS__  \
        );                      \
    } while(0)

void compiler_printf(FILE* stream, cstr file, u32 line, cstr prefix, cstr fmt, ...);

#endif // _LOG_H
