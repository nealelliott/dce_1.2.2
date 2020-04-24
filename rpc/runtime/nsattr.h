/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsattr.h,v $
 * Revision 1.1.319.2  1996/02/18  22:56:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:41  marty]
 *
 * Revision 1.1.319.1  1995/12/08  00:21:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:58  root]
 * 
 * Revision 1.1.317.1  1994/01/21  22:38:27  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:53  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:53:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:10:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:13:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:11:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NSATTR_H
#define _NSATTR_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      nsattr.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) Name Service Component
**
**  ABSTRACT:
**
**      Contains Private routine signatures for export to other NS modules.
**
*/

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif


PRIVATE void rpc__nsattr_add_value _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        unsigned32               /*attr_type*/,
        unsigned_char_p_t        /*nsattr_value*/,
        unsigned32               /*nsattr_value_len*/,
        unsigned32              * /*status*/
    ));

PRIVATE void rpc__nsattr_delete _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        unsigned32               /*attr_type*/,
        unsigned32              * /*status*/
    ));

PRIVATE boolean rpc__nsattr_exists _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        unsigned32               /*attr_type*/,
        unsigned32               /*exp_age*/,
        unsigned32              * /*status*/
    ));

PRIVATE void rpc__nsattr_read_resolution _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t      /*nsentry*/,
        unsigned32           /*attr_type*/,
        unsigned_char_t     * /*resolved_name*/,
        unsigned32          * /*resolved_name_length*/,
        unsigned32          * /*status*/
    ));

PRIVATE void rpc__nsattr_read_value _DCE_PROTOTYPE_ ((
        rpc_ns_inq_rep_p_t       /*inquiry_context*/,
        unsigned_char_p_t       * /*nsattr_value*/,
        unsigned32              * /*nsattr_value_len*/,
        unsigned32              * /*status*/
    ));

PRIVATE void rpc__nsattr_remove_value _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        unsigned32               /*attr_type*/,
        unsigned_char_p_t        /*nsattr_value*/,
        unsigned32               /*nsattr_value_len*/,
        unsigned32              * /*status*/
    ));

#ifdef __cplusplus
}
#endif

#endif /* _NSATTR_H */
