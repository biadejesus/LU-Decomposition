/* This wrapper ensure math constants work even with compiler-private math.h */
#ifndef _IN_UPCR_MATH_H
#define _IN_UPCR_MATH_H


      #if 1
         #include_next <math.h>
         #include_next <math.h>
      #endif


#ifdef __BERKELEY_UPC_FIRST_PREPROCESS__
  /* Map the constants to forms acceptible to clang */
  #undef HUGE_VAL
  #undef HUGE_VALF
  #undef HUGE_VALL
  #undef INFINITY
  #undef NAN
  #define HUGE_VAL  (__builtin_huge_val())
  #define HUGE_VALF (__builtin_huge_valf())
  #define HUGE_VALL (__builtin_huge_vall())
  #define INFINITY  (__builtin_inff())
  #define NAN       (__builtin_nanf (""))
#else
  /* Map the clang builtins back to forms acceptible to the backend */
  #if !HAVE_BUILTIN_HUGE_VAL && !defined(__builtin_huge_val)
    #define __builtin_huge_val()  HUGE_VAL
  #endif
  #if !HAVE_BUILTIN_HUGE_VALF && !defined(__builtin_huge_valf)
    #define __builtin_huge_valf() HUGE_VALF
  #endif
  #if !HAVE_BUILTIN_HUGE_VALL && !defined(__builtin_huge_vall)
    #define __builtin_huge_vall() HUGE_VALL
  #endif
  #if !HAVE_BUILTIN_INFF && !defined(__builtin_inff)
    #define __builtin_inff()      INFINITY
  #endif
  #if !HAVE_BUILTIN_NANF && !defined(__builtin_nanf)
    #define __builtin_nanf(_s)    NAN
  #endif
#endif

#undef _IN_UPCR_MATH_H
#elif !defined(_IN_UPCR_MATH_H_AGAIN)
  /* This is explained in, for instance, upcr_geninclude/math.h */
  #define _IN_UPCR_MATH_H_AGAIN

      #if 1
         #include_next <math.h>
         #include_next <math.h>
      #endif

#endif
