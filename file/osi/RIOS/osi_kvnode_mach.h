/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_kvnode_mach.h,v $
 * Revision 1.1.315.1  1996/10/02  17:59:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:29  damon]
 *
 * Revision 1.1.310.2  1994/06/09  14:16:23  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:38  annie]
 * 
 * Revision 1.1.310.1  1994/02/04  20:25:33  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:02  devsrc]
 * 
 * Revision 1.1.308.1  1993/12/07  17:30:24  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:06:38  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/21  14:51:30  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:54:17  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:51:11  jaffe
 * 	Remove duplicate HEADER and LOG entries
 * 	[1992/09/25  12:28:42  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:33:40  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_kvnode.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    don't undef IO_APPEND, it is set correctly by osi_port_mach.h
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:21:45  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:21:45  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_kvnode.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    don't undef IO_APPEND, it is set correctly by osi_port_mach.h
 * 	    cleanup for OSF1 compilation
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1992 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_KVNODE_MACH_H
#define	TRANSARC_OSI_KVNODE_MACH_H

#ifndef KERNEL

/*
 * public vnode manipulation functions
 */
extern int vn_open(), vn_create(), vn_rdwr(), vn_close(), vn_link();
extern vn_rename(), vn_remove(), getvnodefp();
extern void vn_rele(), vattr_null();

#define VN_HOLD(VP)	{ (VP)->v_count++; }
#define VN_RELE(VP)	{ vn_rele(VP); }

/*
 * The following are needed for BSD versions of mount, unmount
 * these are the generic versions.  most osi_kvnode_mach.h files will
 * redefine them.
 */
#ifndef VOP_OPEN
#define VOP_OPEN(VPP,F,C)		(*(*(VPP))->v_op->vn_open)(VPP, F, C)
#endif

#ifndef VOP_CLOSE
#define VOP_CLOSE(VP,F,C)		(*(VP)->v_op->vn_close)(VP,F,C)
#endif

#endif /* ! KERNEL */

/* The following are from <sys/flock.h> */

#define SETFLCK		2		/* Set a file lock. */
#define SLPFLCK		4		/* Wait if blocked. */

/* The following are from <fcntl.h> */

#define	FOPEN		(-1)
#define	FREAD		(O_RDONLY-FOPEN)
#define	FWRITE		(O_WRONLY-FOPEN)
#define FMASK		00374377
#define FTRUNC		O_TRUNC
#define FEXEC		O_EXEC		/* open/close for exec */
#define FMOUNT		01000000	/* open/close for device mounts */

/* macros to replace the VNOP type macroes */
#define vtognop(vp, func)	(*(vp)->v_gnode->gn_ops->func)
#define vfstovfsop(vfsp, func) 	(*(vfsp)->vfs_ops->func)

/* These are not defined here */
#undef VOP_OPEN
#undef VOP_CLOSE

#endif /* TRANSARC_OSI_KVNODE_MACH_H */
