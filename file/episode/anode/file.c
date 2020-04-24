/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

/* file.c -- contains procedures to needed by the vnode layer to implement Unix
 *     files.  Basic read and write operations are handled directly by lower
 *     level modules.  This module is responsible for creation and deletion as
 *     well as maintenance of file attributes.
 *
 *     It manages file link counts and the related list of files with zero link
 *     count.
 *
 *     Auxiliary containers, used to store access control lists (ACLs) and
 *     property lists, are also managed by functions in this module. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/episode/logbuf.h>
#include "anode.h"
#include "fixed_anode.h"
#include "fid.h"
#include "volume.h"
#include "file.h"

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/file.c,v 4.135 1996/04/16 23:34:38 ota Exp $")

#define DEBUG_THIS_FILE EPIA_DEBUG_FILE

/*
 * hook for implementing monitoring and tuning specific to this submodule.
 */
/* EXPORT */
int ins_anode_file(
  IN struct ins_opDesc *opcode,
  IN struct ins_varDesc *vars,
  IN struct ins_basicObjectId *argObjIds,
  OUT struct ins_basicObjectId *resultObjIds,
  OUT struct ins_basicObject *resultValues)
{
    /* not yet imlemented */
    return 0;
}

#ifndef KERNEL


/* Reschedule -- tries to give up the processor to another runnable thread.
 *     This is used to manually introduce preemption in the code at points
 *     where it would cause the most trouble.  It is needed because the LWP
 *     thread package is non-preemtable.  It should be implemented at a lower
 *     level so that these calls can be inserted in more places.  Currently
 *     usage is spotty.  Its operation is conditional on an afsdb bit. */

static void Reschedule(void)
{
    if (afsdb_episode_anode & (1<<(EPIA_DEBUG_ADDPREEMPTS-1))) {
	osi_Reschedule ();
    }
}
#else
#define Reschedule()
#endif

#define TIME_LEQ(t1, t2) \
    (((t1)->tv_sec < (t2)->tv_sec) || \
     (((t1)->tv_sec == (t2)->tv_sec) && ((t1)->tv_usec <= (t2)->tv_usec)))

/* epif_UniqueTime -- returns an ever increasing time value on every
 *     invocation, even if successive calls to osi_GetTime return the same
 *     value.  It is used to generate time stamps for mtime that NFS could use
 *     for cache consistency version numbers.
 *
 * LOCKS USED -- For MP operation this will need a lock. */

void epif_UniqueTime(OUT struct timeval *t)
{
    static struct timeval lastT = {0, 0};
    epix_EnterCodeBlock(45);
    osi_GetTime (t);
    if (TIME_LEQ (t, &lastT)) {
	epix_EnterCodeBlock(1);
	lastT.tv_usec++;
	if (lastT.tv_usec >= 1000000) {
	    lastT.tv_sec++;
	    lastT.tv_usec = 0;
	}
	*t = lastT;
    } else lastT = *t;
}

#if defined(AFS_DEBUG) && !defined(KERNEL)

/* TestUniqueTime -- verifies that UniqueTime really returns a different,
 *     monotonically increasing value on each call. */

static long TestUniqueTime(void)
{
    long code = 0;
    struct timeval t1, ts[1000];
    int i, j, k;

    epif_UniqueTime (&t1);
    t1.tv_usec += 25000;		/* 1/40th of a second */
    if (t1.tv_usec > 1000000) {
	t1.tv_sec++;
	t1.tv_usec -= 1000000;
    }
    for (i=0; i<1000;) {
	struct timeval *t = &ts[i++];
	epif_UniqueTime (t);
	if (TIME_LEQ(&t1, t)) break;
    }
    printf ("Checked %d times...", i);
    fflush (stdout);
    for (j=0; j<i-1; j++) for (k=j+1; k<i; k++)
	if ((ts[j].tv_sec == ts[k].tv_sec) &&
	    (ts[j].tv_usec == ts[k].tv_usec)) {
	    printf ("\nTwo times are the same #%d and #%d", j, k);
	    code = -1;
	}
    printf ("\n");
    return code;
}
#else
#define TestUniqueTime() 0
#endif

#define ON_ERROR(stmt) \
    MACRO_BEGIN \
        long MACRO_code = code; \
	if (code) { \
	    code = 0; \
	    stmt; \
	    afsl_MBZ (code); \
	} \
	code = MACRO_code; \
    MACRO_END

/* L O C K I N G   H I E R A R C H Y
 *   handleLock -> files' lock -+-> filesetAuxTLock 
 *				+-> zeroLock
 *
 * L O C K I N G   N O T E S
 *
 * The handleLock protects the association of file handles and anodes including
 * the allocation of new file handles.  It also protects modifications to the
 * refCount (and the immutable magic) field of each handle.  No lock is
 * obtained to examine the refCount of a handle.  This lock must be obtained
 * BEFORE locking any individual file's lock.
 *
 * The filesetAuxTLock is a per fileset lock that protects the status structure
 * of all acl and plist containers in that fileset.  At the moment this is only
 * the linkCount.  It must be locked AFTER any individual file's locks are
 * obtained.
 *
 * Since this aux lock is the only one that unambiguously protects
 * modifications to aux containers, it also must be a tlock.  The granularity
 * of the aux tlock (on a per fileset basis) may still be too large.
 *
 * As an optimization this lock does not need to be held when modifying aux
 * containers that can not be referenced by any other process.  This includes
 * newly created acl and plist containers, as well as aux's whose linkCount has
 * been determined to be one (under a least a read lock on the auxLock) and the
 * file is write locked or zero.  The introduction of tlocks, makes this
 * optimization incorrect, even uncontested access to disk data must be
 * protected by a tlock.
 *
 * TODO -- remove this optimization, perhaps in conjunction with the above.
 *
 * We used to have a lock "notifyLock" to allow DeleteAnode 
 * to wait and retry the delete if anyone else had the anode open.
 * This lock notifyLock was the bottom of the locking hierarchy i.e.
 * below filesetAuxTLock. This lock is no longer needed because:
 * - For auxiliary containers, this is no longer an issue since a
 *   fid can not contain an index which would be confused with an
 *   auxiliary container's index.
 * - The vnode layer guarantees that simultaneous epif_Opens on an
 *   index will not happen while the file is being deleted
 *   (through the usage of the per vnode idLock and the vnode hash
 *   table).
 *
 * The zeroLock is a global lock that provides synchronization for all changes
 * to the zero link count lists for all volumes.
 *
 * TODO -- This could be a per volume lock if we maintained such a data base.
 *     At least we could hash on the volume id and manage several locks instead
 *     of only one.
 *
 * The zeroLock is a per-volume lock that provides synchronization for all
 * changes to the volume's zero link count list.
 *
 * A S S U M P T I O N S  and  I N V A R I A N T S
 *
 * Assumptions required of the vnode layer:
 *     1 -- The vnode layer only uses anodes for file identifiers which have
 *	 been opened by a call to \fcn{epif_Open}.
 *     2 -- Calls to (\fcn{epif_ChangeLink}) and (\fcn{epif_Delete})
 *       are single threaded with respect to all other file module
 *       operations on the same file.
 *     3 -- Attempts to open a fid can occur at any time.  However, this is
 *	 subject to the constraints of the previous assumption.
 *     4 -- A uniquifier cannot have the value zero.
 *     5 -- All the auxiliary containers of a file taken together are small
 *	 enough to delete in a single transaction.
 *
 * Invariants:
 *     A -- An anode with a file handle has a (reference count $>$ 0) or is
 *	 closed or deleted, in which case the anode's (reference count == 0).
 *	 As a corollary we can check the reference count of an open file
 *	 without holding any locks.  This however may not be true for some MP
 *	 architectures.
 *     B -- An attempt to open a fid corresponding to a partially created or
 *	 deleted file is rejected with an \cst{EPI_E_NOENT} error.
 *     C -- Bogus fids are rejected by \fcn{epif_Open} without interfering
 *	 with deletion of either files or auxiliary containers using the same
 *	 index.
 *
 * CAUTIONS -- See cautions at epif_Open regarding use of the don't care value
 *     for unique. */

static epi_mutex_t handleLock;
static epi_mutex_t notifyLock;
static epi_cond_t  notifyCond;

/* struct diskStatus -- is the disk image stored in anode status area.  It is
 *     similar to struct epif_status in file.h. */

struct diskStatus { /* 25 words */
    u_int16 flags;			/* misc file module bits */
    int16 spares;			/* short spare */
    u_int16 linkCount;			/* directory names pointing here */
    u_int16 mode;			/* file type & access bits */
    u_int32 unique;			/* anode index uniquifier */
    dfsh_diskHyper_t volumeVersionNumber; /* VV of last update to this file */
    u_int32 dataVersionNumber;		/* file contents uniquifier */
    struct timeval /* ack, double words! */
	mTime,				/*  time of last mod. of contents */
	cTime,				/*  ... modification of status */
	aTime;				/*  ... access to contents (approx) */
    u_int32 oid;			/* owner user id */
    u_int32 gid;			/* owning group id */
    u_int32 acl;			/* container index of acl */
    u_int32 plist;			/* ... property list */
    u_int32 reservedForUserPlist;	/* ... user plist (not meta-data) */
    epi_uuid_t uuid;			/* file's object uuid */
    int32 res_timeUncertainty;		/* from DTS, encoded in 8 bits each */
    int32 spare[2];
    u_int32 clientBits;			/* for client-only attributes (hidden, ...) */
    /* valid only for directories. */
    u_int32 initialDirAcl;		/* ... initial acl for new sub-dirs */
    u_int32 initialFileAcl;		/* ... initial acl for new files */
};

/* Notes on changes:
 *
 * 1. There didn't seem to be any uses of flags.  Also I mistakenly thought
 * that mode and type were separate fields.  It turns out that all that
 * information is squeezed into 16 miserable bits!  So I flushed both the type
 * and flags fields.
 *
 * 2'. We should not remove the dataVersionNumber since it does serve a useful
 * purpose.  Mtime is not as good since it can be set by users (utimes(2)).
 * The volume version number changes more often than the file contents so, at
 * least potentially, this may change much too often.  If we need to we can
 * probably accept one or more of these shortcomings if necessary to avoid
 * updating yet another field everytime the file changes.
 *
 * (2. I have removed dataVersionNumber.  I don't think this is used anymore.
 *  Instead we should really use the volume version number.  Currently this is
 *  stored somewhere else in the anode.  I plan to move it here, but I am not
 *  yet ready to do that level of reorganization.  Should I leave the space
 *  here now?)
 *
 * 3. I renamed time stamps to more traditional names.
 *
 * 4. I flushed the cid, a nice idea, but who can afford the space?  Not POSIX
 * apparently.
 *
 * 5. The oid and gid fields have been changed to the epi_principal_id_t type,
 * so that they can be integrated with the ACL package easily.  These can be
 * converted between 128 and 32 bits via a compile-time switch.
 *
 * 6'. The realm field is NOT being added.  We will qualify the oid and godi
 * with the default realm of the acl.  It (still) isn't legal for the oid and
 * gid to be from different realms.
 *
 * (6. The new realm field qualifies both the oid and gid as to their
 *  authentication domain.)
 *
 * 7. Added both initial directory and file acls.  These are only needed for
 * directories.  Since we may consider not allocating space for them in
 * directories I put them at the end of the structure.  An alternative
 * suggestion is to decide that both types of plists are inappropriate for
 * directories so we can overlay the initial acls with the plists.  This seems
 * quite appealing; what do people think about punting directory plists?
 *
 * 8. I change the type of uuid from fake_uuid_t to epi_uuid_t which has no
 * effect on the size.  The reason this is not an epi_principal_id_t is that
 * the object uuid isn't part of the security system and it makes no sense to
 * have an option to shrink it to 32 bits.
 *
 * 9. I've moved the volume version number here from the fixed part of the
 * anode because it isn't used for non-file types anyway.  Even acl/plist
 * containers don't have a separate VV.  Moving it here will allow us to reduce
 * the number of separate meta-data updated needed to keep this up-to-date.
 * This will not be implemented until the dump design is clarified.
 *
 * 10. I flushed the parent field since I do not think it is used.
 *
 * 11. Do we need more spares?
 */

/* Various constants for auxiliary containers */

/* Convert aux type to compact index space */
static int auxIndex[EPIF_AUX_NTYPES] = {
    -1,					/* reserved1 */
    0,					/* acl */
    1,					/* initial dir acl */
    2,					/* initial file acl */
    -1,					/* reserved2 */
    3,					/* plist */
    4,					/* user plist */
};
#define AUX_NINDEXES 5

/* Convert aux type to diskStatus offset */
static int auxOffset[EPIF_AUX_NTYPES] = {
    -1,					/* reserved1 */
    offsetof(struct diskStatus, acl),
    offsetof(struct diskStatus, initialDirAcl),
    offsetof(struct diskStatus, initialFileAcl),
    -1,					/* reserved2 */
    offsetof(struct diskStatus, plist),
    offsetof(struct diskStatus, reservedForUserPlist),
};
#define AUX_FROM_STATUS(s, t) (*(u_long *)((char *)(s) + auxOffset[t]))

/* #define FILE_MAGIC 0x580b7013 -- For file status format before 910601 */
#define FILE_MAGIC 0x580b7014

#define FILE_BEING_DELETED 0x1		/* Handle flags */
#define FILE_JUST_CREATED  0x2		/* Creator has exclusive access */

/* private file module data associated with every open file's anode handle */

struct fileHandle {
    /* first two fields protected by handleLock */
    long magic;
    int refCount;			/* number of file module opens */

    long flags;
    struct tlock_data lock;
    epi_anode_t aux[AUX_NINDEXES];	/* open handle for aux containers */
    struct diskStatus status;		/* cache disk info */
    struct fileHandle *next;
};

typedef struct fileHandle fileHandle_t;

/* GetFairFile -- sets h to good looking file handle from fileC or zero.  This
 *     predicate must be true for all file anodes which have handles.
 *
 * CAUTIONS -- the first parameter is must be a variable reference; it is
 *     assigned into.
 *
 * RETURNS -- the file handle.
 */

#define GetFairFile(h, fileC) \
    (((fileC) && ((h) = (struct fileHandle *)epia_GetFileRock(fileC)) && \
      ((h)->magic == FILE_MAGIC)) \
     ? (h) : ((h) = 0))

/* GetOKfile -- same as GetFairFile but also requires positive refCount.
 *
 * RETURNS -- a boolean: TRUE if file handle is okay. */

#define GetOKfile(h, fileC) (GetFairFile (h, fileC) && (h)->refCount)

#define MAX_FREE_FHANDLE 40
static u_int maxFreeFHandle = MAX_FREE_FHANDLE;
static u_int nFreeFHandles = 0;
static struct fileHandle *fHandleFreeList = NULL;
osi_dlock_t fHandleMutex;

static void
DiscardFileHandle(struct fileHandle *h)
{
    lock_ObtainWrite_r(&fHandleMutex);
    if (nFreeFHandles >= maxFreeFHandle) {
	lock_ReleaseWrite_r(&fHandleMutex);
	osi_Free_r(h, sizeof (*h));
    } else {
	h->next = fHandleFreeList;
	fHandleFreeList = h;
	++nFreeFHandles;
	lock_ReleaseWrite_r(&fHandleMutex);
    }
}

static struct fileHandle *
GetFileHandle(void)
{
    struct fileHandle *h;
    lock_ObtainWrite_r(&fHandleMutex);
    if (nFreeFHandles == 0) {
	lock_ReleaseWrite_r(&fHandleMutex);
	h = osi_Alloc_r(sizeof (*h));
    } else {
	h = fHandleFreeList;
	fHandleFreeList = h->next;
	--nFreeFHandles;
	lock_ReleaseWrite_r(&fHandleMutex);
    }
    bzero((caddr_t) h, sizeof (*h));
    return (h);
}


