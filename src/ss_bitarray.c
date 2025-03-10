#include "ss_bitarray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ss_bitarray_t* ss_bitarray_create(int num_elements, int bit_width)
{
    if (num_elements <= 0 || bit_width <= 0 || bit_width > (int)sizeof(ss_bitarray_value_t) * 8)
    {
        fprintf(stderr, "ss_bitarray_create: Invalid parameter (num_elements=%d, bit_width=%d)\n",
                num_elements, bit_width);
        return NULL;
    }
    ss_bitarray_t* bf = (ss_bitarray_t*)malloc(sizeof(ss_bitarray_t));
    if (!bf)
    {
        fprintf(stderr, "ss_bitarray_create: Memory allocation failed\n");
        return NULL;
    }
    bf->num_elements = num_elements;
    bf->bit_width = bit_width;
    bf->total_bits = num_elements * bit_width;
    bf->total_bytes = (bf->total_bits + 7) / 8; /* Round up to the nearest byte */
    bf->data = (uint8_t*)calloc(bf->total_bytes, sizeof(uint8_t));
    if (!bf->data)
    {
        fprintf(stderr, "ss_bitarray_create: Data array allocation failed\n");
        free(bf);
        return NULL;
    }
    return bf;
}

void ss_bitarray_free(ss_bitarray_t* bf)
{
    if (bf)
    {
        free(bf->data);
        free(bf);
    }
}

void ss_bitarray_set(ss_bitarray_t* bf, int index, ss_bitarray_value_t value)
{
    if (!bf) return;
    if (index < 0 || index >= bf->num_elements)
    {
        fprintf(stderr, "ss_bitarray_set: Index %d out of range\n", index);
        return;
    }
    /* Check if the value is within valid range when bit_width < 64 */
    if (bf->bit_width < 64 && value >= (1ULL << bf->bit_width))
    {
        fprintf(stderr, "ss_bitarray_set: Value %lu exceeds the range of bit width %d\n",
                (unsigned long)value, bf->bit_width);
        return;
    }
    // Calculate the starting bit offset in the bit stream
    int bit_offset = index * bf->bit_width;
    int bits_remaining = bf->bit_width;
    int byte_index = bit_offset / 8;
    int bit_index = bit_offset % 8;
    ss_bitarray_value_t v = value;
    // May span multiple bytes for writing
    while (bits_remaining > 0)
    {
        int bits_in_this_byte = 8 - bit_index;
        int bits_to_write =
            (bits_remaining < bits_in_this_byte) ? bits_remaining : bits_in_this_byte;
        uint8_t mask = ((1U << bits_to_write) - 1) << bit_index;
        // Clear existing bits in the area
        bf->data[byte_index] &= ~mask;
        // Write lower bits_to_write bits to current byte
        bf->data[byte_index] |= ((uint8_t)(v & ((1ULL << bits_to_write) - 1)) << bit_index) & mask;
        v >>= bits_to_write;
        bits_remaining -= bits_to_write;
        byte_index++;
        bit_index = 0;
    }
}

ss_bitarray_value_t ss_bitarray_get(const ss_bitarray_t* bf, int index)
{
    if (!bf) return 0;
    if (index < 0 || index >= bf->num_elements)
    {
        fprintf(stderr, "ss_bitarray_get: Index %d is out of range\n", index);
        return 0;
    }
    int bit_offset = index * bf->bit_width;
    int bits_remaining = bf->bit_width;
    int byte_index = bit_offset / 8;
    int bit_index = bit_offset % 8;
    ss_bitarray_value_t result = 0;
    int shift = 0;
    // May span multiple bytes for reading
    while (bits_remaining > 0)
    {
        int bits_in_this_byte = 8 - bit_index;
        int bits_to_read =
            (bits_remaining < bits_in_this_byte) ? bits_remaining : bits_in_this_byte;
        uint8_t mask = ((1U << bits_to_read) - 1) << bit_index;
        uint8_t part = (bf->data[byte_index] & mask) >> bit_index;
        result |= ((ss_bitarray_value_t)part) << shift;
        shift += bits_to_read;
        bits_remaining -= bits_to_read;
        byte_index++;
        bit_index = 0;
    }
    return result;
}

void ss_bitarray_print(const ss_bitarray_t* bf)
{
    if (!bf) return;
    int i;
    for (i = 0; i < bf->num_elements; i++)
    {
        ss_bitarray_value_t value = ss_bitarray_get(bf, i);
        printf("%lu ", (long unsigned)value);
    }
    printf("\n");
}
