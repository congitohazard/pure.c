#ifndef PURE_BUILDER_COMPILE_H
#define PURE_BUILDER_COMPILE_H

#include "memory.h"

DECLARE_DA(PureString *, Path);
DECLARE_SLICE(PureLiteral *, Path);
DECLARE_SLICE(PureLiteral *, Context);

typedef struct {
    PureLiteral cc, cflags, ldflags;
    struct {
        PureLiteral obj, exe;
    } extensions;
    struct {
        PureLiteral include, define, output;
    } flags;
} PureToolchain;

typedef struct {
    PureContextSlice definitions, includeDirs, libDirs, libs;
} PureContext;

typedef struct {
    PureToolchain *tc;
    struct {
        PureLiteral *inputs;
        PureLiteral *outputs;
        size_t count;
    } paths;
} PureCompileJob;

typedef struct {
    PureToolchain *tc;
    PureLiteral output;
    PurePathSlice *inputs;
} PureLinkJob;

extern PureToolchain defaultChain;
extern PureToolchain defaultReleaseChain;
extern PureToolchain defaultDebugChain;

void pure_compile_sources(PureCompileJob *job);
void pure_link_sources(PureLinkJob *job);

#endif