#if !1
 #ifndef _UPCR_ISO646_H
  #define UPCR_ISO646_H 1
  #ifndef __cplusplus
   #define and     &&
   #define and_eq  &=
   #define bitand  &
   #define bitor   |
   #define compl   ~
   #define not     !
   #define not_eq  !=
   #define or      ||
   #define or_eq   |=
   #define xor     ^
   #define xor_eq  ^=
  #endif
 #endif
#else
 #ifndef _IN_UPCR_ISO646_H
  #define _IN_UPCR_ISO646_H

      #if 1
         #include_next <iso646.h>
         #include_next <iso646.h>
      #endif

  #undef _IN_UPCR_ISO646_H
 #elif !defined(_IN_UPCR_ISO646_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug
#2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_ISO646_H_AGAIN

      #if 1
         #include_next <iso646.h>
         #include_next <iso646.h>
      #endif

 #endif
#endif