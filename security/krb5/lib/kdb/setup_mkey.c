/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: setup_mkey.c,v $
 * Revision 1.1.4.2  1996/02/18  00:12:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:54:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:11  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:50:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:10:09  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:51  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  setup_mkey.c V=2 10/24/91 //littl/prgy/krb5/lib/kdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/kdb/setup_mkey.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * krb5_kdb_setup_mkey()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_setup_mkey_c[] =
"$Id: setup_mkey.c,v 1.1.4.2 1996/02/18 00:12:25 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/ext-proto.h>

/*
 * Given a key name and a realm name, construct a principal which can be used
 * to fetch the master key from the database.
 * 
 * If the key name is NULL, the default key name will be used.
 */

#define	REALM_SEP_STRING	"@"

krb5_error_code
krb5_db_setup_mkey_name(keyname, realm, fullname, principal)
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

    if (retval = krb5_parse_name(fname, principal))
	return retval;
    if (fullname)
	*fullname = fname;
    else
	free(fname);
    return 0;
}
