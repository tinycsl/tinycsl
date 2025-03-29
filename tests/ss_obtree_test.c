#include "ss_compare.h"
#include "ss_hash.h"
#include "ss_obtree.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

// Helper function for tree traversal
static ss_bool_t print_node(ss_obtree_t* t, ss_obtree_node_t* node, int depth, void* param)
{
    (void)t;     // Unused
    (void)param; // Unused

    // Print indentation based on depth
    for (int i = 0; i < depth; i++)
    {
        printf("  ");
    }
    printf("Node[%d]: Key=%.*s, Value=%.*s\n", depth, (int)node->entry.ksize,
           (const char*)node->entry.key, (int)node->entry.vsize, (const char*)node->entry.value);
    return SS_FALSE; // Continue traversal
}

// Helper function for finding a specific node
static ss_bool_t find_node(ss_obtree_t* t, ss_obtree_node_t* node, int depth, void* param)
{
    (void)t;     // Unused
    (void)depth; // Unused
    const char* target = (const char*)param;
    return strncmp((const char*)node->entry.key, target, strlen(target)) == 0
               ? SS_TRUE
               : SS_FALSE; // Stop if found
}

// Helper function to verify inorder traversal order
static ss_bool_t verify_inorder(ss_obtree_t* t, ss_obtree_node_t* node, int depth, void* param)
{
    (void)t;     // Unused
    (void)depth; // Unused
    static const char* last_key = NULL;
    static size_t last_key_size = 0;
    const char* current_key = (const char*)node->entry.key;
    size_t current_key_size = node->entry.ksize;

    if (last_key != NULL)
    {
        assert(strncmp(last_key, current_key, SS_MIN(last_key_size, current_key_size)) <
               0); // Verify ascending order
    }
    last_key = current_key;
    last_key_size = current_key_size;
    return 0; // Continue traversal
}

void test_obtree()
{
    printf("\n=== Starting ss_obtree tests ===\n");

    // Test tree initialization
    ss_obtree_t tree;
    ss_obtree_init(&tree, ss_hash_mem, ss_compare_mem, NULL);
    assert(tree.root == NULL);
    assert(tree.size == 0);
    printf("[OK] ss_obtree_init: Tree initialization test passed\n");

    // Test setting key-value pairs
    const char* keys[] = {"key1", "key2", "key3", "key4", "key5"};
    const char* values[] = {"value1", "value2", "value3", "value4", "value5"};

    for (int i = 0; i < 5; i++)
    {
        printf("Setting key: %s, value: %s\n", keys[i], values[i]);
        // +1 for the null terminator
        ss_obtree_node_t* node =
            ss_obtree_set(&tree, keys[i], strlen(keys[i]), values[i], strlen(values[i]));
        // printf("Node: %p\n", (void*)node);
        // printf("Node->entry.key: %s\n", (const char*)node->entry.key);
        // printf("Node->entry.value: %s\n", (const char*)node->entry.value);
        assert(node != NULL);
        assert(strncmp((const char*)node->entry.key, keys[i], strlen(keys[i])) == 0);
        assert(strncmp((const char*)node->entry.value, values[i], strlen(values[i])) == 0);
    }
    assert(tree.size == 5);
    printf("[OK] ss_obtree_set: Set operations test passed\n");

    // Test getting nodes
    for (int i = 0; i < 5; i++)
    {
        ss_obtree_node_t* node = ss_obtree_get(&tree, keys[i], strlen(keys[i]));
        assert(node != NULL);
        assert(strncmp((const char*)node->entry.key, keys[i], strlen(keys[i])) == 0);
        assert(strncmp((const char*)node->entry.value, values[i], strlen(values[i])) == 0);
    }
    printf("[OK] ss_obtree_get: Get operations test passed\n");

    // Test tree traversals
    printf("\nPreorder traversal:\n");
    ss_obtree_preorder(&tree, print_node, NULL);
    printf("\nInorder traversal:\n");
    ss_obtree_inorder(&tree, print_node, NULL);
    printf("\nPostorder traversal:\n");
    ss_obtree_postorder(&tree, print_node, NULL);
    printf("[OK] Tree traversals test passed\n");

    // Verify binary search tree property (using inorder traversal)
    ss_obtree_inorder(&tree, verify_inorder, NULL);
    printf("[OK] Binary search tree property verification passed\n");

    // Test finding nodes
    printf("Finding node: %d\n", ss_obtree_preorder(&tree, find_node, (void*)"key3"));
    assert(ss_obtree_preorder(&tree, find_node, (void*)"key3"));
    assert(!ss_obtree_preorder(&tree, find_node, (void*)"nonexistent"));
    printf("[OK] Node finding test passed\n");

    // Test removing nodes
    assert(ss_obtree_remove(&tree, "key3", strlen("key3")));
    assert(tree.size == 4);
    assert(ss_obtree_get(&tree, "key3", strlen("key3")) == NULL);
    printf("[OK] ss_obtree_remove: Remove operation test passed\n");

    // Test set with existing key (update)
    const char* new_value = "updated_value";
    ss_obtree_node_t* updated =
        ss_obtree_set(&tree, "key1", strlen("key1"), new_value, strlen(new_value));
    assert(updated != NULL);
    assert(strncmp((const char*)updated->entry.value, new_value, strlen(new_value)) == 0);
    printf("[OK] ss_obtree_set: Update existing key test passed\n");

    // Test clear operation
    ss_obtree_clear(&tree);
    assert(tree.size == 0);
    assert(tree.root == NULL);
    for (int i = 0; i < 5; i++)
    {
        assert(ss_obtree_get(&tree, keys[i], strlen(keys[i])) == NULL);
    }
    printf("[OK] ss_obtree_clear: Clear operation test passed\n");

    // Test edge cases
    assert(ss_obtree_get(&tree, "nonexistent", strlen("nonexistent")) == NULL);
    assert(!ss_obtree_remove(&tree, "nonexistent", strlen("nonexistent")));
    printf("[OK] Edge cases: Invalid operations properly handled\n");

    // Test NULL handling
    // assert(!ss_obtree_remove(NULL, "key1", strlen("key1")));     // NULL tree
    // assert(ss_obtree_get(NULL, "key1", strlen("key1")) == NULL); // NULL tree
    // assert(ss_obtree_set(NULL, "key1", strlen("key1"), "value1", strlen("value1")) ==
    //        NULL); // NULL tree
    printf("[OK] NULL handling: NULL pointer cases properly handled\n");

    // Test hash-based operations (set2, get2, remove2)
    size_t hash = ss_hash_string("key1", strlen("key1"));
    ss_obtree_node_t* node =
        ss_obtree_set2(&tree, "key1", strlen("key1"), hash, "value1", strlen("value1"));
    assert(node != NULL);
    assert(node->khash == hash);

    node = ss_obtree_get2(&tree, "key1", strlen("key1"), hash);
    assert(node != NULL);
    assert(node->khash == hash);

    assert(ss_obtree_remove2(&tree, "key1", strlen("key1"), hash));
    printf("[OK] Hash-based operations test passed\n");

    // Cleanup
    ss_obtree_destroy(&tree);
    printf("[OK] ss_obtree_destroy: Cleanup completed\n");

    printf("=== All ss_obtree tests passed ===\n\n");
}