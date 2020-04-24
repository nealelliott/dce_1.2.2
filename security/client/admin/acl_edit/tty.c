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
 * Revision 1.1.7.2  1996/03/11  13:26:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:15  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:55:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:40  root]
 * 
 * Revision 1.1.5.5  1993/01/28  18:46:08  burati
 * 	CR6655 Don't recalculate mask if no group class entries were modified
 * 	[1993/01/27  22:13:20  burati]
 * 
 * Revision 1.1.5.4  1992/12/29  12:37:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:33:22  zeliff]
 * 
 * Revision 1.1.5.3  1992/12/16  17:03:14  burati
 * 	CR6478 Fix help for sec_acl_entry
 * 	[1992/12/16  16:23:57  burati]
 * 
 * Revision 1.1.5.2  1992/11/19  21:06:58  burati
 * 	CR6128 Fix typo in usage error message
 * 	[1992/11/19  17:03:08  burati]
 * 
 * Revision 1.1.2.5  1992/06/09  18:04:05  burati
 * 	CR4087 Do list after recalculating mask
 * 	[1992/06/09  18:02:16  burati]
 * 
 * Revision 1.1.2.4  1992/06/08  21:33:45  burati
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.3  1992/05/27  21:15:09  burati
 * 	Exit with status instead of hardcoded 0.
 * 	[1992/05/27  18:43:26  burati]
 * 
 * Revision 1.1.2.2  1992/05/08  15:28:10  burati
 * 	Add new functionality to support POSIX mask calculation
 * 	[1992/05/07  14:58:56  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*
**      sec_acl editor tty interface 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty.c	17 - 12/02/91";
#endif

#include <stdio.h>
#include <ctype.h>
#include <un_strng.h>
#include <un_io.h>
#include <un_maloc.h>

#include <macros.h>

#include <dce/nbase.h>
#include <dce/aclbase.h>

#include <acl_edit.h>
#include <tty_base.h>
#include <deif.h>

#include <commands.h>
#include <args.h>

COMMAND(tty_do_modify);
COMMAND(tty_do_delete);
COMMAND(tty_do_substitute);
COMMAND(tty_do_assign_file);
COMMAND(tty_do_kill_entries);
COMMAND(tty_do_list);
COMMAND(tty_do_cell);
COMMAND(tty_do_permissions);
COMMAND(tty_do_help);
COMMAND(tty_do_exit);
COMMAND(tty_do_abort);
COMMAND(tty_do_nil);
COMMAND(tty_do_nogood);
COMMAND(tty_do_test_access);
COMMAND(tty_do_get_access);
COMMAND(tty_do_commit);
COMMAND(tty_do_purge);

HELP(tty_help_modify);
HELP(tty_help_delete);
HELP(tty_help_substitute);
HELP(tty_help_assign_file);
HELP(tty_help_kill_entries);
HELP(tty_help_list);
HELP(tty_help_cell);
HELP(tty_help_entry);
HELP(tty_help_permissions);
HELP(tty_help_help);
HELP(tty_help_exit);
HELP(tty_help_abort);
HELP(tty_help_test_access);
HELP(tty_help_get_access);
HELP(tty_help_commit);
HELP(tty_help_purge);
HELP(tty_help_all);

command_table commands[] = {
    { "abort",          tty_do_abort,           tty_help_abort,         2 },
    { "assign_file",    tty_do_assign_file,     tty_help_assign_file,   2 },
    { "delete",         tty_do_delete,          tty_help_delete,        1 },
    { "cell",           tty_do_cell,            tty_help_cell,          2 },
    { "commit",         tty_do_commit,          tty_help_commit,        2 },
    { "get_access",     tty_do_get_access,      tty_help_get_access,    1 },
    { "exit",           tty_do_exit,            tty_help_exit,          1 },
    { "help",           tty_do_help,            tty_help_help,          1 },
    { "kill_entries",   tty_do_kill_entries,    tty_help_kill_entries,  1 },
    { "list",           tty_do_list,            tty_help_list,          1 },
    { "modify",         tty_do_modify,          tty_help_modify,        1 },
    { "permissions",    tty_do_permissions,     tty_help_permissions,   2 },
    { "purge",          tty_do_purge,           tty_help_purge,         2 },
    { "sec_acl_entry",  tty_help_entry,         tty_help_entry,         13 },
    { "substitute",     tty_do_substitute,      tty_help_substitute,    2 },
    { "test_access",    tty_do_test_access,     tty_help_test_access,   1 },
    { "?",              tty_do_help,            tty_help_help,          1 },
    { "",               tty_do_nil,             tty_do_nil,             0 },
    { "#",              tty_do_nil,             tty_do_nil,             1 },
    { "*",              tty_do_nil,             tty_help_all,           0 },
    { NULL,             tty_do_nogood,          tty_do_nil,             0 }
};

/* PRIVATE STATE INFORMATION
 *  It's not normally safe to keep such information global,
 * even within a module, but because the tty_* routines are
 * called from an external CLI package, that wasn't built
 * with the capability to tell it what to pass to the action
 * routines, this information must be global to these routines.
 * This is safe in this implementation because of the single
 * threaded/sequential nature of acl_edit.  These globals
 * should only be used through the following macros, so that
 * their behaviour cannot be misinterpreted.
 */

    /* POSIX mask_obj calculation permissions.  See POSIX 1003.6/D12 3.7.2.7 */
    static sec_acl_edit_mask_calc_t mask_calc_type;
    static sec_acl_permset_t        actual_perms, effective_perms, masked_perms,
                                    granted_perms, candidate_mask_perms;

#define CALC_MASK_AFTER (mask_calc_type == CALC_MASK_OBJ_AFTER)
#define MUST_CALC_MASK (mask_calc_type == POSIX_MASK_OBJ_CALC)
#define RECALC_PERMISSIONS(stp);                                            \
    if (mask_calc_type == POSIX_MASK_OBJ_CALC) {                            \
        *stp = deif_calc_union_permissions(false, &actual_perms);           \
        if (GOOD_STATUS(stp))                                               \
            *stp = deif_calc_union_permissions(true, &effective_perms);     \
        if (GOOD_STATUS(stp))                                               \
            masked_perms = actual_perms ^ effective_perms;                  \
    }
#define RESET_PERMISSIONS {\
    actual_perms = effective_perms =    \
    masked_perms = granted_perms =      \
    candidate_mask_perms = 0;           \
    }

