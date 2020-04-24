/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_def.c,v $
 * Revision 1.1.8.2  1996/03/11  13:26:37  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:37  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:58:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:35  root]
 * 
 * Revision 1.1.6.2  1994/09/09  19:11:49  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:05:29  mob]
 * 
 * Revision 1.1.6.1  1994/03/02  17:54:09  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:07:20  hanfei]
 * 
 * Revision 1.1.4.3  1992/12/29  12:38:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:58  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/29  00:50:19  burati
 * 	CR3305 Enhancement to allow group aliases
 * 	[1992/11/28  22:24:10  burati]
 * 
 * Revision 1.1.2.2  1992/05/08  18:19:13  burati
 * 	Use correct defaults, so acct_valid defaults to true
 * 	[1992/05/07  23:09:35  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  eif_def.c V=19 11/11/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit general interface default handling routines 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)eif_def.c	19 - 11/11/91";
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <un_strng.h>

#include <macros.h>
#include <sec_def_flags.h>

#include <ek_util.h>
#include <ek_types.h>
#include <ek_error.h>

#include <tty_io.h>
#include <eif.h>
#include <eif_util.h>
#include <eif_def.h>

/* declaration of storage for user configurable default values */
PRIVATE char  def_p_no_user_mod[ek_lim_max_flag_len + 1]        = ek_flag_off;
PRIVATE char  def_p_is_an_alias[ek_lim_max_flag_len + 1]        = ek_flag_off;
PRIVATE char  def_g_projlist_ok[ek_lim_max_flag_len + 1]        = ek_flag_on;
PRIVATE char  def_go_passwd[ek_lim_max_plain_passwd_len + 1]    = ek_passwd_none;
PRIVATE char  def_pgo_fullname[ek_lim_max_fullname_len + 1]     = "";
PRIVATE char  def_pgo_quota[ek_lim_max_int32_len + 1]           = "unlimited";
PRIVATE char  def_pgo_uid[ek_lim_max_int32_len + 1]             = "auto assign";
PRIVATE char  def_acct_passwd[ek_lim_max_plain_passwd_len + 1]  = "-apollo-";
PRIVATE char  def_acct_misc[ek_lim_max_pname_len + 1]           = "";
PRIVATE char  def_acct_abbrev[ek_lim_max_key_parts_len +1]      =
    ek_acct_key_person;
PRIVATE char  def_acct_homedir[ek_lim_max_pname_len + 1]        = "/";
PRIVATE char  def_acct_shell[ek_lim_max_pname_len + 1]          = "";
PRIVATE char  def_acct_exp_date[ek_lim_max_date_len + 1]        = ek_null_date;
PRIVATE char  def_acct_passwd_valid[ek_lim_max_flag_len + 1]    = ek_flag_on;
PRIVATE char  def_acct_good_since_date[ek_lim_max_date_len + 1] = ek_null_date;
PRIVATE char  def_acct_max_ticket_lifetime[ek_lim_max_date_len + 1]     = "8";
PRIVATE char  def_acct_max_renewable_lifetime[ek_lim_max_date_len + 1]  = "168";

    /* Set via SEC_DEF_FLGS_ACCT_ADMIN flags from sec_def_flags.h */
PRIVATE char  def_acct_server[ek_lim_max_flag_len + 1];
PRIVATE char  def_acct_client[ek_lim_max_flag_len + 1];
PRIVATE char  def_acct_valid[ek_lim_max_flag_len + 1];

    /* Set via SEC_DEF_FLGS_ACCT_AUTH flags from sec_def_flags.h */
PRIVATE char  def_acct_post_dated[ek_lim_max_flag_len + 1];
PRIVATE char  def_acct_forwardable[ek_lim_max_flag_len + 1];
PRIVATE char  def_acct_tgt[ek_lim_max_flag_len + 1];
PRIVATE char  def_acct_renewable[ek_lim_max_flag_len + 1];
PRIVATE char  def_acct_proxiable[ek_lim_max_flag_len + 1];
PRIVATE char  def_acct_dup_session_key[ek_lim_max_flag_len + 1];

