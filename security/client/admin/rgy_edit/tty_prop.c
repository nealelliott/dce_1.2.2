/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_prop.c,v $
 * Revision 1.1.6.2  1996/03/11  13:27:25  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:02:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:46  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:42:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:50  zeliff]
 * 
 * Revision 1.1.2.3  1992/05/08  18:21:56  burati
 * 	Use sec_rgy_name_t instead of ek_pname_buf_t for names
 * 	[1992/05/07  23:13:07  burati]
 * 
 * Revision 1.1.2.2  1992/04/03  20:38:11  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  21:02:41  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty_prop.c V=22 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface properties/policies  processing
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_prop.c	22 - 03/02/92";
#endif

#include <stdio.h>
#include <un_strng.h>
#include <un_io.h>

#include <tty_base.h>
#include <macros.h>

#include <ek_error.h>
#include <ek_types.h>

#include <eif_prop.h>
#include <eif_site.h>

#include <tty_conf.h>
#include <tty_io.h>
#include <tty_util.h>
#include <tty_err.h>
#include <tty_prop.h>

#include <args.h>

PUBLIC void tty_display_policy
#ifndef __STDC__
    ( policy_handle )
    ek_op_handle_t  policy_handle;
#else
  (
    ek_op_handle_t  policy_handle
  )
#endif
{
    ek_pname_buf_t  field_buff;
    long int        field_len;
    boolean32       io_map;
    int             description_width = 42;
    error_status_t  st;

#   define           policy_indent "    "   /* leading blanks for formatting */

    ek_read_field(policy_handle, ekn_policy_acct_life, field_buff,
		   sizeof(field_buff), &field_len);
    printf ("%s%-*s", policy_indent, description_width, "Account lifespan:");
    printf ("%s\n", tty_get_if_io_val(field_buff, &tty_lifespan_conv_table,
				      &io_map));

    ek_read_field(policy_handle, ekn_policy_passwd_min_len, field_buff,
		   sizeof(field_buff), &field_len);
    printf ("%s%-*s%s\n", policy_indent, description_width, "Password min len:",
	    field_buff);

    ek_read_field(policy_handle, ekn_policy_passwd_life, field_buff,
		   sizeof(field_buff), &field_len);

    printf ("%s%-*s", policy_indent, description_width, "Password lifespan:");
    printf ("%s\n", tty_get_if_io_val(field_buff, &tty_lifespan_conv_table,
				      &io_map));

    ek_read_field(policy_handle, ekn_policy_passwd_exp, field_buff,
		   sizeof(field_buff), &field_len);
    printf ("%s%-*s", policy_indent, description_width,
	    "Password expiration date:");
    printf ("%s\n", tty_get_if_io_val(field_buff, &tty_date_conv_table,
				      &io_map));

    ek_read_field(policy_handle, ekn_policy_passwd_no_spaces, field_buff,
		   sizeof(field_buff), &field_len);
    if (CHECK_FLAG(field_buff, strlen(field_buff)))
	printf("%sPasswords MAY NOT be all spaces, ", policy_indent);
    else
	printf("%sPasswords MAY be all spaces,", policy_indent);

    ek_read_field(policy_handle, ekn_policy_passwd_non_alpha, field_buff,
		   sizeof(field_buff), &field_len);
    if (CHECK_FLAG(field_buff, strlen(field_buff)))
	printf(" MAY NOT be all alphanumeric\n");
    else
	printf(" MAY be all alphanumeric\n");
}


PUBLIC void tty_display_auth_policy
#ifndef __STDC__
    ( auth_policy_handle )
    ek_op_handle_t  auth_policy_handle;
#else
  (
    ek_op_handle_t  auth_policy_handle
  )
