/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: lwputil.c,v $
 * Revision 1.1.9.1  1996/10/02  17:28:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:40  damon]
 *
 * Revision 1.1.4.1  1994/06/09  14:03:45  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:40  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  18:24:08  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  14:14:17  zeliff]
 * 
 * Revision 1.1.2.3  1992/11/24  16:56:51  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:26:33  bolinger]
 * 
 * Revision 1.1.2.2  1992/08/31  20:21:02  jaffe
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    These are utilities for the HP version of LWP (from AFS-3).
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  02:52:58  jaffe]
 * 
 * $EndLog$
 */
#include <dcedfs/param.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <utime.h>
/*
 * Emulate the vax instructions for queue insertion and deletion, somewhat.
 * A std_queue structure is defined here and used by these routines.  These
 * routines use caddr_ts so they can operate on any structure.  The std_queue
 * structure is used rather than proc structures so that when the proc struct
 * changes only process management code breaks.  The ideal solution would be
 * to define a std_queue as a global type which is part of all the structures
 * which are manipulated by these routines.  This would involve considerable
 * effort...
 */

struct std_queue {
	struct std_queue *q_link;
	struct std_queue *q_rlink;
}

insque(elementp, quep)
	caddr_t  elementp, quep;
{
#define element ( (struct std_queue *) elementp)
#define que ( (struct std_queue *) quep)
	element->q_link = que->q_link;
	element->q_rlink = que;

	que->q_link->q_rlink = element;
	que->q_link = element;
}

remque(elementp)
	caddr_t elementp;
{
	element->q_link->q_rlink = element->q_rlink;
	element->q_rlink->q_link = element->q_link;

	element->q_rlink = element->q_link = (struct std_queue *) 0;
}
#undef element
#undef que
