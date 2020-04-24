/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: kinit.c,v $
 * Revision 1.1.2.1  1996/06/05  20:24:20  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:22:05  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * clients/kinit/kinit.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Initialize a credentials cache.
 */

#include "k5-int.h"
#include "com_err.h"

#include <stdio.h>
#ifdef HAVE_PWD_H
#include <pwd.h>
#endif

#define KRB5_DEFAULT_OPTIONS 0
#define KRB5_DEFAULT_LIFE 60*60*8 /* 8 hours */

extern int optind;
extern char *optarg;

static time_t convtime();

krb5_error_code
krb5_parse_lifetime (time, len)
    char *time;
    long *len;
{
    *len = convtime(time);
    return 0;
}
    
krb5_data tgtname = {
    0,
    KRB5_TGS_NAME_SIZE,
    KRB5_TGS_NAME
};

/*
 * Try no preauthentication first; then try the encrypted timestamp
 */
krb5_preauthtype * preauth = NULL;
krb5_preauthtype preauth_list[2] = { 0, -1 };

void
main(argc, argv)
    int argc;
    char **argv;
{
	krb5_context kcontext;
    krb5_ccache ccache = NULL;
    char *cache_name = NULL;		/* -f option */
    char *keytab_name = NULL;		/* -t option */
    long lifetime = KRB5_DEFAULT_LIFE;	/* -l option */
    long rlife = 0;
    int options = KRB5_DEFAULT_OPTIONS;
    int option;
    int errflg = 0;
    krb5_error_code code;
    krb5_principal me;
    krb5_principal server;
    krb5_creds my_creds;
    krb5_timestamp now;
    int use_keytab = 0;			/* -k option */
    krb5_keytab keytab = NULL;
    struct passwd *pw = 0;
    int pwsize;
    char password[255], *client_name, prompt[255];

    krb5_init_context(&kcontext);
    krb5_init_ets(kcontext);

    if (strrchr(argv[0], '/'))
	argv[0] = strrchr(argv[0], '/')+1;

    while ((option = getopt(argc, argv, "r:fpl:c:kt:")) != EOF) {
	switch (option) {
	case 'r':
	    options |= KDC_OPT_RENEWABLE;
	    code = krb5_parse_lifetime(optarg, &rlife);
	    if (code != 0 || rlife == 0) {
		fprintf(stderr, "Bad lifetime value (%s hours?)\n", optarg);
		errflg++;
	    }
	    break;
	case 'p':
	    options |= KDC_OPT_PROXIABLE;
	    break;
	case 'f':
	    options |= KDC_OPT_FORWARDABLE;
	    break;
#ifndef NO_KEYTAB
       case 'k':
	    use_keytab = 1;
	    break;
       case 't':
	    if (keytab == NULL) {
		 keytab_name = optarg;

		 code = krb5_kt_resolve(kcontext, keytab_name, &keytab);
		 if (code != 0) {
		      com_err(argv[0], code, "resolving keytab %s",
			      keytab_name);
		 errflg++;
		 }
	    } else {
		 fprintf(stderr, "Only one -t option allowed.\n");
		 errflg++;
	    }
	    break;
#endif
       case 'l':
	    code = krb5_parse_lifetime(optarg, &lifetime);
	    if (code != 0 || lifetime == 0) {
		fprintf(stderr, "Bad lifetime value (%s hours?)\n", optarg);
		errflg++;
	    }
	    break;
	case 'c':
	    if (ccache == NULL) {
		cache_name = optarg;
		
		code = krb5_cc_resolve (kcontext, cache_name, &ccache);
		if (code != 0) {
		    com_err (argv[0], code, "resolving ccache %s",
			     cache_name);
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

    if (argc - optind > 1) {
	fprintf(stderr, "Extra arguments (starting with \"%s\").\n",
		argv[optind+1]);
	errflg++;
    }

    if (errflg) {
	fprintf(stderr, "Usage: %s [-r time] [-puf] [-l lifetime] [-c cachename] [-k] [-t keytab] [principal]\n", argv[0]);
	exit(2);
    }

    if (ccache == NULL) {
	 if (code = krb5_cc_default(kcontext, &ccache)) {
	      com_err(argv[0], code, "while getting default ccache");
	      exit(1);
	 }
    }

    if (optind != argc-1) {       /* No principal name specified */
#ifndef NO_KEYTAB
	 if (use_keytab) {
	      /* Use the default host/service name */
	      code = krb5_sname_to_principal(kcontext, NULL, NULL,
					     KRB5_NT_SRV_HST, &me);
	      if (code) {
		   com_err(argv[0], code,
			   "when creating default server principal name");
		   exit(1);
	      }
	 } else
#endif
	 {
	      /* Get default principal from cache if one exists */
	      code = krb5_cc_get_principal(kcontext, ccache, &me);
	      if (code) {
#ifdef HAVE_PWD_H
		   /* Else search passwd file for client */
		   pw = getpwuid((int) getuid());
		   if (pw) {
			if (code = krb5_parse_name(kcontext,pw->pw_name,&me)) {
			     com_err (argv[0], code, "when parsing name %s",
				      pw->pw_name);
			     exit(1);
			}
		   } else {
			fprintf(stderr, 
			"Unable to identify user from password file\n");
			exit(1);
		   }
#else /* HAVE_PWD_H */
		   fprintf(stderr, "Unable to identify user\n");
		   exit(1);
#endif /* HAVE_PWD_H */
	      }
	 }
    } /* Use specified name */	 
    else if (code = krb5_parse_name (kcontext, argv[optind], &me)) {
	 com_err (argv[0], code, "when parsing name %s",argv[optind]);
	 exit(1);
    }
    
    if (code = krb5_unparse_name(kcontext, me, &client_name)) {
	com_err (argv[0], code, "when unparsing name");
	exit(1);
    }

    code = krb5_cc_initialize (kcontext, ccache, me);
    if (code != 0) {
	com_err (argv[0], code, "when initializing cache %s",
		 cache_name?cache_name:"");
	exit(1);
    }

    memset((char *)&my_creds, 0, sizeof(my_creds));
    
    my_creds.client = me;

    if (code = krb5_build_principal_ext(kcontext, &server,
					krb5_princ_realm(kcontext, me)->length,
					krb5_princ_realm(kcontext, me)->data,
					tgtname.length, tgtname.data,
					krb5_princ_realm(kcontext, me)->length,
					krb5_princ_realm(kcontext, me)->data,
					0)) {
	com_err(argv[0], code, "while building server name");
	exit(1);
    }

    my_creds.server = server;

    if (code = krb5_timeofday(kcontext, &now)) {
	com_err(argv[0], code, "while getting time of day");
	exit(1);
    }
    my_creds.times.starttime = 0;	/* start timer when request
					   gets to KDC */
    my_creds.times.endtime = now + lifetime;
    if (options & KDC_OPT_RENEWABLE) {
	my_creds.times.renew_till = now + rlife;
    } else
	my_creds.times.renew_till = 0;

#ifndef NO_KEYTAB
    if (!use_keytab)
#endif
    {
	 (void) sprintf(prompt,"Password for %s: ", (char *) client_name);

	 pwsize = sizeof(password);

	 code = krb5_read_password(kcontext, prompt, 0, password, &pwsize);
	 if (code || pwsize == 0) {
	      fprintf(stderr, "Error while reading password for '%s'\n",
		      client_name);
	      memset(password, 0, sizeof(password));
	      exit(1);
	 }

	 code = krb5_get_in_tkt_with_password(kcontext, options, 0,
					      NULL, preauth, password, ccache,
					      &my_creds, 0);
	 memset(password, 0, sizeof(password));
#ifndef NO_KEYTAB
    } else {
	 code = krb5_get_in_tkt_with_keytab(kcontext, options, 0,
					    NULL, preauth, keytab, ccache,
					    &my_creds, 0);
#endif
    }
    
    krb5_free_principal(kcontext, server);
    
    if (code) {
	if (code == KRB5KRB_AP_ERR_BAD_INTEGRITY)
	    fprintf (stderr, "%s: Password incorrect\n", argv[0]);
	else
	    com_err (argv[0], code, "while getting initial credentials");
	exit(1);
    }
    exit(0);
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

#include <ctype.h>			/* for isdigit */

static time_t
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
                if (c == '\0')
                        p--;
                switch (c)
                {
                  case 'w':             /* weeks */
                        t *= 7;

                  case 'd':             /* days */
                        t *= 24;

                  case 'h':             /* hours */
                  default:
                        t *= 60;

                  case 'm':             /* minutes */
                        t *= 60;

                  case 's':             /* seconds */
                        break;
                }
                r += t;
        }

        return (r);
}

