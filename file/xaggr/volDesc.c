/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volDesc.c,v $
 * Revision 1.1.483.1  1996/10/02  21:11:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:28  damon]
 *
 * Revision 1.1.478.4  1994/08/01  21:39:56  sasala
 * 	Merged with changes from 1.1.478.3
 * 	[1994/08/01  21:39:40  sasala]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:08:22  sasala]
 * 
 * 	Implement partial S12Y (internationalization) for DFS commands and daemons
 * 	[1994/07/21  15:01:19  sasala]
 * 
 * Revision 1.1.480.2  1994/06/29  12:12:04  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.480.1  1994/04/28  17:45:05  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:50  mbs]
 * 
 * Revision 1.1.478.3  1994/07/13  22:30:44  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:12:04  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:50  mbs]
 * 
 * Revision 1.1.478.2  1994/06/09  14:25:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:31  annie]
 * 
 * Revision 1.1.478.1  1994/02/04  20:36:12  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:39  devsrc]
 * 
 * Revision 1.1.476.1  1993/12/07  17:38:19  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:14:55  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/21  16:33:17  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:27:15  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  20:44:09  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:40:01  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:51:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1991 Transarc Corp. -- all rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/common_data.h>
#include <dcedfs/queue.h>
#include <dcedfs/syscall.h>
#include <ag_init.h>
#include <astab.h>
#include <aggrDesc.h>
#include <volDesc.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsxagmac.h>
#include <dfsxagsvc.h>
#include <dfsxagmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xaggr/volDesc.c,v 1.1.483.1 1996/10/02 21:11:26 damon Exp $")

readVolDesc(part, volDescp)
    char *part;
    register struct volDesc *volDescp;
{
    int fd;
    struct stat status;
    char volDescFile[90];
    char msg[300];

    sprintf(volDescFile, "%s/%s", part, VOLDESC);
    if (stat(volDescFile, &status)) {
/*	sprintf(msg, "Can't stat file %s; aborting", volDescFile); 
 *	perror(msg); 
 */
	dce_svc_printf(XAG_S_NO_STAT_MSG, volDescFile, strerror(errno));
	return 0;	
    }
    fd = open(volDescFile, O_RDONLY);
    if (fd <= 0) {
/*	sprintf(msg, "Can't open %s file for reading; aborting", volDescFile); 
 *	perror(msg); 
 */
	dce_svc_printf(XAG_S_READ_OPEN_FAILED_MSG, volDescFile, strerror(errno)); 
	return 0;	
    }
    errno = 0;
    if (read(fd, volDescp, sizeof(struct volDesc)) != sizeof(struct volDesc)) {
/*	sprintf(msg, "Can't read %d bytes from %s; aborting", sizeof(struct volDesc), volDescFile); 
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_BYTES_READ_MSG, sizeof(struct volDesc), volDescFile, strerror(errno));
	close(fd);
	return 0;
    }
    close(fd);
    return 1;
}



writeVolDesc(part, volDescp)
    char *part;
    register struct volDesc *volDescp;
{
    int fd;
    char volDescFile[90];
    char msg[300];

    sprintf(volDescFile, "%s/%s", part, VOLDESC);
    fd = open(volDescFile, O_CREAT | O_TRUNC, 0600);
    if (fd <= 0) {
/*	sprintf(msg, "Can't open file %s for writing; aborting", volDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_WRITE_OPEN_MSG, volDescFile, strerror(errno));
	return 0;	
    }
    errno = 0;
    if (write(fd, volDescp, sizeof(struct volDesc)) != sizeof(struct volDesc)) {
/*	sprintf(msg, "Can't write %d bytes to %s; aborting", sizeof(struct volDesc), volDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_BYTES_WRITTEN_MSG, sizeof(struct volDesc), volDescFile, strerror(errno));
	close(fd);
	return 0;
    }
    if (fsync(fd) != 0) {
/*	sprintf(msg, "Can't fsync file %s; aborting", volDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_FSYNC_MSG, volDescFile, strerror(errno));
	close(fd);
	return 0;
    }
    close(fd);
    return 1;
}
