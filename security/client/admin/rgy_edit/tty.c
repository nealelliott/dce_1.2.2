/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty.c,v $
 * Revision 1.1.9.2  1996/03/11  13:27:05  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:00  marty]
 *
 * Revision 1.1.9.1  1995/12/08  18:00:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/27  16:40 UTC  mdf
 * 	Fix for Defect Number CHFts14370
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_6/1  1995/02/27  16:38 UTC  mdf
 * 	Fix for CHFts14370
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  22:42 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:33 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  16:30:22  root]
 * 
 * Revision 1.1.7.5  1994/09/09  19:12:03  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:36:13  mob]
 * 
 * Revision 1.1.7.4  1994/03/28  21:34:24  sekhar
 * 	Fixed history corrupted by the SVC code drop.
 * 	[1994/03/28  20:45:59  sekhar]
 * 
 * Revision 1.1.7.3  1994/03/25  16:27:44  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 
 * Revision 1.1.7.2  1994/03/07  23:09:33  mob
 * 	Fix for OT 10070
 * 	[1994/03/07  23:01:42  mob]
 * 
 * Revision 1.1.7.1  1994/03/02  17:54:25  hanfei
 * 	Removal of hp version string
 * 	[1994/02/25  14:48:45  hanfei]
 * 
 * 	Initial ERA drop
 * 	[1994/02/24  21:09:34  hanfei]
 * 
 * Revision 1.1.7.2  1993/07/08  10:53:27  root
 * 	Initial King Kong branch
 * 	[1993/07/08  10:52:39  root]
 * 
 * Revision 1.1.5.2  1993/05/25  16:31:47  rps
 * 	standardize version string
 * 	[1993/05/25  16:21:55  rps]
 * 
 * 	Bug 6413 - call setlocale for correct i18n.
 * 	[1993/01/08  23:12:02  tom]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:05  zeliff]
 * 
 * 	CR6472 Fix cell help
 * 	[1992/12/15  23:34:20  burati]
 * 
 * 	CR3305 Enhancement to allow group aliases
 * 	[1992/11/28  22:41:31  burati]
 * 
 * 	CR5732 Need to specify -u for unix num on pgo view cmd
 * 	[1992/10/23  19:50:03  burati]
 * 
 * 	CR5068 Add help for missing -q option to change cmd
 * 	CR4530 Fix typo in policy cmd usage message
 * 	[1992/09/24  22:17:02  burati]
 * 
 * 	First security replication code drop
 * 	[1992/08/31  14:56:21  sekhar]
 * 
 * Revision 1.1.1.11  1993/05/04  20:23:32  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.4  1992/06/03  19:57:00  burati
 * 	CR3981 Improve ktadd help.
 * 	[1992/06/03  19:37:09  burati]
 * 
 * Revision 1.1.2.3  1992/06/02  15:25:53  burati
 * 	CR3307 change help could use more info on wildcards
 * 	CR3906 Don't allow scope command in account domain.
 * 	[1992/06/01  22:29:21  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  20:14:11  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:55:52  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface
*/

#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include <un_strng.h>
#include <un_io.h>
#include <locale.h>

#include <dce/dce.h>
#include <macros.h>
#include <rgymacro.h>
#include <rgy_obj_names.h>

#include <dce/nbase.h>

#include <com_err.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>   /* for krb5_read_password */

#include <tty_base.h>
#include <ek_error.h>
#include <ek_types.h>

#include <tty_acct.h>
#include <tty_def.h>
#include <tty_err.h>
#include <tty_io.h>
#include <tty_pgo.h>
#include <tty_prop.h>
#include <tty_util.h>
#include <tty_conf.h>

#include <eif_site.h>
#include <eif_pgo.h>
#include <eif_acct.h>

#include <commands.h>
#include <args.h>

#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

PUBLIC boolean32        tty_exit_on_error = false;
PUBLIC error_status_t   tty_exit_status = error_status_ok;

COMMAND(tty_do_lproperties);
COMMAND(tty_do_cell);
COMMAND(tty_do_add_cell);
COMMAND(tty_do_domain);
COMMAND(tty_do_view);
COMMAND(tty_do_lview);
COMMAND(tty_do_add);
COMMAND(tty_do_change);
COMMAND(tty_do_delete);
COMMAND(tty_do_ldelete);
COMMAND(tty_do_adopt);
COMMAND(tty_do_properties);
COMMAND(tty_do_site);
COMMAND(tty_do_member);
COMMAND(tty_do_help);
COMMAND(tty_do_quit);
COMMAND(tty_do_login);
COMMAND(tty_do_defaults);
COMMAND(tty_do_policy);
COMMAND(tty_do_purge);
COMMAND(tty_do_auth_policy);
COMMAND(tty_do_ktadd);
COMMAND(tty_do_ktdelete);
COMMAND(tty_do_ktlist);
COMMAND(tty_do_scope);
COMMAND(tty_do_nogood);
COMMAND(tty_do_nil);

HELP(tty_help_all);
HELP(tty_help_add);
HELP(tty_help_adopt);
HELP(tty_help_auth_policy);
HELP(tty_help_cell);
HELP(tty_help_change);
HELP(tty_help_defaults);
HELP(tty_help_delete);
HELP(tty_help_ldelete);
HELP(tty_help_domain);
HELP(tty_help_help);
HELP(tty_help_ktadd);
HELP(tty_help_ktdelete);
HELP(tty_help_ktlist);
HELP(tty_help_lifespan);
HELP(tty_help_login);
HELP(tty_help_lproperties);
HELP(tty_help_member);
HELP(tty_help_policy);
HELP(tty_help_properties);
HELP(tty_help_purge);
HELP(tty_help_quit);
HELP(tty_help_scope);
HELP(tty_help_site);
HELP(tty_help_view);
HELP(tty_help_lview);

command_table commands[] = {
    { "add",        tty_do_add,             tty_help_add,        1 },
    { "adopt",      tty_do_adopt,           tty_help_adopt,      3 },
    { "authpolicy", tty_do_auth_policy,     tty_help_auth_policy,2 },
    { "cell",       tty_do_cell,            tty_help_cell,       2 },
    { "change",     tty_do_change,          tty_help_change,     1 },
    { "defaults",   tty_do_defaults,        tty_help_defaults,   3 },
    { "delete",     tty_do_delete,          tty_help_delete,     3 },
    { "domain",     tty_do_domain,          tty_help_domain,     2 },
    { "exit",       tty_do_quit,            tty_help_quit,       1 },
    { "help",       tty_do_help,            tty_help_help,       1 },
    { "ktadd",      tty_do_ktadd,           tty_help_ktadd,      3 },
    { "ktdelete",   tty_do_ktdelete,        tty_help_ktdelete,   3 },
    { "ktlist",     tty_do_ktlist,          tty_help_ktlist,     3 },
    { "lifespan",   tty_help_lifespan,      tty_help_lifespan,   8 },
    { "login",      tty_do_login,           tty_help_login,      1 },
    { "member",     tty_do_member,          tty_help_member,     1 },
    { "policy",     tty_do_policy,          tty_help_policy,     2 },
    { "properties", tty_do_properties,      tty_help_properties, 2 },
    { "quit",       tty_do_quit,            tty_help_quit,       1 },
    { "scope",      tty_do_scope,           tty_help_scope,      2 },
    { "site",       tty_do_site,            tty_help_site,       2 },
    { "view",       tty_do_view,            tty_help_view,       1 },
    { "",           tty_do_nil,             tty_do_nil,          0 },
    { "#",          tty_do_nil,             tty_do_nil,          1 },
    { "*",          tty_do_nil,             tty_help_all,        0 },
    { NULL,         tty_do_nogood,          tty_do_nil,          0 }
};

command_table lcommands[] = {
    { "delete",     tty_do_ldelete,         tty_help_ldelete,       1 },
    { "exit",       tty_do_quit,            tty_help_quit,          1 },
    { "help",       tty_do_help,            tty_help_help,          1 },
    { "lifespan",   tty_help_lifespan,      tty_help_lifespan,      8 },
    { "properties", tty_do_lproperties,     tty_help_properties,    2 },
    { "purge",      tty_do_purge,           tty_help_purge,         2 },
    { "quit",       tty_do_quit,            tty_help_quit,          1 },
    { "view",       tty_do_lview,           tty_help_lview,         1 },
    { "",           tty_do_nil,             tty_do_nil,             0 },
    { "#",          tty_do_nil,             tty_do_nil,             1 },
    { "*",          tty_do_nil,             tty_help_all,           0 },
    { NULL,         tty_do_nogood,          tty_do_nil,             0 }
};

boolean32 opt_local_rgy = false;

/* FUNCTION tty_do_view
*/
PRIVATE error_status_t tty_do_view
#ifndef __STDC__
    ( c )
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{

    error_status_t        st;


    SET_STATUS(&st, error_status_ok);

    switch (Current_domain) {
    case eif_domain_acct:
        st = tty_do_view_acct(c);
        break;
    case eif_domain_person:
    case eif_domain_group:
    case eif_domain_org:
        st = tty_do_view_pgo(c, Current_domain);
        break;
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PRIVATE error_status_t tty_do_add
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t  st;

    /* Don't bother unless an update site is available */
    st = eif_site_check(ek_site_update, eif_auto_rebind);
    if (BAD_STATUS(&st)) {
        tty_print_status("", st);
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    }

    switch (Current_domain) {
    case eif_domain_acct:
        st = tty_do_add_acct(c);
        break;
    case eif_domain_person:
    case eif_domain_group:
    case eif_domain_org:
        st = tty_do_add_pgo(c, Current_domain);
        break;
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PRIVATE error_status_t tty_do_cell
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t  st;

    /* Don't bother unless an update site is available */
    st = eif_site_check(ek_site_update, eif_auto_rebind);
    if (BAD_STATUS(&st)) {
        tty_print_status("", st);
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    }

    st = tty_do_add_cell(c);
    if (BAD_STATUS(&st))
        tty_print_status("", st);
    return st;
}


PRIVATE error_status_t tty_do_change
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t  st;

    /* Don't bother unless an update site is available */
    st = eif_site_check(ek_site_update, eif_auto_rebind);
    if (BAD_STATUS(&st)) {
        tty_print_status("", st);
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    }

    switch (Current_domain) {
    case eif_domain_acct:
        st = tty_do_change_acct(c);
        break;
    case eif_domain_person:
    case eif_domain_group:
    case eif_domain_org:
        st = tty_do_change_pgo(c, Current_domain);
        break;
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PRIVATE error_status_t tty_do_delete
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t  st;

    /* Don't bother unless an update site is available */
    st = eif_site_check(ek_site_update, eif_auto_rebind);

    if (BAD_STATUS(&st)) {
        tty_print_status("", st);
    } else {
        switch (Current_domain) {

        case eif_domain_person:
        case eif_domain_group:
        case eif_domain_org:
            st = tty_do_delete_pgo(c, Current_domain);
            break;
        case eif_domain_acct :
            st = tty_do_delete_acct(c);
           break;
        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


/*
 * tty_do_ldelete - Attempt to delete entry from local registry.
 */
PRIVATE error_status_t tty_do_ldelete
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    sec_rgy_name_t  name;
    boolean32       got_name = false;
    boolean32       overflow;
    error_status_t  st, st2;
    CLEAR_STATUS(&st);

    switch (c->num_args) {
        case 1:
            break;
        case 2:
            got_name = true;
            strncpy((char *)name, c->args[1], sec_rgy_name_max_len);
            name[sec_rgy_name_max_len] = '\0';
            break;
        default:
            SET_STATUS(&st, tty_invalid_argument);
            tty_print_status("Too many arguments", st);
    }
    if (GOOD_STATUS(&st)) {
        /* Can't do anything without a name */
        if (!got_name) {
            tty_show_prompt("Name: ");
            if (! tty_get_string((char *)name, sizeof(name), &overflow))
                return st;
            if (overflow) {
                SET_STATUS(&st, tty_err_input_too_long);
                tty_print_status("INVALID name", st);
                return st;
            }
        }
    }
    if (GOOD_STATUS(&st)) {
        if (!ek_do_lrgy_delete(name, &st, &st2)) {
            if (BAD_STATUS(&st)) {
                tty_print_status("Unable to delete account entry", st);
            }
            if (BAD_STATUS(&st2)) {
                tty_print_status("Unable to delete TGT entry", st2);
            }
        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PRIVATE error_status_t tty_do_login
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    int                 cur_arg;
    error_status_t      st;
    krb5_error_code     retval;
    ek_pname_buf_t      password;
    int                 size = sizeof(password);
    ek_login_name_buf_t lname;
    boolean32           overflow;
    boolean32           got_name = false;

    SET_STATUS(&st, error_status_ok);
    cur_arg = 1;

    switch (c->num_args) {
        case 1:
            break;
        case 2:
            got_name = true;
            strncpy((char *)lname, c->args[1], ek_lim_max_login_name_len);
            lname[ek_lim_max_login_name_len] = '\0';
            break;
        default:
            SET_STATUS(&st, tty_invalid_argument);
            tty_print_status("Too many arguments", st);
    }

    if (GOOD_STATUS(&st)) {

        /* Can't do anything without a username */
        if (!got_name) {
            tty_show_prompt("Username: ");
            if (! tty_get_string(lname, sizeof(lname), &overflow))
                return st;
            if (overflow) {
                SET_STATUS(&st, tty_err_input_too_long);
                tty_print_status("INVALID Username", st);
                return st;
            }
        }

        /* Use the krb5 utility routine for obtaining passwords */
        if (retval = krb5_read_password("Password: ",
                                        NULL,
                                        password,
                                        &size)) {
            com_err("login", retval, "Error in input password. Login failed.");
            return st;
        }

        if (GOOD_STATUS(&st)) {
            if (!ek_login(lname, password, &st)) {
                tty_print_status("Login failed.", st);
            }
        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}




PRIVATE error_status_t tty_do_domain
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    char            output_buff[20];
    int             cur_arg;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);

    if (c->num_args == 1)
        strcpy(output_buff, "Current domain is:");
    else {
        cur_arg = 1;
        while (cur_arg < c->num_args && (GOOD_STATUS(&st)) ) {

            if (cur_arg == 1) {
                if (match_command(SEC_PRINCIPAL_OBJ_NAME, c->args[cur_arg], 1)) {
                    Current_domain = eif_domain_person;
                } else if (match_command(SEC_GROUP_OBJ_NAME, c->args[cur_arg], 1)) {
                    Current_domain = eif_domain_group;
                } else if (match_command(SEC_ORG_OBJ_NAME, c->args[cur_arg], 1)) {
                    Current_domain = eif_domain_org;
                } else if (match_command("account", c->args[cur_arg], 1)) {
                    Current_domain = eif_domain_acct;
                } else {
                    INVALID_ARG(c->args[cur_arg], &st);
                }
            } else {
                INVALID_ARG(c->args[cur_arg], &st);
            }

             cur_arg++;

        }

        if (GOOD_STATUS(&st))
            strcpy(output_buff, "Domain changed to:");
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

        /* if we get here then the domain command line was properly formatted */
    switch (Current_domain) {

    case eif_domain_person:
        printf("%s %s\n", output_buff, SEC_PRINCIPAL_OBJ_NAME);
        break;
    case eif_domain_group:
        printf("%s %s\n", output_buff, SEC_GROUP_OBJ_NAME);
        break;
    case eif_domain_org:
        printf("%s %s\n", output_buff, SEC_ORG_OBJ_NAME);
        break;
    case eif_domain_acct:
        printf("%s account\n", output_buff);
        break;
    }

    return st;
}


/* tty_do_site
 *
 */

PRIVATE error_status_t tty_do_site
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    int      cur_arg = 1;    /* ignore the command name slot in argv*/
    boolean32  opt_query    = false,
             opt_update   = false,
             arg_site     = false;
    char     *site;

    error_status_t  st;

    SET_STATUS(&st, error_status_ok);
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {

        if (match_command("-update", c->args[cur_arg], 2)) {
            if (opt_query) {
                INCOMPATIBLE_OPTS("-u", "-q", &st);
            } else if (arg_site) {
                INVALID_OPT(c->args[cur_arg], &st);
            } else {
                opt_update = true;
            }
        } else if (match_command("-query", c->args[cur_arg], 2)) {
            if (opt_update) {
                INCOMPATIBLE_OPTS("-u", "-q", &st);
            } else if (arg_site) {
                INVALID_OPT(c->args[cur_arg], &st);
            } else {
                opt_query = true;
            }
        }
        else {
            if (opt_update || opt_query || arg_site) {
                INVALID_ARG(c->args[cur_arg], &st);
            } else {
                arg_site = true;
                site = c->args[cur_arg];
            }

        }

        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    if (arg_site) {
        st = eif_site_open(site);
    } else if (opt_query) {
        st = eif_site_open_by_access(ek_site_query);
    } else if (opt_update) {
        st = eif_site_open_by_access(ek_site_update);
    } else {
        eif_site_print_current();
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


/*
 * tty_do_scope
 */
PRIVATE error_status_t tty_do_scope
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t  st;

    CLEAR_STATUS(&st);

    if (Current_domain == eif_domain_acct) {
        SET_STATUS(&st, tty_bad_domain);
    } else {

        switch(c->num_args) {
            case 1: printf("\nScope for ");
                    switch (Current_domain) {
                        case eif_domain_person:
                        printf("%s domain is %s\n", SEC_PRINCIPAL_OBJ_NAME,
                                Current_scope[sec_rgy_domain_person]);
                        break;
                    case eif_domain_group:
                        printf("%s domain is %s\n", SEC_GROUP_OBJ_NAME,
                                Current_scope[sec_rgy_domain_group]);
                        break;
                    case eif_domain_org:
                        printf("%s domain is %s\n", SEC_ORG_OBJ_NAME,
                                Current_scope[sec_rgy_domain_org]);
                        break;
                    default:
                        SET_STATUS(&st, tty_bad_domain);
                        break;
                    }
                    break;
            case 2: switch (Current_domain) {
                        case eif_domain_person:
                            strcpy((char *)Current_scope[sec_rgy_domain_person],
                                    c->args[1]);
                            break;
                        case eif_domain_group:
                            strcpy((char *)Current_scope[sec_rgy_domain_group],
                                    c->args[1]);
                            break;
                        case eif_domain_org:
                            strcpy((char *)Current_scope[sec_rgy_domain_org],
                                    c->args[1]);
                            break;
                        default:
                            SET_STATUS(&st, tty_bad_domain);
                            break;
                    }
                    break;
            default:
                    SET_STATUS(&st, tty_invalid_argument);
                    break;
        }
    }
    if (BAD_STATUS(&st)) {
        tty_print_status("Scope not set", st);
    }
    return st;
}


PRIVATE error_status_t tty_do_quit
#ifndef __STDC__
    ( c )
   command_buf *c;
#else
  (
   command_buf *c
  )
#endif
{
    if (! opt_local_rgy) {
        ek_login_cleanup();
    }
    printf("bye.\n");
    exit(tty_exit_status);   /* return the last failure status */
}



/* Help Routines */

PRIVATE error_status_t tty_help_all
#ifndef __STDC__
    ( c )
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    int             i;
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);
    if (opt_local_rgy) {
        for (i = 0; *lcommands[i].name != '\0'; i++) {
            (*(lcommands[i].help))(c);
        }
    } else {
        for (i = 0; *commands[i].name != '\0'; i++) {
            (*(commands[i].help))(c);
        }
    }
}


PRIVATE error_status_t tty_help_ktadd
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nAdd to Server Key Table\n");
    printf("\nUsage:\n");
    printf("kta[dd] -p <principal> [-r[egistry]][-a | -pw password][-f keyfile]\n");
    printf("    -r[egistry]     Update the key in the registry also.\n");
    printf("    -a              Automatically generate a random key.\n");
    printf("    -f keyfile      Optional keytable file name.\n");
    printf("    REQUIRED:       -p <principal>\n");
    printf("                    If -a is specified, -r must also be specified.\n");
    printf("    NOTES:          Updating the key (password) in the registry with\n");
    printf("                    -r option requires the API to pass in the previous\n");
    printf("                    key for this principal.  Thus, there must be a valid\n");
    printf("                    key in the keytable already, in order to use the -r option.\n");
    printf("                    The -a and -pw options may not be used at the same time.\n");
    printf("                    It will prompt for the password if -pw or -a not specified.\n");
    return st;
}


PRIVATE error_status_t tty_help_ktdelete
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nDelete from Server Key Table\n");
    printf("\nUsage:\n");
    printf("ktd[elete] -p <principal> -v <version-number> [-f keyfile]\n");
    printf("    -v version-num  Version number of the key to delete.\n");
    printf("    -f keyfile      Optional keytable file name.\n");

    return st;
}


PRIVATE error_status_t tty_help_ktlist
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nList Server Key Table\n");
    printf("\nUsage:\n");
    printf("ktl[ist] [-p <principal>] [-f keyfile]\n");
    printf("    -p principal    Optional principal to list keys for.\n");
    printf("    -f keyfile      Optional keytable file name.\n");

    return st;
}


PRIVATE error_status_t tty_help_domain
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n");
    printf("%s\n", "do[main] [ p[rincipal] | g[roup] | o[rg] | a[ccount] ]");

    return st;
}


PRIVATE error_status_t tty_help_scope
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nscope - Limit the scope of information displayed by the 'view' command\n");
    printf("        to the specified directory name.  Use the command with no options\n");
    printf("        to display the current scope.  Not valid in the account domain.\n\n");
    printf("\nUsage:\n");
    printf("%s\n", "sc[ope] [name]");

    return st;
}


PRIVATE error_status_t tty_help_view
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n");
    switch (Current_domain) {
        case eif_domain_acct:
            printf("v[iew] [ pname [gname [oname]]] [-f]\n");
            break;
        case eif_domain_org:
            printf("v[iew] [name | -u unix_num] [-f] [-m] [-po]\n");
            break;
        case eif_domain_person:
        case eif_domain_group:
        default:
            printf("v[iew] [name | -u unix_num] [-f] [-m]\n");
            break;
    }

    return st;
}


PRIVATE error_status_t tty_help_lview
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    CLEAR_STATUS(&st);
    printf("\nUsage:\n%s\n", "v[iew]                          -- View local registry account entries");
    return st;
}


