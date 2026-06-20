#ifndef PURE_BUILDER_PLATFORM_H
#define PURE_BUILDER_PLATFORM_H

#include <stdint.h>

#include "../core/error.h"
#include "../core/memory.h"

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

PureErrorCode pure_platform_run_sync(PureStringSlice cmd, PureProcExit *outExit);
PureErrorCode pure_platform_run_async(PureStringSlice cmd, PureProc *outProc);

PureProcExit pure_platform_wait_proc(PureProc *proc);
PureErrorCode pure_platform_wait_proc_slice(PureProcSlice procs, PureProcExitSlice *outExits);

#endif