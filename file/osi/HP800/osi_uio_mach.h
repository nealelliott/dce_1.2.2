/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_uio_mach.h,v $
 * Revision 1.1.42.1  1996/10/02  17:57:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:21  damon]
 *
 * Revision 1.1.37.1  1994/06/09  14:14:54  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:32  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  14:50:14  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:52:08  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:32:14  jaffe
 * 	Remove duplicate HEADER and LOG entries
 * 	[1992/09/25  12:27:48  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:23:55  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_uio.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:14:12  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:14:12  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_uio.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1991, 1990 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_UIO_MACH_H
#define	TRANSARC_OSI_UIO_MACH_H

#include <sys/uio.h>

#undef osi_uio_seg
#define	osi_uio_seg		uio_seg

#undef OSI_UIOSYS
#define	OSI_UIOSYS		UIOSEG_KERNEL

#undef OSI_UIOUSER
#define	OSI_UIOUSER		UIOSEG_USER


#endif /* TRANSARC_OSI_UIO_MACH_H */
