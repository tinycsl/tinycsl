#include "ss_hash.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

static void test_basic_type_hash()
{
    printf("\n=== Testing basic type hash functions ===\n");

    // Test character hash
    char c1 = 'a', c2 = 'b';
    size_t h1 = ss_hash_char(&c1, 0);
    size_t h2 = ss_hash_char(&c2, 0);
    assert(h1 == (size_t)'a');
    assert(h2 == (size_t)'b');
    assert(h1 != h2);
    printf("[OK] ss_hash_char: Character hash tests passed\n");

    // Test unsigned character hash
    unsigned char uc1 = 255, uc2 = 0;
    h1 = ss_hash_uchar(&uc1, 0);
    h2 = ss_hash_uchar(&uc2, 0);
    assert(h1 == 255);
    assert(h2 == 0);
    assert(h1 != h2);
    printf("[OK] ss_hash_uchar: Unsigned character hash tests passed\n");

    // Test short hash
    short s1 = SHRT_MIN, s2 = SHRT_MAX;
    h1 = ss_hash_short(&s1, 0);
    h2 = ss_hash_short(&s2, 0);
    assert(h1 == (size_t)SHRT_MIN);
    assert(h2 == (size_t)SHRT_MAX);
    assert(h1 != h2);
    printf("[OK] ss_hash_short: Short hash tests passed\n");

    // Test unsigned short hash
    unsigned short us1 = 0, us2 = USHRT_MAX;
    h1 = ss_hash_ushort(&us1, 0);
    h2 = ss_hash_ushort(&us2, 0);
    assert(h1 == 0);
    assert(h2 == USHRT_MAX);
    assert(h1 != h2);
    printf("[OK] ss_hash_ushort: Unsigned short hash tests passed\n");

    // Test integer hash
    int i1 = INT_MIN, i2 = INT_MAX;
    h1 = ss_hash_int(&i1, 0);
    h2 = ss_hash_int(&i2, 0);
    assert(h1 == (size_t)INT_MIN);
    assert(h2 == (size_t)INT_MAX);
    assert(h1 != h2);
    printf("[OK] ss_hash_int: Integer hash tests passed\n");

    // Test unsigned integer hash
    unsigned int ui1 = 0, ui2 = UINT_MAX;
    h1 = ss_hash_uint(&ui1, 0);
    h2 = ss_hash_uint(&ui2, 0);
    assert(h1 == 0);
    assert(h2 == UINT_MAX);
    assert(h1 != h2);
    printf("[OK] ss_hash_uint: Unsigned integer hash tests passed\n");

    // Test long hash
    long l1 = LONG_MIN, l2 = LONG_MAX;
    h1 = ss_hash_long(&l1, 0);
    h2 = ss_hash_long(&l2, 0);
    assert(h1 == (size_t)LONG_MIN);
    assert(h2 == (size_t)LONG_MAX);
    assert(h1 != h2);
    printf("[OK] ss_hash_long: Long hash tests passed\n");

    // Test unsigned long hash
    unsigned long ul1 = 0, ul2 = ULONG_MAX;
    h1 = ss_hash_ulong(&ul1, 0);
    h2 = ss_hash_ulong(&ul2, 0);
    assert(h1 == 0);
    assert(h2 == ULONG_MAX);
    assert(h1 != h2);
    printf("[OK] ss_hash_ulong: Unsigned long hash tests passed\n");
}

static void test_memory_hash()
{
    printf("\n=== Testing memory hash functions ===\n");

    // Test memory hash with fixed size
    char mem1[] = "hello";
    char mem2[] = "world";
    char mem3[] = "hello";
    size_t h1 = ss_hash_mem(mem1, 5);
    size_t h2 = ss_hash_mem(mem2, 5);
    size_t h3 = ss_hash_mem(mem3, 5);
    assert(h1 == h3); // Same content should have same hash
    assert(h1 != h2); // Different content should have different hash

    // Test with different sizes
    assert(ss_hash_mem(mem1, 4) !=
           ss_hash_mem(mem1, 5)); // Different sizes should have different hashes
    printf("[OK] ss_hash_mem: Memory hash tests passed\n");

    // Test memory pointer hash
    const void* ptr1 = mem1;
    const void* ptr2 = mem2;
    const void* ptr3 = mem1;
    h1 = ss_hash_mem_ptr(&ptr1, 5); // Pass the address of the pointer
    h2 = ss_hash_mem_ptr(&ptr2, 5);
    h3 = ss_hash_mem_ptr(&ptr3, 5);
    printf("ptr1: %p, ptr2: %p, ptr3: %p\n", ptr1, ptr2, ptr3);
    printf("h1: %zu, h2: %zu, h3: %zu\n", h1, h2, h3);
    assert(h1 == h3); // Same content should have same hash
    assert(h1 != h2); // Different content should have different hash
    printf("[OK] ss_hash_mem_ptr: Memory pointer hash tests passed\n");

    // Test case-insensitive memory hash
    char cmem1[] = "Hello";
    char cmem2[] = "WORLD";
    char cmem3[] = "hElLo";
    h1 = ss_hash_mem_case(cmem1, 5);
    h2 = ss_hash_mem_case(cmem2, 5);
    h3 = ss_hash_mem_case(cmem3, 5);
    assert(h1 == h3); // Same content (case-insensitive) should have same hash
    assert(h1 != h2); // Different content should have different hash
    printf("[OK] ss_hash_mem_case: Case-insensitive memory hash tests passed\n");

    // Test case-insensitive memory pointer hash
    const void* cptr1 = cmem1;
    const void* cptr2 = cmem2;
    const void* cptr3 = cmem3;
    h1 = ss_hash_mem_case_ptr(&cptr1, 5); // Pass the address of the pointer
    h2 = ss_hash_mem_case_ptr(&cptr2, 5);
    h3 = ss_hash_mem_case_ptr(&cptr3, 5);
    assert(h1 == h3); // Same content (case-insensitive) should have same hash
    assert(h1 != h2); // Different content should have different hash
    printf("[OK] ss_hash_mem_case_ptr: Case-insensitive memory pointer hash tests passed\n");
}

