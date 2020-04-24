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
 * Revision 1.1.390.1  1996/10/02  17:58:57  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:15  damon]
 *
 * Revision 1.1.385.2  1994/06/09  14:16:09  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:29  annie]
 * 
 * Revision 1.1.385.1  1994/02/04  20:25:21  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:57  devsrc]
 * 
 * Revision 1.1.383.1  1993/12/07  17:30:16  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:04:53  jaffe]
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/osi/RIOS/osi_buf_mach.h,v 1.1.390.1 1996/10/02 17:58:57 damon Exp $ */

#ifndef TRANSARC_OSI_BUF_MACH_H
#define	TRANSARC_OSI_BUF_MACH_H

#include <sys/param.h>
#include <sys/buf.h>

/*
 * Conversion between bytes, FS blocks and disk blocks
 *
 * XXX Name space pollution
 */
#define osi_btoab(d, b) ((b) >> (d)->logBlkSize)
#define osi_abtob(d, b) ((b) << (d)->logBlkSize)
#define dbtoab(d, b) ((b) >> ((d)->logBlkSize - UBSHIFT))
#define abtodb(d, b) ((b) << ((d)->logBlkSize - UBSHIFT))
#define dbroundup(b) roundup(b, UBSIZE)
#define dbround(b) (b & ~(UBSIZE - 1))  

/* special macro that provides uniform read-only access to bufsize */
#ifdef KERNEL
#define b_Bufsize(b) (PAGESIZE+0)
#endif

/* special macro that allows b_work to be accessed as an lvalue */
#define B_Work(bp) (*(struct buf **)(&(bp)->b_work))

#ifndef KERNEL
#undef iodone				/* get the test_vm procedure */
#endif

#define b_Wanted(bp) 1
#define b_Want(bp) 1

#define b_Call(bp) ((bp)->b_iodone ? ((*(bp)->b_iodone)(bp), 1) : 0)

#ifndef dbtob			/* but osi_port_aix.h defines this 1 */
#define dbtob(b) ((b) << UBSHIFT)
#endif
#define btodb(b) ((b) >> UBSHIFT)

/*
 * Get and set device number.
 */
#define osi_bdev(bp)		((bp)->b_dev)
#define osi_set_bdev(bp, d)	(osi_bdev(bp) = d)

typedef void osi_iodone_t;

#define osi_iodone_return(n)	return

#define osi_set_iodone(bp, func) ((bp)->b_iodone = (func))

#ifdef KERNEL

#define osi_bio_wakeup(bp, cv)	osi_Wakeup((opaque)(bp))

#define osi_bio_init(bp)

#define osi_biodone(bp, cv) \
    MACRO_BEGIN \
	bp->b_flags |= B_DONE; \
	osi_bio_wakeup(bp, cv); \
    MACRO_END

#define osi_bio_cleanup(bp, cv) \
    MACRO_BEGIN \
	(bp)->b_flags |= B_DONE; \
	if ((bp)->b_flags & B_ASYNC) \
	    osi_ReleaseBuf(bp); \
    MACRO_END

/* osi_bio_wait -- [see osi_buf.h for details].  Kernel automatically drops
 *     splbio when sleeping. */

#define osi_bio_wait(bp, cv, mutex) \
    MACRO_BEGIN	\
	if (!((bp)->b_flags & B_DONE)) \
	    osi_Sleep((opaque)(bp)); \
    MACRO_END

#endif /* KERNEL */

#endif /* TRANSARC_OSI_BUF_MACH_H */
