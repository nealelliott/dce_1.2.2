/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: klist.c,v $
 * Revision 1.1.7.2  1996/02/18  00:08:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:38  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:39:15  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  22:43 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:02  mdf
 * 	merge
 * 	[1995/12/08  16:56:28  root]
 * 
 * Revision 1.1.7.2  1993/07/08  12:25:30  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:24:56  root]
 * 
 * Revision 1.1.5.2  1993/05/25  16:32:26  rps
 * 	standardize version string
 * 	[1993/05/25  16:22:27  rps]
 * 
 * Revision 1.1.1.12  1993/05/04  20:30:48  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.5.5  1993/01/08  22:17:51  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:01:35  tom]
 * 
 * Revision 1.1.5.4  1992/12/29  13:55:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:16  zeliff]
 * 
 * Revision 1.1.5.3  1992/10/12  22:39:57  burati
 * 	CR2777 -c option wasn't showing alternate DCE identity
 * 	[1992/10/12  20:30:55  burati]
 * 
 * Revision 1.1.5.2  1992/09/29  21:18:03  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:21:51  sekhar]
 * 
 * Revision 1.1.2.2  1992/04/03  17:08:08  ahop
 * 	 CR 2464: show expired tickets only with new -e option
 * 	[1992/04/03  17:06:32  ahop]
 * 
 * Revision 1.1  1992/01/19  14:54:11  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  klist.c V=6 03/03/92 //littl/prgy/krb5/clients/klist
**
** Copyright (c) Hewlett-Packard Company 1991,1992
** Unpublished work. All Rights Reserved.
** $Author: marty $
*/

/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 *
 */

/* Copyright 1990 by the Massachusetts Institute of Technology.
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
 * List the contents of your credential cache.
 */

#include <stdio.h>

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <com_err.h>
#include <time.h>

#ifdef OSF_DCE
#include <dce/rgybase.h>
#include <dce/sec_login.h>
#include <dce/secidmap.h>
#include <dce/dce_error.h>
#include <locale.h>
#endif

extern int optind;
extern char *optarg;
int show_flags = 0;
int show_expired = 0;
char *progname;
char *defname;
time_t now;

void
show_credential PROTOTYPE((krb5_creds *));

#ifdef OSF_DCE
void print_dce_info PROTOTYPE((char *));

#endif


void
main(argc, argv)
    int argc;
    char **argv;
{
    int c;
    int errflg = 0;
    int code;
    krb5_ccache cache = NULL;
    krb5_cc_cursor cur;
    krb5_creds creds;
    char *cache_name = (char *)NULL;
    krb5_principal princ;
    char *name;
    krb5_flags flags;

#ifdef OSF_DCE
    setlocale(LC_ALL, "");
#endif
    krb5_init_ets();

    time(&now);

    if (strrchr(argv[0], '/'))
        progname = strrchr(argv[0], '/')+1;
    else
        progname = argv[0];

    while ((c = getopt(argc, argv, "efc:")) != EOF) {
        switch (c) {
        case 'e':
             show_expired = 1;
             break;
        case 'f':
             show_flags = 1;
             break;
        case 'c':
            if (cache == NULL) {
                cache_name = optarg;

                code = krb5_cc_resolve (cache_name, &cache);
                if (code != 0) {
                    com_err(progname, code, "while resolving %s", cache_name);
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

    if (optind != argc)
        errflg++;

    if (errflg) {
        fprintf(stderr, "Usage: %s [-e] [-f] [ -c cache ]\n", progname);
        exit(2);
    }

#ifdef OSF_DCE
    print_dce_info(cache_name);
#endif

    if (cache == NULL) {
        if (code = krb5_cc_default(&cache)) {
            com_err(progname, code, "while getting default ccache");
            exit(1);
        }
    }

    flags = 0;                          /* turns off OPENCLOSE mode */
    if (code = krb5_cc_set_flags(cache, flags)) {
        if (code == ENOENT) {
            com_err(progname, code, "(ticket cache %s)",
                    krb5_cc_get_name(cache));
        } else
            com_err(progname, code,
                    "while setting cache flags (ticket cache %s)",
                    krb5_cc_get_name(cache));
        exit(1);
    }
    if (code = krb5_cc_get_principal(cache, &princ)) {
        com_err(progname, code, "while retrieving principal name");
        exit(1);
    }
    if (code = krb5_unparse_name(princ, &defname)) {
        com_err(progname, code, "while unparsing principal name");
        exit(1);
    }
    printf("Ticket cache: %s\nDefault principal: %s\n",
           krb5_cc_get_name(cache), defname);
    if (code = krb5_cc_start_seq_get(cache, &cur)) {
        com_err(progname, code, "while starting to retrieve tickets");
        exit(1);
    }
    while (!(code = krb5_cc_next_cred(cache, &cur, &creds)))
        show_credential(&creds);
    if (code == KRB5_CC_END) {
        if (code = krb5_cc_end_seq_get(cache, &cur)) {
            com_err(progname, code, "while finishing ticket retrieval");
            exit(1);
        }
        flags = KRB5_TC_OPENCLOSE;      /* turns on OPENCLOSE mode */
        if (code = krb5_cc_set_flags(cache, flags)) {
            com_err(progname, code, "while closing ccache");
            exit(1);
        }
        exit(0);
    } else {
        com_err(progname, code, "while retrieving a ticket");
        exit(1);
    }
}

void
print_flags(cred)
register krb5_creds *cred;
{
    printf("F: %X (", cred->ticket_flags);
    if (cred->ticket_flags & TKT_FLG_FORWARDABLE)
        putchar('F');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_FORWARDED)
        putchar('f');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_PROXIABLE)
        putchar('P');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_PROXY)
        putchar('p');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_MAY_POSTDATE)
        putchar('D');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_POSTDATED)
        putchar('d');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_INVALID)
        putchar('i');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_RENEWABLE)
        putchar('R');
    else
        putchar(' ');
    if (cred->ticket_flags & TKT_FLG_INITIAL)
        putchar('I');
    else
        putchar(' ');
    putchar(')');
}

