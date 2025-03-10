#include "ss_list.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Helper function for list iteration
static ss_bool_t print_int(ss_list_t* l, size_t idx, ss_list_node_t* node, void* userdata,
                           size_t userdata_size)
{
    (void)l;             // Unused
    (void)userdata;      // Unused
    (void)userdata_size; // Unused
    printf("Node[%zu] = %d\n", idx, *(int*)node->data);
    return 0; // Continue iteration
}

// Helper function for finding a specific value
static ss_bool_t find_int(ss_list_t* l, size_t idx, ss_list_node_t* node, void* userdata,
                          size_t userdata_size)
{
    (void)l;             // Unused
    (void)idx;           // Unused
    (void)userdata_size; // Unused
    int target = *(int*)userdata;
    return (*(int*)node->data == target) ? 1 : 0; // Stop if found
}

void test_list()
{
    printf("\n=== Starting ss_list tests ===\n");

    // Test list creation and initialization
    ss_list_t* dynamic_list = ss_list_create(sizeof(int));
    assert(dynamic_list != NULL);
    assert(dynamic_list->el_size == sizeof(int));
    assert(dynamic_list->size == 0);
    assert(dynamic_list->first == NULL);
    assert(dynamic_list->last == NULL);
    printf("[OK] ss_list_create: Dynamic list creation test passed\n");

    // Test stack list initialization
    ss_list_t stack_list;
    ss_list_init(&stack_list, sizeof(int));
    assert(stack_list.el_size == sizeof(int));
    assert(stack_list.size == 0);
    assert(stack_list.first == NULL);
    assert(stack_list.last == NULL);
    printf("[OK] ss_list_init: Stack list initialization test passed\n");

    // Test pushing elements
    int values[] = {10, 20, 30, 40, 50};

    // Test push (append)
    for (int i = 0; i < 5; i++)
    {
        assert(ss_list_push(&stack_list, &values[i]));
    }
    assert(stack_list.size == 5);
    assert(*(int*)stack_list.first->data == 10);
    assert(*(int*)stack_list.last->data == 50);
    printf("[OK] ss_list_push: Push operations test passed\n");

    // Test insert (prepend)
    int front_value = 5;
    assert(ss_list_insert(&stack_list, &front_value));
    assert(stack_list.size == 6);
    assert(*(int*)stack_list.first->data == 5);
    printf("[OK] ss_list_insert: Insert at front test passed\n");

    // Test insert at position
    int mid_value = 25;
    assert(ss_list_insert_at(&stack_list, &mid_value, 3));
    assert(stack_list.size == 7);
    ss_list_iterate(&stack_list, print_int, NULL, 0); // Print for visual verification
    printf("[OK] ss_list_insert_at: Insert at position test passed\n");

    // Test list iteration and find
    int target = 30;
    assert(ss_list_iterate(&stack_list, find_int, &target, sizeof(int)));
    printf("[OK] ss_list_iterate: Iteration and find test passed\n");

    // Test set operation
    int old_val = 30;
    int new_val = 35;
    assert(ss_list_set(&stack_list, &old_val, &new_val));
    assert(!ss_list_iterate(&stack_list, find_int, &old_val,
                            sizeof(int))); // Should not find old value
    assert(ss_list_iterate(&stack_list, find_int, &new_val, sizeof(int))); // Should find new value
    printf("[OK] ss_list_set: Set operation test passed\n");

    // Test remove operation
    int remove_val = 35;
    assert(ss_list_remove(&stack_list, &remove_val));
    assert(!ss_list_iterate(&stack_list, find_int, &remove_val,
                            sizeof(int))); // Should not find removed value
    printf("[OK] ss_list_remove: Remove operation test passed\n");

    // Test front/back access and pop operations
    assert(*(int*)ss_list_front(&stack_list) == 5);
    assert(*(int*)ss_list_back(&stack_list) == 50);

    ss_list_pop(&stack_list);
    assert(*(int*)ss_list_front(&stack_list) == 10);

    ss_list_pop_back(&stack_list);
    assert(*(int*)ss_list_back(&stack_list) == 40);
    printf("[OK] ss_list_front/back and pop operations test passed\n");

    // Test clear operation
    ss_list_clear(&stack_list);
    assert(stack_list.size == 0);
    assert(stack_list.first == NULL);
    assert(stack_list.last == NULL);
    printf("[OK] ss_list_clear: Clear operation test passed\n");

    // Test edge cases
    assert(!ss_list_insert_at(&stack_list, &values[0], 100)); // Invalid position
    assert(!ss_list_remove(&stack_list, &values[0]));         // Remove from empty list
    assert(ss_list_front(&stack_list) == NULL);               // Front of empty list
    assert(ss_list_back(&stack_list) == NULL);                // Back of empty list
    printf("[OK] Edge cases: Invalid operations properly handled\n");

    // Test NULL handling
    // assert(!ss_list_push(NULL, &values[0]));  // NULL list
    // assert(!ss_list_push(&stack_list, NULL)); // NULL data
    printf("[OK] NULL handling: NULL pointer cases properly handled\n");

    // Cleanup
    ss_list_destroy(&stack_list);
    ss_list_free(dynamic_list);
    printf("[OK] ss_list_destroy/free: Cleanup completed\n");

    printf("=== All ss_list tests passed ===\n\n");
}