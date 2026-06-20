#ifndef PURE_CORE_ERROR_H
#define PURE_CORE_ERROR_H

#include <stdbool.h>
#include <assert.h>

typedef enum {
    PURE_SUCCESS_CODE,
    PURE_ERROR_GENERIC,
    PURE_ERROR_ENOMEM,
    PURE_ERROR_PROC_FAIL,
    PURE_ERROR_INVALID_INPUT
} PureErrorCode;

#define PURE_IS_SUCCESS(_code) (_code == PURE_SUCCESS_CODE)
#define PURE_IS_ERROR(_code)   (_code != PURE_SUCCESS_CODE)

#ifndef PURE_ASSERT
    #define PURE_ASSERT assert
#endif 

#endif