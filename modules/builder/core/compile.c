#include <pure/pure.h>

PureToolchain defaultChain = { 
    .cc = "gcc",
    .cflags = PURE_CONSTRUCT_SLICE(
        PureLiteral, 
        "-Wall", "-Wextra", "-Wpedantic", "-std=c17"
    ),
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

PureErrorCode pure_compile_sources(PureCompileJob *job) {
    if(
        job == NULL                ||
        job->paths.inputs  == NULL ||
        job->paths.outputs == NULL ||
        job->paths.count   == 0
    )
        return PURE_ERROR_INVALID_INPUT;
    PureToolchain *tc = (job->tc == NULL) ? &defaultChain : job->tc;

    PureStringArray cmd = PURE_CMD_EMPTY;
    for(size_t i = 0; i < job->paths.count; i++) {
        // gcc -Wall -Wextra -Wpedantic -std=c17 -I../../include main.c -o main.o
        pure_cmd_append(cmd, tc->cc);
        pure_cmd_extend(cmd, tc->cflags);
        pure_cmd_append(
            cmd,
            "-I../../include",
            job->paths.inputs[i],
            tc->flags.output,
            job->paths.outputs[i]
        );
        pure_platform_run_sync(cmd.slice, NULL);
        pure_cmd_clear(cmd);
    };
    pure_cmd_free(cmd);
    return PURE_SUCCESS_CODE;
}

PureErrorCode pure_link_sources(PureLinkJob *job) {
    if(
        job == NULL                   ||
        job->inputPaths.elems == NULL ||
        job->inputPaths.len   == 0
    )
        return PURE_ERROR_INVALID_INPUT;
    PureToolchain *tc = (job->tc == NULL) ? &defaultChain : job->tc;

    // gcc -Wall -Wextra -Wpedantic -std=c17 main.o -o main.exe
    PureStringArray cmd = PURE_CMD_EMPTY;
    pure_cmd_append(cmd, tc->cc);
    pure_cmd_extend(cmd, tc->cflags);
    pure_cmd_extend(cmd, job->inputPaths);
    pure_cmd_append(cmd, tc->flags.output, job->output);
    pure_platform_run_sync(cmd.slice, NULL);
    pure_cmd_free(cmd);
    return PURE_SUCCESS_CODE;
}