#ifndef _IN_UPCR_BITS_FLOATN_H
#define _IN_UPCR_BITS_FLOATN_H

#if __BERKELEY_UPC_FIRST_PREPROCESS__ && defined(__GLIBC__)
  /* This case is an intentional dead-end, with no #include_next.
   * The Berkeley UPC translator has known issues with glibc's floatn.h
   * header which serves only to provide a C language extension not in
   * ISO C99, and thus not in UPC.  See bug 3970.
   */
#else

      #if 1
         #include_next <bits/floatn.h>
         #include_next <bits/floatn.h>
      #endif

#endif

#undef _IN_UPCR_BITS_FLOATN_H
#elif !defined(_IN_UPCR_BITS_FLOATN_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_BITS_FLOATN_H_AGAIN
#if !(__BERKELEY_UPC_FIRST_PREPROCESS__ && defined(__GLIBC__))

      #if 1
         #include_next <bits/floatn.h>
         #include_next <bits/floatn.h>
      #endif

#endif
#endif
