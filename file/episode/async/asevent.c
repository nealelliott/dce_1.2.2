/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: asevent.c,v $
 * Revision 1.1.20.1  1996/10/02  17:20:08  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:25  damon]
 *
 * $EndLog$
*/
/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

/*
 *			Asynchronous Event Package
 *
 * This package provides control over asynchronous events, such as I/O, and
 * sets of events.  It is built upon two facilities found in most Unix kernels:
 * the sleep/wakeup facility, and buffered I/O using device drivers.
 *
 * This package also keeps a table of "device handles" (async_device_t),
 * recording block and fragment sizes on each aggregate.
 */

#include <dcedfs/param.h>
/*
 * XXX FIX ME XXX
 *
 * In SunOS 5.x, osi_device.h includes <sys/mman.h> (after several levels
 * of indirection), which defines PRIVATE and SHARED.  These definitions
 * conflict with the definitions in stds.h, so we include osi_device.h
 * first and then undefine PRIVATE and SHARED.
 */
#include <dcedfs/osi_device.h>
#undef PRIVATE
#undef SHARED
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_intr.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/epi_thread.h>

#ifdef AFS_HPUX_ENV
#include <sys/rtprio.h>
#include <sys/conf.h>
#endif /* AFS_HPUX_ENV */

#include <async.h>
#include <asevent_debug.h>

#ifndef KERNEL
#include <us.h>				/* get User Space disk-device */
#endif

#ifdef AFS_DEBUG
char *async_debugFilename[2 + 1] = { "", "asevent.c", "us_io.c" };
#endif /* AFS_DEBUG */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/async/asevent.c,v 1.1.20.1 1996/10/02 17:20:08 damon Exp $")

/* Locking rules:
 *
 * Lock events before sets, since this is what SignalEvent needs to do.
 *
 * Other restrictions on this package:
 * 1.  An event may be in at most one set at a time.
 * 2.  Many processes can wait for a single event.  Thus, every waiter
 *     must hold the event, to keep it from being freed before all waiters
 *     have been awakened.
 * 3.  The user must call either WaitEvent or StatEvent before attempting to
 *     discard an event.  Waiting on a set full of events is not sufficient.
 */

#if 0
/*
 * Embedded and Independent event structures
 *
 * An event structure (asevent_event_t) can either be embedded, that is,
 * a substructure of some other structure, or it can be independent.  If it is
 * independent, it is initialized by asevent_Create, which calls osi_Alloc to
 * allocate space for it.  If it is embedded, it is initialized by
 * asevent_Initialize.
 *
 * An independent event is distinguished by having the ASEVENT_CREATED bit
 * set.  Functions such as asevent_Release are carefully written to handle
 * both embedded and independent events.
 */
#endif /* 0 */

/*
 * Implementation of event set waiting
 *
 * In most kernels, the buf structure includes a pointer to an ``iodone''
 * function.  This function, if present, is called when the device driver
 * knows that an I/O operation has finished.  If no iodone function is
 * present, a standard system iodone function is called.
 *
 * We use this facility to implement event set waiting.  When  we want to
 * wait for any event of a set to complete, we go to sleep on the set.
 * When I/O completes on any event in the set, the iodone function wakes
 * up the set.  If we want to wait for all events of a set to complete,
 * the iodone function decrements the set's counter of events to wait
 * for.  If the counter is zero, it wakes up the set.
 */
#if defined(AFS_AIX_ENV)
#include <sys/sleep.h>
#ifdef KERNEL
#include <sys/malloc.h>
#endif /* KERNEL */
#endif /* AFS_AIX_ENV */

#define DEBUG_THIS_FILE ASYNC_DEBUG_ASEVENT

#include <async.h>

int afsdb_episode_async = 0;

#if defined(AFS_AIX_ENV) && defined(_KERNEL)
#define make_asevent_obj(T) ((T *)xmalloc(sizeof (T), 0, pinned_heap))
#else
#define make_asevent_obj(T) ((T *)osi_Alloc_r(sizeof (T)))
#endif /* AFS_AIX_ENV && _KERNEL */

#ifdef FREEZE_AGGR_OSF_2
#ifdef AFS_OSF_ENV
unsigned int frozenAggrCount = 0;
#endif /* AFS_OSF_ENV */
#endif /* FREEZE_AGGR_OSF_2 */

#define ToBufferBlocks(adevice, ablock, aoffset) \
    (abtodb(adevice, ablock) + btodb(aoffset))
#define FromBufferBlocks(dev, b) dbtoab(dev, b)
#define BlockRoundup(b) dbroundup(b)

/*
 * Used in asevent_QueueIO() and asevent_Iodone() for simulating
 * I/O errors.  These are for testing purposes only.
 */
static long iodoneCalls = 0;
#ifdef AFS_DEBUG
static long ioerrErrno = 0;
static long ioerrFuse = 0;
static long ioerrMasterFuse = 0;
#endif /* AFS_DEBUG */

/*
 * variable indices supported by ins_async()
 */
#define IODONE_CALLS		1
#ifdef AFS_DEBUG
#define IOERR_ERRNO		2
#define IOERR_FUSE		3
#define IOERR_MASTER_FUSE	4
#endif /* AFS_DEBUG */

/*
 * hook for implementing monitoring and tuning specific to this submodule.
 */
