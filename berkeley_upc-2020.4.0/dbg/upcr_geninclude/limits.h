/* This header is part of the C spec, and some systems are lacking it */

#ifndef _IN_UPCR_LIMITS_H
#define _IN_UPCR_LIMITS_H

/* include system one first */
#ifndef PLATFORM_COMPILER_INTEL /* limits.h broken on Intel C */

      #if 1
         #include_next <limits.h>
         #include_next <limits.h>
      #endif

#endif

#ifndef _UPCR_LIMITS_H
#define _UPCR_LIMITS_H

#define _MIN_8BIT_SIGNED     -128
#define _MAX_8BIT_SIGNED      127
#define _MAX_8BIT_UNSIGNED    255
#define _MIN_16BIT_SIGNED    -32768
#define _MAX_16BIT_SIGNED     32767
#define _MAX_16BIT_UNSIGNED   65535
#define _MIN_32BIT_SIGNED    -2147483648
#define _MAX_32BIT_SIGNED     2147483647
#define _MAX_32BIT_UNSIGNED   4294967295U
#define _MIN_64BIT_SIGNED    -9223372036854775808
#define _MAX_64BIT_SIGNED     9223372036854775807
#define _MAX_64BIT_UNSIGNED   18446744073709551615U
#define _MIN_128BIT_SIGNED    -170141183460469231731687303715884105728
#define _MAX_128BIT_SIGNED     170141183460469231731687303715884105727
#define _MAX_128BIT_UNSIGNED   340282366920938463463374607431768211455U

#ifndef _CONCAT
#define _CONCAT_HELPER(a,b) a ## b
#define _CONCAT(a,b) _CONCAT_HELPER(a,b)
#endif


#if 1 == 1
  #ifndef CHAR_BIT
    /* number of bits for smallest object that is not a bit-field (byte) */
    #define CHAR_BIT (8)
  #endif
  #ifndef SCHAR_MIN
    /* minimum value for an object of type signed char -(2^7 - 1) */
    #define SCHAR_MIN (_MIN_8BIT_SIGNED)
  #endif
  #ifndef SCHAR_MAX
    /* maximum value for an object of type signed char +(2^7 - 1) */
    #define SCHAR_MAX (_MAX_8BIT_SIGNED)
  #endif
  #ifndef UCHAR_MAX
    /* maximum value for an object of type unsigned char 2^8 - 1 */
    #define UCHAR_MAX (_MAX_8BIT_UNSIGNED) 
  #endif

  /*  If the value of an object of type char is treated as a signed integer when used in an
      expression, the value of CHAR_MIN shall be the same as that of SCHAR_MIN and the
      value of CHAR_MAX shall be the same as that of SCHAR_MAX. Otherwise, the value of
      CHAR_MIN shall be 0 and the value of CHAR_MAX shall be the same as that of
      UCHAR_MAX. The value UCHAR_MAX shall equal (2 ^ CHAR_BIT) - 1.
  */
  #if 1
    #ifndef CHAR_MIN
      /* minimum value for an object of type char */
      #define CHAR_MIN SCHAR_MIN
    #endif
    #ifndef CHAR_MAX
      /* maximum value for an object of type char */
      #define CHAR_MAX SCHAR_MAX
    #endif
  #else
    #ifndef CHAR_MIN
      /* minimum value for an object of type char */
      #define CHAR_MIN (0)
    #endif
    #ifndef CHAR_MAX
      /* maximum value for an object of type char */
      #define CHAR_MAX UCHAR_MAX
    #endif
  #endif

  #ifndef MB_LEN_MAX
    /* maximum number of bytes in a multibyte character, for any supported locale */
    #define MB_LEN_MAX (1)
  #endif
#else
  #error You have a very strange platform
#endif


#if 2 == 2
  #ifndef SHRT_MIN
    /* minimum value for an object of type short int -(2^15 - 1) */
    #define SHRT_MIN (_MIN_16BIT_SIGNED)
  #endif
  #ifndef SHRT_MAX
    /* maximum value for an object of type short int 2^15 - 1 */
    #define SHRT_MAX (_MAX_16BIT_SIGNED)
  #endif
  #ifndef USHRT_MAX
    /* maximum value for an object of type unsigned short int 2^16 - 1 */
    #define USHRT_MAX (_MAX_16BIT_UNSIGNED)
  #endif
#elif 2 == 4
  #ifndef SHRT_MIN
    /* minimum value for an object of type short int -(2^15 - 1) */
    #define SHRT_MIN (_MIN_32BIT_SIGNED)
  #endif
  #ifndef SHRT_MAX
    /* maximum value for an object of type short int 2^15 - 1 */
    #define SHRT_MAX (_MAX_32BIT_SIGNED)
  #endif
  #ifndef USHRT_MAX
    /* maximum value for an object of type unsigned short int 2^16 - 1 */
    #define USHRT_MAX (_MAX_32BIT_UNSIGNED)
  #endif
#else
  #error sizeof(short) == 2 is not recognized
#endif
  
#if 4 == 4
  #ifndef INT_MIN
    /* minimum value for an object of type int -(2^15 - 1) */
    #define INT_MIN (_MIN_32BIT_SIGNED)
  #endif
  #ifndef INT_MAX
    /* maximum value for an object of type int 2^15 - 1 */
    #define INT_MAX (_MAX_32BIT_SIGNED)
  #endif
  #ifndef UINT_MAX
    /* maximum value for an object of type unsigned int 2^16 - 1 */
    #define UINT_MAX (_MAX_32BIT_UNSIGNED)
  #endif
