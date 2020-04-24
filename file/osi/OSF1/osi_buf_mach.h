/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_buf_mach.h,v $
 * Revision 1.1.175.1  1996/10/02  17:58:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:50  damon]
 *
 * Revision 1.1.170.2  1994/06/09  14:15:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:01  annie]
 * 
 * Revision 1.1.170.1  1994/02/04  20:24:42  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:42  devsrc]
 * 
 * Revision 1.1.168.2  1994/01/20  18:43:32  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:47  annie]
 * 
 * Revision 1.1.168.1  1993/12/07  17:29:52  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:20:09  jaffe]
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/osi/OSF1/osi_buf_mach.h,v 1.1.175.1 1996/10/02 17:58:14 damon Exp $ */

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

/* special macro that provides uniform read-only access to bufsize */
#ifdef KERNEL
#define b_Bufsize(b) ((b)->b_bufsize+0)
#endif

/* special macro that allows b_work to be accessed as an lvalue */
#define B_Work(bp) (*(struct buf **)(&(bp)->b_work))

#ifndef KERNEL
#undef iodone				/* get the test_vm procedure */
#endif

#if defined(KERNEL)
#define b_Wanted(bp) (((bp)->b_flags & B_WANTED) \
		      ? ((bp)->b_flags &= ~B_WANTED, 1) : 0)
#define b_Want(bp) (bp->b_flags |= B_WANTED)
#else
#define b_Wanted(bp) 1
#define b_Want(bp) 1
#endif

#define b_Call(bp) ((bp)->b_iodone ? ((*(bp)->b_iodone)(bp), 1) : 0)

#ifndef B_PFSTORE
#define B_PFSTORE	0x40000000
#endif

/*
 * Get and set device number.
 */
#define osi_bdev(bp)		((bp)->b_dev)
#define osi_set_bdev(bp, d)	(osi_bdev(bp) = d)

typedef void osi_iodone_t;

#define osi_iodone_return(n)	return

#define osi_set_iodone(bp, func) ((bp)->b_iodone = (func))

#define osi_bio_init(bp)

#ifdef KERNEL

#define osi_bio_wakeup(bp, cv)	event_post(&(bp)->b_iocomplete)
#define osi_biodone(bp, cv)	osi_bio_wakeup(bp, cv)

#define osi_bio_wait(bp, cv, mutex) \
    MACRO_BEGIN \
	event_wait(&(bp)->b_iocomplete, FALSE, 0); \
    MACRO_END

#define osi_biodone(bp, cv) \
    MACRO_BEGIN \
	(bp)->b_flags |= B_DONE; \
	osi_bio_wakeup(bp, cv); \
    MACRO_END

#define osi_bio_cleanup(bp, cv) \
    MACRO_BEGIN \
	(bp)->b_flags |= B_DONE; \
	if ((bp)->b_flags & B_ASYNC) \
	    osi_ReleaseBuf(bp); \
    MACRO_END

#endif /* KERNEL */

#endif /* TRANSARC_OSI_BUF_MACH_H */
