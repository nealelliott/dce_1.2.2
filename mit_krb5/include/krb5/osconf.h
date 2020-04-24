/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: osconf.h,v $
 * Revision 1.1.2.1  1996/06/04  21:50:23  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:33 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/03  20:01 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
 *
 * 	Add OSF/HP copyrights & RCS header.
 * 	[1996/05/03  20:01 UTC  mullan_s  /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2]
 *
 * $EndLog$
 */
/*
 * include/krb5/stock/osconf.h
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
 * Site- and OS- dependant configuration.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#ifndef KRB5_OSCONF__
#define KRB5_OSCONF__

#ifndef KRB5_AUTOCONF__
#define KRB5_AUTOCONF__
#include "autoconf.h"
#endif

#define	USE_DBM_LNAME		/* Use a DBM database for the aname/lname
				   conversion */

#ifdef KRB5_ATHENA_COMPAT
/*
 * Let's store krb5.conf is the same place whether or not we're
 * compiling under with Project Athena options turned on.  Things are
 * much more sane that way!!!!  --- Ted
 */
#define DEFAULT_PROFILE_FILENAME "/etc/krb5.conf"
#define	DEFAULT_CONFIG_FILENAME	"/etc/athena/krb.conf.v5"
#define	DEFAULT_TRANS_FILENAME	"/etc/athena/krb.realms"
#define	DEFAULT_LNAME_FILENAME	"/etc/athena/aname"
#define	DEFAULT_KEYTAB_NAME	"FILE:/etc/athena/v5srvtab"
#elif defined(_WINDOWS)
#define DEFAULT_PROFILE_FILENAME "krb5.ini"
#define	DEFAULT_CONFIG_FILENAME	"krb.conf"
#define	DEFAULT_TRANS_FILENAME	"krb.realms"
#define	DEFAULT_LNAME_FILENAME	"/aname"
#define	DEFAULT_KEYTAB_NAME	"FILE:/v5srvtab"
#else
#define DEFAULT_PROFILE_FILENAME "/etc/krb5.conf"
#define	DEFAULT_CONFIG_FILENAME	"/krb5/krb.conf"
#define	DEFAULT_TRANS_FILENAME	"/krb5/krb.realms"
#define	DEFAULT_LNAME_FILENAME	"/krb5/aname"
#ifdef OSF_DCE
#define	DEFAULT_KEYTAB_NAME	"FILE:/krb5/v5srvtab"
#else
#define	DEFAULT_KEYTAB_NAME	"FILE:/etc/v5srvtab"
#endif
#endif

#define DEFAULT_KDB_FILE        "/krb5/principal"
#define DEFAULT_ADMIN_ACL	"/krb5/admin_acl_file"

#define	DEFAULT_KDC_ETYPE	ETYPE_DES_CBC_CRC
#define	DEFAULT_KDC_KEYTYPE	KEYTYPE_DES
#define KDCRCACHE		"dfl:krb5kdc_rcache"

/* have the portname match the DCE version, */
/* also  since K4 still uses kerberos we need to have it different */
#define KDC_PORTNAME          "kerberos5" /* for /etc/services or equiv. */
#define KDC_SECONDARY_PORTNAME	"kerberos-sec" /* For backwards */
					    /* compatibility with */
					    /* port 750 clients */

#define KRB5_DEFAULT_PORT	88
#define KRB5_DEFAULT_SEC_PORT	750

#define MAX_DGRAM_SIZE	4096
#define MAX_SKDC_TIMEOUT 30
#define SKDC_TIMEOUT_SHIFT 2		/* left shift of timeout for backoff */
#define SKDC_TIMEOUT_1 1		/* seconds for first timeout */

#define RCTMPDIR	"/usr/tmp"	/* directory to store replay caches */

#define KRB5_PATH_TTY	"/dev/tty"
#define KRB5_PATH_LOGIN	"/krb5/sbin/login.krb5"
#define KRB5_PATH_RLOGIN "/krb5/bin/rlogin"

#define KRB5_ENV_CCNAME	"KRB5CCNAME"

/*
 * krb4 kadmin stuff follows
 */

/* the default syslog file */
#define KADM_SYSLOG  "/krb5/admin_server.syslog"

/* where to find the bad password table */
#define PW_CHECK_FILE "/krb5/bad_passwd"

#define DEFAULT_ACL_DIR	"/krb5"
#endif /* KRB5_OSCONF__ */