/* Cleanup -- is called whenever an anode handle is being GCed by fixed_anode
 *     module.  See fixed_anode.c: CleanupHandle for details.
 *
 * LOCKS USED -- Since this is really part of the fixed_anode module it cannot
 *     wait for any file module locks. */

static void Cleanup(epi_anode_t fileC)
{
    struct fileHandle *h;
    epix_EnterCodeBlock(3);
    GetFairFile (h, fileC);

    /* FairFile => good magic, also better not be open or have any lockers. */
    afsl_Assert (h && (h->refCount == 0) && (tlock_Check (&h->lock) == 0));

    DiscardFileHandle(h);

    /* clear this field after the assert to aid debugging - wam 19dec91 */
    epia_SetFileRock (fileC, 0, 0);
}

/* epif_Init -- Initializes the file module.  It is called only once during
 *     system initialization by \fcn{epig_Init}, see which. */

/* EXPORT */
long epif_Init(void)
{
    static inited = 0;
    long code;

    if (afsdb_episode_anode & (1<<(EPIA_DEBUG_UNIQUETIME-1))) {
	code = TestUniqueTime();
	if (code) return code;
    }
    if (inited) return 0;
    inited++;

    /* The following asserts verify the correctness of offsets and sizes used
     * for address arithmetic. */

    afsl_Assert((sizeof (epi_uuid_t) == 16) &&
	    /* we used this in calculating offset in epif_SetStatus */
	    (sizeof (struct timeval) == sizeof (dfsh_diskHyper_t)));
#define STATUS_OFFSET_EQUAL(field, offset) \
    afsl_Assert(offsetof(struct diskStatus, field) == offset);

    /* Make sure the offsets in file.h match those of the diskStatus structure.
     * The constants are used for fast accessing macros in file.h */

    afsl_Assert(offsetof(struct fileHandle, status) ==
	    EPIF_FILE_HANDLE_STATUS_OFFSET);
    STATUS_OFFSET_EQUAL(linkCount, EPIF_DISKSTATUS_LINKCOUNT);
    STATUS_OFFSET_EQUAL(mode, EPIF_DISKSTATUS_MODE);
    STATUS_OFFSET_EQUAL(unique, EPIF_DISKSTATUS_UNIQUE)
    STATUS_OFFSET_EQUAL(dataVersionNumber, EPIF_DISKSTATUS_DV)
    STATUS_OFFSET_EQUAL(mTime, EPIF_DISKSTATUS_MTIME);
    STATUS_OFFSET_EQUAL(cTime, EPIF_DISKSTATUS_CTIME);
    STATUS_OFFSET_EQUAL(aTime, EPIF_DISKSTATUS_ATIME);
    STATUS_OFFSET_EQUAL(oid, EPIF_DISKSTATUS_OID);
    STATUS_OFFSET_EQUAL(gid, EPIF_DISKSTATUS_GID);

    epi_mutex_init (&handleLock, "file handle");
    epi_mutex_init (&notifyLock, "file notify");
    epi_cond_init (&notifyCond, "file notify");

#ifdef AFS_DEBUG
    /* CodeBlock: epia */
    epia_codeBlock[DEBUG_THIS_FILE][0] = 83;
#endif

    return 0;
}

/* if set indicates a failed delete is waiting for an opportunity to retry */
static int notifyRequested = 0;

/* MaybeRequestNotify -- is called to unlock the notifyLock which is held
 *     accross calls to epiv_DeleteAnode.  The notify paramter is true if the
 *     delete failed because some other thread had the anode open.
 *
 * PARAMETERS -- The index parameter is used only for diagnostic purposes.
 *
 * LOCKS USED -- The caller obtains notifyLock and we release it before return.
 *     The wakeup and requests for wakeup are synchronized using the
 *     notifyLock.  */

static void MaybeRequestNotify(
  int notify,			/* TRUE => request notification */
  u_long index)
{
    if (notify) {
	notifyRequested++;
	SleepPrint (("DeleteAnode: sleeping for open anode (idx %d) to close",
		     index));
	epi_cond_wait (&notifyCond, &notifyLock);
    }
}

/* MaybeNotify -- is called after a call to GetHandle fails.  It notifies a
 *     thread trying to complete an anode deletion, if one has requested a
 *     wakeup.
 *
 * PARAMETERS -- The index parameter is used only for diagnostic purposes.
 *
 * LOCKS USED -- The wakeup and request for wakeup are synchronized using the
 *     notifyLock. */

static void MaybeNotify(u_long index)
{
    BEGIN_MUTEX (&notifyLock) {
	if (notifyRequested) {
	    DBprint (("MaybeNotify: notifying on index %d", index));
	    epi_cond_broadcast (&notifyCond);
	    notifyRequested = 0;
	}
    } END_LOCK;
}

/* DeleteAnode -- is called to delete any anode used by the file module.  This
 *     includes both files and auxiliary containers.  Its purpose is to
 *     synchronize such deletes with calls to epif_Open in such a way that no
 *     lock has to be held during the call to epia_Open (which may require
 *     I/O), and still handle interference between open and delete. */

static long DeleteAnode(
  IN elbb_tranRec_t trans,
  IN epi_anode_t anode)
{
    long code;
    int retry;
    do {
	u_long index = epiv_GetAnodeIndex(anode);
	Reschedule();
 	BEGIN_MUTEX (&notifyLock) {
	    code = epiv_DeleteAnode (trans, anode);
	    retry = (code == EPI_E_OPEN);
	    if (retry)
		code = 0;
	    MaybeRequestNotify (retry, index);
	} END_LOCK;
	if (code) return code;
    } while (retry);
    return 0;
}

/* GetHandle -- obtains a file handle for the specified anode handle.  If
 *     "create" is true then a fresh handle is returned write-locked.
 *     Otherwise the handle is initialized from the anode status and the unique
 *     is checked.  A mismatch is returned as an error.
 *
 * LOCKS USED -- New handle creation is covered by the handleLock.  One static
 *     variable which caches a single free handle is also protected by this
 *     lock.  Newly created handles are write locked, existing handles are
 *     temporarily read locked.  Although the file lock is a tlock, no
 *     transaction is needed since no disk modifications are performed. */

static GetHandle(
  IN epi_anode_t file,
  IN int create,
  IN u_long unique,
  OUT struct fileHandle **hP)
{
    long code = 0;

    *hP = 0;
    Reschedule();
    afsl_Assert(unique != 0);
    if (!file) return EPI_E_BADFILE;
    if (epia_GetStatusSize(file) != sizeof(struct diskStatus))
	return EPI_E_NOTAFILE;
    BEGIN_MUTEX (&handleLock) {
	struct fileHandle *h;
	h = (struct fileHandle *)epia_GetFileRock(file);
	if (h == NULL) {
	    /* Keep one handle in reserve to make failing opens cheaper. */
	    h = GetFileHandle();
	    tlock_Init (&h->lock, "file");
	    h->magic = FILE_MAGIC;
	    if (create) {
		epix_EnterCodeBlock(33);
		tlock_ObtainWriteNT(&h->lock);
		h->flags |= (FILE_BEING_DELETED|FILE_JUST_CREATED);
	    } else {
		epix_EnterCodeBlock(34);
		BEGIN_TLOCK_WRITE_NT (&h->lock) {
		    unsigned int ss;	/* size of status area */
		    code = epia_GetStatus (file, sizeof(h->status),
					   (char *)&h->status, &ss);
		    if (code) continue;
		    if (ss != sizeof(h->status))
			{ ABORT_CRIT (EPI_E_NOTAFILE) }
		    if ((unique != EPID_DONTCARE_UNIQUE) &&
			(unique != h->status.unique))
			{ ABORT_CRIT (EPI_E_BADUNIQUE) }
		} END_LOCK;
		if (code) {
		    epix_EnterCodeBlock(37);
		    DiscardFileHandle(h);
		    EXIT_CRIT;
		}
	    }
	    epia_SetFileRock (file, h, Cleanup);
	} else {
	    afsl_Assert(!create);
	    if (h->magic != FILE_MAGIC) { ABORT_CRIT (EPI_E_BADFILE) }
	    BEGIN_TLOCK_READ (&h->lock) {
		if (h->flags & FILE_JUST_CREATED) {
		    epix_EnterCodeBlock(78);
		    ABORT_CRIT (EPI_E_JUSTCREATED);
		}
		/* 
		 * Defect 4302 - should not have open races with files being 
		 * deleted.
		 */
		afsl_Assert(!(h->flags & FILE_BEING_DELETED));
		if ((unique != EPID_DONTCARE_UNIQUE) &&
		    (unique != h->status.unique))
		    { ABORT_CRIT (EPI_E_BADUNIQUE) }
	    } END_LOCK;
	}
	if (code) continue;
	h->refCount++;
	*hP = h;
    } END_LOCK;
    return code;
}

/* on disk structure stored for auxiliary containers.  Basically, the number of
 * uses of it by files in the volume. */

struct diskAux {
    u_int32 linkCount;
    int32 flags;	/* basically low 4 bits of flags encode acl or plist */
    int32 spare[2];
};

/* OpenAux -- makes sure file handle has opened all aux continers.
 *
 *   LOCKS USED -- The handle must be write-locked.  Otherwise use
 *	ReadLockWithAux. */

static long OpenAux(
  IN epi_anode_t file,
  IN struct fileHandle *h)
{
    int t;				/* aux type */
    long code;
    epi_volume_t vol = epiv_GetVolume (file);

    epix_EnterCodeBlock(4);
    AssertTWriteLocked (&h->lock);
    for (t=0; t<EPIF_AUX_NTYPES; t++) {
	int i = auxIndex[t];
	u_long index;
	if (i < 0) continue;
	index = AUX_FROM_STATUS (&h->status, t);
	if (h->aux[i]) {
	    afsl_PAssert (index,
			  ("open handle for non-existent aux container"));
	} else {
	    if (index) {
		epix_EnterCodeBlock(5);
		code = epiv_OpenAnode (vol, index, &h->aux[i]);
		if (code) {
		    h->aux[i] = 0;
		    return code;
		}
	    }
	}
    }
    return 0;
}

/* ReadLockWithAux -- locks a file and ensures that its auxiliary containers
 *     are open.
 *
 * LOCKS USED -- Since if the aux containers are already open (presumably the
 *     common case) we don't have to do anything, we approach this
 *     optimistically.  We read lock the file and if the aux files are open, we
 *     return successfully.  Otherwise we release the read lock, and open the
 *     aux containers under a write lock using OpenAux.  Then we try to get a
 *     read lock again.
 * CAUTIONS -- could starve, theoretically. */

static long ReadLockWithAux(
  IN epi_anode_t file,
  OUT struct fileHandle **handleP)
{
    long code;
    struct fileHandle *h;
    int starvation = 0;

    epix_EnterCodeBlock(7);
    if (!GetOKfile (*handleP, file)) return EPI_E_BADFILE;
    h = *handleP;
    do {
	int t;				/* aux type */
	int alreadyOpen;
	tlock_ObtainRead (&h->lock);
	/* check for already open or no aux containers */
	alreadyOpen = 1;
	for (t=0; t<EPIF_AUX_NTYPES; t++) {
	    int i = auxIndex[t];
	    u_long index;
	    if (i < 0) continue;
	    index = AUX_FROM_STATUS (&h->status, t);
	    if (index && !h->aux[i]) {
		alreadyOpen = 0;
		break;
	    }
	}
	if (alreadyOpen) return 0;
	epix_EnterCodeBlock(8);
	starvation++;
	tlock_ReleaseRead (&h->lock);
	BEGIN_TLOCK_WRITE_NT (&h->lock) {
	    code = OpenAux (file, h);
	} END_LOCK;
	if (code) return code;
    } while (starvation < 100);
    afsl_Assert(starvation == 0);		/* wow! */
    return 0;
}

/* DecAux -- reduces the linkCount on the specified auxilary container.  If the
 *     linkCount reaches zero the container is deleted.  In either case the
 *     anode handle is closed and cannot be used after this routne returns.
 *
 * PARAMETERS -- If the discardTransP parameter is non-nill it is set to true
 *     of if a large amount of activity was caused by this operation.  The
 *     caller can then end the current trans and start a new one in its place
 *     if desired.  This feature is only used by Delete.
 *
 * LOCKS USED -- We make certain optimizations on locking by making assumptions
 *     during delete.  See below.  Since we do write lock the tlock with the
 *     appropriate transaction this satisfies the EC requirements even though
 *     we are not holding the tlock *while* doing the delete.
 *
 * CAUTIONS -- This code assumes that the caller as exclusive access to a
 *     legitimate reference to the aux.  It infers that if the linkCount is one
 *     that the caller has the only reference to the aux. */

static long DecAux(
  IN elbb_tranRec_t trans,
  IN epi_anode_t aux,			/* open auxilary container to flush */
  OUT int *discardTransP)		/* maybe get fresh trans */
{
    long code;
    struct diskAux disk;

    epix_EnterCodeBlock(9);
    if (discardTransP) *discardTransP = 0;
    if (aux == 0) return 0;
    BEGIN_TLOCK_WRITE (trans, epiv_GetAuxTLock(epiv_GetVolume(aux))) {
	u_int size;			/* size of aux status area */
	code = epia_GetStatus (aux, sizeof(disk), (char *)&disk, &size);
	if (code) continue;
	if (size != sizeof(disk)) { ABORT_CRIT (EPI_E_NOTANACL) }
	afsl_Assert(disk.linkCount > 0);

	/* If this is last use we can delete aux container w/o holding the
	 * aux tlock since the only file pointing to it is write locked.  Note
	 * that for EC purposes we've already done the write tlock, so we're
	 * safe. */

	disk.linkCount--;
	if (disk.linkCount == 0) continue;

	/* Otherwise just update decremented linkCount */
	code = epia_SetStatus (trans, aux, 0, sizeof(disk), (char *)&disk);
	if (code) continue;
	code = epiv_CloseAnode (aux);
	if (code) continue;
    } END_LOCK; /* global aux lock */
    if (code) return code;
    if (disk.linkCount == 0) {		/* RACE: is this really safe? */
	afs_hyper_t length;
	u_long allocated;		/* former size */
	epix_EnterCodeBlock(10);
	if (discardTransP) allocated = epia_GetAllocated (aux);
	AFS_hzero (length);
	code = epia_Truncate (trans, aux, 0, length);
	if (code) return code;
	code = DeleteAnode (trans, aux);
	if (code) return code;
	if (discardTransP && (allocated > 1)) *discardTransP = 1;
    }
    return 0;
}

/* CopyAux -- copies the contents of the old auxiliary container to the new one.
 *     This is used for duplicating acl/plist's across volumes.
 *
 * LOCKS USED -- All locking is done by the caller. */

static long CopyAux(
  IN elbb_tranRec_t trans,
  IN epi_anode_t old,
  IN epi_anode_t new)
{
    long code;
    char buf[512];
    struct epia_info info;
    struct uio uio;
    struct iovec iov;
    u_long length;			/* total length of aux file */
    u_long offset = 0;
    u_long len;				/* transfer length */

    epix_EnterCodeBlock(11);
    code = epia_GetInfo (old, &info);
    if (code)
	return code;
    if (!AFS_hfitsinu32(info.length))
	return EPI_E_TOOBIG;

    /* set lengths to be the same */
    code = epia_Truncate (trans, new, 0L, info.length);
    if (code)
	return code;

    length = AFS_hgetlo(info.length);
    for (offset = 0; offset < length; offset += len) {
	epix_EnterCodeBlock(12);
	len = MIN(sizeof buf, length - offset);
	epia_InitUio (&uio, &iov, offset, len, buf);
	code = epia_Read (old, 0L, &uio);
	if (code)
	    break;
	epia_InitUio (&uio, &iov, offset, len, buf);
	code = epia_Write (trans, new, 0L, &uio);
	if (code)
	    break;
    }
    return code;
}

