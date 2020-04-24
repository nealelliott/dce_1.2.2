/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bsd44lite_login_tty.c,v $
 * Revision 1.1.2.1  1996/06/04  21:52:15  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:38 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/04/19  18:57 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 *
 * 	Add OSF/HP copyrights
 * 	[1996/04/19  18:57 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/4]
 *
 * $EndLog$
 */
/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)login_tty.c	8.1 (Berkeley) 6/4/93";
#endif /* LIBC_SCCS and not lint */

#include <sys/param.h>
#ifdef POSIX_TERMIOS
#include <termios.h>
#else
#include <sys/ioctl.h>
#endif

login_tty(fd)
	int fd;
{
#ifdef POSIX_TERMIOS
	struct termios new_termio;
#endif
	(void) setsid();
#ifdef POSIX_TERMIOS
        tcgetattr(fd,&new_termio);
#ifdef OSF_DCE
        new_termio.c_iflag &= ~INPCK;
        new_termio.c_iflag |= ICRNL|IXON;
        new_termio.c_oflag |= OPOST|ONLCR|TAB3;
        new_termio.c_oflag &= ~ONLRET;
        new_termio.c_lflag |= (ECHO|ECHOE|ECHOK|ISIG|ICANON);
        new_termio.c_cflag &= ~PARENB;
        new_termio.c_cflag |= CS8;
        new_termio.c_cc[VMIN] = 1;
        new_termio.c_cc[VTIME] = 0;
        new_termio.c_cc[VEOF] = 4;
        tcsetattr(fd, TCSAFLUSH, &new_termio);
#else
        new_termio.c_lflag &=  ~(ICANON|ECHO|ISIG|IEXTEN);
        /* so that login can read the authenticator */
        new_termio.c_iflag &= ~(IXON|IXANY|BRKINT|INLCR|ICRNL|ISTRIP);
        /* new_termio.c_iflag = 0; */
        /* new_termio.c_oflag = 0; */
        new_termio.c_cc[VMIN] = 1;
        new_termio.c_cc[VTIME] = 0;
        tcsetattr(fd,TCSANOW,&new_termio);
#endif
#else
	if (ioctl(fd, TIOCSCTTY, (char *)NULL) == -1)
		return (-1);
#endif
	(void) dup2(fd, 0);
	(void) dup2(fd, 1);
	(void) dup2(fd, 2);
	if (fd > 2)
		(void) close(fd);
	return (0);
}
