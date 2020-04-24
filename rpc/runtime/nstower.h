/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nstower.h,v $
 * Revision 1.1.334.2  1996/02/18  22:56:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:46  marty]
 *
 * Revision 1.1.334.1  1995/12/08  00:21:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:14  root]
 * 
 * Revision 1.1.332.1  1994/01/21  22:38:49  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:27  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:54:13  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:11:06  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:14:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:07:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NSTOWER_H
#define _NSTOWER_H	1
/*
**  Copyright (c) 1990 by
**  
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      nstower.h
**
**  FACILITY:
**
**      DECrpc name service (NS) independent interface
**  
**  ABSTRACT:
**
**      Header file containing macros, definitions, typedefs and prototypes 
**      of exported routines from the nstower.c module.
**  
**
**/
#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


PRIVATE void rpc__tower_delete _DCE_PROTOTYPE_ (( 
    rpc_nsentry_p_t          /*nsentry*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_mbr_add _DCE_PROTOTYPE_ (( 
    rpc_nsentry_p_t          /*nsentry*/,
    twr_p_t                  /*tower*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_mbr_inq_begin _DCE_PROTOTYPE_ (( 
    rpc_nsentry_p_t          /*nsentry*/,
    rpc_ns_handle_t         * /*inquiry_context*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_mbr_inq_done _DCE_PROTOTYPE_ ((
    rpc_ns_handle_t         * /*inquiry_context*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_mbr_inq_next _DCE_PROTOTYPE_ ((
    rpc_ns_handle_t          /*inquiry_context*/,
    twr_p_t                 * /*tower*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_mbr_remove _DCE_PROTOTYPE_ (( 
    rpc_nsentry_p_t          /*nsentry*/,
    twr_p_t                  /*tower*/,
    unsigned32              * /*status*/ 
));

#ifdef __cplusplus
}
#endif

#endif /* _NSTOWER_H */
