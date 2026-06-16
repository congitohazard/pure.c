#include <stdio.h>
#include <stdbool.h>

#include <Windows.h>

#include <internal/pure_platform.h>

PureErrorCode platform_run_command(PureCommandSlice cmd) {
    STARTUPINFO si = { .cb = sizeof(si) };
    PROCESS_INFORMATION pi = {0};
    PureErrorCode code = PURE_SUCCESS_CODE;
    PureStringBuilder builder;
    pure_sb_init(&builder);

    for(size_t i = 0; i < cmd.length; i++) {
        for(PureLiteral current = cmd.data[i]; *current != '\0'; current++) {
            code = pure_sb_push(&builder, *current);
            if(PURE_IS_ERROR(code)) goto CLEANUP;
        }
        if(i != cmd.length - 1) {
            code = pure_sb_push(&builder, ' ');
            if(PURE_IS_ERROR(code)) goto CLEANUP;
        }
    }
    PureString cmdStr = pure_sb_finish(&builder, &code);
    if(PURE_IS_ERROR(code)) goto CLEANUP;

    CreateProcessA(
        NULL,
        cmdStr,
        NULL,
        NULL,
        FALSE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    );
    CLEANUP:
    pure_sb_clean(&builder);
    return code;
}