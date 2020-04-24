/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: in_tkt_ktb.c,v $
 * Revision 1.1.2.1  1996/10/03  15:02:45  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  13:45:58  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:31 UTC  sommerfeld
 * 	Imported from MIT krb5 beta 6
 * 
 * $EndLog$
 */
/*
 * lib/krb5/krb/in_tkt_ktb.c
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
 * krb5_get_in_tkt_with_keytab()
 *	
 */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

#if 0
#include "k5-int.h"
#endif

struct keytab_keyproc_arg {
    krb5_keytab	keytab;
    krb5_principal client;
};

/*
 * Key-generator for in_tkt_keytab, below.
 * "keyseed" is actually a krb5_keytab, or NULL if we should fetch
 * from system area.
 */
krb5_error_code
keytab_keyproc (krb5_context context, const krb5_enctype type,
		krb5_data *salt, krb5_const_pointer keyseed,
		krb5_keyblock **key)
{
    struct keytab_keyproc_arg * arg = (struct keytab_keyproc_arg *)keyseed;
    krb5_keyblock *realkey;
    krb5_error_code retval = 0;
    krb5_keytab kt_id;
    krb5_keytab_entry kt_ent;

    kt_id = arg->keytab;

    if (!valid_enctype(type))
	return KRB5_PROG_ETYPE_NOSUPP;

    if (kt_id == NULL)
	/* Fetch from default keytab location */
	if ((retval = krb5_kt_default(/*context,*/ &kt_id)))
	    return retval;


    if ((retval = krb5_kt_get_entry(/*context,*/ kt_id, arg->client,
				    0, /* don't have vno available */
				    /*type,*/ &kt_ent)))
	    goto cleanup;

    if ((retval = krb5_copy_keyblock(/*context,*/ &kt_ent.key, &realkey))) {
	(void) krb5_kt_free_entry(/*context,*/ &kt_ent);
	goto cleanup;
    }

    (void) krb5_kt_free_entry(/*context,*/ &kt_ent);
    *key = realkey;
    
cleanup:
    if (! arg->keytab) 
	krb5_kt_close(/*context,*/ kt_id);
    return retval;
}

/*
 Similar to krb5_get_in_tkt_with_skey.

 Attempts to get an initial ticket for creds->client to use server
 creds->server, (realm is taken from creds->client), with options
 options, and using creds->times.starttime, creds->times.endtime, 
 creds->times.renew_till as from, till, and rtime. 
 creds->times.renew_till is ignored unless the RENEWABLE option is requested.

 If addrs is non-NULL, it is used for the addresses requested.  If it is
 null, the system standard addresses are used.

 A succesful call will place the ticket in the credentials cache ccache.

 returns system errors, encryption errors

 */
krb5_error_code
krb5_get_in_tkt_with_keytab(context, options, addrs, ktypes, pre_auth_types, 
			    keytab, ccache, creds, ret_as_reply)
    krb5_context context;
    const krb5_flags options;
    krb5_address * const * addrs;
    krb5_enctype * ktypes;
    krb5_preauthtype * pre_auth_types;
    const krb5_keytab keytab;
    krb5_ccache ccache;
    krb5_creds * creds;
    krb5_kdc_rep ** ret_as_reply;
{
    struct keytab_keyproc_arg arg;

    arg.keytab = keytab;
    arg.client = creds->client;

    return(krb5_get_in_tkt(context, options, addrs, ktypes,
			   pre_auth_types,
			   keytab_keyproc, (krb5_pointer)&arg,
			   krb5_kdc_rep_decrypt_proc, 0, creds,
			   ccache, ret_as_reply));
}
