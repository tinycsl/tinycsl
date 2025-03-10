/**
 * @file   ss_hashmap.h
 * @author trywen@qq.com
 * @date   2011-09-02
 * @brief  Dynamic hash table implementation with open addressing
 *
 * Supports custom hash functions and key comparison. Features include:
 * - Automatic resizing based on load factor
 * - Separate chaining collision resolution
 * - Key-value pair storage with arbitrary data types
 * - O(1) average case for basic operations
 */

#ifndef SS_HASHMAP_H
#define SS_HASHMAP_H

#include "ss_types.h"

#include "ss_array.h"
#include "ss_obtree.h"

#include "ss_compare.h"
#include "ss_hash.h"

typedef ss_obtree_t ss_hashmap_bucket;

/**
 * @struct ss_hashmap_s
 * @brief Main hash table container structure
 *
 * @var buckets Array of bucket pointers (separate chaining)
 * @var size Total number of stored key-value pairs
 * @var bnum Current bucket count (capacity)
 * @var hash Function pointer for key hashing
 * @var compare Function pointer for key comparison
 */
struct ss_hashmap_s
{
    ss_hashmap_bucket** buckets; ///< Array of bucket pointers (separate chaining)
    size_t size;                 ///< Total number of stored key-value pairs
    uint32_t bnum;               ///< Current bucket count (capacity)

    ss_hash_f hash;       ///< Function pointer for key hashing
    ss_compare_f compare; ///< Function pointer for key comparison
};

/* If returns true, iteration will stop */
typedef ss_bool_t (*ss_hashmap_iterate_cb_f)(ss_hashmap_t* map, ss_entry_t* entry, void* param);

/**
 * @brief Initialize hashmap with specified parameters
 * @param[in] map Pointer to hashmap structure
 * @param[in] bnum Initial number of buckets
 * @param[in] hash Hash function for keys
 * @param[in] compare Key comparison function
 * @return SS_TRUE if initialization succeeded
 */
ss_bool_t ss_hashmap_init(ss_hashmap_t* map, uint32_t bnum, ss_hash_f hash, ss_compare_f compare);
void ss_hashmap_destroy(ss_hashmap_t* map);

/**
 * @brief Create new hashmap instance
 * @param[in] bnum Initial bucket count
 * @param[in] hash Hash function for keys
 * @param[in] compare Key comparison function
 * @return Newly allocated hashmap pointer
 * @note Caller must free with ss_hashmap_free()
 */
ss_hashmap_t* ss_hashmap_create(uint32_t bnum, ss_hash_f hash, ss_compare_f compare);
void ss_hashmap_free(ss_hashmap_t* map);

/**
 * @brief Insert or update key-value pair
 * @param[in] map Hashmap pointer
 * @param[in] key Pointer to key data
 * @param[in] ksize Key data size in bytes
 * @param[in] value Pointer to value data (may be NULL)
 * @param[in] vsize Value data size in bytes
 * @note Updates value if key exists, creates new entry otherwise
 * @warning Key pointer must not be NULL and size must be greater than 0
 */
void ss_hashmap_put(ss_hashmap_t* map, const void* key, size_t ksize, const void* value,
                    size_t vsize);
/**
 * @brief Retrieve value associated with key
 * @param[in] map Hashmap pointer
 * @param[in] key Pointer to key data
 * @param[in] ksize Key data size in bytes
 * @param[out] vsize Pointer to receive value size (may be NULL)
 * @return Pointer to value data, NULL if not found
 * @note Returned pointer remains valid until next structural modification
 */
void* ss_hashmap_get(ss_hashmap_t* map, const void* key, size_t ksize, size_t* vsize);
ss_bool_t ss_hashmap_remove(ss_hashmap_t* map, const void* key, size_t ksize);
/** Returns key list in keys array, key type is ss_binary_t with element width sizeof(ss_binary_t)
 */
ss_array_t* ss_hashmap_keys(ss_hashmap_t* map, ss_array_t* keys);

// param: user data for callback. Returns TRUE to stop iteration
ss_bool_t ss_hashmap_iterate(ss_hashmap_t* map, ss_hashmap_iterate_cb_f cb, void* param);

void ss_hashmap_clear(ss_hashmap_t* map);

#define ss_hashmap_size(map) ((map)->size)

#endif /* SS_HASHMAP_H */