PRIVATE error_status_t tty_help_add
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nADD - dependent on current domain.\n");
    switch(Current_domain) {
        case eif_domain_person:
            printf("\nUsage:\n");
            printf("a[dd] [principal_name [unix_num][-f fullname][-q quota][-al] ]\n");
            break;
        case eif_domain_group:
            printf("\nUsage:\n");
            printf("a[dd] [group_name  [unix_num][-f fullname][-nl][-al]]\n");
            break;
        case eif_domain_org:
            printf("\nUsage:\n");
            printf("a[dd] [org_name    [unix_num][-f fullname]]\n");
            break;
        case eif_domain_acct:
            printf("a[dd] [principal] \n");
            printf("        [ -g group -o org -mp password [-pw password | -rp]\n");
            printf("        [-m misc_info] [-h homedir] [-s shell] [-gsd good_since_date]\n");
            printf("        [-x account_exp_date|none] [-pv | -pnv] [-av | -anv] ]\n");
            printf("        [-mcr lifespan] [-mcl lifespan] [-ena[ble] flag | -dis[able] flag]\n");
            printf("        OPTIONS:    -g      group name\n");
            printf("                    -o      org name\n");
            printf("                    -mp     my-password, for key validation\n");
            printf("                    -pw     new password for account\n");
            printf("                    -rp     generate random password\n");
            printf("                    -pv     password valid\n");
            printf("                    -pnv    password not valid\n");
            printf("                    -av     account valid\n");
            printf("                    -anv    account not valid\n");
            printf("        FLAGS:      c[lient], s[erver] po[stdated] f[orwardable] pr[oxiable]\n");
            printf("                    T[GT_authentication] r[enewable] dup[_session_key]\n");
            printf("        DEFAULTS:   See 'defaults' for setting/displaying default values\n");
            printf("        NOTES:      If any options are specified on the command line,\n");
            printf("                    rgy_edit will not prompt for values, so all the\n");
            printf("                    required options would also have to be specified.\n");
            printf("                    To use interactive mode, do not specify any options.\n");
            printf("        REQUIRED:   principal, group, org, my-password, password or -rp\n");
            printf("                    (these must be entered via interactive mode or options)\n");
            break;

    }
    return st;
}


