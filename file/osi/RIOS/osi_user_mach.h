/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_user_mach.h,v $
 * Revision 1.1.322.1  1996/10/02  17:59:40  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:44:40  damon]
 *
 * Revision 1.1.317.2  1994/06/09  14:16:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:28:49  annie]
 * 
 * Revision 1.1.317.1  1994/02/04  20:25:52  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:16:09  devsrc]
 * 
 * Revision 1.1.315.1  1993/12/07  17:30:37  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  16:07:50  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/21  14:51:49  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:54:49  cjd]
 * 
 * Revision 1.1.2.3  1992/09/25  18:51:22  jaffe
 * 	Remove duplicate HEADER and LOG entries
 * 	[1992/09/25  12:29:04  jaffe]
 * 
 * Revision 1.1.2.2  1992/08/31  20:34:48  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_user.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 	[1992/08/30  03:23:38  jaffe]
 * 
 * Revision 1.1.1.2  1992/08/30  03:23:38  jaffe
 * 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
 * 	  Selected comments:
 * 	    Fixed many compiler warnings in the osi directory.
 * 	    Reworked ALL of the header files.  All files in the osi directory now
 * 	    have NO machine specific ifdefs.  All machine specific code is in the
 * 	    machine specific subdirectories.  To make this work, additional flags
 * 	    were added to the afs/param.h file so that we can tell if a particular
 * 	    platform has any additional changes for a given osi header file.
 * 	    new file for machine specific additions for the osi_user.h file.
 * 	    Corrected errors that appeared while trying to build everything on AIX3.2
 * 	    cleanup for OSF1 compilation
 * 
 * $EndLog$
 */
/* Copyright (C) 1992 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_OSI_USER_MACH_H
#define	TRANSARC_OSI_USER_MACH_H

/*
 * machine specific definitions for function to obtain user resource limits
 */
#include <sys/user.h>
#define osi_getufilelimit()     u.u_rlimit[RLIMIT_FSIZE].rlim_cur

#endif /* TRANSARC_OSI_USER_MACH_H */
