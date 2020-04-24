/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnp.c,v $
 * Revision 1.1.40.2  1996/02/18  00:01:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:54:41  marty]
 *
 * Revision 1.1.40.1  1995/12/08  00:17:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:40  root]
 * 
 * Revision 1.1.38.1  1994/01/21  22:34:32  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:15  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:55  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:16:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:38  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:08:07  devrcs
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
**  NAME
**
**      cnp.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Definitions of global variables.
**
**
*/

#include <commonp.h>    /* Common declarations for all RPC runtime */
#include <com.h>        /* Common communications services */
#include <comprot.h>    /* Common protocol services */
#include <cnp.h>        /* NCA Connection private declarations */

GLOBAL rpc_cond_t               rpc_g_cn_lookaside_cond;
GLOBAL rpc_list_desc_t          rpc_g_cn_syntax_lookaside_list;
GLOBAL rpc_list_desc_t          rpc_g_cn_sec_lookaside_list;
GLOBAL rpc_list_desc_t          rpc_g_cn_assoc_lookaside_list;
GLOBAL rpc_list_desc_t          rpc_g_cn_binding_lookaside_list;
GLOBAL rpc_list_desc_t          rpc_g_cn_lg_fbuf_lookaside_list;
GLOBAL rpc_list_desc_t          rpc_g_cn_sm_fbuf_lookaside_list;
GLOBAL rpc_list_desc_t          rpc_g_cn_call_lookaside_list;
GLOBAL rpc_cn_assoc_grp_tbl_t   rpc_g_cn_assoc_grp_tbl;
GLOBAL unsigned32               rpc_g_cn_call_id;
GLOBAL rpc_cn_mgmt_t            rpc_g_cn_mgmt;
