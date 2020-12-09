#ifndef _IN_UPCR_STRING_H
#define _IN_UPCR_STRING_H


      #if 1
         #include_next <string.h>
         #include_next <string.h>
      #endif


#ifndef _UPCR_STRING_H
#define _UPCR_STRING_H

#if defined(__BERKELEY_UPC_FIRST_PREPROCESS__) && \
    defined(PLATFORM_COMPILER_XLC) && defined(__STR__) && !defined(__NO_STRING_INLINES)
  /* bug 1022: xlc's string.h macroizes string calls to undeclared intrinsics */
  extern size_t   __strlen(const char *);
  extern char     *__strcpy(char * __restrict__, const char * __restrict__);
  extern char     *__strncpy(char * __restrict__, const char * __restrict__, size_t);
  extern char     *__strcat(char * __restrict__, const char * __restrict__);
  extern char     *__strncat(char * __restrict__, const char * __restrict__, size_t);
  extern int      __strcmp(const char *, const char *);
  extern int      __strncmp(const char *,const char *,size_t);
  extern char     *__strchr(const char *, int);
  extern char     *__strrchr(const char *, int);
  extern char     *__strpbrk(const char *, const char *);
  extern size_t   __strspn(const char *, const char *);
  extern size_t   __strcspn(const char *, const char *);
  extern void     *__memmove(void *, const void *, size_t);
  extern void     *__memchr(const void *, int, size_t);
  extern void     *__memcpy(void * __restrict__, const void * __restrict__, size_t);
  extern void     *__memset(void *, int, size_t);
  extern int      __memcmp(const void *, const void *,size_t);
  extern void     *__memccpy(void * __restrict__, const void * __restrict__, int, size_t);
  /* the following are not currently macroized by xlc, but might be in the future */
  #ifdef strcoll
    extern int    __strcoll(const char *, const char *);
  #endif
  #ifdef strxfrm
    extern size_t __strxfrm(char * __restrict__, const char * __restrict__, size_t);
  #endif
  #ifdef strstr
    extern char  *__strstr(const char *, const char *);
  #endif
  #ifdef strtok
    extern char  *__strtok(char * __restrict__, const char * __restrict__);
  #endif
  #ifdef strerror
    extern char  *__strerror(int);
  #endif
#endif

#endif

#undef _IN_UPCR_STRING_H
#elif !defined(_IN_UPCR_STRING_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_STRING_H_AGAIN

      #if 1
         #include_next <string.h>
         #include_next <string.h>
      #endif

#endif