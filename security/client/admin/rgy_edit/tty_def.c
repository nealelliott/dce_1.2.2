/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_def.c,v $
 * Revision 1.1.4.2  1996/03/11  13:27:12  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:01:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:31  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:41:32  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:48  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_def.c V=13 08/21/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty interface default processing 
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)tty_def.c	13 - 08/21/91";
#endif

#include <stdio.h>
#include <un_strng.h>

#include <tty_base.h>
#include <macros.h>
#include <ek_db.h>
#include <ek_types.h>
#include <eif_def.h>
#include <eif.h>
#include <tty_io.h>
#include <tty_conf.h>
#include <tty_def.h>
#include <tty_err.h>

/* tty_get_default_args
** 
** Unlike the the data in other get_args routines, default data is not stored
** in the registry, so no operation handle is involved.   However, we use
** an arbitrary initialized handle, so we can make use of the tty_get_field_arg
** routine.  As there's no data in the handle field, tty_get_field_args will
** use the default we pass to it.
*/

PRIVATE boolean32 tty_get_default_args
#ifndef __STDC__
    ( prompts, stp ) 
     char            **prompts;
     error_status_t  *stp;
#else
  (
     char            **prompts,
     error_status_t  *stp
  )
#endif
{
    boolean32        one_default_changed = false,  /* assume no new data */
                     input,                        
                     io_map;
    ek_pname_buf_t   io_buff;
    char             *kernel_def,
                     *tty_def;
    ek_op_handle_t   dummy_op = P_change_h;      /* any old op handle */
    ek_field_id_t    dummy_field = ekn_pgo_key;

    *stp = ek_init_op(dummy_op);

#ifdef NOTDEF
    kernel_def = eif_get_default(eif_def_acct_abbrev);
    *stp = tty_get_field_arg(dummy_op, dummy_field, &tty_abbrev_conv_table, 
			     true, kernel_def, io_buff,sizeof(io_buff), 
			     prompts[tty_def_acct_abbrev_prompt], &input); 
    if (GOOD_STATUS(stp) && input) {
	(void) eif_set_default(eif_def_acct_abbrev, io_buff, stp);
	if (GOOD_STATUS(stp))
	    one_default_changed = true;
	else {
	    tty_def = tty_get_if_io_val(io_buff, &tty_abbrev_conv_table,
					&io_map);
	    tty_status_pre_echo(tty_def, "", *stp);
	}
    }
    if (BAD_STATUS(stp))
	return one_default_changed;
#endif /* NOTDEF */
    
    kernel_def = eif_get_default(eif_def_acct_homedir);
    *stp = tty_get_field_arg(dummy_op, dummy_field, &tty_null_conv_table, 
			     true, kernel_def, io_buff,sizeof(io_buff), 
			     prompts[tty_def_acct_homedir_prompt], &input); 
    if (GOOD_STATUS(stp) && input) {
	(void) eif_set_default(eif_def_acct_homedir, io_buff, stp);
	if (GOOD_STATUS(stp))
	    one_default_changed = true;
	else {
	    tty_def = tty_get_if_io_val(io_buff, &tty_null_conv_table, &io_map);
	    tty_status_pre_echo(tty_def, "", *stp);
	}
    }
    if (BAD_STATUS(stp))
	return one_default_changed;

    kernel_def = eif_get_default(eif_def_acct_shell);
    *stp = tty_get_field_arg(dummy_op, dummy_field, &tty_null_conv_table, 
			     true, kernel_def, io_buff,sizeof(io_buff), 
			     prompts[tty_def_acct_shell_prompt], &input); 
    if (GOOD_STATUS(stp) && input) {
	(void) eif_set_default(eif_def_acct_shell, io_buff, stp);
	if (GOOD_STATUS(stp))
	    one_default_changed = true;
	else {
	    tty_def = tty_get_if_io_val(io_buff, &tty_null_conv_table, &io_map);
	    tty_status_pre_echo(tty_def, "", *stp);
	}
    }
    if (BAD_STATUS(stp))
	return one_default_changed;

    kernel_def = eif_get_default(eif_def_acct_passwd_valid);
    *stp = tty_get_field_arg(dummy_op, dummy_field, &tty_flag_conv_table, 
			     true, kernel_def, io_buff,sizeof(io_buff), 
			     prompts[tty_def_acct_passwd_valid_prompt], &input); 
    if (GOOD_STATUS(stp) && input) {
	(void) eif_set_default(eif_def_acct_passwd_valid, io_buff, stp);
	if (GOOD_STATUS(stp))
	    one_default_changed = true;
	else {
	    tty_def = tty_get_if_io_val(io_buff, &tty_flag_conv_table, &io_map);
	    tty_status_pre_echo(tty_def, "", *stp);
	}
    }
    if (BAD_STATUS(stp))
	return one_default_changed;

    kernel_def = eif_get_default(eif_def_acct_exp_date);
    *stp = tty_get_field_arg(dummy_op, dummy_field, &tty_date_conv_table, 
			     true, kernel_def, io_buff,sizeof(io_buff), 
			     prompts[tty_def_acct_exp_date_prompt], &input); 
    if (GOOD_STATUS(stp) && input) {
	(void) eif_set_default(eif_def_acct_exp_date, io_buff, stp);
	if (GOOD_STATUS(stp))
	    one_default_changed = true;
	else {
	    tty_def = tty_get_if_io_val(io_buff, &tty_date_conv_table, &io_map);
	    tty_status_pre_echo(tty_def, "", *stp);
	}
    }
    if (BAD_STATUS(stp))
	return one_default_changed;


    kernel_def = eif_get_default(eif_def_acct_valid);
    *stp = tty_get_field_arg(dummy_op, dummy_field, &tty_flag_conv_table, 
			     true, kernel_def, io_buff,sizeof(io_buff), 
			     prompts[tty_def_acct_valid_prompt], &input); 
    if (GOOD_STATUS(stp) && input) {
	(void) eif_set_default(eif_def_acct_valid, io_buff, stp);
	if (GOOD_STATUS(stp))
	    one_default_changed = true;
	else {
	    tty_def = tty_get_if_io_val(io_buff, &tty_flag_conv_table, &io_map);
	    tty_status_pre_echo(tty_def, "", *stp);
	}
    }
    if (BAD_STATUS(stp))
	return one_default_changed;


    return one_default_changed;
}


