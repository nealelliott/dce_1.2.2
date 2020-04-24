/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cn.h,v $
 * Revision 1.1.12.1  1996/06/04  21:53:29  arvind
 * 	u2u: Pass binding handle torpc_cn_auth_cred_refresh_fn_t prototype
 * 	[1996/05/06  20:12 UTC  burati  /main/DCE_1.2/2]
 *
 * 	merge u2u work
 * 	[1996/04/29  20:02 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 *
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	Pass binding handle to rpc_cn_auth_cred_refresh_fn_t prototype
 * 	[1996/04/29  19:58 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 *
 * 	Submitted the local rpc security bypass.
 * 	[1995/03/06  17:18 UTC  tatsu_s  /main/HPDCE02/1]
 *
 * 	Local RPC Security Bypass.
 * 	[1995/02/22  22:31 UTC  tatsu_s  /main/tatsu_s.local_rpc.b1/1]
 *
 * Revision 1.1.10.2  1996/02/18  22:55:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:32  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  00:16:41  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:29 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/06  17:18 UTC  tatsu_s
 * 	Submitted the local rpc security bypass.
 * 
 * 	HP revision /main/tatsu_s.local_rpc.b1/1  1995/02/22  22:31 UTC  tatsu_s
 * 	Local RPC Security Bypass.
 * 	[1995/12/07  23:57:13  root]
 * 
 * Revision 1.1.8.4  1994/07/06  20:26:43  sommerfeld
 * 	[OT11176] more BIG PAC.
 * 	[1994/07/06  19:41:56  sommerfeld]
 * 
 * Revision 1.1.8.3  1994/03/17  23:06:28  tom
 * 	Big PAC:
 * 	  Add old_server argument to rpc_cn_auth_fmt_client_req_fn_t.
 * 	[1994/03/17  23:03:50  tom]
 * 
 * Revision 1.1.8.2  1994/01/21  22:33:42  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:53:24  cbrooks]
 * 
 * Revision 1.1.8.1  1994/01/14  20:30:58  tom
 * 	Big PAC (server side):
 * 	  Add rpc_cn_sec_auth_buffer_info_t structure.
 * 	  Add auth_buffer_info to rpc_cn_assoc_sec_context_t.
 * 	[1994/01/13  22:50:42  tom]
 * 
 * 	Big PAC - Add 2 arguments to rpc_cn_auth_fmt_client_req_fn_t.
 * 	  Add krb_message pointer to rpc_cn_assoc_sec_context_t.
 * 	  Add rpc_c_cn_dbg_auth_big_pac debug level.
 * 
 * Revision 1.1.5.4  1993/01/03  22:57:47  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:09  bbelch]
 * 
 * Revision 1.1.5.3  1992/12/23  20:12:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:29:52  zeliff]
 * 
 * Revision 1.1.5.2  1992/10/13  20:52:38  weisman
 * 	     15-sep-92    ko      Remove cred_valid entry point.
 * 	                          Add cred_changed entry point.
 * 	                          Add cred_refresh entry point.
 * 	[1992/10/13  20:45:34  weisman]
 * 
 * Revision 1.1.3.3  1992/05/22  18:56:59  wei_hu
 * 	24-mar-92    sp      Add fields for encrypted_crc for md5.
 * 	                     Remove krb-specific fields.
 * 	16-mar-92    sp      Change signature of recv_check.
 * 	11-mar-92    sp      ANSI changes.
 * 	 4-mar-92    sp      Fix prototype definition.
 * 	 3-mar-92    wh      DCE 1.0.1 merge.
 * 	27-feb-92    sp      Changes for credential timeout
 * 	18-feb-92    sp      Add rpc_cn_krb_key_t for key reorg.
 * 	[1992/05/22  18:41:50  wei_hu]
 * 
 * Revision 1.1  1992/01/19  03:09:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cn.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definitions of types/constants external to NCA Connection RPC
**  Protocol Service for use by other runtime components.
**
**
*/

#ifndef _CN_H
#define _CN_H	1

#include <cnpkt.h>
#include <dce/sec_authn.h>

/*
 * rpc_e_dbg_auth debug switch levels
 */


#define RPC_C_CN_DBG_AUTH_ROUTINE_TRACE 20
#define RPC_C_CN_DBG_AUTH_PKT           7
#define RPC_C_CN_DBG_AUTH_REFRESH       5
#define RPC_C_CN_DBG_AUTH_BIG_PAC       3
#define RPC_C_CN_DBG_AUTH_GENERAL       1


/*
 * rpc_e_dbg_cn_errors debug switch levels
 *
 * Switches to set to generate errors and a status code used only
 * when one of the error debug levels are set.
 */

