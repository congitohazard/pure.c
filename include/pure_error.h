#ifndef PURE_BUILDER_ERROR_H
#define PURE_BUILDER_ERROR_H

typedef enum {
    PURE_SUCCESS_CODE,
    PURE_ERROR_GENERIC,
    PURE_ERROR_ENOMEM
} PureErrorCode;

#define PURE_IS_SUCCESS(_code) (_code == PURE_SUCCESS_CODE)
#define PURE_IS_ERROR(_code)   (_code != PURE_SUCCESS_CODE)

#endif