#ifndef _IN_UPCR_STDINT_H
#define _IN_UPCR_STDINT_H

/* get configure defines for portable_inttypes */
#include "upcr_config.h"
#include "portable_inttypes.h"


      #if 1
         #include_next <stdint.h>
         #include_next <stdint.h>
      #endif



      #if 1
         #include_next <inttypes.h>
         #include_next <inttypes.h>
      #endif


#ifndef _UPCR_STDINT_H
#define _UPCR_STDINT_H

/* include limits.h, because on some systems (Solaris) it defines some of the macros redefined below */
#include <limits.h>

/* pass the integral sizes to portable_inttypes */
#ifndef SIZEOF_CHAR
#define SIZEOF_CHAR 1
#endif
#ifndef SIZEOF_SHORT
#define SIZEOF_SHORT 2
#endif
#ifndef SIZEOF_INT
#define SIZEOF_INT 4
#endif
#ifndef SIZEOF_LONG
#define SIZEOF_LONG 8
#endif
#ifndef SIZEOF_LONG_LONG
#define SIZEOF_LONG_LONG 8
#endif
#ifndef SIZEOF_VOID_P
#define SIZEOF_VOID_P 8
#endif

/* prevent header inclusion loops from within portable_inttypes */
#ifdef HAVE_INTTYPES_H
  #undef HAVE_INTTYPES_H
#endif

#ifdef HAVE_STDINT_H
  #undef HAVE_STDINT_H
#endif

/* some platforms may have or be lacking these, so use macros to be safe */
#undef int_least8_t  
#undef int_least16_t 
#undef int_least32_t 
#undef int_least64_t 
#undef uint_least8_t 
#undef uint_least16_t
#undef uint_least32_t
#undef uint_least64_t

#undef int_fast8_t   
#undef int_fast16_t  
#undef int_fast32_t  
#undef int_fast64_t  
#undef uint_fast8_t  
#undef uint_fast16_t 
#undef uint_fast32_t 
#undef uint_fast64_t 

#define int_least8_t  	int8_t
#define int_least16_t 	int16_t
#define int_least32_t 	int32_t
#define int_least64_t 	int64_t
#define uint_least8_t  	uint8_t
#define uint_least16_t 	uint16_t
#define uint_least32_t  uint32_t
#define uint_least64_t  uint64_t

#define int_fast8_t    int8_t
#define int_fast16_t   int16_t
#define int_fast32_t   int32_t
#define int_fast64_t   int64_t
#define uint_fast8_t   uint8_t
#define uint_fast16_t  uint16_t
#define uint_fast32_t  uint32_t
#define uint_fast64_t  uint64_t

/* these may be overly conservative, but should be safe */
#undef intmax_t
#undef uintmax_t

#define intmax_t	int64_t
#define uintmax_t	uint64_t

/* limit value macros */
#undef INT8_MIN        
#undef INT8_MAX        
#undef UINT8_MAX       
#undef INT16_MIN       
#undef INT16_MAX       
#undef UINT16_MAX      
#undef INT32_MIN       
#undef INT32_MAX       
#undef UINT32_MAX      
#undef INT64_MIN       
#undef INT64_MAX       
#undef UINT64_MAX      

#undef INT_LEAST8_MIN  
#undef INT_LEAST8_MAX  
#undef UINT_LEAST8_MAX 
#undef INT_LEAST16_MIN 
#undef INT_LEAST16_MAX 
#undef UINT_LEAST16_MAX
#undef INT_LEAST32_MIN 
#undef INT_LEAST32_MAX 
#undef UINT_LEAST32_MAX
#undef INT_LEAST64_MIN 
#undef INT_LEAST64_MAX 
#undef UINT_LEAST64_MAX

#undef INT_FAST8_MIN   
#undef INT_FAST8_MAX   
#undef UINT_FAST8_MAX  
#undef INT_FAST16_MIN  
#undef INT_FAST16_MAX  
#undef UINT_FAST16_MAX 
#undef INT_FAST32_MIN  
#undef INT_FAST32_MAX  
#undef UINT_FAST32_MAX 
#undef INT_FAST64_MIN  
#undef INT_FAST64_MAX  
#undef UINT_FAST64_MAX 

#define INT8_MIN	(-128)
#define INT8_MAX	( 127)
#define UINT8_MAX	( 255)
#define INT16_MIN	(-32768)
#define INT16_MAX	( 32767)
#define UINT16_MAX	( 65535)
#define INT32_MIN	(-INT32_MAX-1)
#define INT32_MAX	( 2147483647)
#define UINT32_MAX	( 4294967295U)
#define INT64_MIN	(-INT64_MAX-1)
#define INT64_MAX	( 9223372036854775807)
#define UINT64_MAX	( 18446744073709551615U)

