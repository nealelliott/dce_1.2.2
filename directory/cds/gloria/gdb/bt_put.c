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
 * $Log: bt_put.c,v $
 * Revision 1.1.2.1  1996/10/03  20:21:22  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:16:38  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:21 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:35 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:47:14  zee]
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
static char sccsid[] = "@(#)bt_put.c	8.8 (Berkeley) 7/26/94";
#endif /* LIBC_SCCS and not lint */

#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <db.h>
#include <btree.h>

static EPG *
bt_fast __P((BTREE *, TS_BTREE *, const DBT *, const DBT *, int *));

/*
 * __BT_PUT -- Add a btree item to the tree.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *	key:	key
 *	data:	data
 *	flag:	R_NOOVERWRITE
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS and RET_SPECIAL if the key is already in the
 *	tree and R_NOOVERWRITE specified.
 */
int
__bt_put(dbp, key, data, flags)
	const DB *dbp;
	DBT *key;
	const DBT *data;
	u_int flags;
{
	BTREE *t;
	DBT tkey, tdata;
	EPG *e;
	PAGE *h;
	indx_t index, nxtindex;
	pgno_t pg;
	u_int32_t nbytes;
	int dflags, exact, status;
	char *dest, db[NOVFLSIZE], kb[NOVFLSIZE];
	TS_BTREE *ts;


	t = dbp->internal;

	GET_THREAD(t,ts);

	/* Toss any page pinned across calls. */
	if (ts->bt_pinned != NULL) {
		mpool_put(t->bt_mp, ts->bt_pinned, 0);
		ts->bt_pinned = NULL;
	}

	/* Check for change to a read-only tree. */
	if (F_ISSET(t, B_RDONLY)) {
		errno = EPERM;
		return (RET_ERROR);
	}

	switch (flags) {
	case 0:
	case R_NOOVERWRITE:
		break;
	case R_CURSOR:
		/*
		 * If flags is R_CURSOR, put the cursor.  Must already
		 * have started a scan and not have already deleted it.
		 */
		if (F_ISSET(&ts->bt_cursor, CURS_INIT) &&
		    !F_ISSET(&ts->bt_cursor,		
		        CURS_ACQUIRE | CURS_AFTER | CURS_BEFORE))
			break;
		/* FALLTHROUGH */
	default:
		errno = EINVAL;
		return (RET_ERROR);
	}

	/*
	 * If the key/data pair won't fit on a page, store it on overflow
	 * pages.  Only put the key on the overflow page if the pair are
	 * still too big after moving the data to an overflow page.
	 *
	 * XXX
	 * If the insert fails later on, the overflow pages aren't recovered.
	 */
	dflags = 0;
	if (key->size + data->size > t->bt_ovflsize) {
		if (key->size > t->bt_ovflsize) {
storekey:		if (__ovfl_put(t, key, &pg) == RET_ERROR)
				return (RET_ERROR);
			tkey.data = kb;
			tkey.size = NOVFLSIZE;
			memmove(kb, &pg, sizeof(pgno_t));
			memmove(kb + sizeof(pgno_t),
			    &key->size, sizeof(u_int32_t));
			dflags |= P_BIGKEY;
			key = &tkey;
		}
		if (key->size + data->size > t->bt_ovflsize) {
			if (__ovfl_put(t, data, &pg) == RET_ERROR)
				return (RET_ERROR);
			tdata.data = db;
			tdata.size = NOVFLSIZE;
			memmove(db, &pg, sizeof(pgno_t));
			memmove(db + sizeof(pgno_t),
			    &data->size, sizeof(u_int32_t));
			dflags |= P_BIGDATA;
			data = &tdata;
		}
		if (key->size + data->size > t->bt_ovflsize)
			goto storekey;
	}

	/* 
         * Replace the cursor. 
         * We're relying on mpool_get to return an error....
         */
	if (flags == R_CURSOR) {
		if ((h =mpool_get(t->bt_mp, ts->bt_cursor.pg.pgno, 0)) == NULL) {
		   return (RET_ERROR);
		}
		index = ts->bt_cursor.pg.index;
		goto delete;
	}

	/*
	 * Find the key to delete, or, the location at which to insert.
	 * Bt_fast and __bt_search both pin the returned page.
	 */
	if (t->bt_order == NOT || (e = bt_fast(t, ts, key, data, &exact)) == NULL) {
	   if ((e = __bt_search(t, ts, key, &exact)) == NULL)
	     return (RET_ERROR);
	}
	h = e->page;
	index = e->index;

	/*
	 * Add the key/data pair to the tree.  If an identical key is already
	 * in the tree, and R_NOOVERWRITE is set, an error is returned.  If
	 * R_NOOVERWRITE is not set, the key is either added (if duplicates are
	 * permitted) or an error is returned.
	 */
	switch (flags) {
	case R_NOOVERWRITE:
		if (!exact)
			break;
		mpool_put(t->bt_mp, h, 0);
		return (RET_SPECIAL);
	default:
		if (!exact || !F_ISSET(t, B_NODUPS))
			break;
		/*
		 * !!!
		 * Note, the delete may empty the page, so we need to put a
		 * new entry into the page immediately.
		 */
delete:
		if (__bt_dleaf(t, ts, key, h, index) == RET_ERROR) {

			mpool_put(t->bt_mp, h, 0);
			return (RET_ERROR);
		}
		break;
	}

	/*
	 * If not enough room, or the user has put a ceiling on the number of
	 * keys permitted in the page, split the page.  The split code will
	 * insert the key and data and unpin the current page.  If inserting
	 * into the offset array, shift the pointers up.
	 */
	nbytes = NBLEAFDBT(key->size, data->size);
	if (h->upper - h->lower < nbytes + sizeof(indx_t)) {
		if ((status = __bt_split(t, h, key,
		    data, dflags, nbytes, index)) != RET_SUCCESS)
			return (status);
		goto success;
	}

	if (index < (nxtindex = NEXTINDEX(h)))
		memmove(h->linp + index + 1, h->linp + index,
		    (nxtindex - index) * sizeof(indx_t));
	h->lower += sizeof(indx_t);

	h->linp[index] = h->upper -= nbytes;
	dest = (char *)h + h->upper;
	WR_BLEAF(dest, key, data, dflags);

	/* If the cursor is on this page, adjust it as necessary. */
	if (F_ISSET(&ts->bt_cursor, CURS_INIT) &&
	    !F_ISSET(&ts->bt_cursor, CURS_ACQUIRE) &&
	    ts->bt_cursor.pg.pgno == h->pgno && 
	    ts->bt_cursor.pg.index >= index)
		    ++ts->bt_cursor.pg.index;
   /*
    * We don't need to lock the tree mutex while changing members below
    * because caller must hold the writelock to make this call.  
    * So, no other thread is reading or writing.
    */
	if (t->bt_order == NOT)
		if (h->nextpg == P_INVALID) {
			if (index == NEXTINDEX(h) - 1) {
				t->bt_order = FORWARD;
				t->bt_last.index = index;
				t->bt_last.pgno = h->pgno;
			}
		} else if (h->prevpg == P_INVALID) {
			if (index == 0) {
				t->bt_order = BACK;
				t->bt_last.index = 0;
				t->bt_last.pgno = h->pgno;
			}
		}

	mpool_put(t->bt_mp, h, MPOOL_DIRTY);

success:
	if (flags == R_SETCURSOR)
		__bt_setcur(t, e->page->pgno, e->index);

	F_SET(t, B_MODIFIED);
	return (RET_SUCCESS);
}

