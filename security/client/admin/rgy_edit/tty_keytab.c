/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_keytab.c,v $
 * Revision 1.1.8.2  1996/03/11  13:27:19  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:12  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:01:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:40  root]
 * 
 * Revision 1.1.6.1  1994/04/01  20:08:18  jd
 * 	Undefine _NO_PROTO  so the cma prototypes don;t complain. Tis presumably
 * 	won't need to be undef'd when the security cleanup happens.
 * 	[1994/03/28  20:30:24  jd]
 * 
 * Revision 1.1.4.2  1992/12/29  12:42:05  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:36  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/03  19:57:23  burati
 * 	CR3981 Improve ktadd usage and parameter checking
 * 	[1992/06/03  19:38:48  burati]
 * 
 * Revision 1.1.2.2  1992/05/08  18:21:09  burati
 * 	Fix another usage statement
 * 	[1992/05/07  23:25:32  burati]
 * 
 * 	Get rid of extraneous usage statement.
 * 	[1992/05/07  23:12:26  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty_keytab.c V=19 11/18/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**      rgy_edit Key Table manipulation
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_keytab.c	19 - 11/18/91";
#endif
#undef _NO_PROTO
#include <stdio.h>
#include <ctype.h>

#include <com_err.h>
#include <dce/rgybase.h>

#include <bstring.h>
#include <un_strng.h>
#include <un_maloc.h>
#include <commands.h>
#include <args.h>                                 
#include <ek_db.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>     /* for krb5_read_password */
#include <krb5/sysincl.h>       /* for MAXPATHLEN */
#include <krb5/func-proto.h>

#include <tty_base.h>
#include <tty_util.h>
#include <tty_keytab.h>
#include <tty_io.h>
#include <args.h>
#include <macros.h>

extern krb5_kt_ops krb5_ktf_writable_ops;

PRIVATE boolean32   kt_registered = false;
krb5_keyblock       master_keyblock;
krb5_encrypt_block  master_encblock;

PRIVATE void tty_keytab_add_usage(
#ifdef __STDC__
    void
#endif
)
{
    printf("Usage: kta[dd] -p <principal> [-r[egistry]][-a | -pw password][-f keyfile]\n");
}

PRIVATE void tty_keytab_delete_usage(
#ifdef __STDC__
    void
#endif
)
{
    printf("Usage: ktd[elete] -p <principal> -v <version-number> [-f keyfile]\n");
}

PRIVATE void tty_keytab_list_usage(
#ifdef __STDC__
    void
#endif
)
{
    printf("Usage: ktl[ist] [-p <principal>] [-f keyfile]\n");
}

PUBLIC error_status_t tty_do_ktadd
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    krb5_error_code retval;
    krb5_keytab     ktid;
    char            password[BUFSIZ];
    int             i, size = sizeof(password);
    char            ktname[MAXPATHLEN+sizeof("WRFILE:")+1];
    char            *key_file = NULL;
    char            *principal = NULL;
    error_status_t  st;
    boolean32       update_registry = false;
    boolean32       autogenerate = false;
    boolean32	    passflag = false;
                  
    CLEAR_STATUS(&st);

    if(c->num_args < 3 || c->num_args > 9) {
        tty_keytab_add_usage();
        return st;
    }  
 
    for (i = 1; (GOOD_STATUS(&st)) && i < c->num_args; i++) {
        if (match_command("-r", c->args[i], 2)) {
            update_registry = true;
        } else if (match_command("-f", c->args[i], 2)) {
            if ((i+1) < c->num_args) {
                i++;
                key_file = c->args[i];
            } else {
                OPT_REQUIRES_ARG(c->args[i], &st);
            }
        } else if (match_command("-p", c->args[i], 2)) {
            if ((i+1) < c->num_args) {
                i++;
                principal = c->args[i];
            } else {
                OPT_REQUIRES_ARG(c->args[i], &st);
            }
        } else if (match_command("-pw", c->args[i], 2)) {
            if (autogenerate) {
                INCOMPATIBLE_OPTS("-a", "-pw", &st);
            } else if ((i+1) < c->num_args) {
                i++;
                strcpy(password,c->args[i]);
		passflag = true;
            } else {
                OPT_REQUIRES_ARG(c->args[i], &st);
            }
        } else if (match_command("-a", c->args[i], 2)) {
                if (passflag) {
                    INCOMPATIBLE_OPTS("-pw", "-a", &st);
                } else {
                    autogenerate = true;
                    password[0] = '\0';
                }
        } else {
            INVALID_OPT(c->args[i], &st);
        }
    }

    if (BAD_STATUS(&st)) {
        return st;
    }

    /* Principal must have been specified */
    if (! principal) {
        tty_keytab_add_usage();
        return st;
    }  

    if (autogenerate && (!update_registry)) {
        tty_print_status("-a[utogenerate] option only valid with -r[egistry]",
                            tty_invalid_option);
        return st;
    }
  
    if (! autogenerate) {
      if (! passflag) {
        if (retval = krb5_read_password("Enter password:", 
                                        "Re-enter password to verify:",
                                        password,
                                        &size)) {
            com_err("keytab", retval, "Error in input password. Add failed.");
            return st;
        }
      }
    }
    ek_key_add(key_file, principal, password, update_registry, &st);
    bzero(password, sizeof(password)); /* erase it */
    if (BAD_STATUS(&st)) {
        tty_print_status("Unable to add key", st);
    }

    return st;
}

