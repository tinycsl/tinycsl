/**
 * @file ss_bigbitset.h
 * @brief Dynamic bitset implementation for handling large bit arrays
 *
 * Provides memory-efficient storage and manipulation of bit sets that can grow as needed.
 * Supports basic bit operations including set, clear, toggle and check operations.
 *
 * @author trywen@qq.com
 * @version 1.1
 * @date 2024-12-20
 */

#ifndef SS_BIGBITSET_H
#define SS_BIGBITSET_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "ss_types.h"

#if defined(__GNUC__) || defined(__clang__)
#include <inttypes.h> // For PRId32, PRId64
#endif

#define UINT_WIDTH                                                                                 \
    (sizeof(uint8_t) * 8) /**< The number of bits in a single `unsigned int`, platform-dependent   \
                           */
#define UINT_WIDTH_DEC (UINT_WIDTH - 1) /**< UINT_WIDTH - 1  */

    /**
     * @struct ss_bigbitset_t
     * @brief A structure representing a dynamic bitset.
     *
     * This structure holds a dynamically allocated array of unsigned integers (`unsigned int`),
     * which are used to store the individual bits of the bitset. The size of the bitset is
     * specified at runtime and determines how many bits the bitset can store.
     */
    struct ss_bigbitset_s
    {
        uint8_t* bits;  /**< Pointer to bit storage array */
        int bits_size;  /**< Total number of bits in the set */
        int bytes_size; /**< Total allocated bytes (bits_size/8 + 1) */
    };

    /**
     * @brief Creates a new bitset with the specified number of bits.
     *
     * This function allocates memory for a new bitset and initializes all bits to 0.
     * The bitset size is determined by the `bit_count` parameter.
     *
     * @param bit_count The number of bits to be allocated for the bitset.
     * @return A pointer to the newly created bitset, or NULL if memory allocation fails.
     */
    /**
     * @brief Creates a new dynamically allocated bitset
     * @param bit_count Initial number of bits to allocate
     * @return Pointer to allocated bitset, NULL on allocation failure
     */
    ss_bigbitset_t* ss_bigbitset_create(int bit_count);

    /**
     * @brief Releases all resources associated with a bitset
     * @param b Bitset to destroy
     */
    void ss_bigbitset_free(ss_bigbitset_t* b);

    /**
     * @brief Sets a bit at a specified position to 1.
     *
     * This function sets the bit at the given position in the bitset to 1.
     *
     * @param b A pointer to the bitset.
     * @param position The position of the bit to set.
     */
    void ss_bigbitset_set(ss_bigbitset_t* b, int position);

    /**
     * @brief Clears a bit at a specified position (sets it to 0).
     *
     * This function clears the bit at the given position in the bitset.
     *
     * @param b A pointer to the bitset.
     * @param position The position of the bit to clear.
     */
    void ss_bigbitset_clear(ss_bigbitset_t* b, int position);

    void ss_bigbitset_clear_all(ss_bigbitset_t* b);

    /**
     * @brief Toggles the bit at a specified position (flips its value).
     *
     * This function toggles the bit at the given position in the bitset.
     * If the bit is 1, it is set to 0; if the bit is 0, it is set to 1.
     *
     * @param b A pointer to the bitset.
     * @param position The position of the bit to toggle.
     */
    void ss_bigbitset_toggle(ss_bigbitset_t* b, int position);

    /**
     * @brief Checks the value of a bit at a specified position.
     *
     * This function checks whether the bit at the given position in the bitset is set to 1 or 0.
     *
     * @param b A pointer to the bitset.
     * @param position The position of the bit to check.
     * @return 1 if the bit is set to 1, 0 if the bit is set to 0.
     */
    bool ss_bigbitset_check(const ss_bigbitset_t* b, int position);

#define ss_bigbitset_bits_size(b) (b)->bits_size
#define ss_bigbitset_bytes_size(b) (b)->bytes_size

    /**
     * @brief Prints the entire bitset.
     *
     * This function prints all the bits in the bitset to the standard output.
     * Each bit is printed as a character '0' or '1'.
     *
     * @param b A pointer to the bitset to print.
     */
    void ss_bigbitset_print(const ss_bigbitset_t* b, int mod, int max_bits);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // SS_BIGBITSET_H
