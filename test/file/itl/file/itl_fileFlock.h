/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_fileFlock.h,v $
 * Revision 1.1.14.1  1996/10/17  18:25:52  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:01  damon]
 *
 * Revision 1.1.9.2  1993/07/19  19:41:52  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:31:50  zeliff]
 * 
 * Revision 1.1.7.3  1993/07/16  21:53:59  kissel
 * 	Put HP RP work on the 1.0.3i branch.
 * 	[1993/06/21  15:05:06  kissel]
 * 
 * Revision 1.1.5.2  1993/06/04  19:04:42  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  22:43:49  kissel]
 * 
 * Revision 1.1.3.2  1993/02/26  18:01:54  kissel
 * 	Make HPUX be like OSF for flock header files.
 * 	[1993/02/26  17:57:32  kissel]
 * 
 * Revision 1.1.2.4  1993/01/13  21:08:15  shl
 * 	Transarc delta: fred-ot6406-require-lock-var-for-fcntl 1.3
 * 	  Selected comments:
 * 	    To fix the fcntl function to require a lock variable be supplied for
 * 	    locking operations.
 * 	    This is just a checkpoint.  Got a more important defect to fix.
 * 	    Put finishing touches on this delta.
 * 	    Remove external declarations of ITL variable functions.
 * 	    Add the itlFile_FlockGet() function.
 * 	    Fixed the compile problem with flock.h on pmax.
 * 	    Include fcntl.h on pmax to get definition of struct flock.
 * 	[1993/01/13  14:03:34  shl]
 * 
 * Revision 1.1.2.3  1993/01/11  18:29:57  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:08:01  htf]
 * 
 * Revision 1.1.2.2  1992/10/28  17:47:49  jaffe
 * 	New file
 * 	[1992/10/27  22:21:49  jaffe]
 * 
 * $EndLog$
 */
/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * itl_fileFlock.h -- 
 *    Includes declarations for functions and objects dealing with
 *    the struct flock data type.  Used by fcntl().
 * 
 */

#ifndef ITL_FILE_FLOCK_H
#define ITL_FILE_FLOCK_H

#if defined(__OSF1__) || defined (__hpux)
#include <fcntl.h>   /* struct flock defined here */
#else
#include <sys/flock.h>
#endif /* defined(__OSF1__) || defined (__hpux) */

#include <utils/itl/itl_stds.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_value.h>

extern void itlFile_FlockInit _TAKES((
				    void
				    ));

extern struct flock *itlFile_FlockGet _TAKES((
					      void
					      ));


#endif  /* ITL_FILE_FLOCK_H */




