#include "list.h"
#include "evlang_assert.h"
#include "log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void list_append(void** items, u64* count, u64* capacity, u64 item_size, const void* item) {
    if(*count >= *capacity) {
        *capacity = (
            *items ?
            *capacity * 2 :
            LIST_INITIAL_CAPACITY
        );

        *items = realloc(*items, *capacity * item_size);
        ASSERT(items != NULL);
    }

    memcpy(*(char**)items + *count * item_size, item, item_size);
    (*count)++;
}

void list_extend(void** items, u64* count, u64* capacity, u64 item_size, const void* other_items, u64 other_count) {
    if((*count + other_count) >= *capacity) {
        *capacity = (
            *items ?
            *capacity + other_count :
            other_count
        );

        *items = realloc(*items, *capacity * item_size);
        ASSERT(items != NULL);
    }

    memcpy(*(char**)items + *count * item_size, other_items, other_count * item_size);
    *count += other_count;
}

void list_free(void* items) {
    if(items)
        free(items);
}
