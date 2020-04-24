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
 * $Log: mpool.c,v $
 * Revision 1.1.2.1  1996/10/03  20:24:54  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  19:19:48  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:22 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:36 UTC  arvind  /main/arvind_pkss/1]
 * 
 * 	GLORIA (R1.2.2) revision.
 * 	[1996/06/24  20:47:40  zee]
 * 
 * $EndLog$
 */
/*-
 * Copyright (c) 1990, 1993, 1994
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
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)mpool.c	8.5 (Berkeley) 7/26/94";
#endif /* LIBC_SCCS and not lint */

/* just for debugging... */
#include <dce/dce.h>
#include <dcecdssvc.h>


#include <sys/param.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <db.h>

#define	__MPOOLINTERFACE_PRIVATE
#include <mpool.h>

static BKT *mpool_bkt __P((MPOOL *));
static BKT *mpool_look __P((MPOOL *, pgno_t));
static int  mpool_write __P((MPOOL *, BKT *));
static void mpool_trash_page_table(MPOOL *); 
static void mpool_trash_cache(MPOOL *);

static int extend_page_table(MPOOL *);
static int mpool_load_page_table (MPOOL *);

#define P_META 0


/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
/*
 * mpool_load_page_table
 *
 * Create a buffer with room for the page table plus 
 * several new pages to save time when adding to the 
 * database.  Read the file containing the current page 
 * table into a buffer.  Create the free frame list
 * and set appropriate info in MPOOL structure.
 * 
 * Parameters:
 *   MPOOL *mp
 *
 * Returns:
 *   RET_ERROR or RET_SUCCESS
 */
static int
mpool_load_page_table (mp)
     MPOOL *mp; 
{
   struct stat buffer;
   int table_size;
   FRAME *tmp_frame;
   int f,p;
   size_t nr;
   int in_use;

   if (fstat (mp->page_table_fd[mp->current], &buffer) == 0 ) {

      mp->npages = buffer.st_size / sizeof(pgno_t);
      
      table_size = MAX(PAGE_TABLE_SIZE(buffer.st_size),PAGE_TABLE_EXTEND*PAGE_TABLE_INIT_FACTOR);
      if ((mp->page_table = malloc (table_size)) == NULL)
	return (RET_ERROR);
      memset (mp->page_table, 0, table_size);
      
      if (buffer.st_size != 0) {
	 if (lseek(mp->page_table_fd[mp->current], 0, SEEK_SET) != 0){
           free (mp->page_table);
	   return (RET_ERROR);
         }
	 if ((nr = read (mp->page_table_fd[mp->current], 
			 mp->page_table, buffer.st_size)) != buffer.st_size) {
            free (mp->page_table);
	    return (RET_ERROR);
	 }
      }

      /*
       * How many frames we have to work with 
       */
      mp->total_frames = table_size / sizeof (pgno_t); 

      /*
       * Build the free list 
       */
#ifdef STATISTICS
      mp->nfree_frames = 0;
#endif
      /*
       * Build the free frames list, based on pagetable data
       * Start with frame 1 because frame 0 is reserved for P_META
       */
      for (f=mp->total_frames-1; f > 1; f--) {
	 in_use = FALSE;
	 for (p=1; p < mp->npages; p++) {
	    if ( mp->page_table[p] == f ) {
	       in_use = TRUE;
	       break;
	    }
	 }
	 if (!in_use) {
	    tmp_frame = mp->free_frames;
	    mp->free_frames = malloc (sizeof(FRAME));
	    mp->free_frames->frame = f;
	    mp->free_frames->next = tmp_frame;
#ifdef STATISTICS
	    mp->nfree_frames++;
#endif
	 }
      }
   } else {
      return (RET_ERROR);
   }
   
   return (RET_SUCCESS);
}
/* *** G-L-O-R-I-A TXN CHANGES *** END */

/*
 * mpool_open --
 *	Initialize a memory pool.
 */
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
MPOOL *
mpool_open(key, fd, pt_0_fd, pt_1_fd, current_pt, pagesize, maxcache)
/* *** G-L-O-R-I-A TXN CHANGES *** END */
	void *key;
	int fd;
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	int pt_0_fd;
	int pt_1_fd;
	u_long current_pt;
