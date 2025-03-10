/**
 * @file ss_hash.h
 * @brief Hash functions implementation for various data types
 * @author trywen@qq.com
 * @date 2011-09-02
 *
 * Provides hash functions for:
 * - Primitive types (char, int, long, etc)
 * - Memory blocks with case-sensitive/insensitive options
 * - Strings with case-sensitive/insensitive options
 * - Pointer values
 *
 * All functions are inline for optimal performance.
 */

#ifndef SS_HASH_H
#define SS_HASH_H

#include "ss_csl_adapter.h"
#include "ss_types.h"
#include <ctype.h>

/**
 * @brief Hashes a character value
 * @param value Pointer to the character value
 * @param size Unused parameter (required for interface consistency)
 * @return Hash value as the character value
 */
static inline size_t ss_hash_char(const void* value, size_t size)
{
    (void)size;
    return *((char*)value);
}

/**
 * @brief Hashes an unsigned character value
 * @param value Pointer to the unsigned character value
 * @param size Unused parameter
 * @return Hash value as the unsigned character
 */
static inline size_t ss_hash_uchar(const void* value, size_t size)
{
    (void)size;
    return *((unsigned char*)value);
}

/**
 * @brief Hashes a short integer value
 * @param value Pointer to the short integer value
 * @param size Unused parameter (required for interface consistency)
 * @return Hash value as the short integer
 */
static inline size_t ss_hash_short(const void* value, size_t size)
{
    (void)size;
    return *((short*)value);
}

/**
 * @brief Hashes an unsigned short integer value
 * @param value Pointer to the unsigned short value
 * @param size Unused parameter (required for interface consistency)
 * @return Hash value as the unsigned short integer
 */
static inline size_t ss_hash_ushort(const void* value, size_t size)
{
    (void)size;
    return *((unsigned short*)value);
}

/**
 * @brief Hashes an integer value
 * @param value Pointer to the integer value
 * @param size Unused parameter (required for interface consistency)
 * @return Hash value as the integer
 */
static inline size_t ss_hash_int(const void* value, size_t size)
{
    (void)size;
    return *((int*)value);
}

/**
 * @brief Hashes an unsigned integer value
 * @param value Pointer to the unsigned integer value
 * @param size Unused parameter (required for interface consistency)
 * @return Hash value as the unsigned integer
 */
static inline size_t ss_hash_uint(const void* value, size_t size)
{
    (void)size;
    return *((unsigned int*)value);
}

/**
 * @brief Hashes a long integer value
 * @param value Pointer to the long integer value
 * @param size Unused parameter (required for interface consistency)
 * @return Hash value as the long integer
 */
static inline size_t ss_hash_long(const void* value, size_t size)
{
    (void)size;
    return *((long*)value);
}

/**
 * @brief Hashes an unsigned long integer value
 * @param value Pointer to the unsigned long value
 * @param size Unused parameter (required for interface consistency)
 * @return Hash value as the unsigned long integer
 */
static inline size_t ss_hash_ulong(const void* value, size_t size)
{
    (void)size;
    return *((unsigned long*)value);
}

/** Hash calculation for data blocks */
/**
 * @brief Hashes a memory block
 * @param value Pointer to memory block
 * @param size Size of memory block in bytes
 * @return Computed hash value using polynomial accumulation
 */
static inline size_t ss_hash_mem(const void* value, size_t size)
{
    const char* s = (const char*)value;
    size_t h = 0;
    size_t i;
    for (i = 0; i < size; i++)
    {
        h = 5 * h + s[i];
    }
    return h;
}

static inline size_t ss_hash_mem_ptr(const void* value, size_t size)
{
    (void)size;
    return ss_hash_mem(*((const void**)value), size);
}

/**
 * @brief Case-insensitive memory block hashing
 * @param value Pointer to memory block
 * @param size Size of memory block in bytes
 * @return Computed hash value using uppercase conversion
 *
 * Converts characters to uppercase before hashing
 */
static inline size_t ss_hash_mem_case(const void* value, size_t size)
{
    const char* s = (const char*)value;
    size_t h = 0;
    size_t i;
    for (i = 0; i < size; i++)
    {
        h = 5 * h + ss_csl_toupper(s[i]);
    }
    return h;
}

static inline size_t ss_hash_mem_case_ptr(const void* value, size_t size)
{
    (void)size;
    return ss_hash_mem_case(*((const void**)value), size);
}

/** size parameter is meaningless */
static inline size_t ss_hash_string(const void* value, size_t size)
{
    const char* s = (const char*)value;
    size_t h = 0;
    size_t i = 0;

    if (size == 0)
    {
        // Use null terminator as end marker
        for (; *s; ++s)
        {
            h = 5 * h + *s;
        }
    }
    else
    {
        // Use either null terminator or size as end marker
        for (; i < size && *s; ++i, ++s)
        {
            h = 5 * h + *s;
        }
    }
    return h;
}

/**
 * value is a pointer to string (pointer to pointer)
 * size parameter indicates pointer length (not actually used)
 */
/**
 * @brief Hashes a string pointer
 * @param value Pointer to string pointer
 * @param size Unused parameter
 * @return Hash value of the referenced string
 */
static inline size_t ss_hash_string_ptr(const void* value, size_t size)
{
    (void)size;
    return ss_hash_string(*((const char**)value), size);
}

/** size parameter is meaningless */
static inline size_t ss_hash_string_case(const void* value, size_t size)
{
    const char* s = (const char*)value;
    size_t h = 0;
    size_t i = 0;

    if (size == 0)
    {
        // Use null terminator as end marker
        for (; *s; ++s)
        {
            h = 5 * h + ss_csl_toupper(*s);
        }
    }
    else
    {
        // Use either null terminator or size as end marker
        for (; i < size && *s; ++i, ++s)
        {
            h = 5 * h + ss_csl_toupper(*s);
        }
    }
    return h;
}

/**
 * @brief Case-insensitive hash for string pointer
 * @param value Pointer to string pointer
 * @param size Unused parameter
 * @return Case-insensitive hash of the referenced string
 */
static inline size_t ss_hash_string_case_ptr(const void* value, size_t size)
{
    (void)size;
    return ss_hash_string_case(*((const char**)value), size);
}

static inline size_t ss_hash_ptr(const void* value, size_t size)
{
    (void)size;
    return *((unsigned long*)value);
}

#endif /* SS_HASH_H */
