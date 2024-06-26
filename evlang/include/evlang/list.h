#ifndef _EVLANG_LIST_H
#define _EVLANG_LIST_H

#include <evlang/assert.h>
#include <evlang/types.h>

#define LIST_INITIAL_CAPACITY 16

#define LIST_DEFINE_STRUCT(type) \
    struct _list_##type {        \
        type* items;             \
        u64 count;               \
        u64 capacity;            \
    }

#define LIST_APPEND(list, item)                                        \
    do {                                                               \
        ASSERT(sizeof(*(list)->items) == sizeof(*item));               \
        list_append(                                                   \
            (void**)&(list)->items, &(list)->count, &(list)->capacity, \
            sizeof(*(list)->items), item                               \
        );                                                             \
    } while(0)

#define LIST_EXTEND(list, other_list)                                   \
    do {                                                                \
        ASSERT(sizeof(*(list)->items) == sizeof(*(other_list)->items)); \
        list_extend(                                                    \
            (void**)&(list)->items, &(list)->count, &(list)->capacity,  \
            sizeof(*(list)->items),                                     \
            (other_list)->items, (other_list)->count                    \
        );                                                              \
    } while(0)

#define LIST_FREE(list) list_free((list)->items)

#define LIST_ITERATE(list, type, element_name, body)           \
    for(u64 __index = 0; __index < (list)->count; __index++) { \
        type* element_name = &(list)->items[__index];          \
        body;                                                  \
    }

#define LIST_FUNCTIONS_DEFINE(list_type, item_type, prefix)             \
    void prefix##_append(list_type* list, const item_type* item);       \
    void prefix##_extend(list_type* list, const list_type* other_list); \
    void prefix##_free(list_type* list);                                \

#define LIST_FUNCTIONS_IMPLEMENT(list_type, item_type, prefix)                                             \
    void prefix##_append(list_type* list, const item_type* item)        { LIST_APPEND(list, item);       } \
    void prefix##_extend(list_type* list, const list_type* other_list)  { LIST_EXTEND(list, other_list); } \
    void prefix##_free(list_type* list)                                 { LIST_FREE(list);               } \

#define LIST_DEFINE_ALL(list_type, item_type, prefix)   \
    typedef LIST_DEFINE_STRUCT(item_type) list_type;    \
    LIST_FUNCTIONS_DEFINE(list_type, item_type, prefix) \

void list_append(void** items, u64* count, u64* capacity, u64 item_size, const void* item);
void list_extend(void** items, u64* count, u64* capacity, u64 item_size, const void* other_items, u64 other_count);
void list_free(void* items);

#endif // _EVLANG_LIST_H
