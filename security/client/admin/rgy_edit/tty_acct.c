/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_acct.c,v $
 * Revision 1.1.8.2  1996/03/11  13:27:07  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:02  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:00:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/09  16:55 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_chfts13853/1  1994/12/09  16:43 UTC  mullan_s
 * 	[chfts13853]: Do not call password management APIs on a wildcard change account
 * 	because password changes are not allowed.
 * 	[1995/12/08  16:30:25  root]
 * 
 * Revision 1.1.6.1  1994/07/15  14:58:25  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:12  mdf]
 * 
 * Revision 1.1.4.3  1992/12/29  12:41:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:12  zeliff]
 * 
 * Revision 1.1.4.2  1992/10/08  19:42:45  burati
 * 	CR4712 Make sure using same exp date doesn't keep it from updating acct
 * 	[1992/09/25  15:46:13  burati]
 * 
 * Revision 1.1.2.6  1992/06/02  15:35:14  burati
 * 	CR 3959 Finish -gsd -mcr -mcl -ena and -dis options
 * 	[1992/06/02  02:57:30  burati]
 * 
 * Revision 1.1.2.5  1992/05/27  21:25:20  burati
 * 	Fix typo in printf "Account MAY NOT a server principal"
 * 	[1992/05/27  21:12:15  burati]
 * 
 * Revision 1.1.2.4  1992/05/08  18:20:05  burati
 * 	Use sec_rgy_name_t instead of ek_pname_buf_t for names
 * 	[1992/05/07  23:11:25  burati]
 * 
 * Revision 1.1.2.3  1992/04/24  16:13:00  burati
 * 	CR625 Print abort message if ADD command is missing key input
 * 	[1992/04/23  19:30:11  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  20:26:45  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:57:04  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty_acct.c V=26 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface account domain processing
**
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_acct.c	26 - 03/02/92";
#endif

#include <stdio.h>
#include <un_strng.h>

#include <macros.h>
#include <rgymacro.h>
#include <com_err.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>   /* for krb5_read_password */

#include <tty_base.h>
#include <tty_conf.h>
#include <tty_util.h>
#include <tty_io.h>
#include <tty_err.h>
#include <tty_acct.h>

#include <eif.h>
#include <eif_def.h>
#include <eif_acct.h>
#include <eif_util.h>

#include <ek_db.h>
#include <ek_error.h>
#include <args.h>


/* tty_get_acct_args
 *
 * Interactively inputs values for the non-key fields of an account add/change
 * handle.  Returns true if the data in one or more fields have been
 * successfully changed, false if no data is changed.  The function returns on
 * the first failure to write a new data value to the acct handle.  The stp
 * parameter  is set to reflect the failure.
 *
 */
PRIVATE boolean32 tty_get_acct_args (
    ek_op_handle_t   op_handle,
    boolean32        show_defaults,
    boolean32        not_random_passwd,
    char             **prompts,
    ek_pwd_val_type_t pwd_val_type,
    error_status_t   *stp
)
{
    boolean32        one_field_written = false, /* assume no new data entered */
                     input,
                     is_wild,
                     use_def_pwd,
                     io_map;
    sec_rgy_name_t   io_buff,
                     def_name, gname, oname;
    int              size = sizeof(io_buff);
    krb5_error_code  retval;
    long             def_name_len, gname_len, oname_len;
    ek_op_id_t       op_id;
    char             *add_def;

    op_id = ek_get_op_id(op_handle);

    /* If this is a change operation and show_defaults == TRUE (!is_wild), then
     * prompt for a new acct name using the acct id in the key field as the
     * default.
     */
    if ((op_id == ekn_acct_change) && (show_defaults)) {
        ek_read_field(op_handle, ekn_acct_pname_key, (char *) def_name,
                       sizeof(def_name), &def_name_len);
        ek_read_field(op_handle, ekn_acct_gname_key, (char *) gname,
                        sizeof(gname), &gname_len);
        ek_read_field(op_handle, ekn_acct_oname_key, (char *) oname,
                        sizeof(oname), &oname_len);
        *stp = tty_get_field_arg(op_handle, ekn_acct_pname, &tty_null_conv_table,
                                 show_defaults,
				 (char *) def_name, (char *) io_buff,
                                 sizeof(io_buff),
                                 prompts[tty_acct_new_pname_prompt],
                                 &input);
        if (GOOD_STATUS(stp) && input) {
            (void) eif_write_acct_field(op_handle, ekn_acct_pname,
                                        (char *) io_buff, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
        }
        if (BAD_STATUS(stp))
            return one_field_written;
        *stp = tty_get_field_arg(op_handle, ekn_acct_gname,
				 &tty_null_conv_table,
                                 show_defaults, (char *)gname, (char *)io_buff,
                                 sizeof(io_buff),
                                 prompts[tty_acct_new_gname_prompt],
                                 &input);
        if (GOOD_STATUS(stp) && input) {
                    (void) eif_write_acct_field(op_handle, ekn_acct_gname,
                                                (char *) io_buff, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
        }
        if (BAD_STATUS(stp))
            return one_field_written;
        *stp = tty_get_field_arg(op_handle, ekn_acct_oname,
				 &tty_null_conv_table,
                                 show_defaults, (char *)oname, (char *)io_buff,
                                 sizeof(io_buff),
                                 prompts[tty_acct_new_oname_prompt],
                                 &input);
        if (GOOD_STATUS(stp) && input) {
                    (void) eif_write_acct_field(op_handle, ekn_acct_oname,
                                                (char *) io_buff, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
        }
        if (BAD_STATUS(stp))
            return one_field_written;
    }

        /* Password info.
         * Only prompt if account add, or change && wants-to-change-it AND
         * not a wildcard change.
         */
    if ((not_random_passwd) && ((op_id == ekn_acct_add) ||
        ((op_id == ekn_acct_change) &&
         (show_defaults) &&
         (tty_ask_y_n("Change password?",tty_response_no)==tty_response_yes)))){

        if (pwd_val_type == generation_required) {
            tty_get_gen_pwd(op_handle, prompts[tty_acct_passwd_prompt], 
                            &io_buff, stp);
            if (BAD_STATUS(stp)) {
                tty_print_status("Password generation error", *stp);
                return one_field_written;
            }
        } else {
            /* Use the krb5 utility routine for obtaining passwords */
            if (retval = krb5_read_password(prompts[tty_acct_passwd_prompt],
                                            prompts[tty_acct_passwd2_prompt],
                                            (char *) io_buff,
                                            &size)) {
                com_err("keytab", retval, "Error in input password.  Add failed.");
                SET_STATUS(stp, sec_rgy_passwd_invalid);
                return one_field_written;
            }

            /* check to see if user wants generated password */
            if (pwd_val_type == user_can_select) {
                if (!strcmp((char *)io_buff, "*")) {
                    tty_get_gen_pwd(op_handle, prompts[tty_acct_passwd_prompt],
                                    &io_buff, stp);
                    if (BAD_STATUS(stp)) {
                        tty_print_status("Password generation error", *stp);
                        return one_field_written;
                    }
                }
            }
        }

        (void) eif_write_acct_field(op_handle, ekn_acct_user_plain_passwd,
                                    (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
        else
            return one_field_written;

        /* Use the krb5 utility routine for obtaining passwords */
        size = sizeof(io_buff);
        if (retval = krb5_read_password(prompts[tty_acct_my_passwd_prompt],
                                        NULL, (char *) io_buff, &size)) {
            com_err("keytab", retval, "Error in input password.  Add failed.");
            SET_STATUS(stp, sec_rgy_passwd_invalid);
            return one_field_written;
        } else {
            (void) eif_write_acct_field(op_handle,
					ekn_acct_user_my_plain_passwd,
                                        (char *) io_buff, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
            else
                return one_field_written;
        }
        /* But if there was a random password, then need caller's passwd */
    } else if (! not_random_passwd) {
        /* Use the krb5 utility routine for obtaining passwords */
        size = sizeof(io_buff);
        if (retval = krb5_read_password(prompts[tty_acct_my_passwd_prompt],
                                        NULL, (char *) io_buff, &size)) {
            com_err("keytab", retval, "Error in input password.  Add failed.");
            SET_STATUS(stp, sec_rgy_passwd_invalid);
            return one_field_written;
        } else {
            (void) eif_write_acct_field(op_handle,
					ekn_acct_user_my_plain_passwd,
                                        (char *) io_buff, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
            else
                return one_field_written;
        }
   }

       /* misc info */
    add_def = eif_get_default(eif_def_acct_misc);
    *stp = tty_get_field_arg(op_handle, ekn_acct_user_misc,
                             &tty_null_conv_table, show_defaults, add_def,
                             (char *) io_buff, sizeof(io_buff),
                             prompts[tty_acct_misc_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_user_misc,
                                    (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* homedir */
    add_def = eif_get_default(eif_def_acct_homedir);
    *stp = tty_get_field_arg(op_handle, ekn_acct_user_homedir,
                             &tty_null_conv_table, show_defaults,
                             (char *) add_def, (char *) io_buff,
			     sizeof(io_buff),
                             prompts[tty_acct_homedir_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_user_homedir,
                                     (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* shell */
    add_def = eif_get_default(eif_def_acct_shell);
    *stp = tty_get_field_arg(op_handle, ekn_acct_user_shell,
                             &tty_null_conv_table, show_defaults,
                             (char *) add_def, (char *) io_buff,
			     sizeof(io_buff),
                             prompts[tty_acct_shell_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_user_shell,
                                     (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* passwd valid flag */
    add_def = eif_get_default(eif_def_acct_passwd_valid);
    *stp = tty_get_field_arg(op_handle, ekn_acct_user_passwd_valid,
                             &tty_flag_conv_table, show_defaults,
                             (char *) add_def, (char *) io_buff,
			     sizeof(io_buff),
                             prompts[tty_acct_passwd_valid_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_user_passwd_valid,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* expiration date */
    add_def = eif_get_default(eif_def_acct_exp_date);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_exp_date,
                             &tty_date_conv_table, show_defaults, add_def,
                             (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_exp_date_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_exp_date,
                                     (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* account server flag */
    add_def = eif_get_default(eif_def_acct_server);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_acct_server,
                             &tty_flag_conv_table, show_defaults,
                             (char *) add_def, (char *) io_buff,
			     sizeof(io_buff),
                             prompts[tty_acct_server_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_acct_server,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* account client flag */
    add_def = eif_get_default(eif_def_acct_client);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_acct_client,
                             &tty_flag_conv_table, show_defaults,
                             (char *)add_def, (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_client_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_acct_client,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* account valid flag */
    add_def = eif_get_default(eif_def_acct_valid);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_acct_valid,
                             &tty_flag_conv_table, show_defaults,
                             (char *)add_def, (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_valid_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_acct_valid,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* post-dated certificates */
    add_def = eif_get_default(eif_def_acct_post_dated);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_auth_post_dated,
                             &tty_flag_conv_table, show_defaults,
                             (char *)add_def, (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_post_dated_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_auth_post_dated,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* forwardable certificates */
    add_def = eif_get_default(eif_def_acct_forwardable);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_auth_forwardable,
                             &tty_flag_conv_table, show_defaults,
                             (char *)add_def, (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_forwardable_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_auth_forwardable,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* tgt certificates */
    add_def = eif_get_default(eif_def_acct_tgt);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_auth_tgt,
                             &tty_flag_conv_table, show_defaults,
                             (char *)add_def, (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_tgt_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_auth_tgt,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* renewable certificates */
    add_def = eif_get_default(eif_def_acct_renewable);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_auth_renewable,
                             &tty_flag_conv_table, show_defaults, add_def,
                             (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_renewable_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_auth_renewable,
                                    (char *)io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* proxiable certificates */
    add_def = eif_get_default(eif_def_acct_proxiable);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_auth_proxiable,
                             &tty_flag_conv_table, show_defaults, add_def,
                             (char *) io_buff, sizeof(io_buff),
                             prompts[tty_acct_proxiable_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_auth_proxiable,
                                    (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* dup_session_key's */
    add_def = eif_get_default(eif_def_acct_dup_session_key);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_auth_dup_session_key,
                             &tty_flag_conv_table, show_defaults, add_def,
                             (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_dup_session_key], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle,
				    ekn_acct_admin_auth_dup_session_key,
                                    (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* good since date (TGTs issued before this date will not be honored)*/
    add_def = eif_get_default(eif_def_good_since_date);
    *stp = tty_get_field_arg(op_handle, ekn_acct_admin_good_since_date,
                             &tty_date_conv_table, show_defaults, add_def,
                             (char *)io_buff, sizeof(io_buff),
                             prompts[tty_acct_good_since_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_acct_field(op_handle, ekn_acct_admin_good_since_date,
                                    (char *) io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

        /* Auth policy data (optional) */
    if (tty_ask_y_n(
           "Create/Change auth policy for this acct",
           tty_response_no) == tty_response_yes) {

        add_def = eif_get_default(eif_def_max_ticket_lifetime);
        *stp = tty_get_field_arg(op_handle, ekn_acct_max_ticket_lifetime,
                        &tty_lifespan_conv_table, show_defaults, add_def,
                        (char *) io_buff, sizeof(io_buff),
                        Auth_policy_prompts[tty_auth_policy_max_ticket_lifetime],
                        &input);
        if (GOOD_STATUS(stp) && input) {
            (void) eif_write_acct_field(op_handle,ekn_acct_max_ticket_lifetime,
                                        (char *) io_buff, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
        }
        if (BAD_STATUS(stp))
            return one_field_written;

        add_def = eif_get_default(eif_def_max_renewable_lifetime);
        *stp = tty_get_field_arg(op_handle, ekn_acct_max_renewable_lifetime,
                        &tty_lifespan_conv_table, show_defaults, add_def,
                        (char *) io_buff, sizeof(io_buff),
                        Auth_policy_prompts[tty_auth_policy_max_renewable_lifetime],
                        &input);
        if (GOOD_STATUS(stp) && input) {
            (void) eif_write_acct_field(op_handle,
					ekn_acct_max_renewable_lifetime,
                                        (char *)io_buff, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
        }
        if (BAD_STATUS(stp))
            return one_field_written;
    }

    return one_field_written;
}

/*
 * retrieve a generated password from the password
 * management server and prompt user to enter
 * the returned password.
 */
PRIVATE void tty_get_gen_pwd (
    ek_op_handle_t     op_handle,
    char               *prompt,
    sec_rgy_name_t     *new_pwd,
    error_status_t     *stp
)
{
    sec_passwd_type_t  gen_pwd_type;
    int                size = sizeof(sec_rgy_name_t);
    sec_passwd_rec_t   gen_pwd;
    krb5_error_code    retval;

    gen_pwd_type = sec_passwd_plain;
    eif_gen_acct_passwd(op_handle, &gen_pwd_type, &gen_pwd, stp);
    if (BAD_STATUS(stp))
        return;

    printf("Generated password is: %s, please enter as new password.\n",
            PLAIN_PASSWD(&gen_pwd));

    /* Use the krb5 utility routine for obtaining passwords */
    if (retval = krb5_read_password(prompt,
                                    NULL,
                                    (char *) new_pwd,
                                    &size)) {
        com_err("keytab", retval, "Error in input password.  Add failed.");
        SET_STATUS(stp, sec_rgy_passwd_invalid);
        destroy_passwd(&gen_pwd);
        return;
    }

    /* ensure that user entered generated password */
    if (strcmp((char *)PLAIN_PASSWD(&gen_pwd), (char *)new_pwd)) {
        fprintf(stderr, "Input password does not match generated password.  Add failed.\n");
        SET_STATUS(stp, sec_rgy_passwd_invalid);
        destroy_passwd(&gen_pwd);
        return;
    }
    destroy_passwd(&gen_pwd);
}


PUBLIC void tty_display_acct
#ifndef __STDC__
    ( op_handle, show_full, stp )
    ek_op_handle_t   op_handle;
    boolean32        show_full;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t   op_handle,
    boolean32        show_full,
    error_status_t  *stp
  )
#endif
{
    ek_login_name_buf_t  pname, gname, oname,
                         changer,
                         creator;
    ek_key_parts_buf_t   key_parts;
    char                 display_lname[sizeof(ek_login_name_buf_t) + 2];
    ek_enc_passwd_buf_t  passwd;
    ek_unix_num_buf_t    pid,
                         gid;
    ek_pname_buf_t       misc,
                         homedir,
                         shell;
    ek_flag_buf_t        acct_valid,
                         auth_flag,
                         passwd_valid;
    ek_date_time_buf_t   admin_dtm,
                         passwd_dtm,
                         creation_date,
                         exp_date,
                         good_since_date,
                         lifetime;
    char                 *change_date;
    long int             field_len;
    boolean32            map;

#define                  full_indent "  "

    ek_read_field(op_handle, ekn_acct_pname, pname, sizeof(pname), &field_len);
    ek_read_field(op_handle, ekn_acct_gname, gname, sizeof(gname), &field_len);
    ek_read_field(op_handle, ekn_acct_oname, oname, sizeof(oname), &field_len);
    ek_read_field(op_handle, ekn_acct_key_parts, key_parts,
                   sizeof(key_parts), &field_len);
    tty_make_abbrev_lname(pname, gname, oname, key_parts, display_lname);

    ek_read_field(op_handle, ekn_acct_user_enc_passwd, passwd, sizeof(passwd),
                   &field_len);
    ek_read_field(op_handle, ekn_acct_unix_sid_person, pid, sizeof(pid),
                   &field_len);
    ek_read_field(op_handle, ekn_acct_unix_sid_group, gid, sizeof(gid),
                   &field_len);
    ek_read_field(op_handle, ekn_acct_user_misc, misc , sizeof(misc),
                   &field_len);
    ek_read_field(op_handle, ekn_acct_user_homedir, homedir, sizeof(homedir),
                   &field_len);
    ek_read_field(op_handle, ekn_acct_user_shell, shell, sizeof(shell),
                   &field_len);

        /* print the /etc/passwd-like data */
    printf("%s:%s:%s:%s:%s:%s:%s:\n", display_lname, passwd, pid, gid, misc, homedir,
           shell);

        /* show the rest of the account info if show_full is true */
    if (show_full) {

        ek_read_field(op_handle, ekn_acct_admin_creator, creator,
                       sizeof(creator), &field_len);
        ek_read_field(op_handle, ekn_acct_admin_when_created, creation_date,
                       sizeof(creation_date), &field_len);

            /* print the creation information */
        printf("%screated by: %s  %s\n", full_indent, creator, creation_date);

        ek_read_field(op_handle, ekn_acct_admin_when_changed, admin_dtm,
                       sizeof(admin_dtm), &field_len);
        change_date = admin_dtm;
        ek_read_field(op_handle, ekn_acct_admin_changer, changer,
                        sizeof(changer), &field_len);

            /* print the change information */
        printf("%schanged by: %s  %s\n", full_indent, changer, change_date);

        ek_read_field(op_handle, ekn_acct_user_passwd_valid, passwd_valid,
                              sizeof(passwd_valid), &field_len);
        ek_read_field(op_handle, ekn_acct_user_passwd_dtm, passwd_dtm,
                              sizeof(passwd_dtm), &field_len);

        if (CHECK_FLAG(passwd_valid, strlen(passwd_valid)))
            printf("%spassword is: valid, ", full_indent);
        else
            printf("%spassword is: NOT valid, ", full_indent);

        printf("was last changed: %s\n", passwd_dtm);

        ek_read_field(op_handle, ekn_acct_admin_exp_date, exp_date,
                       sizeof(exp_date), &field_len);

        if (strcmp(exp_date, ek_null_date) == 0)
            sprintf(exp_date, "%s", tty_date_none);
        printf("%sAccount expiration date: %s\n", full_indent, exp_date);

        ek_read_field(op_handle, ekn_acct_admin_acct_server, acct_valid,
                       sizeof(acct_valid), &field_len);
        if (CHECK_FLAG(acct_valid, strlen(acct_valid)))
            printf("%sAccount MAY be a server principal\n", full_indent);
        else
            printf("%sAccount MAY NOT be a server principal\n", full_indent);

        ek_read_field(op_handle, ekn_acct_admin_acct_client, acct_valid,
                       sizeof(acct_valid), &field_len);
        if (CHECK_FLAG(acct_valid, strlen(acct_valid)))
            printf("%sAccount MAY be a client principal\n", full_indent);
        else
            printf("%sAccount MAY NOT be a client principal\n", full_indent);

        ek_read_field(op_handle, ekn_acct_admin_acct_valid, acct_valid,
                       sizeof(acct_valid), &field_len);
        if (CHECK_FLAG(acct_valid, strlen(acct_valid)))
            printf("%sAccount is: valid\n", full_indent);
        else
            printf("%sAccount is: NOT valid\n", full_indent);

        /* Acct admin auth flags */
        ek_read_field(op_handle, ekn_acct_admin_auth_post_dated, auth_flag,
                       sizeof(auth_flag), &field_len);
        if (CHECK_FLAG(auth_flag, strlen(auth_flag)))
            printf("%sAccount CAN get post-dated certificates\n", full_indent);
        else
            printf("%sAccount CAN NOT get post-dated certificates\n", full_indent);
        ek_read_field(op_handle, ekn_acct_admin_auth_forwardable, auth_flag,
                       sizeof(auth_flag), &field_len);
        if (CHECK_FLAG(auth_flag, strlen(auth_flag)))
            printf("%sAccount CAN get forwardable certificates\n", full_indent);
        else
            printf("%sAccount CAN NOT get forwardable certificates\n", full_indent);
        ek_read_field(op_handle, ekn_acct_admin_auth_tgt, auth_flag,
                       sizeof(auth_flag), &field_len);
        if (CHECK_FLAG(auth_flag, strlen(auth_flag)))
            printf("%sCertificates to this service account MAY be issued via TGT authentication\n", full_indent);
        else
            printf("%sCertificates to this service account MAY NOT be issued via TGT authentication\n", full_indent);
        ek_read_field(op_handle, ekn_acct_admin_auth_renewable, auth_flag,
                       sizeof(auth_flag), &field_len);
        if (CHECK_FLAG(auth_flag, strlen(auth_flag)))
            printf("%sAccount CAN get renewable certificates\n", full_indent);
        else
            printf("%sAccount CAN NOT get renewable certificates\n", full_indent);
        ek_read_field(op_handle, ekn_acct_admin_auth_proxiable, auth_flag,
                       sizeof(auth_flag), &field_len);
        if (CHECK_FLAG(auth_flag, strlen(auth_flag)))
            printf("%sAccount CAN get proxiable certificates\n", full_indent);
        else
            printf("%sAccount CAN NOT get proxiable certificates\n", full_indent);
        ek_read_field(op_handle, ekn_acct_admin_auth_dup_session_key, auth_flag,
                       sizeof(auth_flag), &field_len);
        if (CHECK_FLAG(auth_flag, strlen(auth_flag)))
            printf("%sAccount CAN have duplicate session keys\n", full_indent);
        else
            printf("%sAccount CAN NOT have duplicate session keys\n", full_indent);

            /* Good since date (TGTs before this date are invalid) */
        ek_read_field(op_handle,ekn_acct_admin_good_since_date,good_since_date,
                       sizeof(good_since_date), &field_len);
        if (strcmp(good_since_date, ek_null_date) == 0)
            sprintf(good_since_date, "%s", tty_date_none);
        printf("%sGood since date: %s\n", full_indent, good_since_date);

            /* Auth policy data */
        ek_read_field(op_handle, ekn_acct_max_ticket_lifetime, lifetime,
                        sizeof(lifetime), &field_len);
        printf("%sMax certificate lifetime: %s\n", full_indent,
                tty_get_if_io_val(lifetime, &tty_lifespan_conv_table, &map));
        ek_read_field(op_handle, ekn_acct_max_renewable_lifetime, lifetime,
                        sizeof(lifetime), &field_len);
        printf("%sMax renewable lifetime: %s\n", full_indent,
                tty_get_if_io_val(lifetime, &tty_lifespan_conv_table, &map));
    }
	
}


PUBLIC error_status_t tty_do_change_acct
#ifndef __STDC__
    (c)
    command_buf      *c;
#else
  (
    command_buf      *c
  )
#endif
{
    char        *acct_key, *new_pkey, *new_gkey, *new_okey,
                *misc, *homedir, *shell, *passwd, *my_passwd,
                *exp_date, *date, *good_since_date, *mcr, *mcl,
                *server, *client, *post_dated, *forwardable, *tgt,
                *renewable, *dup_session_key, *proxiable;

        /* Key fields specified on command line? */
    boolean32   arg_pname_key, arg_gname_key, arg_oname_key;

        /* Options */
    boolean32   opt_new_pkey, opt_new_gkey, opt_new_okey,
                opt_misc, opt_homedir, opt_shell,                           
                opt_passwd, opt_my_passwd, opt_random_passwd,
                opt_exp_date, opt_acct_valid, opt_acct_not_valid,
                opt_pwd_valid,opt_pwd_not_valid, opt_no_query, 
                opt_mcr, opt_mcl, opt_good_since_date,
                opt_client, opt_svr, opt_pdated, opt_fwd, opt_prox,
                opt_tgt, opt_renew, opt_dup_skey;

    boolean32   do_interactive     = true,
                is_wild, overflow, input, change_one, verify, tmp_update,
                do_update = false;  /* assume the data's bad */

    int                  cur_arg;
    ek_login_name_buf_t  pkey, gkey, okey;
    error_status_t       st;
    ek_op_handle_t       chg_handle = A_change_h;
    sec_passwd_type_t    random_passwd_type;
    ek_pwd_val_type_t    pwd_val_type;
    sec_rgy_name_t       new_pwd;

    arg_pname_key = arg_gname_key = arg_oname_key = false;

        /* Assume no options were specified */
    opt_new_pkey = opt_new_gkey = opt_new_okey = opt_random_passwd =
    opt_misc = opt_homedir = opt_shell = opt_passwd = opt_my_passwd =
    opt_exp_date = opt_acct_valid = opt_acct_not_valid = opt_pwd_valid =
    opt_pwd_not_valid = opt_good_since_date = opt_no_query = opt_fwd =
    opt_mcr = opt_mcl = opt_client = opt_svr = opt_pdated = opt_prox =
    opt_tgt = opt_renew = opt_dup_skey = false;

    random_passwd_type = sec_passwd_des;    /* only random keytype supported */

    /* Initialize keys to wildcard values */
    pkey[0] = gkey[0] = okey[0] = '\0';

        /* Look for options */
    SET_STATUS(&st, error_status_ok);
    cur_arg = 1;
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {

           /* -p,-g,-o,-nq,-rp options don't affect command interactivity */
        if (GOOD_STATUS(&st) &&
            !match_command("-p", c->args[cur_arg], 2) &&
            !match_command("-g", c->args[cur_arg], 2) &&
            !match_command("-o", c->args[cur_arg], 2) &&
            !match_command("-rp", c->args[cur_arg], 3) &&
            !match_command("-nq", c->args[cur_arg], 3) ) {
            do_interactive = false;
        }

        if ((strlen(c->args[cur_arg]) == 2) &&
                match_command("-p", c->args[cur_arg], 2)) {
            if (strlen(pkey) != 0) {
                INVALID_ARG(c->args[cur_arg], &st);
            } else {
                strcpy((char *)pkey, c->args[++cur_arg]);
                arg_pname_key = true;
            }
        } else if (match_command("-g", c->args[cur_arg], 2)) {
            if (strlen(gkey) != 0) {
                INVALID_ARG(c->args[cur_arg], &st);
            } else {
                strcpy((char *)gkey, c->args[++cur_arg]);
                arg_gname_key = true;
            }
        } else if (match_command("-o", c->args[cur_arg], 2)) {
            if (strlen(okey) != 0) {
                INVALID_ARG(c->args[cur_arg], &st);
            } else {
                strcpy((char *)okey, c->args[++cur_arg]);
                arg_oname_key = true;
            }
        } else if (match_command("-pv", c->args[cur_arg], 3)) {
            if (opt_pwd_not_valid) {
                INCOMPATIBLE_OPTS("-pv", "-pnv", &st);
            } else {
                opt_pwd_valid = true;
            }
        } else if (match_command("-pnv", c->args[cur_arg], 4)) {
            if (opt_pwd_valid) {
                INCOMPATIBLE_OPTS("-pv", "-pnv", &st);
            } else {
                opt_pwd_not_valid = true;
            }
        } else if (match_command("-av", c->args[cur_arg], 3)) {
            if (opt_acct_not_valid) {
                INCOMPATIBLE_OPTS("-av", "-anv", &st);
            } else {
                opt_acct_valid = true;
            }
        } else if (match_command("-anv", c->args[cur_arg], 4)) {
            if (opt_acct_valid) {
                INCOMPATIBLE_OPTS("-av", "-anv", &st);
            } else {
                opt_acct_not_valid = true;
            }
        } else if (match_command("-np", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_new_pkey = true;
                new_pkey = c->args[cur_arg];
            }
        } else if (match_command("-ng", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_new_gkey = true;
                new_gkey = c->args[cur_arg];
            }
        } else if (match_command("-no", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_new_okey = true;
                new_okey = c->args[cur_arg];
            }
        } else if (match_command("-pw", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                if (opt_random_passwd) {
                    INCOMPATIBLE_OPTS("-pw", "-rp", &st);
                } else {
                    opt_passwd = true;
                    passwd = c->args[cur_arg];
                }
            }
        } else if (match_command("-rp", c->args[cur_arg], 3)) {
            if (opt_passwd) {
                INCOMPATIBLE_OPTS("-pw", "-rp", &st);
            } else {
                opt_random_passwd = true;
            }
        } else if (match_command("-mp", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_my_passwd = true;
                my_passwd = c->args[cur_arg];
            }
        } else if (match_command("-m", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_misc = true;
                misc = c->args[cur_arg];
            }
        } else if (match_command("-h", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_homedir = true;
                homedir = c->args[cur_arg];
            }
        } else if (match_command("-s", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_shell = true;
                shell = c->args[cur_arg];
            }
        } else if (match_command("-x", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_exp_date = true;
                exp_date = c->args[cur_arg];
            }
        } else if (match_command("-gsd", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_good_since_date = true;
                good_since_date = c->args[cur_arg];
            }
        } else if (match_command("-mcr", c->args[cur_arg], 4)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_mcr = true;
                mcr = c->args[cur_arg];
            }
        } else if (match_command("-mcl", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_mcl = true;
                mcl = c->args[cur_arg];
            }
        } else if (match_command("-enable", c->args[cur_arg], 4)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                if (match_command("client", c->args[cur_arg], 1)) {
                    opt_client = true;  client = "on";
                } else if (match_command("server", c->args[cur_arg], 1)) {
                    opt_svr = true;     server = "on";
                } else if (match_command("postdated", c->args[cur_arg], 2)) {
                    opt_pdated = true;  post_dated = "on";
                } else if (match_command("forwardable", c->args[cur_arg], 1)) {
                    opt_fwd = true;     forwardable = "on";
                } else if (match_command("proxiable", c->args[cur_arg], 2)) {
                    opt_prox = true;    proxiable = "on";
                } else if (match_command("TGT_authentication", c->args[cur_arg], 1)) {
                    opt_tgt = true;     tgt = "on";
                } else if (match_command("renewable", c->args[cur_arg], 1)) {
                    opt_renew = true;   renewable = "on";
                } else if (match_command("dup_session_key", c->args[cur_arg], 3)) {
                    opt_dup_skey = true;dup_session_key = "on";
                } else {
                    INVALID_OPT(c->args[cur_arg], &st);
                }
            }
        } else if (match_command("-disable", c->args[cur_arg], 4)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                if (match_command("client", c->args[cur_arg], 1)) {
                    opt_client = true;  client = "off";
                } else if (match_command("server", c->args[cur_arg], 1)) {
                    opt_svr = true;     server = "off";
                } else if (match_command("postdated", c->args[cur_arg], 2)) {
                    opt_pdated = true;  post_dated = "off";
                } else if (match_command("forwardable", c->args[cur_arg], 1)) {
                    opt_fwd = true;     forwardable = "off";
                } else if (match_command("proxiable", c->args[cur_arg], 2)) {
                    opt_prox = true;    proxiable = "off";
                } else if (match_command("TGT_authentication", c->args[cur_arg], 1)) {
                    opt_tgt = true;     tgt = "off";
                } else if (match_command("renewable", c->args[cur_arg], 1)) {
                    opt_renew = true;   renewable = "off";
                } else if (match_command("dup_session_key", c->args[cur_arg], 3)) {
                    opt_dup_skey = true;dup_session_key = "off";
                } else {
                    INVALID_OPT(c->args[cur_arg], &st);
                }
            }
        } else if (match_command("-nq", c->args[cur_arg], 3)) {
            opt_no_query = true;
        } else {
            /* not a valid option */
            INVALID_OPT(c->args[cur_arg], &st);
        }

        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    st = ek_init_op(chg_handle);

        /* If all three keys specified, then changing only one account */
    verify = opt_no_query ? false : eif_verify;

    if (!(arg_pname_key || arg_gname_key || arg_oname_key)) {
        tty_show_prompt(Acct_chg_prompts[tty_acct_pname_prompt]);
        arg_pname_key = tty_get_arg(pkey, sizeof(pkey), &st);
        tty_show_prompt(Acct_chg_prompts[tty_acct_gname_prompt]);
        arg_gname_key = tty_get_arg(gkey, sizeof(gkey), &st);
        tty_show_prompt(Acct_chg_prompts[tty_acct_oname_prompt]);
        arg_oname_key = tty_get_arg(okey, sizeof(okey), &st);
    }

    change_one = (arg_pname_key && arg_gname_key && arg_oname_key);

    if (change_one && GOOD_STATUS(&st)) {
        arg_pname_key = eif_write_acct_field(chg_handle, ekn_acct_pname_key,
                                                 pkey, false, &st);
        if (GOOD_STATUS(&st)) {
            arg_gname_key = eif_write_acct_field(chg_handle, ekn_acct_gname_key,
                                                     gkey, false, &st);
            if (GOOD_STATUS(&st))
                arg_oname_key = eif_write_acct_field(chg_handle,
                                        ekn_acct_oname_key, okey, false, &st);
        }
    }

    if (!do_interactive || change_one) {
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    } else {
        /* don't set exit status if we're going to continue interactively */
        EXIT_ON_ERROR(&st);
    }

    if (do_interactive) {
        do_update = false;  /* assume something will go wrong on the 1st try */
        do {
            CLEAR_STATUS(&st);
            if (!(arg_pname_key || arg_gname_key || arg_oname_key)) {
                    /* get the acct key interactively */
                st = tty_get_field_arg(chg_handle, ekn_acct_pname_key,
                                       &tty_null_conv_table, false,
                                       (char *) NULL, pkey, sizeof(pkey),
                                       Acct_chg_prompts[tty_acct_pname_prompt],
                                       &input);
                if (!input) {
                    return st;    /* return if null input on key prompt */
                }

                st = tty_get_field_arg(chg_handle, ekn_acct_gname_key,
                                       &tty_null_conv_table,
                                       false, (char *) NULL, gkey, sizeof(gkey),
                                       Acct_chg_prompts[tty_acct_gname_prompt],
                                       &input);
                if (!input) {
                    return st;    /* return if null input on key prompt */
                }

                st = tty_get_field_arg(chg_handle, ekn_acct_oname_key,
                                       &tty_null_conv_table,
                                       false, (char *) NULL, okey, sizeof(okey),
                                       Acct_chg_prompts[tty_acct_oname_prompt],
                                       &input);
                if (!input) {
                    return st;    /* return if null input on key prompt */
                }

                CONTINUE_OR_EXIT_ON_ERROR(&st);

                arg_pname_key = eif_write_acct_field(chg_handle, ekn_acct_pname_key,
                                                    pkey, false, &st);
                if (GOOD_STATUS(&st))
                    arg_gname_key = eif_write_acct_field(chg_handle,
                                                        ekn_acct_gname_key,
                                                        gkey, false, &st);
                if (GOOD_STATUS(&st))
                    arg_oname_key = eif_write_acct_field(chg_handle,
                                                        ekn_acct_oname_key,
                                                        okey, false, &st);
                CONTINUE_OR_EXIT_ON_ERROR(&st);
            }

                /* do_update will be set to true if at least one field is
                 * successfully written with new (i.e., not the default) data.
                 * If st is ok, then either no new data was written to any field,
                 * or any data written was valid.
                 */
            if ((strlen(pkey) == 0) || (strlen(gkey) == 0) || (strlen(okey) == 0))
                is_wild = true;
            else
                is_wild = false;

            /* if is_wild, then random passwd cmd line switch is bad */
            if ((is_wild) && (opt_random_passwd)) {
                SET_STATUS(&st, tty_invalid_option);
                tty_print_status("Wildcard password changes not allowed.", st);
                return st;
            }

            /*
             * only retrieve password management information if account change
             * is not a wildcard change
             */
            if (!is_wild) {
                /* get password management info */
                if (GOOD_STATUS(&st)) {
                    (void) eif_get_pwd_mgmt_info(chg_handle, &pwd_val_type, &st);
                }

                /*
                 * disallow users to create their own password or generate
                 * random passwords if pwd val type is generation required.
                 */
                if (GOOD_STATUS(&st) && ((opt_random_passwd || opt_passwd) &&
                        pwd_val_type == generation_required)) {
                    SET_STATUS(&st, tty_pwd_gen_req);
                    tty_print_status("Option -rp/-pw disallowed", st);
                    return st;
                }

                if (opt_random_passwd && GOOD_STATUS(&st)) {
                    (void) eif_gen_acct_passwd(chg_handle, &random_passwd_type, NULL, &st);
                }
            }

            if (GOOD_STATUS(&st) && tty_get_acct_args(chg_handle,
                    !is_wild, !opt_random_passwd, Acct_chg_prompts, pwd_val_type, &st)) {
                do_update = true;
            }
            CONTINUE_OR_EXIT_ON_ERROR(&st);

            if (do_update && GOOD_STATUS(&st)) {
                if (is_wild) {
                    st = eif_change_wild_acct(pkey, gkey, okey, chg_handle, verify);
                    /* if an error occurred on any of the wildcard matches we
                     * record the error as usual but we begin a new loop
                     * rather than continuing the old one.
                     */
                    SET_AND_EXIT_ON_ERROR(&st);
                } else {
                    st = eif_change_one_acct(chg_handle);
                    if (! GOOD_STATUS(&st)) {
                        if (tty_ask_y_n("Do you wish to try again ",
                                tty_response_no) == tty_response_yes)
                            continue;
                    }
                }
            }

            if (change_one) {
                break;
            } else {
                arg_pname_key = arg_gname_key = arg_oname_key = false;
                do_update     = false;
                st = ek_init_op(chg_handle);
            }
         } while (true);

        SET_AND_EXIT_ON_ERROR(&st);
        return st;
    }

    is_wild = !change_one;

    /* if we're here then the user entered enough info to avoid interactive
     *  processing, so get the arguments from the command line
     */

    /* if the acct key contains wildcards, then  -np command line switch is bad */
    if ( (is_wild) && (opt_new_pkey) && GOOD_STATUS(&st) ) {
        SET_STATUS(&st, tty_invalid_option);
        tty_print_status("Account id contains wildcards.\
 The -np option is not allowed", st);
        return st;
    }

    /* if acct key contains wildcards, then passwd cmd line switches are bad */
    if ((is_wild) && ((opt_passwd)||(opt_random_passwd)) && GOOD_STATUS(&st)) {
        SET_STATUS(&st, tty_invalid_option);
        tty_print_status("Wildcard password changes not allowed.", st);
        return st;
    }

        /* We've dealt with the acct key.  Now we need to get the non-key input.
           If the user simply re-enters all the defaults, we don't want to
           attempt the update, hence the use of the do_update variable */

    if (opt_new_pkey && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_pname, new_pkey,
                                        false, &st);
    }
    if (opt_new_gkey && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_gname, new_gkey,
                                        false, &st);
    }
    if (opt_new_okey && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_oname, new_okey,
                                        false, &st);
    }

    /* get password management info */
    if (GOOD_STATUS(&st)) {
        (void) eif_get_pwd_mgmt_info(chg_handle, &pwd_val_type, &st);
    }
    /*
     * disallow users to create their own password or generate
     * random passwords if pwd val type is generation required.
     */
    if (GOOD_STATUS(&st) && ((opt_random_passwd || opt_passwd) &&
               pwd_val_type == generation_required)) {
        SET_STATUS(&st, tty_pwd_gen_req);
        tty_print_status("Option -rp/-pw disallowed", st);
    }

    if (opt_misc && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_user_misc, misc,
                                           !is_wild, &st);
    }
    if (opt_homedir && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_user_homedir,
                                         homedir, !is_wild, &st);
    }
    if (opt_shell && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_user_shell, shell,
                                          !is_wild, &st);
    }
    if (opt_random_passwd && GOOD_STATUS(&st)) {
        do_update = eif_gen_acct_passwd(chg_handle, &random_passwd_type, NULL, &st);
        if (opt_my_passwd && GOOD_STATUS(&st)) {
            do_update = eif_write_acct_field(chg_handle,
                                                ekn_acct_user_my_plain_passwd,
                                                my_passwd, false, &st);
        } else if (GOOD_STATUS(&st)) {
            do_update = false;
            SET_STATUS(&st, tty_entry_is_required);
            tty_print_status("ADD failed on YOUR PASSWORD field", st);
        }
    } else if (opt_passwd && GOOD_STATUS(&st)) {
        /* Check if user requests a generated password */
        if (pwd_val_type == user_can_select && !strcmp(passwd, "*")) {
            tty_get_gen_pwd(chg_handle, Acct_chg_prompts[tty_acct_passwd_prompt],
                            &new_pwd, &st);
            if (BAD_STATUS(&st)) {
                tty_print_status("Password generation error", st);
            } else {
                do_update = eif_write_acct_field(chg_handle,
                                                 ekn_acct_user_plain_passwd,
                                                 (char *) new_pwd, false, &st);
            }
        } else {
            /* there's never a default plain passwd */
            do_update = eif_write_acct_field(chg_handle,
                                             ekn_acct_user_plain_passwd,
                                             passwd, false, &st);
        }
        if (opt_my_passwd && GOOD_STATUS(&st)) {
            do_update = eif_write_acct_field(chg_handle,
                                                ekn_acct_user_my_plain_passwd,
                                                my_passwd, false, &st);
        } else if (GOOD_STATUS(&st)) {
            do_update = false;
            SET_STATUS(&st, tty_entry_is_required);
            tty_print_status("CHANGE failed on YOUR PASSWORD field", st);
        }
    }
    if (opt_exp_date && GOOD_STATUS(&st)) {
        date = tty_get_kernel_io_val(exp_date, &tty_date_conv_table,
                                     &opt_exp_date);
        if (!opt_exp_date) {
            SET_STATUS(&st, tty_bad_data_format);
            tty_status_pre_echo(date, "", st);
        } else {
            tmp_update=eif_write_acct_field(chg_handle,ekn_acct_admin_exp_date,
                                              date, !is_wild, &st);
            do_update = (GOOD_STATUS(&st) && (do_update || tmp_update));
        }
    }
    if (opt_acct_valid && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_admin_acct_valid,
                                  ek_flag_on, !is_wild, &st);
    }
    if (opt_acct_not_valid && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_admin_acct_valid,
                                          ek_flag_off, !is_wild, &st);
    }

    if (opt_pwd_valid && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_user_passwd_valid,
                                          ek_flag_on, !is_wild, &st);
    }
    if (opt_pwd_not_valid && GOOD_STATUS(&st)) {
        do_update = eif_write_acct_field(chg_handle, ekn_acct_user_passwd_valid,
                                          ek_flag_off, !is_wild, &st);
    }
    if (opt_good_since_date && GOOD_STATUS(&st)) {
        date = tty_get_kernel_io_val(good_since_date, &tty_date_conv_table,
                                     &opt_good_since_date);
        if (!opt_good_since_date) {
            SET_STATUS(&st, tty_bad_data_format);
            tty_status_pre_echo(date, "", st);
        } else {
            do_update = eif_write_acct_field(chg_handle,
                                        ekn_acct_admin_good_since_date, date,
                                        !is_wild, &st);
        }
    }

    if (GOOD_STATUS(&st) && opt_svr) {          /* account server flag */
        do_update = eif_write_acct_field(chg_handle, ekn_acct_admin_acct_server,
                                            server, false, &st);
    }
    if (GOOD_STATUS(&st) && opt_client) {       /* account client flag */
        do_update = eif_write_acct_field(chg_handle, ekn_acct_admin_acct_client,
                                            client, false, &st);
    }
    if (GOOD_STATUS(&st) && opt_pdated) {       /* post-dated certificates */
        do_update = eif_write_acct_field(chg_handle,
                                            ekn_acct_admin_auth_post_dated,
                                            post_dated, false, &st);
    }
    if (GOOD_STATUS(&st) && opt_fwd) {          /* forwardable certificates */
        do_update = eif_write_acct_field(chg_handle,
                                            ekn_acct_admin_auth_forwardable,
                                            forwardable, false, &st);
    }
    if (GOOD_STATUS(&st) && opt_tgt) {          /* tgt certificates */
        do_update = eif_write_acct_field(chg_handle, ekn_acct_admin_auth_tgt,
                                            tgt, false, &st);
    }
    if (GOOD_STATUS(&st) && opt_renew) {        /* renewable certificates */
        do_update = eif_write_acct_field(chg_handle,
                                            ekn_acct_admin_auth_renewable,
                                            renewable, false, &st);
    }
    if (GOOD_STATUS(&st) && opt_prox) {         /* proxiable certificates */
        do_update = eif_write_acct_field(chg_handle,
                                            ekn_acct_admin_auth_proxiable,
                                            proxiable, false, &st);
    }
    if (GOOD_STATUS(&st) && opt_dup_skey) {     /* dup_session_key's */
        do_update = eif_write_acct_field(chg_handle,
                                            ekn_acct_admin_auth_dup_session_key,
                                            dup_session_key, false, &st);
    }

    if (GOOD_STATUS(&st) && opt_mcr) {          /* max cert renewable */
        do_update = eif_write_acct_field(chg_handle,
                                            ekn_acct_max_renewable_lifetime,
                                            mcr, false, &st);
        /* If MCR spec'd w/out MCL and auth plcy doesn't exist, use def for MCL */
        if (GOOD_STATUS(&st) && !opt_mcl &&
            (!ek_field_has_data(chg_handle, ekn_acct_max_ticket_lifetime))) {

            mcl = eif_get_default(eif_def_max_ticket_lifetime);
            (void) eif_write_acct_field(chg_handle, ekn_acct_max_ticket_lifetime,
                                        mcl, false, &st);
        }
    }
    if (GOOD_STATUS(&st) && opt_mcl) {          /* max cert lifetime */
        do_update = eif_write_acct_field(chg_handle, ekn_acct_max_ticket_lifetime,
                                    mcl, false, &st);
        /* If MCL spec'd w/out MCR and auth plcy doesn't exist, use def for MCR */
        if (GOOD_STATUS(&st) && !opt_mcr &&
            (!ek_field_has_data(chg_handle, ekn_acct_max_renewable_lifetime))) {

            mcr = eif_get_default(eif_def_max_renewable_lifetime);
            (void) eif_write_acct_field(chg_handle, ekn_acct_max_renewable_lifetime,
                                        mcr, false, &st);
        }
    }

    if (do_update) {
        if (!is_wild) {
            st = eif_change_one_acct(chg_handle);
        } else {
            st = eif_change_wild_acct(pkey, gkey, okey, chg_handle, verify);
        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_add_acct
#ifndef __STDC__
    ( c )
    command_buf      *c;
#else
  (
    command_buf      *c
  )
#endif
{
    char        *abbrev, *misc, *homedir, *shell, *mcr, *mcl,
                *my_passwd, *passwd, *exp_date, *server, *client,
                *post_dated, *forwardable, *tgt, *renewable,
                *proxiable, *dup_session_key, *good_since_date;

    boolean32   opt_abbrev, opt_misc, opt_homedir, opt_shell,
                opt_passwd, opt_my_passwd, opt_random_passwd,
                opt_exp_date, opt_acct_valid, opt_acct_not_valid,
                opt_pwd_valid, opt_pwd_not_valid, opt_good_since_date,
                opt_gkey, opt_okey, opt_svr, opt_client, opt_pdated, opt_fwd,
                opt_tgt, opt_renew, opt_prox, opt_dup_skey, opt_mcr, opt_mcl;

    boolean32   do_interactive     = true,
                arg_acct_key       = false,
                input;
    char        *key_parts, /* the rgy_edit kernel rep of abbreviations */
                *date;

    int                  cur_arg;
    ek_login_name_buf_t  pkey, gkey, okey;
    long                 key_len;
    error_status_t       st;
    ek_op_handle_t       view_handle,
                         add_handle = A_add_h;
    sec_passwd_type_t    random_passwd_type;
    ek_pwd_val_type_t    pwd_val_type;
    sec_rgy_name_t       new_pwd;

    SET_STATUS(&st, error_status_ok);

        /* Assume no options were specified */
    opt_abbrev = opt_misc = opt_homedir = opt_shell = opt_passwd =
    opt_my_passwd = opt_exp_date = opt_acct_valid = opt_acct_not_valid =
    opt_pwd_valid = opt_pwd_not_valid = opt_good_since_date =
    opt_gkey = opt_okey = opt_svr = opt_client = opt_pdated =
    opt_fwd = opt_tgt = opt_renew = opt_prox = opt_dup_skey =
    opt_random_passwd = opt_mcr = opt_mcl = false;

    random_passwd_type = sec_passwd_des;    /* only random keytype supported */

        /* look for non-option arguments at beginning of argument list */
    cur_arg = 1;
    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg])
           && GOOD_STATUS(&st)) {
            /* only non-option argument is the name */
        if (cur_arg == 1) {
            strncpy((char *)pkey, c->args[cur_arg], sizeof(pkey));
            arg_acct_key = true;
        } else {
            INVALID_ARG(c->args[cur_arg], &st);
        }
        cur_arg++;
    }

           /* Look for options */
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {
        if (match_command("-pv", c->args[cur_arg], 3)) {
            if (opt_pwd_not_valid) {
                INCOMPATIBLE_OPTS("-pv", "-pnv", &st);
            } else {
                opt_pwd_valid = true;
            }
        } else if (match_command("-g", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_gkey = true;
                strncpy((char *)gkey, c->args[cur_arg], sizeof(gkey));
            }
        } else if (match_command("-o", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_okey = true;
                strncpy((char *)okey, c->args[cur_arg], sizeof(okey));
            }
        } else if (match_command("-pnv", c->args[cur_arg], 4)) {
            if (opt_pwd_valid) {
                INCOMPATIBLE_OPTS("-pv", "-pnv", &st);
            } else {
                opt_pwd_not_valid = true;
            }
        } else if (match_command("-av", c->args[cur_arg], 3)) {
            if (opt_acct_not_valid) {
                INCOMPATIBLE_OPTS("-av", "-anv", &st);
            } else {
                opt_acct_valid = true;
            }
        } else if (match_command("-anv", c->args[cur_arg], 4)) {
            if (opt_acct_valid) {
                INCOMPATIBLE_OPTS("-av", "-anv", &st);
            } else {
                opt_acct_not_valid = true;
            }
        } else if (match_command("-pw", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                if (opt_random_passwd) {
                    INCOMPATIBLE_OPTS("-pw", "-rp", &st);
                } else {
                    opt_passwd = true;
                    passwd = c->args[cur_arg];
                }
            }
        } else if (match_command("-rp", c->args[cur_arg], 3)) {
            if (opt_passwd) {
                INCOMPATIBLE_OPTS("-pw", "-rp", &st);
            } else {
                opt_random_passwd = true;
            }
        } else if (match_command("-mp", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_my_passwd = true;
                my_passwd = c->args[cur_arg];
            }
        } else if (match_command("-m", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_misc = true;
                misc = c->args[cur_arg];
            }
        } else if (match_command("-h", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_homedir = true;
                homedir = c->args[cur_arg];
            }
        } else if (match_command("-s", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_shell = true;
                shell = c->args[cur_arg];
            }
        } else if (match_command("-x", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_exp_date = true;
                exp_date = c->args[cur_arg];
            }
        } else if (match_command("-gsd", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_good_since_date = true;
                good_since_date = c->args[cur_arg];
            }
        } else if (match_command("-mcr", c->args[cur_arg], 4)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_mcr = true;
                mcr = c->args[cur_arg];
            }
        } else if (match_command("-mcl", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_mcl = true;
                mcl = c->args[cur_arg];
            }
        } else if (match_command("-enable", c->args[cur_arg], 4)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                if (match_command("client", c->args[cur_arg], 1)) {
                    opt_client = true;  client = "on";
                } else if (match_command("server", c->args[cur_arg], 1)) {
                    opt_svr = true;     server = "on";
                } else if (match_command("postdated", c->args[cur_arg], 2)) {
                    opt_pdated = true;  post_dated = "on";
                } else if (match_command("forwardable", c->args[cur_arg], 1)) {
                    opt_fwd = true;     forwardable = "on";
                } else if (match_command("proxiable", c->args[cur_arg], 2)) {
                    opt_prox = true;    proxiable = "on";
                } else if (match_command("TGT_authentication", c->args[cur_arg], 1)) {
                    opt_tgt = true;     tgt = "on";
                } else if (match_command("renewable", c->args[cur_arg], 1)) {
                    opt_renew = true;   renewable = "on";
                } else if (match_command("dup_session_key", c->args[cur_arg], 3)) {
                    opt_dup_skey = true;dup_session_key = "on";
                } else {
                    INVALID_OPT(c->args[cur_arg], &st);
                }
            }
        } else if (match_command("-disable", c->args[cur_arg], 4)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                if (match_command("client", c->args[cur_arg], 1)) {
                    opt_client = true;  client = "off";
                } else if (match_command("server", c->args[cur_arg], 1)) {
                    opt_svr = true;     server = "off";
                } else if (match_command("postdated", c->args[cur_arg], 2)) {
                    opt_pdated = true;  post_dated = "off";
                } else if (match_command("forwardable", c->args[cur_arg], 1)) {
                    opt_fwd = true;     forwardable = "off";
                } else if (match_command("proxiable", c->args[cur_arg], 2)) {
                    opt_prox = true;    proxiable = "off";
                } else if (match_command("TGT_authentication", c->args[cur_arg], 1)) {
                    opt_tgt = true;     tgt = "off";
                } else if (match_command("renewable", c->args[cur_arg], 1)) {
                    opt_renew = true;   renewable = "off";
                } else if (match_command("dup_session_key", c->args[cur_arg], 3)) {
                    opt_dup_skey = true;dup_session_key = "off";
                } else {
                    INVALID_OPT(c->args[cur_arg], &st);
                }
            }
        } else {
            /* not a valid option in any pgo domain */
            INVALID_OPT(c->args[cur_arg], &st);
        }

           /* -rp option doesn't affect command interactivity */
        if (GOOD_STATUS(&st) &&
            !match_command("-rp", c->args[cur_arg], 3) ) {

            do_interactive = false;
        }

        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    st = ek_init_op(add_handle);

    if (!do_interactive) {
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    } else {
        EXIT_ON_ERROR(&st);
    }

    if (do_interactive) {
        do {
            if (!arg_acct_key) {
                /* get the acct key interactively */
                tty_show_prompt(Acct_add_prompts[tty_acct_pname_prompt]);
                input = tty_get_arg(pkey, sizeof(pkey), &st);
                if (!input) {
		    SET_STATUS(&st, tty_entry_is_required);
                    return st;    /* return if null input on key prompt */
                }
                if (GOOD_STATUS(&st)) {
                    arg_acct_key = eif_write_acct_field(add_handle,
                                                        ekn_acct_pname_key,
                                                        pkey, false, &st);
                }
            } else {
                arg_acct_key = eif_write_acct_field(add_handle,
                                                    ekn_acct_pname_key,
                                                    pkey, false, &st);
            }

            if (GOOD_STATUS(&st)) {
                /* get the acct group interactively */
                tty_show_prompt(Acct_add_prompts[tty_acct_gname_prompt]);
                input = tty_get_arg(gkey, sizeof(gkey), &st);
                if (!input) {
		    SET_STATUS(&st, tty_entry_is_required);
                    tty_print_status("ADD command aborted", st);
                    return st;    /* return if null input on key prompt */
                }
                if (GOOD_STATUS(&st)) {
                    arg_acct_key = eif_write_acct_field(add_handle,
                                                        ekn_acct_gname_key,
                                                        gkey, false, &st);
                }
            }
            if (GOOD_STATUS(&st)) {
                /* get the acct org interactively */
                tty_show_prompt(Acct_add_prompts[tty_acct_oname_prompt]);
                input = tty_get_arg(okey, sizeof(okey), &st);
                if (!input) {
		    SET_STATUS(&st, tty_entry_is_required);
                    tty_print_status("ADD command aborted", st);
                    return st;    /* return if null input on key prompt */
                }
                if (GOOD_STATUS(&st)) {
                    arg_acct_key = eif_write_acct_field(add_handle,
                                                        ekn_acct_oname_key,
                                                        okey, false, &st);
                }
            }

            if (! GOOD_STATUS(&st)) {
                if (tty_ask_y_n("Do you wish to try again ",
                        tty_response_no) == tty_response_yes) {
                    arg_acct_key = false;
                    continue;
                }
                else {
                    CLEAR_STATUS(&st);
                    return st;
                }
            }

            /* get password management info */
            if (GOOD_STATUS(&st)) {
                (void) eif_get_pwd_mgmt_info(add_handle, &pwd_val_type, &st);
            }

            /* 
             * disallow users to create their own password or generate
             * random passwords if pwd val type is generation required.
             */
            if (GOOD_STATUS(&st) && ((opt_random_passwd || opt_passwd) && 
                    pwd_val_type == generation_required)) {
                SET_STATUS(&st, tty_pwd_gen_req);
                tty_print_status("Option -rp/-pw disallowed", st);
                return st;
            }

            if (opt_random_passwd && GOOD_STATUS(&st)) {
                (void) eif_gen_acct_passwd(add_handle,&random_passwd_type,NULL,&st);
            }

            if (GOOD_STATUS(&st)) {
                (void) tty_get_acct_args(add_handle, true, !opt_random_passwd,
                                         Acct_add_prompts, pwd_val_type, &st);
            }
            CONTINUE_OR_EXIT_ON_ERROR(&st);

            st = eif_add_acct(add_handle);
            if (! GOOD_STATUS(&st))
                if (tty_ask_y_n("Do you wish to try again ",
                        tty_response_no) == tty_response_yes) {
                    if (STATUS_EQUAL(&st, ek_err_object_exists))
                        arg_acct_key = false;
                    continue;
                }
                else {
                    CLEAR_STATUS(&st);
                    return st;
                }

               /* add was successful so set up a new loop */
            st = ek_init_op(add_handle);
            arg_acct_key = false;

        } while (true);

        SET_AND_EXIT_ON_ERROR(&st);
        return st;
    }

    /* get the data from the command line */

    if (arg_acct_key) {
        (void) eif_write_acct_field(add_handle, ekn_acct_pname_key, pkey,
                                    false, &st);
    } else {
        SET_STATUS(&st, tty_entry_is_required);
        tty_print_status("ADD failed on PRINCIPAL field", st);
    }
    if (GOOD_STATUS(&st) && opt_gkey) {
        (void) eif_write_acct_field(add_handle, ekn_acct_gname_key, gkey,
                                    false, &st);
    } else if (GOOD_STATUS(&st)) {
        SET_STATUS(&st, tty_entry_is_required);
        tty_print_status("ADD failed on GROUP field", st);
    }
    if (GOOD_STATUS(&st) && opt_okey) {
        (void) eif_write_acct_field(add_handle, ekn_acct_oname_key, okey,
                                    false, &st);
    } else if (GOOD_STATUS(&st)) {
        SET_STATUS(&st, tty_entry_is_required);
        tty_print_status("ADD failed on ORG field", st);
    }
    if (GOOD_STATUS(&st)) {
        /* get password management info */
        (void) eif_get_pwd_mgmt_info(add_handle, &pwd_val_type, &st);
    }
    /*
     * disallow users to create their own password or generate
     * random passwords if pwd val type is generation required.
     */
    if (GOOD_STATUS(&st) && ((opt_random_passwd || opt_passwd) &&
               pwd_val_type == generation_required)) {
        SET_STATUS(&st, tty_pwd_gen_req);
        tty_print_status("Option -rp/-pw disallowed", st);
    }
    if (GOOD_STATUS(&st)) {
        if (! opt_misc)
            misc = eif_get_default(eif_def_acct_misc);
        (void) eif_write_acct_field(add_handle, ekn_acct_user_misc, misc,
                                     false, &st);
    }
    if (GOOD_STATUS(&st)) {
        if (! opt_homedir)
            homedir = eif_get_default(eif_def_acct_homedir);
        (void) eif_write_acct_field(add_handle, ekn_acct_user_homedir, homedir,
                                     false, &st);
    }
    if (GOOD_STATUS(&st)) {
        if (! opt_shell)
            shell = eif_get_default(eif_def_acct_shell);
        (void) eif_write_acct_field(add_handle, ekn_acct_user_shell, shell,
                                     false, &st);
    }
    if (opt_random_passwd && GOOD_STATUS(&st)) {
        (void) eif_gen_acct_passwd(add_handle, &random_passwd_type, NULL, &st);
    } else if (opt_passwd && GOOD_STATUS(&st)) {
        /* check if user requests generated password */
        if (pwd_val_type == user_can_select && !strcmp(passwd, "*")) {
            tty_get_gen_pwd(add_handle, Acct_add_prompts[tty_acct_passwd_prompt],
                            &new_pwd, &st);
            if (BAD_STATUS(&st)) {
                tty_print_status("Password generation error", st);
            } else {
                (void) eif_write_acct_field(add_handle, ekn_acct_user_plain_passwd,
                                            (char *) new_pwd, false, &st);
            }
        } else {
            (void) eif_write_acct_field(add_handle, ekn_acct_user_plain_passwd,
                                        passwd, false, &st);
        }
    } else if (GOOD_STATUS(&st)) {
        SET_STATUS(&st, tty_entry_is_required);
        tty_print_status("ADD failed on PASSWORD field", st);
    }
    if (opt_my_passwd && GOOD_STATUS(&st)) {
        (void) eif_write_acct_field(add_handle, ekn_acct_user_my_plain_passwd,
                                     my_passwd, false, &st);
    } else if (GOOD_STATUS(&st)) {
        SET_STATUS(&st, tty_entry_is_required);
        tty_print_status("ADD failed on YOUR PASSWORD field", st);
    }
    if (GOOD_STATUS(&st)) {
        if (! opt_exp_date)
            exp_date = eif_get_default(eif_def_acct_exp_date);
        date = tty_get_kernel_io_val(exp_date, &tty_date_conv_table,
                                     &opt_exp_date);
        if (!opt_exp_date) {
            SET_STATUS(&st, tty_bad_data_format);
            tty_status_pre_echo(exp_date,"" , st);
        } else {
            (void) eif_write_acct_field(add_handle, ekn_acct_admin_exp_date,
                                         date, false, &st);
        }
    }
    if (opt_acct_not_valid && GOOD_STATUS(&st)) {
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_acct_valid,
                                     ek_flag_off, false, &st);
    } else if (GOOD_STATUS(&st)) {
        eif_write_acct_field(add_handle, ekn_acct_admin_acct_valid,
                              ek_flag_on, false, &st);
    }
    if (opt_pwd_not_valid && GOOD_STATUS(&st)) {
        (void) eif_write_acct_field(add_handle, ekn_acct_user_passwd_valid,
                                     ek_flag_off, false, &st);
    } else if (GOOD_STATUS(&st)) {
        (void) eif_write_acct_field(add_handle, ekn_acct_user_passwd_valid,
                                     ek_flag_on, false, &st);
    }
    if (GOOD_STATUS(&st)) {
        if (! opt_good_since_date)
            good_since_date = eif_get_default(eif_def_good_since_date);
        date = tty_get_kernel_io_val(good_since_date, &tty_date_conv_table,
                                     &opt_good_since_date);
        if (!opt_exp_date) {
            SET_STATUS(&st, tty_bad_data_format);
            tty_status_pre_echo(good_since_date,"" , st);
        } else {
            (void) eif_write_acct_field(add_handle,
                                         ekn_acct_admin_good_since_date,
                                         date, false, &st);
        }
    }

    if (GOOD_STATUS(&st)) {                     /* account server flag */
        if (! opt_svr)
            server = eif_get_default(eif_def_acct_server);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_acct_server,
                                    server, false, &st);
    }
    if (GOOD_STATUS(&st)) {                     /* account client flag */
        if (! opt_client)
            client = eif_get_default(eif_def_acct_client);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_acct_client,
                                    client, false, &st);
    }
    if (GOOD_STATUS(&st)) {                     /* post-dated certificates */
        if (! opt_pdated)
            post_dated = eif_get_default(eif_def_acct_post_dated);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_auth_post_dated,
                                    post_dated, false, &st);
    }
    if (GOOD_STATUS(&st)) {                     /* forwardable certificates */
        if (! opt_fwd)
            forwardable = eif_get_default(eif_def_acct_forwardable);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_auth_forwardable,
                                    forwardable, false, &st);
    }
    if (GOOD_STATUS(&st)) {                     /* tgt certificates */
        if (! opt_tgt)
            tgt = eif_get_default(eif_def_acct_tgt);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_auth_tgt,
                                    tgt, false, &st);
    }
    if (GOOD_STATUS(&st)) {                     /* renewable certificates */
        if (! opt_renew)
            renewable = eif_get_default(eif_def_acct_renewable);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_auth_renewable,
                                    renewable, false, &st);
    }
    if (GOOD_STATUS(&st)) {                     /* proxiable certificates */
        if (! opt_prox)
            proxiable = eif_get_default(eif_def_acct_proxiable);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_auth_proxiable,
                                    proxiable, false, &st);
    }
    if (GOOD_STATUS(&st)) {                     /* dup_session_key's */
        if (! opt_dup_skey)
            dup_session_key = eif_get_default(eif_def_acct_dup_session_key);
        (void) eif_write_acct_field(add_handle, ekn_acct_admin_auth_dup_session_key,
                                    dup_session_key, false, &st);
    }

    if (GOOD_STATUS(&st) && opt_mcr) {          /* max cert renewable */
        (void) eif_write_acct_field(add_handle, ekn_acct_max_renewable_lifetime,
                                    mcr, false, &st);
        if (GOOD_STATUS(&st) && !opt_mcl) {
            mcl = eif_get_default(eif_def_max_ticket_lifetime);
            (void) eif_write_acct_field(add_handle,ekn_acct_max_ticket_lifetime,
                                        mcl, false, &st);
        }
    }
    if (GOOD_STATUS(&st) && opt_mcl) {          /* max cert lifetime */
        (void) eif_write_acct_field(add_handle, ekn_acct_max_ticket_lifetime,
                                    mcl, false, &st);
        if (GOOD_STATUS(&st) && !opt_mcr) {
            mcr = eif_get_default(eif_def_max_renewable_lifetime);
            (void) eif_write_acct_field(add_handle,
                                        ekn_acct_max_renewable_lifetime,
                                        mcr, false, &st);
        }
    }

    if (GOOD_STATUS(&st)) {
        st = eif_add_acct(add_handle);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_delete_acct
#ifndef __STDC__
    (c)
    command_buf      *c;
#else
  (
    command_buf      *c
  )
#endif
{
    error_status_t   st;
    boolean32        arg_key      = false,
                     opt_no_query = false,
                     verify;
    int              cur_arg;
    ek_op_handle_t   del_handle = A_delete_h;
    ek_login_name_buf_t pkey, gkey, okey;

    SET_STATUS(&st, error_status_ok);
    strcpy((char *)pkey, "");
    strcpy((char *)gkey, "");
    strcpy((char *)okey, "");

    cur_arg = 1;
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {

	if (match_command("-p", c->args[cur_arg], 2)) {
	    if ((strlen(pkey) != 0) || ((cur_arg+1) >= c->num_args)) {
		OPT_REQUIRES_ARG(c->args[cur_arg], &st);
	    } else {
                strcpy((char *)pkey, c->args[++cur_arg]);
	        arg_key = true;
	    }
	} else if (match_command("-g", c->args[cur_arg], 2)) {
	    if ((strlen(gkey) != 0) || ((cur_arg+1) >= c->num_args)) {
		OPT_REQUIRES_ARG(c->args[cur_arg], &st);
	    } else {
                strcpy((char *)gkey, c->args[++cur_arg]);
	        arg_key = true;
	    }
	} else if (match_command("-o", c->args[cur_arg], 2)) {
	    if ((strlen(okey) != 0) || ((cur_arg+1) >= c->num_args)) {
		OPT_REQUIRES_ARG(c->args[cur_arg], &st);
	    } else {
                strcpy((char *)okey, c->args[++cur_arg]);
	        arg_key = true;
	    }
	} else if (match_command("-nq", c->args[cur_arg], 3)) {
	    opt_no_query = true;
	} else {
	    INVALID_OPT(c->args[cur_arg], &st);
	}
	cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    verify = opt_no_query ? false : eif_verify;

    if (!arg_key) {
	SET_STATUS(&st, tty_entry_is_required);
	tty_print_status(tty_acct_del_key_missing, st);
    } else {
	st = eif_delete_acct(pkey, gkey, okey, del_handle, verify);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_view_acct
#ifndef __STDC__
    (c)
    command_buf      *c;
#else
  (
    command_buf      *c
  )
#endif
{
    boolean32        opt_full       = false;
    char             *key;
    int              i;
    sec_rgy_name_t   view_pname, view_gname, view_oname;
    boolean32        pkey, gkey, okey;
    error_status_t   st;

    strcpy((char *)view_pname, "");/* the view key defaults to a null string */
    strcpy((char *)view_gname, "");
    strcpy((char *)view_oname, "");
    pkey = gkey = okey = false;

    SET_STATUS(&st, error_status_ok);
    for (i = 1; i < c->num_args && GOOD_STATUS(&st); i++) {
	if (match_command("-full", c->args[i], 2))
	    opt_full = true;
	else  {
	    if (!IS_AN_OPTION(c->args[i])) {
                /* 1st non-option arg is pname, 2nd gname, 3rd oname */
                if (!pkey) {
                    pkey = true;
                    strcpy((char *)view_pname, c->args[i]);
                }
                else if (!gkey) {
                    gkey = true;
                    strcpy((char *)view_gname, c->args[i]);
                }
                else if (!okey) {
                    okey = true;
                    strcpy((char *)view_oname, c->args[i]);
                }
                else
                    INVALID_ARG(c->args[i], &st);
	    } else
		    /* not a valid view argument or option in any domain */
		INVALID_ARG(c->args[i], &st);
	}
    }

    if (GOOD_STATUS(&st))
        st = eif_view_acct((char *)view_pname, (char *)view_gname,
                           (char *) view_oname, opt_full);

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC void tty_display_lrgy_entry
#ifndef __STDC__
    ( op_handle, show_full, stp )
    ek_op_handle_t  op_handle;
    boolean32       show_full;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    boolean32       show_full,
    error_status_t  *stp
  )
#endif
{
    ek_unix_num_buf_t   uid,
                        gid;
    ek_enc_passwd_buf_t passwd;
    ek_name_buf_t       name, name2,
                        misc,
                        homedir,
                        shell;
    ek_date_time_buf_t  timestr;
    long int            field_len;
    boolean32           map;
#define                 full_indent "  "

    ek_read_field(op_handle, ekn_lrgy_entry_cell, name, sizeof(name),
                    &field_len);
    ek_read_field(op_handle, ekn_lrgy_entry_princ, name2, sizeof(name2),
                    &field_len);
    printf("Principal: %s/%s\n", name, name2);

    ek_read_field(op_handle, ekn_lrgy_entry_pw_name, name, sizeof(name),
                   &field_len);
    ek_read_field(op_handle, ekn_lrgy_entry_pw_passwd, passwd, sizeof(passwd),
                   &field_len);
    ek_read_field(op_handle, ekn_lrgy_entry_pw_uid, uid, sizeof(uid),
                   &field_len);
    ek_read_field(op_handle, ekn_lrgy_entry_pw_gid, gid, sizeof(gid),
                   &field_len);
    ek_read_field(op_handle, ekn_lrgy_entry_pw_gecos, misc , sizeof(misc),
                   &field_len);
    ek_read_field(op_handle, ekn_lrgy_entry_pw_dir, homedir, sizeof(homedir),
                   &field_len);
    ek_read_field(op_handle, ekn_lrgy_entry_pw_shell, shell, sizeof(shell),
                   &field_len);

        /* print the /etc/passwd-like data */
    printf("%s%s:%s:%s:%s:%s:%s:%s:\n", full_indent, name, passwd, uid, gid,
            misc, homedir, shell);

        /* show the rest of the account info if show_full is true */
    if (show_full) {
        ek_read_field(op_handle, ekn_lrgy_entry_timestamp, timestr,
                        sizeof(timestr), &field_len);
        printf("%sStored:              %s\n", full_indent,
                tty_get_if_io_val(timestr, &tty_date_conv_table, &map));
        ek_read_field(op_handle, ekn_lrgy_entry_acct_exp, timestr,
                        sizeof(timestr), &field_len);
        printf("%sAccount  expiration: %s\n", full_indent,
                tty_get_if_io_val(timestr, &tty_date_conv_table, &map));
        ek_read_field(op_handle, ekn_lrgy_entry_passwd_exp, timestr,
                        sizeof(timestr), &field_len);
        printf("%sPassword expiration: %s\n", full_indent,
                tty_get_if_io_val(timestr, &tty_date_conv_table, &map));
        ek_read_field(op_handle, ekn_lrgy_entry_identity_exp, timestr,
                        sizeof(timestr), &field_len);
        printf("%sIdentity expiration: %s\n", full_indent,
                tty_get_if_io_val(timestr, &tty_date_conv_table, &map));
        ek_read_field(op_handle, ekn_lrgy_entry_passwd_valid, name,
                        sizeof(name), &field_len);
        printf("%sPassword is %s\n", full_indent, name);
    }
}


/*
 * tty_do_lview - local registry view
 */
PUBLIC error_status_t tty_do_lview
#ifndef __STDC__
    (c)
    command_buf      *c;
#else
  (
    command_buf      *c
  )
#endif
{
    error_status_t  st;
    ek_op_handle_t  op_handle;
    boolean32       found_one, show_full, modified;
    CLEAR_STATUS(&st);
    found_one = show_full = modified = false;
    op_handle = L_get_h;

    switch(c->num_args) {
        case 1:
            break;
        case 2:
            if (match_command("-f", c->args[1], 2)) {
                show_full = true;
            } else {
                INVALID_ARG(c->args[1], &st);
            }
            break;
        default:
            SET_STATUS(&st, tty_invalid_argument);
            tty_print_status("Too many arguments", st);
            break;
    }

        /* Don't do anything if cmd line args were bad */
    if (GOOD_STATUS(&st)) {
        st = ek_init_op(op_handle);
    } else {
        return;
    }

    if (GOOD_STATUS(&st)) {
        while (GOOD_STATUS(&st)) {
            st = ek_do_op(op_handle);
            if (GOOD_STATUS(&st)) {
                found_one = true;
                tty_display_lrgy_entry(op_handle, show_full, &st);
            }
        }
        ek_lrgy_entry_release_cursor(op_handle, &modified);
    }
    if (GOOD_STATUS(&st)) {
        if (!found_one) {
            printf("No entries found.\n");
        }
    } else {
        if (!found_one) {
            tty_print_status("Unable to view local registry entries", st);
        } else {
            if (! STATUS_EQUAL(&st, sec_lrgy_s_no_more_entries)) {
                tty_print_status("Can't read next entry", st);
            }
        }
    }
    if (modified) {
        printf("WARNING: Local registry was modified between first and last\n");
        printf("lookup (because a READ lock is not allowed over multiple reads).\n");
        printf("Output may not correspond to current state of local registry.\n\n");
    }

    return st;
}

