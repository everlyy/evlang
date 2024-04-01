#include "log.h"

#include <stdarg.h>
#include <stdio.h>

void log_printf(FILE* stream, cstr file, u32 line, cstr prefix, cstr fmt, ...) {
    fprintf(stream, "%s: [%s:%lu] ", prefix, file, line);

    va_list args;
    va_start(args, fmt);
    vfprintf(stream, fmt, args);
    va_end(args);

    fprintf(stream, "\n");
}
