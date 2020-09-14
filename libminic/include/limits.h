#ifndef __limits_h__
#define __limits_h__

#if defined(__aarch64__) || defined(_LP64)
#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define UCHAR_MAX 255

#define CHAR_MIN (-128)
#define CHAR_MAX 127

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX-1)
#define UINT_MAX (INT_MAX * 2U + 1)

#define LONG_MAX 9223372036854775807L
#define LONG_MIN (-LONG_MAX-1)

#define ULONG_MAX (LONG_MAX * 2UL + 1)

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX-1)
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1)

#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX

#define ULONG_LONG_MAX ULLONG_MAX
#endif

#if defined(__MICROBLAZE__) || (defined(__riscv) && __riscv_xlen == 32)
#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define UCHAR_MAX 255

#define CHAR_MIN (-128)
#define CHAR_MAX 127

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX-1)
#define UINT_MAX (INT_MAX * 2U + 1)

#define LONG_MAX 2147483647L
#define LONG_MIN (-LONG_MAX-1)

#define ULONG_MAX (LONG_MAX * 2UL + 1)

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX-1)
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1)

#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX

#define ULONG_LONG_MAX ULLONG_MAX
#endif

#ifdef __C3P3U__
#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define UCHAR_MAX 255

#define CHAR_MIN (-128)
#define CHAR_MAX 127

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX-1)
#define UINT_MAX (INT_MAX * 2U + 1)

#define LONG_MAX 2147483647L
#define LONG_MIN (-LONG_MAX-1)

#define ULONG_MAX (LONG_MAX * 2UL + 1)

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX-1)
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1)

#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX

#define ULONG_LONG_MAX ULLONG_MAX
#endif /* CRIS.  */

#ifdef __cris__
#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define UCHAR_MAX 255

#define CHAR_MIN (-128)
#define CHAR_MAX 127

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX-1)
#define UINT_MAX (INT_MAX * 2U + 1)

#define LONG_MAX 2147483647L
#define LONG_MIN (-LONG_MAX-1)

#define ULONG_MAX (LONG_MAX * 2UL + 1)

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX-1)
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1)

#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX

#define ULONG_LONG_MAX ULLONG_MAX
#endif /* CRIS.  */

#ifdef __arm__
#define CHAR_BIT 8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127

#define UCHAR_MAX 255

/* Char is unsigned on arm.  */
#define CHAR_MIN (0)
#define CHAR_MAX 255

#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX-1)
#define UINT_MAX (INT_MAX * 2U + 1)

#define LONG_MAX 2147483647L
#define LONG_MIN (-LONG_MAX-1)

#define ULONG_MAX (LONG_MAX * 2UL + 1)

#define LLONG_MAX 9223372036854775807LL
#define LLONG_MIN (-LLONG_MAX-1)
#define ULLONG_MAX (LLONG_MAX * 2ULL + 1)

#define LONG_LONG_MIN LLONG_MIN
#define LONG_LONG_MAX LLONG_MAX

#define ULONG_LONG_MAX ULLONG_MAX
#endif /* CRIS.  */

#endif
