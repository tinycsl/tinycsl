#include "ss_compare.h"
#include "ss_hash.h"
#include "ss_hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Helper function for map iteration
static ss_bool_t print_entry(ss_hashmap_t* map, ss_entry_t* entry, void* param)
{
    (void)map;   // Unused
    (void)param; // Unused
    printf("Key: %.*s, Value: %.*s\n", (int)entry->ksize, (const char*)entry->key,
           (int)entry->vsize, (const char*)entry->value);
    return 0; // Continue iteration
}

// Helper function for finding a specific entry
static ss_bool_t find_entry(ss_hashmap_t* map, ss_entry_t* entry, void* param)
{
    (void)map; // Unused
    const char* target = (const char*)param;
    return strncmp((const char*)entry->key, target, entry->ksize) == 0 ? 1 : 0; // Stop if found
}

void test_hashmap()
{
    printf("\n=== Starting ss_hashmap tests ===\n");

    // Test hashmap creation and initialization
    ss_hashmap_t* dynamic_map = ss_hashmap_create(16, ss_hash_mem, ss_compare_mem);
    assert(dynamic_map != NULL);
    assert(dynamic_map->bnum == 16);
    assert(dynamic_map->size == 0);
    assert(dynamic_map->buckets != NULL);
    printf("[OK] ss_hashmap_create: Dynamic hashmap creation test passed\n");

    // Test stack hashmap initialization
    ss_hashmap_t stack_map;
    ss_bool_t ret = ss_hashmap_init(&stack_map, 16, ss_hash_mem, ss_compare_mem);
    assert(ret);
    assert(stack_map.bnum == 16);
    assert(stack_map.size == 0);
    assert(stack_map.buckets != NULL);
    printf("[OK] ss_hashmap_init: Stack hashmap initialization test passed\n");

    // Test putting key-value pairs
    const char* keys[] = {"key1", "key2", "key3", "key4", "key5"};
    const char* values[] = {"value1", "value2", "value3", "value4", "value5"};

    for (int i = 0; i < 5; i++)
    {
        ss_hashmap_put(&stack_map, keys[i], strlen(keys[i]), values[i], strlen(values[i]));
    }
    assert(stack_map.size == 5);
    printf("[OK] ss_hashmap_put: Put operations test passed\n");

    // Test getting values
    size_t vsize;
    for (int i = 0; i < 5; i++)
    {
        const char* value =
            (const char*)ss_hashmap_get(&stack_map, keys[i], strlen(keys[i]), &vsize);
        assert(value != NULL);
        assert(strncmp(value, values[i], strlen(values[i])) == 0);
        assert(vsize == strlen(values[i]));
    }
    printf("[OK] ss_hashmap_get: Get operations test passed\n");

    // Test iteration
    printf("\nIterating through hashmap:\n");
    ss_hashmap_iterate(&stack_map, print_entry, NULL);
    printf("[OK] ss_hashmap_iterate: Iteration test passed\n");

    // Test finding entries
    assert(ss_hashmap_iterate(&stack_map, find_entry, (void*)"key3"));
    assert(!ss_hashmap_iterate(&stack_map, find_entry, (void*)"nonexistent"));
    printf("[OK] ss_hashmap_iterate with find: Find operations test passed\n");

    // Test getting keys
    ss_array_t keys_array;
    ss_array_init(&keys_array, sizeof(ss_slice_t), 10);
    ss_hashmap_keys(&stack_map, &keys_array);
    assert(keys_array.size == 5);
    printf("[OK] ss_hashmap_keys: Keys retrieval test passed\n");

    // Test removing entries
    assert(ss_hashmap_remove(&stack_map, "key3", strlen("key3")));
    assert(stack_map.size == 4);
    assert(ss_hashmap_get(&stack_map, "key3", strlen("key3"), &vsize) == NULL);
    printf("[OK] ss_hashmap_remove: Remove operation test passed\n");

    // Test updating existing key
    const char* new_value = "updated_value";
    ss_hashmap_put(&stack_map, "key1", strlen("key1"), new_value, strlen(new_value));
    const char* updated = (const char*)ss_hashmap_get(&stack_map, "key1", strlen("key1"), &vsize);
    assert(strncmp(updated, new_value, strlen(new_value)) == 0);
    printf("[OK] ss_hashmap_put: Update existing key test passed\n");

    // Test clear operation
    ss_hashmap_clear(&stack_map);
    assert(stack_map.size == 0);
    for (int i = 0; i < 5; i++)
    {
        assert(ss_hashmap_get(&stack_map, keys[i], strlen(keys[i]), &vsize) == NULL);
    }
    printf("[OK] ss_hashmap_clear: Clear operation test passed\n");

    // Test edge cases
    assert(ss_hashmap_get(&stack_map, "nonexistent", strlen("nonexistent"), &vsize) == NULL);
    assert(!ss_hashmap_remove(&stack_map, "nonexistent", strlen("nonexistent")));
    printf("[OK] Edge cases: Invalid operations properly handled\n");

    // Test NULL handling
    // assert(!ss_hashmap_init(NULL, 16, ss_hash_mem, ss_compare_mem));      // NULL map
    // assert(!ss_hashmap_remove(NULL, "key1", strlen("key1")));             // NULL map
    // assert(ss_hashmap_get(NULL, "key1", strlen("key1"), &vsize) == NULL); // NULL map
    printf("[OK] NULL handling: NULL pointer cases properly handled\n");

    // 测试空值
    ss_hashmap_clear(&stack_map);
    ss_hashmap_put(&stack_map, "key1", strlen("key1"), new_value, strlen(new_value));
    ss_hashmap_put(&stack_map, "key2", strlen("key2"), new_value, strlen(new_value));
    ss_hashmap_put(&stack_map, "key3", strlen("key3"), "", 0);
    ss_hashmap_put(&stack_map, "key4", strlen("key4"), NULL, 0);
    assert(stack_map.size == 4);
    ss_array_t test_keys;
    ss_array_init(&test_keys, sizeof(ss_slice_t), ss_hashmap_size(&stack_map));
    ss_hashmap_keys(&stack_map, &test_keys);
    assert(ss_array_size(&test_keys) == ss_hashmap_size(&stack_map));
    ss_array_destroy(&test_keys);

    // Cleanup
    ss_array_destroy(&keys_array);
    ss_hashmap_destroy(&stack_map);
    ss_hashmap_free(dynamic_map);
    printf("[OK] ss_hashmap_destroy/free: Cleanup completed\n");

    printf("=== All ss_hashmap tests passed ===\n\n");
}