/**
 * @file ss_types.h
 * @brief Common type definitions and data structures
 * @author trywen@qq.com
 * @date 2011-08-19
 *
 * Contains fundamental type aliases, data structure definitions
 * and basic constants used throughout the library.
 */

#ifndef SS_TYPES_H
#define SS_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include "ss_version.h"

/** @brief Slice type for memory buffer operations */
typedef struct ss_slice_s ss_slice_t;
/** @brief Key-Value pair container */
typedef struct ss_entry_s ss_entry_t;
/** @brief Dynamic array container */
typedef struct ss_array_s ss_array_t;
/** @brief String manipulation structure */
typedef struct ss_string_s ss_string_t;
/** @brief Ordered binary tree container */
typedef struct ss_obtree_s ss_obtree_t;
/** @brief Node structure for ordered binary tree */
typedef struct ss_obtree_node_s ss_obtree_node_t;
/** @brief Hash map container */
typedef struct ss_hashmap_s ss_hashmap_t;
/** @brief Linked list container */
typedef struct ss_list_s ss_list_t;
/** @brief Node structure for linked list */
typedef struct ss_list_node_s ss_list_node_t;
/** @brief Large bit set implementation */
typedef struct ss_bigbitset_s ss_bigbitset_t;
/** @brief Compact bit array implementation */
typedef struct ss_bitarray_s ss_bitarray_t;

/* Boolean type definition */
/**
 * @brief Boolean type definition
 * @typedef ss_bool_t
 * Platform-independent boolean type
 */
typedef bool ss_bool_t;

/**
 * @struct ss_entry_s
 * @brief Key-Value pair container
 *
 * @var key Pointer to key data
 * @var ksize Size of key data in bytes
 * @var value Pointer to value data
 * @var vsize Size of value data in bytes
 *
 * Used for storing generic key-value pairs in hash maps and other containers
 */
struct ss_entry_s
{
    void* key;    /**< Pointer to key data */
    size_t ksize; /**< Size of key data in bytes */
    void* value;  /**< Pointer to value data */
    size_t vsize; /**< Size of value data in bytes */
};

/* Defines hash function type, returns hash value */
/**
 * @typedef ss_hash_f
 * @brief Hash function prototype
 * @param value Pointer to data to hash
 * @param size Size of data in bytes
 * @return Computed hash value
 */
typedef size_t (*ss_hash_f)(const void* value, size_t size);

/**
 * @typedef ss_compare_f
 * @brief Comparison function prototype
 * @param lvalue Pointer to left operand
 * @param lsize Size of left operand
 * @param rvalue Pointer to right operand
 * @param rsize Size of right operand
 * @return Negative if l < r, zero if equal, positive if l > r
 */
typedef int (*ss_compare_f)(const void* lvalue, size_t lsize, const void* rvalue, size_t rsize);

#define SS_TRUE true
#define SS_FALSE false

/* Pointer size */
#define SS_PTR_SIZE (sizeof(void*))
/* Default array capacity */
#define SS_DEFAULT_ARRAY_CAPACITY 2
/* Default hashmap bucket count */
#define SS_DEFAULT_HASHMAP_BUCKETS 64

#define SS_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SS_MAX(a, b) ((a) > (b) ? (a) : (b))

/**
 * @brief Debug print function for entry structure
 * @param tag Prefix text for debug output
 * @param entry Pointer to the entry to display
 *
 * Prints key and value of entry in ASCII representation
 */
static inline void ss_entry_debug(const char* tag, const ss_entry_t* entry)
{
    size_t i;
    printf("%s", tag);
    const char* k = (const char*)entry->key;
    for (i = 0; i < entry->ksize; i++)
    {
        printf("%c", k[i]);
    }
    printf("=");
    const char* v = (const char*)entry->value;
    for (i = 0; i < entry->vsize; i++)
    {
        printf("%c", v[i]);
    }
}

#endif /* SS_TYPES_H */
