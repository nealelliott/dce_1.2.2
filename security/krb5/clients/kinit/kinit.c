/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kinit.c,v $
 * Revision 1.1.20.2  1996/11/13  18:03:16  arvind
 * 	Merge fix for CHFts20591
 * 	[1996/11/05  21:28 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth3/1]
 *
 * 	Request KRB5_PADATA_ENC_TIMESTAMPS preauth.
 * 	[1996/09/26  23:16 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.20.1  1996/10/03  14:55:03  arvind
 * 	call krb5_get_in_tkt instead of the (now nonexistant) split functions
 * 	[1996/09/16  21:55 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.18.3  1996/02/18  00:08:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:36  marty]
 * 
 * Revision 1.1.18.2  1995/12/08  17:39:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/17  15:33 UTC  jrr
 * 	Add cast to pw usage.
 * 
 * 	HP revision /main/HPDCE02/6  1995/11/08  20:45 UTC  bartf
 * 	merge fix for MIT like krb
 * 
 * 	change Usage
 * 	[1995/11/07  19:02 UTC  bartf  /main/HPDCE02/bartf_sec_kinit.10/2]
 * 
 * 	add default krb behavior
 * 	[1995/11/02  21:04 UTC  bartf  /main/HPDCE02/bartf_sec_kinit.10/1]
 * 	HP revision /main/HPDCE02/5  1995/07/18  18:40 UTC  rps
 * 	merge CHFts15669 patch
 * 
 * 	HP revision /main/HPDCE02/rps_moth2/1  1995/07/18  18:36 UTC  rps
 * 	merge patch
 * 
 * 	HP revision /main/HPDCE01/3  1995/07/11  16:03 UTC  rps
 * 	merge CHFts15669
 * 
 * 	HP revision /main/GAMERA/KK/KK_FIX/1  1995/07/11  00:35 UTC  rps
 * 	obtain login context name/realm when not provided in PAC
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/09  15:28 UTC  jrr
 * 	Merge in more changes from mullan_s.
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/08  21:16 UTC  jrr
 * 	Revert back to 1.0.3 functionality.
 * 
 * 	HP revision /main/HPDCE02/jrr_sec_chfts14997/2  1995/05/09  15:26 UTC  jrr
 * 	Fix merge problem.
 * 
 * 	HP revision /main/DCE1_1_WP/1  1995/05/05  18:08 UTC  jrr
 * 	Merge changes from mullan_s to fix password problem.
 * 
 * 	HP revision /main/jrr_wp/2  1995/05/05  17:51 UTC  jrr
 * 	Use krb5_read_password for entering password prompting.
 * 
 * 	HP revision /main/jrr_wp/1  1995/05/05  15:59 UTC  jrr
 * 	Revert back to 1.0.3 style kinit.
 * 	[1995/12/08  16:56:25  root]
 * 
 * Revision 1.1.5.5  1993/12/06  23:07:33  rps
 * 	[OT9378] Revert default lifetime units to hours
 * 	[1993/12/03  17:10:52  rps]
 * 
 * Revision 1.1.5.4  1993/09/30  19:40:51  mullan_s
 * 	[OT7499] Fixed kinit -l/-r bug; recognizes invalid times if they
 * 	contain more than one unit and the first unit is valid (ex: 1w1f)
 * 	[1993/09/30  19:40:13  mullan_s]
 * 
 * Revision 1.1.5.3  1993/09/24  13:26:07  mullan_s
 * 	[OT 7499] Modified the routine convtime() so that invalid time
 * 	intervals are returned as errors. Also fixed a minor
 * 	nit in the error message output produced by kinit -l
 * 	(was missing a closed parentheses).
 * 	[1993/09/24  13:25:08  mullan_s]
 * 
 * Revision 1.1.5.2  1993/07/08  12:24:00  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:23:28  root]
 * 
 * 	[7144] Inform DFS of arrival of new TGT.
 * 	[1993/03/16  22:33:22  sommerfeld]
 * 
 * 	[OT6752] fix argument parsing integration with DCE. (argh!)
 * 	[1993/02/12  16:39:53  sommerfeld]
 * 
 * 	[OT6556] Unrelated bugfix to error message printing.
 * 	[1993/02/09  02:37:59  sommerfeld]
 * 
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:01:30  tom]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:08  zeliff]
 * 
 * Revision 1.1.3.2  1993/05/25  16:32:19  rps
 * 	standardize version string
 * 	[1993/05/25  16:22:21  rps]
 * 
 * Revision 1.1.1.12  1993/05/04  20:30:44  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1  1992/01/19  14:54:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  kinit.c V=6 11/14/91 //littl/prgy/krb5/clients/kinit
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/clients/kinit/kinit.c,v $
 * $Author: arvind $
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
 * Initialize a credentials cache.
 */