PRIVATE  char  *Eif_defaults[] = {
    def_p_no_user_mod,
    def_p_is_an_alias,
    def_g_projlist_ok,
    def_go_passwd,
    def_pgo_fullname,
    def_pgo_quota,
    def_pgo_uid,
    def_acct_passwd,
    def_acct_misc,
    def_acct_abbrev,
    def_acct_homedir,
    def_acct_shell,
    def_acct_exp_date,
    def_acct_passwd_valid,
    def_acct_server,
    def_acct_client,
    def_acct_valid,
    def_acct_good_since_date,
    def_acct_max_ticket_lifetime,
    def_acct_max_renewable_lifetime,
    def_acct_post_dated,
    def_acct_forwardable,
    def_acct_tgt,
    def_acct_renewable,
    def_acct_proxiable,
    def_acct_dup_session_key
    };

PUBLIC eif_default_id_t  eif_def_p_no_user_mod          = 0;
PUBLIC eif_default_id_t  eif_def_p_is_an_alias          = 1;
PUBLIC eif_default_id_t  eif_def_g_projlist_ok          = 2;
PUBLIC eif_default_id_t  eif_def_go_passwd              = 3;
PUBLIC eif_default_id_t  eif_def_pgo_fullname           = 4;
PUBLIC eif_default_id_t  eif_def_pgo_quota              = 5;
PUBLIC eif_default_id_t  eif_def_pgo_uid                = 6;
PUBLIC eif_default_id_t  eif_def_acct_passwd            = 7;
PUBLIC eif_default_id_t  eif_def_acct_misc              = 8;
PUBLIC eif_default_id_t  eif_def_acct_abbrev            = 9;
PUBLIC eif_default_id_t  eif_def_acct_homedir           = 10;
PUBLIC eif_default_id_t  eif_def_acct_shell             = 11;
PUBLIC eif_default_id_t  eif_def_acct_exp_date          = 12;
PUBLIC eif_default_id_t  eif_def_acct_passwd_valid      = 13;
PUBLIC eif_default_id_t  eif_def_acct_server            = 14;
PUBLIC eif_default_id_t  eif_def_acct_client            = 15;
PUBLIC eif_default_id_t  eif_def_acct_valid             = 16;
PUBLIC eif_default_id_t  eif_def_good_since_date        = 17;
PUBLIC eif_default_id_t  eif_def_max_ticket_lifetime    = 18;
PUBLIC eif_default_id_t  eif_def_max_renewable_lifetime = 19;
PUBLIC eif_default_id_t  eif_def_acct_post_dated        = 20;
PUBLIC eif_default_id_t  eif_def_acct_forwardable       = 21;
PUBLIC eif_default_id_t  eif_def_acct_tgt               = 22;
PUBLIC eif_default_id_t  eif_def_acct_renewable         = 23;
PUBLIC eif_default_id_t  eif_def_acct_proxiable         = 24;
PUBLIC eif_default_id_t  eif_def_acct_dup_session_key   = 25;

PRIVATE sec_rgy_name_t  Eif_default_buff;   /* large enough for the longest
					     * legal kernel value of any type */

PUBLIC char *eif_get_default
#ifndef __STDC__
    ( default_id ) 
    eif_default_id_t  default_id;
#else
  (
    eif_default_id_t  default_id
  )
#endif
{
    char    *default_ptr = 0;
    time_t  tloc;
    
    /* Special case the good_since_date default, since it must be generated */
    if ( default_id == eif_def_good_since_date ) {
        time(&tloc);
        strftime((char *)Eif_default_buff, 17, "%Y/%m/%d.%H:%M", localtime(&tloc));
        Eif_default_buff[16] = '\0';     /* Make sure the date is null terminated */
        default_ptr = (char *) &(Eif_default_buff[0]);
    }
    else if ( default_id < (sizeof(Eif_defaults) / sizeof(Eif_defaults[0])) ) {
        default_ptr = (char *) Eif_default_buff;
        strcpy(default_ptr, Eif_defaults[default_id]);
    } 

    return default_ptr;
}


