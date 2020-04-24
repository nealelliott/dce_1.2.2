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
 * 
 */
/*
 * HISTORY
 * $Log: bt_close.c,v $
 * Revision 1.1.2.1  1996/10/03  20:19:34  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:15:04  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:19 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:35 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:31:58  zee]
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
static char sccsid[] = "@(#)bt_close.c	8.7 (Berkeley) 8/17/94";
#endif /* LIBC_SCCS and not lint */

#include <sys/param.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <db.h>
#include "btree.h"

/*
 * BT_CLOSE -- Close a btree.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS
 */
int
__bt_close(dbp)
	DB *dbp;
{
	BTREE *t;
	int fd;
	TS_BTREE *ts;

	
	t = dbp->internal;

	/* Sync the tree. */
	if (__bt_sync(dbp, 0) == RET_ERROR)
		return (RET_ERROR);

	/* Close the memory pool. */
	if (mpool_close(t->bt_mp) == RET_ERROR)
		return (RET_ERROR);


/*  G-L-O-R-I-A - TXN CHANGES *** BEGIN */
	/*
	 * Destroy the mutex which guards the tree
	 */
	TREE_LOCK(t) {
	    for (ts = (TS_BTREE *)t->bt_tq.cqh_first; 
		 ts != (TS_BTREE *)&t->bt_tq; 
		 ts = ((ts_qentry *)ts)->cqe_next) {
		ts->bt_closed = TRUE;
	    }
	} TREE_UNLOCK(t);
	(void)pthread_mutex_destroy (&t->bt_mutex);

	/*
	 * Close page table files
	 */
	if (t->bt_pt0_fd != NO_FD) (void)close (t->bt_pt0_fd);
	if (t->bt_pt1_fd != NO_FD) (void)close (t->bt_pt1_fd);

/*  G-L-O-R-I-A - TXN CHANGES *** END */
	fd = t->bt_fd;
	free(t);
	free(dbp);
	return (close(fd) ? RET_ERROR : RET_SUCCESS);
}

/*
 * BT_SYNC -- sync the btree to disk.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *
 * Returns:
 *	RET_SUCCESS, RET_ERROR.
 */
int
__bt_sync(dbp, flags)
	const DB *dbp;
	u_int flags;
{
	BTREE *t;
	int status;
	TS_BTREE *ts;

	
	t = dbp->internal;
	GET_THREAD(t,ts);

	/* Toss any page pinned across calls. */
	if (ts->bt_pinned != NULL) {
		mpool_put(t->bt_mp, ts->bt_pinned, 0);
		ts->bt_pinned = NULL;
	}

	/* Sync doesn't currently take any flags. */
	if (flags != 0) {
		errno = EINVAL;
		return (RET_ERROR);
	}

	if (F_ISSET(t, B_INMEM | B_RDONLY) || !F_ISSET(t, B_MODIFIED))
		return (RET_SUCCESS);

	if (F_ISSET(t, B_METADIRTY) && bt_meta(t) == RET_ERROR)
		return (RET_ERROR);

	if ((status = mpool_sync(t->bt_mp)) == RET_SUCCESS)
		F_CLR(t, B_MODIFIED);

	return (status);
}

/*
 * BT_META -- write the tree meta data to disk.
 *
 * Parameters:
 *	t:	tree
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS
 */
int
bt_meta(t)
	BTREE *t;
{
	BTMETA m;
	void *p;

	if ((p = mpool_get(t->bt_mp, P_META, 0)) == NULL)
		return (RET_ERROR);

	/* Fill in metadata. */
/*  G-L-O-R-I-A - TXN CHANGES *** BEGIN */
	m.current_pt = t->bt_mp->current;
/*  G-L-O-R-I-A - TXN CHANGES *** END */
	m.magic = BTREEMAGIC;
	m.version = BTREEVERSION;
	m.psize = t->bt_psize;
	m.nrecs = t->bt_nrecs;
	m.flags = F_ISSET(t, SAVEMETA);

	TREE_LOCK(t) {
	   m.free = t->bt_free;
	   F_CLR(t, B_METADIRTY); 
	} TREE_UNLOCK(t); 

	memmove(p, (char *)&m, sizeof(BTMETA));
	if (mpool_put(t->bt_mp, p, MPOOL_DIRTY) != RET_SUCCESS) {
	   TREE_LOCK(t) {
	      F_SET(t, B_METADIRTY);
	   } TREE_UNLOCK(t);
	   return (RET_ERROR);
	}

	return (RET_SUCCESS);
}

/*  G-L-O-R-I-A - TXN CHANGES *** BEGIN */
/*
 * BT_CLOSE_AND_DELETE -- Close a btree and delete it from disk.
 *
 * Parameters:
 *	dbp:	pointer to access method
 *    fname:    base filename to delete
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS
 */
int
__bt_close_and_delete(DB         *dbp,
		      const char *fname)
{
    int status;

    status = __bt_close(dbp);
    if (status == RET_SUCCESS) {
	if (fname != NULL) {
	    char tmp_fname[MAXPATHLEN];

	    /* Delete btree file and associated page table files */
	    (void)unlink(fname);

	    strcpy (tmp_fname, fname);
	    strcat (tmp_fname, "_pt0");
	    (void)unlink(tmp_fname);

	    strcpy (tmp_fname, fname);
	    strcat (tmp_fname, "_pt1");
	    (void)unlink(tmp_fname);
	}
    }
    return status;
}
/*  G-L-O-R-I-A - TXN CHANGES *** END */
