/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_pgo.h,v $
 * Revision 1.1.4.2  1996/03/11  13:27:23  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:02:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:45  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:42:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:08  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_pgo.h V=5 04/29/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      tty pgo interface public declarations
** 
*/

#ifndef tty_pgo__h_included
#define tty_pgo__h_included

#include <dce/nbase.h>

#include <ek_db.h>
#include <commands.h>
#include <eif.h>

void tty_pgo_display (
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    boolean32       show_full,
    boolean32       show_members,
    boolean32       show_policy,
    error_status_t  *stp
#endif
);
 
error_status_t tty_do_add_pgo (
#ifdef __STDC__
    command_buf     *c,
    eif_domain_t    domain
#endif
);

error_status_t tty_do_change_pgo (
#ifdef __STDC__
    command_buf    *c,
    eif_domain_t   domain
#endif
);

error_status_t tty_do_delete_pgo (
#ifdef __STDC__
    command_buf    *c,
    eif_domain_t   domain
#endif
); 

error_status_t tty_do_view_pgo (
#ifdef __STDC__
    command_buf    *c,
    eif_domain_t   domain
#endif
); 

error_status_t tty_do_adopt (
#ifdef __STDC__
    command_buf  *c
#endif
);
 
error_status_t tty_do_member (
#ifdef __STDC__
    command_buf  *c
#endif
);

#endif   /* tty_pgo__h_included */