/* NewAux -- creates a new auxiliary container of the requested type.  The new
 *     container's index and its anode handle are returned.
 *
 * LOCKS USED -- grabs aux tlock briefly to get EC merging right. */

static long NewAux(
  IN elbb_tranRec_t trans,
  IN epi_volume_t vol,
  IN int whichAux,
  OUT u_long *indexP,			/* index of duplicated aux */
  OUT epi_anode_t *auxP)		/* newly opened handle for aux */
{
    long code;
    struct diskAux disk;
    int log;

    epix_EnterCodeBlock(13);
    afsl_PAssert (auxIndex[whichAux] >= 0, ("unknown aux type %d", whichAux));
    if (whichAux == EPIF_AUX_IS_USER_PLIST) log = 0;
    else log = EPIA_CFLAGS_LOGGED;
    log |= EPIV_CFLAGS_TYPE_AUX;
    code = epiv_CreateAnode (trans, vol, log, sizeof(disk),
			     0, 0, 0, 0, indexP, auxP);
    if (code) return code;

    /* Since this is a new aux we don't need to hold the aux tlock, but we do
     * need to "touch" it. */

    tlock_ObtainWrite (trans, epiv_GetAuxTLock(vol));
    tlock_ReleaseWriteNT (epiv_GetAuxTLock(vol));

    bzero((caddr_t)&disk, sizeof(disk));
    disk.linkCount = 1;
    disk.flags = whichAux;
    code = epia_SetStatus (trans, *auxP, 0, sizeof(disk), (char *)&disk);
    if (code) {
	long code2;
	code2 = DeleteAnode (trans, *auxP); 
	afsl_MBZ(code2); 
	*auxP = 0;
	return code;
    }
    return 0;
}

/* DupAux -- reopen the specified aux of the handle and return the index and
 *     anode handle.  Since we're making a new reference, increment the
 *     linkCount.  The handle must have had OpenAux called on it.  On error
 *     exit the index and aux handle are set to zero.
 *
 * LOCKS USED -- The file must be locked (read or write).
 *
 *     Source aux is read tlocked if it is in a different volume.
 *
 * RETURN CODES -- Returns EPI_E_BADAUXSOURCE if file didn't have the specified
 *     auxiliary container.
 *
 *     Also returns EPI_E_VOLUMEFULL and EPI_E_AGGREGATEFULL errors caused by
 *     copying the aux contents from another volume. */

static long DupAux(
  IN elbb_tranRec_t trans,
  IN struct fileHandle *h,		/* source of aux */
  IN int whichAux,
  IN epi_volume_t vol,			/* volume aux is being copied to */
  OUT u_long *indexP,			/* index of duplicated auxilary */
  OUT epi_anode_t *auxP)		/* newly opened handle for aux */
{
    long code;
    epi_anode_t aux;			/* current auxilary container */
    u_long index;
    struct diskAux disk;

    epix_EnterCodeBlock(14);

    /* Make sure file is locked and aux is opened */

    afsl_Assert(indexP && auxP);
    AssertTLocked (&h->lock);
    *indexP = 0;
    *auxP = 0;

    afsl_PAssert (auxIndex[whichAux] >= 0, ("unknown aux type %d", whichAux));
    aux = h->aux[auxIndex[whichAux]];
    index = AUX_FROM_STATUS(&h->status, whichAux);
    if (!aux) return EPI_E_BADAUXSOURCE; /* no aux to duplicate */
    afsl_Assert(index);

    if (vol != epiv_GetVolume(aux)) { /* make a whole new one */
	epix_EnterCodeBlock(15);
	code = NewAux (trans, vol, whichAux, indexP, auxP);
	if (code) return code;

	/* Need to read tlock source since it is a different volume and hence a
	 * different tlock than the new aux will have grabbed (in NewAux).  */

	BEGIN_TLOCK_READ (epiv_GetAuxTLock(epiv_GetVolume(aux))) {
	    code = CopyAux (trans, aux, *auxP);

	    /* We would like to use tlock_EquivNoAssign here but the different
             * volumes may actually be on different aggregates so equivalence
             * would be impossible. */

	} END_LOCK;
	if (code) {
	    long ccode;
	    ccode = DecAux (trans, *auxP, 0);
	    afsl_MBZ(ccode);
	    *indexP = 0;
	    *auxP = 0;
	}
	return code;
    }

    /* else reopen source aux and bump its link count */
    BEGIN_CRIT (code = epiv_OpenAnode (vol, index, &aux),
		ON_ERROR (code = epiv_CloseAnode (aux))) {
	BEGIN_TLOCK_WRITE (trans, epiv_GetAuxTLock(vol)) {
	    u_int ss;
	    code = epia_GetStatus (aux, sizeof(disk), (char *)&disk, &ss);
	    if (code) continue;
	    afsl_Assert(ss == sizeof (disk));
	    afsl_PAssert((((whichAux^disk.flags) & EPIF_AUX_TYPE_PLIST) == 0),
			  ("Aux type %x inconsistent with disk status %x",
			   whichAux, disk.flags));
	    /* this probably can be zero */
	    disk.linkCount++;
	    code = epia_SetStatus (trans, aux, 0, sizeof(disk), (char *)&disk);
	    if (code)
		EXIT_CRIT;
	} END_LOCK;
	if (code)
	    EXIT_CRIT;
	*auxP = aux;
	*indexP = index;
    } END_CRIT;
    return 0;
}

/* UnlinkAux -- disconnects the link between a file and an aux container.  The
 *     link count of the aux container is decremented and the file is updated.
 *     A new trans may be started if DecAux reports that a largish amount of
 *     activity was generated.
 *
 * LOCKS USED -- The file is assumed to be write locked. */

static long UnlinkAux(
  IN epi_anode_t file,
  IN struct fileHandle *h,
  IN int whichAux,
  IN int discardTrans,			/* get new trans if needed */
  INOUT elbb_tranRec_t *transP)
{
    long code;
    epi_anode_t aux;
    u_long index;
    int auxoff;
    int largish;			/* DecAux report lots of activity */

    epix_EnterCodeBlock(16);
    AssertTWriteLocked (&h->lock);
    afsl_PAssert (auxIndex[whichAux] >= 0, ("unknown aux type %d", whichAux));
    aux = h->aux[auxIndex[whichAux]];
    index = AUX_FROM_STATUS(&h->status, whichAux);

    /* make sure it is opened iff it exists */
    afsl_Assert((!index  && !aux) || (index && aux));
    if (aux == 0) return 0;		/* already gone */

    h->aux[auxIndex[whichAux]] = 0;
    AUX_FROM_STATUS(&h->status, whichAux) = 0;
    auxoff = auxOffset[whichAux];

    code = epia_SetStatus (*transP, file, auxoff, sizeof(u_int32),
			   (char *)(&h->status) + auxoff);
    afsl_MBZ(code);			/* we've already modified the handle */

    code = DecAux (*transP, aux, &largish);
    afsl_MBZ(code);
    if (discardTrans && largish) {
	code = epia_EndTran (file, *transP);
	afsl_MBZ(code);
	code = epia_StartTran ("UnlinkAux", file, transP);
	afsl_MBZ(code);
    }
    return 0;
}

/* LinkAux -- makes a connection between a file and an aux container.  The only
 *     link, if any, is broken.  The aux container must have already had its
 *     link count incremented.
 *
 * LOCKS USED -- The file is assumed to be write locked. */

static long LinkAux(
  IN elbb_tranRec_t trans,
  IN epi_anode_t file,
  IN struct fileHandle *h,
  IN int whichAux,
  IN u_long index,
  IN epi_anode_t aux)
{
    long code;
    u_long oldIndex;
    epi_anode_t oldAux;
    int auxoff;

    epix_EnterCodeBlock(19);
    AssertTWriteLocked (&h->lock);
    afsl_PAssert (auxIndex[whichAux] >= 0, ("unknown aux type %d", whichAux));
    oldAux = h->aux[auxIndex[whichAux]];
    oldIndex = AUX_FROM_STATUS(&h->status, whichAux);

    /* make sure it is opened iff it exists */
    afsl_Assert((!oldIndex  && !oldAux) || (oldIndex && oldAux));
    if (oldAux) { /* flush previous */
	code = DecAux (trans, oldAux, 0);
	if (code) return code;
    }

    /* install new one into proper slot */
    h->aux[auxIndex[whichAux]] = aux;
    AUX_FROM_STATUS(&h->status, whichAux) = index;
    auxoff = auxOffset[whichAux];

    code = epia_SetStatus (trans, file, auxoff, sizeof(u_int32),
			   (char *)(&h->status) + auxoff);
    afsl_MBZ(code);
    return 0;
}

/* Delete -- deletes a file once the last reference to it is gone.
 *
 * PARAMETERS -- Note that this routine starts and returns a transaction.  Some
 *     of its work, namely the file truncation is done before the transaction
 *     is created so the size of transactions can be limited.  If an error
 *     occurs and a transaction has been started, it is ended before returning.
 *
 * LOCKS USED -- The file is locked because several aux functions require it.
 *     It relies on the vnode layer not trying to open a fid that it is also
 *     trying to close or delete.
 */

static long Delete(
  IN epi_anode_t fileC,
  IN struct fileHandle *fileH,
  OUT elbb_tranRec_t *transP)
{
    long code;

    epix_EnterCodeBlock(24);

    BEGIN_LCRIT (int newTrans,
		 newTrans = 0,
		 ON_ERROR (if (newTrans)
			   code = epia_EndTran (fileC, *transP))) {

	if (!elbb_IsZeroTrans (*transP)) {
	    /* for the error exit case in DeleteOrClose */
	    newTrans = 0;
	} else {
	    /* Delete only works on zero length files so the file must be
	     * truncated first.  In addition, the constraint on transaction
	     * size means that large files may need to be truncated in a series
	     * of transactions. */
	    afs_hyper_t length;
	    AFS_hzero (length);
	    code = epia_Truncate (elbb_zeroTrans, fileC, 0, length);
	    if (code) continue;
	    code = epia_StartTran ("Delete", fileC, transP);
	    if (code) continue;
	    newTrans = 1;
	}
	/* otherwise epia_Delete will fail. */
	afsl_Assert (AFS_hiszero (epia_GetLength(fileC)) &&
		     (epia_GetAllocated(fileC) == 0));

	BEGIN_TLOCK_WRITE (*transP, &fileH->lock) {
	    int t;			/* aux type */
	    /* These routines assume the file is locked, though it's not
	     * strictly necessary in this case, due to refCount considerations.
	     * On the other hand EC management probably does require it. */
	    code = OpenAux (fileC, fileH);
	    if (code) continue;
	    for (t=0; t<EPIF_AUX_NTYPES; t++) {
		if (auxIndex[t] < 0) continue;
		code = UnlinkAux (fileC, fileH, t, newTrans, transP);
		if (code) break;
	    }
	} END_LOCK;
	if (code) continue;

	Reschedule();
	BEGIN_MUTEX (&handleLock) {
	    afsl_Assert(fileH->refCount == 1);
	    /* Checking h->flags is safe since we hold the handleLock and by
	     * Assumption 1 there are no other references to this file. */
	    afsl_Assert(fileH->flags & FILE_BEING_DELETED);
	    fileH->refCount--;
	} END_LOCK;
	code = DeleteAnode (*transP, fileC);
    } END_CRIT;
    return code;
}

/* Close -- cleans up an open file.
 *
 * LOCKS USED -- The handleLock is held while the refCount is checked.  If it
 *     is going to zero we lock the file and close the auxiliary containers.
 *     Then we decrement the refCount and close the anode handle. */

static long Close(
  IN epi_anode_t fileC,
  IN struct fileHandle *fileH)
{
    long code = 0;

    BEGIN_MUTEX (&handleLock) {
	if (fileH->refCount == 1)
	    BEGIN_TLOCK_WRITE_NT (&fileH->lock) {
		int t;
		fileH->flags &= ~FILE_JUST_CREATED;
		afsl_Assert(!(fileH->flags & FILE_BEING_DELETED));
		for (t=0; t<EPIF_AUX_NTYPES; t++) {
		    int i = auxIndex[t];
		    epi_anode_t aux;
		    if (i < 0) continue;
		    aux = fileH->aux[i];
		    if (aux) {
			epix_EnterCodeBlock(6);
			code = epiv_CloseAnode (aux);
			if (code) continue;
			fileH->aux[i] = 0;
		    }
		}
	    } END_LOCK;
	fileH->refCount--;
    } END_LOCK;
    if (code) return code;
    code = epiv_CloseAnode (fileC);
    return code;
}

/* The following procedures manage the per volume list of zero link count
 * files.  The list is stored in the volume status area called fileStatus.  It
 * is treated by the volume module as a simple array of 32 bits integers.  The
 * file module imposes a bit of additional structure on the array.
 *
 * The list of files handled in two different ways.  If there are less than
 * N_ZERO_LINK files the nFiles field may be set to NO_COUNT and then the
 * members of the list are determined by searching for non-zero array elements.
 * Otherwise nFiles is the count of the number of zero link count files and the
 * contents of the array is only a subset of the actual list. */

#define N_ZERO_LINKS 10			/* elts in array */
#define NO_COUNT 0xffffffff		/* no count so examine the list */

struct zeroLinks {
    u_int32 nFiles;			/* number of files in list */
    u_int32 files[N_ZERO_LINKS];	/* epiv-style indexes */
};

/* Unlink -- remembers a file whose link count is zero but which has not yet
 *     been deleted.  Called from epif_ChangeLink.  Compare Inactive.
 *
 * LOCKS USED -- A global lock called zeroLock protects all zeroLinks lists.
 *     It is not a tlock since the volume module already protects the entire
 *     volume header with a tlock. */

static long Unlink(
  IN elbb_tranRec_t trans,
  IN epi_anode_t fileC)
{
    long code;
    epi_volume_t vol;
    struct zeroLinks *zeros;
    u_long index;

    afsl_Assert(!elbb_IsZeroTrans(trans));
    vol = epiv_GetVolume(fileC);
    code = epiv_GetZLCAddr (vol, (int32 **)&zeros);
    if (code) return code;
    index = epiv_GetAnodeIndex (fileC);
    afsl_Assert(index);
    BEGIN_MUTEX (epiv_GetZeroLock(vol)) {
	int i;
	u_long count = zeros->nFiles;
	struct epiv_status s;		/* local for passing to SetStatus */
	int slot = 0;			/* element in s.fileStatus to update */
	long mask;

	for (i = 0; i < N_ZERO_LINKS && zeros->files[i] != 0; i++)
	    afsl_Assert(zeros->files[i] != index);

	if (i != N_ZERO_LINKS)
	    slot = i + 1;

	if (slot == 0) {
	    /* no room to store index, just increment count. */
	    if (count == NO_COUNT) count = N_ZERO_LINKS;
	    count++;
	    s.fileStatus[slot] = count;
	} else if (count == 0) {
	    /* list is empty so add this index and switch to NO_COUNT mode. */
	    s.fileStatus[slot] = index;
	    mask = (slot<<EPIV_STATUS_FILESTATUS_N_SHIFT) |
		EPIV_STATUS_FILESTATUS_WORD_N;
	    code = epiv_SetStatus (trans, vol, mask, &s);
	    if (code)
		EXIT_CRIT;
	    slot = 0;
	    s.fileStatus[slot] = NO_COUNT;
	} else if (count == NO_COUNT) {
	    /* we're not maintaining count so just use the free slot. */
	    s.fileStatus[slot] = index;
	} else {
	    /* we have to update count and we should add index to array. */
	    s.fileStatus[slot] = index;
	    mask = (slot<<EPIV_STATUS_FILESTATUS_N_SHIFT) |
		EPIV_STATUS_FILESTATUS_WORD_N;
	    code = epiv_SetStatus (trans, vol, mask, &s);
	    if (code)
		EXIT_CRIT;
	    count++;
	    slot = 0;
	    s.fileStatus[slot] = count;
	}
	mask = (slot<<EPIV_STATUS_FILESTATUS_N_SHIFT) |
	    EPIV_STATUS_FILESTATUS_WORD_N;
	code = epiv_SetStatus (trans, vol, mask, &s);
    } END_LOCK;
    return code;
}

