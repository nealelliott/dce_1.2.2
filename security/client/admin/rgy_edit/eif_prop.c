/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_prop.c,v $
 * Revision 1.1.6.2  1996/03/11  13:26:41  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:40  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:46  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:39:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:14  zeliff]
 * 
 * Revision 1.1.2.3  1992/05/08  18:19:43  burati
 * 	Use sec_rgy_name_t instead of ek_pname_buf_t for names
 * 	[1992/05/07  23:10:49  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  19:37:27  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:38:46  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  eif_prop.c V=13 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic pgo interface properties/policies management
**
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)eif_prop.c	13 - 03/02/92";
#endif

#include <stdio.h>
#include <un_strng.h>
#include <macros.h>

#include <eif_prop.h>
#include <eif_util.h>
#include <eif.h>

#include <ek_error.h>
#include <ek_types.h>

#include <tty_io.h>
#include <tty_prop.h>


/*
** Public routines
*/


PUBLIC error_status_t eif_change_policy
#ifndef __STDC__
    ( op_handle )
    ek_op_handle_t  op_handle;
#else
  (
    ek_op_handle_t  op_handle
  )
#endif
{
    ek_name_buf_t   name;
    long            name_len;
    error_status_t  st;

    st = ek_do_op(op_handle);

    if (BAD_STATUS(&st)) {
	ek_read_field(op_handle, ekn_policy_key, name, sizeof(name), &name_len);
	if (strlen(name) == 0)
	    tty_print_status("Unable to set Registry policy", st);
	else
	    tty_status_post_echo(name, "Unable to set policy for org", st);

   }

    return st;
}


PUBLIC error_status_t eif_change_auth_policy
#ifndef __STDC__
    ( op_handle )
    ek_op_handle_t  op_handle;
#else
  (
    ek_op_handle_t  op_handle
  )
#endif
{
    ek_name_buf_t   name;
    long            name_len;
    error_status_t  st;

    st = ek_do_op(op_handle);

    if (BAD_STATUS(&st)) {
        ek_read_field(op_handle, ekn_auth_policy_pname, name, sizeof(name),
                                                                    &name_len);
        if (strlen(name) == 0)
            tty_print_status("Unable to set Registry authorization policy",st);
        else
            tty_status_post_echo(name,
                        "Unable to set authorization policy for account", st);
   }

    return st;
}


PUBLIC boolean32 eif_write_policy_field
#ifndef __STDC__
    ( op_handle, field, data, check_default, stp )
    ek_op_handle_t  op_handle;
    ek_field_id_t   field;
    char            *data;
    boolean32       check_default;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
  )
#endif
{
    ek_op_id_t      cur_op;
    sec_rgy_name_t  default_val;
    long            default_len;
    char            policy_err[256];
    char            op_text[8];
    boolean32       registry_policy,
                    ret_val = true;

    cur_op = ek_get_op_id(op_handle);

    SET_STATUS(stp, error_status_ok);

    if (check_default) {
	ek_read_field(op_handle, field, (char *)default_val,
                        sizeof(default_val), &default_len);
	if ( (strlen(data) == default_len) &&
	     (strncmp((char *) default_val, data, default_len) == 0) )
	    return false;
    }

    if (cur_op == ekn_policy_get)
	strcpy(op_text, "get");
    else
	strcpy(op_text, "set");


    if (strlen(data) == 0) {
	sprintf(policy_err, "Unable to %s Registry policy", op_text);
	registry_policy = true;
    } else {
	sprintf(policy_err, "Unable to %s policy for org", op_text);
	registry_policy = false;
    }

    *stp = ek_write_field(op_handle, field, data, strlen(data));

    if (BAD_STATUS(stp)) {
	ret_val = false;

	switch (field) {
	case ekn_policy_key:
	    if (registry_policy)
		tty_print_status(policy_err, *stp);
	    else
		tty_status_post_echo(data, policy_err, *stp);
	break;

	case ekn_policy_passwd_exp:
	case ekn_policy_passwd_life:
	case ekn_policy_acct_life:
	case ekn_policy_passwd_min_len:
	    tty_status_post_echo(data, "", *stp);
	    break;

	case ekn_policy_passwd_no_spaces:
	case ekn_policy_passwd_non_alpha:
	    tty_print_status("", *stp);
	break;
	}

    }
    return ret_val;
}


PUBLIC boolean32 eif_write_auth_policy_field
#ifndef __STDC__
    ( op_handle, field, data, check_default, stp)
    ek_op_handle_t  op_handle;
    ek_field_id_t   field;
    char            *data;
    boolean32       check_default;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
  )
