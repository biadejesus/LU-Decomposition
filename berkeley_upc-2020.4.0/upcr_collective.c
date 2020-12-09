/*
 * UPC Collectives implementation on GASNet
 * $Source: bitbucket.org:berkeleylab/upc-runtime.git/upcr_collective.c $
 */

#include <upcr.h>
#include <upcr_internal.h>
#include <gasnet_coll.h>

/*----------------------------------------------------------------------------*/
/* Magical helpers */

#if (UPC_IN_ALLSYNC == 0) || (UPC_OUT_ALLSYNC == 0)
  #error "No longer support a upc_flag_t representation with zero-valued ALLSYNCs"
#endif

GASNETT_INLINE(upcri_coll_syncmode_only)
unsigned int upcri_coll_syncmode_only(upc_flag_t sync_mode) {
  unsigned int result;

  if (sync_mode & UPC_IN_NOSYNC) {
    result = GASNET_COLL_IN_NOSYNC;
  } else if (sync_mode & UPC_IN_MYSYNC) {
    result = GASNET_COLL_IN_MYSYNC;
  } else if (sync_mode & UPC_IN_ALLSYNC) {
    result = GASNET_COLL_IN_ALLSYNC;
  } else result = 0;

  if (sync_mode & UPC_OUT_NOSYNC) {
    result |= GASNET_COLL_OUT_NOSYNC;
  } else if (sync_mode & UPC_OUT_MYSYNC) {
    result |= GASNET_COLL_OUT_MYSYNC;
  } else if (sync_mode & UPC_OUT_ALLSYNC) {
    result |= GASNET_COLL_OUT_ALLSYNC;
  }

  return result;
}

#if UPCRI_SINGLE_ALIGNED_REGIONS
  // addresses passed are single-valued
  #define UPCRI_COLL_ADDR_MODE GASNET_COLL_SINGLE
#else
  // addresses passed are only valid locally
  #define UPCRI_COLL_ADDR_MODE GASNET_COLL_LOCAL
#endif

GASNETT_INLINE(upcri_coll_syncmode)
unsigned int upcri_coll_syncmode(upc_flag_t sync_mode) {
  return UPCRI_COLL_ADDR_MODE |
	 GASNET_COLL_SRC_IN_SEGMENT | GASNET_COLL_DST_IN_SEGMENT |
	 upcri_coll_syncmode_only(sync_mode);
}

#define UPCRI_COLL_ONE_DST(D)	upcr_threadof_shared(D), upcri_shared_to_remote(D)
#define UPCRI_COLL_ONE_SRC(S)	upcr_threadof_shared(S), upcri_shared_to_remote(S)

#if UPCRI_SINGLE_ALIGNED_REGIONS
    #define UPCRI_COLL_MULTI_DST(D)	upcri_shared_to_remote(D)
    #define UPCRI_COLL_MULTI_SRC(S)	upcri_shared_to_remote(S)
    #define UPCRI_COLL_TEAM_MULTI_DST(TEAM, D)	upcri_shared_to_remote(D)
    #define UPCRI_COLL_TEAM_MULTI_SRC(TEAM, S)	upcri_shared_to_remote(S)
#else /* Multiple regions or single unaligned regions */
    #define UPCRI_COLL_MULTI_DST(D)	upcri_shared_to_remote_withthread(D,upcr_mythread())
    #define UPCRI_COLL_MULTI_SRC(S)	upcri_shared_to_remote_withthread(S,upcr_mythread())
    #define UPCRI_COLL_TEAM_MULTI_DST(TEAM, D)	upcri_shared_to_remote_withthread(D,upcr_mythread())
    #define UPCRI_COLL_TEAM_MULTI_SRC(TEAM, S)	upcri_shared_to_remote_withthread(S,upcr_mythread())
#endif

#define UPCRI_COLL_GASNETIFY(NAME,ARGS)\
	_CONCAT(gasnet_coll_,NAME)ARGS;
#define UPCRI_COLL_GASNETIFY_NB(NAME,ARGS)\
	_CONCAT(_CONCAT(gasnet_coll_,NAME),_nb)ARGS;

#define UPCRI_TRACE_COLL(name, nbytes) \
	UPCRI_TRACE_PRINTF(("COLLECTIVE UPC_ALL_" _STRINGIFY(name) ": sz = %6llu", (long long unsigned)(nbytes)))

/*----------------------------------------------------------------------------*/
/* Data movement collectives
 *
 * With the aid of the helpers above, these functions compile correctly
 * with and without aligned segments and with and without pthreads.
 */

