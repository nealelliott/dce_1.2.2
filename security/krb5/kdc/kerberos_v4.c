/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kerberos_v4.c,v $
 * Revision 1.1.5.2  1996/02/18  00:08:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:13  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:45:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:39  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:18:35  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  kerberos_v4.c V=2 10/24/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/kerberos_v4.c,v $
 * $Author: marty $
 *
 * Copyright 1985, 1986, 1987, 1988,1991 by the Massachusetts Institute
 * of Technology.
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
 */

#ifdef KRB4
#if !defined(lint) && !defined(SABER)
static char rcsid_kerberos_v4_c[] =
"$Id: kerberos_v4.c,v 1.1.5.2 1996/02/18 00:08:52 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/config.h>

#if __STDC__ || defined(STDARG_PROTOTYPES)
#include <stdarg.h>
#else
#include <varargs.h>
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sgtty.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/file.h>
#include <ctype.h>
#include <syslog.h>
#include <string.h>

/* v4 include files:
 */
#include <krb.h>
#include <des.h>
#include <klog.h>
#include <prot.h>
#include <krb_db.h>
#include <kdc.h>

extern int errno;

#ifdef notdef
static struct sockaddr_in sin = {AF_INET};
#endif
int     f;

/* XXX several files in libkdb know about this */
char *progname;

#ifndef BACKWARD_COMPAT
static Key_schedule master_key_schedule;
static C_Block master_key;
#endif

static struct timeval kerb_time;
static Principal a_name_data;	/* for requesting user */
static Principal s_name_data;	/* for services requested */
static C_Block session_key;
/*
static C_Block user_key;
static C_Block service_key;
*/
static u_char master_key_version;
/*
static char k_instance[INST_SZ];
*/
static char log_text[128];
static char *lt;
static int more;

#ifdef notdef
static int mflag;		/* Are we invoked manually? */
static int lflag;		/* Have we set an alterate log file? */
static char *log_file;	/* name of alt. log file */
static int nflag;		/* don't check max age */
static int rflag;		/* alternate realm specified */
#endif

/* fields within the received request packet */
static u_char req_msg_type;
static u_char req_version;
static char *req_name_ptr;
static char *req_inst_ptr;
static char *req_realm_ptr;
/*
static u_char req_no_req;
*/
static u_long req_time_ws;

int req_act_vno = KRB_PROT_VERSION; /* Temporary for version skew */

static char local_realm[REALM_SZ];

/* statistics */
#ifdef notdef
static long q_bytes;		/* current bytes remaining in queue */
static long q_n;		/* how many consecutive non-zero
				 * q_bytes   */
#endif
/*
static long max_q_bytes;
static long max_q_n; */
static long n_auth_req;
static long n_appl_req;
/* static long n_packets;
static long n_user;
static long n_server; */

extern char *sys_errlist[];
/* static long max_age = -1; */
static long pause_int = -1;

/* static void check_db_age(); */
static void hang();

#ifndef BACKWARD_COMPAT
/*
 * Print usage message and exit.
 */
static void usage()
{
    fprintf(stderr, "Usage: %s [-s] [-m] [-n] [-p pause_seconds]%s%s\n", progname, 
	    " [-a max_age] [-l log_file] [-r realm]"
	    ," [database_pathname]"
	    );
    exit(1);
}