#define INT_LEAST8_MIN		INT8_MIN
#define INT_LEAST8_MAX		INT8_MAX
#define UINT_LEAST8_MAX		UINT8_MAX
#define INT_LEAST16_MIN		INT16_MIN
#define INT_LEAST16_MAX		INT16_MAX
#define UINT_LEAST16_MAX	UINT16_MAX
#define INT_LEAST32_MIN		INT32_MIN
#define INT_LEAST32_MAX		INT32_MAX
#define UINT_LEAST32_MAX	UINT32_MAX
#define INT_LEAST64_MIN		INT64_MIN
#define INT_LEAST64_MAX		INT64_MAX
#define UINT_LEAST64_MAX	UINT64_MAX

#define INT_FAST8_MIN          INT8_MIN
#define INT_FAST8_MAX          INT8_MAX
#define UINT_FAST8_MAX         UINT8_MAX
#define INT_FAST16_MIN         INT16_MIN
#define INT_FAST16_MAX         INT16_MAX
#define UINT_FAST16_MAX        UINT16_MAX
#define INT_FAST32_MIN         INT32_MIN
#define INT_FAST32_MAX         INT32_MAX
#define UINT_FAST32_MAX        UINT32_MAX
#define INT_FAST64_MIN         INT64_MIN
#define INT_FAST64_MAX         INT64_MAX
#define UINT_FAST64_MAX        UINT64_MAX

#undef INTPTR_MIN  
#undef INTPTR_MAX  
#undef UINTPTR_MAX 

#if   8 == 2
#define INTPTR_MIN  	INT16_MIN
#define INTPTR_MAX  	INT16_MAX
#define UINTPTR_MAX  	UINT16_MAX
#elif 8 == 4
#define INTPTR_MIN  	INT32_MIN
#define INTPTR_MAX  	INT32_MAX
#define UINTPTR_MAX  	UINT32_MAX
#elif 8 == 8
#define INTPTR_MIN 	INT64_MIN
#define INTPTR_MAX  	INT64_MAX
#define UINTPTR_MAX  	UINT64_MAX
#else
#error you have a bizarre void * size
#endif

#undef PTRDIFF_MIN
#undef PTRDIFF_MAX

#if   8 == 2
#define PTRDIFF_MIN      INT16_MIN
#define PTRDIFF_MAX      INT16_MAX
#elif 8 == 4
#define PTRDIFF_MIN      INT32_MIN
#define PTRDIFF_MAX      INT32_MAX
#elif 8 == 8
#define PTRDIFF_MIN      INT64_MIN
#define PTRDIFF_MAX      INT64_MAX
#else
#error you have a bizarre ptrdiff_t size
#endif

#undef SIZE_MAX

#if   8 == 2
#define SIZE_MAX      UINT16_MAX
#elif 8 == 4
#define SIZE_MAX      UINT32_MAX
#elif 8 == 8
#define SIZE_MAX      UINT64_MAX
#else
#error you have a bizarre size_t size
#endif

#if 0
/* currently unimplemented, as some platforms lack these types */
#define WCHAR_MIN
#define WCHAR_MAX

#define SIG_ATOMIC_MIN
#define SIG_ATOMIC_MAX

#define WINT_MIN
#define WINT_MAX
#endif

/* these should really expand to simple type suffixes, but those are too hard 
   to detect automatically for a given system */
#undef INT8_C
#undef UINT8_C
#undef INT16_C
#undef UINT16_C
#undef INT32_C
#undef UINT32_C
#undef INT64_C
#undef UINT64_C

#undef INTMAX_C
#undef UINTMAX_C

#define INT8_C(value)  ((int_least8_t)value##LL)
#define UINT8_C(value) ((uint_least8_t)value##ULL)
#define INT16_C(value)  ((int_least16_t)value##LL)
#define UINT16_C(value) ((uint_least16_t)value##ULL)
#define INT32_C(value)  ((int_least32_t)value##LL)
#define UINT32_C(value) ((uint_least32_t)value##ULL)
#define INT64_C(value)  ((int_least64_t)value##LL)
#define UINT64_C(value) ((uint_least64_t)value##ULL)

#define INTMAX_C(value)  ((int_max_t)value##LL)
#define UINTMAX_C(value) ((uint_max_t)value##ULL)

#endif

#undef _IN_UPCR_STDINT_H
#elif !defined(_IN_UPCR_STDINT_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_STDINT_H_AGAIN

      #if 1
         #include_next <stdint.h>
         #include_next <stdint.h>
      #endif

#endif