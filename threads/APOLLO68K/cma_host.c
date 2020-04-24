/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_host.c,v $
 * Revision 1.1.6.2  1996/02/17  23:32:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:48:53  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:42:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:02  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:33:18  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:24:40  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:19:36  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:58:23  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <apollo/base.h>
#include <apollo/pfm.h>
#include "cma_tcb_defs.h"
#include "cma_dispatch.h"

#ifdef USE_US
#  include "/us/us/pm/src/pfm.pvt.c" /*???*/
#  include "/us/us/pm/src/pm.pvt.c" /*???*/
#else
  typedef struct {
      char *clnup_p;               /* most recent clnup hdlr rec */
      char *clhd_p;               /* first clnup rec */
  
  } pfm_$level;
  
  extern pfm_$level pfm_level;      /* current level info */
  extern status_$t pfm_$deferred_cleanup_status;
  
  extern boolean pm_$in_distinguished_task;
  extern boolean pm_$tasking_enabled;
  extern boolean pm_$cleanup_pending;
  extern pinteger pm_$level;
  extern short pm_$inh_count;
  extern short pm_$fault_inh_count;
  extern time_$clock_t pm_$cpu_time;
  extern boolean pm_$multi_tasking;
#endif



/*??? make sure host-specific interfaces are listed in cma_host_m68k_apollo.h */

#define SAVE_CTX(ctx) { \
    (ctx)->clnup_p =            pfm_level.clnup_p; \
    (ctx)->clhd_p =             pfm_level.clhd_p; \
    (ctx)->inh_count =          pm_$inh_count; \
    (ctx)->fault_inh_count =    pm_$fault_inh_count; \
    (ctx)->def_cleanup_status = pfm_$deferred_cleanup_status.all; \
    (ctx)->cleanup_pending =    pm_$cleanup_pending; \
}

/*??? inhibit around restore? */
#define RESTORE_CTX(ctx) { \
    pfm_level.clnup_p =            (ctx)->clnup_p; \
    pfm_level.clhd_p =             (ctx)->clhd_p; \
    pm_$fault_inh_count =          (ctx)->fault_inh_count; \
    pm_$inh_count =                (ctx)->inh_count; \
    pfm_$deferred_cleanup_status.all = (ctx)->def_cleanup_status; \
    pm_$cleanup_pending =          (ctx)->cleanup_pending; \
}

void cma__create_thread(ctx, stk, tcb, func, arg)
cma__t_static_ctx *ctx;
void *stk;
cma__t_int_tcb *tcb;
void* (*func)();
void *arg;
{
    /*** Init static context */
    ctx->clnup_p =                0;
    ctx->clhd_p =                 0; /*???*/
    ctx->inh_count =              0; /*???*/
    ctx->fault_inh_count =        0; /*???*/
    ctx->def_cleanup_status =     0;
    ctx->cleanup_pending =        false;
    ctx->start_routine =          func;
    ctx->arg =                    arg;

    cma__enter_kernel();
    cma__init_thread_stack(stk, tcb, ctx);
    cma__start_thread(tcb);
    cma__exit_kernel();
}

void cma__thread_start1(tcb)
cma__t_int_tcb *tcb;
{
    status_$t fst;
    pfm_$cleanup_rec crec;
    cma__t_static_ctx *ctx = &tcb->static_ctx;
    
    /*** Set pm state which is thread specific */
    RESTORE_CTX(ctx);

    /*** Set up a cleanup handler so the PFM cleanup list looks correctly 
     *** when the exception package does a longjmp. */
    fst = pfm_$cleanup(&crec);
    if (fst.all == pfm_$cleanup_set) {
        cma__thread_base(tcb, ctx->start_routine, ctx->arg);
        }
    else {
        cma__bugcheck ("cma__thread_start1: Shouldn't be here (cleanup)");
        }

    cma__bugcheck ("cma__thread_start1: Shouldn't be here (return)");

    /*** Never get here, cma__thread_base doesn't return. */
}

void cma__restore_thread_ctx(new_ctx)
cma__t_static_ctx *new_ctx;
{
    /*** Switch to new thread, abandoning current stack */
    cma__restore_regs(new_ctx);
}

void cma__transfer_thread_ctx(cur_ctx, new_ctx)
cma__t_static_ctx *cur_ctx;
cma__t_static_ctx *new_ctx;
{
    /*** Has the stack overflowed??? */

    /*** Save the fp state??? */

    /*** Save pm state which is thread specific */
    SAVE_CTX(cur_ctx);

    /*** Switch to new thread */
    cma__transfer_regs(cur_ctx, new_ctx);

    /*** Restore pm state which is thread specific */
    RESTORE_CTX(cur_ctx);

    /*** Restore fp state??? */
}

/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_M68K_APOLLO.C */
/*  *3    21-JUN-1990 12:51:52 BUTENHOF "Optimize mutexes" */
/*  *2    18-MAY-1990 07:10:03 BUTENHOF "Optimize mutex and cv" */
/*  *1    17-APR-1990 07:17:37 BUTENHOF "Apollo's host source" */
/*  DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_M68K_APOLLO.C */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.C */
/*   *2    17-SEP-1991 15:05:50 BUTENHOF "Integrate CMA5 reverse drop" */
/*   *1     7-DEC-1990 16:26:07 SCALES "HP-specific routines" */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.C */