/* EXPORT */
int ins_async(
  /* IN */ struct ins_opDesc *opcode,
  /* IN */ struct ins_varDesc *vars,
  /* IN */ struct ins_basicObjectId *argObjIds,
  /* OUT */ struct ins_basicObjectId *resultObjIds,
  /* OUT */ struct ins_basicObject *resultValues)
{
    unsigned allowed_ops = INS_GET;
    long *dataAddr;
    long dataSize;

    dataAddr = (long *)0;
    dataSize = 0;

    switch (vars->varIdx) {
      case IODONE_CALLS:
	dataAddr = &iodoneCalls;
	dataSize = sizeof(iodoneCalls);
	break;
#ifdef AFS_DEBUG
      case IOERR_ERRNO:
	allowed_ops |= INS_SET;
	dataAddr = &ioerrErrno;
	dataSize = sizeof(ioerrErrno);
	break;
      case IOERR_FUSE:
	allowed_ops |= INS_SET;
	dataAddr = &ioerrFuse;
	dataSize = sizeof(ioerrFuse);
	break;
      case IOERR_MASTER_FUSE:
	allowed_ops |= INS_SET;
	dataAddr = &ioerrMasterFuse;
	dataSize = sizeof(ioerrMasterFuse);
	break;
#endif /* AFS_DEBUG */
      default:
	return EINVAL;
    }

    afsl_Assert(dataSize <= INS_MAXOBJLEN);

    if (!(opcode->opcode & allowed_ops))
	return EINVAL;

    switch (opcode->opcode) {
      case INS_SET:
	bcopy((caddr_t)resultValues->value, (caddr_t)dataAddr, dataSize);
	break;
      case INS_GET:
	bcopy((caddr_t)dataAddr, (caddr_t)resultValues->value, dataSize);
	break;
      case INS_GETNEXT:
	/* not implemented */
	break;
      default:
	return EINVAL;
    }

    return 0;
}

/* SHARED */
u_long
async_ToBufferBlocks(async_device_t *device, daddr_t block)
{
    return ToBufferBlocks(device, block, 0);
}

/* SHARED */
daddr_t
async_FromBufferBlocks(async_device_t *device, u_long block)
{
    return FromBufferBlocks(device, block);
}

/* FORWARD */
static async_device_t *FindDevice(long, long);

/*
 * asevent_Reinit -- get ready to start I/O
 */
static void
asevent_Reinit(asevent_event_t *ae)
{
    ae->flags &= ~ASEVENT_SIGNALLED;
    ae->flags |= ASEVENT_INTRANSIT;
    ae->code = 0;
}

/*
 * asevent_Initialize -- initialize a new event
 */
void
asevent_Initialize(asevent_event_t *ae, opaque data)
{
    bzero((caddr_t)ae, sizeof (*ae));
    ae->refCount = 1;
    ae->data = data;
    epi_mutex_init(&ae->lock, "asevent");
    osi_cv_init (&ae->cv);
}

/*
 * asevent_Create -- create and initialize an (embedded) event structure
 */

static struct aseventFreeList {
    asevent_event_t *first;	/* first element */
    epi_mutex_t lock;		/* mutex */
} aseventFreeList;

/* EXPORT */
asevent_event_t *
asevent_Create(short flags)
{
    asevent_event_t *te;

    epi_mutex_lock(&aseventFreeList.lock);
    te = aseventFreeList.first;
    if (te != NULL)
	aseventFreeList.first = te->next;
    epi_mutex_unlock(&aseventFreeList.lock);

    if (te == NULL)
	te = make_asevent_obj(asevent_event_t);

    asevent_Initialize(te, NULL);
    te->flags = (flags | ASEVENT_CREATED);

    return te;
}

/*
 * asevent_InitializeSet -- initialize a new set
 */
void
asevent_InitializeSet(asevent_set_t *as)
{
    bzero((caddr_t)as, sizeof (*as));
    as->refCount = 1;
    epi_mutex_init(&as->lock, "asevent set");
    osi_cv_init (&as->cv);
}

static struct aseventsetFreeList {
    asevent_set_t *first;	/* first element */
    epi_mutex_t lock;
} aseventsetFreeList;

/*
 * asevent_CreateSet -- create an event set structure
 */

/* EXPORT */
asevent_set_t *
asevent_CreateSet(void)
{
    asevent_set_t *te;

    epi_mutex_lock(&aseventsetFreeList.lock);
    te = aseventsetFreeList.first;
    if (te != NULL)
	aseventsetFreeList.first = te->next;
    epi_mutex_unlock(&aseventsetFreeList.lock);

    if (te == NULL)
	te = make_asevent_obj(asevent_set_t);

    asevent_InitializeSet(te);
    te->flags |= ASEVENT_SETCREATED;

    return te;
}

/*
 * asevent_AddToSet -- add an event to an event set structure
 */