/*
 * FUNCTION tty_do_modify
 */
PRIVATE error_status_t tty_do_modify
#ifndef __STDC__
    ( c ) 
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    error_status_t      st;
    sec_acl_entry_t     sec_acl_entry, mask_obj;
    sec_acl_permset_t   orig_perms, granted, candidate_mask_perms;
    boolean32           no_calc = false, calc_after = false;
    orig_perms = granted = candidate_mask_perms = 0;
    CLEAR_STATUS(&st);

    switch (c->num_args) {
        case 1:
            SET_STATUS(&st, ae_tty_opt_requires_arg);
            break;
        case 2:
            st = deif_validate_sec_acl_entry(*(++c->args), &sec_acl_entry, false);
            break;
        case 3:
            st = deif_validate_sec_acl_entry(*(++c->args), &sec_acl_entry, false);
            if (GOOD_STATUS(&st)) {
                c->args++;
                if (strcmp(*(c->args), "-n")==0) {
                    no_calc = true;
                } else if (strcmp(*(c->args), "-c")==0) {
                    calc_after = true;
                } else {
                    SET_STATUS(&st, ae_tty_invalid_option);
                }
            }
            break;
        default:
            SET_STATUS(&st, ae_tty_too_many_args);
    }

    if (GOOD_STATUS(&st)) {
        /* If we have to recalculate the mask, do the operation
         * in the steps according to POSIX1003.6/D12 3.7.2.7.
         */
        if (MUST_CALC_MASK && !(no_calc || calc_after) &&
	    deif_is_group_class(&sec_acl_entry)) {
            st = deif_do_modify(&sec_acl_entry, DECREASE_MOD, &orig_perms);
            if (GOOD_STATUS(&st)) {
                RECALC_PERMISSIONS(&st);
            } else if (STATUS_EQUAL(&st, sec_acl_no_acl_found)) {
                RESET_PERMISSIONS;
                CLEAR_STATUS(&st);
            }
            if (GOOD_STATUS(&st)) {
                st = deif_do_modify(&sec_acl_entry, INCREASE_MOD, &granted);
                candidate_mask_perms = effective_perms | granted;
                if (GOOD_STATUS(&st) &&
                    (masked_perms & candidate_mask_perms) != 0) {
                    /* Undo the increase */
                    sec_acl_entry.perms = orig_perms;
                    st = deif_do_modify(&sec_acl_entry, NORMAL_MOD, &granted);
                    SET_STATUS(&st, ae_tty_would_grant_perms);
                } else if (GOOD_STATUS(&st)) {  /* Replace recalculated mask */
                    mask_obj.perms = candidate_mask_perms;
                    mask_obj.entry_info.entry_type = sec_acl_e_type_mask_obj;
                    st = deif_do_modify(&mask_obj, NORMAL_MOD, &granted);
                }
            }
        } else {
            st = deif_do_modify(&sec_acl_entry, NORMAL_MOD, &granted);
            if (GOOD_STATUS(&st) && (calc_after || CALC_MASK_AFTER)) {
                st = deif_calc_mask();
            }
        }

    }

    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_delete
 */
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
    sec_acl_entry_t sec_acl_entry;

    if (c->num_args <= 1)
        SET_STATUS(&st, ae_tty_opt_requires_arg);
    else
        st = deif_validate_sec_acl_entry(*(++c->args), &sec_acl_entry, true);

    if (GOOD_STATUS(&st))
        st = deif_do_delete(&sec_acl_entry);
    if (GOOD_STATUS(&st) && deif_is_group_class(&sec_acl_entry))
        RECALC_PERMISSIONS(&st);
    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_substitute
 */
