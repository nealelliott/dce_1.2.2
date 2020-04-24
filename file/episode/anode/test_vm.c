/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* test_vm.c -- contains procedures to implement the VM scaffolding for
 *     test_anode. */

/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <com_err.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/critical.h>
#include <dcedfs/osi_buf.h>

#include <string.h>			/* memset */

#include <anode.h>
#include <volume.h>
#include <aggregate.h>

#include <test_vm.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/test_vm.c,v 4.40 1996/03/04 19:01:55 ota Exp $")

int useVM = 0;

struct vm_mem { /* header for "VM" memory blocks */
    struct vm_mem *next;
    u_long base;
    u_long len;
};

struct vm_file {
    struct vm_file *next;		/* hash chain */
    epi_anode_t file;
    afs_hyper_t length;			/* intendLength */
    int nbufs;				/* number of bufs in VM for file */
    struct buf *bufs;			/* single chained with b_back */
    struct vm_mem *mem;			/* unordered list of mem structs */
};

/* vm_paghHT -- organizes (struct buf)s representing each page of VM.  They are
 *     hashed by b_vp (really the anode handle) and blkno.  The b_forw ptr is
 *     used for the hash chain.  Each struct buf does not have a separate
 *     reference counted ptr to the anode handle.  Instead the anode handle in
 *     the struct vm_file must be held open while there are any bufs.
 */
static struct dfsh_hashTable vm_pageHT;

static struct dfsh_hashTable vm_fileHT;
static struct lock_data vm_lock;	/* protects vm system data bases */
static u_long vm_extentSize = 32*1024;
static struct buf *vm_freeList;

/* Also references via b_Bufsize macro defined in osi_buf.h. */
/* SHARED */
u_long vm_pageSize = 2048;

static osi_dlock_t io_lock;	/* for iodone/iowait */

#define VM_PAGEHASH(vp, blkno) (((long)(vp) | 1) * (blkno))
static u_long vm_PageHash(struct buf *bp)
{
    return VM_PAGEHASH(bp->b_vp, bp->b_blkno);
}

#define VM_PAGEMATCH(bp, file, blkno) \
    (((opaque)(bp)->b_vp == (opaque)(file)) && ((bp)->b_blkno == (blkno)))

static u_long vm_FileHash(struct vm_file *file)
{
    u_long hash;
    hash = (long)file->file;
    return hash;
}

/* SHARED */
void vm_Init(void)
{
    long code;

    vm_pageSize = 2048;
    lock_Init (&vm_lock);
    lock_Init (&io_lock);
    code = dfsh_HashInit (&vm_pageHT, vm_PageHash,
			  offsetof (struct buf, b_forw));
    MBZ (code);
    code = dfsh_HashInit (&vm_fileHT, vm_FileHash,
			  offsetof (struct vm_file, next));
    MBZ (code);
}

/* SHARED */
long vm_SetSizes(
  char *whereami,
  u_long blockSize,			/* advisory block size */
  u_long pageSize,
  u_long extentSize)
{
    long code;

    if (blockSize && !pageSize && (blockSize < vm_pageSize))
	/* just changing the block size, so reset page size if necessary. */
	pageSize = blockSize;
    if (!pageSize) pageSize = vm_pageSize;
    if (!extentSize) extentSize = vm_extentSize;
    if (blockSize % pageSize != 0) {
	com_err (whereami, 0, "inconsistent page/block sizes (%u/%u)",
		 pageSize, blockSize);
	return -1;
    }
    if (extentSize % pageSize != 0) {
	com_err (whereami, 0, "inconsistent page/extent sizes (%u/%u)",
		 pageSize, extentSize);
	return -1;
    }
    if (pageSize != vm_pageSize) {
	code = vm_Write (VM_WRITE|VM_SYNC|VM_AWAIT|VM_INVALIDATE, 0);
	if (code) return code;
    }
    vm_pageSize = pageSize;
    vm_extentSize = extentSize;
    return 0;
}

/* FORWARD */ char *FtoS();

/* vm_IOWait -- returns when I/O has completed on the specified buffer.  It
 *     waits if necessary.
 *
 * CAUTIONS -- Error detection should be done by the caller. */