/* EXPORT */
long
asevent_AddToSet(asevent_set_t *aset, asevent_event_t *aevent)
{
    int s;
    long rcode = 0;

    /* each event in at most one set */
    epi_mutex_lock(&aevent->lock);
    epi_mutex_lock(&aset->lock);

    if (aevent->set != NULL) {
	rcode = ASEVENT_BUSY;		/* already in a set */
	goto done;
    }
    aevent->refCount++;

    osi_intr_exclude(&osi_iodoneLock, s);
    aevent->set = aset;

    /* thread on the list */
    aevent->next = aset->first;
    aset->first = aevent;

    aset->total++;			/* one more pending event */
    if (aevent->flags & ASEVENT_SIGNALLED)
	aevent->flags |= ASEVENT_IGNORESET;
    else {
	/* event has not yet been signalled */
	aset->pending++;
	aevent->flags &= ~ASEVENT_IGNORESET;
    }
    osi_intr_allow(&osi_iodoneLock, s);

  done:
    epi_mutex_unlock(&aset->lock);
    epi_mutex_unlock(&aevent->lock);
    return rcode;
}

/*
 * asevent_RemoveFromSet -- remove an event from an event set structure
 */
void
asevent_RemoveFromSet(asevent_set_t *aset, asevent_event_t *aevent)
{
    int s;
    asevent_event_t **le, *te;

    epi_mutex_lock(&aevent->lock);
    epi_mutex_lock(&aset->lock);

    le = &aset->first;
    te = *le;
    while (te != NULL && te != aevent) {
	le = &te->next;
	te = *le;
    }

    osi_intr_exclude(&osi_iodoneLock, s);
    if (te != NULL) {
	*le = te->next;
	if ((te->flags & (ASEVENT_SIGNALLED | ASEVENT_IGNORESET)) == 0)
	    aset->pending--;
	aset->total--;
    }
    aevent->set = NULL;
    osi_intr_allow(&osi_iodoneLock, s);

    epi_mutex_unlock(&aset->lock);
    epi_mutex_unlock(&aevent->lock);
    asevent_Release(aevent);
}

/*
 * Hash table mapping bufs to events; used by asevent_Iodone.
 */
typedef struct asevent_bufhash {
    asevent_event_t *h_head;	/* associated events */
} asevent_bufhash_t;

#define ASEVENT_BUFHASHSZ	127
#define ASEVENT_BUFHASH(bp)	((u_long)(bp) % ASEVENT_BUFHASHSZ)
static asevent_bufhash_t asevent_bufhash[ASEVENT_BUFHASHSZ];

/*
 * Enter an event in the hash table; the event should be locked.
 */
/* NOTRACE */
static void
bufhash_Enter(asevent_event_t *ae)
{
    asevent_bufhash_t *h = &asevent_bufhash[ASEVENT_BUFHASH(ae->buf)];

    ae->hash = h->h_head;
    h->h_head = ae;
}

/*
 * Remove an event from the hash table; the event should be locked.
 * This routine is called from asevent_DetachBuf as part of cleanup.
 * Since the entry is normally removed during iodone processing, we
 * should only find a match if there was an error.
 */
/* NOTRACE */
static void
bufhash_Remove(asevent_event_t *ae)
{
    asevent_event_t *p, *q;

    asevent_bufhash_t *h = &asevent_bufhash[ASEVENT_BUFHASH(ae->buf)];

    for (q = NULL, p = h->h_head; p != NULL && p != ae; p = p->hash)
	q = p;

    if (p != NULL) {
	if (q == NULL)
	    h->h_head = ae->hash;
	else
	    q->hash = ae->hash;
    }
    ae->hash = NULL;
}

/*
 * Look up event in the hash table using buf as key.
 */
/* NOTRACE */
static asevent_event_t *
bufhash_LookupAndRemove(struct buf *bp)
{
    asevent_event_t *e, *p;
    asevent_bufhash_t *h = &asevent_bufhash[ASEVENT_BUFHASH(bp)];

    for (p = NULL, e = h->h_head; e != NULL && e->buf != bp; e = e->hash)
	p = e;

    afsl_Assert(e != NULL);
    if (p == NULL)
	h->h_head = e->hash;
    else
	p->hash = e->hash;
    e->hash = NULL;

    return (e);
}

/*
 * update_event and update_set -- macros executed upon completion of an event
 */

#define update_event(aevent, buf) \
  MACRO_BEGIN \
    if ((buf) && ((aevent)->code == 0)) \
	(aevent)->code = geterror(buf); \
    (aevent)->flags |= ASEVENT_SIGNALLED; \
    osi_intr_wakeup(&aevent->cv); \
  MACRO_END

#define update_set(aset, aevent) \
  MACRO_BEGIN \
    if (!((aevent)->flags & ASEVENT_IGNORESET)) { \
	if ((aset)->code == 0) \
	    (aset)->code = (aevent)->code; \
	(aset)->pending--; \
	(aevent)->flags |= ASEVENT_IGNORESET; \
	if (((aset)->pending == 0) || ((aset)->flags & ASEVENT_WAITINGANY)) \
	    osi_intr_wakeup(&aset->cv); \
    } \
  MACRO_END

#if 0
/*
 * SignalSet -- explicitly notify an event set of a completion
 */

static void
SignalSet(asevent_set_t *ts, asevent_event_t *ae)
{
    int s;

    osi_intr_exclude(&osi_iodoneLock, s);
    update_set(ts, ae);
    osi_intr_allow(&osi_iodoneLock, s);
}

/*
 * asevent_SignalEvent -- explicitly notify an event of a completion
 * This should be used only with ``unbuffered'' events.
 */