main(argc, argv)
    int     argc;
    char  **argv;
{
    struct sockaddr_in from;
    register int n;
    int     on = 1;
    int     child;
    struct servent *sp;
    int     fromlen;
    static KTEXT_ST pkt_st;
    KTEXT   pkt = &pkt_st;
    Principal *p;
    int     more, kerror;
    C_Block key;
    int c;
    extern char *optarg;
    extern int optind;

    progname = argv[0];

    while ((c = getopt(argc, argv, "snmp:a:l:r:")) != EOF) {
	switch(c) {
	case 's':
	    /*
	     * Set parameters to slave server defaults.
	     */
	    if (max_age == -1 && !nflag)
		max_age = ONE_DAY;	/* 24 hours */
	    if (pause_int == -1)
		pause_int = FIVE_MINUTES; /* 5 minutes */
	    if (lflag == 0) {
		log_file = KRBSLAVELOG;
		lflag++;
	    }
	    break;
	case 'n':
	    max_age = -1;	/* don't check max age. */
	    nflag++;
	    break;
	case 'm':
	    mflag++;		/* running manually; prompt for master key */
	    break;
	case 'p':
	    /* Set pause interval. */
	    if (!isdigit(optarg[0]))
		usage();
	    pause_int = atoi(optarg);
	    if ((pause_int < 5) ||  (pause_int > ONE_HOUR)) {
		fprintf(stderr, "pause_int must be between 5 and 3600 seconds.\n");
		usage();
	    }
	    break;
	case 'a':
	    /* Set max age. */
	    if (!isdigit(optarg[0])) 
		usage();
	    max_age = atoi(optarg);
	    if ((max_age < ONE_HOUR) || (max_age > THREE_DAYS)) {
		fprintf(stderr, "max_age must be between one hour and three days, in seconds\n");
		usage();
	    }
	    break;
	case 'l':
	    /* Set alternate log file */
	    lflag++;
	    log_file = optarg;
	    break;
	case 'r':
	    /* Set realm name */
	    rflag++;
	    strcpy(local_realm, optarg);
	    break;
	default:
	    usage();
	    break;
	}
    }

    if (optind == (argc-1)) {
	if (kerb_db_set_name(argv[optind]) != 0) {
	    fprintf(stderr, "Could not set alternate database name\n");
	    exit(1);
	}
	optind++;
    }

    if (optind != argc)
	usage();
	
    printf("Kerberos server starting\n");
    
    if ((!nflag) && (max_age != -1))
	printf("\tMaximum database age: %d seconds\n", max_age);
    if (pause_int != -1)
	printf("\tSleep for %d seconds on error\n", pause_int);
    else
	printf("\tSleep forever on error\n");
    if (mflag)
	printf("\tMaster key will be entered manually\n");
    
    printf("\tLog file is %s\n", lflag ? log_file : KRBLOG);

    if (lflag)
	kset_logfile(log_file);
    
    /* find our hostname, and use it as the instance */
    if (gethostname(k_instance, INST_SZ)) {
	fprintf(stderr, "%s: gethostname error\n", progname);
	exit(1);
    }

    if ((sp = getservbyname("kerberos", "udp")) == 0) {
	fprintf(stderr, "%s: udp/kerberos unknown service\n", progname);
	exit(1);
    }
    sin.sin_port = sp->s_port;

    if ((f = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	fprintf(stderr, "%s: Can't open socket\n", progname);
	exit(1);
    }
    if (setsockopt(f, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	fprintf(stderr, "%s: setsockopt (SO_REUSEADDR)\n", progname);

    if (bind(f, &sin, S_AD_SZ, 0) < 0) {
	fprintf(stderr, "%s: Can't bind socket\n", progname);
	exit(1);
    }
    /* do all the database and cache inits */
    if (n = kerb_init()) {
	if (mflag) {
	    printf("Kerberos db and cache init ");
	    printf("failed = %d ...exiting\n", n);
	    exit(-1);
	} else {
	    klog(L_KRB_PERR,
	    "Kerberos db and cache init failed = %d ...exiting", n);
	    hang();
	}
    }

    /* Make sure database isn't stale */
    check_db_age();
    
    /* setup master key */
    if (kdb_get_master_key (mflag, master_key, master_key_schedule) != 0) {
      klog (L_KRB_PERR, "kerberos: couldn't get master key.\n");
      exit (-1);
    }
    kerror = kdb_verify_master_key (master_key, master_key_schedule, stdout);
    if (kerror < 0) {
      klog (L_KRB_PERR, "Can't verify master key.");
      bzero (master_key, sizeof (master_key));
      bzero (master_key_schedule, sizeof (master_key_schedule));
      exit (-1);
    }

    master_key_version = (u_char) kerror;

    fprintf(stdout, "\nCurrent Kerberos master key version is %d\n",
	    master_key_version);

    if (!rflag) {
	/* Look up our local realm */
	krb_get_lrealm(local_realm, 1);
    }
    fprintf(stdout, "Local realm: %s\n", local_realm);
    fflush(stdout);

    if (set_tgtkey(local_realm)) {
	/* Ticket granting service unknown */
	klog(L_KRB_PERR, "Ticket granting ticket service unknown");
	fprintf(stderr, "Ticket granting ticket service unknown\n");
	exit(1);
    }
    if (mflag) {
	if ((child = fork()) != 0) {
	    printf("Kerberos started, PID=%d\n", child);
	    exit(0);
	}
	setup_disc();
    }
    /* receive loop */
    for (;;) {
	fromlen = S_AD_SZ;
	n = recvfrom(f, pkt->dat, MAX_PKT_LEN, 0, &from, &fromlen);
	if (n > 0) {
	    pkt->length = n;
	    pkt->mbz = 0; /* force zeros to catch runaway strings */
	    /* see what is left in the input queue */
	    ioctl(f, FIONREAD, &q_bytes);
	    gettimeofday(&kerb_time, NULL);
	    q_n++;
	    max_q_n = max(max_q_n, q_n);
	    n_packets++;
	    klog(L_NET_INFO,
	 "q_byt %d, q_n %d, rd_byt %d, mx_q_b %d, mx_q_n %d, n_pkt %d",
		 q_bytes, q_n, n, max_q_bytes, max_q_n, n_packets, 0);
	    max_q_bytes = max(max_q_bytes, q_bytes);
	    if (!q_bytes)
		q_n = 0;	/* reset consecutive packets */
	    kerberos(&from, pkt);
	} else
	    klog(L_NET_ERR,
	    "%s: bad recvfrom n = %d errno = %d", progname, n, errno, 0);
    }
}
#else /* BACKWARD_COMPAT */

/* v4/v5 backwards-compatibility stub routines,
 * which allow the v5 server to handle v4 packets
 * by invoking substantially-unaltered v4 server code.
 * this is only necessary during the installation's conversion to v5.
 * process_v4() is invoked by v5's dispatch() routine;
 * when the v4 server needs to access the v5 database,
 * it calls the other stubs.
 *
 * until all kerberized application-programs are updated,
 * this approach inflates the v5 server's code size,
 * but it's easier to debug than a concurrent, subordinate v4 server would be.
 */

/*
 * v5 include files:
 */
#include <krb5/krb5.h>
#include <krb5/base-defs.h>
#include <krb5/encryption.h>
#include <krb5/wordsize.h>
#include <krb5/mit-des.h>
#include <krb5/kdb.h>

static krb5_error_code retval; 
static krb5_data *response;
void bzero(), bcopy(), com_err(), sleep();
void kerberos_v4(), kerb_err_reply();
#ifdef HAS_STDLIB_H
#include <stdlib.h>
#else
char *free(), *malloc();
#endif
krb5_error_code krb5_timeofday(), krb5_get_default_realm();
 
krb5_error_code
process_v4( pkt, client_fulladdr, resp)
const krb5_data *pkt;
const krb5_fulladdr *client_fulladdr;
krb5_data **resp;
{
    struct sockaddr_in client_sockaddr;
    krb5_address *addr = client_fulladdr->address;
    krb5_error_code retval;
    KTEXT_ST v4_pkt;
    char *lrealm;

    if (retval = krb5_timeofday((krb5_timestamp *) &kerb_time.tv_sec))
        return(retval);

    if (!*local_realm) {		/* local-realm name already set up */
	retval = krb5_get_default_realm(&lrealm);
	if (!retval) {
	    if (strlen(lrealm) < sizeof(local_realm))
		strcpy(local_realm, lrealm);
	    else
		retval = KRB5_CONFIG_NOTENUFSPACE;
        }
	if (retval) {
	    com_err("KRBV4KDC", retval,
		    "while attempting to get default realm");
	    exit(1);
        }
    }
    /* convert client_fulladdr to client_sockaddr:
     */
    client_sockaddr.sin_family	= AF_INET;
    client_sockaddr.sin_port	= client_fulladdr->port;
    bcopy( addr->contents, &client_sockaddr.sin_addr, 
		     sizeof client_sockaddr.sin_addr);
    bzero( client_sockaddr.sin_zero, sizeof client_sockaddr.sin_zero);

    /* convert v5 packet structure to v5's.
     * this copy is gross, but necessary:
     */
    v4_pkt.length = pkt->length;
    bcopy( pkt->data, v4_pkt.dat, pkt->length);

    kerberos_v4( &client_sockaddr, &v4_pkt);
    *resp = response;
    return(retval);
}
/* convert k4's klog() levels into corresponding errors for v5: */
int type_2_v5err[] = { 0,	/* 		0  No error		      */
    KDC_ERR_NONE,		/* L_NET_ERR	1  Error in network code      */
    KDC_ERR_NONE,		/* L_NET_INFO	2  Info on network activity   */
    KRB_AP_ERR_BADVERSION,	/* L_KRB_PERR	3  Kerberos protocol errors   */
    KDC_ERR_NONE,		/* L_KRB_PINFO	4  Kerberos protocol info     */
    KDC_ERR_NONE,		/* L_INI_REQ	5  Request for initial ticket */
    KRB_AP_ERR_BADVERSION,	/* L_NTGT_INTK	6  Initial request not for TGT*/
    KDC_ERR_NONE,		/* L_DEATH_REQ	7  Request for server death   */
    KDC_ERR_NONE,		/* L_TKT_REQ	8  All ticket requests w/ tgt */
    KDC_ERR_SERVICE_EXP,	/* L_ERR_SEXP	9  Service expired	      */
    KDC_ERR_C_OLD_MAST_KVNO,	/* L_ERR_MKV	10 Master key version old     */
    KDC_ERR_NULL_KEY,		/* L_ERR_NKY    11 User's key is null         */
    KDC_ERR_PRINCIPAL_NOT_UNIQUE, /* L_ERR_NUN	12 Principal not unique       */
    KDC_ERR_C_PRINCIPAL_UNKNOWN,  /* L_ERR_UNK	13 Principal Unknown          */
    KDC_ERR_NONE,		/* L_ALL_REQ    14 All requests	     	      */
    KDC_ERR_NONE,		/* L_APPL_REQ   15 Application requests w/ tgt*/
    KRB_AP_ERR_BADVERSION	/* L_KRB_PWARN  16 Protocol warning messages  */
};
#define klog v4_klog
#if __STDC__ || defined(STDARG_PROTOTYPES)
char * v4_klog( int type, const char *format, ...)
#else
char * v4_klog( type, format, va_alist)
    int type;
    char *format;
    va_dcl
#endif
{
    va_list pvar;
#if __STDC__ || defined(STDARG_PROTOTYPES)
    va_start(pvar, format);
#else
    va_start(pvar);
#endif

    switch (type) {
    case L_INI_REQ:
    case L_NTGT_INTK:
    case L_TKT_REQ:
    case L_ERR_SEXP:
    case L_ERR_NKY:
    case L_ERR_NUN:
    case L_ERR_UNK:
    case L_APPL_REQ:
	strcpy(log_text, "PROCESS_V4:");
	vsprintf(log_text+strlen(log_text), format, pvar);
	com_err(0, 0, log_text);
    /* ignore the other types... */
    }
    va_end(pvar);
    return(log_text);
}

static
int sendto(s, msg, len, flags, to, to_len)
int s;
const char *msg;
int len, flags;
const struct sockaddr *to;
int to_len;
{
    if (  !(response = (krb5_data *) malloc( sizeof *response))) {
	return ENOMEM;
    }
    if ( !(response->data = (char *) malloc( len))) {
	krb5_free_data( response);
	return ENOMEM;
    }
    response->length = len;
    bcopy( msg, response->data, len);
    return( 0);
}
static void
hang()
{
    if (pause_int == -1) {
        klog(L_KRB_PERR, "Kerberos will pause so as not to loop init");
     /* for (;;)
            pause(); */
    } else {
        char buf[256];
        sprintf(buf,
	   "Kerberos will wait %d seconds before dying so as not to loop init",
	   pause_int);
        klog(L_KRB_PERR, buf);
        sleep(pause_int);
        klog(L_KRB_PERR, "Do svedania....\n");
     /* exit(1); */
    }
}
#define kdb_encrypt_key( in, out, mk, mks, e_d_flag)
#define LONGLEN 4
extern krb5_encrypt_block master_encblock;

/* take a v5 keyblock, masquerading as a v4 key,
 * decrypt it, and convert the resulting v5 keyblock
 * to a real v4 key.
 * this is ugly, but it saves changing more v4 code.
 */
int
compat_decrypt_key (in5, out4)
     krb5_encrypted_keyblock *in5;
     C_Block out4;
{
    krb5_keyblock out5;
    int retval = -1;

    out5.contents = NULL;
    if ( krb5_kdb_decrypt_key( &master_encblock, in5, &out5)) {
	lt = klog(L_DEATH_REQ, "KDC can't decrypt principal's key.");
    }
    if ( ! out5.contents) return( retval);
    if ( out5.length != MIT_DES_KEYSIZE) {
	lt = klog( L_DEATH_REQ,"internal keysize error in kdc");
    }
    else {
	retval = 0;
	bcopy(	out5.contents, out4, out5.length);
    }
    bzero(	out5.contents,       out5.length);
    xfree(	out5.contents);
    return( retval);
}
/* array of name-components + NULL ptr
 */
static krb5_data search_stg[3];
static krb5_data *search[4] ={ &search_stg[0], &search_stg[1], &search_stg[2]};
#define MIN5 300
#define HR21 255

int
kerb_get_principal(name, inst, principal, maxn, more)
    char   *name;               /* could have wild card */
    char   *inst;               /* could have wild card */
    Principal *principal;
    unsigned int maxn;          /* max number of name structs to return */
    int    *more;               /* more tuples than room for */
{
    krb5_db_entry entries;	/* filled in by krb5_db_get_principal() */
    int nprinc;			/* how many found */
    krb5_boolean more5;		/* are there more? */
    C_Block k;
    short toggle = 0;
    int i, time;
    unsigned long *date;
    char* text;
    struct tm *tp;

    if ( maxn > 1) {
	lt = klog(L_DEATH_REQ, "KDC V4 is requesting too many principals");
	return( 0);
    }
    /* begin setting up the principal structure
     * with the first info we have:
     */
    bcopy( name, principal->name,     1 + strlen( name));
    bcopy( inst, principal->instance, 1 + strlen( inst));

    /* the principal-name format changed between v4 & v5:
     *     v4: name.instance@realm
     *     v5: realm/name/instance
     *     in v5, null instance means the null-component doesn't exist.
     */
    search_stg[0].data = local_realm;
    search_stg[1].data = name;
    search_stg[2].data = inst;
    for ( i = 0; i < 3; i++) {
	if (! search[i])		search[i] = &search_stg[i];
	if (!*( search[i]->data))	search[i] = NULL;
	else search[i]->length = strlen( search[i]->data);
    }
    if (retval = krb5_db_get_principal( search, &entries, &nprinc, &more5)) {
	more = 0;
        return( 0);
    }
    principal->key_low = principal->key_high = 0;

    if ( nprinc < 1) {
	goto cleanup;
    } else if ( entries.key.keytype != KEYTYPE_DES) {
	lt = klog(L_KRB_PERR, "KDC V4: principal %s.%s has non-DES keytype %d",
		  (int) name, (int) inst, entries.key.keytype);
	nprinc = 0;
	goto cleanup;
    } else if (! compat_decrypt_key( &entries.key, k)) {
	bcopy(		k,	&principal->key_low,  LONGLEN);
	bcopy((long *) 	k + 1,	&principal->key_high, LONGLEN);
    }
    /* convert v5's entries struct to v4's Principal struct:
     * v5's time-unit for lifetimes is 1 sec, while v4 uses 5 minutes.
     */
    time = (entries.max_life + MIN5 - 1) / MIN5;
    principal->max_life = time > HR21 ? HR21 : (unsigned char) time;
    principal->exp_date = (unsigned long) entries.expiration;
    principal->mod_date = (unsigned long) entries.mod_date;
    principal->attributes = 0;
    principal->kdc_key_ver = entries.mkvno;
    principal->key_version = entries.kvno;

    /* set up v4 format of each date's text:
     */
    for ( date = &principal->exp_date, text = principal->exp_date_txt;
	  toggle ^= 1;
	  date = &principal->mod_date, text = principal->mod_date_txt) {
	tp = localtime( (time_t *) date);
	sprintf( text, "%4d-%02d-%02d",
		 tp->tm_year > 1900 ? tp->tm_year : tp->tm_year + 1900,
		 tp->tm_mon + 1, tp->tm_mday); /* January is 0, not 1 */
    }
cleanup:
    /* free the storage held by the v5 entry struct,
     * which was allocated by krb5_db_get_principal().
     * this routine clears the keyblock's contents for us.
     */
    krb5_db_free_principal( &entries, nprinc);

    *more = (int) more5 || (nprinc > maxn);
    return( nprinc);
}
void
#endif /* BACKWARD_COMPAT */

kerberos_v4(client, pkt)
    struct sockaddr_in *client;
    KTEXT   pkt;
{
    static KTEXT_ST rpkt_st;
    KTEXT   rpkt = &rpkt_st;
    static KTEXT_ST ciph_st;
    KTEXT   ciph = &ciph_st;
    static KTEXT_ST tk_st;
    KTEXT   tk = &tk_st;
    static KTEXT_ST auth_st;
    KTEXT   auth = &auth_st;
    AUTH_DAT ad_st;
    AUTH_DAT *ad = &ad_st;


    static struct in_addr client_host;
    static int msg_byte_order;
    static int swap_bytes;
    static u_char k_flags;
 /* char   *p_name, *instance; */
    u_long  lifetime = 0;
    int     i;
    C_Block key;
    Key_schedule key_s;
    char   *ptr;



    ciph->length = 0;

    client_host = client->sin_addr;

    /* eval macros and correct the byte order and alignment as needed */
    req_version = pkt_version(pkt);	/* 1 byte, version */
    req_msg_type = pkt_msg_type(pkt);	/* 1 byte, Kerberos msg type */

    req_act_vno = req_version;

    /* check packet version */
    if (req_version != KRB_PROT_VERSION) {
	lt = klog(L_KRB_PERR,
	"KRB prot version mismatch: KRB =%d request = %d",
		  KRB_PROT_VERSION, req_version, 0);
	/* send an error reply */
	req_name_ptr = req_inst_ptr = req_realm_ptr = "";
	kerb_err_reply(client, pkt, KERB_ERR_PKT_VER, lt);
	return;
    }
    msg_byte_order = req_msg_type & 1;

    swap_bytes = 0;
    if (msg_byte_order != HOST_BYTE_ORDER) {
	swap_bytes++;
    }
    klog(L_KRB_PINFO,
	"Prot version: %d, Byte order: %d, Message type: %d",
	 (int) req_version, msg_byte_order, req_msg_type);

    switch (req_msg_type & ~1) {

    case AUTH_MSG_KDC_REQUEST:
	{
#ifdef notdef
	    u_long  time_ws;	/* Workstation time */
#endif
	    u_long  req_life;	/* Requested liftime */
	    char   *service;	/* Service name */
	    char   *instance;	/* Service instance */
#ifdef notdef
	    int     kerno;	/* Kerberos error number */
#endif
	    n_auth_req++;
	    tk->length = 0;
	    k_flags = 0;	/* various kerberos flags */


	    /* set up and correct for byte order and alignment */
	    req_name_ptr = (char *) pkt_a_name(pkt);
	    req_inst_ptr = (char *) pkt_a_inst(pkt);
	    req_realm_ptr = (char *) pkt_a_realm(pkt);
	    bcopy(pkt_time_ws(pkt), &req_time_ws, sizeof(req_time_ws));
	    /* time has to be diddled */
	    if (swap_bytes) {
		swap_u_long(req_time_ws);
	    }
	    ptr = (char *) pkt_time_ws(pkt) + 4;

	    req_life = (u_long) (*ptr++);

	    service = ptr;
	    instance = ptr + strlen(service) + 1;

	    rpkt = &rpkt_st;
	    klog(L_INI_REQ,
	    "Initial ticket request Host: %s User: \"%s\" \"%s\"",
	       inet_ntoa(client_host), req_name_ptr, req_inst_ptr, 0);

	    if (i = check_princ(req_name_ptr, req_inst_ptr, 0,
		&a_name_data)) {
		kerb_err_reply(client, pkt, i, lt);
		return;
	    }
	    tk->length = 0;	/* init */
	    if (strcmp(service, "krbtgt"))
		klog(L_NTGT_INTK,
		    "INITIAL request from %s.%s for %s.%s", (int) req_name_ptr,
		    (int) req_inst_ptr, (int) service, (int) instance, 0);
	    /* this does all the checking */
	    if (i = check_princ(service, instance, lifetime,
		&s_name_data)) {
		kerb_err_reply(client, pkt, i, lt);
		return;
	    }
	    /* Bound requested lifetime with service and user */
	    lifetime = min(req_life, ((u_long) s_name_data.max_life));
	    lifetime = min(lifetime, ((u_long) a_name_data.max_life));
#ifdef NOENCRYPTION
	    bzero(session_key, sizeof(C_Block));
#else
	    /* random session key */
	    random_key(session_key);
#endif

	    /* unseal server's key from master key */
	    bcopy(&s_name_data.key_low, key, 4);
	    bcopy(&s_name_data.key_high, ((long *) key) + 1, 4);
	    kdb_encrypt_key(key, key, master_key,
			    master_key_schedule, DECRYPT);
	    /* construct and seal the ticket */
	    krb_create_ticket(tk, k_flags, a_name_data.name,
		a_name_data.instance, local_realm,
		 client_host.s_addr, session_key, lifetime, kerb_time.tv_sec,
			 s_name_data.name, s_name_data.instance, key);
	    bzero(key, sizeof(key));
	    bzero(key_s, sizeof(key_s));

	    /*
	     * get the user's key, unseal it from the server's key, and
	     * use it to seal the cipher 
	     */

	    /* a_name_data.key_low a_name_data.key_high */
	    bcopy(&a_name_data.key_low, key, 4);
	    bcopy(&a_name_data.key_high, ((long *) key) + 1, 4);

	    /* unseal the a_name key from the master key */
	    kdb_encrypt_key(key, key, master_key, 
			    master_key_schedule, DECRYPT);

	    create_ciph(ciph, session_key, s_name_data.name,
			s_name_data.instance, local_realm, lifetime,
		  s_name_data.key_version, tk, kerb_time.tv_sec, key);

	    /* clear session key */
	    bzero(session_key, sizeof(session_key));

	    bzero(key, sizeof(key));



	    /* always send a reply packet */
	    rpkt = create_auth_reply(req_name_ptr, req_inst_ptr,
		req_realm_ptr, req_time_ws, 0, a_name_data.exp_date,
		a_name_data.key_version, ciph);
	    sendto(f, (char *) rpkt->dat, rpkt->length, 0,
		   (struct sockaddr *) client, S_AD_SZ);
	    bzero(&a_name_data, sizeof(a_name_data));
	    bzero(&s_name_data, sizeof(s_name_data));
	    break;
	}
    case AUTH_MSG_APPL_REQUEST:
	{
	    u_long  time_ws;	/* Workstation time */
	    u_long  req_life;	/* Requested liftime */
	    char   *service;	/* Service name */
	    char   *instance;	/* Service instance */
	    int     kerno = 0;	/* Kerberos error number */
	    char    tktrlm[REALM_SZ];

	    n_appl_req++;
	    tk->length = 0;
	    k_flags = 0;	/* various kerberos flags */

	    auth->length = 4 + strlen((char *)pkt->dat + 3);
	    auth->length += (int) *(pkt->dat + auth->length) +
		(int) *(pkt->dat + auth->length + 1) + 2;

	    bcopy(pkt->dat, auth->dat, auth->length);

	    strncpy(tktrlm, (char *)auth->dat + 3, REALM_SZ);
	    if (set_tgtkey(tktrlm)) {
		lt = klog(L_ERR_UNK,
		    "FAILED realm %s unknown. Host: %s ",
			  (int) tktrlm, inet_ntoa(client_host));
		kerb_err_reply(client, pkt, kerno, lt);
		return;
	    }
	    kerno = krb_rd_req(auth, "ktbtgt", tktrlm, client_host.s_addr,
		ad, 0);

	    if (kerno) {
		klog(L_ERR_UNK, "FAILED krb_rd_req from %s: %s",
		     inet_ntoa(client_host), krb_err_txt[kerno]);
		req_name_ptr = req_inst_ptr = req_realm_ptr = "";
		kerb_err_reply(client, pkt, kerno, "krb_rd_req failed");
		return;
	    }
	    ptr = (char *) pkt->dat + auth->length;

	    bcopy(ptr, &time_ws, 4);
	    ptr += 4;

	    req_life = (u_long) (*ptr++);

	    service = ptr;
	    instance = ptr + strlen(service) + 1;

	    klog(L_APPL_REQ, "APPL Request %s.%s@%s on %s for %s.%s",
	     (int) ad->pname, (int) ad->pinst, (int) ad->prealm,
	     (int) inet_ntoa(client_host), (int) service, (int) instance, 0);
	    req_name_ptr = ad->pname;
	    req_inst_ptr = ad->pinst;
	    req_realm_ptr = ad->prealm;

	    if (strcmp(ad->prealm, tktrlm)) {
		kerb_err_reply(client, pkt, KERB_ERR_PRINCIPAL_UNKNOWN,
		     "Can't hop realms");
		return;
	    }
	    if (!strcmp(service, "changepw")) {
		kerb_err_reply(client, pkt, KERB_ERR_PRINCIPAL_UNKNOWN,
		     "Can't authorize password changed based on TGT");
		return;
	    }
	    kerno = check_princ(service, instance, req_life,
		&s_name_data);
	    if (kerno) {
		kerb_err_reply(client, pkt, kerno, lt);
		return;
	    }
	    /* Bound requested lifetime with service and user */
	    lifetime = min(req_life,
	      (ad->life - ((kerb_time.tv_sec - ad->time_sec) / 300)));
	    lifetime = min(lifetime, ((u_long) s_name_data.max_life));

	    /* unseal server's key from master key */
	    bcopy(&s_name_data.key_low, key, 4);
	    bcopy(&s_name_data.key_high, ((long *) key) + 1, 4);
	    kdb_encrypt_key(key, key, master_key,
			    master_key_schedule, DECRYPT);
	    /* construct and seal the ticket */

#ifdef NOENCRYPTION
	    bzero(session_key, sizeof(C_Block));
#else
	    /* random session key */
	    random_key(session_key);
#endif

	    krb_create_ticket(tk, k_flags, ad->pname, ad->pinst,
			      ad->prealm, client_host.s_addr,
			      session_key, lifetime, kerb_time.tv_sec,
			      s_name_data.name, s_name_data.instance,
			      key);
	    bzero(key, sizeof(key));
	    bzero(key_s, sizeof(key_s));

	    create_ciph(ciph, session_key, service, instance,
			local_realm,
			lifetime, s_name_data.key_version, tk,
			kerb_time.tv_sec, ad->session);

	    /* clear session key */
	    bzero(session_key, sizeof(session_key));

	    bzero(ad->session, sizeof(ad->session));

	    rpkt = create_auth_reply(ad->pname, ad->pinst,
				     ad->prealm, time_ws,
				     0, 0, 0, ciph);
	    sendto(f, (char *) rpkt->dat, rpkt->length, 0,
		   (struct sockaddr *) client, S_AD_SZ);
	    bzero(&s_name_data, sizeof(s_name_data));
	    break;
	}


#ifdef notdef_DIE
    case AUTH_MSG_DIE:
	{
	    lt = klog(L_DEATH_REQ,
	        "Host: %s User: \"%s\" \"%s\" Kerberos killed",
	        inet_ntoa(client_host), req_name_ptr, req_inst_ptr, 0);
	    exit(0);
	}
#endif /* notdef_DIE */

    default:
	{
	    lt = klog(L_KRB_PERR,
		"Unknown message type: %d from %s port %u",
		(int) req_msg_type, (int) inet_ntoa(client_host),
		ntohs(client->sin_port));
	    break;
	}
    }
}


#ifndef BACKWARD_COMPAT
/*
 * setup_disc 
 *
 * disconnect all descriptors, remove ourself from the process
 * group that spawned us. 
 */

setup_disc()
{

    int     s;

    for (s = 0; s < 3; s++) {
	(void) close(s);
    }

    (void) open("/dev/null", 0);
    (void) dup2(0, 1);
    (void) dup2(0, 2);

    s = open("/dev/tty", 2);

    if (s >= 0) {
	ioctl(s, TIOCNOTTY, (struct sgttyb *) 0);
	(void) close(s);
    }
    (void) chdir("/tmp");
    return;
}
#endif /* BACKWARD_COMPAT */


/*
 * kerb_er_reply creates an error reply packet and sends it to the
 * client. 
 */

void
kerb_err_reply(client, pkt, err, string)
    struct sockaddr_in *client;
    KTEXT   pkt;
    long    err;
    char   *string;

{
    static KTEXT_ST e_pkt_st;
    KTEXT   e_pkt = &e_pkt_st;
    static char e_msg[128];

    strcpy(e_msg, "\nKerberos error -- ");
    strcat(e_msg, string);
    cr_err_reply(e_pkt, req_name_ptr, req_inst_ptr, req_realm_ptr,
		 req_time_ws, err, e_msg);
    sendto(f, (char *) e_pkt->dat, e_pkt->length, 0,
	   (struct sockaddr *) client, S_AD_SZ);

}

#ifndef BACKWARD_COMPAT
/*
 * Make sure that database isn't stale.
 *
 * Exit if it is; we don't want to tell lies.
 */

static void check_db_age()
{
    long age;
    
    if (max_age != -1) {
	/* Requires existance of kerb_get_db_age() */
	gettimeofday(&kerb_time, 0);
	age = kerb_get_db_age();
	if (age == 0) {
	    klog(L_KRB_PERR, "Database currently being updated!");
	    hang();
	}
	if ((age + max_age) < kerb_time.tv_sec) {
	    klog(L_KRB_PERR, "Database out of date!");
	    hang();
	    /* NOTREACHED */
	}
    }
}
#endif /* BACKWARD_COMPAT */

int check_princ(p_name, instance, lifetime, p)
    char   *p_name;
    char   *instance;
    unsigned lifetime;

    Principal *p;
{
    static int n;
    static int more;
 /* long trans; */

    n = kerb_get_principal(p_name, instance, p, 1, &more);
    klog(L_ALL_REQ,
	 "Principal: \"%s\", Instance: \"%s\" Lifetime = %d n = %d",
	 (int) p_name, (int) instance, lifetime, n, 0);
    
    if (n < 0) {
	lt = klog(L_KRB_PERR, "Database unavailable!");
	hang();
    }
    
    /*
     * if more than one p_name, pick one, randomly create a session key,
     * compute maximum lifetime, lookup authorizations if applicable,
     * and stuff into cipher. 
     */
    if (n == 0) {
	/* service unknown, log error, skip to next request */
	lt = klog(L_ERR_UNK, "UNKNOWN \"%s\" \"%s\"", (int) p_name,
	    (int) instance, 0);
	return KERB_ERR_PRINCIPAL_UNKNOWN;
    }
    if (more) {
	/* not unique, log error */
	lt = klog(L_ERR_NUN, "Principal NOT UNIQUE \"%s\" \"%s\"",
		  (int) p_name, (int) instance, 0);
	return KERB_ERR_PRINCIPAL_NOT_UNIQUE;
    }
    /* If the user's key is null, we want to return an error */
    if ((p->key_low == 0) && (p->key_high == 0)) {
	/* User has a null key */
	lt = klog(L_ERR_NKY, "Null key \"%s\" \"%s\"", (int) p_name,
	    (int) instance, 0);
	return KERB_ERR_NULL_KEY;
    }
    if (master_key_version != p->kdc_key_ver) {
	/* log error reply */
	lt = klog(L_ERR_MKV,
	    "Key vers incorrect, KRB = %d, \"%s\" \"%s\" = %d",
	    master_key_version, (int)p->name, (int)p->instance, p->kdc_key_ver,
	    0);
	return KERB_ERR_NAME_MAST_KEY_VER;
    }
    /* make sure the service hasn't expired */
    if ((u_long) p->exp_date < (u_long) kerb_time.tv_sec) {
	/* service did expire, log it */
	lt = klog(L_ERR_SEXP,
	    "EXPIRED \"%s\" \"%s\"  %s", (int) p->name, (int) p->instance,
	     (int) stime(&(p->exp_date)), 0);
	return KERB_ERR_NAME_EXP;
    }
    /* ok is zero */
    return 0;
}


/* Set the key for krb_rd_req so we can check tgt */
int set_tgtkey(r)
    char   *r;			/* Realm for desired key */
{
    int     n;
    static char lastrealm[REALM_SZ];
    Principal p_st;
    Principal *p = &p_st;
    C_Block key;

    if (!strcmp(lastrealm, r))
	return (KSUCCESS);

    log("Getting key for %s", r);

    n = kerb_get_principal("krbtgt", r, p, 1, &more);
    if (n == 0)
	return (KFAILURE);

    /* unseal tgt key from master key */
    bcopy(&p->key_low, key, 4);
    bcopy(&p->key_high, ((long *) key) + 1, 4);
    kdb_encrypt_key(key, key, master_key,
		    master_key_schedule, DECRYPT);
    krb_set_key(key, 0);
    strcpy(lastrealm, r);
    return (KSUCCESS);
}

#ifndef BACKWARD_COMPAT
static void
hang()
{
    if (pause_int == -1) {
	klog(L_KRB_PERR, "Kerberos will pause so as not to loop init");
	for (;;)
	    pause();
    } else {
	char buf[256];
	sprintf(buf,  "Kerberos will wait %d seconds before dying so as not to loop init", pause_int);
	klog(L_KRB_PERR, buf);
	sleep(pause_int);
	klog(L_KRB_PERR, "Do svedania....\n");
	exit(1);
    }
}
#endif /* BACKWARD_COMPAT */
#endif /* KRB4 */
