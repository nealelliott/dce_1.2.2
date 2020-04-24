/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_acct.c,v $
 * Revision 1.1.8.2  1996/03/11  13:26:35  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:35  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:58:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/27  16:40 UTC  mdf
 * 	Fix for Defect Number CHFts14370
 * 
 * 	HP revision /main/mdf_MOTHRA_6/1  1995/02/27  16:37 UTC  mdf
 * 	Fix for Defect Number CHFts14370
 * 	[1995/12/08  16:29:29  root]
 * 
 * Revision 1.1.6.1  1994/07/15  14:58:13  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:13:19  mdf]
 * 
 * Revision 1.1.4.4  1992/12/29  12:38:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:51  zeliff]
 * 
 * Revision 1.1.4.3  1992/11/20  21:14:41  burati
 * 	CR5909 Don't allow duplicate error messages on account delete
 * 	[1992/11/20  18:43:48  burati]
 * 
 * Revision 1.1.4.2  1992/11/05  16:20:37  burati
 * 	CR4803 Need better error msgs on change account failures
 * 	[1992/11/04  22:54:58  burati]
 * 
 * Revision 1.1.2.4  1992/06/03  20:05:41  burati
 * 	CR3986 Make sure it initializes delete form before each delete
 * 	[1992/06/03  18:05:17  burati]
 * 
 * Revision 1.1.2.3  1992/05/08  18:18:35  burati
 * 	Use sec_rgy_name_t instead of ek_pname_buf_t for names.
 * 	[1992/05/07  23:09:10  burati]
 * 
 * Revision 1.1.2.2  1992/04/24  16:12:35  burati
 * 	CR2727 Fix wildcard deletes
 * 	[1992/04/23  19:26:02  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  eif_acct.c V=22 12/16/91
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic account interface routines
**
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)eif_acct.c	22 - 12/16/91";
#endif

#include <stdio.h>
#include <un_strng.h>

#include <macros.h>

#include <ek_types.h>
#include <ek_error.h>
#include <tty_io.h>
#include <tty_acct.h>
#include <tty_util.h>
#include <eif.h>
#include <eif_acct.h>
#include <eif_def.h>
#include <eif_util.h>

extern boolean32 opt_local_rgy;

/* eif_gen_acct_passwd
 *
 * Have the edit kernel generate a random password and put it in the acct form.
 * Returns true if a value was successfully written, false otherwise. Check the
 * the status parameter for a particular cause of failure.
 */
PUBLIC boolean32  eif_gen_acct_passwd (
    ek_op_handle_t      acct_handle,
    sec_passwd_type_t   *passwd_type,
    sec_passwd_rec_t    *passwd,
    error_status_t      *stp
)
{
    char            error_buf[256];
    ek_op_id_t      cur_op;

    SET_STATUS(stp, error_status_ok);
    
    cur_op = ek_get_op_id(acct_handle);
   
    switch (cur_op) {
        case ekn_acct_add:
        case ekn_acct_change: 
            *stp = ek_acct_gen_passwd(acct_handle, passwd_type, passwd);
            break;

        case ekn_acct_delete:
        case ekn_acct_get:
        default:
            SET_STATUS(stp, ek_bad_field);
            EIF_FATAL("Internal error in eif_gen_acct_passwd", stp);
            break;
    }

    return GOOD_STATUS(stp);
} 


/* eif_get_pwd_mgmt_info
 *
 * Have the edit kernel retrieve the password management policy information
 * for an account and put it in the acct form. Check the status parameter
 * for a particular cause of failure.
 */
PUBLIC boolean32 eif_get_pwd_mgmt_info (
    ek_op_handle_t      acct_handle,
    ek_pwd_val_type_t   *pwd_val_type,
    error_status_t      *stp
)
{
    ek_op_id_t          cur_op;

    SET_STATUS(stp, error_status_ok);

    cur_op = ek_get_op_id(acct_handle);

    switch (cur_op) {
        case ekn_acct_add:
        case ekn_acct_change:
            *stp = ek_acct_get_pwd_mgmt_info(acct_handle, pwd_val_type);
            break;

        case ekn_acct_delete:
        case ekn_acct_get:
        default:
            SET_STATUS(stp, ek_bad_field);
            EIF_FATAL("Internal error in eif_get_pwd_mgmt_info", stp);
            break;
    }

    return GOOD_STATUS(stp);
}


/* eif_write_acct_field
 *
 * returns true if a value was successfully written, false otherwise.  Check the
 * the status parameter for a particular cause of failure (if it's okay, then
 * the value was not written because it's the same as what's already there) 
 */
