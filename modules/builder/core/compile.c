#include <pure/pure.h>
#include <errno.h>

PureToolchain defaultChain = { 
    .cc = "gcc",
    .cflags = PURE_CONSTRUCT_SLICE(PureLiteral, "-Wall", "-Wextra", "-Wpedantic", "-std=c17"),
    .ldflags = PURE_EMPTY_SLICE,
    .extensions = {
        .obj = ".o",
        .exe = ".exe"
    },
    .flags = {
        .nolink = "-c",
        .include = "-I",
        .define = "-D",
        .output = "-o"
    }
};

PureToolchain defaultReleaseChain = {
    0
};

PureToolchain defaultDebugChain = {
    0
};

void pure_compile_sources(PureCompileJob *job) {


}

void pure_link_sources(PureLinkJob *job) {

}