/* Inactive -- removes a file from the zeros list.  Compare Unlink. */

static long Inactive(
  IN elbb_tranRec_t trans,
  IN epi_volume_t vol,
  IN u_long index)
{
    long code;
    struct zeroLinks *zeros;

    afsl_Assert(!elbb_IsZeroTrans(trans) && index);
    code = epiv_GetZLCAddr (vol, (int32 **)&zeros);
    if (code) return code;
    BEGIN_MUTEX (epiv_GetZeroLock(vol)) {
	int i;
	int found = -1;
	int nFiles = 0;			/* number actually in list */
	u_long count = zeros->nFiles;
	struct epiv_status s;		/* local for passing to SetStatus */
	int slot;			/* element in s.fileStatus to update */
	long mask;

	for (i=0; i<N_ZERO_LINKS; i++)
	    if (zeros->files[i] == index) {
		found=i;
		break;
	    } else if (zeros->files[i] > 0)
		nFiles++;

	if (count != NO_COUNT) {
	    count--;
	    if (count == nFiles) {
		/* listed files are the only ones, revert to NO_COUNT mode. */
		slot = 0;
		s.fileStatus[slot] = NO_COUNT;
	    } else {
		/* otherwise just decrement the count. */
		slot = 0;
		s.fileStatus[slot] = count;
	    }
	    mask = (slot<<EPIV_STATUS_FILESTATUS_N_SHIFT) |
		EPIV_STATUS_FILESTATUS_WORD_N;
	    code = epiv_SetStatus (trans, vol, mask, &s);
	    if (code) continue;
	}
	if (found != -1) {
	    /* remove entry from list */
	    slot = found+1;
	    s.fileStatus[slot] = 0;
	    mask = (slot<<EPIV_STATUS_FILESTATUS_N_SHIFT) |
		EPIV_STATUS_FILESTATUS_WORD_N;
	    code = epiv_SetStatus (trans, vol, mask, &s);
	    if (code) continue;
	}
    } END_LOCK;
    return code;
}

/* epif_NextDeletedFile -- Returns the fid of the next file with a zero link
 *     count in the fileset.  An iterator is used to control the return of all
 *     files in the list.  If no changes are made to the list during a
 *     succession of calls to the procedure all the entries will be returned.
 *     If entries are only removed from the list, the same fid may be returned
 *     twice, but no fids will be missed.  For robustness, this procedure does
 *     not assume that each returned entry is deleted before the next call.
 *
 * PARAMETERS --
 *     vol -- fileset handle.
 *     iter -- iterator, set to 0 to start the iteration.
 *     nextIterP -- the iterator to use to get next value.  When this becomes
 *	 \hex{ffffffff}, it indicates that the last fid has been returned.
 *     fid -- file identifier of the deleted file.
 *
 * LOCKS USED -- The zeroLock is held for read during the search.  No locking
 *     prevents changes to the list between calls.  Note also that the call to
 *     epiv_OpenAnode must be while the lock is help.  
 *
 * PERFORMANCE -- When there are more zero link count files than fit in the
 *     array, the entire volume must be searched.  This could be optimized by
 *     keeping minimum and maximum values in the array to bound the search.
 *
 * RETURN CODES -- E_ITERATIONDONE is returned if there are no more deleted
 *     files.  Also an \arg{iter} value of \hex{ffffffff} is rejected with this
 *     code.  This will happen if the last operation's \arg{nextIterP} output
 *     parameter is used on a subsequent call. */

/* EXPORT */
long epif_NextDeletedFile(
  IN epi_volume_t vol,
  IN u_long iter,
  OUT u_long *nextIterP,
  OUT epi_volFileId_t *fid)
{
    long code;
    struct zeroLinks *zeros;
    u_long index;
    u_long lastIndex;			/* for whole volume searches */
    epi_anode_t f;
    struct fileHandle *h;
    int done;

    if (iter == 0xffffffff) return EPI_E_ITERATIONDONE;
    *nextIterP = 0xffffffff;
    epid_MakeInvalid (fid);
    done = 0;
    lastIndex = 0;

    if ((iter == N_ZERO_LINKS) || !nextIterP) return EPI_E_BADARGS;
    code = epiv_GetZLCAddr (vol, (int32 **)&zeros);
    if (code) return code;

    /* The basic logic here is that we encode our progress in two ways.  If
     * iter is < N_ZERO_LINKS we are just walking through the table.  If it is
     * greater we are walking through the whole volume.  The equal case is
     * disallowed (reserved for future use?).  We may decide to switch between
     * modes if changes indicate that this would be advantageous, but we
     * generally need to start over if this occurs. */

    for (done = 0; !done; iter++) {
	code = 0;
	index = 0;			/* no file found */
	BEGIN_MUTEX (epiv_GetZeroLock(vol)) {
	    u_long count = zeros->nFiles;

	    /* handle boundary conditions for iter */

	    if (count == 0) continue;	/* no files */
	    if ((count == NO_COUNT) && (iter > N_ZERO_LINKS))
		iter = 0;		/* restart in no count mode */
	    else if ((count != NO_COUNT) && (iter < N_ZERO_LINKS))
		iter = N_ZERO_LINKS+1;	/* start, or restart in count mode */

	    /* calculate appropriate index based on iter */

	    if (iter > N_ZERO_LINKS) {
		/* walk through whole volume */
		index = iter - N_ZERO_LINKS;
		if (!lastIndex) {
		    /* for efficiency, only do this once per call */
		    code = epiv_LastIndex (vol, &lastIndex);
		    if (code) continue;
		    afsl_Assert(lastIndex);
		}
		if (index > lastIndex) {
		    index = 0;		/* no more files */
		    continue;
		}

	    } else if (iter < N_ZERO_LINKS) {
		/* walk through array */
		do {
		    index = zeros->files[iter];
		    if (index) break;
		    iter++;
		} while (iter < N_ZERO_LINKS);
		if (iter == N_ZERO_LINKS)
		    /* assert (index == 0) */
		    continue;		/* no more files */
	    } else {
		/* can't happen... */
		afsl_AssertCond (iter != N_ZERO_LINKS);
	    }
	    code = epiv_OpenAnode (vol, index, &f);
	    if (code) continue;
	} END_LOCK;
	if (code == EPI_E_BADANODEINDEX) {
	    /* A deleted file: okay if iterating through the whole volume */
	    epix_EnterCodeBlock(32);
	    afsl_Assert (iter > N_ZERO_LINKS);
	    continue;
	} else if (code) return code;
	if (index == 0) return EPI_E_ITERATIONDONE;

	code = GetHandle (f, /*!create*/0, EPID_DONTCARE_UNIQUE, &h);
	if (code == EPI_E_NOTAFILE) {
	    /* This error means we hit an ACL or Plist.  It is okay only if
	     * iterating through the whole volume */
	    epix_EnterCodeBlock(42);
	    afsl_Assert (iter > N_ZERO_LINKS);
	}
	if (code == 0) {
	    if (h->status.linkCount == 0) {
		/* this is a good one, return it. */
		epix_EnterCodeBlock(43);
		epid_Make (fid, epia_AnodeDevice(vol),
			   index, h->status.unique);
		done++;
	    } else {
		/* This means we hit a good file.  It is okay only if iterating
		 * through the whole volume */
		epix_EnterCodeBlock(77);
		afsl_Assert (iter > N_ZERO_LINKS);
	    }
	    code = Close (f, h);
	} else {
	    long code2;

	    code2 = epiv_CloseAnode(f);
	    afsl_MBZ(code2);
	}
    }
    if (code) return code;
    if (iter == N_ZERO_LINKS)
	iter = 0xffffffff;		/* all done */
    *nextIterP = iter;
    return 0;
}

/* DeleteOrClose -- does the processing cleanup after a create.  Special
 *     attention is given to handling the error cases.
 *
 * LOCKS USED -- References file flags to check deleted flag w/o locking.
 *     Since this bit is only used in restricted, single threaded
 *     circumstances, this shouldn't cause any trouble. */

static long DeleteOrClose(
  IN elbb_tranRec_t trans,
  IN epi_anode_t fileC,
  IN struct fileHandle *fileH)
{
    long code;

    if (fileH == 0) {
	/* In the case the handle could not be created. */
	epix_EnterCodeBlock(22);
	code = DeleteAnode (trans, fileC);
    } else {
	int delete = fileH->flags & FILE_BEING_DELETED;
	if (delete) {
	    /* Some error occurred after the handle was created and its
	     * refCount was bumped. */
	    epix_EnterCodeBlock(2);
	    code = Delete (fileC, fileH, &trans);
	} else {
	    /* This is the successful exit case. */
	    epix_EnterCodeBlock(23);
	    code = Close (fileC, fileH);
	}
    }
    return code;
}

/* epif_CreateE -- This is the test_anode interface to epif_Create.  The vnops
 *     layer only calls epif_Create of directly to epiv_OpenAnode from the
 *     volops.
 *
 * PARAMETERS -- The backingSource argument is used to specify the target for a
 *     newly created COW or backing file and may only be used for testing. */

/* EXPORT */
long epif_CreateE(
  elbb_tranRec_t trans,
  epi_volume_t vol,
  long flags,
  u_long estLength,
  struct epif_status *initialStatus,
  epi_anode_t aclSource,		/* file to get acl from */
  epi_anode_t plistSource,		/* file to get plist from */
  epi_anode_t backingSource,		/* file to be copy on write from */
  epi_volFileId_t *fid,			/* output fid */
  epi_anode_t *fileP)			/* output anode handle */
{
    long code;
    int i, t;				/* aux index and type */
    struct diskStatus status;
    epi_anode_t fileC;			/* opened new file */
    unsigned int statusSize;
    u_long index;
    u_long unique;
    struct timeval now;
    epi_anode_t sAux[AUX_NINDEXES];	/* aux containers from source */
    u_long sAuxIndex[AUX_NINDEXES];	/* aux container indexes from source */
    epi_anode_t backingV;		/* backing volume container */
    u_long backingI;			/* backing index */

    epix_EnterCodeBlock(28);
    *fileP = NULL;
    if (flags & (EPIA_CFLAGS_TYPE | EPIA_CFLAGS_LOGGED |
		 EPIA_CFLAGS_PREALLOCATED | EPIV_CFLAGS))
	return EPI_E_BADFLAGS;
    if ((flags & (EPIA_CFLAGS_COPYONWRITE|EPIA_CFLAGS_BACKING)) ==
	(backingSource == 0))
	return EPI_E_BADFLAGS;
    if ((flags & EPIA_CFLAGS_BACKING) && (aclSource || plistSource))
	return EPI_E_BADARGS;
    if (flags & EPIF_CFLAGS_DIRECTORY) flags |= EPIA_CFLAGS_LOGGED;
    if (flags & EPIF_CFLAGS_VOLUMEROOT) {
	epix_EnterCodeBlock(29);
	if (flags & (EPIF_CFLAGS_SHARE_ACL | EPIF_CFLAGS_SHARE_PLIST))
	    return EPI_E_BADFLAGS;
	flags |= EPIF_CFLAGS_VOLUMEROOT;
	flags |= EPIA_CFLAGS_LOGGED | EPIV_CFLAGS_USEFID;
	index = 1;
    } else if (flags & EPIF_CFLAGS_USEFID) {
	flags |= EPIV_CFLAGS_USEFID;
	index = epid_ToEPIVIndex (fid, epia_AnodeDevice(vol));
    } else index = 0;
    flags |= EPIV_CFLAGS_TYPE_FILE;

    backingV = 0, backingI = 0;
    statusSize = sizeof(status);
    if (backingSource) {
	epix_EnterCodeBlock(30);
	backingV = epiv_GetVolume (backingSource);
	backingI = epiv_GetAnodeIndex (backingSource);
	if (epia_GetStatusSize(backingSource) != statusSize)
	    return EPI_E_NOTAFILE;
	statusSize = 0;
    }

    /* get acl and plist info */
    BEGIN_CRIT(for (i=0; i<AUX_NINDEXES; i++) sAux[i] = 0,
	       for (i=0; i<AUX_NINDEXES; i++) {
		   long code2;
		   if (sAux[i]) {
		       code2 = DecAux(trans, sAux[i], 0);
		       afsl_MBZ(code2);
		   }
	       });

#define DupSource(st, t) \
    code = DupAux (trans, sh, st, vol, \
		   &sAuxIndex[auxIndex[t]], &sAux[auxIndex[t]]); \
    if (code == EPI_E_BADAUXSOURCE) code = 0; /* no source of this type */ \
    if (code) continue;

    if (aclSource) {
	struct fileHandle *sh;
	epix_EnterCodeBlock(17);
	BEGIN_CRIT (code = ReadLockWithAux (aclSource, &sh),
		    tlock_ReleaseRead (&sh->lock)) {
	    if (flags & EPIF_CFLAGS_DIRECTORY) {
		DupSource (EPIF_AUX_IS_INITIAL_DIR_ACL,
			   EPIF_AUX_IS_INITIAL_DIR_ACL);
		DupSource (EPIF_AUX_IS_INITIAL_FILE_ACL,
			   EPIF_AUX_IS_INITIAL_FILE_ACL);

		/* TODO -- In case we already copied this from another volume
		 *     we'd like to avoid making a second copy.  DupAux doesn't
		 *     provide quite the right semantics, however.  One
		 *     approach would be to dup the ACL from the directories
		 *     own initial dir acl once it has been created.  For now
		 *     just do the easy thing. */

		DupSource (EPIF_AUX_IS_INITIAL_DIR_ACL, EPIF_AUX_IS_ACL);
	    } else {
		DupSource (EPIF_AUX_IS_INITIAL_FILE_ACL, EPIF_AUX_IS_ACL);
	    }
	} END_CRIT;
	if (code) continue;
    }
    if (plistSource) {
	struct fileHandle *sh;
	epix_EnterCodeBlock(18);
	BEGIN_CRIT (code = ReadLockWithAux (plistSource, &sh),
		    tlock_ReleaseRead (&sh->lock)) {
	    DupSource (EPIF_AUX_IS_PLIST, EPIF_AUX_IS_PLIST);
	    DupSource (EPIF_AUX_IS_USER_PLIST, EPIF_AUX_IS_USER_PLIST);
	} END_CRIT;
	if (code) continue;
    }

    BEGIN_LCRIT (struct fileHandle *h = 0,
		 code = epiv_CreateAnode
		     (trans, vol, flags, statusSize, estLength,
		      backingV, backingI, /*near*/0, &index, &fileC),
		 ON_ERROR (code = DeleteOrClose (trans, fileC, h))) {
	code = GetHandle (fileC, /*create*/1, EPID_DONTCARE_UNIQUE, &h);
	if (code) continue;

	BEGIN_CRIT (AssertTWriteLocked (&h->lock),
		    tlock_ReleaseWrite (trans, &h->lock)) {
	    if (flags & EPIA_CFLAGS_BACKING) {
		unsigned int ss;	/* size of status area */
		code = epia_GetStatus (fileC, sizeof(h->status),
					(char *)&h->status, &ss);
		afsl_Assert ((code == 0) && (ss == sizeof(h->status)));
	    } else {
		afs_hyper_t vv;

		/* Now initialized new file's status. */

		/* NOTE -- it isn't clear it makes much sense to set the status
		 *     of a newly cloned COW file either, so this probably
		 *     should be (!backingSource).  The reason it can't be
		 *     changed, however, is because of the way cloning is used
		 *     from test_anode (backingSource is never specified in
		 *     real usage).  We don't clone whole volumes so that
		 *     acl/plist indexes are invalid in a freshly created COW.
		 *     To prevent trouble we just zero these things out. */
		bzero((caddr_t)&status, sizeof (status));
		status.mode = initialStatus->mode;
		status.oid = initialStatus->oid;
		status.gid = initialStatus->gid;
		status.uuid = initialStatus->uuid;

		status.linkCount = 1;
		epif_UniqueTime (&now);
		status.cTime = now;
		status.mTime = now;
		status.aTime = now;

		code = epiv_NewVolumeVersion (trans, vol, &vv);
		if (code) continue;
		DFSH_DiskFromMemHyper(status.volumeVersionNumber, vv);

		h->status = status;

		if (flags & EPIF_CFLAGS_VOLUMEROOT) h->status.unique = 1;
		else {
		    if (index == 1) { ABORT_CRIT (EPI_E_NOVOLUMEROOT) }
		    if (flags & EPIF_CFLAGS_USEFID) {
			h->status.unique = epid_ToUnique (fid);
		    } else {
			code = epiv_NewUnique (trans, vol, /*gen*/1, index,
						&h->status.unique);
			if (code) continue;
		    }
		    afsl_Assert(h->status.unique != 0 &&
			    h->status.unique != EPID_DONTCARE_UNIQUE);
		}
		code = epia_SetStatus (trans, fileC, 0, sizeof(h->status),
					(char *)&h->status);
		if (code) continue;
	    }
	    unique = h->status.unique;

	    for (t=0; t<EPIF_AUX_NTYPES; t++) {
		int i = auxIndex[t];
		if (i < 0) continue;
		if (sAux[i]) {
		    code = LinkAux (trans, fileC, h, t, sAuxIndex[i], sAux[i]);
		    if (code) break;
		    sAux[i] = 0;	/* don't flush after this */
		    sAuxIndex[i] = 0;
		}
	    }
	    if (code) continue;
	    h->flags &= ~FILE_BEING_DELETED;
	} END_CRIT; /* unlock handle */
    } END_CRIT; /* delete anode on error */
    END_CRIT; /* punt auxilary containers on error */
    if (code) return code;
    *fileP = fileC;
    epid_Make (fid, epia_AnodeDevice(vol), index, unique);
    afsl_Assert (epid_IsLegal (fid));
    return 0;
}

