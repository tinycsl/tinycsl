/**
 * @file ss_alloc.h
 * @brief Memory allocation interfaces
 * @author trywen@qq.com
 * @date 2011-08-20
 *
 * Provides custom memory allocation functions for the library
 * including malloc/realloc/free implementations with alignment support.
 */

#ifndef SS_ALLOC_H
#define SS_ALLOC_H

#ifdef _MSC_VER
#include <malloc.h>
/**
 * @brief MSVC stack allocation macro using _alloca
 * @param name Buffer variable name
 * @param size Allocation size in bytes
 * @warning Stack-allocated memory is automatically freed when function returns
 */
#define SS_STACK_ALLOC(name, size) void* name = _alloca(size);
#else
/**
 * @brief GCC/Clang stack allocation using variable-length arrays
 * @param name Buffer variable name
 * @param size Allocation size in bytes
 * @note Creates a stack-allocated byte array with automatic storage duration
 */
#define SS_STACK_ALLOC(name, size) unsigned char name[size];
#endif

/**
 * @brief Memory allocation function type
 * @param size Requested memory size in bytes
 * @param align Alignment requirement for memory address
 * @return void* Pointer to allocated memory, NULL if failed
 */
typedef void* (*ss_malloc_f)(unsigned long size);

/**
 * @brief Custom memory reallocation function type
 * @param ptr Original memory pointer
 * @param malloc_size Originally allocated size in bytes
 * @param realloc_size New requested size in bytes
 * @return void* New memory pointer on success, NULL on failure
 * @note If returns NULL, the original memory block remains valid
 */
typedef void* (*ss_realloc_f)(void* ptr, unsigned long malloc_size, unsigned long realloc_size);

/**
 * @brief Custom memory deallocation function type
 * @param ptr Memory pointer to free
 * @note Must handle NULL pointer input properly
 */
typedef void (*ss_free_f)(void* ptr);

/**
 * @brief Initializes custom memory allocator functions
 * @param malloc_f Custom malloc function pointer
 * @param free_f Custom free function pointer
 * @param realloc_f Custom realloc function pointer (optional)
 *
 * If realloc_f is NULL, a default implementation will be used that:
 * 1. Allocates new memory block
 * 2. Copies existing data
 * 3. Frees original block
 */
void ss_alloc_init(ss_malloc_f malloc_f, ss_free_f free_f, ss_realloc_f realloc_f);

/**
 * @brief Allocates memory using current allocator
 * @param size Requested allocation size in bytes
 * @return void* Pointer to allocated memory, NULL if allocation failed
 * @note Zero-size allocations will return NULL
 */
void* ss_malloc(unsigned long size);

/**
 * @brief Reallocates memory using current allocator
 * @param ptr Original memory pointer
 * @param malloc_size Original allocation size
 * @param realloc_size New requested size
 * @return void* Pointer to reallocated memory, NULL if reallocation failed
 *
 * If new size <= original size, returns original pointer unchanged.
 * Otherwise delegates to configured realloc implementation.
 */
void* ss_realloc(void* ptr, unsigned long malloc_size, unsigned long realloc_size);

/**
 * @brief Releases memory using current allocator
 * @param p Memory pointer to free
 * @note Safe to call with NULL pointer (no-operation)
 */
void ss_free(void* p);

#endif