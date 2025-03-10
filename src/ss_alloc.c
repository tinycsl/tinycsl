#include "ss_alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void* __malloc(unsigned long size) { return malloc(size); }

static void* __realloc(void* ptr, unsigned long malloc_size, unsigned long realloc_size)
{
    if (realloc_size <= malloc_size)
    {
        return ptr;
    }
    void* new_ptr = ss_malloc(realloc_size);
    if (!new_ptr)
    {
        return NULL;
    }
    memcpy(new_ptr, ptr, malloc_size);
    ss_free(ptr);
    return new_ptr;
}

static void __free(void* p) { free(p); }

static ss_malloc_f _malloc_f = __malloc;
static ss_realloc_f _realloc_f = __realloc;
static ss_free_f _free_f = __free;

void ss_alloc_init(ss_malloc_f malloc_f, ss_free_f free_f, ss_realloc_f realloc_f)
{
    _malloc_f = malloc_f;
    _free_f = free_f;
    if (realloc_f)
    {
        _realloc_f = realloc_f;
    }
}

void* ss_malloc(unsigned long size)
{
    if (size == 0)
    {
        return NULL;
    }
    return _malloc_f(size);
}

void* ss_realloc(void* ptr, unsigned long malloc_size, unsigned long realloc_size)
{
    if (realloc_size <= malloc_size)
    {
        return ptr;
    }
    return _realloc_f(ptr, malloc_size, realloc_size);
}

void ss_free(void* p)
{
    if (p)
    {
        _free_f(p);
    }
    else
    {
        printf("ss_free: p is NULL\n");
    }
};