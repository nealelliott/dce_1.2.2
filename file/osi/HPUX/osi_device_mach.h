/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_device_mach.h,v $
 * Revision 1.1.26.1  1996/10/02  17:57:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:24  damon]
 *
 * Revision 1.1.21.2  1994/06/09  14:15:01  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:36  annie]
 * 
 * Revision 1.1.21.1  1994/02/04  20:23:54  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:15:24  devsrc]
 * 
 * Revision 1.1.19.1  1993/12/07  17:29:26  jaffe
 * 	New File from Transarc 1.0.3a
 * 	[1993/12/02  21:30:47  jaffe]
 * 
 * $EndLog$
 */

/*
 *      Copyright (C) 1993 Transarc Corporation
 *      All rights reserved.
 */

/* $Header: /u0/rcs_trees/dce/rcs/file/osi/HPUX/osi_device_mach.h,v 1.1.26.1 1996/10/02 17:57:27 damon Exp $ */

/*
 * HPUX device I/O definitions
 */
#ifndef TRANSARC_OSI_DEVICE_MACH_H
#define	TRANSARC_OSI_DEVICE_MACH_H


#ifdef _KERNEL
#define osi_inc_ru_oublock(n)	(u.u_ru.ru_oublock += (n))
#define osi_inc_ru_inblock(n)	(u.u_ru.ru_inblock += (n))
#endif

#ifdef _KERNEL
#define osi_strategy(bp) \
    ((*(bdevsw[osi_major(osi_bdev(bp))].d_strategy))(bp), 0)
#else
/*extern int us_strategy(struct buf *bp);*/
#define osi_strategy(bp)	us_strategy(bp)
#endif /* _KERNEL */

#define	osi_major(d)		major(d)
#define osi_minor(d)		minor(d)
#define	osi_makedev(maj, min)	makedev(maj, min)

#endif /* TRANSARC_OSI_DEVICE_MACH_H */
