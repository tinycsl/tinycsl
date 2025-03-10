#include "ss_alloc.h"
#include "ss_csl_adapter.h"
#include "ss_string.h"
#include "ss_string_utils.h"
#include <ctype.h>
#include <string.h>

ss_bool_t ss_string_utils_replace(const char* srcstr, const char* substr, const char* dststr,
                                  size_t count, ss_string_t* retstr)
{
    if (!srcstr || !substr || !dststr || !retstr)
    {
        return SS_FALSE;
    }

    if (ss_string_utils_empty(srcstr))
    {
        if (ss_string_utils_empty(substr))
        {
            ss_string_append_cstr(retstr, dststr);
        }
        return SS_TRUE;
    }

    if (ss_string_utils_empty(substr))
    {
        ss_string_append_cstr(retstr, srcstr);
        return SS_TRUE;
    }

    size_t substrlen = strlen(substr);
    size_t dststrlen = strlen(dststr);
    size_t i;
    const char* idx = strstr(srcstr, substr);
    const char* str = srcstr;

    for (i = 0; (count == 0 || i < count) && idx; i++)
    {
        ss_string_append(retstr, str, idx - str);
        ss_string_append(retstr, dststr, dststrlen);
        str = idx + substrlen;
        idx = strstr(str, substr);
    }

    ss_string_append_cstr(retstr, str);
    return SS_TRUE;
}

ss_bool_t _has_prefix(const void* s, int len, const void* prefix, int prefix_len)
{
    return len >= prefix_len && memcmp(s, prefix, prefix_len) == 0;
}

int ss_string_utils_index(const char* s, const char* substr)
{
    int n = strlen(substr);
    switch (n)
    {
    case 0:
        return 0;
    case 1:
    {
        int s_len = strlen(s);
        int i;
        for (i = 0; i < s_len; i++)
        {
            if (s[i] == substr[0])
            {
                return i;
            }
        }
        return -1;
    }
    default:
    {
        int s_len = strlen(s);
        if (s_len == n)
        {
            return memcmp(s, substr, n) == 0 ? 0 : -1;
        }
        else if (n > s_len)
        {
            return -1;
        }
        int i;
        int remainder = s_len;
        for (i = 0; remainder >= n; i++)
        {
            if (_has_prefix(&s[i], remainder, substr, n))
            {
                return i;
            }
            remainder = s_len - i;
        }
        return -1;
    }
    break;
    }
}

// Prefix match, case-insensitive
ss_bool_t ss_string_utils_has_prefix_case(const char* s, const char* prefix)
{
    size_t prefix_len = strlen(prefix);
    return strlen(s) >= prefix_len && ss_csl_memicmp(s, prefix, prefix_len) == 0;
}

// Prefix match, case-sensitive
ss_bool_t ss_string_utils_has_prefix(const char* s, const char* prefix)
{
    size_t prefix_len = strlen(prefix);
    return strlen(s) >= prefix_len && memcmp(s, prefix, prefix_len) == 0;
}

char* ss_string_utils_uppercase(const char* str, char* output)
{
    int i = 0;
    while (str[i])
    {
        output[i] = ss_csl_toupper(str[i]);
        i++;
    }
    output[i] = '\0';
    return output;
}
