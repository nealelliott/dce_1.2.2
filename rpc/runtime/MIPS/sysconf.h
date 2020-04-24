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
 * Revision 1.1.4.2  1996/02/18  22:55:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  00:16:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:59  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:56:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:56:47  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  20:09:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:27:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:05:30  devrcs
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
**      ultrix_mips.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the Ultrix MIPS platform
**
**
*/

#ifndef ULTRIX_MIPS_H
#define ULTRIX_MIPS_H

/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
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

#define NO_VOID_STAR
#define USE_PROTOTYPES
#define NO_ELLIPSIS
#define STDARG_PRINTF

/****************************************************************************
 *
 * Forward declarations to quiet C compiler info messages.
 */

extern void bcopy();
extern void bzero();
extern int bcmp();
extern int gettimeofday();
extern pid_t getpid();
extern int ioctl();
extern int bind();
extern int listen();
extern int getsockname();
extern int setsockopt();
extern int getsockopt();
extern int getpeername();
extern long random();
extern void srandom();
extern unsigned long inet_addr();
extern int select();
extern uid_t getuid();
extern gid_t getgid();
extern int getgroups();
extern char *strcpy();
extern int strcmp();
extern char *dnet_ntoa();
extern int dnet_eof();
extern int socket();
/**************************************************************************/

/*
 * This definition means the ioctl() call to get the interface
 * addresses in ipnaf_bsd.c in enumerate_interfaces() will not be
 * called. This is because Ultrix returns the same internet address
 * for all interfaces.
 */
#define NO_SIOCGIFADDR

/*
 * Define protocol sequences that are always available on ULTRIX
 * MIPS platforms
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
