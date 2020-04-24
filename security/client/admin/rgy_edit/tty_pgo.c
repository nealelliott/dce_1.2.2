/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_pgo.c,v $
 * Revision 1.1.14.2  1996/03/11  13:27:22  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:14  marty]
 *
 * Revision 1.1.14.1  1995/12/08  18:02:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/10/13  15:18 UTC  psn
 * 	Fix the signature of tty_get_pgo_args() (related to DCE OT 12877)
 * 
 * 	HP revision /main/HPDCE02/psn_fix/1  1995/10/13  15:12 UTC  psn
 * 	Fix  tty_get_pgo_args()'s signature (related to OT 12877)
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  16:30:42  root]
 * 
 * Revision 1.1.8.5  1994/09/21  20:43:44  max
 * 	CR 11961 fix.  Malloc one more byte for null termination of string.
 * 	[1994/09/21  20:42:49  max]
 * 
 * Revision 1.1.8.4  1994/09/09  19:12:09  mob
 * 	Remove temporary ERA support
 * 	[1994/09/09  19:05:16  mob]
 * 
 * Revision 1.1.8.3  1994/08/08  14:23:27  mdf
 * 	Put back 64 bit porting changes
 * 	[1994/08/08  12:45:53  mdf]
 * 
 * Revision 1.1.8.2  1994/03/09  19:41:29  mob
 * 	Fix for CR 10084
 * 	[1994/03/08  19:59:11  mob]
 * 
 * Revision 1.1.8.1  1994/03/02  17:54:32  hanfei
 * 	Initial ERA Drop
 * 
 * 	Initial ERA Drop
 * 
 * 	Initial King Kong branch
 * 	[1993/07/08  10:54:31  root]
 * 
 * Revision 1.1.5.2  1993/05/07  14:35:08  burati
 * 	CR7823 Fix domain checking on -alias of pgo add
 * 	[1993/05/07  14:33:06  burati]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:42  zeliff]
 * 
 * 	CR6400 Fix sizeof(iobuff) for exp date in cell command
 * 	[1992/12/11  15:29:20  burati]
 * 
 * 	CR3305 Enhancement to allow group aliases
 * 	[1992/11/28  22:26:51  burati]
 * 
 * 	CR5732 Must specify -u for view by unix number
 * 	CR5704 Don't allow -q unless in principal domain
 * 	[1992/10/23  19:50:44  burati]
 * 
 * 	CR5068 Fix missing -q option on change command
 * 	CR4529 Remove extra/misleading error msg on adopt failure
 * 	[1992/09/24  22:08:27  burati]
 * 
 * Revision 1.1.1.11  1993/05/04  20:23:58  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.2.4  1992/05/08  18:21:29  burati
 * 	Don't restrict usernames to <256 chars, use sec_rgy_name_t
 * 	[1992/05/07  23:20:36  burati]
 * 
 * Revision 1.1.2.3  1992/04/03  20:46:15  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  21:01:51  burati]
 * 
 * Revision 1.1.2.2  1992/03/17  20:10:08  burati
 * 	Fixed fullname arg checking in tty_do_add_cell for OT2035
 * 	[1992/03/17  15:20:00  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  tty_pgo.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface pgo processing
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_pgo.c	28 - 02/24/92";
#endif

#include <pthread.h>

#include <stdio.h>
#include <tty_base.h>
#include <un_strng.h>
#include <un_maloc.h>
#include <macros.h>

#include <com_err.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>   /* for krb5_read_password */

#include <ek_error.h>
#include <ek_types.h>
#include <ek_util.h>

#include <eif_def.h>
#include <eif_pgo.h>
#include <eif_prop.h>
#include <eif_util.h>
#include <eif_site.h>

#include <tty_util.h>
#include <tty_io.h>
#include <tty_err.h>
#include <tty_conf.h>
#include <tty_pgo.h>
#include <tty_prop.h>

#include <args.h>

PUBLIC void tty_pgo_display
#ifndef __STDC__
    ( op_handle, show_full, show_members, show_policy, stp )
    ek_op_handle_t  op_handle;
    boolean32       show_full;
    boolean32       show_members;
    boolean32       show_policy;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    boolean32       show_full,
    boolean32       show_members,
    boolean32       show_policy,
    error_status_t  *stp
  )