PRIVATE error_status_t tty_help_cell
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n");
    printf("ce[ll] cellname [-ul unix_num] [-uf unix_num] [-gl gname] [-ol oname]\n");
    printf("                [-gf gname] [-of oname] [-mp passwd]\n");
    printf("                [-fa name] [-fp passwd]\n");
    printf("                [-q quota] [-x account_expiration_date]\n\n");
    printf("        OPTIONS:    -ul Unix number for the locally stored cell principal\n");
    printf("                    -uf Unix number for cell principal stored in foreign cell\n");
    printf("                    -gl group name for cell account stored in local cell\n");
    printf("                    -ol org name for cell account stored in local cell\n");
    printf("                    -gf group name for cell account stored in foreign cell\n");
    printf("                    -of org name for cell account stored in foreign cell\n");
    printf("                    -mp invoker's password\n");
    printf("                    -q  object creation quota for the new cell accounts\n");
    printf("                    -fa foreign cell administrator's name\n");
    printf("                    -fp foreign cell administrator's password\n");
    printf("\n        DEFAULTS:   quota = 0; account_expiration_date = none\n");
    printf("                    unix_num = auto generate\n");
    printf("\nAll arguments that do not have default values are required.\n");
    printf("If any are missing, interactive mode is entered.\n");
    return st;
}


