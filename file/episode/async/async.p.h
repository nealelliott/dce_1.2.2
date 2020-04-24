/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: async.p.h,v $
 * Revision 1.1.62.1  1996/10/02  17:20:13  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:32  damon]
 *
 * Revision 1.1.56.2  1994/06/09  13:59:55  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:10  annie]
 * 
 * Revision 1.1.56.1  1994/02/04  20:12:36  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:55  devsrc]
 * 
 * Revision 1.1.54.1  1993/12/07  17:18:14  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:52:04  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1994, 1990 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/async/async.p.h,v 1.1.62.1 1996/10/02 17:20:13 damon Exp $ */

#ifndef TRANSARC_EPI_ASEVENT_H
#define TRANSARC_EPI_ASEVENT_H

#include <dcedfs/lock.h>
#include <dcedfs/episode/fifo.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_intr.h>
#include <dcedfs/episode/epi_thread.h>

/*
 * Event and other async I/O definitions.
 */

/*
 * Struct async_device contains parameters used in doing I/O to an
 * Episode aggregate.
 *
 * XXX The name is poorly chosen; of the information in this structure,
 * only the device number has anything to do with the underlying device.
 */
typedef struct async_device {
    struct async_device *next;		/* link */
    long majdev;			/* major device number */
    long mindev;			/* minor device number */
    long blkSize;			/* file system block size */
    int  logBlkSize;			/* log base 2 of blkSize */
    long fragSize;			/* size of fragment */
    int  logFragSize;			/*  log2(fragSize) */
    unsigned sleep;			/* used to freeze aggregate */
    struct fifo_Thread statusThread;	/* status */
} async_device_t;

typedef struct asevent_event {
    struct buf *buf;			/* system buffer descriptor */
    struct asevent_event *next;		/* next event in this list */
    struct asevent_event *hash;		/* next event on hash chain */
    struct asevent_set *set;		/* the set containing this event */
    short flags;			/* flags--see below  */
    short refCount;			/* reference count */
    long code;				/* code for I/O result */
    epi_mutex_t lock;			/* for event mgmt, not I/O */
    osi_cv_t cv;			/* for event I/O, using iodone mutex */
    opaque data;			/* backptr to user's private data */
} asevent_event_t;

/* asevent_event flags */
#define ASEVENT_SIGNALLED	1	/* event has been signalled */
#define ASEVENT_INTRANSIT	2	/* I/O in progress on this event */
#define ASEVENT_CREATED		8	/* allocated in asevent_Create */
#define ASEVENT_NOPIN		16	/* do not pin/unpin data  (AIXism) */
#define ASEVENT_IGNORESET	32	/* do not update set when completing */

typedef struct asevent_set {
    asevent_event_t *first;		/* first event in this list */
    struct asevent_set *next;		/* free list link */
    short flags;			/* flags--see below */
    short total;			/* total events */
    short pending;			/* count of pending events */
    short refCount;			/* reference count */
    long code;				/* first non-zero code signalled */
    epi_mutex_t lock;			/* for set mgmt, not I/O */
    osi_cv_t cv;			/* for set I/O, using iodone mutex */
} asevent_set_t;

/* asevent_set flags */
#define ASEVENT_SETDELETED	1	/* this event set has been deleted */
#define ASEVENT_SETCREATED	2	/* allocated (by asevent_CreateSet) */
#define ASEVENT_WAITINGANY	4	/* waiting for any event */

/* Debug mask bits for async modules */
#define ASYNC_DEBUG_ASEVENT	1
#define ASYNC_DEBUG_US_IO	2

#define ASYNC_DEBUG_SLEEP	32	/* all calls to sleep */

#ifdef AFS_DEBUG
extern char *async_debugFilename[];
#endif /* AFS_DEBUG */

#if 0
#define async_DbMask() \
    ((strcmp(__FILE__, "asevent.c") == 0) ? ASYNC_DEBUG_ASEVENT : \
     ((strcmp(__FILE__, "us_io.c") == 0) ? ASYNC_DEBUG_US_IO :	  \
      0))
#endif

extern int afsdb_episode_async;
#define async_DBprint(p) dmprintf (afsdb_episode_async, DEBUG_THIS_FILE, p)
#define async_SleepPrint(p) \
    dprintf(afsdb_episode_async & \
	((1 << (DEBUG_THIS_FILE - 1)) | \
	(1u << (ASYNC_DEBUG_SLEEP - 1))), p)

/* Exported Procedures */

extern void asevent_Init (void);	/* package initialization */

extern int ins_async(
  struct ins_opDesc *opcode,
  struct ins_varDesc *vars,
  struct ins_basicObjectId *argObjIds,
  struct ins_basicObjectId *resultObjIds,
  struct ins_basicObject *resultValues
);

extern asevent_event_t *asevent_Create(short flags);
extern void asevent_Initialize(asevent_event_t *ae, opaque data);
extern asevent_set_t *asevent_CreateSet(void);
extern void asevent_InitializeSet(asevent_set_t *as);

extern long asevent_AddToSet(asevent_set_t *as, asevent_event_t *ae);
extern void asevent_RemoveFromSet(asevent_set_t *as, asevent_event_t *ae);

extern void asevent_SignalEvent(asevent_event_t *ae, long acode);

extern void asevent_Hold(asevent_event_t *ae);
extern void asevent_Release(asevent_event_t *ae);
extern void asevent_HoldSet(asevent_set_t *as);
extern void asevent_ReleaseSet(asevent_set_t *as);

extern long asevent_WaitEvent(asevent_event_t *ae);
extern long asevent_WaitAll(asevent_set_t *as);
extern long asevent_WaitAny(asevent_set_t *as);

extern long asevent_InitDevice(
  long maj,				/* major device number to use */
  long min,				/* minor device */
  u_long size,				/* block size of device */
  async_device_t **deviceP		/* returned device */
);

extern long asevent_CloseDevice(async_device_t *device);

extern struct buf *asevent_MakeBuf(
  int flags,			/* buf flags */
  caddr_t addr,			/* data area */
  size_t length,		/* transfer length */
  async_device_t *aggr,		/* aggregate info */
  long ablock,			/* file system block no. */
  long aoffset			/* offset in FS block */
);

extern long asevent_QueueIO(
  async_device_t *adevice,
  struct buf *bp,
  asevent_event_t *ae
);

extern asevent_event_t *asevent_EnumerateSet(
  asevent_set_t *as,
  asevent_event_t *apreve
);

extern long asevent_StatEvent(asevent_event_t *ae, long *astat);

extern u_long async_ToBufferBlocks(async_device_t *device, daddr_t block);
extern daddr_t async_FromBufferBlocks(async_device_t *device, u_long block);

extern long asevent_efs_debug(long op, long parm1, long parm2);

#endif /* TRANSARC_EPI_ASEVENT_H */
