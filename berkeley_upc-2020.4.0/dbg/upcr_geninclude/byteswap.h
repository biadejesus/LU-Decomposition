#ifndef _IN_UPCR_BYTESWAP_H
#define _IN_UPCR_BYTESWAP_H


      #if 1
         #include_next <byteswap.h>
         #include_next <byteswap.h>
      #endif


#ifndef _UPCR_BYTESWAP_H
#define _UPCR_BYTESWAP_H

#if defined(__GLIBC__) && defined(__GNUC__) && (__GNUC__ >= 2)
  // For any compiler (not just gcc) claiming to be GNUC:
  // We need to avoid glibc's inline asm which chokes the translator
  // We go to some length to avoid including stdint.h or inttypes.h
  #if defined(__BERKELEY_UPC_FIRST_PREPROCESS__)
    #ifdef __bswap_16
      #undef __bswap_16
      #define __bswap_16 bupc_mangled_bswap16
      #if 1 == 2
        extern unsigned char bupc_mangled_bswap16(unsigned char);
      #elif 2 == 2
        extern unsigned short bupc_mangled_bswap16(unsigned short);
      #elif 4 == 2
        extern unsigned int bupc_mangled_bswap16(unsigned int);
      #else
        #error "No 16-bit type?"
      #endif
    #endif
    #ifdef __bswap_32
      #undef __bswap_32
      #define __bswap_32 bupc_mangled_bswap32
      #if 2 == 4
        extern unsigned short bupc_mangled_bswap32(unsigned short);
      #elif 4 == 4
        extern unsigned int bupc_mangled_bswap32(unsigned int);
      #elif 8 == 4
        extern unsigned long bupc_mangled_bswap32(unsigned long);
      #else
        #error "No 32-bit type?"
      #endif
    #endif
    #ifdef __bswap_64
      #undef __bswap_64
      #define __bswap_64 bupc_mangled_bswap64
      #if 4 == 8
        extern unsigned int bupc_mangled_bswap64(unsigned int);
      #elif 8 == 8
        extern unsigned long bupc_mangled_bswap64(unsigned long);
      #elif 8 == 8
        extern unsigned long long bupc_mangled_bswap64(unsigned long long);
      #else
        #error "No 64-bit type?"
      #endif
    #endif
  #else
    #define bupc_mangled_bswap16 __bswap_16
    #define bupc_mangled_bswap32 __bswap_32
    #define bupc_mangled_bswap64 __bswap_64
  #endif
#endif

#endif

#undef _IN_UPCR_BYTESWAP_H
#elif !defined(_IN_UPCR_BYTESWAP_H_AGAIN)
  /* There is a known gcc bug with regards to #include_next not starting its
   * search at the next directory in the path as is documented.  This causes
   * some problems with gcc's private header's use of #include_next finding
   * THIS header rather than the system one (see Berkeley UPC bug #2118).
   * A similar bug is present in some xlc versions (see Berkeley UPC bug #2133).
   * Here we just allow the #include_next to pass through one extra time.
   */
  #define _IN_UPCR_BYTESWAP_H_AGAIN

      #if 1
         #include_next <byteswap.h>
         #include_next <byteswap.h>
      #endif

#endif