PRIVATE error_status_t tty_help_change
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    switch(Current_domain) {
        case eif_domain_person:
            printf("\nUsage:\n");
            printf("%s\n", "c[hange] [principal_name [new_fields] [-al|-pr]]");
            printf("%s\n", "     new_fields may include:");
            printf("%s\n", "         [-n new_name] [-f new_fullname] [-q quota]");
            break;
        case eif_domain_group:
            printf("\nUsage:\n");
            printf("%s\n", "c[hange] [group_name  [new_fields] [-nl|-l] [-al|-pr]]");
            printf("%s\n", "     new_fields may include:");
            printf("%s\n", "         [-n new_name] [-f new_fullname]");
            break;
        case eif_domain_org:
            printf("\nUsage:\n");
            printf("%s\n", "c[hange] [org_name    [new_fields]]");
            printf("%s\n", "     new_fields may include:");
            printf("%s\n", "         [-n new_name] [-f new_fullname]");
            break;
        case eif_domain_acct:
            printf("%s\n",   "c[hange]  [-p pname] [-g gname] [-o oname] [-np new_pname] [-ng new_group]");
            printf("%s\n",   "          [-no new_org] [ [-h homedir] [-s shell] [-pnv|-pv] [-m misc_info]");
            printf("%s\n",   "          [-rp | -pw passwd] -mp password] [-gsd good_since_date] [-anv|-av]");
            printf("%s\n",   "          [-x account_exp_date|none] [-ena[ble] flag | -dis[able] flag] ]");
            printf("%s\n",   "        OPTIONS:    -p      principal name");
            printf("%s\n",   "                    -g      group name");
            printf("%s\n",   "                    -o      org name");
            printf("%s\n",   "                    -np     new principal name");
            printf("%s\n",   "                    -ng     new group name");
            printf("%s\n",   "                    -no     new org name");
            printf("%s\n",   "                    -mp     my-password, for key validation");
            printf("%s\n",   "                    -pw     new password for account");
            printf("%s\n",   "                    -rp     generate random password");
            printf("%s\n",   "                    -pv     password valid");
            printf("%s\n",   "                    -pnv    password not valid");
            printf("%s\n",   "                    -av     account valid");
            printf("%s\n",   "                    -anv    account not valid");
            printf("%s\n",   "        FLAGS:      c[lient], s[erver] po[stdated] f[orwardable] pr[oxiable]");
            printf("%s\n",   "                    T[GT_authentication] r[enewable] dup[_session_key]");
            printf("%s\n",   "        DEFAULTS:   Fields that aren't explicitly changed, retain their values.");
            printf("%s\n",   "        NOTES:      If any options are specified on the command line,");
            printf("%s\n",   "                    rgy_edit will not prompt for values, so all the");
            printf("%s\n",   "                    required options would also have to be specified.");
            printf("%s\n",   "                    To use interactive mode, do not specify any options.");
            printf("%s\n",   "                    If any of P G or O are not specified in options or");
            printf("%s\n",   "                    interactive mode, then a wildcard change will be attempted.");
            printf("%s\n",   "        REQUIRED:   If -pw or -rp are specified, then -mp must also be");
            printf("%s\n",   "                    specified, to ensure the callers identity.");
            printf("%s\n",   "                    Wildcard password changes are not allowed.  If the password");
            printf("%s\n",   "                    is to be changed, then the principal, group and org must be"); 
            printf("%s\n",   "                    specified in either interactive mode or via -p -g and -o.");
            break;

        default:
            break;
    }
    return st;
}


