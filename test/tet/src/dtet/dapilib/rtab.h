/*
 *      SCCS:  @(#)rtab.h	1.2 (92/09/30) 
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

/************************************************************************

SCCS:   	@(#)rtab.h	1.2 92/09/30
NAME:		rtab.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	Structure of the remote process execution table -
	used to map API remoteids to sysid/pid pairs.

	An entry is made in the remote execution table for each process
	started successfully by tet_remexec().
	The pid is set to -1 once the process is waited for successfully by
	tet_remwait().

	Ideally, I would like to be able to remove a table entry when a
	process has been waited for, but the spec wants to distinguish between
	remoteid not started by tet_remexec() (EINVAL), and remoteid already
	waited for (ECHILD) -- so this table has to grow for ever.

MODIFICATIONS:

************************************************************************/

/*
**	Structure of the remote process execution table
**
**	The next and last pointers must be first so as to allow the use
**	of the llist routines to manipulate the table.
*/

struct rtab {
	struct rtab *rt_next;	/* ptr to next element in the list */
	struct rtab *rt_last;	/* ptr to last element in the list */
	long rt_magic;		/* magic number */
	int rt_remoteid;	/* remote id returned by tet_remexec() */
	int rt_sysid;		/* remote system id */
	long rt_pid;		/* pid on that system (-1 when waited for) */
};

#define RT_MAGIC	0x52746142	/* rtab magic number */


/* extern function declarations */

#if __STDC__

extern void rtadd(struct rtab *);
extern struct rtab *rtalloc();
extern struct rtab *rtfind(int);
extern void rtfree(struct rtab *);
extern void rtrm(struct rtab *);

#else /* __STDC__ */

extern void rtadd();
extern struct rtab *rtalloc();
extern struct rtab *rtfind();
extern void rtfree();
extern void rtrm();

#endif /* __STDC__ */

