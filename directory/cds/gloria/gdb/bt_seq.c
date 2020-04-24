/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * @DEC_COPYRIGHT@
 * G-L-O-R-I-A modifications:
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: bt_seq.c,v $
 * Revision 1.1.2.1  1996/10/03  20:21:53  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:17:03  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:21 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:35 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:47:30  zee]
 * 
 * $EndLog$
 */
/*-
 * Copyright (c) 1990, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Olson.
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
static char sccsid[] = "@(#)bt_seq.c	8.7 (Berkeley) 7/20/94";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include <db.h>
#include <btree.h>

/* just for debugging... */
#include <dce/dce.h>
#include <dcecdssvc.h>

static int __bt_first __P((BTREE *, TS_BTREE *, const DBT *, EPG *, int *));
static int __bt_seqadv __P((BTREE *, TS_BTREE *, EPG *, int));
static int __bt_seqset __P((BTREE *, TS_BTREE *, EPG *, DBT *, int));

/*
 * Sequential scan support.
 *
 * The tree can be scanned sequentially, starting from either end of the
 * tree or from any specific key.  A scan request before any scanning is
 * done is initialized as starting from the least node.
 */

/*
 * __bt_seq --
 *	Btree sequential scan interface.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *	key:	key for positioning and return value
 *	data:	data return value
 *	flags:	R_CURSOR, R_FIRST, R_LAST, R_NEXT, R_PREV.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
int
__bt_seq(dbp, key, data, flags)
	const DB *dbp;
	DBT *key, *data;
	u_int flags;
{
	BTREE *t;
	EPG e;
	int status;
	TS_BTREE *ts;


	t = dbp->internal;

	GET_THREAD(t,ts);

	/* Toss any page pinned across calls. */
	if (ts->bt_pinned != NULL) {
		mpool_put(t->bt_mp, ts->bt_pinned, 0);
		ts->bt_pinned = NULL;
	}

	/*
	 * If scan unitialized as yet, or starting at a specific record, set
	 * the scan to a specific key.  Both __bt_seqset and __bt_seqadv pin
	 * the page the cursor references if they're successful.
	 */
	switch (flags) {
	case R_NEXT:
	case R_PREV:
		if (F_ISSET(&ts->bt_cursor, CURS_INIT)) {
			status = __bt_seqadv(t, ts, &e, flags);
			break;
		}
		/* FALLTHROUGH */
	case R_FIRST:
	case R_LAST:
	case R_CURSOR:
/*** G-L-O-R-I-A - TXN CHANGES -- BEGIN ***/
	case R_ECURSOR:
/*** G-L-O-R-I-A - TXN CHANGES -- END ***/
		status = __bt_seqset(t, ts, &e, key, flags);
		break;
	default:
		errno = EINVAL;
		return (RET_ERROR);
	}

	if (status == RET_SUCCESS) {
/*** G-L-O-R-I-A - GDB CHANGES -- BEGIN ***/
	/*
         * This change allows us to call __bt_seq and get back a 
	 * buffer that we (the caller) owns and can free when we
	 * are done.
         */
	        DBT tmp_data;
	        DBT tmp_key;
		tmp_data.data = NULL;
		tmp_data.size = 0;
		tmp_key.data = NULL;
		tmp_key.size = 0;

		__bt_setcur(t, e.page->pgno, e.index);

		status =
		  __bt_ret(t, &e, key, &tmp_key, data, &tmp_data, 0);

/*** G-L-O-R-I-A - TXN CHANGES -- BEGIN ***/

		/*
		 * If the user is doing concurrent access, we copied the
		 * key/data, toss the page.
		 */
		if (F_ISSET(t, B_DB_LOCK)) {
		    mpool_put(t->bt_mp, e.page, 0);
		}
		else {
			ts->bt_pinned = e.page;
		}
	}

	return (status);
}