PRIVATE error_status_t tty_help_delete
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n");
    printf("%s\n",  "del[ete] pgo_name");
    printf("%s\n",  "         (for the account domain, at least one of the");
    printf("%s\n",  "          following argument pairs must be specified)");
    printf("%s\n",  "del[ete] [-p pname] [-g gname] [-o oname]");

    return st;
}


PRIVATE error_status_t tty_help_ldelete
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n%s\n", "d[elete] name                   -- Delete local registry entry");

    return st;
}


PRIVATE error_status_t tty_help_properties
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n%s\n", "pr[operties]                    -- Display/Set Properties.");

    return st;
}


PRIVATE error_status_t tty_help_purge
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n%s\n", "pu[rge]                         -- Purge expired entries.");

    return st;
}


PRIVATE error_status_t tty_help_site
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t  st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n%s\n", "si[te]  [[address] | -u[pdate] | -q[uery]]");

    return st;
}


PRIVATE error_status_t tty_help_member
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n%s\n", "m[ember] [group_name|org_name [-a member_list] [-r member_list]]");

    return st;
}


PRIVATE error_status_t tty_help_adopt
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t  st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n");
    printf("ado[pt] uuid-string principal_name [-u unix_num] [-f fullname] [-q quota] |\n");
    printf("                    group_name  [-u unix_num] [-f fullname] [-nl] |\n");
    printf("                    org_name    [-u unix_num] [-f fullname]\n");

    return st;
}

