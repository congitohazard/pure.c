#ifndef PURE_BUILDER_COMPILE_H
#define PURE_BUILDER_COMPILE_H

#include "memory.h"

#define PURE_CMD_EMPTY               PURE_EMPTY_SLICE
#define PURE_CMD_EMPTY_WITH(_alloc)  PURE_DA_EMPTY_WITH(_alloc)

#define pure_cmd_append(_cmdArray, ...) \
    pure_da_extend(                     \
        _cmdArray,                      \
        PURE_CONSTRUCT_SLICE_LITERAL(   \
            PureString,                 \
            __VA_ARGS__                 \
        )                               \
    )

#define pure_cmd_extend(_cmdArray, _slice) \
    pure_da_extend(_cmdArray, _slice)

#define pure_cmd_shrink(_cmdArray, _newLen) \
    pure_da_shrink(_cmdArray, _newLen)

#define pure_cmd_clear(_cmdArray) \
    pure_da_clear(_cmdArray)

#define pure_cmd_free(_cmdArray) \
    pure_da_free(_cmdArray)

typedef struct {
    PureLiteral cc;
    PureStringSlice cflags, ldflags;
    struct {
        PureLiteral obj, exe;
    } extensions;
    struct {
        PureLiteral nolink, include, define, output;
    } flags;
} PureToolchain;

typedef struct {
    PureStringSlice definitions, includeDirs, libDirs, libs;
} PureContext;

typedef struct {
    PureToolchain *tc;
    PureContext   *ctx;
    struct {
        PureLiteral *inputs;
        PureLiteral *outputs;
        size_t count;
    } paths;
} PureCompileJob;

typedef struct {
    PureToolchain *tc;
    PureStringSlice inputPaths;
    PureLiteral output;
} PureLinkJob;

extern PureToolchain defaultChain;
extern PureToolchain defaultReleaseChain;
extern PureToolchain defaultDebugChain;

PureErrorCode pure_compile_sources(PureCompileJob *job);
PureErrorCode pure_link_sources(PureLinkJob *job);

#endif