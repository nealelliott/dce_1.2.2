/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: itl_stds.h,v $
 * Revision 1.1.419.1  1996/10/17  18:27:41  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:53:53  damon]
 *
 * Revision 1.1.414.1  1994/02/04  20:45:39  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:24:41  devsrc]
 * 
 * Revision 1.1.412.1  1993/12/07  17:44:52  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:29:30  jaffe]
 * 
 * Revision 1.1.2.4  1993/01/11  18:31:26  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  17:10:57  htf]
 * 
 * Revision 1.1.2.3  1992/11/24  21:45:09  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:44:56  bolinger]
 * 
 * Revision 1.1.2.2  1992/09/25  17:23:19  jaffe
 * 	Transarc delta: fred-checkin-itl 1.1
 * 	 Selected comments:
 * 	   Port of Interactive Test Language from Encina BDE to DCE pthreads.
 * 	   Initial checkin.
 * 	[1992/09/23  21:29:45  jaffe]
 * 
 * $EndLog$
 */
/*
 * itl_stds.h -- 
 *    Declares some standard things for ITL.  Needed because the FS and
 *    TP group have different standards files.
 */

#ifndef    ITL_STDS_H
#define    ITL_STDS_H

#ifdef     ITL_FS_TEST

#include <dcedfs/stds.h>

#ifdef SUNOS5
/*
 *  On Solaris, types.h defines a boolean_t, so we include it here so that
 *  the define follows.
 */
#include <sys/types.h>
#endif /* SUNOS5 */

#define boolean_t int

#ifdef     __STDC__

typedef void * pointer_t;

#else   /* __STDC__ */

typedef char * pointer_t;

#endif  /* __STDC__ */

#else   /* ITL_FS_TEST */

#include <stds/transarc.stds.h>

#endif  /* ITL_FS_TEST */

/*
 *  These should be defined in the stds.h files, but the DFS one
 *  does not define them.
 */
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#endif  /* ITL_STDS_H */
