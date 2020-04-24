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
 * Revision 1.1.14.1  1996/10/02  17:14:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:26  damon]
 *
 * Revision 1.1.8.2  1994/06/09  13:55:17  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:29:51  annie]
 * 
 * Revision 1.1.8.1  1994/02/04  20:09:30  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:08:13  devsrc]
 * 
 * Revision 1.1.6.1  1994/01/28  20:43:31  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:09  annie]
 * 
 * 	expand OSF copyright
 * 
 * Revision 1.1.4.1  1993/09/01  15:25:40  delgado
 * 	DFS 486 port
 * 	[1993/09/01  15:25:21  delgado]
 * 
 * $EndLog$
*/
/* Copyright (C) 1989, 1990 Transarc Corporation - All rights reserved */

#ifndef	_PARAMAT386_OSF
#define	_PARAMAT386_OSF

/*
 * AFS-related System definitions for OSF/1 (AT386)
 */

/* General AFS definitions */
#define AFS_VFS_ENV		1
#define AFS_ENV  		1
#define AFS_SYSCALL             258


/* osi machine specific files */
#define HAS_OSI_UIO_MACH_H 
#define HAS_OSI_CRED_MACH_H 
#define HAS_OSI_NET_MACH_H 
#define HAS_OSI_KVNODE_MACH_H 
#define HAS_OSI_USER_MACH_H

/* 
 * File system entry (used if mount.h doesn't define MOUNT_AFS) 
 */
#define AFS_MOUNT_AFS		6

/*
 * Used by agfs_mount.h if MOUNT_AGFS is not defined
 */
#define AFS_MOUNT_AGFS		4	/* Steal 4 from PC FS if we have to */


/* 
 * Machine / Operating system information 
 */
#define SYS_NAME		"at386_osf1"
/*
 * #define AFS_DEFAULT_ENV for non-reference platforms.
 * This will cause the 'portable' code to be compiled, or a #error
 * in situations where it was difficult to supply such code.
 */
#undef  AFS_DEFAULT_ENV			
#define AFS_OSF_ENV             1	/* true for all OSF systems */
#define AFS_OSF11_ENV		1	/* true for OSF/1 R1.1 */

/* 
 * Extra kernel definitions
 */
#ifdef KERNEL
#define AFS_SYSVLOCK		1
#endif /* KERNEL */

/*
 * Says that the PAG is stored in a separate item, cr_pag, in the
 * ucred structure.  If undefined, the PAG must be stored in the group list.
 */
#define AFS_PAG_IN_CRED 1

/*
 * Says that RPC and DFS are dynamically loaded into the kernel at runtime.
 */
/***** #define AFS_DYNAMIC     1 *****/

/*
 * Says that the CM caches to any VFS file system.
 * If undefined, CM caches only to UFS file systems.
 */
#define AFS_VFSCACHE    1
	
#endif	/* _PARAM.AT386_OSF */