/* *** G-L-O-R-I-A TXN CHANGES *** END */
        pgno_t pagesize, maxcache;
{
	struct stat sb;
	MPOOL *mp;
	int entry;
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	int status;
/* *** G-L-O-R-I-A TXN CHANGES *** END */	

	/*
	 * Get information about the file.
	 *
	 * XXX
	 * We don't currently handle pipes, although we should.
	 */
	if (fstat(fd, &sb)) {
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	   perror("fstat in mpool_open()");
/* *** G-L-O-R-I-A TXN CHANGES *** END */
	   return (NULL);
	}
	if (!S_ISREG(sb.st_mode)) {
		errno = ESPIPE;
		return (NULL);
	}

	/* Allocate and initialize the MPOOL cookie. */
	if ((mp = (MPOOL *)calloc(1, sizeof(MPOOL))) == NULL)
		return (NULL);
	CIRCLEQ_INIT(&mp->lqh);
	for (entry = 0; entry < HASHSIZE; ++entry)
		CIRCLEQ_INIT(&mp->hqh[entry]);
	mp->maxcache = maxcache;
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
/*	mp->npages = sb.st_size / pagesize; */
/* *** G-L-O-R-I-A TXN CHANGES *** END */
	mp->pagesize = pagesize;
	mp->fd = fd;

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if ( pt_1_fd >= 0 && pt_0_fd >= 0 ) {
	   mp->page_table_fd[0] = pt_0_fd;
	   mp->page_table_fd[1] = pt_1_fd;
	   mp->in_memory = FALSE;

	   /*
	    * Save the indication of which page table is current
	    * (__bt_open passes us from the metadata page)
	    *
	    * Then load the page table and set appropriate info
	    */
	   mp->current = current_pt;
	   if ( mpool_load_page_table(mp) != RET_SUCCESS) {
	      return (NULL);
	   }
	   mp->shadow_frames = NULL; /* No shadow frames until we're in a 
					transaction */
#ifdef STATISTICS
	   mp->nshadow_frames = mp->max_free_frames = 
	     mp->max_shadow_frames = 0;
	   mp->rollback = mp->commit = 0;
#endif
	} else if (pt_1_fd == NO_FD) {
	   mp->in_memory = TRUE;
	}

#ifdef THREADS
	/*
	 * Create mutex to guard the queue and statistical data
	 */
	if ( pthread_mutex_init (&mp->mutex, pthread_mutexattr_default) != 0) {
	   return (NULL);
	}
#endif

/* *** G-L-O-R-I-A TXN CHANGES *** END */

	return (mp);
}

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */

/*
 * extend_page_table -- make room for more pages in table
 *
 * Parameters:
 *      mp - MPOOL cookie
 *
 * Returns:
 *      1 for success
 *      NULL for failure
 */
static 
int extend_page_table(mp) 
     MPOOL *mp;
{
   int table_size;
   void *tmp_ptr;
   int orig_total_frames;
   int f;
   FRAME *tmp_frame;

   table_size = (mp->npages * PAGE_TABLE_ENTRY_SIZE) + PAGE_TABLE_EXTEND;
   tmp_ptr = (void *)mp->page_table;
   if ((mp->page_table = (pgno_t *)realloc(tmp_ptr, table_size)) == NULL) {
      mp->page_table = tmp_ptr;
      return ((int)NULL);
   }
   orig_total_frames = mp->total_frames;
   mp->total_frames = table_size / PAGE_TABLE_ENTRY_SIZE;   

   /*
    * Add them to the free list (in reverse order so lower
    * numbers will be used first, and file won't need to jump
    * by factors of PAGE_TABLE_EXTEND 
    */
   for (f = mp->total_frames-1; f >= orig_total_frames; f--) {
      tmp_frame = mp->free_frames;
      mp->free_frames = malloc (sizeof(FRAME));
      mp->free_frames->frame = f;
      mp->free_frames->next = tmp_frame;

#ifdef STATISTICS
      mp->nfree_frames++;
#endif

   }

#ifdef STATISTICS
   if (mp->nfree_frames > mp->max_free_frames) 
     mp->max_free_frames = mp->nfree_frames;
#endif

   return (1);
}
/* *** G-L-O-R-I-A TXN CHANGES *** END */

/*
 * mpool_filter --
 *	Initialize input/output filters.
 */