PUBLIC boolean32  eif_write_acct_field
#ifndef __STDC__
    ( acct_handle, field, data, check_default, stp )
    ek_op_handle_t  acct_handle;
    ek_field_id_t   field;
    char            *data;
    boolean32       check_default;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  acct_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
  )
#endif
{
    char            error_buf[256];
    ek_op_id_t      cur_op;
    sec_rgy_name_t  default_val;
    long            default_len;
    char            op_text[8];
    boolean32       data_written = false;   /* assume nothing written */

    SET_STATUS(stp, error_status_ok);
    
    cur_op = ek_get_op_id(acct_handle);
   
        /* never a default for a key value */
    if (check_default && (field != ekn_acct_pname_key) &&
        (field != ekn_acct_gname_key) && (field != ekn_acct_oname_key)) {
	ek_read_field(acct_handle, field, (char *) default_val,
		       sizeof(default_val), &default_len);
	if ( (strlen(data) == default_len) &&
	     (strncmp((char *) default_val, data, default_len) == 0) )
	    return data_written;
    }

    switch (field) {
    case ekn_acct_pname_key:
    case ekn_acct_gname_key:
    case ekn_acct_oname_key:
	*stp =  ek_write_field(acct_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	    
	    switch (cur_op) {

	    case ekn_acct_add:
		strcpy(op_text, "add");
		break;

	    case ekn_acct_change: 
		strcpy(op_text, "change");
		break;

	    case ekn_acct_delete:
		strcpy(op_text, "delete");
		break;

	    case ekn_acct_get:
		strcpy(op_text, "view");
		break;
	    }
	    
	    sprintf(error_buf, "Unable to %s account", op_text);
	    tty_status_post_echo(data, error_buf, *stp);
	} else
	    data_written = true;
	break;

    case ekn_acct_pname: 
    case ekn_acct_gname: 
    case ekn_acct_oname: 
	*stp =  ek_write_field(acct_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	    tty_status_post_echo(data, "Can't change account name to", *stp);
	} else
	    data_written = true;
	break; 

    case ekn_acct_key_parts:
	*stp =  ek_write_field(acct_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	    tty_print_status( "", *stp);
	} else
	    data_written = true;
	break; 

    case ekn_acct_user_misc: 
    case ekn_acct_user_shell: 
    case ekn_acct_user_homedir: 
    case ekn_acct_user_plain_passwd:
    case ekn_acct_user_my_plain_passwd:
    case ekn_acct_admin_exp_date:
    case ekn_acct_admin_good_since_date:
    case ekn_acct_max_ticket_lifetime:
    case ekn_acct_max_renewable_lifetime:
	*stp =  ek_write_field(acct_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	    tty_status_pre_echo(data, "", *stp);
	}else
	    data_written = true;
	break;

    case ekn_acct_admin_acct_server:
    case ekn_acct_admin_acct_client:
    case ekn_acct_admin_acct_valid:
    case ekn_acct_admin_auth_post_dated:
    case ekn_acct_admin_auth_forwardable:
    case ekn_acct_admin_auth_tgt:
    case ekn_acct_admin_auth_renewable:
    case ekn_acct_admin_auth_proxiable:
    case ekn_acct_admin_auth_dup_session_key:
    case ekn_acct_user_passwd_valid:
	*stp = ek_write_field(acct_handle, field, data, strlen(data));
	if (BAD_STATUS(stp)) {
	    tty_print_status("", *stp);
	} else
	    data_written = true;
	break;
	
    default: 
	SET_STATUS(stp, ek_bad_field);
	EIF_FATAL("Internal error in eif_write_acct_field", stp);
    }

    return data_written;
} 


/*
** FUNCTION eif_view_acct  
*/

PUBLIC error_status_t eif_view_acct
#ifndef __STDC__
    ( pkey, gkey, okey, show_full ) 
    char        *pkey;
    char        *gkey;
    char        *okey;
    boolean32   show_full;
#else
  (
    char        *pkey,
    char        *gkey,
    char        *okey,
    boolean32  show_full
  )