#endif
{
    ek_pname_buf_t  field_buff;
    long int        field_len;
    boolean32       io_map;
    int             description_width = 42;
    error_status_t  st;

#   define           policy_indent "    "   /* leading blanks for formatting */

    printf("  Authorization Policy:\n");

    ek_read_field(auth_policy_handle, ekn_auth_policy_max_ticket_lifetime,
                    field_buff, sizeof(field_buff), &field_len);
    printf ("%s%-*s", policy_indent, description_width, "Max certificate lifetime:");
    printf ("%s\n", tty_get_if_io_val(field_buff, &tty_lifespan_conv_table,
				      &io_map));

    ek_read_field(auth_policy_handle, ekn_auth_policy_max_renewable_lifetime,
                    field_buff, sizeof(field_buff), &field_len);
    printf ("%s%-*s", policy_indent, description_width, "Max renewable lifetime:");
    printf ("%s\n", tty_get_if_io_val(field_buff, &tty_lifespan_conv_table,
				      &io_map));
}


PUBLIC void tty_display_prop
#ifndef __STDC__
    ( prop_handle )
    ek_op_handle_t  prop_handle;
#else
  (
    ek_op_handle_t  prop_handle
  )
#endif
{
    sec_rgy_name_t  field_buff;
    long int        field_len;
    char            not_str[5];    /* output buffers - contains "" or "NOT" */

#   define          prop_indent "    "    /* leading blanks for formatting */
#   define          description_width  20
    boolean32       io_map;

    printf("  Properties:\n");

    ek_read_field(prop_handle, ekn_prop_realm, (char *)field_buff,
                   sizeof(field_buff), &field_len);
    printf("%sProperties for Registry at:               %s\n", policy_indent,
            field_buff);

    *not_str = '\0';
    ek_read_field(prop_handle, ekn_prop_is_readonly, (char *)field_buff,
                   sizeof(field_buff), &field_len);
    if (!CHECK_FLAG((char *)field_buff, field_len))
        strcpy(not_str, "NOT ");
    printf ("%s%s%s%s\n", policy_indent, "Registry is ", not_str,
            "read-only");

    *not_str = '\0';
    ek_read_field(prop_handle, ekn_prop_auth_certificates_unbounded,
                    (char *)field_buff, sizeof(field_buff), &field_len);
    if (CHECK_FLAG((char *)field_buff, field_len))
        printf ("%s%s\n", policy_indent,
                "Certificates to this server may be generated at any site.");
    else
        printf ("%s%s\n", policy_indent,
      "Certificates to this server must have been generated at a valid site.");

    *not_str = '\0';
    ek_read_field(prop_handle, ekn_prop_shadow_passwd, (char *) field_buff,
                   sizeof(field_buff), &field_len);
    if (!CHECK_FLAG((char *)field_buff, field_len))
        strcpy(not_str, "NOT ");
    printf ("%s%s%s%s\n", policy_indent, "Encrypted passwords are ", not_str,
            "hidden");

    *not_str = '\0';
    ek_read_field(prop_handle, ekn_prop_embedded_unix_id, (char *) field_buff,
                   sizeof(field_buff), &field_len);
    if (!CHECK_FLAG((char *)field_buff, field_len))
        strcpy(not_str, "NOT ");
    printf ("%s%s%s%s\n", policy_indent, "Unix IDs ARE ", not_str,
            "embedded in PGO UUIDs");

    ek_read_field(prop_handle, ekn_prop_low_unix_id_person, (char *)field_buff,
                   sizeof(field_buff), &field_len);
    printf("%sLow UID for principal creation:           %s\n", policy_indent,
            field_buff);
    ek_read_field(prop_handle, ekn_prop_low_unix_id_group, (char *) field_buff,
                   sizeof(field_buff), &field_len);
    printf("%sLow UID for group creation:               %s\n", policy_indent,
            field_buff);
    ek_read_field(prop_handle, ekn_prop_low_unix_id_org, (char *) field_buff,
                   sizeof(field_buff), &field_len);
    printf("%sLow UID for org creation:                 %s\n", policy_indent,
            field_buff);
    ek_read_field(prop_handle, ekn_prop_max_unix_id, (char *) field_buff,
                   sizeof(field_buff), &field_len);
    printf("%sMaximum possible UID:                     %s\n", policy_indent,
            field_buff);

    ek_read_field(prop_handle, ekn_prop_minimum_certificate_lifetime,
                    (char *)field_buff, sizeof(field_buff), &field_len);
    printf("%sMinimum certificate lifetime              %s\n", policy_indent,
                tty_get_if_io_val((char *)field_buff, &tty_lifespan_conv_table,
				      &io_map));
    ek_read_field(prop_handle, ekn_prop_default_certificate_lifetime,
                    (char *)field_buff, sizeof(field_buff), &field_len);
    printf("%sDefault certificate lifetime              %s\n", policy_indent,
                tty_get_if_io_val((char *)field_buff, &tty_lifespan_conv_table,
				      &io_map));
}


