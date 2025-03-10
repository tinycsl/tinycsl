#include "ss_bigbitset.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ss_bigbitset_t* ss_bigbitset_create(int bit_count)
{
    if (bit_count <= 0)
    {
        return NULL;
    }

    ss_bigbitset_t* b = (ss_bigbitset_t*)malloc(sizeof(ss_bigbitset_t));
    if (!b)
    {
        fprintf(stderr, "Memory allocation failed, malloc size: %ld\n",
                (long)sizeof(ss_bigbitset_t));
        return NULL;
    }

    b->bits_size = bit_count;
    b->bytes_size = (bit_count + UINT_WIDTH - 1) / UINT_WIDTH * sizeof(uint8_t);

    b->bits = (uint8_t*)calloc(b->bytes_size, sizeof(uint8_t));
    if (!b->bits)
    {
        fprintf(stderr, "Memory allocation failed, calloc size: %d\n", b->bytes_size);
        free(b);
        return NULL;
    }

    return b;
}

void ss_bigbitset_free(ss_bigbitset_t* b)
{
    if (b)
    {
        free(b->bits);
        free(b);
    }
}

void ss_bigbitset_set(ss_bigbitset_t* b, int position)
{
    if (position < 0 || position >= b->bits_size)
    {
        fprintf(stderr, "Position out of bounds\n");
        return;
    }

    int index = position / UINT_WIDTH;
    int bit_position = UINT_WIDTH_DEC - position % UINT_WIDTH;
    b->bits[index] |= (1U << bit_position);
}

void ss_bigbitset_clear(ss_bigbitset_t* b, int position)
{
    if (position < 0 || position >= b->bits_size)
    {
        fprintf(stderr, "Position out of bounds\n");
        return;
    }

    int index = position / UINT_WIDTH;
    int bit_position = UINT_WIDTH_DEC - position % UINT_WIDTH;
    b->bits[index] &= ~(1U << bit_position);
}

void ss_bigbitset_clear_all(ss_bigbitset_t* b) { memset(b->bits, 0, b->bytes_size); }

void ss_bigbitset_toggle(ss_bigbitset_t* b, int position)
{
    if (position < 0 || position >= b->bits_size)
    {
        fprintf(stderr, "Position out of bounds\n");
        return;
    }

    int index = position / UINT_WIDTH;
    int bit_position = UINT_WIDTH_DEC - position % UINT_WIDTH;
    b->bits[index] ^= (1U << bit_position);
}

bool ss_bigbitset_check(const ss_bigbitset_t* b, int position)
{
    if (position < 0 || position >= b->bits_size)
    {
        return false; // Error flag
    }

    int index = position / UINT_WIDTH;
    int bit_position = UINT_WIDTH_DEC - position % UINT_WIDTH;
    return (b->bits[index] & (1U << bit_position)) != 0;
}

void ss_bigbitset_print(const ss_bigbitset_t* b, int mod, int max_bits)
{
    int i;
    for (i = 0; i < b->bits_size; i++)
    {
        if (i >= max_bits)
        {
            break;
        }
        int res = ss_bigbitset_check(b, i);
        switch (mod)
        {
        case 1:
            printf(res ? "*" : " ");
            break;
        default:
            printf("%d", res);
            break;
        }
    }
    printf("\n");
}
