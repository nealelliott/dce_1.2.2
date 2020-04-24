/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_acct.h,v $
 * Revision 1.1.6.2  1996/03/11  13:27:08  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:04  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:01:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:27  root]
 * 
 * Revision 1.1.4.1  1994/07/15  14:58:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:17  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  12:41:13  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_acct.h V=5 04/29/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      tty account interface public declarations
** 
*/

#ifndef tty_acct__h_included
#define tty_acct__h_included
 
#include <dce/nbase.h>

#include <ek_db.h>
#include <commands.h>

void tty_display_acct ( 
#ifdef __STDC__
    ek_op_handle_t   op_handle,
    boolean32        show_full,
    error_status_t   *stp
#endif
);

error_status_t tty_do_view_acct (
#ifdef __STDC__
    command_buf  *c
#endif
);

error_status_t tty_do_change_acct (
#ifdef __STDC__
    command_buf  *c
#endif
);

error_status_t tty_do_add_acct (
#ifdef __STDC__
    command_buf  *c
#endif
);
 

error_status_t tty_do_delete_acct (
#ifdef __STDC__
    command_buf  *c
#endif
);

PRIVATE void tty_get_gen_pwd (
    ek_op_handle_t   op_handle,
    char             *prompt,
    sec_rgy_name_t   *new_pwd,
    error_status_t   *stp
);

#endif  /* tty_acct__h_included */
