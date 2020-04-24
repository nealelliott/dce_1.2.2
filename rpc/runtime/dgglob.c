/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dgglob.c,v $
 * Revision 1.1.307.2  1996/02/18  00:03:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:56:12  marty]
 *
 * Revision 1.1.307.1  1995/12/08  00:19:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:00  root]
 * 
 * Revision 1.1.305.1  1994/01/21  22:36:54  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:57:00  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:24:11  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:05:11  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:48:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:36:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:08:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      dgglob.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Defining instances of DG-specific runtime global variables.
**
**
*/

#include <dg.h>

/* ======= */

    /*
     * The following variables are the "defining instance" and static
     * initialization of variables mentioned (and documented) in
     * "dgglob.h".
     */

GLOBAL rpc_dg_cct_t rpc_g_dg_cct;

GLOBAL rpc_dg_ccall_p_t rpc_g_dg_ccallt[RPC_DG_CCALLT_SIZE];

GLOBAL rpc_dg_sct_elt_p_t rpc_g_dg_sct[RPC_DG_SCT_SIZE];

GLOBAL rpc_dg_stats_t rpc_g_dg_stats = {0};

GLOBAL unsigned32 rpc_g_dg_server_boot_time;

GLOBAL uuid_t rpc_g_dg_my_cas_uuid;

GLOBAL rpc_dg_sock_pool_t rpc_g_dg_sock_pool;
/* ======= */

