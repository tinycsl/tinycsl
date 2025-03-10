#include "ss_string.h"
#include "ss_string_utils.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_string_utils()
{
    printf("\n=== Starting ss_string_utils tests ===\n");

    // Test string replacement
    ss_string_t result;
    ss_string_init(&result, "");

    // Test single replacement
    assert(ss_string_utils_replace_one("hello world", "world", "there", &result));
    assert(strcmp(ss_string_to_cstr(&result), "hello there") == 0);
    printf("[OK] ss_string_utils_replace_one: Single replacement test passed\n");

    // Test multiple replacements (results will be appended)
    assert(ss_string_utils_replace("hello hello hello", "hello", "hi", 2, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hello therehi hi hello") == 0);
    printf("[OK] ss_string_utils_replace: Multiple replacement test passed\n");

    // Test replace all (results will be appended)
    assert(ss_string_utils_replace_all("hello hello hello", "hello", "hi", &result));
    assert(strcmp(ss_string_to_cstr(&result), "hello therehi hi hellohi hi hi") == 0);
    printf("[OK] ss_string_utils_replace_all: Replace all test passed\n");

    // Start fresh for other tests
    ss_string_clear(&result);

    // Test empty string handling
    assert(ss_string_utils_empty(""));
    assert(ss_string_utils_empty(NULL));
    assert(!ss_string_utils_empty("not empty"));
    printf("[OK] ss_string_utils_empty: Empty string handling test passed\n");

    // Test string index
    assert(ss_string_utils_index("hello world", "world") == 6);
    assert(ss_string_utils_index("hello world", "xyz") == -1);
    assert(ss_string_utils_index("hello world", "") == 0);
    printf("[OK] ss_string_utils_index: String index test passed\n");

    // Test prefix checking (case sensitive)
    assert(ss_string_utils_has_prefix("hello world", "hello"));
    assert(!ss_string_utils_has_prefix("hello world", "Hello"));
    assert(!ss_string_utils_has_prefix("hello", "hello world"));
    assert(ss_string_utils_has_prefix("hello", ""));
    printf("[OK] ss_string_utils_has_prefix: Case-sensitive prefix test passed\n");

    // Test prefix checking (case insensitive)
    assert(ss_string_utils_has_prefix_case("hello world", "HELLO"));
    assert(ss_string_utils_has_prefix_case("HELLO WORLD", "hello"));
    assert(!ss_string_utils_has_prefix_case("hello", "hello world"));
    assert(ss_string_utils_has_prefix_case("hello", ""));
    printf("[OK] ss_string_utils_has_prefix_case: Case-insensitive prefix test passed\n");

    // Test uppercase conversion
    char output[256];
    assert(strcmp(ss_string_utils_uppercase("hello", output), "HELLO") == 0);
    assert(strcmp(ss_string_utils_uppercase("Hello World", output), "HELLO WORLD") == 0);
    assert(strcmp(ss_string_utils_uppercase("123!@#", output), "123!@#") == 0);
    printf("[OK] ss_string_utils_uppercase: Uppercase conversion test passed\n");

    // Test edge cases for replacement
    ss_string_clear(&result);
    assert(ss_string_utils_replace("", "any", "thing", 1, &result));
    assert(ss_string_size(&result) == 0);

    assert(ss_string_utils_replace("hello", "", "x", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hello") == 0);

    assert(ss_string_utils_replace("hello", "l", "", 0, &result));
    assert(strcmp(ss_string_to_cstr(&result), "helloheo") == 0);

    assert(ss_string_utils_replace("hello", "l", "", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result), "helloheohelo") == 0);
    printf("[OK] ss_string_utils_replace: Edge cases test passed\n");

    // Test NULL handling
    assert(!ss_string_utils_replace(NULL, "test", "replace", 1, &result));
    assert(!ss_string_utils_replace("test", NULL, "replace", 1, &result));
    assert(!ss_string_utils_replace("test", "test", NULL, 1, &result));
    printf("[OK] ss_string_utils_replace: NULL handling test passed\n");

    ss_string_clear(&result);
    // Test special characters
    assert(ss_string_utils_replace("hello\nworld", "\n", " ", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hello world") == 0);
    printf("[OK] ss_string_utils_replace: Special characters test passed\n");

    // Test overlapping patterns
    assert(ss_string_utils_replace("aaa", "aa", "b", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hello worldba") == 0);
    printf("[OK] ss_string_utils_replace: Overlapping patterns test passed\n");

    // Test count edge cases
    assert(
        ss_string_utils_replace("hello", "l", "x", -1, &result)); // -1 converts to size_t max value
    assert(strcmp(ss_string_to_cstr(&result), "hello worldbahexxo") == 0);

    assert(ss_string_utils_replace("hello", "l", "x", 10, &result)); // More than actual matches
    assert(strcmp(ss_string_to_cstr(&result), "hello worldbahexxohexxo") == 0);
    printf("[OK] ss_string_utils_replace: Count edge cases test passed\n");

    ss_string_clear(&result);
    // Test very long string
    char long_str[1000];
    memset(long_str, 'a', 999);
    long_str[999] = '\0';
    assert(ss_string_utils_replace(long_str, "a", "b", 1, &result));
    assert(strncmp(ss_string_to_cstr(&result), "baaaaa", 6) == 0);
    printf("[OK] ss_string_utils_replace: Long string test passed\n");

    ss_string_clear(&result);
    // Test exact match count
    assert(ss_string_utils_replace("hello hello hello", "hello", "hi", 2, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hi hi hello") == 0);
    printf("[OK] ss_string_utils_replace: Exact match count test passed\n");

    // Test complex overlapping patterns
    assert(ss_string_utils_replace("aaaaa", "aa", "b", 2, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hi hi hellobba") == 0);
    printf("[OK] ss_string_utils_replace: Complex overlapping patterns test passed\n");

    // Test more special characters
    assert(ss_string_utils_replace("hello\t\nworld", "\t\n", "--", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hi hi hellobbahello--world") == 0);
    printf("[OK] ss_string_utils_replace: Additional special characters test passed\n");

    // Test replacing with longer string
    assert(ss_string_utils_replace("abc", "b", "xyz", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hi hi hellobbahello--worldaxyzc") == 0);
    printf("[OK] ss_string_utils_replace: Longer replacement test passed\n");

    // Test replacing with shorter string
    assert(ss_string_utils_replace("hello", "ll", "y", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hi hi hellobbahello--worldaxyzcheyo") == 0);
    printf("[OK] ss_string_utils_replace: Shorter replacement test passed\n");

    // Test multiple character matches
    assert(ss_string_utils_replace("test test test", "test", "", 2, &result));
    assert(strcmp(ss_string_to_cstr(&result), "hi hi hellobbahello--worldaxyzcheyo  test") == 0);
    printf("[OK] ss_string_utils_replace: Multiple character matches test passed\n");

    // Test with Chinese characters (UTF-8)
    assert(ss_string_utils_replace("你好世界", "世界", "地球", 1, &result));
    assert(strcmp(ss_string_to_cstr(&result),
                  "hi hi hellobbahello--worldaxyzcheyo  test你好地球") == 0);
    printf("[OK] ss_string_utils_replace: UTF-8 characters test passed\n");

    ss_string_clear(&result);
    // Test consecutive replacements
    assert(ss_string_utils_replace("aaa", "a", "bb", 3, &result));
    assert(strcmp(ss_string_to_cstr(&result), "bbbbbb") == 0);
    printf("[OK] ss_string_utils_replace: Consecutive replacements test passed\n");

    // Test pattern at string boundaries
    assert(ss_string_utils_replace("abcabc", "abc", "x", 2, &result));
    assert(strcmp(ss_string_to_cstr(&result), "bbbbbbxx") == 0);
    printf("[OK] ss_string_utils_replace: Pattern at boundaries test passed\n");

    // Cleanup
    ss_string_destroy(&result);
    printf("[OK] ss_string_destroy: Cleanup completed\n");

    printf("=== All ss_string_utils tests passed ===\n\n");
}