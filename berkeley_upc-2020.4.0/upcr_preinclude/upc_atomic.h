/* Reference implementation of UPC atomics from UPC 1.3 
 * Written by Dan Bonachea 
 * Copyright 2018, The Regents of the University of California 
 * See LICENSE.TXT for licensing information.
 * See README for more information
 */

#if defined(__BERKELEY_UPC_FIRST_PREPROCESS__) || defined(__BERKELEY_UPC_ONLY_PREPROCESS__)
/* Every inclusion of <upc_atomic.h> has the effect of including <upc_types.h> */
#include <upc_types.h>
#endif

#ifndef _UPC_ATOMIC_H_
#define _UPC_ATOMIC_H_

/* Provide atomic library specific upc_op_t values, rest in upc_types.h */
// This value for UPC_GET takes advantage of an undocumented reserved bit 
// in the GEX op space that exists precisely to allow this optimization
#define UPC_GET   GEX_OP_TO_NONFETCHING(GEX_OP_GET)
#define UPC_SET   GEX_OP_SET
#define UPC_CSWAP GEX_OP_CAS
#define UPC_SUB   GEX_OP_SUB
#define UPC_INC   GEX_OP_INC
#define UPC_DEC   GEX_OP_DEC

typedef int upc_atomichint_t;

// ************************************************************
// UPC 1.3 Spec mandated upc_atomichint_t values

#define UPC_ATOMIC_HINT_DEFAULT      ((upc_atomichint_t)(0))
#define UPC_ATOMIC_HINT_LATENCY      ((upc_atomichint_t)(1<<0))
#define UPC_ATOMIC_HINT_THROUGHPUT   ((upc_atomichint_t)(1<<1))

// ************************************************************
// Implementation-defined hint extensions (spec allows these in UPC_ namespace)
// Clients should check for these using #ifdef to maximize portability

// These hints influence atomicdomain algorithm selection to favor the performance
// of accesses with the given locality property of the target memory location,
// with respect to the initiating rank.
//  NEAR - favor performance of AMOs on memory with affinity "near" the initiator
//  FAR  - favor performance of AMOs on memory with affinity "far" from the initiator
// Currently the NEAR/FAR distinction corresponds to whether the threads in question
// communicate through cache-coherent shared memory (NEAR), or over a system-level network (FAR).
// These hints are mutually exclusive, and the default behavior is system-specific.
#define UPC_ATOMIC_HINT_FAVOR_NEAR  ((upc_atomichint_t)(1<<2))
#define UPC_ATOMIC_HINT_FAVOR_FAR   ((upc_atomichint_t)(1<<3))

struct bupc_atomicdomain_S;
#if defined(__BERKELEY_UPC_FIRST_PREPROCESS__) || defined(__BERKELEY_UPC_ONLY_PREPROCESS__)
  #ifndef __UPC_ATOMIC__
  #error Missing predefine of __UPC_ATOMIC__
  #endif

  /* upc_atomicdomain_t is a shared datatype with incomplete type */
  typedef shared struct bupc_atomicdomain_S upc_atomicdomain_t;
  #define _BUPC_AD_PTR     upc_atomicdomain_t *
  #define _BUPC_AD_TGT_PTR shared void * restrict
#else
  #define _BUPC_AD_PTR     upcr_pshared_ptr_t
  #define _BUPC_AD_TGT_PTR upcr_shared_ptr_t
#endif

#if defined(__BERKELEY_UPC_FIRST_PREPROCESS__) || defined(UPCRI_LIBWRAP)
  #define _BUPC_AD_FN(name) upc_ ## name
  #define _BUPC_AD_PT_ARG 
#else
  #define _BUPC_AD_FN(name) _upcr_ ## name
  #define _BUPC_AD_PT_ARG UPCRI_PT_ARG
#endif

_BUPC_AD_PTR _BUPC_AD_FN(all_atomicdomain_alloc)(upc_type_t _type, upc_op_t _ops, 
                                                 upc_atomichint_t _hints _BUPC_AD_PT_ARG);

void _BUPC_AD_FN(all_atomicdomain_free)(_BUPC_AD_PTR _domain _BUPC_AD_PT_ARG);

void _BUPC_AD_FN(atomic_strict)(_BUPC_AD_PTR _domain,
    void * restrict _fetch_ptr, upc_op_t _op,
    _BUPC_AD_TGT_PTR _target,
    const void * restrict _operand1,
    const void * restrict _operand2 _BUPC_AD_PT_ARG);

void _BUPC_AD_FN(atomic_relaxed)(_BUPC_AD_PTR _domain,
    void * restrict _fetch_ptr, upc_op_t _op,
    _BUPC_AD_TGT_PTR _target,
    const void * restrict _operand1,
    const void * restrict _operand2 _BUPC_AD_PT_ARG);

int _BUPC_AD_FN(atomic_isfast)(upc_type_t _type, upc_op_t _ops, _BUPC_AD_TGT_PTR _addr _BUPC_AD_PT_ARG);

#if defined(__BERKELEY_UPC_SECOND_PREPROCESS__)
#define upc_all_atomicdomain_alloc(type, ops, hints) \
        (upcri_srcpos(), _upcr_all_atomicdomain_alloc(type, ops, hints UPCRI_PT_PASS))
#define upc_all_atomicdomain_free(ptr) \
        (upcri_srcpos(), _upcr_all_atomicdomain_free(ptr UPCRI_PT_PASS))
#define upc_atomic_strict(domain, fetch_ptr, op, target, operand1, operand2) \
        (upcri_srcpos(), _upcr_atomic_strict(domain, fetch_ptr, op, target, operand1, operand2 UPCRI_PT_PASS))
#define upc_atomic_relaxed(domain, fetch_ptr, op, target, operand1, operand2) \
        (upcri_srcpos(), _upcr_atomic_relaxed(domain, fetch_ptr, op, target, operand1, operand2 UPCRI_PT_PASS))
#define upc_atomic_isfast(type, ops, addr) \
        (upcri_srcpos(), _upcr_atomic_isfast(type, ops, addr UPCRI_PT_PASS))


#endif

#undef _BUPC_AD_PTR
#undef _BUPC_AD_TGT_PTR
#undef _BUPC_AD_PT_ARG 

#endif