static void vm_IOWait(struct buf *bp)
{
    afsl_Trace (31, ("vm_IOWait: bp=%x, flags=%s", bp, FtoS(bp->b_flags)));
    while (!(bp->b_flags & B_DONE)) {
	lock_ObtainWrite (&io_lock);
	if (bp->b_flags & B_DONE) {
	    /* in case of race condition */
	    afsl_Trace (AFSL_TR_UNUSUAL,
			("vm_IOWait: race condition w/ bp=%x", bp));
	    lock_ReleaseWrite (&io_lock);
	    return;
	}
	b_Want(bp);
	afsl_Trace (AFSL_TR_SLEEP,
 ("VM sleeping for I/O complete on %x (for blkno %d of file %x, flags=%s)",
		     bp, bp->b_blkno, bp->b_vp, FtoS(bp->b_flags)));
	osi_SleepWI ((opaque)bp, &io_lock);
    }
}

/* Hold -- bumps the refcount on a anode handle. */

static epi_anode_t Hold(epi_anode_t file)
{
    long code;
    epi_anode_t h;
    epi_volume_t vol, newvol;
    epi_anode_t avl;

    vol = epiv_GetVolume (file);
    code = epiv_GetAggregate (vol, &avl);
    afsl_MBZ (code);
    code = epig_OpenVolume (avl, epia_GetVolIndex (file), &newvol);
    afsl_MBZ (code);
    afsl_Assert (newvol == vol);
    code = epiv_OpenAnode (vol, epiv_GetAnodeIndex(file), &h);
    afsl_MBZ (code);
    afsl_Assert (h == file);
    return h;
}

/* Rele -- drops the refcount on a anode handle. */

static void Rele(epi_anode_t file)
{
    long code;

    code = epiv_CloseAnode (file);
    afsl_MBZ (code);
    code = epig_CloseVolume (elbb_NullTranId, epiv_GetVolume (file));
    afsl_MBZ (code);
}

/* Buf flags used by test_vm.c. */

#ifndef B_DIRTY				/* page frame contains dirty data */
#define B_DIRTY		0x80000000
#endif

/* this is really defined in osi_buf.h */
#ifndef B_PFSTORE
#define B_PFSTORE	0x40000000
#endif

#ifndef B_INVAL				/* kill attempted on busy buf */
#ifdef B_STALE
#define B_INVAL B_STALE
#else
#define B_INVAL		0x20000000
#endif
#endif

#ifndef B_INFLIGHT			/* page frame being written */
#define B_INFLIGHT	0x10000000
#endif

char *FtoS(long flags)
{
    static char s[100];
    if (flags & B_READ) strcpy (s, "READ");
    else strcpy (s, "WRITE");
    if (flags & B_DONE) strcat (s, "|DONE");
    if (flags & B_ERROR) strcat (s, "|ERROR");
    if (flags & B_BUSY) strcat (s, "|BUSY");
#ifdef B_WANTED
    if (flags & B_WANTED)
#endif
	strcat (s, "|WANTED");
    if (flags & B_ASYNC) strcat (s, "|ASYNC");
    if (flags & B_DELWRI) strcat (s, "|DELWRI");
    if (flags & B_DIRTY) strcat (s, "|DIRTY");
    if (flags & B_INVAL) strcat (s, "|INVAL");
    if (flags & B_PFSTORE) strcat (s, "|PFSTORE");
    if (flags & B_INFLIGHT) strcat (s, "|INFLIGHT");
    return s;
}

