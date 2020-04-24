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
 * Revision 1.1.98.2  1996/02/18  23:46:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:32  marty]
 *
 * Revision 1.1.98.1  1995/12/08  00:16:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:49  root]
 * 
 * Revision 1.1.96.1  1994/01/21  22:33:13  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:36  cbrooks]
 * 
 * Revision 1.1.8.1  1993/09/16  20:18:21  damon
 * 	Synched with ODE 2.1 based build
 * 	[1993/09/16  20:18:06  damon]
 * 
 * Revision 1.1.8.2  1993/09/15  19:57:43  tatsu_s
 * 	Bug 8103 - Enabled the atfork handler.
 * 	[1993/09/15  19:56:16  tatsu_s]
 * 
 * Revision 1.1.6.2  1993/05/24  20:47:26  cjd
 * 	Submitting 102-dme port to 103i
 * 	[1993/05/24  20:15:15  cjd]
 * 
 * Revision 1.1.4.2  1993/05/12  14:07:05  jd
 * 	Initial 486 port.
 * 	[1993/05/12  14:06:08  jd]
 * 
 * $EndLog$
 */

/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      osf/1_mips.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the osf/1 MIPS platform
**
**
*/

#ifndef _SYSCONF_H
#define _SYSCONF_H	1	

/******************************************************************************/

#include <dce/dce.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <dce/cma_px.h>
#include <dce/cma_errno.h>
#include <dce/pthread_exc.h>

/*
*
* IMPORTANT ORDER DEPENDENCY:
*
*       The <cmaxxx.h> include files above do #ifdefs on 
*       _POSIX_REENTRANT_FUNCTIONS.  Under OSF/1, this
*       is set in <unistd.h>.  The problem is that for
*       the OSF/1 DCE reference port, we need to build
*       with vanilla CMA threads and no help from the 
*       kernel.
*
*       Therefore, the include of <unistd.h> must come
*       after the <cmaxxx.h> files above.
*
*       You have been warned.
*
*/

#include <unistd.h>

#include <assert.h>
#include <fcntl.h>
#include <string.h>

/*#define NO_VOID_STAR	1*/	/* void * is supported in GCC -ansi mode*/

#define USE_PROTOTYPES  1

#define STDARG_PRINTF   1

#define NO_VARARGS_PRINTF 1

/* if SOCKADDR_LEN isn't defined MSG_MAXIOVLEN will not be defined in
 * <sys/socket.h>
 */

#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN	16
#endif /* MSG_MAXIOVLEN */

/**************************************************************************/

/*
 * This definition means the ioctl() call to get the interface
 * addresses in ipnaf_bsd.c in enumerate_interfaces() will not be
 * called. This is because Ultrix returns the same internet address
 * for all interfaces.
 */

#define NO_SIOCGIFADDR 1

/*
 * Define protocol sequences that are always available on
 * OSF/1 platforms
 */

#ifndef PROT_NCACN
#define PROT_NCACN	1
#endif

#ifndef PROT_NCADG
#define PROT_NCADG	1
#endif

#ifndef NAF_IP
#define NAF_IP	1
#endif

#define RPC_DEFAULT_NLSPATH "/usr/lib/nls/msg/en_US.ISO8859-1/%s.cat"

/****************************************************************************/

/* define some macros to support atfork handler */

#define ATFORK_SUPPORTED

#define ATFORK(handler) rpc__cma_atfork(handler)

extern void rpc__cma_atfork _DCE_PROTOTYPE_((void *));

/****************************************************************************/

#endif /* _SYSCONF_H */
