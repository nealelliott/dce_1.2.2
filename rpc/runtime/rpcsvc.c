/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rpcsvc.c,v $
 * Revision 1.1.6.1  1996/05/10  13:12:30  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/04/18  19:14 UTC  bissen
 * 	unifdef for single threaded client
 * 	[1996/02/29  20:42 UTC  bissen  /main/HPDCE02/bissen_st_rpc/1]
 *
 * 	Faster svc.
 * 	[1995/09/28  03:29 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:43 UTC  psn
 * 	Remove Mothra specific code
 * 	[1995/11/16  21:35 UTC  jrr  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Faster svc.
 * 	[1995/09/28  03:29 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 *
 * 	Submitted rfc31.0: Single-threaded DG client and RPC_PREFERRED_PROTSEQ.
 * 	[1994/12/09  19:19 UTC  tatsu_s  /main/HPDCE02/1]
 *
 * 	rfc31.0: Initial version.
 * 	[1994/11/30  22:25 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/1]
 *
 * Revision 1.1.2.3  1994/08/02  21:38:59  tom
 * 	Bug 11531
 * 	Don't need time and thread info since svc gives it to us already.
 * 	[1994/08/02  21:38:50  tom]
 * 
 * Revision 1.1.2.2  1994/06/09  19:03:39  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:35:18  annie]
 * 
 * Revision 1.1.2.1  1994/05/19  21:15:04  hopkins
 * 	More serviceability.
 * 	[1994/05/18  21:25:33  hopkins]
 * 
 * 	Serviceability work.
 * 
 * $EndLog$
 */

#include <ctype.h>
#include <commonp.h>
#include <string.h>
#include <rpcsvc.h>
#include <stdarg.h>

/*
dce_svc_handle_t rpc_g_svc_handle;
*/
DCE_SVC_DEFINE_HANDLE(rpc_g_svc_handle, rpc_g_svc_table, "rpc")
      

/*
 * R P C _ _ S V C _ E P R I N T F
 *
 * Format and print arguments as a serviceability
 * debug message.
 */

PRIVATE int rpc__svc_eprintf ( char *fmt, ... )
{
    char	buf[RPC__SVC_DBG_MSG_SZ];
    va_list	arg_ptr;

    va_start (arg_ptr, fmt);
    vsprintf (buf, fmt, arg_ptr);
    va_end (arg_ptr);
    DCE_SVC_DEBUG((RPC__SVC_HANDLE, rpc_svc_general, RPC__SVC_DBG_LEVEL(0), buf));
    return(0);
}


/*
 * R P C _ _ S V C _ I N I T
 *
 * Do initialization required for serviceability
 */

PRIVATE void rpc__svc_init ( void )
{
    error_status_t status;

    /*
     * Currently, all we have to do is return, since
     * everything is statically registered.
     * 
     * But someday we might do something like turn
     * on debug levels corresponding to things set
     * in rpc_g_dbg_switches[], or ...
     */

    /*
     * This silliness is a placeholder, so that we
     * remember to do things differently in the kernel
     * if we ever decide to do more than just return
     * out of this routine.
     */
#if !defined(KERNEL) && !defined(_KERNEL)
    if (!RPC__SVC_HANDLE->setup)
	(void)dce_svc__debug_fillin2(RPC__SVC_HANDLE, 0);
    assert(RPC__SVC_HANDLE->setup != FALSE);
    return;
#else
    return;
#endif
}

#ifdef DEBUG
/*
 * R P C _ _ S V C _ F M T _ D B G _ M S G
 *
 * This routine takes the printf "pargs" passed to
 * the RPC_DBG_PRINTF() macro and formats them
 * into a string that can be handed to DCE_SVC_DEBUG.
 *
 * This is necessary because the pargs are passed
 * in as a single, parenthesized argument -- which
 * also requires that the resulting string be passed
 * back as a pointer return value.
 *
 * The returned pointer must be free()'ed by the
 * caller (see comments at malloc() below).  This
 * should be fairly safe, since this routine should
 * only ever be called by RPC_DBG_PRINTF.
 */

PRIVATE char * rpc__svc_fmt_dbg_msg (char *format, ...)
{
    char            *bptr;
    char            *s;
    va_list         arg_ptr;

    /*
     * Using malloc here is ugly but necessary.  The formatted
     * string must be passed back as a pointer return value.  The
     * possibility of recursive calls due to evaluation of pargs
     * (where, e.g., one of the pargs is a call to a routine that
     * calls RPC_DBG_PRINTF) preclude an implementation using a
     * mutex to protect a static buffer.  The potential for infinite
     * recursion precludes allocating memory using internal RPC
     * interfaces, since those interfaces call RPC_DBG_PRINTF.
     */

    if( (bptr = malloc(RPC__SVC_DBG_MSG_SZ*sizeof(char))) == NULL )
    {
	/* die horribly */
	abort();
    }

    s = bptr;

    if (RPC_DBG2(rpc_e_dbg_timer, 1))
    {
	sprintf (s, "[time: %06lu] ", rpc__clock_stamp());
	s = bptr + strlen(bptr);
    }

    va_start (arg_ptr, format);
    vsprintf (s, format, arg_ptr);
    va_end (arg_ptr);

    return( bptr );
}

void rpc__svc_emit_dbg_msg (
    int swlevel,
    char *message
) 
{
    int sw = swlevel >> 4;
    int level = swlevel & 0xf;

    dce_svc__debug (RPC__SVC_HANDLE, sw, level,	"%s", message);
    free (message);
}
#endif	/* DEBUG */


