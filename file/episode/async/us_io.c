/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: us_io.c,v $
 * Revision 1.1.105.1  1996/10/02  17:20:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:34  damon]
 *
 * Revision 1.1.99.3  1994/07/13  22:22:16  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:43:41  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:56:45  mbs]
 * 
 * Revision 1.1.99.2  1994/06/09  13:59:58  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:13  annie]
 * 
 * Revision 1.1.99.1  1994/02/04  20:12:42  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:58  devsrc]
 * 
 * Revision 1.1.97.1  1993/12/07  17:18:20  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:52:57  jaffe]
 * 
 * $EndLog$
 */

/* us_io.c -- User space versions of I/O routines used in-kernel.  These
 *     routines are used to provide an interface to a pseudo-kernel device
 *     driver style service.  It is implemented to write on a raw file.
 *
 * We use "us" as the prefix for our command (us = user space).
 */

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h> 

#include <dcedfs/episode/epi_thread.h>

#include <async.h>
#include <us.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/async/us_io.c,v 1.1.105.1 1996/10/02 17:20:15 damon Exp $")

#define DEBUG_THIS_FILE ASYNC_DEBUG_US_IO

#ifdef OSI_TRACE_SYNC
/* this should probably go into epi_thread.c */ 
#include <dcedfs/icl.h>
#include <dcedfs/episode/ethr_trace.h>

extern struct icl_set *epi_lockInitSet;
extern struct icl_set *epi_lockSet;

#define epi_TraceInit(a,s) \
    icl_Trace2(epi_lockInitSet, TR_ETHR_COND_INIT, \
	       ICL_TYPE_STRING, s, ICL_TYPE_POINTER, a)
#define epi_TraceSleep(a) \
    icl_Trace1(epi_lockSet, TR_ETHR_COND_WAIT, ICL_TYPE_POINTER, a)
#define epi_TraceWakeup(a) \
    icl_Trace1(epi_lockSet, TR_ETHR_COND_WAKE, ICL_TYPE_POINTER, a)

#else

#define epi_TraceInit(a,s) 0
#define epi_TraceSleep(a)  0
#define epi_TraceWakeup(a) 0

#endif

/* Deal with some transitional 64bit stuff (db6444) */

#ifdef AFS_SUNOS5_ENV
extern offset_t llseek(int, offset_t, int);
#endif /* AFS_SUNOS5_ENV */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX4_ENV)
#define llseek(fd,o,w) lseek(fd,(off_t)o,w)
#endif /* AFS_SUNOS5_ENV */

#if defined(AFS_SUNOS5_ENV) && !defined(ldbtob)
/* only defined on Sol2.4 */
#define ldbtob(db)                      /* calculates (db * DEV_BSIZE) */ \
    ((osi_off_t)(db) << DEV_BSHIFT)
#endif

#if defined(AFS_AIX_ENV)
#define ldbtob(db) dbtob((osi_off_t)(db))
#endif

/* path of directory for pseudo-device files */

static char *scratchPath = ".";

/* Define 4 pseudo-devices for testing in users space.  Only test_anode
 * actually uses more than one. */

static struct usInfo {
    epi_mutex_t lock;			/* protects these fields */
    char *devname;			/* filename to use for pseudo device */
    int fid;				/* file descriptor returned by open */
    int status;				/* status bits for this pseudo disk */
    osi_off_t maxSize;			/* size of "disk" */
    int openCount;			/* # of users */
    long readCount;			/* number of read operations */
    long writeCount;			/* ... writes ... */
} us_info[US_MAX_MINOR_DEV];

/* used in the status field */
#define US_OPENED 1
#define US_DEBUG 2			/* 1=>print debugging info */
#define US_REALDEV 4			/* 0=> reg file, 1=>block/char dev */
#define US_ASYNC_COMPLETE 8
#define US_RDONLY (0x10)

/* In the kernel, interactions with interrupt processing are traditionally
 * managed using splbio/splx pairs.  These function logically as a mutex.  In
 * user space we don't have interrupt levels so we use a real mutex.  The
 * ../../osi/osi_intr.h file defines these macros.  The SunOS5 kernel also uses
 * a mutex, see ../../osi/SUNOS5/osi_intr_mach.h. */

/* FinishIO -- marks I/O as finished and do a wakeup on the buffer ptr since
 *     that's what the kernel device driver does. */

