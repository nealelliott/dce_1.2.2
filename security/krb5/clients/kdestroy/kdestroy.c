/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdestroy.c,v $
 * Revision 1.1.8.2  1996/02/18  00:08:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:34  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:38:59  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/08/14  13:32 UTC  bartf
 * 	merge fix for dce creds shouldn't be needed
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_90/4  1995/08/03  19:13 UTC  bartf
 * 	put error condition back
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_90/3  1995/08/02  19:27 UTC  fotino_r
 * 	completed added krb5_cc_destroy
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_90/3  1995/08/02  19:27 UTC  fotino_r
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_90/3  1995/08/02  19:27 UTC  fotino_r
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_90/2  1995/08/01  13:35 UTC  fotino_r
 * 	Checkpoint version
 * 
 * 	HP revision /main/HPDCE02/bartf_sec_90/1  1995/08/01  13:14 UTC  fotino_r
 * 	Addressing no dce_creds only destroy.
 * 
 * 	HP revision /main/HPDCE02/4  1995/07/20  20:33 UTC  cuti
 * 	Initialize uninitized varaible unveil by  compiling with +O2
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra10_fix/1  1995/07/19  13:42 UTC  cuti
 * 	CHFts15455: Initialize unintialized variables unveil by +O2
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/09  11:54 UTC  mdf
 * 	merge
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_5/1  1995/02/08  19:31 UTC  mdf
 * 	Changes for CHFts14258, If the root user really wants to remove the
 * 	machine credential files, they need to use the -c option.
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  22:43 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/2  1994/02/02  21:20  mdf
 * 	 merge bug fixes.
 * 
 * 	HP revision /main/HPDCE01/mdf_mothra_bl0/1  1994/02/02  16:46  mdf
 * 	"Removed extra #ifndef OSF_DCE that was added by the merge"
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:02  mdf
 * 	merge
 * 	[1995/12/08  16:56:23  root]
 * 
 * Revision 1.1.6.2  1994/06/02  21:28:52  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:16:38  mdf]
 * 
 * Revision 1.1.6.1  1994/03/07  20:00:04  sekhar
 * 	[OT 10042 ] User not required to be root to use -c option.
 * 	[1994/03/04  15:44:45  sekhar]
 * 
 * Revision 1.1.4.1  1993/10/15  12:23:19  sekhar
 * 	[OT 7498] fixed -c option and also did some cleanup.
 * 	          (N.B. -c option now requires a user to be root.)
 * 	[1993/10/14  20:38:37  sekhar]
 * 
 * Revision 1.1.5.4  1993/12/06  23:33:16  rps
 * 	[CHFts09498] Add -e option for expired tickets
 * 	[1993/12/04  01:14:40  rps]
 * 
 * Revision 1.1.5.3  1993/10/19  19:16:21  sekhar
 * 	[CHFts09242]  fixed -c option and also did some cleanup.
 * 	[OT 7498]     (N.B. -c option now requires a user to be root.)
 * 	[1993/10/19  19:16:01  sekhar]
 * 
 * Revision 1.1.5.2  1993/07/08  12:22:24  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:21:53  root]
 * 
 * Revision 1.1.3.2  1993/05/25  16:32:11  rps
 * 	standardize version string
 * 	[1993/05/25  16:22:14  rps]
 * 
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:01:25  tom]
 * 
 * Revision 1.1.2.2  1992/12/29  13:54:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:00  zeliff]
 * 
 * Revision 1.1.1.12  1993/05/04  20:30:42  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1  1992/01/19  14:54:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  kdestroy.c V=3 10/23/91 //littl/prgy/krb5/clients/kdestroy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/clients/kdestroy/kdestroy.c,v $
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
 * Destroy the contents of your credential cache.
 */

#include <stdio.h>

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include <com_err.h>

#ifdef OSF_DCE
#include <dirent.h>
#include <locale.h>
#include <dce/rgybase.h>
#include <dce/sec_login.h>
#include <dce/dce_error.h>
#include <dce/sec_login_file_loc.h>

char *argv0;

/*
 * Purge named ticket if expired before time t
 * Return true on error
 */
idl_boolean
purge_ent(char *ccname, time_t t)
{
    unsigned char err_text[dce_c_error_string_len+1];
    error_status_t st;
    int lst;
    unsigned32 len;

    len=strlen(ccname);
    sec_login_purge_context_exp(len, (ndr_byte *)ccname, t, &st);
    if (st) {
	dce_error_inq_text(st, err_text, &lst);
	if (lst == 0)
	    fprintf(stderr, "%s: %s while destroying %s\n", argv0, err_text, ccname);
	else
	    fprintf(stderr, "%s: Error %#x while destroying %s\n", argv0, st, ccname);
        return true;
    }
    return false;
}


/*
 * Purge tickets expired before time t; NULL name means all.
 * Return true on error.
 * [libdce can't do thread-safe readdir() portably]
 */