static struct buf *Cluster(
  struct vm_file *f,
  struct buf *bp,
  int writeFlags)
{
    epi_anode_t file = f->file;
    daddr_t blkno, minBlkno, maxBlkno;
    struct buf *firstBp;
    long blkPerExtent = btodb(vm_extentSize);
    long blkPerPage = btodb(vm_pageSize);

    AssertWriteLocked (&vm_lock);

    blkno = bp->b_blkno;
    minBlkno = blkno & ~(blkPerExtent-1);
    maxBlkno = minBlkno + blkPerExtent;

    bp->b_bcount = vm_pageSize;
    bp->b_work = 0;

    /* First walk up as far as we can.  Increment the blkno a page at a time
     * and look up the corresponding buf.  If the buf exists and is dirty but
     * not busy then add it to out list. */
    firstBp = bp;
    while ((blkno += blkPerPage) < maxBlkno) {
	struct buf *b;
	DFSH_LOOKUP (struct buf *, &vm_pageHT, b,
		     VM_PAGEHASH(file, blkno), VM_PAGEMATCH(b, file, blkno));
	if (!b) break;
	if ((b->b_flags & B_BUSY) || !(b->b_flags & B_DIRTY)) break;
	B_Work(bp) = b;
	b->b_work = 0;
	firstBp->b_bcount += vm_pageSize;
	bp = b;
    }

    /* Now do the same thing but going down. */
    blkno = firstBp->b_blkno;
    while ((blkno -= blkPerPage) >= minBlkno) {
	struct buf *b;
	DFSH_LOOKUP (struct buf *, &vm_pageHT, b,
		     VM_PAGEHASH(file, blkno), VM_PAGEMATCH(b, file, blkno));
	if (!b) break;
	if ((b->b_flags & B_BUSY) || !(b->b_flags & B_DIRTY)) break;
	B_Work(b) = firstBp;
	b->b_bcount = firstBp->b_bcount + vm_pageSize;
	firstBp = b;
    }

    /* While we still hold the VM lock, mark all these buf for writing. */
    for (bp = firstBp; bp; bp = B_Work(bp)) {
	afsl_Trace (31, ("Cluster: firstBp=%x, bp=%x", firstBp, bp));
	bp->b_flags |= writeFlags;
	bp->b_flags &= ~(B_READ|B_ERROR|B_DONE);
	bp->b_error = 0;
    }

    return firstBp;
}

static void PutPage(struct vm_file *f, struct buf *bp)
{
    long code;
    int foundHole;			/* dummy */

    afsl_Trace (31, ("PutPage: bp=%x, flags=%s, vp=%x, blkno=%d, bcount=%d",
		     bp, FtoS(bp->b_flags),
		     bp->b_vp, bp->b_blkno, bp->b_bcount));
    code = epia_Strategy (bp, f->file);
    afsl_PAssert (code==0, ("PutPage: strategy failed with code=%d", code));
}

static void FreeFileIfPossible(struct vm_file *f)
{
    long code;
    struct vm_mem *m, *nextM;

    if (f->nbufs > 0) return;		/* still some bufs left, so quit now */

    for (m = f->mem; m; m = nextM) {
	nextM = m->next;
	osi_Free ((opaque)m, sizeof(*m) + m->len);
    }
    f->mem = 0;
    Rele (f->file);
    code = dfsh_HashOut_r (&vm_fileHT, f);
    afsl_MBZ (code);
    osi_Free ((opaque)f, sizeof(*f));
}

static void KillBuf(struct buf *bp)
{
    epi_anode_t file;
    struct vm_file *f;
    struct buf *pb;			/* previous bp */
    long code;

    afsl_Trace (31, ("KillBuf: bp=%x, flags=%s", bp, FtoS(bp->b_flags)));

    /* get vm_file structure for this anode */
    file = (epi_anode_t)bp->b_vp;
    DFSH_LOOKUP (struct vm_file *, &vm_fileHT, f,
		 (u_long)file, (f->file == file));
    afsl_Assert (f && f->bufs && f->nbufs);

    /* unthread from vm_file bufs list, and unhash buf ptr */
    if (f->bufs == bp) {
	/* we're at the head of the list */
	f->bufs = bp->b_back;
    } else {
	for (pb = f->bufs; pb->b_back != bp; pb = pb->b_back) {
	    afsl_Assert (pb->b_back);
	}
	pb->b_back = bp->b_back;
    }
    f->nbufs--;
    code = dfsh_HashOut_r (&vm_pageHT, bp);
    afsl_MBZ (code);

    /* If possible, free vm_file structure and associated memory */
    FreeFileIfPossible (f);
}

/* KillFile -- removes all the bufs for a file from the vm system.  BUSY bufs
 *     are marked as invalid.  The number of bufs that were removed is
 *     returned. */

