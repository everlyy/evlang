#ifndef _EVLANG_ASSERT_H
#define _EVLANG_ASSERT_H

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(assertion)                               \
    if(!(assertion)) {                                  \
        fprintf(                                        \
            stderr,                                     \
            "Assertion `%s` failed in %s on line %d\n", \
            #assertion,                                 \
            __FILE__, __LINE__                          \
        );                                              \
        abort();                                        \
    }

#endif // _EVLANG_ASSERT_H
