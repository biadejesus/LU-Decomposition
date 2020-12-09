/* $Source: bitbucket.org:berkeleylab/upc-runtime.git/upcr_handler_decls.h $
 * Jason Duell <jduell@lbl.gov>
 *
 * Central location for Active Message handler declarations.  These
 * declarations take a rather odd form in order to 'transparently' support 32
 * vs. 64 bit architectures over AM (which only supports 32 bit arguments).
 *
 * Each Active Message handler function in the runtime must have a
 * corresponding entry here.  NOTHING besides entries, comments, and
 * preprocessor commands can exist in this file (it gets expanded into enum
 * definitions, and other funny contexts).  This file gets included by other
 * files multiple times, which is why it lacks the conventional "#ifndef
 * FOO_H..." wrappers around the body.
 *
 * Do NOT put a semicolon at the end of each macro, or Bad Things are
 * guaranteed to happen.
 *
 * A declaration must consist of an encompassing
 * UPCRI_{SHORT|MEDIUM|LONG}_{REQUEST|REPLY|REQREP}() macro, the arguments of which are
 *
 * 1) The name of the handler function.  This function must be implemented
 *    somewhere,  but you do not need to declare a prototype for it in a
 *    header file.  The function can be declared inline, so long as you
 *    guarantee that the definition is visible to upcr_handlers.c. If the
 *    function is not inline, it cannot be static.
 * 2) The number of *extra* arguments on a 32 bit platform. 
 * 3) The number of *extra* arguments on a 64 bit platform.  Extra arguments
 *    that are (or may be) 64 bits in size must be counted as 2 arguments.
 * 4) A parenthesized list of the prototype arguments of the function.  
 *    AM handlers must all return an int, and all must take a
 *    gex_Token_t as their first argument.  Medium and Long handlers
 *    must also take a void * pointer and a size_t length as the next
 *    arguments.  Extra arguments may be passed, but must all be either 32 bit
 *    gex_AM_Arg_t's, or void * pointers (which are 32/64 bits
 *    depending on the platform).  Any integer arguments that
 *    could require 64 bits of info on a 64 bit platform (example: an integer
 *    offset into an address space area) should be passed as pointers (i.e
 *    cast the integer to a pointer--don't pass the address of the int).
 *    Structs (like shared pointers) should be passed in a Medium or Long
 *    calls in the opaque payload.
 * 5) A parenthesized list, as it would appear in a function call on a 32 bit
 *    platform to your handler, with all arguments named a0, a1, ... aN.
 *    Pointers must be wrapper in the UPCRI_RECV_PTR() macro (to cast from a
 *    gex_AM_Arg_t to a void *).
 * 6) Same thing, but for 64 bit platforms.  Since gex_AM_Arg_t's are
 *    32 bits, the ptr macro must consume 2 arguments.
 *
 * Convention for UPC Runtime AM handler names:  use 'S/M/L' 
 * letter for short/medium/long, and then one of two conventions:
 * 1) 'Request' or 'Reply'
 *     e.g. upcri_free_MRequest, upcri_remote_memop_SReply
 * 2) 'RQ' (Request), 'RP' (Reply) or 'R' (both Request and Reply)
 *     e.g. upcri_LRQ_memput_signal, upcri_SRP_lockreply, upcri_SR_sem_free
 */

/* 
 * These macros are just to make the lines shorter... 
 */
#define tok gex_Token_t
#define arg gex_AM_Arg_t
#ifdef PLATFORM_ARCH_32
#  define ptr(p) UPCRI_RECV_PTR(p)
#else 
#  define ptr2(p1, p2) UPCRI_RECV_PTR(p1, p2)
#endif
 
#ifndef UPCRI_SHORT_REQUEST
#  define UPCRI_SHORT_REQUEST  UPCRI_SHORT_HANDLER
#  define UPCRI_SHORT_REPLY    UPCRI_SHORT_HANDLER
#  define UPCRI_SHORT_REQREP   UPCRI_SHORT_HANDLER
#  define UPCRI_MEDIUM_REQUEST UPCRI_MEDIUM_HANDLER
#  define UPCRI_MEDIUM_REPLY   UPCRI_MEDIUM_HANDLER
#  define UPCRI_MEDIUM_REQREP  UPCRI_MEDIUM_HANDLER
#  define UPCRI_LONG_REQUEST   UPCRI_LONG_HANDLER
#  define UPCRI_LONG_REPLY     UPCRI_LONG_HANDLER
#  define UPCRI_LONG_REQREP    UPCRI_LONG_HANDLER
#endif

/*
 * Memory allocation handler functions
 */
