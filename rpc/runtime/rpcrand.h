/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpcrand.h,v $
 * Revision 1.1.352.2  1996/02/18  22:56:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:57  marty]
 *
 * Revision 1.1.352.1  1995/12/08  00:22:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/06  17:20 UTC  tatsu_s
 * 	Submitted the local rpc security bypass.
 * 
 * 	HP revision /main/tatsu_s.local_rpc.b1/1  1995/02/17  16:35 UTC  tatsu_s
 * 	Fixed RPC_RANDOM_GET().
 * 	[1995/12/08  00:00:31  root]
 * 
 * Revision 1.1.350.1  1994/01/21  22:39:17  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  22:00:02  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:55:05  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:12:37  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:16:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:44:18  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:06:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _RPCRAND_H
#define _RPCRAND_H
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      rpcrand.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
** 
**  Random number generator abstraction to isolate random number generation
**  routines and allow alternate implementations to be substituted more
**  easily.
**
**  This package provides the following PRIVATE operations:
**  
**      void       RPC_RANDOM_INIT(seed)
**      unsigned32 RPC_RANDOM_GET(lower, upper)
**
**
*/   


/* 
 * R P C _ R A N D O M _ I N I T
 *
 * Used for random number 'seed' routines or any other one time
 * initialization required.
 */

#define RPC_RANDOM_INIT(seed) \
        rpc__random_init(seed)

/* 
 * R P C _ R A N D O M _ G E T
 *
 * Get a random number in the range lower - upper (inclusive)
 */ 

#define RPC_RANDOM_GET(lower, upper) \
        (((rpc__random_get(lower, upper)) % ((upper) - (lower) + 1)) + (lower)) 

/*
 * Prototype for the private 'c' routines used by the RPC_RANDOM macros.
 */

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif


PRIVATE void rpc__random_init _DCE_PROTOTYPE_ (( unsigned32  /*seed*/ ));

PRIVATE unsigned32 rpc__random_get _DCE_PROTOTYPE_ ((
        unsigned32  /*lower*/,
        unsigned32  /*upper*/
    ));

#endif /* _RPCRAND_H */
        
