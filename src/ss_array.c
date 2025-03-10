#include "ss_alloc.h"
#include "ss_array.h"
#include "ss_compare.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static ss_bool_t _ss_array_ensure_capacity(ss_array_t* a, size_t min_capacity);
static void _ss_array_sort(ss_array_t* a, ss_compare_f compare, int low, int high);

ss_array_t* ss_array_create(size_t el_size, size_t capacity)
{
    ss_array_t* a = (ss_array_t*)ss_malloc(sizeof(ss_array_t));
    if (!ss_array_init(a, el_size, capacity))
    {
        ss_free(a);
        return NULL;
    }
    return a;
}

void ss_array_free(ss_array_t* a)
{
    ss_array_destroy(a);
    ss_free(a);
}

ss_bool_t ss_array_init(ss_array_t* a, size_t el_size, size_t capacity)
{
    // assert(a);
    a->size = 0;
    a->el_size = el_size ? el_size : sizeof(void*);
    a->capacity = capacity == 0 ? SS_DEFAULT_ARRAY_CAPACITY : capacity;
    a->elts = ss_malloc(a->capacity * a->el_size);
    if (!a->elts)
    {
        return SS_FALSE;
    }

    return _ss_array_ensure_capacity(a, capacity);
}

void ss_array_destroy(ss_array_t* a) { ss_free(a->elts); }

ss_bool_t ss_array_reserve(ss_array_t* a, size_t n)
{
    assert(a);
    if (n == 0)
    {
        return SS_TRUE;
    }
    if (_ss_array_ensure_capacity(a, n))
    {
        return SS_TRUE;
    }
    return SS_FALSE;
}

ss_bool_t ss_array_reserve_append(ss_array_t* a, size_t n)
{
    assert(a);
    if (n == 0)
    {
        return SS_TRUE;
    }
    if (_ss_array_ensure_capacity(a, a->size + n))
    {
        return SS_TRUE;
    }
    return SS_FALSE;
}

ss_bool_t ss_array_resize(ss_array_t* a, size_t n)
{
    assert(a);
    if (_ss_array_ensure_capacity(a, n))
    {
        a->size = n;
        return SS_TRUE;
    }
    else
    {
        return SS_FALSE;
    }
}

ss_bool_t ss_array_push(ss_array_t* a, const void* value)
{
    assert(a);
    assert(value);
    if (_ss_array_ensure_capacity(a, a->size + 1))
    {
        memcpy((char*)a->elts + a->size * a->el_size, value, a->el_size);
        a->size++;
        return SS_TRUE;
    }
    return SS_FALSE;
}

ss_bool_t ss_array_push_n(ss_array_t* a, const void* values, size_t n)
{
    assert(a);
    assert(values);
    if (n == 0)
    {
        return SS_TRUE;
    }
    if (_ss_array_ensure_capacity(a, a->size + n))
    {
        memcpy((char*)a->elts + a->size * a->el_size, values, n * a->el_size);
        a->size += n;
        return SS_TRUE;
    }
    return SS_FALSE;
}

ss_bool_t ss_array_replace(ss_array_t* a, const void* ovalue, const void* nvalue)
{
    assert(a);
    assert(ovalue);
    assert(nvalue);
    size_t i;
    for (i = 0; i < ss_array_size(a); i++)
    {
        void* value = ss_array_at(a, i);
        if (ss_compare_mem(value, a->el_size, ovalue, a->el_size) == 0)
        {
            if (nvalue)
            {
                memcpy(value, nvalue, a->el_size);
            }
            else
            {
                memset(value, 0, a->el_size);
            }
            return SS_TRUE;
        }
    }
    return SS_FALSE;
}

ss_bool_t ss_array_update(ss_array_t* a, size_t off, const void* start, size_t n)
{
    assert(a);
    assert(start);
    if (off + n > a->size)
    {
        return SS_FALSE;
    }
    memcpy(ss_array_at(a, off), start, n * a->el_size);
    return SS_TRUE;
}

ss_bool_t ss_array_set(ss_array_t* a, size_t idx, const void* value, void* out_old_value)
{
    assert(a);
    assert(value);
    if (idx >= a->size)
    {
        return SS_FALSE;
    }
    if (out_old_value)
    {
        memcpy(out_old_value, ss_array_at(a, idx), a->el_size);
    }
    memcpy(ss_array_at(a, idx), value, a->el_size);
    return SS_TRUE;
}

void* ss_array_get(ss_array_t* a, size_t idx, void* out_value)
{
    assert(a);
    if (idx >= a->size)
    {
        return NULL;
    }
    if (!out_value)
    {
        return NULL;
    }
    memcpy(out_value, ss_array_at(a, idx), a->el_size);
    return out_value;
}

static ss_bool_t _ss_array_ensure_capacity(ss_array_t* a, size_t min_capacity)
{
    if (min_capacity > a->capacity)
    {
        size_t new_capacity = (a->capacity * 3) / 2 + 1;
        if (new_capacity < min_capacity)
        {
            new_capacity = min_capacity; // + 1;
        }
        size_t mem_size = new_capacity * a->el_size;
        void* ptr = ss_realloc(a->elts, a->el_size * a->capacity, mem_size);
        if (!ptr)
        {
            ptr = ss_malloc(mem_size);
            if (!ptr)
            {
                return SS_FALSE;
            }
            memcpy(ptr, a->elts, a->el_size * a->capacity);
            ss_free(a->elts);
            a->elts = ptr;
        }
        else
        {
            a->elts = ptr;
        }
        a->capacity = new_capacity;
    }
    return SS_TRUE;
}

void ss_array_sort(ss_array_t* a, ss_compare_f compare)
{
    if (a->size > 1)
    {
        _ss_array_sort(a, compare, 0, a->size - 1);
    }
}

static void _ss_array_sort(ss_array_t* a, ss_compare_f compare, int low, int high)
{
    if (low < high)
    {
        int i = low;
        int j = high;

        SS_STACK_ALLOC(k, a->el_size);

        ss_array_get(a, low, k); // Retrieve the value, not the pointer
        while (i < j)
        {
            while (i < j && compare(ss_array_at(a, j), a->el_size, k, a->el_size) >= 0)
            { // Find first element smaller than k from right
                j--;
            }
            if (i < j)
            {
                ss_array_set(a, i, ss_array_at(a, j), NULL); // Swap values
                i++;
            }
            while (i < j && compare(ss_array_at(a, i), a->el_size, k, a->el_size) < 0)
            { // Find first element >= k from left
                i++;
            }
            if (i < j)
            {
                ss_array_set(a, j, ss_array_at(a, i), NULL); // Swap values
                j--;
            }
        }
        ss_array_set(a, i, k, NULL); // Swap values
        // Recursive calls
        _ss_array_sort(a, compare, low, i - 1);  // Sort left of k
        _ss_array_sort(a, compare, i + 1, high); // Sort right of k
    }
}