void
asevent_SignalEvent(asevent_event_t *ae, long acode)
{
    int s;

    osi_intr_exclude(&osi_iodoneLock, s);

    if (acode != 0 && ae->code == 0)
	ae->code = acode;

    if (!(ae->flags & ASEVENT_SIGNALLED)) {
	update_event(ae, 0);
	if (ae->set != NULL)
	    update_set(ae->set, ae);
    }

    osi_intr_allow(&osi_iodoneLock, s);
}
#endif /* 0 */

/*
 * asevent_Hold -- increment reference count of an event
 */
void
asevent_Hold(asevent_event_t *ae)
{
    epi_mutex_lock(&ae->lock);
    afsl_Assert(ae->refCount > 0);
    ae->refCount++;
    epi_mutex_unlock(&ae->lock);
}

/*
 * asevent_AttachBuf -- attach a buf to this event.
 */
static void
asevent_AttachBuf(asevent_event_t *ae, struct buf *bp)
{
    int s;
    afsl_Assert(ae->buf == NULL);
    ae->buf = bp;
    osi_intr_exclude(&osi_iodoneLock, s);
    bufhash_Enter(ae);
    osi_intr_allow(&osi_iodoneLock, s);
}

/*
 * asevent_DetachBuf -- break association between an event and its buf.
 *    This must be called after I/O on the buf has completed; the event
 *    should be locked.
 */
static void
asevent_DetachBuf(asevent_event_t *ae)
{
    int s;
    afsl_Assert(ae->buf != NULL);
    osi_intr_exclude(&osi_iodoneLock, s);
    bufhash_Remove(ae);
    osi_intr_allow(&osi_iodoneLock, s);
    ae->buf = NULL;
}

/* FORWARD */
static void asevent_CleanBuf(asevent_event_t *ae);

/*
 * Asevent_Delete -- delete an event
 */
static void
asevent_Delete(asevent_event_t *ae)
{
    /*
     * There must be no pending I/O when the event is deleted.
     */
    afsl_Assert(!(ae->flags & ASEVENT_INTRANSIT));

    /*
     * Detach buf if necessary
     */
    if (ae->buf != NULL) {
	asevent_CleanBuf(ae);
	asevent_DetachBuf(ae);
    }

    epi_mutex_unlock(&ae->lock);
    epi_mutex_destroy(&ae->lock);
    osi_cv_destroy (&ae->cv);

    if (ae->flags & ASEVENT_CREATED) {
	/*
	 * place on free list
	 */
	epi_mutex_lock(&aseventFreeList.lock);
	ae->next = aseventFreeList.first;
	aseventFreeList.first = ae;
	epi_mutex_unlock(&aseventFreeList.lock);
    }
}

/*
 * asevent_Release -- decrement reference count of an event, and
 *		      release it if necessary
 */
void
asevent_Release(asevent_event_t *ae)
{
    epi_mutex_lock(&ae->lock);
    afsl_Assert(ae->refCount > 0);
    if (--ae->refCount == 0)
	asevent_Delete(ae);
    else
	epi_mutex_unlock(&ae->lock);
}

/*
 * asevent_HoldSet -- increment reference count of an event set
 */
void
asevent_HoldSet(asevent_set_t *as)
{
    epi_mutex_lock(&as->lock);
    as->refCount++;
    epi_mutex_unlock(&as->lock);
}

/*
 * asevent_DeleteSet -- delete an event set
 */
static void
asevent_DeleteSet(asevent_set_t *as)
{
    asevent_event_t *te, *next;

    te = as->first;
    while (te != NULL) {
	afsl_Assert(te->set == as);
	next = te->next;
	te->set = NULL;
	asevent_Release(te);
	te = next;
    }

    epi_mutex_unlock(&as->lock);
    epi_mutex_destroy(&as->lock);
    osi_cv_destroy (&as->cv);

    if (as->flags & ASEVENT_SETCREATED) {
	epi_mutex_lock(&aseventsetFreeList.lock);
	as->next = aseventsetFreeList.first;
	aseventsetFreeList.first = as;
	epi_mutex_unlock(&aseventsetFreeList.lock);
    } else {
	as->first = NULL;
	as->refCount = 0;
	as->total = as->pending = 0;
    }
}

/*
 * asevent_ReleaseSet -- decrement reference count of an event set, and
 *			 release it if necessary
 */
void
asevent_ReleaseSet(asevent_set_t *as)
{
    epi_mutex_lock(&as->lock);
    if (--as->refCount == 0)
	asevent_DeleteSet(as);
    else
	epi_mutex_unlock(&as->lock);
}

static osi_iodone_t asevent_Iodone(struct buf *bp); /* forward decl. */

/*
 * asevent_MakeBuf -- create a struct buf for use in an event.
 */
struct buf *
asevent_MakeBuf(
  int flags,			/* buf flags */
  caddr_t addr,			/* data area */
  size_t length,		/* transfer length */
  async_device_t *aggr,		/* aggregate info */
  long ablock,			/* file system block no. */
  long aoffset)			/* offset in FS block */
{
    struct buf *bp = osi_GetBuf_r();
    daddr_t dblk = ToBufferBlocks(aggr, ablock, aoffset);
    long count = BlockRoundup(length);
    dev_t device = osi_makedev(aggr->majdev, aggr->mindev);

    osi_BufInit(bp, flags, addr, length, device, dblk, NULL, asevent_Iodone);
    return bp;
}

