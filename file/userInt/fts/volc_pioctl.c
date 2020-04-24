/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_pioctl.c,v $
 * Revision 1.1.324.1  1996/10/02  21:15:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:46  damon]
 *
 * Revision 1.1.319.4  1994/09/16  21:32:08  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/09/16  21:19:24  ruby]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 
 * Revision 1.1.319.3  1994/07/13  22:22:51  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:51:03  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:13  mbs]
 * 
 * Revision 1.1.319.2  1994/06/09  14:24:27  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:36:25  annie]
 * 
 * Revision 1.1.319.1  1994/02/04  20:35:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:05  devsrc]
 * 
 * Revision 1.1.317.1  1993/12/07  17:37:22  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  17:51:39  jaffe]
 * 
 * Revision 1.1.2.3  1993/01/21  16:30:11  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:23:44  cjd]
 * 
 * Revision 1.1.2.2  1992/11/24  20:41:37  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:37:16  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:50:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1990, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <ctype.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/aggr.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/flserver.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/compat.h>

#include <volc.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

/* this function returns TRUE (1) if pioctl's work at all */
int fts_PioctlWorks()
{/* VIOCGETCACHEPARMS is as simple as possible, taking not even a path name */
    struct afs_ioctl blob;
    register long code;
    long rslt[16];

    blob.in_size = 0;
    blob.out_size = sizeof(rslt);
    blob.out = (caddr_t) &rslt[0];
    code = pioctl(0, VIOCGETCACHEPARMS, &blob, 1);
    return (code == 0);
}

/* this function returns TRUE (1) if the file is in DFS, otherwise false (0) */
int fts_InDFS(apath, cellP)
register char *apath;
char *cellP;
{
    struct afs_ioctl blob;
    register long code;
    char cellName[300];
    struct stat sbuf;

    blob.in_size = 0;
    blob.out_size = sizeof(cellName);
    blob.out = cellName;

    code = pioctl(apath, VIOC_FILE_CELL_NAME, &blob, 1);
    if (code) {
	if (errno == ENOENT) {
	    code = stat(apath, &sbuf);
	    if (code == 0) return 0;	/* exists, but not in AFS */
	    return 1;	/* if real ENOENT, let caller discover */
	}
	return 0;		/* else (like EINVAL), forget it. */
    }
    if (cellP) strcpy(cellP, cellName);
    return 1;
}

int fts_openCellFldb(cellName)
char *cellName;
{
    char cellBuff[1024];
    register char *cellP;

    if (*cellName != '/') {
	sprintf(cellBuff, "/.../%s", cellName);
	cellP = cellBuff;
    } else {
	cellP = cellName;
    }
    return vldb_InitUbikHandle(cellP);	/* won't take redundant action */
}

int fts_PathToFid(awho, apath, fidp)
char *awho, *apath;
register struct afsFid *fidp;
{
    struct afs_ioctl blob;
    register long code;
    char cellB[500];

    blob.in_size = 0;
    blob.out_size = sizeof(afsFid);
    blob.out = (caddr_t) fidp;

    code = pioctl(apath, VIOCGETFID, &blob, 1);
    if (code) {
	code = errno;
	if (code == 0) code = EIO;
	if (code == EINVAL) {
	    dce_svc_printf(FTS_S_PIO_NO_FILESET_MSG, awho, apath);
	} else {
	    dce_svc_printf(FTS_S_PIO_NO_NAME_MSG, awho, apath, 
			   strerror(code));
	}
	return code;
    }

    /* Now get the name of the cell that the file is in, and point FTS at its FLDB. */
    blob.in_size = 0;
    blob.out_size = sizeof(cellB);
    blob.out = (caddr_t) cellB;
    code = pioctl(apath, VIOC_FILE_CELL_NAME, &blob, 1);
    if (code) {
	code = (errno ? errno : EIO);
	dce_svc_printf(FTS_S_PIO_NO_CELL_MSG, awho, apath, strerror(code));
	return code;
    }
    return fts_openCellFldb(cellB);
}