void _upcr_all_broadcast(upcr_shared_ptr_t dst,
                         upcr_shared_ptr_t src,
                         size_t nbytes,
                         upc_flag_t sync_mode
                         UPCRI_PT_ARG)
{
    UPCRI_PASS_GAS();
    sync_mode = upcri_coll_fixsync(sync_mode);
    UPCRI_STRICT_HOOK_IF(sync_mode & UPC_IN_ALLSYNC);
    UPCRI_TRACE_COLL(BROADCAST, nbytes);
    #define UPCRI_PEVT_ARGS , &dst, &src, nbytes, (int)sync_mode
    upcri_pevt_start(GASP_UPC_ALL_BROADCAST);
    UPCRI_COLL_GASNETIFY(broadcast,
				(GASNET_TEAM_ALL,
				 UPCRI_COLL_MULTI_DST(dst),
				 UPCRI_COLL_ONE_SRC(src),
				 nbytes, upcri_coll_syncmode(sync_mode)));
    upcri_pevt_end(GASP_UPC_ALL_BROADCAST);
    #undef UPCRI_PEVT_ARGS
}



void _upcr_all_scatter(upcr_shared_ptr_t dst,
                       upcr_shared_ptr_t src,
                       size_t nbytes,
                       upc_flag_t sync_mode
                       UPCRI_PT_ARG)
{
    UPCRI_PASS_GAS();
    sync_mode = upcri_coll_fixsync(sync_mode);
    UPCRI_STRICT_HOOK_IF(sync_mode & UPC_IN_ALLSYNC);
    UPCRI_TRACE_COLL(SCATTER, nbytes);
    #define UPCRI_PEVT_ARGS , &dst, &src, nbytes, (int)sync_mode
    upcri_pevt_start(GASP_UPC_ALL_SCATTER);
    UPCRI_COLL_GASNETIFY(scatter,
				(GASNET_TEAM_ALL,
				 UPCRI_COLL_MULTI_DST(dst),
				 UPCRI_COLL_ONE_SRC(src),
				 nbytes, upcri_coll_syncmode(sync_mode)));
    upcri_pevt_end(GASP_UPC_ALL_SCATTER);
    #undef UPCRI_PEVT_ARGS
}

void _upcr_all_gather(upcr_shared_ptr_t dst,
                      upcr_shared_ptr_t src,
                      size_t nbytes,
                      upc_flag_t sync_mode
                      UPCRI_PT_ARG)
{
    UPCRI_PASS_GAS();
    sync_mode = upcri_coll_fixsync(sync_mode);
    UPCRI_TRACE_COLL(GATHER, nbytes);
    #define UPCRI_PEVT_ARGS , &dst, &src, nbytes, (int)sync_mode
    upcri_pevt_start(GASP_UPC_ALL_GATHER);
    UPCRI_COLL_GASNETIFY(gather,
				(GASNET_TEAM_ALL,
				 UPCRI_COLL_ONE_DST(dst),
				 UPCRI_COLL_MULTI_SRC(src),
				 nbytes, upcri_coll_syncmode(sync_mode)));
    upcri_pevt_end(GASP_UPC_ALL_GATHER);
    #undef UPCRI_PEVT_ARGS
}

void _upcr_all_gather_all(upcr_shared_ptr_t dst,
                          upcr_shared_ptr_t src,
                          size_t nbytes,
                          upc_flag_t sync_mode
                          UPCRI_PT_ARG)
{
    UPCRI_PASS_GAS();
    sync_mode = upcri_coll_fixsync(sync_mode);
    UPCRI_STRICT_HOOK_IF(sync_mode & UPC_IN_ALLSYNC);
    UPCRI_TRACE_COLL(GATHER_ALL, nbytes);
    #define UPCRI_PEVT_ARGS , &dst, &src, nbytes, (int)sync_mode
    upcri_pevt_start(GASP_UPC_ALL_GATHER_ALL);
    UPCRI_COLL_GASNETIFY(gather_all,
				(GASNET_TEAM_ALL,
				 UPCRI_COLL_MULTI_DST(dst),
				 UPCRI_COLL_MULTI_SRC(src),
				 nbytes, upcri_coll_syncmode(sync_mode)));
    upcri_pevt_end(GASP_UPC_ALL_GATHER_ALL);
    #undef UPCRI_PEVT_ARGS
}

