/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: noauthcn.c,v $
 * Revision 1.1.515.1  1996/06/04  21:56:25  arvind
 * 	u2u: Pass  binding handle to rpc_cn_auth_cred_refresh_fn_t proto.
 * 	[1996/05/06  20:26 UTC  burati  /main/DCE_1.2/2]
 *
 * 	merge u2u work
 * 	[1996/04/29  21:03 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 *
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	Pass binding handle to rpc_cn_auth_cred_refresh_fn_t prototype\
 * 	[1996/04/29  20:26 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 *
 * 	Remove Mothra specific code
 * 	[1995/11/16  21:34 UTC  jrr  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Merge allocation failure detection functionality into Mothra
 * 	[1995/04/02  01:16 UTC  lmm  /main/HPDCE02/1]
 *
 * 	add memory allocation failure detection
 * 	[1995/04/02  00:20 UTC  lmm  /main/lmm_rpc_alloc_fail_detect/1]
 *
 * Revision 1.1.513.2  1996/02/18  00:04:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:56:55  marty]
 * 
 * Revision 1.1.513.1  1995/12/08  00:21:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/07  23:59:51  root]
 * 
 * Revision 1.1.511.2  1994/05/19  21:14:57  hopkins
 * 	Merge with DCE1_1.
 * 	[1994/05/04  19:43:22  hopkins]
 * 
 * 	Serviceability work.
 * 	[1994/05/03  20:28:36  hopkins]
 * 
 * Revision 1.1.511.1  1994/01/21  22:38:18  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:45  cbrooks]
 * 
 * Revision 1.1.4.4  1993/01/03  23:53:16  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:09:38  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/23  21:12:44  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:23  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/18  19:57:32  wei_hu
 * 	           19-oct-92   ko      Remove rpc__noauth_cn_get_tkt
 * 	                               Remove rpc__noauth_cn_cred_valid
 * 	                               Add rpc__noauth_cn_cred_changed
 * 	                               Add rpc__noauth_cn_cred_refresh
 * 	[1992/12/18  19:47:08  wei_hu]
 * 
 * Revision 1.1.2.2  1992/05/01  13:38:28  rsalz
 * 	 17-mar-1992 sp      Initial Version
 * 	[1992/04/30  23:20:20  rsalz]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME
**
**      noauthcn.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  The noauth CN authentication module.
**
**
*/

#include <noauth.h>

#ifndef _KERNEL
#include <pwd.h>                /* for getpwuid, etc, for "level_none" */
#endif

#include <noauthcn.h>

INTERNAL boolean32 rpc__noauth_cn_three_way _DCE_PROTOTYPE_((void));

INTERNAL boolean32 rpc__noauth_cn_context_valid _DCE_PROTOTYPE_((
        rpc_cn_sec_context_p_t           /*sec*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_create_info _DCE_PROTOTYPE_((
       rpc_authn_level_t                 /*authn_level*/,
       rpc_auth_info_p_t                * /*auth_info*/,
       unsigned32                       * /*st*/
    ));

INTERNAL boolean32 rpc__noauth_cn_cred_changed _DCE_PROTOTYPE_((
        rpc_cn_sec_context_p_t           /*sec*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_cred_refresh _DCE_PROTOTYPE_((
        rpc_auth_info_p_t                /*auth_info*/,
        rpc_binding_handle_t             /*h*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_fmt_client_req _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t     /*assoc_sec*/,
        rpc_cn_sec_context_p_t           /*sec*/,
        pointer_t                        /*auth_value*/,
        unsigned32                      * /*auth_value_len*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_fmt_srvr_resp _DCE_PROTOTYPE_((
        unsigned32                       /*verify_st*/,
        rpc_cn_assoc_sec_context_p_t     /*assoc_sec*/,
        rpc_cn_sec_context_p_t           /*sec*/,
        pointer_t                        /*req_auth_value*/,
        unsigned32                       /*req_auth_value_len*/,
        pointer_t                        /*auth_value*/,
        unsigned32                      * /*auth_value_len*/
    ));

INTERNAL void rpc__noauth_cn_free_prot_info _DCE_PROTOTYPE_((
        rpc_auth_info_p_t                /*info*/,
        rpc_cn_auth_info_p_t            * /*cn_info*/
    ));

INTERNAL void rpc__noauth_cn_get_prot_info _DCE_PROTOTYPE_((
        rpc_auth_info_p_t                /*info*/,
        rpc_cn_auth_info_p_t            * /*cn_info*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_pre_call _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t     /*assoc_sec*/,
        rpc_cn_sec_context_p_t           /*sec*/,
        pointer_t                        /*auth_value*/,
        unsigned32                      * /*auth_value_len*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_pre_send _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t     /*assoc_sec*/,
        rpc_cn_sec_context_p_t           /*sec*/,
        rpc_socket_iovec_p_t             /*iov*/,
        unsigned32                       /*iovlen*/,
	rpc_socket_iovec_p_t             /*out_iov*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_recv_check _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t     /*assoc_sec*/,
        rpc_cn_sec_context_p_t           /*sec*/,
        rpc_cn_common_hdr_p_t            /*pdu*/,
        unsigned32                       /*pdu_len*/,
        unsigned32                       /*cred_len*/,
        rpc_cn_auth_tlr_p_t              /*auth_tlr*/,
        boolean32                        /*unpack_ints*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_tlr_uuid_crc _DCE_PROTOTYPE_((
        pointer_t                /*auth_value*/,
        unsigned32               /*auth_value_len*/,
        unsigned32              * /*uuid_crc*/
    ));