/*
 * __bt_seqset --
 *	Set the sequential scan to a specific key.
 *
 * Parameters:
 *	t:	tree
 *	ep:	storage for returned key
 *	key:	key for initial scan position
 *	flags:	R_CURSOR, R_FIRST, R_LAST, R_NEXT, R_PREV
 *
 * Side effects:
 *	Pins the page the cursor references.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
static int
__bt_seqset(t, ts, ep, key, flags)
	BTREE *t;
	TS_BTREE *ts;
	EPG *ep;
	DBT *key;
	int flags;
{
	PAGE *h;
	pgno_t pg;
	int exact;
        int status;


	/*
	 * Find the first, last or specific key in the tree and point the
	 * cursor at it.  The cursor may not be moved until a new key has
	 * been found.
	 */
	switch (flags) {
/*** G-L-O-R-I-A - TXN CHANGES -- BEGIN ***/
	case R_ECURSOR:	      /* Keyed scan. Exact match only. */
		/*
		 * Find the first instance of the key or the smallest key
		 * which is greater than or equal to the specified key.
		 */
		if (key->data == NULL || key->size == 0) {
		    errno = EINVAL;
		    return (RET_ERROR);
		}
		status = __bt_first(t, ts, key, ep, &exact);

		/*
		 * Only return it if it's an exact match
		 */
		if (status == RET_SUCCESS && !exact) {
		   mpool_put(t->bt_mp, ep->page, 0);
		   return (RET_SPECIAL);
		}

		return (status);

/*** G-L-O-R-I-A - TXN CHANGES -- END ***/

	case R_CURSOR:				/* Keyed scan. */
		/*
		 * Find the first instance of the key or the smallest key
		 * which is greater than or equal to the specified key.
		 */
		if (key->data == NULL || key->size == 0) {
			errno = EINVAL;
			return (RET_ERROR);
		}
		return (__bt_first(t, ts, key, ep, &exact));

	case R_FIRST:				/* First record. */
	case R_NEXT:
		/* Walk down the left-hand side of the tree. */
		for (pg = P_ROOT;;) {
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL) {
			    return (RET_ERROR);
			    }

			/* Check for an empty tree. */
			if (NEXTINDEX(h) == 0) {
				mpool_put(t->bt_mp, h, 0);
				return (RET_SPECIAL);
			}

			if (h->flags & (P_BLEAF | P_RLEAF))
				break;
			pg = GETBINTERNAL(h, 0)->pgno;
			mpool_put(t->bt_mp, h, 0);
		}
		ep->page = h;
		ep->index = 0;
		break;
	case R_LAST:				/* Last record. */
	case R_PREV:
		/* Walk down the right-hand side of the tree. */
		for (pg = P_ROOT;;) {
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL) {
				return (RET_ERROR);
			    }
			/* Check for an empty tree. */
			if (NEXTINDEX(h) == 0) {
				mpool_put(t->bt_mp, h, 0);
				return (RET_SPECIAL);
			}

			if (h->flags & (P_BLEAF | P_RLEAF))
				break;
			pg = GETBINTERNAL(h, NEXTINDEX(h) - 1)->pgno;
			mpool_put(t->bt_mp, h, 0);
		}

		ep->page = h;
		ep->index = NEXTINDEX(h) - 1;
		break;
	}
	
	return (RET_SUCCESS);
}

/*
 * __bt_seqadv --
 *	Advance the sequential scan.
 *
 * Parameters:
 *	t:	tree
 *	flags:	R_NEXT, R_PREV
 *
 * Side effects:
 *	Pins the page the new key/data record is on.
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS or RET_SPECIAL if there's no next key.
 */
__bt_seqadv(t, ts, ep, flags)
	BTREE *t;
	TS_BTREE *ts;
	EPG *ep;
	int flags;
{
	CURSOR *c;
	PAGE *h;
	indx_t index;
	pgno_t pg;
	int exact;


	/*
	 * There are a couple of states that we can be in.  The cursor has
	 * been initialized by the time we get here, but that's all we know.
	 */
	c = &ts->bt_cursor;

	/*
	 * The cursor was deleted where there weren't any duplicate records,
	 * so the key was saved.  Find out where that key would go in the
	 * current tree.  It doesn't matter if the returned key is an exact
	 * match or not -- if it's an exact match, the record was added after
	 * the delete so we can just return it.  If not, as long as there's
	 * a record there, return it.
	 */
	if (F_ISSET(c, CURS_ACQUIRE)) {
		return (__bt_first(t, ts, &c->key, ep, &exact));
        }

	/* Get the page referenced by the cursor. */
	if ((h = mpool_get(t->bt_mp, c->pg.pgno, 0)) == NULL) {
		return (RET_ERROR);
	}
	/*
 	 * Find the next/previous record in the tree and point the cursor at
	 * it.  The cursor may not be moved until a new key has been found.
	 */
	switch (flags) {
	case R_NEXT:			/* Next record. */
		/*
		 * The cursor was deleted in duplicate records, and moved
		 * forward to a record that has yet to be returned.  Clear
		 * that flag, and return the record.
		 */
		if (F_ISSET(c, CURS_AFTER))
			goto usecurrent;
		index = c->pg.index;
		if (++index == NEXTINDEX(h)) {
			pg = h->nextpg;
			mpool_put(t->bt_mp, h, 0);
			if (pg == P_INVALID) {
			    return (RET_SPECIAL);
			    }
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL) {
			    return (RET_ERROR);
			    }
			index = 0;
		}
		break;
	case R_PREV:			/* Previous record. */
		/*
		 * The cursor was deleted in duplicate records, and moved
		 * backward to a record that has yet to be returned.  Clear
		 * that flag, and return the record.
		 */
		if (F_ISSET(c, CURS_BEFORE)) {
usecurrent:		F_CLR(c, CURS_AFTER | CURS_BEFORE);
			ep->page = h;
			ep->index = c->pg.index;
			return (RET_SUCCESS);
		}
		index = c->pg.index;
		if (index == 0) {
			pg = h->prevpg;
			mpool_put(t->bt_mp, h, 0);
			if (pg == P_INVALID) {
			    return (RET_SPECIAL);
			    }
			if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL) {
			    return (RET_ERROR);
			    }
			index = NEXTINDEX(h) - 1;
		} else
			--index;
		break;
	}

	ep->page = h;
	ep->index = index;

	return (RET_SUCCESS);
}