#define RPC_S_CN_DBG_AUTH_FAILURE               0xdeadbeefU
#define RPC_C_CN_DBG_AUTH_CREATE_INFO           64      /* server */
#define RPC_C_CN_DBG_AUTH_CRED_CHANGED          65      /* client */
#define RPC_C_CN_DBG_AUTH_FMT_CLIENT_REQ        66      /* client */
#define RPC_C_CN_DBG_AUTH_FMT_SERVER_RESP       67      /* server */
#define RPC_C_CN_DBG_AUTH_GET_PROT_INFO         68      /* client & server */
#define RPC_C_CN_DBG_AUTH_PRE_CALL              69      /* client & server */
#define RPC_C_CN_DBG_AUTH_PRE_SEND              70      /* client & server */
#define RPC_C_CN_DBG_AUTH_RECV_CHECK            71      /* client & server */
#define RPC_C_CN_DBG_AUTH_VFY_CLIENT_REQ        72      /* server */
#define RPC_C_CN_DBG_AUTH_VFY_SERVER_RESP       73      /* client */
#define RPC_C_CN_DBG_AUTH_CONTEXT_VALID         74      /* client */
#define RPC_C_CN_DBG_AUTH_CRED_REFRESH          75      /* client */

/*
 * NOTE: rpc_c_cn_large_frag_size must always be at least
 * rpc_c_assoc_must_recv_frag_size as defined in cnassm.h. This is
 * an architectural requirement which is checked in cnfbuf.h.
 */

#define RPC_C_CN_LARGE_FRAG_SIZE        4096
#define RPC_C_CN_SMALL_FRAG_SIZE        256

/*
 * R P C _ C N _ A U T H _ I N F O _ T
 */

typedef struct rpc_cn_auth_info_s_t
{
    struct rpc_cn_auth_epv_s_t  *cn_epv;
} rpc_cn_auth_info_t, *rpc_cn_auth_info_p_t;

/*
 * R P C _ C N _ S E C _ C O N T E X T _ T
 */

typedef struct rpc_cn_sec_context_s_t
{
    rpc_list_t           link;         /* MUST BE 1ST                    */
    unsigned             sec_valid : 1;
    unsigned32           sec_status;
    unsigned32           sec_key_id;
    rpc_auth_info_t      *sec_info;
    rpc_cn_auth_info_t   *sec_cn_info;
} rpc_cn_sec_context_t, *rpc_cn_sec_context_p_t;

/*
 * R P C _ C N _ S E C _ A U T H _ B U F F E R _ I N F O _ T
 *
 * Used to piece together auth info from rpc_bind and alter_context PDU's
 */
typedef struct rpc_cn_sec_auth_buffer_info_s_t
{
    unsigned32           auth_buffer_len;   /* number of bytes       */
    unsigned32           auth_buffer_max;   /* actual memory used    */
    unsigned8            *auth_buffer;      /* reconstruction buffer */
} rpc_cn_sec_auth_buffer_info_t, rpc_cn_sec_auth_buffer_info_p_t;

/*
 * R P C _ C N _ A S S O C _ S E C _ C O N T E X T _ T
 */

typedef struct rpc_cn_assoc_sec_context_s_t
{
    unsigned                            assoc_have_uuid_crc : 1;    /* server only */
    unsigned32                          assoc_uuid_crc;
    unsigned32                          assoc_next_snd_seq;
    unsigned32                          assoc_next_rcv_seq;
    rpc_cn_sec_context_t                *assoc_current_sec_context; /* server only */
    sec_krb_message			krb_message;	/* for rpc__krb_cn_fmt_client_req */
    rpc_cn_sec_auth_buffer_info_t       auth_buffer_info;
    rpc_list_t                          context_list;   /* rpc_cn_sec_context_t */
} rpc_cn_assoc_sec_context_t, *rpc_cn_assoc_sec_context_p_t;

/*
 * The CN specific authentication protocol EPV.
 */
typedef boolean32 (*rpc_cn_auth_context_valid_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_sec_context_p_t         /*  sec */,
        unsigned32                   * /* st */
    ));

typedef void (*rpc_cn_auth_create_info_fn_t) _DCE_PROTOTYPE_((
       rpc_authn_level_t                /* authn_level */,
       rpc_auth_info_p_t                * /* auth_info */,
       unsigned32                       * /* st*/
    ));

typedef boolean32 (*rpc_cn_auth_cred_changed_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_sec_context_p_t         /*  sec */,
        unsigned32                     * /* st */
    ));

/*
 * This routine is not CN specific and could be moved into the
 * protocol-independent auth interface.
 */
typedef void (*rpc_cn_auth_cred_refresh_fn_t) _DCE_PROTOTYPE_((
        rpc_auth_info_p_t              /*  auth_info */,
        rpc_binding_handle_t           /*  h */,
        unsigned32                      * /* st */
    ));

typedef void (*rpc_cn_auth_fmt_client_req_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t      /* assoc_sec */,
        rpc_cn_sec_context_p_t            /* sec */,
        pointer_t                         /* auth_value */,
        unsigned32                      * /* auth_value_len */,
        pointer_t                       * /* last_auth_pos */,
        unsigned32                      * /* auth_len_remain */,
        unsigned32                        /* old_server */,
        unsigned32                      * /* st */
    ));

