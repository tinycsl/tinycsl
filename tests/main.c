void test_list();
void test_obtree();
void test_bitarray();
void test_array();
void test_string();
void test_string_utils();
void test_bigbitset();
void test_hashmap();
void test_alloc();
void test_compare();
void test_hash();
void log_env();

int main()
{
    test_list();
    test_obtree();
    test_bitarray();
    test_array();
    test_string();
    test_string_utils();
    test_bigbitset();
    test_hashmap();
    test_alloc();
    test_compare();
    test_hash();

    log_env();

    return 0;
}

#include "ss_version.h"
#include <stdio.h>

void log_env()
{
    printf("USE_STANDARD_TOLOWER: %d\n", USE_STANDARD_TOLOWER);
    printf("USE_STANDARD_TOUPPER: %d\n", USE_STANDARD_TOUPPER);

#ifdef SS_C11_ENABLED
    printf("SS_C11_ENABLED\n");
#endif

#ifdef SS_STRUCT_LITERAL_ENABLED
    printf("SS_STRUCT_LITERAL_ENABLED\n");
#endif
}