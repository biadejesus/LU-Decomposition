#ifndef _IN_UPCR_STDIO_H
#define _IN_UPCR_STDIO_H

#if defined(PLATFORM_OS_LINUX) && !defined(__restrict)
/* On some glibc systems stdio.h and bits/stdio.h contain inconsistent use of __restrict.
 * We work around that here (at an insignificant cost in lost optimization.)
 * bug1502: the fix is constrained to Linux where it's needed, because the fact we apply this fix to a 
 *  subset of the system headers creates inconsistent defs on MacOS (and possibly elsewhere as well)
 */
  #define __restrict /**/
  #define _UPCRI_RESTRICT_WORKAROUND
#endif

#if __BERKELEY_UPC_FIRST_PREPROCESS__ && \
   (defined(__sun) || defined(__sun__) || \
    (defined(__PGI) && defined(__osx86__)))  /* PGI on Darwin */
  /* For bug 3210 - need wrapped stdarg before stdio.h */
  #include <stdarg.h>
#endif


      #if 1
         #include_next <stdio.h>
         #include_next <stdio.h>
      #endif


#ifdef _UPCRI_RESTRICT_WORKAROUND
  #undef _UPCRI_RESTRICT_WORKAROUND
  #undef __restrict
#endif

#ifndef _UPCR_STDIO_H
#define _UPCR_STDIO_H

#endif

#undef _IN_UPCR_STDIO_H
#elif !defined(_IN_UPCR_STDIO_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_STDIO_H_AGAIN

      #if 1
         #include_next <stdio.h>
         #include_next <stdio.h>
      #endif

#endif
