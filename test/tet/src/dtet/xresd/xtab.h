/*
 *      SCCS:  @(#)xtab.h	1.2 (92/09/30) 
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

SCCS:   	@(#)xtab.h	1.2 92/09/30
NAME:		xtab.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	execution results file table description

MODIFICATIONS:

************************************************************************/


/*
**	Execution results file table.
**
**	An element is allocated in the execution results file table for each
**	execution results file (xres file) opened by a call to OP_XROPEN.
**	Storage for an element is allocated by xtalloc() and freed by
**	xtfree().
**	An element is added to the table by xtadd() and removed by xtrm().
*/

/* per-user details structure for the execution results file table
	(really per-system since there can be more than one process per system)
*/
struct uxtab {
	int ux_sysid;			/* system id */
	struct ptab *ux_ptab;		/* ptr to first user's ptab entry */
	int ux_state;			/* process state - see below */
	int ux_result;			/* TP result code */
};

/* values for ux_state (discrete values) */
#define XS_NOTREPORTED	1
#define XS_REPORTED	2
#define XS_DEAD		3

/*
**	structure of the execution results file table
**
**	the next and last pointers must be first so as to allow the
**	use of the llist routines to manipulate the table
*/

struct xtab {
	struct xtab *xt_next;		/* ptr to next element in list */
	struct xtab *xt_last;		/* ptr to last element in list */
	long xt_xrid;			/* id for xres requests */
	struct ptab *xt_ptab;		/* ptr to owner's ptab */
	char *xt_xfname;		/* tet_xres file name */
	FILE *xt_xfp;			/* fp for tet_xres file */
	struct uxtab *xt_ud;		/* ptr to per-user details */
	int xt_nud;			/* no of active xt_ud elements */
	int xt_udlen;			/* no of bytes in xt_ud */
	int xt_icno;			/* current IC number */
	long xt_activity;		/* current TCC activity number */
	int xt_tpcount;			/* expected number of TPs in this IC */
	int xt_tpno;			/* current TP number */
	int xt_result;			/* TP result */
	int xt_flags;			/* flags - see below */
};

/* values for xt_flags (a bit field) */
#define XF_ICINPROGRESS		001	/* IC is in progress */
#define XF_ICDONE		002	/* IC finished */
#define XF_TPINPROGRESS		004	/* TP is in progress */
#define XF_TPDONE		010	/* TP finished */


/* extern function declarations */

#if __STDC__

extern int icend(struct xtab *);
extern int tpend(struct xtab *);
extern int uxtalloc(struct xtab *, int);
extern void xtadd(struct xtab *);
extern struct xtab *xtalloc();
extern struct xtab *xtfind(long);
extern void xtfree(struct xtab *);
extern void xtrm(struct xtab *);

#else /* __STDC__ */

extern int icend();
extern int tpend();
extern int uxtalloc();
extern void xtadd();
extern struct xtab *xtalloc();
extern struct xtab *xtfind();
extern void xtfree();
extern void xtrm();

#endif /* __STDC__ */

