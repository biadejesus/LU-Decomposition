#ifndef _IN_UPCR_STDDEF_H
#define _IN_UPCR_STDDEF_H

#if __BERKELEY_UPC_FIRST_PREPROCESS__
  #undef offsetof /* avoid redef warnings upon reinclusion */

  #if defined(PLATFORM_COMPILER_CLANG) && defined(PLATFORM_OS_OPENBSD)
    /* bug3075: prevent Clang's incompatible (re)definition */
    #include <sys/types.h>
    #define _SIZE_T
  #endif
#endif


      #if 1
         #include_next <stddef.h>
         #include_next <stddef.h>
      #endif


#if __BERKELEY_UPC_FIRST_PREPROCESS__
  /* bug1318: some systems fail to define offsetof in their system headers, 
     so do it ourselves to make sure we get a working version 
     use long below instead of uintptr_t to avoid introducing a stdint dependence
   */
  #undef offsetof
  #define offsetof(type, field)  ((size_t)(long)(&((type *)0)->field))
  /* bug2383: gcc 4.3.x inserts __builtin_offsetof in sys/ucontext.h
   * and our translator does not know how to parse that
   */
  #undef __builtin_offsetof
  #define __builtin_offsetof(type, field)  ((size_t)(long)(&((type *)0)->field))
#endif

#ifndef _UPCR_STDDEF_H
#define _UPCR_STDDEF_H

#endif

#undef _IN_UPCR_STDDEF_H
#elif !defined(_IN_UPCR_STDDEF_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_STDDEF_H_AGAIN

      #if 1
         #include_next <stddef.h>
         #include_next <stddef.h>
      #endif

#endif
