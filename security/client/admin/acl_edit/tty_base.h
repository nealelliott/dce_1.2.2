/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_base.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:14  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:16  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:56:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:44  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:37:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:33:28  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/08  15:25:44  burati
 * 	Add new functionality to support POSIX mask calculation
 * 	[1992/05/07  14:59:31  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_base.h V=5 11/21/91 //littl/prgy/src/admin/dacl_edit
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      tty base declarations
** 
*/

#ifndef tty_base__h_included
#define tty_base__h_included 

#include <dce/nbase.h>

/* IS_AN_OPTION expands to a boolean32 indicating whether or not the 
 * string passed in begins with a dash (-).
 */
#define IS_AN_OPTION(arg) (strncmp((arg), "-", 1) == 0)

void tty_sec_acl_cli(
#ifdef __STDC__
    int                         must_prompt,
    char                        *prompt,
    sec_acl_edit_mask_calc_t    *mask_calc_type_p
#endif
);

error_status_t tty_process_args(
#ifdef __STDC__
    int                         argc,
    char                        **argv,
    sec_acl_edit_mask_calc_t    *mask_calc_type
#endif
);

#endif /* tty_base__h_included */
