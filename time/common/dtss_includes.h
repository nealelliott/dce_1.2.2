/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtss_includes.h,v $
 * Revision 1.1.37.2  1996/02/18  23:11:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:17  marty]
 *
 * Revision 1.1.37.1  1995/12/08  18:06:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:47  root]
 * 
 * Revision 1.1.35.7  1994/08/16  18:12:46  cbrooks
 * 	CR 11494
 * 	[1994/08/09  18:59:53  cbrooks]
 * 
 * Revision 1.1.35.6  1994/05/16  18:51:37  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:08  rhw]
 * 
 * Revision 1.1.35.5  1994/05/12  15:17:06  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:24  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:24  rhw]
 * 
 * Revision 1.1.35.4  1994/04/21  22:05:05  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:13  cbrooks]
 * 
 * Revision 1.1.35.3  1994/04/07  10:22:34  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:20  kevins]
 * 
 * Revision 1.1.35.2  1994/01/21  21:40:16  rsalz
 * 	Merged with changes from 1.1.35.1
 * 	[1994/01/21  21:39:49  rsalz]
 * 
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:24  rsalz]
 * 
 * Revision 1.1.35.1  1994/01/21  21:08:22  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:24  rsalz]
 * 
 * Revision 1.1.2.2  1992/12/30  20:39:00  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:07:26  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module: DTSS_INCLUDES
 */
/*
 * Copyright (c) 1989, 1990 by
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
 */
#ifndef _DTSS_INCLUDES_H
#define _DTSS_INCLUDES_H 1

/*
 * 	COMMON HEADER FILES
 */

/*
 * dtss_config.h must come first.  dtss_threads.h depends on it.  dtss_threads.h
 * must be before any system include files, so any non-thread-safe routines can
 * be replaced by their thread-safe counterparts.
 */
#include <dce/dce.h>

#include <dce/dtss_config.h>		/* Config control file */
#include <dce/dtss_threads.h>		/* thread declarations */

#include <stdio.h>			/* C standard I/O library */
#include <string.h>			/* String routines */
#include <dce/que.h>			/* portable queue facility */
#include <dce/dtssdef.h>         	/* portable DTSS defintions */
#include <dce/dtssmacros.h>		/* DTSS group-wide macros */
#include <dce/sysdep.h>			/* system depdenent facility header */
#include <dce/commondef.h>		/* definitions for common routines */
#include <dce/utc_arith.h>		/* utc arithmetic */

#include <fcntl.h>			/* Open File Descriptors */
#include <errno.h>			/* error returns */
#include <dce/dtssmsg_ultrix.h>		/* unix specific defintions */

#include <dce/dcedtsmsg.h>		/* DTS DCE specific errors - generated from .idl */
#include <dce/dtss_rpc.h>		/* RPC common declarations */
#include <dce/dtss_ctrl.h>		/* Control Program <-> DTSS interface */

#endif  /* #ifndef _DTSS_INCLUDES_H */
