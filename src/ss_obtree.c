#include "ss_alloc.h"
#include "ss_obtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ss_obtree_node_t* _ss_obtree_node_new(const void* key, size_t ksize, size_t khash,
                                             const void* data, size_t dsize)
{
    ss_obtree_node_t* node = (ss_obtree_node_t*)ss_malloc(sizeof(ss_obtree_node_t));
    if (!node)
    {
        return NULL;
    }
    memset(node, 0, sizeof(ss_obtree_node_t));
    node->entry.key = ss_malloc(ksize);
    if (!node->entry.key)
    {
        ss_free(node);
        return NULL;
    }
    memcpy(node->entry.key, key, ksize);
    node->entry.ksize = ksize;
    node->khash = khash;

    if (data && dsize > 0)
    {
        node->entry.value = ss_malloc(dsize);
        if (!node->entry.value)
        {
            ss_free(node->entry.key);
            ss_free(node);
            return NULL;
        }
        memcpy(node->entry.value, data, dsize);
        node->entry.vsize = dsize;
    }
    else
    {
        node->entry.value = NULL;
        node->entry.vsize = 0;
    }

    return node;
}

void _ss_obtree_node_free(ss_obtree_node_t* node)
{
    if (node->entry.value)
    {
        ss_free(node->entry.value);
    }
    ss_free(node->entry.key);
    ss_free(node);
}

int _ss_obtree_node_compare(ss_obtree_t* t, ss_obtree_node_t* node, const void* key, size_t ksize,
                            size_t khash)
{
    int cmprs;
    if (khash < node->khash)
    {
        cmprs = -1;
    }
    else if (khash > node->khash)
    {
        cmprs = 1;
    }
    else
    {
        cmprs = t->key_compare(key, ksize, node->entry.key, node->entry.ksize);
    }
    return cmprs;
}

// Returns true with found node in 'node' parameter, false with last traversed node
ss_bool_t _ss_obtree_node_find(ss_obtree_t* t, const void* key, size_t ksize, size_t khash,
                               ss_obtree_node_t** retnode, int* cmprs)
{
    if (!t->root)
    {
        return SS_FALSE;
    }
    ss_obtree_node_t* node = t->root;
    while (SS_TRUE)
    {
        *cmprs = _ss_obtree_node_compare(t, node, key, ksize, khash);
        if (*cmprs == 0)
        {
            // Found match, return
            *retnode = node;
            return SS_TRUE;
        }
        else if (*cmprs < 0)
        {
            *retnode = node;
            node = node->left;
            if (!node)
            { // Reached leaf node
                break;
            }
        }
        else
        {
            *retnode = node;
            node = node->right;
            if (!node)
            { // Reached leaf node
                break;
            }
        }
    }
    return SS_FALSE;

    /*

        while(SS_TRUE){
            if(node){
                *cmprs = khash == node->khash ? 0:(khash < node->khash ? -1 : 1);
                if(*cmprs == 0){
                    if((!node->left || node->left->khash != khash) && (!node->right ||
       node->right->khash != khash)){ printf("find, key:%d, node->key:%d, node->left->key:%d,
       node->right->key:%d\n", *((int*)(key)), *((int*)(node->key)),
       node->left?*((int*)(node->left->key)):-1, node->right?*((int*)(node->right->key)):-1);
                        // Found, return
                        *retnode = node;
                        return SS_TRUE;
                    }else{
                        *cmprs = t->compare(key, ksize, node->key, node->ksize);
                        printf("*cmprs:%d\n", *cmprs);
                        if(*cmprs == 0){
                            // Found, return
                            *retnode = node;
                            return SS_TRUE;
                        }else if(*cmprs < 0){
                            *retnode = node;
                            node = node->left;
                            if(!node){// Reached leaf node
                                break;
                            }
                        }else{ //cmprs > 0
                            *retnode = node;
                            node = node->right;
                            if(!node){// Reached leaf node
                                break;
                            }
                        }
                    }
                }else if(*cmprs < 0){
                    *retnode = node;
                    node = node->left;
                    if(!node){// Reached leaf node
                        break;
                    }
                }else{ //khash > node->khash
                    *retnode = node;
                    node = node->right;
                    if(!node){// Reached leaf node
                        break;
                    }
                }
            }else{
                break;
            }
        }
        return SS_FALSE;
    */
}