typedef void (*rpc_cn_auth_fmt_srvr_resp_fn_t) _DCE_PROTOTYPE_((
        unsigned32                     /*  verify_st */,
        rpc_cn_assoc_sec_context_p_t   /*  assoc_sec */,
        rpc_cn_sec_context_p_t         /*  sec */,
        pointer_t                      /*  req_auth_value */,
        unsigned32                     /*  req_auth_value_len */,
        pointer_t                      /*  auth_value */,
        unsigned32                      * /* auth_value_len */
    ));

typedef void (*rpc_cn_auth_free_prot_info_fn_t) _DCE_PROTOTYPE_((
        rpc_auth_info_p_t              /*  info */,
        rpc_cn_auth_info_p_t            * /* cn_info */
    ));

typedef void (*rpc_cn_auth_get_prot_info_fn_t) _DCE_PROTOTYPE_((
        rpc_auth_info_p_t              /*  info */,
        rpc_cn_auth_info_p_t           * /* cn_info */,
        unsigned32                      * /* st */
    ));

typedef void (*rpc_cn_auth_pre_call_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t   /*  assoc_sec */,
        rpc_cn_sec_context_p_t         /*  sec */,
        pointer_t                      /*  auth_value */,
        unsigned32                     * /*  auth_value_len */,
        unsigned32                      * /* st */
    ));

typedef void (*rpc_cn_auth_pre_send_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t    /* assoc_sec */,
        rpc_cn_sec_context_p_t          /* sec */,
        rpc_socket_iovec_p_t            /* iov */,
        unsigned32                      /* iovlen */,
        rpc_socket_iovec_p_t            /* out_iov */,
        unsigned32                      *st
    ));

typedef void (*rpc_cn_auth_recv_check_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t    /* assoc_sec */,
        rpc_cn_sec_context_p_t          /* sec */,
        rpc_cn_common_hdr_p_t           /* pdu */,
        unsigned32                      /* pdu_len */,
        unsigned32                      /* cred_len */,
        rpc_cn_auth_tlr_p_t             /* auth_tlr */,
        boolean32                       /* unpack_ints */,
        unsigned32                      * /* st */
    ));

typedef void (*rpc_cn_auth_tlr_uuid_crc_fn_t) _DCE_PROTOTYPE_((
        pointer_t               /* auth_value */,
        unsigned32              /* auth_value_len */,
        unsigned32              * /* uuid_crc */
    ));

typedef void (*rpc_cn_auth_tlr_unpack_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_packet_p_t       /* pkt_p */,
        unsigned32              /* auth_value_len */,
        unsigned8               * /* packed_drep */
    ));

typedef boolean32 (*rpc_cn_auth_three_way_fn_t) _DCE_PROTOTYPE_((void));

typedef void (*rpc_cn_auth_vfy_client_req_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t    /* assoc_sec */,
        rpc_cn_sec_context_p_t          /* sec */,
        pointer_t                       /* auth_value */,
        unsigned32                      /* auth_value_len */,
	unsigned32		        /* old_client */,
        unsigned32                      * /* st */
    ));

typedef void (*rpc_cn_auth_vfy_srvr_resp_fn_t) _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t    /* assoc_sec */,
        rpc_cn_sec_context_p_t          /* sec */,
        pointer_t                       /* auth_value */,
        unsigned32                      /* auth_value_len */,
        unsigned32                      * /* st */
    ));

typedef struct rpc_cn_auth_epv_s_t
{
    rpc_cn_auth_three_way_fn_t          three_way;
    rpc_cn_auth_context_valid_fn_t      context_valid;
    rpc_cn_auth_create_info_fn_t        create_info;
    rpc_cn_auth_cred_changed_fn_t       cred_changed;
    rpc_cn_auth_cred_refresh_fn_t       cred_refresh;
    rpc_cn_auth_fmt_client_req_fn_t     fmt_client_req;
    rpc_cn_auth_fmt_srvr_resp_fn_t      fmt_srvr_resp;
    rpc_cn_auth_free_prot_info_fn_t     free_prot_info;
    rpc_cn_auth_get_prot_info_fn_t      get_prot_info;
    rpc_cn_auth_pre_call_fn_t           pre_call;
    rpc_cn_auth_pre_send_fn_t           pre_send;
    rpc_cn_auth_recv_check_fn_t         recv_check;
    rpc_cn_auth_tlr_uuid_crc_fn_t       tlr_uuid_crc;
    rpc_cn_auth_tlr_unpack_fn_t         tlr_unpack;
    rpc_cn_auth_vfy_client_req_fn_t     vfy_client_req;
    rpc_cn_auth_vfy_srvr_resp_fn_t      vfy_srvr_resp;
} rpc_cn_auth_epv_t , *rpc_cn_auth_epv_p_t;

PRIVATE unsigned32      rpc__cn_crc_compute _DCE_PROTOTYPE_((
        unsigned8       * /* block */,
        unsigned32      /* block_len */
    ));
#endif /* _CN_H */
