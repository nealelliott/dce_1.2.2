/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cncall.h,v $
 * Revision 1.1.35.2  1996/02/18  22:55:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:38  marty]
 *
 * Revision 1.1.35.1  1995/12/08  00:16:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:24  root]
 * 
 * Revision 1.1.33.1  1994/01/21  22:33:59  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:39  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:48  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:13:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:30:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:09:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNCALL_H
#define _CNCALL_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cncall.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**  Interface to the NCA Connection Protocol Service's Call Service.
**
**
*/

/*
 * R P C _ _ C N _ C A L L _ S T A R T
 */

PRIVATE rpc_call_rep_t *rpc__cn_call_start _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t          /* binding_rep */,
    unsigned32                   /* call_options */,
    rpc_if_rep_p_t               /* ifspec_r */,
    unsigned32                   /* opnum */,
    rpc_transfer_syntax_t       * /* xfer_syntax */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ T R A N S M I T
 */

PRIVATE void rpc__cn_call_transmit _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    rpc_iovector_p_t             /* call_args */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ T R A N S C E I V E
 */

PRIVATE void rpc__cn_call_transceive _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    rpc_iovector_p_t             /* in_call_args */,
    rpc_iovector_elt_t          * /* out_call_args */,
    ndr_format_t                * /* remote_ndr_format */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ R E C E I V E
 */

PRIVATE void rpc__cn_call_receive _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    rpc_iovector_elt_t          * /* call_args */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ B L O C K _ U N T I L _ F R E E
 */

PRIVATE void rpc__cn_call_block_until_free _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ A L E R T
 */

PRIVATE void rpc__cn_call_alert _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ E N D
 */

PRIVATE void rpc__cn_call_end _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t            * /* call_r */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ T R A N S M I T _ F A U L T
 */

PRIVATE void rpc__cn_call_transmit_fault _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    rpc_iovector_p_t             /* call_fault_info */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ R E J E C T
 */

PRIVATE void rpc__cn_call_reject _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    unsigned32                   /* l_st */));

/*
 * R P C _ _ C N _ C A L L _ R E C E I V E _ F A U L T
 */

PRIVATE void rpc__cn_call_receive_fault _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    rpc_iovector_elt_t          * /* call_fault_info */,
    ndr_format_t                * /* remote_ndr_format */,
    unsigned32                  * /* st */));

/*
 * R P C _ _ C N _ C A L L _ D I D _ M G R _ E X E C U T E 
 */

PRIVATE boolean32 rpc__cn_call_did_mgr_execute _DCE_PROTOTYPE_ ((
    rpc_call_rep_p_t             /* call_r */,
    unsigned32                  * /* st */));

/*
 * R P C _ C N _ C A L L _ N O _ C O N N _ I N D
 */

PRIVATE void rpc__cn_call_no_conn_ind _DCE_PROTOTYPE_ ((
    rpc_cn_call_rep_p_t          /* call_r */));

/*
 * R P C _ _ C N _ C A L L _ C C B _ C R E A T E
 */

PRIVATE void rpc__cn_call_ccb_create _DCE_PROTOTYPE_ ((
    rpc_cn_call_rep_p_t          /* call_r */));

/*
 * R P C _ _ C N _ C A L L _ C C B _ F R E E
 */

PRIVATE void rpc__cn_call_ccb_free _DCE_PROTOTYPE_ ((
    rpc_cn_call_rep_p_t          /* call_r */));

/*
 * R P C _ _ C N _ C A L L _ L O C A L _ C A N C E L
 */

PRIVATE void rpc__cn_call_local_cancel _DCE_PROTOTYPE_ ((
    rpc_cn_call_rep_p_t          /* call_rep */,
    volatile boolean32          * /* retry_op */,
    unsigned32                  * /* st */
    ));

/*
 * R P C _ _ C N _ C A L L _ S T A R T _ C A N C E L _ T I M E R
 */

PRIVATE void rpc__cn_call_start_cancel_timer _DCE_PROTOTYPE_ ((
    rpc_cn_call_rep_p_t      /* call_r */, 
    unsigned32              * /* st */
    ));

/*
 * R P C _ _ C N _ C A L L _ S T O P _ C A N C E L _ T I M E R
 */

PRIVATE void rpc__cn_call_stop_cancel_timer _DCE_PROTOTYPE_((
    rpc_cn_call_rep_p_t     /* call_r */
    ));

#endif /* _CNCALL_H */

