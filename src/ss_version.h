#ifndef SS_VERSION_H
#define SS_VERSION_H

#define SS_VERSION "0.1.0"

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define SS_C11_ENABLED
#endif

#if defined(SS_C11_ENABLED) || defined(__GNUC__)
#define SS_STRUCT_LITERAL_ENABLED
#endif

#ifdef HAVE_CTYPE_H
#include <ctype.h>
#define USE_STANDARD_TOLOWER 1
#define USE_STANDARD_TOUPPER 1
#elif defined(_MSC_VER) || defined(__GNUC__) || defined(__clang__)
#include <ctype.h>
#define USE_STANDARD_TOLOWER 1
#define USE_STANDARD_TOUPPER 1
#else
#define USE_STANDARD_TOLOWER 0
#define USE_STANDARD_TOUPPER 0
#endif

#endif