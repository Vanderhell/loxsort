#include "lox_internal.h"

size_t lox_checked_multiply_size(size_t left, size_t right, size_t *product)
{
    if (left != 0u && right > SIZE_MAX / left) {
        return 0u;
    }
    if (product != NULL) {
        *product = left * right;
    }
    return 1u;
}

void *lox_element_address(void *base, size_t index, size_t element_size)
{
    return (void *)((unsigned char *)base + (index * element_size));
}

const void *lox_const_element_address(const void *base, size_t index, size_t element_size)
{
    return (const void *)((const unsigned char *)base + (index * element_size));
}

void lox_copy_bytes(void *dest, const void *src, size_t size)
{
    if (size != 0u) {
        memcpy(dest, src, size);
    }
}

void lox_swap_bytes(void *left, void *right, size_t size)
{
    unsigned char buffer[256];
    unsigned char *a = (unsigned char *)left;
    unsigned char *b = (unsigned char *)right;

    while (size != 0u) {
        size_t chunk = size < sizeof(buffer) ? size : sizeof(buffer);
        memcpy(buffer, a, chunk);
        memcpy(a, b, chunk);
        memcpy(b, buffer, chunk);
        a += chunk;
        b += chunk;
        size -= chunk;
    }
}

int lox_ranges_overlap(const void *left, size_t left_size, const void *right, size_t right_size)
{
    uintptr_t l0;
    uintptr_t l1;
    uintptr_t r0;
    uintptr_t r1;

    if (left_size == 0u || right_size == 0u) {
        return 0;
    }

    l0 = (uintptr_t)left;
    r0 = (uintptr_t)right;
    l1 = l0 + left_size;
    r1 = r0 + right_size;

    return (l0 < r1) && (r0 < l1);
}

