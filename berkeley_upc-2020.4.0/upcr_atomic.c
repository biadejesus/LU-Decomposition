/*   $Source: bitbucket.org:berkeleylab/upc-runtime.git/upcr_atomic.c $
 * Description: Berkeley UPC atomic operations
 *
 * Copyright 2018, Dan Bonachea <bonachea@cs.berkeley.edu>
 */

#include <upcr.h>
#include <upcr_internal.h>
#include <gasnet_ratomic.h>

/* ------------------------------------------------------------------------------- */

GASNETT_IDENT(upcri_IdentString_Atomics, "$UPCAtomics: Version 2.0 $");

#define OPS_ACCESS  ((upc_op_t)(UPC_GET | UPC_SET | UPC_CSWAP))
#define OPS_BITWISE ((upc_op_t)(UPC_AND | UPC_OR | UPC_XOR))
#define OPS_NUMERIC ((upc_op_t)(UPC_ADD | UPC_SUB | UPC_MULT | \
                                UPC_INC | UPC_DEC | \
                                UPC_MAX | UPC_MIN))

#define OPS_ALL   (OPS_ACCESS | OPS_BITWISE | OPS_NUMERIC)
#define OPS_INT   OPS_ALL
#define OPS_PTS   OPS_ACCESS
#define OPS_FLT   (OPS_ACCESS | OPS_NUMERIC)

// private-use convenience definition, which is guaranteed not to collide with GEX
#define GEX_DT_PTS 0

#define UNSUPPORTED_TYPES(func)                                            \
      case UPC_CHAR:    func(signed char,        UPC_CHAR,    INT, NONE); break; \
      case UPC_UCHAR:   func(unsigned char,      UPC_UCHAR,   INT, NONE); break; \
      case UPC_SHORT:   func(short,              UPC_SHORT,   INT, NONE); break; \
      case UPC_USHORT:  func(unsigned short,     UPC_USHORT,  INT, NONE); break; \
      case UPC_LLONG:   func(long long,          UPC_LLONG,   INT, NONE); break; \
      case UPC_ULLONG:  func(unsigned long long, UPC_ULLONG,  INT, NONE); break; \
      case UPC_INT8:    func(int8_t,             UPC_INT8,    INT, NONE); break; \
      case UPC_UINT8:   func(uint8_t,            UPC_UINT8,   INT, NONE); break; \
      case UPC_INT16:   func(int16_t,            UPC_INT16,   INT, NONE); break; \
      case UPC_UINT16:  func(uint16_t,           UPC_UINT16,  INT, NONE); break; \
      case UPC_LDOUBLE: func(long double,        UPC_LDOUBLE, FLT, NONE); break;   

#if SIZEOF_LONG == 4
  #define _SWITCH_TYPE_LONG(func) \
      case UPC_LONG:    func(long,          UPC_LONG,   INT, I32); break; \
      case UPC_ULONG:   func(unsigned long, UPC_ULONG,  INT, U32); break; 
#else
  #define _SWITCH_TYPE_LONG(func) \
      case UPC_LONG:    func(long,          UPC_LONG,   INT, I64); break; \
      case UPC_ULONG:   func(unsigned long, UPC_ULONG,  INT, U64); break; 
#endif

#define SWITCH_TYPE(t, func, unsup_func, nomatch) do {                 \
    switch (t) {                                                       \
      case UPC_INT32:   func(int32_t,       UPC_INT32,  INT, I32); break; \
      case UPC_UINT32:  func(uint32_t,      UPC_UINT32, INT, U32); break; \
      case UPC_INT64:   func(int64_t,       UPC_INT64,  INT, I64); break; \
      case UPC_UINT64:  func(uint64_t,      UPC_UINT64, INT, U64); break; \
      case UPC_INT:     func(int,           UPC_INT,    INT, I32); break; \
      case UPC_UINT:    func(unsigned int,  UPC_UINT,   INT, U32); break; \
      case UPC_FLOAT:   func(float,         UPC_FLOAT,  FLT, FLT); break; \
      case UPC_DOUBLE:  func(double,        UPC_DOUBLE, FLT, DBL); break; \
      case UPC_PTS:     func(upcr_shared_ptr_t,UPC_PTS, PTS, PTS); break; \
      _SWITCH_TYPE_LONG(func)                                          \
      UNSUPPORTED_TYPES(unsup_func)                                    \
      default: nomatch;                                                \
    }                                                                  \
} while(0)

