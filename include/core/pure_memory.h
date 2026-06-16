#ifndef PURE_BUILDER_MEMORY_H
#define PURE_BUILDER_MEMORY_H

#include <stdbool.h>
#include <stddef.h>

#include "pure_error.h"

#define GET_STATIC_SIZE(_arr)     \
    (sizeof(_arr) / sizeof(*_arr))

#define TAKE_STATIC_SLICE(_arr)   \
    { _arr, GET_STATIC_SIZE(_arr) }

#define TAKE_DA_SLICE(_da, _name) \
    (Pure ## _name ## Slice) { .data = _da.data, .length = _da.length }

#define DECLARE_DA(_type, _name)                \
    typedef struct _Pure ## _name ## Array {    \
        _type *data;                            \
        size_t length, capacity;                \
        PureAllocator mem;                      \
    } Pure ## _name ## Array;

#define DECLARE_SLICE(_type, _name)             \
    typedef struct _Pure ## _name ## Slice {    \
        _type *data;                            \
        size_t length;                          \
    } Pure ## _name ## Slice;

#define NEXT_INDEX(_arr, _val) _arr.data[_arr.length] = _val

#define da_append_def(_da, _item)
#define da_append(_da, _item)

typedef const char *PureLiteral;
typedef char *PureString;
typedef char PureDefaultStringBuffer[256];

DECLARE_DA(char, String);

typedef struct {
    void *(*alloc)(size_t size);
    void *(*realloc)(void *data, size_t newSize);
    void (*free)(void *data);
} PureAllocator;

typedef struct {
    PureStringArray str;
    PureDefaultStringBuffer staticBuffer;
} PureStringBuilder;

#define INITIAL_DEFAULT_ALLOCATOR { malloc, realloc, free }
extern PureAllocator defaultAllocator;

void pure_sb_init(PureStringBuilder *builder);
void pure_sb_init_with(PureStringBuilder *builder, PureAllocator alloc);
PureErrorCode pure_sb_push(PureStringBuilder *builder, char c);
PureString pure_sb_finish(PureStringBuilder *builder, PureErrorCode *outCode);
void pure_sb_clean(PureStringBuilder *builder);

#endif