INTERNAL void rpc__noauth_cn_tlr_unpack _DCE_PROTOTYPE_((
        rpc_cn_packet_p_t        /*pkt_p*/,
        unsigned32               /*auth_value_len*/,
        unsigned8               * /*packed_drep*/
    ));

INTERNAL void rpc__noauth_cn_vfy_client_req _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t     /*assoc_sec*/,
        rpc_cn_sec_context_p_t           /*sec*/,
        pointer_t                        /*auth_value*/,
        unsigned32                       /*auth_value_len*/,
        unsigned32                      * /*st*/
    ));

INTERNAL void rpc__noauth_cn_vfy_srvr_resp _DCE_PROTOTYPE_((
        rpc_cn_assoc_sec_context_p_t     /*assoc_sec*/,
        rpc_cn_sec_context_p_t           /*sec*/,
        pointer_t                        /*auth_value*/,
        unsigned32                       /*auth_value_len*/,
        unsigned32                      * /*st*/
    ));

GLOBAL rpc_cn_auth_epv_t rpc_g_noauth_cn_epv =
{
    rpc__noauth_cn_three_way,
    rpc__noauth_cn_context_valid,
    rpc__noauth_cn_create_info,
    rpc__noauth_cn_cred_changed,
    rpc__noauth_cn_cred_refresh,
    rpc__noauth_cn_fmt_client_req,
    rpc__noauth_cn_fmt_srvr_resp,
    rpc__noauth_cn_free_prot_info,
    rpc__noauth_cn_get_prot_info,
    rpc__noauth_cn_pre_call,
    rpc__noauth_cn_pre_send,
    rpc__noauth_cn_recv_check,
    rpc__noauth_cn_tlr_uuid_crc,
    rpc__noauth_cn_tlr_unpack,
    rpc__noauth_cn_vfy_client_req,
    rpc__noauth_cn_vfy_srvr_resp
};

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_three_way
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      Determine whether the authentication protocol requires a
**      3-way authentication handshake. If true the client is expected to
**      provide an rpc_auth3 PDU before the security context is fully
**      established and a call can be made.
**
**  INPUTS:             none
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     boolean32
**
**      True if the authentication protocol requires a 3-way
**      authentication handshake.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL boolean32 rpc__noauth_cn_three_way (void)
{
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_three_way)\n"));


    return (false);
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_context_valid
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      Determine whether the established security context will be
**      valid (i. e. timely) for the next 300 seconds. If
**      not this routine will try to renew the context.
**      If it cannot be renewed false is returned. This is
**      called from the client side.
**
**  INPUTS:
**
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     boolean32
**
**      True if security context identified by the auth
**      information rep and RPC auth information rep will
**      still be valid in 300 seconds, false if not.
**
**  SIDE EFFECTS:       
**
**      The context may be renewed.
**
**--
**/

INTERNAL boolean32 rpc__noauth_cn_context_valid 
#ifdef _DCE_PROTO_
(
    rpc_cn_sec_context_p_t          sec,
    unsigned32                      *st
)
#else
(sec, st)
rpc_cn_sec_context_p_t          sec;
unsigned32                      *st;
#endif
{
    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_context_valid)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_GENERAL,
                    ("(rpc__noauth_cn_context_valid) time->%x\n", time));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_context_valid) prot->%x level->%x key_id->%x\n",
                    rpc_c_authn_dce_dummy,
                    sec->sec_info->authn_level,
                    sec->sec_key_id));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_CONTEXT_VALID))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return (false);
    }
#endif

    *st = rpc_s_ok;
    return (true);
}


/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_create_info
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      Create an auth information rep data structure with and
**      add a reference to it. This is called on the server
**      side. The fields will be initialized to NULL values.
**      The caller should fill them in based on information
**      decrypted from the authenticator passed in the bind
**      request.
**
**  INPUTS:
**
**      authn_level     The authentication level to be applied over this
**                      security context.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      info            A pointer to the auth information rep structure
**                      containing RPC protocol indenpendent information.
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       
**
**      The newly create auth info will have a reference count of 1.
**
**--
**/

