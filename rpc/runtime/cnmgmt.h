/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnmgmt.h,v $
 * Revision 1.1.37.2  1996/02/18  22:55:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:42  marty]
 *
 * Revision 1.1.37.1  1995/12/08  00:17:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:37  root]
 * 
 * Revision 1.1.35.1  1994/01/21  22:34:25  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:11  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:48  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:43  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:15:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:26  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNMGMT_H
#define _CNMGMT_H	1
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnmgmt.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Interface to the NCA Connection Protocol Service's Management Service.
**
**
*/


#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

/*
 * R P C _ _ C N _ M G M T _ I N I T
 */

PRIVATE void rpc__cn_mgmt_init _DCE_PROTOTYPE_ ((void));

/*
 * R P C _ _ C N _ M G M T _ I N Q _ C A L L S _ S E N T
 */

PRIVATE unsigned32 rpc__cn_mgmt_inq_calls_sent _DCE_PROTOTYPE_ ((void));

/*
 * R P C _ _ C N _ M G M T _ I N Q _ C A L L S _ R C V D 
 */

PRIVATE unsigned32 rpc__cn_mgmt_inq_calls_rcvd _DCE_PROTOTYPE_ ((void));

/*
 * R P C _ _ C N _ M G M T _ I N Q _ P K T S _ S E N T
 */

PRIVATE unsigned32 rpc__cn_mgmt_inq_pkts_sent _DCE_PROTOTYPE_ ((void));

/*
 * R P C _ _ C N _ M G M T _ I N Q _ P K T S _ R C V D
 */

PRIVATE unsigned32 rpc__cn_mgmt_inq_pkts_rcvd _DCE_PROTOTYPE_ ((void));

#endif /* _CNMGMT_H */
