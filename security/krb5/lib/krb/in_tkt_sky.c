/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: in_tkt_sky.c,v $
 * Revision 1.1.11.1  1996/10/03  15:03:09  arvind
 * 	Merge in code from beta6
 * 	[1996/09/16  21:14 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.8.2  1996/02/18  00:14:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:44  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  17:59:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/12  12:47 UTC  mdf
 * 	Removal of extra free, CHFts15353
 * 	[1995/06/09  15:17 UTC  mdf  /main/mdf_mothra_defects_6/1]
 * 
 * Revision 1.1.4.1  1994/05/11  19:23:02  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:20:38  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  15:56:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:32  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/in_tkt_sky.c,v $
 * $Author: arvind $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
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

#if !defined(lint) && !defined(SABER)
static char rcsid_in_tkt_skey_c [] =
"$Id: in_tkt_sky.c,v 1.1.11.1 1996/10/03 15:03:09 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <krb5/ext-proto.h>

#if 0
struct skey_keyproc_arg {
    const krb5_keyblock *key;
    krb5_principal client;		/* it's a pointer, really! */
};

/*
 * Key-generator for in_tkt_skey, below.
 * "keyseed" is actually a krb5_keyblock *, or NULL if we should fetch
 * from system area.
 */
static krb5_error_code
skey_keyproc(krb5_context ctxt,
	     krb5_const krb5_enctype type,
	     krb5_data *salt,
	     krb5_const_pointer keyseed,
	     krb5_keyblock **key)
{
    krb5_keyblock *realkey;
    const struct skey_keyproc_arg *arg;
    krb5_error_code retval;
    krb5_keytab kt_id;
    krb5_keytab_entry kt_ent;

    arg = (const struct skey_keyproc_arg *)keyseed;

    if (!valid_keytype(type))
	return KRB5_PROG_ETYPE_NOSUPP;

    if (arg->client) {
	/* do keytab stuff */
	/* else we need to fetch from system key place */
	if (retval = krb5_kt_default(&kt_id))
	    return retval;
	if (retval = krb5_kt_get_entry(kt_id, arg->client,
				       0, /* don't have vno available */
				       &kt_ent))
	    return retval;
    }
#define cleanup() {if (arg->client) (void) krb5_kt_free_entry(&kt_ent);}

    if (arg->key)
	retval = krb5_copy_keyblock(arg->key, &realkey);
    else
	retval = krb5_copy_keyblock(&kt_ent.key, &realkey);
    if (retval) {
	cleanup();
	return retval;
    }
	
    if (realkey->keytype != type) {
	krb5_free_keyblock(realkey);
	cleanup();
	return KRB5_PROG_ETYPE_NOSUPP;
    }	

    *key = realkey;
    cleanup();
    return 0;
}

/*
 Similar to krb5_get_in_tkt_with_password.

 Attempts to get an initial ticket for creds->client to use server
 creds->server, (realm is taken from creds->client), with options
 options, requesting encryption type etype, and using
 creds->times.starttime,  creds->times.endtime,  creds->times.renew_till
 as from, till, and rtime.  creds->times.renew_till is ignored unless
 the RENEWABLE option is requested.

 If addrs is non-NULL, it is used for the addresses requested.  If it is
 null, the system standard addresses are used.

 If keyblock is NULL, an appropriate key for creds->client is retrieved
 from the system key store (e.g. /etc/srvtab).  If keyblock is non-NULL,
 it is used as the decryption key.

 A succesful call will place the ticket in the credentials cache ccache.

 returns system errors, encryption errors

 */
krb5_error_code
krb5_get_in_tkt_with_skey(DECLARG(const krb5_flags, options),
			  DECLARG(krb5_address * const *, addrs),
			  DECLARG(const krb5_preauthtype, pre_auth_type),
			  DECLARG(const krb5_enctype, etype),
			  DECLARG(const krb5_keyblock *,key),
			  DECLARG(krb5_ccache, ccache),
			  DECLARG(krb5_creds *,creds),
			  DECLARG(krb5_kdc_rep **, ret_as_reply))
OLDDECLARG(const krb5_flags, options)
OLDDECLARG(krb5_address * const *, addrs)
OLDDECLARG(const krb5_preauthtype, pre_auth_type)
OLDDECLARG(const krb5_enctype, etype)
OLDDECLARG(const krb5_keyblock *,key)
OLDDECLARG(krb5_ccache, ccache)
OLDDECLARG(krb5_creds *, creds)
OLDDECLARG(krb5_kdc_rep **, ret_as_reply)

{
    struct skey_keyproc_arg arg;
    krb5_keytype keytype;

    if (key) {
	arg.key = key;
	arg.client = 0;
	keytype = key->keytype;
    } else {
	arg.key = 0;
	arg.client = creds->client;
	if (!valid_etype(etype))
	    return(KRB5_PROG_ETYPE_NOSUPP);

	keytype = krb5_csarray[etype]->system->proto_keytype;
    }
    return (krb5_get_in_tkt(options, addrs, pre_auth_type, etype, keytype,
			    skey_keyproc, (krb5_pointer) &arg,
			    krb5_kdc_rep_decrypt_proc, 0, creds,
			    ccache, ret_as_reply));
}
#endif

struct skey_keyproc_arg {
    const krb5_keyblock *key;
    krb5_principal client;		/* it's a pointer, really! */
};

/*
 * Key-generator for in_tkt_skey, below.
 * "keyseed" is actually a krb5_keyblock *, or NULL if we should fetch
 * from system area.
 */
krb5_error_code
skey_keyproc(krb5_context context, const krb5_enctype type,
	     krb5_data *salt,
	     krb5_const_pointer keyseed,
	     krb5_keyblock **key)
{
    krb5_keyblock *realkey;
    krb5_error_code retval;
    const krb5_keyblock * keyblock;

    keyblock = (const krb5_keyblock *)keyseed;

    if (!valid_enctype(type))
	return KRB5_PROG_ETYPE_NOSUPP;

    if ((retval = krb5_copy_keyblock(/*context, */keyblock, &realkey)))
	return retval;
	
    if (realkey->keytype != type) {
	krb5_free_keyblock(/*context, */realkey);
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
krb5_get_in_tkt_with_skey(context, options, addrs, ktypes, pre_auth_types, 
			  key, ccache, creds, ret_as_reply)
    krb5_context context;
    const krb5_flags options;
    krb5_address * const * addrs;
    krb5_enctype * ktypes;
    krb5_preauthtype * pre_auth_types;
    const krb5_keyblock * key;
    krb5_ccache ccache;
    krb5_creds * creds;
    krb5_kdc_rep ** ret_as_reply;

{
    if (key) 
    	return krb5_get_in_tkt(context, options, addrs, ktypes, pre_auth_types, 
			       skey_keyproc, (krb5_pointer)key,
			       krb5_kdc_rep_decrypt_proc, 0, creds,
			       ccache, ret_as_reply);
    else 
	return krb5_get_in_tkt_with_keytab(context, options, addrs, ktypes,
					   pre_auth_types, NULL, ccache,
					   creds, ret_as_reply);
}
