#include <stdlib.h>
#include <stdalign.h>
#include <stddef.h>

#include <pure/definitions.h>
#include <pure/core/memory.h>

PureAllocator defaultAllocator = INITIAL_DEFAULT_ALLOCATOR;

PureErrorCode pure_da_reserve_raw(
    void **daElems,
    size_t amount,
    PureDaMetaData *meta,
    size_t elemSize
) {
    PURE_ASSERT(
        daElems != NULL  &&
        meta != NULL     &&
        elemSize != 0
    );
    if(PURE_DA_GROWTH_FACTOR > meta->growthFactor)
        meta->growthFactor = PURE_DA_GROWTH_FACTOR;
    if(meta->mem == NULL)
        meta->mem = &defaultAllocator;
    if(meta->cap >= amount)
        return PURE_SUCCESS_CODE;

    size_t newCap = (meta->cap == 0) ? PURE_DA_INITIAL_CAP : meta->cap;
    while(newCap < amount)
        newCap *= meta->growthFactor;

    void *tmp = meta->mem->realloc(*daElems, newCap * elemSize);
    if(tmp == NULL)
        return PURE_ERROR_ENOMEM;

    *daElems = tmp;
    meta->cap = newCap;
    return PURE_SUCCESS_CODE;
}

void pure_sb_start(PureStringBuilder *builder) {
    PURE_ASSERT(builder != NULL);
    builder->str = (PureCharArray) {
        .elems = builder->staticBuffer,
        .len = 0,
        .cap = GET_STATIC_SIZE(builder->staticBuffer)  
    };
    builder->str.mem = &defaultAllocator;
}

void pure_sb_start_with(PureStringBuilder *builder, PureAllocator *mem) {
    PURE_ASSERT(builder != NULL && mem != NULL);
    builder->str = (PureCharArray) {
        .elems = builder->staticBuffer,
        .len = 0,
        .cap = GET_STATIC_SIZE(builder->staticBuffer)  
    };
    builder->str.mem = mem;
}

PureErrorCode pure_sb_push(PureStringBuilder *builder, char c) {
    PURE_ASSERT(builder != NULL);
    PureErrorCode code = PURE_SUCCESS_CODE;
    if(builder->str.elems == builder->staticBuffer) {
        if(builder->str.cap > builder->str.len)
            builder->str.elems[builder->str.len++] = c;
        else {
            PureCharArray daString = PURE_DA_EMPTY;
            pure_da_reserve_e(daString, builder->str.len + 1, code);
            if(PURE_IS_SUCCESS(code)) {
                builder->str = daString;
                memcpy(
                    daString.elems,
                    builder->staticBuffer,
                    sizeof(builder->staticBuffer)
                );
                builder->str.elems[builder->str.len++] = c;
            }
        }
    }
    else
        pure_da_append_e(builder->str, c, code);
    return code;
}

PureString pure_sb_finish(PureStringBuilder *builder, PureErrorCode *outCode) {
    PURE_ASSERT(builder != NULL);
    PureErrorCode localCode = pure_sb_push(builder, '\0');
    if(outCode != NULL) *outCode = localCode;
    if(PURE_IS_ERROR(localCode)) return NULL;
    return builder->str.elems;
}

void pure_sb_clean(PureStringBuilder *builder) {
    PURE_ASSERT(builder != NULL);
    builder->str.len = 0;
}

void pure_sb_free(PureStringBuilder *builder) {
    PURE_ASSERT(builder != NULL);
    if(builder->str.elems != builder->staticBuffer)
        pure_da_free(builder->str);
    pure_sb_start_with(builder, builder->str.mem);
}