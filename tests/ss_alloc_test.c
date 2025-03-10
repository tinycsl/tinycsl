#include "ss_alloc.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Custom allocator functions for testing
static void* test_malloc(unsigned long size)
{
    printf("test_malloc(%lx)\n", size);
    void* ptr = malloc(size);
    if (ptr)
    {
        printf("test_malloc(%lu) = %p\n", size, ptr);
    }
    return ptr;
}

static void* test_realloc(void* ptr, unsigned long malloc_size, unsigned long realloc_size)
{
    void* new_ptr = realloc(ptr, realloc_size);
    if (new_ptr)
    {
        printf("test_realloc(%p, %lu, %lu) = %p\n", ptr, malloc_size, realloc_size, new_ptr);
    }
    return new_ptr;
}

static void test_free(void* ptr)
{
    printf("test_free(%p)\n", ptr);
    free(ptr);
}

void test_alloc()
{
    printf("\n=== Starting ss_alloc tests ===\n");

    // Test custom allocator initialization
    ss_alloc_init(test_malloc, test_free, test_realloc);
    printf("[OK] ss_alloc_init: Custom allocator initialization completed\n");

    // Test basic malloc and free
    void* ptr1 = ss_malloc(100);
    assert(ptr1 != NULL);
    printf("[OK] ss_malloc: Basic allocation test passed\n");

    // Test writing to allocated memory
    memset(ptr1, 0xAA, 100);
    printf("[OK] Memory write test passed\n");

    // Test realloc with size increase
    void* ptr2 = ss_realloc(ptr1, 100, 200);
    assert(ptr2 != NULL);
    // Verify that the original data is preserved
    unsigned char* check = (unsigned char*)ptr2;
    for (int i = 0; i < 100; i++)
    {
        assert(check[i] == 0xAA);
    }
    printf("[OK] ss_realloc: Size increase test passed\n");

    // Test realloc with size decrease
    ptr1 = ss_realloc(ptr2, 200, 50);
    assert(ptr1 != NULL);
    // Verify that the data is still preserved
    check = (unsigned char*)ptr1;
    for (int i = 0; i < 50; i++)
    {
        assert(check[i] == 0xAA);
    }
    printf("[OK] ss_realloc: Size decrease test passed\n");

    // Test multiple allocations
    void* ptrs[5];
    for (int i = 0; i < 5; i++)
    {
        ptrs[i] = ss_malloc(50 * (i + 1));
        assert(ptrs[i] != NULL);
        memset(ptrs[i], i + 1, 50 * (i + 1));
    }
    printf("[OK] Multiple allocations test passed\n");

    // Test multiple frees
    for (int i = 0; i < 5; i++)
    {
        ss_free(ptrs[i]);
    }
    printf("[OK] Multiple frees test passed\n");

    // Test stack allocation (platform specific)
#ifdef _MSC_VER
    printf("\nTesting MSVC stack allocation:\n");

    // Test different variable names
    SS_STACK_ALLOC(stack_buf1, 100);
    SS_STACK_ALLOC(my_buffer, 200);
    SS_STACK_ALLOC(temp_123, 50);

    // Verify we can use these buffers
    assert(stack_buf1 != NULL);
    assert(my_buffer != NULL);
    assert(temp_123 != NULL);

    memset(stack_buf1, 0xBB, 100);
    memset(my_buffer, 0xCC, 200);
    memset(temp_123, 0xDD, 50);

    // Verify the contents
    unsigned char* check1 = (unsigned char*)stack_buf1;
    unsigned char* check2 = (unsigned char*)my_buffer;
    unsigned char* check3 = (unsigned char*)temp_123;

    for (int i = 0; i < 100; i++)
        assert(check1[i] == 0xBB);
    for (int i = 0; i < 200; i++)
        assert(check2[i] == 0xCC);
    for (int i = 0; i < 50; i++)
        assert(check3[i] == 0xDD);

    printf("[OK] MSVC stack allocation test passed\n");
#else
    printf("\nTesting GCC/Clang stack allocation:\n");

    // Test different variable names
    SS_STACK_ALLOC(stack_buf2, 100);
    SS_STACK_ALLOC(my_buffer, 200);
    SS_STACK_ALLOC(temp_123, 50);

    // Verify we can use these buffers (for VLA they're arrays, not pointers)
    memset(stack_buf2, 0xBB, 100);
    memset(my_buffer, 0xCC, 200);
    memset(temp_123, 0xDD, 50);

    // Verify the contents
    for (int i = 0; i < 100; i++)
        assert(stack_buf2[i] == 0xBB);
    for (int i = 0; i < 200; i++)
        assert(my_buffer[i] == 0xCC);
    for (int i = 0; i < 50; i++)
        assert(temp_123[i] == 0xDD);

    printf("[OK] GCC/Clang stack allocation test passed\n");
#endif

    // Test edge cases
    char* null_ptr = (char*)ss_malloc(0); // Zero size allocation
    assert(null_ptr == NULL);
    ss_free(null_ptr); // Free NULL pointer should not crash

    // Test very large allocation (might fail depending on system memory)
    // unsigned long size = -1;
    // int8_t* large_ptr = (int8_t*)ss_malloc(size); // Very large size
    // printf("large_ptr: %p\n", large_ptr);
    // large_ptr[0] = 0xAA;
    // large_ptr[size - 1] = 0xBB;
    // printf("size: %lu\n", size);
    // printf("large_ptr[0]: %02X\n", (uint8_t)large_ptr[0]);
    // printf("large_ptr[size - 1]: %02X\n", (uint8_t)large_ptr[size - 1]);
    printf("[OK] Edge cases test passed\n");

    // Final cleanup
    ss_free(ptr1);
    printf("[OK] Final cleanup completed\n");

    printf("=== All ss_alloc tests passed ===\n\n");
}