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
 * Revision 1.1.4.2  1996/02/18  22:55:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:16:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:02  root]
 * 
 * Revision 1.1.2.4  1993/01/03  22:57:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:03  bbelch]
 * 
 * Revision 1.1.2.3  1992/12/23  20:10:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:28:14  zeliff]
 * 
 * Revision 1.1.2.2  1992/12/10  15:13:18  sommerfeld
 * 	Clean up default NLSPATH handling.
 * 	[1992/10/26  20:57:57  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  03:13:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 */

/*
 * HISTORY
 * $Log: sysconf.h,v $
 * Revision 1.1.4.2  1996/02/18  22:55:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:26  marty]
 *
 * Revision 1.1.5.2  1996/02/18  22:14:26  marty
 * 	Update OSF copyright years
 *
 * Revision 1.1.4.1  1995/12/08  00:16:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:02  root]
 *
 * Revision 1.1.3.2  1995/12/07  23:57:02  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.2.4  1993/01/03  22:57:08  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:57:03  bbelch]
 *
 * Revision 1.1.3.2  1993/01/03  19:57:03  bbelch
 * 	Embedding copyright notice
 *
 * Revision 1.1.2.3  1992/12/23  20:10:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:28:14  zeliff]
 *
 * Revision 1.1.3.2  1992/12/23  15:28:14  zeliff
 * 	Embedding copyright notice
 *
 * Revision 1.1.2.2  1992/12/10  15:13:18  sommerfeld
 * 	Clean up default NLSPATH handling.
 * 	[1992/10/26  20:57:57  sommerfeld]
 *
 * Revision 1.1.1.2  1992/10/26  20:57:57  sommerfeld
 * 	Clean up default NLSPATH handling.
 *
 * Revision 1.1  1992/01/19  03:13:05  devrcs
 * Initial revision
 *
 * Revision 1.1.19.2  91/12/11  10:57:12  melman
 * 	Merge to dce.72
 * 	[91/12/11  10:31:51  melman]
 * 
 * Revision 1.1.17.4  91/11/19  15:23:18  weisman
 * 		Reorder include of <unistd.h> to occur after the CMA
 * 		include files.  See large comment in code.
 * 
 * 		IMPORTANT ORDER DEPENDENCY:
 * 
 * 		The <cmaxxx.h> include files above do #ifdefs on
 * 		_POSIX_REENTRANT_FUNCTIONS.  Under OSF/1, this
 * 		is set in <unistd.h>.  The problem is that for
 * 		the OSF/1 DCE reference port, we need to build
 * 		with vanilla CMA threads and no help from the
 * 		kernel.
 * 
 * 		Therefore, the include of <unistd.h> must come
 * 		after the <cmaxxx.h> files above.
 * 
 * 		You have been warned.
 * 	[91/11/19  15:22:43  weisman]
 * 
 * Revision 1.1.17.3  91/11/13  09:11:22  melman
 * 	Changed time.h to sys/time.h
 * 	Removed #define NO_ELLIPSIS.
 * 	Removed lots of forward declarations of system functions.
 * 	Changed the #define for the file from ULTRIX_MIPS_H to
 * 	PMAX_SYSCONF_H
 * 	[91/11/13  09:08:21  melman]
 * 
 * Revision 1.1.17.2  91/11/12  17:48:12  weisman
 * 	Brought up to date with dce.68/mips version.
 * 	[91/11/12  17:47:17  weisman]
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

#ifndef PMAX_SYSCONF_H
#define PMAX_SYSCONF_H

/******************************************************************************/

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

#define NO_VOID_STAR
#define USE_PROTOTYPES
#define STDARG_PRINTF
#define NO_VARARGS_PRINTF

/* if SOCKADDR_LEN isn't defined MSG_MAXIOVLEN will not be defined in
 * <sys/socket.h>
 */
#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN	16
#endif

/**************************************************************************/

/*
 * This definition means the ioctl() call to get the interface
 * addresses in ipnaf_bsd.c in enumerate_interfaces() will not be
 * called. This is because Ultrix returns the same internet address
 * for all interfaces.
 */
#define NO_SIOCGIFADDR

/*
 * Define protocol sequences that are always available on
 * OSF/1 PMAX platforms
 */

#ifndef PROT_NCACN
#define PROT_NCACN
#endif

#ifndef PROT_NCADG
#define PROT_NCADG
#endif

#ifndef NAF_IP
#define NAF_IP
#endif

#define __OSF11__		/*?? why? */
#ifndef __OSF11__
#define RPC_DEFAULT_NLSPATH "/usr/lib/nls/msg/en_US.88591/%s.cat"
#else
#define RPC_DEFAULT_NLSPATH "/usr/lib/nls/msg/en_US.ISO8859-1/%s.cat"
#endif

#endif