void
mpool_filter(mp, pgin, pgout, pgcookie)
	MPOOL *mp;
	void (*pgin) __P((void *, pgno_t, void *));
	void (*pgout) __P((void *, pgno_t, void *));
	void *pgcookie;
{
	mp->pgin = pgin;
	mp->pgout = pgout;
	mp->pgcookie = pgcookie;
}

/*
 * mpool_new --
 *	Get a new page of memory.
 */
void *
mpool_new(mp, pgnoaddr)
	MPOOL *mp;
	pgno_t *pgnoaddr;
{
	struct _hqh *head;
	BKT *bp;
/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	FRAME *tmp_frame;
/* *** G-L-O-R-I-A TXN CHANGES *** END */

   MP_LOCK(mp){

	if (mp->npages == MAX_PAGE_NUMBER) {
		(void)fprintf(stderr, "mpool_new: page allocation overflow.\n");
		MP_UNLOCK(mp);
		abort();
	}
#ifdef STATISTICS
	++mp->pagenew;
#endif
	/*
	 * Get a BKT from the cache.  Assign a new page number, attach
	 * it to the head of the hash chain, the tail of the lru chain,
	 * and return.
	 */
	if ((bp = mpool_bkt(mp)) == NULL) {
	   MP_UNLOCK(mp);
	   return (NULL);
	}
	*pgnoaddr = bp->pgno = mp->npages++;
	bp->flags = MPOOL_PINNED;
#ifdef THREADS
	bp->users = 1; /* we just pinned it.  This thread is a user */
#ifdef TDEBUG
	(void)fprintf(stderr,
		      "Thread 0x%lx set page %d user count to %d (mpool_filter)\n",
		      dthread_self(),bp->pgno,bp->users);
#endif	     
#endif
	head = &mp->hqh[HASHKEY(bp->pgno)];
	CIRCLEQ_INSERT_HEAD(head, bp, hq);
	CIRCLEQ_INSERT_TAIL(&mp->lqh, bp, q);

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if (!(mp->in_memory)) {
	   /*
	    * Find a free or new frame to associate with the new page
	    */
	   if (!(mp->free_frames)) {
	      if(!extend_page_table(mp)) {
		 MP_UNLOCK(mp);
		 return (NULL);
	      }
	   }
	   mp->page_table[bp->pgno] = mp->free_frames->frame;
	   tmp_frame = mp->free_frames->next;
	   free (mp->free_frames);
	   mp->free_frames = tmp_frame;
#ifdef STATISTICS
	   mp->nfree_frames--;
#endif
	}
/* *** G-L-O-R-I-A TXN CHANGES *** END */

   } MP_UNLOCK(mp);

   return (bp->page);
}

/*
 * mpool_get
 *	Get a page.
 */
