#ifndef PURE_BUILDER_PLATFORM_H
#define PURE_BUILDER_PLATFORM_H

#include <stdint.h>

#include "../core/error.h"
#include "../core/memory.h"

DECLARE_SLICE(PureProc, Proc);
DECLARE_SLICE(PureProcExit, ProcExit);

DECLARE_DA(PureString *, Command);
DECLARE_SLICE(PureString *, Command);

typedef union {
    uintptr_t handle;
    uint32_t  id;
} PureProc;

typedef enum {
    PURE_PROC_EXIT_SUCCESS,
    PURE_PROC_EXIT_ERROR,
    PURE_PROC_EXIT_SIGNAL,
    PURE_PROC_EXIT_CRASH
} PureProcExitKind;

typedef struct {
    PureProcExitKind type;
    int code;
} PureProcExit;

#define pure_cmd_append(_cmdArray, ...)

#define pure_cmd_append_e(_error, _cmdArray, ...)

#define pure_cmd_extend(_destArray, _srcSlice)

#define pure_cmd_extend_e(_error, _destArray, _srcSlice)

#define pure_cmd_clear(_cmdArray)

#define pure_cmd_free(_cmdArray)


PureErrorCode pure_platform_run_sync(PureCommandSlice cmd, PureProcExit *outExit);
PureErrorCode pure_platform_run_async(PureCommandSlice cmd, PureProc *outProc);

PureProcExit pure_platform_wait_proc(PureProc *proc);
PureErrorCode pure_platform_wait_proc_slice(PureProcSlice procs, PureProcExitSlice *outExits);

#endif