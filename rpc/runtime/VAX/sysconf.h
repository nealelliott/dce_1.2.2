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
 * Revision 1.1.4.2  1996/02/18  22:55:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:31  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:16:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:57:12  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:57:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:58:02  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:12:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:29:44  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:06:19  devrcs
 * 	Initial revision
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
**      ultrix_vax.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the Ultrix VAX platform
**
**
*/

#ifndef ULTRIX_VAX_H
#define ULTRIX_VAX_H

/******************************************************************************/

#include <stdio.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <time.h>
#include <dce/cma.h>
#include <dce/cma_ux.h>
#include <dce/cmalib_crtlx.h>
#include <dce/cma_px.h>
#include <dce/cma_errno.h>
#include <dce/pthread_exc.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>

#ifdef vaxc
#  define EXTERNAL      globalref
#  define GLOBAL        globaldef
#  define USE_PROTOTYPES
#endif

extern int errno;

#define NO_VOID_STAR

/*
 * This definition means the ioctl() call to get the interface
 * addresses in ipnaf_bsd.c in enumerate_interfaces() will not be
 * called. This is because Ultrix returns the same internet address
 * for all interfaces.
 */
#define NO_SIOCGIFADDR

/*
 * Define protocol sequences that are always available on ULTRIX
 * VAX platforms
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

#endif
