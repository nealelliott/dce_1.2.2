/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_BUF_MACH_H
#define	TRANSARC_OSI_BUF_MACH_H

#include <sys/param.h>
#include <sys/buf.h>
#include <dcedfs/stds.h>

/*
 * Conversion between bytes, FS blocks and disk blocks
 *
 * XXX Name space pollution
 */
#define osi_btoab(d, b) ((b) >> (d)->logBlkSize)
#define osi_abtob(d, b) ((b) << (d)->logBlkSize)
#define dbtoab(d, b) ((b) >> ((d)->logBlkSize - DEV_BSHIFT))
#define abtodb(d, b) ((b) << ((d)->logBlkSize - DEV_BSHIFT))
#define dbroundup(b) roundup(b, DEV_BSIZE)
#define dbround(b) (b & ~(DEV_BSIZE - 1))

#define b_work b_pfcent			/* use as driver work area */

#ifdef	KERNEL
/* special macro that provides uniform access to bufsize */
#define b_Bufsize(b) ((b)->b_bufsize)
#endif

/* special macro that allows b_work to be accessed as an lvalue */
#define B_Work(bp) (*(struct buf **)(&(bp)->b_work))

#ifndef KERNEL
#undef iodone				/* get the test_vm procedure */
#endif

#ifdef	KERNEL
#define b_Wanted(bp) (((bp)->b_flags & B_WANTED) \
		      ? ((bp)->b_flags &= ~B_WANTED, 1) : 0)
#define b_Want(bp) ((bp)->b_flags |= B_WANTED)
#else	/* KERNEL */
#define b_Wanted(bp) 1
#define b_Want(bp) 1
#endif	/* KERNEL */

#define b_Call(bp) (((bp)->b_flags & B_CALL) ? ((*(bp)->b_iodone)(bp), 1) : 0)

#ifndef B_PFSTORE
#define B_PFSTORE	0x40000000
#endif

/*
 * Get and set device number.
 */
#define osi_bdev(bp)		((bp)->b_dev)
#define osi_set_bdev(bp, d)	(osi_bdev(bp) = (d))

typedef int osi_iodone_t;

#define osi_iodone_return(n)	return (n)

#define osi_set_iodone(bp, func) \
    ((bp)->b_iodone = (func), (bp)->b_flags |= B_CALL)


#ifdef KERNEL
#define osi_bio_wakeup(bp, cv)	osi_Wakeup((opaque)(bp))
#define osi_bio_init(bp)

#define osi_bio_cleanup(bp, cv) \
    MACRO_BEGIN \
	(bp)->b_flags |= B_DONE; \
	if ((bp)->b_flags & B_ASYNC) \
	    osi_ReleaseBuf(bp); \
    MACRO_END

#define osi_biodone(bp, cv) \
    MACRO_BEGIN \
	if ((bp)->b_flags & B_WANTED) { \
	    (bp)->b_flags &= ~B_WANTED; \
	    osi_bio_wakeup(bp, cv); \
	} \
     MACRO_END

#define osi_bio_wait(bp, cv, mutex) \
    MACRO_BEGIN \
	if (!((bp)->b_flags & B_DONE)) { \
	     (bp)->b_flags |= B_WANTED; \
	     osi_Sleep((opaque)(bp)); \
	} \
    MACRO_END

#endif /* KERNEL */

#endif /* TRANSARC_OSI_BUF_MACH_H */
