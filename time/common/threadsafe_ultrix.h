/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: threadsafe_ultrix.h,v $
 * Revision 1.1.8.2  1996/02/18  23:11:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:49  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:07:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:00  root]
 * 
 * Revision 1.1.6.1  1994/05/12  15:17:25  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:39  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  20:39:59  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:37  htf]
 * 
 * Revision 1.1.2.2  1992/05/11  13:43:58  comuzzi
 * 	Removed wrappers now defined by CMA. OT defect 3232.
 * 	[1992/05/07  17:02:54  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:32:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  Copyright (c) 1990,1991 by
 *  Digital Equipment Corporation, Maynard Massachusetts.
 *  All rights reserved.
 *
 *  This software is furnished under a license and may be used and  copied
 *  only  in  accordance  with  the  terms  of  such  license and with the
 *  inclusion of the above copyright notice.  This software or  any  other
 *  copies  thereof may not be provided or otherwise made available to any
 *  other person.  No title to and ownership of  the  software  is  hereby
 *  transferred.
 *
 *  The information in this software is subject to change  without  notice
 *  and  should  not  be  construed  as  a commitment by DIGITAL Equipment
 *  Corporation.
 *
 *  DIGITAL assumes no responsibility for the use or  reliability  of  its
 *  software on equipment which is not supplied by DIGITAL.
 */

/*
 *  FACILITY:
 *
 *	Distributed Time Synchronization Service (DTSS, DECdts)
 *
 *  ABSTRACT:
 *
 *	Header file for jacket routines which make the selected Unix system and
 *	C run-time library routines thread reentrant.  These jackets are
 *	transparent to the calling code, so that, when a reentrant version
 *	of the library becomes available, they can be dispensed with without
 *	requiring changes to the calling code.
 *
 *	This module is basically cma_stdio.h as provided by CMA, with other
 *	routines added as necessary for DTSS.  An attempt has been made to
 *	remove as many references to CMA as possible for portability.  
 *
 *  AUTHORS:
 *
 *	Webb Scales (original CMA version)
 *	Brian Bailey (DTSS additions)
 *
 *  CREATION DATE:
 *
 *	29 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	31 August 1990	Brian Bailey    Rename cma_ to tsafe_.  Add bind, chmod,
 *	listen, and unlink.
 */


#ifndef _THREADSAFE_ULTRIX_H_
#define _THREADSAFE_ULTRIX_H_

/*
 *  INCLUDE FILES
 */
#include <dtss_config.h>
#include <dtss_threads.h>

#if CONFIG_THREADS && _DECTHREADS_

#include <stdio.h>

#ifndef SOCK_STREAM
#include <sys/types.h>
#include <sys/socket.h>
#endif

/*
 * MACROS
 */

/*
 * Ultrix system call wrappers
 */

/*
#define bind		tsafe_bind
#define chmod		tsafe_chmod
#define listen		tsafe_listen
#define unlink		tsafe_unlink
#define ioctl		tsafe_ioctl
#define setsockopt	tsafe_setsockopt
*/


/*
 * INTERFACES
 */

extern int
tsafe_bind _DCE_PROTOTYPE_ ((
	int	s,
	struct sockaddr *name,
	int	namelen));

extern int
tsafe_chmod _DCE_PROTOTYPE_ ((
	char	*path,
	mode_t	mode));

extern int
tsafe_listen _DCE_PROTOTYPE_ ((
	int	s,
	int	backlog));

extern int
tsafe_unlink _DCE_PROTOTYPE_ ((
	char	*path));

extern int
tsafe_ioctl _DCE_PROTOTYPE_ ((
        int	d,
	int	request,
	char	*argp));

extern int
tsafe_setsockopt _DCE_PROTOTYPE_ ((
        int	s,
	int	level,
	int	optname,
	char	*optval,
	int	optlen));

#endif /* CONFIG_THREADS && _DECTHREADS_ */

#endif /* _THREADSAFE_ULTRIX_H_ */