#elif 4 == 8
  #ifndef INT_MIN
    /* minimum value for an object of type int -(2^15 - 1) */
    #define INT_MIN (_MIN_64BIT_SIGNED)
  #endif
  #ifndef INT_MAX
    /* maximum value for an object of type int 2^15 - 1 */
    #define INT_MAX (_MAX_64BIT_SIGNED)
  #endif
  #ifndef UINT_MAX
    /* maximum value for an object of type unsigned int 2^16 - 1 */
    #define UINT_MAX (_MAX_64BIT_UNSIGNED)
  #endif
#else
  #error sizeof(int) == 4 is not recognized
#endif
  
#if 8 == 4
  #ifndef LONG_MIN
    /* minimum value for an object of type long int -(2^31 - 1) */
    #define LONG_MIN (_CONCAT(_MIN_32BIT_SIGNED,L))
  #endif
  #ifndef LONG_MAX
    /* maximum value for an object of type long int 2^31 - 1 */
    #define LONG_MAX (_CONCAT(_MAX_32BIT_SIGNED,L))
  #endif
  #ifndef ULONG_MAX
    /* maximum value for an object of type unsigned long int 2^32 - 1 */
    #define ULONG_MAX (_CONCAT(_MAX_32BIT_UNSIGNED,L))
  #endif
#elif 8 == 8
  #ifndef LONG_MIN
    /* minimum value for an object of type long int -(2^31 - 1) */
    #define LONG_MIN (_CONCAT(_MIN_64BIT_SIGNED,L))
  #endif
  #ifndef LONG_MAX
    /* maximum value for an object of type long int 2^31 - 1 */
    #define LONG_MAX (_CONCAT(_MAX_64BIT_SIGNED,L))
  #endif
  #ifndef ULONG_MAX
    /* maximum value for an object of type unsigned long int 2^32 - 1 */
    #define ULONG_MAX (_CONCAT(_MAX_64BIT_UNSIGNED,L))
  #endif
#elif 8 == 16
  #ifndef LONG_MIN
    /* minimum value for an object of type long int -(2^31 - 1) */
    #define LONG_MIN (_CONCAT(_MIN_128BIT_SIGNED,L))
  #endif
  #ifndef LONG_MAX
    /* maximum value for an object of type long int 2^31 - 1 */
    #define LONG_MAX (_CONCAT(_MAX_128BIT_SIGNED,L))
  #endif
  #ifndef ULONG_MAX
    /* maximum value for an object of type unsigned long int 2^32 - 1 */
    #define ULONG_MAX (_CONCAT(_MAX_128BIT_UNSIGNED,L))
  #endif
#else
  #error sizeof(long) == 8 is not recognized
#endif
  
#if 8 == 4
  #ifndef LLONG_MIN
    /* minimum value for an object of type long long int -(2^63 - 1) */
    #define LLONG_MIN (_CONCAT(_MIN_32BIT_SIGNED,LL))
  #endif
  #ifndef LLONG_MAX
    /* maximum value for an object of type long long int 2^63 - 1 */
    #define LLONG_MAX (_CONCAT(_MAX_32BIT_SIGNED,LL))
  #endif
  #ifndef ULLONG_MAX
    /* maximum value for an object of type unsigned long long int 2^64 - 1 */
    #define ULLONG_MAX (_CONCAT(_MAX_32BIT_UNSIGNED,LL))
  #endif
#elif 8 == 8
  #ifndef LLONG_MIN
    /* minimum value for an object of type long long int -(2^63 - 1) */
    #define LLONG_MIN (_CONCAT(_MIN_64BIT_SIGNED,LL))
  #endif
  #ifndef LLONG_MAX
    /* maximum value for an object of type long long int 2^63 - 1 */
    #define LLONG_MAX (_CONCAT(_MAX_64BIT_SIGNED,LL))
  #endif
  #ifndef ULLONG_MAX
    /* maximum value for an object of type unsigned long long int 2^64 - 1 */
    #define ULLONG_MAX (_CONCAT(_MAX_64BIT_UNSIGNED,LL))
  #endif
#elif 8 == 16
  #ifndef LLONG_MIN
    /* minimum value for an object of type long long int -(2^63 - 1) */
    #define LLONG_MIN (_CONCAT(_MIN_128BIT_SIGNED,LL))
  #endif
  #ifndef LLONG_MAX
    /* maximum value for an object of type long long int 2^63 - 1 */
    #define LLONG_MAX (_CONCAT(_MAX_128BIT_SIGNED,LL))
  #endif
  #ifndef ULLONG_MAX
    /* maximum value for an object of type unsigned long long int 2^64 - 1 */
    #define ULLONG_MAX (_CONCAT(_MAX_128BIT_UNSIGNED,LL))
  #endif
#else
  #error sizeof(long) == 8 is not recognized
#endif

#endif
  
#undef _IN_UPCR_LIMITS_H
#elif !defined(_IN_UPCR_LIMITS_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_LIMITS_H_AGAIN

      #if 1
         #include_next <limits.h>
         #include_next <limits.h>
      #endif

#endif