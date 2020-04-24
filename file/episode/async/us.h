/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: us.h,v $
 * Revision 1.1.103.1  1996/10/02  17:20:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:33  damon]
 *
 * $EndLog$
*/
/* us.h */

/* (C) Copyright 1994, 1990 Transarc Corporation - All Rights Reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/async/us.h,v 1.1.103.1 1996/10/02 17:20:14 damon Exp $ */
/*
 * This file contains pseudo-declarations needed for users of the
 * "us" device (user space pseudo disk device.)
 */

#ifndef TRANSARC_EPI_US_H
#define TRANSARC_EPI_US_H

#ifdef KERNEL
This is the include file for the simulated User Space disk-device
#endif

#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_device.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/osi_intr.h>

#define US_MAJOR_DEV (1)
#define US_MAX_MINOR_DEV (4)		/* e.g. 0..3 */

/*
 * These bits are passed to us_open.  Defined this way to be "backwards"
 * compatible.  However, these bit assignments are non-standard; beware.
 */
#define US_OFLAGS_RDWR   (0x0)
#define US_OFLAGS_ASYNC  (0x1)
#define US_OFLAGS_RDONLY (0x2)
#define US_OFLAGS_SYNC   (0x4)		/* force synchronous I/O */

/* ioctl operations.  Note that type of third argument varies.  */

/* (osi_off_t *)addr */
#define EPIU_IOCTL_SET_MAX_OFFSET 1	/* set eof point for file */
/* (char *)addr */
#define EPIU_IOCTL_SET_DIR 2		/* dir for pseudo-device files */
#define EPIU_IOCTL_SET_FILE 3		/* file name for this minor dev */
/* (int *)addr */
#define EPIU_IOCTL_SET_DEBUG 4		/* set debug value */
/* (struct us_ioctl_rw *)addr */
#define EPIU_IOCTL_PEEK 5		/* read from device */
#define EPIU_IOCTL_POKE 6		/* write to device */
/* (int *)addr */
#define EPIU_IOCTL_GET_READCOUNT 7	/* get number of reads */
#define EPIU_IOCTL_GET_WRITECOUNT 8	/* get number of writes */

struct us_ioctl_rw { /* use for PEEK or POKE */
    u_long block;			/* device block */
    unsigned long offset;		/* offset in block */
    unsigned long length;		/* bytes to transfer */
    caddr_t buffer;
};

extern int us_open(dev_t dev, int flag);
extern int us_ioctl(dev_t dev, int op, caddr_t addr);
extern int us_wakeup(caddr_t addr);
extern int us_close(dev_t dev, int flag);
extern int us_strategy(struct buf *bp);
extern int us_dump(void);
extern osi_off_t us_psize(dev_t dev);

extern long us_Init (void);
extern void *us_Complete(/* IN */ void *p);
extern int us_InitDevice(
  char *rawDev,
  u_long blockSize,
  osi_off_t maxSize,
  int openFlags,
  dev_t *fakeDev,
  struct async_device **asDevP);


#if defined(AFS_OSF_ENV) && !defined(_KERNEL)
/*
 * transaction finished:  not part of OSF/1;
 * only used for fake user space version
 */
#define B_DONE 0x00000002
#endif /* AFS_OSF_ENV */
#endif
