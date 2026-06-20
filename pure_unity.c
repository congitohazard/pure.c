#include <pure/pure.h>

#include "modules/builder/core/compile.c"
#include "modules/builder/core/error.c"
#include "modules/builder/core/memory.c"

#if   defined(PLATFORM_WINDOWS)
    #include "modules/builder/platforms/windows.c"
#elif defined(PLATFORM_LINUX)
    #include "modules/builder/platforms/src/linux.c"
#else
    #error "Unsupported platform"
#endif