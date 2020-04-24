/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aggrDesc.c,v $
 * Revision 1.1.419.1  1996/10/02  21:11:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:24  damon]
 *
 * Revision 1.1.414.4  1994/08/01  21:39:51  sasala
 * 	Merged with changes from 1.1.414.3
 * 	[1994/08/01  21:39:30  sasala]
 * 
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:08:18  sasala]
 * 
 * 	Implement partial S12Y (internationalization) for DFS commands and daemons
 * 	[1994/07/21  15:00:56  sasala]
 * 
 * Revision 1.1.416.2  1994/06/29  12:12:00  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.416.1  1994/04/28  17:44:57  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:45  mbs]
 * 
 * Revision 1.1.414.3  1994/07/13  22:30:39  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  12:12:00  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:04:45  mbs]
 * 
 * Revision 1.1.414.2  1994/06/09  14:25:26  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:23  annie]
 * 
 * Revision 1.1.414.1  1994/02/04  20:36:04  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:35  devsrc]
 * 
 * Revision 1.1.412.1  1993/12/07  17:38:11  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:12:58  jaffe]
 * 
 * Revision 1.1.3.3  1993/01/21  16:32:49  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:26:48  cjd]
 * 
 * Revision 1.1.3.2  1992/11/24  20:43:53  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:39:42  bolinger]
 * 
 * Revision 1.1  1992/01/19  02:51:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1991 Transarc Corp. -- all rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <aggrDesc.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsxagmac.h>
#include <dfsxagsvc.h>
#include <dfsxagmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xaggr/aggrDesc.c,v 1.1.419.1 1996/10/02 21:11:18 damon Exp $")

readAggrDesc(part, aggrDescp)
    char *part;
    register struct aggrDesc *aggrDescp;
{
    int fd;
    struct stat status;
    char aggrDescFile[90];
    char msg[300];

    sprintf(aggrDescFile, "%s/%s", part, AGGRDESC);
    if (stat(aggrDescFile, &status)) {
/*	sprintf(msg, "Can't stat %s file; aborting", aggrDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_STAT2_MSG, aggrDescFile, strerror(errno));
	return 0;	
    }
    fd = open(aggrDescFile, O_RDONLY);
    if (fd <= 0) {
/*	sprintf(msg, "Can't open file %s for reading; aborting", aggrDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_OPEN_FAILED2_MSG, aggrDescFile, strerror(errno));
	return 0;	
    }
    if (read(fd, aggrDescp, sizeof(struct aggrDesc)) != sizeof(struct aggrDesc)) {
/*	sprintf(msg, "Can't read %d bytes from %s; aborting", sizeof(struct aggrDesc), aggrDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_BYTES_READ2_MSG, sizeof(struct aggrDesc), aggrDescFile, strerror(errno));
	close(fd);
	return 0;
    }
    close(fd);
    return 1;
}


writeAggrDesc(part, aggrDescp)
    char *part;
    register struct aggrDesc *aggrDescp;
{
    int fd;
    char aggrDescFile[90];
    char msg[300];

    sprintf(aggrDescFile, "%s/%s", part, AGGRDESC);
    fd = open(aggrDescFile, O_CREAT | O_TRUNC, 0600);
    if (fd <= 0) {
/*	sprintf(msg, "Can't open file %s for writing; aborting:", aggrDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_WRITE_OPEN2_MSG, aggrDescFile, strerror(errno));
	return 0;	
    }
    if (write(fd, aggrDescp, sizeof(struct aggrDesc)) != sizeof(struct aggrDesc)) {
/*	sprintf(msg, "Can't write %d bytes to %s; aborting", sizeof(struct aggrDesc), aggrDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_BYTES_WRITTEN2_MSG, sizeof(struct aggrDesc), aggrDescFile, strerror(errno));
	close(fd);
	return 0;
    }
    if (fsync(fd) != 0) {
/*	sprintf(msg, "Can't fsync file %s; aborting", aggrDescFile);
 *	perror(msg);
 */
	dce_svc_printf(XAG_S_NO_FSYNC2_MSG, aggrDescFile, strerror(errno));
	close(fd);
	return 0;
    }
    close(fd);
    return 1;
}