/* epif_Create -- Creates a new file with the initial attributes specified by
 *     \arg{initialStatus}.  The initial ACL is obtained from the file
 *     \arg{aclSource} if it is non-zero.  If the new file is to be a
 *     directory it will also obtain its initial file ACL and initial directory
 *     ACL from this source following the ACL inheritance rules.  The initial
 *     property list is obtained from \arg{plistSource} similarly.  Some fields
 *     from \arg{initialStatus} are ignored, namely, \fld{linkCount},
 *     \fld{flags}, \fld{unique}, and \fld{dataVersionNumber}.  These fields
 *     can be set by routines such as \fcn{epif_ChangeLink} and
 *     \fcn{epif_SetStatus}.  Other fields are also ignored and are initialized
 *     to appropriate values, for example, \fld{mtime} and
 *     \fld{volumeVersionNumber}.
 *
 * PARAMETERS --
 *     trans -- transaction to use for the operation.
 *     vol -- fileset in which to create the file.
 *     flags -- bits which control various creation options.
 *	 EPIF_CFLAGS_DIRECTORY -- the new file will be used as a directory.
 *	 EPIF_CFLAGS_VOLUMEROOT -- the new file is the root of a fileset.
 *	     This implies it is a directory.
 *	 EPIF_CFLAGS_USEFID -- the \arg{fid} is to be used as the file
 *	     identifier of the new file.
 *     estLength -- passed to lower level versions of this procedure. (unused)
 *     initialStatus -- pointer to the attributes for the new file.
 *     aclSource -- file from which to copy the initial ACL.
 *     plistSource -- file from which to copy the initial property list.
 *     fid -- pointer to the new file's fid.
 *     fileP -- pointer to the new file's anode handle.  This handle is
 *         reserved for the exclusive access by the caller, because
 *         \cst{FILE_JUST_CREATED} is set.  This flag will prevent epif_Open
 *         from returning this anode handle, it will return \cst{EPI_E_NOENT}
 *         instead.  The flag is cleared when the anode handle is closed or
 *         when \fcn{epif_AllowOpens}. */

/* EXPORT */
long epif_Create (
  IN elbb_tranRec_t trans,
  IN epi_volume_t vol,
  IN long flags,
  IN unsigned long estLength,
  IN struct epif_status *initialStatus,
  IN epi_anode_t aclSource,		/* file to get acl from */
  IN epi_anode_t plistSource,		/* file to get plist from */
  OUT epi_volFileId_t *fid,		/* output fid */
  OUT epi_anode_t *fileP)		/* output anode handle */
{
    long code;

    epix_EnterCodeBlock(35);
    if (flags & (EPIA_CFLAGS_COPYONWRITE|EPIA_CFLAGS_BACKING))
	return EPI_E_BADFLAGS;
    code = epif_CreateE (trans, vol, flags, estLength, initialStatus,
			 aclSource, plistSource, 0, fid, fileP);
    return code;
}

/* epif_Open -- Returns a handle for a file given its \arg{fid}.  This
 *     handle must be closed by calling \fcn{epif_Close} or
 *     \fcn{epif_ChangeLink}.
 *
 *     Most internally generated errors that could result from routine
 *     operation are converted to ENOENT.  This is because a \arg{fid} contains
 *     an index which may be arbitrarily bogus.  In the process of determining
 *     the validity of a fid errors may be encountered at many different points
 *     which can generate a broad spectrum of errors.
 *
 * PARAMETERS --
 *     vol -- fileset in which the file resides.
 *     fid -- description of the file to be opened.
 *     fileP -- pointer to the handle for the file.  It is zeroed on any error.
 *
 * CAUTIONS -- The caller must ensure that use of the don't care value for
 *     unique (e.g. directory lookups and duing volume operations) does not
 *     conflict with any other operations on the same file.  For instance,
 *     attempting to delete a file (i.e. using \fcn{epif_ChangeLink}) while
 *     it is open during a volume operation will fail.  See especially
 *     Assumptions 2 \& 3 at the beginning of this section. */

/* EXPORT */
long epif_Open(
  IN epi_volume_t vol,
  IN epi_volFileId_t *fid,
  OUT epi_anode_t *fileP)
{
    long code;
    u_long index;			/* epiv-style index */
    epi_anode_t fileC;
    struct fileHandle *h;

    epix_EnterCodeBlock(36);
    if (!fileP) return EPI_E_BADARGS;
    *fileP = 0;
    if (epid_ToUnique(fid) == 0) return EPI_E_NOENT; /* bad unique */
    index = epid_ToEPIVIndex (fid, epia_AnodeDevice(vol));
    fileC = 0;

    code = epiv_OpenAnode (vol, index, &fileC);
    if (code) {
	icl_Trace4(epi_iclSet, TR_EPIF_OPEN_OPENANODE_ERROR,
		   ICL_TYPE_POINTER, vol,
		   ICL_TYPE_LONG, fid->index,
		   ICL_TYPE_LONG, index,
		   ICL_TYPE_LONG, code);
    }
    if (!code) {
	code = GetHandle (fileC, /*!create*/0, epid_ToUnique(fid), &h);
	if (code) {
	    long code2;
	    code2 = epiv_CloseAnode(fileC);
	    afsl_MBZ(code2);
	    icl_Trace4(epi_iclSet, TR_EPIF_OPEN_GETHANDLE_ERROR,
			ICL_TYPE_POINTER, vol,
			ICL_TYPE_LONG, fid->index,
			ICL_TYPE_LONG, fid->unique,
			ICL_TYPE_LONG, code);
	}
    }
    if (code) {
	epix_EnterCodeBlock(40);
	if ((code == EPI_E_BADANODEINDEX) ||
	    (code == EPI_E_BADUNIQUE) ||
	    (code == EPI_E_JUSTCREATED) ||
	    (code == EPI_E_NOTAFILE)) {
	    EFS_TRACE_ANODE_ERROR_MAPPING(code, EPI_E_NOENT);
	    return EPI_E_NOENT;
	}
    } else *fileP = fileC;
    return code;
}

/* epif_GetStatus -- Gets the status information associated with the file.
 *
 * PARAMTERS --
 *     f -- file handle.
 *     status -- pointer to file's status.
 *
 * LOCKS USED -- The file is read locked, although this probably duplicates the
 *     vnode lock. */

/* EXPORT */
long epif_GetStatus(
  IN epi_anode_t f,
  OUT struct epif_status *status)
{
    long code;
    struct fileHandle *h;

    epix_EnterCodeBlock(41);
    BEGIN_CRIT (code = ReadLockWithAux (f, &h),
		tlock_ReleaseRead (&h->lock)) {
	afs_hyper_t vd;			/* old VV location in anode volData */
	int t;				/* aux type */

	bzero((caddr_t)status, sizeof(*status));
	status->linkCount = h->status.linkCount;
	status->mode = h->status.mode;
	status->flags = h->status.flags;
	status->unique = h->status.unique;
	status->dataVersionNumber = h->status.dataVersionNumber;
	status->mTime = h->status.mTime;
	status->cTime = h->status.cTime;
	status->aTime = h->status.aTime;
	status->res_timeUncertainty = h->status.res_timeUncertainty;
	status->clientBits = h->status.clientBits;
	status->oid = h->status.oid;
	status->gid = h->status.gid;

	/* Get VV from "best" place to provide compatibility with ancient disk
         * formats. */
	DFSH_MemFromDiskHyper (status->volumeVersionNumber,
			       h->status.volumeVersionNumber);
	AFS_hset64(vd, epia_GetVolDataHi(f), epia_GetVolDataLo(f));
	if (AFS_hcmp (status->volumeVersionNumber, vd) < 0)
	    status->volumeVersionNumber = vd;

	for (t=0; t<EPIF_AUX_NTYPES; t++) {
	    int i = auxIndex[t];
	    epi_anode_t aux;
	    u_long auxLen = 0;
	    if (i < 0) continue;
	    aux = h->aux[i];
	    if (aux) {
		epix_EnterCodeBlock(20);
		afsl_PAssert (AFS_hfitsinu32(epia_GetLength(aux)) &&
			      (epia_GetStatusSize(aux) ==
				sizeof (struct diskAux)),
			      ("bogus aux container"));
		auxLen = AFS_hgetlo(epia_GetLength(aux));
	    }
	    switch (t) {
	      case EPIF_AUX_IS_ACL:
		status->acl = auxLen; break;
	      case EPIF_AUX_IS_INITIAL_DIR_ACL:
		status->initialDirAcl = auxLen; break;
	      case EPIF_AUX_IS_INITIAL_FILE_ACL:
		status->initialFileAcl = auxLen; break;
	      case EPIF_AUX_IS_PLIST:
		status->plist = auxLen; break;
	      case EPIF_AUX_IS_USER_PLIST:
		status->reservedForUserPlist = auxLen; break;
	    }
	}

	status->uuid = h->status.uuid;
	status->spares = h->status.spares;
	bcopy((caddr_t)h->status.spare, (caddr_t)status->spare,
		sizeof (status->spare));
    } END_CRIT;
    if (code) return code;
    return 0;
}

/* epif_SetStatusAndMark -- Changes those fields of the file's status that
 *     are selected by the setStatusFlags to have the values specified in
 *     status.  Changes those (time-stamp) fields of the file's status that
 *     are selected by the markFlags to have the value of the current time.
 *     (These include one or more of:  atime, mtime, ctime.)
 *     Either setStatusFlags or markFlags may be zero; if setStatusFlags is
 *     zero, status will be a null pointer.  The valid bits that may be
 *     combined to form setStatusFlags and markFlags are defined by the
 *     constants using the prefix EPIF_SFLAGS_ in file.h.
 *
 *     The caller may also wish to update the file data version number.  This
 *     can be specified explicitly, by setting EPIF_SFLAGS_DVERSION in
 *     markFlags.  It is also specified implicitly when EPIF_SFLAGS_MTIME is
 *     set in markFlags.  It is NOT specified implicitly when EPIF_SFLAGS_MTIME
 *     is set in setStatusFlags.
 *
 *     If any flag in markFlags is set, the fileset version number is updated.
 *     SPECIAL HACK used only by test_anode:
 *	  If setStatusFlags and markFlags are both zero and status is null,
 *	  it is assumed that macro epif_Mark was invoked with flags set to
 *	  zero, with the intention of solely updating the fileset version
 *	  number.
 *
 * PARAMETERS --
 *     trans -- transaction to use for the operation.
 *     f -- file's open handle.
 *     markFlags -- mask used to select timestamp fields in the status.
 *     setStatusFlags -- mask used to select fields in the status.
 *     status -- file's new status.
 *
 * PERFORMANCE -- See the SET_FIELDS macro below.
 */