#endif
{
    unsigned long   output_buff_len;
    char            *msg_ptr;    /* for retrieving formatted messages from
                                  * output_buff
                                  */
    sec_rgy_name_t  field_buff;  /* big enough for anything the kernel might
                                  * return
                                  */
    long int        field_len;
    ek_op_handle_t  policy_handle = Pol_get_h;
    ek_op_handle_t  member_handle;  /* handle for membership queries */
    eif_domain_t    domain;         /* which pgo domain are we in? */
    ek_op_id_t      which_op;

#   define      max_pgo_output sec_rgy_name_max_len
#   define      member_indent  "    "

    sec_rgy_name_t      pgo_name;
    long int            pgo_name_len;
    ek_unix_num_buf_t   unix_num;
    ek_quota_buf_t      quota;
    ek_fullname_buf_t   fullname;
    ek_uuid_buf_t       uid;
    ek_flag_buf_t       flag;
    ek_name_buf_t       member_name;

    int                 name_field_width,
                        member_output;

    char                primary[3],
                        reserved[3],
                        user_mod[3],
                        projlist[3];   /* output buffers for converted flags */

    SET_STATUS(stp, error_status_ok);

        /* the operation id in the handle tells us what domain we're in */
    which_op = ek_get_op_id(op_handle);

    switch (which_op) {
    case ekn_person_get_by_name:
    case ekn_person_get_by_num:
        domain = eif_domain_person;
        break;
    case ekn_group_get_by_name:
    case ekn_group_get_by_num:
        domain = eif_domain_group;
        break;
    case ekn_org_get_by_name:
    case ekn_org_get_by_num:
        domain = eif_domain_org;
        break;
    }

        /* show common pgo fields, in full if requested */
    ek_read_field(op_handle, ekn_pgo_name, (char *) pgo_name, sizeof(pgo_name),
                   &pgo_name_len);
    ek_read_field(op_handle, ekn_pgo_unix_num, unix_num, sizeof(unix_num),
                   &field_len);

        /* the pgo name is displayed in field width chunks of 36 characters */
    if ((pgo_name_len / 36) == 0)
        name_field_width = 36;
    else
        name_field_width = ((pgo_name_len / 36) + 1) * 36;

    printf("%-*s%12s\n", name_field_width, pgo_name, unix_num);

    if (show_full) {
        ek_read_field(op_handle, ekn_pgo_fullname, fullname,
                       sizeof(fullname), &field_len);
        ek_read_field(op_handle, ekn_pgo_uuid, uid,  sizeof(uid), &field_len);

        sprintf(primary, "%s", "pr");

        ek_read_field(op_handle, ekn_pgo_is_reserved, flag,
                       sizeof(flag), &field_len);
        if (CHECK_FLAG(flag, strlen(flag)))
            sprintf(reserved, "%s", "rq");
        else
            sprintf(reserved, "%s", "--");

        if ((domain == eif_domain_person) || (domain == eif_domain_group)) {
            ek_read_field(op_handle, ekn_pgo_is_an_alias, flag,
                           sizeof(flag), &field_len);
            if (CHECK_FLAG(flag, strlen(flag)))
                sprintf(primary, "%s", "al");
        }

            /* print the common fields in "full" output */
        if (strlen(fullname) > 0)
            printf("  Full name:  %s\n", fullname);
        printf("  Uuid:       %s\n", uid);
        printf("  Primary:%4s   Reserved: %4s\n", primary, reserved);

        if (domain == eif_domain_group) {
            ek_read_field(op_handle, ekn_pgo_projlist_ok, flag, sizeof(flag),
                           &field_len);
            if (CHECK_FLAG(flag, strlen(flag)))
                sprintf(projlist, "%s", "l");
            else
                sprintf(projlist, "%s", "nl");
            printf("  Project List:%-4s\n", projlist);
        }

        if (domain == eif_domain_person) {
            ek_read_field(op_handle, ekn_pgo_quota, quota, sizeof(quota),
                            &field_len);
            printf("  Quota: %s\n", quota);
        }
    }  /* show_full */


        /* show org policy info if requested */
    if (domain == eif_domain_org && show_policy) {
        ek_get_op_handle(&policy_handle, ekn_policy_get);
        ek_read_field(op_handle, ekn_pgo_name, (char *) pgo_name,
                        sizeof(pgo_name), &field_len);
        *stp = ek_write_field(policy_handle, ekn_policy_key, (char *) pgo_name,
                               strlen((char *)pgo_name));
        if (GOOD_STATUS(stp)) {
            *stp = ek_do_op(policy_handle);
        }

        if (STATUS_EQUAL(stp, ek_misc_object_not_found))
            printf ("  Policy:  <none>\n");
        else if (BAD_STATUS(stp)) {
            tty_print_status("Unable to get policy info", *stp);
        } else {
            printf("  Policy:\n");
            tty_display_policy(policy_handle);    /* display org policy */
        }
        ek_free_op_handle(&policy_handle);

        if (GOOD_STATUS(stp)) {
            /* get the effective policy info */
            ek_get_op_handle(&policy_handle, ekn_policy_get_effective);
            *stp = ek_write_field(policy_handle, ekn_policy_key,
                                   (char *)pgo_name, strlen((char *)pgo_name));
            if (GOOD_STATUS(stp)) {
                *stp = ek_do_op(policy_handle);
            }
            if (BAD_STATUS(stp)) {
                tty_print_status("Unable to get effective policy info", *stp);
            } else {
                printf("  Effective Policy:\n");
                tty_display_policy(policy_handle);
            }
            ek_free_op_handle(&policy_handle);
        }
    }

    if (BAD_STATUS(stp)) {
        return;
    }

        /* show membership info if requested */
    if (show_members) {
        switch (domain) {
        case eif_domain_person:
            member_handle = M_get_projlist_h;
            break;
        case eif_domain_group:
            member_handle = M_get_group_members_h;
            break;
        case eif_domain_org:
            member_handle = M_get_org_members_h;
            break;
        }

        *stp = ek_init_op(member_handle);

        /* make the first membership call to find out how many members there
         * are all together
         */
        *stp = ek_write_field(member_handle, ekn_mem_key, (char *) pgo_name,
                              strlen((char *) pgo_name));
        *stp = ek_do_op(member_handle);

        /* if there aren't any members just print out the message appropriate
         * to the domain and return
         */
        if (STATUS_EQUAL(stp, ek_misc_no_members)) {
            switch (domain) {
            case eif_domain_person:
                printf ("  Is not a member of any group\n");
                break;
            case eif_domain_group:
            case eif_domain_org:
                printf ("  No members\n");
                break;
            }
            SET_STATUS(stp, error_status_ok);   /* and everything makes sense */
            return;
        } else if (BAD_STATUS(stp)) {
                /* Anything but ek_no_members makes us bail out */
            tty_print_status("Unable to get membership info", *stp);
            return;
        }

        /* if we got here then there's some membership names to be displayed
         * (and we've already got the first one)
         */
        ek_read_field(member_handle, ekn_mem_num_members, (char *) field_buff,
                       sizeof(field_buff), &field_len);
        switch (domain) {
        case eif_domain_person:
            printf ("  Member of %s groups:\n", field_buff);
            break;
        case eif_domain_group:
        case eif_domain_org:
            printf ("  %s members\n", field_buff);
            break;
        }

        member_output = 0;
        printf ("%s", member_indent);
        while (GOOD_STATUS(stp)) {
            ek_read_field(member_handle, ekn_mem_name, member_name,
                           sizeof(member_name), &field_len);
                /* if printing this member would push  beyond 80 characters then
                   start a new line */
            if (member_output + field_len + strlen(", ") > 79) {
                printf("\n%s", member_indent);
                member_output = 0;
            }
            if (member_output != 0)
                printf("%s", ", ");
            member_output = member_output + field_len + strlen(", ");
            printf("%s", member_name);

            *stp = ek_do_op(member_handle);   /* get the next member */
        }

        if (member_output != 0)
            printf("\n");

            /* report any status but ek_no_more_entries */
        if (STATUS_EQUAL(stp, ek_misc_no_more_entries)) {
            SET_STATUS(stp, error_status_ok);
        } else  if (BAD_STATUS(stp)) {
            tty_print_status("Unable to get membership info", *stp);
        }
    }
}


/* tty_get_pgo_args
 *
 * Interactively inputs values for the non-key fields of a pgo update handle.
 * Returns false if any of the newly entered data is bad, or no data was changed.
 * only prompts for a new name value on a change.
 */
PRIVATE boolean32 tty_get_pgo_args
#ifndef __STDC__
    (op_handle, domain, show_defaults, reserved, is_alias, prompts, stp)
    ek_op_handle_t  op_handle;
    eif_domain_t    domain;
    boolean32       show_defaults;
    boolean32       reserved;
    boolean32       is_alias;
    char            **prompts;
    error_status_t  *stp;
#else
  (
    ek_op_handle_t  op_handle,
    eif_domain_t    domain,
    boolean32       show_defaults,
    boolean32       reserved,
    boolean32       is_alias,
    char            **prompts,
    error_status_t  *stp
  )