PUBLIC void tty_display_lrgy_prop
#ifndef __STDC__
    ( prop_handle )
    ek_op_handle_t  prop_handle;
#else
  (
    ek_op_handle_t  prop_handle
  )
#endif
{
    sec_rgy_name_t  field_buff;
    long int        field_len;
    char            not_str[5];    /* output buffers - contains "" or "NOT" */

#   define          prop_indent "    "    /* leading blanks for formatting */
#   define          description_width  20
    boolean32       io_map;

    printf("  Local Registry Properties:\n");

    ek_read_field(prop_handle, ekn_prop_lrgy_max_entries, (char *)field_buff,
                    sizeof(field_buff), &field_len);
    printf("%sCapacity:   %s entries\n", policy_indent, field_buff);
    ek_read_field(prop_handle, ekn_prop_lrgy_num_entries, (char *)field_buff,
                    sizeof(field_buff), &field_len);
    printf("%sContains:   %s account entries\n", policy_indent, field_buff);
    ek_read_field(prop_handle, ekn_prop_lrgy_num_tgt_entries,
		    (char *) field_buff,
                    sizeof(field_buff), &field_len);
    printf("%sContains:   %s tgt entries\n", policy_indent, field_buff);
    ek_read_field(prop_handle, ekn_prop_lrgy_lifespan, (char *) field_buff,
                    sizeof(field_buff), &field_len);
    printf("%sLifespan:   %s\n", policy_indent,
            tty_get_if_io_val((char *) field_buff, &tty_lifespan_conv_table, &io_map));
}


PUBLIC boolean32 tty_get_policy_args
#ifndef __STDC__
    ( op_handle, prompts, stp )
     ek_op_handle_t op_handle;
     char           **prompts;
     error_status_t *stp;
#else
  (
     ek_op_handle_t op_handle,
     char           **prompts,
     error_status_t *stp
  )