PUBLIC error_status_t tty_do_ktdelete
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    char            *key_file  = NULL;
    char            *principal = NULL;
    int             i;
    error_status_t  st;
    unsigned32      key_vno = -1;

    CLEAR_STATUS(&st);

    if(c->num_args < 5 || c->num_args > 7) {
        tty_keytab_delete_usage();
        return st;
    }  
 
    for (i = 1; i < c->num_args; i++) {
        if (match_command("-f", c->args[i], 2)) {
            if ((i+1) < c->num_args) {
                i++;
                key_file = c->args[i];
            } else
                OPT_REQUIRES_ARG(c->args[i], &st);
        } else if (match_command("-p", c->args[i], 2)) {
            if ((i+1) < c->num_args) {
                i++;
                principal = c->args[i];
            } else
                OPT_REQUIRES_ARG(c->args[i], &st);
        } else if (match_command("-v", c->args[i], 2)) {
            if ((i+1) < c->num_args) {
                i++;
                if (! isdigit(*(c->args[i])) ) {
                    tty_keytab_delete_usage();
                    return st;
                } else {
                    key_vno = (unsigned32) atoi(c->args[i]);
                }
            } else
                OPT_REQUIRES_ARG(c->args[i], &st);
        } else {
            INVALID_OPT(c->args[i], &st);
            return st;
        }
    }

    /* Principal must have been specified */
    if ((! principal) || (key_vno == -1)) {
        tty_keytab_delete_usage();
        return st;
    }  

    ek_key_delete(key_file, principal, key_vno, &st);
    if (BAD_STATUS(&st)) {
        tty_print_status("Unable to delete key from table", st);
    }

    return st;
}

PUBLIC error_status_t tty_do_ktlist
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t  st;
    unsigned32      key_vno;
    int             i;
    char            *principal = "";
    char            *key_file = NULL;
    char            *nextprinc;
    void            *cursor;

    CLEAR_STATUS(&st);

    if(c->num_args > 5) {
        tty_keytab_list_usage();
        return st;
    }  

    for (i = 1; i < c->num_args; i++) {
        if (match_command("-p", c->args[i], 2)) {
            if ((i+1) < c->num_args) {
                i++;
                principal = c->args[i];
            } else
                OPT_REQUIRES_ARG(c->args[i], &st);
        } else if (match_command("-f", c->args[i], 2)) {
            if ((i+1) < c->num_args) {
                i++;
                key_file = c->args[i];
            } else
                OPT_REQUIRES_ARG(c->args[i], &st);
        } else {
            INVALID_OPT(c->args[i], &st);
            return st;
        }
    }

    ek_start_key_list(&cursor, key_file, principal, &st);
    if (BAD_STATUS(&st)) {
        tty_print_status("Unable to retrieve key(s)", st);
    } else {
        for (i = 0; GOOD_STATUS(&st); i++) {
            ek_do_key_list(cursor, &nextprinc, &key_vno, &st);
            if (GOOD_STATUS(&st)) {
                printf("%s\t\t%d\n", nextprinc, key_vno);
                free(nextprinc);
            }
        }
        if (i <= 1) {
            tty_print_status("Unable to retrieve key(s)", st);
        }
        ek_end_key_list(&cursor, &st);
    }
    return st;
}