#endif
{
    error_status_t  st;
    boolean32       one_field_written = false,   /* assume no new data entered */
                    input,
                    io_map,
                    use_def_pwd,
                    is_change,
                    is_add;
    char            *add_def;
    sec_rgy_name_t  io_buff,
                    def_val;
    long            def_val_len;
    ek_op_id_t      op_id;

    op_id = ek_get_op_id(op_handle);
    is_change = ( op_id == ekn_person_change ||
                  op_id == ekn_group_change ||
                  op_id == ekn_org_change);
    is_add = ( op_id == ekn_person_add ||
               op_id == ekn_group_add ||
               op_id == ekn_org_add); 

    /* 
     * If this is a change operation and we're showing defaults, then prompt
     * for a new pgo name using the acct id in the key field as the default 
     */

    if (show_defaults && is_change) {
        ek_read_field(op_handle, ekn_pgo_key, (char *)def_val, sizeof(def_val),
                       &def_val_len);
        if (reserved) {
            tty_show_prompt(prompts[tty_pgo_new_name_prompt]);
            tty_show_default((char *)def_val);
            tty_end_prompt(tty_cannot_be_changed);
        }  else {
            *stp = tty_get_field_arg(op_handle, ekn_pgo_name,
                                     &tty_null_conv_table, show_defaults,
                                     (char *)def_val, (char *)io_buff,
				     sizeof(io_buff),
                                     prompts[tty_pgo_new_name_prompt], &input);
            if (GOOD_STATUS(stp) && input) {
                (void) eif_write_pgo_field(op_handle, ekn_pgo_name,
					   (char *) io_buff,
                                           domain, false, stp);
                if (GOOD_STATUS(stp)) {
                    one_field_written = true;
                }
            }
            if (BAD_STATUS(stp))
                return one_field_written;
        }
    }


    /*
     * unix number - can't change unix numbers
     */
    add_def = eif_get_default(eif_def_pgo_uid);
    if (is_change) {
        ek_read_field(op_handle, ekn_pgo_unix_num, (char *) def_val,
                        sizeof(def_val), &def_val_len);
        tty_show_prompt(prompts[tty_pgo_unix_num_prompt]);
        tty_show_default((char *)def_val);
        tty_end_prompt(tty_cannot_be_changed);
    } else {
        *stp = tty_get_field_arg(op_handle, ekn_pgo_unix_num,
                                 &tty_uid_conv_table,
                                 (show_defaults && !is_alias), (char *)add_def,
                                 (char *)io_buff, sizeof(io_buff),
                                 prompts[tty_pgo_unix_num_prompt], &input);
        if (GOOD_STATUS(stp)) {
            if (input) {
                (void) eif_write_pgo_field(op_handle, ekn_pgo_unix_num,
                                           (char *)io_buff, domain, false,stp);
                if (GOOD_STATUS(stp))
                    one_field_written = true;
            }else if (is_add && !ek_field_written(op_handle,ekn_pgo_unix_num)){
                    /* unix num is required on add */
                SET_STATUS(stp, tty_entry_is_required);
                tty_print_status(tty_unix_number_missing, *stp);
            }
        }
        if (BAD_STATUS(stp))
            return one_field_written;
    }

       /* get the new fullname info */
    add_def = eif_get_default(eif_def_pgo_fullname);
    *stp = tty_get_field_arg(op_handle, ekn_pgo_fullname, &tty_null_conv_table,
                             show_defaults, (char *) add_def, (char *)io_buff,
			     sizeof(io_buff),
                             prompts[tty_pgo_fullname_prompt], &input);
    if (GOOD_STATUS(stp) && input) {
        (void) eif_write_pgo_field(op_handle, ekn_pgo_fullname,
				    (char *) io_buff, domain,
                                    false, stp);
        if (GOOD_STATUS(stp))
            one_field_written = true;
    }
    if (BAD_STATUS(stp))
        return one_field_written;

       /* get the new quota info - persons only*/
    if (domain == eif_domain_person) {
        add_def = eif_get_default(eif_def_pgo_quota);
        *stp = tty_get_field_arg(op_handle, ekn_pgo_quota,
				 &tty_null_conv_table,
                                 show_defaults, (char *) add_def,
				 (char *) io_buff, sizeof(io_buff),
                                 prompts[tty_pgo_quota_prompt], &input);
        if (GOOD_STATUS(stp) && input) {
            (void) eif_write_pgo_field(op_handle, ekn_pgo_quota,
				        (char *) io_buff, domain,
                                        false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
        }
        if (BAD_STATUS(stp))
            return one_field_written;
    }

        /* projlist flag - groups only */
    if (domain == eif_domain_group) {
        add_def = eif_get_default(eif_def_g_projlist_ok);
        *stp = tty_get_field_arg(op_handle, ekn_pgo_projlist_ok,
                                 &tty_flag_conv_table, show_defaults,
				 (char *) add_def,
                                 (char *) io_buff, sizeof(io_buff),
                                 prompts[tty_pgo_projlist_prompt],  &input);
        if (GOOD_STATUS(stp) && input) {
            (void) eif_write_pgo_field(op_handle, ekn_pgo_projlist_ok,
				       (char *) io_buff,
                                       domain, false, stp);
            if (GOOD_STATUS(stp))
                one_field_written = true;
        }
        if (BAD_STATUS(stp))
            return one_field_written;
    }

    return one_field_written;
}

PUBLIC error_status_t tty_do_add_pgo
#ifndef __STDC__
    ( c, domain )
    command_buf     *c;
    eif_domain_t    domain;
#else
  (
    command_buf     *c,
    eif_domain_t    domain
  )
#endif
{
    char            *name,
                    *number,
                    *fullname,
                    *quota_str = "unlimited",
                    *sub_prompt;

    boolean32       arg_name        = false,
                    arg_number      = false,
                    arg_fullname    = false,
                    opt_alias       = false,  /* persons,groups only */
                    opt_nolist      = false,  /* groups only */
                    opt_aa          = false,
                    arg_encoding    = false,
                    arg_value       = false, 
                    input           = false,
                    retrying        = false,
                    pgo_added       = false,
                    pgo_written     = false,
                    do_interactive  = true;

    int                  cur_arg = 0,
                         quota = 0;
    char                 *add_def;
    sec_rgy_name_t       pgo_key;
    sec_rgy_name_t       new_pgo_name;
    ek_pname_buf_t       num_buff;
    tty_error_buf_t      error_msg;
    ek_op_handle_t       add_handle; 
    error_status_t       st;
    tty_y_n_response_t   ans;

        /* look for non-option arguments at beginning of argument list */
    SET_STATUS(&st, error_status_ok);
    cur_arg = 1;
    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg])
           && GOOD_STATUS(&st)) {

            /* first non-option argument is the name */
        if (cur_arg == 1) {
            name = c->args[cur_arg];
            arg_name = true;

            /* second non-option argument is the unix number */
        } else if (cur_arg == 2) {
            number = c->args[cur_arg];
            arg_number = true;

        } else {
            INVALID_ARG(c->args[cur_arg], &st);
        }

        if (GOOD_STATUS(&st)) {
            do_interactive = false;
        }

        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

        /* Look for options */
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {
        if (match_command("-al", c->args[cur_arg], 3)) {
            if ((domain == eif_domain_person) || (domain == eif_domain_group)) {
                opt_alias = true;
            } else {
                    /* -al valid only in the person domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }

        } else if (match_command("-nl", c->args[cur_arg], 3)) {
            if (domain == eif_domain_group) {
                opt_nolist = true;
                do_interactive = false;
            } else {
                    /* -nl valid only in the group domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }

            /* quota */
        } else if (match_command("-q", c->args[cur_arg], 2)) {
            if (domain == eif_domain_person) {
                if ((cur_arg+1) < c->num_args) {
                    cur_arg++;
                    if (eku_chk_integer32(c->args[cur_arg],
                        strlen(c->args[cur_arg]),
                        &quota) == error_status_ok) {
                                                
                        quota_str = c->args[cur_arg];
                        do_interactive = false;
                    } else {
                        INVALID_ARG(c->args[cur_arg], &st);
                    }
                } else {
                    OPT_REQUIRES_ARG(c->args[cur_arg], &st);
                }
            } else {
                    /* -q valid only in the principal domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }

            /* fullname */
        } else if (match_command("-f", c->args[cur_arg], 2)) {
            if ((cur_arg+1) < c->num_args) {
                cur_arg++;
                fullname = c->args[cur_arg];
                arg_fullname = true;
                do_interactive = false;
            } else {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            }
	}
           
        cur_arg++;
    }

       /* shouldn't be any arguments or options left */
    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    switch (Current_domain) {
        case eif_domain_person:
            add_handle = P_add_h;
            sub_prompt =  Pgo_add_prompts[tty_person_name_prompt];
            break;
        case eif_domain_group:
            add_handle = G_add_h;
            sub_prompt =  Pgo_add_prompts[tty_group_name_prompt];
            break;
         case eif_domain_org:
            add_handle = O_add_h;
            sub_prompt =  Pgo_add_prompts[tty_org_name_prompt];
            break;
    }

    st = ek_init_op(add_handle);

    if (!arg_name) {   /* get key interactively */
        tty_show_prompt(sub_prompt);
        input = tty_get_arg((char *)pgo_key, sizeof(pgo_key), &st);
        if (!input) {
            return st;
        }
    } else {
            /* copy command line arg to key buffer */
        strncpy((char *) pgo_key, name, sec_rgy_name_max_len);
        pgo_key[sec_rgy_name_max_len] = '\0';
    }

    if (GOOD_STATUS(&st)) {
        arg_name = eif_write_pgo_field(add_handle, ekn_pgo_key,
				        (char *) pgo_key,
                                        domain, false, &st);
    } else {
        arg_name = false;  /* set up for key prompt in loop */
    }

    /* Safety net.  If no UnixNum was specified, an arg that caused
     * do_interactive to be set false, but -al (alias) was specified
     * then reset do_interactive because it doesn't make sense to
     * auto-generate an alias UnixNumber
     */
    if (!do_interactive && !arg_number && opt_alias) {
        do_interactive = true;
    }

    if (!do_interactive) {
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    } else {
        EXIT_ON_ERROR(&st);
    }

    if (do_interactive) {

        do {
            /* Just exit on interrupt for now.  Use try/catch later */
            pthread_testcancel();

            if (!arg_name) {
                st = tty_get_field_arg(add_handle, ekn_pgo_key,
                                       &tty_null_conv_table,
                                       false, (char *) NULL, (char *) pgo_key,
                                       sizeof(pgo_key),
                                       sub_prompt, &input);
                if ((!input) && (!retrying)) {
                    return st;   /* bail out on null key input */
                }
                CONTINUE_OR_EXIT_ON_ERROR(&st);

                if (input) {
                    arg_name = eif_write_pgo_field(add_handle, ekn_pgo_key,
                                                    (char *) pgo_key, domain,
                                                    false, &st);
                }

                if (!GOOD_STATUS(&st)) {
                    if (tty_ask_y_n("Do you wish to try again ",
                            tty_response_no) == tty_response_yes) {
                        st = ek_init_op(add_handle);
                        continue;
                    } else {
                        CLEAR_STATUS(&st);
                        return st;
                    }
                }

                CONTINUE_OR_EXIT_ON_ERROR(&st);
            }

            (void) tty_get_pgo_args(add_handle, domain, true, false, 
                                    opt_alias, Pgo_add_prompts, &st);

            CONTINUE_OR_EXIT_ON_ERROR(&st);

            if (opt_alias && GOOD_STATUS(&st)) {
                (void) eif_write_pgo_field(add_handle, ekn_pgo_is_an_alias,
                                            ek_flag_on, domain, false, &st);
            }

            CONTINUE_OR_EXIT_ON_ERROR(&st);

               /* supply defaults for any fields that can only be gotten from a
                * a command line switch.   If this fails, there is no way the
                * user can correct it in interactive mode, so we just bail out
                * of the loop
                */
            st = eif_apply_add_defaults(add_handle, domain);
            if (BAD_STATUS(&st))
                break;

            st = eif_add_pgo(add_handle, domain);
            if (! GOOD_STATUS(&st)) {
                if (tty_ask_y_n("Do you wish to try again ",
                    tty_response_no) == tty_response_yes) {
                    arg_name = false;
                    retrying = true;
                    continue;
                } else {
                    CLEAR_STATUS(&st);
                    return st;
                }
            } 

            st = ek_init_op(add_handle);
            arg_name = false;
            retrying = false;

        } while (true);

        SET_AND_EXIT_ON_ERROR(&st);
        return st;
    }

    /* If we get here, then the user specified some command line arguments */

    /* the unix number is required  */
    if (arg_number) {
        (void) eif_write_pgo_field(add_handle, ekn_pgo_unix_num, number,
                                    domain, false, &st);
    } else {
        /* some cl args specified but not unix_num, default to auto-generate */
        (void) eif_write_pgo_field(add_handle, ekn_pgo_unix_num,
                                    ek_pgo_unknown_uid, domain, false, &st);
    }

    if (arg_fullname && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(add_handle, ekn_pgo_fullname, fullname,
                                    domain, false, &st);
    }

    if (GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(add_handle, ekn_pgo_quota, quota_str,
                                    domain, false, &st);
    }

    if (opt_alias && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(add_handle, ekn_pgo_is_an_alias,
                                    ek_flag_on, domain, false, &st);
    }

    if (opt_nolist && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(add_handle, ekn_pgo_projlist_ok,
                                    ek_flag_off, domain, false, &st);
    }

    if (GOOD_STATUS(&st)) {
        st = eif_apply_add_defaults(add_handle, domain);
    }

    if (GOOD_STATUS(&st)) {
        st = eif_add_pgo(add_handle, domain);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_change_pgo
#ifndef __STDC__
    ( c, domain )
    command_buf     *c;
    eif_domain_t    domain;
#else
  (
    command_buf     *c,
    eif_domain_t    domain
  )
#endif
{
    char             *name,
                     *new_name,
                     *fullname,
                     *quota_str,
                     *sub_prompt;

    boolean32        arg_name        = false,
                     opt_new_name    = false,
                     opt_fullname    = false,
                     opt_alias       = false,  /* persons,groups only */
                     opt_primary     = false,  /* persons only */
                     opt_quota       = false,  /* persons only */
                     opt_list        = false,  /* groups only */
                     opt_nolist      = false,  /* groups only */
                     opt_aa          = false,
                     arg_encoding    = false,
                     arg_value       = false,
                     do_interactive  = true,
                     pgo_changed     = false,
                     reserved,          /* is this a reserved entry change? */
                     change_one,
                     input,
                     change_policy;

    int              cur_arg;
    signed32	     quota;
    tty_error_buf_t  error_msg;
    sec_rgy_name_t   pgo_key,
                     policy_key;
    sec_rgy_name_t   new_pgo_name;
    long             policy_key_len;  
    ek_flag_buf_t        reserved_flag;
    long                 flag_len;
    tty_y_n_response_t   ans;
    ek_op_handle_t       policy_handle = Pol_set_h,
                         chg_handle;
    error_status_t       st;


        /* look for non-option arguments at beginning of argument list */
    SET_STATUS(&st, error_status_ok);
    cur_arg = 1;
    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg]) &&
           GOOD_STATUS(&st)) {

            /* only non-option argument is the name */
        if (cur_arg == 1) {
            name = c->args[cur_arg];
            arg_name = true;
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
            if ((domain == eif_domain_person) || (domain = eif_domain_group)) {
                if (opt_primary) {
                    INCOMPATIBLE_OPTS("-al", "-pr", &st);
                } else {
                    opt_alias = true;
                    pgo_changed = true;
                }
            } else {
                /* -al valid only in the person or group domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }
        } else if (match_command("-pr", c->args[cur_arg], 3)) {
            if ((domain == eif_domain_person) || (domain == eif_domain_group))
                if (opt_alias) {
                    INCOMPATIBLE_OPTS("-al", "-pr", &st);
                } else {
                    opt_primary = true;
                    pgo_changed = true;
                }
            else {
                /* -al valid only in the person or group domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }
        } else if (match_command("-q", c->args[cur_arg], 2)) {
            if (domain == eif_domain_person) {
                if ((cur_arg+1) < c->num_args) {
                    cur_arg++;
                    if (eku_chk_integer32(c->args[cur_arg],
                                            strlen(c->args[cur_arg]),
                                            &quota) == error_status_ok){
                        quota_str = c->args[cur_arg];
                        opt_quota = true;
                        pgo_changed = true;
                    } else {
                        INVALID_ARG(c->args[cur_arg], &st);
                    }
                } else {
                    OPT_REQUIRES_ARG(c->args[cur_arg], &st);
                }
            } else {
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }
        } else if (match_command("-nl", c->args[cur_arg], 3)) {
            if (domain == eif_domain_group) {
                 if (opt_list) {
                    INCOMPATIBLE_OPTS("-nl", "-l", &st);

                } else {
                    opt_nolist = true;
                    pgo_changed = true;
                }
            } else {
                /* -nl valid only in the group domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }
        } else if (match_command("-l", c->args[cur_arg], 2)) {
            if (domain == eif_domain_group)
                 if (opt_list) {
                    INCOMPATIBLE_OPTS("-nl", "-l", &st);
                } else {
                    opt_list = true;
                    pgo_changed = true;
                }
            else {
                /* -nl valid only in the group domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }
        } else if (match_command("-n", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_new_name = true;
                new_name = c->args[cur_arg];
                pgo_changed = true;
            }
        } else if (match_command("-f", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_fullname = true;
                fullname = c->args[cur_arg];
                pgo_changed = true;
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

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    switch (domain) {
    case eif_domain_person:
        chg_handle = P_change_h;
        sub_prompt =  Pgo_chg_prompts[tty_person_name_prompt];
        break;
    case eif_domain_group:
        chg_handle = G_change_h;
        sub_prompt =  Pgo_chg_prompts[tty_group_name_prompt];
        break;
     case eif_domain_org:
        sub_prompt =  Pgo_chg_prompts[tty_org_name_prompt];
        chg_handle = O_change_h;
        break;
    }

    st = ek_init_op(chg_handle);

    change_one = arg_name;   /* loop just once if name was on command line */

    if (!arg_name) {   /* get key interactively */
        tty_show_prompt(sub_prompt);
        input = tty_get_arg((char *)pgo_key, sizeof(pgo_key), &st);
        if (!input) {
            return st;
        }
    } else {
            /* copy command line arg to key buffer */
        strncpy((char *)pgo_key, name, sec_rgy_name_max_len);
        pgo_key[sec_rgy_name_max_len] = '\0';
    }

    if (GOOD_STATUS(&st)) {
        arg_name = eif_write_pgo_field(chg_handle, ekn_pgo_key,
				        (char *)pgo_key,
                                        domain, false, &st);
    } else {
        arg_name = false;  /* set up for key prompt in loop */
    }

    if (!do_interactive || change_one) {
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    } else {
        /* don't set exit status if we're going to continue interactively */
        EXIT_ON_ERROR(&st);
    }

    if (do_interactive) {
        pgo_changed = false;
        change_policy = false;

        /* Ask for new pgo data, prompting for a name if one was not supplied
         * on the command line.  Continue asking for pgo data until a valid set
         * of new data is obtained and applied.  If pgo data is successfully
         * changed, or no pgo changes are requested then enter the nested policy
         * data input loop.
         */
        do {
            /* Just exit on interrupt for now.  Use try/catch later */
            pthread_testcancel();

            if (!arg_name) {
                st = tty_get_field_arg(chg_handle, ekn_pgo_key,
                                       &tty_null_conv_table, arg_name,
                                       (char *) NULL, (char *) pgo_key,
				       sizeof(pgo_key),
                                       sub_prompt, &input);
               
                if (!input) {      /* return if null input on initial prompt */
                    return st;
                }
                CONTINUE_OR_EXIT_ON_ERROR(&st);

                arg_name = eif_write_pgo_field(chg_handle, ekn_pgo_key,
                                                 (char *) pgo_key, domain,
					         false, &st);
                CONTINUE_OR_EXIT_ON_ERROR(&st);
            }

                /* need to know if this is a reserved item */
            ek_read_field(chg_handle, ekn_pgo_is_reserved, reserved_flag,
                           sizeof(reserved_flag), &flag_len);
            if (strcmp(reserved_flag, ek_flag_on) == 0)
                reserved = true;
            else
                reserved = false;

            if (tty_get_pgo_args(chg_handle, domain, true, reserved, 
                                 opt_alias, Pgo_chg_prompts, &st)) {
                /* at least one field successfully changed */
                pgo_changed = true;
            }

            CONTINUE_OR_EXIT_ON_ERROR(&st);

            if (pgo_changed) { 
                 st = eif_change_pgo(chg_handle, domain);
            }

            if (! GOOD_STATUS(&st)) {
                if (tty_ask_y_n("Do you wish to try again ",
                        tty_response_no) == tty_response_yes) {
                    arg_name = false;
                    continue;
                } else {
                    CLEAR_STATUS(&st);
                    return st;
                }
            }

            /* if we get this far then the user either entered some valid pgo
             * changes, or didn't enter any changes at all, so ask for policy
             * changes */

            if (domain == eif_domain_org) {
                ans = tty_ask_y_n(
                              Policy_prompts[tty_policy_want_to_enter_prompt],
                              tty_response_no);
                if (ans == tty_response_yes) {
                    /* write the pgo name to the set policy handle.  This
                     * shouldn't ever fail unless some race condition causes
                     * the pgo item item to be deleted out from under us.  If
                     * it has been we go back to the outer input loop.  We read
                     * the  pgo name for the policy operation key from the
                     * name field of the pgo change operation so we're
                     * sure to pick up the new pgo name if it's been changed.
                     */
                    ek_read_field(chg_handle, ekn_pgo_name, (char *)policy_key,
                                   sizeof(policy_key), &policy_key_len);
                    if (eif_write_policy_field(policy_handle, ekn_policy_key,
                                                 (char *) policy_key,
					         false, &st)) {

                        /* continue looping for policy input until a valid set of
                         * policy data is obtained and applied.  If no changes
                         * are entered on the first pass through then we
                         * exit the policy loop without invoking the change
                         * operation
                         */
                        do {
                            /* Just exit on interrupt for now.  Use try/catch later */
                            pthread_testcancel();

                            if (tty_get_policy_args(policy_handle,
                                                    Policy_prompts, &st)) {
                                change_policy = true;
                            }
                            if (GOOD_STATUS(&st) && change_policy) {
                                st = eif_change_policy(policy_handle);
                            }
                            if (BAD_STATUS(&st)) {
                                if (tty_ask_y_n("Do you wish to try again ",
                                    tty_response_no) == tty_response_yes) {
                                    arg_name = false;
                                    continue;
                                } else {
                                    CLEAR_STATUS(&st);
                                    return st;
                                }
                            }
                        } while (BAD_STATUS(&st));
                    }
                }
            }

                /* WHEW! - if we're here then the user's changes have been
                 * successfully applied, or we had to bail out of the policy
                 * change for some wierd reason so start a fresh input loop.
                 */
            if (change_one) {
                break;  /* exit after first successful loop */
            } else {
                arg_name = false;
                pgo_changed = false;
                change_policy = false;
                st = ek_init_op(chg_handle);
                st = ek_init_op(policy_handle);
            }

        } while (true);

        SET_AND_EXIT_ON_ERROR(&st);
        return st;
    }

    /* write the arguments collected from the command line to the
     * appropriate fields in the change handle - we assume the
     * command line processing code weeded out any incompatible options, or
     * arguments inappropriate for the current domain, so we don't bother
     * checking here (besides, the kernel will signal if we try writing
     *  any inappropriate fields)
     */

    /* all fields but the key are optional on changes */
    if (opt_new_name && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(chg_handle, ekn_pgo_name, new_name,
                                    domain, false, &st);
        pgo_changed = true;
    }
    if (opt_fullname && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(chg_handle, ekn_pgo_fullname, fullname,
                                    domain, false, &st);
    }

    if (opt_alias && GOOD_STATUS(&st)) {
          (void) eif_write_pgo_field(chg_handle, ekn_pgo_is_an_alias,
                                      ek_flag_on, domain, false, &st);
    }
    if (opt_primary && GOOD_STATUS(&st)) {
          (void) eif_write_pgo_field(chg_handle, ekn_pgo_is_an_alias,
                                      ek_flag_off, domain, false, &st);
    }
    if (opt_nolist && GOOD_STATUS(&st)) {
          (void) eif_write_pgo_field(chg_handle, ekn_pgo_projlist_ok,
                                      ek_flag_off, domain, false, &st);
    }
    if (opt_list && GOOD_STATUS(&st)) {
          (void) eif_write_pgo_field(chg_handle, ekn_pgo_projlist_ok,
                                      ek_flag_on, domain, false, &st);
    }
    if (opt_quota && GOOD_STATUS(&st)) {
          (void) eif_write_pgo_field(chg_handle, ekn_pgo_quota, quota_str,
                                      domain, false, &st);
    }

    if (GOOD_STATUS(&st) && pgo_changed) {
        st = eif_change_pgo(chg_handle, domain);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_delete_pgo
#ifndef __STDC__
    ( c, domain )
    command_buf     *c;
    eif_domain_t    domain;
#else
  (
    command_buf     *c,
    eif_domain_t    domain
  )
#endif
{
    error_status_t   st;
    char             *name;
    boolean32        arg_name     = false,
                     opt_no_query = false,
                     verify;
    int              cur_arg;
    tty_error_buf_t  error_msg;
    ek_op_handle_t   del_handle;

    SET_STATUS(&st, error_status_ok);

    cur_arg = 1;
    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg])
           && GOOD_STATUS(&st)) {
            /* only  argument is the name */
        if (cur_arg == 1) {
            name = c->args[cur_arg];
            arg_name = true;
        } else {
            INVALID_ARG(c->args[cur_arg], &st);
        }
        cur_arg++;
    }

    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {
        if (match_command("-nq", c->args[cur_arg], 3)) {
            opt_no_query = true;
        } else {
            INVALID_OPT(c->args[cur_arg], &st);
        }
        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    switch (Current_domain) {

    case eif_domain_person:
        if (!arg_name) {
            SET_STATUS(&st, tty_entry_is_required);
            tty_print_status("A principal name must be supplied with delete", st);
        } else {
            del_handle = P_delete_h;
        }
        break;
    case eif_domain_group:
        if (!arg_name) {
            SET_STATUS(&st, tty_entry_is_required);
            tty_print_status("A group name must be supplied with delete", st);
        } else {
            del_handle = G_delete_h;
        }
        break;
    case eif_domain_org:
        if (!arg_name) {
            SET_STATUS(&st, tty_entry_is_required);
            tty_print_status("An org name must be supplied with delete", st);
        } else {
            del_handle = O_delete_h;
        }
        break;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    (void) eif_write_pgo_field(del_handle, ekn_pgo_key, name, Current_domain,
                             false, &st);
    if (GOOD_STATUS(&st)) {
        verify = opt_no_query ? false : eif_verify;
        st = eif_delete_pgo(del_handle, Current_domain, verify);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_view_pgo
#ifndef __STDC__
    ( c, domain )
    command_buf     *c;
    eif_domain_t    domain;
#else
  (
    command_buf     *c,
    eif_domain_t    domain
  )
#endif
{
    boolean32        arg_key        = false,
                     opt_full       = false,
                     opt_member     = false,
                     opt_policy     = false,
                     list_by_num    = false;
    char             *key;
    int              i;
    tty_error_buf_t  error_msg;
    ek_op_handle_t   op_handle;
    sec_rgy_name_t   view_key;
    error_status_t   st;

    strcpy((char *)view_key, ""); /* the view key defaults to a null string */

    SET_STATUS(&st, error_status_ok);
    for (i = 1; i < c->num_args && GOOD_STATUS(&st); i++) {
        if (match_command("-full", c->args[i], 2))
            opt_full = true;
        else if (match_command("-m", c->args[i], 2))
            opt_member = true;
        else if (match_command("-policy", c->args[i], 3)) {
            if ( (domain == eif_domain_person) ||
                (domain == eif_domain_group)) {
                INVALID_OPT_DOMAIN(c->args[i], domain, &st);
            } else {
                opt_policy = true;
            }
        } else if (match_command("-u", c->args[i], 2)) {
            if (arg_key) {
                INVALID_ARG(c->args[i], &st);
            } else if ((i + 1) < c->num_args) { /* make sure num is there */
                i++;
                arg_key = list_by_num = true;
                key = c->args[i];
            } else {
                OPT_REQUIRES_ARG (c->args[i], &st);
            }
        } else  {
                /* if there's a name argument it has to be first */
            if (i == 1 && !IS_AN_OPTION(c->args[i])) {
                arg_key = true;
                key = c->args[i];
            } else {
                    /* not a valid view argument or option in any domain */
                INVALID_ARG(c->args[i], &st);
            }
        }
    }


    if (GOOD_STATUS(&st)) {

        if (arg_key)
            strcpy((char *)view_key, key);

        st = eif_view_pgo(domain, (char *) view_key, opt_full,
			   opt_member, opt_policy, list_by_num);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_adopt
#ifndef __STDC__
    ( c )
    command_buf     *c;
#else
  (
    command_buf     *c
  )
#endif
{
    char             *uuid,
                     *name,
                     *number = ek_pgo_unknown_uid,
                     *quota_str = "unlimited",
                     *fullname;

    boolean32        arg_uuid        = false,
                     arg_name        = false,
                     arg_fullname    = false,
                     opt_nolist      = false;  /* groups only */

    int              cur_arg;
    signed32	     quota;
    eif_domain_t     domain;
    ek_op_handle_t   op_handle;
    error_status_t   st;

       /* look for non-option arguments at beginning of argument list */
    SET_STATUS(&st, error_status_ok);
    cur_arg = 1;
    domain = Current_domain;

    switch (domain) {
    case eif_domain_person:
        op_handle = P_adopt_h;
        break;
    case eif_domain_group:
        op_handle = G_adopt_h;
        break;
    case eif_domain_org:
        op_handle = O_adopt_h;
        break;
    default:
        SET_STATUS(&st, tty_bad_domain);
        tty_print_status("", st);
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg])
           && GOOD_STATUS(&st)) {

            /* first non-option argument is the uuid */
        if (cur_arg == 1) {
            uuid = c->args[cur_arg];
            arg_uuid = true;

            /* second non-option argument is the name */
         } else if (cur_arg == 2) {
            name = c->args[cur_arg];
            arg_name = true;

        } else {
            INVALID_ARG(c->args[cur_arg], &st);
        }
        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

        /* Look for options */
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {
        if (match_command("-u", c->args[cur_arg], 2)) {
            if ((cur_arg + 1) < c->num_args) {      /* make sure num is there*/
                cur_arg++;
                number = c->args[cur_arg];
            } else {
                OPT_REQUIRES_ARG (c->args[cur_arg], &st);
            }
        } else if (match_command("-f", c->args[cur_arg], 2)) {
            if ((cur_arg + 1) < c->num_args) {      /* make sure arg is there*/
                cur_arg++;
                fullname = c->args[cur_arg];
                arg_fullname = true;
            } else {
                OPT_REQUIRES_ARG (c->args[cur_arg], &st);
            }
        } else if (match_command("-q", c->args[cur_arg], 2)) {
            if ((cur_arg+1) < c->num_args) {
                cur_arg++;
                if (eku_chk_integer32(c->args[cur_arg],
                                        strlen(c->args[cur_arg]),
                                        &quota) == error_status_ok) {
                    quota_str = c->args[cur_arg];
                }
                else {
                    INVALID_ARG(c->args[cur_arg], &st);
                }
            } else {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            }
        } else if (match_command("-nl", c->args[cur_arg], 3)) {
            if (domain == eif_domain_group)
                opt_nolist = true;
            else {
                    /* -nl valid only in the group domain */
                INVALID_OPT_DOMAIN(c->args[cur_arg], domain, &st);
            }

        } else {
            /* not a valid option in any pgo domain */
            INVALID_OPT(c->args[cur_arg], &st);
        }

        cur_arg++;
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);


    st = ek_init_op(op_handle);

        /* the uuid is required */
    if (arg_uuid) {
        (void) eif_write_pgo_field(op_handle, ekn_pgo_uuid, uuid, domain,
                                    false, &st);
    } else {
        SET_STATUS(&st, tty_entry_is_required);
        tty_print_status(tty_uuid_missing, st);
    }

        /* the pgo name is required */
    if (arg_name && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(op_handle, ekn_pgo_key, name, domain,
                                    false, &st);
    } else if (!arg_name) {
        SET_STATUS(&st, tty_entry_is_required);
        tty_print_status(tty_name_missing, st);
    }

        /* the unix number is optional */
    if (GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(op_handle, ekn_pgo_unix_num, number,
                                    domain, false, &st);
    }

        /* the fullname is optional */
    if (arg_fullname && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(op_handle, ekn_pgo_fullname, fullname,
                                    domain, false, &st);
    }

    if (opt_nolist && GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(op_handle, ekn_pgo_projlist_ok, ek_flag_off,
                                    domain, false, &st);
    }

    if (GOOD_STATUS(&st)) {
        (void) eif_write_pgo_field(op_handle, ekn_pgo_quota, quota_str,
                                    domain, false, &st);
    }

    if (GOOD_STATUS(&st)) {
        st = eif_apply_add_defaults(op_handle, domain);
    }
    if (GOOD_STATUS(&st)) {
        st = eif_adopt_pgo(op_handle, domain);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}


PUBLIC error_status_t tty_do_add_cell
#ifndef __STDC__
    ( c )
    command_buf     *c;
#else
  (
    command_buf     *c
  )
#endif
{
    char            *uuid,
                    *name, *tmp_name,
                    *lnumber            = ek_pgo_unknown_uid,
                    *fnumber            = ek_pgo_unknown_uid,
                    *date, *exp_date    = NULL,
                    *passwd             = NULL,
                    *quota_str = "0",
                    *add_def,
                    *fullname           = NULL;

    boolean32       arg_cell_key    = true,
                    arg_lgroup      = false,
                    arg_lorg        = false,
                    arg_fgroup      = false,
                    arg_forg        = false,
                    arg_fname       = false,
                    arg_fpasswd     = false,
                    opt_valid       = false,
                    input,
                    opt_nolist      = false;  /* groups only */

    ek_login_name_buf_t lgroup, lorg, fgroup, forg, fname, fpasswd;
    krb5_error_code retval;
    eif_domain_t    saved_domain;
    int             cur_arg;
    signed32	    quota;
    ek_op_handle_t  op_handle;
    sec_rgy_name_t  io_buff,
                    def_val;
    long            def_val_len;
    int		    size;
    error_status_t  st;

       /* look for non-option arguments at beginning of argument list */
    SET_STATUS(&st, error_status_ok);
    saved_domain = Current_domain;
    cur_arg = 1;

    Current_domain = eif_domain_person;
    op_handle = C_add_h;

    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg])
           && GOOD_STATUS(&st)) {

            /* first non-option argument is the cellname */
        if (cur_arg == 1) {
            fullname = c->args[cur_arg];

        } else {
            INVALID_ARG(c->args[cur_arg], &st);
        }
        cur_arg++;
    }

        /* Look for options */
    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {
        if (match_command("-mp", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                passwd = c->args[cur_arg];
            }

            /* unix number for principal stored in local cell */
        } else if (match_command("-ul", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                lnumber = c->args[cur_arg];
            }

            /* unix number for principal stored in foreign cell */
        } else if (match_command("-uf", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                fnumber = c->args[cur_arg];
            }

        } else if (match_command("-gl", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                strncpy((char *)lgroup, c->args[cur_arg], sizeof(lgroup));
                arg_lgroup = true;
            }

        } else if (match_command("-gf", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                strncpy((char *)fgroup, c->args[cur_arg], sizeof(fgroup));
                arg_fgroup = true;
            }

        } else if (match_command("-ol", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                strncpy((char *)lorg, c->args[cur_arg], sizeof(lorg));
                arg_lorg = true;
            }

        } else if (match_command("-of", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                strncpy((char *)forg, c->args[cur_arg], sizeof(forg));
                arg_forg = true;
            }

        } else if (match_command("-fa", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                strncpy((char *)fname, c->args[cur_arg], sizeof(fname));
                arg_fname = true;
            }

        } else if (match_command("-fp", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                strncpy((char *)fpasswd, c->args[cur_arg], sizeof(fpasswd));
                arg_fpasswd = true;
            }

        } else if (match_command("-x", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                exp_date = c->args[cur_arg];
            }

        } else if (match_command("-q", c->args[cur_arg], 2)) {
            if ((cur_arg+1) < c->num_args) {
                cur_arg++;
                if (eku_chk_integer32(c->args[cur_arg],
                                        strlen(c->args[cur_arg]),
                                        &quota) == error_status_ok) {
                    quota_str = c->args[cur_arg];
                }
                else
                    INVALID_ARG(c->args[cur_arg], &st);
            } else
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);

        } else {
            /* not a valid option */
            INVALID_OPT(c->args[cur_arg], &st);
        }

        cur_arg++;
    }

    st = ek_init_op(op_handle);

    if (GOOD_STATUS(&st)) {
        /* convert cell name to realm name */
        if (fullname == NULL) {
            SET_STATUS(&st, tty_opt_requires_arg);
        } else if (strncmp(fullname,GLOBAL_DIR_ROOT,GLOBAL_DIR_ROOT_LEN)==0) {
            tmp_name = fullname + GLOBAL_DIR_ROOT_LEN;
            name = (char *) malloc((KRB_DIR_ROOT_LEN + strlen(tmp_name) + 1) *
                                    sizeof(char));
            sprintf(name, "%s%s", KRB_DIR_ROOT, tmp_name);
                /* Put the realm name in the form */
            st = ek_write_field(op_handle, ekn_cell_pname, name, strlen(name));
        } else {
            SET_STATUS(&st, tty_bad_data_format);
        }
    }

    /* Only do if the above succeeded */
    if (GOOD_STATUS(&st)) {
            /* the unix numbers are required */
        st = ek_write_field(op_handle, ekn_cell_lunix_num, lnumber,
                            strlen(lnumber));
        st = ek_write_field(op_handle, ekn_cell_funix_num, fnumber,
                            strlen(fnumber));
            /* fullname */
        if (GOOD_STATUS(&st)) {
            st = ek_write_field(op_handle, ekn_cell_fullname, fullname,
                                strlen(fullname));
        }
    }

    /* Only fill in the ACCT related fields if all of the above succeeded */
    if (GOOD_STATUS(&st)) {
        if (! arg_lgroup) {
            /* get the group for cell in local cell interactively */
            tty_show_prompt(Cell_add_prompts[tty_cell_lgname_prompt]);
            input = tty_get_arg(lgroup, sizeof(lgroup), &st);
            if (BAD_STATUS(&st) || !input) {
                arg_cell_key = false;
                SET_STATUS(&st, tty_entry_is_required);
            }
        }
        if ((! arg_fgroup) && GOOD_STATUS(&st)) {
            /* get the group for cell in foreign cell interactively */
            tty_show_prompt(Cell_add_prompts[tty_cell_fgname_prompt]);
            input = tty_get_arg(fgroup, sizeof(fgroup), &st);
            if (BAD_STATUS(&st) || !input) {
                arg_cell_key = false;
                SET_STATUS(&st, tty_entry_is_required);
            }
        }
        if ((! arg_lorg) && GOOD_STATUS(&st)) {
            /* get the cell org interactively */
            tty_show_prompt(Cell_add_prompts[tty_cell_loname_prompt]);
            input = tty_get_arg(lorg, sizeof(lorg), &st);
            if (BAD_STATUS(&st) || !input) {
                arg_cell_key = false;
                SET_STATUS(&st, tty_entry_is_required);
            }
        }
        if ((! arg_forg) && GOOD_STATUS(&st)) {
            /* get the cell org interactively */
            tty_show_prompt(Cell_add_prompts[tty_cell_foname_prompt]);
            input = tty_get_arg(forg, sizeof(forg), &st);
            if (BAD_STATUS(&st) || !input) {
                arg_cell_key = false;
                SET_STATUS(&st, tty_entry_is_required);
            }
        }

        if (arg_cell_key && GOOD_STATUS(&st)) {
            st = ek_write_field(op_handle, ekn_cell_lgname, lgroup,
                                strlen(lgroup));
            if (GOOD_STATUS(&st))
                st = ek_write_field(op_handle, ekn_cell_fgname, fgroup,
                                    strlen(fgroup));
            if (GOOD_STATUS(&st))
                st = ek_write_field(op_handle, ekn_cell_loname, lorg,
                                    strlen(lorg));
            if (GOOD_STATUS(&st))
                st = ek_write_field(op_handle, ekn_cell_foname, forg,
                                    strlen(forg));
        }
        if (BAD_STATUS(&st))
            arg_cell_key = false;

        /* Don't bother with the acct info if we couldn't get the keys */
        if (GOOD_STATUS(&st) && arg_cell_key) {

            size = sec_rgy_name_t_size;
            if (passwd) {
                st = ek_write_field(op_handle, ekn_cell_my_plain_passwd,
                                    passwd, strlen(passwd));

                /* Use the krb5 utility routine for obtaining passwords */
            } else if (retval =
                   krb5_read_password(Cell_add_prompts[tty_cell_passwd_prompt],
                                      NULL, (char *) io_buff, (int *) &size)) {
                com_err("CELL", retval, "Error in input password.");
                SET_STATUS(&st, sec_rgy_passwd_invalid);
            } else
                st = ek_write_field(op_handle, ekn_cell_my_plain_passwd,
                                    (char *) io_buff, strlen((char *)io_buff));

            /* Admin account in foreign cell to use while creating
             * the cell principal and acct in the foreign secd.
             */
            if (GOOD_STATUS(&st)) {
                if (! arg_fname) {
                    tty_show_prompt(Cell_add_prompts[tty_cell_foreign_name_prompt]);
                    input = tty_get_arg((char *)io_buff, sizeof(io_buff), &st);
                    if (!input) {
                        SET_STATUS(&st, tty_entry_is_required);
                    }
                    if (GOOD_STATUS(&st)) {
                        st = ek_write_field(op_handle, ekn_cell_foreign_name,
                                            (char *) io_buff,
					    strlen((char *) io_buff));
                    }
                } else {
                    st = ek_write_field(op_handle, ekn_cell_foreign_name,
                                        fname, strlen(fname));
                }
            }
            if (GOOD_STATUS(&st)) {
                size = sec_rgy_name_t_size;
                if (arg_fpasswd) {
                    st = ek_write_field(op_handle, ekn_cell_foreign_passwd,
                                        fpasswd, strlen(fpasswd));
                    /* Use the krb5 utility routine for obtaining passwords */
                } else if (retval =
                    krb5_read_password(
                            Cell_add_prompts[tty_cell_foreign_passwd_prompt],
                            NULL, (char *) io_buff, (int *) &size)) {
                    com_err("CELL", retval, "Error in input password.");
                    SET_STATUS(&st, sec_rgy_passwd_invalid);
                } else {
                    st = ek_write_field(op_handle, ekn_cell_foreign_passwd,
                                        (char *) io_buff,
					strlen((char *) io_buff));
                }
            }

            /* expiration date */
            if (GOOD_STATUS(&st) && (! exp_date)) {
                add_def = eif_get_default(eif_def_acct_exp_date);
                st = tty_get_field_arg(op_handle, ekn_cell_admin_exp_date,
                                        &tty_date_conv_table, true,
                                        add_def, (char *) io_buff,
				        sizeof(io_buff),
                                        Cell_add_prompts[tty_cell_exp_date_prompt],
                                        &input);
                if (GOOD_STATUS(&st))
                    st = ek_write_field(op_handle, ekn_cell_admin_exp_date,
                                        (char *) io_buff,
					strlen((char *) io_buff));
            } else if (GOOD_STATUS(&st)) {
                date = tty_get_kernel_io_val(exp_date, &tty_date_conv_table,
                                                &opt_valid);
                if (!opt_valid) {
                    SET_STATUS(&st, tty_bad_data_format);
                    tty_status_pre_echo(exp_date,"" , st);
                } else
                    st = ek_write_field(op_handle, ekn_cell_admin_exp_date,
                                        date, strlen(date));
            }
        }

        if (GOOD_STATUS(&st)) {
            st = ek_do_op(op_handle);
            if (GOOD_STATUS(&st))
                printf("  Principals and Accounts have been created\n");
            else {
                tty_print_status("Incomplete cell add", st);
                CLEAR_STATUS(&st);
            }
        } else {
            tty_print_status("Incomplete cell add", st);
            CLEAR_STATUS(&st);
        }

    }   /* End of acct field generation */

    Current_domain = saved_domain;
    return st;
}


PUBLIC error_status_t tty_do_member
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    boolean32        arg_name      = false,   /* name  on the command line? */
                     opt_add       = false,   /* -a option on command line? */
                     opt_delete    = false,   /* -r option on command line? */
                     opt_no_query  = false,
                     interactive   = true,
                     verify,
                     input,
                     do_add,
                     change_one;

    sec_rgy_name_t   name_buff, add_buff, del_buff;
    char             *prompt_ptr;
    int              cur_arg;
    tty_error_buf_t  error_msg;
    ek_op_handle_t   add_handle,
                     delete_handle;
    error_status_t   st;

    SET_STATUS(&st, error_status_ok);

    switch (Current_domain) {
    case eif_domain_group:
        add_handle = M_add_group_member_h;
        delete_handle = M_del_group_member_h;
        prompt_ptr = Member_update_prompts[tty_mem_group_name_prompt];
        break;
    case eif_domain_org:
        add_handle = M_add_org_member_h;
        delete_handle = M_del_org_member_h;
        prompt_ptr = Member_update_prompts[tty_mem_org_name_prompt];
        break;
    default:
        SET_STATUS(&st, tty_bad_domain);
        tty_print_status("", st);
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
        break;
    }

    SET_STATUS(&st, error_status_ok);
    cur_arg = 1;
    while (cur_arg < c->num_args && !IS_AN_OPTION(c->args[cur_arg]) &&
           GOOD_STATUS(&st)) {

            /* only non-option argument is the name */
        if (cur_arg == 1) {
            strncpy((char *)name_buff, c->args[cur_arg], sizeof(name_buff));
            name_buff[sizeof(name_buff) -1] = '\0';
            arg_name = true;
        } else {
            INVALID_ARG(c->args[cur_arg], &st);
        }

        cur_arg++;
    }

    while (cur_arg < c->num_args && GOOD_STATUS(&st)) {
        if (match_command("-a", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_add = true;
                if ((strlen(c->args[cur_arg]) + 1) > sizeof(add_buff)) {
                     SET_STATUS(&st, tty_err_input_too_long);
                } else {
                    strcpy((char *)add_buff, c->args[cur_arg]);
                }
            }
        } else if (match_command("-r", c->args[cur_arg], 2)) {
            if ((cur_arg + 2) > c->num_args) {
                OPT_REQUIRES_ARG(c->args[cur_arg], &st);
            } else {
                cur_arg++;
                opt_delete = true;
                if ((strlen(c->args[cur_arg]) + 1) > sizeof(del_buff)) {
                     SET_STATUS(&st, tty_err_input_too_long);
                } else {
                    strcpy((char *)del_buff, c->args[cur_arg]);
                }
            }
        } else if (match_command("-nq", c->args[cur_arg], 3)) {
            opt_no_query = true;
        } else {
            INVALID_OPT(c->args[cur_arg], &st);
        }

        if (GOOD_STATUS(&st)
            && !match_command("-nq", c->args[cur_arg], 3)) {
            interactive = false;
        }

        cur_arg++;
    }

    if (GOOD_STATUS(&st)) {
        st = eif_site_check( ek_site_update, eif_auto_rebind);
        if (BAD_STATUS(&st)) {
            tty_print_status("", st);
        }
    }

    SET_EXIT_STATUS(&st);
    RETURN_OR_EXIT_ON_ERROR(&st);

    change_one = arg_name;
    verify = opt_no_query ? false : eif_verify;

    if (!arg_name) {
        tty_show_prompt(prompt_ptr);
        input = tty_get_arg((char *)name_buff, sizeof(name_buff), &st);
        if (!input) {
            return st;
        } else {
            arg_name = GOOD_STATUS(&st);
        }
    }

    if (!interactive) {
        SET_EXIT_STATUS(&st);
        RETURN_OR_EXIT_ON_ERROR(&st);
    } else {
        /* don't set exit status if we're going to continue interactively */
        EXIT_ON_ERROR(&st);
    }


    if (interactive) {
        do_add = true;
        do {
            /* Just exit on interrupt for now.  Use try/catch later */
            pthread_testcancel();

            /* loop until a a reasonably sized name or an immediate newline is
             * detected
             */
            while (!arg_name) {
                tty_show_prompt(prompt_ptr);
                input = tty_get_arg((char *)name_buff, sizeof(name_buff), &st);

                if (!input) {
                    return st;   /* return on immediate newline */
                } else {
                    arg_name = GOOD_STATUS(&st);
                }

                CONTINUE_OR_EXIT_ON_ERROR(&st);
            }

            if (do_add) {
                st = ek_init_op(add_handle);
                arg_name = eif_write_member_field(add_handle, ekn_mem_key,
                                                   (char *)name_buff, &st);
                CONTINUE_OR_EXIT_ON_ERROR(&st);   /* loop if key is bad */

                do {
                    /* Just exit on interrupt for now.  Use try/catch later */
                    pthread_testcancel();

                    tty_show_prompt(
                                Member_update_prompts[tty_member_add_prompt]);
                    opt_add = tty_get_arg((char *)add_buff, sizeof(add_buff), &st);
                    if (opt_add && GOOD_STATUS(&st)) {
                        eif_write_member_field(add_handle, ekn_mem_name,
                                                (char *)add_buff, &st);
                        if (GOOD_STATUS(&st))
                            st = eif_do_member_op(add_handle, verify);
                    }

                    CONTINUE_OR_EXIT_ON_ERROR(&st);

                } while (opt_add);
            }

            /* don't redo add processing if key fails for deletes */
            do_add = false;
            st = ek_init_op(delete_handle);
            arg_name = eif_write_member_field(delete_handle, ekn_mem_key,
                                               (char *)name_buff, &st);
            CONTINUE_OR_EXIT_ON_ERROR(&st);   /* loop if key is bad */

            do {
                /* Just exit on interrupt for now.  Use try/catch later */
                pthread_testcancel();

                tty_show_prompt(Member_update_prompts[tty_member_del_prompt]);
                opt_delete=tty_get_arg((char *)del_buff,sizeof(del_buff),&st);
                if (opt_delete && GOOD_STATUS(&st)) {
                    eif_write_member_field(delete_handle, ekn_mem_name,
                                            (char *)del_buff, &st);
                    if (GOOD_STATUS(&st))
                        st = eif_do_member_op(delete_handle, verify);
                }

                CONTINUE_OR_EXIT_ON_ERROR(&st);

            } while (opt_delete);

            if (change_one) {
                break;
            } else {
                /* set up a new loop */
                arg_name = false;
                do_add = true;
            }

        } while  (true);

        SET_AND_EXIT_ON_ERROR(&st);
        return st;
    }


        /* process any command-line add arguments */
    if (opt_add) {
        st = ek_init_op(add_handle);
        st = eif_member_update(add_handle, (char *)name_buff, (char *)add_buff, verify);
        SET_AND_EXIT_ON_ERROR(&st);
    }

    if (opt_delete) {
        st = ek_init_op(delete_handle);
        st = eif_member_update(delete_handle, (char *)name_buff, (char *)del_buff, verify);
        SET_AND_EXIT_ON_ERROR(&st);
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}
