#ifndef PURE_BUILDER_PLATFORM_H
#define PURE_BUILDER_PLATFORM_H

#include "../pure_error.h"
#include "../pure_memory.h"

DECLARE_SLICE(PureString *, Command);

PureErrorCode platform_run_command(PureCommandSlice cmd);
// Sync commands

#endif