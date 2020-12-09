#ifndef _IN_UPCR_STDLIB_H
#define _IN_UPCR_STDLIB_H


      #if 1
         #include_next <stdlib.h>
         #include_next <stdlib.h>
      #endif


#ifndef _UPCR_STDLIB_H
#define _UPCR_STDLIB_H

/* capture calls to rand to ensure thread-safety */
#if defined(__BERKELEY_UPC_FIRST_PREPROCESS__) || defined(__BERKELEY_UPC_ONLY_PREPROCESS__)
  #undef rand
  #define rand upcri_rand
  #undef srand
  #define srand upcri_srand

  extern int upcri_rand(void);
  extern void upcri_srand(unsigned int _seed);
#endif

#endif

#undef _IN_UPCR_STDLIB_H
#elif !defined(_IN_UPCR_STDLIB_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_STDLIB_H_AGAIN

      #if 1
         #include_next <stdlib.h>
         #include_next <stdlib.h>
      #endif

#endif