void *
mpool_get(mp, pgno, flags)
	MPOOL *mp;
	pgno_t pgno;
	u_int flags;				/* XXX not used? */
{
	struct _hqh *head;
	BKT *bp;
	off_t off;
	int nr;



   MP_LOCK(mp) {

	/* Check for attempt to retrieve a non-existent page. */
	if (pgno >= mp->npages) {
		errno = EINVAL;
		MP_UNLOCK(mp);
		return (NULL);
	}

#ifdef STATISTICS
	++mp->pageget;
#endif

	/* Check for a page that is cached. */
	if ((bp = mpool_look(mp, pgno)) != NULL) {
#ifdef THREADS
	   /* 
	    * Assume that if it's already pinned, then
	    * another thread did it, and we're still in
	    * good shape 
	    */
#else
#ifdef DEBUG
		if (bp->flags & MPOOL_PINNED) {
			(void)fprintf(stderr,
			    "mpool_get: page %d already pinned\n", bp->pgno);
			MP_UNLOCK(mp);
			abort();
		}
#endif
#endif
		/*
		 * Move the page to the head of the hash chain and the tail
		 * of the lru chain.
		 */
		head = &mp->hqh[HASHKEY(bp->pgno)];
		CIRCLEQ_REMOVE(head, bp, hq);
		CIRCLEQ_INSERT_HEAD(head, bp, hq);
		CIRCLEQ_REMOVE(&mp->lqh, bp, q);
		CIRCLEQ_INSERT_TAIL(&mp->lqh, bp, q);

		/* Return a pinned page. */
#ifdef THREADS
		bp->users++; /* do this while we have the mutex locked */

#ifdef TDEBUG
	(void)fprintf(stderr,
		      "Thread 0x%lx incremented page %d user count to %d\n",
		      dthread_self(),bp->pgno,bp->users);
#endif	     
#endif		
		bp->flags |= MPOOL_PINNED;
		MP_UNLOCK(mp);
		return (bp->page);
        }

	/* Get a page from the cache. */
	if ((bp = mpool_bkt(mp)) == NULL) {
           errno = EFAULT;
	   MP_UNLOCK(mp);
	   return (NULL);
	}

	/* Set the page number, pin the page. */
	bp->pgno = pgno;
	bp->flags = MPOOL_PINNED;
#ifdef THREADS
	bp->users=1; /* We are the first user thread */
#ifdef TDEBUG
	(void)fprintf(stderr,
		      "Thread 0x%lx set page %d user count to %d(mpool_get)\n",
		      dthread_self(),bp->pgno,bp->users);
#endif	     
#endif		

	/* Read in the contents. */
#ifdef STATISTICS
	++mp->pageread;
#endif

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if (mp->in_memory) {
	   off = mp->pagesize * pgno;
	}
	else {
	   off = mp->pagesize * mp->page_table[bp->pgno];
	}
/* *** G-L-O-R-I-A TXN CHANGES *** END */

	if (lseek(mp->fd, off, SEEK_SET) != off) {
	   /*
	    * ??? G-L-O-R-I-A:
	    * Here's a bug.  THis page should be put back
	    * on the lru and hash lists.  We'll lose
	    * this page forever this way.
	    */
           errno = ESPIPE;
	   MP_UNLOCK(mp);
	   return (NULL);
	}
	if ((nr = read(mp->fd, bp->page, mp->pagesize)) != mp->pagesize) {
		if (nr >= 0) {
		   errno = EFTYPE;
		}
		/*
		 * ??? G-L-O-R-I-A:
		 * Here's a bug.  THis page should be put back
		 * on the lru and hash lists.  We'll lose
		 * this page forever this way.
		 */
		MP_UNLOCK(mp);
		return (NULL);
	}

	/*
	 * Add the page to the head of the hash chain and the tail
	 * of the lru chain.  If threads, take the mutex while we do
	 * this
	 */
	head = &mp->hqh[HASHKEY(bp->pgno)];
	CIRCLEQ_INSERT_HEAD(head, bp, hq);
	CIRCLEQ_INSERT_TAIL(&mp->lqh, bp, q);

	/* Run through the user's filter. */
	if (mp->pgin != NULL)
		(mp->pgin)(mp->pgcookie, bp->pgno, bp->page);

   } MP_UNLOCK(mp);

   return (bp->page);
}

/*
 * mpool_put
 *	Return a page.
 */