PRIVATE error_status_t tty_help_lifespan
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nLIFESPAN -- lifespan fields may be entered in one of the following ways:\n\n");
    printf("    'forever'   - Designates that the specified attribute is good forever\n");
    printf("                  (never expires; equivalent to the value '0').\n");
    printf("    number      - Entering a decimal number specifies that you want that number\n");
    printf("                  of units, of the type listed in the prompt.  For example:\n");
    printf("                  Enter acct lifespan in days or 'forever': (52w1d) 90\n");
    printf("                  (The default value was 52 weeks and 1 day.  By entering the\n");
    printf("                   value 90 only, we have selected the default units type for\n");
    printf("                   this field, which the prompt says is: 'days')\n");
    printf("    [#w][#d][#h][#m][#[s]]\n");
    printf("                - If any of {w,d,h,m,s} are found in the entered string, then\n");
    printf("                  the edit kernel assumes the lifespan is of this format, with\n");
    printf("                  each of these specifiers separating a particular unit of the\n");
    printf("                  lifespan (weeks, days, hours, minutes, seconds respectively).\n");
    printf("                  If any other non-numeric characters are found, the lifespan\n");
    printf("                  is invalid, and an error is returned.\n");
    printf("                  Any combination of the tokens may be used (ie: 12w8h30s),\n");
    printf("                  but if no specifier is found for the last (rightmost) token,\n");
    printf("                  (ie: 12w8h30) then the last number will be assumed to mean\n");
    printf("                  seconds. (This does not apply to the previous rule, where no\n");
    printf("                  specifiers are used, and only a decimal number is entered.)\n");
    return st;
}

PRIVATE error_status_t tty_help_login
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n%s\n", "lo[gin]  [user_name]");
}

PRIVATE error_status_t tty_help_policy
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n");
    printf("pol[icy] [[org_name] [-al acct_lifespan]\n");
    printf("                     [-pl passwd_lifespan] [-pm passwd_min_len]\n");
    printf("                     [-px passwd_exp_date|none] [-pa|-pna] [-ps|-pns]]\n");

}

PRIVATE error_status_t tty_help_auth_policy
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nDisplay/Modify the registry wide authorization policy\n\n");
    printf("\nUsage:\n");
    printf("%s\n", "au[thpolicy]");

}


PRIVATE error_status_t tty_help_defaults
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nUsage:\n");
    printf("def[aults]\n");

}


PRIVATE error_status_t tty_help_quit
#ifndef __STDC__
    ( c )
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t   st;
    SET_STATUS(&st, error_status_ok);

    printf("\nq[uit]                          -- terminate session\n");

}