static void FinishIO (/* IN */ struct buf *bp)
{
    int min = osi_minor(osi_bdev(bp));

    osi_intr_exclude(&osi_iodoneLock, 0);

    async_DBprint (("%s %d bytes to blkno %d",
		    (bp->b_flags & B_READ) ? "read " : "wrote",
		   bp->b_bcount, bp->b_blkno));
    if (us_info[min].status & US_DEBUG) {
	fprintf(stderr, "%s %d bytes to blkno %d\n",
		(bp->b_flags & B_READ) ? "read " : "wrote",
		bp->b_bcount, bp->b_blkno);
    }

#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
    bp->b_flags |= B_DONE;
#endif	/* AFS_AIX_ENV, AFS_HPUX_ENV */
    (*bp->b_iodone)(bp);
    osi_intr_allow(&osi_iodoneLock, 0);
}

/* DoIO -- actually do the I/O requested. */

static void DoIO (struct buf *bp)
{
    int min = osi_minor(osi_bdev(bp));
    int fid;
    int nbytes;
    osi_off_t offset;
    osi_off_t pos;

    epi_mutex_lock (&us_info[min].lock);

    fid = us_info[min].fid;
    offset = ldbtob(bp->b_blkno);

    afsl_Assert ((us_info[min].status & US_OPENED) && (fid != -1) &&
		 !((bp->b_blkno < 0)
		   || (offset < 0)
		   || (offset + bp->b_bcount < 0)
		   || (offset + bp->b_bcount > us_info[min].maxSize)));

    epi_TraceSleep (&us_strategy);
    pos = llseek(fid, offset, SEEK_SET);
    afsl_Assert (pos == offset);
    if (bp->b_flags & B_READ) {
	us_info[min].readCount++;
	nbytes = read (fid, bp->b_un.b_addr, bp->b_bcount);
    } else {
	us_info[min].writeCount++;
	nbytes = write (fid, bp->b_un.b_addr, bp->b_bcount);
    }
    epi_TraceWakeup (&us_strategy);

    if (nbytes != bp->b_bcount) {
	fprintf (stderr, "%s(%d, %#x, %d) failed\n", (bp->b_flags & B_READ) ?
		 "read" : "write", fid, bp->b_un.b_addr, bp->b_bcount);
	if (nbytes < 0) perror("(*rdwr)");
	else fprintf (stderr, "transfered only %d bytes\n", nbytes);

	/* Probably the pseudo-disk file has filled up its partition.  We
         * aren't good at handling these out-of-the-blue errors so exit
         * clearly. */
	abort();
    }
    epi_mutex_unlock (&us_info[min].lock);

    FinishIO (bp);
}

static epi_mutex_t incompleteLock;
static epi_cond_t incompleteWork;	/* completer thread waiting for work */
static epi_cond_t incompleteSpace;	/* scheduling thread needs que space */
static int waitingWork = 0;
static int waitingSpace = 0;
static int completerRunning = 0;

#define MAX_INCOMPLETE_IO 32
static struct buf *incomplete[MAX_INCOMPLETE_IO];

/* us_Complete -- is called in a separate thread when it is desired that user
 *     space I/O appear to complete asynchronously. */

void *
us_Complete(/* IN */ void *p)
{
    u_int lastSlot = 0;
    int loop = 0;
    if (completerRunning)
	return 0;
    completerRunning++;
    async_DBprint (("Completer process running"));

    epi_mutex_lock (&incompleteLock);
    while (1) {
	struct buf *bp;
	bp = incomplete[lastSlot];
	if (bp) {
	    int notify = waitingSpace;
	    incomplete[lastSlot] = 0;
	    epi_mutex_unlock (&incompleteLock);
	    if (notify)
		epi_cond_signal(&incompleteSpace);

	    DoIO (bp);

	    epi_mutex_lock (&incompleteLock);

	    loop = 0;
	} else {
	    loop++;
	    if (loop >= MAX_INCOMPLETE_IO) {
		loop = 0;
		async_SleepPrint (("I/O completer process is idle"));
		waitingWork++;
		epi_cond_wait (&incompleteWork, &incompleteLock);
		waitingWork--;
	    }
	}
	if (lastSlot == 0)
	    lastSlot = MAX_INCOMPLETE_IO;
	lastSlot--;
    }
}

