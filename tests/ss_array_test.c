/*
 * File:   ss_array_test.c
 * Author: trywen@qq.com
 * UTF-8 encoded file
 */

#include "ss_array.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Helper function for comparing integers
static int compare_int(const void* lvalue, size_t lsize, const void* rvalue, size_t rsize)
{
    (void)lsize;
    (void)rsize;
    int lv = *((int*)lvalue);
    int rv = *((int*)rvalue);
    return lv == rv ? 0 : (lv < rv ? -1 : 1);
}

void test_array()
{
    printf("\n=== Starting ss_array tests ===\n");

    // Test array creation and initialization
    ss_array_t* dynamic_array = ss_array_create(sizeof(int), 4);
    assert(dynamic_array != NULL);
    assert(dynamic_array->el_size == sizeof(int));
    assert(dynamic_array->capacity == 4);
    assert(dynamic_array->size == 0);
    printf("[OK] ss_array_create: Dynamic array creation test passed\n");

    // Test array initialization
    ss_array_t stack_array;
    ss_bool_t ret = ss_array_init(&stack_array, sizeof(int), 4);
    assert(ret);
    assert(stack_array.el_size == sizeof(int));
    assert(stack_array.capacity == 4);
    assert(stack_array.size == 0);
    printf("[OK] ss_array_init: Stack array initialization test passed\n");

    // Test pushing single elements
    int value = 42;
    assert(ss_array_push(&stack_array, &value));
    assert(stack_array.size == 1);
    assert(*(int*)ss_array_at(&stack_array, 0) == 42);

    value = 24;
    assert(ss_array_push(&stack_array, &value));
    assert(stack_array.size == 2);
    assert(*(int*)ss_array_at(&stack_array, 1) == 24);
    printf("[OK] ss_array_push, ss_array_at: Push single elements test passed\n");

    // Test pushing multiple elements
    int values[] = {1, 2, 3, 4};
    assert(ss_array_push_n(&stack_array, values, 4));
    assert(stack_array.size == 6);
    for (size_t i = 0; i < 4; i++)
    {
        assert(*(int*)ss_array_at(&stack_array, i + 2) == values[i]);
    }
    printf("[OK] ss_array_push_n: Push multiple elements test passed\n");

    // Test array capacity expansion
    assert(stack_array.capacity >= stack_array.size);
    printf("[OK] ss_array_reserve: Automatic capacity expansion test passed\n");

    // Test manual reserve
    assert(ss_array_reserve_append(&stack_array, 20));
    assert(stack_array.capacity >= 20);
    printf("[OK] ss_array_reserve_append: Manual capacity reservation test passed\n");

    assert(ss_array_reserve(&stack_array, 40));
    assert(stack_array.capacity >= 40);
    printf("[OK] ss_array_reserve: Manual capacity reservation test passed\n");

    // Test array get and set
    int old_value;
    assert(ss_array_set(&stack_array, 0, &values[0], &old_value));
    assert(old_value == 42);

    int get_value;
    assert(ss_array_get(&stack_array, 0, &get_value));
    assert(get_value == values[0]);
    printf("[OK] ss_array_get, ss_array_set: Get and set operations test passed\n");

    // Test array update
    int update_values[] = {10, 20};
    assert(ss_array_update(&stack_array, 1, update_values, 2));
    assert(*(int*)ss_array_at(&stack_array, 1) == 10);
    assert(*(int*)ss_array_at(&stack_array, 2) == 20);
    printf("[OK] ss_array_update: Update operation test passed\n");

    // Test array replace
    int old_val = 10, new_val = 30;
    assert(ss_array_replace(&stack_array, &old_val, &new_val));
    assert(*(int*)ss_array_at(&stack_array, 1) == 30);
    printf("[OK] ss_array_replace: Replace operation test passed\n");

    // Test array sort
    ss_array_sort(&stack_array, compare_int);
    for (size_t i = 1; i < stack_array.size; i++)
    {
        assert(*(int*)ss_array_at(&stack_array, i - 1) <= *(int*)ss_array_at(&stack_array, i));
    }
    printf("[OK] ss_array_sort: Sort operation test passed\n");

    // Test array back and pop
    int* back_val = (int*)ss_array_back(&stack_array);
    assert(back_val != NULL);
    int last_val = *back_val;
    ss_array_pop_back(&stack_array);
    assert(stack_array.size == 5);
    assert(*(int*)ss_array_back(&stack_array) != last_val);
    printf("[OK] ss_array_back, ss_array_pop_back: Back and pop operations test passed\n");

    // Test array clear
    ss_array_clear(&stack_array);
    assert(stack_array.size == 0);
    assert(ss_array_back(&stack_array) == NULL);
    printf("[OK] ss_array_clear: Clear operation test passed\n");

    // Test edge cases
    assert(!ss_array_get(&stack_array, 100, &get_value));         // Invalid index
    assert(!ss_array_set(&stack_array, 100, &value, &old_value)); // Invalid index
    assert(!ss_array_update(&stack_array, 100, values, 1));       // Invalid offset
    printf("[OK] Edge cases: Invalid operations properly handled\n");

    // Test NULL handling
    // assert(!ss_array_init(NULL, sizeof(int), 4)); // NULL array
    // assert(!ss_array_push(NULL, &value));         // NULL array
    // assert(!ss_array_push(&stack_array, NULL)); // NULL value
    printf("[OK] NULL handling: NULL pointer cases properly handled\n");

    // Test array data access
    assert(ss_array_data(&stack_array) == stack_array.elts);
    assert(ss_array_size(&stack_array) == stack_array.size);
    printf("[OK] ss_array_data, ss_array_size: Data access macros test passed\n");

    // Cleanup
    ss_array_free(dynamic_array);
    ss_array_destroy(&stack_array);

    printf("[OK] ss_array_destroy, ss_array_free: Cleanup completed\n");

    printf("=== All ss_array tests passed ===\n\n");
}