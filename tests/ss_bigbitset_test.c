#include "ss_bigbitset.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_bigbitset()
{
    printf("\n=== Starting ss_bigbitset tests ===\n");

    // Test bitset creation
    int bit_count = 1000; // Test with a large number of bits
    ss_bigbitset_t* bitset = ss_bigbitset_create(bit_count);
    assert(bitset != NULL);
    assert(bitset->bits != NULL);
    assert(bitset->bits_size == bit_count);
    assert(bitset->bytes_size == (bit_count + 7) / 8); // Ceiling of bit_count/8
    printf("[OK] ss_bigbitset_create: Creation test passed\n");

    // Test setting individual bits
    int test_positions[] = {0, 1, 8, 9, 16, 17, 42, 99, 100, 500, 999};
    int num_positions = sizeof(test_positions) / sizeof(test_positions[0]);

    for (int i = 0; i < num_positions; i++)
    {
        ss_bigbitset_set(bitset, test_positions[i]);
        assert(ss_bigbitset_check(bitset, test_positions[i]));
    }
    printf("[OK] ss_bigbitset_set: Set operations test passed\n");

    // Test checking bits
    for (int i = 0; i < num_positions; i++)
    {
        assert(ss_bigbitset_check(bitset, test_positions[i]));
    }
    // Check some unset bits
    assert(!ss_bigbitset_check(bitset, 2));
    assert(!ss_bigbitset_check(bitset, 7));
    assert(!ss_bigbitset_check(bitset, 15));
    printf("[OK] ss_bigbitset_check: Check operations test passed\n");

    // Test clearing individual bits
    for (int i = 0; i < num_positions; i++)
    {
        ss_bigbitset_clear(bitset, test_positions[i]);
        assert(!ss_bigbitset_check(bitset, test_positions[i]));
    }
    printf("[OK] ss_bigbitset_clear: Clear operations test passed\n");

    // Test toggling bits
    for (int i = 0; i < num_positions; i++)
    {
        ss_bigbitset_toggle(bitset, test_positions[i]);
        assert(ss_bigbitset_check(bitset, test_positions[i]));
        ss_bigbitset_toggle(bitset, test_positions[i]);
        assert(!ss_bigbitset_check(bitset, test_positions[i]));
    }
    printf("[OK] ss_bigbitset_toggle: Toggle operations test passed\n");

    // Test clear all
    // First set some bits
    for (int i = 0; i < num_positions; i++)
    {
        ss_bigbitset_set(bitset, test_positions[i]);
    }
    // Then clear all
    ss_bigbitset_clear_all(bitset);
    // Verify all bits are cleared
    for (int i = 0; i < bit_count; i++)
    {
        assert(!ss_bigbitset_check(bitset, i));
    }
    printf("[OK] ss_bigbitset_clear_all: Clear all test passed\n");

    // Test boundary patterns
    // Set alternating bits
    for (int i = 0; i < bit_count; i += 2)
    {
        ss_bigbitset_set(bitset, i);
    }
    // Verify pattern
    for (int i = 0; i < bit_count; i++)
    {
        assert(ss_bigbitset_check(bitset, i) == (i % 2 == 0));
    }
    printf("[OK] Boundary pattern test passed\n");

    // Test edge cases
    ss_bigbitset_t* edge_bitset = ss_bigbitset_create(0); // Zero bits
    assert(edge_bitset == NULL);

    edge_bitset = ss_bigbitset_create(-1); // Negative bits
    assert(edge_bitset == NULL);
    printf("[OK] Edge cases test passed\n");

    // Test printing (visual verification)
    printf("\nPrinting first 50 bits with mod 10:\n");
    ss_bigbitset_print(bitset, 10, 50);

    // Test bit operations at byte boundaries
    int boundary_positions[] = {7, 8, 15, 16, 23, 24, 31, 32};
    int num_boundaries = sizeof(boundary_positions) / sizeof(boundary_positions[0]);

    ss_bigbitset_clear_all(bitset);
    for (int i = 0; i < num_boundaries; i++)
    {
        ss_bigbitset_set(bitset, boundary_positions[i]);
        assert(ss_bigbitset_check(bitset, boundary_positions[i]));
    }
    printf("[OK] Byte boundary operations test passed\n");

    // Cleanup
    ss_bigbitset_free(bitset);
    printf("[OK] ss_bigbitset_free: Cleanup completed\n");

    printf("=== All ss_bigbitset tests passed ===\n\n");
}