#endif
{
    ek_login_name_buf_t   acct_key;
    long int              acct_key_len;
    boolean32             found_one,
                          is_wild;
    char                  error_msg[256];
    error_status_t        op_st, st;
    ek_op_handle_t        op_handle;

    op_handle = A_get_h;
    st = ek_init_op(op_handle);

    if (!eif_write_acct_field(op_handle, ekn_acct_pname_key, pkey, false, &op_st))
	if (BAD_STATUS(&op_st))  return op_st;
    if (!eif_write_acct_field(op_handle, ekn_acct_gname_key, gkey, false, &op_st))
	if (BAD_STATUS(&op_st))  return op_st;
    if (!eif_write_acct_field(op_handle, ekn_acct_oname_key, okey, false, &op_st))
	if (BAD_STATUS(&op_st))  return op_st;

        /* look for the first matching entry */
    op_st = ek_do_op(op_handle);
    found_one = (GOOD_STATUS(&op_st));

    while (GOOD_STATUS(&op_st)) {
	tty_display_acct(op_handle, show_full, &op_st);
	     /* bail out if the display routine failed */
	if (BAD_STATUS(&op_st))   
	    return op_st;
	op_st = ek_do_op(op_handle);
    }

        /* if no entries were found just report the error.  If at least one was
	   found then "no more entries" is ok, but report anything else */
    if (!found_one) {
        if (!opt_local_rgy) {		/* Fix for CHFts14370 */
	    sprintf(error_msg,
		"Cannot retrieve any matching entries for %s %s %s",
		pkey, gkey, okey);
	    tty_status_post_echo("", error_msg, op_st);
	}
	SET_STATUS(&op_st, error_status_ok);
    } else 
    if (! (STATUS_EQUAL(&op_st, ek_misc_object_not_found) ||
		  STATUS_EQUAL(&op_st, ek_misc_no_more_entries)) ) {	
	tty_status_post_echo(pkey, "Cannot retrieve next entry for", op_st );
    } else 
	SET_STATUS(&op_st, error_status_ok);

    return op_st;
}



PUBLIC error_status_t eif_add_acct
#ifndef __STDC__
    (add_handle)
    ek_op_handle_t  add_handle;
#else
  (
    ek_op_handle_t  add_handle
  )
#endif
{
    ek_login_name_buf_t cur_pname, cur_gname, cur_oname;
    long int            cur_pname_len, cur_gname_len, cur_oname_len;
    error_status_t      st;

    /* apply defaults for items not obtained interactively */
    st = eif_apply_add_defaults(add_handle, eif_domain_acct);
    if (BAD_STATUS(&st)) {
        return st;
    }

    st = ek_do_op(add_handle);

    ek_read_field(add_handle, ekn_acct_pname_key, cur_pname,
		   sizeof(cur_pname), &cur_pname_len);
    ek_read_field(add_handle, ekn_acct_gname_key, cur_gname,
		   sizeof(cur_gname), &cur_gname_len);
    ek_read_field(add_handle, ekn_acct_oname_key, cur_oname,
		   sizeof(cur_oname), &cur_oname_len);

    if (BAD_STATUS(&st)) {
	tty_status_post_echo(cur_pname, "Unable to add", st);
    }
    
    return st;
}
 

PUBLIC error_status_t eif_change_one_acct
#ifndef __STDC__
    ( chg_handle ) 
    ek_op_handle_t  chg_handle;
#else
  (
    ek_op_handle_t  chg_handle
  )
#endif
{
    ek_login_name_buf_t cur_pname, cur_gname, cur_oname;
    long int            cur_pname_len, cur_gname_len, cur_oname_len;
    error_status_t      st;

    st = ek_do_op(chg_handle);

    ek_read_field(chg_handle, ekn_acct_pname_key, cur_pname, sizeof(cur_pname),
		   &cur_pname_len);
    ek_read_field(chg_handle, ekn_acct_gname_key, cur_gname, sizeof(cur_gname),
		   &cur_gname_len);
    ek_read_field(chg_handle, ekn_acct_oname_key, cur_oname, sizeof(cur_oname),
		   &cur_oname_len);

    if (BAD_STATUS(&st)) {
        error_status_t  lst;
        tty_print_status("Change was not complete", st);
        lst = ek_get_last_acct_chg_status(chg_handle);
        if (BAD_STATUS(&lst)) {
            if ((STATUS_EQUAL(&lst, sec_rgy_bad_integrity))&&
                (STATUS_EQUAL(&st,  ek_misc_chg_passwd_fail))) {
                tty_print_status(
                  "Your passwd may not have been entered correctly; Error returned was\n",
                  lst);
            } else {
                tty_print_status("Underlying cause was", lst);
            }
        }
    }

    return st;
}


PUBLIC error_status_t eif_change_wild_acct
#ifndef __STDC__
    (pkey, gkey, okey, chg_handle, verify)
    char            *pkey;
    char            *gkey;
    char            *okey; 
    ek_op_handle_t  chg_handle;
    boolean32       verify;