/*
 * asevent_CleanBuf -- do cleanup processing on this event's buf.
 * I/O must have completed, i.e., ASEVENT_INTRANSIT must not be set.
 * The event should be (read-)locked.
 *
 * On SunOS, we do not clean up here if B_PAGEIO is set, since in
 * that case, the buf was given to us by the system and not created
 * in asevent_MakeBuf.  We leave the cleanup to the higher-level code.
 *
 * Otherwise, we will call osi_bio_cleanup to do the cleanup.  If B_ASYNC
 * is true, we expect osi_bio_cleanup to free the buffer; we free it
 * ourselves by calling osi_ReleaseBuf in the synchronous case.
 *
 * XXX We assume that the buf was obtained by calling asevent_MakeBuf
 * except in the B_PAGEIO case.  This may be a portability problem in the
 * future.
 */
static void
asevent_CleanBuf(asevent_event_t *ae)
{
    int s, release;
    struct buf *bp = ae->buf;

    if (bp != NULL) {
#ifdef AFS_SUNOS5_ENV
	if (bp->b_flags & B_PAGEIO)
	    return;
#endif /* AFS_SUNOS5_ENV */
	release = !(bp->b_flags & B_ASYNC);
	osi_intr_exclude(&osi_iodoneLock, s);
	afsl_Assert(!(ae->flags & ASEVENT_INTRANSIT));
	osi_intr_allow(&osi_iodoneLock, s);
	osi_bio_cleanup(bp, &osi_biowaitCond);
	if (release)
	    osi_ReleaseBuf_r(bp);
    }
}

/*
 * asevent_WaitEvent -- wait for completion of an event.  The basic primitive
 *     for I/O events is osi_bio_wait, which has some fairly complex
 *     requirements.  See its definition for details.
 */
long
asevent_WaitEvent(asevent_event_t *ae)
{
    int s;
    struct buf *tb;
    long acode;

    epi_mutex_lock(&ae->lock);
    osi_intr_exclude(&osi_iodoneLock, s);

    while (ae->flags & ASEVENT_INTRANSIT) {
	/*
	 * If here, we must wait for buffer I/O to complete; the event
	 * lock protects us from races with asevent_StatEvent or other
	 * waiters.
	 */
	osi_bio_wait(ae->buf, &osi_biowaitCond, &osi_iodoneLock);
	if (ae->flags & ASEVENT_SIGNALLED) {
	    ae->buf->b_flags &= ~B_BUSY;
	    ae->flags &= ~ASEVENT_INTRANSIT;
	}
    }

    acode = ae->code;
    osi_intr_allow(&osi_iodoneLock, s);
    epi_mutex_unlock(&ae->lock);

    return acode;
}

/*
 * asevent_WaitAll -- wait for completion of all events in an event set
 */
long
asevent_WaitAll(asevent_set_t *as)
{
    long acode;
    int s;

    osi_intr_exclude(&osi_iodoneLock, s);

    while (as->pending != 0) {
	afsl_Trace (AFSL_TR_SLEEP,
		    ("asevent_WaitAll: sleeping on set (%x) with %d pending",
		     as, as->pending));
	osi_intr_wait(&as->cv, &osi_iodoneLock);
    }

    acode = as->code;
    osi_intr_allow(&osi_iodoneLock, s);

    return acode;
}

/*
 * asevent_WaitAny -- wait for completion of any event in an event set.
 *     Returns when the set contains at least one completed event.
 */
long
asevent_WaitAny(asevent_set_t *as)
{
    long acode;
    int s;

    osi_intr_exclude(&osi_iodoneLock, s);
    as->flags |= ASEVENT_WAITINGANY;

    while (as->pending == as->total && as->pending != 0) {
	afsl_Trace(AFSL_TR_SLEEP,
		    ("asevent_WaitAny: sleeping on set (%x) with %d pending",
		     as, as->pending));
	osi_intr_wait(&as->cv, &osi_iodoneLock);
    }

    acode = as->code;
    osi_intr_allow(&osi_iodoneLock, s);

    return acode;
}

/*
 * Device handle support
 */

static osi_dlock_t devLock;	/* for allocation of new devices */

/* If we exceed this limit we should use a hash table or something faster than
 * a linear list.  This constant can be patched. */

#ifdef EPI_MAX_DEVICES
static unsigned maxDevices = EPI_MAX_DEVICES;
#else
static unsigned maxDevices = 0xffffffff;
#endif /* EPI_MAX_DEVICES */

static struct fifo devFreeList;       /* list of free devices */
static struct fifo devActiveList;     /* list of active devices */
static struct dfsh_hashTable devHash; /* hash table */

/*
 * we XOR the major # left shifted 16 bits into the minor device #.
 */
#define HASHDEV(maj, min) (((maj) << 16) ^ (min))

/*
 * HashDevice -- compute device hash code
 */
static u_long HashDevice(async_device_t *dP)
{
      return HASHDEV(dP->majdev, dP->mindev);
}

/*
 * asevent_Init -- package initialization code
 */
