#include "ss_compare.h"
#include "ss_slice.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

void test_compare()
{
    printf("\n=== Starting ss_compare tests ===\n");

    // Test character comparison
    char c1 = 'a', c2 = 'b', c3 = 'a';
    assert(ss_compare_char(&c1, 0, &c2, 0) < 0);
    assert(ss_compare_char(&c2, 0, &c1, 0) > 0);
    assert(ss_compare_char(&c1, 0, &c3, 0) == 0);
    printf("[OK] ss_compare_char: Character comparison tests passed\n");

    // Test unsigned character comparison
    unsigned char uc1 = 255, uc2 = 0, uc3 = 255;
    assert(ss_compare_uchar(&uc1, 0, &uc2, 0) > 0);
    assert(ss_compare_uchar(&uc2, 0, &uc1, 0) < 0);
    assert(ss_compare_uchar(&uc1, 0, &uc3, 0) == 0);
    printf("[OK] ss_compare_uchar: Unsigned character comparison tests passed\n");

    // Test short comparison
    short s1 = -32768, s2 = 32767, s3 = -32768;
    assert(ss_compare_short(&s1, 0, &s2, 0) < 0);
    assert(ss_compare_short(&s2, 0, &s1, 0) > 0);
    assert(ss_compare_short(&s1, 0, &s3, 0) == 0);
    printf("[OK] ss_compare_short: Short comparison tests passed\n");

    // Test unsigned short comparison
    unsigned short us1 = 0, us2 = 65535, us3 = 0;
    assert(ss_compare_ushort(&us1, 0, &us2, 0) < 0);
    assert(ss_compare_ushort(&us2, 0, &us1, 0) > 0);
    assert(ss_compare_ushort(&us1, 0, &us3, 0) == 0);
    printf("[OK] ss_compare_ushort: Unsigned short comparison tests passed\n");

    // Test integer comparison
    int i1 = -2147483648, i2 = 2147483647, i3 = -2147483648;
    assert(ss_compare_int(&i1, 0, &i2, 0) < 0);
    assert(ss_compare_int(&i2, 0, &i1, 0) > 0);
    assert(ss_compare_int(&i1, 0, &i3, 0) == 0);
    printf("[OK] ss_compare_int: Integer comparison tests passed\n");

    // Test unsigned integer comparison
    unsigned int ui1 = 0, ui2 = 4294967295U, ui3 = 0;
    assert(ss_compare_uint(&ui1, 0, &ui2, 0) < 0);
    assert(ss_compare_uint(&ui2, 0, &ui1, 0) > 0);
    assert(ss_compare_uint(&ui1, 0, &ui3, 0) == 0);
    printf("[OK] ss_compare_uint: Unsigned integer comparison tests passed\n");

    // Test long comparison
    long l1 = LONG_MIN, l2 = LONG_MAX, l3 = LONG_MIN;
    assert(ss_compare_long(&l1, 0, &l2, 0) < 0);
    assert(ss_compare_long(&l2, 0, &l1, 0) > 0);
    assert(ss_compare_long(&l1, 0, &l3, 0) == 0);
    printf("[OK] ss_compare_long: Long comparison tests passed\n");

    // Test unsigned long comparison
    unsigned long ul1 = 0, ul2 = ULONG_MAX, ul3 = 0;
    assert(ss_compare_ulong(&ul1, 0, &ul2, 0) < 0);
    assert(ss_compare_ulong(&ul2, 0, &ul1, 0) > 0);
    assert(ss_compare_ulong(&ul1, 0, &ul3, 0) == 0);
    printf("[OK] ss_compare_ulong: Unsigned long comparison tests passed\n");

    // Test memory comparison
    char mem1[] = "hello";
    char mem2[] = "world";
    char mem3[] = "hello";
    char mem4[] = "hell";
    assert(ss_compare_mem(mem1, 5, mem2, 5) < 0);
    assert(ss_compare_mem(mem2, 5, mem1, 5) > 0);
    assert(ss_compare_mem(mem1, 5, mem3, 5) == 0);
    assert(ss_compare_mem(mem1, 5, mem4, 4) > 0);
    assert(ss_compare_mem(mem4, 4, mem1, 5) < 0);
    printf("[OK] ss_compare_mem: Memory comparison tests passed\n");

    // Test string comparison
    const char* str1 = "apple";
    const char* str2 = "banana";
    const char* str3 = "apple";
    assert(ss_compare_string(str1, 0, str2, 0) < 0);
    assert(ss_compare_string(str2, 0, str1, 0) > 0);
    assert(ss_compare_string(str1, 0, str3, 0) == 0);
    printf("[OK] ss_compare_string: String comparison tests passed\n");

    // Test case-insensitive string comparison
    const char* cstr1 = "Apple";
    const char* cstr2 = "BANANA";
    const char* cstr3 = "aPpLe";
    assert(ss_compare_string_case(cstr1, 0, cstr2, 0) < 0);
    assert(ss_compare_string_case(cstr2, 0, cstr1, 0) > 0);
    assert(ss_compare_string_case(cstr1, 0, cstr3, 0) == 0);
    printf("[OK] ss_compare_string_case: Case-insensitive string comparison tests passed\n");

    // Test pointer comparison
    void* ptr1 = (void*)0x1000;
    void* ptr2 = (void*)0x2000;
    void* ptr3 = (void*)0x1000;
    assert(ss_compare_ptr(&ptr1, 0, &ptr2, 0) < 0);
    assert(ss_compare_ptr(&ptr2, 0, &ptr1, 0) > 0);
    assert(ss_compare_ptr(&ptr1, 0, &ptr3, 0) == 0);
    printf("[OK] ss_compare_ptr: Pointer comparison tests passed\n");

    // Test ss_slice_t comparison
    ss_slice_t data1 = {(void*)"hello", 5};
    ss_slice_t data2 = {(void*)"world", 5};
    ss_slice_t data3 = {(void*)"hello", 5};
    assert(ss_compare_data(&data1, 0, &data2, 0) < 0);
    assert(ss_compare_data(&data2, 0, &data1, 0) > 0);
    assert(ss_compare_data(&data1, 0, &data3, 0) == 0);
    printf("[OK] ss_compare_data: Data structure comparison tests passed\n");

    // Test case-insensitive ss_slice_t comparison
    ss_slice_t cdata1 = {(void*)"Hello", 5};
    ss_slice_t cdata2 = {(void*)"WORLD", 5};
    ss_slice_t cdata3 = {(void*)"hElLo", 5};
    assert(ss_compare_data_case(&cdata1, 0, &cdata2, 0) < 0);
    assert(ss_compare_data_case(&cdata2, 0, &cdata1, 0) > 0);
    assert(ss_compare_data_case(&cdata1, 0, &cdata3, 0) == 0);
    printf("[OK] ss_compare_data_case: Case-insensitive data structure comparison tests passed\n");

    printf("=== All ss_compare tests passed ===\n\n");
}