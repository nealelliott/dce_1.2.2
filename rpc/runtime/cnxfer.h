/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cnxfer.h,v $
 * Revision 1.1.36.2  1996/02/18  22:55:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:48  marty]
 *
 * Revision 1.1.36.1  1995/12/08  00:17:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:54  root]
 * 
 * Revision 1.1.34.1  1994/01/21  22:34:55  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:54:50  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:59:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:00:57  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:18:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:32:36  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:08:08  devrcs
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
**      cnxfer.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definitions of entrypoints to support buffered data transfer
**  within the Connection-oriented protocol services component of
**  the RPC runtime.
**
**
*/

#ifndef _CNXFER_H
#define _CNXFER_H	1

/***********************************************************************/
/*
 * rpc_c_cn_bcopy_lim determines the maximum byte count which we
 * will copy instead of allocating an I/O vector element.  i.e., if
 * the stub data contains less than or equal to rpc_c_cn_bcopy_lim,
 * then the data will be copied to an internal buffer.
 */
#define RPC_C_CN_BCOPY_LIM 200

/***********************************************************************/
/*
 * R P C _ _ C N _ C O P Y _ B U F F E R
 *
 */

PRIVATE void rpc__cn_copy_buffer _DCE_PROTOTYPE_ ((
        rpc_cn_call_rep_p_t /* call_rep */, 
        rpc_iovector_elt_p_t /* iov_elt_p */,
        unsigned32     * /* status */
    ));

/***********************************************************************/
/*
 * R P C _ _ C N _ T R A N S M I T _ B U F F E R S
 *
 */

PRIVATE void rpc__cn_transmit_buffers _DCE_PROTOTYPE_ ((
        rpc_cn_call_rep_p_t /* call_rep */, 
        unsigned32     */* status */
    ));

#if 0
/***********************************************************************/
/*
 * R P C _ _ C N _ F L U S H _ B U F F E R S
 *
 */

PRIVATE void rpc__cn_flush_buffers _DCE_PROTOTYPE_ ((
        rpc_cn_call_rep_p_t /* call_rep */, 
        unsigned32     */* status */
    ));
#endif /* 0 */

/***********************************************************************/
/*
 * R P C _ _ C N _ A D D _ N E W _ I O V E C T O R _ E L M T
 *
 */

PRIVATE void rpc__cn_add_new_iovector_elmt _DCE_PROTOTYPE_ ((
        rpc_cn_call_rep_p_t /* call_rep */, 
        rpc_iovector_elt_p_t /* iov_elt_p */,
        unsigned32     */* status */
        ));


/***********************************************************************/
/*
 * R P C _ _ C N _ D E A L L O C _ B U F F E R E D _ D A T A
 *
 */

PRIVATE void rpc__cn_dealloc_buffered_data _DCE_PROTOTYPE_ ((
     rpc_cn_call_rep_p_t /*call_rep*/
    ));


/*
 * Macro to fix up the iovector in the call_rep so that we have
 * only the cached protocol header (and no stub data).
 */
#define RPC_CN_FREE_ALL_EXCEPT_PROT_HDR(call_rep) \
{\
    RPC_CN_CREP_IOVLEN (call_rep) = 1; \
    RPC_CN_CREP_CUR_IOV_INDX (call_rep) = 0; \
    RPC_CN_CREP_ACC_BYTCNT (call_rep) = RPC_CN_CREP_SIZEOF_HDR (call_rep); \
    RPC_CN_CREP_FREE_BYTES (call_rep) = \
        RPC_C_CN_SMALL_FRAG_SIZE - RPC_CN_CREP_SIZEOF_HDR (call_rep); \
    RPC_CN_CREP_FREE_BYTE_PTR (call_rep) = \
        (byte_p_t) RPC_CN_CREP_IOV(call_rep)[0].data_addr; \
    RPC_CN_CREP_FREE_BYTE_PTR (call_rep) += RPC_CN_CREP_SIZEOF_HDR (call_rep); \
    RPC_CN_CREP_IOV(call_rep)[0].data_len = RPC_CN_CREP_SIZEOF_HDR (call_rep);\
    if ((call_rep)->sec != NULL)\
    {\
        RPC_CN_CREP_FREE_BYTE_PTR (call_rep) -= call_rep->prot_tlr->data_size; \
        RPC_CN_CREP_IOVLEN (call_rep)++;\
    }\
}
#endif /* _CNXFER_H */
