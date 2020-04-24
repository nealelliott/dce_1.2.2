/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: vhangup.c,v $
 * Revision 1.1.2.1  1996/06/05  21:27:50  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:16:25  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * pty_open_slave: open slave side of terminal, clearing for use.
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
 *
 * 
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that copyright notice and this permission
 * notice appear in supporting documentation, and that the name of
 * M.I.T. not be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 * 
 */

#include <com_err.h>
#include "libpty.h"
#include "pty-int.h"

void ptyint_vhangup()
{
#ifdef HAVE_VHANGUP
#ifdef POSIX_SIGNALS
    struct sigaction sa;
    /* Initialize "sa" structure. */
    (void) sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    
#endif

#ifdef POSIX_SIGNALS
	sa.sa_handler = SIG_IGN;
	(void) sigaction(SIGHUP, &sa, (struct sigaction *)0);
	vhangup();
	sa.sa_handler = SIG_DFL;
	(void) sigaction(SIGHUP, &sa, (struct sigaction *)0);
#else
	signal(SIGHUP, SIG_IGN);
	vhangup();
	signal(SIGHUP, SIG_DFL);
#endif
#endif
}
