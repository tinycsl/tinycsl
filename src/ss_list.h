/**
 * @file ss_list.h
 * @brief Doubly linked list implementation
 * @author trywen@qq.com
 * @date 2011-08-28
 *
 * Provides type-safe operations for doubly linked list including:
 * - Insertion/removal at arbitrary positions
 * - Iterator patterns
 * - Memory management of list elements
 */

#ifndef SS_LIST_H
#define SS_LIST_H

/**
 * @file ss_list.h
 * @brief Doubly linked list implementation
 *
 * Provides generic doubly linked list functionality with type-safe operations.
 * Supports insertion/removal at arbitrary positions and iterator patterns.
 */

#include "ss_types.h"

/**
 * Linked list, values can be NULL
 */

/**
 * @struct ss_list_s
 * @brief Core container structure for doubly linked list
 * 
 * @var el_size Element size in bytes (immutable after initialization)
 * @var size Total number of elements in the list
 * @var first Pointer to the first node (NULL when empty)
 * @var last Pointer to the last node (NULL when empty)
 */
struct ss_list_s
{
    size_t el_size;
    size_t size;
    ss_list_node_t* first;
    ss_list_node_t* last;
};

struct ss_list_node_s
{
    ss_list_node_t* prev;
    ss_list_node_t* next;
    void* data;
};

/* If returns true, stop the traversal */
/**
 * @brief List iteration callback type
 * @param l List pointer
 * @param idx Current element index
 * @param node Current list node
 * @param userdata User-provided data
 * @param userdata_size Size of user-provided data
 * @return SS_TRUE to stop iteration, SS_FALSE to continue
 */
typedef ss_bool_t (*ss_list_iterate_cb_f)(ss_list_t* l, size_t idx, ss_list_node_t* node,
                                          void* userdata, size_t userdata_size);

/**
 * @brief Initialize list structure
 * @param l List pointer to initialize
 * @param el_size Size of elements in bytes
 */
void ss_list_init(ss_list_t* l, size_t el_size);

/**
 * @brief Destroy list structure
 * @param l List pointer to destroy
 */
void ss_list_destroy(ss_list_t* l);

/**
 * @brief Create new linked list
 * @param[in] el_size Size of elements in bytes
 * @return Newly allocated list pointer
 * @note Caller must free with ss_list_free()
 */
/**
 * @brief Create new linked list
 * @param el_size Size of elements in bytes
 * @return Newly allocated list pointer
 * @note Caller must free with ss_list_free()
 */
ss_list_t* ss_list_create(size_t el_size);

/**
 * @brief Free a linked list
 * @param l List pointer to free
 */
void ss_list_free(ss_list_t* l);

/**
 * @brief Iterate through list elements
 * @param l List pointer
 * @param cb Callback function for each element
 * @param userdata User-defined data to pass to callback
 * @param userdata_size Size of user-defined data
 * @return SS_TRUE if iteration completed fully, SS_FALSE if stopped early
 */
ss_bool_t
ss_list_iterate(ss_list_t* l, ss_list_iterate_cb_f cb, void* userdata,
                size_t userdata_size);
/**
 * @brief Inserts an element at the beginning of the list
 * @param l Pointer to the list structure
 * @param data Pointer to element data to insert
 * @return SS_TRUE if insertion succeeded, SS_FALSE otherwise
 */
ss_bool_t ss_list_insert(ss_list_t* l, void* data);
/**
 * @brief Insert element at specified position
 * @param[in] l List pointer
 * @param[in] data Pointer to element data
 * @param[in] index Insertion position (0-based)
 * @return SS_TRUE if insertion succeeded
 * @note Index beyond list size will append to end
 */
ss_bool_t ss_list_insert_at(ss_list_t* l, void* data,
                            size_t index);        // Insert object at specified position
/**
 * @brief Appends an element to the end of the list
 * @param l Pointer to the list structure
 * @param data Pointer to element data to append
 * @return SS_TRUE if append succeeded, SS_FALSE otherwise
 */
ss_bool_t ss_list_push(ss_list_t* l, void* data);

/**
 * @brief Removes the first occurrence of specified element
 * @param l Pointer to the list structure
 * @param data Pointer to element data to remove
 * @return SS_TRUE if element found and removed, SS_FALSE otherwise
 */
ss_bool_t ss_list_remove(ss_list_t* l, void* data);

// Modify element value, new_value can be NULL
/**
 * @brief Replace an existing element value
 * @param l List pointer
 * @param odata Pointer to old value to replace
 * @param ndata Pointer to new value
 * @return SS_TRUE if replacement succeeded, SS_FALSE otherwise
 */
ss_bool_t ss_list_set(ss_list_t* l, void* odata, void* ndata);

// Get element from front, returns NULL if empty
/**
 * @brief Get front element of list
 * @param l List pointer
 * @return Pointer to front element data, NULL if empty
 */
#define ss_list_front(l) ((l)->first ? (l)->first->data : NULL)
// Remove front element
void ss_list_pop(ss_list_t* l);

// Get element from rear, returns NULL if empty
/**
 * @brief Get back element of list
 * @param l List pointer
 * @return Pointer to back element data, NULL if empty
 */
#define ss_list_back(l) ((l)->last ? (l)->last->data : NULL)
// Remove rear element
void ss_list_pop_back(ss_list_t* l);

void ss_list_clear(ss_list_t* l);

/**
 * @brief Get current list size
 * @param l List pointer
 * @return Number of elements in the list
 */
#define ss_list_size(l) ((l)->size)

#endif /* SS_LIST_H */
