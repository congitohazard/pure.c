#include <pure/abs/platform.h>

PureErrorCode pure_platform_run_sync(PureCommandSlice cmd, PureProcExit *outExit) {
    // fork
    // exec
}

PureErrorCode pure_platform_run_async(PureCommandSlice cmd, PureProc *outProc) {

}

PureErrorCode pure_platform_wait_proc(PureProc *proc) {

}

PureErrorCode pure_platform_wait_proc_slice(PureProcSlice procs, PureProcExitSlice *outExits) {

}