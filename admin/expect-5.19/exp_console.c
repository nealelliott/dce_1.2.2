/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: exp_console.c,v $
 * Revision 1.1.2.1  1996/08/09  18:37:35  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:37:17  arvind]
 *
 * $EndLog$
 */

/* exp_console.c - grab console.  This stuff is in a separate file to
avoid unpleasantness of AIX (3.2.4) .h files which provide no way to
reference TIOCCONS and include both sys/ioctl.h and sys/sys/stropts.h
without getting some sort of warning from the compiler.  The problem
is that both define _IO but only ioctl.h checks to see if it is
defined first.  This would suggest that it is sufficient to include
ioctl.h after stropts.h.  Unfortunately, ioctl.h, having seen that _IO
is defined, then fails to define other important things (like _IOW).

Written by: Don Libes, NIST, 2/6/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#include <expect_cf.h>
#include <stdio.h>
#include <sys/ioctl.h>

#ifdef HAVE_STRREDIR_H
#include <sys/strredir.h>
# ifdef SRIOCSREDIR
#  undef TIOCCONS
# endif
#endif

#ifdef HAVE_SYS_FCNTL_H
#include <sys/fcntl.h>
#endif

#include <tcl.h>
#include <exp_rename.h>
#include <exp_prog.h>
#include <exp_log.h>

static void
exp_console_manipulation_failed(s)
char *s;
{
	errorlog(stderr, "expect: spawn: cannot %s console, check permissions of /dev/console\n",s);
	exit(-1);
}

void
exp_console_set()
{
#ifdef SRIOCSREDIR
	int fd;

	if ((fd = open("/dev/console", O_RDONLY)) == -1) {
		exp_console_manipulation_failed("open");
	}
	if (ioctl(fd, SRIOCSREDIR, 0) == -1) {
		exp_console_manipulation_failed("redirect");
	}
	close(fd);
#endif

#ifdef TIOCCONS
	int on = 1;

	if (ioctl(0,TIOCCONS,(char *)&on) == -1) {
		exp_console_manipulation_failed("redirect");
	}
#endif /*TIOCCONS*/
}
