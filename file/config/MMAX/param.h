/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: param.h,v $
 * Revision 1.1.12.1  1996/10/02  17:14:28  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:27  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:55:20  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:29:54  annie]
 * 
 * Revision 1.1.4.2  1993/01/18  20:53:21  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:39:41  cjd]
 * 
 * Revision 1.1.2.2  1992/01/24  01:50:50  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:10:06  devsrc]
 * 
 * $EndLog$
*/

/* Copyright (C) 1989, 1990 Transarc Corporation - All rights reserved */

#ifndef	_PARAM_MMAX_OSF
#define	_PARAM_MMAX_OSF

/*
 * AFS-related System definitions for OSF/1 (PMAX)
 */

/* General AFS definitions */
#define AFS_VFS_ENV		1
#define AFS_GREEDY43_ENV	1
#define AFS_ENV  		1
#define AFS_SYSCALL             258

/* 
 * File system entry (used if mount.h doesn't define MOUNT_AFS) 
 */
#define AFS_MOUNT_AFS		6

/* 
 * Machine / Operating system information 
 */
#define sys_mmax_osf		1
#define SYS_NAME		"mmax_osf"
#define AFS_OSF_ENV             1	/* true for all OSF systems */
/* #define AFS_OSF11_ENV        1       /* true for OSF/1 R1.1 */

/* 
 * Extra kernel definitions
 */
#ifdef KERNEL
#define AFS_SYSVLOCK		1
#endif /* KERNEL */

#endif	/* _PARAM_MMAX_OSF */