#endif
{
    ek_op_id_t      cur_op;
    sec_rgy_name_t  default_val;
    long            default_len;
    char            policy_err[256];
    char            op_text[8];
    boolean32       registry_policy,
                    ret_val = true;

    cur_op = ek_get_op_id(op_handle);

    SET_STATUS(stp, error_status_ok);

    if (check_default) {
        ek_read_field(op_handle, field, (char *) default_val,
                       sizeof(default_val), &default_len);
        if ( (strlen(data) == default_len) &&
             (strncmp((char *)default_val, data, default_len) == 0) )
            return false;
    }

    if (cur_op == ekn_auth_policy_get)
        strcpy(op_text, "get");
    else
        strcpy(op_text, "set");

    if (strlen(data) == 0) {
        sprintf(policy_err, "Unable to %s Registry auth policy", op_text);
        registry_policy = true;
    } else {
        sprintf(policy_err, "Unable to %s auth policy for account", op_text);
        registry_policy = false;
    }

    *stp = ek_write_field(op_handle, field, data, strlen(data));

    if (BAD_STATUS(stp)) {
        ret_val = false;

        switch (field) {
        case ekn_auth_policy_pname:
        case ekn_auth_policy_gname:
        case ekn_auth_policy_oname:
            if (registry_policy)
                tty_print_status(policy_err, *stp);
            else
                tty_status_post_echo(data, policy_err, *stp);
        break;

        case ekn_auth_policy_max_ticket_lifetime:
        case ekn_auth_policy_max_renewable_lifetime:
            tty_status_post_echo(data, "", *stp);
            break;
        }
    }
    return ret_val;
}


PUBLIC boolean32 eif_write_lrgy_prop_field
#ifndef __STDC__
    ( op_handle, field, data, check_default, stp)
    ek_op_handle_t  op_handle;
    ek_field_id_t   field;
    char            *data;
    boolean32       check_default;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
  )
#endif
{
    sec_rgy_name_t       default_val;
    long                 default_len;
    char                 prop_err[256];
    ek_login_name_buf_t  expanded_owner;
    boolean32            is_wild;
    boolean32            ret_val = true;

    SET_STATUS(stp, error_status_ok);

    if (check_default) {
	ek_read_field(op_handle, field, (char *)default_val, sizeof(default_val),
		       &default_len);
	if ( (strlen(data) == default_len) &&
	     (strncmp((char *)default_val, data, default_len) == 0) )
	    return false;
    }

    sprintf(prop_err, "Unable to set local registry properties");

    if (GOOD_STATUS(stp)) {

	*stp = ek_write_field(op_handle, field, data, strlen(data));
	
	if (BAD_STATUS(stp)) {
	    ret_val = false;
	}
    }

    return ret_val;
}


/* Don't complain about "not implemented" status returns.  Leave it up to
 * the caller
 */


PUBLIC boolean32 eif_write_prop_field
#ifndef __STDC__
    ( op_handle, field, data, check_default, stp)
    ek_op_handle_t  op_handle;
    ek_field_id_t   field;
    char            *data;
    boolean32       check_default;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
  )
#endif
{
    sec_rgy_name_t       default_val;
    long                 default_len;
    char                 prop_err[256];
    ek_login_name_buf_t  expanded_owner;
    boolean32            is_wild;
    boolean32            ret_val = true;

    SET_STATUS(stp, error_status_ok);

    if (check_default) {
	ek_read_field(op_handle, field, (char *)default_val, sizeof(default_val),
		       &default_len);
	if ( (strlen(data) == default_len) &&
	     (strncmp((char *)default_val, data, default_len) == 0) )
	    return false;
    }

    sprintf(prop_err, "Unable to set registry properties");

    if (GOOD_STATUS(stp)) {

	*stp = ek_write_field(op_handle, field, data, strlen(data));
	
	if (BAD_STATUS(stp)) {
	    ret_val = false;
	
	    switch (field) {
	    case ekn_policy_passwd_no_spaces:
	    case ekn_policy_passwd_non_alpha:
		tty_print_status("", *stp);
		break;
	    }
	}
    }

    return ret_val;
}


/* eif_change_prop
 *
 * Can't do the check for a readonly site because that would prevent a
 * caller form ever toggling the registry out of readonly mode
 */

PUBLIC error_status_t eif_change_prop
#ifndef __STDC__
    ( op_handle )
    ek_op_handle_t  op_handle;
#else
  (
    ek_op_handle_t  op_handle
  )
#endif
{
    error_status_t  st;

    st = ek_do_op(op_handle);

    if (BAD_STATUS(&st)) {
	tty_print_status("Unable to set Registry properties", st);
    }

    return st;
}