void _upcr_all_exchange(upcr_shared_ptr_t dst,
                        upcr_shared_ptr_t src,
                        size_t nbytes,
                        upc_flag_t sync_mode
                        UPCRI_PT_ARG)
{
    UPCRI_PASS_GAS();
    sync_mode = upcri_coll_fixsync(sync_mode);
    UPCRI_STRICT_HOOK_IF(sync_mode & UPC_IN_ALLSYNC);
    UPCRI_TRACE_COLL(EXCHANGE, nbytes);
    #define UPCRI_PEVT_ARGS , &dst, &src, nbytes, (int)sync_mode
    upcri_pevt_start(GASP_UPC_ALL_EXCHANGE);
    UPCRI_COLL_GASNETIFY(exchange,
				(GASNET_TEAM_ALL,
				 UPCRI_COLL_MULTI_DST(dst),
				 UPCRI_COLL_MULTI_SRC(src),
				 nbytes, upcri_coll_syncmode(sync_mode)));
    upcri_pevt_end(GASP_UPC_ALL_EXCHANGE);
    #undef UPCRI_PEVT_ARGS
}

/* GASNet interface not yet (ever?) specified */
/* This one is an odd ball regardless, and may never follow the template */
void _upcr_all_permute(upcr_shared_ptr_t dst,
                       upcr_shared_ptr_t src,
                       upcr_pshared_ptr_t perm,
                       size_t nbytes,
                       upc_flag_t sync_mode
                       UPCRI_PT_ARG)
{
#if UPCRI_UPC_PTHREADS
    const int thread_count = upcri_mypthreads();
    const int first_thread = upcri_1stthread(upcri_mynode);
    const int i_am_master = (upcri_mypthread() == 0);
#else
    const int thread_count = 1;
    const int first_thread = upcr_mythread();
    const int i_am_master = 1;
#endif
    #if UPCRI_GASP
      upcr_shared_ptr_t permtmp = upcr_pshared_to_shared(perm);
    #endif
    int i;
    sync_mode = upcri_coll_fixsync(sync_mode);
    UPCRI_STRICT_HOOK_IF(sync_mode & UPC_IN_ALLSYNC);
    UPCRI_TRACE_COLL(PERMUTE, nbytes);

    #define UPCRI_PEVT_ARGS , &dst, &src, &permtmp, nbytes, (int)sync_mode
    upcri_pevt_start(GASP_UPC_ALL_PERMUTE);

    if (!(sync_mode & UPC_IN_NOSYNC)) {
	UPCRI_SINGLE_BARRIER();
    }
    if (i_am_master) { /* One distinguished thread per node does all the work */
	for (i = 0; i < thread_count; ++i) {
	    /* XXX: use puti? */
	    upcr_thread_t dst_th = *((int *)upcri_pshared_to_remote_withthread(perm, first_thread+i));
	    void *dst_addr = upcri_shared_to_remote_withthread(dst, dst_th);
	    void *src_addr = upcri_shared_to_remote_withthread(src, first_thread+i);
    
	    if (upcri_thread_is_addressable(dst_th)) {
		UPCRI_UNALIGNED_MEMCPY(UPCRI_COLL_LOCALIZE(dst_addr, dst_th), src_addr, nbytes);
	    } else {
		gex_RMA_PutNBI(upcri_tm, upcri_thread_to_node(dst_th), dst_addr, src_addr,
                                 nbytes, GEX_EVENT_DEFER, 0);
	    }
	}
        gex_NBI_Wait(GEX_EC_PUT,0);
    }
    if (!(sync_mode & UPC_OUT_NOSYNC)) {
	UPCRI_SINGLE_BARRIER();
    }

    upcri_pevt_end(GASP_UPC_ALL_PERMUTE);
    #undef UPCRI_PEVT_ARGS
}

/*----------------------------------------------------------------------------*/

#if defined(GASNET_TRACE)
const char *upcri_op2str(upc_op_t op) {
    switch (op) {
        case UPC_NONCOMM_FUNC: return("UPC_NONCOMM_FUNC");
        case UPC_FUNC:   return("UPC_FUNC");
        case UPC_ADD:    return("UPC_ADD");
        case UPC_MULT:   return("UPC_MULT");
        case UPC_MIN:    return("UPC_MIN");
        case UPC_MAX:    return("UPC_MAX");
        case UPC_AND:    return("UPC_AND");
        case UPC_OR:     return("UPC_OR");
        case UPC_LOGAND: return("UPC_LOGAND");
        case UPC_LOGOR:  return("UPC_LOGOR");
        case UPC_XOR:    return("UPC_XOR");
    }
    return "INVALID";
}
#endif

/*----------------------------------------------------------------------------*/
