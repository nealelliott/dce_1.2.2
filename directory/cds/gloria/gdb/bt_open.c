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
 * $Log: bt_open.c,v $
 * Revision 1.1.2.1  1996/10/03  20:20:43  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:16:00  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:20 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:35 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	Add caller specified key and data display routines
 * 	[1996/06/28  17:06:26  farrell]
 * 	 *
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:47:09  zee]
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
 * 3. Alladvertising materials mentioning features or use of this software
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
static char sccsid[] = "@(#)bt_open.c	8.10 (Berkeley) 8/17/94";
#endif /* LIBC_SCCS and not lint */

/*
 * Implementation of btree access method for 4.4BSD.
 *
 * The design here was originally based on that of the btree access method
 * used in the Postgres database system at UC Berkeley.  This implementation
 * is wholly independent of the Postgres code.
 */

#include <sys/param.h>
#include <sys/stat.h>

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <db.h>
#include <btree.h>

#ifdef DEBUG
#undef	MINPSIZE
#define	MINPSIZE	128
#endif

static int byteorder __P((void));
static int nroot __P((BTREE *));
static int tmp __P((void));

/*
 * __BT_OPEN -- Open a btree.
 *
 * Creates and fills a DB struct, and calls the routine that actually
 * opens the btree.
 *
 * Parameters:
 *	fname:	filename (NULL for in-memory trees)
 *	flags:	open flag bits
 *	mode:	open permission bits
 *	b:	BTREEINFO pointer
 *
 * Returns:
 *	NULL on failure, pointer to DB on success.
 *
 */
