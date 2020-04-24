/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */

/*                              DEcorum file system
                                 Directory package
                            Directory buffer subpackage                 */

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>			/* osi_Alloc, osi_Free */
#include <dcedfs/episode/epi_thread.h>	/* osi_Sleep, etc. outside kernel */
#include <dcedfs/lock.h>		/* osi_dlock_t */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/episode/instrument.h>	/* tuning and monitoring stuff */
#include <dcedfs/episode/async.h>

#include <dir_buffer.h>			/* struct Dir_Handle */
#include <dir.h>			/* DIRPAGESIZE, LOGDIRPAGESIZE */


RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/dir/RCS/dir_buffer.c,v 4.32 1996/03/06 16:54:05 ota Exp $")

#define PHSIZE 32			/* page hash table size
					 * must be a power of two */

/* buffer package hashing function */

#define PHASH(dirHandle,page) (DHOP_HASH(dirHandle,page) & (PHSIZE-1))
#define PHASHB(buff,page) (DHOP_HASHB(buff,page) & (PHSIZE-1))

/* buffer descriptor */

struct dir_buffer {
    struct squeue lruq;		/* links to lru queue; should be first field */
    struct Dir_Handle dir;	/* handle for I/O to directory */
    long page;			/* page number within directory */
    struct dir_buffer *hashNext;/* hash chain pointer */
    caddr_t data;		/* pointer to buffer data area */
    char lockers;		/* how many times am I pointing to buffer */
    char hashIndex;		/* index into hash table */
    char inTransit;		/* waiting for data read */
};

/* Macros to convert between buffer and queue pointer */

#define QTOB(q) ((struct dir_buffer *) q)
#define BTOQ(b) (&b->lruq)

/* State of the buffer package */

static osi_mutex_t dir_buf_lock;
static osi_cv_t dir_buf_cv;

#ifdef KERNEL
static int dinit_flag = 0;
#endif /* KERNEL */

static struct dir_buffer *Buffers;
static char *BufferData;

static struct squeue lruheadobj;
static struct squeue *lruhead;		/* head of LRU queue */

static struct dir_buffer *phTable[PHSIZE];	/* page hash table */
static struct dir_buffer *LastBuffer; /* cache of size 1 to avoid hash searches */
static int nbuffers;		/* # buffers requested in call to dirb_Init */
static int calls=0;		/* # calls to dirb_Read */
static int ios=0;		/* # calls to dirb_Read that had to do I/O */
static int waiters=0;		/* # processes waiting for FindBuffer */
static struct dh_ops nullops = {0, 0, 0, 0, 0, 0};
static int hashSize = PHSIZE;	/* size of the page hash table */
/* Forward declarations */

static struct dir_buffer *FindBuffer(struct Dir_Handle *dir, long apage);
static void RehashBuffer(struct dir_buffer *ap, long page);

/*
 * the list of variables in the directory buffer package that can be monitored.
 * These are supported by ins_dirbuf procedure.
 */
static struct ins_varTableEntry varTable[] = {
    {(long *)&nbuffers,sizeof(nbuffers)},	/* index 0 */
    {(long *)&calls,sizeof(calls)},		/* index 1 */
    {(long *)&ios,sizeof(ios)},			/* index 2 */
    {(long *)&waiters,sizeof(waiters)},		/* index 3 */
    {(long *)&hashSize,sizeof(hashSize)}	/* index 4 */
    /* the rest are computed */
};

static long varTableSize = sizeof(varTable)/sizeof(struct ins_varTableEntry);


/*
 * get or set the value of variable indicated by <vars>->varIdx. The
 * result of lookup is stored in <resultValues>. Object Id parameters
 * are ignored for now. --This proc should be generalized for common
 * use by all packages.
 */
/* EXPORT */
int ins_dirbuf(opcode,vars,argObjIds,resultObjIds,resultValues)
  struct ins_opDesc *opcode;
  struct ins_varDesc *vars;
  struct ins_basicObjectId *argObjIds;
  struct ins_basicObjectId *resultObjIds;
  struct ins_basicObject *resultValues;
{
   if (vars->varIdx >= 0 && vars->varIdx < varTableSize) {
	/* the variable index is within range */
	if (opcode->opcode == INS_GET) {
	    bcopy ((caddr_t) varTable[vars->varIdx].varAddr,
		   (caddr_t) resultValues->value,
		   varTable[vars->varIdx].varSize);
	} else if (opcode->opcode == INS_SET) {
	    /* not writeable */
	    return EINVAL;
	    /*
	      bcopy ((caddr_t) resultValues->value,
		     (caddr_t) varTable[vars->varIdx].varAddr,
		     varTable[vars->varIdx].varSize);
	     */
	} else if (opcode->opcode == INS_GETNEXT) {
	    /* not yet supported */
	}
	return 0;
    } else {
	return EINVAL;
    }
}


