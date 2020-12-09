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
  /* This is explained in, for instance, upcr_geninclude/stdlib.h */
  #define _IN_UPCR_STDLIB_H_AGAIN

      #if 1
         #include_next <stdlib.h>
         #include_next <stdlib.h>
      #endif

#endif