PUBLIC boolean32 eif_set_default
#ifndef __STDC__
    ( default_id, default_val, stp ) 
    eif_default_id_t  default_id;    /* [in]  - id of default being set */
    char              *default_val;  /* [in]  - value of default being set */
    error_status_t    *stp;          /* [out] - status */
#else
  (
    eif_default_id_t  default_id,    /* [in]  - id of default being set */
    char              *default_val,  /* [in]  - value of default being set */
    error_status_t    *stp
  )
#endif
{				  
    sec_rgy_name_t    buff;
    char              *def_ptr = default_val;
    boolean32         ret_val,
                      dummy_bool;

    if ( !(default_id < (sizeof(Eif_defaults) / sizeof(Eif_defaults[0]))) )
	return false;
    else 
	ret_val = true;
	
    SET_STATUS(stp, error_status_ok);

        /* Don't bother checking the default if it's the same as what's already
         * there.
	 */
    if (strcmp(default_val, Eif_defaults[default_id]) == 0)
	return ret_val;
    
    if ((default_id == eif_def_p_no_user_mod) ||
	       (default_id == eif_def_p_is_an_alias) ||
	       (default_id == eif_def_g_projlist_ok) ||	
	       (default_id == eif_def_acct_passwd_valid) ||
	       (default_id == eif_def_acct_server) ||
	       (default_id == eif_def_acct_client) ||
	       (default_id == eif_def_acct_valid)) {
	*stp = eku_chk_flag(default_val, strlen(default_val), &dummy_bool);
    } else if (default_id == eif_def_go_passwd) {
	*stp = ek_init_op(G_change_h);
    } else if (default_id == eif_def_pgo_fullname) {
	*stp = ek_init_op(G_change_h);
	*stp = ek_write_field(G_change_h, ekn_pgo_fullname, default_val,
			       strlen(default_val));
    } else if (default_id == eif_def_pgo_quota) {
	*stp = ek_init_op(G_change_h);
	*stp = ek_write_field(G_change_h, ekn_pgo_quota, default_val,
			       strlen(default_val));
    } else if (default_id ==  eif_def_acct_passwd) {
	*stp = ek_init_op(A_change_h);
	*stp = ek_write_field(A_change_h, ekn_acct_user_plain_passwd,
			      default_val, strlen(default_val));
    } else  if (default_id ==  eif_def_acct_misc) {
	*stp = ek_init_op(A_change_h);
	*stp = ek_write_field(A_change_h, ekn_acct_user_misc, default_val,
			       strlen(default_val));
#ifdef NOTDEF
    } else if (default_id ==  eif_def_acct_abbrev) {
	*stp = ek_init_op(A_change_h);
	*stp = ek_write_field(A_change_h, ekn_acct_key_parts, default_val, 
			       strlen(default_val));
#endif
    } else if (default_id ==  eif_def_acct_homedir) {
	*stp = ek_init_op(A_change_h);
	*stp = ek_write_field(A_change_h, ekn_acct_user_homedir, default_val, 
			       strlen(default_val));
    } else if (default_id ==  eif_def_acct_shell) {
	*stp = ek_init_op(A_change_h);
	*stp = ek_write_field(A_change_h, ekn_acct_user_shell, default_val,
			       strlen(default_val));
    } else if (default_id ==  eif_def_acct_exp_date) {
	*stp = ek_init_op(A_change_h);
	*stp = ek_write_field(A_change_h, ekn_acct_admin_exp_date, default_val,
			       strlen(default_val));
    } else if (default_id ==  eif_def_good_since_date) {
        *stp = ek_init_op(A_change_h);
        *stp = ek_write_field(A_change_h, ekn_acct_admin_good_since_date,
                                default_val, strlen(default_val));
    } else
	ret_val = false;

    if (ret_val && GOOD_STATUS(stp)) {
	strcpy(Eif_defaults[default_id], def_ptr);
    }

    return ret_val;
}   


