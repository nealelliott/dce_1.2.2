/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: kinit.c,v $
 * Revision 1.1.6.1  1996/10/02  17:49:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:06  damon]
 *
 * Revision 1.1.2.1  1994/07/26  19:12:53  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:37  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:08:07  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfs_login/kinit.c,v 1.1.6.1 1996/10/02 17:49:35 damon Exp $ */
/*
 * kinit.c - Initialize a credential cache.
 *
 * A credential cache is created and pre-populated with a TGT.
 *
 * Most of this code was stolen from the MIT kinit, we have just added
 * the ability to specify alternate hosts from which the TGT can
 * be used.
 */
#include <stdio.h>
#include <com_err.h>
#include <pwd.h>
#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <netdb.h>
#include <errno.h>
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include "kinit.h"

/*
 * Local data.
 */
static krb5_data tgtname = {
    0,
#ifdef KRB5_TGS_NAME
    KRB5_TGS_NAME_SIZE,
    KRB5_TGS_NAME
#else
    sizeof(TGTNAME)-1,
    TGTNAME
#endif
};

/*
 * External data.
 */
extern char *progname;

/*
 * kinit - Initialize a credential cache.
 *
 * Inputs:
 *	pname		: Kerberos pricipal name
 *      opassword 	: (Optional) password
 *      lifetime        : Lifetime in seconds (0 for default).
 *	...		: List of alternate hosts to use, null terminated.
 * Return value:
 *	-1 on failure, 0 otherwise.  Prints errors to stderr.
 */
int kinit
#ifdef __STDC__
	(char *pname, char *opassword, time_t lifetime, ...)
#else
	(pname, opassword, lifetime, va_alist)
	char *pname;
	char *opassword;
	time_t lifetime;
	va_dcl
