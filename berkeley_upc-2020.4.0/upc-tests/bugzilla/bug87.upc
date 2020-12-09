/* Test all the headers listed in the C spec, that are supported by this configuration */


/* First the C89 headers, specified by ANSI/ISO 9899-1990 */

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Next the C99 headers that were added in ISO/IEC 9899:1999 */
/* this test allows selective disable of groups of related headers, for use on pre-C99 conformant systems */

#ifndef USE_C99_HEADERS
#define USE_C99_HEADERS 1
#endif

#if __BERKELEY_UPC__ && !defined(USE_C99_COMPLEX)
/* bug87: complex.h currently not supported in BUPC */
#define USE_C99_COMPLEX 0
#endif

#ifndef USE_C99_COMPLEX
#define USE_C99_COMPLEX USE_C99_HEADERS
#endif
#if USE_C99_COMPLEX
#include <complex.h>
#include <tgmath.h> /* tgmath.h explicitly relies upon complex.h */
#endif

#ifndef USE_C99_FENV
#define USE_C99_FENV USE_C99_HEADERS
#endif
#if USE_C99_FENV
#include <fenv.h>
#endif

#ifndef USE_C99_ISO646
#define USE_C99_ISO646 USE_C99_HEADERS
#endif
#if USE_C99_ISO646
#include <iso646.h>
#endif

#ifndef USE_C99_INT
#define USE_C99_INT USE_C99_HEADERS
#endif
#if USE_C99_INT
#include <stdint.h>
#include <inttypes.h> /* inttypes.h explicitly relies upon stdint.h */
#endif

#ifndef USE_C99_BOOL
#define USE_C99_BOOL USE_C99_HEADERS
#endif
#if USE_C99_BOOL
#include <stdbool.h>
#endif

#ifndef USE_C99_WCHAR
#define USE_C99_WCHAR USE_C99_HEADERS
#endif
#if USE_C99_WCHAR
#include <wchar.h>  /* wchar.h relies upon wctype.h in XPG4 */
#include <wctype.h>
#endif

#define CHECKSZ(x,val) assert(sizeof(x) == val)
int main(void) {
#if USE_C99_INT
  int8_t  i8 = 0;
  uint8_t u8 = 0;
  int16_t  i16 = 0;
  uint16_t u16 = 0;
  int32_t  i32 = 0;
  uint32_t u32 = 0;
  int64_t  i64 = 0;
  uint64_t u64 = 0;
  intptr_t ip = 0;
  uintptr_t up = 0;
  CHECKSZ(int8_t,1);
  CHECKSZ(uint8_t,1);
  CHECKSZ(int16_t,2);
  CHECKSZ(uint16_t,2);
  CHECKSZ(int32_t,4);
  CHECKSZ(uint32_t,4);
  CHECKSZ(int64_t,8);
  CHECKSZ(uint64_t,8);
  CHECKSZ(i8,1);
  CHECKSZ(u8,1);
  CHECKSZ(i16,2);
  CHECKSZ(u16,2);
  CHECKSZ(i32,4);
  CHECKSZ(u32,4);
  CHECKSZ(i64,8);
  CHECKSZ(u64,8);
  u64=u64+u32+u16+u8;
  i64=i64+i32+i16+i8;
#endif
#if USE_C99_BOOL
  bool t = true;
  bool f = false;
#endif
  printf("done.\n");
  return 0;
}


