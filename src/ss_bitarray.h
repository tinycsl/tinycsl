#ifndef SS_BITARRAY_H
#define SS_BITARRAY_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdint.h>

#include "ss_types.h"

#ifndef ss_bitarray_value_t
#define ss_bitarray_value_t uint8_t
#endif

    /**
     * @brief Dynamic bit array structure with configurable bit width (1-64 bits) per element
     * @author trywen@qq.com
     * @date 2011-09-02
     * 
     * @var data Byte array storing all bit elements
     * @var num_elements Number of data elements
     * @var bit_width Bit width per element (1-64)
     * @var total_bits Total bits = num_elements * bit_width
     * @var total_bytes Allocated bytes count
     */
    struct ss_bitarray_s
    {
        uint8_t* data;    /**< Byte array storing all bit elements */
        int num_elements; /**< Number of data elements */
        int bit_width;    /**< Bit width per element (1-64) */
        int total_bits;   /**< Total bits = num_elements * bit_width */
        int total_bytes;  /**< Allocated bytes count */
    };

    /**
     * @brief Create a new dynamic bit array
     *
     * @param num_elements Number of data elements
     * @param bit_width Bit width per element (1-64)
     * @return ss_bitarray_t* Pointer to the newly created bit array, returns NULL on failure
     */
    /**
     * @brief Create a new dynamic bit array
     * @param num_elements Number of data elements
     * @param bit_width Bit width per element (1-64)
     * @return Pointer to newly created bit array, NULL on failure
     */
    ss_bitarray_t* ss_bitarray_create(int num_elements, int bit_width);

    /**
     * @brief Free memory occupied by dynamic bitarray
     *
     * @param bf Pointer to the bitarray to be freed
     */
    void ss_bitarray_free(ss_bitarray_t* bf);

    /**
     * @brief Set value for specified data element
     *
     * @param bf Pointer to bitarray
     * @param index Element index (0 to num_elements-1)
     * @param value Value to set. When bit_width <64, must be in [0, (1ULL << bit_width)-1]
     *              When bit_width=64, valid range is 0~UINT64_MAX
     */
    /**
     * @brief Set value for specified data element
     * @param bf Pointer to bitarray
     * @param index Element index (0 to num_elements-1)
     * @param value Value to set. When bit_width <64, must be in [0, (1ULL << bit_width)-1]
     *              When bit_width=64, valid range is 0~UINT64_MAX
     */
    void ss_bitarray_set(ss_bitarray_t* bf, int index, ss_bitarray_value_t value);

    /**
     * @brief Get value of specified element
     *
     * @param bf Pointer to bitarray
     * @param index Element index
     * @return uint64_t The stored value of the element
     */
    ss_bitarray_value_t ss_bitarray_get(const ss_bitarray_t* bf, int index);

    /**
     * @brief Print all element values as unsigned integers
     *
     * @param bf Pointer to bitarray
     */
    void ss_bitarray_print(const ss_bitarray_t* bf);

#ifdef __cplusplus
}
#endif

#endif // ss_bitarray_H
