/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: in_tkt_pwd.c,v $
 * Revision 1.1.10.2  1996/11/13  18:08:04  arvind
 * 	CHFts20160: memory leak in pwd_keyproc
 * 	CHFts20151 (partial): rip out #if 0
 * 	[1996/10/16  23:23 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.10.1  1996/10/03  15:02:56  arvind
 * 	Merge in code from beta6
 * 	[1996/09/16  21:13 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.8.2  1996/02/18  00:14:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:42  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  17:59:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:53  root]
 * 
 * Revision 1.1.4.1  1994/05/11  19:22:48  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:20:14  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  15:56:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:29  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:50  devrcs
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/in_tkt_pwd.c,v $
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
 * krb5_get_in_tkt_with_password()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_in_tkt_pwd_c[] =
"$Id: in_tkt_pwd.c,v 1.1.10.2 1996/11/13 18:08:04 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>

extern char *krb5_default_pwd_prompt1;

/* 
 * key-producing procedure for use by krb5_get_in_tkt_with_password.
 */
krb5_error_code pwd_keyproc (krb5_context context,
			     const krb5_enctype type,
			     krb5_data *salt,
			     krb5_const_pointer keyseed,
			     krb5_keyblock **key)
{
    krb5_error_code retval;
    krb5_encrypt_block eblock;
    krb5_data * password;
    int pwsize;

    if (!valid_enctype(type))
	return KRB5_PROG_ETYPE_NOSUPP;

    krb5_use_enctype(/*context,*/ &eblock, type);
    
    password = (krb5_data *)keyseed;

    if (!password->length) {
	pwsize = BUFSIZ;
	if ((password->data = malloc(pwsize)) == NULL)
	    return ENOMEM;
	
	if ((retval = krb5_read_password(/*context, */krb5_default_pwd_prompt1,
					 NULL,
					 password->data, &pwsize))) {
	    free(password->data);
	    password->data = NULL;
	    return retval;
	}
	password->length = pwsize;
    }

    if (!(*key = (krb5_keyblock *)malloc(sizeof(**key))))
	return ENOMEM;

    if ((retval = krb5_string_to_key(/*context,*/ &eblock, type, *key, password, salt)))
	krb5_xfree(*key);
    return(retval);
}

/*
 Attempts to get an initial ticket for creds->client to use server
 creds->server, (realm is taken from creds->client), with options
 options, and using creds->times.starttime, creds->times.endtime,
 creds->times.renew_till as from, till, and rtime.  
 creds->times.renew_till is ignored unless the RENEWABLE option is requested.

 If addrs is non-NULL, it is used for the addresses requested.  If it is
 null, the system standard addresses are used.

 If password is non-NULL, it is converted using the cryptosystem entry
 point for a string conversion routine, seeded with the client's name.
 If password is passed as NULL, the password is read from the terminal,
 and then converted into a key.

 A succesful call will place the ticket in the credentials cache ccache.

 returns system errors, encryption errors
 */
krb5_error_code 
krb5_get_in_tkt_with_password(context, options, addrs, ktypes, pre_auth_types, 
			      password, ccache, creds, ret_as_reply)
    krb5_context context;
    const krb5_flags options;
    krb5_address * const * addrs;
    krb5_enctype * ktypes;
    krb5_preauthtype * pre_auth_types;
    const char * password;
    krb5_ccache ccache;
    krb5_creds * creds;
    krb5_kdc_rep ** ret_as_reply;
{
    krb5_error_code retval;
    krb5_data data;


    if ((data.data = (char *)password)) {
	data.length = strlen(password);
    } else {
	data.length = 0;
    }

    retval = krb5_get_in_tkt(context, options, addrs, ktypes, pre_auth_types, 
			     pwd_keyproc, (krb5_pointer) &data,
			     krb5_kdc_rep_decrypt_proc, 0,
			     creds, ccache, ret_as_reply);

    if ((password == NULL) && (data.data)) {
	memset(data.data, 0, strlen(data.data));
	free(data.data);
    }

    return retval;
}

