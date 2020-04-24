/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ns.h,v $
 * Revision 1.1.317.2  1996/02/18  22:56:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:40  marty]
 *
 * Revision 1.1.317.1  1995/12/08  00:21:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:55  root]
 * 
 * Revision 1.1.315.2  1994/07/29  16:19:13  tom
 * 	Remove DNSPI_V2 ifdef.
 * 	[1994/07/28  22:33:42  tom]
 * 
 * Revision 1.1.315.1  1994/01/21  22:38:24  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:50  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:53:25  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:09:53  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:13:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:08:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NS_H
#define _NS_H	1
/*
**  Copyright (c) 1989 by
**  
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      ns.h
**
**  FACILITY:
**
**      DECrpc name service (NS) independent interface
**  
**  ABSTRACT:
**
**      Contains Private RPC Name Service function prototypes, 
**      typedefs, constant definitions, etc, that are made
**      available to other RPC components.
**  
**
**/

/*
 * Define OSF_DCE as TRUE (1) at runtime if
 * running under CDS. Otherwise, OSF_DCE is
 * FALSE (0)
 */
 
#define OSF_DCE 1

#endif /* _NS_H */