/* tty_display_defaults
 * 
 * displays user-configurable defaults
 */
PUBLIC error_status_t tty_display_defaults (
#ifdef __STDC__
    void
#endif
)
{
    char            *kernel_def,
                    *tty_def,
                    header_indent[3],
                    data_indent[5];
    boolean32       io_map;
    error_status_t  st;


    strcpy(header_indent, "  ");
    strcpy(data_indent, "    ");

    SET_STATUS(&st, tty_input_out_of_range);   /* for io mapping errors */

    printf("%sAccount entries:\n", header_indent);

#ifdef NOTDEF
    kernel_def = eif_get_default(eif_def_acct_abbrev);
    tty_def = tty_get_if_io_val(kernel_def, &tty_abbrev_conv_table, &io_map);
    if (!io_map)
	TTY_FATAL("io mapping error in tty_display_defaults", &st);
    printf("%sAbbreviation type: %s\n",  data_indent, tty_def);
#endif

    kernel_def = eif_get_default(eif_def_acct_homedir);
    tty_def = tty_get_if_io_val(kernel_def, &tty_null_conv_table, &io_map);
    if (!io_map)
	TTY_FATAL("io mapping error in tty_display_defaults", &st);
    printf("%sHome Directory:    %c%s%c\n",  data_indent, DQ, tty_def, DQ);

    kernel_def = eif_get_default(eif_def_acct_shell);
    tty_def = tty_get_if_io_val(kernel_def, &tty_null_conv_table, &io_map);
    if (!io_map)
	TTY_FATAL("io mapping error in tty_display_defaults", &st);
    printf("%sShell:             %c%s%c\n",  data_indent, DQ, tty_def, DQ);

    kernel_def = eif_get_default(eif_def_acct_passwd_valid);
    tty_def = tty_get_if_io_val(kernel_def, &tty_flag_conv_table, &io_map);
    if (!io_map)
	TTY_FATAL("io mapping error in tty_display_defaults", &st);
    printf("%sPassword valid?:   %c%s%c\n",  data_indent, DQ, tty_def, DQ);

    kernel_def = eif_get_default(eif_def_acct_exp_date);
    tty_def = tty_get_if_io_val(kernel_def, &tty_date_conv_table, &io_map);
    if (!io_map)
	TTY_FATAL("io mapping error in tty_display_defaults", &st);
    printf("%sExpiration date:   %s\n",  data_indent, tty_def);

    kernel_def = eif_get_default(eif_def_acct_valid);
    tty_def = tty_get_if_io_val(kernel_def, &tty_flag_conv_table, &io_map);
    if (!io_map)
	TTY_FATAL("io mapping error in tty_display_defaults", &st);
    printf("%sAccount valid?:    %c%s%c\n",  data_indent, DQ, tty_def, DQ);

    SET_STATUS(&st, error_status_ok);
    return st;
}   


PUBLIC error_status_t tty_do_defaults
#ifndef __STDC__
    (c)
    command_buf  *c;
#else
  (
    command_buf  *c
  )
#endif
{
    int                  cur_arg;
    error_status_t            st;
    tty_y_n_response_t  ans;

    SET_STATUS(&st, error_status_ok);
    cur_arg = 1;
    while (cur_arg < c->num_args && (GOOD_STATUS(&st)) ) {
	INVALID_ARG(c->args[cur_arg], &st);
	cur_arg++;
    }	    

    if (GOOD_STATUS(&st)) {
	tty_display_defaults();
	ans = tty_ask_y_n("Do you wish to make changes", tty_response_no);
	if (ans == tty_response_yes) {
	    (void) tty_get_default_args(Default_chg_prompts, &st);
	}
    }
    if (GOOD_STATUS(&st)) {
        eif_save_defaults();
    }

    SET_AND_EXIT_ON_ERROR(&st);
    return st;
}