/* EXPORT */
long epif_SetStatusAndMark(
  IN elbb_tranRec_t trans,
  IN epi_anode_t f,
  IN long markFlags,
  IN long setStatusFlags,		/* which fields to set */
  IN struct epif_status *status)
{
    long code = 0;			/* in case of no action */
    struct fileHandle *h;
    epi_volume_t vol;
    struct timeval t;
    long first, last;			/* offsets bracketing modifications */

    /* SET_FIELD -- does most of the work in modifying a particular field in
     *     the status area.  It maintains a low and high water mark so all
     *     updates can be merged into a single call to elbb_Modify.  It may be
     *     that it would be more efficient to do separate Modifies if two
     *     widely separated fields are being updated.
     *
     *     A code block corresponding to each field is maintained by passing
     *     the block to the SET_FIELD macro. */

#define SET_FIELD(b, f, v, n) if (setStatusFlags & (b)) { \
    int o = offsetof(struct diskStatus, f); \
    epix_EnterCodeBlock /* */ (n); \
    if (o < first) first = o; \
    if ((o += sizeof(h->status.f)) > last) last = o; \
    h->status.f = (v); }

    epix_EnterCodeBlock(44);
    if (!GetOKfile (h, f)) return EPI_E_BADFILE;
    first = sizeof(struct diskStatus);
    last = 0;

    /* SPECIAL HACK -- see comments above.  Short circuit no-op. */
    if (setStatusFlags == 0 && markFlags == 0) {
	if (!status)
	    markFlags = EPIF_SFLAGS_VVERSION;
	else
	    return 0;
    }

    if (markFlags) {
	/* This code depends on the fields being in this order:
	 *   volumeVersionNumber, dataVersionNumber, mTime, cTime, aTime. */

	if (markFlags & EPIF_SFLAGS_ATIME)
	    last = offsetof(struct diskStatus, aTime) + sizeof(t);
	else if (markFlags & EPIF_SFLAGS_CTIME)
	    last = offsetof(struct diskStatus, cTime) + sizeof(t);
	else if (markFlags & EPIF_SFLAGS_MTIME)
	    last = offsetof(struct diskStatus, mTime) + sizeof(t);
	else if (markFlags & EPIF_SFLAGS_DVERSION)
	    last = offsetof(struct diskStatus, dataVersionNumber)
			+ sizeof(h->status.dataVersionNumber);
	else if (markFlags & EPIF_SFLAGS_VVERSION)
	    last = offsetof(struct diskStatus, volumeVersionNumber)
			+ sizeof(h->status.volumeVersionNumber);
	else return (EPI_E_BADFLAGS);
	first = offsetof(struct diskStatus, volumeVersionNumber);
    }
    BEGIN_TLOCK_WRITE (trans, &h->lock) {
	struct diskStatus oldStatus = h->status;
	afs_hyper_t vv;			/* volume version number */
	dfsh_diskHyper_t dvv;		/* disk VV */

	SET_FIELD (EPIF_SFLAGS_MODE, mode, status->mode, /* CodeBlock(*/46);
	SET_FIELD (EPIF_SFLAGS_UNIQUE,
		   unique, status->unique, /* CodeBlock(*/74);
	SET_FIELD (EPIF_SFLAGS_DVERSION, dataVersionNumber,
		   status->dataVersionNumber, /* CodeBlock(*/75);
	SET_FIELD (EPIF_SFLAGS_MTIME,
		   mTime, status->mTime, /* CodeBlock(*/48);
	SET_FIELD (EPIF_SFLAGS_CTIME,
		   cTime, status->cTime, /* CodeBlock(*/49);
	SET_FIELD (EPIF_SFLAGS_ATIME,
		   aTime, status->aTime, /* CodeBlock(*/50);
	SET_FIELD (EPIF_SFLAGS_OID, oid, status->oid, /* CodeBlock(*/52);
	SET_FIELD (EPIF_SFLAGS_GID, gid, status->gid, /* CodeBlock(*/53);
	SET_FIELD (EPIF_SFLAGS_UUID, uuid, status->uuid, /* CodeBlock(*/54);
	SET_FIELD (EPIF_SFLAGS_FLAGS, flags, status->flags, /* CodeBlock(*/47);
	SET_FIELD (EPIF_SFLAGS_CLIENTBITS,
		   clientBits, status->clientBits, /* CodeBlock(*/82);
	if (setStatusFlags & EPIF_SFLAGS_VVERSION) 
	    DFSH_DiskFromMemHyper(dvv, status->volumeVersionNumber);
	SET_FIELD (EPIF_SFLAGS_VVERSION, volumeVersionNumber,
		   dvv, /* CodeBlock(*/21);
#if 0
	SET_FIELD (EPIF_SFLAGS_LINK,	/* must use epif_ChangeLink */
		   linkCount, status->linkCount, /* CodeBlock(*/73);
	SET_FIELD (EPIF_SFLAGS_TYPE, type, status->type, /* CodeBlock(*/76);
	SET_FIELD (EPIF_SFLAGS_CID, cid, status->cid, /* CodeBlock(*/51);
	SET_FIELD (EPIF_SFLAGS_PARENT,
		   parent, status->parent, /* CodeBlock(*/55);
#endif

	if (markFlags) {
	    vol = epiv_GetVolume (f);
	    code = epiv_NewVolumeVersion (trans, vol, &vv);
	    if (code) continue;
	    DFSH_DiskFromMemHyper(h->status.volumeVersionNumber, vv);

	    epif_UniqueTime (&t);

	    if (markFlags & EPIF_SFLAGS_MTIME) {
		epix_EnterCodeBlock(57);
		h->status.mTime = t;
	    }
	    if (markFlags & EPIF_SFLAGS_CTIME) {
		epix_EnterCodeBlock(58);
		h->status.cTime = t;
	    }
	    if (markFlags & EPIF_SFLAGS_ATIME) {
		epix_EnterCodeBlock(59);
		h->status.aTime = t;
	    }
	    if (markFlags & (EPIF_SFLAGS_DVERSION | EPIF_SFLAGS_MTIME)) {
		epix_EnterCodeBlock(56);
		h->status.dataVersionNumber++;
	    }
	}

	if (first < last) {
	    code = epia_SetStatus (trans, f, first, last-first,
				   (char *)(&h->status) + first);
	    if (code) {
		/* restore handle status to consistent state */
		h->status = oldStatus;
		continue;
	    }
	}
    } END_LOCK;
    return code;
}

/* epif_Close -- This closes a file handle previously obtained from
 *     \fcn{epif_Open}.
 *
 *     All the actual work is done in Close. */

/* EXPORT */
long epif_Close(IN epi_anode_t fileC)
{
    long code;
    struct fileHandle *fileH;

    epix_EnterCodeBlock(60);
    if (!GetOKfile (fileH, fileC)) return EPI_E_BADFILE;
    code = Close (fileC, fileH);
    return code;
}

/* epif_AllowOpens -- This clears the JUST_CREATED file handle flag and allows
 *     other callers of \fcn{epif_Open} to succeed. */

/* EXPORT */
void epif_AllowOpens(IN epi_anode_t fileC)
{
    struct fileHandle *fileH;

    epix_EnterCodeBlock(83);
    GetOKfile (fileH, fileC);
    afsl_Assert (fileH && (fileH->flags & FILE_JUST_CREATED));
    fileH->flags &= ~FILE_JUST_CREATED;
}

/* epif_ChangeLink -- changes the link count of a file.  If the file is not
 *     also being deleted, the list of zero link count files will be updated if
 *     the linkCount changes to or from zero.
 *
 * PARAMETERS -- The amount to change the link count by is specified in the
 *     delta paramter, which may be positive, negative or zero.
 *
 *     If delete parameter is true, the file is also closed.  If the link count
 *     reaches zero the file will be deleted.
 *
 *     If newlinkP is non-zero the new link count is returned in that
 *     parameter.
 *
 * LOCKS USED -- The file's lock is held while the link count is being changed.
 *     If delete is true Delete is called after the lock is dropped since the
 *     caller may no longer use the file's handle.
 *
 * CAUTIONS -- Because of interactions with \fcn{epif_Open} and because
 *     significant portions of a delete operation (e.g. truncation) are not
 *     protected by the file's lock, the caller must ensure that multiple
 *     threads do not attempt to delete a file simultaneously with other
 *     operations on the same file. */

/* EXPORT */
long epif_ChangeLink(
  IN elbb_tranRec_t trans,
  IN epi_anode_t f,
  IN int delta,
  IN int delete,
  OUT u_int *newLinkP)
{
    long code = 0;
    struct fileHandle *h;
    u_int16 link;

    epix_EnterCodeBlock(61);
    if (!GetOKfile(h, f)) return EPI_E_BADFILE;

    /* Print suspect link count as we do not have filehandle locked */
    icl_Trace4(epi_iclSet, TR_EPIF_CHANGELINK,
	       ICL_TYPE_POINTER, f,
	       ICL_TYPE_LONG, delta,
	       ICL_TYPE_LONG, delete,
	       ICL_TYPE_LONG, h->status.linkCount);

    if (delete) {
	/* Delete must be single threaded */
	afsl_Assert((h->refCount == 1) && (elbb_IsZeroTrans(trans)));
    } else {
	afsl_Assert(!elbb_IsZeroTrans(trans));	
    }

    BEGIN_TLOCK_WRITE(trans, &h->lock) {
	if (epia_GetCopies(f) != 0) {
	    code = EPI_E_OUTSTANDINGCOPIES;
	    continue;
	}
	link = h->status.linkCount;
	if (delta) {
	    if (delta < 0)
		afsl_Assert(link >= (unsigned)-delta);
	    link += delta;
	}

	if (delete && link != 0) {
	    if (newLinkP) 
		*newLinkP = link;
	    code = EPI_E_HASLINKS;
	    continue;
	}

	if (delta && !delete) {
	    epix_EnterCodeBlock(62);
	    code = epia_SetStatus(trans, f,
				  offsetof(struct diskStatus, linkCount),
				  sizeof(h->status.linkCount),
				  (char *)(&link));
	    /* On error, restore handle status to consistent state */
	    if (code) 
		continue;
	    h->status.linkCount = link;

	    if (link == delta) {
		/* File's link count being increased FROM zero.  Undeleting a
		 * file???  At least remove from zeros list. */
		epix_EnterCodeBlock(25);
		afsl_Assert(!(h->flags & FILE_BEING_DELETED));
		code = Inactive (trans,
				 epiv_GetVolume(f), epiv_GetAnodeIndex(f));
		afsl_MBZ(code);
	    }

	    if (link == 0) {
		/* Add to zeros list if it just became zero. */
		epix_EnterCodeBlock(39);
		code = Unlink (trans, f);
		afsl_MBZ(code);
	    }
	}
    } END_LOCK;

    if (code) return code;

    /* 
     * Truncate and delete file.  Using transaction created after the
     * file was truncated, remove the file from the zero link count
     * list, if the file already has a zero link count. If the file
     * does not have a zero link count, there is not need to reduce
     * the link count to zero before deleting it.
     */
    if (delete) { 	
	elbb_tranRec_t deltrans;
	elbb_tranRec_t inactiveTrans;
	epi_volume_t vol = epiv_GetVolume(f); 
	u_long index = epiv_GetAnodeIndex (f);
	afs_hyper_t length;

	afsl_Assert(link == 0);
	/* 
	 * Should be safe since this is the only reference and no one
	 * else should get another.
	 */
	h->flags |= FILE_BEING_DELETED;

	AFS_hzero(length);
	code = epia_Truncate(elbb_zeroTrans, f, 0, length);
	afsl_MBZ(code);

	code = epia_StartTran("Delete", vol, &deltrans);
	afsl_MBZ(code);
	inactiveTrans = deltrans;	/* remember this for assert below */
	if (h->status.linkCount == 0) {
	    code = Inactive (deltrans, vol, index);
	    afsl_MBZ(code);
	}
	code = Delete (f, h, &deltrans);
	if (code) return code;
	Reschedule();
	afsl_Assert (deltrans == inactiveTrans);
	code = epia_EndTran (vol, deltrans);
	afsl_MBZ(code);
    }
    if (newLinkP) *newLinkP = link;
    return 0;
}

/* 
 * epif_Delete -- Delete a file.  Typically this interface should be used
 * to delete files with zero link count. If the file does not have a zero 
 * link count, then the flags should specify EPIF_DELETE_IGNORE_LINKCNT
 * else the function will fai with EPI_E_HASLINKS. The only out parameter 
 * newLinkP is only good to look at if the return code is EPI_E_HASLINKS.
 */
/* EXPORT */
long epif_Delete(
  IN epi_anode_t f,
  IN int flags,
  OUT u_int* newLinkP)
{
    long code = 0, code2 = 0;
    fileHandle_t *h;
    elbb_tranRec_t trans;
    afs_hyper_t length;
    epi_volume_t vol;

    if (!GetOKfile (h, f)) 
	return EPI_E_BADFILE;    

    afsl_Assert(h->refCount == 1);
    /* Safe to examine linkcount without any lock as the refcount is 1 */
    if ((h->status.linkCount != 0) && !(flags & EPIF_DELETE_IGNORE_LINKCNT)) {
	if (newLinkP)
	    *newLinkP = h->status.linkCount;
	return EPI_E_HASLINKS;
    }
    vol = epiv_GetVolume(f);

    /* Should be safe since this is the only reference */
    h->flags |= FILE_BEING_DELETED;

    /* Truncate file to zero length */
    AFS_hzero(length);
    code = epia_Truncate(elbb_zeroTrans, f, 0 /* flags */, length);
    afsl_MBZ(code);

    code = epia_StartTran("epif_Delete", vol, &trans);
    afsl_MBZ(code);

    if (h->status.linkCount == 0) {
	u_long index = epiv_GetAnodeIndex (f);

	code = Inactive(trans, vol, index);
	afsl_MBZ(code);
    }

    code = Delete(f, h, &trans);

    code2 = epia_EndTran(vol, trans);
    afsl_MBZ(code2);

    return code;
}

/* InappropriateAux -- is a macro which detects attempts to set initial dir or
 *     file acls on a file that isn't a dir.
 *
 *     Note that it uses the LOGGED bit to identify directories. */

#define InappropriateAux(file, whichAux) \
    (/* an acl */   !(whichAux & EPIF_AUX_TYPE_PLIST) && \
     /* a diracl */ (whichAux & EPIF_AUX_TYPE_DIRACL) && \
     /* not a dir */!(epia_GetFlags(file) & EPIA_CFLAGS_LOGGED))

/* epif_ReadAux -- Read \arg{length} bytes of a file's auxiliary container
 *     specified by \arg{whichAux}, starting at \arg{offset}, into the provided
 *     \arg{buffer}.
 *
 * PARAMETERS --
 *     file -- file handle whose auxiliary container is to be read.
 *     whichAux -- a flag, one of the following values, specified which of the
 *	 \arg{file}'s auxiliary containers is to be read:
 *	 EPIF_AUX_IS_ACL -- chooses the ACL.
 *	 EPIF_AUX_IS_INITIAL_DIR_ACL -- chooses the initial directory ACL.
 *	 EPIF_AUX_IS_INITIAL_FILE_ACL -- chooses the initial file ACL.
 *	 EPIF_AUX_IS_PLIST -- chooses the property list.
 *	 EPIF_AUX_IS_USER_PLIST -- chooses the user defined property list.
 *     offset -- position in auxiliary container (in bytes) from which to start
 *	 reading.
 *     length -- number of bytes to read.
 *     buffer -- buffer into which the data should be copied.  The buffer is
 *	 assumed to be large enough to hold the requested \arg{length}.
 *	 The \arg{buffer} must be in the kernel's address space.
 *
 * LOCKS USED -- The file is read locked and the aux tlock is held while
 *     reading the auxiliary container. */

/* EXPORT */
long epif_ReadAux(
  IN epi_anode_t file,
  IN int whichAux,
  IN u_long offset,
  IN u_long length,
  OUT char *buffer)
{
    long code;
    struct fileHandle *h;

    epix_EnterCodeBlock(64);
    if (auxIndex[whichAux] < 0) return EPI_E_BADARGS;
    if (InappropriateAux (file, whichAux)) return EPI_E_DIRONLYAUX;

    BEGIN_CRIT (code = ReadLockWithAux (file, &h),
		tlock_ReleaseRead (&h->lock)) {
	epi_anode_t aux;
	aux = h->aux[auxIndex[whichAux]];
	if (!aux) {			/* no container, just return "" */
	    epix_EnterCodeBlock(65);
	    if (offset) { ABORT_CRIT (EPI_E_EOF) }
/* Wouldn't this make more sense? -ota 910322
	    if (length > 0) { ABORT_CRIT (EPI_E_READSHORT) }
 */
	    if (length == 0) continue;
	    buffer[0] = 0;
	    continue;
	}
	BEGIN_TLOCK_READ (epiv_GetAuxTLock(epiv_GetVolume(aux))) {
	    struct uio uio;
	    struct iovec iov;
	    epia_InitUio (&uio, &iov, offset, length, buffer);
	    code = epia_Read (aux, 0, &uio);
	    if (code) continue;
	} END_LOCK;
    } END_CRIT;
    return code;
}