UPCRI_SHORT_REQUEST(upcri_global_alloc_SRequest, 5, 9, 
    (tok token, void *nblocks, void *blocksz, void *result_sptr, void *trigger,
    gex_AM_Arg_t caller),
    (token, ptr(a0), ptr(a1), ptr(a2), ptr(a3), a4),
    (token, ptr2(a0,a1), ptr2(a2,a3), ptr2(a4,a5), ptr2(a6,a7), a8))

UPCRI_MEDIUM_REPLY(upcri_global_alloc_MReply, 3, 6,
    (tok token, void *msg, size_t len, void *res, void *trigger, void *lheapsz),
    (token, addr, nbytes, ptr(a0), ptr(a1), ptr(a2)),
    (token, addr, nbytes, ptr2(a0,a1), ptr2(a2,a3), ptr2(a4,a5)))

UPCRI_SHORT_REQUEST(upcri_expand_local_SRequest, 4, 7,
    (tok token, void *neededlheapsz, void *requestsz, void *trigger_addr,
    gex_AM_Arg_t caller),
    (token, ptr(a0), ptr(a1), ptr(a2), a3),
    (token, ptr2(a0,a1), ptr2(a2,a3), ptr2(a4,a5), a6))

UPCRI_SHORT_REPLY(upcri_expand_local_SReply, 2, 4,
    (tok token, void *newlargestlh, void *trigger_addr),
    (token, ptr(a0), ptr(a1)),
    (token, ptr2(a0,a1), ptr2(a2,a3)))
    
UPCRI_MEDIUM_REQUEST(upcri_free_MRequest, 0, 0,
    (tok token, void *addr, size_t nbytes),
    (token, addr, nbytes),
    (token, addr, nbytes))

/* 
 * Remote memcpy and memset functions
 */
UPCRI_SHORT_REQUEST(upcri_remote_memcpy_SRequest, 4, 8,
    (tok token, void *dst, void *src, void *sz, void *trigger),
    (token, ptr(a0), ptr(a1), ptr(a2), ptr(a3)),
    (token, ptr2(a0,a1), ptr2(a2,a3), ptr2(a4,a5), ptr2(a6,a7)))

UPCRI_SHORT_REQUEST(upcri_remote_memset_SRequest, 4, 7,
    (tok token, void *dst, arg value, void *sz, void *trigger),
    (token, ptr(a0),     a1, ptr(a2),     ptr(a3)),
    (token, ptr2(a0,a1), a2, ptr2(a3,a4), ptr2(a5,a6)))

UPCRI_SHORT_REPLY(upcri_remote_memop_SReply, 1, 2,
    (tok token, void *trigger),
    (token, ptr(a0)),
    (token, ptr2(a0,a1)))

/* 
 * Lock handler functions
 * Target tids are needed only in pthread builds where node numbers are not enough.
 * So, arg counts for several messages depend on UPCRI_UPC_PTHREADS
 */
#if UPCRI_UPC_PTHREADS
UPCRI_SHORT_REPLY(upcri_SRP_lockreply, 4, 5,
    (tok token, arg threadid, arg generation, arg thread, void *addr),
    (token, a0, a1, a2, ptr(a3)),
    (token, a0, a1, a2, ptr2(a3,a4)))

UPCRI_SHORT_REPLY(upcri_SRP_unlockreply, 2, 2,
    (tok token, arg threadid, arg reply),
    (token, a0, a1),
    (token, a0, a1))

UPCRI_SHORT_REQUEST(upcri_SRQ_lockgrant, 1, 1,
    (tok token, arg threadid),
    (token, a0),
    (token, a0))

UPCRI_SHORT_REQUEST(upcri_SRQ_lockdestroy, 3, 4,
    (tok token, arg freer_threadid, arg lock_threadid, void *lock),
    (token, a0, a1, ptr(a2)), 
    (token, a0, a1, ptr2(a2,a3)))
#else /* UPCRI_UPC_PTHREADS */
UPCRI_SHORT_REPLY(upcri_SRP_lockreply, 3, 4,
    (tok token, arg generation, arg thread, void *addr),
    (token, a0, a1, ptr(a2)),
    (token, a0, a1, ptr2(a2,a3)))

UPCRI_SHORT_REPLY(upcri_SRP_unlockreply, 1, 1,
    (tok token, arg reply),
    (token, a0),
    (token, a0))

UPCRI_SHORT_REQUEST(upcri_SRQ_lockgrant, 0, 0,
    (tok token),
    (token),
    (token))

UPCRI_SHORT_REQUEST(upcri_SRQ_lockdestroy, 2, 3,
    (tok token, arg freer_threadid, void *lock),
    (token, a0, ptr(a1)), 
    (token, a0, ptr2(a1,a2)))
#endif /* UPCRI_UPC_PTHREADS */

