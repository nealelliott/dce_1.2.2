/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hst_realm.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:35  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:30:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  hst_realm.c V=3 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/hst_realm.c,v $
 * $Author: marty $
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
 * krb5_get_host_realm()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_hst_realm_c[] =
"$Id: hst_realm.c,v 1.1.4.2 1996/02/18 00:15:23 marty Exp $";
#endif	/* !lint & !SABER */

/*
 Figures out the Kerberos realm names for host, filling in a
 pointer to an argv[] style list of names, terminated with a null pointer.
 
 If host is NULL, the local host's realms are determined.

 If there are no known realms for the host, the filled-in pointer is set
 to NULL.

 The pointer array and strings pointed to are all in allocated storage,
 and should be freed by the caller when finished.

 returns system errors
*/

/*
 * Implementation notes:
 *
 * this implementation only provides one realm per host, using the same
 * mapping file used in kerberos v4.

 * Given a fully-qualified domain-style primary host name,
 * return the name of the Kerberos realm for the host.
 * If the hostname contains no discernable domain, or an error occurs,
 * return the local realm name, as supplied by krb5_get_default_realm().
 * If the hostname contains a domain, but no translation is found,
 * the hostname's domain is converted to upper-case and returned.
 *
 * The format of each line of the translation file is:
 * domain_name kerberos_realm
 * -or-
 * host_name kerberos_realm
 *
 * domain_name should be of the form .XXX.YYY (e.g. .LCS.MIT.EDU)
 * host names should be in the usual form (e.g. FOO.BAR.BAZ)
 */


#include <krb5/sysincl.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <ctype.h>

#include <stdio.h>
#include <krb5/ext-proto.h>

/* for old Unixes and friends ... */
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN 64
#endif

#define DEF_REALMNAME_SIZE	256

extern char *krb5_trans_file;

krb5_error_code
krb5_get_host_realm(host, realmsp)
const char *host;
char ***realmsp;
{
    char **retrealms;
    char *domain;
    FILE *trans_file;
    char trans_host[MAXHOSTNAMELEN+1];
    char local_host[MAXHOSTNAMELEN+1];
    char trans_realm[DEF_REALMNAME_SIZE];
    krb5_error_code retval;
    int scanval;
    char scanstring[7+2*16];		/* 7 chars + 16 for each decimal
					   conversion */

    if (!(retrealms = (char **)calloc(2, sizeof(*retrealms))))
	return ENOMEM;
    if (!host) {
	if (gethostname(local_host, sizeof(local_host)-1) == -1)
	    return errno;
	local_host[sizeof(local_host)-1] = '\0';
	host = local_host;
    }
    domain = strchr(host, '.');

    /* prepare default */
    if (domain) {
	char *cp;

	if (!(retrealms[0] = malloc(strlen(&domain[1])+1))) {
	    xfree(retrealms);
	    return ENOMEM;
	}
	strcpy(retrealms[0], &domain[1]);
	/* Upper-case realm */
	for (cp = retrealms[0]; *cp; cp++)
	    if (islower(*cp))
		*cp = toupper(*cp);
    } else {
	if (retval = krb5_get_default_realm(&retrealms[0])) {
	    xfree(retrealms);
	    return retval;
	}
    }

    if ((trans_file = fopen(krb5_trans_file, "r")) == (FILE *) 0) {
	xfree(retrealms[0]);
	xfree(retrealms);
	return KRB5_TRANS_CANTOPEN;
    }
    (void) sprintf(scanstring, "%%%ds %%%ds",
		   sizeof(trans_host)-1,sizeof(trans_realm)-1);
    while (1) {
	if ((scanval = fscanf(trans_file, scanstring,
			      trans_host, trans_realm)) != 2) {
	    if (scanval == EOF) {
		fclose(trans_file);
		goto out;
	    }
	    continue;			/* ignore broken lines */
	}
	trans_host[sizeof(trans_host)-1] = '\0';
	trans_realm[sizeof(trans_realm)-1] = '\0';
	if (!strcasecmp(trans_host, host)) {
	    /* exact match of hostname, so return the realm */
	    if (!(retrealms[0] = realloc(retrealms[0],
					 strlen(trans_realm)+1))) {
		xfree(retrealms);
		return ENOMEM;
	    }
	    (void) strcpy(retrealms[0], trans_realm);
	    fclose(trans_file);
	    goto out;
	}
	if ((trans_host[0] == '.') && domain) { 
	    /* this is a possible domain match */
	    if (!strcasecmp(trans_host, domain)) {
		/* domain match, save for later */
		if (!(retrealms[0] = realloc(retrealms[0],
					     strlen(trans_realm)+1))) {
		    xfree(retrealms);
		    return ENOMEM;
		}
		(void) strcpy(retrealms[0], trans_realm);
		continue;
	    }
	}
    }
 out:
    *realmsp = retrealms;
    return 0;
}
