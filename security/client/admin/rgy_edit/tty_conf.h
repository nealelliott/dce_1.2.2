/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_conf.h,v $
 * Revision 1.1.8.2  1996/03/11  13:27:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:06  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:01:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:30  root]
 * 
 * Revision 1.1.6.2  1994/09/09  19:12:07  mob
 * 	Remove temporary ERA support
 * 	[1994/09/09  19:06:41  mob]
 * 
 * Revision 1.1.6.1  1994/03/02  17:54:31  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:11:05  hanfei]
 * 
 * Revision 1.1.4.2  1992/12/29  12:41:26  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:24  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/03  20:33:58  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  21:01:11  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_conf.h V=17 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit - tty interface configuration module
**
*/

#ifndef tty_conf_h__included
#define tty_conf_h__included

/*
**  tty user io string constants
*/
#define tty_passwd_none        ""
#define tty_misc_none          ""
#define tty_homedir_none       ""
#define tty_shell_none         ""
#define tty_date_none          "none"
#define tty_fullname_none      ""
#define tty_valid              "valid"
#define tty_not_valid          "not valid"
#define tty_lifespan_infinite  "forever"
#define tty_acct_abbrev_person "p"
#define tty_acct_abbrev_group  "pg"
#define tty_acct_abbrev_org    "pgo"
#define tty_yes                "yes"
#define tty_no                 "no"
#define tty_y                  "y"
#define tty_n                  "n"
#define tty_quit               "q"
#define tty_go                 "g"
#define tty_uid_unknown        "auto assign"

/*
** tty prompt strings
*/
#define Major_prompt       "rgy_edit=> "    /* the highest level rgy_edit prompt */
#define Acct_add_prompt    "Add Account=> "
#define Acct_chg_prompt    "Change Account=> "
#define Person_add_prompt  "Add principal=> "
#define Person_chg_prompt  "Change principal=> "
#define Group_add_prompt   "Add Group=> "
#define Group_chg_prompt   "Change Group=> "
#define Org_add_prompt     "Add Org=> "
#define Org_chg_prompt     "Change Org=> "
#define Member_prompt      "Member=> "

extern char  *Pgo_chg_prompts[];
extern char  *Pgo_add_prompts[];

/* indices into PGO prompt arrays */
#define tty_person_name_prompt        0   /* person items only       */
#define tty_group_name_prompt         1   /* group items only        */
#define tty_org_name_prompt           2   /* organization items only */
#define tty_pgo_unix_num_prompt       3
#define tty_pgo_fullname_prompt       4
#define tty_pgo_passwd_prompt         5
#define tty_pgo_projlist_prompt       6
#define tty_pgo_new_name_prompt       7   /* changes only */
#define tty_pgo_quota_prompt          8   /* person items only */

extern char *Policy_prompts[];

/* indices into policy prompt array */
#define tty_policy_acct_lifespan_prompt    0
#define tty_policy_passwd_min_len_prompt   1
#define tty_policy_passwd_lifespan_prompt  2
#define tty_policy_passwd_exp_date_prompt  3
#define tty_policy_passwd_no_spaces_prompt 4
#define tty_policy_passwd_non_alpha_prompt 5
#define tty_policy_want_to_enter_prompt    6

extern char *Auth_policy_prompts[];

/* indices into auth policy prompt array */
#define tty_auth_policy_max_ticket_lifetime     0
#define tty_auth_policy_max_renewable_lifetime  1

extern char *Property_prompts[];

/* indices into property prompt array */
#define tty_prop_readonly_prompt      0
#define tty_prop_want_to_enter_prompt 1
#define tty_prop_shadow_passwd_prompt 2
#define tty_prop_luid_person_prompt   3
#define tty_prop_luid_group_prompt    4
#define tty_prop_luid_org_prompt      5
#define tty_prop_max_uid_prompt       6
#define tty_prop_min_tkt_prompt       7
#define tty_prop_def_tkt_prompt       8

