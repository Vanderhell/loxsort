#include "../lox_internal.h"

static void lox_intro_heap_sift_down(
    unsigned char *base,
    size_t start,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user)
{
    size_t root = start;

    while ((root * 2u) + 1u < count) {
        size_t child = (root * 2u) + 1u;
        size_t swap_index = root;

        if (compare(
                lox_const_element_address(base, swap_index, element_size),
                lox_const_element_address(base, child, element_size),
                compare_user) < 0) {
            swap_index = child;
        }
        if (child + 1u < count &&
            compare(
                lox_const_element_address(base, swap_index, element_size),
                lox_const_element_address(base, child + 1u, element_size),
                compare_user) < 0) {
            swap_index = child + 1u;
        }
        if (swap_index == root) {
            return;
        }
        lox_swap_bytes(
            lox_element_address(base, root, element_size),
            lox_element_address(base, swap_index, element_size),
            element_size);
        root = swap_index;
    }
}

static void lox_intro_heapsort(
    unsigned char *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user)
{
    size_t start;

    if (count < 2u) {
        return;
    }

    start = (count - 2u) / 2u;
    while (1u) {
        lox_intro_heap_sift_down(base, start, count, element_size, compare, compare_user);
        if (start == 0u) {
            break;
        }
        --start;
    }

    while (count > 1u) {
        --count;
        lox_swap_bytes(
            lox_element_address(base, 0u, element_size),
            lox_element_address(base, count, element_size),
            element_size);
        lox_intro_heap_sift_down(base, 0u, count, element_size, compare, compare_user);
    }
}

static size_t lox_intro_depth_limit(size_t count)
{
    size_t depth = 0u;
    while (count > 1u) {
        count >>= 1u;
        ++depth;
    }
    return depth * 2u;
}

static void lox_intro_sort_range(
    unsigned char *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    size_t depth_limit)
{
    const size_t insertion_cutoff = 16u;

    while (count > insertion_cutoff) {
        size_t lo = 0u;
        size_t hi = count - 1u;
        size_t mid = count / 2u;
        size_t lt = 0u;
        size_t i = 1u;
        size_t gt = hi;
        int c0;
        int c1;
        int c2;

        if (depth_limit == 0u) {
            lox_intro_heapsort(base, count, element_size, compare, compare_user);
            return;
        }
        --depth_limit;

        c0 = compare(
            lox_const_element_address(base, lo, element_size),
            lox_const_element_address(base, mid, element_size),
            compare_user);
        c1 = compare(
            lox_const_element_address(base, mid, element_size),
            lox_const_element_address(base, hi, element_size),
            compare_user);
        c2 = compare(
            lox_const_element_address(base, lo, element_size),
            lox_const_element_address(base, hi, element_size),
            compare_user);

        if (c0 > 0) {
            lox_swap_bytes(
                lox_element_address(base, lo, element_size),
                lox_element_address(base, mid, element_size),
                element_size);
        }
        if (c1 > 0) {
            lox_swap_bytes(
                lox_element_address(base, mid, element_size),
                lox_element_address(base, hi, element_size),
                element_size);
        }
        if (c2 > 0) {
            lox_swap_bytes(
                lox_element_address(base, lo, element_size),
                lox_element_address(base, hi, element_size),
                element_size);
        }

        lox_swap_bytes(
            lox_element_address(base, lo, element_size),
            lox_element_address(base, mid, element_size),
            element_size);

        while (i <= gt) {
            int cmp = compare(
                lox_const_element_address(base, i, element_size),
                lox_const_element_address(base, lo, element_size),
                compare_user);
            if (cmp < 0) {
                ++lt;
                lox_swap_bytes(
                    lox_element_address(base, lt, element_size),
                    lox_element_address(base, i, element_size),
                    element_size);
                ++i;
            } else if (cmp > 0) {
                lox_swap_bytes(
                    lox_element_address(base, i, element_size),
                    lox_element_address(base, gt, element_size),
                    element_size);
                if (gt == 0u) {
                    break;
                }
                --gt;
            } else {
                ++i;
            }
        }

        lox_swap_bytes(
            lox_element_address(base, lo, element_size),
            lox_element_address(base, lt, element_size),
            element_size);

        if (lt < count - gt - 1u) {
            lox_intro_sort_range(base, lt, element_size, compare, compare_user, depth_limit);
            base = lox_element_address(base, gt + 1u, element_size);
            count -= gt + 1u;
        } else {
            lox_intro_sort_range(
                lox_element_address(base, gt + 1u, element_size),
                count - gt - 1u,
                element_size,
                compare,
                compare_user,
                depth_limit);
            count = lt;
        }
    }

    if (count > 1u) {
        lox_insertion_sort(base, count, element_size, compare, compare_user, NULL);
    }
}

void lox_intro_sort(
    void *base,
    size_t count,
    size_t element_size,
    lox_compare_fn compare,
    void *compare_user,
    const lox_sort_options_t *options)
{
    size_t depth_limit = lox_intro_depth_limit(count);
    (void)options;
    if (count > 1u) {
        lox_intro_sort_range((unsigned char *)base, count, element_size, compare, compare_user, depth_limit);
    }
}