idl_boolean
purge_expired(char *ccname, time_t t)
{
    DIR *creds;
    struct dirent *ent;
    idl_boolean rv = false;

    if (ccname)
	return (purge_ent(ccname, t));

    creds = opendir(SEC_LOGIN_CREDS_DIR);
    if (creds == NULL) {
	fprintf(stderr, "Error opening credential directory %s", SEC_LOGIN_CREDS_DIR);
	perror("Error was");
	return(true);
    }

    while ((ent = readdir(creds)) != NULL) {
	unsigned int id;
	char suffix;
	char ccname[sec_rgy_name_t_size];

	if (sscanf(ent->d_name, SEC_LOGIN_CCACHE_TKT_LEAF "%c", &id, &suffix) == 1) {
	    strcpy(ccname, SEC_LOGIN_CCACHE_PREFIX SEC_LOGIN_CREDS_DIR "/");
	    strcat(ccname, ent->d_name);
	    rv |= purge_ent(ccname, t);
	}
    }
    closedir(creds);

    return(rv);
}


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
                        return (-1);
                }
                r += t;
        }

        return (r);
}
#endif /* OSF_DCE */


extern int optind;
extern char *optarg;

void
main(argc, argv)
    int argc;
    char **argv;
{
    int c;
    krb5_ccache cache = NULL;
    char *cache_name = NULL;
    int code;
    int errflg=0;
#ifdef OSF_DCE
    int			    exp_flag = 0;
    time_t		    now, exp_time;
    sec_login_handle_t      h;
    error_status_t          st;
    unsigned char           err_text[dce_c_error_string_len+1];
    int                     lst;
    unsigned32              len;

    setlocale(LC_ALL, "");
    argv0 = argv[0];
#define MAXS32 ~((signed32)(1<<31))
#endif

    
    krb5_init_ets();

    if (strrchr(argv[0], '/'))
	argv[0] = strrchr(argv[0], '/')+1;

    while ((c = getopt(argc, argv, "e:c:")) != EOF) {
	switch (c) {
	case 'c':
	    if (cache == NULL) {
		cache_name = optarg;
		
#ifndef OSF_DCE
		code = krb5_cc_resolve (cache_name, &cache);
		if (code != 0) {
		    com_err (argv[0], code, "while resolving %s", cache_name);
		    errflg++;
		}
#endif
	    } else {
		fprintf(stderr, "Only one -c option allowed\n");
		errflg++;
	    }
	    break;
#ifdef OSF_DCE
	case 'e':
	    exp_flag++;
	    exp_time = convtime(optarg);
	    if (exp_time == -1) {
		fprintf(stderr, "Bad expiration value (%s hours?)\n", optarg);
		errflg++;
	    } else {
		time(&now);
		exp_time = now - exp_time;
	    }
	    break;
#endif
	case '?':
	default:
	    errflg++;
	    break;
	}
    }

    if (optind != argc)
	errflg++;
    
    if (errflg) {
	fprintf(stderr, "Usage: %s [ -c cache-name ] [ -e exp-period ]\n", argv[0]);
	exit(2);
    }

#ifdef OSF_DCE 

    /* Root privilege required to destroy credentials for the named file */
    if ((cache_name || exp_flag) && geteuid() != 0) {
	fprintf(stderr, "ERROR DCE credentials not destroyed (-c/-e options require root privilege)\n");
	exit(1);
    }

    if (exp_flag) {
	exit (purge_expired(cache_name, exp_time));
    }

    if (cache_name == NULL) {
        sec_login_get_current_context(&h, &st);
        if (st == 0) {
            sec_login_purge_context(&h, &st);
	}
    }
    else {
        len=strlen(cache_name);
        sec_login_import_context(len, (ndr_byte *) cache_name, &h, &st);
        if (st == 0) {
            sec_login_purge_context_exp(len, (ndr_byte *) cache_name, MAXS32, &st);
	}
    }

    if (st  && st != sec_login_s_no_current_context) {
        dce_error_inq_text(st, err_text, &lst);
        if (lst == 0) {
            fprintf(stderr, "%s: %s while destroying DCE credentials\n", argv[0], err_text);
        } else {
            fprintf(stderr, "%s: Error %#x while destroying DCE credentials\n", argv[0], st);
        }
        exit(1);
    }
    if (st == 0)
        exit(0);

/* In the case where there is no dce_login context, only a kinit context
   do only a krb5 cache destroy. The OSF_DCE macro is defined during HP-DCE
   builds and identifies HP-added code. When done inside a dce_login context,
   this destroys the credential cache file(s) for that context only.
   If there is an underlying, host-wide, kinit context, it is untouched.
   However, if there is _only_ a kinit context, the cache file is destroyed
   for all processes. 
*/ 

#endif 
/* ifndef OSF_DCE */

    if (cache == NULL) {
	if (code = krb5_cc_default(&cache)) {
	    com_err(argv[0], code, "while getting default ccache");
	    exit(1);
	}
    }

    code = krb5_cc_destroy (cache);
    if (code != 0) {
	com_err (argv[0], code, "while destroying cache");
#ifdef __STDC__
	fprintf(stderr, "Ticket cache \aNOT\a destroyed!\n");
#else
	fprintf(stderr, "Ticket cache \007NOT\007 destroyed!\n");
#endif
	exit (1);
    }
    exit (0);

}
