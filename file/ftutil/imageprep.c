/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: imageprep.c,v $
 * Revision 1.1.38.1  1996/10/02  17:49:02  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:38:43  damon]
 *
 * Revision 1.1.33.2  1994/06/09  14:09:34  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:24:10  annie]
 * 
 * Revision 1.1.33.1  1994/02/04  20:19:59  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:13:44  devsrc]
 * 
 * Revision 1.1.31.2  1994/01/20  18:43:17  annie
 * 	added copyright header
 * 	[1994/01/20  18:39:33  annie]
 * 
 * Revision 1.1.31.1  1993/12/07  17:26:09  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:29:27  jaffe]
 * 
 * $EndLog$
 */
/*
*/
/*
 * (C) Copyright 1993 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 */

#define AFSDEBUG_DECLARE 1	/* Declare AFS-related debug code */

#include <dcedfs/param.h>		/* General system parameters */
#include <dcedfs/sysincludes.h>		/* Basic kernel include files */
#include <dcedfs/basicincludes.h>		/* Another basic include file set */
#include <dcedfs/osi.h>
#include <pthread.h>
#ifdef AFS_AIX_ENV
#include <dce/utc.h>
/* #include <dce/utctime.h> */
#endif /* AFS_AIX_ENV */
#include <dcedfs/ftutil.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ftutil/imageprep.c,v 1.1.38.1 1996/10/02 17:49:02 damon Exp $")

EXPORT long ftutil_ImagePrep(void)
{
/*
 * Prepare the core image for volop execution in a platform-dependent fashion.
 */
#ifdef AFS_AIX_ENV
    /* Dynamic loading of kernel extensions may hold the anchor-lock. */
    /* Ensure that when somebody holds the anchor lock and waits for a fileset, */
    /* that our process (that holds a fileset offline) doesn't then wait for the anchor */
    /* lock by doing dynamic loading. */
    /* We note that the calls kutc_gettime and kutc_adjtime can be dynamically loaded. */
    struct utc myutc;
    unsigned char adjarg[32];

    (void) dts_syscall_installed();
    (void) utc_gettime(&myutc);	/* hope that this initializes its lclcalls.c */
/* Want to initialize the kernel's kutc_adjtime caller, even though we don't want to adjust anything. */
    /* It's also made more difficult since we don't have access to utctime.h, which defines the utc_adjtime
      arguments. */
    (void) utc_adjtime(999, &adjarg[0]);
    /* getresolution is supposed to be 3 */
    /* (void) utc_adjtime(getresolution, &adjarg); */
    /* might just get the resolution into adjarg.resolution */
#endif /* AFS_AIX_ENV */
    return 0;
}