ss_bool_t _ss_obtree_node_data_replace(ss_obtree_t* t, ss_obtree_node_t* node, const void* data,
                                       size_t dsize)
{
    if (!data)
    {
        if (node->entry.value)
        {
            ss_free(node->entry.value);
            node->entry.value = NULL;
            node->entry.vsize = 0;
        }
    }
    else if (!node->entry.value)
    {
        void* ptr = ss_malloc(dsize);
        if (ptr)
        {
            node->entry.value = ptr;
            memcpy(node->entry.value, data, dsize);
            node->entry.vsize = dsize;
        }
        else
        {
            return SS_FALSE;
        }
    }
    else if (!t->val_compare ||
             t->val_compare(node->entry.value, node->entry.vsize, data, dsize) != 0)
    {
        if (node->entry.vsize >= dsize)
        {
            memcpy(node->entry.value, data, dsize);
            node->entry.vsize = dsize;
        }
        else
        {
            // void* ptr = realloc(node->entry.value, dsize);
            void* ptr = ss_malloc(dsize);
            if (ptr)
            {
                ss_free(node->entry.value);
                node->entry.value = ptr;
                memcpy(node->entry.value, data, dsize);
                node->entry.vsize = dsize;
            }
            else
            {
                return SS_FALSE;
            }
        }
    }
    return SS_TRUE;
}

// Query and set node: create new if not found, otherwise update existing value
ss_obtree_node_t* _ss_obtree_node_find_and_set(ss_obtree_t* t, const void* key, size_t ksize,
                                               size_t khash, const void* data, size_t dsize)
{
    ss_obtree_node_t* retnode = NULL;
    int cmprs = 0;
    if (_ss_obtree_node_find(t, key, ksize, khash, &retnode, &cmprs))
    {
        _ss_obtree_node_data_replace(t, retnode, data, dsize);
        return retnode;
    }
    else
    {
        ss_obtree_node_t* newnode = _ss_obtree_node_new(key, ksize, khash, data, dsize);
        if (newnode)
        {
            if (cmprs < 0)
            {
                retnode->left = newnode;
            }
            else
            {
                retnode->right = newnode;
            }
            newnode->parent = retnode;
            t->size++;
        }
        return newnode;
    }
}

ss_obtree_node_t* _ss_obtree_right_leaf_find(ss_obtree_node_t* node)
{
    while (node->right)
    {
        node = node->right;
    }
    return node;
}

ss_obtree_node_t* _ss_obtree_left_leaf_find(ss_obtree_node_t* node)
{
    while (node->left)
    {
        node = node->left;
    }
    return node;
}

void ss_obtree_init(ss_obtree_t* t, ss_hash_f key_hash, ss_compare_f key_compare,
                    ss_compare_f val_compare)
{
    memset(t, 0, sizeof(ss_obtree_t));
    // t->pool = pool;
    t->key_hash = key_hash;
    t->key_compare = key_compare;
    t->val_compare = val_compare;
}

ss_obtree_node_t* ss_obtree_set(ss_obtree_t* t, const void* key, size_t ksize, const void* data,
                                size_t dsize)
{
    return ss_obtree_set2(t, key, ksize, t->key_hash(key, ksize), data, dsize);
}

ss_obtree_node_t* ss_obtree_set2(ss_obtree_t* t, const void* key, size_t ksize, size_t khash,
                                 const void* data, size_t dsize)
{
    if (t->root)
    {
        return _ss_obtree_node_find_and_set(t, key, ksize, khash, data, dsize);
    }
    else
    {
        t->root = _ss_obtree_node_new(key, ksize, khash, data, dsize);
        t->size++;
        return t->root;
    }
}

ss_obtree_node_t* ss_obtree_get(ss_obtree_t* t, const void* key, size_t ksize)
{
    return ss_obtree_get2(t, key, ksize, t->key_hash(key, ksize));
}

ss_obtree_node_t* ss_obtree_get2(ss_obtree_t* t, const void* key, size_t ksize, size_t khash)
{
    ss_obtree_node_t* retnode;
    int cmprs;
    if (_ss_obtree_node_find(t, key, ksize, khash, &retnode, &cmprs))
    {
        return retnode;
    }
    return NULL;
}
ss_bool_t ss_obtree_remove(ss_obtree_t* t, const void* key, size_t ksize)
{
    return ss_obtree_remove2(t, key, ksize, t->key_hash(key, ksize));
}

ss_bool_t ss_obtree_remove2(ss_obtree_t* t, const void* key, size_t ksize, size_t khash)
{
    ss_obtree_node_t* retnode;
    int cmprs;
    if (_ss_obtree_node_find(t, key, ksize, khash, &retnode, &cmprs))
    {
        return ss_obtree_node_remove(t, retnode);
    }
    return SS_FALSE;
}