int
mpool_put(mp, page, flags)
	MPOOL *mp;
	void *page;
	u_int flags;
{
	BKT *bp;

   MP_LOCK(mp) {

#ifdef STATISTICS
	++mp->pageput;
#endif
	bp = (BKT *)((char *)page - sizeof(BKT));

#ifdef DEBUG
#ifndef THREADS
	if (!(bp->flags & MPOOL_PINNED)) {
		(void)fprintf(stderr,
		    "mpool_put: page %d not pinned\n", bp->pgno);
		MP_UNLOCK(mp);
		abort();
	}
#endif
#endif

#ifdef THREADS
	/*
	 * We shouldn't have any other threads with this 
	 * page pinned if we are writing it.  If we aren't writing it, then 
	 * other threads are welcome.
	 * 
	 * If this is the last thread using the page, then unpin it.
	 */
	if ( --bp->users != 0 ) {
	    if ( flags & MPOOL_DIRTY ) {
#ifdef TDEBUG
		(void)fprintf(stderr,
			      "Trying to write to page with non-0 user count (%d)\n",bp->users);
#endif	     
		MP_UNLOCK(mp);
		abort();
	    }
/* G-L-O-R-I-A _- fixing this to clear pinned flags. */
	}
	else {
	    bp->flags &= ~MPOOL_PINNED;
	}

#ifdef TDEBUG
	(void)fprintf(stderr,
		      "Thread 0x%lx decremented page %d user count to %d\n",
		      dthread_self(),bp->pgno,bp->users);
#endif	     
#else
	bp->flags &= ~MPOOL_PINNED;
#endif

	/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if (mp->in_memory) {
	   bp->flags |= (flags & MPOOL_DIRTY);
	   MP_UNLOCK(mp);
	   return (RET_SUCCESS);
	}

	/*
	 * Check to see if this page is just becoming dirty
	 */
	if ( flags & MPOOL_DIRTY && !(bp->flags & MPOOL_DIRTY)) {
	   FRAME *tmp_frame;	   

	   /*
	    * Mark it dirty 
	    */
	   bp->flags |= (flags & MPOOL_DIRTY);	   
	   
	   if (bp->pgno != P_META) {
	      /*
	       * If it isn't a brand new page, 
	       * put original frame on the shadow frame list, so
	       * we don't overwrite it before the transaction is committed
	       */
	      if ( mp->page_table[bp->pgno] ) {
		 if ((tmp_frame = malloc (sizeof(FRAME))) == NULL) {
		    MP_UNLOCK(mp);
		    return (RET_ERROR);
		 }
		 tmp_frame->frame = mp->page_table[bp->pgno];
		 tmp_frame->next = mp->shadow_frames;
		 mp->shadow_frames = tmp_frame;
#ifdef STATISTICS
		 mp->nshadow_frames++;
		 if (mp->max_shadow_frames < mp->nshadow_frames)
		   mp->max_shadow_frames = mp->nshadow_frames;
#endif
		 /*
		  * Now, assign a new frame to the page, so that we 
		  * can write the new changes without squashing the shadow
		  * copies of these pages.
		  */
		 if (!(mp->free_frames)) {
		    if(!extend_page_table(mp)) {
		       MP_UNLOCK(mp);
		       return (RET_ERROR);
		    }
		 }
		 mp->page_table[bp->pgno] = mp->free_frames->frame;
		 tmp_frame = mp->free_frames->next;
		 free (mp->free_frames);
		 mp->free_frames = tmp_frame;
#ifdef STATISTICS
		 mp->nfree_frames--;
#endif
	      }
	   }
	   else { 
	      /*
	       * For page 0 or P_META, we always use frame 0
	       * This is okay because transactions don't change it.
	       * Except for the first word which is written automicly (we hope),
	       * it is frozen after the database is created.
	       */
	      mp->page_table[bp->pgno] = 0;
	   }
	}
	/* *** G-L-O-R-I-A TXN CHANGES *** END */

   } MP_UNLOCK(mp);

   return (RET_SUCCESS);
}

/*
 * mpool_close
 *	Close the buffer pool.
 */
int
mpool_close(mp)
	MPOOL *mp;
{
	BKT *bp;

   MP_LOCK(mp) {

	/* 
	 * Free up any space allocated to the cache
	 */
	while ((bp = mp->lqh.cqh_first) != (void *)&mp->lqh) {
		CIRCLEQ_REMOVE(&mp->lqh, mp->lqh.cqh_first, q);
		free(bp);
	}
/*** G-L-O-R-I-A TXN CHANGES - BEGIN ***/
	/*
	 * Free page table memory, and lru and free frame queues
	 */
	mpool_trash_page_table(mp);

   } MP_UNLOCK(mp);

#ifdef THREADS
	/*
	 * Destroy the mutex which guards the queues and statistical data
	 */
	if ( pthread_mutex_destroy (&mp->mutex) != 0) {
	    free (mp);
	    return ((int)NULL);
	}
#endif

	/* Free the MPOOL cookie. */
	free(mp);

/*** G-L-O-R-I-A TXN CHANGES - BEGIN ***/

   return (RET_SUCCESS);
}


/*
 * mpool_sync
 *	Sync the pool to disk.
 */
int
mpool_sync(mp)
	MPOOL *mp;
{
	BKT *bp;

  MP_LOCK(mp) {

	/* Walk the lru chain, flushing any dirty pages to disk. */
	for (bp = mp->lqh.cqh_first; bp != (void *)&mp->lqh; bp = bp->q.cqe_next) {

	    if (bp->flags & MPOOL_DIRTY && mpool_write(mp, bp) == RET_ERROR) {
		MP_UNLOCK(mp);
		return (RET_ERROR);
	    }
	}

  } MP_UNLOCK(mp);

  /* Sync the file descriptor. */
  return (fsync(mp->fd) ? RET_ERROR : RET_SUCCESS);
}