#include <stdio.h>
#include <pwd.h>

#include <krb5/krb5.h>
#include <krb5/kdb.h>                   /* for TGTNAME */
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>

#include <com_err.h>

#ifdef OSF_DCE
#include <dce/rgybase.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/policy.h>
#include <u_str.h>
#include <un_maloc.h>
#include <locale.h>
#endif

#define KRB5_DEFAULT_OPTIONS 0
#define KRB5_DEFAULT_LIFE 0	/* default to what the KDC provides. */

extern int optind;
extern char *optarg;

/*
 * this next function was lifted from the source to sendmail, which is:
 * 
 * Copyright (c) 1983 Eric P. Allman
 * Copyright (c) 1988 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted provided
 * that: (1) source distributions retain this entire copyright notice and
 * comment, and (2) distributions including binaries display the following
 * acknowledgement:  ``This product includes software developed by the
 * University of California, Berkeley and its contributors'' in the
 * documentation or other materials provided with the distribution and in
 * all advertising materials mentioning features or use of this software.
 * Neither the name of the University nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

time_t
convtime(p)
        char *p;
{
        register time_t t, r;
        register char c;

        r = 0;
        while (*p != '\0')
        {
                t = 0;
                while (isdigit(c = *p++))
                        t = t * 10 + (c - '0');
                switch (c)
                {
                  case 'w':             /* weeks */
                        t *= 7;

                  case 'd':             /* days */
                        t *= 24;

                  case 'h':             /* hours */
                        t *= 60;

                  case 'm':             /* minutes */
                        t *= 60;

                  case 's':             /* seconds */
                        break;

                  case '\0':		/* default if no interval is specified */
                        t *= 3600;
                        p--;
                        break;

                  default:		/* error! : incorrect interval specified */
                        return (0);
                }
                r += t;
        }

        return (r);
}

krb5_error_code
krb5_parse_lifetime (time, len)
    char *time;
    long *len;
{
    *len = convtime(time);
    return 0;
}

int get_dce_info
#ifndef __STDC__
    ( princ, lifetime_set, lifetime ) 
    char    **princ;
    int     lifetime_set;
    long    *lifetime;
#else
  (
    char    **princ,
    int     lifetime_set,
    long    *lifetime
  )
#endif
{
    sec_login_handle_t      h;
    error_status_t          st;
    sec_login_net_info_t    net_info;
    unsigned char           err_text[dce_c_error_string_len+1];
    int                     lst;
    char                    *global_name = NULL;
    int                     pos;
    int                     len;
    sec_id_pac_t            *p;
    sec_rgy_properties_t    prop;
    sec_rgy_name_t	    realm_name, princ_name;

    sec_login_get_current_context(&h, &st);
    if (st) {
        return st;
    }

    sec_login_inquire_net_info(h, &net_info, &st);
    if (st && st != sec_login_s_not_certified) {
        printf("No DCE network identity information available\n");
        dce_error_inq_text(st, err_text, &lst);
	printf("Error: %s\n", err_text);
        return st;
    }

    p = &net_info.pac;

    if (! (p->realm.name && *p->realm.name &&
	   p->principal.name && *p->principal.name)) {
	/*
	 * Names not available locally; fill them in -- arguably, for
	 * compatibility, this should be done in the client library code...
	 */
	if (!uuid_is_nil(&p->realm.uuid, &lst) &&
	    !uuid_is_nil(&p->principal.uuid, &lst)) {

	    sec_id_gen_name(NULL, &p->realm.uuid, &p->principal.uuid,
			    NULL, realm_name, princ_name, &st);
	    if (st) {
		fprintf(stderr, "DCE network identity unavailable\n");
		dce_error_inq_text(st, err_text, &lst);
		fprintf(stderr, "Error: %s\n", err_text);
		return st;
	    }
	    else {
		p->realm.name = realm_name;
		p->principal.name = princ_name;
	    }
	}
	else {
	    fprintf(stderr, "DCE network identity unavailable\n");
	    return 1;
	}
    }

    if (*princ && **princ) {
        sec_id_globalize_name(*princ, (unsigned_char_p_t *) &global_name);
        if (global_name == NULL) {
            printf("Unable to convert %s to global form\n");
            return 1;
        }
        len = u_strlen(p->realm.name);
        if (u_strncmp(global_name, p->realm.name, len) != 0) {
            goto sec_err_out;
        }

        pos = len;
        if (global_name[pos] != '/') {
            goto sec_err_out;
        }

        pos++;
        if (u_strcmp(&global_name[pos], p->principal.name) != 0) {
            goto sec_err_out;
        }
    }

    pos = u_strlen(p->principal.name);
    len = pos + 2 + u_strlen(p->realm.name);
    *princ = malloc(len);
    u_strcpy(*princ, p->principal.name);
    (*princ)[pos] = '@';
    u_strcpy((*princ)+pos+1, (p->realm.name) + GLOBAL_DIR_ROOT_LEN);

#if 0
    /* this is nonsense.. */
    if (!lifetime_set) {
        /*
         * Attempt to get the cell's default lifetime
         */
        sec_rgy_properties_get_info(sec_rgy_default_handle, &prop, &st);
        if (st == 0) {
            *lifetime = prop.default_certificate_lifetime;
        }
    }
#endif
    return 0;

sec_err_out:
    printf("Can't change existing principal information with kinit\n");
    printf("Current information is for %s/%s\n", p->realm.name,
                    p->principal.name);
    exit(1);

}    