INTERNAL void rpc__noauth_cn_create_info 
#ifdef _DCE_PROTO_
(
    rpc_authn_level_t                authn_level,
    rpc_auth_info_p_t                *auth_info,
    unsigned32                       *st
)
#else
(authn_level, auth_info, st)
rpc_authn_level_t                authn_level;
rpc_auth_info_p_t                *auth_info;
unsigned32                       *st;
#endif
{
    rpc_noauth_info_p_t noauth_info;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_create_info)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_create_info) prot->%x level->%x\n",
                    rpc_c_authn_dce_dummy,
                    authn_level));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_CREATE_INFO))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    /*
     * Allocate storage for a noauth info structure from heap.
     */
    RPC_MEM_ALLOC (noauth_info, 
                   rpc_noauth_info_p_t, 
                   sizeof (rpc_noauth_info_t), 
                   RPC_C_MEM_NOAUTH_INFO, 
                   RPC_C_MEM_WAITOK);
    if (noauth_info == NULL){
	*st = rpc_s_no_memory;
	return;
    }

    /*
     * Initialize it.
     */
    memset (noauth_info, 0, sizeof(rpc_noauth_info_t));
    RPC_MUTEX_INIT (noauth_info->lock);

    /*
     * Initialize the common auth_info stuff.
     */
    noauth_info->auth_info.refcount = 1;
    noauth_info->auth_info.server_princ_name = '\0';
    noauth_info->auth_info.authn_level = authn_level;
    noauth_info->auth_info.authn_protocol = rpc_c_authn_dce_dummy;
    noauth_info->auth_info.authz_protocol = rpc_c_authz_name;
    noauth_info->auth_info.is_server = true;

    *auth_info = (rpc_auth_info_t *) noauth_info;
    *st = rpc_s_ok;
}


/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_cred_changed
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      Determine whether the client's credentials stored in the
**      security context are different from those in the auth info.
**      If they are not the same return true, else false.
**
**  INPUTS:
**
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     boolean32
**
**      True if the credentials identified by the auth
**      information rep and RPC auth information rep are different,
**      false if not.
**
**      The md5 checksum algorithm requires the use of the session key
**      to encrypt the CRC(assoc_uuid).  Since the session key will 
**      change when the credential changes, this routine sets the flag
**      indicating that a (potentially) valid encrypted crc is now 
**      invalid, forcing a recomputation.
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL boolean32 rpc__noauth_cn_cred_changed 
#ifdef _DCE_PROTO_
(
    rpc_cn_sec_context_p_t          sec,
    unsigned32                      *st
)
#else
(sec, st)
rpc_cn_sec_context_p_t          sec;
unsigned32                      *st;
#endif
{
    rpc_noauth_cn_info_t        *noauth_cn_info;
    rpc_noauth_info_p_t         noauth_info;
    boolean32                   different_creds;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_cred_changed)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_cred_changed) prot->%x level->%x key_id->%x\n",
                    rpc_c_authn_dce_private,
                    sec->sec_info->authn_level,
                    sec->sec_key_id));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_CRED_CHANGED))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return (false);
    }
#endif

    /*
     * Assume that cred is already valid.
     */
    different_creds = false;
    *st = rpc_s_ok;
    return (different_creds);
}


