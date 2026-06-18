#include <pure/abs/platform.h>

PureErrorCode pure_platform_run_sync(PureCommandSlice cmd, PureProcExit *outExit) {
    // fork
    // exec
}

PureErrorCode pure_platform_run_async(PureCommandSlice cmd, PureProc *outProc) {

}

PureErrorCode pure_platform_sync_proc(PureProc *proc, PureProcExit *outExit) {

}

PureErrorCode pure_platform_sync_proc_slice(PureProcSlice procs, PureProcExitSlice *outExits) {

}