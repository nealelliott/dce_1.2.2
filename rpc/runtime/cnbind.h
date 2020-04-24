/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnbind.h,v $
 * Revision 1.1.67.2  1996/02/18  22:55:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:37  marty]
 *
 * Revision 1.1.67.1  1995/12/08  00:16:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:21  root]
 * 
 * Revision 1.1.65.1  1994/01/21  22:33:53  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:35  cbrooks]
 * 
 * Revision 1.1.5.2  1993/08/26  18:05:50  tatsu_s
 * 	Bug 8104 - Added rpc__cn_binding_cross_fork().
 * 	[1993/08/24  16:06:42  tatsu_s]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:10  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:13:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:30:25  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:07:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNBIND_H
#define _CNBIND_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnbind.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Interface to the NCA Connection Protocol Service's Binding Service.
**
**
*/

/*
 * R P C _ _ C N _ B I N D I N G _ A L L O C
 */

PRIVATE rpc_binding_rep_t *rpc__cn_binding_alloc _DCE_PROTOTYPE_ ((
    boolean32            /* is_server */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ I N I T
 */

PRIVATE void rpc__cn_binding_init _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t  /* binding_r */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ R E S E T
 */

PRIVATE void rpc__cn_binding_reset _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t  /* binding_r */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ C H A N G E D
 */

PRIVATE void rpc__cn_binding_changed _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t  /* binding_r */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ F R E E
 */

PRIVATE void rpc__cn_binding_free _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t * /* binding_r */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ I N Q _ A D D R
 */

PRIVATE void rpc__cn_binding_inq_addr _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t  /* binding_r */,
    rpc_addr_p_t        * /* rpc_addr */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ I N Q _ C L I E N T
 */

PRIVATE void rpc__cn_binding_inq_client _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t  /* binding_r */,
    rpc_client_handle_t * /* client_h */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ C O P Y
 */

PRIVATE void rpc__cn_binding_copy _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t  /* src_binding_r */,
    rpc_binding_rep_p_t  /* dst_binding_r */,
    unsigned32          * /* st */));

/*
 * R P C _ _ C N _ B I N D I N G _ C R O S S _ F O R K
 */

PRIVATE void rpc__cn_binding_cross_fork _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t  /* binding_r */,
    unsigned32          * /* st */));

#endif /* _CNBIND_H */
