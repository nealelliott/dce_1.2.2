/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/gateway/dfs_login/RCS/dfs_logout.c,v 1.7 1996/04/23 20:23:11 rajesh Exp $ */

/*
 * dfs_logout - Destroy remote DFS authentication relationship
 *
 * This client program is used to destroy authentication information
 * on a gateway node.  It also destroys the local kerberos ticket cache.
 * This program is suitable to run at logout.
 *
 * To do the removal, three pieces of information are sent to the
 * remote server: the user id of the remote user, a DCE login name,
 * and a network address.  This is all that is needed to remove a
 * authentication relationship.
 *
 * This program tries to be as easy to use as possible.  It must
 * know the name of the node where the dfsgwd server lives.  This node
 * must also be the same node that is running the file server translator
 * software, so it tries to determine this by examining mount information
 * (where and how DFS has been mounted).  It also must know the name
 * of the dce principal; it assumes the current login name unless prompted
 * otherwise.
 *
 * Usage: dfs_logout [ -h <hostname> ] [ <dce_user_name> ]
 */

/* 
 * Cannot include dcedfs/osi.h and dcedfs/osi_net.h in place of the
 * following since this file is not built in a DCE/DFS sandbox 
 * but instead is copied over into the kerberos source tree under the
 * clients subdirectory to be built.
 */
#include <sys/param.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <com_err.h>
#include <pwd.h>
#include "sendrcv.h"
#include "dfsgw.h"
#include "common.h"

#if defined(i386) || defined(vax) || defined(alpha)
#define host_byte_order         DFSGW_LITTLE_ENDIAN
#else
#define host_byte_order         DFSGW_BIG_ENDIAN
#endif

/*
 * Global data.
 */
char *progname;				/* Program name */
int debug;				/* Debugging flag */
/*
 * External data.
 */
extern krb5_deltat krb5_clockskew;

/*
 * usage - Print out syntax.
 *
 * This routine prints out the usage of the program and an optional
 * message.
 *
 * Inputs:
 *	msg		: Optional message to print.
 * Return value:
 *	void
 */
static void usage
#ifdef __STDC__
	(char *msg)
#else
	(msg)
	char *msg;
#endif
{
	if (msg) {
		com_err(progname, 0, "%s", msg);
	}
	com_err(0, 0, "Usage: %s [ -h <hostname> ] [ <dce_user_name> ]", progname);
	com_err(0, 0, "Where:");
	com_err(0, 0, "\t-h specifies an alternate remote authentication host");
	com_err(0, 0, "\tdce_user_name is the dce login name");
	exit(0);
}

/*
 * main - Entry point for dfs_logout.
 *
 * This function drives the whole client program.
 */
int main
#ifdef __STDC__
	(int argc, char **argv)
#else
	(argc, argv)
	int argc;
	char **argv;
