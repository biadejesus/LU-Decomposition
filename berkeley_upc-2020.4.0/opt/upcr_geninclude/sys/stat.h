#ifndef _IN_UPCR_SYS_STAT_H
#define _IN_UPCR_SYS_STAT_H

#ifndef __restrict
/* On some glibc systems sys/stat.h contain inconsistent use of __restrict.
 * We work around that here (at an insignificant cost in lost optimization.)
 */
  #define __restrict /**/
  #define _UPCRI_RESTRICT_WORKAROUND
#endif


      #if 1
         #include_next <sys/stat.h>
         #include_next <sys/stat.h>
      #endif


#ifdef _UPCRI_RESTRICT_WORKAROUND
  #undef _UPCRI_RESTRICT_WORKAROUND
  #undef __restrict
#endif

#ifndef _UPCR_SYS_STAT_H
#define _UPCR_SYS_STAT_H

#endif

#undef _IN_UPCR_SYS_STAT_H
#elif !defined(_IN_UPCR_SYS_STAT_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_SYS_STAT_H_AGAIN

      #if 1
         #include_next <sys/stat.h>
         #include_next <sys/stat.h>
      #endif

#endif
