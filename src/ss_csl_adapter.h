
#ifndef SS_STD_ADAPTER_H
#define SS_STD_ADAPTER_H

#include "ss_version.h"

#if USE_STANDARD_TOLOWER
#define ss_csl_tolower(c) tolower((int)(c))
#else
#define ss_csl_tolower(c) ((((c) > 'A') && ((c) < 'Z')) ? ((c) - 'A' + 'a') : (c))
#endif

#if USE_STANDARD_TOUPPER
#define ss_csl_toupper(c) toupper((int)(c))
#else
#define ss_csl_toupper(c) ((((c) >= 'a') && ((c) <= 'z')) ? ((c) - 'a' + 'A') : (c))
#endif

static inline int ss_csl_memicmp(const void* lbuf, const void* rbuf, int count)
{
    int f = 0;
    int l = 0;
    while (count--)
    {
        if ((*(unsigned char*)lbuf == *(unsigned char*)rbuf) ||
            ((f = ss_csl_tolower(*(unsigned char*)lbuf)) ==
             (l = ss_csl_tolower(*(unsigned char*)rbuf))))
        {
            lbuf = (char*)lbuf + 1;
            rbuf = (char*)rbuf + 1;
        }
        else
        {
            break;
        }
    }
    return f - l;
}

static inline int ss_csl_stricmp(const char* dst, const char* src)
{
    int ch1 = 0, ch2 = 0;
    do
    {
        if (((ch1 = (unsigned char)(*(dst++))) >= 'A') && (ch1 <= 'Z')) ch1 += 0x20;
        if (((ch2 = (unsigned char)(*(src++))) >= 'A') && (ch2 <= 'Z')) ch2 += 0x20;
    } while (ch1 && (ch1 == ch2));
    return (ch1 - ch2);
}

static inline char* ss_csl_itoa(long num, char* str, int radix)
{
    char index[] = "0123456789ABCDEF";
    unsigned unum;
    int i = 0, j, k;
    if (radix == 10 && num < 0)
    {
        unum = (unsigned)-num;
        str[i++] = '-';
    }
    else
    {
        unum = (unsigned)num;
    }
    do
    {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while (unum);
    str[i] = '\0';

    if (str[0] == '-')
    {
        k = 1;
    }
    else
    {
        k = 0;
    }

    for (j = k; j <= (i - 1) / 2; j++)
    {
        char temp;
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
    return str;
}

#endif