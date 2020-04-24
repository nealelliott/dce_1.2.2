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
 * Revision 1.1.20.1  1996/10/02  17:14:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:26  damon]
 *
 * Revision 1.1.14.2  1994/06/09  13:55:19  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:29:52  annie]
 * 
 * Revision 1.1.14.1  1994/02/04  20:09:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:08:15  devsrc]
 * 
 * Revision 1.1.12.1  1993/12/07  17:15:30  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:29:42  jaffe]
 * 
 * Revision 1.1.4.5  1993/01/18  20:53:13  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:39:36  cjd]
 * 
 * Revision 1.1.4.4  1992/10/28  21:38:55  jaffe
 * 	Removed duplicate TALog entries.
 * 	[1992/10/28  21:03:57  jaffe]
 * 
 * Revision 1.1.4.3  1992/09/15  13:11:57  jaffe
 * 	Transarc delta: jaffe-ot4608-ucred-cleanup 1.3
 * 	  Selected comments:
 * 	    use osi macros for ucred access.  New macros are osi_GetNGroups,
 * 	    osi_SetNGroups, osi_GetUID, osi_GetRUID, and osi_GetGID.  New
 * 	    constants are OSI_NGROUPS, OSI_NOGROUP, and OSI_MAXGROUPS.
 * 	    add HAS_OSI_CRED_MACH to the params.
 * 	    include osi_cred.h for appropriate definitions.
 * 	[1992/09/14  19:33:09  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  19:22:48  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    Added defines for machine specific files for osi_vmm.h, osi_uio.h,
 * 	    osi_buf.h, and osi_net.h
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    Added defines for machine specific files for osi_vmm.h, osi_uio.h,
 * 	    osi_buf.h, and osi_net.h
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * 	$TALog: param.h,v $
 * 	Revision 1.8  1994/11/01  21:13:53  cfe
 * 	Bring over the changes that the OSF made in going from their DCE 1.0.3
 * 	release to their DCE 1.1 release.
 * 	[from r1.7 by delta cfe-db6109-merge-1.0.3-to-1.1-diffs, r1.1]
 * 
 * 	Revision 1.7  1993/07/30  18:17:15  bwl
 * 	Port DFS 1.0.3 to HP/UX, adapting HP's changes (which were made to the
 * 	1.0.2a code base) to our own code base.
 * 
 * 	Various changes from HP.
 * 	[from r1.6 by delta bwl-o-db3961-port-103-to-HP, r1.1]
 * 
 * Revision 1.6  1993/01/19  21:21:24  jaffe
 * 	import OSF changes since 2.3 drop.
 * 	Mostly OSF copyright changes.
 * 	[from r1.5 by delta osf-revdrop-01-19-93, r1.1]
 * 
 * 	Revision 1.4  1992/07/29  21:35:38  jaffe
 * 	Fixed many compiler warnings in the osi directory.
 * 	Reworked ALL of the header files.  All files in the osi directory now
 * 	have NO machine specific ifdefs.  All machine specific code is in the
 * 	machine specific subdirectories.  To make this work, additional flags
 * 	were added to the afs/param.h file so that we can tell if a particular
 * 	platform has any additional changes for a given osi header file.
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation - All rights reserved */

#ifndef	_PARAM_HPUX_H
#define	_PARAM_HPUX_H

/*
 * DFS-related definitions for HPUX.
 */

/* osi machine specific files */
#define HAS_OSI_USER_MACH_H 
#define HAS_OSI_INTR_MACH_H 
#define HAS_OSI_UIO_MACH_H  
#define HAS_OSI_CRED_MACH_H 
#define HAS_OSI_NET_MACH_H   
#define HAS_OSI_KVNODE_MACH_H 

#if	0
#define HAS_OSI_VMM_MACH_H
#define HAS_OSI_PARAM_MACH_H
#define HAS_OSI_DEVICE_MACH_H
#define HAS_OSI_CALLTRACE_MACH_H
#define HAS_OSI_BUF_MACH_H
#endif

/* 
 * File system entry (used if mount.h doesn't define MOUNT_AFS) 
 */
#define AFS_MOUNT_AFS3		3
#define AFS_MOUNT_DFS		4 
#define AFS_MOUNT_AFS		4  /* for now, let's not force the issue */
#if 0
#define AFS_MOUNT_EPISODE       dont!use!me!i!am!dynamic!now
#endif


/* 
 * Machine / Operating system information 
 */
#define sys_hp700_90		1
#define sys_hpux_90		1
#define SYS_NAME		"hp700_ux900" 
#define SYS_NAME_ID       SYS_NAME_ID_hp700_ux90 

/*
 * #define AFS_DEFAULT_ENV for non-reference platforms.
 * This will cause the 'portable' code to be compiled, or a #error
 * in situations where it was difficult to supply such code.
 */
#undef  AFS_DEFAULT_ENV			
#define AFS_HPUX_ENV             1	/* true for all HPUX systems */

/* 
 * Extra kernel definitions
 */
#ifdef KERNEL
#define AFS_SYSVLOCK		1
#endif /* KERNEL */

/*
 * Says that RPC and DFS are dynamically loaded into the kernel at runtime.
 */
#define AFS_DYNAMIC     1 

/*
 * Says that the CM caches to any VFS file system.
 * If undefined, CM caches only to UFS file systems.
 */
#ifdef	notdef
#define AFS_VFSCACHE    1 
#endif
	
#if	defined(AFS_DEBUG) && defined(KERNEL)
/* this makes kernel debugging a whole lot easier */
#define STATIC
#endif

#endif	/* _PARAM_HPUX_H_ */
