/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnrcvr.h,v $
 * Revision 1.1.41.2  1996/02/18  22:55:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:47  marty]
 *
 * Revision 1.1.41.1  1995/12/08  00:17:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:46  root]
 * 
 * Revision 1.1.39.1  1994/01/21  22:34:41  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:23  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:59:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:00:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:17:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:32:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:06:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNRCV_H
#define _CNRCV_H	1

/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnrcvr.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Connection-based protocol network receiver thread.
**
**
*/

PRIVATE void rpc__cn_network_receiver    _DCE_PROTOTYPE_ ((rpc_cn_assoc_p_t));

#endif /* _CNRCVR_H */