static void MaybeDoIO(/* IN */ struct buf *bp)
{
    static int lastSlot = 0;
    int min = osi_minor(osi_bdev(bp));

    if (us_info[min].status & US_ASYNC_COMPLETE) {
	epi_mutex_lock (&incompleteLock);
	while (1) {
	    int i;
	    for (i=0; i<MAX_INCOMPLETE_IO; i++) {
		lastSlot = (lastSlot+1) % MAX_INCOMPLETE_IO;
		if (incomplete[lastSlot] == 0) {
		    int notify = waitingWork;
		    incomplete[lastSlot] = bp;
		    epi_mutex_unlock (&incompleteLock);
		    if (notify)
			epi_cond_signal (&incompleteWork);
		    return;
		}
	    }
	    /* table is full, wait for slots */
	    async_SleepPrint (("incomplete I/O table full"));
	    waitingSpace++;
	    epi_cond_wait (&incompleteSpace, &incompleteLock);
	    waitingSpace--;
	}
    }
    /* Else, since we're synchronous, just do it now. */
    DoIO(bp);
}

/* us_Init -- do initialization for this module.  We use pthread_once to make
 *     sure the internal worker function is only called once.  Non-test
 *     programs that do not use the completer thread will generally not call
 *     this routine and instead we call it internally from us_ioctl and
 *     us_open. */

static void Init (void)
{
    int i;

    epi_mutex_init (&incompleteLock, "incomplete");
    epi_cond_init (&incompleteWork, "incomplete work");
    epi_cond_init (&incompleteSpace, "incomplete space");
    for (i=0; i<US_MAX_MINOR_DEV; i++) {
	epi_mutex_init (&us_info[i].lock, "us info");
	us_info[i].devname = NULL;
	us_info[i].fid = -1;
	us_info[i].status = 0;
	us_info[i].maxSize = 0;
	us_info[i].openCount = 0;
	us_info[i].readCount = 0;
	us_info[i].writeCount = 0;
    }
    epi_TraceInit (&us_strategy, "I/O");
}

long us_Init (void)
{
    static pthread_once_t initDone = pthread_once_init;
    pthread_once (&initDone, Init);
    return 0;
}

/* EXPORT */
int us_wakeup(caddr_t addr)
{
    osi_Wakeup(addr);
    return 0;
}

/*
 * us_open -- opens a user space device.  Initialize the device's file.  It is
 *     supposed to correspond to the bdevsw.d_open function pointer.
 *
 * PARAMETERS -- flag provides some control over how the device works.  It does
 *     not use the standard interpretation of these bits.  See, for instance,
 *     ../vnops/stubs.c:dummyopen. */

/* EXPORT */
int us_open(dev_t dev, int flag)
{
    int min = minor(dev);
    char path[256];
    struct stat sbuf;
    int code;
    int oflags;

    afsl_Assert(osi_major(dev) == US_MAJOR_DEV);
    afsl_Assert((min >= 0) && (min < US_MAX_MINOR_DEV));

    code = us_Init();
    if (code) return code;

    epi_mutex_lock (&us_info[min].lock);
    if (us_info[min].status & US_OPENED) {
	if (!(flag & US_OFLAGS_RDONLY) != 
	    !(us_info[min].status & US_RDONLY))
	    code = EACCES;
	else {
	    us_info[min].openCount++;
	}
	epi_mutex_unlock (&us_info[min].lock);
	return code;
    }

    if (us_info[min].devname == NULL)
	sprintf (path, "%s/dev-%d", scratchPath, min);
    else
	sprintf (path, "%s/%s", scratchPath, us_info[min].devname);
    if (flag & US_OFLAGS_RDONLY) {
	us_info[min].fid = open(path, O_RDONLY|O_CREAT, 0600);
	us_info[min].status |= US_RDONLY;
    } else {
	oflags = O_RDWR|O_CREAT;
	if (flag & US_OFLAGS_SYNC) {
	    oflags |= O_SYNC;
	}
	us_info[min].fid = open(path, oflags, 0600);
	us_info[min].status &= ~US_RDONLY;
    }
    if (us_info[min].fid == -1)
	code = errno;
    else {
	us_info[min].status |= US_OPENED;
	us_info[min].openCount = 1;
	code = fstat(us_info[min].fid, &sbuf);
	afsl_Assert (code == 0);
	us_info[min].maxSize = sbuf.st_size;
	if (((sbuf.st_mode & S_IFMT) == S_IFBLK) ||
	    ((sbuf.st_mode & S_IFMT) == S_IFCHR))
	    us_info[min].status |= US_REALDEV;
	else
	    us_info[min].status &= ~US_REALDEV;
	us_info[min].readCount = us_info[min].writeCount = 0;
	if (flag & US_OFLAGS_ASYNC) {
	    /* simulate async I/O completions */
	    us_info[min].status |= US_ASYNC_COMPLETE;
	}
    }
    epi_mutex_unlock (&us_info[min].lock);
    return code;
}