void printtime(tv)
    time_t tv;
{
    struct tm *stime;
    stime = localtime((time_t *)&tv);

    printf("%02d/%02d/%02d:%02d:%02d:%02d",
           stime->tm_year,
           stime->tm_mon + 1,
           stime->tm_mday,
           stime->tm_hour,
           stime->tm_min,
           stime->tm_sec);
}

void
show_credential(cred)
register krb5_creds *cred;
{
    krb5_error_code retval;
    char *name, *sname;

    if (!cred->times.starttime)
        cred->times.starttime = cred->times.authtime;

    if (cred->times.endtime < now && !show_expired)
        return;

    retval = krb5_unparse_name(cred->client, &name);
    if (retval) {
        com_err(progname, retval, "while unparsing client name");
        return;
    }
    retval = krb5_unparse_name(cred->server, &sname);
    if (retval) {
        com_err(progname, retval, "while unparsing server name");
        free(name);
        return;
    }
    if (strcmp(name, defname) == 0) {
        printf("Server: %s\n\t", sname);
    } else {
        printf("Client: %s\tServer: %s\n\t", name, sname);
    }

    if (cred->times.endtime < now) {
        printf("EXPIRED; was ");
    }
    printf("valid ");
    printtime(cred->times.starttime);
    printf(" to ");
    printtime(cred->times.endtime);
    if (cred->times.renew_till) {
        printf("\n\trenew until ");
        printtime(cred->times.renew_till);
    }
    if (show_flags) {
        fputs("\n\t",stdout);
        print_flags(cred);
    }
    putchar('\n');
    free(name);
    free(sname);
}

#ifdef OSF_DCE

#define UUID_UNKNOWN (unsigned char *) "<UUID unknown>"

void print_group(id, cell)
    sec_id_t    *id;
    sec_id_t    *cell;
{
    unsigned_char_p_t   uuid_string = UUID_UNKNOWN;
    sec_rgy_name_t      gn;
    error_status_t      st;
    unsigned char       *p;

    p = id->name;
    if (!p || *p == '\0') {
        sec_id_gen_group(sec_rgy_default_handle, &cell->uuid, &id->uuid, NULL, NULL,
                            gn, &st);
        if (st) {
            p = (unsigned char *) "<group name unknown>";
        } else {
            p = gn;
        }
    }

    uuid_to_string(&id->uuid, &uuid_string, &st);
    printf("%s %s\n", uuid_string, p);

    if (st == 0) {
        rpc_string_free(&uuid_string, &st);
    }
}