UPCRI_SHORT_REQUEST(upcri_SRQ_lock, 4, 6,
    (tok token, arg isblocking, void *lock, arg threadid, void *address),
    (token, a0, ptr(a1), a2, ptr(a3)), 
    (token, a0, ptr2(a1,a2), a3, ptr2(a4,a5)))

UPCRI_SHORT_REQUEST(upcri_SRQ_unlock, 3, 4,
    (tok token, arg threadid, arg generation, void *lock),
    (token, a0, a1, ptr(a2)), 
    (token, a0, a1, ptr2(a2,a3)))

UPCRI_SHORT_REQUEST(upcri_SRQ_lockenqueue, 2, 3,
    (tok token, arg thread, void *tail_addr),
    (token, a0, ptr(a1)),
    (token, a0, ptr2(a1,a2)))

UPCRI_SHORT_REQUEST(upcri_SRQ_freeheldlock, 1, 2,
    (tok token, void *tail_addr),
    (token, ptr(a0)), 
    (token, ptr2(a0,a1)))

/* 
 * Atomic handler functions
 */
#if UPCRI_USE_AM_PTS_AMO
UPCRI_MEDIUM_REQUEST(upcri_MRQ_AMO_PTS, 3, 5,
    (tok token, void *addr, size_t nbytes, arg op, void *target, void *amo_op),
    (token, addr, nbytes, a0, ptr(a1), ptr(a2)), 
    (token, addr, nbytes, a0, ptr2(a1,a2), ptr2(a3,a4)))

UPCRI_MEDIUM_REPLY(upcri_MRP_AMO_PTS, 1, 2,
    (tok token, void *addr, size_t nbytes, void *amo_op),
    (token, addr, nbytes, ptr(a0)), 
    (token, addr, nbytes, ptr2(a0,a1)))
#endif

/* 
 * Semaphore handler functions
 */
UPCRI_SHORT_REQREP(upcri_SR_sem_free, 3, 5,
    (tok token, arg threadid, void *ps, void *done),
    (token, a0, ptr(a1), ptr(a2)), 
    (token, a0, ptr2(a1,a2), ptr2(a3,a4)))

UPCRI_SHORT_REQREP(upcri_SR_tinypacket_connect, 4, 7,
    (tok token, arg threadid, void *ps, void *psp, void *pst),
    (token, a0, ptr(a1), ptr(a2), ptr(a3)), 
    (token, a0, ptr2(a1,a2), ptr2(a3,a4), ptr2(a5,a6)))

UPCRI_SHORT_REQUEST(upcri_SRQ_sem_upN, 2, 3,
    (tok token, arg seminc, void *semaddr),
    (token, a0, ptr(a1)), 
    (token, a0, ptr2(a1,a2)))

UPCRI_MEDIUM_REQUEST(upcri_MRQ_memput_signal, 6, 8,
    (tok token, void *addr, size_t nbytes, arg seqnum, arg threadid, arg numfragments, arg seminc, void *semaddr, void *dstaddr),
    (token, addr, nbytes, a0, a1, a2, a3, ptr(a4), ptr(a5)), 
    (token, addr, nbytes, a0, a1, a2, a3, ptr2(a4,a5), ptr2(a6,a7)))

UPCRI_LONG_REQUEST(upcri_LRQ_memput_signal, 5, 6,
    (tok token, void *addr, size_t nbytes, arg seqnum, arg threadid, arg numfragments, arg seminc, void *semaddr),
    (token, addr, nbytes, a0, a1, a2, a3, ptr(a4)), 
    (token, addr, nbytes, a0, a1, a2, a3, ptr2(a4,a5)))

#if defined(GASNET_SEGMENT_EVERYTHING) && UPCRI_UPC_PTHREADS
  /* Gather gasnet_seginfo_t's for segment exchange */
  UPCRI_SHORT_REQUEST(upcri_SRQ_seginfo, 3, 6,
    (tok token, void *info_addr, void *seg_addr, void *seg_size),
    (token, ptr(a0), ptr(a1), ptr(a2)),
    (token, ptr2(a0,a1), ptr2(a2,a3), ptr2(a4,a5)))
#endif

#undef UPCRI_SHORT_REQUEST
#undef UPCRI_SHORT_REPLY
#undef UPCRI_SHORT_REQREP
#undef UPCRI_MEDIUM_REQUEST
#undef UPCRI_MEDIUM_REPLY
#undef UPCRI_MEDIUM_REQREP
#undef UPCRI_LONG_REQUEST
#undef UPCRI_LONG_REPLY
#undef UPCRI_LONG_REQREP

#undef UPCRI_SHORT_HANDLER
#undef UPCRI_MEDIUM_HANDLER
#undef UPCRI_LONG_HANDLER

#undef tok
#undef arg
#ifdef PLATFORM_ARCH_32
#undef ptr
#else
#undef ptr2
#endif

