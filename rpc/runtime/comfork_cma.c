/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comfork_cma.c,v $
 * Revision 1.1.18.1  1996/05/10  13:09:51  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/04/18  19:13 UTC  bissen
 * 	unifdef for single threaded client
 * 	[1996/02/29  20:41 UTC  bissen  /main/HPDCE02/bissen_st_rpc/1]
 *
 * 	Submitted the fix for CHFts15608.
 * 	[1995/07/14  19:06 UTC  tatsu_s  /main/HPDCE02/3]
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:27 UTC  psn
 * 	Remove Mothra specific code
 * 	[1995/11/16  21:30 UTC  jrr  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Submitted the fix for CHFts15608.
 * 	[1995/07/14  19:06 UTC  tatsu_s  /main/HPDCE02/3]
 *
 * 	Re-enabled the atfork handler in the server.
 * 	[1995/07/06  20:56 UTC  tatsu_s  /main/HPDCE02/tatsu_s.local_rpc.b4/1]
 *
 * 	Submitted the local rpc cleanup.
 * 	[1995/01/31  21:16 UTC  tatsu_s  /main/HPDCE02/2]
 *
 * 	Added rpc__fork_is_in_progress().
 * 	[1995/01/25  20:33 UTC  tatsu_s  /main/HPDCE02/tatsu_s.vtalarm.b0/1]
 *
 * 	Submitted the CN timer fix and the server atfork fix.
 * 	[1995/01/13  14:35 UTC  tatsu_s  /main/HPDCE02/1]
 *
 * 	Turned off the atfork handler in the server.
 * 	[1995/01/12  22:28 UTC  tatsu_s  /main/tatsu_s.mothra_cn_fix.b1/1]
 *
 * Revision 1.1.14.2  1994/07/15  16:56:10  tatsu_s
 * 	Fix OT9794: Pre-fork handler causes the fork'ed child coredump.
 * 	Do nothing in the fork handler, if RPC runtime is not initialized.
 * 	[1994/07/09  02:41:58  tatsu_s]
 * 
 * Revision 1.1.14.1  1994/01/21  22:35:16  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:55:05  cbrooks]
 * 
 * Revision 1.1.12.1  1993/09/15  15:39:45  damon
 * 	Synched with ODE 2.1 based build
 * 	[1993/09/15  15:31:59  damon]
 * 
 * Revision 1.1.2.2  1993/09/14  16:48:19  tatsu_s
 * 	Initial revision
 * 	[1993/09/13  17:08:08  tatsu_s]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989,1993 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      rpc/runtime/comfork_cma.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Include CMA atfork grodiness.
**
**
*/

#include <commonp.h>
#include <com.h>        /* Externals for Common Services component  */

#if defined(ATFORK_SUPPORTED)

/*
 * Boolean indicating whether fork is in progress or not.
 */
INTERNAL volatile boolean32 atfork_in_progress = false;

#if defined(CMA_INCLUDE)

/*
 * This really doesn't belong here, but is here for pragmatic reasons.
 */

typedef void (*atfork_handler_ptr_t) (rpc_fork_stage_id_t /*stage*/);
/*
 * Save the address of the fork handler registered through atfork.  
 * This pointer should be saved ONLY once by rpc__cma_atfork(). If
 * rpc__cma_atfork() is called more than once, which happens in the
 * child context when rpc__init() is called, do not register the
 * fork handler.
 */
INTERNAL volatile atfork_handler_ptr_t  atfork_handler_ptr = NULL;

INTERNAL struct atfork_user_data_t
{
    unsigned32  pid;    /* who registered */
    unsigned32  pre;    /* How many times _pre_fork called */
    unsigned32  post_p; /*                _post_fork_parent called */
    unsigned32  post_c; /*                _post_fork_child called */
} atfork_user_data;

/* ========================================================================= */

INTERNAL void _pre_fork _DCE_PROTOTYPE_((
        cma_t_address   /*arg*/
    ));

INTERNAL void _post_fork_child _DCE_PROTOTYPE_((
        cma_t_address   /*arg*/
    ));

INTERNAL void _post_fork_parent _DCE_PROTOTYPE_((
        cma_t_address   /*arg*/
    ));

/* ========================================================================= */

/*
 * _ P R E _ F O R K
 *
 * This procedure is called by the Pthreads library prior to calling
 * the fork/vfork system call.
 */         
