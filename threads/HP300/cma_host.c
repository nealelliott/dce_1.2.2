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
 * Revision 1.1.6.2  1996/02/17  23:32:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:06  marty]
 *
 * Revision 1.1.6.1  1995/12/07  21:45:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:58:56  root]
 * 
 * Revision 1.1.4.1  1994/06/09  13:33:58  devsrc
 * 	 CR10892 - fix copyright in file
 * 	[1994/06/09  13:25:11  devsrc]
 * 
 * Revision 1.1.2.2  1992/12/15  22:20:06  alan
 * 	Insert copyright notices
 * 	[1992/12/07  15:58:43  alan]
 * 
 * Revision 1.1  1992/01/19  14:39:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include "cma_tcb_defs.h"
#include "cma_dispatch.h"

void cma__create_thread(ctx, stk, tcb, func, arg)
cma__t_static_ctx *ctx;
void *stk;
cma__t_int_tcb *tcb;
void* (*func)();
void *arg;
{
    /*** Init static context */
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
    cma__t_static_ctx *ctx = &tcb->static_ctx;
    
    cma__thread_base(tcb, ctx->start_routine, ctx->arg);

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

    /*** Switch to new thread */
    cma__transfer_regs(cur_ctx, new_ctx);

    /*** Restore fp state??? */
}

/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_HP9000S300.C  */
/*   *1     6-DEC-1990 22:41:09 SCALES "HP-specific routines"  */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST_HP9000S300.C  */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.C  */
/*   *2    17-SEP-1991 14:55:16 BUTENHOF "Integrate CMA5 reverse drop"  */
/*   *1     6-DEC-1990 22:41:09 SCALES "HP-specific routines"  */
/*   DEC/CMS REPLACEMENT HISTORY, Element CMA_HOST.C  */
