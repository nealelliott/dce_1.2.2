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
 * Revision 1.1.6.2  1996/03/11  13:27:09  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:05  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:01:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:28  root]
 * 
 * Revision 1.1.4.1  1994/03/25  17:05:39  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  17:04:51  sekhar]
 * 
 * Revision 1.1.2.2  1992/12/29  12:41:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:44  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_base.h V=13 11/21/91 //littl/prgy/src/edrgy
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
#include <dce/rgybase.h>
#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>

/* IS_AN_OPTION expands to a boolean32 indicating whether or not the 
 * string passed in begins with a dash (-).
*/
#define IS_AN_OPTION(arg) (strncmp((arg), "-", 1) == 0)

#endif /* tty_base__h_included */
