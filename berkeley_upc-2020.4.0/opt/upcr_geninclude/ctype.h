#ifndef _IN_UPCR_CTYPE_H
#define _IN_UPCR_CTYPE_H


      #if 1
         #include_next <ctype.h>
         #include_next <ctype.h>
      #endif


#if __BERKELEY_UPC_FIRST_PREPROCESS__ && defined(_NEWLIB_VERSION)
  /* bug3069: disable the toupper/tolower optimized macro on first pass,
     because expanding the macros twice causes them to break in horrible ways */
  #undef toupper
  #undef tolower
#endif

#undef _IN_UPCR_CTYPE_H
#elif !defined(_IN_UPCR_CTYPE_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_CTYPE_H_AGAIN

      #if 1
         #include_next <ctype.h>
         #include_next <ctype.h>
      #endif

#endif