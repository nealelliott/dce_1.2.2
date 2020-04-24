/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sa_util.h,v $
 * Revision 1.1.7.2  1996/03/11  13:27:33  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:24  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:02:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/28  20:04 UTC  mdf
 * 	Resolve CHFts15068
 * 	[1995/04/27  13:03 UTC  mdf  /main/mdf_mothra_defects_3/1]
 * 
 * Revision 1.1.5.1  1994/06/17  18:41:30  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:13  devsrc]
 * 
 * Revision 1.1.2.3  1993/02/20  23:21:29  burati
 * 	Added sa_status_only() util rtn, to display status w/out err msg
 * 	[1993/02/19  23:56:19  burati]
 * 
 * Revision 1.1.2.2  1993/01/29  18:53:17  burati
 * 	Initial revision
 * 	[1993/01/14  23:01:05  burati]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */

#include <time.h>
#include <dce/dce_error.h>
#include <dce/rpcpvt.h>
#include <dce/rpc.h>
#include <dce/binding.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <dce/repadm.h>
#include <rgymacro.h>
#include <commands.h>
#include <args.h>

#define saINDENT "          "
#define SETUP_DEFAULT_AUTH_INFO(auth_info) \
 auth_info.info_type = sec_rgy_bind_auth_dce; \
 auth_info.tagged_union.dce_info.authn_level= rpc_c_protect_level_pkt_integ; \
 auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret; \
 auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce; \
 auth_info.tagged_union.dce_info.identity = NULL;

HELP(tty_help_site);
HELP(tty_help_stop);
HELP(tty_help_state);
HELP(tty_help_mkey);
HELP(tty_help_quit);
HELP(tty_help_all);
HELP(tty_help_info);
HELP(tty_help_initr);
HELP(tty_help_delr);
HELP(tty_help_replr);
HELP(tty_help_lrep);
HELP(tty_help_mon);
HELP(tty_help_destroy);
HELP(tty_help_chm);
HELP(tty_help_become);

PUBLIC void cmd_usage(
#ifdef __STDC__
    char *cmd
#endif
);

PUBLIC void sa_status_print(
#ifdef __STDC__
    error_status_t status
#endif
);

PUBLIC void sa_status_only(
#ifdef __STDC__
    error_status_t status
#endif
);

PUBLIC void sa_error_print(
#ifdef __STDC__
    error_status_t status
#endif
);

PUBLIC void msg_status_print(
#ifdef __STDC__
    char           *msg,
    error_status_t status
#endif
);

PUBLIC void sa_print_def_rep_info(
#ifdef __STDC__
    sec_rgy_handle_t	ctxt,
    error_status_t      *status
#endif
);

PUBLIC void sa_print_site_info(
#ifdef __STDC__
    rs_replica_info_t   	*infoP,
    boolean32           	display_update_time,
    boolean32          		full,
    rs_replica_twr_vec_p_t	rtwrs,
    rs_replica_twr_vec_p_t	mtwrs,
    error_status_t      	*status
#endif
);

PUBLIC boolean32 sa_ask_y_n(
#ifdef __STDC__
    void
#endif
);
