#ifndef PURE_BUILDER_MEMORY_H
#define PURE_BUILDER_MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "error.h"

#define GET_STATIC_SIZE(_arr)     \
    (sizeof(_arr) / sizeof(*_arr))

#define TAKE_STATIC_SLICE(_arr)   \
    { _arr, GET_STATIC_SIZE(_arr) }

#define TAKE_STATIC_SLICE_LITERAL(_arr, _name)   \
    (Pure ## _name ## Slice) { _arr, GET_STATIC_SIZE(_arr) }

#define PURE_CONSTRUCT_SLICE(_type, ...)                                        \
    {                                                                           \
        .elems = (_type []) { __VA_ARGS__ },                            \
        .len = sizeof((_type []) {__VA_ARGS__}) / sizeof(_type) \
    }

#define PURE_EMPTY_SLICE { NULL, 0 }

#define DECLARE_SLICE(_type, _name)             \
    typedef union _Pure ## _name ## Slice {     \
        struct {                                \
            _type *elems;                       \
            size_t len;                         \
        };                                      \
        PureAnonSlice anon;                     \
    } Pure ## _name ## Slice

#define DECLARE_DA_WITH_SLICE(_type, _name) \
    DECLARE_SLICE(_type, _name);            \
    typedef union {                         \
        struct {                            \
            Pure ## _name ## Slice;         \
            PureArrayMeta;                  \
        };                                  \
        Pure ## _name ## Slice slice;       \
        PureAnonArray anon;                 \
    } Pure ## _name ## Array
typedef struct _PureArrayMeta {
    size_t cap, growthFactor;
    PureAllocator *mem;
} PureArrayMeta;

typedef struct _PureAnonSlice {
    void *elems;
    size_t len;
} PureAnonSlice;

typedef struct _PureAnonArray {
    union {
        PureAnonSlice;
        PureAnonSlice slice;
    };
    PureArrayMeta;
} PureAnonArray;

DECLARE_DA_WITH_SLICE(char, Char);

typedef char Byte;
typedef Byte *ByteStream;

typedef const char *PureLiteral;
typedef char *PureString;
typedef char PureDefaultStringBuffer[256];

typedef struct _PureAllocator {
    void *(*alloc)(size_t size);
    void *(*realloc)(void *data, size_t newSize);
    void (*free)(void *data);
} PureAllocator;

typedef struct _PureStringBuilder {
    PureCharArray str;
    PureDefaultStringBuffer staticBuffer;
} PureStringBuilder;

#define INITIAL_DEFAULT_ALLOCATOR { malloc, realloc, free }

extern PureAllocator defaultAllocator;

// DA system (Dynamic Array)

#define PURE_DA_INITIAL_CAP          64
#define PURE_DA_GROWTH_FACTOR        2
#define PURE_DA_EMPTY                {0}
#define PURE_DA_EMPTY_WITH(_alloc)   { .mem = &(_alloc) }
#define PURE_DA_MEM_SIZE(_da)        (_da.len * sizeof(*da.elems))

#define pure_da_reserve(_da, _amount)   \
    pure_da_reserve_raw(                \
        (PureAnonArray *) &_da.anon,    \
        _amount,                        \
        sizeof(*_da.elems)              \
    )

PureErrorCode pure_da_reserve_raw(PureAnonArray *da, size_t amount, size_t elemSize);
PureErrorCode pure_da_append_raw(PureAnonArray *da, void *elem, size_t elemSize);
PureErrorCode pure_da_extend_raw(PureAnonArray *da, PureAnonSlice *slice, size_t elemSize);

#define pure_da_append(_da, _elem) \
    pure_da_append_raw(&_da.anon, (void *) _elem, sizeof(*_da.elems))

#define pure_da_extend(_da, _slice) \
    pure_da_extend_raw(&_da.anon, &_slice.anon, sizeof(*_da.elems))

#define pure_da_clear(_da)  \
    do {                    \
        _da.len = 0;        \
    } while(0)

#define pure_da_free(_da)               \
    do {                                \
        _da.len = _da.cap = 0;          \
        if(_da.mem != NULL)             \
            _da.mem->free(_da.elems);   \
        _da.elems = NULL;               \
    } while(0)

// pure_da_shrink
// pure_da_insert
// pure_da_remove
// pure_da_swap_remove
// pure_da_extend
// pure_da_byte_size
// pure_da_fill

// SB system (String Builder)

void pure_sb_start(PureStringBuilder *builder);
void pure_sb_start_with(PureStringBuilder *builder, PureAllocator *mem);
PureErrorCode pure_sb_push(PureStringBuilder *builder, char c);
PureString pure_sb_finish(PureStringBuilder *builder, PureErrorCode *outCode);
void pure_sb_clean(PureStringBuilder *builder);

// TODO: stringops, temp arena, arb arenas, align

#endif