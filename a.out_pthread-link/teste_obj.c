/* --- UPCR system headers --- */ 
#include "upcr.h" 
#include "whirl2c.h"
#include "upcr_proxy.h"
/*******************************************************
 * C file translated from WHIRL Tue Dec  8 10:56:41 2020
 *******************************************************/

/* UPC Runtime specification expected: 3.6 */
#define UPCR_WANT_MAJOR 3
#define UPCR_WANT_MINOR 6
/* UPC translator version: release 2.28.0, built on Jul 19 2018 at 20:29:47, host aphid.lbl.gov linux-x86_64/64, gcc v4.2.4 (Ubuntu 4.2.4-1ubuntu4) */
/* Included code from the initialization script */
#include</usr/local/berkeley_upc/opt/include/upcr_config.h>
#include</usr/local/berkeley_upc/opt/include/gasnet_portable_platform.h>
#include</usr/local/berkeley_upc/opt/include/upcr_preinclude/upc_types.h>
#include "upcr_geninclude/stddef.h"
#include</usr/local/berkeley_upc/opt/include/upcr_preinclude/upc_bits.h>
#include "upcr_geninclude/stdlib.h"
#include "upcr_geninclude/inttypes.h"
#include "upcr_geninclude/stdio.h"
#line 1 "teste.w2c.h"
/* Include builtin types and operators */

#ifndef UPCR_TRANS_EXTRA_INCL
#define UPCR_TRANS_EXTRA_INCL
extern int upcrt_gcd (int _a, int _b);
extern int _upcrt_forall_start(int _start_thread, int _step, int _lo, int _scale);
#define upcrt_forall_start(start_thread, step, lo, scale)  \
       _upcrt_forall_start(start_thread, step, lo, scale)
