/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: recvauth.c,v $
 * Revision 1.1.2.1  1996/06/04  21:47:32  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:38 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/05/17  17:42 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/5]
 *
 * 	Change "ifdef CRYPT" to "ifndef NOENCRYPTION"
 * 	[1996/05/16  17:11 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 *
 * 	Don't exit if rd_and_store_for_creds fails, instead return status
 * 	to caller.
 * 	[1996/05/03  20:04 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
 *
 * 	Add OSF/HP copyrights
 * 	[1996/05/03  20:04 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include "krb5.h"
#include <kerberosIV/krb.h>

#ifndef KRB_SENDAUTH_VLEN
#define KRB_SENDAUTH_VLEN 8         /* length for version strings */
#endif
#define KRB5_RECVAUTH_V4        4
#define KRB5_RECVAUTH_V5        5
#define NMAX   16
#ifndef NCARGS
/* linux doesn't seem to have it... */
#define NCARGS 1024
#endif
#define ENVSIZE (sizeof("TERM=")-1)     /* skip null for concatenation */

extern krb5_context bsd_context;
extern krb5_keytab keytab;
extern int auth_sys;
extern AUTH_DAT *v4_kdata;
extern char lusername[NMAX+1];
extern char rusername[NMAX+1];
extern char cmdbuf[NCARGS+1];
extern char *krusername;
extern krb5_principal client;
extern krb5_encrypt_block eblock;
extern krb5_ticket *ticket;
extern Key_schedule v4_schedule;
extern char term[64];
extern int doencrypt;
extern krb5_ccache ccache;

krb5_error_code
recvauth(netf, peersin, rshd_flag)
     int netf;
     struct sockaddr_in peersin;
     int rshd_flag;
{
    krb5_auth_context *auth_context = NULL;
    krb5_error_code status;
    struct sockaddr_in laddr;
    char krb_vers[KRB_SENDAUTH_VLEN + 1];
    int len;
    krb5_principal server;
    krb5_data inbuf;
    char v4_instance[INST_SZ];  /* V4 Instance */
    char v4_version[9];

    len = sizeof(laddr);
    if (getsockname(netf, (struct sockaddr *)&laddr, &len)) {
            exit(1);
    }
       
#ifdef unicos61
#define SIZEOF_INADDR  SIZEOF_in_addr
#else
#define SIZEOF_INADDR sizeof(struct in_addr)
#endif

    if (status = krb5_sname_to_principal(bsd_context, NULL, "host",
                                         KRB5_NT_SRV_HST, &server)) {
            syslog(LOG_ERR, "parse server name %s: %s", "host",
                   error_message(status));
            exit(1);
    }

    strcpy(v4_instance, "*");

    if (status = krb5_auth_con_init(bsd_context, &auth_context))
        return status;

    if (status = krb5_auth_con_genaddrs(bsd_context, auth_context, netf,
                        KRB5_AUTH_CONTEXT_GENERATE_REMOTE_FULL_ADDR))
        return status;

    status = krb5_compat_recvauth(bsd_context, &auth_context, &netf,
                                  "KCMDV0.1",
                                  server, /* Specify daemon principal */
                                  0,            /* no flags */
                                  keytab, /* normally NULL to use v5srvtab */
				  doencrypt ? KOPT_DO_MUTUAL : 0, /*v4_opts*/
                                  "rcmd",       /* v4_service */
                                  v4_instance,  /* v4_instance */
                                  &peersin,     /* foreign address */
                                  &laddr,       /* our local address */
                                  "",           /* use default srvtab */
                                  &ticket,      /* return ticket */
                                  &auth_sys,    /* which authentication system*/
                                  &v4_kdata, v4_schedule, v4_version);

    if (status) {
        if (auth_sys == KRB5_RECVAUTH_V5) {
            /*
             * clean up before exiting
             */
            getstr(lusername, sizeof(lusername), "lusername");
            if (rshd_flag)
    	    	getstr(cmdbuf, sizeof(cmdbuf), "command");
	    else
	    	getstr(term+ENVSIZE, sizeof(term)-ENVSIZE, "Terminal type");
            getstr(rusername, sizeof(rusername), "rusername");
        }
        return status;
    }

    getstr(lusername, sizeof(lusername), "lusername");
    if (rshd_flag)
    	getstr(cmdbuf, sizeof(cmdbuf), "command");
    else
    	getstr(term+ENVSIZE, sizeof(term)-ENVSIZE, "Terminal type");

#ifdef KRB5_KRB4_COMPAT
    if (auth_sys == KRB5_RECVAUTH_V4) {
        /* We do not really know the remote user's login name.
         * Assume it to be the same as the first component of the
         * principal's name.
         */
        strcpy(rusername, v4_kdata->pname);
        krusername = (char *) malloc(strlen(v4_kdata->pname) + 1 +
                                   strlen(v4_kdata->pinst) + 1 +
                                   strlen(v4_kdata->prealm) + 1);
        sprintf(krusername, "%s/%s@%s", v4_kdata->pname,
                v4_kdata->pinst, v4_kdata->prealm);

        if (status = krb5_parse_name(bsd_context, krusername, &client))
          return(status);
        return 0;
    }
#endif

    /* Must be V5  */
       
    getstr(rusername, sizeof(rusername), "rusername");

    if (status = krb5_unparse_name(bsd_context, ticket->enc_part2->client,
                                   &krusername))
        return status;

    if (status = krb5_copy_principal(bsd_context, ticket->enc_part2->client,
                                     &client))
        return status;


    /* Null out the "session" because eblock.key references the session
     * key here, and we do not want krb5_free_ticket() to destroy it. */
    ticket->enc_part2->session = 0;

    if (status = krb5_read_message(bsd_context, (krb5_pointer)&netf, &inbuf)) {
	fatal(netf, "Error reading message");
    }

    if (inbuf.length) { /* Forwarding being done, read creds */
        if (status = rd_and_store_for_creds(bsd_context, auth_context, &inbuf,
                                            ticket, lusername, &ccache)) {
            syslog(LOG_ERR, "Can't get forwarded credentials: %s",
                   error_message(status));
	    return status;
        }
    }
    krb5_free_ticket(bsd_context, ticket);
    return 0;
}
