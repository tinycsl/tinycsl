#include "ss_bitarray.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_bitarray()
{
    printf("\n=== Starting ss_bitarray tests ===\n");

    // Test array creation with different bit widths
    // Note: Support for other bit widths requires either:
    // 1. Modifying ss_bitarray_value_t in ss_bitarray.h, or
    // 2. Defining ss_bitarray_value_t at compile time
    // int test_sizes[] = {1, 4, 8, 16, 32, 64};
    int test_sizes[] = {1, 4, 8};
    int num_elements = 10;

    for (int i = 0; i < sizeof(test_sizes) / sizeof(test_sizes[0]); i++)
    {
        int bit_width = test_sizes[i];
        ss_bitarray_t* array = ss_bitarray_create(num_elements, bit_width);

        assert(array != NULL);
        assert(array->num_elements == num_elements);
        assert(array->bit_width == bit_width);
        assert(array->total_bits == num_elements * bit_width);
        assert(array->data != NULL);

        ss_bitarray_free(array);
        printf("[OK] Creation test passed for %d-bit width\n", bit_width);
    }

    // Test basic set/get operations
    ss_bitarray_t* array = ss_bitarray_create(num_elements, 8); // 8-bit elements
    assert(array != NULL);

    // Test setting values
    for (int i = 0; i < num_elements; i++)
    {
        ss_bitarray_set(array, i, i * 2); // Set even numbers
    }

    // Test getting values
    for (int i = 0; i < num_elements; i++)
    {
        assert(ss_bitarray_get(array, i) == (ss_bitarray_value_t)(i * 2));
    }
    printf("[OK] Basic set/get operations test passed\n");

    // Test value bounds for different bit widths
    ss_bitarray_free(array);

    // Test for 4-bit width (max value: 15)
    array = ss_bitarray_create(5, 4);
    assert(array != NULL);

    ss_bitarray_set(array, 0, 0);  // Min value
    ss_bitarray_set(array, 1, 15); // Max value
    ss_bitarray_set(array, 2, 7);  // Middle value
    ss_bitarray_set(array, 3, 10); // Random value
    ss_bitarray_set(array, 4, 3);  // Small value

    assert(ss_bitarray_get(array, 0) == 0);
    assert(ss_bitarray_get(array, 1) == 15);
    assert(ss_bitarray_get(array, 2) == 7);
    assert(ss_bitarray_get(array, 3) == 10);
    assert(ss_bitarray_get(array, 4) == 3);
    printf("[OK] 4-bit value bounds test passed\n");

    // Test for 1-bit width (boolean values)
    ss_bitarray_free(array);
    array = ss_bitarray_create(8, 1);
    assert(array != NULL);

    // Set alternating pattern
    for (int i = 0; i < 8; i++)
    {
        ss_bitarray_set(array, i, i % 2);
    }

    // Verify pattern
    for (int i = 0; i < 8; i++)
    {
        assert(ss_bitarray_get(array, i) == (ss_bitarray_value_t)(i % 2));
    }
    printf("[OK] 1-bit boolean values test passed\n");

    // Test edge cases
    ss_bitarray_t* edge_array = ss_bitarray_create(0, 8); // Zero elements
    assert(edge_array == NULL);

    edge_array = ss_bitarray_create(10, 0); // Zero bit width
    assert(edge_array == NULL);

    edge_array = ss_bitarray_create(10, 65); // Exceeding max bit width
    assert(edge_array == NULL);
    printf("[OK] Edge cases test passed\n");

    // Test printing (visual verification)
    printf("\nPrinting 4-bit array contents:\n");
    ss_bitarray_print(array);

    // Cleanup
    ss_bitarray_free(array);
    printf("[OK] Cleanup completed\n");

    printf("=== All ss_bitarray tests passed ===\n\n");
}