error_status_t eif_do_cl_args
#ifndef __STDC__
    ( argc, argv, cl_op )
    int      argc;
    char     **argv;
    boolean32  *cl_op;
#else
  (
    int      argc,
    char     **argv,
    boolean32  *cl_op
  )
#endif
{
    int      cur_arg = 1;    /* ignore the command name slot in argv*/
    boolean32  opt_person   = false,
             opt_group    = false,
             opt_org      = false,
             opt_acct     = false,
             opt_full     = false,
             opt_member   = false,
             opt_policy   = false,
             opt_view     = false,
             opt_site     = false,
             opt_update   = false,
             opt_no_rebind = false,
             opt_exit_on_error = false,
             opt_no_query = false,
             arg_key      = false,
             list_by_num  = false;

    char     null_key[1],
             *pkey        = null_key, /* assume no key entered on command line */
             *gkey        = null_key, /* assume no key entered on command line */
             *okey        = null_key, /* assume no key entered on command line */
             *site        = NULL;

    error_status_t    st;
    ek_site_access_t  access;

    ek_op_handle_t    view_handle;

    *null_key = '\0';
    *cl_op    = false;   /* assume no command line operation requested */

    SET_STATUS(&st, error_status_ok);
    while (cur_arg < argc && GOOD_STATUS(&st)) {

        if (match_command("-p", argv[cur_arg], 2)) {
            if (opt_group) {
                INCOMPATIBLE_OPTS("-p", "-g", &st);
            } else if (opt_org) {
                INCOMPATIBLE_OPTS("-p", "-o", &st);
            } else if (opt_acct) {
                INCOMPATIBLE_OPTS("-p", "-a", &st);
            } else if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-p", "-l", &st);
            } else {
                opt_person = true;
            }
        } else if (match_command("-g", argv[cur_arg], 2)) {
            if (opt_person) {
                INCOMPATIBLE_OPTS("-p", "-g", &st);
            } else if (opt_org) {
                INCOMPATIBLE_OPTS("-g", "-o", &st);
            } else if (opt_acct) {
                INCOMPATIBLE_OPTS("-g", "-a", &st);
            } else if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-g", "-l", &st);
            } else {
                opt_group = true;
            }
        } else if (match_command("-o", argv[cur_arg], 2)) {
            if (opt_person) {
                INCOMPATIBLE_OPTS("-p", "-o", &st);
            } else if (opt_group) {
                INCOMPATIBLE_OPTS("-g", "-o", &st);
            } else if (opt_acct) {
                INCOMPATIBLE_OPTS("-o", "-a", &st);
            } else if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-o", "-l", &st);
            } else {
                opt_org = true;
            }
        } else if (match_command("-l", argv[cur_arg], 2)) {
            if (opt_person) {
                INCOMPATIBLE_OPTS("-l", "-p", &st);
            } else if (opt_group) {
                INCOMPATIBLE_OPTS("-l", "-g", &st);
            } else if (opt_org) {
                INCOMPATIBLE_OPTS("-l", "-o", &st);
            } else if (opt_acct) {
                INCOMPATIBLE_OPTS("-l", "-a", &st);
            } else if (opt_no_rebind) {
                INCOMPATIBLE_OPTS("-l", "-nr", &st);
            } else if (opt_view) {
                INCOMPATIBLE_OPTS("-l", "-v", &st);
            } else if (opt_no_query) {
                INCOMPATIBLE_OPTS("-l", "-nq", &st);
            } else if (opt_full) {
                INCOMPATIBLE_OPTS("-l", "-f", &st);
            } else if (opt_member) {
                INCOMPATIBLE_OPTS("-l", "-m", &st);
            } else if (opt_policy) {
                INCOMPATIBLE_OPTS("-l", "-po", &st);
            } else if (opt_exit_on_error) {
                INCOMPATIBLE_OPTS("-l", "-e", &st);
            } else if (opt_update) {
                INCOMPATIBLE_OPTS("-l", "-u", &st);
            } else if (opt_site) {
                INCOMPATIBLE_OPTS("-l", "-s", &st);
            } else if (arg_key) {
                INCOMPATIBLE_OPTS("-l", pkey, &st);
            } else {
                opt_local_rgy = true;
            }
        } else if (match_command("-a", argv[cur_arg], 2)) {
            if (opt_person) {
                INCOMPATIBLE_OPTS("-p", "-a", &st);
            } else if (opt_group) {
                INCOMPATIBLE_OPTS("-g", "-a", &st);
            } else if (opt_org) {
                INCOMPATIBLE_OPTS("-o", "-a", &st);
            } else if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-l", "-a", &st);
            } else {
                opt_acct = true;
            }
        } else if (match_command("-nr", argv[cur_arg], 3)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-nr", "-l", &st);
            } else {
                opt_no_rebind = true;
            }
        } else if (match_command("-v", argv[cur_arg], 2)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-v", "-l", &st);
            } else {
                opt_view = true;
            }
        } else if (match_command("-nq", argv[cur_arg], 3)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-nq", "-l", &st);
            } else {
                opt_no_query = true;
            }
        } else if (match_command("-f", argv[cur_arg], 2)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-f", "-l", &st);
            } else {
                opt_full = true;
            }
        } else if (match_command("-m", argv[cur_arg], 2)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-m", "-l", &st);
            } else {
                opt_member = true;
            }
        } else if (match_command("-po", argv[cur_arg], 3)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-po", "-l", &st);
            } else {
                opt_policy = true;
            }
        } else if (match_command("-e", argv[cur_arg], 2)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-e", "-l", &st);
            } else {
                opt_exit_on_error = true;
            }
        } else if (match_command("-update", argv[cur_arg], 3)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-update", "-l", &st);
            } else {
                opt_update = true;
            }
        } else if (match_command("-s", argv[cur_arg], 2)) {
            if (opt_local_rgy) {
                INCOMPATIBLE_OPTS("-s", "-l", &st);
            }
            if ((cur_arg + 2) > argc) {
                OPT_REQUIRES_ARG(argv[cur_arg], &st);
            } else {
                cur_arg++;
                opt_site = true;
                site = argv[cur_arg];
            }
        } else if (match_command("-unix_number", argv[cur_arg], 3)) {
            if (arg_key) {
                INVALID_OPT(argv[cur_arg], &st);
            } else if ((cur_arg + 2) > argc) {
                OPT_REQUIRES_ARG(argv[cur_arg], &st);
            } else {
                cur_arg++;
                arg_key = list_by_num = true;
                pkey = argv[cur_arg];
            }
        } else {
                /* if there's an argument, it must be a key associated with
                 * a view request.  We can't insist that a -v option have an
                 * argument because a null argument means "show the whole
                 * database".  if they enter more than one argument we report
                 * it as unknown.
                 */
            if (IS_AN_OPTION(argv[cur_arg])) {
                INVALID_OPT(argv[cur_arg], &st);
            } else if (!arg_key) {
                pkey = argv[cur_arg];
                arg_key = true;
                if (opt_local_rgy) {
                    INCOMPATIBLE_OPTS(pkey, "-l", &st);
                }
            } else {
                INVALID_ARG(argv[cur_arg], &st);
                SET_STATUS(&st, tty_invalid_option);
            }
        }

        cur_arg++;
    }

        /* Nothing else to do if local_rgy or something went wrong */
    if (BAD_STATUS(&st) || opt_local_rgy)  return st;

        /* the -f option is only valid if -v was specified */
    if (opt_full && !opt_view) {
        INVALID_OPT("-f", &st);
        return st;
    }

        /* an argument is only valid in conjunction with the -v option */
    if (arg_key && !opt_view) {
        INVALID_ARG(pkey, &st);
        return st;
    }

        /* policy only applies if we're viewing organizations */
    if (opt_policy && !(opt_view && opt_org)) {
        INVALID_OPT("-po", &st);
        return st;
    }

        /* membership info only applies if we're viewing pgo items */
    if (opt_member && (!opt_view || (Current_domain == eif_domain_acct))) {
        INVALID_OPT("-m", &st);
        return st;
    }

    if (opt_exit_on_error) {
        tty_exit_on_error = true;
    }
         /* bind to a specific site or an update site if the user made a
         * site request on the command line, otherwise just bind to an arbitrary
         * readonly site.
         */
    if (opt_site) {
        st = eif_site_open(site);
    } else if (opt_update) {
        st = eif_site_open_by_access(ek_site_update);
        if (STATUS_EQUAL(&st, ek_misc_read_only)) {
                /* we got a read-only site.  Close it and return a
                 * ek_misc_no_update_site */
            ek_site_close();
            SET_STATUS(&st, ek_misc_no_update_site);
        }
    } else {
        st = eif_site_open_by_access(ek_site_query);
    }

    if (opt_no_query) {
        eif_verify = false;  /* don't verify update requests */
    }

    if (BAD_STATUS(&st))  return st;

      /* set the domain.  default is account */
    if (opt_person)
        Current_domain = eif_domain_person;
    else if (opt_group)
        Current_domain = eif_domain_group;
    else if (opt_org)
        Current_domain = eif_domain_org;
    else if (opt_acct)
        Current_domain = eif_domain_acct;
    else
        Current_domain = eif_domain_acct;

    if (opt_no_rebind)
        eif_auto_rebind = false;   /* user doesn't want auto_rebinding */

    if (opt_view) {
        *cl_op = true;
        if (Current_domain == eif_domain_acct)
            st = eif_view_acct(pkey, gkey, okey, opt_full);
        else
          st = eif_view_pgo(Current_domain, pkey, opt_full, opt_member,
                            opt_policy, list_by_num);
    }

    return st;
}

