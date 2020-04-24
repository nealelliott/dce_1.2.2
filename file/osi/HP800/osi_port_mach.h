/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_port_mach.h,v $
 * Revision 1.1.41.1  1996/10/02  17:57:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:19  damon]
 *
 * Revision 1.1.36.1  1994/06/09  14:14:52  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:31  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  14:50:10  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:52:03  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:32:12  jaffe
 * 	Remove duplicate HEADER and LOG entries
 * 	[1992/09/25  12:27:44  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:23:47  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    moved from HP800/osi_port_hpux.h
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:13:44  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:13:44  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    moved from HP800/osi_port_hpux.h
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1992 Transarc Corporation
 *      All rights reserved.
 */

/* HP800 port information */

#ifndef TRANSARC_OSI_PORT_MACH_H
#define	TRANSARC_OSI_PORT_MACH_H

#undef osi_ExclusiveLockNoBlock
#define osi_ExclusiveLockNoBlock(fid)	lockf((fid), F_TLOCK, 0)
#undef osi_UnLock
#define osi_UnLock(fid)			lockf((fid), F_ULOCK, 0)

/*
 * make a declaration of these functions available at any place that might use
 * the above macros
 */
extern int gettimeofday _TAKES((struct timeval *tp, struct timezone *tzp));
extern int bcmp _TAKES(( /* char *ptr1, char *ptr2, int len */ ));
extern void bcopy _TAKES(( /* char *src, char *dest, int len */ ));
extern void bzero _TAKES(( /* char *ptr, int len */ ));

#ifndef KERNEL                          /* somethings don't exist in kernel */
extern void *malloc _TAKES((size_t size));
extern void abort _TAKES(( void ));
extern void exit _TAKES(( int status ));
#endif

#endif /* TRANSARC_OSI_PORT_MACH_H */
