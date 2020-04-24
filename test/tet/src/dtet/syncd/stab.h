/*
 *      SCCS:  @(#)stab.h	1.3 (92/11/16) 
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

SCCS:   	@(#)stab.h	1.3 92/11/16
NAME:		stab.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	sync event table description

MODIFICATIONS:

************************************************************************/

/*
**	Sync event table.
**
**	An entry in the sync table is made for each sync event that may be
**	waited on by client processes.
**	The table is maintained and accessed by routines in stab.c.
**
**	Each process that requests a sync event stores request parameters in
**	the server-specific part of its process table entry.
**
**	It is possible for processes to request the same sync event but with
**	different sync numbers.
**	The event ocurs as soon as all participating processes have voted but,
**	if all processes vote YES, only those who have specified the lowest
**	non-zero sync number are notified.
**	In this case, the sync table element is retained for reuse when the
**	processes next vote on the same event (with a higher sync number).
*/

/* per-user details structure for the sync event table */
struct ustab {
	int us_sysid;		/* system id */
	int us_ptype;		/* process id */
	int us_state;		/* process state (defined in synreq.h) */
	long us_spno;		/* sync request spno parameter */
	int us_ntimedout;	/* no of times this user has caused this
				   event to time out */
	struct ptab *us_ptab;	/* ptr to user's ptab */
};

/*
**	sync event table - this is a linked list
**
**	the next and last pointers must be first so as to allow the
**	use of the llist routines to manipulate the table
*/

struct stab {
	struct stab *st_next;		/* ptr to next element in list */
	struct stab *st_last;		/* ptr to last element in list */
	long st_snid;			/* id for auto sync request */
	long st_xrid;			/* xres id for user syncs */
	long st_lastspno;		/* spno of last event */
	struct ptab *st_ptab;		/* ptr to owner's ptab (a/sync) */
	struct ustab *st_ud;		/* ptr to per-user details */
	int st_nud;			/* no of valid elements in st_ud */
	int st_udlen;			/* length of st_ud */
	int st_flags;			/* flags - see below */
};

/* values for st_flags (a bit field) */
#define SF_ATTENTION	001		/* entry needs servicing */
#define SF_INPROGRESS	002		/* sync is in progress */
#define SF_OK		004		/* sync has happend successfully */
#define	SF_ERR		010		/* sync has happend unsuccessfully */
#define SF_TIMEDOUT	020		/* proc timed out in sync event */
#define SF_USYNC	040		/* entry is a user sync, else auto */

/* syncd server-specific process table data - pointed to by ptab.pt_sdata */
struct sptab {
	struct stab *sp_stab;		/* sync event being waited on */
	long sp_timeout;		/* timeout from sync request */
};


/* extern function declarations */

#if __STDC__

extern void stadd(struct stab *);
extern struct stab *stafind(long);
extern struct stab *stalloc();
extern void stcheck(struct stab *);
extern void stfree(struct stab *);
extern void strm(struct stab *);
extern void stuend(int);
extern struct stab *stufind(long, struct ustab *, int);
extern int ustalloc(struct stab *, int);

#else /* __STDC__ */

extern void stadd();
extern struct stab *stafind();
extern struct stab *stalloc();
extern void stcheck();
extern void stfree();
extern void strm();
extern void stuend();
extern struct stab *stufind();
extern int ustalloc();

#endif