static int KillFile(epi_anode_t file)
{
    struct buf *bp;
    struct vm_file *f;
    struct buf *pb;			/* previous bp */
    long code;
    int removed = 0;

    /* get vm_file structure for this anode, creating if necessary */
    DFSH_LOOKUP (struct vm_file *, &vm_fileHT, f,
		 (u_long)file, (f->file == file));
    if (!f) return 0;			/* no such file */

    /* walk down list of bufs an free each, if possible. */
    pb = 0;
    for (bp = f->bufs; bp; bp = bp->b_back) {
	afsl_Trace (31, ("KillFile: bp=%x, flags=%s vp=%x",
			 bp, FtoS(bp->b_flags), bp->b_vp));
	if (bp->b_flags & B_BUSY) {
	    /* invalidate on release */
	    bp->b_flags |= B_INVAL;
	    pb = bp;			/* advance pb */
	    continue;
	}

	/* unthread from vm_file bufs list, and unhash buf ptr */
	if (pb) pb->b_back = bp->b_back;
	else f->bufs = bp->b_back;	/* head of list */
	f->nbufs--;

	code = dfsh_HashOut_r (&vm_pageHT, bp);
	afsl_MBZ (code);
	removed++;
    }

    FreeFileIfPossible (f);
    return removed;
}

static void Release(struct buf *bp)
{
    AssertWriteLocked (&vm_lock);
    afsl_Assert (bp->b_flags & B_BUSY);
    afsl_Trace (31, ("Release: bp=%x, flags=%s", bp, FtoS(bp->b_flags)));

    if (b_Wanted(bp)) {
	osi_Wakeup ((opaque)bp);
    }
    if (bp->b_flags & B_INVAL) {
	KillBuf (bp);
	return;
    }
    if (bp->b_flags & (B_ASYNC|B_INFLIGHT)) {
	/* just finished I/O */
	if (bp->b_flags & B_ERROR) {
	    afsl_TraceEnabled (1, ((bp->b_error != ENOSPC) &&
				   (bp->b_error != EDQUOT)), AFSL_TR_ALWAYS,
			       ("vm_IOWait: I/O error=%d, file/blkno=%x/%d",
				bp->b_error, bp->b_vp, bp->b_blkno));
	} else {
	    /* don't turn off dirty if we got an error? */
	    bp->b_flags &= ~B_DIRTY;
	}
    }
    bp->b_flags &= ~(B_BUSY|B_ASYNC|B_INFLIGHT);
}

static osi_iodone_t vm_IODone (bp)
  IN struct buf *bp;
{
    afsl_Trace (31, ("vm_IODone: bp=%x, flags=%s", bp, FtoS(bp->b_flags)));
    lock_ObtainWrite (&io_lock);
    bp->b_flags |= B_DONE;
    if (b_Wanted(bp)) {
	osi_Wakeup ((opaque)bp);
    }
    lock_ReleaseWrite (&io_lock);
    if (bp->b_flags & B_ASYNC)
	BEGIN_LOCK_WRITE (&vm_lock) {
	    Release (bp);
	} END_LOCK;
}

/* AwaitNotBusy -- is a function that sleeps on a BUSY buffer.  If the buffer
 *     is not BUSY when it is called we do NOT sleep but the vm_lock is
 *     released anyway.
 *
 * LOCKS USED -- This fcn must be called with the vm_lock held.  It returns
 *     with the lock released. */

static void AwaitNotBusy(
  struct buf *bp,			/* supposedly busy buffer */
  char *who)				/* identity of caller */
{
    if (!(bp->b_flags & B_BUSY)) {
	afsl_Trace (AFSL_TR_UNUSUAL,
		    ("%s: race condition *not* BUSY buf %x (flags=%s)",
		     who, bp, FtoS(bp->b_flags)));
	lock_ReleaseWrite (&vm_lock);
	return;
    }
    b_Want (bp);
    afsl_Trace (AFSL_TR_SLEEP,
		("%s: sleeping on BUSY buf %x (vp/blkno %x/%d, flags=%s)",
		 who, bp, bp->b_vp, bp->b_blkno, FtoS(bp->b_flags)));
    osi_SleepWI ((opaque)bp, &vm_lock);
}