#define SWITCH_GEXDT(t, func, ptsaction) do {         \
  switch(t) {                                         \
    case GEX_DT_I32: func(int32_t,  INT, I32); break; \
    case GEX_DT_U32: func(uint32_t, INT, U32); break; \
    case GEX_DT_I64: func(int64_t,  INT, I64); break; \
    case GEX_DT_U64: func(uint64_t, INT, U64); break; \
    case GEX_DT_FLT: func(float,    FLT, FLT); break; \
    case GEX_DT_DBL: func(double,   FLT, DBL); break; \
    case GEX_DT_PTS: ptsaction; break;                \
    default: gasnett_unreachable();                   \
  }                                                   \
} while (0)

#define NOOP4(a,b,c,d) ((void)0)

#if UPCR_DEBUG
  /* check an ops flag block */
  #define CHECK_VALIDOPS(ops) do {                                                      \
    upc_op_t _ops = (ops);                                                              \
    if ((_ops == 0) || (_ops & ~OPS_ALL))                                               \
      upcri_err("invalid upc_op_t value (%llu)", (unsigned long long)_ops);             \
  } while (0)
  /* check a single op value */
  #define CHECK_VALIDOP(op) do {                                                        \
    upc_op_t _op = (op);                                                                \
    unsigned long long _tmp = (unsigned long long)_op;                                  \
    CHECK_VALIDOPS(_op);                                                                \
    while ((_tmp & 0x1) == 0) _tmp >>= 1;                                               \
    if (_tmp > 1)                                                                       \
      upcri_err("invalid unique upc_op_t value (%llu)", (unsigned long long)_op);       \
  } while (0)
#else
  #define CHECK_VALIDOPS(ops) ((void)0)
  #define CHECK_VALIDOP(op)   ((void)0)
#endif

