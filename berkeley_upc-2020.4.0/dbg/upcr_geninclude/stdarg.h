#ifndef _IN_UPCR_STDARG_H
#define _IN_UPCR_STDARG_H

#if !1
  /* no native support and no good way to fake it */
  #error "No stdarg.h available on this platform"
#endif

#if __BERKELEY_UPC_FIRST_PREPROCESS__ && \
    defined(PLATFORM_COMPILER_XLC) && defined(PLATFORM_ARCH_64)
  /* bug1462: workaround a definition in xlc7-64/Linux stdarg.h header 
     that makes our translator unhappy */
  #define __BUILTIN_VA_LIST
#endif


      #if 1
         #include_next <stdarg.h>
         #include_next <stdarg.h>
      #endif


#ifndef _UPCR_STDARG_H
#define _UPCR_STDARG_H
  #if __BERKELEY_UPC_FIRST_PREPROCESS__
    #undef	va_start
    #define	va_start(ap, last)	bupc_mangled_va_start(ap, #last)
    #undef	va_arg
    #define va_arg(ap,type) \
	({type _bupc_vaarg_tmp; \
	  bupc_mangled_va_arg(ap, #type, &_bupc_vaarg_tmp);\
	  _bupc_vaarg_tmp;})	/* whirl2c generates ISO C for this GNU C construct */
    #undef	va_end
    #define	va_end	bupc_mangled_va_end
    #undef	va_copy
    #define	va_copy	bupc_mangled_va_copy
    struct bupc_mangled_va_list;
    typedef struct bupc_mangled_va_list *bupc_mangled_va_listp; /* bug 3213: use a real typedef */
    #undef	va_list
    #define	va_list bupc_mangled_va_listp
    extern void bupc_mangled_va_start(va_list, const char *);
    extern void bupc_mangled_va_arg(va_list, const char *, void *);
    extern void bupc_mangled_va_end(va_list);
    extern void bupc_mangled_va_copy(va_list, va_list);

    /* Ensure that any subsequent headers don't redefine va_list */
    /* We used to try to define at most one of these, but now define them all */
      #undef  _VA_LIST
      #define _VA_LIST
      #undef  _VA_LIST_DECLARED
      #define _VA_LIST_DECLARED
      #undef  _VA_LIST_DEFINED
      #define _VA_LIST_DEFINED
      #undef  _VA_LIST_T
      #define _VA_LIST_T
    /* Now handle additional definitions bypassed due to the defines above */
    #if PLATFORM_COMPILER_PGI
      #undef  __gnuc_va_list
      #define __gnuc_va_list va_list
      #undef  __pgi_va_list
      #define __pgi_va_list va_list
    #elif PLATFORM_COMPILER_SUN
      #if PLATFORM_OS_SOLARIS
        #define __va_list va_list
      #elif defined(__GNUC_VA_LIST) /* Linux/glibc */
        #undef  __gnuc_va_list
        #define __gnuc_va_list va_list
      #endif
    #endif
  #else
    /* Reverse name shifting and destringification done in upcc.pl */
  #endif
#endif

#undef _IN_UPCR_STDARG_H
#elif !defined(_IN_UPCR_STDARG_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_STDARG_H_AGAIN

      #if 1
         #include_next <stdarg.h>
         #include_next <stdarg.h>
      #endif

#endif