/*
 * dirb_Stat -- Display package status variables.
 *
 * abuffers is set to the number of buffers.
 * acalls is set to the number of calls to dirb_Read.
 * aios is set to the number of calls to dirb_Read that resulted in
 *   calls to dh_Read.
 */

/* EXPORT */
void dirb_Stat(int *abuffers, int *acalls, int *aios)
{
    *abuffers = nbuffers;
    *acalls = calls;
    *aios = ios;
}

/*
 * dirb_Init -- Initialize directory buffer package.
 */

/* EXPORT */
int dirb_Init(struct dirb_params *dirbParams)
{
    int i;
    struct dir_buffer *tb;
    struct squeue *tq;
    int abuffers = dirbParams->abuffers;
    int code;

    if (code = epig_Init(&dirbParams->epigParams))
	return code;

#ifdef KERNEL
    if (dinit_flag) return 0;
    dinit_flag = 1;
#endif /* KERNEL */

    osi_mutex_init(&dir_buf_lock);
    osi_cv_init(&dir_buf_cv);
    Buffers = osi_Alloc_r(abuffers * sizeof (struct dir_buffer));
    BufferData = osi_Alloc_r(abuffers * DIRPAGESIZE);
    lruhead = &lruheadobj;
    QInit(lruhead);
    LastBuffer = Buffers;
    nbuffers = abuffers;
    for (i = 0 ; i < PHSIZE; i++)
	phTable[i] = 0;
    for (i = 0; i < abuffers; i++) {
	/* Fill in each buffer with an empty indication. */
	tb = &Buffers[i];
	bzero((caddr_t)&tb->dir, sizeof (tb->dir));
	tb->dir.dh_ops = &nullops;
	tq = BTOQ(tb);
	QAdd(lruhead, tq);
	tb->data = &BufferData[DIRPAGESIZE * i];
	tb->hashIndex = tb->lockers = tb->inTransit = 0;
    }

    return 0;
}

static void
ReadBuffer(
  struct Dir_Handle *dir,	/* directory */
  int page,			/* page number within directory */
  struct dir_buffer *tb)	/* pointer to buffer header */
{
    u_int len;
    int code;

    /* read into the buffer */
    if (page == 0) {
	len = DHOP_LENGTH(dir);
	len = MIN(len, DIRPAGESIZE);
    } else
	len = DIRPAGESIZE;

    /* no one may use the buffer while we unlock the lock */

    tb->inTransit = 1;
    tb->lockers++;

    osi_mutex_exit(&dir_buf_lock);
    code = DHOP_READ(dir, page * DIRPAGESIZE, tb->data, len);
    osi_mutex_enter(&dir_buf_lock);
    afsl_Assert(code != 0);	/* i.e., success */

    tb->inTransit = 0;
    tb->lockers--;
    if (waiters != 0)
	osi_cv_broadcast(&dir_buf_cv);

    ios++;
}

/*
 * dirb_Read -- Access a directory page.
 */

/* EXPORT */
caddr_t dirb_Read(
  struct Dir_Handle *dir,		/* directory */
  int page)				/* page number within directory */
{
    return dirb_Read0(dir, page, NULL);
}
 
/*
 * dirb_Read0 -- Access a directory page.
 */

/* EXPORT */
caddr_t dirb_Read0(
  struct Dir_Handle *dir,	/* directory */
  int page,			/* page number within directory */
  u_int *reallength)		/* pointer to # bytes read */
{
    struct dir_buffer *tb;	/* pointer to buffer header */
    struct squeue *tq;		/* lruq field of tb */

    calls++;
    if (reallength != NULL)
	*reallength = 0;

    osi_mutex_enter(&dir_buf_lock);

    do {
	/* check if LastBuffer is the one we want */
	if (LastBuffer->page == page && DHOP_EQ(dir, LastBuffer))
	    tb = LastBuffer;
	else {
	    /* check if any buffer is the one we want */
	    tb = phTable[PHASH(dir, page)];
	    while (tb != NULL) {
		if (tb->page == page && DHOP_EQ(dir, tb))
		    break;
		tb = tb->hashNext;
	    }

	    /* must anoint a buffer */
	    if (tb == NULL) {
		tb = FindBuffer(dir, page);
		if (tb != NULL) {
		    afsl_Assert(!tb->inTransit);
		    ReadBuffer(dir, page, tb);
		}
	    }
	}

	if (tb == NULL || tb->inTransit) {
	    waiters++;
	    osi_cv_wait(&dir_buf_cv, &dir_buf_lock);
	    waiters--;
	    tb = NULL;
	}

    } while (tb == NULL);

    LastBuffer = tb;
    if (reallength != NULL && page == 0) {
	*reallength = DHOP_LENGTH(dir);
	if (*reallength > DIRPAGESIZE)
	    *reallength = DIRPAGESIZE;
    }
    tb->lockers++;
    tq = BTOQ(tb);
    QRemove(tq);
    QAdd(lruhead, tq);
    osi_mutex_exit(&dir_buf_lock);
    return tb->data;
}