static void SaveError(
  epi_anode_t file,
  struct buf *bp,
  long *codeP)
{
    long code;
    afsl_Assert (bp->b_flags & B_ERROR);
    code = bp->b_error;
    bp->b_error = 0;
    bp->b_flags &= ~(B_ERROR|B_DIRTY);
    if (file) {
	if (*codeP && (*codeP != code)) {
	    afsl_TraceEnabled (1, 1, AFSL_TR_ALWAYS,
 ("vm_Write: buf %x overwriting prev I/O error=%d with %d, file/blkno=%x/%d",
  bp, *codeP, code, bp->b_vp, bp->b_blkno));
	}
	*codeP = code;
	return;
    }
    afsl_TraceEnabled (1, 1, AFSL_TR_ALWAYS,
 ("vm_Write: buf %x losing I/O error=%d, file/blkno=%x/%d",
  bp, code, bp->b_vp, bp->b_blkno));
    return;
}

static struct vm_file *GetFile(epi_anode_t file)
{
    struct vm_file *f;
    long code;

    /* get vm_file structure for this anode, creating if necessary */
    DFSH_LOOKUP (struct vm_file *, &vm_fileHT, f,
		 (u_long)file, (f->file == file));
    if (f) return f;

    f = (struct vm_file *) osi_Alloc (sizeof(*f));
    bzero ((char *)f, sizeof(*f));
    f->file = Hold(file);
    code = dfsh_HashIn_r (&vm_fileHT, f);
    afsl_MBZ(code);
    return f;
}

/* vm_Write -- traverses through the VM and examines all pages for the
 *     specified file or for all files if the file parameter is zero.  If flags
 *     has VM_WRITE set, modified pages will be written.  If flags has VM_AWAIT
 *     set, busy bufs will be awaited before attempting to write them.  If
 *     flags has VM_SYNC set, any writes will be awaited.  If flags has
 *     VM_INVALIDATE set, the pages will be removed from the VM system.
 *
 * LOCKS USED -- Most of the work here is protected by the vm_lock.  However,
 *     we may not hold the vm_lock while calling strategy (via PutPage) or
 *     waiting for I/O or waiting for busy buffers.  In all of these cases a
 *     call to vm_Release, either by another user or via iodone, would generate
 *     a deadlock because it grabs the vm_lock as well. */

