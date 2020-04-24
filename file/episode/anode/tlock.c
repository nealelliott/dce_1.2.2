/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tlock.c,v $
 * Revision 1.1.86.1  1996/10/02  17:19:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:04  damon]
 *
 * $EndLog$
 */

/* Copyright (C) 1995, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>

#include "tlock.h"
#include "anode.h"

#define DEBUG_THIS_FILE EPIA_DEBUG_TLOCK

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/tlock.c,v 1.1.86.1 1996/10/02 17:19:31 damon Exp $")

/* tlock_Init -- Initialize a \arg{tlock}. */

/* EXPORT */
void tlock_Init(struct tlock_data *tlock, const char *name)
{
    lock_InitDesc (&tlock->lock, name);
    tlock->trans = (elbb_tranRec_t) 0;
}

/* tmutex_Init -- Initialize a \arg{tmutex}. */

/* EXPORT */
void tmutex_Init(struct tmutex_data *tlock, const char *name)
{
    epi_mutex_init (&tlock->lock, name);
    tlock->trans = (elbb_tranRec_t) 0;
}

static void RecycleTrans (elbb_tranRec_t *transP)
{
    if (!elbb_IsZeroTrans(*transP)) {
	long code;
	code = elbb_LogForce(*transP, 2/*complete*/);
	afsl_MBZ(code);
	*transP = elbb_zeroTrans;
    }
}

/* EXPORT */
void tlock_Recycle (struct tlock_data *tlock)
{
    RecycleTrans(&tlock->trans);
}

/* tlock_Destroy -- Destroy a \arg{tlock}. */

void tlock_Destroy(tlock)
  IN struct tlock_data *tlock;
{
    RecycleTrans (&tlock->trans);
    lock_Destroy (&tlock->lock);
}

/* tmutex_Destroy -- Destroy a \arg{tmutex}. */

void tmutex_Destroy(tlock)
  IN struct tmutex_data *tlock;
{
    RecycleTrans (&tlock->trans);
    epi_mutex_destroy (&tlock->lock);
}

/* tlock_Equiv -- Performs the non-trivial operations needed by the tlock
 *     macros.  The \arg{op} specifies the required operation (e.g.
 *     \cst{TLOCK_OBTAIN_WRITE} or \cst{TLOCK_RELEASE_READ}).  This procedure
 *     is always invoked via one of the following marcos. */

/* EXPORT */
void tlock_Equiv (op, tlock, trans)
  IN int op;
  IN struct tlock_data *tlock;
  IN elbb_tranRec_t trans;
{
    if (op & TLOCK_BEFORE) switch (op) {
      case TLOCK_OBTAIN_READ:
	lock_ObtainRead_r (&tlock->lock);
	break;
      case TLOCK_OBTAIN_WRITE:
	lock_ObtainWrite_r (&tlock->lock);
	break;
   /* case TMUTEX_OBTAIN: is unused. */
      default:
	afsl_PAssert (0, ("Bogus TLOCK op=%d", op));
    }
    /* This will do the right thing if either trans is zero. */
    if (trans) {
	elbb_tranRec_t *tlockTransP;
	if (op & TMUTEX_REALLY)
	    tlockTransP = &((struct tmutex_data *)tlock)->trans;
	else
	    tlockTransP = &tlock->trans;
	elbb_EquivTran (*tlockTransP, trans, (unsigned long) &tlock->lock);
	if (op & TLOCK_ASSIGN)
	    *tlockTransP = trans;
    }

    if (!(op & TLOCK_BEFORE)) switch (op) {
      case TLOCK_EQUIV:
      case TLOCK_EQUIV_ASSIGN:
      case TMUTEX_EQUIV:
      case TMUTEX_EQUIV_ASSIGN:
	break;
   /* case TLOCK_RELEASE_READ: is unused. */
      case TLOCK_RELEASE_WRITE:
	lock_ReleaseWrite_r (&tlock->lock);
	break;
      case TMUTEX_RELEASE:
	epi_mutex_unlock (&((struct tmutex_data *)tlock)->lock);
	break;
      default:
	afsl_PAssert (0, ("Bogus TLOCK op=%d", op));
	break;
    }
}

/*
 * Get two tlocks "simultaneously".  We first get one lock, then
 * try to get the other without blocking.  If this fails, we drop
 * the first lock and try again in the reverse order.  Used by
 * efs_rename and efs_link.
 */
void
tlock_ObtainDoubleWriteNT(
  struct tlock_data *tlock1,
  struct tlock_data *tlock2)
{
    struct tlock_data *temp;
    int have_both;

    tlock_ObtainWriteNT(tlock1);
    have_both = (tlock1 == tlock2);

    while (!have_both) {
	have_both = tlock_ObtainWriteNoBlockNT(tlock2);
	if (!have_both) {
	    tlock_ReleaseWriteNT(tlock1);
	    tlock_ObtainWriteNT(tlock2);
	    temp = tlock1;
	    tlock1 = tlock2;
	    tlock2 = temp;
	}
    }
}