/*
 * FindBuffer - find a buffer for a directory page.
 *
 * We use an LRU algorithm.
 *
 * In AFS 3.0, the LRU algorithm took note of dirty vs. non-dirty buffers.
 * In DEcorum, we do not have dirty buffers, since we write immediately,
 *   so the LRU algorithm looks only at time of last access.
 */

static struct dir_buffer *FindBuffer(
    struct Dir_Handle *dir,		/* directory */
    long apage)				/* page number within directory */
{
    struct dir_buffer *tb;		/* buffer pointer */

    tb = QTOB(lruhead->prev);
    while (tb != QTOB(lruhead) && tb->lockers != 0)
	tb = QTOB(tb->lruq.prev);

    if (tb == QTOB(lruhead))
	return 0;

    /* Now fill in the header. */
    DHOP_COPY(dir, tb);
    tb->page = apage;

    RehashBuffer(tb, apage);

    return tb;
}

/*
 * RehashBuffer -- put a buffer in a new hash bucket.
 *
 * First we unlink the buffer from its current hash chain (if it is on one),
 * determined from its hashIndex field.
 * Then we put it on the hash chain calculated from its current DirHandle,
 * and update the hashIndex field accordingly.
 *
 * If a buffer has never been used, its hashIndex is 0, but it will not
 * be found on hash chain 0.  This code handles that case by simply falling
 * through if it fails to find the buffer on the appropriate hash chain.
 */

static void RehashBuffer(struct dir_buffer *ap, long page)
{
    struct dir_buffer **lp, *tp;
    int i;

    i = ap->hashIndex;
    lp = &phTable[i];
    for (tp = *lp; tp; tp=tp->hashNext) {
	if (tp == ap) {
	    *lp = tp->hashNext;
	    break;
	}
	lp = &tp->hashNext;
    }

    /* now figure the new hash bucket */
    i = PHASHB (ap, page);
    ap->hashIndex = i;		/* remember where we are for deletion */
    ap->hashNext = phTable[i];	/* add us to the list */
    phTable[i] = ap;
}

/*
 * dirb_Release -- End of accessing of directory page.
 */

/* EXPORT */
void dirb_Release(caddr_t dp)
{
    int index;
    struct dir_buffer *bp;

    if (dp == NULL)
	return;

    index = (((char *)dp) - ((char *)BufferData)) >> LOGDIRPAGESIZE;
    bp = &Buffers[index];
    osi_mutex_enter(&dir_buf_lock);
    bp->lockers--;
    if (waiters != 0)
	osi_cv_broadcast(&dir_buf_cv);
    osi_mutex_exit(&dir_buf_lock);
}

/*
 * dirb_AnnulPage -- Disassociate a single buffer from directory.
 *
 * Assumes that the buffer is not in transit.
 */

/* EXPORT */
void dirb_AnnulPage(
    struct Dir_Handle *dir,	/* dir handle */
    int page)			/* page whose buffer is invalid */
{
    struct dir_buffer *tb;

    osi_mutex_enter(&dir_buf_lock);
    for (tb = phTable[PHASH(dir, page)]; tb; tb = tb->hashNext) {
	if (tb->page == page && DHOP_EQ(dir, tb)) {
	    DHOP_ZEROB(tb);
	    break;
	}
    }
    osi_mutex_exit(&dir_buf_lock);
}

/*
 * dirb_Annul -- Disassociate buffers from directory.
 */

/* EXPORT */
void dirb_Annul(struct Dir_Handle *dir)
{
    struct dir_buffer *tb;
    int i;

    osi_mutex_enter(&dir_buf_lock);
    for (i = 0; i < PHSIZE; i++) {
	for (tb = phTable[i]; tb != NULL; tb = tb->hashNext) {
	    if (DHOP_EQ(dir, tb))
		DHOP_ZEROB(tb);
	}
    }
    osi_mutex_exit(&dir_buf_lock);
}

/*
 * dirb_New -- Access a newly created directory page.
 *
 * We do not try to read the page, since it doesn't exist yet.
 * Note that we don't even zero the buffer.  Perhaps will have to fix this.
 */

/* EXPORT */
caddr_t dirb_New(
  struct Dir_Handle *dir,		/* directory */
  int page)				/* page within directory */
{
    struct dir_buffer *tb;		/* buffer pointer */
    struct squeue *tq;			/* LRU queue field of tb */

    osi_mutex_enter(&dir_buf_lock);
    while ((tb = FindBuffer(dir, page)) == NULL) {
	waiters++;
	osi_cv_wait(&dir_buf_cv, &dir_buf_lock);
	waiters--;
    }

    LastBuffer = tb;
    tb->lockers++;
    tq = BTOQ(tb);
    QRemove(tq);
    QAdd(lruhead, tq);
    osi_mutex_exit(&dir_buf_lock);
    return tb->data;
}