/* eif_apply_add_defaults
 * 
 * For each field in the handle that has not had any value written to it, applies
 * the default value.  If the current default is unacceptable an error message
 * is issued and the function returns false, otherwise the defaults are
 * (silently) applied to the handle and the function returns true.
 *
 * There are two kinds of defaults: defaults that can be set by the user 
 * (e.g, domain owners), and defaults that can't (e.g., the default of "off"
 * for the is_an_alias flag is applied by the interface and can't be changed by 
 * the user).  If one of the latter defaults returns a bad status when it's 
 * applied, there's a bug in the works.
 */

PUBLIC error_status_t eif_apply_add_defaults
#ifndef __STDC__
    (add_handle, domain)
    ek_op_handle_t  add_handle;
    eif_domain_t    domain;
#else
  (
    ek_op_handle_t  add_handle,
    eif_domain_t    domain
  )
#endif
{
    char            error_msg[256];
    error_status_t  st;
    char            *def_val;

    SET_STATUS(&st, error_status_ok);

    if (domain == eif_domain_acct) {
	
       
	if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
						  ekn_acct_key_parts)) {
	    def_val = eif_get_default(eif_def_acct_abbrev);
	    st = ek_write_field(add_handle, ekn_acct_key_parts, def_val,
				 strlen(def_val));
	    if (BAD_STATUS(&st)) {
		sprintf(error_msg, "%s %s", eif_bad_default, 
			"acct abbreviation");
		tty_status_pre_echo(def_val, error_msg, st);
		return st; 
	    }
	}

	if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
						  ekn_acct_user_misc)) {
	    def_val = eif_get_default(eif_def_acct_misc);
	    st = ek_write_field(add_handle, ekn_acct_user_misc, def_val,
				 strlen(def_val));
	    if (BAD_STATUS(&st)) {
		sprintf(error_msg, "%s %s", eif_bad_default, "misc info");
		tty_status_pre_echo(def_val, error_msg, st);
		return st; 
	    }
	}

	if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
						  ekn_acct_user_homedir)) {
	    def_val = eif_get_default(eif_def_acct_homedir);
	    st = ek_write_field(add_handle, ekn_acct_user_homedir, def_val,
				 strlen(def_val));
	    if (BAD_STATUS(&st)) {
		sprintf(error_msg, "%s %s", eif_bad_default, "home directory");
		tty_status_pre_echo(def_val, error_msg, st);
		return st; 
	    }
	}

	if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
						  ekn_acct_user_shell)) {
	    def_val = eif_get_default(eif_def_acct_shell);
	    st = ek_write_field(add_handle, ekn_acct_user_shell, def_val,
				 strlen(def_val));
	    if (BAD_STATUS(&st)) {
		sprintf(error_msg, "%s %s", eif_bad_default, "shell");
		tty_status_pre_echo(def_val, error_msg, st);
		return st; 
	    }
	}

 	if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
						  ekn_acct_admin_exp_date)) {
	    def_val = eif_get_default(eif_def_acct_exp_date);
	    st = ek_write_field(add_handle, ekn_acct_admin_exp_date, def_val,
				 strlen(def_val));
	    if (BAD_STATUS(&st)) {
		sprintf(error_msg, "%s %s", eif_bad_default,
			"acct expiration date");
		tty_status_pre_echo(def_val, error_msg, st);
		return st; 
	    }
	}

 	if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
                                            ekn_acct_admin_good_since_date)) {
	    def_val = eif_get_default(eif_def_good_since_date);
	    st = ek_write_field(add_handle, ekn_acct_admin_good_since_date,
                                def_val, strlen(def_val));
	    if (BAD_STATUS(&st)) {
		sprintf(error_msg, "%s %s", eif_bad_default,
			"good since date");
		tty_status_pre_echo(def_val, error_msg, st);
		return st; 
	    }
	}

        if (GOOD_STATUS(&st) && 
                !ek_field_written(add_handle, ekn_acct_admin_acct_server)) {
            def_val = eif_get_default(eif_def_acct_server);
            st = ek_write_field(add_handle, ekn_acct_admin_acct_server,
                                def_val, strlen(def_val));
            if (BAD_STATUS(&st)) {
                sprintf(error_msg, "%s %s", eif_bad_default,
                        "account valid server value");
                tty_status_pre_echo(def_val, error_msg, st);
                return st; 
            }
        }

        if (GOOD_STATUS(&st) &&
                !ek_field_written(add_handle, ekn_acct_admin_acct_client)) {
            def_val = eif_get_default(eif_def_acct_client);
            st = ek_write_field(add_handle, ekn_acct_admin_acct_client,
                                def_val, strlen(def_val));
            if (BAD_STATUS(&st)) {
                sprintf(error_msg, "%s %s", eif_bad_default,
                        "account valid client value");
                tty_status_pre_echo(def_val, error_msg, st);
                return st; 
            }
        }

        if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
                                                  ekn_acct_admin_acct_valid)) {
            def_val = eif_get_default(eif_def_acct_valid);
            st = ek_write_field(add_handle, ekn_acct_admin_acct_valid, def_val,
                                strlen(def_val));
            if (BAD_STATUS(&st)) {
                sprintf(error_msg, "%s %s", eif_bad_default,
                        "account valid value");
                tty_status_pre_echo(def_val, error_msg, st);
                return st; 
            }
        }

        if (GOOD_STATUS(&st) &&
                !ek_field_written(add_handle, ekn_acct_user_passwd_valid)) {
            def_val = eif_get_default(eif_def_acct_passwd_valid);
            st = ek_write_field(add_handle, ekn_acct_user_passwd_valid, def_val,
                                strlen(def_val));
            if (BAD_STATUS(&st)) {
                sprintf(error_msg, "%s %s", eif_bad_default,
                        "passwd valid value");
                tty_status_pre_echo(def_val, error_msg, st);
                return st; 
            }
        }
    } else {
            /* common pgo fields */

        if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
                                                  ekn_pgo_fullname)) {
            def_val = eif_get_default(eif_def_pgo_fullname);
            st = ek_write_field(add_handle, ekn_pgo_fullname, def_val, 
                                 strlen(def_val));

            if (BAD_STATUS(&st)) {
                sprintf(error_msg, "%s %s", eif_bad_default, "fullname");
                tty_status_pre_echo(def_val, error_msg, st);
                return st; 
            }
        }

        switch (domain) {    /* now the pgo domain-specific defaults */

        case eif_domain_person:
            if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
                                                      ekn_pgo_is_an_alias)) {
                def_val = eif_get_default(eif_def_p_is_an_alias);
                st = ek_write_field(add_handle, ekn_pgo_is_an_alias, def_val,
                                     strlen(def_val));
                if (BAD_STATUS(&st)) {
                    sprintf(error_msg,"Unable to use default alias value %c%s%c"
                            , DQ, def_val, DQ);
                    tty_print_status(error_msg, st);
                    return st;
                }
            }
            break;

        case eif_domain_group:
            if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
                                                      ekn_pgo_is_an_alias)) {
                def_val = eif_get_default(eif_def_p_is_an_alias);
                st = ek_write_field(add_handle, ekn_pgo_is_an_alias, def_val,
                                     strlen(def_val));
                if (BAD_STATUS(&st)) {
                    sprintf(error_msg,"Unable to use default alias value %c%s%c"
                            , DQ, def_val, DQ);
                    tty_print_status(error_msg, st);
                    return st;
                }
            }
            if (GOOD_STATUS(&st) && !ek_field_written(add_handle,
                                                        ekn_pgo_projlist_ok)) {
                def_val = eif_get_default(eif_def_g_projlist_ok);
                st = ek_write_field(add_handle, ekn_pgo_projlist_ok, def_val,
                                    strlen(def_val));
                    /* this shouldn't ever fail but we leave the check 
                     * here just in case...
                     */
                if (BAD_STATUS(&st)) {
                    sprintf(error_msg, "Unable to use default projlist value\
                            %c%s%c",
                            DQ, ek_flag_on, DQ);
                    tty_print_status(def_val, st);
                    return st;
                }
            }
        }
    }

    return st;
}