#endif
{
	char *service = DFSGW_SERVICE;		/* Service name of host */
	char *server_name = DFSGW_SERVER;	/* Server name of host */
	char *username = NULL;			/* DCE user name */
	char *hostname = NULL;			/* Target host */
        char *sysname = "";                     /* @sys name of client */
	int len;				/* Lengths */
	int asock;				/* Socket for dfsgwd */
	struct servent *sp, default_serv;	/* Service info entry */
	struct sockaddr_in addr;		/* Host network address */
	struct hostent *hp;			/* Info about various hosts */
	char localname[MAXHOSTNAMELEN];		/* Our local machine name */
	char fullhostname[MAXHOSTNAMELEN];	/* Saved full name of host */
	unsigned long *data;			/* Our data packet */
	unsigned char *cdata;			/* Character pointer into packet */
	int off;				/* Offset for building data packet */
	int total;				/* Total size of packet */
	char *p;				/* String surgery */
	struct sockaddr_in c_sock;		/* Client full address */
	struct in_addr serveraddr;		/* Server internet address */
	int code;				/* Status of krb calls */
	char *c_realm;				/* Local (client) realm */
	char **s_realms;			/* Remote (server) realms */
	krb5_checksum send_cksum;		/* Checksum of packet */
	krb5_ccache ccdef;			/* Our local cred cache */
	krb5_principal server;			/* Server principal */
	krb5_data ap_msg, auth_msg, cat_msg;   	/* Messages to send over */
	dfsgw_data dfsgw_ap_msg, dfsgw_auth_msg; /* Server-independent format */
	krb5_data auth_pkt;			/* Packed authentication packet */
	krb5_creds screds;			/* Creds to user server */
	krb5_address server_addr, client_addr;  /* Address of server, client */
	krb5_address *port_clientaddr;		/* Full address of client */
	krb5_address *port_serveraddr;		/* Full address of server */
	char replydata[DFSGW_MAXRSIZE];		/* KRB_SAFE reply data */
	krb5_data reply;			/* Format reply for krb5_rd_safe */
	krb5_data authok;			/* Actual response */
	krb5_timestamp currenttime;             /* Current time */
        unsigned char dfsgw_version;            /* DFSGW_PROTOCOL_VERSION */
        int use_service = 0;                      /* use service in pricipal vs. new
                                                   OSF server-name in principal */

	krb5_init_ets();

	if (progname = strrchr(argv[0], '/'))
            progname++;
        else
            progname = argv[0];

        if (getenv("DFSGWSERVICE"))
            service = getenv("DFSGWSERVICE");

	/*
	 * Collect arguments.
	 */
	argv++;
	while (*argv) {
		if (**argv == '-') {
			switch (*++*argv) {
			case 'h' :
				if (!(hostname = *++argv)) {
					usage("-h requires an argument");
				}
				break;
			default :
				usage("unknown argument");
				break;
			}
		} else {
			break;
		}
		argv++;
	}
	/*
	 * Remaining is taken as username.
	 */
	if (*argv) {
		username = *argv++;
	}
	/*
	 * Resolve username.
	 */
	if (!username) {
		if (!(username = default_username())) {
			com_err(progname, 0, "cannot determine default username");
			usage("Please reinvoke supplying username argument");
		}
	}
	/*
	 * Resolve hostname.
 	 */
	if (!hostname) {
		if (!(hostname = default_hostname())) {
			com_err(progname, 0, "cannot determine default host");
			usage("Please reinvoke supplying -h <hostname> argument");
		}
	}
	/*
	 * Setup network communications to host.
	 */
	if ((asock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		fatal("can't create socket");
	}
	if ((sp = getservbyname(service, "udp")) == NULL) {
#ifdef DFSGW_SERVICE_DEFAULT
                default_serv.s_port = DFSGW_SERVICE_DEFAULT;
                sp = &default_serv;
#else
		com_err(progname, errno, "while obtaining service %s/udp (unknown maybe?)", service);
		fatal(NULL);
#endif
	}
	if ((hp = gethostbyname(hostname)) == NULL) {
		com_err(progname, errno, "while finding network address of %s", hostname);
		fatal(NULL);
	}
	strcpy(fullhostname, hp->h_name);
	memcpy((char *)&serveraddr, hp->h_addr, hp->h_length);
	server_addr.addrtype = hp->h_addrtype;
	server_addr.length = hp->h_length;
	server_addr.contents = (krb5_octet *)&serveraddr;
	/*
	 * Lower-case saved hostname for later kerberos calls.
	 */
	for (p = fullhostname; *p; p++) {
		if (isupper(*p)) {
			*p = tolower(*p);
		}
	}
	memset((char *)&addr, 0, sizeof(addr));
	addr.sin_family = hp->h_addrtype;
	addr.sin_port = sp->s_port;
	memcpy(&addr.sin_addr, hp->h_addr, hp->h_length);
	if (connect(asock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                com_err(progname, 0, "can't bind to %s@s (port %d)", service, hp->h_name, sp->s_port);
		fatal(NULL);
	}
	/*
	 * Massage username.  For some reason we can't send
	 * it over as <username@realm>, must use just username.
	 */
	if ((p = strchr(username, '@')) != NULL) {
		*p = '\0';
	}
	/*
	 * Get network address statistics.
	 */
	if (gethostname(localname, sizeof(localname)) < 0) {
		fatal("Couldn't get local hostname");
	}
	/*
	 * Build remote revokation packet:
	 *
 	 *      unsigned char DFSGW_PROTOCOL_VERSION
	 *      unsigned char DFSGW_RAUTH_PACKET (host_byte_order encoded in lsb)
 	 *      (round up to longword boundary)
	 * 	unsigned long dce_username_len
	 *      <dce_username>
 	 * 	(round up to longword boundary)
	 *      unsigned long networkAddrLen
	 *      <networkAddr>
	 *      (round up to longword boundary)
	 *	unsigned long unix user ID
         *      (round up to longword boundary)      version 2
         *      unsigned long sysname_len            version 2
         *      <sysname>                            version 2
         *      (round up to longword boundary)      version 2
	 *
	 */
#ifdef BAD_PROTOCOL_TEST
        dfsgw_version = (unsigned char)DFSGW_BAD_PROTOCOL_VERSION;
#else
        dfsgw_version = (unsigned char)DFSGW_PROTOCOL_VERSION;
#endif
pkt_build:
        data = (unsigned long *)build_dfsgw_prot_version(dfsgw_version, (unsigned char) DFSGW_RAUTH_PACKET, username, localname, sysname, NULL, NULL, &total);
	/*
	 * We have all the necessary information; now send it
	 * over to the host securely.  We send a single datagram
 	 * to the dfsgwd server:
	 *
 	 * <----------- Remote Revokation Packet ----> <---- KRB_AP_REQ Packet (ticket) ---->
	 * {dce_username,userID,networkAddr}Kc->dfsgwd,{Tc->dfsgwd}Kdfsgwd,{Ac->dfsgwd}Kc->dfsgwd
	 */
	if (!valid_cksumtype(CKSUMTYPE_CRC32)) {
		com_err(progname, KRB5_PROG_SUMTYPE_NOSUPP, "while using CRC-32");
		fatal(NULL);
	}
	if (code = krb5_get_default_realm(&c_realm)) {
		com_err(progname, code, "while retrieving local realm");
		fatal(NULL);
	}
	if (code = krb5_get_host_realm(fullhostname, &s_realms)) {
		com_err(progname, code, "while retrieving realm for %s", fullhostname);
		fatal(NULL);
	}
	/*
	 * Compute checksum.
	 */
	if (!(send_cksum.contents = (krb5_octet *)
	      malloc(krb5_checksum_size(CKSUMTYPE_CRC32)))) {
		fatal("memory allocation failure");
	}
	if (code = krb5_calculate_checksum(CKSUMTYPE_CRC32, localname, strlen(localname),
					   0, 0, &send_cksum)) {
		com_err(progname, code, "while computing initial checksum");
		fatal(NULL);
	}
	/*
	 * Get default cred cache.
	 */
	if (code = krb5_cc_default(&ccdef)) {
		com_err(progname, code, "while getting default ccache");
		fatal(NULL);
	}
retry:
        /* this is here for backward compatability with KK servers */
        /* OSF had appended -server to the rgy entry for the gwy   */
        /* server principal, the tgt may or may not have it as its */
        /* principal */
        if (use_service) {
           code = krb5_build_principal(&server, strlen(s_realms[0]), s_realms[0],
                                        "hosts", hostname, service, 0);
           use_service = 0;
        } else {
           code = krb5_build_principal(&server, strlen(s_realms[0]), s_realms[0],
                                        "hosts", hostname, server_name, 0);
           use_service++;
        }
        if (code) {
           com_err(progname, code, "while setting up server principal");
           fatal(NULL);
        }

	/*
	 * Construct KRB_AP_REQ message.
	 */
        code = krb5_mk_req(server, 0, &send_cksum, ccdef, &ap_msg);
        if (code) {
           if (use_service) {
              goto retry;
           } else {
              com_err(progname, code, "while preparing KRB_AP_REQ message");
              fatal(NULL);
           }
        }
	/*
	 * Get session key and creds for server.
	 */
	memset((char *)&screds, 0, sizeof(screds));
	screds.server = server;
	if (code = krb5_cc_get_principal(ccdef, &screds.client)) {
		com_err(progname, code, "while getting client principal name");
		fatal(NULL);
	}
	if (code = krb5_get_credentials(0, ccdef, &screds)) {
		com_err(progname, code, "while fetching credentials");
		fatal(NULL);
	}
	/*
	 * Ensure that these credentials are still valid.
	 */
	if (code = krb5_timeofday(&currenttime)) {
		com_err(progname, code, "while getting time of day");
		fatal(NULL);
	}
        if ((currenttime - screds.times.endtime) > krb5_clockskew) {
		com_err(progname, KRB5KRB_AP_ERR_TKT_EXPIRED, "while checking ticket state");
		fatal(NULL);
   	}
	/*
	 * Get client address, port number from socket.
	 */
	memset((char *)&c_sock, 0, sizeof(c_sock));
	len = sizeof(c_sock);
	if (getsockname(asock, (struct sockaddr *)&c_sock, &len) < 0) {
		fatal("Couldn't get socket info");
	}
	client_addr.addrtype = c_sock.sin_family;
	client_addr.length = sizeof(struct in_addr);
	client_addr.contents = (krb5_octet *)&c_sock.sin_addr;
	if (code = krb5_gen_portaddr(&client_addr, (krb5_pointer)&c_sock.sin_port,
				     &port_clientaddr)) {
		com_err(progname, code, "while generating port address");
		fatal(NULL);
	}
	/*
	 * Construct KRB_PRIV message.  We don't use a replay cache.
	 */
	auth_pkt.data = (char *)data;
	auth_pkt.length = total;
	if (code = krb5_mk_priv(&auth_pkt, ETYPE_DES_CBC_CRC, &screds.keyblock,
				port_clientaddr, &server_addr, 0, KRB5_PRIV_NOTIME, 0, 0, &auth_msg)) {
		com_err(progname, code, "while making KRB_PRIV message");
		fatal(NULL);
	}
	/*
	 * Concatenate the two messages and send them together.
	 */
	dfsgw_ap_msg.length = ap_msg.length;
	dfsgw_ap_msg.data = ap_msg.data;
	dfsgw_auth_msg.length = auth_msg.length;
	dfsgw_auth_msg.data = auth_msg.data;
	cat_msg.length = dfsgw_roundup(dfsgw_ap_msg.length + sizeof(dfsgw_data));
	cat_msg.length += dfsgw_auth_msg.length + sizeof(dfsgw_data);
	if ((cat_msg.data = malloc(cat_msg.length)) == NULL) {
		fatal("memory allocation failure");
	}
	memcpy((char *)(cat_msg.data + sizeof(dfsgw_ap_msg)), dfsgw_ap_msg.data, dfsgw_ap_msg.length);
	memcpy(cat_msg.data, (char *)&dfsgw_ap_msg, sizeof(dfsgw_ap_msg));
	memcpy((char *)(dfsgw_roundup(cat_msg.data + sizeof(dfsgw_ap_msg) + dfsgw_ap_msg.length) + sizeof(dfsgw_auth_msg)),
               dfsgw_auth_msg.data, dfsgw_auth_msg.length);
	memcpy((char *)(dfsgw_roundup(cat_msg.data + sizeof(dfsgw_ap_msg) + dfsgw_ap_msg.length)), &dfsgw_auth_msg, sizeof(dfsgw_auth_msg));
	/*
	 * Send packet/read reply.
	 */
	reply.data = replydata;
	reply.length = DFSGW_MAXRSIZE;
	if (sendrcv(asock, cat_msg.data, cat_msg.length, 
		    (unsigned long)SR_DITIMO, (int)SR_DINCR, (int)SR_DMXTIMO,
		    (int *)&reply.length, (char *)reply.data) < 0) {
		fatal("error while sending/recv data packets");
        }
	/*
	 * Reply is a KRB_SAFE message that serves as both an "authenticator" and
	 * a safe way of sending a message.
	 */
	if (reply.length <= 0) {
		fatal("Error on recv");
	} else {
		if (code = krb5_gen_portaddr(&server_addr, (krb5_pointer)&sp->s_port, &port_serveraddr)) {
			com_err(progname, code, "while generating server full address");
			fatal(NULL);
		}
		if (code = krb5_rd_safe(&reply, &screds.keyblock,
			                port_serveraddr, NULL, 0, KRB5_SAFE_NOTIME, 0, &authok)) {
			com_err(progname, code, "while reading KRB_SAFE reply message");
			fatal(NULL);
		}
		/*
		 * Packet contents should be:
		 *
		 * unsigned char protocol_version
		 * unsigned char DFSGW_REPLY_PACKET (with encoded byte order)
		 * dfsgw_roundup
		 * unsigned long reply_code
	 	 */
		cdata = (unsigned char *)authok.data;
                /* this is where it must send old packet version */
                if ((*cdata != dfsgw_version) && (dfsgw_msg(*(cdata + 1)) == DFSGW_REPLY_PACKET)) {
                        dfsgw_version = DFSGW_PROTOCOL_VERSION_1;
                        goto pkt_build;
                }
		else if ((*cdata != dfsgw_version) || (dfsgw_msg(*(cdata + 1)) != DFSGW_REPLY_PACKET)) {
			fatal("Protocol violation - authentication failed");
		}
                data = (unsigned long *)cdata;
		data++;
		if (dfsgw_endian(*(cdata + 1)) != host_byte_order) {
			*data = dfsgw_wswap(*data);
		}
		/* handle errors returned from server */
                switch (*data) {
                   case DFSGW_REPLY_OK :
                     break;

                   case DFSGW_REPLY_ERROR :
                     fatal("Remote authentication failure");
                     break;

                   case DFSGW_REPLY_ERROR_BAD_VERSION :
                     fatal("Protocol violation - dfsgw version not recognized by server");
                     break;

                   default :
                     fatal("Unknown reply failure");
                     break;
                }
	}
	/*
	 * Finally, destroy local cache.
	 */
	if (code = krb5_cc_destroy(ccdef)) {
		com_err (progname, code, "while destroying cache");
	}
	exit(0);
}