DB *
__bt_open(fname, flags, mode, openinfo, dflags)
	const char *fname;
	int flags, mode, dflags;
	const BTREEINFO *openinfo;
{
	struct stat sb;
	BTMETA m;
	BTREE *t;
	BTREEINFO b;
	DB *dbp;
	pgno_t ncache;
	ssize_t nr;
	int machine_lorder;
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	u_long current_pt = 0;
/* *** G-L-O-R-I-A TXN CHANGES *** END */

	t = NULL;

	/*
	 * Intention is to make sure all of the user's selections are okay
	 * here and then use them without checking.  Can't be complete, since
	 * we don't know the right page size, lorder or flags until the backing
	 * file is opened.  Also, the file's page size can cause the cachesize
	 * to change.
	 */
	machine_lorder = byteorder();
	if (openinfo) {
		b = *openinfo;

		/* Flags: R_DUP. */
		if (b.flags & ~(R_DUP))
			goto einval;

		/*
		 * Page size must be indx_t aligned and >= MINPSIZE.  Default
		 * page size is set farther on, based on the underlying file
		 * transfer size.
		 */
		if (b.psize &&
		    (b.psize < MINPSIZE || b.psize > MAX_PAGE_OFFSET + 1 ||
		    b.psize & sizeof(indx_t) - 1))
			goto einval;

		/* Minimum number of keys per page; absolute minimum is 2. */
		if (b.minkeypage) {
			if (b.minkeypage < 2)
				goto einval;
		} else
			b.minkeypage = DEFMINKEYPAGE;

		/* If no comparison, use default comparison and prefix. */
		if (b.compare == NULL) {
			b.compare = __bt_defcmp;
			if (b.prefix == NULL)
				b.prefix = __bt_defpfx;
		}

		if (b.lorder == 0)
			b.lorder = machine_lorder;
	} else {
		b.compare = __bt_defcmp;
		b.cachesize = 0;
		b.flags = 0;
		b.lorder = machine_lorder;
		b.minkeypage = DEFMINKEYPAGE;
		b.prefix = __bt_defpfx;
		b.psize = 0;
	}

	/* Check for the ubiquitous PDP-11. */
	if (b.lorder != BIG_ENDIAN && b.lorder != LITTLE_ENDIAN)
		goto einval;

	/* Allocate and initialize DB and BTREE structures. */
	if ((t = (BTREE *)malloc(sizeof(BTREE))) == NULL)
		goto err;
	memset(t, 0, sizeof(BTREE));

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	/*
	 * Initialize the queue for thread-specific data
	 */
	CIRCLEQ_INIT(&t->bt_tq);

	/*
	 * Create the pthread key that we'll use for finding the 
	 * thread-specific data buffers
	 */
	if (pthread_keycreate ((pthread_key_t *)&t->bt_ts_key, 
			       (pthread_destructor_t)bt_ts_destruct) == 0) {
	   TS_BTREE *ts_buf;

	   /*
	    * Allocate thread-specific data buffer for this thread now,
	    * associate it with the key, fill in pointer to the tree, 
	    * and insert it in the queue.
	    */
	   ts_buf = (TS_BTREE *)malloc(sizeof(TS_BTREE));

	   if (pthread_setspecific (t->bt_ts_key, (void *)ts_buf) != 0) {
	      goto err;
	   }

	   bzero((char *)ts_buf, sizeof(*ts_buf));

	   ts_buf->bt_tree = t;

	   CIRCLEQ_INSERT_HEAD( &(t->bt_tq), ts_buf, tq);
	}
	else {
	   goto err;
	}	

	/*
	 * Create mutex to protect the BTREE structure members
	 * that change while the tree is available: 
	 *    bt_first, bt_last, bt_order, bt_tq
	 */
	if (pthread_mutex_init(&t->bt_mutex, pthread_mutexattr_default) != 0) {
	   goto err;
	}
/* *** G-L-O-R-I-A TXN CHANGES *** END */

	t->bt_fd = -1;			/* Don't close unopened fd on error. */
	t->bt_lorder = b.lorder;
	t->bt_order = NOT;
	t->bt_cmp = b.compare;
	t->bt_key_disp = b.key_display;
	t->bt_data_disp = b.data_display;
	t->bt_pfx = b.prefix;
	t->bt_rfd = -1;

	if ((t->bt_dbp = dbp = (DB *)malloc(sizeof(DB))) == NULL)
		goto err;
	memset(t->bt_dbp, 0, sizeof(DB));
	if (t->bt_lorder != machine_lorder)
		F_SET(t, B_NEEDSWAP);

	dbp->type = DB_BTREE;
	dbp->internal = t;
	/*
	 * COMPILE WORKAROUND:
	 * closer should be close, but on AIX and CMA threads, close gets
	 * redefined to cma_close by the preprocessor, and a compile
	 * error ensues.  Fix if possible.
	 */
	dbp->closer = __bt_close;
	dbp->del = __bt_delete;
	dbp->fd = __bt_fd;
	dbp->get = __bt_get;
	dbp->put = __bt_put;
	dbp->seq = __bt_seq;
	dbp->sync = __bt_sync;

	/*
	 * If no file name was supplied, this is an in-memory btree and we
	 * open a backing temporary file.  Otherwise, it's a disk-based tree.
	 */
	if (fname) {
		switch (flags & O_ACCMODE) {
		case O_RDONLY:
			F_SET(t, B_RDONLY);
			break;
		case O_RDWR:
			break;
		case O_WRONLY:
		default:
			goto einval;
		}
		
		if ((t->bt_fd = open(fname, flags, mode)) < 0)
			goto err;

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
		{
		   char tmp_fname[MAXPATHLEN];
		   
		   strcpy (tmp_fname, fname);
		   strcat (tmp_fname, "_pt0");
		   if ((t->bt_pt0_fd = open(tmp_fname, flags, mode)) < 0)
		     goto err;
		   strcpy (tmp_fname, fname);
		   strcat (tmp_fname, "_pt1");
		   if ((t->bt_pt1_fd = open(tmp_fname, flags, mode)) < 0)
		     goto err;
		}

/* *** G-L-O-R-I-A TXN CHANGES *** END */


	} else {
		if ((flags & O_ACCMODE) != O_RDWR)
			goto einval;
		if ((t->bt_fd = tmp()) == -1)
			goto err;
		F_SET(t, B_INMEM);
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
		t->bt_pt0_fd = NO_FD;
		t->bt_pt1_fd = NO_FD;
/* *** G-L-O-R-I-A TXN CHANGES *** END */
	}

/***** AIX BUG?  The following call returns -1 and sets errno to 9 (EBADF),
but the descriptor is valid.

	if (fcntl(t->bt_fd, F_SETFD, 1) == -1)
		goto err;
******/
	if (fstat(t->bt_fd, &sb))
		goto err;
	if (sb.st_size) {
		if ((nr = read(t->bt_fd, &m, sizeof(BTMETA))) < 0)
			goto err;
		if (nr != sizeof(BTMETA))
			goto eftype;

		/*
		 * Read in the meta-data.  This can change the notion of what
		 * the lorder, page size and flags are, and, when the page size
		 * changes, the cachesize value can change too.  If the user
		 * specified the wrong byte order for an existing database, we
		 * don't bother to return an error, we just clear the NEEDSWAP
		 * bit.
		 */
		if (m.magic == BTREEMAGIC)
			F_CLR(t, B_NEEDSWAP);
		else {
			F_SET(t, B_NEEDSWAP);
			M_32_SWAP(m.magic);
			M_32_SWAP(m.version);
			M_32_SWAP(m.psize);
			M_32_SWAP(m.free);
			M_32_SWAP(m.nrecs);
			M_32_SWAP(m.flags);
		}
		if (m.magic != BTREEMAGIC || m.version != BTREEVERSION)
			goto eftype;
		if (m.psize < MINPSIZE || m.psize > MAX_PAGE_OFFSET + 1 ||
		    m.psize & sizeof(indx_t) - 1)
			goto eftype;
		if (m.flags & ~SAVEMETA)
			goto eftype;
		b.psize = m.psize;
		F_SET(t, m.flags);
		t->bt_free = m.free;
		t->bt_nrecs = m.nrecs;
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
		current_pt = m.current_pt;
/* *** G-L-O-R-I-A TXN CHANGES *** END */
	} else {
		/*
		 * Set the page size to the best value for I/O to this file.
		 * Don't overflow the page offset type.
		 */
		if (b.psize == 0) {
			b.psize = sb.st_blksize;
			if (b.psize < MINPSIZE)
				b.psize = MINPSIZE;
			if (b.psize > MAX_PAGE_OFFSET + 1)
				b.psize = MAX_PAGE_OFFSET + 1;
		}

		/* Set flag if duplicates permitted. */
		if (!(b.flags & R_DUP))
			F_SET(t, B_NODUPS);

		t->bt_free = P_INVALID;
		t->bt_nrecs = 0;
		F_SET(t, B_METADIRTY);
	}

	t->bt_psize = b.psize;

	/* Set the cache size; must be a multiple of the page size. */
	if (b.cachesize && b.cachesize & b.psize - 1)
		b.cachesize += (~b.cachesize & b.psize - 1) + 1;
	if (b.cachesize < b.psize * MINCACHE)
		b.cachesize = b.psize * MINCACHE;

	/* Calculate number of pages to cache. */
	ncache = (b.cachesize + t->bt_psize - 1) / t->bt_psize;

	/*
	 * The btree data structure requires that at least two keys can fit on
	 * a page, but other than that there's no fixed requirement.  The user
	 * specified a minimum number per page, and we translated that into the
	 * number of bytes a key/data pair can use before being placed on an
	 * overflow page.  This calculation includes the page header, the size
	 * of the index referencing the leaf item and the size of the leaf item
	 * structure.  Also, don't let the user specify a minkeypage such that
	 * a key/data pair won't fit even if both key and data are on overflow
	 * pages.
	 */
	t->bt_ovflsize = (t->bt_psize - BTDATAOFF) / b.minkeypage -
	    (sizeof(indx_t) + NBLEAFDBT(0, 0));
	if (t->bt_ovflsize < NBLEAFDBT(NOVFLSIZE, NOVFLSIZE) + sizeof(indx_t))
		t->bt_ovflsize =
		    NBLEAFDBT(NOVFLSIZE, NOVFLSIZE) + sizeof(indx_t);

	/* Initialize the buffer pool. */
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if ((t->bt_mp =
	    mpool_open(NULL, t->bt_fd, t->bt_pt0_fd, t->bt_pt1_fd, 
		       current_pt, t->bt_psize, ncache)) == NULL)
		goto err;
/* *** G-L-O-R-I-A TXN CHANGES *** END */

	if (!F_ISSET(t, B_INMEM))
		mpool_filter(t->bt_mp, __bt_pgin, __bt_pgout, t);

	/* Create a root page if new tree. */
	if (nroot(t) == RET_ERROR)
		goto err;

	/* Global flags. */
	if (dflags & DB_LOCK)
		F_SET(t, B_DB_LOCK);
	if (dflags & DB_SHMEM)
		F_SET(t, B_DB_SHMEM);
	if (dflags & DB_TXN)
		F_SET(t, B_DB_TXN);

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if ( F_ISSET(t,B_METADIRTY)) {
	   bt_meta(t);
	   bt_txn_commit(dbp);
	}
/* *** G-L-O-R-I-A TXN CHANGES *** END */

	return (dbp);

einval:	errno = EINVAL;
	goto err;

eftype:	errno = EFTYPE;
	goto err;

err:	if (t) {
		if (t->bt_dbp)
			free(t->bt_dbp);
		if (t->bt_fd != -1)
			(void)close(t->bt_fd);
		free(t);
	}
	return (NULL);
}