#endif
{
    boolean32            one_field_written = false,
                         input;
    ek_pname_buf_t      io_buff;

    SET_STATUS(stp, error_status_ok);

   *stp = tty_get_field_arg(op_handle, ekn_policy_acct_life,
			    &tty_lifespan_conv_table, false, (char *) NULL,
			    io_buff, sizeof(io_buff),
			    prompts[tty_policy_acct_lifespan_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
	(void) eif_write_policy_field(op_handle, ekn_policy_acct_life, io_buff,
				       false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;

   *stp = tty_get_field_arg(op_handle, ekn_policy_passwd_min_len,
			    &tty_null_conv_table, false, (char *) NULL, io_buff,
			    sizeof(io_buff),
			    prompts[tty_policy_passwd_min_len_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
	(void) eif_write_policy_field(op_handle, ekn_policy_passwd_min_len,
				      io_buff, false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;

   *stp = tty_get_field_arg(op_handle, ekn_policy_passwd_life,
			    &tty_lifespan_conv_table, false, (char *) NULL,
			    io_buff, sizeof(io_buff),
			    prompts[tty_policy_passwd_lifespan_prompt],
			    &input);
    if (GOOD_STATUS(stp) && input) {
	(void) eif_write_policy_field(op_handle, ekn_policy_passwd_life, io_buff,
				       false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;


   *stp = tty_get_field_arg(op_handle, ekn_policy_passwd_exp,
			    &tty_date_conv_table, false, (char *) NULL, io_buff,
			    sizeof(io_buff),
			    prompts[tty_policy_passwd_exp_date_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
	(void) eif_write_policy_field(op_handle, ekn_policy_passwd_exp,
				       io_buff, false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;


    /* policy flags are weird because a yes answer turns the flag off, unlike
      * most other flag fields
      */
    *stp = tty_get_field_arg(op_handle, ekn_policy_passwd_no_spaces,
			     &tty_flag_yes_off_conv_table, false, (char *) NULL,
			     io_buff, sizeof(io_buff),
			     prompts[tty_policy_passwd_no_spaces_prompt],
			     &input);
    if (GOOD_STATUS(stp) && input) {
	(void) eif_write_policy_field(op_handle, ekn_policy_passwd_no_spaces,
				      io_buff, false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;

     *stp = tty_get_field_arg(op_handle, ekn_policy_passwd_non_alpha,
			     &tty_flag_yes_off_conv_table, false, (char *) NULL,
			     io_buff, sizeof(io_buff),
			     prompts[tty_policy_passwd_non_alpha_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
	(void) eif_write_policy_field(op_handle, ekn_policy_passwd_non_alpha,
				      io_buff, false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;

   return one_field_written;
}


PUBLIC boolean32 tty_get_auth_policy_args
#ifndef __STDC__
    ( op_handle, prompts, stp)
     ek_op_handle_t     op_handle;
     char               **prompts;
     error_status_t     *stp;
#else
  (
     ek_op_handle_t     op_handle,
     char               **prompts,
     error_status_t     *stp
  )
#endif
{
    boolean32            one_field_written = false,
                         input;
    ek_pname_buf_t      io_buff;

    SET_STATUS(stp, error_status_ok);

   *stp = tty_get_field_arg(op_handle, ekn_auth_policy_max_ticket_lifetime,
                            &tty_lifespan_conv_table, false, (char *) NULL,
                            io_buff, sizeof(io_buff),
                            prompts[tty_auth_policy_max_ticket_lifetime],
                            &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_auth_policy_field(op_handle,
                                           ekn_auth_policy_max_ticket_lifetime,
                                           io_buff,
			                   false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;

   *stp = tty_get_field_arg(op_handle, ekn_auth_policy_max_renewable_lifetime,
                            &tty_lifespan_conv_table, false, (char *) NULL,
                            io_buff, sizeof(io_buff),
                            prompts[tty_auth_policy_max_renewable_lifetime],
                            &input);
    if (GOOD_STATUS(stp) && input) {
	(void) eif_write_auth_policy_field(op_handle,
                                        ekn_auth_policy_max_renewable_lifetime,
                                        io_buff, false, stp);
	if (GOOD_STATUS(stp))
	    one_field_written = true;
    }

    return one_field_written;
}


INTERNAL boolean32 tty_get_prop_args
#ifndef __STDC__
    ( op_handle, prompts, stp )
     ek_op_handle_t  op_handle;
     char            **prompts;
     error_status_t  *stp;
#else
  (
     ek_op_handle_t  op_handle,
     char            **prompts,
     error_status_t  *stp
  )
#endif
{
    boolean32        one_field_written = false,   /* assume no new data */
                     input;
    ek_pname_buf_t   io_buff;

    SET_STATUS(stp, error_status_ok);

    *stp = tty_get_field_arg(op_handle, ekn_prop_is_readonly,
                             &tty_flag_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_readonly_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle, ekn_prop_is_readonly, io_buff,
                                     false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
	return one_field_written;

    *stp = tty_get_field_arg(op_handle, ekn_prop_shadow_passwd,
                             &tty_flag_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_shadow_passwd_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle, ekn_prop_shadow_passwd, io_buff,
                                     false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

    *stp = tty_get_field_arg(op_handle, ekn_prop_low_unix_id_person,
                             &tty_null_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_luid_person_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle, ekn_prop_low_unix_id_person, io_buff,
                                     false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;
    *stp = tty_get_field_arg(op_handle, ekn_prop_low_unix_id_group,
                             &tty_null_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_luid_group_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle, ekn_prop_low_unix_id_group, io_buff,
                                     false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;
    *stp = tty_get_field_arg(op_handle, ekn_prop_low_unix_id_org,
                             &tty_null_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_luid_org_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle, ekn_prop_low_unix_id_org, io_buff,
                                     false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;
    *stp = tty_get_field_arg(op_handle, ekn_prop_max_unix_id,
                             &tty_null_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_max_uid_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle, ekn_prop_max_unix_id, io_buff,
                                     false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;
    *stp = tty_get_field_arg(op_handle, ekn_prop_minimum_certificate_lifetime,
                             &tty_null_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_min_tkt_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle,
                                    ekn_prop_minimum_certificate_lifetime,
                                    io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;
    *stp = tty_get_field_arg(op_handle, ekn_prop_default_certificate_lifetime,
                             &tty_null_conv_table, false, (char *) NULL,
                             io_buff, sizeof(io_buff),
                             prompts[tty_prop_def_tkt_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_prop_field(op_handle,
                                    ekn_prop_default_certificate_lifetime,
                                    io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

    return one_field_written;
}


INTERNAL boolean32 tty_get_lrgy_prop_args
#ifndef __STDC__
    ( op_handle, prompts, stp )
     ek_op_handle_t  op_handle;
     char            **prompts;
     error_status_t  *stp;
#else
  (
     ek_op_handle_t  op_handle,
     char            **prompts,
     error_status_t  *stp
  )
#endif
{
    boolean32        one_field_written = false,   /* assume no new data */
                     input;
    ek_pname_buf_t   io_buff;

    SET_STATUS(stp, error_status_ok);

    *stp = tty_get_field_arg(op_handle, ekn_prop_lrgy_max_entries,
                                &tty_null_conv_table, false, (char *) NULL,
                                io_buff, sizeof(io_buff),
                                prompts[tty_lrgy_max_entries_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_lrgy_prop_field(op_handle, ekn_prop_lrgy_max_entries,
                                            io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

    *stp = tty_get_field_arg(op_handle, ekn_prop_lrgy_lifespan,
                                &tty_lifespan_conv_table, false, (char *) NULL,
                                io_buff, sizeof(io_buff),
                                prompts[tty_lrgy_lifespan_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_lrgy_prop_field(op_handle, ekn_prop_lrgy_lifespan,
                                            io_buff, false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

    return one_field_written;
}


/* tty_do_properties
 *
 * registry properties may be changed via comand line arguments (but
 * not registry policy).   If there are any command line  arguments,
 * we parse them in the normal fashion and perform a set properties
 * operation.  If no command line arguments are supplied we interactively
 * display and input arguments for registry properties and policy
 */
PUBLIC error_status_t tty_do_properties
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    int                  cur_arg = 1;
    tty_y_n_response_t   ans;
    boolean32            change_properties  = true,
                         opt_write          = false,
                         opt_readonly       = false,
                         do_interactive     = true;
    error_status_t       st;

    /* Don't bother unless an update site is available */
    st = eif_site_check(ek_site_update, eif_auto_rebind);
    if (BAD_STATUS(&st)) {
	tty_print_status("", st);
	SET_EXIT_STATUS(&st);
	RETURN_OR_EXIT_ON_ERROR(&st);
    }

    SET_STATUS(&st, error_status_ok);
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {

	if (match_command("-w", c->args[cur_arg], 2)) {
	    if (opt_readonly) {
		INCOMPATIBLE_OPTS("-w", "-nw", &st);
	    } else {
		opt_write = true;
	    }
	} else if (match_command("-nw", c->args[cur_arg], 3)) {
	    if (opt_write) {
		INCOMPATIBLE_OPTS("-nw", "-w", &st);
	    } else {
		opt_readonly = true;
	    }
	} else {
	    /* not a valid option in any pgo domain */
	    INVALID_OPT(c->args[cur_arg], &st);
	}

	if (GOOD_STATUS(&st)) {
	    do_interactive = false;
	}

	cur_arg++;
    }

    SET_EXIT_STATUS(&st) ;
    RETURN_OR_EXIT_ON_ERROR(&st);

    st = ek_init_op(Prop_rgy_set_h);

    /* If we can't even retrieve the property record, don't try to change it */
    if (! GOOD_STATUS(&st)) {
        change_properties = false;
        tty_print_status("Unable to read property record", st);
    }

    if (change_properties && !do_interactive) {
                                /* something was entered on the command line */

        if (opt_readonly && GOOD_STATUS(&st)) {
	    (void) eif_write_prop_field(Prop_rgy_set_h, ekn_prop_is_readonly,
				       ek_flag_on, false, &st);
	}
        if (opt_write && GOOD_STATUS(&st)) {
	    (void) eif_write_prop_field(Prop_rgy_set_h, ekn_prop_is_readonly,
					 ek_flag_off, false, &st);

	}
	
	if (GOOD_STATUS(&st)) {
	    st = eif_change_prop(Prop_rgy_set_h);
	}

	SET_AND_EXIT_ON_ERROR(&st);
	return st;
    }

    /* If we still can change the record ... */
    if (change_properties) {
        tty_display_prop(Prop_rgy_set_h);

        ans = tty_ask_y_n(Property_prompts[tty_prop_want_to_enter_prompt],
                           tty_response_no);

        if (ans == tty_response_yes) {
            change_properties = tty_get_prop_args(Prop_rgy_set_h,
                                                  Property_prompts, &st);
            if (GOOD_STATUS(&st) && change_properties)
                st = eif_change_prop(Prop_rgy_set_h);
            if (BAD_STATUS(&st))
                tty_print_status("Unable to set properties ", st);
        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


/* tty_do_policy
 *
 * if no org name is entered operate on registry policy, otherwise operate on the
 * specified org.  If no options are entered do a lookup with optional update,
 * otherwise just do an update.
 */
PUBLIC error_status_t tty_do_policy
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    int         cur_arg = 1;
    boolean32   update            = false,   /* assume it's a query */
                arg_org_name      = false,
                opt_acct_life     = false,
                opt_pwd_life      = false,
                opt_pwd_min_len   = false,
                opt_pwd_exp_date  = false,
                opt_pwd_alpha     = false,
                opt_pwd_non_alpha = false,
                opt_pwd_space     = false,
                opt_pwd_no_space  = false;
    char        *org_name,
                *acct_life,
                *pwd_life,
                *pwd_min_len,
                *pwd_exp_date,

                /* the following are for converting tty input to kernel format */
                *kernel_date,
                *kernel_life;
    error_status_t      st;
    tty_y_n_response_t  ans;

    ek_op_handle_t  op_handle;

    SET_STATUS(&st, error_status_ok);

        /* look for non-option arguments at beginning of argument list */
    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg])
           && GOOD_STATUS(&st)) {

            /* only non-option argument is the org name  */
        if (cur_arg == 1) {
            org_name = c->args[cur_arg];
            arg_org_name = true;
        } else {
            INVALID_ARG(c->args[cur_arg], &st);
        }
        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

        /* Look for options */
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {

        if (match_command("-al", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_acct_life = true;
                acct_life = c->args[cur_arg];
            }
        } else if (match_command("-pl", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_pwd_life = true;
                pwd_life = c->args[cur_arg];
            }
        } else if (match_command("-pm", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_pwd_min_len = true;
                pwd_min_len = c->args[cur_arg];
            }
        } else if (match_command("-px", c->args[cur_arg], 3)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_pwd_exp_date = true;
                pwd_exp_date = c->args[cur_arg];
            }
        } else if (match_command("-pa", c->args[cur_arg], 3)) {
            if (opt_pwd_non_alpha) {
                INCOMPATIBLE_OPTS("-pa", "-pna", &st);
            } else {
                opt_pwd_alpha = true;
            }
        } else if (match_command("-pna", c->args[cur_arg], 4)) {
            if (opt_pwd_alpha) {
                INCOMPATIBLE_OPTS("-pna", "-pa", &st);
            } else {
                opt_pwd_non_alpha = true;
            }
        } else if (match_command("-ps", c->args[cur_arg], 3)) {
            if (opt_pwd_no_space) {
                INCOMPATIBLE_OPTS("-pa", "-pns", &st);
            } else {
                opt_pwd_space = true;
            }
        } else if (match_command("-pns", c->args[cur_arg], 4)) {
            if (opt_pwd_space) {
                INCOMPATIBLE_OPTS("-pns", "-ps", &st);
            } else {
                opt_pwd_no_space = true;
            }
        } else {
            /* not a valid option in any pgo domain */
            INVALID_OPT(c->args[cur_arg], &st);
        }

        if (GOOD_STATUS(&st)) {
            update = true;
        }

        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    op_handle = Pol_set_h;

    st = ek_init_op(op_handle);

    /* If can't initialize policy, don't try anything else */
    if (! GOOD_STATUS(&st))
        return st;

    if (arg_org_name) {
        (void) eif_write_policy_field(op_handle, ekn_policy_key, org_name,
                                       false, &st);
    } else {                                 /* operate on registry policy */
        (void) eif_write_policy_field(op_handle, ekn_policy_key, "",
                                       false, &st);
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    if (update) {

        if (opt_acct_life && GOOD_STATUS(&st)) {
            kernel_life = tty_get_kernel_io_val(acct_life,
                                                &tty_lifespan_conv_table,
                                                &opt_acct_life);
            if (!opt_acct_life) {
                DATA_FORMAT_ERROR(acct_life, &st);
            } else {
                (void) eif_write_policy_field(op_handle, ekn_policy_acct_life,
                                               kernel_life, false, &st);
            }
        }
        if (opt_pwd_min_len && GOOD_STATUS(&st)) {
            (void) eif_write_policy_field(op_handle, ekn_policy_passwd_min_len,
                                           pwd_min_len, false, &st);
        }
        if (opt_pwd_life && GOOD_STATUS(&st)) {
            kernel_life = tty_get_kernel_io_val(pwd_life,
                                                &tty_lifespan_conv_table,
                                                &opt_pwd_life);
            if (!opt_pwd_life) {
                DATA_FORMAT_ERROR(acct_life, &st);
            } else {
                (void) eif_write_policy_field(op_handle, ekn_policy_passwd_life,
                                               kernel_life, false, &st);
            }
        }
        if (opt_pwd_exp_date && GOOD_STATUS(&st)) {
            kernel_date = tty_get_kernel_io_val(pwd_exp_date,
                                                &tty_date_conv_table,
                                                &opt_pwd_exp_date);
            if (!opt_pwd_exp_date) {
                DATA_FORMAT_ERROR(pwd_exp_date, &st);
            } else {
                (void) eif_write_policy_field(op_handle, ekn_policy_passwd_exp,
                                               kernel_date, false, &st);
            }
        }
        if (opt_pwd_space && GOOD_STATUS(&st)) {
            (void) eif_write_policy_field(op_handle, ekn_policy_passwd_no_spaces,
                                           ek_flag_off, false, &st);
        }
        if (opt_pwd_no_space && GOOD_STATUS(&st)) {
            (void) eif_write_policy_field(op_handle, ekn_policy_passwd_no_spaces,
                                           ek_flag_on, false, &st);
        }
        if (opt_pwd_alpha && GOOD_STATUS(&st)) {
            (void) eif_write_policy_field(op_handle, ekn_policy_passwd_non_alpha,
                                          ek_flag_off, false, &st);
        }
        if (opt_pwd_non_alpha && GOOD_STATUS(&st)) {
            (void) eif_write_policy_field(op_handle, ekn_policy_passwd_non_alpha,
                                           ek_flag_on, false, &st);
        }

        st = eif_change_policy(op_handle);

    } else {       /* it's a lookup */

        if (STATUS_EQUAL(&st, ek_misc_object_not_found))
            printf ("  Policy:  <none>\n");
        else if (! GOOD_STATUS(&st)) {
            tty_print_status("Unable to get policy info", st);
        } else {
            printf("  Policy:\n");
            tty_display_policy(op_handle);    /* display org policy */

            if (arg_org_name) {
                /* get the effective policy info */
                op_handle = Pol_get_effective_h;
                st = ek_init_op(op_handle);
                if (GOOD_STATUS(&st))
                    (void) eif_write_policy_field(op_handle, ekn_policy_key,
                                                   org_name,  false, &st);

                if (GOOD_STATUS(&st)) {
                    st = ek_do_op(op_handle);
                    if (BAD_STATUS(&st)) {
                        tty_print_status("Unable to get effective policy info",
                                         st);
                    } else {
                        printf("  Effective Policy:\n");
                        tty_display_policy(op_handle);
                    }
                }
            }
                /* Change the policy ? */
            ans = tty_ask_y_n(Property_prompts[tty_prop_want_to_enter_prompt],
                                tty_response_no);
            if (ans == tty_response_yes) {
                update = tty_get_policy_args(Pol_set_h,
                                                    Policy_prompts, &st);
                if (GOOD_STATUS(&st) && update)
                    st = eif_change_policy(Pol_set_h);
            }

        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


/* tty_do_lproperties
 *
 */
PUBLIC error_status_t tty_do_lproperties
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    boolean32           change_properties  = true,
                        opt_write          = false,
                        do_interactive     = true;
    tty_y_n_response_t  ans;
    error_status_t      st;

    CLEAR_STATUS(&st);

    st = ek_init_op(Prop_lrgy_set_h);

    /* If we can't even retrieve the property record, don't try to change it */
    if (! GOOD_STATUS(&st)) {
        change_properties = false;
        tty_print_status("Unable to read local registry property record", st);
    }

    if (change_properties) {
        tty_display_lrgy_prop(Prop_lrgy_set_h);

        ans = tty_ask_y_n(Property_prompts[tty_prop_want_to_enter_prompt],
                           tty_response_no);

        if (ans == tty_response_yes) {
            change_properties = tty_get_lrgy_prop_args(Prop_lrgy_set_h,
                                                        Lrgy_property_prompts,
                                                        &st);
            if (GOOD_STATUS(&st) && change_properties) {
                st = ek_do_op(Prop_lrgy_set_h);
            }
            if (BAD_STATUS(&st))
                tty_print_status("Unable to set local registry properties ", st);
        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


/* tty_do_purge
 *
 *  Purge expired local registry entries.
 */
PUBLIC error_status_t tty_do_purge
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    unsigned32      num_purged;
    unsigned32      num_tgts_purged;
    error_status_t  st;

    ek_do_lrgy_purge(&num_purged, &num_tgts_purged, &st);
    if (BAD_STATUS(&st)) {
        tty_print_status("Unable to purge expired local registry entries", st);
    } else {
        printf("\n%d account entries purged\n", num_purged);
        printf("%d TGT entries purged\n", num_tgts_purged);
    }

    return st;
}


/* tty_do_auth_policy
 *
 * Display/Modify the authorization policy
 */
PUBLIC error_status_t tty_do_auth_policy
#ifndef __STDC__
    ( c )
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    int                  cur_arg = 1;
    tty_y_n_response_t   ans;
    boolean32            change_auth_policy = true,
                         do_interactive     = true;
    error_status_t       st;

    /* Don't bother unless an update site is available */
    st = eif_site_check(ek_site_update, eif_auto_rebind);
    if (BAD_STATUS(&st)) {
	tty_print_status("", st);
	SET_EXIT_STATUS(&st);
	RETURN_OR_EXIT_ON_ERROR(&st);
    }

    SET_STATUS(&st, error_status_ok);
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {

	if (GOOD_STATUS(&st)) {
	    do_interactive = false;
	}

	cur_arg++;
    }

    SET_EXIT_STATUS(&st) ;
    RETURN_OR_EXIT_ON_ERROR(&st);

    eif_write_auth_policy_field
                    (Auth_pol_set_h, ekn_auth_policy_pname, "", false, &st);
    eif_write_auth_policy_field
                    (Auth_pol_set_h, ekn_auth_policy_gname, "", false, &st);
    eif_write_auth_policy_field
                    (Auth_pol_set_h, ekn_auth_policy_oname, "", false, &st);

    st = ek_init_op(Auth_pol_set_h);

    if (! GOOD_STATUS(&st)) {
        change_auth_policy = false;
        tty_print_status("Unable to read auth policy", st);
    } else {

        tty_display_auth_policy(Auth_pol_set_h);
        ans = tty_ask_y_n(Property_prompts[tty_prop_want_to_enter_prompt],
                           tty_response_no);

        if (ans == tty_response_yes) {
            change_auth_policy = tty_get_auth_policy_args(Auth_pol_set_h,
                                                Auth_policy_prompts, &st);
            if (GOOD_STATUS(&st) && change_auth_policy)
                st = eif_change_auth_policy(Auth_pol_set_h);
            if (BAD_STATUS(&st))
                tty_print_status("Unable to set auth policy", st);
        }
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}

