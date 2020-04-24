/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: auth_con.h,v $
 * Revision 1.1.2.1  1996/06/05  20:56:53  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:54:52  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#ifndef KRB5_AUTH_CONTEXT
#define KRB5_AUTH_CONTEXT

struct _krb5_auth_context {
    krb5_address      *	remote_addr;
    krb5_address      *	remote_port;
    krb5_address      *	local_addr;
    krb5_address      *	local_port;
    krb5_keyblock     * keyblock;
    krb5_keyblock     * local_subkey;
    krb5_keyblock     * remote_subkey;

    krb5_int32		auth_context_flags;
    krb5_int32		remote_seq_number;
    krb5_int32		local_seq_number;
    krb5_authenticator *authentp;		/* mk_req, rd_req, mk_rep, ...*/
    krb5_cksumtype	cksumtype;		/* mk_safe, ... */
    krb5_pointer	i_vector;		/* mk_priv, rd_priv only */
    krb5_rcache		rcache;
};


/* Internal auth_context_flags */
#define KRB5_AUTH_CONN_INITIALIZED	0x00010000
#define KRB5_AUTH_CONN_USED_W_MK_REQ	0x00020000
#define KRB5_AUTH_CONN_USED_W_RD_REQ	0x00040000

#endif