void asevent_Init(void)
{
      static u_int initialized;
      async_device_t *dP = NULL;
      long code;

      if (initialized)
	  return;
      initialized = 1;

      epi_Init();			/* initialize tools layer */

      osi_mutex_init (&osi_iodoneLock);
      osi_cv_init (&osi_biowaitCond);

      epi_mutex_init(&aseventFreeList.lock, "asevent free list");
      epi_mutex_init(&aseventsetFreeList.lock, "asevent set free list");

      lock_InitDesc(&devLock, "asevent dev");
      lock_ObtainWrite_r(&devLock);

      FIFO_INIT(&devFreeList, offsetof(async_device_t, statusThread));
      FIFO_INIT(&devActiveList, offsetof(async_device_t, statusThread));

      code = dfsh_HashInit(&devHash, HashDevice,
			   offsetof(async_device_t, next));
      afsl_MBZ(code);

      /*
       * We assume the majdev and mindev are adjacent in the device handle.
       */
      code = dfsh_HashInitKeyFixed(&devHash,
				   offsetof(async_device_t, majdev),
				   sizeof(dP->majdev) + sizeof(dP->mindev));
      afsl_MBZ(code);

      lock_ReleaseWrite_r(&devLock);
}

static async_device_t *
FindDevice(long maj, long min)
{
      unsigned hash;
      async_device_t *dP;

      lock_ObtainRead_r(&devLock);
      hash = HASHDEV(maj, min);
      DFSH_LOOKUP(async_device_t *, &devHash, dP, hash,
		  (hash == HASHDEV(dP->majdev, dP->mindev)));
      lock_ReleaseRead_r(&devLock);
      return dP;
}

/*
 * asevent_InitDevice --
 *
 *	If size is 0, then lookup an existing device.
 *	Otherwise create a new device.
 */


/* EXPORT */
long asevent_InitDevice(
  /* IN */ long maj,			/* major device number to use */
  /* IN */ long min,			/* minor device */
  /* IN */ u_long size,			/* block size of device */
  /* OUT */ async_device_t **deviceP)	/* returned device */
{
      async_device_t *dP;
      int logBlockSize = 0;
      unsigned hash;

      asevent_Init ();			/* ... in case this is first call */
      lock_ObtainWrite_r(&devLock);
      hash = HASHDEV(maj, min);
      DFSH_LOOKUP(async_device_t *, &devHash, dP, hash,
		  (hash == HASHDEV(dP->majdev, dP->mindev)));

      if (dP) {
	  afsl_Assert(size == 0);
	  lock_ReleaseWrite_r(&devLock);
	  *deviceP = dP;
	  return 0;
      }

      /*
       * This is the code path for creating a new device
       */
      if (size == 0) {
	  lock_ReleaseWrite_r(&devLock);
	  return ASEVENT_NODEVICE;
      }
      if (devActiveList.length >= maxDevices) {
	  lock_ReleaseWrite_r(&devLock);
	  return ASEVENT_TOOMANYDEVICES;
      }
      /*
       * compute the log (base 2) value of the block size, verifying
       * that it really is a power-of-two value.
       * XXX We should have symbols for the minimum and maximum sizes.
       */
      logBlockSize = 10;	/* Minimum == 1KB */
      while (logBlockSize <= 16 && size != (1 << logBlockSize))
	  logBlockSize++;

      if (logBlockSize > 16) {	/* Maximum == 64KB */
	  lock_ReleaseWrite_r(&devLock);
	  return ASEVENT_BADARGS;
      }

      /*
       * The usual allocation algorithm - look on the free list, if there
       * isn't one, create one (unless we have too many already!
       */
      dP = (async_device_t *)FIFO_LookAtOldest(&devFreeList,
		offsetof(async_device_t, statusThread));
      if (dP == NULL) {
	  dP = osi_Alloc_r(sizeof (*dP));
	  bzero((char *)dP, sizeof (*dP));
      } else {
	  FIFO_REMOVE(&devFreeList, &dP->statusThread);
      }
      afsl_Assert(dP != NULL);

      /* now we initialize the sundry fields... */
      dP->majdev = maj;
      dP->mindev = min;
      dP->blkSize = size;
      dP->logBlkSize = logBlockSize;
      /*
       * and inject it into the various tables.  The active list probably
       * isn't critical, but it should prove useful for debugging, since
       * hash tables are very hard to traverse under the debugger.
       */
      DFSH_HASHIN_R(&devHash, dP);
      FIFO_ADD(&devActiveList, &dP->statusThread);

      /*
       * Done, so we drop the lock and return.
       */
      lock_ReleaseWrite_r (&devLock);
      *deviceP = dP;
      return 0;
}

/*
 * asevent_CloseDevice -- remove a device from the table
 */

/* EXPORT */
long asevent_CloseDevice(async_device_t *dP)
{
      lock_ObtainWrite_r (&devLock);
      dfsh_HashOut_r(&devHash, dP);
      FIFO_REMOVE(&devActiveList, &dP->statusThread);
      FIFO_ADD(&devFreeList, &dP->statusThread);
      lock_ReleaseWrite_r (&devLock);
      return 0;
}

/*
 * asevent_Iodone -- interrupt-time iodone processing.
 */