/* eif_init_default_flag
 *
 * Set the char based default storage for a flag, based on it's actual value.
 * Called by eif_init_defaults to initialize the character based defaults
 * for default flags defined as bitsets in sec_def_flags.h
 */
PRIVATE void eif_init_default_flag
#ifndef __STDC__
    (defaultflags, checkflag, default_storage)
    unsigned32  defaultflags;
    unsigned32  checkflag;
    char        *default_storage;
#else
  (
    unsigned32  defaultflags,
    unsigned32  checkflag,
    char        *default_storage
  )
#endif
{
    if (FLAG_SET(defaultflags, checkflag))
        strcpy(default_storage, ek_flag_on);
    else 
        strcpy(default_storage, ek_flag_off);
}


/* eif_init_defaults
 * 
 * Set the acct_auth flags from those defined in sec_def_flags.h.
 * If $HOME/.rgy_editrc exists, attempt to read default values from there,
 * else create the rc file and write the standard default values to it.
 */
PUBLIC error_status_t eif_init_defaults (
#ifdef __STDC__
    void
#endif
)
{
    char    *home, *defaults_path, *def_val;
    char    error_msg[256];
    FILE    *fptr;
    error_status_t  st;
    char            tmp_name[ek_lim_max_pname_len+1];

    CLEAR_STATUS(&st);

    /* Get initial defaults for acct_admin flags from sec_def_flags.h */
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                            sec_rgy_acct_admin_valid,
                            def_acct_valid);
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                            sec_rgy_acct_admin_client,
                            def_acct_client);
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_ADMIN_LOGIN,
                            sec_rgy_acct_admin_server,
                            def_acct_server);

    /* Get initial defaults for acct_auth flags from sec_def_flags.h */
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_AUTH,
                            sec_rgy_acct_auth_post_dated,
                            def_acct_post_dated);
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_AUTH,
                            sec_rgy_acct_auth_forwardable,
                            def_acct_forwardable);
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_AUTH,
                            sec_rgy_acct_auth_tgt,
                            def_acct_tgt);
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_AUTH,
                            sec_rgy_acct_auth_renewable,
                            def_acct_renewable);
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_AUTH,
                            sec_rgy_acct_auth_proxiable,
                            def_acct_proxiable);
    eif_init_default_flag(SEC_DEF_FLGS_ACCT_AUTH,
                            sec_rgy_acct_auth_dup_skey,
                            def_acct_dup_session_key);

    /* Don't do anything if we can't find out where the user's homedir exists
     * or if we can't malloc the space for the defaults-file-path.
     */
    if ( (home = getenv("HOME")) &&
         ((defaults_path = (char *) malloc(
            (strlen(home)+strlen(defaults_file)+2) * sizeof(char))) != 0)
       ) {
    
        if (strcmp(home, "/") == 0)
            sprintf(defaults_path, "/%s", defaults_file);
        else
            sprintf(defaults_path, "%s/%s", home, defaults_file);

        /* If we can open the file for reading then attempt to parse it */
        if ((fptr = fopen(defaults_path, "r")) != NULL) {
            if ( (GOOD_STATUS(&st)) && (!feof(fptr)) ) {
                /* Homedir */
                fgets(tmp_name, ek_lim_max_pname_len, fptr);
                /* fgets keeps newlines, so get rid of it manually */
                if (tmp_name[strlen(tmp_name)-1] == '\n')
                    tmp_name[strlen(tmp_name)-1] = '\0';
                eif_set_default(eif_def_acct_homedir, tmp_name, &st);
            }

            if ( (GOOD_STATUS(&st)) && (!feof(fptr)) ) {
                /* Shell */
                fgets(tmp_name, ek_lim_max_pname_len, fptr);
                /* fgets keeps newlines, so get rid of it manually */
                if (tmp_name[strlen(tmp_name)-1] == '\n')
                    tmp_name[strlen(tmp_name)-1] = '\0';
                eif_set_default(eif_def_acct_shell, tmp_name, &st);
            }

            if ( (GOOD_STATUS(&st)) && (!feof(fptr)) ) {
                /* Passwd valid */
                fgets(tmp_name, ek_lim_max_pname_len, fptr);
                /* fgets keeps newlines, so get rid of it manually */
                if (tmp_name[strlen(tmp_name)-1] == '\n')
                    tmp_name[strlen(tmp_name)-1] = '\0';
                eif_set_default(eif_def_acct_passwd_valid, tmp_name, &st);
            }

            if ( (GOOD_STATUS(&st)) && (!feof(fptr)) ) {
                /* Expiration Date */
                fgets(tmp_name, ek_lim_max_pname_len, fptr);
                /* fgets keeps newlines, so get rid of it manually */
                if (tmp_name[strlen(tmp_name)-1] == '\n')
                    tmp_name[strlen(tmp_name)-1] = '\0';
                eif_set_default(eif_def_acct_exp_date, tmp_name, &st);
            }

            if ( (GOOD_STATUS(&st)) && (!feof(fptr)) ) {
                /* Account valid */
                fgets(tmp_name, ek_lim_max_pname_len, fptr);
                /* fgets keeps newlines, so get rid of it manually */
                if (tmp_name[strlen(tmp_name)-1] == '\n')
                    tmp_name[strlen(tmp_name)-1] = '\0';
                eif_set_default(eif_def_acct_valid, tmp_name, &st);
            }

            fclose(fptr);

            /* An error here should generate a warning, but isn't fatal */
            if (BAD_STATUS(&st)) {
                sprintf(error_msg, "Error parsing defaults file: %c%s%c\n",
                        DQ, defaults_path, DQ);
                tty_print_status(error_msg, st);
                CLEAR_STATUS(&st);
            }
        }
        /* otherwise, create the file and write the standard defaults to it */
        else if ((fptr = fopen(defaults_path, "w")) != NULL) {
	    fputs(eif_get_default(eif_def_acct_homedir), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_shell), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_passwd_valid), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_exp_date), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_valid), fptr);
            fputs("\n", fptr);
            fclose(fptr);
        } else {
            fprintf(stderr, "Unable to create defaults file %s\n", defaults_path);
        }

        free(defaults_path);
    }
    return st;
}


