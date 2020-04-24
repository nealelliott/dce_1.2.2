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
 * Revision 1.1.6.2  1996/02/18  23:46:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:34  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:16:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:51  root]
 * 
 * Revision 1.1.4.3  1993/01/03  22:56:43  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:56:21  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:09:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:59:15  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  13:37:29  rsalz
 * 	Added as part of rpc6 drop.
 * 	[1992/04/30  22:49:24  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:07:40  devrcs
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
**      sysconf.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the HPUX platforms.
**
**
*/

#ifndef HP9000S300_H
#define HP9000S300_H

/******************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <sys/file.h>
#include <signal.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <dce/pthread_exc.h>
#include <dce/exc_handling.h>

/****************************************************************************/

/* define some macros to support Unix bstring functions */

#ifdef __hpux_rel_7
#  define bcopy(src, dst, len) memcpy((dst), (src), (len))
#  define bcmp memcmp
#  define bzero(dst, len) memset((dst), 0, (len))
#endif

/****************************************************************************/

#define NO_VOID_STAR

/******************************************************************************/

#ifndef UNIX
#define UNIX
#endif

#ifndef BSD
#define BSD
#endif

#ifndef STDARG_PRINTF
#define STDARG_PRINTF
#endif

#define srandom(seed)     srand ((int) seed)
#define random            rand 

#endif
