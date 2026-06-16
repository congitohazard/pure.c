#ifndef PURE_BUILDER_COMPILE_H
#define PURE_BUILDER_COMPILE_H

typedef struct {
    const char *cc;
    const char *cflags;
    // const char *ldflags;
    struct {
        const char *obj;
        const char *exe;
    } extensions;
    const char *include_flag;
    const char *define_flag;
    const char *output_flag;
} PureToolchain;

typedef struct {
    const char **paths;
    size_t length, capacity;
} PurePathList;

typedef struct {
    PureToolchain *tc;
    struct {
        const char **inputs;
        const char **outputs;
        size_t count;
    } paths;
} PureJob;

extern PureToolchain defaultChain;
extern PureToolchain defaultReleaseChain;
extern PureToolchain defaultDebugChain;

void pure_compile_sources(PureJob *job);

#endif