/* SHARED */
long vm_Write(long flags, epi_anode_t file)
{
    long code;
    struct buf *bp;			/* for ALL_BUFS macro */
    int writeFlags = 0;
    int sleep;				/* wait for BUSY buf */
    long ioCode;			/* code from any bufs */

    /* no sync w/o write */
    afsl_Assert (!(flags & VM_SYNC) || (flags & VM_WRITE));

    if (flags & VM_WRITE) {
	writeFlags |= B_BUSY;
	if (flags & VM_INVALIDATE) writeFlags |= B_INVAL;
	if (flags & VM_SYNC)
	    /* we mark these somehow so we can find them again. */
	    writeFlags |= B_INFLIGHT;
	else {
	    /* if we're not going to be waiting for writes to complete
             * synchronously, then set B_ASYNC to release buf and, if
             * invalidating, do that then as well. */
	    writeFlags |= B_ASYNC;
	}
    }

    ioCode = 0;				/* collected errors from bufs */

#define ALL_BUFS(bp) \
    (bp = dfsh_HashNext(&vm_pageHT, NULL); \
     bp;					      \
     bp = dfsh_HashNext(&vm_pageHT, bp))

    /* do writes as necessary */
    if (flags & VM_WRITE) do {
	struct vm_file *f;
	sleep = 0;
	BEGIN_LOCK_WRITE (&vm_lock) {
restart_write:
	    for ALL_BUFS(bp) {
		if (!file || ((opaque)bp->b_vp == (opaque)file)) {
		    /* Return any error found.  But don't try to write it
                     * again, since otherwise we'll loop. */
		    if (bp->b_flags & B_ERROR) {
			SaveError (file, bp, &ioCode);
		    }
		    else if ((bp->b_flags & B_DIRTY) &&
			!(bp->b_flags & B_INFLIGHT)) {
			/* look for dirty ones, but don't stumble on our own
                         * bufs */
			if (bp->b_flags & B_BUSY) {
			    if (!(flags & VM_AWAIT)) /* skip these */
				continue;
			    sleep = 1;	/* sleep instead */
			    break;
			}
			else {
			    /* Got a live one.  Lookup the corresponding file
                             * and find all the other nearby bufs that are also
                             * dirty.  Cluster will set the write flags and
                             * turn off B_DONE in all the bufs it finds.  It
                             * returns the first in the series. */
			    f = GetFile ((epi_anode_t)bp->b_vp);
			    bp = Cluster (f, bp, writeFlags);
			    break;
			}
		    }
		    if (flags & VM_INVALIDATE) {
			/* we want to do any invalidation on the same pass so
                         * we get all the dirty buffers safely. */
			if (bp->b_flags & B_BUSY) bp->b_flags |= B_INVAL;
			else {
			    /* kill buffer, then restart iteration because
                             * KillBuf may have caused a rehash. */
			    KillBuf (bp);
			    goto restart_write;
			}
		    }
		}
	    }
	} END_LOCK;
	if (bp) {
	    if (sleep) {
		lock_ObtainWrite (&vm_lock);
		AwaitNotBusy (bp, "vm_Write (write)");
	    }
	    else PutPage (f, bp);
	}
    } while (bp);

    /* now wait for any scheduled writes */
    if (flags & VM_SYNC) do {
	sleep = 0;
	BEGIN_LOCK_WRITE (&vm_lock) {
restart_sync:
	    for ALL_BUFS(bp) {
		if (!file || ((opaque)bp->b_vp == (opaque)file)) {
		    if (bp->b_flags & B_ERROR)
			SaveError(file, bp, &ioCode);
		    if (bp->b_flags & B_INFLIGHT) {
			if (bp->b_flags & B_DONE) {
			    /* Don't touch bp after this returns.  Also restart
                             * search because if B_INVAL this will result in a
                             * call to KillBuf which may trigger rehash. */
			    Release (bp);
			    goto restart_sync;
			} else {
			    break;	/* wait for it w/o lock */
			}
		    }
		    else if (bp->b_flags & B_ASYNC) {
			/* someone else started this I/O wait for it too */
			sleep = 1;
			break;
		    }
		}
	    }
	} END_LOCK;
	if (bp) {
	    if (sleep) {
		lock_ObtainWrite (&vm_lock);
		AwaitNotBusy (bp, "vm_Write (sync)");
	    }
	    else vm_IOWait (bp);
	}
    } while (bp);

    /* Now invalidate matching pages, unless we already got them all during
     * write phase.  We await busy buffers to make sure everything is really
     * gone when we return. */
    if ((flags & VM_INVALIDATE) && !(flags & VM_WRITE)) do {
	sleep = 0;
	BEGIN_LOCK_WRITE (&vm_lock) {
restart_invalidate:
	    for ALL_BUFS(bp) {
		if (!file || ((opaque)bp->b_vp == (opaque)file)) {
		    int restart;	/* count of bufs removed. */
		    if (bp->b_flags & B_BUSY) {
			sleep = 1;
			break;
		    }
		    /* KillFile will damage hash threads, so zero bp to restart
                     * iteration. */
		    restart = KillFile ((epi_anode_t)bp->b_vp);
		    if (restart) goto restart_invalidate;
		}
	    }
	} END_LOCK;
	if (bp) {
	    afsl_Assert (sleep);
	    lock_ObtainWrite (&vm_lock);
	    AwaitNotBusy (bp, "vm_Write (invalidate)");
	}
    } while (bp);

    return ioCode;
}

/* SETLENGTH -- sets the vm_file's length to the MAX of the current and
 *     specified length. */

#define SETLENGTH(f, intendLength) \
    if (AFS_hcmp ((intendLength), (f)->length) > 0) \
	(f)->length = (intendLength)

/* vm_GetBuf -- returns a buf containing the requested file contents.  The
 *     flags specify if the page will be dirtied (VM_FORWRITE) or not
 *     (VM_FORREAD). */

/* SHARED */
long vm_GetBuf(
  epi_anode_t file,
  int flags,
  u_long offset,
  u_long length,
  struct vm_buf *bP)
{
    long code;
    struct buf *bp;
    struct vm_file *f;
    struct vm_mem *m;
    u_long base;
    daddr_t blkno;
    afs_hyper_t intendLength;			/* for strategy call */
    int foundHole;			/* dummy for strategy call */