#ifdef STATISTICS
u_long bt_cache_hit, bt_cache_miss;
#endif

/*
 * BT_FAST -- Do a quick check for sorted data.
 *
 * Parameters:
 *	t:	tree
 *	key:	key to insert
 *
 * Returns:
 * 	EPG for new record or NULL if not found.
 */
static EPG *
bt_fast(t, ts, key, data, exactp)
	BTREE *t;
	TS_BTREE *ts;	
	const DBT *key, *data;
	int *exactp;
{
	PAGE *h;
	u_int32_t nbytes;
	int cmp;

	/* 
	 * THREADS:
	 * We don't need to lock the tree mutex while changing members  
	 * of t, because caller must hold the writelock to make this call.  
	 * So, no other thread is reading or writing.
	 */

	if ((h = mpool_get(t->bt_mp, t->bt_last.pgno, 0)) == NULL) {
		t->bt_order = NOT;
		return (NULL);
	}

	ts->bt_cur.page = h;
	ts->bt_cur.index = t->bt_last.index;

	/*
	 * If won't fit in this page or have too many keys in this page,
	 * have to search to get split stack.
	 */
	nbytes = NBLEAFDBT(key->size, data->size);
	if (h->upper - h->lower < nbytes + sizeof(indx_t))
		goto miss;

	if (t->bt_order == FORWARD) {
		if (ts->bt_cur.page->nextpg != P_INVALID)
			goto miss;
		if (ts->bt_cur.index != NEXTINDEX(h) - 1)
			goto miss;
		if ((cmp = __bt_cmp(t, ts, key, &ts->bt_cur)) < 0)
			goto miss;
		t->bt_last.index = cmp ? ++ts->bt_cur.index : ts->bt_cur.index;
	} else {
		if (ts->bt_cur.page->prevpg != P_INVALID)
			goto miss;
		if (ts->bt_cur.index != 0)
			goto miss;
		if ((cmp = __bt_cmp(t, ts, key, &ts->bt_cur)) > 0)
			goto miss;
		t->bt_last.index = 0;
	}
	*exactp = cmp == 0;
#ifdef STATISTICS
	++bt_cache_hit;
#endif /* STATISTICS */
	return (&ts->bt_cur);

miss:
#ifdef STATISTICS
	++bt_cache_miss;
#endif
	t->bt_order = NOT;
	mpool_put(t->bt_mp, h, 0);
	return (NULL);
}
