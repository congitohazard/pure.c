#ifndef PURE_BUILDER_H
#define PURE_BUILDER_H

#include <stddef.h>

#include "pure_detect.h"
#include "pure_compile.h"

typedef struct {
    void *(*alloc)(size_t size);
    void (*free)(void *mem);
} PureAllocator;

#endif