/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysconf.h,v $
 * Revision 1.1.6.2  1996/02/18  22:55:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:23  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:16:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:58  root]
 * 
 * Revision 1.1.4.3  1993/01/03  22:56:52  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:56:37  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:09:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:27:24  zeliff]
 * 
 * Revision 1.1.2.3  1992/05/12  21:12:45  sommerfeld
 * 	Include exc_handling, pthread_exc from the "right" place.
 * 	[1992/05/09  04:09:19  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/01  16:04:41  rsalz
 * 	 06-feb-92 sommerfeld      Fix atfork defines to allow dff to work.
 * 	[1992/05/01  00:17:30  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:09:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef HPOSF_PA_H
#define HPOSF_PA_H
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      hposf_pa.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the HP PA-RISC OSF platform
**
**
*/

/******************************************************************************/

#define _SOCKADDR_LEN
#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/file.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>

/******************************************************************************/

#include <pthread.h>
#include <dce/pthread_np.h>

/*
 * Added for OSF/1 user-space build
 */
#include <dce/exc_handling.h>
#include <dce/pthread_exc.h>


/******************************************************************************/

extern int errno;

/******************************************************************************/

/* #define UNIX !!! should this be gone? */
#if 0
#define NO_VOID_STAR
#endif

/******************************************************************************/

/*
 * Redefine atfork to fit the scheme used by the Domain pthreads library.
 */

#define ATFORK_SUPPORTED

#define ATFORK(handler) rpc__hposf_atfork(handler)

extern void rpc__hposf_atfork(void *);

/******************************************************************************/

/*
 * Added for hposf_pa build.
 */
#define pthread_addr_t any_t
#define cma_t_address pthread_addr_t
#define pthread_startroutine_t pthread_func_t
#define pthread_equal_np pthread_equal
#ifndef hp_pa
#define hp_pa
#endif

#define PTHREAD

#define NO_SIOCGIFADDR

#ifndef NAF_IP
#define NAF_IP
#endif
#ifndef PROT_NCADG
#define PROT_NCADG
#endif

/*
 * For pthreads implementations that do not allow cancels to be delivered
 * in stdio (read, write, select, etc.) this define enables a timed select
 * in the listener thread that performs a pthread_testcancel() to receive
 * cancels.
 */
#define NON_CANCELLABLE_IO

#define NO_RPC_PRINTF
#define rpc__printf printf

/*
 * the CN runtime seems to want this.
 */

#define _TIMESPEC_T_
typedef struct timespec timespec_t;
#ifndef SIGCLD
#define SIGCLD SIGCHLD
#endif
#ifndef unix
#define unix 1
#endif
#ifndef UNIX
#define UNIX 1
#endif
#if !defined(NDEBUG) && !defined(DEBUG)
#define DEBUG 1
#define MAX_DEBUG
#define RPC_MUTEX_DEBUG
#define RPC_MUTEX_STATS
#endif

/******************************************************************************/

#endif
