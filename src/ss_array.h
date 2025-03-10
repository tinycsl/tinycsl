/**
 * @file ss_array.h
 * @brief Dynamic array implementation with automatic capacity management
 *
 * Provides type-safe dynamic array operations including creation, destruction,
 * element access, sorting and memory management. Supports configurable initial
 * capacity and automatic expansion.
 */

#ifndef SS_ARRAY_H
#define SS_ARRAY_H

#include "ss_types.h"

/**
 * @struct ss_array_s
 * @brief Core structure of dynamic array
 *
 * @var elts Pointer to element storage area
 * @var el_size Single element size in bytes
 * @var size Current number of elements stored
 * @var capacity Currently allocated storage capacity
 * @var allocator Memory allocator pointer (NULL when using default allocator)
 */
struct ss_array_s
{
    void* elts;
    size_t el_size;
    size_t size;
    size_t capacity;
};

/**
 * @brief Create heap-allocated dynamic array
 * @param[in] el_size Element size in bytes
 * @param[in] capacity Initial capacity
 * @return Pointer to newly created array, NULL on failure
 * @note Caller must free with ss_array_free()
 */
ss_array_t* ss_array_create(size_t el_size, size_t capacity);

/**
 * @brief Releases memory for array created by ss_array_create()
 * @param[in] a Array pointer to free
 */
void ss_array_free(ss_array_t* a);

/**
 * @brief Initializes existing array structure
 * @param a Array to initialize
 * @param el_size Element size in bytes
 * @param capacity Initial capacity
 * @return SS_TRUE on success, SS_FALSE on allocation failure
 */
ss_bool_t ss_array_init(ss_array_t* a, size_t el_size, size_t capacity);

/**
 * @brief Releases resources for initialized array
 * @param a Array to destroy
 */
void ss_array_destroy(ss_array_t* a);

/**
 * @brief Sorts array elements in-place
 * @param a Array to sort
 * @param compare Comparison function pointer
 */
void ss_array_sort(ss_array_t* a, ss_compare_f compare);

/**
 * @brief Ensures minimum capacity for array
 * @param a Array to modify
 * @param n Minimum required capacity
 * @return SS_TRUE on success, SS_FALSE on allocation failure
 */
ss_bool_t ss_array_reserve(ss_array_t* a, size_t n);

/**
 * @brief Reserves additional capacity for appending operations
 * @param a Array to modify
 * @param n Number of elements to reserve
 * @return SS_TRUE on success, SS_FALSE on allocation failure
 */
ss_bool_t ss_array_reserve_append(ss_array_t* a, size_t n);

/**
 * @brief Resizes array to exact element count
 * @param a Array to resize
 * @param n New element count
 * @return SS_TRUE on success, SS_FALSE on allocation failure
 * @note May truncate existing elements if n < current size
 */
ss_bool_t ss_array_resize(ss_array_t* a, size_t n);

/**
 * @brief Appends single element to array
 * @param a Array to modify
 * @param value Pointer to element data (must match el_size)
 * @return SS_TRUE on success, SS_FALSE on allocation failure
 */
ss_bool_t ss_array_push(ss_array_t* a, const void* value);

/**
 * @brief Appends multiple elements to array
 * @param a Array to modify
 * @param values Pointer to contiguous elements data
 * @param n Number of elements to append
 * @return SS_TRUE on success, SS_FALSE on allocation failure
 */
ss_bool_t ss_array_push_n(ss_array_t* a, const void* values, size_t n);

// Modify the value at specified position, new_value can be NULL
ss_bool_t ss_array_replace(ss_array_t* a, const void* ovalue, const void* nvalue);

// Modify elements starting from specified position, n is the count of elements
ss_bool_t ss_array_update(ss_array_t* a, size_t off, const void* start, size_t n);
// Modify the value at specified position, returns previous value in out_old_value
ss_bool_t ss_array_set(ss_array_t* a, size_t idx, const void* value, void* out_old_value);
// Retrieve value at specified position into out_value, returns NULL on invalid index or out_value,
// otherwise returns out_value For direct reference access use: ss_array_at()
void* ss_array_get(ss_array_t* a, size_t idx, void* out_value);

/**
 * @brief Gets pointer to element at specified index
 * @param a Target array
 * @param idx Zero-based element index
 * @return void* Pointer to element data
 * @warning No bounds checking - caller must ensure valid index
 */
#define ss_array_at(a, idx) ((char*)(a)->elts + (idx) * (a)->el_size)

/**
 * @brief Gets pointer to last element in array
 * @param a Target array
 * @return void* Pointer to last element, NULL if empty
 */
#define ss_array_back(a)                                                                           \
    (((a)->size == 0) ? NULL : ((char*)(a)->elts + ((a)->size - 1) * (a)->el_size))

/**
 * @brief Reduces array size by one (removes last element)
 * @param a Array to modify
 * @note Does not free memory - element remains until overwritten
 */
#define ss_array_pop_back(a) ((a)->size--)

/**
 * @brief Clears all elements while retaining storage
 * @param a Array to clear
 */
#define ss_array_clear(a) ((a)->size = 0)

/**
 * @brief Gets current element count
 * @param a Target array
 * @return size_t Number of elements
 */
#define ss_array_size(a) ((a)->size)

/**
 * @brief Gets raw data pointer
 * @param a Target array
 * @return void* Pointer to contiguous element storage
 */
#define ss_array_data(a) ((a)->elts)

#endif /* SS_ARRAY_H */
