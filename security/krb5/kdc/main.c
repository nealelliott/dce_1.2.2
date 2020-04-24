/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: main.c,v $
 * Revision 1.1.15.3  1996/11/13  18:06:33  arvind
 * 	CHFts577
 * 	[1996/10/30  16:02 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * 	CHFts20577: Free nfmt in kdc_com_err_proc to fix MLK
 * 	[1996/10/14  19:02 UTC  sommerfeld  /main/DCE_1.2.2/2]
 *
 * Revision 1.1.15.2  1996/10/15  20:51:31  arvind
 * 	Plug a few trivial memory leaks at startup time.
 * 	[1996/10/11  00:02 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision 1.1.15.1  1996/10/03  15:00:36  arvind
 * 	silence compiler warning.
 * 	[1996/09/16  20:59 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.10.2  1996/02/18  00:08:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:16  marty]
 * 
 * Revision 1.1.10.1  1995/12/08  17:45:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:40  root]
 * 
 * Revision 1.1.7.1  1993/10/14  17:45:20  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:42:53  mccann]
 * 
 * Revision 1.1.5.4  1992/12/29  14:18:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:55:01  zeliff]
 * 
 * Revision 1.1.5.3  1992/10/23  02:29:01  sommerfeld
 * 	[OT4808] Delete LOG_CONS from openlog, since it seems to cause more
 * 	problems than it solves.
 * 	[1992/10/21  18:50:57  sommerfeld]
 * 
 * Revision 1.1.5.2  1992/09/29  21:19:21  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:33:32  sekhar]
 * 
 * Revision 1.1.2.2  1992/05/08  17:17:33  sommerfeld
 * 	Don't clobber retval from rc_recover call when calling rc_initialize.
 * 	[1992/05/05  02:42:05  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:54:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  main.c V=9 12/19/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/main.c,v $
 * $Author: arvind $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Main procedure body for the KDC server process.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_main_c[] =
"$Id: main.c,v 1.1.15.3 1996/11/13 18:06:33 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <stdio.h>
#include <syslog.h>
#include <signal.h>
#include <errno.h>

#include <com_err.h>
/* for STDC, com_err gets varargs/stdarg */
#ifndef __STDC__
#include <varargs.h>
#endif

#include <krb5/osconf.h>
#include <krb5/kdb.h>
#include <krb5/kdb_dbm.h>
#include <krb5/los-proto.h>
#include <krb5/ext-proto.h>

#include <krb5/config.h>
#ifdef PROVIDE_DES_CBC_CRC
#include <krb5/mit-des.h>
#endif

#include "kdc_util.h"
#include "extern.h"
#include "kdc5_err.h"

#ifdef OSF_DCE
#   include <rgymacro.h>
#   include <rs_task.h>
#endif


static void
kdc_com_err_proc
#ifndef __STDC__
    (whoami, code, format, pvar)
	const char *whoami;
	long code;
	const char *format;
	va_list pvar;
#else
  (
	const char *whoami,
	long code,
	const char *format,
	va_list pvar
  )
#endif
{
    /* XXX need some way to do this better... */

#ifndef __STDC__
    extern int vfprintf();
#endif

#if defined(OSF_DCE) && (defined(ultrix) || defined(__osf__))
    /* Ultrix integration must rely on bsd4.2 syslog instead
    ** of vsyslog function...
    */
#   define LOG_FN syslog
#else
#   define LOG_FN vsyslog
#endif

    if (whoami) {
        fputs(whoami, stderr);
        fputs(": ", stderr);
    }
    if (code) {
        fputs(error_message(code), stderr);
        fputs(" ", stderr);
    }
    if (format) {
        vfprintf (stderr, format, pvar);
    }
    putc('\n', stderr);
    /* should do this only on a tty in raw mode */
    putc('\r', stderr);
    fflush(stderr);
    if (format) {
	/* now need to frob the format a bit... */
	if (code) {
	    char *nfmt;
	    nfmt = malloc(strlen(format)+strlen(error_message(code))+2);
	    strcpy(nfmt, error_message(code));
	    strcat(nfmt, " ");
	    strcat(nfmt, format);
	    LOG_FN(LOG_ERR, nfmt, pvar);
            /* CHFts20577: memory leak */
            free(nfmt);
	} else
	    LOG_FN(LOG_ERR, format, pvar);
    } else {
	if (code)
	    syslog(LOG_ERR, "%s", error_message(code));
    }

    return;
}

void
setup_com_err(
#ifdef __STDC__
    void
#endif
)
{
#ifndef OSF_DCE
    initialize_krb5_error_table();
    initialize_kdb5_error_table();
    initialize_kdc5_error_table();
    initialize_isod_error_table();
#endif

    (void) set_com_err_hook(kdc_com_err_proc);
    return;
}

krb5_sigtype
request_exit(
#ifdef __STDC__
    void
#endif
)
{
    signal_requests_exit = 1;

    return;
}

#ifndef OSF_DCE
/* Don't setup signal handlers in pthread environment when the server is
 * a thread in the DCE security daemon.
 */
void
setup_signal_handlers()
{
    signal(SIGINT, request_exit);
    signal(SIGHUP, request_exit);
    signal(SIGTERM, request_exit);

    return;
}
#endif /* OSF_DCE */

void
usage
#ifndef __STDC__
    (name)
    char *name;
#else
  (
    char *name
  )
#endif
{
    fprintf(stderr, "usage: %s [-d dbpathname] [-r dbrealmname] [-R replaycachename ]\n\t[-m] [-k masterkeytype] [-M masterkeyname]\n", name);
    return;
}

void
process_args
#ifndef __STDC__
    (argc, argv)
    int argc;
    char **argv;
#else
  (
    int argc,
    char **argv
  )
#endif
{
    int c;
    krb5_boolean manual = FALSE;
    int keytypedone = 0;
    char *db_realm = NULL;
    char *mkey_name = NULL;
    char *rcname = NULL;
    char *lrealm = NULL;
    krb5_error_code retval, retval2;
    krb5_enctype etype;
    extern krb5_deltat krb5_clockskew;

    extern char *optarg;

    while ((c = getopt(argc, argv, "r:d:mM:k:R:")) != EOF) {
	switch(c) {
	case 'r':			/* realm name for db */
	    db_realm = optarg;
	    break;
	case 'd':			/* pathname for db */
	    dbm_db_name = optarg;
	    break;
	case 'm':			/* manual type-in of master key */
	    manual = TRUE;
	    break;
	case 'M':			/* master key name in DB */
	    mkey_name = optarg;
	    break;
	case 'k':			/* keytype for master key */
	    master_keyblock.keytype = atoi(optarg);
	    keytypedone++;
	    break;
	case 'R':
	    rcname = optarg;
	    break;
	case '?':
	default:
	    usage(argv[0]);
	    exit(1);
	}
    }
    if (!db_realm) {
	/* no realm specified, use default realm */
	if (retval = krb5_get_default_realm(&lrealm)) {
	    com_err(argv[0], retval,
		    "while attempting to retrieve default realm");
	    exit(1);
	}
	db_realm = lrealm;
    }

    if (!mkey_name)
	mkey_name = KRB5_KDB_M_NAME;

    if (!keytypedone)
	master_keyblock.keytype = KEYTYPE_DES;

    if (!rcname)
	rcname = KDCRCACHE;
    if (retval = krb5_rc_resolve_full(&kdc_rcache, rcname)) {
	com_err(argv[0], retval, "while resolving replay cache '%s'", rcname);
	exit(1);
    }
    if ((retval = krb5_rc_recover(kdc_rcache)) &&
	(retval2 = krb5_rc_initialize(kdc_rcache, krb5_clockskew))) {
	com_err(argv[0], retval, "while recovering replay cache '%s:%s'",
		kdc_rcache->ops->type,
		krb5_rc_get_name(kdc_rcache));
	com_err(argv[0], retval2, "while initializing replay cache '%s:%s'",
		kdc_rcache->ops->type,
		krb5_rc_get_name(kdc_rcache));
	exit(1);
    }
    if ((retval = krb5_rc_expunge(kdc_rcache))) {
	com_err(argv[0], retval, "while expunging replay cache '%s:%s'",
		kdc_rcache->ops->type,
		krb5_rc_get_name(kdc_rcache));
	exit(1);
    }
    /* assemble & parse the master key name */

    if (retval = krb5_db_setup_mkey_name(mkey_name, db_realm, (char **) 0,
					 &master_princ)) {
	com_err(argv[0], retval, "while setting up master key name");
	(void) krb5_rc_close(kdc_rcache);
	exit(1);
    }
    if (lrealm != NULL) {
	free(lrealm);
	lrealm = NULL;
    }
#ifdef PROVIDE_DES_CBC_CRC
    master_encblock.crypto_entry = &mit_des_cryptosystem_entry;
#else
error(You gotta figure out what cryptosystem to use in the KDC);
#endif

#ifdef NOTDEF_DCE_RGYD_MASTER_KEY
    if (retval = krb5_db_fetch_mkey(master_princ, &master_encblock, manual,
				    FALSE, /* only read it once, if at all */
				    &master_keyblock)) {
	com_err(argv[0], retval, "while fetching master key");
	(void) krb5_rc_close(kdc_rcache);
	exit(1);
    }
#else
    master_keyblock.keytype = KEYTYPE_DES;
    master_keyblock.length = 8;
    master_keyblock.contents = (krb5_octet *) malloc(master_keyblock.length);
    bzero(master_keyblock.contents, master_keyblock.length);
#endif

    /* initialize random key generators */
    for (etype = 0; (int) etype <= krb5_max_cryptosystem; etype++) {
	if (krb5_csarray[etype]) {
	    if (retval = (*krb5_csarray[etype]->system->
			  init_random_key)(&master_keyblock,
					   &krb5_csarray[etype]->random_sequence)) {
		com_err(argv[0], retval, "while setting up random key generator for etype %d--etype disabled", etype);
		krb5_csarray[etype] = 0;
	    }
	}
    }

    return;
}

void
finish_args
#ifndef __STDC__
    (prog)
    char *prog;
#else
  (
    char *prog
  )
#endif
{
    krb5_error_code retval;
    if (retval = krb5_rc_close(kdc_rcache)) {
	com_err(prog, retval, "while closing replay cache '%s:%s'",
		kdc_rcache->ops->type,
		krb5_rc_get_name(kdc_rcache));
    }
    return;
}


krb5_error_code
init_db
#ifndef __STDC__
    (dbname, masterkeyname, masterkeyblock)
    char *dbname;
    krb5_principal masterkeyname;
    krb5_keyblock *masterkeyblock;
#else
  (
    char *dbname,
    krb5_principal masterkeyname,
    krb5_keyblock *masterkeyblock
  )
#endif
{
    krb5_error_code retval;
    int nprincs;
    krb5_boolean more;
    krb5_db_entry server;
    char *principal;
    
    /* set db name if appropriate */
    if (dbname && (retval = krb5_db_set_name(dbname)))
	return(retval);

    /* initialize database */
    if (retval = krb5_db_init()) {
	com_err("krb5_db_init", retval, "while calling krb5_db_init");
	return(retval);
    }

#ifdef NOTDEF_DCE_RGYD_MASTER_KEY
    if (retval = krb5_db_verify_master_key(masterkeyname, masterkeyblock,
					   &master_encblock)) {
	master_encblock.crypto_entry = 0;
	return(retval);
    }

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(&master_encblock, masterkeyblock)) {
	master_encblock.crypto_entry = 0;
	(void) krb5_db_fini();
	return(retval);
    }
#endif

    /* fetch the TGS key, and hold onto it; this is an efficiency hack */
    /*XXX 
     * This efficiency hack is usless if the TGS key can be changed 
     * without invalidating all outstanding TGTs, which seems like a good 
     * idea (I think is true of the DCE version).  Also, when multiple
     * encryption/key types are possible, this will have to be a vector of 
     * of keys.  FOR NOW we assume only one keytype: DES
     */

    /* the master key name here is from the master_princ global,
       so we can safely share its substructure */

    tgs_server[0] = krb5_princ_realm(masterkeyname);
    /* tgs_server[1] is init data */
    tgs_server[2] = krb5_princ_realm(masterkeyname);
    /* tgs_server[3] is init data (0) */

    retval = krb5_unparse_name (tgs_server, &principal);
    if (retval) {
	com_err("krb5_unparse_name", retval, "while unparsing tgs server name");
	return retval;
    }
    

    nprincs = 1;
    if (retval = krb5_db_get_principal(tgs_server, KEYTYPE_DES, 0,
				       &server, &nprincs,
				       &more))
    {
	com_err("krb5_db_get_principal", retval, "while fetching %s", principal);
	free(principal);
	return(retval);
    }
    if (more) {
	(void) krb5_finish_key(&master_encblock);
	memset((char *)&master_encblock, 0, sizeof(master_encblock));
	(void) krb5_db_fini();
	retval = KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE;
	goto out;
    } else if (nprincs != 1) {
	(void) krb5_finish_key(&master_encblock);
	memset((char *)&master_encblock, 0, sizeof(master_encblock));
	(void) krb5_db_fini();
	retval = KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN;
	com_err("krb5_db_get_principal", retval, "while looking for %s; nprincs = %d", principal, nprincs);
	goto out;
    }
    /* convert server.key into a real key (it may be encrypted
       in the database) */
    if (retval = KDB_CONVERT_KEY_OUTOF_DB(&server.key, &tgs_key)) {
	(void) krb5_finish_key(&master_encblock);
	memset((char *)&master_encblock, 0, sizeof(master_encblock));
	(void) krb5_db_fini();
	goto out;
    }
    tgs_kvno = server.kvno;
  out:
    krb5_db_free_principal(&server, nprincs);
    free(principal);
    return retval;
}

