/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: noauthdg.c,v $
 * Revision 1.1.619.3  1996/02/18  00:04:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:56:57  marty]
 *
 * Revision 1.1.619.2  1995/12/08  00:21:13  root
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
 * 	[1995/12/07  23:59:53  root]
 * 
 * Revision 1.1.615.1  1994/10/18  18:04:15  tatsu_s
 * 	Fix OT12540: Added first_frag argument to rpc__noauth_dg_recv_ck().
 * 	[1994/10/06  20:06:41  tatsu_s]
 * 
 * Revision 1.1.613.2  1994/01/28  23:09:41  burati
 * 	EPAC changes - Add client creds where only pac was before (dlg_bl1)
 * 	[1994/01/24  23:42:49  burati]
 * 
 * Revision 1.1.613.1  1994/01/21  22:38:22  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:48  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  23:53:21  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:09:47  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  21:12:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:41:32  zeliff]
 * 
 * Revision 1.1.2.3  1992/05/01  16:38:25  rsalz
 * 	  9-mar-92 wh              DCE 1.0.1 merge.
 * 	 11-nov-91 mishkin         auth-reorg
 * 	[1992/05/01  16:30:08  rsalz]
 * 
 * Revision 1.1.2.2  1992/01/22  22:33:49  jim
 * 	Cast a parameter to security call.  This will not compile on AIX
 * 	without this cast.
 * 	[1992/01/22  21:26:39  jim]
 * 
 * Revision 1.1  1992/01/19  03:11:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      rpcdgcom.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  The noauth DG authentication module.
**
**
*/

#include <noauthdg.h>

EXTERNAL int rpc_g_noauth_alloc_count;
EXTERNAL int rpc_g_noauth_free_count;

INTERNAL rpc_dg_auth_epv_t rpc_g_noauth_dg_epv =
{
    rpc_c_authn_dce_dummy,      /* "3" */
    16,                         /* 10 bytes overhead, rounded up */
    8,                          /* 8 byte block */
    rpc__noauth_dg_create,
    rpc__noauth_dg_pre_call,
    rpc__noauth_dg_encrypt,
    rpc__noauth_dg_pre_send,
    rpc__noauth_dg_recv_ck,
    rpc__noauth_dg_who_are_you,
    rpc__noauth_dg_way_handler
};


/*
 * R P C _ _ N O A U T H _ D G _ E N C R Y P T
 *
 * Optionally encrypt user data in the packet.
 */

PRIVATE void rpc__noauth_dg_encrypt
#ifdef _DCE_PROTO_
(
        rpc_auth_info_p_t               info,
        rpc_dg_xmitq_elt_p_t            xqe,
        unsigned32                      *st
)
#else
(info, xqe, st)
    rpc_auth_info_p_t               info;
    rpc_dg_xmitq_elt_p_t            xqe;
    unsigned32                      *st;
#endif
{
    *st = rpc_s_ok;
}


/*
 * R P C _ _ N O A U T H _ D G _ P R E _ S E N D
 *
 * Optionally encrypt user data in the packet.
 */

PRIVATE void rpc__noauth_dg_pre_send 
#ifdef _DCE_PROTO_
(
        rpc_auth_info_p_t info,
        rpc_dg_xmitq_elt_p_t pkt,
        rpc_dg_pkt_hdr_p_t hdrp,
        rpc_socket_iovec_p_t iov,
        int iovlen,
        pointer_t cksum,
        error_status_t *st
)
#else
(info, pkt, hdrp, iov, iovlen, cksum, st)
    rpc_auth_info_p_t info;
    rpc_dg_xmitq_elt_p_t pkt;
    rpc_dg_pkt_hdr_p_t hdrp;
    rpc_socket_iovec_p_t iov;
    int iovlen;
    pointer_t cksum;
    error_status_t *st;
#endif
{
    *st = rpc_s_ok;
}


