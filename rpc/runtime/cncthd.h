/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cncthd.h,v $
 * Revision 1.1.40.2  1996/02/18  22:55:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:40  marty]
 *
 * Revision 1.1.40.1  1995/12/08  00:17:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:31  root]
 * 
 * Revision 1.1.38.1  1994/01/21  22:34:11  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:48  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:14:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:03  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNCTHD_H
#define _CNCTHD_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cncthd.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definitions of types/constants for the NCA Connection Protocol
**  Service's Call Executor Service.
**
**
*/


/***********************************************************************/
/*
 * R P C _ _ C N _ C A L L _ E X E C U T O R
 */

PRIVATE void rpc__cn_call_executor _DCE_PROTOTYPE_ ((
        pointer_t       /* call_r */,
        boolean32       /* call_was_queued */
    ));
#endif /* _CNCTHD_H */
