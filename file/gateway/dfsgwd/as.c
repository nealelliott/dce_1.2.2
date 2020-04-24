/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * Copyright (c) 1996, 1995 Transarc Corp.  All rights reserved.
 */
/*
 * as.c - Authentication service.
 *
 * This is the multithreaded authentication service that provides
 * the meat of the dfsgwd daemon.
 */

#include <dce/dce.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <krb5/func-proto.h>
#include <krb5/sysincl.h>
#include <com_err.h>
#include <dce/dce_cf.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/passwd.h>
#include <dce/binding.h>
#include <dce/misc.h>
#include <dce/rpcsts.h>
#include <pthread.h>
#include <dcedfs/param.h>
#ifdef AFS_SUNOS5_ENV
#undef MIN
#undef MAX
#endif

#include <dcedfs/osi.h>
#include <dcedfs/auth_at.h>
#include <dcedfs/syscall.h>
#include <dfsgw.h>
#include <tgt_unpack.h>
#include <as.h>
#include <dfsgwd.h>
#include <dfsgwymac.h>
#include <dfsgwysvc.h>
#include <dfsgwymsg.h>

RCSID("$Header")

#define AS_SEL_TIMEOUT		(60)		/* Number of seconds to wait in select(2) for something to happen */
#define STRLEN			(256)		/* Length of various strings */
#ifdef _POSIX_REENTRANT_FUNCTIONS
#define CTIME_STRLEN		(26)		/* Reentrant ctime string */
#endif
#define host_byte_order		DFSGW_BIG_ENDIAN 

/*
 * Local data structures.
 */
typedef struct {
	unsigned char data[DFSGW_MAXPSIZE];	/* Data packet */
	int len;				/* Length of network address */
	struct sockaddr client;			/* Network address */
} as_req_t;

typedef as_req_t *as_req_p;

extern int debug;
extern char *sysname;

/*
 * Local read-only (shared without locking) data.
 */
static int asock;				/* Authentication socket */
static int shutdown_as = 0;		       /* Shutdown flag */

/*
 * Local shared data.
 */
static int nthreads;				/* Number of threads working on requests */
static pthread_mutex_t nthread_lock;		/* Lock protecting nthreads */
static krb5_principal sprinc;			/* Server principal */
static krb5_address *local_portaddr;		/* Local full address */
static sec_rgy_handle_t rgy_context;	       /* RPC handle to rgy server */

/*
 * as_init - Initialize authentication service.
 *
 * This function sets up the initial networking for the authentication
 * service.  The most important thing that is done here is binding to
 * the service socket, because failure to do so may indicate that there
 * is already a gateway server running, and it is better to detect 
 * this early.
 *
 * Inputs:
 *	service			: Name of authentication service (port) to bind to.
 * Return value:
 *	-1 on failure, 0 otherwise.
 */
int as_init
#ifdef __STDC__
	(char *service, char *server)
#else
	(service, server)
	char *service;
        char *server;