ss_bool_t ss_obtree_node_remove(ss_obtree_t* t, ss_obtree_node_t* node)
{
    if (!t->root)
    {
        return SS_FALSE;
    }
    if (node == t->root)
    { // Delete root node
        if (node->left)
        {
            t->root = node->left;
            t->root->parent = NULL;
            if (node->right)
            {
                // If right subtree exists, attach to rightmost leaf of left subtree
                // Return rightmost leaf node of left subtree
                ss_obtree_node_t* leaf = _ss_obtree_right_leaf_find(node->left);
                leaf->right = node->right;
                node->right->parent = leaf;
            }
        }
        else
        {
            t->root = node->right;
            if (t->root) t->root->parent = NULL;
        }
    }
    else
    {
        // Is left child node
        if (node == node->parent->left)
        {
            if (node->right)
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
                if (node->left)
                {
                    // If right subtree exists, attach to rightmost leaf of left subtree
                    // Return rightmost leaf node of left subtree
                    ss_obtree_node_t* leaf = _ss_obtree_left_leaf_find(node->right);
                    leaf->left = node->left;
                    node->left->parent = leaf;
                }
            }
            else
            {
                node->parent->left = node->left;
                if (node->left) node->left->parent = node->parent;
            }
        }
        else
        { // Is right child node
            if (node->left)
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
                if (node->right)
                {
                    // If right subtree exists, attach to rightmost leaf of left subtree
                    // Return rightmost leaf node of left subtree
                    ss_obtree_node_t* leaf = _ss_obtree_right_leaf_find(node->left);
                    leaf->right = node->right;
                    node->right->parent = leaf;
                }
            }
            else
            {
                node->parent->right = node->right;
                if (node->right) node->right->parent = node->parent;
            }
        }
    }
    _ss_obtree_node_free(node);
    t->size--;
    return SS_TRUE;
}

ss_bool_t _ss_obtree_preorder(ss_obtree_t* t, ss_obtree_node_t* node, int* depth,
                              ss_obtree_iterate_cb_f it, void* param)
{
    if (node)
    {
        if (it(t, node, *depth, param))
        {
            return SS_TRUE;
        }
        (*depth)++;
        if (_ss_obtree_preorder(t, node->left, depth, it, param))
        {
            return SS_TRUE;
        }
        if (_ss_obtree_preorder(t, node->right, depth, it, param))
        {
            return SS_TRUE;
        }
        (*depth)--;
    }
    return SS_FALSE;
}

ss_bool_t ss_obtree_preorder(ss_obtree_t* t, ss_obtree_iterate_cb_f it, void* param)
{
    if (!t->root)
    {
        return SS_FALSE;
    }
    int depth = 0;
    return _ss_obtree_preorder(t, t->root, &depth, it, param);
}

ss_bool_t _ss_obtree_inorder(ss_obtree_t* t, ss_obtree_node_t* node, int* depth,
                             ss_obtree_iterate_cb_f it, void* param)
{
    if (node)
    {
        (*depth)++;
        _ss_obtree_inorder(t, node->left, depth, it, param);
        (*depth)--;
        if (it(t, node, *depth, param))
        {
            return SS_TRUE;
        }
        (*depth)++;
        _ss_obtree_inorder(t, node->right, depth, it, param);
        (*depth)--;
    }
    return SS_FALSE;
}

ss_bool_t ss_obtree_inorder(ss_obtree_t* t, ss_obtree_iterate_cb_f it, void* param)
{
    if (!t->root)
    {
        return SS_FALSE;
    }
    int depth = 0;
    return _ss_obtree_inorder(t, t->root, &depth, it, param);
}

ss_bool_t _ss_obtree_postorder(ss_obtree_t* t, ss_obtree_node_t* node, int* depth,
                               ss_obtree_iterate_cb_f it, void* param)
{
    if (node)
    {
        (*depth)++;
        _ss_obtree_postorder(t, node->left, depth, it, param);
        _ss_obtree_postorder(t, node->right, depth, it, param);
        (*depth)--;
        if (it(t, node, *depth, param))
        {
            return SS_TRUE;
        }
    }
    return SS_FALSE;
}

ss_bool_t ss_obtree_postorder(ss_obtree_t* t, ss_obtree_iterate_cb_f it, void* param)
{
    if (!t->root)
    {
        return SS_FALSE;
    }
    int depth = 0;
    return _ss_obtree_postorder(t, t->root, &depth, it, param);
}

/*
ss_bool_t ss_obtree_left_iterate(ss_obtree_t* t, ss_obtree_iterate_cb_f it){


}


ss_bool_t ss_obtree_right_iterate(ss_obtree_t* t, ss_obtree_iterate_cb_f it){

}
*/

ss_bool_t _ss_obtree_clear_iterate_cb(ss_obtree_t* t, ss_obtree_node_t* node, int depth,
                                      void* param)
{
    (void)t;
    (void)depth;
    (void)param;
    _ss_obtree_node_free(node);
    return SS_TRUE;
}

void ss_obtree_clear(ss_obtree_t* t)
{
    ss_obtree_postorder(t, _ss_obtree_clear_iterate_cb, NULL);
    t->root = NULL;
    t->size = 0;
}

void ss_obtree_destroy(ss_obtree_t* t) { ss_obtree_clear(t); }
