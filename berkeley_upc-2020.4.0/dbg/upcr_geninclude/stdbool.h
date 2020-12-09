#ifndef _IN_UPCR_STDBOOL_H
#define _IN_UPCR_STDBOOL_H


      #if 1
         #include_next <stdbool.h>
         #include_next <stdbool.h>
      #endif


#ifndef _UPCR_STDBOOL_H
#define _UPCR_STDBOOL_H

#ifndef bool
  #if 1
    #define bool _Bool
  #else 
    /* no native _Bool support - 
       this is not exactly right because it allows values other than 0/1, 
       but it's close enough for many purposes */
    #define bool unsigned char 
  #endif
#endif
#ifndef true
  #define true  1
#endif
#ifndef false
  #define false 0
#endif
#ifndef __bool_true_false_are_defined
  #define __bool_true_false_are_defined 1
#endif

#if __BERKELEY_UPC_FIRST_PREPROCESS__
  /* Open64 is lacking _Bool support (is it really?) */
  #undef bool
  #define bool unsigned char
#endif

#endif

#undef _IN_UPCR_STDBOOL_H
#elif !defined(_IN_UPCR_STDBOOL_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_STDBOOL_H_AGAIN

      #if 1
         #include_next <stdbool.h>
         #include_next <stdbool.h>
      #endif

#endif
