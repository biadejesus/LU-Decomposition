#ifndef _IN_UPCR_TIME_H
#define _IN_UPCR_TIME_H


      #if 1
         #include_next <time.h>
         #include_next <time.h>
      #endif


#ifndef _UPCR_TIME_H
#define _UPCR_TIME_H

#if defined(__LIBCATAMOUNT__)
  /* try to provide something on systems which lack a working clock */
  #undef CLOCKS_PER_SEC
  #define CLOCKS_PER_SEC 1000000
  #undef clock
  #define clock() upcri_clock()
  extern clock_t upcri_clock(void);
#endif

#endif

#undef _IN_UPCR_TIME_H
#elif !defined(_IN_UPCR_TIME_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_TIME_H_AGAIN

      #if 1
         #include_next <time.h>
         #include_next <time.h>
      #endif

#endif