/* epif_WriteAux -- Writes the contents of the file's ACL or property list as
 *     selected by the low four bits of the \arg{flags} parameter.  This
 *     will set the auxiliary container's length to \arg{offset} plus
 *     \arg{length} if this is greater than its current length or if the
 *     \cst{EPIF_FLAGS_TRUNCATE_AUX} bit is set.  If the auxiliary
 *     container is being shared and \cst{EPIF_FLAGS_NEW_AUX} is set then
 *     a new auxiliary container is created.  If the length and offset are both
 *     zero then the specified auxiliary container (if any) is removed.
 *
 * PARAMETERS --
 *     trans -- transaction to use for the operation.
 *     file -- file handle whose auxiliary information is to be over-written.
 *     flags -- bits that control the write operation.  The value is the
 *	 logical OR of a type values and any of the following bits:
 *	   EPIF_FLAGS_NEW_AUX -- specifies that a new container should be
 *		created, breaking all existing sharing. No support exists for
 *		operations where this bit is {\em not} set.
 *	   EPIF_FLAGS_TRUNCATE_AUX -- specifies that the container should be
 *		truncated to the length of this write operation.
 *	 Legal type values are the same as for the \arg{whichAux} parameter
 *	 to \fcn{epif_ReadAux}
 *     offset -- position in the auxiliary container (in bytes) at which to
 *	 write.
 *     length -- number of bytes to write.
 *     buffer -- pointer to the data to be written.  The \arg{buffer} must
 *	 be in the kernel's address space.
 *
 * LOCKS USED -- The file is write locked for the duration.  The aux tlock is
 *     held for read while get the container's link count.
 *
 * CAUTIONS -- When NEW_AUX is set we do not call CopyAux in cases where we are
 *     not overwriting the whole container.  We probably should.
 */

/* EXPORT */
long epif_WriteAux(
  IN elbb_tranRec_t trans,
  IN epi_anode_t file,
  IN long flags,
  IN u_long offset,
  IN u_long length,
  IN char *buffer)
{
    long code;
    struct uio uio;
    struct iovec iov;
    struct fileHandle *h;
    int whichAux;
    afs_hyper_t newLength;
    epi_volume_t vol;

    epix_EnterCodeBlock(66);
    if (!GetOKfile (h, file)) return EPI_E_BADFILE;

    AFS_hset64 (newLength, 0, offset);
    AFS_hadd32 (newLength, length);
    if (!AFS_hfitsinu32(newLength)) return (EPI_E_TOOBIG);

    whichAux = flags & 0xf;
    if (auxIndex[whichAux] < 0) return EPI_E_BADFLAGS;
    if (InappropriateAux (file, whichAux)) return EPI_E_DIRONLYAUX;
    vol = epiv_GetVolume(file);

    BEGIN_TLOCK_WRITE (trans, &h->lock) {
	epi_anode_t aux;
	u_long index;

	code = OpenAux (file, h);
	if (code) continue;
	aux = h->aux[auxIndex[whichAux]];
	index = AUX_FROM_STATUS(&h->status, whichAux);

	if (index == 0) {
	    if (AFS_hiszero(newLength)) {
		epix_EnterCodeBlock(79);
		code = 0;
		continue;		/* do nothing: already no aux */
	    }
	    epix_EnterCodeBlock(67);
	    BEGIN_CRIT (code = NewAux (trans, vol, whichAux, &index, &aux),
			ON_ERROR (code = DecAux (trans, aux, 0))) {
		code = LinkAux (trans, file, h, whichAux, index, aux);
	    } END_CRIT;
	    if (code) continue;
	} else {
	    struct diskAux disk;
	    u_int size;
	    if (AFS_hiszero(newLength)) {
		/* Disconnect exsiting aux. */
		epix_EnterCodeBlock(80);
		code = UnlinkAux (file, h, whichAux, /*!discard*/0, &trans);
		continue;
	    }
	    epix_EnterCodeBlock(68);
	    BEGIN_TLOCK_READ (epiv_GetAuxTLock(epiv_GetVolume(aux))) {
		code = epia_GetStatus (aux, sizeof(disk),
					(char *)&disk, &size);
		if (code) continue;
		afsl_Assert(size == sizeof (disk));
		afsl_Assert(disk.linkCount > 0);
	    } END_LOCK;
	    /* Since we have the file write locked, then if the current count
	     * is 1 then it can't get any bigger and we have exclusive access.
	     * If its larger and NEW_AUX is set, call NewAux. */
	    if ((disk.linkCount > 1) && (flags & EPIF_FLAGS_NEW_AUX)) {
		epix_EnterCodeBlock(69);

		/* TODO -- maybe we should call CopyAux unless ((offset == 0)
		 *     && (flags & TRUNCATE_AUX)) is true. */
		BEGIN_CRIT (code = NewAux (trans, vol, whichAux, &index, &aux),
			    ON_ERROR (code = DecAux (trans, aux, 0))) {
		    code = LinkAux (trans, file, h, whichAux, index, aux);
		} END_CRIT;
		if (code) continue;
	    }
	}
	/* at this point we have the acl we want to work with */
	epia_InitUio (&uio, &iov, offset, length, buffer);
	code = epia_Write (trans, aux, 0, &uio);
	if (code) continue;
	if (flags & EPIF_FLAGS_TRUNCATE_AUX) {
	    epix_EnterCodeBlock(70);
	    code = epia_Truncate (trans, aux, 0, newLength);
	    if (code) continue;
	}
    } END_LOCK; /* file's lock */
    return code;
}

/* epif_ReplaceAux -- Changes one of \arg{file}'s auxiliary containers to
 *     be the same as one of the auxiliary containers of \arg{auxFile}.
 *     The previous value of the auxiliary container is discarded.  The types
 *     of both auxiliary containers must be compatible, namely, both ACLs or
 *     both PLists.  This is implemented by increasing the sharing of the
 *     desired auxiliary container unless the \arg{file} and
 *     \arg{auxFile} are in separate filesets in which case the contents are
 *     copied.
 *
 * PARAMETERS --
 *     trans -- transaction to use for the operation.
 *     file -- file handle whose auxiliary container's information is to be
 *	 replaced.
 *     flags -- selects the auxiliary container type for both the source and
 *	 destination files.  The types are as specified for
 *	 \fcn{epif_ReadAux}.  The low four bits specifies which auxiliary
 *	 container of \arg{file} is to be replaced.  The next four bits
 *	 specify which auxiliary continer of \arg{auxFile} it is to come
 *	 from.
 *     auxFile -- file from which to obtain the auxiliary container.
 *
 * LOCKS USED -- The source auxFile is read locked, and then the destination
 *     file is write locked.  The two locks are never held simultaneously. */

/* EXPORT */
long epif_ReplaceAux(
  IN elbb_tranRec_t trans,
  IN epi_anode_t file,
  IN long flags,
  IN epi_anode_t auxFile)
{
    long code;
    struct fileHandle *h;
    int whichFile;
    int whichAux;
    epi_anode_t aux;
    u_long index;

    epix_EnterCodeBlock(71);
    if (!GetOKfile (h, file)) return EPI_E_BADFILE;

    whichFile = flags & 0xf;
    whichAux = (flags>>4) & 0xf;
    if (whichAux == 0)
	whichAux = whichFile;		/* for backward compatibility */
    if ((auxIndex[whichAux] < 0) || (auxIndex[whichFile] < 0))
	return EPI_E_BADFLAGS;
    if (InappropriateAux (file, whichFile) ||
	InappropriateAux (auxFile, whichAux)) return EPI_E_DIRONLYAUX;
    /* Check for immiscible types */
    if (((whichAux^whichFile) & EPIF_AUX_TYPE_PLIST) != 0)
	return EPI_E_BADAUXSOURCE;

    BEGIN_CRIT(index = 0,
	       if (index) {
		   long code2;
		   code2 = DecAux (trans, aux, 0);
		   afsl_MBZ(code2);
		}) {
	BEGIN_LCRIT(struct fileHandle *ah,
		    code = ReadLockWithAux (auxFile, &ah),
		    tlock_ReleaseRead (&ah->lock)) {
	    code = DupAux (trans, ah, whichAux, epiv_GetVolume(file),
			   &index, &aux);
	    if (code == EPI_E_BADAUXSOURCE)
		code = 0;		/* no aux: clear corresponding slot */
	} END_CRIT;

	BEGIN_TLOCK_WRITE (trans, &h->lock) {
	    code = OpenAux (file, h);
	    if (code) continue;
	    code = LinkAux (trans, file, h, whichFile, index, aux);
	    if (code) continue;
	    index=0;			/* don't delete aux after this point */
	} END_LOCK;
    } END_CRIT; /* un-duplicate auxilary container */
    return code;
}

#ifndef KERNEL

/* epif_Clone -- allows a file to be recloned or uncloned.
 *
 * CAUTIONS -- As with direct calls to epif_CreateE this interface to COW files
 *     is ONLY useful for testing.  The volume clone, reclone and unclone
 *     operations directly access the epiv_OpenBackingAnode and epia_Clone
 *     without going through the file module.  Care must be taken that all
 *     relevant containers be closed at the file level before these operations
 *     are invoked behind its back.  In addition, the fact that reclone
 *     overwrites the status area of the backing anode means that we need to be
 *     careful of aux containers especially.  Pragmatically, all files in a
 *     volume should be closed before doing any volume operations on it. */

/* EXPORT */
long epif_Clone(
  IN elbb_tranRec_t trans,
  IN epi_anode_t f,
  IN long flags)
{
    long code;
    struct fileHandle *h;
    int reclone = (flags & 1) != 0;

    epix_EnterCodeBlock(72);
    if (!GetOKfile (h, f)) return EPI_E_BADFILE;
    if (!(epia_GetFlags(f) & EPIA_CFLAGS_COPYONWRITE))
	return EPI_E_NOTCOPYONWRITE;

    BEGIN_LLCRIT (epi_volume_t volB,
		  epi_anode_t b,
		  code = epiv_OpenBackingAnode (f, &volB, &b),
		  (code = epiv_CloseAnode (b),
		   code = epiv_Close(elbb_zeroTrans, volB))) {
	/* "b" is a handle for the backing anode with appropriate quota
	 * function attached.  This is not a "file" handle. */

	if (reclone) {

	    /* CAUTIONS -- Since the backing file's aux containers will be
	     *     trashed if this is a reclone, we require that it not have
	     *     any and that the COW file have none as well.  The real
	     *     volops handle this by always doing whole volume clones.  For
	     *     testing this will have to do. */

	    struct diskStatus status;	/* disk info */
	    unsigned int ss;		/* size of status area */
	    int t;

	    code = epia_GetStatus (b, sizeof(status), (char *)&status, &ss);
	    afsl_Assert (!code && (ss == sizeof(status)));
	    for (t=0; t<EPIF_AUX_NTYPES; t++) {
		int i = auxIndex[t];
		u_long bindex, cindex;
		if (i < 0) continue;
		cindex = AUX_FROM_STATUS (&h->status, t);
		bindex = AUX_FROM_STATUS (&status, t);
		afsl_PAssert (cindex == 0,
			      ("Reclone COW file has aux of type %d", t));
		afsl_PAssert (bindex == 0,
			      ("Reclone backing file has aux of type %d", t));
	    }
	}

	code = epia_Clone (trans, f, b, flags);

    } END_CRIT;
    return code;
}
#endif

#ifndef	KERNEL
#include "verify.h"
#include "salvage.h"

/* epiz_LegalFileStatusLength -- is used by the salvager to validate anode
 *     status lengths that it expects to be managed by the file module.
 *
 * RETURN CODES -- It returns some of the SS_*CNT values from salvage.h.  All
 *     auxilary containers are returned as ACLCNT (since they're all the same
 *     size), and files and directories and symlinks are returned as FILECNT
 *     (for the same reason). */

/* SHARED */
int epiz_LegalFileStatusLength (len)
  IN int len;
{
    if (len == sizeof (struct diskAux)) return EPIZ_SS_ACLCNT;
    if (len == sizeof(struct diskStatus)) return EPIZ_SS_FILECNT;
    return 0;
}

/* SHARED */
long epiz_SalvageLinkCount( hP, cnt )
  IN epi_anode_t hP;
  IN long cnt;
{
    int code;
    struct fileHandle * f;
    int oldLink;
    if (!GetOKfile (f, hP))
	return EPI_E_BADFILE;

    BEGIN_TLOCK_WRITE_NT (&f->lock) {
	oldLink = f->status.linkCount;
	f->status.linkCount = cnt;
	code = epia_SetStatus
	    (elbb_zeroTrans, hP, offsetof (struct diskStatus, linkCount),
	     sizeof (f->status.linkCount), (char *)&f->status.linkCount);
	if (code) {
	    /* restore handle status to consistent state */
	    f->status.linkCount = oldLink;
	    continue;
	}
    } END_LOCK;
    return (code);
}

/*
** epiz_VerifyFile
** Test some of the bits in the status structure of a file
** to make sure epif_open's will suceed when the salvager
** attempts the directory scan.
*/

/* UNUSED -ota 930421 */
/* SHARED */
long epiz_VerifyFile(
  IN epi_anode_t vol,
  IN epi_volFileId_t *fid,
  IN struct epiz_salvageState *ss)
{
    long code;
    epi_anode_t fileC;
    u_long index = epid_ToEPIVIndex (fid, epia_AnodeDevice(vol));

    afsl_Assert(index != 0);
    code = epiv_OpenAnode (vol, index, &fileC);
    if (code == 0) {
	/*
	** Currently, GetHandle provides enough error conditions
	** to correctly abort epif_Open (without panicing).
	** If we need more, this is the place.
	*/
	code = epiv_CloseAnode (fileC);
	afsl_MBZ(code);
    }
}

/* SHARED */
long epiz_SalvageAux (volC, index, newLinkCount)
  IN epi_volume_t volC;
  IN u_long index;
  IN u_long newLinkCount;
{
    long code;

    epix_EnterCodeBlock(81);
    BEGIN_LCRIT (epi_anode_t aux,
		 code = epiv_OpenAnode (volC, index, &aux),
		 code = epiv_CloseAnode (aux)) {
	u_int size;			/* size of aux status area */
	struct diskAux disk;
	code = epia_GetStatus (aux, sizeof(disk), (char *)&disk, &size);
	if (code) continue;
	if (size != sizeof(disk)) { ABORT_CRIT (EPI_E_NOTANACL) }
	disk.linkCount = newLinkCount;
	code = epia_SetStatus (elbb_zeroTrans, aux, 0, size, (char *)&disk);
	if (code) continue;
    } END_CRIT;
    return code;
}