/*
 * R P C _ _ N O A U T H _ D G _ R E C V _ C K
 *
 */

PRIVATE void rpc__noauth_dg_recv_ck 
#ifdef _DCE_PROTO_
(
        rpc_auth_info_p_t info,
        rpc_dg_recvq_elt_p_t pkt,
        pointer_t cksum,
        boolean32 first_frag,
        error_status_t *st
)
#else
(info, pkt, cksum, st)
    rpc_auth_info_p_t info;
    rpc_dg_recvq_elt_p_t pkt;
    pointer_t cksum;
    boolean32 first_frag;
    error_status_t *st;
#endif
{
    *st = rpc_s_ok;
}


/*
 * R P C _ _ N O A U T H _ D G _ P R E _ C A L L
 *
 */

PRIVATE void rpc__noauth_dg_pre_call 
#ifdef _DCE_PROTO_
(
        rpc_auth_info_p_t info,
        handle_t h,
        unsigned32 *st
)
#else
(info, h, st)
    rpc_auth_info_p_t info;
    handle_t h;
    unsigned32 *st;
#endif
{
    *st = rpc_s_ok;
}


/*
 * R P C _ _ N O A U T H _ D G _ W A Y _ H A N D L E R
 *
 */

PRIVATE void rpc__noauth_dg_way_handler 
#ifdef _DCE_PROTO_
(
        rpc_auth_info_p_t info,
        ndr_byte *in_data,
        signed32 in_len,
        signed32 out_max_len,
        ndr_byte *out_data,
        signed32 *out_len,
        unsigned32 *stp
)
#else
(info, in_data, in_len, out_max_len, out_data, out_len, stp)
    rpc_auth_info_p_t info;
    ndr_byte *in_data;
    signed32 in_len;
    signed32 out_max_len;
    ndr_byte *out_data;
    signed32 *out_len;
    unsigned32 *stp;
#endif
{
    sec_krb_message message;
    error_status_t st;
    
    rpc_noauth_info_p_t noauth_info = (rpc_noauth_info_p_t)info;
        
    *out_len = 0;
    
    RPC_DBG_PRINTF(rpc_e_dbg_auth, 2, ("(rpc__noauth_dg_way_handler) %x called back\n", info));
    
    if (noauth_info->status != rpc_s_ok)
    {
        RPC_DBG_GPRINTF(("(rpc__noauth_dg_way_handler) handle was poisoned with %x\n",
            noauth_info->status));
        *stp = noauth_info->status;
        return;
    }

    message.data = 0;
    message.length = 0;
    
    st = sec_krb_dg_build_message (noauth_info->auth_info.u.auth_identity, 0, 0,
        rpc_c_authn_level_none, noauth_info->auth_info.authz_protocol,
        0, 0, 0, &message);

    if (st != rpc_s_ok)
        goto out;
        
    if (message.length > out_max_len)
    {
        st = rpc_s_credentials_too_large;
        goto out;
    }

    memcpy(out_data, message.data, message.length);
    *out_len = message.length;
out:
    sec_krb_message_free(&message);
    *stp = st;
    return;
}


/*
 * R P C _ _ N O A U T H _ D G _ W H O _ A R E _ Y O U
 *
 * Issue challenge to client; decompose response and sanity-check it.
 */

PRIVATE void rpc__noauth_dg_who_are_you 
#ifdef _DCE_PROTO_
(
        rpc_auth_info_p_t info,
        handle_t h,
        uuid_t *actuid,
        unsigned32 boot_time,
        unsigned32 *seq,
        uuid_t *cas_uuid,
        unsigned32 *stp
)
#else
(info, h, actuid, boot_time, seq, cas_uuid, stp)
    rpc_auth_info_p_t info;
    handle_t h;
    uuid_t *actuid;
    unsigned32 boot_time;
    unsigned32 *seq;
    uuid_t *cas_uuid;
    unsigned32 *stp;
