/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsuuid.h,v $
 * Revision 1.1.338.2  1996/02/18  22:56:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:48  marty]
 *
 * Revision 1.1.338.1  1995/12/08  00:21:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:18  root]
 * 
 * Revision 1.1.336.1  1994/01/21  22:38:54  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:32  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:54:23  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:11:22  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:14:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:43:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:06:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NSUUID_H
#define _NSUUID_H	1
/*
**  Copyright (c) 1990 by
**  
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      nsuuid.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) name service (NS) independent interface
**  
**  ABSTRACT:
**
**      Header file containing prototypes of exported routines from the
**      nsuuid.c module.
**  
**
**/
#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

PRIVATE void rpc__nsuuid_mbr_add _DCE_PROTOTYPE_ (( 
        rpc_nsentry_p_t          /*nsentry*/,
        rpc_nsuuid_p_t           /*nsobj_uuid*/,
        unsigned32              *status 
    ));

PRIVATE void rpc__nsuuid_delete _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        unsigned32              *status 
    ));

PRIVATE rpc_nsuuid_t *rpc__nsuuid_from_uuid _DCE_PROTOTYPE_ ((
        uuid_p_t                 /*uuid*/,
        unsigned32              *status 
    ));

PRIVATE void rpc__nsuuid_free _DCE_PROTOTYPE_ (( 
        rpc_nsuuid_p_t          * /*nsuuid*/,
        unsigned32              *status 
    ));

PRIVATE void rpc__nsuuid_mbr_remove 
    _DCE_PROTOTYPE_ (( 
        rpc_nsentry_p_t          /*nsentry*/,
        rpc_nsuuid_p_t           /*nsobj_uuid*/,
        unsigned32              *status 
    ));

PRIVATE void rpc__nsuuid_to_uuid 
    _DCE_PROTOTYPE_ (( 
        rpc_nsuuid_p_t           /*nsuuid*/,
        uuid_t                  * /*uuid*/,
        unsigned32              *status 
    ));

#ifdef __cplusplus
}
#endif


#endif /* _NSUUID_H */
