/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: store_mkey.c,v $
 * Revision 1.1.4.2  1996/02/18  00:12:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:54:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:12  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:50:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:10:14  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  store_mkey.c V=2 10/24/91 //littl/prgy/krb5/lib/kdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/kdb/store_mkey.c,v $
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
 * krb5_db_store_mkey():
 * Store a database master key in a file.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_store_mkey_c[] =
"$Id: store_mkey.c,v 1.1.4.2 1996/02/18 00:12:26 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/sysincl.h>		/* for MAXPATHLEN */
#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/los-proto.h>
#include <krb5/ext-proto.h>
#include "kdbint.h"

/*
 * Put the KDC database master key into a file.  If keyfile is NULL,
 * then a default name derived from the principal name mname is used.
 */

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

krb5_error_code
krb5_db_store_mkey(keyfile, mname, key)
char *keyfile;
krb5_principal mname;
krb5_keyblock *key;
{
    FILE *kf;
    krb5_error_code retval = 0;
    char defkeyfile[MAXPATHLEN+1];
    krb5_data *realm = krb5_princ_realm(mname);
#if defined(unix) || defined(__unix__)
    int oumask;
#endif

    if (!keyfile) {
	(void) strcpy(defkeyfile, DEFAULT_KEYFILE_STUB);
	(void) strncat(defkeyfile, realm->data,
		       min(sizeof(defkeyfile)-sizeof(DEFAULT_KEYFILE_STUB)-1,
			   realm->length));
	(void) strcat(defkeyfile, "");
	keyfile = defkeyfile;
    }

#if defined(unix) || defined(__unix__)
    oumask = umask(077);
#endif
#if defined(__STDC__)
    if (!(kf = fopen(keyfile, "wb")))
#else
    if (!(kf = fopen(keyfile, "w")))
#endif
    {
#if defined(unix) || defined(__unix__)
	(void) umask(oumask);
#endif
	return errno;
    }
    if ((fwrite((krb5_pointer) &key->keytype,
		sizeof(key->keytype), 1, kf) != 1) ||
	(fwrite((krb5_pointer) &key->length,
		sizeof(key->length), 1, kf) != 1) ||
	(fwrite((krb5_pointer) key->contents,
		sizeof(key->contents[0]), key->length, kf) != key->length)) {
	retval = errno;
	(void) fclose(kf);
    }
    if (fclose(kf) == EOF)
	retval = errno;
#if defined(unix) || defined(__unix__)
    (void) umask(oumask);
#endif
    return retval;
}
