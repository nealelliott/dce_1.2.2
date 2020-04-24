/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsinit.h,v $
 * Revision 1.1.326.2  1996/02/18  22:56:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:43  marty]
 *
 * Revision 1.1.326.1  1995/12/08  00:21:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:05  root]
 * 
 * Revision 1.1.324.1  1994/01/21  22:38:36  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:01  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:53:49  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:30  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:13:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:07:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NSINIT_H
#define _NSINIT_H	1
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      nsinit.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Interface to the NS Initialization Service.
**
**
*/


#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif


/***********************************************************************/
/*
 * R P C _ _ N S _ I N I T
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

PRIVATE void rpc__ns_init _DCE_PROTOTYPE_ ((void));

#ifdef __cplusplus
}
#endif

#endif /* _NSINIT_H */