/*
 * NROOT -- Create the root of a new tree.
 *
 * Parameters:
 *	t:	tree
 *
 * Returns:
 *	RET_ERROR, RET_SUCCESS
 */
static int
nroot(t)
	BTREE *t;
{
	PAGE *meta, *root;
	pgno_t npg;

	if ((meta = mpool_get(t->bt_mp, 0, 0)) != NULL) {
		mpool_put(t->bt_mp, meta, 0);
		return (RET_SUCCESS);
	}
	if (errno != EINVAL)		/* It's OK to not exist. */
		return (RET_ERROR);
	errno = 0;

	if ((meta = mpool_new(t->bt_mp, &npg)) == NULL)
		return (RET_ERROR);

	if ((root = mpool_new(t->bt_mp, &npg)) == NULL)
		return (RET_ERROR);

	if (npg != P_ROOT)
		return (RET_ERROR);
	root->pgno = npg;
	root->prevpg = root->nextpg = P_INVALID;
	root->lower = BTDATAOFF;
	root->upper = t->bt_psize;
	root->flags = P_BLEAF;
	memset(meta, 0, t->bt_psize);
	mpool_put(t->bt_mp, meta, MPOOL_DIRTY);
	mpool_put(t->bt_mp, root, MPOOL_DIRTY);
	return (RET_SUCCESS);
}

