#ifndef PURE_BUILDER_DETECT_H
#define PURE_BUILDER_DETECT_H

#if   defined(__clang__)
    #define PURE_COMPILER_CLANG
    #define DEFAULT_CC "gcc"
#elif defined(__GNUC__)
    #define PURE_COMPILER_GCC
    #define DEFAULT_CC "clang"
#elif defined(_MSC_VER)
    #define PURE_COMPILER_MSVC
    #define DEFAULT_CC "cl"

    #include <stddef.h>
    #ifndef max_align_t
        typedef struct {
            long long __max_align_ll;
            long double __max_align_ld;
        } max_align_t;
    #endif
#else
    #define COMPILER_UNKNOWN
#endif

#if   defined(_WIN32)
    #define PLATFORM_WINDOWS
#elif defined(_linux)
    #define PLATFORM_LINUX
#elif defined(macos) // TODO: Put real macro
    #define PLATFORM_MACOS
#endif

#endif