#endif
{
    rpc_noauth_info_p_t noauth_info = (rpc_noauth_info_p_t)info;
    unsigned char inbuf[12];    /* XXX size */
    unsigned char outbuf[1000]; /* XXX size */
    unsigned_char_p_t server;
    signed32 outlen;
    sec_krb_message message;
    int st;

    /* XXX set up exception handler here around remote call? */
    RPC_DBG_PRINTF(rpc_e_dbg_auth, 2, ("(rpc__noauth_dg_way) %x doing callback\n", info));
    
    /* do call */
    (*conv_v3_0_c_epv.conv_who_are_you_auth)
        (h, actuid, boot_time, inbuf, 0, sizeof(outbuf),
         seq, cas_uuid, outbuf, &outlen, stp);
    
    st = *stp;
    if (st != rpc_s_ok)
    {
        RPC_DBG_GPRINTF(("(rpc__noauth_dg_way) conv_who_are_you_auth failed, st %x\n", st));
        return;
    }
    message.data = outbuf;
    message.length = outlen;

    *stp = sec_krb_dg_decode_message (&message, 0,
        &noauth_info->client_name,
        &noauth_info->client_pac,
        &noauth_info->client_creds,    /* FAKE-EPAC */
        &server,
        &noauth_info->auth_info.authn_level,
        &noauth_info->auth_info.authz_protocol,
        0, 0, 0, 0);
}


/*
 * R P C _ _ N O A U T H _ D G _ C R E A T E
 *
 * Issue challenge to client; decompose response and sanity-check it.
 */

PRIVATE rpc_auth_info_p_t rpc__noauth_dg_create 
#ifdef _DCE_PROTO_
(
        unsigned32 *stp
)
#else
(stp)
    unsigned32 *stp;
#endif
{
    rpc_noauth_info_p_t noauth_info;

    RPC_MEM_ALLOC (noauth_info, rpc_noauth_info_p_t, sizeof (*noauth_info), RPC_C_MEM_UTIL, RPC_C_MEM_WAITOK);
    if (noauth_info == NULL){
	*stp = rpc_s_no_memory;
	return(NULL);
    }

    rpc_g_noauth_alloc_count++;
    RPC_DBG_PRINTF(rpc_e_dbg_auth, 1,
        ("(rpc__noauth_dg_create) %x created (now %d active)\n", noauth_info,
            rpc_g_noauth_alloc_count - rpc_g_noauth_free_count));

    memset (noauth_info, '\0', sizeof(*noauth_info));

    RPC_MUTEX_INIT(noauth_info->lock);

    noauth_info->creds_valid = 0;
    noauth_info->level_valid = 0;
    noauth_info->client_valid = 0;

    /*
     * fill in the common auth_info stuff.
     */
    
    noauth_info->auth_info.refcount = 1;
    noauth_info->auth_info.server_princ_name = 0;
    noauth_info->auth_info.authn_level = -1;
    noauth_info->auth_info.authn_protocol = rpc_c_authn_dce_dummy;
    noauth_info->auth_info.authz_protocol = rpc_c_authz_name;
    noauth_info->auth_info.is_server = 1;
    noauth_info->auth_info.u.s.privs = 0;
    { /* FAKE-EPAC */
	noauth_info->auth_info.u.s.creds = 0;
    }
    

    /* XXX do other initialization here. */
    *stp = 0;
    return (rpc_auth_info_p_t) noauth_info;
}


/*
 * R P C _ _ N O A U T H _ D G _ I N I T
 *
 * Tell the datagram runtime about the noauth module.
 *
 */

PRIVATE rpc_protocol_id_t rpc__noauth_dg_init 
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
    *epv = (rpc_auth_rpc_prot_epv_p_t) (&rpc_g_noauth_dg_epv);
    *st = rpc_s_ok;
    return (RPC_C_PROTOCOL_ID_NCADG);
}