#endif /* __STDC__ */
{
	struct servent *sp, default_serv;	/* Server entry structure */
	struct sockaddr_in addr;		/* Network address */
	char pname[STRLEN];			/* Principal name */
	char hostname[STRLEN];			/* Hostname */
	char *dce_hostname;			/* DCE hostname */
	struct hostent *hp;			/* Host entry descriptor structure */
	krb5_address localaddr;			/* Kerberos local address */
	char *realm;				/* Our realm */	
	krb5_error_code code;			/* Kerberos error codes */
	error_status_t st;			/* DCE status */

	krb5_init_ets();
	/*
	 * Set up shared authentication service socket.
	 */
	if ((asock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
	        dce_svc_printf(GWY_S_OPENSOCKERR_MSG, progname);
		return(-1);
	}
        /* -service switch may have been used, be sure to try it if
         * getservbyname fails
         */
	if ( ((sp = getservbyname(service, "udp")) == NULL)  && ((sp = getservbyport(atoi(service), "udp")) == NULL) ){
	        dce_svc_printf(GWY_S_SERVICEUNKNOWN_MSG, progname, (void *)service);
                default_serv.s_port = DFSGW_SERVICE_DEFAULT;
                default_serv.s_name = DFSGW_SERVICE;
                sp = &default_serv;
	}
	memset((char *)&addr, 0, sizeof(addr)); 
	addr.sin_family = AF_INET;
	addr.sin_port =	 sp->s_port;
	if (bind(asock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                dce_svc_printf(GWY_S_BINDSOCKERR_MSG, progname, (void *) sp->s_port);
		return(-1);
	}
	/*
	 * Setup our full address.  This will be used when making/reading krb messages.
	 */
	if (gethostname(hostname, STRLEN) < 0) {
                dce_svc_printf(GWY_S_HOSTNAMEERR_MSG, progname);
		return(-1);
	}
	if ((hp = gethostbyname((const char *)hostname)) == NULL) {
                dce_svc_printf(GWY_S_HOSTADDRERR_MSG, progname);
		return(-1);
	}
	localaddr.addrtype = hp->h_addrtype;
	localaddr.length = hp->h_length;
	localaddr.contents = (krb5_octet *)hp->h_addr;
	if (code = krb5_gen_portaddr(&localaddr, (krb5_pointer)&sp->s_port, &local_portaddr)) {
                dce_svc_printf(GWY_S_GENPORTADDRERR_MSG, progname);
		return(-1);
	}
	/*
	 * Setup our principal name.  This is:
	 *
	 * "hosts/<hostname>/<servicename>-server@<realm>"
	 */
	dce_cf_get_host_name (&dce_hostname, &st);
	if (st != dce_cf_st_ok) {
		dce_svc_printf(GWY_S_HOSTNAMEERR_MSG, progname);
		return(-1);
	}
	if (code = krb5_get_default_realm(&realm)) {
                dce_svc_printf(GWY_S_DEFAULTREALMERR_MSG, progname); 
		return(-1);
	}

        sprintf(pname, "%s/%s-server@%s", dce_hostname, sp->s_name, realm);
        
	xfree(realm);
	if (code = krb5_parse_name((char *)pname, &sprinc)) {
                dce_svc_printf(GWY_S_PARSENAMEERR_MSG, progname, (void *) pname);
		return(-1);
	}
	/*
	 * Bind to registry.  We'll use this later on to check unix IDs.
	 */
	sec_rgy_site_open(NULL, &rgy_context, &st);
	if (st != error_status_ok) {
		dce_error_string_t err_string;
		int lst;

		dce_error_inq_text(st, err_string, &lst);
		if (lst) {
			sprintf((char *)err_string, "%d", st);
		}
                dce_svc_printf(GWY_S_DCEREGBINDERR_MSG, progname, (void *) err_string);
		return(-1);
	}
	/*
	 * Misc setups.
	 */
	pthread_mutex_init(&nthread_lock, pthread_mutexattr_default);
	return(0);
}
#if 0
/*
 * gotsignal - Signal catcher.
 *
 * The preferred way to bring down the gateway is by sending a kill
 * signal (NOT -9) so that all threads can finish off the outstanding
 * requests.  This signal catcher notes the reciept of such a signal
 * and logs it so the threads will notice.
 *
 * Return value:
 *	void
 */
static void gotsignal()
{
	signal(SIGINT, SIG_IGN);
	signal(SIGTERM, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	shutdown_as++;
}
#endif
/*
 * log_dfsgw_msg - Log message while processing remote authentication request.
 *
 * This is a common routine to log a message while tacking on information
 * about the request being processed.  
 *
 * Inputs:
 *	req			: as_req_p of the request.
 *	st			: DCE error status, can be zero
 *	fmt			: printf-style format string.
 *	...			: variable arguments for fmt.
 * Return value:
 *	void
 */
static void log_dfsgw_msg
#ifdef __STDC__
	(as_req_p req, error_status_t st, char *fmt, ...)
#else 
	(req, st, fmt, va_alist)
	as_req_p req;
	error_status_t st;
	char *fmt;
	va_dcl		
#endif /* __STDC__ */
{
	dce_error_string_t err_string;		/* DCE error string */
	int lst;				/* Local status */ 
        struct hostent hostentry;               /* Host info */
        char h_name[MAXHOSTNAMELEN];            /* Hostname */
#ifdef _POSIX_REENTRANT_FUNCTIONS
	struct hostent_data data;		/* Data for above thread-safeness */
#else
        struct hostent *hostentryptr;           /* ptr to Host info */
#endif
	va_list ap;				/* Pointer to variable length list */
	char sbuf[1024];			/* Buffer for variable printf */
	
	/*
	 * Since many people can be banging away on the log, we'll assemble the
	 * message here first, then log it.
	 *
	 * First get error string for DCE if it exists.
	 */
	if (st) {
		dce_error_inq_text(st, err_string, &lst);
		if (lst) {
			sprintf((char *)err_string, "%d", st);
		} 
	}
	/*
	 * Now resolve node ID.
	 */
#ifdef _POSIX_REENTRANT_FUNCTIONS
	if (gethostbyaddr_r(&req->client.sa_data[2], sizeof(struct in_addr), req->client.sa_family, &hostentry, &data) < 0) {
                strncpy(h_name, "Unknown!", strlen("Unknown!"));
                strcat(h_name, "\0");
        }
        else {
                strcpy(h_name, hostentry.h_name); 
                strcat(h_name, "\0");
        }
     
#else
       if ((hostentryptr = gethostbyaddr(&req->client.sa_data[2], sizeof(struct in_addr), req->client.sa_family)) == 0) {
                strncpy(h_name, "Unknown!", strlen("Unknown!"));
                strcat(h_name, "\0");
      }
      else {
                strcpy(h_name, hostentryptr->h_name);
                strcat(h_name, "\0");
      }
#endif

	/*
	 * Construct caller's message.
	 */
	va_start(ap, fmt);
	vsprintf(sbuf, fmt, ap);
	va_end(ap);
	if (st) {
		log("Req from %s : %s ; DCE error = %s", h_name, sbuf, err_string);
	} else {
		log("Req from %s : %s", h_name, sbuf);
	}
	return;
}

/* 
 * update_cred - update existing credentials
 *
 * This function associates externally-obtained refreshed credentials with 
 * a local dce username, when credentials already exist locally for the user. 
 * 
 * Inputs:
 *	username	: DCE username.	 Must be just the name, no realm.
 *	creds		: DCE creds, just a TGT.
 *	req		: Request structure for this packet, for error 
 * 			  reporting.
 * 	known_pag	: pag number for the existing context to be
 *			  refreshed
 *
 * Outputs:
 *	expiration_time		: Expiration time of the resulting credentials.
 *
 * Return:
 * 	0 on success, error code on failure
 */
static unsigned32 update_cred
#ifdef __STDC__
    (char *username, krb5_creds *creds, as_req_p req, time_t *expiration_time,
     unsigned32 known_pag)
#else
(username, creds, req, expiration_time, known_pag)
  char *username;
  krb5_cred *creds;
  as_req_p req;
  time_t *expiration_time;
  unsigned32 known_pag;
#endif /* __STDC__ */
{
    error_status_t st;			/* Status from DCE calls */
    sec_login_handle_t lcontext;	/* Login context */
    boolean32 reset_passwd;		/* If password needs to be reset */
    sec_login_net_info_t net_info;	/* Network info for cred */
    sec_login_auth_src_t auth_src; 

    sec_login_context_from_pag(known_pag, &lcontext, &st);
    if (st) {
	log_dfsgw_msg(req, st, "Cannot find existing context for %s, pag %#x",
		      username, known_pag);
	return st;
    }
    
    /*
     * Add our TGT.
     */
    sec_login_krb5_add_cred(lcontext, creds, &st);
    if (st) {
	log_dfsgw_msg(req, 0,
		      "Error while adding refreshed ticket to existing context for %s, pag %#x", 			      
		      username, known_pag);
	return st;
    }	

    /*
     * Now "validate" the identity, usually we would pass a
     * password here, but...  
     */ 		
    
    if (sec_login_validate_identity(lcontext, 0, &reset_passwd, 
				    &auth_src, &st)) {
	if (st) {
	    log_dfsgw_msg(req, st, 
			  "Error on validating refreshed ticket for %s, pag %#x", 
			  username, known_pag);
	    return st;
	}
	
	/* 
	 * this is here to attempt at getting the creds file
	 * with correct owner id still need to find out how to
	 * get correct gid on creds files generated via gateway 
	 * server
	 */
	if (!sec_login_certify_identity(lcontext, &st)) {
	    log_dfsgw_msg(req, st, 
			  "Error on certifying refreshed ticket for %s, pag %#x", 
			  username, known_pag);
	    return st;
	} 
	if (reset_passwd) {
	    log_dfsgw_msg(req, 0, "Password must be changed for %s", 
			  username);
	}
	if (auth_src == sec_login_auth_src_local) {
	    log_dfsgw_msg(req, 0, "Credentials obtained from local registry for %s", username);
	}
	if (auth_src == sec_login_auth_src_overridden) {
	    log_dfsgw_msg(req, 0, "Validated %s from local override entry, no network credentials obtained", username);
	    return st;
	}
	/*
	 * Retrieve the exipration time.
	 */
	sec_login_inquire_net_info(lcontext, &net_info, &st);
	if ((st) && (st != sec_login_s_not_certified)) {
	    log_dfsgw_msg(req, st, "Unable to get expiration date for %s", username);
	    return st;	
	}
	*expiration_time = net_info.identity_expiration_date;
	sec_login_free_net_info(&net_info);

    } else {
	log_dfsgw_msg(req, st, "Can't validate refreshed identity for %s, pag %#x", username, known_pag);
	return st;
    }

    return 0;
}    

/*
 * install_cred - Install credentials.
 *
 * This function associates externally-obtained credentials with a local
 * dce username.
 * 
 * Inputs:
 *	username		: DCE username.	 Must be just the name, no 
 *				  realm.
 *	creds			: DCE creds, just a TGT.
 *	req			: Request structure for this packet, for error
 *				  reporting.
 * Outputs:
 *	expiration_time		: Expiration time of the resulting credentials.
 * 	pag			: Pag number for newly installed credentials.
 *
 * Return Value:
 *	0 on success, -1 on failure
 */
static unsigned32 install_cred
#ifdef __STDC__
    (char *username, krb5_creds *creds, as_req_p req, time_t *expiration_time,
     unsigned32 *pagp)
#else
(username, creds, req, expiration_time, pagp)
  char *username;
  krb5_cred *creds;
  as_req_p req;
  time_t *expiration_time;
  unsigned32 *pagp;
#endif /* __STDC__ */
{
    error_status_t st;			/* Status from DCE calls */
    sec_login_handle_t lcontext;	/* Login context */
    boolean32 reset_passwd;		/* If password needs to be reset */
    unsigned32 pag = 0;			/* PAG */
    sec_login_net_info_t net_info;	/* Network info for cred */
    sec_login_auth_src_t auth_src; 
    
    /*
     * Setup identity (login context).  Inform security code
     * that we will be supplying a TGT later, and that we are
     * proxying a cred.
     */
    if (debug)
	log_dfsgw_msg(req, 0, "INFO: Adding ticket for \"%s\"\n", username);
    
    if (sec_login_setup_identity((unsigned_char_p_t)username, 
				 (sec_login_external_tgt | 
				  sec_login_proxy_cred),
				 &lcontext, &st)) {
	/*
	 * Add our TGT.
	 */
	sec_login_krb5_add_cred(lcontext, creds, &st);
	if (st) {
	    log_dfsgw_msg(req, 0, 
			  "Error while adding ticket for %s", username);
	    return -1;
	}	
	
	/*
	 * Now "validate" the identity, usually we would pass a
	 * password here, but...
	 */
	
	if (sec_login_validate_identity(lcontext, 0, &reset_passwd, 
					&auth_src, &st)) { 
	    
	    if (st) {
		log_dfsgw_msg(req, st, "Validation error for %s", 
			      username);
		return -1;
	    }
	    /* 
	     * this is here to attempt at getting the creds file with
             * correct * owner id, gid is now correct due to security
             * code fix.
	     * Which SECURITY BUG FIX ???
	     * [ still need to find out how to get correct gid on ]
	     * [ creds files generated via gateway server         ]
	     */
	    
	    if (!sec_login_certify_identity(lcontext, &st)) {
		log_dfsgw_msg(req, st, "Certify error for %s", username);
		return -1;
	    } 
	    if (reset_passwd) {
		log_dfsgw_msg(req, 0, "Password must be changed for %s", 
			      username);
	    }
	    if (auth_src == sec_login_auth_src_local) {
		log_dfsgw_msg(req, 0, 
			      "Credentials obtained from local registry for %s", 
			      username);
	    }
	    if (auth_src == sec_login_auth_src_overridden) {
		log_dfsgw_msg(req, 0, 
			      "Validated %s from local override entry, no network credentials obtained", 
			      username);
		return -1;
	    }
	    /*
	     * Actually create the cred files.
	     */
	    sec_login_set_context(lcontext, &st);
	    if (st) {
		log_dfsgw_msg(req, st, "Unable to set context for %s", 
			      username);
		return -1;
	    }
	    /*
	     * Retrieve the resulting pag.
	     */
	    pag = sec_login_inq_pag(lcontext, &st);
	    if ((st) || (pag == 0)) {
		log_dfsgw_msg(req, st, "Unable get pag for %s", username);
		return -1;
	    }
	    /*
	     * Retrieve the exipration time.
	     */
	    sec_login_inquire_net_info(lcontext, &net_info, &st);
	    *expiration_time = net_info.identity_expiration_date;
	    sec_login_free_net_info(&net_info);
	    if ((st) && (st != sec_login_s_not_certified)) {
		log_dfsgw_msg(req, st, 
			      "Unable to get expiration date for %s", 
			      username);
		return -1;
	    }
	    /* Success */
	} else {
	    log_dfsgw_msg(req, st, "Can't validate identity for %s", 
			  username);
	    return -1;
	}
    } else {
	log_dfsgw_msg(req, st, "Can't setup identity for %s", username);
	return -1;
    }
    *pagp = pag;
    return 0;
}

/*
 * rd_krb5_req - Process KRB_AP_REQ message.
 *
 * This routine is the same as the kerberos krb5_rd_req_simple 
 * routine, except we don't use a replay cache.
 *
 * Inputs:
 *	inbuf		: Input request.
 *	server		: Server principal.
 *	sender_addr	: Client network address.
 * Outputs:
 *	authdata	: Pointer to authentication data.
 *
 * Returns krb5_error_code.
 */
krb5_error_code rd_krb5_req
#ifdef __STDC__
	(const krb5_data *inbuf, krb5_const_principal server, 
	 const krb5_address *sender_addr, krb5_tkt_authent **authdat)
#else 
	(inbuf, server, sender_addr, authdat)
	const krb5_data *inbuf;
	krb5_const_principal server;
	const krb5_address *sender_addr;
	krb5_tkt_authent **authdat;
#endif /* __STDC__ */
{
	krb5_error_code retval;
	krb5_ap_req *request;

	if (!krb5_is_ap_req(inbuf))
		return KRB5KRB_AP_ERR_MSG_TYPE;
	if (retval = decode_krb5_ap_req(inbuf, &request)) {
		return KRB5KRB_AP_ERR_BADVERSION; 
	}
	retval = krb5_rd_req_decoded(request, server,
				     sender_addr, 0,
				     0, 0, 0, authdat);
	krb5_free_ap_req(request);
	return(retval);
}

/*
 * send_dfsgw_reply - Construct and transmit a DFSGW_REPLY_PACKET.
 *
 * Inputs:
 *      dfsgw_version           : protocol version of dfsgw pkt
 *	req			: as_req_p of the request
 *	client_addr		: krb5_address of client
 *	ad			: Kerberos authenticator of original KRB_AP_REQ message
 *	code			: dfsgw reply code
 * Return value:
 *	void
 */
static void send_dfsgw_reply
#ifdef __STDC__
	(unsigned char dfsgw_version,as_req_p req, krb5_address *client_addr, krb5_tkt_authent *ad, int code)
#else
	(dfsgw_version, req, client_addr, ad, code)
        char dfsgw_version;
	as_req_p req;
	krb5_address *client_addr;
	krb5_tkt_authent *ad;
	int code;
#endif /* __STDC__ */
{
	char *p;			/* Packet surgery */
	krb5_data outbuf;		/* Encoded reply to client */
	krb5_data reply;		/* Reply to client */
	krb5_error_code kcode;		/* Kerberos errors */

	/*
	 * Create KRB_SAFE message for sending reply to client, informing of
	 * success/failure.  
	 *
	 * Packet contents:
	 *	unsigned char protocol_version
	 *	unsigned char DFSGW_REPLY_PACKET (with encoded byte order)
	 *	dfsgw_roundup
	 *	unsigned long reply_code
	 */
	reply.length = dfsgw_roundup(sizeof(unsigned long) + (sizeof(unsigned char) * 2));
	if ((reply.data = (char *)malloc(reply.length)) == NULL) {
		log_dfsgw_msg(req, errno, "Memory allocation failure while attempting to respond");
		return;
	} 
	p = reply.data;
        switch (dfsgw_version) {
        case DFSGW_PROTOCOL_VERSION :
	   *p++ = DFSGW_PROTOCOL_VERSION;
           break;
        case DFSGW_PROTOCOL_VERSION_1 :
           *p++ = DFSGW_PROTOCOL_VERSION_1;
           break;
        /*
         *  dfsgwd should return an error of DFSGW_REPLY_BAD_VERSION if
         *              protocol version not recognized by dfsgwd 
         */
        default :
           *p++ = dfsgw_version;
           code = DFSGW_REPLY_ERROR_BAD_VERSION;
           break;
        }
	*p++ = DFSGW_REPLY_PACKET | host_byte_order;
	p = (char *)dfsgw_roundup(((unsigned long)p));
	*(unsigned long *)p = code;
	if (kcode = krb5_mk_safe(&reply, 
			 CKSUMTYPE_RSA_MD4_DES,
			 ad->ticket->enc_part2->session, 
			 local_portaddr,  
			 client_addr,
			 0, KRB5_SAFE_NOTIME, 0, &outbuf)) {
		log_dfsgw_msg(req, kcode, "Error while creating KRB_SAFE reply message");
	} else if (outbuf.length > DFSGW_MAXRSIZE) {
		log_dfsgw_msg(req, 0, "Can't respond; KRB_SAFE reply message too big"); 
	} else {
		if (sendto(asock, outbuf.data, outbuf.length, 0, &req->client, sizeof(struct sockaddr)) < 0) {
			log_dfsgw_msg(req, 0, "sendto failure while responding: %s", strerror(errno));
		}
		xfree(outbuf.data);
	}
	xfree(reply.data);
	return;
}

/*
 * authorize_request - Authorize request given paramaters.
 *
 * In addition to the authentication and security given by Kerberos,
 * dfsgwd enforces other rules to ensure that attackers don't mess
 * with other people's credentials.  This routine performs those
 * checks, which basically are consistency checks.
 *
 * Inputs:
 *	req		: as_req_p of the request
 *	netaddr		: network address of the request
 *	len		: length of the network address
 *	username	: DCE username
 *	ad		: Request authenticator
 *	uid		: Unix UID of request
 * Return value:
 *	0 on sucess, -1 otherwise.
 */
static int authorize_request
#ifdef __STDC__
	(as_req_p req, char *netaddr, int len, char *username, krb5_tkt_authent *ad, uid_t uid)
#else
	(req, netaddr, len, username, ad, uid)
	as_req_p req;
	char *netaddr;
	int len;
	char *username;
	krb5_tkt_authent *ad;
	uid_t uid;
#endif /* __STDC__ */
{
	char *r;				/* String surgery */
	char *principalname = NULL;		/* Client Kerberos principal name */
	int retval = -1;			/* Assume failure */
	krb5_error_code code;			/* Kerberos errors */
	sec_rgy_login_name_t login_name;	/* DCE username, massaged */
	sec_rgy_login_name_t login_result;	/* DCE username, result */
	sec_rgy_cursor_t account_cursor;	/* Account cursor for perusal */
	sec_rgy_acct_key_t key_parts;		/* Relevant key parts */
	sec_rgy_sid_t id_sid;			/* Subject IDentifier for this account */
	sec_rgy_unix_sid_t unix_sid;		/* Unix subject identifier */
	sec_rgy_acct_user_t user_part;		/* User data for this account */
	sec_rgy_acct_admin_t admin_part;	/* Administrative data for this account */
	error_status_t st;			/* DCE status */

	/*
	 * Compare the network address encoded in the request to the machine
	 * that sent the message, and reject it if there is a mismatch.	 This prevents
	 * attackers from stomping on other machine's credentials.  
	 */
	if ((len != req->len) || 
	    (memcmp(netaddr, &req->client.sa_data[2], len))) { 
		log_dfsgw_msg(req, 0, "WARNING: Intruder alert. User claiming to have id of %d is attacking this service by attempting to change creds of another node", uid); 
		goto out;
	}
	/*
	 * Now check to ensure that the principal in the authenticator is equal
	 * to the dce username and that the resulting unix uid of the dce username is
	 * equal to the unix uid presented in the packet.  This prevents people from 
	 * installing/modifying credentials of other people from the same client.
	 */
	if (code = krb5_unparse_name(ad->authenticator->client, &principalname)) {
		log_dfsgw_msg(req, code, "error while unparsing client principal");
		goto out;
	}
	if (r = strchr(principalname, '@')) {
		*r = '\0';
	}
	if ((strlen(principalname) != strlen(username)) || 
	    (strcmp(principalname, username))) {
		log_dfsgw_msg(req, 0, "WARNING: Intruder alert.	Actual principal %s is attempting to change the credentials of dce principal %s", principalname, username);
		goto out;
	}
	/*
	 * Now compare the unix UID of the specified DCE username to the unix UID
	 * supplied.  This is necessary since we don't want people posing as DCE
	 * principals to use any other UIDs except their own.
	 *
	 * There's an obnoxious number of arguments we have to pass to this routine,
	 * most of which we don't use.
	 */
	memset(&login_name, 0, sizeof(login_name));
	strcpy((char *)&login_name.pname, username);
	sec_rgy_cursor_reset(&account_cursor);
	sec_rgy_acct_lookup(rgy_context, &login_name, &account_cursor, &login_result, &id_sid,
			    &unix_sid, &key_parts, &user_part, &admin_part, &st);
	if (st) {
		log_dfsgw_msg(req, st, "error while getting registry info for %s", username);
		goto out;
	}
	if (unix_sid.person != uid) {
		log_dfsgw_msg(req, 0, "WARNING: Intruder alert.	Actual principal %s is attempting to imposter as unix UID %d", principalname, uid);
		goto out;
	}
	retval = 0;
out:
	if (principalname)
		xfree(principalname);
	return(retval);
}

/*
 * process_krb_pkt - Authenticate and decrypt kerberos packet.
 *
 * In a single datagram, the KRB_AP_REQ and KRB_MK_PRIV messages are
 * piggybacked together.  We process the KRB_AP_REQ (authentication) and
 * decrypt the KRB_MK_PRIV.  We return the decrypted data, plus the
 * authentication information, later used to send an authenticator
 * back to the client.
 *
 * Inputs:
 *	req			: as_req_p of the request. 
 * Outputs:
 *	ad			: authenticator (must be deallocated later).
 *	client_addr		: address of client  
 * Return value:
 *	decrypted data on success, NULL otherwise.
 */
unsigned char *
process_krb_pkt
#ifdef __STDC__
	(as_req_p req, krb5_tkt_authent **ad, krb5_address *client_addr)
#else
	(req, ad, client_addr)
	as_req_p req;
	krb5_tkt_authent **ad;
	krb5_address *client_addr;
#endif /* __STDC__ */
{
	krb5_data msg;			/* Message being processed */
	krb5_data info;			/* Data in encrypted message */
	krb5_error_code code;		/* Kerberos error code */ 
	unsigned char *retval = NULL;	/* Return value from function */
	krb5_address *client_portaddr;	/* Client port address */

	/*
	 * The messages are prepended with krb5_data structures whose
	 * length field is valid.
	 */
	msg.length = ((dfsgw_data *)(req->data))->length;
	msg.data = (char *)(req->data + sizeof(dfsgw_data));
	client_addr->addrtype = req->client.sa_family;
	client_addr->length = sizeof(struct in_addr);
	client_addr->contents = (krb5_octet *)&req->client.sa_data[2];
	if (code = krb5_gen_portaddr(client_addr, (krb5_pointer)&req->client.sa_data, 
			      &client_portaddr)) {
		log_dfsgw_msg(req, code, "Error while generating port address");
		goto out;
	}
	/*
	 * Process KRB_AP_REQ message.
	 */ 
	if ((code = rd_krb5_req(&msg, sprinc, client_addr, ad))) {
		log_dfsgw_msg(req, code, "Error while processing AP_REQ");
		goto out;
	}
	/*
	 * Process KRB_PRIV message.
	 */
	msg.data += msg.length + sizeof(dfsgw_data);
	msg.data = (char *)dfsgw_roundup(msg.data);
	msg.length = ((dfsgw_data *)(msg.data - sizeof(dfsgw_data)))->length;
	if ((code = krb5_rd_priv(&msg, (*ad)->ticket->enc_part2->session, client_portaddr, 0, 0, KRB5_PRIV_NOTIME,
				0, 0, &info))) {
		log_dfsgw_msg(req, code, "Error while processing PRIV message");
		goto out;
	}
	retval = (unsigned char *)info.data;
out:
	krb5_free_address(client_portaddr);
	return(retval);
}

/* 
 * process_rauth_req - Process removal of authentication request.
 * 
 * Inputs:
 *	req		: as_req_p of the request
 *	ad		: Kerberos authenticator
 *	pkt		: Authenticated and decrypted packet
 *	swap		: Code to swap data or not
 * Return Value:
 *	0 on suceess, -1 otherwise.
 */
static int process_rauth_req
#ifdef __STDC__
	(as_req_p req, krb5_tkt_authent *ad, unsigned char *pkt, int swap)
#else
	(req, ad, pkt, swap)
	as_req_p req;
	krb5_tkt_authent *ad;
	unsigned char *pkt;	
	int swap;
#endif /* __STDC__ */
{
	char *username = NULL;			/* DCE username */
	int len;				/* Length of various things */
	int success = -1;			/* Assume failure */
	uid_t uid;				/* Unix user ID in packet */
	unsigned char *p;			/* Pointer into packet */
	at_pag_mapping_t mapping;		/* Mapping for RAR */
	char *netaddr = NULL;			/* Network address */
	unsigned32 pag;				/* Resulting PAG (from kernel) */
	error_status_t st;			/* Status from DCE calls */
	sec_login_handle_t lcontext;		/* Login context */
        unsigned char dfsgw_version;            /* protocol version sent from dfs_login */
        char *clientsysname = NULL;             /* remote client @sys value in pkt version 2 */
        char clienthostname[MAXHOSTNAMELEN];    /* remote client @host value in pkt version 2 */
        struct hostent *h;                      /* host info from gethostbyaddr */
        extern int basehostname;                /* hostname w/o domain part */
        size_t baselen;
        

	memset((char *)&mapping, 0, sizeof(at_pag_mapping_t));
	/*
	 * Strip out information.  Packet is:
	 *
	 *	unsigned char DFSGW_PROTOCOL_VERSION
	 *	unsigned char DFSGW_RAUTH_PACKET (host_byte_order encoded in lsb) 
	 *	(round up to longword boundary)
	 *	unsigned long dce_username_len
	 *	<dce_username>		  
	 *	(round up to longword boundary)	 
	 *	unsigned long networkAddrLen
	 *	<networkAddr>
	 *	(round up to longword boundary) 
	 *	unsigned long unix user ID  
         *      (round up to longword boundary)
         *      unsigned long sysname_len       version 2
         *      <sysname>                       version 2
         *      (round up to longword boundary) version 2
         */
        dfsgw_version = (unsigned char) *pkt;
	p = pkt;
	p = (unsigned char *)dfsgw_roundup(((unsigned long)++p));
	memcpy(&len, p, sizeof(unsigned long));
	if (swap) {
		len = dfsgw_wswap(len);
	}
	if ((username = (char *)malloc(len+1)) == NULL) {
		log_dfsgw_msg(req, 0, "Memory allocation failure while processing request");
		goto out;
	}
	memcpy(username, (p + sizeof(unsigned long)), len);
	username[len] = '\0';
	p += (sizeof(unsigned long) + len);
	p = (unsigned char *)dfsgw_roundup(((unsigned long)p));
	memcpy(&len, p, sizeof(unsigned long));
	if (swap) {
		len = dfsgw_wswap(len);
	}
	p += sizeof(unsigned long);
	if ((netaddr = malloc(len+1)) == NULL) {
		log_dfsgw_msg(req, 0, "Memory allocation failure while processing request");
		goto out;
	}		
	memcpy(netaddr, p, len);
	p += dfsgw_roundup(len);	
	memcpy(&uid, p, sizeof(uid_t));
	if (swap) {
		uid = dfsgw_wswap(uid);
	}

        if (authorize_request(req, netaddr, len, username, ad, uid) < 0) {
                log_dfsgw_msg(req, 0, "Request failure");
                goto out;
        }
        switch (dfsgw_version) {
        case DFSGW_PROTOCOL_VERSION :
           /*
            * @ sys Sysname
            */
            p += sizeof(unsigned long);
            memcpy(&len, p , sizeof(unsigned long));
            if (swap) {
                    len = dfsgw_wswap(len);
            }
            p += sizeof(unsigned long);
            if ((clientsysname = malloc(len+1)) == NULL) {
                    log_dfsgw_msg(req, 0, "Memory allocation failure while processing request");
                    goto out;
            }
            memcpy(clientsysname, p, len);
            clientsysname[len] =  '\0';
            p += dfsgw_roundup(len);
            p += sizeof(unsigned long);

           /*
            * @host hostname value
            */
            if ((h = gethostbyaddr(netaddr, 4, AF_INET)) == 0) {
                    log_dfsgw_msg(req, 0, "Unable to obtain client netid hostname");
                    goto out;
            }
            if (basehostname) {
                /*
                 * don't use domain part of name
                 */
                 strncpy((char *)clienthostname, h->h_name, baselen = strcspn(h->h_name, "."));
                 clienthostname[baselen] = '\0';
            }
            else
                 strcpy((char *)clienthostname, h->h_name);
            break;
        default :
            p += (sizeof(unsigned long) * 2);
            break;
        }

	/*
	 * Do an AT_QUERY to get the stored pag.
	 */
	mapping.uid = uid;
	mapping.addr.sa_family = req->client.sa_family;
	memcpy((char *)&mapping.addr.sa_data, 
	       (char *)&req->client.sa_data[2], 
	       req->len);
	if (afs_syscall(AFSCALL_AT, AT_QUERY, (long)&mapping, 
			NULL, NULL, NULL) < 0) {
		log_dfsgw_msg(req, 0, "Couldn't find RAR for %s:%s: %s", 
			      clienthostname, username, strerror(errno));
		goto out;
	} else {
		pag = mapping.pag;
	}
	/*
	 * Now remove the mapping from the kernel.
	 */
	if (afs_syscall(AFSCALL_AT, AT_DELETE, (long)&mapping, 
			NULL, NULL, NULL) < 0) {
		log_dfsgw_msg(req, 0, "Couldn't remove RAR for %s:%s: %s", 
			      clienthostname, username, strerror(errno));
	} else {
		log_dfsgw_msg(req, 0, "RAR deleted: %d", uid);
	}
	/*
	 * Obtain pointer to cached login context, then destroy.
	 */
	sec_login_context_from_pag(pag, &lcontext, &st);
	if (st) {
		log_dfsgw_msg(req, st, "Error while fetching stored context");
		goto out;
	}
	sec_login_purge_context(&lcontext, &st);
	if (st) {
		log_dfsgw_msg(req, st, "Error while destroying context");
		goto out;
	}
	success = 0;
out:
	/*
	 * Free up all of our mess.   
	 */
	if (netaddr)
		free(netaddr);
	if (username) 
		free(username);
        if (clientsysname)
                free(clientsysname);
	return(success);
}
 
/*
 * process_auth_req - Process authentication request.
 *
 * This function processes an authentication request. 
 *
 * Inputs:
 *	req		: as_req_p of the request
 *	ad		: Kerberos authenticator
 *	pkt		: Authenticated and decrypted packet
 *	swap		: Code to swap data or not
 * Return Value:
 *	0 on success, -1 otherwise.
 */
static int process_auth_req
#ifdef __STDC__
    (as_req_p req, krb5_tkt_authent *ad, unsigned char *pkt, int swap)
#else
(req, ad, pkt, swap)
  as_req_p req;
  krb5_tkt_authent *ad;
  unsigned char *pkt;	
  int swap;
#endif /* __STDC__ */
{	
    char *username = NULL;		/* DCE username */
    int len;				/* Length of various things */
    int code = -1;			/* Assume failure */
    krb5_creds tgt;			/* Unpacked TGT */
    uid_t uid;				/* Unix user ID in packet */
    unsigned char *p;			/* Pointer into packet */
    unsigned32 pag = 0;			/* The PAG itself */
    at_pag_mapping_t mapping;		/* Mapping for RAR */
    char *netaddr = NULL;		/* Network address */
    time_t expiration_time;		/* Lifetime of cred */
#ifdef _POSIX_REENTRANT_FUNCTIONS
    char timestr[CTIME_STRLEN];		/* Time string for reentrant ctime_r */
    char mtimestr[CTIME_STRLEN];
#else
    char *timestr;                      /* Time string for ctime */
    char *mtimestr;
#endif
    unsigned char dfsgw_version;        /* protocol version sent from dfs_login */
    char *clientsysname = NULL;         /* dfs_login @sys value  */
    extern int basehostname;            /* hostname w/o domain part */
    size_t baselen;
    struct hostent *h;                  /* host info from gethostbyaddr */
    char clienthostname[MAXHOSTNAMELEN];     
    int known_context = 0;
    error_status_t st;                      /* Status from DCE calls */
    sec_login_handle_t lcontext;            /* Login context */
    
    memset((char *)&mapping, 0, sizeof(at_pag_mapping_t));
    memset((char *)&tgt, 0, sizeof(tgt));
    memset(&clienthostname, 0, OSI_MAXHOSTNAMELEN);

    /*
     * Strip out information.  Packet is:
     *
     *	unsigned char DFSGW_PROTOCOL_VERSION 
     *	unsigned char DFSGW_AUTH_PACKET (host_byte_order encoded in lsb) 
     *	(round up to longword boundary)
     *	unsigned long dce_username_len
     *	<dce_username>		  
     *	(round up to longword boundary)	 
     *	unsigned long networkAddrLen
     *	<networkAddr>
     *	(round up to longword boundary) 
     *	unsigned long unix user ID  
     *      (round up to longword boundary)
     *      unsigned long sysname_len       version 2
     *      <sysname>                       version 2
     *      (round up to longword boundary) version 2
     *	unsigned long size of packed TGT
     *	packed TGT 
     *
     */	

    dfsgw_version = (unsigned char) *pkt;
    p = pkt;
    p = (unsigned char *)dfsgw_roundup(((unsigned long)++p));
    memcpy(&len, p, sizeof(unsigned long));
    if (swap) {
	len = dfsgw_wswap(len);
    }
    if ((username = (char *)malloc(len+1)) == NULL) {
	log_dfsgw_msg(req, 0,
		      "Memory allocation failure while processing request");
	goto out;
    }
    memcpy(username, (p + sizeof(unsigned long)), len);
    username[len] = '\0';
    p += (sizeof(unsigned long) + len);
    p = (unsigned char *)dfsgw_roundup(((unsigned long)p));
    memcpy(&len, p, sizeof(unsigned long));
    if (swap) {
	len = dfsgw_wswap(len);
    }
    p += sizeof(unsigned long);
    if ((netaddr = malloc(len+1)) == NULL) {
	log_dfsgw_msg(req, 0, 
		      "Memory allocation failure while processing request");
	goto out;
    }		
    memcpy(netaddr, p, len);
    p += dfsgw_roundup(len);	
    memcpy(&uid, p, sizeof(uid_t));
    if (swap) {
	uid = dfsgw_wswap(uid);
    }
    if (authorize_request(req, netaddr, len, username, ad, uid) < 0) {
	log_dfsgw_msg(req, 0, "Request failure");
	goto out;
    }
    switch (dfsgw_version) {
      case DFSGW_PROTOCOL_VERSION: 
	/*
	 * @sys Sysname
	 */
	p += sizeof(unsigned long);
	memcpy(&len, p, sizeof(unsigned long));
	if (swap) {
	    len = dfsgw_wswap(len);
	}
	p += sizeof(unsigned long);
	if ((clientsysname = malloc(len+1)) == NULL) {
	    log_dfsgw_msg(req, 0, 
			  "Memory allocation failure while processing request");
	    goto out;
	}
	memcpy(clientsysname, p, len);
	clientsysname[len] =  '\0';
	p += dfsgw_roundup(len);
	p += sizeof(unsigned long);
	if (len == 0) {
	    strcpy((char *)&mapping.sysname, (char *)sysname);
	} else {
	    strcpy((char *)&mapping.sysname, (char *)clientsysname);
	}
	
	/*
	 * @host hostname value 
	 */
	if ((h = gethostbyaddr(netaddr, 4, AF_INET)) == 0) {    
	    log_dfsgw_msg(req, 0, "Unable to obtain client netid hostname");
	    goto out;
	}
	if (basehostname) {
	    /*
	     * don't use domain part of name 
	     */
	    baselen = strcspn(h->h_name, ".");
	    strncpy((char *)clienthostname, h->h_name, baselen);
	    clienthostname[baselen] = '\0';
	} else {
	    strcpy((char *)clienthostname, h->h_name);
	}
	strcpy((char *)&mapping.hostname, (char *)clienthostname);

	break;
	
      default:
	p += (sizeof(unsigned long) * 2);
	strcpy((char *)&mapping.sysname, (char *)sysname);
	strcpy((char *)&mapping.hostname, (char *)clienthostname);
	break;
    }
    if (tgt_unpack((char *)p, &tgt, swap)) {
	log_dfsgw_msg(req, 0, "Error while unpacking TGT");
	goto out;
    }
    
    mapping.uid = uid;
    mapping.addr.sa_family = req->client.sa_family;
    memcpy((char *)&mapping.addr.sa_data,
	   (char *)&req->client.sa_data[2], 
	   req->len);
    
    if (afs_syscall(AFSCALL_AT, AT_QUERY, (long)&mapping, 
		    NULL, NULL, NULL) == 0) {
	known_context = 1;
	pag = mapping.pag;
	osi_assert(pag != 0);
    } 
    
    /*
     * Install the credentials by proxy into this users's 
     * ticket cache.
     */
    
    if (known_context) {
	code = update_cred(username, &tgt, req, &expiration_time, pag);
	
	if (code == 0) {
	    mapping.uid = uid;
	    mapping.pag = pag;
	    mapping.addr.sa_family = req->client.sa_family;
	    memcpy((char *)&mapping.addr.sa_data,
		   (char *)&req->client.sa_data[2], 
		   req->len);
	    mapping.expiration_time = expiration_time;
	    if (strcmp((char *)&mapping.sysname,
		       strlen(clientsysname) ? 
		       (char *)clientsysname : (char *)sysname)) 
	      strcpy((char *)&mapping.sysname,
                     strlen(clientsysname) ? 
		     (char *)clientsysname : (char *)sysname);
	    if (strcmp((char *)&mapping.hostname, (char *)clienthostname)) 
	      strcpy((char *)&mapping.hostname, (char *)clienthostname);

	    /* Update expiration time */
	    if (afs_syscall(AFSCALL_AT, AT_ADD, (long) &mapping, 
			    AT_ADD_UPDATE_EXPTIME, NULL, NULL) < 0) {
		log_dfsgw_msg(req, 0, 
			      "Couldn't update expiration time of RAR for %s, pag %#x, errno %s ", 
			      username, pag, strerror(errno));
		code = -1;
	    } else {
#ifdef _POSIX_REENTRANT_FUNCTIONS
		ctime_r(&expiration_time, timestr, CTIME_STRLEN);
		timestr[24] = '\0';
#else
		timestr = ctime(&expiration_time);
#endif
		log_dfsgw_msg(req, 0, 
			      "RAR expiration time updated: %d => 0x%x until %s, pag %#x", 
			      uid, pag, timestr, pag);
	    }
#if 0
	} else if (code == rpc_s_auth_tkt_expired) {
#endif
	} else {
	    /* 
	     * Destroy existing context that could not be updated. 
	     * A new one will be created.
	     */
	    log_dfsgw_msg(req, 0, 
			  "context update for pag %#x failed, try to destory current context and replace with new context", pag);
	    sec_login_context_from_pag(pag, &lcontext, &st);
	    if (st) {
		log_dfsgw_msg(req, st, "Could not destory context for pag %#x on which update failed, as error in fetching the context", pag);
	    } else {
		sec_login_purge_context(&lcontext, &st);
		if (st) {
		    log_dfsgw_msg(req, st, "Error in destroying context for pag %#x on which update failed", pag);
		    code = -1;
		    goto out;
		}
	    }
	}
#if 0
	} else {
	    log_dfsgw_msg(req, code, "Error in updating context for pag %#x on which update failed", pag);
	    goto out;
	}
#endif
    }

    if (!known_context || (code != 0)) {
	pag = 0;
	code = install_cred(username, &tgt, req, &expiration_time, 
			    &pag);
	if (code == 0) {
	    osi_assert(pag != 0);
	    /*
	     * Set up the RAR and make the AFS syscall.
	     */
	    mapping.uid = uid;
	    mapping.pag = pag;
	    mapping.addr.sa_family = req->client.sa_family;
	    mapping.expiration_time = expiration_time;
	    memcpy((char *)&mapping.addr.sa_data, 
		   (char *)&req->client.sa_data[2], 
		   req->len);
	    strcpy((char *)&mapping.sysname, 
		   strlen(clientsysname) ? 
		   (char *)clientsysname : (char *)sysname);
	    strcpy((char *)&mapping.hostname, (char *)clienthostname);
	    if (afs_syscall(AFSCALL_AT, AT_ADD, (long)&mapping, 
			    AT_ADD_NEW_ENTRY, NULL, NULL) < 0) {
	      log_dfsgw_msg(req, 0, "Couldn't make RAR for %s:%s: %s",
			    clienthostname, username, strerror(errno));
	      code = -1;
	    } else {
#ifdef _POSIX_REENTRANT_FUNCTIONS
		ctime_r(&expiration_time, timestr, CTIME_STRLEN);
		timestr[24] = '\0';
#else
		timestr = ctime(&expiration_time);
#endif
		log_dfsgw_msg(req, 0, "RAR formed: %s:%s => 0x%x until %s", 
			      clienthostname, username, pag, timestr);
	    }
	}
    }

  out:
    /*
     * Free up all of our mess.   
     */
    krb5_free_cred_contents(&tgt);
    if (netaddr)
	free(netaddr);
    if (username) 
	free(username);
    if (clientsysname)
	free(clientsysname);

    return(code);
}

/*
 * dispatch_req - Dispatch dfsgw request.
 *
 * This function is the control thread that decodes/decrypts the request, then
 * dispatches it to the proper routine given the type of message.  It then sends
 * the result back to the client.
 *
 * Inputs:
 *	req		: as_req_p of the request
 * Return Value:
 *	DOES NOT RETURN - pthread_exits instead.
 */
static void dispatch_req
#ifdef __STDC__
	(as_req_p req)
#else
	(req)
	as_req_p req;
#endif /* __STDC__ */
{
	krb5_tkt_authent *ad = NULL;		/* Authentication data */
	krb5_address client_addr;		/* Client address */
	unsigned char *pkt;			/* Pointer into packet */
	int swap;				/* If should byte swap */
	int rcode = -1;				/* Assume failure */

	/*
	 * Authenticate and decrypt message. 
	 */
	if ((pkt = process_krb_pkt(req, &ad, &client_addr)) == NULL) {
		goto out;
	}
	/*
	 * Check protocol version, extract packet type.
	 */
	if ((*pkt != DFSGW_PROTOCOL_VERSION) && (*pkt != DFSGW_PROTOCOL_VERSION_1)) {
		log_dfsgw_msg(req, 0, "Protocol violation");
		goto out;
	}
	swap = dfsgw_endian(*(pkt + 1)) != host_byte_order;
	switch (dfsgw_msg(*(pkt + 1))) {
	case DFSGW_AUTH_PACKET :
		rcode = process_auth_req(req, ad, pkt, swap);
		break;
	case DFSGW_RAUTH_PACKET :
		rcode = process_rauth_req(req, ad, pkt, swap);
		break;
	default :
		log_dfsgw_msg(req, 0, "Protocol violation - unknown request");
		break;
	}
out:		
	/*
	 * If we were able to decrypt the AP_REQ message, then send a reply.  Otherwise
	 * just ignore the client - could be an attacker.
	 */
	if (ad) {
		send_dfsgw_reply((unsigned char)*pkt, req, &client_addr, ad, (rcode == 0) ? DFSGW_REPLY_OK : DFSGW_REPLY_ERROR);
		krb5_free_tkt_authent(ad);				 
	}
	/*
	 * Free up all other storage.
	 */ 
	if (pkt)
		xfree(pkt);
	free(req);
	pthread_mutex_lock(&nthread_lock);
	nthreads--;
	pthread_mutex_unlock(&nthread_lock);
	pthread_exit((pthread_addr_t)(rcode));
}

/*
 * as - Authentication service.
 *
 * This is the main control thread for the server.  It listens for the
 * first authentication packet from a client, then kicks off another thread
 * to handle the request.  Upon reciept of a signal it synchronizes the 
 * smooth shutdown of the server.
 *
 * Return value:
 *	-1 on any error, 0 otherwise.
 */
int as()
{
	fd_set readable;		/* Mask for select(2) */
	struct timeval timeout;		/* Timeout for select(2) */
	int lnthreads;			/* Local number of threads */
	as_req_p req;			/* Request */
	int len;			/* Length for client addr (recvfrom) */
	pthread_t thread;		/* Thread structure */
	int st;				/* Status from select(2) */

	req = NULL;
#if 0
	/*
	 * Setup signal handler for graceful shutdown.
	 */
	signal(SIGINT, gotsignal);
	signal(SIGTERM, gotsignal);
	signal(SIGQUIT, gotsignal);
#endif
	timeout.tv_usec = 0;
	timeout.tv_sec = AS_SEL_TIMEOUT;
	while (!shutdown_as) {
		/*
		 * Allocate free buffer(s) if necessary.
		 */
		if ((!req) && ((req = (as_req_p)malloc(sizeof(as_req_t))) == NULL)) {
			fatal("memory allocation failure");
		}
		/*
		 * Do a select on the service socket, don't rely on the OS
		 * to pop us out of recvfrom upon receipt of a signal.
		 */
		FD_ZERO(&readable);
		FD_SET(asock, &readable);
		if ((st = select(asock + 1,
#ifdef AFS_HPUX_ENV
				 (int *)&readable,
#elif AFS_SUNOS5_ENV
				 (fd_set *)&readable,
#else
				 &readable,
#endif
				 0, 0, &timeout)) > 0) {
			/*
			 * Read request from client.
			 */	
			len = sizeof(struct sockaddr);
			if (recvfrom(asock, (char *) req->data, (int)DFSGW_MAXPSIZE, 0, &req->client, &len) > 0) {
				/* 
				 * Send off to worker thread.
				 */
				req->len = sizeof(unsigned long);	
				pthread_mutex_lock(&nthread_lock);
				if (!pthread_create(&thread, pthread_attr_default, (pthread_startroutine_t)dispatch_req, req)) {
					nthreads++;
					pthread_detach(&thread);
				} else {
					log("Error %d while attempting to create thread", errno);
					free(req);
				}
				pthread_mutex_unlock(&nthread_lock);

				req = NULL;	
			} else {
				log("Error: %s: while doing recvfrom", strerror(errno));
			}
		} else if ((st == -1) && (errno != EINTR)) {
			log("Error: %s: during select", strerror(errno));
			shutdown_as++;
		}
	}
	/*
	 * Shut system down gracefully, wait for all worker threads
	 * to exit.
	 */
	do {
		pthread_mutex_lock(&nthread_lock);
		lnthreads = nthreads;
		pthread_mutex_unlock(&nthread_lock);
	} while (lnthreads);
	return(0);
}

/*
 * These routines were stolen out of the kerberos source, apparently we can't 
 * get to them through libdce.
 */ 
krb5_error_code
krb5_make_full_ipaddr(DECLARG(krb5_int32, adr),
		      DECLARG(krb5_int16, port),
		      DECLARG(krb5_address **,outaddr))
OLDDECLARG(krb5_int32, adr)
OLDDECLARG(krb5_int16, port)
OLDDECLARG(krb5_address **,outaddr)
{
    unsigned long smushaddr = (unsigned long) adr; /* already in net order */
    unsigned short smushport = (unsigned short) port; /* ditto */
    register krb5_address *retaddr;
    register krb5_octet *marshal;
    krb5_addrtype temptype;
    krb5_int32 templength;

    if (!(retaddr = (krb5_address *)malloc(sizeof(*retaddr)))) {
	return ENOMEM;
    }
    retaddr->addrtype = ADDRTYPE_ADDRPORT;
    retaddr->length = sizeof(smushaddr)+ sizeof(smushport) +
	2*sizeof(temptype) + 2*sizeof(templength);

    if (!(retaddr->contents = (krb5_octet *)malloc(retaddr->length))) {
	xfree(retaddr);
	return ENOMEM;
    }
    marshal = retaddr->contents;

    temptype = htons(ADDRTYPE_INET);
    (void) memcpy((char *)marshal, (char *)&temptype, sizeof(temptype));
    marshal += sizeof(temptype);

    templength = htonl(sizeof(smushaddr));
    (void) memcpy((char *)marshal, (char *)&templength, sizeof(templength));
    marshal += sizeof(templength);

    (void) memcpy((char *)marshal, (char *)&smushaddr, sizeof(smushaddr));
    marshal += sizeof(smushaddr);

    temptype = htons(ADDRTYPE_IPPORT);
    (void) memcpy((char *)marshal, (char *)&temptype, sizeof(temptype));
    marshal += sizeof(temptype);

    templength = htonl(sizeof(smushport));
    (void) memcpy((char *)marshal, (char *)&templength, sizeof(templength));
    marshal += sizeof(templength);

    (void) memcpy((char *)marshal, (char *)&smushport, sizeof(smushport));
    marshal += sizeof(smushport);

    *outaddr = retaddr;
    return 0;
}

krb5_error_code
krb5_gen_portaddr(addr, ptr, outaddr)
const krb5_address *addr;
krb5_const_pointer ptr;
krb5_address **outaddr;
{
    krb5_int32 adr;
    krb5_int16 port;

    if (addr->addrtype != ADDRTYPE_INET)
	return KRB5_PROG_ATYPE_NOSUPP;
    port = *(krb5_int16 *)ptr;
    
    memcpy((char *)&adr, (char *)addr->contents, sizeof(adr));
    return krb5_make_full_ipaddr(adr, port, outaddr);
}