/*
 * mpool_bkt
 *	Get a page from the cache (or create one).
 *
 * Assumptions:
 *      MP_LOCK must be held when this is called
 */
static BKT *
mpool_bkt(mp)
	MPOOL *mp;
{
	struct _hqh *head;
	BKT *bp;


	/* If under the max cached, always create a new page. */
	if (mp->curcache < mp->maxcache)
		goto new;

	/*
	 * If the cache is max'd out, walk the lru list for a buffer we
	 * can flush.  If we find one, write it (if necessary) and take it
	 * off any lists.  If we don't find anything we grow the cache anyway.
	 * The cache never shrinks.
	 */
	for (bp = mp->lqh.cqh_first;
	    bp != (void *)&mp->lqh; bp = bp->q.cqe_next)

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	  if (!(bp->flags & MPOOL_PINNED)) {
	     if (!mp->in_memory) {
		if ((bp->flags & MPOOL_DIRTY) && (mpool_write(mp,bp) == RET_ERROR)) {
		    return (NULL);
		}
	     }
	     else if (bp->flags & MPOOL_DIRTY)
	       continue; /* leave the dirty one alone until txn completes */

/* *** G-L-O-R-I-A TXN CHANGES *** END */
	     
#ifdef STATISTICS
			++mp->pageflush;
#endif
			/* Remove from the hash and lru queues. */

			head = &mp->hqh[HASHKEY(bp->pgno)];
			CIRCLEQ_REMOVE(head, bp, hq);
			CIRCLEQ_REMOVE(&mp->lqh, bp, q);
#ifdef DEBUG
			{ void *spage;
				spage = bp->page;
				memset(bp, 0xff, sizeof(BKT) + mp->pagesize);
				bp->page = spage;
			}
#endif
			return (bp);
	         }

new:	if ((bp = (BKT *)malloc(sizeof(BKT) + mp->pagesize)) == NULL) {
           return (NULL);
	}

#ifdef STATISTICS
	++mp->pagealloc;

#ifdef DEBUG
	if (mp->curcache > mp->maxcache)
	  mpool_stat(mp);
#endif 

#endif

	++mp->curcache;

#if defined(DEBUG) || defined(PURIFY)
	memset(bp, 0xff, sizeof(BKT) + mp->pagesize);
#endif
	bp->page = (char *)bp + sizeof(BKT);

  return (bp);
}

/*
 * mpool_write
 *	Write a page to disk.
 *
 * Assumptions:
 *      MP_LOCK must be held when this is called
 */
static int
mpool_write(mp, bp)
	MPOOL *mp;
	BKT *bp;
{
	off_t off;


#ifdef STATISTICS
	++mp->pagewrite;
#endif

	/* Run through the user's filter. */
	if (mp->pgout)
		(mp->pgout)(mp->pgcookie, bp->pgno, bp->page);

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if (mp->in_memory)
	  off = mp->pagesize * bp->pgno; 
	else 
	  off = mp->pagesize * (mp->page_table[bp->pgno]);
/* *** G-L-O-R-I-A TXN CHANGES *** END */

	if (lseek(mp->fd, off, SEEK_SET) != off) {
	   return (RET_ERROR);
	}
	if (write(mp->fd, bp->page, mp->pagesize) != mp->pagesize) {
	   return (RET_ERROR);
	}

	bp->flags &= ~MPOOL_DIRTY;

  return (RET_SUCCESS);
}

/*
 * mpool_look
 *	Lookup a page in the cache.
 *
 * Assumptions:
 *      MP_LOCK must be held when this is called
 */
static BKT *
mpool_look(mp, pgno)
	MPOOL *mp;
	pgno_t pgno;
{
	struct _hqh *head;
	BKT *bp;

	head = &mp->hqh[HASHKEY(pgno)];
	for (bp = head->cqh_first; bp != (void *)head; bp = bp->hq.cqe_next)
		if (bp->pgno == pgno) {
#ifdef STATISTICS
			++mp->cachehit;
#endif
			return (bp);
		}
#ifdef STATISTICS
	++mp->cachemiss;
#endif

  return (NULL);
}

#ifdef STATISTICS
/*
 * mpool_stat
 *	Print out cache statistics.
 */