int32_t UPCR_TLD_DEFINE_TENTATIVE(upcrt_forall_control, 4, 4);
#define upcr_forall_control upcrt_forall_control
#ifndef UPCR_EXIT_FUNCTION
#define UPCR_EXIT_FUNCTION() ((void)0)
#endif
#if UPCR_RUNTIME_SPEC_MAJOR > 3 || (UPCR_RUNTIME_SPEC_MAJOR == 3 && UPCR_RUNTIME_SPEC_MINOR >= 8)
  #define UPCRT_STARTUP_SHALLOC(sptr, blockbytes, numblocks, mult_by_threads, elemsz, typestr) \
      { &(sptr), (blockbytes), (numblocks), (mult_by_threads), (elemsz), #sptr, (typestr) }
#else
  #define UPCRT_STARTUP_SHALLOC(sptr, blockbytes, numblocks, mult_by_threads, elemsz, typestr) \
      { &(sptr), (blockbytes), (numblocks), (mult_by_threads) }
#endif
#define UPCRT_STARTUP_PSHALLOC UPCRT_STARTUP_SHALLOC

/**** Autonb optimization ********/

extern void _upcrt_memput_nb(upcr_shared_ptr_t _dst, const void *_src, size_t _n);
#define upcrt_memput_nb(dst, src, n) \
       (upcri_srcpos(), _upcrt_memput_nb(dst, src, n))

#endif


/* Types */
/* File-level vars and routines */
extern int user_main();


#define UPCR_SHARED_SIZE_ 8
#define UPCR_PSHARED_SIZE_ 8

void UPCRI_ALLOC_teste_229483745279553(void) { 
UPCR_BEGIN_FUNCTION();

UPCR_SET_SRCPOS("_teste_229483745279553_ALLOC",0);
}

void UPCRI_INIT_teste_229483745279553(void) { 
UPCR_BEGIN_FUNCTION();
UPCR_SET_SRCPOS("_teste_229483745279553_INIT",0);
}

#line 4 "teste.upc"
extern int user_main()
#line 4 "teste.upc"
{
#line 4 "teste.upc"
  UPCR_BEGIN_FUNCTION();
  
#line 5 "teste.upc"
  printf("Hello from thread %i/%i\n", ((int) upcr_mythread () ), ((int) upcr_threads () ));
#line 7 "teste.upc"
  UPCR_EXIT_FUNCTION();
#line 7 "teste.upc"
  return 0;
} /* user_main */

#line 1 "_SYSTEM"
/**************************************************************************/
/* upcc-generated strings for configuration consistency checks            */

GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_GASNetConfig_gen, 
 "$GASNetConfig: (/tmp/upcc--986-1607453801/teste.trans.c) RELEASE=2020.3.0,SPEC=0.8,CONDUIT=UDP(UDP-3.16/REFERENCE-2020.3),THREADMODEL=PAR,SEGMENT=FAST,PTR=64bit,CACHE_LINE_BYTES=64,noalign,pshm,nodebug,notrace,nostats,nodebugmalloc,nosrclines,timers_native,membars_native,atomics_native,atomic32_native,atomic64_native,notiopt $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_UPCRConfig_gen,
 "$UPCRConfig: (/tmp/upcc--986-1607453801/teste.trans.c) VERSION=2020.4.0,PLATFORMENV=shared-distributed,SHMEM=pthreads/pshm,SHAREDPTRREP=packed/p20:t10:a34,TRANS=berkeleyupc,nodebug,nogasp,dynamicthreads $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_translatetime, 
 "$UPCTranslateTime: (teste.o) Tue Dec  8 10:56:41 2020 $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_GASNetConfig_obj, 
 "$GASNetConfig: (teste.o) " GASNET_CONFIG_STRING " $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_UPCRConfig_obj,
 "$UPCRConfig: (teste.o) " UPCR_CONFIG_STRING UPCRI_THREADCONFIG_STR " $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_translator, 
 "$UPCTranslator: (teste.o) /usr/local/upc/2.28.0/translator/install/targ (aphid.lbl.gov) $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_upcver, 
 "$UPCVersion: (teste.o) 2.28.0 $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_compileline, 
 "$UPCCompileLine: (teste.o) /usr/local/upc/2.28.0/runtime/inst/bin/upcc.pl --at-remote-http -translator=/usr/local/upc/2.28.0/translator/install/targ --arch-size=64 --network=udp --pthreads 4 --lines --trans --sizes-file=upcc-sizes teste.i $");
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_compiletime, 
 "$UPCCompileTime: (teste.o) " __DATE__ " " __TIME__ " $");
#ifndef UPCRI_CC /* ensure backward compatilibity for http netcompile */
#define UPCRI_CC <unknown>
#endif
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_backendcompiler, 
 "$UPCRBackendCompiler: (teste.o) " _STRINGIFY(UPCRI_CC) " $");

#ifdef GASNETT_CONFIGURE_MISMATCH
  GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_configuremismatch, 
   "$UPCRConfigureMismatch: (teste.o) 1 $");
  GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_configuredcompiler, 
   "$UPCRConfigureCompiler: (teste.o) " GASNETT_PLATFORM_COMPILER_IDSTR " $");
  GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_buildcompiler, 
   "$UPCRBuildCompiler: (teste.o) " PLATFORM_COMPILER_IDSTR " $");
#endif

/**************************************************************************/
GASNETT_IDENT(UPCRI_IdentString_teste_o_1607453801_transver_2,
 "$UPCTranslatorVersion: (teste.o) 2.28.0, built on Jul 19 2018 at 20:29:47, host aphid.lbl.gov linux-x86_64/64, gcc v4.2.4 (Ubuntu 4.2.4-1ubuntu4) $");
GASNETT_IDENT(UPCRI_IdentString_ALLOC_teste_229483745279553,"$UPCRAllocFn: (teste.trans.c) UPCRI_ALLOC_teste_229483745279553 $");
GASNETT_IDENT(UPCRI_IdentString_INIT_teste_229483745279553,"$UPCRInitFn: (teste.trans.c) UPCRI_INIT_teste_229483745279553 $");
