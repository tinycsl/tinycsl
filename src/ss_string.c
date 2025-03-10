
#include "ss_alloc.h"
#include "ss_csl_adapter.h"
#include "ss_string.h"
#include "ss_string_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Set string termination marker
// #define _ss_string_end_set(str) ((char*)str->data.elts)[str->data.size * str->data.el_size] =
// '\0'; #define _ss_string_end_set(str)
//     {
//         // static char lf[1] = {0};
//         // ss_array_push(&((str)->data), &lf);
//         // ss_array_resize_zero(&((str)->data), 1);
//         ((char*)(str->data.elts))[str->data.size * str->data.el_size] = 0;
//     }

// ss_array_reserve(ss_array_t* a, size_t n)

// Set string termination marker
static inline void _ss_string_end_set(ss_string_t* str)
{
    ss_array_reserve_append(&(str->data), 1);
    ((char*)(str->data.elts))[str->data.size * str->data.el_size] = '\0';
}

void ss_string_init(ss_string_t* str, const char* data)
{
    str->_destroy = SS_FALSE;
    size_t size = data ? strlen(data) : 0;
    ss_array_init(&(str->data), sizeof(char), size);
    if (data)
    {
        ss_array_push_n(&(str->data), (void*)data, size);
    }
    _ss_string_end_set(str);
}

ss_string_t* ss_string_create(const char* data)
{
    ss_string_t* str = (ss_string_t*)ss_malloc(sizeof(ss_string_t));
    ss_string_init(str, data);
    return str;
}

void ss_string_destroy(ss_string_t* str)
{
    ss_array_destroy(&str->data);
    str->_destroy = SS_TRUE;
}

void ss_string_free(ss_string_t* str)
{
    if (!str->_destroy)
    {
        ss_string_destroy(str);
    }
    ss_free(str);
}

void ss_string_clear(ss_string_t* str)
{
    ss_array_clear(&str->data);
    _ss_string_end_set(str);
}

ss_string_t* ss_string_append_cstr(ss_string_t* str, const char* data)
{
    ss_array_push_n(&(str->data), (void*)data, strlen(data));
    _ss_string_end_set(str);
    return str;
}

ss_string_t* ss_string_append_str(ss_string_t* str, ss_string_t* data)
{
    ss_array_push_n(&(str->data), data->data.elts, ss_string_size(data));
    _ss_string_end_set(str);
    return str;
}

ss_string_t* ss_string_append(ss_string_t* str, const char* data, size_t size)
{
    ss_array_push_n(&(str->data), (void*)data, size);
    _ss_string_end_set(str);
    return str;
}

ss_string_t* ss_string_append_char(ss_string_t* str, char c)
{
    ss_array_push(&(str->data), &c);
    _ss_string_end_set(str);
    return str;
}

ss_string_t* ss_string_append_long(ss_string_t* str, long data)
{
    char buff[32];
    ss_csl_itoa(data, buff, 10);
    ss_string_append_cstr(str, buff);
    return str;
}

ss_string_t* ss_string_set_cstr(ss_string_t* str, const char* data)
{
    ss_array_clear(&str->data);
    ss_array_push_n(&(str->data), (void*)data, strlen(data));
    _ss_string_end_set(str);
    return str;
}

ss_string_t* ss_string_set_str(ss_string_t* str, ss_string_t* data)
{
    ss_array_clear(&str->data);
    ss_array_push_n(&(str->data), data->data.elts, ss_string_size(data));
    _ss_string_end_set(str);
    return str;
}

ss_string_t* ss_string_set(ss_string_t* str, const char* data, size_t size)
{
    ss_array_clear(&str->data);
    ss_array_push_n(&(str->data), (void*)data, size);
    _ss_string_end_set(str);
    return str;
}

ss_string_t* ss_string_connect_cstr(ss_string_t* str1, const char* data)
{
    ss_string_t* str = ss_string_create(NULL);
    ss_string_append_str(str, str1);
    ss_string_append_cstr(str, data);
    return str;
}

ss_string_t* ss_string_replace_cstr(ss_string_t* str, const char* src, const char* dst)
{
    ss_string_t* newstr = ss_string_create(NULL);
    ss_string_utils_replace_all(ss_string_to_cstr(str), src, dst, newstr);
    ss_string_set_str(str, newstr);
    ss_string_free(newstr);

    return str;
}
