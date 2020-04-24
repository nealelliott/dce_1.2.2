/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tty_keytab.h,v $
 * Revision 1.1.4.2  1996/03/11  13:27:20  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:13  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:01:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:30:41  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:42:09  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:05  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  tty_keytab.h V=1 07/19/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
**      Key Table manipulation - public interface declarations
*/

#ifndef tty_keytab__h_included
#define tty_keytab__h_included
 
#include <commands.h>

error_status_t tty_do_ktadd (
#ifdef __STDC__
    command_buf *c
#endif
);

error_status_t tty_keytab_remove (
#ifdef __STDC__
    command_buf *c
#endif
);

#endif /* tty_keytab__h_included */