/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_cred_refresh
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      Determine whether the client's credentials are still
**      valid. If not this routine will try to renew the credentials.
**      If they cannot be renewed an error is returned. This routine
**      is called from the client side. 
**
**  INPUTS:
**
**      auth_info       A pointer to the auth information rep
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_cred_refresh 
#ifdef _DCE_PROTO_
(
    rpc_auth_info_p_t               auth_info,
    rpc_binding_handle_t            h,
    unsigned32                      *st
)
#else
(auth_info, h, st)
rpc_auth_info_p_t               auth_info;
rpc_binding_handle_t            h;
unsigned32                      *st;
#endif
{
    rpc_noauth_cn_info_t        *noauth_cn_info;
    rpc_noauth_info_p_t         noauth_info;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_cred_refresh)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_cred_refresh) prot->%x level->%x\n",
                    rpc_c_authn_dce_private,
                    auth_info->authn_level));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_CRED_REFRESH))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    /*
     * Assume that cred is already valid.
     */
    *st = rpc_s_ok;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_fmt_client_req
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will format the auth_value field of
**      either an rpc_bind or rpc_alter_context PDU. This is
**      called from the client side association state machine.
**
**  INPUTS:
**
**      assoc_sec       A pointer to per-association security context
**                      including association UUID CRC and sequence numbers.
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**      auth_value      A pointer to the auth_value field in the rpc_bind or
**                      rpc_alter_context PDU authentication trailer.
**
**  INPUTS/OUTPUTS:     
**
**      auth_value_len  On input, the lenght, in bytes of the available space
**                      for the auth_value field. On output, the lenght in
**                      bytes used in encoding the auth_value field. Zero if
**                      an error status is returned.
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_fmt_client_req 
#ifdef _DCE_PROTO_
(
    rpc_cn_assoc_sec_context_p_t    assoc_sec,
    rpc_cn_sec_context_p_t          sec,
    pointer_t                       auth_value,
    unsigned32                      *auth_value_len,
    unsigned32                      *st
)
#else
(assoc_sec, sec, auth_value, auth_value_len, st)
rpc_cn_assoc_sec_context_p_t    assoc_sec;
rpc_cn_sec_context_p_t          sec;
pointer_t                       auth_value;
unsigned32                      *auth_value_len;
unsigned32                      *st;
#endif
{
    rpc_cn_bind_auth_value_priv_t       *priv_auth_value;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_fmt_client_req)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_fmt_client_req) prot->%x level->%x key_id->%x assoc_uuid_crc->%x xmit_seq->%x recv_seq->%x\n",
                    rpc_c_authn_dce_dummy,
                    sec->sec_info->authn_level,
                    sec->sec_key_id,
                    assoc_sec->assoc_uuid_crc,
                    assoc_sec->assoc_next_snd_seq,
                    assoc_sec->assoc_next_rcv_seq));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_FMT_CLIENT_REQ))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    if (*auth_value_len < RPC_CN_PKT_SIZEOF_BIND_AUTH_VAL)
    {
        *st = rpc_s_credentials_too_large;
        return;
    }
    else
    {
        *auth_value_len = RPC_CN_PKT_SIZEOF_BIND_AUTH_VAL;
    }

    priv_auth_value = (rpc_cn_bind_auth_value_priv_t *)auth_value;
    priv_auth_value->assoc_uuid_crc = assoc_sec->assoc_uuid_crc;
    priv_auth_value->sub_type = RPC_C_CN_DCE_SUB_TYPE;
    priv_auth_value->checksum_length = 0;
    priv_auth_value->cred_length = 0;
    *st = rpc_s_ok;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_fmt_srvr_resp
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will format the auth_value field of
**      either an rpc_bind_ack or rpc_alter_context_response
**      PDU. The authentication protocol encoding in the
**      credentials field of auth_value should correspond
**      to the status returned from rpc__auth_cn_vfy_
**      client_req()  routine. This credentials field, when
**      decoded by rpc__auth_cn_vfy_srvr_resp(),  should
**      result in the same error code being returned. If the
**      memory provided is not large enough an authentication
**      protocol specific error message will be encoded in
**      the credentials field indicating this error. This is
**      called from the server side association state machine.
**
**  INPUTS:
**
**      verify_st       The status code returned by rpc__auth_cn_verify_
**                      client_req().
**      assoc_sec       A pointer to per-association security context
**                      including association UUID CRC and sequence numbers.
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**      req_auth_value  A pointer to the auth_value field in the
**                      rpc_bind or rpc_alter_context PDU authentication trailer.
**      req_auth_value_len The length, in bytes, of the
**                      req_auth_value field.
**      auth_value      A pointer to the auth_value field in the rpc_bind or
**                      rpc_alter_context PDU authentication trailer.
**
**  INPUTS/OUTPUTS:     
**
**      auth_value_len  On input, the length, in bytes of the available space
**                      for the auth_value field. On output, the length in
**                      bytes used in encoding the auth_value field. Zero if
**                      an error status is returned.
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_fmt_srvr_resp 
#ifdef _DCE_PROTO_
(
    unsigned32                      verify_st,
    rpc_cn_assoc_sec_context_p_t    assoc_sec,
    rpc_cn_sec_context_p_t          sec,
    pointer_t                       req_auth_value,
    unsigned32                      req_auth_value_len,
    pointer_t                       auth_value,
    unsigned32                      *auth_value_len
)
#else
(verify_st, assoc_sec, sec, req_auth_value, req_auth_value_len, auth_value, auth_value_len)
unsigned32                      verify_st;
rpc_cn_assoc_sec_context_p_t    assoc_sec;
rpc_cn_sec_context_p_t          sec;
pointer_t                       req_auth_value;
unsigned32                      req_auth_value_len;
pointer_t                       auth_value;
unsigned32                      *auth_value_len;
#endif
{
    rpc_cn_bind_auth_value_priv_t       *priv_auth_value;

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_fmt_srvr_resp)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_fmt_srvr_resp) prot->%x level->%x key_id->%x assoc_uuid_crc->%x xmit_seq->%x recv_seq->%x, vfy_client_st->%x\n",
                    rpc_c_authn_dce_dummy,
                    sec->sec_info->authn_level,
                    sec->sec_key_id,
                    assoc_sec->assoc_uuid_crc,
                    assoc_sec->assoc_next_snd_seq,
                    assoc_sec->assoc_next_rcv_seq,
                    verify_st));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_FMT_SERVER_RESP))
    {
        verify_st = RPC_S_CN_DBG_AUTH_FAILURE;
    }
#endif

    assert (verify_st == rpc_s_ok);
    assert (*auth_value_len >= RPC_CN_PKT_SIZEOF_BIND_AUTH_VAL);
    *auth_value_len = RPC_CN_PKT_SIZEOF_BIND_AUTH_VAL;

    priv_auth_value = (rpc_cn_bind_auth_value_priv_t *)auth_value;
    priv_auth_value->assoc_uuid_crc = assoc_sec->assoc_uuid_crc;
    priv_auth_value->sub_type = RPC_C_CN_DCE_SUB_TYPE;
    priv_auth_value->checksum_length = 0;
    priv_auth_value->cred_length = 0;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_free_prot_info
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will free an NCA Connection RPC auth
**      information rep.
**
**  INPUTS:             
**
**      info            A pointer to the auth information rep structure
**                      containing RPC protocol indenpendent information.
**
**  INPUTS/OUTPUTS:     
**
**      cn_info         A pointer to the RPC auth information rep structure
**                      containing NCA Connection specific
**                      information. NULL on output. 
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_free_prot_info 
#ifdef _DCE_PROTO_
(
    rpc_auth_info_p_t               info,    
    rpc_cn_auth_info_p_t            *cn_info
)
#else
(info, cn_info)
rpc_auth_info_p_t               info;    
rpc_cn_auth_info_p_t            *cn_info;
#endif
{
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_free_prot_info)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_free_prot_info) prot->%x level->%x \n",
                    rpc_c_authn_dce_dummy,
                    info->authn_level));