/* ------------------------------------------------------------------------------- */
static const char *type_str(upc_type_t type) {
  static char tmp[80];
  #define RETURN_TYPE_STR(ctype, typemacro, typecat, gexdt) return #typemacro;
  SWITCH_TYPE(type, RETURN_TYPE_STR, RETURN_TYPE_STR, );
  #undef RETURN_TYPE_STR
  snprintf(tmp, sizeof(tmp), "Unknown upc_type_t (%d)", (int)type);
  return tmp;
}
static const char *op_str(upc_op_t op) {
  static char tmp[80];
  switch (op) {
    #define RETURN_OP_STR(o) case o: return #o
    RETURN_OP_STR(UPC_AND);
    RETURN_OP_STR(UPC_OR);
    RETURN_OP_STR(UPC_XOR);
    RETURN_OP_STR(UPC_ADD);
    RETURN_OP_STR(UPC_SUB);
    RETURN_OP_STR(UPC_MULT);
    RETURN_OP_STR(UPC_MIN);
    RETURN_OP_STR(UPC_MAX);
    RETURN_OP_STR(UPC_INC);
    RETURN_OP_STR(UPC_DEC);
    RETURN_OP_STR(UPC_CSWAP);
    RETURN_OP_STR(UPC_SET);
    RETURN_OP_STR(UPC_GET);
    #undef RETURN_OP_STR
  }
  snprintf(tmp, sizeof(tmp), "Unknown upc_op_t (%d)", (int)op);
  return tmp;
}
static void check_valid_typeop(upc_type_t type, upc_op_t ops) {
  #define CHECKOP_TYPE(ctype, typemacro, typecat, gexdt) \
    if (ops & ~OPS_##typecat) upcri_err("Unsupported upc_op_t value for type " #typemacro); 
  #define UNSUP_TYPE(ctype, typemacro, typecat, gexdt) \
    upcri_err(#typemacro " is not a supported upc_type_t type for upc_atomic"); 
  SWITCH_TYPE(type, CHECKOP_TYPE, UNSUP_TYPE, upcri_err("Unrecognized upc_type_t type argument"));
  #undef CHECKOP_TYPE
  #undef UNSUP_TYPE 
}

/* ------------------------------------------------------------------------------- */

typedef struct { 
  gex_AD_t gexad;
  gex_DT_t gexdt;
  #if UPCR_DEBUG
    uint64_t magic;
    upc_op_t ops;
    upc_atomichint_t hints;
  #endif
  #if UPCR_DEBUG || GASNET_TRACE
    upc_type_t type;
  #endif
} upcr_ad_t;
#define UPCRI_LAD_MAGIC 0x300d300dad300dadLLU
#define UPCRI_BAD_MAGIC 0x0badadbadadbadadLLU

#if UPCR_DEBUG
 static void CHECK_LAD(upcr_ad_t const *lad) {
   upcri_assert(lad);
   if (lad->magic != UPCRI_LAD_MAGIC) upcri_err("Invalid or corrupt upc_atomicdomain_t");
   upcri_assert(lad->gexad != GEX_AD_INVALID);
   CHECK_VALIDOPS(lad->ops);
 }
#else
  #define CHECK_LAD(lad) ((void)0)
#endif

/* return given thread's representative for a user-provided (upc_atomicdomain_t *) */
GASNETT_INLINE(upcri_ad_to_lad)
upcr_ad_t *upcri_ad_to_lad(upcr_pshared_ptr_t ad, upcr_thread_t id) {
  upcri_assert(upcr_threadof_pshared(ad) == 0);
  //return upcri_pshared_to_remote(upcr_add_pshared1(ad, sizeof(upcr_ad_t), id));
  return upcri_pshared_to_remote_withthread(ad, id);
}
#if 0
  // TODO-EX: What we actually want here is upcri_pshared_to_mylocal_fast, which does not exist
  #define upcri_ad_to_mylad(ad) (void *) (upcr_addrfield_pshared(ad) UPCRI_PLUS_MY_OFFSET)
#else // slower version based on the generic function
  #define upcri_ad_to_mylad(ad) upcri_ad_to_lad(ad, upcr_mythread())
#endif

/* ------------------------------------------------------------------------------- */

upcr_pshared_ptr_t _BUPC_AD_FN(all_atomicdomain_alloc)(upc_type_t type, upc_op_t ops, 
                                                 upc_atomichint_t hints UPCRI_PT_ARG) {
  UPCRI_PASS_GAS();
  #if UPCR_DEBUG
    // use broadcast to validate single valued args
    struct { 
      upc_type_t type;
      upc_op_t ops;
      upc_atomichint_t hints;
    } check = { type, ops, hints };
    upcri_broadcast(0, &check, sizeof(check));
    if (check.type != type || check.ops != ops || check.hints != hints)
      upcri_err("Arguments to upc_all_atomicdomain_alloc must be single-valued!");
    // verify permitted combination
    CHECK_VALIDOPS(ops);
    check_valid_typeop(type, ops);
  #endif
  upcr_shared_ptr_t _ad = upcr_all_alloc(upcr_threads(), sizeof(upcr_ad_t));
  upcr_pshared_ptr_t ad = upcr_shared_to_pshared(_ad);
  upcr_ad_t *lad = upcri_ad_to_mylad(ad);
  memset(lad, 0, sizeof(upcr_ad_t));
  #if UPCR_DEBUG || GASNET_TRACE
    lad->type = type;
  #endif
  #if UPCR_DEBUG
    lad->ops = ops;
    lad->hints = hints;
    lad->magic = UPCRI_LAD_MAGIC;
  #endif
  gex_Flags_t gexflags = 0;
  if      (hints & UPC_ATOMIC_HINT_FAVOR_FAR)  gexflags |= GEX_FLAG_AD_FAVOR_REMOTE;
  else if (hints & UPC_ATOMIC_HINT_FAVOR_NEAR) gexflags |= GEX_FLAG_AD_FAVOR_MY_NBRHD;
  gex_OP_t gexops = (gex_OP_t)ops;
  { // UPC ops encode the non-fetching GEX variant, add the fetching variants
    upc_op_t op = 1;
    upc_op_t tmp = ops;
    while (tmp) {
      if (tmp & 0x1) {
        if (op == UPC_GET) {
          gexops &= ~UPC_GET;
          gexops |= GEX_OP_GET;
        } else gexops |= GEX_OP_TO_FETCHING((gex_OP_t)op);
      }
      tmp >>= 1;
      op <<= 1;
    }
  }
  static gex_AD_t gexad;
  upcri_pthread_barrier();
  #define AD_CREATE(ctype, typemacro, typecat, _gexdt)                     \
    lad->gexdt = GEX_DT_##_gexdt;                                          \
    if (upcri_mypthread() == 0) {                                          \
      gex_DT_t gdt = (lad->gexdt == GEX_DT_PTS ? GEX_DT_U64 : lad->gexdt); \
      gex_AD_Create(&gexad, upcri_tm, gdt, gexops, gexflags);              \
    }
  SWITCH_TYPE(type, AD_CREATE, NOOP4, );
  #undef AD_CREATE
  upcri_pthread_barrier();
  lad->gexad = gexad;
  CHECK_LAD(lad);
  return ad;
}

void _BUPC_AD_FN(all_atomicdomain_free)(upcr_pshared_ptr_t domain UPCRI_PT_ARG) {
  UPCRI_PASS_GAS();
  #if UPCR_DEBUG
    upcr_pshared_ptr_t check = domain;
    upcri_broadcast(0, &check, sizeof(check));
    if (!upcr_isequal_pshared_pshared(check,domain)) 
      upcri_err("Domain argument to upc_all_atomicdomain_free must be single-valued!");
  #endif
  if (upcr_isnull_pshared(domain)) return;
  upcr_ad_t *lad = upcri_ad_to_mylad(domain);
  CHECK_LAD(lad);
  gex_AD_t gexad = lad->gexad;
  #if UPCR_DEBUG
    lad->magic = UPCRI_BAD_MAGIC;
  #endif
  UPCRI_SINGLE_BARRIER();
  if (upcri_mypthread() == 0) gex_AD_Destroy(gexad);
  if (upcr_mythread() == 0) {
    upcr_free(upcr_pshared_to_shared(domain));
  }
}

/* ------------------------------------------------------------------------------- */
// Struct PTS AMO support
// AM-based algorithm to handle 128-bit AMOs not yet natively supported in GASNet
#if UPCRI_USE_AM_PTS_AMO
// AM-based AMO protocol for handling UPC_PTS ops with arbitrary-sized PTS
//
// Uses a centralized HSL for serialization:
// This over-synchronizes which theoretically increases contention for multi-domain PTS apps,
// but the trade-off is it always reduces bits on the wire
static gex_HSL_t upcri_pts_amo_hsl = GEX_HSL_INITIALIZER;
// algorithm is currently fully blocking, but designed to allow adaptation to NB if we ever expose that
typedef struct {  
  void *fetchptr;
  upc_op_t op; // also serves as done flag
} upcri_amo_op_t;

GASNETT_INLINE(upcri_do_AMO_PTS)
void upcri_do_AMO_PTS(upc_op_t op, upcr_shared_ptr_t *target, 
                      const upcr_shared_ptr_t *op1, const upcr_shared_ptr_t *op2, 
                      upcr_shared_ptr_t *result) {
  gex_HSL_Lock(&upcri_pts_amo_hsl);
    upcri_assert(result);
    *result = *target;
    switch (op) {
      case UPC_GET: 
        /* done */
        break;
      case UPC_SET:
        upcri_assert(op1);
        *target = *op1;
        break;
      case UPC_CSWAP:
        upcri_assert(op1 && op2);
        if (upcr_isequal_shared_shared(*target, *op1)) *target = *op2;
        break;
      default: gasnett_unreachable();   
  }
  gex_HSL_Unlock(&upcri_pts_amo_hsl);
}

void upcri_MRQ_AMO_PTS(gex_Token_t token, void *addr, size_t nbytes,
                       gex_AM_Arg_t _op, void *target, void *_amo_op_p) {
  upcr_shared_ptr_t result;
  upc_op_t op = (upc_op_t)_op;
  upcr_shared_ptr_t *operand = addr;
  upcri_do_AMO_PTS(op, target, operand, operand+1, &result);
  gex_AM_ReplyMedium(token, UPCRI_HANDLER_ID(upcri_MRP_AMO_PTS), 
                     &result, sizeof(result), GEX_EVENT_NOW, 0,
                     UPCRI_SEND_PTR(_amo_op_p));
}

void upcri_MRP_AMO_PTS(gex_Token_t token, void *addr, size_t nbytes,
                       void *_amo_op_p) {
  upcri_amo_op_t *amo_op_p = _amo_op_p;
  if (amo_op_p->fetchptr) {
    upcri_assert(nbytes > 0 && addr);
    memcpy(amo_op_p->fetchptr, addr, nbytes);
    gasnett_local_wmb();
  }
  amo_op_p->op = 0; // mark done
}
#endif // UPCRI_USE_AM_PTS_AMO
/* ------------------------------------------------------------------------------- */

GASNETT_INLINE(upcri_ad_op)
bupc_handle_t upcri_ad_op(int isstrict, int isNB, int isNBI, const char *context,
                  upcr_pshared_ptr_t domain,
                  void * restrict fetch_ptr, upc_op_t op,
                  upcr_shared_ptr_t target,
                  const void * restrict operand1,
                  const void * restrict operand2 UPCRI_PT_ARG) {
  UPCRI_PASS_GAS();
  upcr_ad_t const * const lad = upcri_ad_to_mylad(domain);
  #if UPCR_DEBUG
    CHECK_LAD(lad);
    if (upcr_isnull_shared(target)) upcri_err("NULL target address passed to %s", context);
    CHECK_VALIDOP(op);
    if (!(op & lad->ops)) upcri_err("%s invalid for this upc_atomicdomain_t in %s", op_str(op), context);
    if (op == UPC_GET && !fetch_ptr) upcri_err("fetch_ptr must be non-null for UPC_GET in %s", context);
    if (op == UPC_GET || op == UPC_INC || op == UPC_DEC) {
      if (operand1) upcri_err("operand1 must be null for %s in %s", op_str(op), context);
    } else {
      if (!operand1) upcri_err("operand1 must not be null for %s in %s", op_str(op), context);
    }
    if (op == UPC_CSWAP) {
      if (!operand2) upcri_err("operand2 must not be null for %s in %s", op_str(op), context);
    } else {
      if (operand2) upcri_err("operand2 must be null for %s in %s", op_str(op), context);
    }
    if ((fetch_ptr && fetch_ptr == operand1) ||
        (fetch_ptr && fetch_ptr == operand2) ||
        (operand1 && operand1 == operand2))
      upcri_err("pointer arguments to %s must not alias (restrict)", context);
    if (upcr_threadof_shared(target) == upcr_mythread()) {
       void *ltarget = upcr_shared_to_local(target);
       if (ltarget == fetch_ptr || ltarget == operand1 || ltarget == operand2)
         upcri_err("pointer arguments to %s must not alias (restrict)", context);
    }
  #endif

  gex_OP_t const gexop = (fetch_ptr ? GEX_OP_TO_FETCHING((gex_OP_t)op) : (gex_OP_t)op);
  gex_Flags_t const gexflags = (isstrict ? GEX_FLAG_AD_ACQ | GEX_FLAG_AD_REL : 0);
  gex_Rank_t const tgtrank = upcri_shared_nodeof(target);
  void * const tgtaddr = upcri_shared_to_remote(target);

  #if GASNET_TRACE
    upc_type_t const type = lad->type;
    char ptrstr[UPCRI_DUMP_MIN_LENGTH];
    upcri_dump_shared(target, ptrstr, UPCRI_DUMP_MIN_LENGTH);

    UPCRI_TRACE_PRINTF_NOPOS(("%s: type=0x%x(%s), op=0x%x(%s), gexop=0x%x, fetch=%p, op1=%p, op2=%p target=%s\n", 
       context, (int)lad->type, type_str(lad->type), (int)op, op_str(op), gexop, fetch_ptr, operand1, operand2, ptrstr));
  #endif

  #define STAGE_OPERANDS(ctype) \
      ctype const op1 = (operand1 ? *(ctype *)operand1 : 0); \
      ctype const op2 = (operand2 ? *(ctype *)operand2 : 0)

  #define OP_NBI(ctype, typecat, gexdt) do {                  \
    STAGE_OPERANDS(ctype);                                    \
    gex_AD_OpNBI_##gexdt(lad->gexad, fetch_ptr, tgtrank, tgtaddr, gexop, op1, op2, gexflags); \
    return NULL;                                              \
  } while (0)

  #define OP_NB(ctype, typecat, gexdt) do {                   \
    STAGE_OPERANDS(ctype);                                    \
    evt = gex_AD_OpNB_##gexdt(lad->gexad, fetch_ptr, tgtrank, tgtaddr, gexop, op1, op2, gexflags); \
  } while (0)
  #define OP_NB_RETURN() do {                                 \
    if (isNB) { /* need UPCRI_EOP_ENCAPSULATE */              \
      upcri_err("NB atomics not yet implemented");            \
    } else { gex_Event_Wait(evt); return NULL; }              \
  } while (0)


  #if UPCRI_USE_AM_PTS_AMO
    // An AM-based implementation could (evnetually) expose overlap and handles
    // 128-bit struct sptrs, but gains sensitivity to remote attentiveness.
    #define OP_PTS do {                                                              \
      if (tgtrank == upcri_mynode) { /* HSL only allows bypass for same-process */   \
        upcr_shared_ptr_t *fetch = fetch_ptr;                                        \
        upcr_shared_ptr_t _fetch;  /* handle non-fetching ops */                     \
        if (!fetch) fetch = &_fetch;                                                 \
        upcri_do_AMO_PTS(op, tgtaddr, operand1, operand2, fetch);                    \
      } else { /* different process */                                               \
        upcri_amo_op_t amo_op;                                                       \
        amo_op.fetchptr = fetch_ptr;                                                 \
        amo_op.op = op;                                                              \
        upcr_shared_ptr_t operands[2];                                               \
        void *srcbuf = NULL;                                                         \
        size_t nbytes = 0;                                                           \
        if (op == UPC_SET) {                                                         \
          srcbuf = (void *)operand1;                                                 \
          nbytes = sizeof(upcr_shared_ptr_t);                                        \
        } else if (op == UPC_CSWAP) {                                                \
          operands[0] = *(upcr_shared_ptr_t*)operand1;                               \
          operands[1] = *(upcr_shared_ptr_t*)operand2;                               \
          srcbuf = &operands;                                                        \
          nbytes = sizeof(operands);                                                 \
        } else upcri_assert(op == UPC_GET);                                          \
        gex_AM_RequestMedium(upcri_tm, tgtrank,                                      \
                             UPCRI_HANDLER_ID(upcri_MRQ_AMO_PTS),                    \
                             srcbuf, nbytes, GEX_EVENT_NOW, 0,                       \
                             op, UPCRI_SEND_PTR(tgtaddr), UPCRI_SEND_PTR(&amo_op));  \
        GASNET_BLOCKUNTIL(amo_op.op == 0);                                           \
      }                                                                              \
      return NULL;                                                                   \
    } while (0)
  #elif UPCRI_PACKED_SPTR
    // PTS CSWAP has a special requirement that it ignores the phase field, 
    // so cannot directly use an unmasked 64-bit integer CSWAP.
    // Currently special-cased to a fully-blocking algorithm written 
    // in terms of U64 CSWAP to be amenable to offload/RMA. 
    #define PTS_FCAS(old,new) \
      gex_Event_Wait(gex_AD_OpNB_U64(lad->gexad, fetch_ptr, tgtrank, tgtaddr, \
                                   GEX_OP_FCAS, old, new, gexflags))
    #define OP_PTS do {                                                      \
     if (op == UPC_CSWAP) {                                                  \
      upcri_assert(operand1 && operand2);                                    \
      upcri_assert(sizeof(upcr_shared_ptr_t) == 8);                          \
      upcr_shared_ptr_t old = *(upcr_shared_ptr_t *)operand1;                \
      upcr_shared_ptr_t new = *(upcr_shared_ptr_t *)operand2;                \
      upcr_shared_ptr_t *fetch = fetch_ptr;                                  \
      upcr_shared_ptr_t _fetch;  /* handle non-fetching CAS */               \
      if (!fetch) fetch = &_fetch;                                           \
      PTS_FCAS(old,new);                                                     \
      if (*fetch == old) return NULL; /* simple success */                   \
      if_pt (!upcr_isequal_shared_shared(*fetch,old))                        \
         return NULL; /* simple failure (addr or thread differs) */          \
      _CONCAT(retry,__LINE__):; /* unlikely: phase difference collision */   \
      upcr_shared_ptr_t fakeold = upcri_addrfield_to_shared(                 \
                                    upcr_addrfield_shared(old),              \
                                    upcr_threadof_shared(old),               \
                                    upcr_phaseof_shared(*fetch));            \
      PTS_FCAS(fakeold,new);                                                 \
      if_pt (*fetch == fakeold) return NULL; /* success w/ adjusted phase */ \
      else { /* lost a race with another mutator */                          \
        if_pt (!upcr_isequal_shared_shared(*fetch,old))                      \
          return NULL; /* failure - racing mutator changed addr or thread */ \
        else /* VERY unlikely: racing mutator changed only the phase field */\
          goto _CONCAT(retry,__LINE__);                                      \
      }                                                                      \
     } else if (isNBI) { /* PTS get/set NBI */                               \
        OP_NBI(upcr_shared_ptr_t, PTS, U64);                                 \
     } else {            /* PTS get/set NB */                                \
        gex_Event_t evt;                                                     \
        OP_NB(upcr_shared_ptr_t, PTS, U64);                                  \
        OP_NB_RETURN();                                                      \
     }                                                                       \
    } while (0)
  #else // should never happen
    #define OP_PTS upcri_err("PTS AMOs unimplemented for this configuration")  
  #endif

  if (isNBI) { 
    SWITCH_GEXDT(lad->gexdt, OP_NBI, OP_PTS);
  } else {
    gex_Event_t evt;
    SWITCH_GEXDT(lad->gexdt, OP_NB, OP_PTS);
    OP_NB_RETURN();
  }
  #undef OP_NBI
  #undef OP_NB
  #undef OP_PTS
  #undef PTS_CSWAP
  #undef PTS_FCAS
  #undef STAGE_OPERANDS
}