void print_foreign_group(id)
    sec_id_foreign_t    *id;
{
    unsigned_char_p_t   uuid_string = UUID_UNKNOWN;
    unsigned_char_p_t   cuuid_string = UUID_UNKNOWN;
    sec_rgy_name_t      gn;
    sec_rgy_name_t      cn;
    sec_rgy_name_t      Gn;
    error_status_t      st;
    error_status_t      cst;
    unsigned char       *p;

    p = id->id.name;
    if (!p || *p == '\0') {
        sec_id_gen_group(sec_rgy_default_handle, &id->realm.uuid, &id->id.uuid,
                            Gn, cn, gn, &st);
        if (st) {
            p = (unsigned char *) "<group name unknown>";
        } else {
            p = Gn;
        }
    }

    uuid_to_string(&id->id.uuid, &uuid_string, &st);
    uuid_to_string(&id->realm.uuid, &cuuid_string, &cst);

    printf("%s %s Cell=%s\n", uuid_string, p, cuuid_string);

    if (st == 0) {
        rpc_string_free(&uuid_string, &st);
    }
    if (cst == 0) {
        rpc_string_free(&cuuid_string, &st);
    }
}


void print_pac(p)
    sec_id_pac_t    *p;
{
    unsigned char   *cell;
    unsigned char   *princ;
    sec_rgy_name_t  cn;
    sec_rgy_name_t  pn;
    sec_rgy_name_t  gn;
    error_status_t  st;
    unsigned int    i;
    unsigned_char_p_t   cuuid_string = UUID_UNKNOWN;
    unsigned_char_p_t   puuid_string = UUID_UNKNOWN;

    cell = p->realm.name;
    princ = p->principal.name;

    printf("\tGlobal Principal: ");
    if (cell && *cell && princ && *princ) {
        printf("%s/%s\n", cell, princ);
    } else {
        sec_id_gen_name(sec_rgy_default_handle, &p->realm.uuid, &p->principal.uuid,
                            gn, cn, pn, &st);
        if (st) {
            printf("<Unable to generate principal name>\n");
            if (!cell) {
                cell = (unsigned char *) "<cell name unknown>";
            }
            if (!princ) {
                princ = (unsigned char *) "<principal name unknown>";
            }
        } else {
            printf("%s\n", gn);
            cell = cn;
            princ = pn;
        }
    }

    uuid_to_string(&p->realm.uuid, &cuuid_string, &st);
    uuid_to_string(&p->principal.uuid, &puuid_string, &st);

    printf("\tCell:      %s %s\n", cuuid_string, cell);
    printf("\tPrincipal: %s %s\n", puuid_string, princ);

    printf("\tGroup:     ");
    print_group(&p->group, &p->realm);

    if (p->num_groups > 0) {
        printf("\tLocal Groups:\n");
        for (i = 0; i < p->num_groups; i++) {
            printf("\t\t");
            print_group(&p->groups[i], &p->realm);
        }
    }

    if (p->num_foreign_groups > 0) {
        printf("\tForeign Groups:\n");
        for (i = 0; i < p->num_foreign_groups; i++) {
            printf("\t\t");
            print_foreign_group(&p->foreign_groups[i]);
        }
    }
}

void print_dce_info
#ifndef __STDC__
    ( cache_name )
    char *cache_name;
#else
    ( char *cache_name )
#endif
{
    sec_login_handle_t      h;
    error_status_t          st;
    sec_login_net_info_t    net_info;
    unsigned char           err_text[dce_c_error_string_len+1];
    int                     lst;

    if (cache_name == (char *)NULL) {
        sec_login_get_current_context(&h, &st);
        if (st) {
            printf("No DCE identity available: ");
            goto err_out;
        }
    } else {
        sec_login_import_context((unsigned32) strlen(cache_name),
                                    (unsigned char *) cache_name, &h, &st);
        if (st) {
            printf("Specified credentials not found: ");
            goto err_out;
        }
    }

    printf("DCE Identity Information:\n");
    sec_login_inquire_net_info(h, &net_info, &st);
    if (st) {
        if (st == sec_login_s_not_certified) {
            printf("\tWarning: Identity information is not certified\n");
            st = 0;
        } else {
            printf("No DCE network identity information available: ");
            goto err_out;
        }
    }

    print_pac(&net_info.pac);

    printf("\n");
    printf("Identity Info Expires: ");
    if (net_info.identity_expiration_date) {
        printtime(net_info.identity_expiration_date);
    } else {
        printf("never");
    }
    printf("\nAccount Expires:       ");
    if (net_info.acct_expiration_date) {
        printtime(net_info.acct_expiration_date);
    } else {
        printf("never");
    }
    printf("\nPasswd Expires:        ");
    if (net_info.passwd_expiration_date) {
        printtime(net_info.passwd_expiration_date);
    } else {
        printf("never");
    }
    printf("\n");

err_out:
    if (st) {
        dce_error_inq_text(st, err_text, 0);

        printf("%s\n", err_text);
    }
    printf("\n");
    printf("Kerberos Ticket Information:\n");
}

#endif



