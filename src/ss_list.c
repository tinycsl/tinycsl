#include "ss_alloc.h"
#include "ss_compare.h"
#include "ss_list.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static ss_list_node_t* ss_list_node_new(ss_list_t* l, void* data)
{
    ss_list_node_t* node;
    node = (ss_list_node_t*)ss_malloc(sizeof(ss_list_node_t));
    if (node != NULL)
    {
        node->next = NULL;
        node->prev = NULL;
        if (data != NULL)
        {
            node->data = ss_malloc(l->el_size);
            if (node->data == NULL)
            {
                ss_free(node);
                return NULL;
            }
            memcpy(node->data, data, l->el_size);
        }
        else
        {
            node->data = NULL;
        }
    }
    return node;
}

ss_list_t* ss_list_create(size_t el_size)
{
    ss_list_t* l = (ss_list_t*)ss_malloc(sizeof(ss_list_t));
    assert(l);
    ss_list_init(l, el_size);
    return l;
}

void ss_list_free(ss_list_t* l)
{
    assert(l);
    ss_list_destroy(l);
    ss_free(l);
}

void ss_list_init(ss_list_t* l, size_t el_size)
{
    assert(l);
    memset(l, 0, sizeof(ss_list_t));
    l->el_size = el_size ? el_size : sizeof(void*);
}

void ss_list_destroy(ss_list_t* l) { ss_list_clear(l); }

ss_bool_t ss_list_iterate(ss_list_t* l, ss_list_iterate_cb_f cb, void* userdata,
                          size_t userdata_size)
{
    assert(l);
    size_t i;
    ss_list_node_t* node = l->first;
    for (i = 0; i < l->size; i++)
    {
        if (cb(l, i, node, userdata, userdata_size))
        {
            return SS_TRUE;
        }
        node = node->next;
    }
    return SS_FALSE;
}

ss_bool_t ss_list_insert(ss_list_t* l, void* data)
{
    assert(l);
    ss_list_node_t* newel = ss_list_node_new(l, data);
    if (newel)
    {
        if (l->size == 0)
        {
            l->last = newel;
        }
        else
        {
            newel->next = l->first;
            l->first->prev = newel;
        }
        l->first = newel;
        l->size++;
        return SS_TRUE;
    }
    return SS_FALSE;
}

ss_bool_t ss_list_insert_at(ss_list_t* l, void* data, size_t index)
{
    assert(l);
    if (l->size == 0)
    {
        if (index == 0)
        {
            return ss_list_insert(l, data);
        }
        return SS_FALSE;
    }

    if (index >= l->size)
    {
        return ss_list_push(l, data); // When index is larger than current size, append at the end
    }

    ss_list_node_t* node = l->first;
    size_t i;
    for (i = 0; i < index; i++)
    {
        node = node->next;
    }

    ss_list_node_t* newel = ss_list_node_new(l, data);
    if (!newel) return SS_FALSE;

    newel->next = node;
    newel->prev = node->prev;
    node->prev = newel;
    if (newel->prev)
    {
        newel->prev->next = newel;
    }
    else
    {
        l->first = newel;
    }
    l->size++;
    return SS_TRUE;
}

ss_bool_t ss_list_push(ss_list_t* l, void* data)
{
    assert(l);
    ss_list_node_t* newel = ss_list_node_new(l, data);
    if (newel)
    {
        if (l->size == 0)
        {
            l->first = newel;
        }
        else
        {
            l->last->next = newel;
            newel->prev = l->last;
        }
        l->last = newel;
        l->size++;
        return SS_TRUE;
    }
    return SS_FALSE;
}

void ss_list_pop(ss_list_t* l)
{
    assert(l);
    ss_list_node_t* node = l->first;
    if (!node)
    {
        return;
    }
    l->first = node->next;
    if (l->first)
    {
        l->first->prev = NULL;
    }
    else
    {
        l->last = NULL; // List is now empty
    }
    l->size--;
    if (node->data)
    {
        ss_free(node->data);
    }
    ss_free(node);
}

void ss_list_pop_back(ss_list_t* l)
{
    assert(l);
    ss_list_node_t* node = l->last;
    if (!node)
    {
        return;
    }
    l->last = node->prev;
    if (l->last)
    {
        l->last->next = NULL;
    }
    else
    {
        l->first = NULL; // List is now empty
    }
    l->size--;
    if (node->data)
    {
        ss_free(node->data);
    }
    ss_free(node);
}

void ss_list_clear(ss_list_t* l)
{
    assert(l);
    size_t i;
    ss_list_node_t* node = l->first;
    ss_list_node_t* next = NULL;
    for (i = 0; i < l->size; i++)
    {
        next = node->next;
        if (node->data)
        {
            ss_free(node->data);
        }
        ss_free(node);
        node = next;
    }
    l->first = NULL;
    l->last = NULL;
    l->size = 0;
}

ss_bool_t ss_list_remove(ss_list_t* l, void* data)
{
    assert(l);
    if (!data)
    {
        return SS_FALSE;
    }
    size_t i;
    ss_list_node_t* node = l->first;
    for (i = 0; i < l->size; i++)
    {
        if (ss_compare_mem(node->data, l->el_size, data, l->el_size) == 0)
        {
            if (node->prev)
            {
                node->prev->next = node->next;
            }
            if (node->next)
            {
                node->next->prev = node->prev;
            }
            if (l->first == node)
            {
                l->first = node->next;
            }
            if (l->last == node)
            {
                l->last = node->prev;
            }

            if (node->data)
            {
                ss_free(node->data);
            }
            ss_free(node);
            l->size--;

            return SS_TRUE;
        }
        node = node->next;
    }
    return SS_FALSE;
}

ss_bool_t ss_list_set(ss_list_t* l, void* odata, void* ndata)
{
    assert(l);
    size_t i;
    ss_list_node_t* node = l->first;
    for (i = 0; i < l->size; i++)
    {
        if (ss_compare_mem(node->data, l->el_size, odata, l->el_size) == 0)
        {
            if (ndata)
            {
                memcpy(node->data, ndata, l->el_size);
            }
            else
            {
                if (node->data)
                {
                    ss_free(node->data);
                }
                node->data = NULL;
            }
            return SS_TRUE;
        }
        node = node->next;
    }
    return SS_FALSE;
}
