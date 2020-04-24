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
 * Revision 1.1.14.1  1996/10/02  17:14:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:29  damon]
 *
 * Revision 1.1.8.1  1994/06/09  13:55:24  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:29:56  annie]
 * 
 * Revision 1.1.5.6  1993/01/18  20:53:27  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:39:50  cjd]
 * 
 * Revision 1.1.5.5  1992/12/10  22:26:50  marty
 * 	Upgrade DFS to OSF/1 1.1.1.  [OT defect 5236]
 * 	[1992/12/10  21:56:08  marty]
 * 
 * Revision 1.1.5.4  1992/11/18  17:54:51  jaffe
 * 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach 1.4
 * 	  Selected comments:
 * 	    This delta contains the bulk of the changes necessary to have aggregates
 * 	    mounted at attach time.  The purpose of this feature is to provide a
 * 	    means by which exported filesets (with no local mount points) will be
 * 	    sync'd periodically.  To accomplish this, we have created a new VFS type
 * 	    (agfs).  Whenever an aggregate is exported, a local mount of the aggregate
 * 	    is also done; then, when the sync daemon does its thing, the aggregate's sync
 * 	    vfsop will be called.  The agfs sync then calls the new sync aggrop, which
 * 	    in the case of Episode, calls efs_sync().  The UFS sync aggrop doen't do
 * 	    anything, since we know the UFS filesystem is mounted, and so, its sync vfsop
 * 	    will get called anyway.
 * 	    This delta must be bundled with the corresponding deltas in the Episode and
 * 	    test configurations.
 * 	    Added an OSF1-specific definition for AFS_MOUNT_AGGREGATE.
 * 	    Remove leftover junk from $OFILES.
 * 	    Rearrange things slightly to make use of a vendor-supplied definition
 * 	    of MOUNT_AGFS.
 * 	    Replace AFS_MOUNT_AGGREGATE with AFS_MOUNT_AGFS.
 * 	    Fix bogus linker warnings about shared libraries when compiling .o files.
 * 	[1992/11/17  20:03:13  jaffe]
 * 	Revision 1.1.7.3  1992/11/30  22:55:52  marty
 * 	OSF 1.1.1 port
 * 
 * Revision 1.1.5.3  1992/09/15  13:12:00  jaffe
 * 	Transarc delta: jaffe-sync-with-aug-31-osf-sources 1.4
 * 	  Selected comments:
 * 	    Pick up changes from last submission to the OSF.
 * 	    machine type is pmax_osf1
 * 	    remove commondefs.idl.  This was done incompletely in another delta
 * 	[1992/09/14  19:33:33  jaffe]
 * 
 * Revision 1.1.5.2  1992/08/31  19:30:16  jaffe
 * 	Sync with Transarc
 * 	[1992/08/31  14:13:11  jaffe]
 * 
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    Added defines for machine specific files for osi_uio.h, osi_cred.h,
 * 	    osi_net.h, and osi_kvnode.h
 * 	    Corrected nested comment. (by using an #if 0)
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  02:11:23  jaffe]
 * 
 * Revision 1.1.2.7  1992/06/03  21:41:23  garyf
 * 	pmax_osf -> pmax_osf1
 * 	[1992/06/03  21:40:55  garyf]
 * 
 * Revision 1.1.2.6  1992/05/28  20:47:54  toml
 * 	Portability fixes.
 * 	[1992/05/28  20:45:22  toml]
 * 
 * Revision 1.1.2.5  1992/05/01  11:52:15  garyf
 * 	fix ANSI C warning
 * 	[1992/05/01  11:51:55  garyf]
 * 
 * Revision 1.1.2.4  1992/03/09  19:45:03  delgado
 * 	Remove previous bsubmit - we have a better way to solve the problem
 * 	[1992/03/09  19:44:40  delgado]
 * 
 * Revision 1.1.2.3  1992/03/09  15:49:41  delgado
 * 	define _SOCKADDR_LEN so that user-space and kernel are using the same
 * 	sockaddr_in strucutre.  This will help little endian machines get
 * 	the correct value for sin_family.
 * 	[1992/03/09  15:48:58  delgado]
 * 
 * Revision 1.1.2.2  1992/01/24  01:50:54  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:10:08  devsrc]
 * 
 * $EndLog$
*/
/* Copyright (C) 1989, 1990 Transarc Corporation - All rights reserved */

#ifndef	_PARAMPMAX_OSF
#define	_PARAMPMAX_OSF

/*
 * AFS-related System definitions for OSF/1 (PMAX)
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
#define SYS_NAME		"pmax_osf1"
/*
 * #define AFS_DEFAULT_ENV for non-reference platforms.
 * This will cause the 'portable' code to be compiled, or a #error
 * in situations where it was difficult to supply such code.
 */
#undef  AFS_DEFAULT_ENV			
#define AFS_OSF_ENV             1	/* true for all OSF systems */
#define AFS_OSF11_ENV	1	/* true for OSF/1 R1.1 */

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
	
#endif	/* _PARAM.PMAX_OSF */

