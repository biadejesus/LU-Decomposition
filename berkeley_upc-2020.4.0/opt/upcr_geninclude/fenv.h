#ifndef _IN_UPCR_FENV_H
#define _IN_UPCR_FENV_H

#if 1 && defined(PLATFORM_OS_CYGWIN)
  /* bug 3068 - Cygwin 1.7.9 fenv.h has broken multiple-include protection */
  #include_next <fenv.h>
  #ifndef _FENV_H_
  #define _FENV_H_ 1
  #endif
#endif


      #if 1
         #include_next <fenv.h>
         #include_next <fenv.h>
      #endif


#if !1 && !defined(_UPCR_FENV_H)
  #define _UPCR_FENV_H
  /* no native fenv support - provide a (legal) do-nothing stub implementation 
     the only thing missing is #pragma STDC FENV_ACCESS, but nobody should use
     that anyhow
   */ 

  #include <assert.h>

  typedef int fenv_t;
  typedef int fexcept_t;

  /* no FE_ macros provided */
  #define FE_ALL_EXCEPT 0
  const fenv_t _dfl_env;
  #define FE_DFL_ENV (&_dfl_env)	

  #define feclearexcept(ex) assert(ex == 0)
  #define feraiseexcept(ex) assert(ex == 0)
  #define fetestexcept(ex) (assert(ex == 0),0)
  #define fegetexceptflag(flagp,ex) \
      (void)(assert(ex == 0),*((fexcept_t*)(flagp))=0)
  #define fesetexceptflag(flagp,ex) \
      (void)(assert(ex == 0),assert(*((fexcept_t*)(flagp))==0))

  #define fegetround()	-1
  #define fesetround(r) -1

  #define fegetenv(envp)      (void)(*((fenv_t*)(envp))=0)
  #define fesetenv(envp)      assert(*((fenv_t*)(envp))==0)
  #define feupdateenv(envp)   assert(*((fenv_t*)(envp))==0)
  #define feholdexcept(envp)  (*((fenv_t*)(envp))=0,-1) 

#endif

#undef _IN_UPCR_FENV_H
#elif !defined(_IN_UPCR_FENV_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_FENV_H_AGAIN

      #if 1
         #include_next <fenv.h>
         #include_next <fenv.h>
      #endif

#endif