/*
 * __bt_first --
 *	Find the first entry.
 *
 * Parameters:
 *	t:	the tree
 *    key:	the key
 *  erval:	return EPG
 * exactp:	pointer to exact match flag
 *
 * Returns:
 *	The first entry in the tree greater than or equal to key,
 *	or RET_SPECIAL if no such key exists.
 */
static int
__bt_first(t, ts, key, erval, exactp)
	BTREE *t;
	TS_BTREE *ts;
	const DBT *key;
	EPG *erval;
	int *exactp;
{
	PAGE *h;
	EPG *ep, save;
	pgno_t pg;


	/*
	 * Find any matching record; __bt_search pins the page.
	 *
	 * If it's an exact match and duplicates are possible, walk backwards
	 * in the tree until we find the first one.  Otherwise, make sure it's
	 * a valid key (__bt_search may return an index just past the end of a
	 * page) and return it.
	 */
	if ((ep = __bt_search(t, ts, key, exactp)) == NULL) {
		return ((int)NULL);
	    }

	if (*exactp) {
		if (F_ISSET(t, B_NODUPS)) {
			*erval = *ep;
			return (RET_SUCCESS);
		}
			
		/*
		 * Walk backwards, as long as the entry matches and there are
		 * keys left in the tree.  Save a copy of each match in case
		 * we go too far.
		 */
		save = *ep;
		h = ep->page;
		do {
			if (save.page->pgno != ep->page->pgno) {
				mpool_put(t->bt_mp, save.page, 0);
				save = *ep;
			} else
				save.index = ep->index;

			/*
			 * Don't unpin the page the last (or original) match
			 * was on, but make sure it's unpinned if an error
			 * occurs.
			 */
			if (ep->index == 0) {
				if (h->prevpg == P_INVALID)
					break;
				if (h->pgno != save.page->pgno)
					mpool_put(t->bt_mp, h, 0);
				if ((h = mpool_get(t->bt_mp,
				    h->prevpg, 0)) == NULL) {
					if (h->pgno == save.page->pgno)
						mpool_put(t->bt_mp,
						    save.page, 0); 
					return (RET_ERROR);
				}
				ep->page = h;
				ep->index = NEXTINDEX(h);
			}
			--ep->index;
		} while (__bt_cmp(t, ts, key, ep) == 0);

		/*
		 * Reach here with the last page that was looked at pinned,
		 * which may or may not be the same as the last (or original)
		 * match page.  If it's not useful, release it.
		 */
		if (h->pgno != save.page->pgno)
			mpool_put(t->bt_mp, h, 0);

		*erval = save;
		return (RET_SUCCESS);
	}

	/* If at the end of a page, find the next entry. */
	if (ep->index == NEXTINDEX(ep->page)) {
		h = ep->page;
		pg = h->nextpg;
		mpool_put(t->bt_mp, h, 0);
		if (pg == P_INVALID) {
		    return (RET_SPECIAL);
		}
		if ((h = mpool_get(t->bt_mp, pg, 0)) == NULL) {
		    return (RET_ERROR);
		}
		ep->index = 0;
		ep->page = h;
	}
	*erval = *ep;

	return (RET_SUCCESS);
}

/*
 * __bt_setcur --
 *	Set the cursor to an entry in the tree.
 *
 * Parameters:
 *	t:	the tree
 *   pgno:	page number
 *  index:	page index
 */
void
__bt_setcur(t, pgno, index)
	BTREE *t;
	pgno_t pgno;
	u_int index;
{
   TS_BTREE *ts;


   /*
    * Get thread-specific data
    */
   GET_THREAD(t,ts);

   /*
    * Lose any already deleted key. 
    */
   if (ts->bt_cursor.key.data != NULL) {
      free(ts->bt_cursor.key.data);
      ts->bt_cursor.key.size = 0;
      ts->bt_cursor.key.data = NULL;
   }
   F_CLR(&ts->bt_cursor, CURS_ACQUIRE | CURS_AFTER | CURS_BEFORE);

   /*
    * Update the cursor. 
    */
   ts->bt_cursor.pg.pgno = pgno;
   ts->bt_cursor.pg.index = index;
   F_SET(&ts->bt_cursor, CURS_INIT);

}


