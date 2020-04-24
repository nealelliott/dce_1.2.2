/*
 *      SCCS:  @(#)eaccess.c	1.3 (92/10/06) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

#ifndef lint
static char sccsid[] = "@(#)eaccess.c	1.3 (92/10/06) DTET release 1.0b";
#endif

/************************************************************************

SCCS:   	@(#)eaccess.c	1.3 92/10/06
NAME:		eaccess.c
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	function to check access permissions wrt effective user and group IDs

MODIFICATIONS:

************************************************************************/

#if __STDC__
#include <stdio.h>
#endif

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "dtetlib.h"


/*
**	tet_eaccess() - like access() but checks permissions wrt
**		effective user and group IDs
**
**	Note: this routine assumes that access and file modes are
**	encoded in the traditional way; ie: 4 = read, 2 = write, 1 = exec
**	user perms = 0700, group perms = 070, other perms = 07.
**
**	Since this routine provides support for the tccd OP_ACCESS
**	request (among other things) and thus needs to be able to
**	receive a mode argument from another system, it will be necessary
**	to implement a machine-independent mode transfer mechanism if
**	the above assumption is incorrect for a particular system.
*/

int tet_eaccess(path, mode)
char *path;
register int mode;
{
	struct stat stbuf;
	register int rc, euid;

	/* first check for things like non-existent file,
		read-only file system etc. */
	if (access(path, mode) < 0) {
		if (errno != EACCES)
			return(-1);
	}
	else if ((mode &= 07) == 0)
		return(0);

	/* here if access() succeeded, or failed because of wrong permissions;
		first get the file permissions */
	if (stat(path, &stbuf) < 0)
		return(-1);

	/* check the permissions wrt the euid and egid, treating root
		specially (like the kernel does) */
	rc = 0;
	if ((euid = geteuid()) == 0) {
		if ((stbuf.st_mode & S_IFMT) != S_IFDIR &&
			(stbuf.st_mode & 0111) == 0 && (mode & 01))
				rc = -1;
	}
	else if (stbuf.st_uid == euid) {
		if (((stbuf.st_mode >> 6) & mode) != mode)
			rc = -1;
	}
	else if (stbuf.st_gid == getegid()) {
		if (((stbuf.st_mode >> 3) & mode) != mode)
			rc = -1;
	}
	else {
		if ((stbuf.st_mode & mode) != mode)
			rc = -1;
	}

	if (rc < 0)
		errno = EACCES;
	return(rc);
}