static int
tmp()
{
	sigset_t set, oset;
	int fd;
	char *envtmp;
	char path[MAXPATHLEN];

	envtmp = getenv("TMPDIR");
	(void)snprintf(path,
	    sizeof(path), "%s/bt.XXXXXX", envtmp ? envtmp : "/tmp");

	(void)sigfillset(&set);
	(void)sigprocmask(SIG_BLOCK, &set, &oset);
	if ((fd = mkstemp(path)) != -1)
		(void)unlink(path);
	(void)sigprocmask(SIG_SETMASK, &oset, NULL);
	return(fd);
}

static int
byteorder()
{
	u_int32_t x;
	u_char *p;

	x = 0x01020304;
	p = (u_char *)&x;
	switch (*p) {
	case 1:
		return (BIG_ENDIAN);
	case 4:
		return (LITTLE_ENDIAN);
	default:
		return (0);
	}
}

int
__bt_fd(dbp)
        const DB *dbp;
{
	BTREE *t;
	TS_BTREE *ts;


	t = dbp->internal;
	GET_THREAD(t,ts);

	/* Toss any page pinned across calls. */
	if (ts->bt_pinned != NULL) {
		mpool_put(t->bt_mp, ts->bt_pinned, 0);
		ts->bt_pinned = NULL;
	}

	/* In-memory database can't have a file descriptor. */
	if (F_ISSET(t, B_INMEM)) {
		errno = ENOENT;
		return (-1);
	}
	return (t->bt_fd);
}
