/* eif_save_defaults
 * 
 * (re)write $HOME/.rgy_editrc with the current defaults.
 * Called by tty_do_defaults.
 */
PUBLIC error_status_t eif_save_defaults (
#ifdef __STDC__
    void
#endif
)
{
    char    *home, *defaults_path;
    FILE    *fptr;
    error_status_t  st;
    char            tmp_name[ek_lim_max_pname_len+1];

    CLEAR_STATUS(&st);
    /* Don't do anything if we can't find out where the user's homedir exists
     * or if we can't malloc the space for the defaults-file-path.
     */
    if ( (home = getenv("HOME")) &&
         ((defaults_path = (char *) malloc(
            (strlen(home)+strlen(defaults_file)+2) * sizeof(char))) != 0)
       ) {
    
        if (strcmp(home, "/") == 0)
            sprintf(defaults_path, "/%s", defaults_file);
        else
            sprintf(defaults_path, "%s/%s", home, defaults_file);

        /* Create the file and write the defaults to it */
        if ((fptr = fopen(defaults_path, "w")) != NULL) {
	    fputs(eif_get_default(eif_def_acct_homedir), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_shell), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_passwd_valid), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_exp_date), fptr);
            fputs("\n", fptr);
	    fputs(eif_get_default(eif_def_acct_valid), fptr);
            fputs("\n", fptr);
            fclose(fptr);
        } else {
            fprintf(stderr, "Unable to create defaults file %s\n", defaults_path);
        }
        free(defaults_path);
    }
    return st;
}