#else
  (
    char            *pkey,
    char            *gkey,
    char            *okey, 
    ek_op_handle_t  chg_handle,
    boolean32       verify
  )
#endif
{
    char                   verify_prompt[256];
    ek_login_name_buf_t    cur_pname, cur_gname, cur_oname;
    long int               cur_pname_len, cur_gname_len, cur_oname_len;
    boolean32              change_this_one = true;
    tty_verify_response_t  response; 
    error_status_t         save_status,  /* save last change failure
					  * status, if any
					  */
                            st;
    st = ek_init_op(A_get_h);

    st = ek_write_field(A_get_h, ekn_acct_pname_key, pkey, strlen(pkey));
    if (BAD_STATUS(&st)) {
	tty_status_post_echo(pkey, "Unable to change accounts matching principal", st);
	return st;
    }
    st = ek_write_field(A_get_h, ekn_acct_gname_key, gkey, strlen(gkey));
    if (BAD_STATUS(&st)) {
	tty_status_post_echo(gkey, "Unable to change accounts matching group", st);
	return st;
    }
    st = ek_write_field(A_get_h, ekn_acct_oname_key, okey, strlen(okey));
    if (BAD_STATUS(&st)) {
	tty_status_post_echo(okey, "Unable to change accounts matching org", st);
	return st;
    }

    st = ek_do_op(A_get_h);
         /* maybe there aren't any accounts to change */
    if ((STATUS_EQUAL(&st, ek_misc_object_not_found)) || 
	(STATUS_EQUAL(&st, ek_misc_no_more_entries))) {
	SET_STATUS(&st, ek_misc_object_not_found);
	tty_status_post_echo(pkey, "Unable to change accounts matching", st);
	return st;
    } else if (BAD_STATUS(&st)) {
        tty_print_status("Unable to search for matching accounts", st);
        return st;
    }
      
    SET_STATUS(&save_status, error_status_ok);

    do {
	ek_read_field(A_get_h, ekn_acct_pname, cur_pname, sizeof(cur_pname),
		       &cur_pname_len);
	ek_read_field(A_get_h, ekn_acct_gname, cur_gname, sizeof(cur_gname),
		       &cur_gname_len);
	ek_read_field(A_get_h, ekn_acct_oname, cur_oname, sizeof(cur_oname),
		       &cur_oname_len);
	    
	if (verify) {
	    sprintf(verify_prompt, "Change account %c%s %s %s%c", DQ, cur_pname,
                    cur_gname, cur_oname, DQ);
	    response = tty_verify(verify_prompt);

	    switch (response) {
	    case tty_verify_quit:
		return st;   /* just bail out */
		break;
	    case tty_verify_no:
		change_this_one = false;
		break;    
	    case tty_verify_go:
		verify = false;   /* don't verify on subsequent iterations */
		change_this_one = true;
		break;
	    case tty_verify_yes:
		change_this_one = true;
		break;
	    }
	}

	if (change_this_one) {

            st = ek_acct_reset_key_field(chg_handle, sec_rgy_domain_person);
	    st = ek_write_field(chg_handle, ekn_acct_pname_key, cur_pname,
				cur_pname_len);
            if (GOOD_STATUS(&st)) {
                st = ek_acct_reset_key_field(chg_handle, sec_rgy_domain_group);
                st = ek_write_field(chg_handle, ekn_acct_gname_key, cur_gname,
                                    cur_gname_len);
            }
            if (GOOD_STATUS(&st)) {
                st = ek_acct_reset_key_field(chg_handle, sec_rgy_domain_org);
                st = ek_write_field(chg_handle, ekn_acct_oname_key, cur_oname,
                                    cur_oname_len);
            }

	    if (BAD_STATUS(&st)) {
		tty_status_post_echo(cur_pname, "Unable to change" , st);
	    } else
		st = eif_change_one_acct(chg_handle);
	}

	if (BAD_STATUS(&st)) {
	    SET_STATUS(&save_status, st);
	} else if (!verify) {   
	    /* echo acct name if change succeeded and wasn't verified */
	    printf("%s %s %s\n", cur_pname, cur_gname, cur_oname);
	}

	st = ek_do_op(A_get_h);   /* get the next acct to change */

    } while (GOOD_STATUS(&st));
    
           /* do loop should have been terminated by "no more entries" */
    if ( (STATUS_EQUAL(&st, ek_misc_no_more_entries)) || 
	 (STATUS_EQUAL(&st, ek_misc_object_not_found)) ) {
	/* set status to last change failure */
	SET_STATUS(&st, save_status); 
    } else {
	tty_print_status("", st);
    }
 
    return st;
}  