GASNETT_HOT
void _BUPC_AD_FN(atomic_strict)(upcr_pshared_ptr_t domain,
    void * restrict fetch_ptr, upc_op_t op,
    upcr_shared_ptr_t target,
    const void * restrict operand1,
    const void * restrict operand2 UPCRI_PT_ARG) {
    upcri_ad_op(1,0,0, "UPC_ATOMIC_STRICT",
                domain, fetch_ptr, op, target, operand1, operand2 UPCRI_PT_PASS);
}

GASNETT_HOT
void _BUPC_AD_FN(atomic_relaxed)(upcr_pshared_ptr_t domain,
    void * restrict fetch_ptr, upc_op_t op,
    upcr_shared_ptr_t target,
    const void * restrict operand1,
    const void * restrict operand2 UPCRI_PT_ARG) {
    upcri_ad_op(0,0,0, "UPC_ATOMIC_RELAXED",
                domain, fetch_ptr, op, target, operand1, operand2 UPCRI_PT_PASS);
}

int _BUPC_AD_FN(atomic_isfast)(upc_type_t type, upc_op_t ops, upcr_shared_ptr_t addr UPCRI_PT_ARG) {
  #if UPCR_DEBUG
    if (upcr_isnull_shared(addr)) upcri_err("NULL addr passed to upc_atomic_isfast");
    CHECK_VALIDOPS(ops);
    check_valid_typeop(type, ops);
  #endif
  // TODO-EX: use a programmatic GEX interface to query offload status of type/ops combo
  return upcr_threadof_shared(addr) == upcr_mythread();
}


/*---------------------------------------------------------------------------------*/