PRIVATE error_status_t tty_do_substitute
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
    sec_acl_entry_t *sec_acl_entries;
    int             i, num_entries;

    if (c->num_args <= 1)
        SET_STATUS(&st, ae_tty_opt_requires_arg);
    else {
        SET_STATUS(&st, error_status_ok);
        /* Attempt to convert each argument to a sec_acl_entry_t */
        num_entries = c->num_args - 1;
        sec_acl_entries = (sec_acl_entry_t *)
                            malloc(num_entries * sizeof(sec_acl_entry_t));
        for (i = 0; (GOOD_STATUS(&st)) && (i < num_entries); i++)
            st = deif_validate_sec_acl_entry(*(++c->args),
                                                &(sec_acl_entries[i]), false);
    }
    if (GOOD_STATUS(&st))
        st = deif_do_substitute(num_entries, sec_acl_entries);
    if (GOOD_STATUS(&st) && deif_any_group_class(num_entries, sec_acl_entries))
        RECALC_PERMISSIONS(&st);
    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_assign_file
 */
PRIVATE error_status_t tty_do_assign_file
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

    switch(c->num_args) {
        case 0:
        case 1:
            SET_STATUS(&st, ae_tty_opt_requires_arg);
            break;
        case 2:
            st = deif_sec_acl_from_file(c->args[1]);
            break;
        default:
            SET_STATUS(&st, ae_tty_too_many_args);
    }
    if (GOOD_STATUS(&st))
        RECALC_PERMISSIONS(&st);
    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_kill_entries
 */
PRIVATE error_status_t tty_do_kill_entries
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

    if (c->num_args > 1)
        SET_STATUS(&st, ae_tty_too_many_args);
    else
        st = deif_do_kill_entries();
    if (GOOD_STATUS(&st))
        RECALC_PERMISSIONS(&st);
    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_list
 */
PRIVATE error_status_t tty_do_list
#ifndef __STDC__
    ( c ) 
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    void            *dummy = NULL;
    error_status_t  st;

    if (c->num_args > 1)
        SET_STATUS(&st, ae_tty_too_many_args);
    else
        st = deif_do_list(TTY_OUTPUT, dummy);

    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_permissions
 */
PRIVATE error_status_t tty_do_permissions
#ifndef __STDC__
    ( c ) 
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    void            *dummy = NULL;
    error_status_t  st;

    if (c->num_args > 1)
        SET_STATUS(&st, ae_tty_too_many_args);
    else
        st = deif_list_permissions(TTY_OUTPUT, dummy);

    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_purge
 */
PRIVATE error_status_t tty_do_purge
#ifndef __STDC__
    ( c ) 
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    void            *dummy = NULL;
    error_status_t  st;

    if (c->num_args > 1)
        SET_STATUS(&st, ae_tty_too_many_args);
    else
        st = deif_purge_ineffective();

    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_cell
 */
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

    switch(c->num_args) {
        case 0:
        case 1:
            SET_STATUS(&st, ae_tty_opt_requires_arg);
            break;
        case 2:
            st = deif_set_default_cell(c->args[1]);
            break;
        default:
            SET_STATUS(&st, ae_tty_too_many_args);
    }
    
    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_commit
 */
PRIVATE error_status_t tty_do_commit
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

    if (c->num_args > 1)
        SET_STATUS(&st, ae_tty_too_many_args);
    else
        st = deif_save_sec_acl();
    
    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_test_access
 */
PRIVATE error_status_t tty_do_test_access
#ifndef __STDC__
    ( c ) 
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    void            *dummy = NULL;
    error_status_t  st;

    switch(c->num_args) {
        case 0:
        case 1:
            SET_STATUS(&st, ae_tty_opt_requires_arg);
            break;
        case 2:
            st = deif_do_test_access(c->args[1], TTY_OUTPUT, dummy);
            break;
        default:
            SET_STATUS(&st, ae_tty_too_many_args);
    }

    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}


/*
 * FUNCTION tty_do_get_access
 */
PRIVATE error_status_t tty_do_get_access
#ifndef __STDC__
    ( c ) 
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    void            *dummy = NULL;
    error_status_t  st;

    if (c->num_args != 1) {
        SET_STATUS(&st, ae_tty_too_many_args);
    } else {
        st = deif_do_get_access(TTY_OUTPUT, dummy);
    }

    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    return st;
}

   
PRIVATE error_status_t tty_do_exit
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t  st;
    st = deif_save_sec_acl();
    if (! GOOD_STATUS(&st))
        deif_status_print(&st, false);

    exit(st);
}


