#include <pure/pure.h>
#include <errno.h>

PureToolchain defaultChain = {
    .cc = "gcc",
    .cflags = "-Wall -Wextra -Wpedantic -std=c17",
    .ldflags = "",
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