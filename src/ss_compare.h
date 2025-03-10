/**
 * @file ss_compare.h
 * @brief Comparison functions implementation
 * @author trywen@qq.com
 * @date 2011-09-02
 *
 * Provides comparison functions for various data types including:
 * - Memory block comparison
 * - String comparison (case-sensitive/insensitive)
 * - Primitive type comparisons
 */

#ifndef SS_COMPARE_H
#define SS_COMPARE_H

#include "ss_slice.h"
#include "ss_types.h"

#include "ss_csl_adapter.h"

#include <string.h>
#ifdef _WIN32
#ifndef strcasecmp
#define strcasecmp _stricmp // Windows
#endif
#else
#include <strings.h> // Linux/POSIX
#endif

/**
 * Defines comparison functions
 */

/**
 * @brief Character comparison function
 * @param lvalue Pointer to left character
 * @param lsize Unused parameter (interface consistency)
 * @param rvalue Pointer to right character
 * @param rsize Unused parameter (interface consistency)
 * @return Negative if left < right, 0 if equal, positive if left > right
 */
static inline int ss_compare_char(const void* lvalue, size_t lsize, const void* rvalue,
                                  size_t rsize)
{
    (void)lsize;
    (void)rsize;
    char lv = *((char*)lvalue);
    char rv = *((char*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

/**
 * @brief Unsigned character comparison function
 * @param lvalue Pointer to left unsigned char
 * @param lsize Unused parameter (interface consistency)
 * @param rvalue Pointer to right unsigned char
 * @param rsize Unused parameter (interface consistency)
 * @return Negative if left < right, 0 if equal, positive if left > right
 */
static inline int ss_compare_uchar(const void* lvalue, size_t lsize, const void* rvalue,
                                   size_t rsize)
{
    (void)lsize;
    (void)rsize;
    unsigned char lv = *((unsigned char*)lvalue);
    unsigned char rv = *((unsigned char*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

/**
 * @brief Short integer comparison function
 * @param lvalue Pointer to left short integer
 * @param lsize Unused parameter (interface consistency)
 * @param rvalue Pointer to right short integer
 * @param rsize Unused parameter (interface consistency)
 * @return Negative if left < right, 0 if equal, positive if left > right
 */
static inline int ss_compare_short(const void* lvalue, size_t lsize, const void* rvalue,
                                   size_t rsize)
{
    (void)lsize;
    (void)rsize;
    short lv = *((short*)lvalue);
    short rv = *((short*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

/**
 * @brief Unsigned short integer comparison function
 * @param lvalue Pointer to left unsigned short
 * @param lsize Unused parameter (interface consistency)
 * @param rvalue Pointer to right unsigned short
 * @param rsize Unused parameter (interface consistency)
 * @return Negative if left < right, 0 if equal, positive if left > right
 */
static inline int ss_compare_ushort(const void* lvalue, size_t lsize, const void* rvalue,
                                    size_t rsize)
{
    (void)lsize;
    (void)rsize;
    unsigned short lv = *((unsigned short*)lvalue);
    unsigned short rv = *((unsigned short*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

static inline int ss_compare_int(const void* lvalue, size_t lsize, const void* rvalue, size_t rsize)
{
    (void)lsize;
    (void)rsize;
    int lv = *((int*)lvalue);
    int rv = *((int*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

static inline int ss_compare_uint(const void* lvalue, size_t lsize, const void* rvalue,
                                  size_t rsize)
{
    (void)lsize;
    (void)rsize;
    unsigned int lv = *((unsigned int*)lvalue);
    unsigned int rv = *((unsigned int*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

static inline int ss_compare_long(const void* lvalue, size_t lsize, const void* rvalue,
                                  size_t rsize)
{
    (void)lsize;
    (void)rsize;
    long lv = *((long*)lvalue);
    long rv = *((long*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

static inline int ss_compare_ulong(const void* lvalue, size_t lsize, const void* rvalue,
                                   size_t rsize)
{
    (void)lsize;
    (void)rsize;
    unsigned long lv = *((unsigned long*)lvalue);
    unsigned long rv = *((unsigned long*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

/**
 * @brief Generic memory comparison function
 * @param lvalue Pointer to left operand
 * @param lsize Size of left operand
 * @param rvalue Pointer to right operand
 * @param rsize Size of right operand
 * @return Negative if l < r, zero if equal, positive if l > r
 */
/**
 * @brief Generic memory block comparison
 * @param lvalue Pointer to left memory block
 * @param lsize Size of left memory block
 * @param rvalue Pointer to right memory block
 * @param rsize Size of right memory block
 * @return Negative if left < right, 0 if equal, positive if left > right
 *
 * Compares memory blocks with different size handling:
 * - Equal size: direct memcmp
 * - Different sizes: compare common prefix then size
 */
static inline int ss_compare_mem(const void* lvalue, size_t lsize, const void* rvalue, size_t rsize)
{
    if (lsize == rsize)
    {
        if (lvalue == rvalue)
        {
            return 0;
        }
        return memcmp(lvalue, rvalue, lsize);
    }
    if (lsize < rsize)
    {
        int cmprs = memcmp(lvalue, rvalue, lsize);
        return cmprs != 0 ? cmprs : -1;
    }
    int cmprs = memcmp(lvalue, rvalue, rsize);
    return cmprs != 0 ? cmprs : 1;
}

static inline int ss_compare_mem_ptr(const void* lvalue, size_t lsize, const void* rvalue,
                                     size_t rsize)
{
    (void)lsize;
    (void)rsize;
    return ss_compare_mem(*((const void**)lvalue), 0, *((const void**)rvalue), 0);
}

static inline int ss_compare_mem_case(const void* lvalue, size_t lsize, const void* rvalue,
                                      size_t rsize)
{
    if (lsize == rsize)
    {
        if (lvalue == rvalue)
        {
            return 0;
        }
        return ss_csl_memicmp(lvalue, rvalue, lsize);
    }
    if (lsize < rsize)
    {
        int cmprs = ss_csl_memicmp(lvalue, rvalue, lsize);
        return cmprs != 0 ? cmprs : -1;
    }
    int cmprs = ss_csl_memicmp(lvalue, rvalue, rsize);
    return cmprs != 0 ? cmprs : 1;
}

static inline int ss_compare_mem_case_ptr(const void* lvalue, size_t lsize, const void* rvalue,
                                          size_t rsize)
{
    (void)lsize;
    (void)rsize;
    return ss_compare_mem_case(*((const void**)lvalue), 0, *((const void**)rvalue), 0);
}

static inline int ss_compare_string(const void* lvalue, size_t lsize, const void* rvalue,
                                    size_t rsize)
{
    (void)lsize;
    (void)rsize;
    if (lvalue == rvalue)
    {
        return 0;
    }
    return strcmp((const char*)lvalue, (const char*)rvalue);
}

static inline int ss_compare_string_case(const void* lvalue, size_t lsize, const void* rvalue,
                                         size_t rsize)
{
    (void)lsize;
    (void)rsize;
    if (lvalue == rvalue)
    {
        return 0;
    }
    return strcasecmp((const char*)lvalue, (const char*)rvalue);
}

static inline int ss_compare_string_ptr(const void* lvalue, size_t lsize, const void* rvalue,
                                        size_t rsize)
{
    (void)lsize;
    (void)rsize;
    return ss_compare_string(*((const char**)lvalue), 0, *((const char**)rvalue), 0);
}

static inline int ss_compare_string_case_ptr(const void* lvalue, size_t lsize, const void* rvalue,
                                             size_t rsize)
{
    (void)lsize;
    (void)rsize;
    return ss_compare_string_case(*((const char**)lvalue), 0, *((const char**)rvalue), 0);
}

static inline int ss_compare_ptr(const void* lvalue, size_t lsize, const void* rvalue, size_t rsize)
{
    (void)lsize;
    (void)rsize;
    unsigned long lv = *((unsigned long*)lvalue);
    unsigned long rv = *((unsigned long*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

// ss_slice_t
static inline int ss_compare_data(const void* lvalue, size_t lsize, const void* rvalue,
                                  size_t rsize)
{
    (void)lsize;
    (void)rsize;
    ss_slice_t* left = (ss_slice_t*)lvalue;
    ss_slice_t* right = (ss_slice_t*)rvalue;
    return ss_compare_mem(left->data, left->size, right->data, right->size);
}

// ss_slice_t
inline static int ss_compare_data_case(const void* lvalue, size_t lsize, const void* rvalue,
                                       size_t rsize)
{
    (void)lsize;
    (void)rsize;
    ss_slice_t* left = (ss_slice_t*)lvalue;
    ss_slice_t* right = (ss_slice_t*)rvalue;
    return ss_compare_mem_case(left->data, left->size, right->data, right->size);
}

#endif /* SS_COMPARE_H */