void
mpool_stat(mp)
	MPOOL *mp;
{
	BKT *bp;
	int cnt;
	char *sep;


  MP_LOCK(mp) {

	(void)fprintf(stderr, "%lu pages in the file\n", mp->npages);
	(void)fprintf(stderr,
	    "page size %lu, cacheing %lu pages of %lu page max cache\n",
	    mp->pagesize, mp->curcache, mp->maxcache);
	(void)fprintf(stderr, "%lu page puts, %lu page gets, %lu page new\n",
	    mp->pageput, mp->pageget, mp->pagenew);
	(void)fprintf(stderr, "%lu page allocs, %lu page flushes\n",
	    mp->pagealloc, mp->pageflush);
	if (mp->cachehit + mp->cachemiss)
		(void)fprintf(stderr,
		    "%.0f%% cache hit rate (%lu hits, %lu misses)\n", 
		    ((double)mp->cachehit / (mp->cachehit + mp->cachemiss))
		    * 100, mp->cachehit, mp->cachemiss);
	(void)fprintf(stderr, "%lu page reads, %lu page writes\n",
	    mp->pageread, mp->pagewrite);
	/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
	if (!mp->in_memory) {
	   (void)fprintf(stderr, "free frames: %lu, max: %lu.\n",
			 mp->nfree_frames, mp->max_free_frames);
	   (void)fprintf(stderr, "shadow frames: %lu, max: %lu.\n",
			 mp->nshadow_frames, mp->max_shadow_frames);
	   (void)fprintf(stderr, "commits: %lu, rollbacks: %lu.\n",
			 mp->commit, mp->rollback);	
	}
	/* *** G-L-O-R-I-A TXN CHANGES *** END */

	sep = "";
	cnt = 0;
	for (bp = mp->lqh.cqh_first;
	    bp != (void *)&mp->lqh; bp = bp->q.cqe_next) {
		(void)fprintf(stderr, "%s%d", sep, bp->pgno);
		if (bp->flags & MPOOL_DIRTY)
			(void)fprintf(stderr, "d");
		if (bp->flags & MPOOL_PINNED)
			(void)fprintf(stderr, "P");
		if (++cnt == 10) {
			sep = "\n";
			cnt = 0;
		} else
			sep = ", ";
			
	}
	(void)fprintf(stderr, "\n");

  } MP_UNLOCK(mp);
}
#endif

/* *** G-L-O-R-I-A TXN CHANGES *** BEGIN */
/*
 * mpool_trash_cache -- throw dirty pages in cache away, forcing
 *                      new read of on-disk copy
 * 
 *
 */
static void
mpool_trash_cache(mp) 
    MPOOL *mp;
{
   BKT *bp, *next_bp;

#ifdef DEBUG_TXN
      fprintf(stderr, "Dumping pages from cache: ");
#endif

   for (bp = mp->lqh.cqh_first; bp != (void *)&mp->lqh; 
	bp = next_bp) {

      next_bp = bp->q.cqe_next;

      if (bp->flags & MPOOL_DIRTY) {
#ifdef DEBUG_TXN
	 fprintf(stderr, "%d ", bp->pgno);
#endif 
	 
	 CIRCLEQ_REMOVE(&mp->hqh[HASHKEY(bp->pgno)], bp, hq);
	 CIRCLEQ_REMOVE(&mp->lqh, bp, q);

	 free (bp);
	 --(mp->curcache);
#ifdef STATISTICS
	 --(mp->pagealloc);
#endif
      }
   }
#ifdef DEBUG_TXN
	 fprintf(stderr, ".\n");
#endif 
}

/*
 * mpool_trash_page_table -- trash structures associated with 
 *                           current page table: free_list,
 *                           shadow_list and page_table.
 * Parameters:
 *   MPOOL *mp
 *
 * Returns:
 *   none
 */
static void
mpool_trash_page_table(mp) 
   MPOOL *mp;
{
   FRAME *f,*nextframe;

   /*
    * We don't really need to change much, just 
    * free all the allocated memory, and set some
    * pointers to NULL, and it's as though the 
    * table and lists never existed.
    */
   for (f = mp->free_frames; f != NULL; f = nextframe) {
      nextframe = f->next;
      free (f);
   }
   mp->free_frames = NULL;

   for (f = mp->shadow_frames; f != NULL; f = nextframe) {
      nextframe = f->next;
      free (f);
   }
   mp->shadow_frames = NULL;

   if (mp->page_table) {
       free (mp->page_table);
   }

   mp->total_frames = 0;

#ifdef STATISTICS
   mp->nfree_frames = mp->nshadow_frames = 0;
#endif
}

