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
 * $Log: mpool.h,v $
 * Revision 1.1.2.1  1996/10/03  20:25:08  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:19:59  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:22 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:48:03  zee]
 * 
 * $EndLog$
 */
/*-
 * Copyright (c) 1991, 1993, 1994
 *	The Regents of the University of California.  All rights reserved.
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
 *
 *	@(#)mpool.h	8.2 (Berkeley) 7/14/94
 */
#ifdef THREADS
#include <pthread.h>
#endif

#include <sys/queue.h>

/*
 * The memory pool scheme is a simple one.  Each in-memory page is referenced
 * by a bucket which is threaded in up to two of three ways.  All active pages
 * are threaded on a hash chain (hashed by page number) and an lru chain.
 * Inactive pages are threaded on a free chain.  Each reference to a memory
 * pool is handed an opaque MPOOL cookie which stores all of this information.
 */
#define	HASHSIZE	128
#define	HASHKEY(pgno)	((pgno - 1) % HASHSIZE)

/* The BKT structures are the elements of the queues. */
typedef struct _bkt {
	CIRCLEQ_ENTRY(_bkt) hq;		/* hash queue */
	CIRCLEQ_ENTRY(_bkt) q;		/* lru queue */
	void    *page;			/* page */
	pgno_t   pgno;			/* page number */

#ifdef THREADS
	unsigned int users;             /* number of threads that
					   have this page pinned */
#endif

#define	MPOOL_DIRTY	0x01		/* page needs to be written */
#define	MPOOL_PINNED	0x02		/* page is pinned into memory */
	u_int8_t flags;			/* flags */
} BKT;

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
/* 
 * The FRAME structure exists for keeping lists of frame numbers 
 */
typedef struct FRAME {
        pgno_t  frame;                  /* Frame number within file */
	struct FRAME   *next;           /* next in list */
} FRAME;
/* *** G-L-O-R-I-A TXN CHANGES *** END */

typedef struct MPOOL {
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
        pthread_mutex_t mutex;          /* Protecting queues and statistics */
/* *** G-L-O-R-I-A TXN CHANGES *** END */
	CIRCLEQ_HEAD(_lqh, _bkt) lqh;	/* lru queue head */
					/* hash queue array */
	CIRCLEQ_HEAD(_hqh, _bkt) hqh[HASHSIZE];
	pgno_t	curcache;		/* current number of cached pages */
	pgno_t	maxcache;		/* max number of cached pages */
	pgno_t	npages;			/* number of pages in the file */
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	pgno_t	rollback_npages;	/* Number of pages in the file. */
/* *** G-L-O-R-I-A TXN CHANGES *** END */
	u_long	pagesize;		/* file page size */
	int	fd;			/* file descriptor */
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	int     in_memory;              /* In-memory database */
	int	page_table_fd[2];       /* Page Table File descriptors. */
#define NO_FD -1                       
        u_long  current;                /* Which page table's current? 0/1 */
/* *** G-L-O-R-I-A TXN CHANGES *** END */
					/* page in conversion routine */
	void    (*pgin) __P((void *, pgno_t, void *));
					/* page out conversion routine */
	void    (*pgout) __P((void *, pgno_t, void *));
	void	*pgcookie;		/* cookie for page in/out routines */
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	FRAME   *free_frames;           /* Free frames list */
	FRAME   *shadow_frames;         /* Frames necessary for rollback */
	unsigned int total_frames;      /* Number of frames in the file */
	pgno_t  *page_table;            /* Page table address */

#ifdef STATISTICS
	unsigned long   rollback;
	unsigned long   commit;
	unsigned long   nfree_frames;
	unsigned long   max_free_frames;
	unsigned long   nshadow_frames;
	unsigned long   max_shadow_frames;
/* *** G-L-O-R-I-A TXN CHANGES *** END */

	u_long	cachehit;
	u_long	cachemiss;
	u_long	pagealloc;
	u_long	pageflush;
	u_long	pageget;
	u_long	pagenew;
	u_long	pageput;
	u_long	pageread;
	u_long	pagewrite;
#endif
} MPOOL;

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */

#ifdef THREADS
#define MP_LOCK(x) pthread_mutex_lock(&x->mutex);
#else
#define MP_LOCK(x) 
#endif 

#ifdef THREADS
#define MP_UNLOCK(x) pthread_mutex_unlock(&x->mutex);
#else
#define MP_UNLOCK(x) 
#endif 

#define PAGE_TABLE_EXTEND_FACTOR 256
#define PAGE_TABLE_ENTRY_SIZE sizeof(pgno_t)
#define PAGE_TABLE_EXTEND (PAGE_TABLE_EXTEND_FACTOR*PAGE_TABLE_ENTRY_SIZE)
#define PAGE_TABLE_INIT_FACTOR 8
#define PAGE_TABLE_MASK (PAGE_TABLE_EXTEND-1)

/* Given a current size of the table, round it up to a PAGE_TABLE_EXTEND
   boundary, and add one PAGE_TABLE_EXTEND to that.  This is how large
   we will make an in-memory page table when reading in an existing one 
   from disk.  It helps prolong the agony of having to extend the table */
#define PAGE_TABLE_SIZE(ss) ((ss+(2*PAGE_TABLE_EXTEND)-1) & (~PAGE_TABLE_MASK))


/*
 *  Added to take over for a very small routine of the same name.
 */

#define MPOOL_TXN_BEGIN(p)   (p)->rollback_npages = (p)->npages; 



/* *** G-L-O-R-I-A TXN CHANGES *** END */




__BEGIN_DECLS
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
MPOOL	*mpool_open __P((void *, int, int, int, u_long, pgno_t, pgno_t));
int      mpool_txn_commit (MPOOL *);
int      mpool_txn_rollback (MPOOL *);
/* *** G-L-O-R-I-A TXN CHANGES *** END */
void	 mpool_filter __P((MPOOL *, void (*)(void *, pgno_t, void *),
	    void (*)(void *, pgno_t, void *), void *));
void	*mpool_new __P((MPOOL *, pgno_t *));
void	*mpool_get __P((MPOOL *, pgno_t, u_int));
int	 mpool_put __P((MPOOL *, void *, u_int));
int	 mpool_sync __P((MPOOL *));
int	 mpool_close __P((MPOOL *));
#ifdef STATISTICS
void	 mpool_stat __P((MPOOL *));
#endif
__END_DECLS

