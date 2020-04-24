/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnnet.h,v $
 * Revision 1.1.39.2  1996/02/18  22:55:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:43  marty]
 *
 * Revision 1.1.39.1  1995/12/08  00:17:23  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:39  root]
 * 
 * Revision 1.1.37.2  1994/08/12  16:22:28  tom
 * 	Add rpc__cn_{set,inq}_sock_buffsize prototypes (OT 11440).
 * 	[1994/08/11  20:20:26  tom]
 * 
 * Revision 1.1.37.1  1994/01/21  22:34:30  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:14  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:58:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:59:51  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:16:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:31:35  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:09:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CNNET_H
#define _CNNET_H	1
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cnnet.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Interface to the NCA Connection Protocol Service's Network Service.
**
**
*/

#ifndef _DCE_PROTOTYPE_
#include <dce/dce.h>
#endif

/*
 * C O N N E C T I O N   S T A T E S
 */
#define RPC_C_CN_CLOSED         0
#define RPC_C_CN_CONNECTING     1  
#define RPC_C_CN_OPEN           2

/*
 * R P C _ C N _ N E T W O R K _ I O V E C T O R _ T O _ I O V
 */

#define RPC_CN_NETWORK_IOVECTOR_TO_IOV(iovector, iov, iovcnt, bytes_to_send)\
{\
    unsigned8   _num_elts;\
\
    for (_num_elts = 0, (bytes_to_send) = 0; _num_elts < (iovector)->num_elt; _num_elts++) \
    { \
        (iov)[_num_elts].base = (byte_p_t) ((iovector)->elt[_num_elts].data_addr); \
        (iov)[_num_elts].len = (int) ((iovector)->elt[_num_elts].data_len); \
        (bytes_to_send) += (iov)[_num_elts].len; \
    } \
    (iovcnt) = _num_elts; \
}

/*
 * R P C _ C N _ N E T W O R K _ I O V _ A D J U S T
 */

#define RPC_CN_NETWORK_IOV_ADJUST(iovp, iovcnt, cc)\
{\
    unsigned8   _num_elts;\
    signed32    _bytes_to_adjust;\
\
    for (_bytes_to_adjust = (cc), _num_elts = 0;; _num_elts++, iovp++) \
    { \
        if ((iovp)->len > _bytes_to_adjust) \
        { \
            (iovp)->len -= _bytes_to_adjust; \
            (iovp)->base += _bytes_to_adjust; \
            break; \
        } \
        _bytes_to_adjust -= (iovp)->len; \
    } \
    (iovcnt) -= _num_elts; \
}

/*
 * R P C _ _ C N _ N E T W O R K _ U S E _ P R O T S E Q 
 */

PRIVATE void rpc__cn_network_use_protseq _DCE_PROTOTYPE_ ((
    rpc_protseq_id_t            /* pseq_id */, 
    unsigned32                  /* max_calls */,
    rpc_addr_p_t                /* rpc_addr */,
    unsigned_char_p_t           /* endpoint */,
    unsigned32                  */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ M O N 
 */

PRIVATE void rpc__cn_network_mon _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t     /* binding_r */,
    rpc_client_handle_t     /* client_h */,
    rpc_network_rundown_fn_t /* rundown */,
    unsigned32              */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ S T O P _ M O N 
 */

PRIVATE void rpc__cn_network_stop_mon _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t     /* binding_r */,
    rpc_client_handle_t     /* client_h */,
    unsigned32              */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ M A I N T
 */

PRIVATE void rpc__cn_network_maint _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t     /* binding_r */,
    unsigned32              */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ S T O P _ M A I N T
 */

PRIVATE void rpc__cn_network_stop_maint _DCE_PROTOTYPE_ ((
    rpc_binding_rep_p_t     /* binding_r */,
    unsigned32              */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ S E L E C T _ D I S P A T C H 
 */

PRIVATE void rpc__cn_network_select_dispatch _DCE_PROTOTYPE_ ((
    rpc_socket_t            /* desc */,
    pointer_t               /* priv_info */,
    boolean32               /* is_active */,
    unsigned32              */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ I N Q _ P R O T _ V E R S
 */

PRIVATE void rpc__cn_network_inq_prot_vers _DCE_PROTOTYPE_ ((
    unsigned8               */* prot_id */,
    unsigned32              */* version_major */,
    unsigned32              */* version_minor */,
    unsigned32              */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ R E Q _ C O N N E C T
 */

PRIVATE void rpc__cn_network_req_connect _DCE_PROTOTYPE_ ((
    rpc_addr_p_t            /* rpc_addr */,
    rpc_cn_assoc_p_t        /* assoc */,
    unsigned32              */* st */));

/*
 * R P C _ _ C N _ N E T W O R K _ C L O S E _ C O N N E C T
 */

PRIVATE void rpc__cn_network_close_connect _DCE_PROTOTYPE_ ((
    rpc_cn_assoc_p_t        /* assoc */,
    unsigned32              */* st */));


/*
 * R P C _ _ C N _ N E T W O R K _ C O N N E C T _ F A I L
 */

PRIVATE boolean32 rpc__cn_network_connect_fail _DCE_PROTOTYPE_ ((unsigned32));


/*
 * These two are internal API's so you can tweak the TCP buffering.
 */

/*
 * R P C _ _ C N _ S E T _ S O C K _ B U F F S I Z E
 */
PRIVATE void rpc__cn_set_sock_buffsize _DCE_PROTOTYPE_ ((
        unsigned32	  /* rsize */,
        unsigned32	  /* ssize */,
        error_status_t	* /* st */));

/*
 * R P C _ _ C N _ I N Q _ S O C K _ B U F F S I Z E
 */
PRIVATE void rpc__cn_inq_sock_buffsize _DCE_PROTOTYPE_ ((
        unsigned32	* /* rsize */,
        unsigned32	* /* ssize */,
        error_status_t  * /* st */));


#endif /* _CNNET_H */





