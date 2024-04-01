#ifndef _LOG_H
#define _LOG_H

#include "evlang.h"
#include "lexer.h"
#include <stdio.h>

#define PREFIX_ERROR   "\e[31mERROR\e[0m"
#define PREFIX_WARNING "\e[33mWARNING\e[0m"
#define PREFIX_INFO    "\e[36mINFO\e[0m"

#define LOG_ERROR(fmt, ...)     \
    do {                        \
        log_printf(             \
            stderr,             \
            __FILE__, __LINE__, \
            PREFIX_ERROR, fmt,  \
            ##__VA_ARGS__       \
        );                      \
        exit(1);                \
    } while(0)

#define LOG_ERROR_IF(cond, fmt, ...) if(cond) LOG_ERROR(fmt, ##__VA_ARGS__)

#define LOG_ERROR_LOC(loc, fmt, ...) LOG_ERROR(LOCATION_FMT ": " fmt, LOCATION_ARGS(loc), ##__VA_ARGS__)

#define LOG_WARN(fmt, ...)      \
    do {                        \
        log_printf(             \
            stdout,             \
            __FILE__, __LINE__, \
            PREFIX_WARNING,     \
            fmt, ##__VA_ARGS__  \
        );                      \
    } while(0)

void log_printf(FILE* stream, cstr file, u32 line, cstr prefix, cstr fmt, ...);

#endif // _LOG_H