/* NOTRACE */
static osi_iodone_t
asevent_Iodone(struct buf *bp)
{
    asevent_event_t *ae;
    asevent_set_t *ts;

    osi_intr_start(&osi_iodoneLock);

#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
    afsl_Assert((bp->b_flags & B_DONE) != 0);
#else
    afsl_Assert((bp->b_flags & B_DONE) == 0);
#endif	/* AFS_AIX_ENV, AFS_HPUX_ENV */

#ifdef	AFS_AIX_ENV
    if (bp->b_error == ESOFT) {
	bp->b_resid = 0;
	bp->b_error = 0;
	bp->b_flags &= ~B_ERROR;
    }
#endif	/* AFS_AIX_ENV */

    ae = bufhash_LookupAndRemove(bp);

#if defined(AFS_AIX_ENV) && defined(_KERNEL)
    if (!(ae->flags & ASEVENT_NOPIN))
	unpin (bp->b_un.b_addr, bp->b_bcount);
#endif	/* AFS_AIX_ENV && _KERNEL */

    iodoneCalls++;

#ifdef AFS_DEBUG
    /*
     * If fuse has expired, simulate an error.  This code is for debugging
     * only.
     */
    if (ioerrErrno != 0 && ioerrFuse > 0 && --ioerrFuse == 0) {
	bp->b_flags |= B_ERROR;
	bp->b_error = ioerrErrno;
	bp->b_resid = bp->b_bcount;
	if (ioerrMasterFuse >= 0)
	    ioerrFuse = ioerrMasterFuse;
    }
#endif

    update_event (ae, bp);
    ts = ae->set;
    if (ts != NULL) {
	update_set (ts, ae);
    }
    osi_biodone(bp, &osi_biowaitCond);

    osi_intr_end(&osi_iodoneLock);
    osi_iodone_return(0);
}

#ifndef KERNEL
/* iodone -- provides a user space equivalent for this basic kernel primitive.
 *     Runs the iodone fcn for the buffer. */

void iodone(struct buf *bp)
{
    (void) b_Call(bp);
}

/*
 * geterror -- get error status for buf.
 */
int
geterror(struct buf *bp)
{
    if (bp->b_flags & B_ERROR) {
	if (bp->b_error == 0)
	    bp->b_error = EIO;
	return (bp->b_error);
    } else
	return 0;
}
#endif	/* !KERNEL */

/*
 * asevent_QueueIO -- start a buffered I/O event; may use only
 *    part of a block
 */
/* EXPORT */
long asevent_QueueIO (
  /* IN */ async_device_t *adevice,
  /* IN */ struct buf *bp,
  /* IN */ asevent_event_t *ae)
{
    long code;

#if AFSL_DEBUG_LEVEL >= 3
    while (adevice->sleep == 1) {
	osi_Sleep(&adevice->sleep);
    }
    if (adevice->sleep > 1)
	adevice->sleep--;
#endif /* AFSL_DEBUG_LEVEL */

    /*
     * Prepare event for I/O and attach buf.
     */
    epi_mutex_lock(&ae->lock);
    afsl_Assert(!(ae->flags & ASEVENT_INTRANSIT));
    asevent_Reinit(ae);
    asevent_AttachBuf(ae, bp);
    epi_mutex_unlock(&ae->lock);

    /*
     * Update kernel resource usage statistics.
     */
    if (bp->b_flags & B_READ)
	osi_inc_ru_inblock(1);
    else
	osi_inc_ru_oublock(1);

    /*
     * Start the I/O.
     */
#if defined(AFS_AIX_ENV) && defined(_KERNEL)
    bp->b_event = EVENT_NULL;
    if (!(ae->flags & ASEVENT_NOPIN))
	pin (bp->b_un.b_addr, bp->b_bcount);
#endif /* AFS_AIX_ENV && _KERNEL */
    code = osi_strategy(bp);
    if (code != 0) {
	epi_mutex_lock(&ae->lock);
	ae->flags &= ~ASEVENT_INTRANSIT;
	epi_mutex_unlock(&ae->lock);
    }

    return (code);
}

/*
 * asevent_EnumerateSet -- iterate over all events in an event set
 */

/* EXPORT */
asevent_event_t *
asevent_EnumerateSet(
  /* IN */ asevent_set_t *as,
  /* IN */ asevent_event_t *apreve)
{
    asevent_event_t *te;

    if (apreve == NULL)	{		/* find guy to proceed from */
	epi_mutex_lock(&as->lock);
	te = as->first;
	epi_mutex_unlock(&as->lock);
    } else {
	epi_mutex_lock(&apreve->lock);
	te = apreve->next;
	epi_mutex_unlock(&apreve->lock);
	asevent_Release(apreve);
    }
    if (te != NULL)
	asevent_Hold(te);

    return (te);
}

/*
 * asevent_StatEvent -- poll (explicitly check) for completion of an event.
 *     This function may be called instead of WaitEvent so we must follow the
 *     same rules for calling osi_bio_wait.  If the event has not been
 *     signalled the caller is still required to call WaitEvent or StatEvent.
 */
long
asevent_StatEvent(asevent_event_t *ae, long *astat)
{
    long acode;
    int s;

    osi_intr_exclude(&osi_iodoneLock, s);
    if (ae->flags & ASEVENT_SIGNALLED) {
	osi_intr_allow(&osi_iodoneLock, s);
	acode = asevent_WaitEvent(ae);
	*astat = 1;
    } else {
	acode = ae->code;
	osi_intr_allow(&osi_iodoneLock, s);
	*astat = 0;
    }
    return acode;
}

#if AFSL_DEBUG_LEVEL < 3
/* EXPORT */
long asevent_efs_debug(long op, long parm1, long parm2)
{
      return ERANGE;
}
#else
/*
 * asevent_efs_debug - debugging interface into this package
 */