PRIVATE error_status_t tty_do_abort
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    exit(0);
}


/*
 * HELP ROUTINES
 */


PRIVATE error_status_t tty_help_modify
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

    printf("\nmodify --\n");
    printf("    Add a new sec_acl entry or change the permissions of an\n");
    printf("    existing sec_acl entry.\n\n");
    printf("    If this acl belongs to an object that requires a mask entry to\n");
    printf("    be recalculated after entry modification, then that step\n");
    printf("    will be attempted during the modify operation.  If the new\n");
    printf("    mask would unintentionally grant permissions to an existing\n");
    printf("    entry, then the modify operation will abort with an error and\n");
    printf("    acl will retain its previous state, unless -c or -n is specified\n\n");
    printf("    -n      Do not calculate a mask entry.\n");
    printf("            This option is meaningless for objects that don't\n");
    printf("            support the mask_obj type or don't require the mask\n");
    printf("            to be recalculated after modification\n");
    printf("    -c      Create or replace a mask_obj entry with permissions equal\n");
    printf("            to the union of all entries other than user_obj, other_obj\n");
    printf("            and unauthenticated. This new mask will be set even if it\n");
    printf("            does grant permissions, that were previously masked out, to\n");
    printf("            an existing entry.  It is recommended that this option only\n");
    printf("            be used if not specifying it would result in an error.\n");
    printf("Usage:\n");
    printf("%s\n", "m[odify] SEC_ACL_ENTRY [-n | -c]");
 
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

    printf("\ndelete --\n");
    printf("    Delete an existing entry from the sec_acl associated\n");
    printf("    with the specified object.\n");
    printf("Usage:\n");
    printf("%s\n", "d[elete] SEC_ACL_ENTRY");
 
    return st;
}

PRIVATE error_status_t tty_help_substitute
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

    printf("\nsubstitute --\n");
    printf("    Replace (substitute) the sec_acl information associated\n");
    printf("    with this object.  All existing entries are removed\n");
    printf("    and replaced by the newly specified sec_acl entries.\n");
    printf("Usage:\n");
    printf("%s\n", "su[bstitute] SEC_ACL_ENTRY [SEC_ACL_ENTRY ...]");
 
    return st;
}

PRIVATE error_status_t tty_help_assign_file
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

    printf("\nassign_file --\n");
    printf("    Assign the sec_acl entries contained in the specified file\n");
    printf("    to the object\n");
    printf("Usage:\n");
    printf("%s\n", "a[ssign] FILENAME");
 
    return st;
}

PRIVATE error_status_t tty_help_kill_entries
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

    printf("\nkill_entries --\n");
    printf("    Remove all sec_acl_entries (other than USER_OBJ entries)\n");
    printf("Usage:\n");
    printf("%s\n", "k[ill_entries]");
 
    return st;
}

PRIVATE error_status_t tty_help_list
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

    printf("\nlist --\n");
    printf("    List the sec_acl entries associated with the object\n");
    printf("Usage:\n");
    printf("%s\n", "l[ist]");
 
    return st;
}

PRIVATE error_status_t tty_help_permissions
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

    printf("\npermissions --\n");
    printf("    List the permission tokens and corresponding helpstrings\n");
    printf("Usage:\n");
    printf("%s\n", "pe[rmissions]");
 
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

    printf("\ncell --\n");
    printf("    Set the default cell for this sec_acl\n");
    printf("Usage:\n");
    printf("%s\n", "ce[ll] name");
 
    return st;
}

PRIVATE error_status_t tty_help_commit
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

    printf("\ncommit --\n");
    printf("    Commit all changes by replacing the ACL without exiting.\n");
    printf("Usage:\n");
    printf("%s\n", "co[mmit]");
 
    return st;
}

PRIVATE error_status_t tty_help_test_access
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

    printf("\ntest_access --\n");
    printf("    Test access to the object using the stored acl, and\n");
    printf("    the authentication information of the current process.\n");
    printf("    (Any changes to the sec_acl during this acl_edit invokation\n");
    printf("     will not affect the results of this command).\n");
    printf("Usage:\n");
    printf("%s\n", "t[est_access] permissions");
 
    return st;
}

PRIVATE error_status_t tty_help_get_access
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

    printf("\nget_access --\n");
    printf("    Determine the permissible access to the specified object,\n");
    printf("    using the stored acl and the authentication information of\n");
    printf("    the current process.\n");
    printf("    (Any changes to the sec_acl during this acl_edit invokation\n");
    printf("    will not affect the results of this command unless a commit\n");
    printf("    has been performed).\n");
    printf("Usage:\n");
    printf("g[et_access]\n");
 
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

    printf("\npurge --\n");
    printf("    Purge any ineffective (masked) permissions.\n");
    printf("Usage:\n");
    printf("pu[rge]\n");
 
    return st;
}