#endif
{
	krb5_ccache ccache;		/* The cred cache */
	krb5_principal princ = NULL;	/* This principal */
	krb5_principal server = NULL;	/* Our kdc server */
	krb5_address **os_addrs = NULL;	/* OS addr vector */
	krb5_address **all_addrs = NULL;/* Final addr vector */
	krb5_address **addr;		/* Address pointer */
	krb5_address *thisaddr;		/* Address to process */
	krb5_timestamp now;		/* Current time */
	int code;			/* Krb5 error codes */
	krb5_creds creds;		/* Our credentials */
	char *host;			/* Alternalte host name */
	va_list ap;			/* For traversing list */
	int nhosts;			/* Total number of hosts */
	struct hostent *hp;		/* Host entry pointer */
	char password[256], prompt[256];/* Storage for pwd and prompt */
	int pwsize;			/* Size of password */
	extern char *krb5_kdc_udp_portname;

	krb5_kdc_udp_portname = "kerberos5";

	/*
	 * Build ccache, principal name, and server name.
 	 */
	if (code = krb5_cc_default(&ccache)) {
		com_err(progname, code, "while getting cred cache");
		return(-1);
	}
	if (code = krb5_parse_name(pname, &princ)) {
		com_err(progname, code, "while parsing principal");
		return(-1);
	}
	if (code = krb5_cc_initialize(ccache, princ)) {
		com_err(progname, code, "while initializing ccache");
		goto out;
	}
	memset((char *)&creds, 0, sizeof(creds));
	creds.client = princ;
	if (code = krb5_build_principal_ext(&server,
					    krb5_princ_realm(princ)->length,
					    krb5_princ_realm(princ)->data,
					    tgtname.length, tgtname.data,
					    krb5_princ_realm(princ)->length,
					    krb5_princ_realm(princ)->data,
					    0)) {
		com_err(progname, code, "while building server name");
		goto out;
	}
 	creds.server = server;
	/*
	 * Now build address vector.  First get the standard ones.
	 */
	if (code = krb5_os_localaddr(&os_addrs)) {
		com_err(progname, code, "while getting local OS address");
		goto out;
	}
	/*
	 * See how many host names we have.
	 */
#ifdef __STDC__
	va_start(ap, pname);
#else
	va_start(ap);
#endif
	host = va_arg(ap, char *);
	for (nhosts = 0; host; nhosts++)
		host = va_arg(ap, char *);
	for (addr = os_addrs; *addr; addr++, nhosts++);
	/*
	 * Allocate space for addresses.
	 */
	if ((all_addrs = (krb5_address **)malloc((nhosts + 1) * sizeof(krb5_address *))) == NULL) {
		com_err(progname, ENOMEM, "while allocating address vector");
		goto out;
	}
	all_addrs += nhosts;
	*all_addrs = NULL;
	/*
	 * Copy over addresses.  OS vector first, then do lookups to get
	 * addresses of alternate hosts.
	 */
	for (addr = os_addrs; *addr; addr++) {
		*--all_addrs = *addr;
		*addr = NULL;
	}
#ifdef __STDC__
	va_start(ap, pname);
#else
	va_start(ap);
#endif
	host = va_arg(ap, char *);
	while (host) {
		if ((thisaddr = (krb5_address *)malloc(sizeof(krb5_address))) == NULL) {
			com_err(progname, ENOMEM, "while allocating individual vector");
			goto out;
		}
		if ((hp = gethostbyname(host)) == NULL) {
			com_err(progname, errno, "while resolving addr for %s", host);
			goto out;
		}
		thisaddr->addrtype = hp->h_addrtype;
		thisaddr->length = hp->h_length;
		if ((thisaddr->contents = (krb5_octet *)malloc(hp->h_length)) == NULL) {
			com_err(progname, ENOMEM, "while allocating contents for addr vector");
			goto out;
		}
		memcpy(thisaddr->contents, hp->h_addr, hp->h_length);
		*--all_addrs = thisaddr;
		host = va_arg(ap, char *);
	}

	/*
	 * Fill in remaining necessary fields.
 	 */
	creds.times.starttime = 0;
	creds.times.renew_till = 0;
	if (code = krb5_timeofday(&now)) {
		com_err(progname, code, "while getting time of day");
		goto out;
	}
	if (lifetime == 0) {
		/* default infinity, modified by server KDC policy */
		creds.times.endtime = 0;
	} else {
		creds.times.endtime = now + lifetime;
	}
	if (opassword) {
		strcpy(password, opassword);
	} else {
		sprintf(prompt, "Password for %s: ", pname);
		pwsize = sizeof(password);
		/*
		 * Fetch actual password.
		 */
		if (code = krb5_read_password(prompt, 0, password, &pwsize)) {
			memset(password, 0, sizeof(password));
			com_err(progname, code, "while reading password");
			goto out;
		}
	}
	/*
	 * Finally talk to the kdc.
	 *					     
	 * TRANSARC NOTE: We do not have a version of krb5 that supports
	 * KRB5_PADATA_ENC_TIMESTAMP preauth type. Hence I am continuing to 
	 * use KRB5_PADATA_ENC_UNIX_TIME.
	 */
	code = krb5_get_in_tkt_with_password(0, all_addrs,
					     KRB5_PADATA_ENC_UNIX_TIME,      
#if 0
					     KRB5_PADATA_ENC_TIMESTAMP,
#endif
					     ETYPE_DES_CBC_CRC,
					     KEYTYPE_DES,
					     password, ccache,
					     &creds, 0);
	memset(password, 0, sizeof(password));
	if (opassword) {
		memset(opassword, 0, strlen(opassword));
	}
	if (code) {
		if (code == KRB5KRB_AP_ERR_BAD_INTEGRITY) {
	        	fprintf(stderr, "%s: Password incorrect\n", progname);
		} else {
	    		com_err(progname, code, "while getting initial credentials");
		}
    	}
out:
	if (princ)
		krb5_free_principal(princ);
	if (server)
		krb5_free_principal(server);
	if (all_addrs)
		krb5_free_addresses(all_addrs);
	if (os_addrs)
		krb5_free_addresses(os_addrs);
	if (code)
		return(-1);
	else
		return(0);
}

#ifdef PRE_KRB5B4
/*
 * krb5_free_addresses - Free address vector.
 *
 * Inputs:
 *	val		: Address vector, contents freed.
 *
 * Return value:
 *	None.
 */
void
krb5_free_addresses
#ifdef __STDC__
	(krb5_address **val)
#else
	(val)
	krb5_address **val;
#endif /* __STDC__ */
{
    register krb5_address **temp;

    for (temp = val; *temp; temp++) {
	if ((*temp)->contents)
	    xfree((*temp)->contents);
	xfree(*temp);
    }
    xfree(val);
    return;
}
#endif
