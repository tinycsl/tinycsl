/**
 * @file ss_slice.h
 * @brief Slice operations for memory buffers
 * @author trywen@qq.com
 * @date 2011-09-02
 *
 * Provides non-owning view into memory buffers with various utility functions
 * including trimming, prefix checking and debug output.
 */

#ifndef SS_SLICE_H
#define SS_SLICE_H

#include "ss_types.h"

/**
 * @struct ss_slice_s
 * @brief Non-owning view of a memory buffer
 * 
 * @var data Pointer to memory buffer
 * @var size Size of buffer in bytes
 */
struct ss_slice_s
{
    const void* data;
    size_t size;
};

/**
 * @brief Check if slice starts with given prefix
 * @param d Slice to check
 * @param prefix Prefix data to match
 * @param prefix_len Length of prefix data
 * @return SS_TRUE if slice starts with prefix, SS_FALSE otherwise
 */
ss_bool_t ss_slice_has_prefix(const ss_slice_t* d, const void* prefix, size_t prefix_len);
ss_bool_t ss_slice_has_prefix_case(const ss_slice_t* d, const void* prefix, size_t prefix_len);

ss_slice_t ss_slice_ltrim(const ss_slice_t* d);
ss_slice_t ss_slice_rtrim(const ss_slice_t* d);
ss_slice_t ss_slice_trim(const ss_slice_t* d);

#define ss_slice_empty(d) (!(d) || !((d)->data) || (d)->size == 0)

/**
 * @brief Debug print slice contents
 * @param tag Debug tag identifier
 * @param data Slice to display
 */
static inline void ss_slice_debug(const char* tag, const ss_slice_t* data)
{
    if (!data->data)
    {
        printf("%s(%u)=>NULL", tag, (unsigned int)data->size);
    }
    else
    {
        size_t i;
        const char* d = (const char*)data->data;
        printf("%s(%u)=>", tag, (unsigned int)data->size);
        for (i = 0; i < data->size; i++)
        {
            printf("%c", d[i]);
        }
    }
}

#endif