/* EXPORT */
long asevent_efs_debug(long op, long parm1, long parm2)
{
      long code = 0;
      async_device_t *dP;
      unsigned tsleep;
      char *parm2p = *(char **)&parm2;	/* Groan... */

      struct core {
	  struct fifo_Thread thread;
	  char dummy[1024 - sizeof(struct fifo_Thread)];
      };
      static struct fifo pinnedCore;
      static int inited = 0;

      if (!inited) {
	  inited = 1;
	  FIFO_INIT (&pinnedCore, 0);
      }

      switch (op) {
	  /* For FREEZE ops --
	   *     parm1 is the async device
	   *     parm2p is a pointer to the new value.
	   */
	case EFS_DEBUG_ASYNC_FREEZE:
	case EFS_DEBUG_ASYNC_UNFREEZE:
	case EFS_DEBUG_ASYNC_QUERY:
	case EFS_DEBUG_ASYNC_TEST:
	  dP = FindDevice(osi_major(parm1), osi_minor(parm1));
	  if (!dP) {
	      return ENODEV;
	  }
	  break;

	  /* For PIN op --
	   *     parm1 is total amount to pin in Kilobytes.
	   */
	case EFS_DEBUG_ASYNC_PINCORE:
	  if (parm1 < 0) return EINVAL;
	  if (parm1 > 1000000) return EINVAL; /* pin a gigabyte? */
	  break;

	default:
	  return EINVAL;
      }

      switch (op) {
	case EFS_DEBUG_ASYNC_FREEZE:
	  code = osi_copyin_r((char *)parm2, (char *)&tsleep, sizeof(dP->sleep));
	  if (!code) {
	      if ( dP->sleep == 0 ) {
		  /* Freezing an unfrozen aggregate */
		  /* Prevent kernel deadlock on OSF/1 if an aggregate is frozen */
#ifdef AFS_OSF_ENV
#ifdef FREEZE_AGGR_OSF_1
		  /*
		   * Prevents only vnodes corresponding to frozen aggregate
		   * from being recycled
		   */
		  efs_freeze(dP);
#else  /* FREEZE_AGGR_OSF_1 */
#ifdef FREEZE_AGGR_OSF_2
		  /*
		   * Prevents any DFS vnode from being recycled
		   */
		  frozenAggrCount++;
#endif /* FREEZE_AGGR_OSF_2 */
#endif /* FREEZE_AGGR_OSF_1 */
#endif /* AFS_OSF_ENV */
	      }
	      /*
	       * If dP->sleep not zero, then the aggregate has already been
	       * frozen and we need not do any OSF1 specific things
	       */
	      /*
	       * We need to do the following as a different count may have
	       * been specified.
	       */
	      dP->sleep = tsleep;
	      osi_Wakeup(&dP->sleep);
	  }
	  break;
	case EFS_DEBUG_ASYNC_UNFREEZE:
	  if (dP->sleep != 0) {
	      dP->sleep = 0;

#ifdef AFS_OSF_ENV
#ifdef FREEZE_AGGR_OSF_1
	      /*
	       * Reallow recycling of episode vnodes now that the aggregate
	       * has been unfrozen.
	       */
	      efs_unfreeze(dP);
#else  /* FREEZE_AGGR_OSF_1 */
#ifdef FREEZE_AGGR_OSF_2
	      frozenAggrCount--;
#endif /* FREEZE_AGGR_OSF_2 */
#endif  /* FREEZE_AGGR_OSF_1 */
#endif /* AFS_OSF_ENV */
	      osi_Wakeup(&dP->sleep);
	  }
	  /*
	   * else, the aggregate has already been unfrozen and we need
	   * not do anything
	   */
	  break;
	case EFS_DEBUG_ASYNC_QUERY:
	  code = osi_copyout_r((char *)&dP->sleep, parm2p, sizeof (dP->sleep));
	  break;
	case EFS_DEBUG_ASYNC_TEST:
	  code = osi_copyout_r((char *)&dP->blkSize, parm2p, sizeof (dP->blkSize));
	  break;

	  /* The following case is for simulating small memory configurations
	   * on machines with more memory.  It is rather arbitrarily placed in
	   * this module.  It is unrelated to the FREEZE code. */

	case EFS_DEBUG_ASYNC_PINCORE:
	  while (pinnedCore.length > parm1) {
	      /* Need to free some chunks */
	      struct core *chunk;
	      chunk = (struct core *)FIFO_LookAtOldest(&pinnedCore,
			offsetof(struct core, thread));
	      FIFO_REMOVE(&pinnedCore, &chunk->thread);
#if defined(AFS_AIX_ENV) && defined(_KERNEL)
	      unpin (chunk, sizeof(struct core));
#endif /* AFS_AIX_ENV && _KERNEL */
	      osi_Free_r (chunk, sizeof(struct core));
	  }
	  while (pinnedCore.length < parm1) {
	      /* Need to allocate some more chunks */
	      struct core *chunk;
	      chunk = osi_Alloc_r (sizeof(struct core));
#if defined(AFS_AIX_ENV) && defined(_KERNEL)
	      pin (chunk, sizeof(struct core));
#endif /* AFS_AIX_ENV && _KERNEL */
	      FIFO_ADD(&pinnedCore, &chunk->thread);
	  }
	  break;

	default:
	  code = EAGAIN; /* XXX "No more processes" ?!? */
	  break;
      }
      return code;
}
#endif /* AFSL_DEBUG_LEVEL */
