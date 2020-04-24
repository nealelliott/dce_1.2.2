/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: comtwrflr.h,v $
 * Revision 1.1.313.2  1996/02/18  22:55:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:07  marty]
 *
 * Revision 1.1.313.1  1995/12/08  00:18:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:58:28  root]
 * 
 * Revision 1.1.311.2  1994/03/17  23:06:44  tom
 * 	Big PAC/protocll version support:
 * 	  Add protocol version argument to rpc__tower_flr_from_rpc_prot_id().
 * 	[1994/03/17  23:04:34  tom]
 * 
 * Revision 1.1.311.1  1994/01/21  22:36:04  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:56:12  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  23:23:04  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:03:23  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:45:46  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:35:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:07:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _COMTWRFLR_H
#define _COMTWRFLR_H 1

/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      comtwrflr.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**      Contains private definitions and prototypes of the 
**      comtwrflr.c module.
**
**
*/

/*
 * Constants
 */

/*
 * The architecturally defined tower floor protocol identifier 
 * prefix to signify the succeeding data as an RPC uuid.
 */
#define RPC_C_PROT_ID_PREFIX    (0x0D)

/*
 * Prototypes
 */

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

PRIVATE void rpc__tower_flr_free _DCE_PROTOTYPE_ ((
    rpc_tower_floor_p_t     * /*floor*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_from_drep _DCE_PROTOTYPE_ ((
    rpc_syntax_id_p_t        /*transfer_syntax*/,
    rpc_tower_floor_p_t     * /*floor*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_from_if_id _DCE_PROTOTYPE_ ((
    rpc_if_id_p_t            /*if_id*/,
    rpc_tower_floor_p_t     * /*floor*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_from_rpc_prot_id _DCE_PROTOTYPE_ ((
    rpc_protseq_id_t         /*rpc_protseq_id*/,
    rpc_protocol_version_p_t /*protocol_version*/,
    rpc_tower_floor_p_t     * /*floor*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_from_uuid _DCE_PROTOTYPE_ ((
    uuid_p_t                 /*uuid*/,
    unsigned32               /*version_major*/,
    unsigned32               /*version_minor*/,
    rpc_tower_floor_p_t     * /*floor*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_id_from_uuid _DCE_PROTOTYPE_ ((
    uuid_p_t         /*uuid*/,
    unsigned32       /*version_major*/,
    unsigned32      * /*prot_id_len*/,
    unsigned8       ** /*prot_id*/,
    unsigned32      * /*status*/ 
));

PRIVATE void rpc__tower_flr_id_to_uuid _DCE_PROTOTYPE_ ((
    unsigned8       * /*prot_id*/,
    uuid_t          * /*uuid*/,
    unsigned32      * /*version_major*/,
    unsigned32      * /*status*/ 
));

PRIVATE void rpc__tower_flr_to_drep _DCE_PROTOTYPE_ ((
    rpc_tower_floor_p_t      /*floor*/,
    rpc_syntax_id_t         * /*transfer_syntax*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_to_if_id _DCE_PROTOTYPE_ ((
    rpc_tower_floor_p_t      /*floor*/,
    rpc_if_id_t             * /*if_id*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_to_rpc_prot_id _DCE_PROTOTYPE_ ((
    rpc_tower_floor_p_t      /*floor*/,
    rpc_protocol_id_t       * /*rpc_protocol_id*/,
    unsigned32              * /*version_major*/,
    unsigned32              * /*version_minor*/,
    unsigned32              * /*status*/ 
));

PRIVATE void rpc__tower_flr_to_uuid _DCE_PROTOTYPE_ ((
    rpc_tower_floor_p_t      /*floor*/,
    uuid_t                  * /*uuid*/,
    unsigned32              * /*version_major*/,
    unsigned32              * /*version_minor*/,
    unsigned32              * /*status*/ 
));


#endif /* _COMTWRFLR_H */