PRIVATE error_status_t tty_help_entry
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

    printf("\nsec_acl_entry --\n");
    printf("        A sec_acl_entry contains a set of permissions\n");
    printf("    and other information, from the following categories:\n");
    printf("        1) Type - Tag identifying the role of the entry\n");
    printf("        2) Keys - identifier for the entry (principal name)\n");
    printf("        3) Foreign keys - same as Keys, with addition of cell\n");
    printf("    Syntax:\n");
    printf("        type{:key}:permissions\n\n");
    printf("    Valid types are:\n");
    printf("        user_obj -  permissions for the implied user\n");
    printf("        group_obj - permissions for the implied group\n");
    printf("        other_obj - permissions for other principals\n");
    printf("        mask_obj  - maximum permissions for entries other than\n");
    printf("                    user_obj, other_obj and unauthenticated.\n");
    printf("        unauthenticated - permissions applied when the accessor\n");
    printf("                        does not pass authentication procedures\n");
    printf("\n        The following require the key specifier in the entry.\n");
    printf("        user      - entry contains a key identifying a user\n");
    printf("        group     - entry contains a key identifying a group\n");
    printf("        foreign_user    - entry contains a /.../cell/user key\n\n");
    printf("    Typical permissions are comprised of a combination of:\n");
    printf("        prwxid, where:\n");
    printf("        c - Grants control privileges, to modify an acl.\n");
    printf("        r - Grants read  privileges.\n");
    printf("        w - Grants write privileges.\n");
    printf("        x - Grants execute privileges.\n");
    printf("        i - Grants insert  privileges.\n");
    printf("        d - Grants delete  privileges.\n\n");
    printf("    Example sec_acl entries\n");
    printf("        user_obj:crwxid\n");
    printf("        user:mike:rwx\n");
    printf("        group:rpc:rwx\n");
    printf("        foreign_user:/.../osf.org/root:r\n\n");
    return st;
}

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
    for (i = 0; *commands[i].name != '\0'; i++) {
        (*(commands[i].help))(c);
    }
}

PRIVATE error_status_t tty_help_abort
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);
    printf("abort -- terminate session without modifying sec_acl\n");
    return st;
}

PRIVATE error_status_t tty_help_exit
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t  st;

    SET_STATUS(&st, error_status_ok);
    printf("exit -- save modified sec_acl and terminate session\n");
    return st;
}

PUBLIC void tty_sec_acl_cli
#ifndef __STDC__
    (must_prompt, prompt, mask_calc_type_p)
    int                         must_prompt;
    char                        *prompt;
    sec_acl_edit_mask_calc_t    *mask_calc_type_p;
#else
  (
    int                         must_prompt,
    char                        *prompt,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
  )
#endif
{
    /* Set modules private state, since the external CLI package
     * can't handle optional parameters.
     */
    mask_calc_type = *mask_calc_type_p; /* See private state at top */
    set_commands(commands);
    procs_cmds(must_prompt, prompt);
}


/*
 * tty_process_args
 *  Process the arguments, validating them and building
 *  and instruction list as it goes along.
 *
 *  If no action oriented argument is found return right away,
 *  letting the caller know with status: ae_tty_args_not_command
 *  so that it may enter interactive mode. 
 *
 *  If it's supposed to recalculate the mask via POSIX1003.6/D12
 *  3.7.2.7, and the new mask would grant permissions to existing
 *  entries that were previously masked out, then return with an
 *  error status immediately and do not process any further.
 */
PUBLIC error_status_t tty_process_args
#ifndef __STDC__
    (argc, args, mask_calc_type_p)
    int                         argc;
    char                        **args;
    sec_acl_edit_mask_calc_t    *mask_calc_type_p;
#else
  (
    int                         argc,
    char                        **args,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
  )
