/**
 * @file ss_string_utils.h
 * @brief String manipulation utilities with enhanced functionality
 * 
 * Provides advanced string operations including substring replacement, prefix checking,
 * case conversion and search functions. Designed to work with custom string types.
 */

#ifndef SS_STRING_UTILS_H
#define SS_STRING_UTILS_H

#include "ss_string.h"
#include "ss_types.h"

/**
 * @brief Replace occurrences of substring in source string
 * @param srcstr Source string to process
 * @param substr Substring to replace
 * @param dststr Replacement string
 * @param count Maximum replacements (0=unlimited)
 * @param retstr Output string to store result
 * @return SS_TRUE on success, SS_FALSE on invalid parameters
 */
ss_bool_t ss_string_utils_replace(const char* srcstr, const char* substr, const char* dststr,
                                  size_t count, ss_string_t* retstr);

/**
 * @brief Replace first occurrence of substring
 * @param srcstr Source string to modify
 * @param substr Substring to replace
 * @param dststr Replacement string
 * @param retstr Output string for result
 */
#define ss_string_utils_replace_one(srcstr, substr, dststr, retstr)                                \
    (ss_string_utils_replace((srcstr), (substr), (dststr), 1, (retstr)))

/**
 * @brief Replace all occurrences of substring
 * @param srcstr Source string to modify
 * @param substr Substring to replace
 * @param dststr Replacement string
 * @param retstr Output string for result
 */
#define ss_string_utils_replace_all(srcstr, substr, dststr, retstr)                                \
    (ss_string_utils_replace((srcstr), (substr), (dststr), 0, (retstr)))

/**
 * @brief Check if string is empty or NULL
 * @param str String to check
 * @return SS_TRUE if empty/NULL, SS_FALSE otherwise
 */
#define ss_string_utils_empty(str) (!(str) || ((const char*)(str))[0] == 0)

/**
 * @brief Find first occurrence of substring
 * @param s Target string to search
 * @param substr Substring to locate
 * @return Zero-based index of first occurrence, -1 if not found
 */
int ss_string_utils_index(const char* s, const char* substr);

/**
 * @brief Case-insensitive prefix check
 * @param s String to check
 * @param prefix Prefix to search for
 * @return SS_TRUE if string starts with prefix (case-insensitive)
 */
ss_bool_t ss_string_utils_has_prefix_case(const char* s, const char* prefix);

/**
 * @brief Case-sensitive prefix check
 * @param s String to check
 * @param prefix Prefix to search for
 * @return SS_TRUE if string starts with prefix (case-sensitive)
 */
ss_bool_t ss_string_utils_has_prefix(const char* s, const char* prefix);

/**
 * @brief Convert string to uppercase in-place
 * @param str Input string to convert
 * @param output Buffer to store result (must be at least strlen(str)+1)
 * @return Pointer to converted string
 */
char* ss_string_utils_uppercase(const char* str, char* output);

#endif /* SS_STRING_UTILS_H */