extern char *Lrgy_property_prompts[];

/* indices into property prompt array */
#define tty_lrgy_max_entries_prompt     0
#define tty_lrgy_lifespan_prompt        1

extern char *Acct_chg_prompts[];
extern char *Acct_add_prompts[];

/* indices into Account prompt arrays */
#define tty_acct_pname_prompt         0
#define tty_acct_gname_prompt         1
#define tty_acct_oname_prompt         2
#define tty_acct_abbrev_prompt        3
#define tty_acct_passwd_prompt        4
#define tty_acct_passwd2_prompt       5
#define tty_acct_misc_prompt          6
#define tty_acct_homedir_prompt       7
#define tty_acct_shell_prompt         8
#define tty_acct_passwd_valid_prompt  9
#define tty_acct_exp_date_prompt      10
#define tty_acct_server_prompt        11
#define tty_acct_client_prompt        12
#define tty_acct_valid_prompt         13
#define tty_acct_new_pname_prompt     14     /* account changes only! */
#define tty_acct_new_gname_prompt     15
#define tty_acct_new_oname_prompt     16
#define tty_acct_good_since_prompt    17
#define tty_acct_post_dated_prompt    18
#define tty_acct_forwardable_prompt   19
#define tty_acct_tgt_prompt           20
#define tty_acct_renewable_prompt     21
#define tty_acct_proxiable_prompt     22
#define tty_acct_dup_session_key      23
#define tty_acct_my_passwd_prompt     24

extern char *Override_prompts[];

/* indices into override policy prompt array */
#define tty_override_name_prompt            0
#define tty_override_exclude_passwd_prompt  1
#define tty_override_root_passwd_prompt     2
#define tty_override_other_passwd_prompt    3
#define tty_override_custom_data_prompt     4
#define tty_override_want_to_enter_prompt   5

extern char *Member_update_prompts[];

/* indices into member prompt arrays */
#define tty_member_add_prompt     0
#define tty_member_del_prompt     1
#define tty_mem_group_name_prompt 2
#define tty_mem_org_name_prompt   3


extern char *Default_chg_prompts[];

/* indices into defaults prompt array */
#define tty_def_acct_abbrev_prompt          0
#define tty_def_acct_homedir_prompt         1
#define tty_def_acct_shell_prompt           2
#define tty_def_acct_passwd_valid_prompt    3
#define tty_def_acct_exp_date_prompt        4
#define tty_def_acct_valid_prompt           5

extern char *Cell_add_prompts[];

/* indices into cell prompt array */
#define tty_cell_lgname_prompt              0
#define tty_cell_fgname_prompt              1
#define tty_cell_loname_prompt              2
#define tty_cell_foname_prompt              3
#define tty_cell_passwd_prompt              4
#define tty_cell_exp_date_prompt            5
#define tty_cell_foreign_name_prompt        6
#define tty_cell_foreign_passwd_prompt      7

/*
 * error text string constants - used to augment status output in a
 * consistent way
 */
#define tty_dont_understand      "Don't understand"
#define tty_input_too_long       "input argument too long"
#define tty_cannot_be_changed    "cannot be changed"
#define tty_acct_bad_abbrev      "Unknown abbreviation type"
#define tty_acct_invalid_id      "Invalid account id"
#define tty_acct_no_wild_id      "Invalid account id.  Wildcards not allowed"
#define tty_unix_number_missing  "The unix number is missing"
#define tty_uuid_missing         "The uuid is missing"
#define tty_name_missing         "The name is missing"
#define tty_bad_op_domain        "Command not valid in the current domain"
#define tty_bad_person_op        "Command not valid in the principal domain"
#define tty_bad_group_op         "Command not valid in the group domain"
#define tty_bad_org_op           "Command not valid in the org domain"
#define tty_bad_acct_op          "Command not valid in the acct domain"
#define tty_acct_del_key_missing "An account key (-p -g -o) (or partial key) must be\
 supplied with delete\n"

#endif
