#include <stdlib.h>
#include <stdalign.h>
#include <stddef.h>

#include <pure/definitions.h>
#include <pure/core/memory.h>

PureAllocator defaultAllocator = INITIAL_DEFAULT_ALLOCATOR;

PureErrorCode pure_da_reserve_raw(PureAnonArray *da, size_t amount, size_t elemSize) {
    PURE_ASSERT(da != NULL && elemSize != 0);

    if(PURE_DA_GROWTH_FACTOR > da->growthFactor)
        da->growthFactor = PURE_DA_GROWTH_FACTOR;

    if(da->mem == NULL)
        da->mem = &defaultAllocator;

    if(da->cap >= amount)
        return PURE_SUCCESS_CODE;

    size_t newCap = (da->cap == 0) ? PURE_DA_INITIAL_CAP : da->cap;
    while(newCap < amount)
        newCap *= da->growthFactor;

    void *tmp = da->mem->realloc(da->elems, newCap * elemSize);
    if(tmp == NULL)
        return PURE_ERROR_ENOMEM;

    da->elems = tmp;
    da->cap = newCap;
    return PURE_SUCCESS_CODE;
}

PureErrorCode pure_da_append_raw(PureAnonArray *da, void *elem, size_t elemSize) {
    PURE_ASSERT(da != NULL && elem != NULL && elemSize != 0);

    PureErrorCode code = pure_da_reserve_raw(da, da->len + 1, elemSize);
    if(PURE_IS_SUCCESS(code)) {
        ByteStream target = ((ByteStream) da->elems) + da->len * elemSize;
        memcpy((void *) target, elem, elemSize);
        da->len++;
    }
    return code;
}

PureErrorCode pure_da_extend_raw(PureAnonArray *da, PureAnonSlice *slice, size_t elemSize) {
    PURE_ASSERT(da != NULL && slice != NULL && slice->len != 0 && elemSize != 0);

    PureErrorCode code = pure_da_reserve_raw(da, da->len + slice->len, elemSize);
    if(PURE_IS_SUCCESS(code)) {
        ByteStream target = ((ByteStream) da->elems) + da->len * elemSize;
        memcpy((void *) target, slice->elems, slice->len * elemSize);
        da->len += slice->len;
    }
    return code;
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
            code = pure_da_reserve(daString, builder->str.len + 1);
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
        code = pure_da_append(builder->str, c);
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