static void test_string_hash()
{
    printf("\n=== Testing string hash functions ===\n");

    // Test string hash
    const char* str1 = "apple";
    const char* str2 = "banana";
    const char* str3 = "apple";
    const char* str4 = "applecat";

    // Test with size = 0 (use null terminator)
    size_t h1 = ss_hash_string(str1, 0);
    size_t h2 = ss_hash_string(str2, 0);
    size_t h3 = ss_hash_string(str3, 0);
    size_t h4 = ss_hash_string(str4, 0);
    assert(h1 == h3); // Same string should have same hash
    assert(h1 != h2); // Different strings should have different hash
    assert(h1 != h4); // Longer string should have different hash
    printf("[OK] ss_hash_string: Null-terminated string hash tests passed\n");

    // Test with explicit size
    h1 = ss_hash_string(str1, 3);          // "app"
    h2 = ss_hash_string("appetite", 3);    // "app"
    h3 = ss_hash_string("application", 3); // "app"
    assert(h1 == h2);                      // Same prefix should have same hash
    assert(h1 == h3);                      // Same prefix should have same hash

    h1 = ss_hash_string(str4, 5); // "apple"
    h2 = ss_hash_string(str1, 5); // "apple"
    h3 = ss_hash_string(str1, 0); // "apple"
    assert(h1 == h2);             // Same content with size should match
    assert(h1 == h3);             // Size-limited should match null-terminated when content is same

    // Test early null terminator
    h1 = ss_hash_string("abc\0def", 6); // should only hash "abc"
    h2 = ss_hash_string("abc", 6);      // should only hash "abc"
    h3 = ss_hash_string("abc", 0);      // should only hash "abc"
    assert(h1 == h2);                   // Early null terminator should stop hashing
    assert(h1 == h3);                   // Should match null-terminated version
    printf("[OK] ss_hash_string: Size-limited string hash tests passed\n");

    // Test string pointer hash with size
    const char* sptr1 = "apple";
    const char* sptr2 = "appetite";
    const char* sptr3 = "application";
    h1 = ss_hash_string_ptr(&sptr1, 3); // should hash "app"
    h2 = ss_hash_string_ptr(&sptr2, 3); // should hash "app"
    h3 = ss_hash_string_ptr(&sptr3, 3); // should hash "app"
    assert(h1 == h2);                   // Same prefix should have same hash
    assert(h1 == h3);                   // Same prefix should have same hash
    printf("[OK] ss_hash_string_ptr: Size-limited string pointer hash tests passed\n");

    // Test case-insensitive string hash with size
    const char* cstr1 = "Apple";
    const char* cstr2 = "APPETITE";
    const char* cstr3 = "aPpLiCaTiOn";
    h1 = ss_hash_string_case(cstr1, 3); // should hash "APP"
    h2 = ss_hash_string_case(cstr2, 3); // should hash "APP"
    h3 = ss_hash_string_case(cstr3, 3); // should hash "APP"
    assert(h1 == h2);                   // Same case-insensitive prefix should have same hash
    assert(h1 == h3);                   // Same case-insensitive prefix should have same hash

    // Test early null terminator with case insensitive
    h1 = ss_hash_string_case("ABC\0DEF", 6); // should only hash "ABC"
    h2 = ss_hash_string_case("abc", 6);      // should only hash "abc"
    h3 = ss_hash_string_case("ABC", 0);      // should only hash "ABC"
    assert(h1 == h2);                        // Early null terminator should stop hashing
    assert(h1 == h3);                        // Should match null-terminated version
    printf("[OK] ss_hash_string_case: Size-limited case-insensitive string hash tests passed\n");

    // Test case-insensitive string pointer hash with size
    const char* csptr1 = "Apple";
    const char* csptr2 = "APPETITE";
    const char* csptr3 = "aPpLiCaTiOn";
    h1 = ss_hash_string_case_ptr(&csptr1, 3); // should hash "APP"
    h2 = ss_hash_string_case_ptr(&csptr2, 3); // should hash "APP"
    h3 = ss_hash_string_case_ptr(&csptr3, 3); // should hash "APP"
    assert(h1 == h2);                         // Same case-insensitive prefix should have same hash
    assert(h1 == h3);                         // Same case-insensitive prefix should have same hash
    printf("[OK] ss_hash_string_case_ptr: Size-limited case-insensitive string pointer hash tests "
           "passed\n");
}

static void test_pointer_hash()
{
    printf("\n=== Testing pointer hash functions ===\n");

    // Test pointer hash
    void* ptr1 = (void*)0x1000;
    void* ptr2 = (void*)0x2000;
    void* ptr3 = (void*)0x1000;
    size_t h1 = ss_hash_ptr(&ptr1, 0);
    size_t h2 = ss_hash_ptr(&ptr2, 0);
    size_t h3 = ss_hash_ptr(&ptr3, 0);
    assert(h1 == h3); // Same pointer value should have same hash
    assert(h1 != h2); // Different pointer values should have different hash
    printf("[OK] ss_hash_ptr: Pointer hash tests passed\n");
}

void test_hash()
{
    printf("\n=== Starting ss_hash tests ===\n");

    test_basic_type_hash();
    test_memory_hash();
    test_string_hash();
    test_pointer_hash();

    printf("=== All ss_hash tests passed ===\n\n");
}