#include "ss_csl_adapter.h"
#include "ss_slice.h"
#include <string.h>

static inline ss_bool_t _is_space(char c);
static inline const char* _ltrim(const char* s);
static inline size_t _rtrim(const char* s, size_t s_len);

ss_bool_t ss_slice_has_prefix(const ss_slice_t* d, const void* prefix, size_t prefix_len)
{
    return d->size >= prefix_len && memcmp(d->data, prefix, prefix_len) == 0;
}

ss_bool_t ss_slice_has_prefix_case(const ss_slice_t* d, const void* prefix, size_t prefix_len)
{
    return d->size >= prefix_len && ss_csl_memicmp(d->data, prefix, prefix_len) == 0;
}

ss_slice_t ss_slice_ltrim(const ss_slice_t* d)
{
    const char* data = _ltrim((const char*)d->data);
#ifdef SS_STRUCT_LITERAL_ENABLED
    return (ss_slice_t){.data = data, .size = d->size - (data - (const char*)d->data)};
#else
    ss_slice_t ret;
    ret.data = data;
    ret.size = d->size - (data - (const char*)d->data);
    return ret;
#endif
}

ss_slice_t ss_slice_rtrim(const ss_slice_t* d)
{
    size_t size = _rtrim((const char*)d->data, d->size);
#ifdef SS_STRUCT_LITERAL_ENABLED
    return (ss_slice_t){.data = d->data, .size = size};
#else
    ss_slice_t ret;
    ret.data = d->data;
    ret.size = size;
    return ret;
#endif
}

ss_slice_t ss_slice_trim(const ss_slice_t* d)
{
    const char* data = _ltrim((const char*)d->data);
    size_t size = _rtrim(data, d->size - (data - (const char*)d->data));
#ifdef SS_STRUCT_LITERAL_ENABLED
    return (ss_slice_t){.data = data, .size = size};
#else
    ss_slice_t ret;
    ret.data = data;
    ret.size = size;
    return ret;
#endif
}

static inline ss_bool_t _is_space(char c)
{
    switch (c)
    {
    case ' ':
        return SS_TRUE;
    case '\r':
        return SS_TRUE;
    case '\n':
        return SS_TRUE;
    case '\t':
        return SS_TRUE;
    case '\v':
        return SS_TRUE;
    case '\f':
        return SS_TRUE;
    case (char)0x85:
        return SS_TRUE;
    case (char)0xA0:
        return SS_TRUE;
    }
    return SS_FALSE;
}

// Return new position pointer
static inline const char* _ltrim(const char* s)
{
    while (_is_space(*s))
    {
        s++;
    }
    return s;
}

// Return new length
static inline size_t _rtrim(const char* s, size_t s_len)
{
    if (s_len == 0)
    {
        return 0;
    }
    size_t i = s_len - 1;
    while (_is_space(s[i]))
    {
        i--;
    };
    return i + 1;
}