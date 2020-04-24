/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtssdef_ultrix.h,v $
 * Revision 1.1.7.2  1996/02/18  23:11:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:29  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:07:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  13:08 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:23:52  root]
 * 
 * Revision 1.1.5.3  1994/08/23  20:31:55  cbrooks
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:56:45  cbrooks]
 * 
 * 	Code Cleanup
 * 	[1994/08/19  17:52:27  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:08 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.7.2  1993/07/08  19:38:51  root
 * 	Initial King Kong branch
 * 	[1993/07/08  19:38:24  root]
 * 
 * Revision 1.1.5.2  1993/04/05  17:39:37  truitt
 * 	Recreate GAMERA branch.
 * 	[1993/04/05  17:38:41  truitt]
 * 
 * Revision 1.1.5.2  1993/03/19  14:22:24  truitt
 * 	Merge Hp_dce1_0_1 with DCE1_0_2
 * 	[1993/03/19  14:21:12  truitt]
 * 
 * Revision 1.1.1.8  1993/03/11  22:31:18  osfrcs
 * 	    Loading drop DCE1_0_2B18
 * 
 * Revision 1.1.5.2  1994/05/31  21:29:11  cbrooks
 * 	Use K_DTS_HOME_DIR, K_DTS_BINDING_FILE macros
 * 	[1994/05/13  21:01:45  cbrooks]
 * 
 * Revision 1.1.5.1  1994/04/21  22:05:09  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:16  cbrooks]
 * 
 * Revision 1.1.2.3  1992/12/30  20:39:24  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:58  htf]
 * 
 * Revision 1.1.3.2  1992/12/21  23:31:02  smythe
 * 	added home directory for core files from OSF OT 3912
 * 	[1992/12/21  19:59:40  smythe]
 * 
 * Revision 1.1.1.7  1992/09/21  18:54:03  osfrcs
 * 	    Loading drop Dce1_0_1fnl
 * 
 * Revision 1.1  1992/01/19  15:32:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * 	Module: DTSSDEF_ULTRIX.H
 */
/*
 * Copyright (c) 1989 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:	ULTRIX definition file
 *
 * ENVIRONMENT:	Ultrix 3.0 and above
 *
 * AUTHORS:     Cesar Cortes
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	August 18, 1989
 *
 */


#ifndef _DTSSDEF_ULTRIX_H
#define _DTSSDEF_ULTRIX_H

#include <time.h>		/* for struct time interval */

#define K_OS_COMMENT_CHAR	'#'	
extern int _fuzzy_wuzzy_bear ;
/*
 * System specific file names
 */

# define K_PRSFILE_NAME		"/etc/dtscp.bpt"
# define K_INACCFILE_NAME	"/var/adm/time/dts-inacc.log"
# define K_MGT_ACL_FILE		"/var/adm/time/mgt_acl"
# define K_DTS_HOME_DIR		"/var/adm/time/"
# define K_DTS_BINDING_FILE     "dtsd.binding"

/* 
 * System specific sizes 
 */

#define K_MAX_FILENAME	(1024)		/* file name size */
#define K_MAX_LOGNAME	(1024)		/* logical name size */
#define K_MAX_SOCKNAME	(255)		/* socket name size */
#define K_MAX_DEVNAME	K_MAX_SOCKNAME	/* device name size */
#define K_MAX_CHANMSG	(512)		/* channel message size */
#define K_MAX_OSMSG	(255)		/* text for status messages */
#define K_MAX_OS_DATE_STR (24)		/* date/time ascii string length */



#define IntQue Que /* this is absolutely necessary.  IntQue is a vms interlocked Que. Que
		      is defined in que.h */

#endif	/* #ifndef DTSSDEF_ULTRIX_H */