#ifdef DEBUG
    memset (*cn_info, 0, sizeof (rpc_cn_auth_info_t));
#endif

    RPC_MEM_FREE (*cn_info, RPC_C_MEM_NOAUTH_CN_INFO);
    *cn_info = NULL;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_get_prot_info
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will create and return an NCA Connection
**      RPC auth information rep.
**
**  INPUTS:
**
**      info            A pointer to the auth information rep structure
**                      containing RPC protocol indenpendent information.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      cn_info         A pointer to the RPC auth information rep structure
**                      containing NCA Connection specific information.
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_get_prot_info 
#ifdef _DCE_PROTO_
(
    rpc_auth_info_p_t               info,
    rpc_cn_auth_info_p_t            *cn_info,
    unsigned32                      *st
)
#else
(info, cn_info, st)
rpc_auth_info_p_t               info;
rpc_cn_auth_info_p_t            *cn_info;
unsigned32                      *st;
#endif
{
    rpc_noauth_cn_info_t        *noauth_cn_info;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_get_prot_info)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_get_prot_info) prot->%x level->%x \n",
                    rpc_c_authn_dce_dummy,
                    info->authn_level));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_GET_PROT_INFO))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    /*
     * Allocate storage for a noauth cn info structure from heap.
     */
    RPC_MEM_ALLOC (noauth_cn_info, 
                   rpc_noauth_cn_info_p_t, 
                   sizeof (rpc_noauth_cn_info_t), 
                   RPC_C_MEM_NOAUTH_CN_INFO, 
                   RPC_C_MEM_WAITOK);
    if (noauth_cn_info == NULL){
	*st = rpc_s_no_memory;
	return;
    }

    /*
     * Initialize it.
     */
    memset (noauth_cn_info, 0, sizeof(rpc_noauth_cn_info_t));

    *cn_info = (rpc_cn_auth_info_t *)noauth_cn_info;
    *st = rpc_s_ok;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_pre_call
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will format the auth_value field of
**      a call level PDU, namely an rpc_request, rpc_response
**      rpc_fault, rpc_remote_alert or rpc_orphaned PDU. It will
**      also format the auth_value field of the association level
**      rpc_shutdown PDU. This does
**      not include generating any checksums in the auth_value_field
**      or encrypting of data corresponding to the authentication
**      level. That will be done in the rpc__cn_auth_pre_send
**      routine. This is called on both client and server when a the
**      data structures and header template for a call is being set up.
**
**  INPUTS:
**
**      assoc_sec       A pointer to per-association security context
**                      including association UUID CRC and sequence numbers.
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**      auth_value      A pointer to the auth_value field in the rpc_bind or
**                      rpc_alter_context PDU authentication trailer.
**
**  INPUTS/OUTPUTS:     
**
**      auth_value_len  On input, the lenght, in bytes of the available space
**                      for the auth_value field. On output, the lenght in
**                      bytes used in encoding the auth_value field. Zero if
**                      an error status is returned.
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_pre_call 
#ifdef _DCE_PROTO_
(
    rpc_cn_assoc_sec_context_p_t    assoc_sec,
    rpc_cn_sec_context_p_t          sec,
    pointer_t                       auth_value,
    unsigned32                      *auth_value_len,
    unsigned32                      *st
)
#else
(assoc_sec, sec, auth_value, auth_value_len, st)
rpc_cn_assoc_sec_context_p_t    assoc_sec;
rpc_cn_sec_context_p_t          sec;
pointer_t                       auth_value;
unsigned32                      *auth_value_len;
unsigned32                      *st;
#endif
{
    rpc_cn_auth_value_priv_t    *priv_auth_value;

    CODING_ERROR(st);

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_pre_call)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_pre_call) prot->%x level->%x key_id->%x assoc_uuid_crc->%x xmit_seq->%x recv_seq->%x\n",
                    rpc_c_authn_dce_dummy,
                    sec->sec_info->authn_level,
                    sec->sec_key_id,
                    assoc_sec->assoc_uuid_crc,
                    assoc_sec->assoc_next_snd_seq,
                    assoc_sec->assoc_next_rcv_seq));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_PRE_CALL))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    if (sec->sec_info->authn_level >= rpc_c_authn_level_pkt)
    {
        if (*auth_value_len < RPC_CN_PKT_SIZEOF_AUTH_VAL_CKSM)
        {
            *st = rpc_s_credentials_too_large;
            return;
        }
        else
        {
            *auth_value_len = RPC_CN_PKT_SIZEOF_AUTH_VAL_CKSM;
        }
    }
    else
    {
        if (*auth_value_len < RPC_CN_PKT_SIZEOF_AUTH_VAL)
        {
            *st = rpc_s_credentials_too_large;
            return;
        }
        else
        {
            *auth_value_len = RPC_CN_PKT_SIZEOF_AUTH_VAL;
        }
    }

    priv_auth_value = (rpc_cn_auth_value_priv_t *)auth_value;
    priv_auth_value->sub_type = RPC_C_CN_DCE_SUB_TYPE;
    priv_auth_value->checksum_length = 0;
    *st = rpc_s_ok;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_pre_send
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will perform per-packet security
**      processing on a packet before it is sent. This
**      includes checksumming and encryption.
**
**      Note that in some cases, the data is copied to
**      a contiguous buffer for checksumming and 
**      encryption.  In these cases, the contiguous
**      iov element should be used instead of the original 
**      iovector.
**
**  INPUTS:
**
**      assoc_sec       A pointer to per-association security context
**                      including association UUID CRC and sequence numbers.
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**      iov             A pointer to the iovector containing the PDU
**                      about to be sent. The appropriate per-packet security
**                      services will be applied to it. 
**      iovlen          The length, in bytes, of the PDU.
**      out_iov         An iovector element.  This iovector element
**                      will describe packet if the original iov
**                      had to be copied.  If the original iov was
**                      copied, out_iov->base will be non-NULL.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_pre_send 
#ifdef _DCE_PROTO_
(
    rpc_cn_assoc_sec_context_p_t    assoc_sec,
    rpc_cn_sec_context_p_t          sec,
    rpc_socket_iovec_p_t            iov,
    unsigned32                      iovlen,
    rpc_socket_iovec_p_t            out_iov,
    unsigned32                      *st
)
#else
(assoc_sec, sec, iov, iovlen, out_iov, st)
rpc_cn_assoc_sec_context_p_t    assoc_sec;
rpc_cn_sec_context_p_t          sec;
rpc_socket_iovec_p_t            iov;
unsigned32                      iovlen;
rpc_socket_iovec_p_t            out_iov;
unsigned32                      *st;
#endif
{
    unsigned32          ptype;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_pre_send)\n"));

    ptype = ((rpc_cn_common_hdr_t *)(iov[0].base))->ptype;
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_GENERAL,
                    ("(rpc__noauth_cn_pre_send) authn level->%x packet type->%x\n", sec->sec_info->authn_level, ptype));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_pre_send) prot->%x level->%x key_id->%x assoc_uuid_crc->%x xmit_seq->%x recv_seq->%x ptype->%x\n",
                    rpc_c_authn_dce_dummy,
                    sec->sec_info->authn_level,
                    sec->sec_key_id,
                    assoc_sec->assoc_uuid_crc,
                    assoc_sec->assoc_next_snd_seq,
                    assoc_sec->assoc_next_rcv_seq,
                    ptype));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_PRE_SEND))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif
    
    out_iov->base = NULL;
    switch (ptype)
    {
        case RPC_C_CN_PKT_REQUEST:
        case RPC_C_CN_PKT_RESPONSE:
        case RPC_C_CN_PKT_FAULT:
        {
            break;
        }

        case RPC_C_CN_PKT_BIND:
        case RPC_C_CN_PKT_BIND_ACK:
        case RPC_C_CN_PKT_BIND_NAK:
        case RPC_C_CN_PKT_ALTER_CONTEXT:
        case RPC_C_CN_PKT_ALTER_CONTEXT_RESP:
        case RPC_C_CN_PKT_AUTH3:
        case RPC_C_CN_PKT_SHUTDOWN:
        case RPC_C_CN_PKT_REMOTE_ALERT:
        case RPC_C_CN_PKT_ORPHANED:
        default:
        {
            break;
        }
    }

    *st = rpc_s_ok;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_recv_check
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will perform per-packet security
**      processing on a packet after it is received. This
**      includes decryption and verification of checksums.
**
**  INPUTS:
**
**      assoc_sec       A pointer to per-association security context
**                      including association UUID CRC and sequence numbers.
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**      pdu             A pointer to the PDU about to be sent. The appropriate
**                      per-packet security services will be applied to it.
**      pdu_len         The length, in bytes, of the PDU.
**      cred_len        The length, in bytes, of the credentials.
**      auth_tlr        A pointer to the auth trailer.
**      unpack_ints     A boolean indicating whether the integer rep
**                      of fields in the pdu need to be adjusted for
**                      endian differences.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_recv_check 
#ifdef _DCE_PROTO_
(
    rpc_cn_assoc_sec_context_p_t    assoc_sec,
    rpc_cn_sec_context_p_t          sec,
    rpc_cn_common_hdr_p_t           pdu,
    unsigned32                      pdu_len,
    unsigned32                      cred_len,
    rpc_cn_auth_tlr_p_t             auth_tlr,
    boolean32                       unpack_ints,
    unsigned32                      *st
)
#else
(assoc_sec, sec, pdu, pdu_len, cred_len, auth_tlr, unpack_ints, st)
rpc_cn_assoc_sec_context_p_t    assoc_sec;
rpc_cn_sec_context_p_t          sec;
rpc_cn_common_hdr_p_t           pdu;
unsigned32                      pdu_len;
unsigned32                      cred_len;
rpc_cn_auth_tlr_p_t             auth_tlr;
boolean32                       unpack_ints;
unsigned32                      *st;
#endif
{
    rpc_cn_auth_value_priv_t    *priv_auth_value;
    unsigned32                  ptype;
    unsigned32                  authn_level;
    unsigned32                  assoc_uuid_crc;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_recv_check)\n"));

    ptype = pdu->ptype;
    priv_auth_value = (rpc_cn_auth_value_priv_t *)(auth_tlr->auth_value);
    authn_level = auth_tlr->auth_level;
    if (ptype == RPC_C_CN_PKT_BIND)
    {
        assoc_uuid_crc = ((rpc_cn_bind_auth_value_priv_t *)priv_auth_value)->assoc_uuid_crc;
    }
    else
    {
        assoc_uuid_crc = assoc_sec->assoc_uuid_crc;
    }

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_GENERAL,
                    ("(rpc__noauth_cn_recv_check) authn level->%x packet type->%x\n", authn_level, ptype));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_recv_check) prot->%x level->%x key_id->%x assoc_uuid_crc->%x xmit_seq->%x recv_seq->%x ptype->%x\n",
                    rpc_c_authn_dce_dummy,
                    authn_level,
                    sec->sec_key_id,
                    assoc_uuid_crc,
                    assoc_sec->assoc_next_snd_seq,
                    assoc_sec->assoc_next_rcv_seq,
                    ptype));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_RECV_CHECK))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    switch (ptype)
    {
        case RPC_C_CN_PKT_REQUEST:
        case RPC_C_CN_PKT_RESPONSE:
        case RPC_C_CN_PKT_FAULT:
        case RPC_C_CN_PKT_BIND:
        case RPC_C_CN_PKT_BIND_ACK:
        case RPC_C_CN_PKT_BIND_NAK:
        case RPC_C_CN_PKT_ALTER_CONTEXT:
        case RPC_C_CN_PKT_ALTER_CONTEXT_RESP:
        case RPC_C_CN_PKT_AUTH3:
        case RPC_C_CN_PKT_SHUTDOWN:
        case RPC_C_CN_PKT_REMOTE_ALERT:
        case RPC_C_CN_PKT_ORPHANED:
        default:
        {
            break;
        }
    }

    *st = rpc_s_ok;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_tlr_uuid_crc
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will locate and return the association
**      UUID CRC contained in the auth_value field of an
**      authentication trailer of an rpc_bind, rpc_bind_ack,
**      rpc_alter_context or rpc_alter_context_response PDU.
**
**  INPUTS:
**
**      auth_value      A pointer to the auth_value field in an authentication
**                      trailer.
**      auth_value_len  The length, in bytes, of the auth_value field.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      assoc_uuid_crc  The association UUID CRC contained in the auth_value
**                      field.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_tlr_uuid_crc 
#ifdef _DCE_PROTO_
(
    pointer_t               auth_value,
    unsigned32              auth_value_len,
    unsigned32              *uuid_crc
)
#else
(auth_value, auth_value_len, uuid_crc)
pointer_t               auth_value;
unsigned32              auth_value_len;
unsigned32              *uuid_crc;
#endif
{
    rpc_cn_bind_auth_value_priv_t       *priv_auth_value;

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_tlr_uuid_crc)\n"));

    assert (auth_value_len >= RPC_CN_PKT_SIZEOF_BIND_AUTH_VAL);
    priv_auth_value = (rpc_cn_bind_auth_value_priv_t *)auth_value;
    *uuid_crc = priv_auth_value->assoc_uuid_crc;

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_GENERAL,
                    ("(rpc__noauth_cn_tlr_uuid_crc) assoc_uuid_crc->%x\n", *uuid_crc));
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_tlr_unpack
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will byte swap all the appropriate fields
**      of the the auth_value field of an authentication
**      trailer. It will also convert any characters from
**      the remote representation into the local, for example,
**      ASCII to EBCDIC.
**
**  INPUTS:
**
**      auth_value_len  The length, in bytes, of the auth_value field.
**      packed_drep     The packed Networ Data Representation, (see NCA RPC
**                      RunTime Extensions Specification Version OSF TX1.0.9
**                      pre 1003 for details), of the remote machine.
**
**  INPUTS/OUTPUTS:     
**
**      pkt_p           A pointer to the entire packet.
**
**  OUTPUTS:            none
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_tlr_unpack 
#ifdef _DCE_PROTO_
(
    rpc_cn_packet_p_t       pkt_p,
    unsigned32              auth_value_len,
    unsigned8               *packed_drep
)
#else
(pkt_p, auth_value_len, packed_drep) 
rpc_cn_packet_p_t       pkt_p;
unsigned32              auth_value_len;
unsigned8               *packed_drep;
#endif
{
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_tlr_unpack)\n"));
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_vfy_client_req
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will decode the auth_value field of
**      either an rpc_bind or rpc_alter_context PDU. Any
**      error encountered while authenticating the client
**      will result in an error status return. The contents
**      of the credentials field includes the authorization
**      data. This is called from the server side association
**      state machine. Note that upon successful return
**      the auth information rep will contain the client's
**      authorization protocol and data.
**
**  INPUTS:
**
**      assoc_sec       A pointer to per-association security context
**                      including association UUID CRC and sequence numbers.
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**      auth_value      A pointer to the auth_value field in the rpc_bind or
**                      rpc_alter_context PDU authentication trailer.
**      auth_value_len  The length, in bytes, of auth_value.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_vfy_client_req 
#ifdef _DCE_PROTO_
(
    rpc_cn_assoc_sec_context_p_t    assoc_sec,
    rpc_cn_sec_context_p_t          sec,
    pointer_t                       auth_value,
    unsigned32                      auth_value_len,
    unsigned32                      *st
)
#else
(assoc_sec, sec, auth_value, auth_value_len, st)
rpc_cn_assoc_sec_context_p_t    assoc_sec;
rpc_cn_sec_context_p_t          sec;
pointer_t                       auth_value;
unsigned32                      auth_value_len;
unsigned32                      *st;
#endif
{
    rpc_cn_bind_auth_value_priv_t       *priv_auth_value;

    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_vfy_client_req)\n"));

    priv_auth_value = (rpc_cn_bind_auth_value_priv_t *)auth_value;
    
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_vfy_client_req) prot->%x level->%x key_id->%x assoc_uuid_crc->%x xmit_seq->%x recv_seq->%x\n",
                    rpc_c_authn_dce_dummy,
                    sec->sec_info->authn_level,
                    sec->sec_key_id,
                    assoc_sec->assoc_uuid_crc,
                    assoc_sec->assoc_next_snd_seq,
                    assoc_sec->assoc_next_rcv_seq));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_VFY_CLIENT_REQ))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    *st = rpc_s_ok;
}

