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
 * $Log: setup_mkey.c,v $
 * Revision 1.1.2.1  1996/06/05  20:32:27  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:36:25  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/kdb/setup_mkey.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * krb5_kdb_setup_mkey()
 */

#include "k5-int.h"

/*
 * Given a key name and a realm name, construct a principal which can be used
 * to fetch the master key from the database.
 * 
 * If the key name is NULL, the default key name will be used.
 */

#define	REALM_SEP_STRING	"@"

krb5_error_code
krb5_db_setup_mkey_name(context, keyname, realm, fullname, principal)
    krb5_context context;
    const char *keyname;
    const char *realm;
    char **fullname;
    krb5_principal *principal;
{
    krb5_error_code retval;
    int keylen;
    int rlen = strlen(realm);
    char *fname;
    
    if (!keyname)
	keyname = KRB5_KDB_M_NAME;	/* XXX external? */

    keylen = strlen(keyname);
	 
    fname = malloc(keylen+rlen+2);
    if (!fname)
	return ENOMEM;

    strcpy(fname, keyname);
    strcat(fname, REALM_SEP_STRING);
    strcat(fname, realm);

    if (retval = krb5_parse_name(context, fname, principal))
	return retval;
    if (fullname)
	*fullname = fname;
    else
	free(fname);
    return 0;
}