INTERNAL void _pre_fork
#ifdef _DCE_PROTO_
(
  cma_t_address arg 
)
#else
(arg)
cma_t_address arg;
#endif
{                      
    RPC_DBG_PRINTF(rpc_e_dbg_atfork, 1,
               ("(_pre_fork) entering, pid %d, pre %d, post_p %d, post_c %d\n",
                    ((struct atfork_user_data_t *) arg)->pid,
                    ((struct atfork_user_data_t *) arg)->pre,
                    ((struct atfork_user_data_t *) arg)->post_p,
                    ((struct atfork_user_data_t *) arg)->post_c));

    atfork_in_progress = true;
    ((struct atfork_user_data_t *) arg)->pre++;

    if (rpc_g_initialized == true)
        (*atfork_handler_ptr)(RPC_C_PREFORK);

    RPC_DBG_PRINTF(rpc_e_dbg_atfork, 1,
              ("(_pre_fork) returning, pid %d, pre %d, post_p %d, post_c %d\n",
                    ((struct atfork_user_data_t *) arg)->pid,
                    ((struct atfork_user_data_t *) arg)->pre,
                    ((struct atfork_user_data_t *) arg)->post_p,
                    ((struct atfork_user_data_t *) arg)->post_c));
}

/*
 * _ P O S T _ F O R K _ C H I L D
 *
 * This procedure is called in the child of a forked process immediately
 * after the fork is performed.
 */         

INTERNAL void _post_fork_child
#ifdef _DCE_PROTO_
(
  cma_t_address arg
)
#else
(arg)
cma_t_address arg;
#endif
{
    RPC_DBG_PRINTF(rpc_e_dbg_atfork, 1,
        ("(_post_fork_child) entering, pid %d, pre %d, post_p %d, post_c %d\n",
                    ((struct atfork_user_data_t *) arg)->pid,
                    ((struct atfork_user_data_t *) arg)->pre,
                    ((struct atfork_user_data_t *) arg)->post_p,
                    ((struct atfork_user_data_t *) arg)->post_c));

    ((struct atfork_user_data_t *) arg)->post_c++;

    if (rpc_g_initialized == true)
        (*atfork_handler_ptr)(RPC_C_POSTFORK_CHILD);

    atfork_in_progress = false;

    RPC_DBG_PRINTF(rpc_e_dbg_atfork, 1,
       ("(_post_fork_child) returning, pid %d, pre %d, post_p %d, post_c %d\n",
                    ((struct atfork_user_data_t *) arg)->pid,
                    ((struct atfork_user_data_t *) arg)->pre,
                    ((struct atfork_user_data_t *) arg)->post_p,
                    ((struct atfork_user_data_t *) arg)->post_c));
}

/*
 * _ P O S T _ F O R K _ P A R E N T
 *
 * This procedure is called in the parent of a forked process immediately
 * after the fork is performed.
 */         


INTERNAL void _post_fork_parent
#ifdef _DCE_PROTO_
(
  cma_t_address arg
)
#else 
(arg)
cma_t_address arg;
#endif
{
    RPC_DBG_PRINTF(rpc_e_dbg_atfork, 1,
       ("(_post_fork_parent) entering, pid %d, pre %d, post_p %d, post_c %d\n",
                    ((struct atfork_user_data_t *) arg)->pid,
                    ((struct atfork_user_data_t *) arg)->pre,
                    ((struct atfork_user_data_t *) arg)->post_p,
                    ((struct atfork_user_data_t *) arg)->post_c));

    ((struct atfork_user_data_t *) arg)->post_p++;

    if (rpc_g_initialized == true)
        (*atfork_handler_ptr)(RPC_C_POSTFORK_PARENT);

    atfork_in_progress = false;

    RPC_DBG_PRINTF(rpc_e_dbg_atfork, 1,
      ("(_post_fork_parent) returning, pid %d, pre %d, post_p %d, post_c %d\n",
                    ((struct atfork_user_data_t *) arg)->pid,
                    ((struct atfork_user_data_t *) arg)->pre,
                    ((struct atfork_user_data_t *) arg)->post_p,
                    ((struct atfork_user_data_t *) arg)->post_c));
}


PRIVATE void rpc__cma_atfork
#ifdef _DCE_PROTO_
(
 void *handler
)
#else 
(handler)
void *handler;
#endif
{
    if (handler == NULL)
        return;

    /*
     * Don't register it again!
     */
    if (atfork_handler_ptr != NULL)
        return;

    /*
     * Save the address of the handler routine, and register our own
     * handlers. (see note above)
     */
    atfork_handler_ptr = (atfork_handler_ptr_t) handler;
    atfork_user_data.pid = (unsigned32) getpid();
    atfork_user_data.pre = 0;
    atfork_user_data.post_p = 0;
    atfork_user_data.post_c = 0;

    cma_atfork((cma_t_address)(&atfork_user_data), _pre_fork,
               _post_fork_parent, _post_fork_child);
}
#endif  /* defined(CMA_INCLUDE) */


/*
**++
**
**  ROUTINE NAME:       rpc__fork_is_in_progress
**
**  SCOPE:              PRIVATE - declared in com.h
**
**  DESCRIPTION:
**      
**  Return true iff the atfork handler is in progress.
**  
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     boolean
**
**  SIDE EFFECTS:       none
**
**--
**/

PRIVATE boolean32 rpc__fork_is_in_progress (void)
{
    /*
     * We could lock, but there should be only one thread in fork-progress
     * which can change it.
     */
    return (atfork_in_progress);
}

#endif  /* defined(ATFORK_SUPPORTED) */