static long VerifyAux(
  epi_anode_t aP,
  struct fileHandle *hP,
  struct epiz_salvageState * ss,
  int which,				/* aux type to check */
  epiz_AuxVerify_t aclVerify,
  epiz_AuxVerify_t plistVerify,
  opaque rock)
{
    long code = 0;
    int i = auxIndex[which];		/* aux index */
    epi_anode_t aux;
    u_long index;			/* epiv-style index */
    struct diskAux disk;		/* for checking aux status */
    u_int size;
    /* acl/plist specific things */
    char oldType;			/* only verify/count each aux once */
    int cnt;				/* type of link count reference */
    epiz_AuxVerify_t verify;

    index = AUX_FROM_STATUS (&hP->status, which);
    if (index == 0)
	return 0;			/* none */
    ss->auxIx = epiz_ToEPIAIndex(index);
    ss->auxWhich = which;

    if (which & EPIF_AUX_TYPE_PLIST) {
	epix_EnterCodeBlock(26);
	cnt = EPIZ_SS_PLISTCNT;
	verify = plistVerify;
    } else {
	epix_EnterCodeBlock(27);
	cnt = EPIZ_SS_ACLCNT;
	verify = aclVerify;
    }

    {    extern int epit_useOldFidIndex;
	 if (!epit_useOldFidIndex && epid_IndexForFid (index, ss->device)) {
	     epiz_ReportError (ss, 0, EPIZ_E_AUX_BOGUSINDEX);
	     goto bad_ref;
	 }
     }

    /* do OpenAux thing, but a bit more carefully */
    if (hP->aux[i]) {
	epix_EnterCodeBlock(31);
	afsl_PAssert (ss->auxIx == epia_GetIndex(hP->aux[i]),
		      ("open handle for wrong or non-existent aux container"));
    } else {
	epi_volume_t vol = epiv_GetVolume (aP);
	code = epiv_OpenAnode (vol, index, &hP->aux[i]);
	if (code) {
	    hP->aux[i] = 0;
	    epiz_ReportError (ss, code, EPIZ_E_AUX_INVALIDINDEX);
	    goto bad_ref;
	}
    }
    ss->aux = aux = hP->aux[i];
    afsl_Assert (ss->auxIx <= ss->lastIndex);

    if (which & EPIF_AUX_TYPE_PLIST) {
	if (!(epia_GetFlags(aux) & EPIA_CFLAGS_LOGGED) ==
	    (which == EPIF_AUX_IS_PLIST)) {
	    /* Plist is not logged when it should be or visa versa. */
	    epiz_ReportError (ss, 0, EPIZ_E_AUX_PLISTBADLOGGED);
	    goto bad_ref;
	}
    } else {
	if (!(epia_GetFlags(aux) & EPIA_CFLAGS_LOGGED)) {
	    /* ACL container isn't logged when it should be. */
	    epiz_ReportError (ss, 0, EPIZ_E_AUX_ACLNOTLOGGED);
	    goto bad_aux;
	}
    }
    code = epia_GetStatus (aux, sizeof(disk), (char *)&disk, &size);
    if ((code != 0) || (size != sizeof(disk))) {
	epiz_ReportError (ss, code, EPIZ_E_AUX_NOT);
	goto bad_aux;
    }
    if ((((which^disk.flags) & EPIF_AUX_TYPE_PLIST) != 0)
	|| InappropriateAux(aP, which)
	|| ((ss->typeLinkCounts[ss->auxIx] != EPIZ_SS_NOCNT)
	    && (ss->typeLinkCounts[ss->auxIx] != cnt)) ) {
	epiz_ReportError (ss, code, EPIZ_E_AUX_BADTYPE);
	goto bad_ref;
    }
    if (disk.linkCount == 0) {		/* referenced aux should be gone! */
	epiz_ReportError (ss, 0, EPIZ_E_AUX_ZEROLINK);
	goto bad_ref;
    }
    oldType = ss->typeLinkCounts[ss->auxIx];
    if (verify && (oldType == 0)) {

	/* verify the aux contents on first reference. */

	u_long length;
	char *buffer;
	afs_hyper_t l;
	l = epia_GetLength (aux);
	/* user plists can be longer than this but just check first 8K */
	if (AFS_hfitsinu32(l)) {
	    length = AFS_hgetlo(l);
	    if (length > 8192) length = 8192;
	} else length = 8192;

	buffer = osi_Alloc_r( length );
	epif_ReadAux (aP, which, 0, length, buffer);
	code = (*verify)( buffer, length, ss, rock );
	osi_Free_r (buffer, length);
	if (code != 0) {
	    epiz_ReportError (ss, code, EPIZ_E_AUX_INVALID);
	    goto bad_aux;
	}
    }
    ss->fileLinkCounts[ss->auxIx] = disk.linkCount;
    ss->typeLinkCounts[ss->auxIx] = cnt;
    ss->linkCounts[ss->auxIx]++;
    if (!(ss->flags & EPIZ_SS_GETPATHNAMES) && (oldType == 0))
	ss->nAuxs++;
    return 0;

bad_aux:
    ss->typeLinkCounts[ss->auxIx] == EPIZ_SS_DELETECNT;

bad_ref:
    /*
     * Some problem so see if we can remove the reference.
     */
    if (ss->flags & EPIZ_SS_SALVAGE) {
	if (ss->flags & EPIZ_SS_BACKINGVOLUME) {

	    /* If this is a backing anode, then we're going to bail out because
	     * we cannot modify the file. */

	    epiz_ReportError (ss, 0, EPIZ_E_AUX_BADBACKING);
	} else {

	    /* remove reference and delete aux container. */

	    int auxOff = auxOffset[which];
	    if (hP->aux[i]) {
		code = epiv_CloseAnode (hP->aux[i]);
		afsl_MBZ(code);
	    }
	    hP->aux[i] = 0;
	    AUX_FROM_STATUS (&hP->status, which) = 0;
	    code = epia_SetStatus ( elbb_zeroTrans, aP, auxOff,
				   sizeof(u_int32),
				   (char *)(&hP->status) + auxOff);
	    afsl_MBZ(code);
	}
    }
    return 0;
}

/*
 * epiz_VerifyFileAux -- Perform any verification associated with files.  This
 *     includes opening any acl's and plists', recording information about
 *     their type and link counts.
 *
 * RETURN CODES -- returns -1 if the file is damaged beyond repair.  The reason
 *     is reported; the caller should arrange to delete the anode. */

/* SHARED */
long epiz_VerifyFileAux(
  epi_anode_t aP,
  long flags,			/* is it a directory? */
  struct epiz_salvageState *ss,
  epiz_AuxVerify_t aclVerify,
  epiz_AuxVerify_t plistVerify,
  opaque rock)
{
    long code;
    struct fileHandle * hP;
    afs_hyper_t vv;
    afs_hyper_t vd;			/* old VV location in anode volData */
    int t;				/* aux type */

    /*
     ** We need to set up the link count stuff,
     ** check the flags on the file, and verify
     ** some small fields in the file.
     */
    {   int ok = GetOKfile(hP, aP);
	afsl_Assert (ok);
    }
    /*
     ** Do the fields testing,
     ** Time check, flags check.
     */

    /* If the file doesn't have correct logged-ness have it blown away. */

    if (epia_GetFlags(aP) & EPIA_CFLAGS_LOGGED) {
	if (!(flags & EPIF_CFLAGS_DIRECTORY)) {
	    /* logged but shouldn't be */
	    epiz_ReportError (ss, 0, EPIZ_E_FILELOGGED);
	    return -1;
	}
	if (!AFS_hfitsinu32(epia_GetLength(aP))) {
	    epiz_ReportError (ss, 0, EPIZ_E_DIRTOOLONG,
			      epiz_Printed(epiz_PrintedHyper,
					   epia_GetLength(aP)));
	    return -1;
	}
    } else { /* not logged */
	if (flags & EPIF_CFLAGS_DIRECTORY) {
	    /* Directory isn't logged but should be. */
	    epiz_ReportError (ss, 0, EPIZ_E_DIRNOTLOGGED);
	    return -1;
	}
    }

    /* Test the file's VV against the volume's, and save the highest we see. */

    /* Get VV from "best" place to provide compatibility with ancient disk
     * formats. */
    DFSH_MemFromDiskHyper (vv, hP->status.volumeVersionNumber);
    AFS_hset64(vd, epia_GetVolDataHi(aP), epia_GetVolDataLo(aP));
    if (AFS_hcmp (vv, vd) < 0)
	vv = vd;

    if (AFS_hcmp(ssVolInfo(ss)->version, vv) < 0) {
	epiz_ReportError (ss, 0, EPIZ_E_2LARGEVOLVERSION,
			  epiz_Printed(epiz_PrintedHyper, vv));
	if (AFS_hcmp(vv, ss->highAnode) > 0)
	    ss->highAnode = vv;
    }

    /*
     * Check each type of auxiliary container
     */
    ss->auxIx = 0;
    ss->aux = 0;
    for (t=0; t<EPIF_AUX_NTYPES; t++) {
	long ccode;
	if (auxIndex[t] < 0) continue;
	ccode = VerifyAux (aP, hP, ss, t, aclVerify, plistVerify, rock);
	afsl_MBZ(ccode);
    }
    ss->auxWhich = 0;
    ss->auxIx = 0;
    ss->aux = 0;
    return 0;
}

/*
** epiz_InitZeroLinks.
** see epiz_AllocZeroLink.
*/

static struct zeroLinks epiz_zeroLinks;

/* SHARED */
long epiz_InitZeroLinks(void)
{
    bzero((caddr_t)&epiz_zeroLinks, sizeof (epiz_zeroLinks) );
    epiz_zeroLinks.nFiles = NO_COUNT;
    return(0);
}

/*
** epiz_AllocZeroLink
** This creates an entry to describe the zero-link-count list.
** The idea is to recreate the list from scratch, then compare
** it to the list we have in place.  If they match, the
** verification is complete, if they don't match, we can
** replace the contents (if we are salvaging).
** The only problem is keeping one static copy of the struct zeroLinks
** around, and clearing it...
*/

/* SHARED */
long epiz_AllocZeroLink(u_int32 index, struct epiz_salvageState * ss)
{
    /* note that ss->anodeIx and index differ by the usual epiv correction */
    (void) epiz_AllocAnodeItem (&ss->zeroLinkCnt, ss->anodeIx, ss->volIx);

    if (epiz_zeroLinks.nFiles == NO_COUNT) {
	/* attempt to jam the entry into the structure */
	int i;
	for (i=0; i<N_ZERO_LINKS; ++i) {
	    if (epiz_zeroLinks.files[i] == 0) {
		epiz_zeroLinks.files[i] = index;
		return(0);
	    }
	}
	/* If it didn't fit, we have to change states.  Set nFiles to full
	 * sizeof the array. */
	epiz_zeroLinks.nFiles = N_ZERO_LINKS;
    }
    /* just bump the count */
    epiz_zeroLinks.nFiles++;
    return(0);
}

/*
 * VerifyZeroLink -- Compare the passed zeros entry to the one we have built.
 *
 * RETURN CODES -- return 1 for 'not equal', and 0 for equal, and -1 for mostly
 *     equal (though since we are paranoid, we may just replace the contents
 *     anyway).
 */

static long VerifyZeroLink(
  IN struct zeroLinks *zeros,
  IN struct epiz_salvageState * ss)
{
    int i;
    int diskNfiles;			/* actual counts from zeros list */
    int goodNfiles;

    goodNfiles = diskNfiles = 0;
    for (i=0; i<N_ZERO_LINKS; i++) {
	if (zeros->files[i]) diskNfiles++;
	if (epiz_zeroLinks.files[i]) goodNfiles++;
    }
    goodNfiles = (epiz_zeroLinks.nFiles == NO_COUNT
		  ? goodNfiles : epiz_zeroLinks.nFiles);
    afsl_Assert (goodNfiles <= epiz_zeroLinks.nFiles);

    /* check counts for consistency and correctness */
    if (zeros->nFiles < diskNfiles) {
	printf( "Verify Zero Link Count List: Volume %s file count inconsistent: claims %d but at least %d found, should be %d.\n",
		ssVolInfo(ss)->ident.name,
		zeros->nFiles, diskNfiles, goodNfiles );
	return(1);			/* not a match */
    }
    diskNfiles = (zeros->nFiles == NO_COUNT ? diskNfiles : zeros->nFiles);
    if (diskNfiles != goodNfiles) {
	printf( "Verify Zero Link Count List: Volume %s file counts didn't match: claims %d but should be %d.\n",
		ssVolInfo(ss)->ident.name, diskNfiles, goodNfiles );
	return(1);			/* not a match */
    }

    if ((zeros->nFiles == NO_COUNT) && (epiz_zeroLinks.nFiles == NO_COUNT)) {
	/* Both lists are complete so we have to find each entry in zeros in
	 * epiz_VerifyZeroLink */
	for (i=0; i<N_ZERO_LINKS; ++i) {
	    if (zeros->files[i]) {
		/* look for zeros->files[i] in epiz_VerifyZeroLink[j] */
		int j;
		for (j=0; j<N_ZERO_LINKS; ++j) {
		    if (zeros->files[i] == epiz_zeroLinks.files[j]) {
			/* match. */
			break;
		    }
		}
		if (j == N_ZERO_LINKS) {
		    /* didn't find it. No match */
		    printf( "Verify Zero Link Count List: Volume %s entry %d ",
			   ssVolInfo(ss)->ident.name, i );
		    printf( " index %d wan't found in recomputed list\n",
			   zeros->files[i] );
		    return(1);
		}
	    }
	}
	return(0); /* This is a real match. */
    }
    /*
     * We can't compare the static epiz_zeroLinks against zeros, but we can
     * find out if the indexes that are mentioned should be on the list.  See
     * if the typeLinkCounts and linkCounts arrays agree about all the files in
     * the list.
     */
    for (i=0; i<N_ZERO_LINKS; ++i) {
	int index = zeros->files[i];
	if ( index == 0 ) continue;
	index = epiz_ToEPIAIndex(index);

	if ( ss->typeLinkCounts[index] != EPIZ_SS_FILECNT) {
	    printf( "Verify Zero Link Count List: Volume %s entry %d ",
		   ssVolInfo(ss)->ident.name, i );
	    printf( " anode %d is not a file.\n", index );
	    return(1);			/* bad zero link count list */
	}
	if ( ss->fileLinkCounts[index] != 0 ) {
	    printf( "Verify Zero Link Count List: Volume %s entry %d ",
		   ssVolInfo(ss)->ident.name, i );
	    printf( " anode %d has non zero link count.\n", index );
	    return(1);
	}
	if ( ss->linkCounts[index] != 0 ) {
	    printf( "Verify Zero Link Count List: Volume %s entry %d ",
		   ssVolInfo(ss)->ident.name, i );
	    printf( " anode %d has computed non zero link count.\n", index );
	    return(1);
	}
    }
    return(0); /* all list elements are valid! */
}

/*
 * epiz_SalvageZeroLink -- check and repair, if necessary, the ZLC list.
 *
 * RETURN CODES -- we could return EPIZ_E_BADBACKINGZLC if fileset was backing,
 *     indicating that the repair could not be made.  However, it seems
 *     reasonable to repair the ZLC even of a backing fileset.  So we don't do
 *     this here. */

/* SHARED */
long epiz_SalvageZeroLink(epi_anode_t volC, struct epiz_salvageState *ss)
{
    int code;
    struct zeroLinks * zeros;

    code = epiv_GetZLCAddr(volC, (int32 **)&zeros);
    if (code) return code;
    /*
     ** Did we get a match?
     */
    code = VerifyZeroLink( zeros, ss );
    if (code == 0) {
	/* if everything was OK then flush the ss->zeroLinkCnt list so that we
	 * don't report them as errors. */
	/* This is a GLOBAL LIST: only REMOVE files from THIS volume */
	struct epiz_anodeItem *item;
	struct epiz_anodeItem *next;
	struct epiz_anodeItem **nextP;
	nextP = &ss->zeroLinkCnt;
	for (item = ss->zeroLinkCnt; item; item = next) {
	    next = item->next;
	    if (item->anode->volIx == ss->volIx) {
		*nextP = next;
		item->anode->refCnt--;
		osi_Free_r (item, sizeof(*item));
	    } else {
		nextP = &item->next;
	    }
	}
	return 0;
    }

    epiz_ReportError (ss, 0, EPIZ_E_BADZLC);
    /*
     * replace the contents if we are salvaging.
     */
    if (ss->flags & EPIZ_SS_SALVAGE) {
	struct epiv_status s;		/* for passing to epiv_SetStatus */
	long mask;
	long max = (sizeof(epiz_zeroLinks)/sizeof(epiz_zeroLinks.files[0]));
	int i;

	bcopy((caddr_t)&epiz_zeroLinks, (caddr_t)s.fileStatus,
		sizeof (epiz_zeroLinks) );

	/*
	 * Replace all <max> words one by one... kinda brute force, but gentle.
	 */
	for (i = 0; i < max; ++i) {
	    mask = (i << EPIV_STATUS_FILESTATUS_N_SHIFT) |
		EPIV_STATUS_FILESTATUS_WORD_N;
	    code = epiv_SetStatus (elbb_zeroTrans, volC, mask, &s);
	    afsl_MBZ(code);
	}
	ss->flags |= EPIZ_SS_REPAIRED;
    }
    return(0);
}

#endif	/* KERNEL */
