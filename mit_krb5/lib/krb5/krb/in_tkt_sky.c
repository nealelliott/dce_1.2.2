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
 * $Log: in_tkt_sky.c,v $
 * Revision 1.1.2.1  1996/06/05  21:02:41  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:59:47  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/krb/in_tkt_sky.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * krb5_get_in_tkt_with_skey()
 *	
 */

#include "k5-int.h"

struct skey_keyproc_arg {
    const krb5_keyblock *key;
    krb5_principal client;		/* it's a pointer, really! */
};

/*
 * Key-generator for in_tkt_skey, below.
 * "keyseed" is actually a krb5_keyblock *, or NULL if we should fetch
 * from system area.
 */
krb5_error_code skey_keyproc
    PROTOTYPE((krb5_context,
               const krb5_keytype,
               krb5_data *,
               krb5_const_pointer,
               krb5_keyblock **));

krb5_error_code
skey_keyproc(context, type, salt, keyseed, key)
    krb5_context context;
    const krb5_keytype type;
    krb5_data * salt;
    krb5_const_pointer keyseed;
    krb5_keyblock ** key;
{
    krb5_keyblock *realkey;
    krb5_error_code retval;
    const krb5_keyblock * keyblock;

    keyblock = (const krb5_keyblock *)keyseed;

    if (!valid_keytype(type))
	return KRB5_PROG_ETYPE_NOSUPP;

    if (retval = krb5_copy_keyblock(context, keyblock, &realkey))
	return retval;
	
    if (realkey->keytype != type) {
	krb5_free_keyblock(context, realkey);
	return KRB5_PROG_ETYPE_NOSUPP;
    }	

    *key = realkey;
    return 0;
}

/*
 Similar to krb5_get_in_tkt_with_password.

 Attempts to get an initial ticket for creds->client to use server
 creds->server, (realm is taken from creds->client), with options
 options, and using creds->times.starttime, creds->times.endtime,
 creds->times.renew_till as from, till, and rtime.  
 creds->times.renew_till is ignored unless the RENEWABLE option is requested.

 If addrs is non-NULL, it is used for the addresses requested.  If it is
 null, the system standard addresses are used.

 If keyblock is NULL, an appropriate key for creds->client is retrieved
 from the system key store (e.g. /etc/srvtab).  If keyblock is non-NULL,
 it is used as the decryption key.

 A succesful call will place the ticket in the credentials cache ccache.

 returns system errors, encryption errors

 */
krb5_error_code
krb5_get_in_tkt_with_skey(context, options, addrs, etypes, pre_auth_types, 
			  key, ccache, creds, ret_as_reply)
    krb5_context context;
    const krb5_flags options;
    krb5_address * const * addrs;
    krb5_enctype * etypes;
    krb5_preauthtype * pre_auth_types;
    const krb5_keyblock * key;
    krb5_ccache ccache;
    krb5_creds * creds;
    krb5_kdc_rep ** ret_as_reply;

{
    if (key) 
    	return krb5_get_in_tkt(context, options, addrs, etypes, pre_auth_types, 
			       skey_keyproc, (krb5_pointer)key,
			       krb5_kdc_rep_decrypt_proc, 0, creds,
			       ccache, ret_as_reply);
    else 
	return krb5_get_in_tkt_with_keytab(context, options, addrs, etypes,
					   pre_auth_types, NULL, ccache,
					   creds, ret_as_reply);
}