/*
 * us_close -- closes a user space device.
 */
/* EXPORT */
int us_close(dev_t dev, int flag)
{
    int code;
    int min = minor(dev);

    afsl_Assert(us_info[min].status & US_OPENED);

    epi_mutex_lock (&us_info[min].lock);
    if (--us_info[min].openCount == 0) {
	code = close(us_info[min].fid);
	afsl_Assert (code != -1);
	us_info[min].fid = -1;
	us_info[min].status &= ~US_OPENED;
	us_info[min].maxSize = 0;
    }
    epi_mutex_unlock (&us_info[min].lock);
    return 0;
}

/*
 * us_strategy -- In the kernel, this call is asynchronous.  We approximate
 *     this behavior only when US_OFLAGS_ASYNC was specified in the call to
 *     us_open. */

/* EXPORT */
int us_strategy(/* IN */ struct buf *bp)
{
    int min;
    osi_off_t offset;

    /* Do these check without locking since they don't really depend on values
     * that are changing dynamically. */

    afsl_Assert(bp);
    min = osi_minor(osi_bdev(bp));
    afsl_Assert(min >= 0 && min < US_MAX_MINOR_DEV);

    if (!(us_info[min].status & US_OPENED)) {
	bp->b_error = ENXIO;
	goto bad;
    }
    afsl_Assert(us_info[min].fid != -1);

    offset = ldbtob(bp->b_blkno);
    if ((bp->b_blkno < 0)
	  || (offset < 0)
	  || (offset + bp->b_bcount < 0)
	  || (offset + bp->b_bcount > us_info[min].maxSize)) {
	bp->b_error = EINVAL;
	goto bad;
    }

    MaybeDoIO (bp);
    return 0;

bad:
    bp->b_flags |= B_ERROR;
#ifdef	AFS_AIX_ENV
    bp->b_flags |= B_DONE;
#endif	/* AFS_AIX_ENV */
    (*bp->b_iodone)(bp);
    return 0;
}

/*
 * write physical memory to a device.  This won't be implemented in
 * this driver because this is called upon kernel "panic."
 */

/* EXPORT */
int us_dump(void)
{
    panic("us_dump called, not supported!");
    return 0;
}

/*
 * Returns the size of a partition.
 */

/* EXPORT */
osi_off_t us_psize(dev_t dev)
{
    int min = minor(dev);
    osi_off_t size;

    afsl_Assert(osi_major(dev) == US_MAJOR_DEV);
    afsl_Assert((min >= 0) && (min < US_MAX_MINOR_DEV));
    
    epi_mutex_lock (&us_info[min].lock);
    if (!(us_info[min].status & US_OPENED))
	size = -1;
    else
	size = us_info[min].maxSize;
    epi_mutex_unlock (&us_info[min].lock);
    return size;
}

/*
 * Support ioctl operations.
 */