/*
 * mpool_txn_begin
 *
 * Parameters:
 *   MPOOL *mp
 *
 * Returns:
 *   RET_ERROR, RET_SUCCESS
 */
/*
 * Changed to MACRO per code review.  Stored in MPOOL.H
 *
 * int mpool_txn_begin (mp)
 *     MPOOL *mp;
 * {
 *   mp->rollback_npages = mp->npages;
 * }
 */



/*
 * mpool_txn_commit - commit this txn to the non-volatile database
 *
 * Parameters:
 *   MPOOL *mp
 *
 * Returns:
 *   RET_ERROR, RET_SUCCESS
 */
int mpool_txn_commit (mp)
     MPOOL *mp;
{
   int new_current;
   int table_size;
   FRAME *f,*nextframe;

   /*
    * Check to see if there's no changes to commit
    */
   if (mp->shadow_frames == NULL) {
      return(RET_SUCCESS);
   }

   /*
    * Sync dirty blocks to disk 
    */
   if (mpool_sync (mp) != RET_SUCCESS) {
      return (RET_ERROR);
   }

   /*
    * Write page table to disk file that does not
    * have the current page table
    */
   new_current = mp->current == 0 ? 1 : 0;
   table_size = mp->npages * PAGE_TABLE_ENTRY_SIZE;
   if (lseek (mp->page_table_fd[new_current], 0, SEEK_SET) != 0)
     return (RET_ERROR);
   if (write (mp->page_table_fd[new_current], mp->page_table, table_size) !=
       table_size)
     return (RET_ERROR);
   if (fsync(mp->page_table_fd[new_current]) != RET_SUCCESS)
     return (RET_ERROR);

   /*
    * Tell disk to use the new page table
    */
   if (lseek (mp->fd, 0, SEEK_SET) != 0)
     return (RET_ERROR);
   if (write (mp->fd, &new_current, sizeof(new_current)) != sizeof(new_current))
     return (RET_ERROR);
   if (fsync(mp->fd) != RET_SUCCESS)
     return (RET_ERROR);

   /*
    * Remind ourselves which on disk page table is good too
    */
   mp->current = new_current;

   /*
    * Move shadow frames onto the free frame list 
    */
#ifdef DEBUG_TXN
      fprintf(stderr, "Freeing shadow frames: ");
#endif

   for (f = mp->shadow_frames; f != NULL; f = nextframe) {
      nextframe = f->next;
      f->next = mp->free_frames;
      mp->free_frames = f;
#ifdef STATISTICS
      mp->nfree_frames++;
      mp->nshadow_frames--;
#endif 
#ifdef DEBUG_TXN
      fprintf(stderr, "%d ",f->frame);
#endif
   }
   mp->shadow_frames = NULL;

#ifdef DEBUG_TXN
      fprintf(stderr, ".\n ");
#endif
   
#ifdef STATISTICS
   mp->commit++;
   if (mp->nfree_frames > mp->max_free_frames) 
     mp->max_free_frames = mp->nfree_frames;
#endif 

   return (RET_SUCCESS);
}

/*
 * mpool_txn_rollback (mp) 
 *
 * Parameters:
 *   MPOOL *mp
 * 
 * Returns:
 *   RET_ERROR, RET_SUCCESS
 */
int mpool_txn_rollback (mp)
     MPOOL *mp;
{
   int status;

   /*
    * Dump dirty pages from cache
    */
   mpool_trash_cache (mp); /* always success */
   mp->npages = mp->rollback_npages;

   /*
    * Read in on-disk current page table, this remains current
    *
    * Start over reading the page table in.  This isn't
    * the fastest way to accomplish the task, but it 
    * won't happen often.  We hope.
    */
   mpool_trash_page_table (mp); /* always success */
   status = mpool_load_page_table (mp);

#ifdef STATISTICS
   mp->rollback++;
#endif
   
   return (status);
}
/* *** G-L-O-R-I-A TXN CHANGES *** END */













