/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bsd44lite_logout.c,v $
 * Revision 1.1.2.1  1996/06/04  21:52:41  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:38 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 	[1996/04/19  18:57 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
 *
 * 	Add OSF/HP copyrights
 * 	[1996/04/19  18:57 UTC  mullan_s  /main/mullan_dce_k5_bsd44_rutils/3]
 *
 * $EndLog$
 */
/*
 * Copyright (c) 1988, 1993
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
static char sccsid[] = "@(#)logout.c	8.1 (Berkeley) 6/4/93";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>
#include <sys/time.h>

#include <fcntl.h>
#include <utmp.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#ifdef OSF_DCE
#if !defined(UTMP_FILE) && defined(_PATH_UTMP)
#define UTMP_FILE _PATH_UTMP
#endif

/* if it is *still* missing, assume SunOS */
#ifndef UTMP_FILE
#define UTMP_FILE       "/etc/utmp"
#endif
#endif

typedef struct utmp UTMP;

int
logout(line)
	register char *line;
{
	register int fd;
	UTMP ut;
	int rval;

#ifdef OSF_DCE
	if ((fd = open(UTMP_FILE, O_RDWR, 0)) < 0)
#else
	if ((fd = open(_PATH_UTMP, O_RDWR, 0)) < 0)
#endif
		return(0);
	rval = 0;
	while (read(fd, &ut, sizeof(UTMP)) == sizeof(UTMP)) {
#ifdef OSF_DCE
		if (!ut.ut_name[0] || strncmp(ut.ut_line, line, sizeof(ut.ut_line)))
#else
		if (!ut.ut_name[0] || strncmp(ut.ut_line, line, UT_LINESIZE))
#endif
			continue;
#ifdef OSF_DCE
		bzero(ut.ut_name, sizeof(ut.ut_name));
		bzero(ut.ut_host, sizeof(ut.ut_host));
#else
		bzero(ut.ut_name, UT_NAMESIZE);
		bzero(ut.ut_host, UT_HOSTSIZE);
#endif
		(void)time(&ut.ut_time);
#ifdef OSF_DCE
		(void)lseek(fd, -(off_t)sizeof(UTMP), SEEK_CUR);
#else
		(void)lseek(fd, -(off_t)sizeof(UTMP), L_INCR);
#endif
		(void)write(fd, &ut, sizeof(UTMP));
		rval = 1;
	}
	(void)close(fd);
	return(rval);
}
