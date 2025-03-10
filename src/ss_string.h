/**
 * @file ss_string.h
 * @brief Dynamic string implementation with automatic memory management
 * 
 * Provides string manipulation functions including concatenation, substring replacement,
 * and type conversion. Built on top of ss_array for efficient memory handling.
 */

#ifndef SS_STRING_H
#define SS_STRING_H

#include "ss_array.h"
#include "ss_types.h"

/**
 * @struct ss_string_s
 * @brief Dynamic string container structure
 * 
 * Maintains string data using a dynamic array structure with automatic
 * memory management and null-termination guarantees.
 */
struct ss_string_s
{
    ss_array_t data;    /**< Underlying character storage array */
    ss_bool_t _destroy; /**< Internal flag for memory management */
};

/**
 * @brief Initializes string structure with optional initial data
 * @param str String structure to initialize
 * @param data Null-terminated initialization string (optional)
 */
void ss_string_init(ss_string_t* str, const char* data);

/**
 * @brief Releases resources for initialized string structure
 * @param str String to destroy
 */
void ss_string_destroy(ss_string_t* str);

/**
 * @brief Creates new string object on heap
 * @param data Null-terminated initialization string (optional)
 * @return Pointer to newly created string, NULL on failure
 * @note Caller must free with ss_string_free()
 */
ss_string_t* ss_string_create(const char* data);

/**
 * @brief Releases heap-allocated string object
 * @param str String pointer to free
 */
void ss_string_free(ss_string_t* str);

/**
 * @brief Clears string content while retaining storage
 * @param str String to clear
 */
void ss_string_clear(ss_string_t* str);

/**
 * @brief Gets null-terminated C-style string pointer
 * @param str Target string object
 * @return const char* Read-only pointer to character data
 * @warning Returned pointer becomes invalid after string modification
 */
#define ss_string_to_cstr(str) ((const char*)((str)->data.elts))

/**
 * @brief Concatenates string with C-style string
 * @param str1 Base string
 * @param data String to append
 * @return New concatenated string
 * @note Caller must free result with ss_string_free()
 */
ss_string_t* ss_string_connect_cstr(ss_string_t* str1, const char* data);

/**
 * @brief Appends binary data to string
 * @param str Target string
 * @param data Data to append
 * @param size Data length in bytes
 * @return Modified string
 */
ss_string_t* ss_string_append(ss_string_t* str, const char* data, size_t size);

/**
 * @brief Appends C-style string
 * @param str Target string
 * @param data Null-terminated string to append
 * @return Modified string
 */
ss_string_t* ss_string_append_cstr(ss_string_t* str, const char* data);

/**
 * @brief Appends another string object
 * @param str Target string
 * @param data String to append
 * @return Modified string
 */
ss_string_t* ss_string_append_str(ss_string_t* str, ss_string_t* data);

/**
 * @brief Appends single character
 * @param str Target string
 * @param c Character to append
 * @return Modified string
 */
ss_string_t* ss_string_append_char(ss_string_t* str, char c);

/**
 * @brief Appends long integer as string
 * @param str Target string
 * @param data Long value to append
 * @return Modified string
 */
ss_string_t* ss_string_append_long(ss_string_t* str, long data);

/**
 * @brief Sets string content from binary data
 * @param str Target string
 * @param data Data to copy
 * @param size Data length in bytes
 * @return Modified string
 */
ss_string_t* ss_string_set(ss_string_t* str, const char* data, size_t size);

/**
 * @brief Sets string content from C-style string
 * @param str Target string
 * @param data Null-terminated source string
 * @return Modified string
 */
ss_string_t* ss_string_set_cstr(ss_string_t* str, const char* data);

/**
 * @brief Sets string content from another string object
 * @param str Target string
 * @param data Source string
 * @return Modified string
 */
ss_string_t* ss_string_set_str(ss_string_t* str, ss_string_t* data);

/**
 * @brief Replaces all occurrences of substring
 * @param str Target string
 * @param src Substring to replace
 * @param dst Replacement string
 * @return Modified string
 */
ss_string_t* ss_string_replace_cstr(ss_string_t* str, const char* src, const char* dst);

/**
 * @brief Gets current string length
 * @param str Target string
 * @return size_t Number of characters (excluding null terminator)
 */
#define ss_string_size(str) ss_array_size(&((str)->data))

/**
 * @brief Checks if string is empty
 * @param str Target string
 * @return ss_bool_t SS_TRUE if empty, SS_FALSE otherwise
 */
#define ss_string_empty(str) (ss_string_size((str)) == 0)

#endif /* SS_STRING_H */
