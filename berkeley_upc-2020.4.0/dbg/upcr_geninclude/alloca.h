#ifndef _IN_UPCR_ALLOCA_H
#define _IN_UPCR_ALLOCA_H

#if defined(PLATFORM_COMPILER_PGI) && defined(PLATFORM_OS_DARWIN) && defined(PLATFORM_ARCH_X86_64) /* bug 2235: system header bug workaround */
  #include <sys/cdefs.h>
#endif

#if 1

      #if 1
         #include_next <alloca.h>
         #include_next <alloca.h>
      #endif

#elif 0
 #include <stdlib.h>
#endif


#ifndef _UPCR_ALLOCA_H
#define _UPCR_ALLOCA_H

  #if __BERKELEY_UPC_FIRST_PREPROCESS__
    #if 1 || 0
      /* Forward name shift */
      #undef	alloca
      #define	alloca bupc_mangled_alloca
      #if defined (__OSF__) || defined (__osf__) || defined(__sgi) /* Any others? */
        extern void *bupc_mangled_alloca(int);
      #else
        extern void *bupc_mangled_alloca(size_t);
      #endif
    #else
     #if 0 /* Disabled - see bug 2131 */
      #include <stdlib.h> /* for malloc() */
      extern void ** _bupc_alloca_head; /* will be local var in final compile */
      #undef	alloca
      #define alloca(SZ) /* Translator generates ISO C for the following: */ \
	({ void ** _bupc_alloca_tmp = malloc((SZ) + 8); \
	   *_bupc_alloca_tmp = _bupc_alloca_head;       \
	   _bupc_alloca_head = _bupc_alloca_tmp;        \
	   (void *)((char *)_bupc_alloca_tmp + 8);      \
 	})
     #endif
    #endif
  #else
    /* Reverse name shift */
    #if defined(_AIX) && defined(__xlC__) && 0 /* DOB: no longer need this due to fn-like macro below */
      /* AIX's /usr/include/alloca.h sets a #pragma until xlc that causes the
       * preprocessor to emit __alloca(x) when alloca(x) seen.
       *   - for some reason they chose to make this a builtin instead of using
       *     a simple #define
       */
      #undef    alloca
      #define	alloca			__alloca
    #endif
    #if 0 /* special xlc hack - See bug 1823 and 2123 */
      #undef    alloca
      #define   alloca   __builtin_alloca
    #endif
    #define	bupc_mangled_alloca(x)	alloca(x)
  #endif

#endif

#undef _IN_UPCR_ALLOCA_H
#elif !defined(_IN_UPCR_ALLOCA_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_ALLOCA_H_AGAIN
#if 1

      #if 1
         #include_next <alloca.h>
         #include_next <alloca.h>
      #endif

#elif 0
 #include <stdlib.h>
#endif
#endif
