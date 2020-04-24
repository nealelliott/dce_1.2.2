/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nsutil.h,v $
 * Revision 1.1.336.2  1996/02/18  22:56:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:47  marty]
 *
 * Revision 1.1.336.1  1995/12/08  00:21:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  00:00:16  root]
 * 
 * Revision 1.1.334.1  1994/01/21  22:38:52  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:59:30  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:54:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:11:15  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  21:14:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:42:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:11:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _NSUTIL_H
#define _NSUTIL_H
/*
**  Copyright (c) 1989 by
**  
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      nsutil.h
**
**  FACILITY:
**
**      DECrpc name service (NS) independent interface
**  
**  ABSTRACT:
**
**      Header file containing prototypes of exported routines 
**      from the nsutil.c module.
**  
**
**/

#include <nsp.h>

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

PRIVATE boolean rpc__ns_check_name_syntax _DCE_PROTOTYPE_   ((
        unsigned32              /*name_syntax*/,        
        unsigned32              * /*status*/
    ));

PRIVATE rpc_ns_inq_rep_t *rpc__inqcntx_create _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        rpc_inqcntx_usage_t      /*usage*/,
        unsigned32              * /*status*/
    ));

PRIVATE void rpc__inqcntx_free _DCE_PROTOTYPE_ ((
        rpc_ns_inq_rep_p_t      * /*inquiry_context*/,
        unsigned32              * /*status*/
    ));

PRIVATE void rpc__ns_status_from_nserror _DCE_PROTOTYPE_ ((
        struct dnsFlagStat      * /*dns_flags*/,
        unsigned32              * /*rpc_status*/
    ));

PRIVATE void rpc__ns_init_dns_flags _DCE_PROTOTYPE_ ((
        struct dnsFlagStat      * /*dns_flags*/,
        unsigned32               /*expiration_age*/
    ));

PRIVATE unsigned32 rpc__ns_count_obj_uuids_in_vec _DCE_PROTOTYPE_ ((
        uuid_vector_p_t          /*object_uuid_vec*/
    ));

PRIVATE boolean rpc__if_in_id_list _DCE_PROTOTYPE_ ((
        rpc_list_p_t             /*if_id_list*/,
        rpc_if_id_p_t            /*if_id*/,
        unsigned32              * /*status*/
    ));

PRIVATE void rpc__ns_check_rpc_class_version _DCE_PROTOTYPE_ ((
        rpc_nsentry_p_t          /*nsentry*/,
        unsigned32               /*exp_age*/,
        unsigned32              * /*rpc_status*/
    ));

#ifdef __cplusplus
}
#endif

#endif /* _NSUTIL_H */
