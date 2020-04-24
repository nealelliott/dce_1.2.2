/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsentry.h,v $
 * Revision 1.1.323.2  1996/02/18  22:56:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:42  marty]
 *
 * Revision 1.1.323.1  1995/12/08  00:21:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:02  root]
 * 
 * Revision 1.1.321.2  1994/07/29  16:19:19  tom
 * 	Add entry type to rpc__nsentry_create arguments. (OT 7945)
 * 	[1994/07/28  22:34:04  tom]
 * 
 * Revision 1.1.321.1  1994/01/21  22:38:33  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:58  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:53:41  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:19  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:13:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:08:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NSENTRY_H
#define _NSENTRY_H	1
/*
**  Copyright (c) 1989 by
**  
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      nsentry.h
**
**  FACILITY:
**
**      DECrpc name service (NS) independent interface
**  
**  ABSTRACT:
**
**      Header file containing prototypes of exported routines 
**      from the nsentry.c module.
**  
**
**/

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

PRIVATE void rpc__ns_expand_entry_name _DCE_PROTOTYPE_((
        unsigned_char_p_t    /*entry_name*/,
        unsigned_char_t     * /*expanded_name*/,
        unsigned32          * /*expanded_length*/,
        unsigned32          * /*status*/
    ));

PRIVATE void rpc__nsentry_create _DCE_PROTOTYPE_((
        rpc_nsentry_p_t      /*nsentry*/,
        unsigned_char_t     * /*attr_value*/,
        unsigned32          * /*status*/
    ));

PRIVATE rpc_nsentry_p_t rpc__nsentry_copy _DCE_PROTOTYPE_((
        rpc_nsentry_p_t      /*nsentry*/,
        unsigned32          * /*status*/
    ));

PRIVATE boolean rpc__nsentry_exists _DCE_PROTOTYPE_((
        rpc_nsentry_p_t      /*nsentry*/,
        unsigned32          * /*status*/
    ));

PRIVATE void rpc__nsentry_free _DCE_PROTOTYPE_((
        rpc_nsentry_p_t     * /*nsentry*/,
        unsigned32          *status 
    ));

PRIVATE rpc_nsentry_t *rpc__nsentry_from_entry _DCE_PROTOTYPE_((
        unsigned32           /*name_syntax*/,
        unsigned_char_p_t    /*entry_name*/,
        unsigned32          * /*status*/
    ));

PRIVATE void rpc__nsentry_make_it_rpc _DCE_PROTOTYPE_((
        rpc_nsentry_p_t      /*nsentry*/,
        unsigned32          * /*status*/
    ));

PRIVATE void rpc__nsentry_to_entry _DCE_PROTOTYPE_((
        rpc_nsentry_p_t      /*nsentry*/,
        unsigned32           /*entry_name_syntax*/,
        unsigned_char_t     * /*entry_name*/,
        unsigned32          * /*entry_name_length*/,
        unsigned32          * /*status */
    ));

#ifdef __cplusplus
}
#endif


#endif /* _NSENTRY_H */
