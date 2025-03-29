/**
 * @file ss_obtree.h
 * @brief Ordered binary tree implementation
 * @author trywen@qq.com
 * @date 2011-08-30
 *
 * Implements an ordered binary tree structure with key-value pairs.
 * Supports insertion, deletion and various traversal methods.
 */

#ifndef SS_OBTREE_H
#define SS_OBTREE_H

#include "ss_types.h"

/**
 * @struct ss_obtree_s
 * @brief Ordered binary tree container structure
 *
 * @var root Pointer to root node of the tree
 * @var size Total number of nodes in the tree
 * @var hash Hash function for key hashing
 * @var compare Key comparison function
 */

struct ss_obtree_s
{
    ss_obtree_node_t* root; // Root node
    size_t size;            // Node count

    ss_hash_f key_hash;
    ss_compare_f key_compare;
    ss_compare_f val_compare; // 可以为空，如果为空，ss_obtree_set操作时将不会比较值是否相等
};

struct ss_obtree_node_s
{
    ss_obtree_node_t* parent;
    ss_obtree_node_t* left;
    ss_obtree_node_t* right;
    ss_entry_t entry;
    size_t khash;
};

/* If returns true, stop the traversal */
typedef ss_bool_t (*ss_obtree_iterate_cb_f)(ss_obtree_t* t, ss_obtree_node_t* node, int depth,
                                            void* param);

void ss_obtree_init(ss_obtree_t* t, ss_hash_f key_hash, ss_compare_f key_compare,
                    ss_compare_f val_compare);
void ss_obtree_destroy(ss_obtree_t* t);

ss_obtree_node_t* ss_obtree_set(ss_obtree_t* t, const void* key, size_t ksize, const void* data,
                                size_t dsize);
ss_obtree_node_t* ss_obtree_set2(ss_obtree_t* t, const void* key, size_t ksize, size_t khash,
                                 const void* data, size_t dsize);
ss_obtree_node_t* ss_obtree_get(ss_obtree_t* t, const void* key, size_t ksize);
ss_obtree_node_t* ss_obtree_get2(ss_obtree_t* t, const void* key, size_t ksize, size_t khash);
ss_bool_t ss_obtree_remove(ss_obtree_t* t, const void* key, size_t ksize);
ss_bool_t ss_obtree_remove2(ss_obtree_t* t, const void* key, size_t ksize, size_t khash);

ss_bool_t ss_obtree_node_remove(ss_obtree_t* t, ss_obtree_node_t* node);

ss_bool_t ss_obtree_preorder(ss_obtree_t* t, ss_obtree_iterate_cb_f it,
                             void* param); // Preorder traversal
ss_bool_t ss_obtree_inorder(ss_obtree_t* t, ss_obtree_iterate_cb_f it,
                            void* param); // Inorder traversal
ss_bool_t ss_obtree_postorder(ss_obtree_t* t, ss_obtree_iterate_cb_f it,
                              void* param); // Postorder traversal

void ss_obtree_clear(ss_obtree_t* t);

#endif /* SS_OBTREE_H */