krb5_error_code
closedown_db(
#ifdef __STDC__
    void
#endif
)
{
    krb5_error_code retval;

    /* clean up master key stuff */
    retval = krb5_finish_key(&master_encblock);

    memset((char *)&master_encblock, 0, sizeof(master_encblock));

    memset((char *)tgs_key.contents, 0, tgs_key.length);

    /* close database */
    if (retval) {
	(void) krb5_db_fini();
	return retval;
    } else
	return (krb5_db_fini());
}

/*
 outline:

 process args & setup

 initialize database access (fetch master key, open DB)

 initialize network

 loop:
 	listen for packet

	determine packet type, dispatch to handling routine
		(AS or TGS (or V4?))

	reflect response

	exit on signal

 clean up secrets, close db

 shut down network

 exit
 */

#ifndef OSF_DCE
/*
  * This void is a bit bogus, but it's necessary to prevent some compilers
  * from complaining about a no-value return path from a non-void
  * function. 
  */
void main (
    int argc,
    char *argv[]
  )

{
    kdc_init(argc, argv);
    kdc_run(argc, argv);
}
#endif

void kdc_init(argc, argv)
    int argc;
    char **argv;
{
    krb5_error_code retval;

    if (strrchr(argv[0], '/'))
	argv[0] = strrchr(argv[0], '/')+1;

    setup_com_err();

#if defined(OSF_DCE) && defined(ultrix)
    openlog(argv[0], LOG_PID);
#else
    openlog(argv[0], LOG_NDELAY|LOG_PID, LOG_LOCAL6); /* XXX */
#endif

    process_args(argc, argv);		/* includes reading master key */

#ifndef OSF_DCE
    /* Don't setup signal handlers in pthread environment when the server is
     * a thread in the DCE security daemon.
     */
     setup_signal_handlers();
#endif /* OSF_DCE */

    if (retval = init_db(dbm_db_name, master_princ, &master_keyblock)) {
	com_err(argv[0], retval, "while initializing database");
	finish_args(argv[0]);
	exit(1);
    }
}

void kdc_run(argc, argv)
    int argc;
    char **argv;
{
    krb5_error_code retval;
    int errout = 0;
#ifdef OSF_DCE
    unsigned32 sleep_time = 120;       /*  decaying sleep time */
#endif

#ifdef OSF_DCE
    syslog(LOG_INFO, "commencing operation");
    while (retval = setup_network(argv[0])) {
        syslog(LOG_INFO, "Unable to listen to MIT V5 protocol UDP port");
        rs_task_sleep_backoff(&sleep_time, 60 * 60);
    }
#else
    if (retval = setup_network(argv[0])) {
	com_err(argv[0], retval, "while initializing network");
	finish_args(argv[0]);
	exit(1);
    }
    syslog(LOG_INFO, "commencing operation");
#endif
    if (retval = listen_and_process(argv[0])){
	com_err(argv[0], retval, "while processing network requests");
	errout++;
    }
    if (retval = closedown_network(argv[0])) {
	com_err(argv[0], retval, "while shutting down network");
	errout++;
    }
    if (retval = closedown_db()) {
	com_err(argv[0], retval, "while closing database");
	errout++;
    }
    syslog(LOG_INFO, "shutting down");
    finish_args(argv[0]);
    exit(errout);
}