/* EXPORT */
int us_ioctl(dev_t dev, int op, caddr_t addr)
{
    int min = minor(dev);
    int code;
    osi_off_t size;
    int fid;

    afsl_Assert(osi_major(dev) == US_MAJOR_DEV);
    afsl_Assert((min >= 0) && (min < US_MAX_MINOR_DEV));

    code = us_Init();
    if (code) return code;
    
    epi_mutex_lock (&us_info[min].lock);

    switch (op) {
      case EPIU_IOCTL_SET_MAX_OFFSET:
	if (!(us_info[min].status & US_OPENED)) {
	    code = ENODEV;
	    break;
	}
	fid = us_info[min].fid;

	size = *(osi_off_t *)addr;
	if (size < 0) {
	    code = EINVAL;
	    break;
	}
	if (!(us_info[min].status & US_REALDEV)
	    && size > LONG_MAX) {
	    code = EFBIG;
	    break;
	}
	if (size == us_info[min].maxSize)
	    break;			/* no-op */

	if (!(us_info[min].status & US_REALDEV) &&
	    !(us_info[min].status & US_RDONLY)) {

	    /* We could just use ftruncate in all cases.  (This even works on
             * AIX; I tried it). -blake 931118 */
	    /* However, I'm pretty sure this doesn't work on Ultrix so I am
             * unsure about OSF/1 and HP/UX. -ota 931118 */

	    if (size < us_info[min].maxSize) {
		/* shrink to this size */
		code = ftruncate (fid, (off_t)size);
		if (code == -1) {
		    code = errno;
		    break;
		}
	    } else {
		/* grow file EOF to this point */
		int zero = 0;
		osi_off_t pos;

		pos = llseek (fid, size-1, SEEK_SET);
		if (pos != size-1) {
		    code = errno;
		    break;
		}
		code = write (fid, &zero, 1);
		if (code != 1) {
		    code = errno;
		    break;
		}
	    }
	}
	us_info[min].maxSize = size;
	code = 0;
	break;

	/* set debugging based on flag pointed to be "addr". */
      case EPIU_IOCTL_SET_DEBUG:
	if (*(int *)addr) us_info[min].status |= US_DEBUG;
	else us_info[min].status &= ~US_DEBUG;
	break;

	/* set path for scratch files.  effective only for future opens */
      case EPIU_IOCTL_SET_DIR:
	if (strlen(addr) == 0)
	    code = EINVAL;
	else {
	    char *newPath = (char *)addr;
	    scratchPath = (char *) osi_Alloc (strlen(newPath)+1);
	    strcpy (scratchPath, newPath);
	}
	break;

	/* set file name for this minor device.  Must not be open. */
      case EPIU_IOCTL_SET_FILE:
	if (strlen(addr) == 0)
	    code = EINVAL;
	else if (us_info[min].status & US_OPENED)
	    code = EBUSY;
	else {
	    char *newName = (char *)addr;
	    us_info[min].devname = (char *) osi_Alloc (strlen(newName)+1);
	    strcpy (us_info[min].devname, newName);
	}
	break;

      case EPIU_IOCTL_PEEK:
      case EPIU_IOCTL_POKE:
	{   struct us_ioctl_rw *desc;
	    osi_off_t offset;
	    osi_off_t pos;

	    desc = (struct us_ioctl_rw *)addr;

	    if (!(us_info[min].status & US_OPENED)) {
		code = ENODEV;
		break;
	    }
	    fid = us_info[min].fid;
	    offset = ldbtob (desc->block) + desc->offset;
	    pos = llseek (fid, offset, SEEK_SET);
	    if (pos != offset) {
		code = errno;
		break;
	    }
	    if (op == EPIU_IOCTL_PEEK)
		code = read (fid, desc->buffer, desc->length);
	    else
		code = write (fid, desc->buffer, desc->length);
	    if (code != desc->length) {
		code = errno;
		break;
	    }
	    code = 0;
	}
	break;

      case EPIU_IOCTL_GET_READCOUNT:
	*(int *)addr = us_info[min].readCount;
	break;

      case EPIU_IOCTL_GET_WRITECOUNT:
	*(int *)addr = us_info[min].writeCount;
	break;

      default:
	code = ENOTTY;
    }
    
    epi_mutex_unlock (&us_info[min].lock);
    
    return code;
}

int us_InitDevice(
  /* IN */ char *rawDev,
  /* IN */ u_long blockSize,
  /* IN */ osi_off_t maxSize,
  /* IN */ int openFlags,
  /* OUT */ dev_t *fakeDev,
  /* OUT */ struct async_device **asDevP)
{
    long code;

    code = us_Init();
    if (code)
	return code;

    if (asDevP)
	if (code = asevent_InitDevice(US_MAJOR_DEV, 1, blockSize, asDevP))
	    return code;

    *fakeDev = makedev(US_MAJOR_DEV, 1);
    if (code = us_ioctl(*fakeDev, EPIU_IOCTL_SET_DIR, "/"))
	return code;

    if (code = us_ioctl(*fakeDev, EPIU_IOCTL_SET_FILE, &(rawDev[1])))
	return code;

    if (code = us_open(*fakeDev, openFlags))
	return code;

    code = us_ioctl(*fakeDev, EPIU_IOCTL_SET_MAX_OFFSET, (caddr_t)&maxSize);

    return code;
}