/*****************************************************************************/
/*
**++
**
**  ROUTINE NAME:       rpc__noauth_cn_vfy_srvr_resp
**
**  SCOPE:              INTERNAL - declared locally
**
**  DESCRIPTION:
**      
**      This routine will decode auth_value field either an
**      rpc_bind_ack or rpc_alter_context_response PDU. If the
**      credentials field of the auth_value field contains an
**      authentication protocol specific encoding of an error
**      this will be returned as an error status code. This is
**      called from the client side association state machine.
**      Note that upon successful return the auth information
**      rep will contain the client's authorization protocol
**      and data.
**
**  INPUTS:
**
**      assoc_sec       A pointer to per-association security context
**                      including association UUID CRC and sequence numbers.
**      sec             A pointer to security context element which includes
**                      the key ID, auth information rep and RPC auth
**                      information rep.
**      auth_value      A pointer to the auth_value field in the rpc_bind or
**                      rpc_alter_context PDU authentication trailer.
**      auth_value_len  The length, in bytes, of auth_value.
**
**  INPUTS/OUTPUTS:     none
**
**  OUTPUTS:            
**
**      st              The return status of this routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

INTERNAL void rpc__noauth_cn_vfy_srvr_resp 
#ifdef _DCE_PROTO_
(
    rpc_cn_assoc_sec_context_p_t    assoc_sec,
    rpc_cn_sec_context_p_t          sec,
    pointer_t                       auth_value,
    unsigned32                      auth_value_len,
    unsigned32                      *st
)
#else
(assoc_sec, sec, auth_value, auth_value_len, st) 
rpc_cn_assoc_sec_context_p_t    assoc_sec;
rpc_cn_sec_context_p_t          sec;
pointer_t                       auth_value;
unsigned32                      auth_value_len;
unsigned32                      *st;
#endif
{
    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_vfy_srvr_resp)\n"));

    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_PKT,
                    ("(rpc__noauth_cn_vfy_server_resp) prot->%x level->%x key_id->%x assoc_uuid_crc->%x xmit_seq->%x recv_seq->%x\n",
                    rpc_c_authn_dce_dummy,
                    sec->sec_info->authn_level,
                    sec->sec_key_id,
                    assoc_sec->assoc_uuid_crc,
                    assoc_sec->assoc_next_snd_seq,
                    assoc_sec->assoc_next_rcv_seq));

#ifdef DEBUG
    if (RPC_DBG_EXACT(rpc_es_dbg_cn_errors,
                      RPC_C_CN_DBG_AUTH_VFY_SERVER_RESP))
    {
        *st = RPC_S_CN_DBG_AUTH_FAILURE;
        return;
    }
#endif

    *st = rpc_s_ok;
}


PRIVATE rpc_protocol_id_t       rpc__noauth_cn_init 
#ifdef _DCE_PROTO_
(
    rpc_auth_rpc_prot_epv_p_t       *epv,
    unsigned32                      *st
)
#else
(epv, st)
rpc_auth_rpc_prot_epv_p_t       *epv;
unsigned32                      *st;
#endif
{
    CODING_ERROR (st);
    RPC_DBG_PRINTF (rpc_e_dbg_auth, RPC_C_CN_DBG_AUTH_ROUTINE_TRACE,
                    ("(rpc__noauth_cn_init)\n"));

    *epv = (rpc_auth_rpc_prot_epv_p_t) (&rpc_g_noauth_cn_epv);
    *st = rpc_s_ok;
    return (RPC_C_PROTOCOL_ID_NCACN);
}