#endif
{
    error_status_t  st;
    boolean32       acl_modified, group_class_modified = false;
    boolean32       do_kill, do_list, do_sub, do_mod, do_file, do_del, do_purge;
    sec_acl_entry_t *sub_acl_entries, *del_acl_entries, *old_del_entries;
    sec_acl_entry_t *mod_acl_entries, *old_mod_entries, new_mask;
    int             i, num_sub_entries, num_del_entries, old_num_del_entries;
    int             num_mod_entries, old_num_mod_entries;
    void            *dummy = NULL;
    char            *acl_file = NULL;
 
    CLEAR_STATUS(&st);
    do_kill = do_list = do_sub = do_mod = do_file = do_del = do_purge = acl_modified = false;
    num_sub_entries = old_num_mod_entries = num_mod_entries = num_del_entries = old_num_del_entries = 0;
    actual_perms = effective_perms = masked_perms = 0;

    while ((argc > 0) && (GOOD_STATUS(&st))) {
        if (strcmp(*args,"-m") == 0) {              /* MODIFY */
            if (argc < 2) {
                SET_STATUS(&st, ae_tty_opt_requires_arg);
                deif_msg_status_print(*args, &st, false);
                argc--; args++;
            } else {
                if (do_mod) {   /* must combine sets of mods */
                    old_num_mod_entries = num_mod_entries;
                    old_mod_entries = mod_acl_entries;
                    num_mod_entries = 0;
                }
                do_mod = true;
                argc--; args++;         /* get rid of -m */
                for (i = 0;(i < argc) && (args[i]) && (*(args[i]) != '-'); i++)
                    num_mod_entries++;
                if (num_mod_entries < 1) {
                    SET_STATUS(&st, ae_tty_opt_requires_arg);
                    deif_msg_status_print("-m", &st, false);
                    argc--; args++;
                } else {
                    mod_acl_entries = (sec_acl_entry_t *) malloc(
                                    (num_mod_entries+old_num_mod_entries) *
                                    (sizeof(sec_acl_entry_t)) );
                    for (i = 0; (GOOD_STATUS(&st)) && (i < num_mod_entries); i++) {
                        st = deif_validate_sec_acl_entry(*args,
                                                        &(mod_acl_entries[i]),
                                                        false);
                        if (BAD_STATUS(&st)) {
                            deif_msg_status_print(*args, &st, false);
                        } else if ((mod_acl_entries[i].entry_info.entry_type ==
                                    sec_acl_e_type_mask_obj) &&
                                    (*mask_calc_type_p == POSIX_MASK_OBJ_CALC)) {
                            /* Don't auto recalc mask if one was explicitly given */
                            *mask_calc_type_p = NO_MASK_OBJ_CALC;
                        }
                        argc--; args++;
                    }
                    /* Copy any previously saved mod entries if > 1 -m */
                    for (i = 0; i < old_num_mod_entries; i++)
                        mod_acl_entries[i+num_mod_entries] = old_mod_entries[i];
                    if (old_num_mod_entries != 0)
                        free((char *)old_mod_entries);
                    num_mod_entries += old_num_mod_entries;
                }
            }

        } else if (strcmp(*args,"-d") == 0) {       /* DELETE */
            if (argc < 2) {
                SET_STATUS(&st, ae_tty_opt_requires_arg);
                deif_msg_status_print(*args, &st, false);
                argc--; args++;
            } else {
                if (do_del) {   /* must combine sets of dels */
                    old_num_del_entries = num_del_entries;
                    old_del_entries = del_acl_entries;
                    num_del_entries = 0;
                }
                do_del = true;
                argc--; args++;         /* get rid of -d */
                for (i = 0;(i < argc) && (args[i]) && (*(args[i]) != '-'); i++)
                    num_del_entries++;
                if (num_del_entries < 1) {
                    SET_STATUS(&st, ae_tty_opt_requires_arg);
                    deif_msg_status_print("-d", &st, false);
                    argc--; args++;
                } else {
                    del_acl_entries = (sec_acl_entry_t *) malloc(
                                    (num_del_entries+old_num_del_entries) *
                                    (sizeof(sec_acl_entry_t)) );
                    for (i = 0; (GOOD_STATUS(&st)) && (i < num_del_entries); i++) {
                        st = deif_validate_sec_acl_entry(*args,
                                                        &(del_acl_entries[i]),
                                                        true);
                        if (BAD_STATUS(&st)) {
                            deif_msg_status_print(*args, &st, false);
                        }
                        argc--; args++;
                    }
                    /* Copy any previously saved del entries if > 1 -d */
                    for (i = 0; i < old_num_del_entries; i++)
                        del_acl_entries[i+num_del_entries] = old_del_entries[i];
                    if (old_num_del_entries != 0)
                        free((char *)old_del_entries);
                    num_del_entries += old_num_del_entries;
                }
            }

        } else if (strcmp(*args,"-s") == 0) {       /* SUBSTITUTE */
            if (argc < 2) {
                SET_STATUS(&st, ae_tty_opt_requires_arg);
                deif_msg_status_print(*args, &st, false);
                argc--; args++;
            } else if (do_sub) {
                SET_STATUS(&st, ae_tty_incompatible_options);
                deif_msg_status_print("-s already specified", &st, false);
            } else if (do_file) {
                SET_STATUS(&st, ae_tty_incompatible_options);
                deif_msg_status_print("-f <file> -s <entries>", &st, false);
            } else if (do_kill) {
                SET_STATUS(&st, ae_tty_incompatible_options);
                deif_msg_status_print("-k -s <entries>", &st, false);
            } else {
                do_sub = true;
                argc--; args++;         /* get rid of -s */
                for (i = 0;(i < argc) && (args[i]) && (*(args[i]) != '-'); i++)
                    num_sub_entries++;
                if (num_sub_entries < 1) {
                    SET_STATUS(&st, ae_tty_opt_requires_arg);
                    deif_msg_status_print("-s", &st, false);
                    argc--; args++;
                } else {
                    sub_acl_entries = (sec_acl_entry_t *) malloc(
                                    num_sub_entries * sizeof(sec_acl_entry_t));
                    for (i = 0; (GOOD_STATUS(&st)) && (i < num_sub_entries); i++) {
                        st = deif_validate_sec_acl_entry(*args,
                                                        &(sub_acl_entries[i]),
                                                        false);
                        if (BAD_STATUS(&st)) {
                            deif_msg_status_print(*args, &st, false);
                        }
                        argc--; args++;
                    }
                }
            }

        } else if (strcmp(*args,"-f") == 0) {       /* ASSIGN_FILE */
            if (argc < 2) {
                SET_STATUS(&st, ae_tty_opt_requires_arg);
                deif_msg_status_print(*args, &st, false);
                argc--; args++;
            } else {
                argc = argc - 2; args++;
                if (do_file) {
                    SET_STATUS(&st, ae_tty_incompatible_options);
                    deif_msg_status_print("-f already specified", &st, false);
                } else if (do_sub) {
                    SET_STATUS(&st, ae_tty_incompatible_options);
                    deif_msg_status_print("-s <entries> -f <file>", &st, false);
                } else if (do_kill) {
                    SET_STATUS(&st, ae_tty_incompatible_options);
                    deif_msg_status_print("-k -f <file>", &st, false);
                } else {
                    acl_file = (*args); do_file = true;
                }
                args++;
            }

        } else if (strcmp(*args,"-k") == 0) {       /* KILL_ENTRIES */
            argc--; args++;
            if (do_sub) {
                SET_STATUS(&st, ae_tty_incompatible_options);
                deif_msg_status_print("-s <entries> -k", &st, false);
            } else if (do_file) {
                SET_STATUS(&st, ae_tty_incompatible_options);
                deif_msg_status_print("-f <file> -k", &st, false);
            } else {
                do_kill = true;
            }

        } else if (strcmp(*args,"-l") == 0) {       /* LIST */
            argc--; args++;
            do_list = true;

        } else if (strcmp(*args,"-p") == 0) {       /* PURGE INEFFECTIVE */
            argc--; args++;
            do_purge = true;

            /* Ignore these since they were handled at invokation time */
        } else if ((strcmp(*args,"-ic") == 0) ||    /* INITIAL DIR DEFAULT  */
                   (strcmp(*args,"-io") == 0) ||    /* INITIAL FILE DEFAULT */
                   (strcmp(*args,"-v") == 0)  ||    /* VERBOSE FLAG         */
                   (strcmp(*args,"-e") == 0)  ||    /* BIND TO ENTRY FLAG   */
                   (strcmp(*args,"-n") == 0)  ||    /* DON'T RECALC MASK    */
                   (strcmp(*args,"-c") == 0)  ||    /* RECALC MASK AT END   */
                   (strcmp(*args,"-ngui") == 0)) {  /* NO GRAPHICS UI       */
            argc--; args++;
        } else {
            SET_STATUS(&st, ae_tty_invalid_argument);
            deif_msg_status_print(*args, &st, false);
            argc--; args++;
        }
    }

        /* If illegal arguments or no command given, return now */
    if (BAD_STATUS(&st) ||
        (!(do_kill || do_list || do_sub || do_mod || do_file || do_del || do_purge)) ) {
        if (GOOD_STATUS(&st)) {
            SET_STATUS(&st, ae_tty_args_not_command);
        }
        return st;
    }

    /* Purge ineffective permissions from all file group class entries
     * that are not specified with -m -f or -s by performing this
     * operation first, if -p was specified.
     */
    if (GOOD_STATUS(&st) && do_purge) {
        st = deif_purge_ineffective();
        if (GOOD_STATUS(&st)) {
            acl_modified = true;
        }
    }

    /* The Kill-entries, Substitute or Acl-From-File operation must
     * be done next, and only one of them is allowed to be performed
     * per invocation, as documented in POSIX 1003.6/D12.
     */
    if (GOOD_STATUS(&st) && do_kill) {
        st = deif_do_kill_entries();
        if (GOOD_STATUS(&st)) {
            acl_modified = true;
        } else {
            deif_msg_status_print("Cannot update acl", &st, false);
        }
    } else if (GOOD_STATUS(&st) && do_sub) {
        st = deif_do_substitute(num_sub_entries, sub_acl_entries);
        if (GOOD_STATUS(&st)) {
            acl_modified = true;
            group_class_modified = (group_class_modified ||
                deif_any_group_class(num_sub_entries, sub_acl_entries));
        } else {
            deif_msg_status_print("Cannot substitute acl entries", &st, false);
        }
    } else if (GOOD_STATUS(&st) && do_file) {
        st = deif_sec_acl_from_file(acl_file);
        if (GOOD_STATUS(&st)) {
            acl_modified = true;
        } else {
            deif_msg_status_print("Cannot update acl", &st, false);
        }
    }

    /* Do all deletions before any modifications, per POSIX 1003.6/D12 */
    if (GOOD_STATUS(&st) && do_del) {
        for (i = 0; i < num_del_entries; i++) {
            st = deif_do_delete(&(del_acl_entries[i]));
        }
        if (GOOD_STATUS(&st)) {
            acl_modified = true;
            group_class_modified = (group_class_modified ||
                deif_any_group_class(num_del_entries, del_acl_entries));
        } else {
            deif_msg_status_print("Cannot delete entry", &st, false);
        }
    }

    /* Perform any mods that will decrease an entry's permissions */
    if (GOOD_STATUS(&st) && do_mod) {
        st = deif_do_decrease_mods(num_mod_entries, mod_acl_entries);
        if (GOOD_STATUS(&st)) {
            acl_modified = true;
            group_class_modified = (group_class_modified ||
                deif_any_group_class(num_mod_entries, mod_acl_entries));
        } else {
            deif_msg_status_print("Cannot modify acl", &st, false);
        }
    }

    if (GOOD_STATUS(&st) && (*mask_calc_type_p == POSIX_MASK_OBJ_CALC)) {
        /* Calculate the union of actual perms of all maskable entries
         * (POSIX 1003.6/D12 section 3.7.2.7 step (3)
         */
        st = deif_calc_union_permissions(false, &actual_perms);
        if (BAD_STATUS(&st)) {
            deif_status_print(&st, false);
            return st;
        }

        /* Calculate the union of the effective perms of all maskable entries
         * (POSIX 1003.6/D12 section 3.7.2.7 step (4)), and XOR the result with
         * the result of step (3), to determine which perms differ (step (5)).
         * (ie: which permissions are not granted because of the current mask)
         */
        st = deif_calc_union_permissions(true, &effective_perms);
        if (BAD_STATUS(&st)) {
            deif_status_print(&st, false);
            return st;
        }
        masked_perms = actual_perms ^ effective_perms;
    }

    /* Perform any mods that will increase an entry's perms or add entries */
    if (GOOD_STATUS(&st) && do_mod) {
        st = deif_do_increase_mods(num_mod_entries, mod_acl_entries, &granted_perms);
        if (GOOD_STATUS(&st)) {
            acl_modified = true;
            group_class_modified = (group_class_modified ||
                deif_any_group_class(num_mod_entries, mod_acl_entries));
        }
    }

    if (GOOD_STATUS(&st) && (*mask_calc_type_p == POSIX_MASK_OBJ_CALC) &&
	acl_modified && group_class_modified) {
        /* Calculate the union of the new perms granted, with the old
         * effective permissions from step (4).  This is the candidate
         * new mask value. (POSIX 1003.6/D12 section 3.7.2.7 step (7))
         */
        candidate_mask_perms = effective_perms | granted_perms;

        /* Any permissions in this new mask that are also in the set
         * of permissions that differ between the original sets of actual
         * and effective permissions (the previously masked-out perms)
         * implies that this new mask would unexpectedly (on the user's
         * part) grant some new right to an entry.  Since we wouldn't
         * be in this part of the mask calculation if the user had
         * specified -n (don't recalc mask) or -c (calc after all mods
         * and create the mask even if it does grant new rights), then
         * we can't continue and must terminate with an error, per step (8).
         */
        if ((masked_perms & candidate_mask_perms) != 0) {
            SET_STATUS(&st, ae_tty_would_grant_perms);
            deif_status_print(&st, false);
        } else {
            new_mask.perms = candidate_mask_perms;
            new_mask.entry_info.entry_type = sec_acl_e_type_mask_obj;
            st = deif_do_modify(&new_mask, NORMAL_MOD, &granted_perms);
            if (BAD_STATUS(&st)) {
                deif_msg_status_print("Cannot apply new mask", &st, false);
            }
        }
    }

    /* Save the acl if it was modified and no errors occurred */
    if ((GOOD_STATUS(&st)) && (acl_modified)) {
        if (*mask_calc_type_p == CALC_MASK_OBJ_AFTER)
            st = deif_calc_mask();
        if (GOOD_STATUS(&st))
            st = deif_save_sec_acl();
        if (BAD_STATUS(&st)) {
            deif_status_print(&st, false);
        }
    }

    if (GOOD_STATUS(&st) && do_list) {
        st = deif_do_list(TTY_OUTPUT, dummy);
        if (BAD_STATUS(&st)) {
            deif_msg_status_print("LIST failed", &st, false);
        }
    }

    return st;
}