void rgy_edit_pthread_init (
#ifdef __STDC__
    void
#endif
)
{
#ifdef CMA_INCLUDE
    sigset_t    sigset;
    pthread_t   dt;

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);

    dt = pthread_self();
    pthread_signal_to_cancel_np(&sigset, &dt);
#endif
}

int main
#ifndef __STDC__
    ( argc, argv )
    int   argc;
    char  **argv;
#else
  (
    int   argc,
    char  **argv
  )
#endif
{
    boolean32         must_prompt,
                      cl_op;       /* an operation specified on the cmd line */
    error_status_t    clean_st,
                      st;

    setlocale(LC_ALL, "");

    /*
     * Setup pthread state - make sure interrupts get mapped into cancels.
     */
    rgy_edit_pthread_init();

    dce_svc_set_progname("rgy_edit", &st);
    dce_msg_define_msg_table(sad__table,
        sizeof sad__table / sizeof sad__table[0], &st);

    st = eif_init();

    if (GOOD_STATUS(&st)) {
        st = eif_do_cl_args(argc, argv, &cl_op);
    }

    if (isatty(0)) {
        must_prompt = true;
    } else {
        must_prompt = false;
        eif_verify = false;     /* don't verify updates if input isn't a tty */
    }

    if (GOOD_STATUS(&st) && !cl_op) {
        if (opt_local_rgy) {
            set_commands(lcommands);
        } else {
            set_commands(commands);
        }
        procs_cmds(must_prompt, Major_prompt);
    }

    SET_EXIT_STATUS(&st);
    exit(tty_exit_status);      /* return the last failure status */
}

