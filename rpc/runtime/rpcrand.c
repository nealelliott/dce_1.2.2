/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpcrand.c,v $
 * Revision 1.1.351.2  1996/02/18  00:05:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:24  marty]
 *
 * Revision 1.1.351.1  1995/12/08  00:22:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:30  root]
 * 
 * Revision 1.1.349.1  1994/01/21  22:39:16  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  22:00:01  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:55:03  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:12:33  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:16:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:44:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:06:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      rpcrand.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  The support routines for the rpcrand.h abstraction.  These should NOT 
**  be called directly; use the macros defined in rpcrand.h .
**
**
*/

#include <commonp.h>

/* 
 * R P C _ _ R A N D O M _ I N I T
 */

PRIVATE void rpc__random_init
#ifdef _DCE_PROTO_
(
    unsigned32 seed
)
#else
(seed)
unsigned32 seed;
#endif
{
    srandom ((int) seed);
}

/* 
 * R P C _ _ R A N D O M _ G E T
 */

PRIVATE unsigned32 rpc__random_get
#ifdef _DCE_PROTO_
(
    unsigned32 lower,
    unsigned32 upper
)
#else
(lower, upper)
unsigned32 lower;
unsigned32 upper;
#endif
{
    return (random ());
}
