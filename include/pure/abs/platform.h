#ifndef PURE_BUILDER_PLATFORM_H
#define PURE_BUILDER_PLATFORM_H

#include <stdint.h>

#include "../core/error.h"
#include "../core/memory.h"
#include "../core/command.h"

DECLARE_SLICE(PureProc, Proc);
DECLARE_SLICE(PureProcExit, ProcExit);

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

PureErrorCode pure_platform_run_sync(PureCommandSlice cmd, PureProcExit *outExit);
PureErrorCode pure_platform_run_async(PureCommandSlice cmd, PureProc *outProc);

PureErrorCode pure_platform_sync_proc(PureProc *proc, PureProcExit *outExit);
PureErrorCode pure_platform_sync_proc_slice(PureProcSlice procs, PureProcExitSlice *outExits);

#endif