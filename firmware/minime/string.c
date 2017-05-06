#include <string.h>
#include <stdint.h>

#define likely(x) __builtin_expect((x), 1)
void *memcpy(void *dst, const void *src, size_t n)
{
    if (likely(!(((uintptr_t)dst) & 3) && !(((uintptr_t)src) & 3))) {
        // pointers aligned
        uint32_t *d = dst;
        const uint32_t *s = src;

        // copy words first
        for (size_t i = (n >> 2); i; i--) {
            *d++ = *s++;
        }

        if (n & 2) {
            // copy half-word
            *(uint16_t*)d = *(const uint16_t*)s;
            d = (uint32_t*)((uint16_t*)d + 1);
            s = (const uint32_t*)((const uint16_t*)s + 1);
        }

        if (n & 1) {
            // copy byte
            *((uint8_t*)d) = *((const uint8_t*)s);
        }
    } else {
        // unaligned access, copy bytes
        uint8_t *d = dst;
        const uint8_t *s = src;

        for (; n; n--) {
            *d++ = *s++;
        }
    }

    return dst;
}

void *memset(void *s, int c, size_t n)
{
    if (c == 0 && ((uintptr_t)s & 3) == 0) {
        // aligned store of 0
        uint32_t *s32 = s;
        for (size_t i = n >> 2; i > 0; i--) {
            *s32++ = 0;
        }
        if (n & 2) {
            *((uint16_t*)s32) = 0;
            s32 = (uint32_t*)((uint16_t*)s32 + 1);
        }
        if (n & 1) {
            *((uint8_t*)s32) = 0;
        }
    } else {
        uint8_t *s2 = s;
        for (; n > 0; n--) {
            *s2++ = c;
        }
    }
    return s;
}


