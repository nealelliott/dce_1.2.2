/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_def.h,v $
 * Revision 1.1.4.2  1996/03/11  13:27:14  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:01:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:33  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:41:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_def.h V=4 04/29/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      tty defaults management
** 
*/

#ifndef tty_def__h_included
#define tty_def__h_included

#include <dce/nbase.h>
 
#include <commands.h>

error_status_t tty_display_defaults ( 
#ifdef __STDC__ 
    void
#endif
);
  
error_status_t tty_do_defaults (
#ifdef __STDC__
    command_buf  *c
#endif
);

#endif
