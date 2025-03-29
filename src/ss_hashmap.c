
#include "ss_alloc.h"
#include "ss_hashmap.h"
#include "ss_slice.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ss_bool_t _ss_hashmap_obtree_names_iterate_cb(ss_hashmap_t* map, ss_entry_t* entry, void* param);

ss_bool_t ss_hashmap_init(ss_hashmap_t* map, uint32_t bnum, ss_hash_f hash, ss_compare_f compare)
{
    // map->buckets = (ss_hashmap_bucket**)calloc(bnum , sizeof(ss_hashmap_bucket*));
    int size = bnum * sizeof(ss_hashmap_bucket*);
    void* buckets = ss_malloc(size);
    if (!buckets)
    {
        return SS_FALSE;
    }
    memset(buckets, 0, size);
    map->buckets = (ss_hashmap_bucket**)buckets;
    map->bnum = bnum;
    map->size = 0;
    map->hash = hash;
    map->compare = compare;

    return SS_TRUE;
}

void ss_hashmap_destroy(ss_hashmap_t* map)
{
    uint32_t i;
    for (i = 0; i < map->bnum; i++)
    {
        ss_hashmap_bucket* bucket = map->buckets[i];
        if (bucket)
        {
            ss_obtree_destroy(bucket);
            ss_free(bucket);
            map->buckets[i] = NULL;
        }
    }
    ss_free(map->buckets);
}

ss_hashmap_t* ss_hashmap_create(uint32_t bnum, ss_hash_f hash, ss_compare_f compare)
{
    ss_hashmap_t* map = (ss_hashmap_t*)ss_malloc(sizeof(ss_hashmap_t));
    if (!map)
    {
        return NULL;
    }
    if (!ss_hashmap_init(map, bnum, hash, compare))
    {
        ss_free(map);
        return NULL;
    }
    return map;
}

void ss_hashmap_free(ss_hashmap_t* map)
{
    ss_hashmap_destroy(map);
    ss_free(map);
}

void ss_hashmap_put(ss_hashmap_t* map, const void* key, size_t ksize, const void* value,
                    size_t vsize)
{
    size_t khash = map->hash(key, ksize);
    uint32_t bidx = khash % map->bnum;
    ss_hashmap_bucket* bucket = map->buckets[bidx];
    if (bucket)
    {
        size_t bktsize = bucket->size;
        ss_obtree_set2(bucket, key, ksize, khash, value, vsize);
        if (bucket->size > bktsize)
        {
            map->size++;
        }
    }
    else
    {
        bucket = (ss_hashmap_bucket*)ss_malloc(sizeof(ss_hashmap_bucket));
        ss_obtree_init(bucket, map->hash, map->compare, NULL);
        ss_obtree_set2(bucket, key, ksize, khash, value, vsize);
        map->size++;
        map->buckets[bidx] = bucket;
    }
}

void* ss_hashmap_get(ss_hashmap_t* map, const void* key, size_t ksize, size_t* vsize)
{
    size_t khash = map->hash(key, ksize);
    uint32_t bidx = khash % map->bnum;
    ss_hashmap_bucket* bucket = map->buckets[bidx];
    if (bucket && bucket->root)
    {
        ss_obtree_node_t* node = ss_obtree_get2(bucket, key, ksize, khash);
        if (node)
        {
            if (vsize)
            {
                *vsize = node->entry.vsize;
            }
            return node->entry.value;
        }
    }
    return NULL;
}

ss_bool_t ss_hashmap_remove(ss_hashmap_t* map, const void* key, size_t ksize)
{
    size_t khash = map->hash(key, ksize);
    uint32_t bidx = khash % map->bnum;
    ss_hashmap_bucket* bucket = map->buckets[bidx];
    if (bucket)
    {
        if (ss_obtree_remove2(bucket, key, ksize, khash))
        {
            map->size--;
            return SS_TRUE;
        }
    }
    return SS_FALSE;
}

ss_array_t* ss_hashmap_keys(ss_hashmap_t* map, ss_array_t* keys)
{
    ss_hashmap_iterate(map, _ss_hashmap_obtree_names_iterate_cb, keys);
    return keys;
}

ss_bool_t _ss_hashmap_obtree_names_iterate_cb(ss_hashmap_t* map, ss_entry_t* entry, void* param)
{
    (void)map;
    ss_array_t* keys = (ss_array_t*)param;
    ss_slice_t key;
    key.data = entry->key;
    key.size = entry->ksize;
    ss_array_push(keys, &key);
    return SS_FALSE;
}

ss_bool_t _ss_hashmap_bucket_iterate(ss_hashmap_t* map, ss_obtree_t* t, ss_obtree_node_t* node,
                                     ss_hashmap_iterate_cb_f it, void* param)
{
    if (node)
    {
        if (it(map, &node->entry, param))
        {
            return SS_TRUE;
        }
        _ss_hashmap_bucket_iterate(map, t, node->left, it, param);
        _ss_hashmap_bucket_iterate(map, t, node->right, it, param);
    }
    return SS_FALSE;
}

ss_bool_t ss_hashmap_iterate(ss_hashmap_t* map, ss_hashmap_iterate_cb_f it, void* param)
{
    uint32_t i;
    for (i = 0; i < map->bnum; i++)
    {
        ss_hashmap_bucket* bucket = map->buckets[i];
        if (bucket && bucket->root)
        {
            if (_ss_hashmap_bucket_iterate(map, bucket, bucket->root, it, param))
            {
                return SS_TRUE;
            }
        }
    }
    return SS_FALSE;
}

void ss_hashmap_clear(ss_hashmap_t* map)
{
    uint32_t i;
    for (i = 0; i < map->bnum; i++)
    {
        ss_hashmap_bucket* bucket = map->buckets[i];
        if (bucket)
        {
            ss_obtree_clear(bucket);
        }
    }
    map->size = 0;
}
