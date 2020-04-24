/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: threadsafe_ultrix.c,v $
 * Revision 1.1.8.2  1996/02/17  23:34:34  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:41  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:07:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:59  root]
 * 
 * Revision 1.1.6.2  1994/08/23  20:31:59  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:32  cbrooks]
 * 
 * Revision 1.1.6.1  1994/05/12  15:17:24  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:38  rhw]
 * 
 * Revision 1.1.4.2  1992/12/30  16:34:16  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:10:44  htf]
 * 
 * Revision 1.1.2.2  1992/05/11  13:43:50  comuzzi
 * 	Removed wrappers now defined by CMA. OT defect 3232.
 * 	[1992/05/07  17:10:07  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:32:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
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
 *	This module contains jacket routines which make the selected Ultrix
 *	system calls and C run-time library routines thread-safe
 *	(thread-reentrant).  They may block the process while called, but they
 *	will return correct results.  These jackets are transparent to the
 *	calling code, so that when reentrant versions of these routines becomes
 *	available, the jacket routines here can be removed without requiring
 *	changes to the calling code.
 *
 *	This module is basically cma_stdio.c as provided by CMA, with other
 *	routines added as necessary for DTSS.  An attempt has been made to
 *	remove as many references to CMA as possible for portability.  The
 *	remaining work to be done is to replace the references to the CMA
 *	configuration macros (_CMA_PROTOTYPE_, _CMA_HARDWARE_, etc.).
 *
 *  AUTHORS:
 *
 *	Webb Scales (original CMA version)
 *	Brian Bailey (DTSS additions and bug fixes)
 *
 *  CREATION DATE:
 *
 *	29 June 1990
 *
 *  MODIFICATION HISTORY:
 *
 *	31 August 1990	Brian Bailey	Bug fixes.  Add some routines (bind,
 *	unlink, etc.)  Rename routines from cma_ to tsafe_.  Hide cma global
 *	locking calls behind macros.
 */



/* LINTLIBRARY */

/*
 *  INCLUDE FILES
 */
#include <dtss_config.h>
#include <dtss_threads.h>
#include <sys/stat.h>

#if CONFIG_THREADS && _DECTHREADS_

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

#ifdef _CMA_VARARGS_
# include <stdarg.h>
#else
# include <varargs.h>
#endif /* CONFIG_THREADS ** _DECTHREADS_ */

#ifndef SOCK_STREAM
#include <sys/types.h>
#include <sys/socket.h>
#endif

/*
 * threadsafe_ultrix.h must not be included, directly or indirectly, by this
 * file (or compilation unit) since it redefines the names of the system and
 * rtl routines that we want to call here.
 */
#ifdef _THREADSAFE_ULTRIX_H_
Error: "threadsafe_ultrix.h" has been included into "threadsafe_ultrix.c".
#endif

/*
 * GLOBAL DATA
 */

/*
 * LOCAL DATA
 */

/*
 * LOCAL MACROS
 */
#define threadsafe_simple(type, func, arg_list)  \
    { \
    type	result;		\
    BEGIN_NONREENTRANT;		\
    result = func arg_list;	\
    END_NONREENTRANT;		\
    return result;		\
    }

#define threadsafe_void(func, arg_list)  \
    { \
    BEGIN_NONREENTRANT;		\
    func arg_list;		\
    END_NONREENTRANT;		\
    return;			\
    }

/*
 * prototypes 
 */

int tsafe_bind(int, struct sockaddr *, int) ;
int tsafe_chmod(char *, mode_t) ;
int tsafe_listen(int, int) ;
int tsafe_unlink( char * );
int tsafe_ioctl(int , int, char *) ;
int tsafe_setsockopt(int, int, int, char *, int) ;

/*
 * bind() - bind a socket to a name
 */
extern int
tsafe_bind (int	s, struct sockaddr *name, int	namelen)
    {
    threadsafe_simple (int, bind, (s, name, namelen));
    }

/*
 * chmod() - change protection on a directory entry (a socket, in DTSS's case)
 */
extern int
tsafe_chmod (char	*path, mode_t	mode)
    {
    threadsafe_simple (int, chmod, (path, mode));
    }

/*
 * listen() - listen for connections on a socket
 */
extern int
tsafe_listen	(int	s,	 int	backlog)
    {
    threadsafe_simple (int, listen, (s, backlog));
    }

/*
 * unlink() - unlink a directory entry (a socket, in DTSS's case)
 */
extern int
tsafe_unlink	(char	*path)
    {
    threadsafe_simple (int, unlink, (path));
    }

/*
 * ioctl() - I/O control
 */
extern int
tsafe_ioctl (int d, int	request, char	*argp)
{
    threadsafe_simple (int, ioctl, (d, request, argp));
}

/*
 * setsockopt() - set options on sockets
 */
extern int
tsafe_setsockopt (int s, int level, int	optname, char *optval, int optlen)
{
    threadsafe_simple (int, setsockopt, (s, level, optname, optval, optlen));
}


#endif /* CONFIG_THREADS && _DECTHREADS_ */