PUBLIC error_status_t eif_delete_acct
#ifndef __STDC__
    ( pkey, gkey, okey, del_handle, verify ) 
    char            *pkey;
    char            *gkey;
    char            *okey;
    ek_op_handle_t  del_handle;
    boolean32       verify;
#else
  (
    char            *pkey,
    char            *gkey,
    char            *okey,
    ek_op_handle_t  del_handle,
    boolean32       verify
  )
#endif
{
    char                   prompt[256];
    ek_login_name_buf_t    cur_pname, cur_gname, cur_oname,
                           expanded_key;
    long int               cur_pname_len, cur_gname_len, cur_oname_len;
    boolean32              is_wild;
    tty_y_n_response_t     y_n_ans;
    error_status_t         save_st, lst, st;

    SET_STATUS(&st, error_status_ok);

    st = ek_init_op(A_get_h);

    st = ek_write_field(A_get_h, ekn_acct_pname_key, pkey, strlen(pkey));
    if (BAD_STATUS(&st)) {
	tty_status_post_echo(pkey, eif_acct_cant_delete, st);
	return st;
    }
    st = ek_write_field(A_get_h, ekn_acct_gname_key, gkey, strlen(gkey));
    if (BAD_STATUS(&st)) {
	tty_status_post_echo(gkey, eif_acct_cant_delete, st);
	return st;
    }
    st = ek_write_field(A_get_h, ekn_acct_oname_key, okey, strlen(okey));
    if (BAD_STATUS(&st)) {
	tty_status_post_echo(okey, eif_acct_cant_delete, st);
	return st;
    }
    
    st = ek_do_op(A_get_h);
    /* maybe there aren't any accounts to delete */
    if ((STATUS_EQUAL(&st, ek_misc_object_not_found)) || 
	(STATUS_EQUAL(&st, ek_misc_no_more_entries))) {
	SET_STATUS(&st,  ek_misc_object_not_found);
        sprintf((char *)expanded_key, "'%s' '%s' '%s'", pkey, gkey, okey);
	tty_status_post_echo(expanded_key, eif_acct_cant_delete, st);
	return st;
    }
    
    SET_STATUS(&save_st, error_status_ok);
    
    do {
	ek_read_field(A_get_h, ekn_acct_pname, cur_pname, 
		       sizeof(cur_pname),  &cur_pname_len);
	ek_read_field(A_get_h, ekn_acct_gname, cur_gname, 
		       sizeof(cur_gname),  &cur_gname_len);
	ek_read_field(A_get_h, ekn_acct_oname, cur_oname, 
		       sizeof(cur_oname),  &cur_oname_len);
	
	if (verify) {
	    sprintf(prompt, "Please confirm delete of acct %c%s %s %s%c",
		    DQ, cur_pname, cur_gname, cur_oname, DQ);
	    y_n_ans = tty_ask_y_n(prompt, tty_response_no);
	} else {
	    y_n_ans = tty_response_yes;
	}

	lst = ek_do_op(A_get_h);
	
	if (y_n_ans == tty_response_yes) {
            st = ek_init_op(del_handle);
            if (GOOD_STATUS(&st))
                st = ek_write_field(del_handle, ekn_acct_pname_key, cur_pname, 
                                     cur_pname_len);
            if (GOOD_STATUS(&st))
                st = ek_write_field(del_handle, ekn_acct_gname_key, cur_gname, 
                                     cur_gname_len);
            if (GOOD_STATUS(&st))
                st = ek_write_field(del_handle, ekn_acct_oname_key, cur_oname, 
                                     cur_oname_len);
            if (GOOD_STATUS(&st))
                st = ek_do_op(del_handle);
	    if (BAD_STATUS(&st)) {
		tty_status_post_echo(cur_pname, eif_acct_cant_delete, st);
                return st;
	    }
	}
	if (GOOD_STATUS(&st)) {
	  SET_STATUS(&st, lst);
	}
    } while (GOOD_STATUS(&st));
    
	/* do loop should been terminated by "no more entries" */
    if ((STATUS_EQUAL(&st, ek_misc_no_more_entries)) || 
	(STATUS_EQUAL(&st, ek_misc_object_not_found)))
	SET_STATUS(&st, save_st);
    else
	tty_print_status("", st);
 
    return st;
}