krb5_data tgtname = {
    sizeof(TGTNAME)-1,
    TGTNAME
};
     

/* 
 * declarations needed to make use of the DCE version of the kerberos
 * TGT acquisition API
 */ 
      
krb5_error_code pwd_keyproc
    PROTOTYPE((krb5_context,
               const krb5_enctype,
               krb5_data *,
               krb5_const_pointer,
               krb5_keyblock **));

void
main(argc, argv)
    int argc;
    char **argv;
{
    krb5_ccache ccache = NULL;
    char *cache_name = NULL;            /* -f option */
    long lifetime = KRB5_DEFAULT_LIFE;  /* -l option */
    long rlife = 0;
    long verbose = 0;           /* -v option */
    int options = KRB5_DEFAULT_OPTIONS;
    int option;
    int errflg = 0;
    krb5_address **my_addresses;
    krb5_error_code code;
    krb5_principal me;
    krb5_principal server;
    krb5_creds my_creds;
    krb5_timestamp now;
    char *princname = NULL;
    int lifetime_set = false; 
    krb5_kdc_rep *as_reply = 0;
    krb5_data reply;
    krb5_int32 nonce;
    krb5_data  keyseed;
    char pbuf[BUFSIZ];
    int pwsize;
    krb5_enctype		enctypes[2];
    krb5_preauthtype		patypes[3];

    int status;
    struct passwd *pw = 0;
    int mitflg = 0;
    const char usage_str[] =
    "Usage: kinit [ -r time ] [ -vpf ] [ -l lifetime ] [ -c cachename ] [principal]\n";

#ifdef OSF_DCE
    setlocale(LC_ALL, "");
#endif
    krb5_init_ets();

    if (strrchr(argv[0], '/'))
        argv[0] = strrchr(argv[0], '/')+1;

    while ((option = getopt(argc, argv, "vr:fpl:c:")) != EOF) {
        switch (option) {
        case 'r':
            options |= KDC_OPT_RENEWABLE;
            code = krb5_parse_lifetime(optarg, &rlife);
            if (code != 0 || rlife == 0) {
                fprintf(stderr, "Bad lifetime value (%s hours?)\n", optarg);
                errflg++;
            }
            break;
        case 'v':
            verbose++;
            break;
        case 'p':
            options |= KDC_OPT_PROXIABLE;
            break;
        case 'f':
            options |= KDC_OPT_FORWARDABLE;
            break;
        case 'l':
            code = krb5_parse_lifetime(optarg, &lifetime);
            if (code != 0 || lifetime == 0) {
                fprintf(stderr, "Bad lifetime value (%s hours?)\n", optarg);
                errflg++;
            }
            lifetime_set = true;
            break;
        case 'c':
            if (ccache == NULL) {
                cache_name = optarg;
                
                code = krb5_cc_resolve (cache_name, &ccache);
                if (code != 0) {
                    com_err (argv[0], code, "resolving %s", cache_name);
                    errflg++;
                }
            } else {
                fprintf(stderr, "Only one -c option allowed\n");
                errflg++;
            }
            break;
        case '?':
        default:
            errflg++;
            break;
        }
    }

    if (errflg) {
        fprintf(stderr, usage_str);
        exit(2);
    }

    if (ccache == NULL) {
        if (code = krb5_cc_default(&ccache)) {
            com_err(argv[0], code, "while getting default ccache");
            exit(1);
        }
    }

    if (optind == argc-1) {	/* cmd line principal */
        princname = argv[optind];
	/* try to get in dce context first, else assume mit-krb */
	status = get_dce_info(&princname, lifetime_set, &lifetime);	
        if (status == sec_login_s_no_current_context) {
           if (code = krb5_parse_name (princname, &me)) {
              com_err (argv[0], code, "when parsing name %s", princname);
              exit(1);
           }    
	   mitflg = 1;
        }
    } else if (optind == argc) {    /* No principal name specified */
	/* try to get princ in dce context first, else assume mit-krb */
	status = get_dce_info(&princname, lifetime_set, &lifetime);
        if (status == sec_login_s_no_current_context) {
           /* Get default principal from cache if one exists */
           code = krb5_cc_get_principal(ccache, &me);
           if (code) {
		   /* Else search passwd file for client */
		   pw = getpwuid((int) getuid());
		   if (pw) {
			if (code = krb5_parse_name (pw->pw_name, &me)) {
			     com_err (argv[0], code, "when parsing name %s",
				      pw->pw_name);
			     exit(1);
			}
		   } else {
			fprintf(stderr, 
			"Unable to identify user from password file\n");
			exit(1);
		   }
	   }
	   mitflg = 1;
        }
	   
    }

    if ( mitflg == 1 ) {  
       code = krb5_cc_initialize (ccache, me);
       if (code != 0) {
	  com_err (argv[0], code, "when initializing cache %s",
		 cache_name?cache_name:"");
	  exit(1);
       }
    }
    
    if ( mitflg == 0 ) {  /* DCE */
       if (princname == NULL || *princname == '\0') {
          fprintf(stderr, "%s: Unable to get principal.\n", argv[0]);
          fprintf(stderr, usage_str);
          exit(2);
       }
       if (code = krb5_parse_name (princname, &me)) {
          com_err (argv[0], code, "when parsing name %s", princname);
          exit(1);
       }    
    }

    memset((char *)&my_creds, 0, sizeof(my_creds));
    
    my_creds.client = me;

    if (code = krb5_build_principal_ext(&server,
                                        me[0]->length, me[0]->data,
                                        tgtname.length, tgtname.data,
                                        me[0]->length, me[0]->data,
                                        0)) {
        com_err(argv[0], code, "while building server name");
        exit(1);
    }

    my_creds.server = server;

    code = krb5_os_localaddr(&my_addresses);
    if (code != 0) {
        com_err (argv[0], code, "when getting my address");
        exit(1);
    }
    if (code = krb5_timeofday(&now)) {
        com_err(argv[0], code, "while getting time of day");
        exit(1);
    }
    my_creds.times.starttime = 0;       /* start timer when request
                                           gets to KDC */
    my_creds.times.endtime = (lifetime != 0)?(now + lifetime):0;
    if (options & KDC_OPT_RENEWABLE) {
        my_creds.times.renew_till = now + rlife;
    } else
        my_creds.times.renew_till = 0;

    pwsize = sizeof(pbuf);
    code = krb5_read_password("Enter Password: ", NULL,
		pbuf, &pwsize);
    if (code != 0) {
        com_err (argv[0], code, "when entering password");
        exit(1);
    }      

    code = krb5_cc_initialize (ccache, me);
    if (code != 0) {
        com_err (argv[0], code, "when initializing cache %s",
                 cache_name?cache_name:"");
        exit(1);
    }      

    keyseed.data = pbuf;
    keyseed.length = pwsize;

    enctypes[0] = ETYPE_DES_CBC_CRC;
    enctypes[1] = 0;

    patypes[0] = KRB5_PADATA_ENC_UNIX_TIME;
    patypes[1] = KRB5_PADATA_ENC_TIMESTAMP;
    patypes[2] = KRB5_PADATA_ROOT_CERT;
    patypes[3] = 0;

    code = krb5_get_in_tkt(NULL,
			   options,
			   my_addresses,
			   enctypes,
			   patypes,
			   pwd_keyproc,
			   &keyseed,
			   krb5_kdc_rep_decrypt_proc,
			   0,
			   &my_creds,
			   ccache,
			   &as_reply);
    if (code != 0) {
        com_err (argv[0], code, "while getting initial credentials");
        exit(1);
    }

    /* 
     * Inform DFS of new pag.
     */
    sec_login_pag_new_tgt (0, my_creds.times.endtime);
      

    if (verbose) {
        char *client_string;
        krb5_unparse_name (me, &client_string);
        
        /* note that ctime includes trailing \n */
        printf("%s: ticket expires at %s", client_string,
            ctime((time_t *)&my_creds.times.endtime));
    }
    exit(0);
}
