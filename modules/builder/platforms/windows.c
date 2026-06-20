#include <stdio.h>
#include <stdbool.h>

#include <Windows.h>

#include <pure/abs/platform.h>

static STARTUPINFO emptyConfig = { .cb = sizeof(STARTUPINFO) };
static PROCESS_INFORMATION emptyInfo;

static PureProcExit read_windows_proc_exit(HANDLE procHandle);

PureErrorCode pure_platform_run_sync(PureStringSlice cmd, PureProcExit *outExit) {
    STARTUPINFO config = emptyConfig;
    PROCESS_INFORMATION info = emptyInfo;
    PureErrorCode code = PURE_SUCCESS_CODE;

    PureStringBuilder builder;
    pure_sb_start(&builder);

    for(size_t i = 0; i < cmd.len; i++) {
        for(PureLiteral current = cmd.elems[i]; *current != '\0'; current++) {
            code = pure_sb_push(&builder, *current);
            if(PURE_IS_ERROR(code)) goto CLEANUP;
        }
        if(i != cmd.len - 1) {
            code = pure_sb_push(&builder, ' ');
            if(PURE_IS_ERROR(code)) goto CLEANUP;
        }
    }
    PureString cmdStr = pure_sb_finish(&builder, &code);
    if(PURE_IS_ERROR(code)) goto CLEANUP;

    bool started = (bool) CreateProcessA(
        NULL,
        cmdStr,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &config,
        &info
    );

    if(started) {
        WaitForSingleObject(info.hProcess, INFINITE);
        if(outExit != NULL)
            *outExit = read_windows_proc_exit(info.hProcess);
        CloseHandle(info.hThread);
        CloseHandle(info.hProcess);
    }
    else
        code = PURE_ERROR_PROC_FAIL;

    CLEANUP:
    pure_sb_clean(&builder);
    return code;
}

PureErrorCode pure_platform_run_async(PureStringSlice cmd, PureProc *outProc) {
    STARTUPINFO config = emptyConfig;
    PROCESS_INFORMATION info = emptyInfo;
    PureErrorCode code = PURE_SUCCESS_CODE;

    PureStringBuilder builder;
    pure_sb_start(&builder);

    for(size_t i = 0; i < cmd.len; i++) {
        for(PureLiteral current = cmd.elems[i]; *current != '\0'; current++) {
            code = pure_sb_push(&builder, *current);
            if(PURE_IS_ERROR(code)) goto CLEANUP;
        }
        if(i != cmd.len - 1) {
            code = pure_sb_push(&builder, ' ');
            if(PURE_IS_ERROR(code)) goto CLEANUP;
        }
    }
    PureString cmdStr = pure_sb_finish(&builder, &code);
    if(PURE_IS_ERROR(code)) goto CLEANUP;

    bool started = (bool) CreateProcessA(
        NULL,
        cmdStr,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &config,
        &info
    );
    if(started) {
        CloseHandle(info.hThread);
        if(outProc != NULL)
            outProc->handle = (uintptr_t) info.hProcess;
    }
    else
        code = PURE_ERROR_PROC_FAIL;

    CLEANUP:
    pure_sb_clean(&builder);
    return code;
}

PureProcExit pure_platform_wait_proc(PureProc *proc) {
    WaitForSingleObject((HANDLE) proc->handle, INFINITE);
    PureProcExit exitData = read_windows_proc_exit((HANDLE) proc->handle);
    CloseHandle((HANDLE) proc->handle);
    return exitData;
}

PureErrorCode pure_platform_wait_proc_slice(PureProcSlice procs, PureProcExitSlice *outExits) {
    if(outExits != NULL && outExits->len < procs.len)
        return PURE_ERROR_INVALID_INPUT;

    HANDLE *handles = defaultAllocator.alloc(procs.len * sizeof(HANDLE));
    if(handles == NULL)
        return PURE_ERROR_ENOMEM;

    for(size_t i = 0; i < procs.len; i++)
        handles[i] = (HANDLE) procs.elems[i].handle;

    WaitForMultipleObjects((DWORD) procs.len, handles, TRUE, INFINITE);
    for(size_t i = 0; i < procs.len; i++) {
        if(outExits != NULL)
            outExits->elems[i] = read_windows_proc_exit(handles[i]);
        CloseHandle(handles[i]);
    }
    defaultAllocator.free(handles);
    return PURE_SUCCESS_CODE;
}

static PureProcExit read_windows_proc_exit(HANDLE procHandle) {
    DWORD exitCode = 0;
    PureProcExit exitData;
    GetExitCodeProcess(procHandle, &exitCode);
    exitData.code = (int) exitCode;
    if(exitCode == 0)
        exitData.type = PURE_PROC_EXIT_SUCCESS;
    else if(exitCode & 0xC0000000)
        exitData.type = PURE_PROC_EXIT_CRASH;
    else
        exitData.code = PURE_PROC_EXIT_ERROR;
    return exitData;
}