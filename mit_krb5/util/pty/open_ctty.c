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
 * $Log: open_ctty.c,v $
 * Revision 1.1.2.1  1996/06/05  21:26:41  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:15:35  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * pty_open_ctty: Open and establish controlling terminal.
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
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

/* 
 * This routine will be called twice.  It's not particularly important
 * that the setsid() or TIOCSTTY ioctls succeed (they may not the
 * second time), but rather that we have a controlling terminal at the
 * end.  It is assumed that vhangup doesn't exist and confuse the
 * process's notion of controlling terminal on any system without
 * TIOCNOTTY.  That is, either vhangup() leaves the controlling
 * terminal in tact, breaks the association completely, or the system
 * provides TIOCNOTTY to get things back into a reasonable state.  In
 * practice, vhangup() either breaks the association completely or
 * doesn't effect controlling terminals, so this condition is met.
 */
long
pty_open_ctty (slave, fd)
    const char * slave;
    int *fd;
{
    int testfd, retval;

/* First, dissociate from previous terminal */
    if ( (retval = ptyint_void_association()) != 0 )
	return retval;
#ifdef ultrix
    /* The Ultrix (and other BSD tty drivers) require the process group
     * to be zero, in order to acquire the new tty as a controlling tty. */
    (void) setpgrp(0, 0);
#endif

    *fd = open(slave, O_RDWR);
    if (*fd < 0 )
	return PTY_OPEN_SLAVE_OPENFAIL;
#ifdef ultrix
    setpgrp(0, getpid());
#endif
#ifdef ultrix
    setpgrp(0, getpid());
#endif

#ifdef TIOCSCTTY
    ioctl(*fd, TIOCSCTTY, 0); /* Don't check return.*/
#endif /* TIOCSTTY */

    testfd = open("/dev/tty", O_RDWR|O_NDELAY);
    if ( testfd < 0 )
	{
	    close(*fd);
	    *fd = -1;
	    return PTY_OPEN_SLAVE_NOCTTY;
	}
    close(testfd);
    return 0;
}