    base = offset & ~(vm_pageSize-1);
    blkno = btodb(base);
    AFS_hset64 (intendLength, 0, offset);
    AFS_hadd32 (intendLength, length);

  loop:
    lock_ObtainWrite (&vm_lock);

    /* Get or create a vm_file for this anode */
    f = GetFile (file);

    /* look for the buf if there is one */
    DFSH_LOOKUP (struct buf *, &vm_pageHT, bp,
		 VM_PAGEHASH(file, blkno), VM_PAGEMATCH(bp, file, blkno));
    if (bp) {
	if (bp->b_flags & B_BUSY) {
	    AwaitNotBusy (bp, "vm_GetBuf");
	    goto loop;
	}

	if (flags & VM_FORWRITE) SETLENGTH (f, intendLength);

	bp->b_flags |= B_BUSY;
	lock_ReleaseWrite (&vm_lock);
	goto done;
    }

    /* not in hash table, we'll have to add it. */
    if (!vm_freeList)
	bp = (struct buf *) osi_Alloc (sizeof(*bp));
    else {
	bp = vm_freeList;
	vm_freeList = bp->b_forw;
    }
    bzero ((char *)bp, sizeof (*bp));

    /* get or create a vm_file for this anode */

    /* get VM address space for this offset, allocating if necessary */
    for (m = f->mem; m; m = m->next) {
	if ((m->base <= offset) && (m->base+m->len > offset))
	    break;
    }
    if (!m) {
	u_long membase = offset & ~(vm_extentSize-1);
	struct timeval tv;
	u_char value; /* garbage for initialization */

	afsl_Assert (vm_extentSize % vm_pageSize == 0);
	m = (struct vm_mem *)
	    osi_Alloc (sizeof (struct vm_mem) + vm_extentSize);
	osi_GetTime (&tv);
	value = (tv.tv_usec % 254) + 1;
	memset ((char *)m+sizeof(*m), value, vm_extentSize);
	m->next = f->mem;
	f->mem = m;
	m->base = membase;
	m->len = vm_extentSize;
    }

    afsl_Assert ((base >= m->base) &&
		 ((base-m->base) <= vm_extentSize-vm_pageSize));
    bp->b_un.b_addr = ((char *)m) + sizeof(*m) + base-m->base;
    bp->b_blkno = blkno;

    bp->b_vp = (struct vnode *)file;	/* struct vm_file holds refcount */

    code = dfsh_HashIn_r (&vm_pageHT, bp);
    afsl_MBZ(code);

    /* add to vm_file list */
    bp->b_back = f->bufs;
    f->bufs = bp;
    f->nbufs++;

    if (flags & VM_FORWRITE) {
	SETLENGTH (f, intendLength);
	bp->b_flags |= B_PFSTORE;
    }
    bp->b_flags |= B_BUSY;
    lock_ReleaseWrite (&vm_lock);

    bp->b_flags |= B_READ;
    osi_set_iodone(bp, vm_IODone);
    bp->b_bcount = vm_pageSize;

    code = epia_Strategy (bp, file);
    if (code) {
	com_err ("test_vm", code, "epia_Strategy failed");
	return 0;
    }

    vm_IOWait (bp);
    if (bp->b_flags & B_ERROR) {
	com_err ("test_vm", bp->b_error,
 "vm_GetBuf: vm_IOWait returned error; file/offset/length %x/%d/%d",
		 file, offset, length);
	return bp->b_error;
    }

done:
    if (flags & VM_FORWRITE) bp->b_flags |= B_DIRTY;
    bP->base = base;
    bP->length = vm_pageSize;
    bP->addr = bp->b_un.b_addr;
    bP->data = (opaque)bp;
    return 0;
}

/* SHARED */
void vm_Release(struct vm_buf *bP)
{
    BEGIN_LOCK_WRITE (&vm_lock) {
	Release ((struct buf *)(bP->data));
    } END_LOCK;
}

/* SHARED */
void vm_SetLength(epi_anode_t file, afs_hyper_t length)
{
    BEGIN_LOCK_WRITE (&vm_lock) {
	struct vm_file *f;
	f = GetFile (file);
	SETLENGTH (f, length);
    } END_LOCK;
}
