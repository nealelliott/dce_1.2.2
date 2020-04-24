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
 * Revision 1.1.12.1  1996/10/02  17:14:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:30  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:55:28  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:29:59  annie]
 * 
 * Revision 1.1.4.4  1993/01/18  20:53:30  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:39:55  cjd]
 * 
 * Revision 1.1.4.3  1992/11/18  17:54:55  jaffe
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
 * 	    Added an AIX-specific definition for AFS_MOUNT_AGGREGATE.
 * 	    Remove leftover junk from $OFILES.
 * 	    Rearrange things slightly to make use of a vendor-supplied definition
 * 	    of MOUNT_AGFS.
 * 	    Replace AFS_MOUNT_AGGREGATE with AFS_MOUNT_AGFS.
 * 	    Fix bogus linker warnings about shared libraries when compiling .o files.
 * 	[1992/11/17  20:03:38  jaffe]
 * 
 * Revision 1.1.4.2  1992/08/31  19:30:23  jaffe
 * 	Transarc delta: cburnett-ot4621-add-vfscache-support-for-aix 1.1
 * 	  Selected comments:
 * 	    Add AFS_VFSCACHE support for the AIX platform
 * 	    VFSCACHE AIX support
 * 	Transarc delta: cburnett-ot4624-fix-cm-sysname-on-aix32 1.1
 * 	  Selected comments:
 * 	    cm sysname returns wrong value on AIX 3.2
 * 	    Added SYS_NAME def for AFS_AIX32_ENV
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    Added defines for machine specific files for osi_vmm.h, osi_user.h,
 * 	    osi_param.h, osi_device.h, osi_intr.h, osi_calltrace.h, osi_uio.h,
 * 	    osi_buf.h, osi_cred.h, osi_net.h, and osi_kvnode.h.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  02:11:39  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/28  20:48:00  toml
 * 	Add #define AFS_DYNAMIC.
 * 	[1992/05/28  20:45:37  toml]
 * 
 * Revision 1.1  1992/01/19  02:51:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1989, 1990 Transarc Corporation - All rights reserved */

#ifndef	_PARAM_RS_AIX31_H_
#define	_PARAM_RS_AIX31_H_

/*
 * AFS-related System definitions for AIX 3.1
 */

/* General AFS definitions */
#define AFS_VFS_ENV		1
#define AFS_GREEDY43_ENV	1
#define AFS_ENV  		1

/* osi machine specific files */
#define HAS_OSI_VMM_MACH_H 
#define HAS_OSI_USER_MACH_H 
#define HAS_OSI_PARAM_MACH_H 
#define HAS_OSI_DEVICE_MACH_H 
#define HAS_OSI_INTR_MACH_H 
#define HAS_OSI_CALLTRACE_MACH_H 
#define HAS_OSI_UIO_MACH_H 
#define HAS_OSI_BUF_MACH_H 
#define HAS_OSI_CRED_MACH_H 
#define HAS_OSI_NET_MACH_H 
#define HAS_OSI_KVNODE_MACH_H 
#define HAS_OSI_MOUNT_MACH_H

/*
 * Machine / Operating system information 
 */
#ifdef AFS_AIX32_ENV
#define SYS_NAME		"rs_aix32"  /* for @sys */
#else
#define SYS_NAME		"rs_aix31"
#endif /* AFS_AIX32_ENV */
#define	RIOS			1	/* POWERseries 6000 */
#define	AFS_AIX_ENV		1	/* All AIX systems */
#define	AFS_AIX31_ENV		1	/* AIX 3.1 specific */
#ifdef notdef
#define	AFS_AIX31_VM		1	/* Enable AIX3.1 VM support */
#endif

/* 
 * Extra kernel definitions
 */
#ifdef KERNEL
#define	AFS_VFS34		1
#define	AFS_SHORTGID		1
#define	AFS_SYSVLOCK		1
#define	sys_bsd43		1	
#else
#ifndef	_BSD
#define	_BSD			1
#endif
#endif /* KERNEL */

/*
 * Build for dynamic loadable kernel extensions.
 */
#define AFS_DYNAMIC 1
#define AFS_VFSCACHE 1

#endif	/* _PARAM_RS_AIX31_H_ */
