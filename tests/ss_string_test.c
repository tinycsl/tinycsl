#include "ss_string.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_string()
{
    printf("\n=== Starting ss_string tests ===\n");

    // Test string initialization and basic operations
    ss_string_t str1;
    ss_string_init(&str1, "hello");
    assert(strcmp(ss_string_to_cstr(&str1), "hello") == 0);
    printf("ss_string_size(&str1) = %d\n", (int)ss_string_size(&str1));
    assert(ss_string_size(&str1) == 5);
    printf("[OK] ss_string_init, ss_string_to_cstr, ss_string_size: Basic initialization tests "
           "passed\n");

    // Test string append operations
    ss_string_append_cstr(&str1, " world");
    assert(strcmp(ss_string_to_cstr(&str1), "hello world") == 0);

    ss_string_append_char(&str1, '!');
    assert(strcmp(ss_string_to_cstr(&str1), "hello world!") == 0);

    ss_string_append_long(&str1, 42);
    assert(strcmp(ss_string_to_cstr(&str1), "hello world!42") == 0);
    printf("[OK] ss_string_append_cstr, ss_string_append_char, ss_string_append_long: Append "
           "operations tests passed\n");

    // Test string set operations
    ss_string_set_cstr(&str1, "new content");
    assert(strcmp(ss_string_to_cstr(&str1), "new content") == 0);
    printf("[OK] ss_string_set_cstr: Set operations tests passed\n");

    // Test string connect operations
    ss_string_t* connected = ss_string_connect_cstr(&str1, " appended");
    assert(strcmp(ss_string_to_cstr(connected), "new content appended") == 0);
    ss_string_free(connected);
    printf("[OK] ss_string_connect_cstr, ss_string_free: Connect operations tests passed\n");

    // Test empty string operations
    ss_string_clear(&str1);
    assert(ss_string_empty(&str1));
    assert(ss_string_size(&str1) == 0);
    printf("[OK] ss_string_clear, ss_string_empty: Empty string operations tests passed\n");

    // Test string set with size
    ss_string_set(&str1, "hello\0hidden", 12); // Include null character
    assert(ss_string_size(&str1) == 12);
    assert(memcmp(ss_string_to_cstr(&str1), "hello\0hidden", 12) == 0);
    printf("[OK] ss_string_set: Set with explicit size tests passed\n");

    // Test string append with size
    ss_string_append(&str1, "\0more", 5);
    assert(ss_string_size(&str1) == 17);
    printf("[OK] ss_string_append: Append with explicit size tests passed\n");

    // Test string to string operations
    ss_string_t str2;
    ss_string_init(&str2, "source");
    ss_string_set_str(&str1, &str2);
    assert(strcmp(ss_string_to_cstr(&str1), "source") == 0);

    ss_string_append_str(&str1, &str2);
    assert(strcmp(ss_string_to_cstr(&str1), "sourcesource") == 0);
    printf(
        "[OK] ss_string_set_str, ss_string_append_str: String to string operations tests passed\n");

    // Test string replace
    ss_string_replace_cstr(&str1, "source", "target");
    assert(strcmp(ss_string_to_cstr(&str1), "targettarget") == 0);
    printf("[OK] ss_string_replace_cstr: Replace operations tests passed\n");

    // Test create and free
    ss_string_t* str3 = ss_string_create("dynamic");
    assert(strcmp(ss_string_to_cstr(str3), "dynamic") == 0);
    ss_string_free(str3);
    printf("[OK] ss_string_create, ss_string_free: Dynamic string creation and deletion tests "
           "passed\n");

    // Test NULL and empty string handling
    ss_string_t empty_str;
    ss_string_init(&empty_str, NULL);
    assert(ss_string_empty(&empty_str));
    ss_string_destroy(&empty_str);

    ss_string_init(&empty_str, "");
    assert(ss_string_empty(&empty_str));
    printf("[OK] ss_string_init: NULL and empty string handling tests passed\n");

    // Cleanup
    ss_string_destroy(&str1);
    ss_string_destroy(&str2);
    ss_string_destroy(&empty_str);
    printf("[OK] ss_string_destroy: Cleanup completed\n");

    printf("=== All ss_string tests passed ===\n\n");
}