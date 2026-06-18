#ifndef PURE_BUILDER_MEMORY_H
#define PURE_BUILDER_MEMORY_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "error.h"

#define GET_STATIC_SIZE(_arr)     \
    (sizeof(_arr) / sizeof(*_arr))

#define TAKE_STATIC_SLICE(_arr)   \
    { _arr, GET_STATIC_SIZE(_arr) }

#define TAKE_STATIC_SLICE_LITERAL(_arr)   \
    { _arr, GET_STATIC_SIZE(_arr) }

#define TAKE_DA_SLICE(_da, _name) \
    (Pure ## _name ## Slice) { .elems = _da.elems, .len = _da.len }

#define DECLARE_DA(_type, _name)                \
    typedef struct Pure ## _name ## _Array {    \
        _type *elems;                           \
        union {                                 \
            PureDaMetaData;                     \
            PureDaMetaData meta;                \
        };                                      \
    } Pure ## _name ## Array

#define DECLARE_SLICE(_type, _name)             \
    typedef struct _Pure ## _name ## Slice {    \
        _type *elems;                           \
        size_t len;                             \
    } Pure ## _name ## Slice

DECLARE_DA(char, Char);

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

typedef struct _PureDaMetaData {
    size_t len, cap, growthFactor;
    PureAllocator *mem;
} PureDaMetaData;

#define pure_da_reserve(_da, _amount) \
    PURE_ASSERT(                        \
        PURE_IS_SUCCESS( \
            pure_da_reserve_raw( \
                (void **) &_da.elems, \
                _amount, \
                &_da.meta, \
                sizeof(*_da.elems) \
            ) \
        )     \
    )

#define pure_da_reserve_e(_da, _amount, _error) \
    _error = pure_da_reserve_raw( \
        (void **) &_da.elems, \
        _amount, \
        &_da.meta, \
        sizeof(*_da.elems) \
    ) \

PureErrorCode pure_da_reserve_raw(
    void **daElems,
    size_t amount,
    PureDaMetaData *meta,
    size_t elemSize
);

#define pure_da_append(_da, _elem)                                          \
    do {                                                                    \
        pure_da_reserve(_da, _da.len + 1)                       \
        _da.elems[_da.len++] = _elem;                                    \
    } while(0)

#define pure_da_append_e(_da, _elem, _error)                                \
    do {                                                                    \
        pure_da_reserve_e(_da, _da.len + 1, _error);                      \
        if(PURE_IS_ERROR(_error)) break;                                    \
        _da.elems[_da.len++] = _elem;                                    \
    } while(0)

#define pure_da_append_slice(_da, _slice)                                        \
    do {                                                                                \
        pure_da_reserve(_da, _da.len + _slice.len)                                  \
        memcpy( \
            _da.elems + _da.len, \
            _slice.elems, \
            _slice.len * sizeof(*_da.elems) \
        ); \
        _da.len += _slice.len; \
    } while(0)

#define pure_da_append_slice_e(_da, _slice, _error)                                  \
    do {                                                                                    \
        pure_da_reserve(_da, _da.len + _slice.len, _error);                        \
        if(PURE_IS_ERROR(_error))    \
            break;                                                                          \
        memcpy( \
            _da.elems + _da.len, \
            _slice.elems, \
            _slice.len * sizeof(*_da.elems) \
        ); \
        _da.len += _slice.len; \
    } while(0)

#define pure_da_clear(_da) \
    _da.len = 0

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