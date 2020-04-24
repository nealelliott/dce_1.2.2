/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: hsearch.c,v $
 * Revision 1.1.6.2  1996/02/18  19:21:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:06:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  21:36:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:08:08  root]
 * 
 * Revision 1.1.4.1  1993/12/22  14:00:55  bowe
 * 	Change info.cachesize = NULL to 0, since it's an int, not a pointer.
 * 	[1993/12/22  14:00:20  bowe]
 * 
 * Revision 1.1.2.1  1993/12/01  22:30:24  bowe
 * 	Initial checkin.
 * 	[1993/12/01  16:54:56  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Margo Seltzer.
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

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)hsearch.c	8.2 (Berkeley) 9/7/93";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>

#include <fcntl.h>
#include <string.h>

#define	__DBINTERFACE_PRIVATE
#include <db.h>
#include "search.h"

static DB *dbp = NULL;
static ENTRY retval;

extern int
hcreate(nel)
	u_int nel;
{
	HASHINFO info;

	info.nelem = nel;
	info.bsize = 256;
	info.ffactor = 8;
	info.cachesize = 0;
	info.hash = NULL;
	info.lorder = 0;
	dbp = (DB *)__hash_open(NULL, O_CREAT | O_RDWR, 0600, &info, 0);
	return ((int)dbp);
}

extern ENTRY *
hsearch(item, action)
	ENTRY item;
	ACTION action;
{
	DBT key, val;
	int status;

	if (!dbp)
		return (NULL);
	key.data = (u_char *)item.key;
	key.size = strlen(item.key) + 1;

	if (action == ENTER) {
		val.data = (u_char *)item.data;
		val.size = strlen(item.data) + 1;
		status = (dbp->put)(dbp, &key, &val, R_NOOVERWRITE);
		if (status)
			return (NULL);
	} else {
		/* FIND */
		status = (dbp->get)(dbp, &key, &val, 0);
		if (status)
			return (NULL);
		else
			item.data = (char *)val.data;
	}
	retval.key = item.key;
	retval.data = item.data;
	return (&retval);
}

extern void
hdestroy()
{
	if (dbp) {
		(void)(dbp->close)(dbp);
		dbp = NULL;
	}
}
