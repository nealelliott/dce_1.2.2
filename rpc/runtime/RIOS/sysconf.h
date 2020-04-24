/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sysconf.h,v $
 * Revision 1.1.69.1  1996/05/10  13:08:57  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/04/18  20:09 UTC  bissen
 * 	Add RPC_IS_SINGLE_THREADED
 * 	[1996/05/09  20:44:09  arvind]
 *
 * Revision 1.1.65.2  1994/06/24  15:49:21  tatsu_s
 * 	Added RPC_C_IP_UDP_MAX_LOCAL_FRAG_SIZE.
 * 	[1994/06/22  13:33:41  tatsu_s]
 * 
 * Revision 1.1.65.1  1994/01/21  22:33:29  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:46  cbrooks]
 * 
 * Revision 1.1.5.1  1993/09/16  20:18:24  damon
 * 	Synched with ODE 2.1 based build
 * 	[1993/09/16  20:18:07  damon]
 * 
 * Revision 1.1.5.2  1993/09/15  19:57:56  tatsu_s
 * 	Bug 8103 - Enabled the atfork handler.
 * 	[1993/09/15  19:56:27  tatsu_s]
 * 
 * Revision 1.1.3.5  1993/01/07  16:58:46  tom
 * 	Bug 6412 - Remove RPC_DO_SETLOCALE since setlocale should never
 * 	  be done in a library.
 * 	Add a %s format to the AIX32 default NLS path.
 * 	[1993/01/05  23:16:08  tom]
 * 
 * Revision 1.1.3.4  1993/01/03  22:57:19  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:22  bbelch]
 * 
 * Revision 1.1.3.3  1992/12/23  20:10:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:28:46  zeliff]
 * 
 * Revision 1.1.3.2  1992/12/10  15:14:09  sommerfeld
 * 	Clean up DEFAULT_NLSPATH handling.
 * 	[1992/10/26  20:58:31  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  03:05:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef _SYSCONF_H
#define _SYSCONF_H
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      RIOS/sysconf.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the Risc System/6000 AIX 3.1
**  platform
**
**
*/

/******************************************************************************/

#include <sys/errno.h>
#include <sys/time.h>
#include <stdio.h>
#include <sys/file.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <ctype.h>
#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <dce/cma_px.h>
#include <dce/cma_errno.h>
#include <dce/pthread_exc.h>
#include <signal.h>




/******************************************************************************/

#ifndef ETOOMANYREFS
#define ETOOMANYREFS	59	/* too many remote connections */
#endif

/******************************************************************************/

#define UNIX

#define STDARG_PRINTF

/*
#ifndef NAF_IP
#define NAF_IP
#endif

#define PROT_NCADG
#define PROT_NCACN
*/


#define RPC_DEFAULT_NLSPATH "/usr/lib/nls/msg/En_US/%s.cat"


/****************************************************************************/

/* define some macros to override defaults */

/*
 * Set the local fragment size to (FDDI MTU - IP/UDP header).
 * See ipnaf.h.
 */
#define RPC_C_IP_UDP_MAX_LOCAL_FRAG_SIZE        4316

/****************************************************************************/

/* define some macros to support atfork handler */

#define ATFORK_SUPPORTED

#define ATFORK(handler) rpc__cma_atfork(handler)

extern void rpc__cma_atfork(void *);

/****************************************************************************/

#define RPC_IS_SINGLE_THREADED(junk)    (! pthread_is_multithreaded_np())

#endif /* _SYSCONF_H */
