/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_prop.h,v $
 * Revision 1.1.4.2  1996/03/11  13:27:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:18  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:02:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:47  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:42:34  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:11  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_prop.h V=8 05/21/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit tty properties/policies interface declarations
** 
*/

#ifndef tty_prop__h_included
#define tty_prop__h_included

#include <dce/nbase.h>

#include <ek_db.h>
#include <commands.h>

void tty_display_policy (
#ifdef __STDC__
    ek_op_handle_t  policy_handle
#endif
);

void tty_display_auth_policy (
#ifdef __STDC__
    ek_op_handle_t  auth_policy_handle
#endif
);
    
void tty_display_prop (
#ifdef __STDC__
    ek_op_handle_t  prop_handle
#endif
);
      
void tty_display_override_policy (
#ifdef __STDC__
    ek_op_handle_t  override_handle
#endif
);  

boolean32 tty_get_policy_args ( 
#ifdef __STDC__
     ek_op_handle_t  op_handle,
     char            **prompts,
     error_status_t  *stp
#endif
);  

boolean32 tty_get_auth_policy_args ( 
#ifdef __STDC__
     ek_op_handle_t  op_handle,
     char            **prompts,
     error_status_t  *stp
#endif
);  

error_status_t tty_do_view_override (
#ifdef __STDC__
    command_buf  *c
#endif
);
    
error_status_t tty_do_add_override ( 
#ifdef __STDC__
    command_buf  *c
#endif
);

error_status_t tty_do_delete_override ( 
#ifdef __STDC__
    command_buf  *c
#endif
);

error_status_t tty_do_change_override (
#ifdef __STDC__
    command_buf  *c
#endif
);

error_status_t tty_do_properties (
#ifdef __STDC__
    command_buf  *c
#endif
);

error_status_t tty_do_policy (
#ifdef __STDC__
    command_buf  *c
#endif
);

#endif  /* tty_prop__h_included */
