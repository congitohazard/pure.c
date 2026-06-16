#include <stdlib.h>

#include <pure_memory.h>

PureAllocator defaultAllocator = { malloc, realloc, free };

void pure_sb_init(PureStringBuilder *builder) {
    builder->str = (PureStringArray) {
        .data = builder->staticBuffer,
        .length = 0,
        .capacity = GET_STATIC_SIZE(builder->staticBuffer)  
    };
    builder->mem = defaultAllocator;
}

void pure_sb_init_with(PureStringBuilder *builder, PureAllocator mem) {
    builder->str = (PureStringArray) {
        .data = builder->staticBuffer,
        .length = 0,
        .capacity = GET_STATIC_SIZE(builder->staticBuffer)  
    };
    builder->mem = mem;
}

PureErrorCode pure_sb_push(PureStringBuilder *builder, char c) {

    return PURE_SUCCESS_CODE;
}

PureString pure_sb_finish(PureStringBuilder *builder, PureErrorCode *outCode) {
    PureErrorCode localCode = pure_sb_push(builder, '\0');
    if(outCode != NULL) *outCode = localCode;
    if(PURE_IS_ERROR(localCode)) return NULL;
    return builder->str.data;
}

void pure_sb_clean(PureStringBuilder *builder) {

}