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
 * Revision 1.1.94.2  1996/07/08  18:23:25  arvind
 * 	Update OSF copyright years
 * 	[1996/02/18  22:14:21  marty]
 *
 * 	Remove Mothra specific code
 * 	HP 	[1995/10/19  15:32 UTC  jrr  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Remove HP internal code.
 * 	HP 	[1995/01/18  21:39 UTC  hart  /main/HPDCE02/3]
 *
 * 	HP 	[1995/01/17  22:18 UTC  hart  /main/HPDCE02/hart_mothra/1]
 *
 * 	for merge of HA changes for MOTHRA
 * 	HP 	[1994/12/22  19:00 UTC  tatsu_s  /main/HPDCE02/2]
 *
 * 	Submitted pthread_is_multithreaded_np() change.
 * 	HP 	[1994/12/22  18:54 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/3]
 *
 * 	Added pthread_is_multithreaded_np().
 * 	HP 	[1994/12/09  19:19 UTC  tatsu_s  /main/HPDCE02/1]
 *
 * 	Submitted rfc31.0: Single-threaded DG client and RPC_PREFERRED_PROTSEQ.
 * 	HP 	[1994/12/07  20:59 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/2]
 *
 * 	rfc31.0: Cleanup.
 * 	HP 	[1994/11/30  22:26 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/1]
 *
 * 	rfc31.0: Initial version.
 * 	[1995/12/07  23:56:55  root]
 *
 * 	Drop max private sockets back to a reasonable #
 * 	[1996/04/18  19:14 UTC  bissen  /main/DCE_1.2/2]
 *
 * Revision 1.1.94.1  1996/05/10  13:08:35  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/04/18  19:14 UTC  bissen
 * 	unifdef for single threaded client
 * 	[1996/02/29  20:43 UTC  bissen  /main/HPDCE02/bissen_st_rpc/1]
 * 
 * 	[1995/01/18  21:39 UTC  hart  /main/HPDCE02/3]
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  18:57 UTC  psn
 * 	Remove Mothra specific code
 * 	[1995/11/16  21:35 UTC  jrr  /main/HPDCE02/jrr_1.2_mothra/2]
 * 
 * 	Remove HP internal code.
 * 	[1995/10/19  15:32 UTC  jrr  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	[1995/01/18  21:39 UTC  hart  /main/HPDCE02/3]
 * 
 * 	for merge of HA changes for MOTHRA
 * 	[1995/01/17  22:18 UTC  hart  /main/HPDCE02/hart_mothra/1]
 * 
 * 	Submitted pthread_is_multithreaded_np() change.
 * 	[1994/12/22  19:00 UTC  tatsu_s  /main/HPDCE02/2]
 * 
 * 	Added pthread_is_multithreaded_np().
 * 	[1994/12/22  18:54 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/3]
 * 
 * 	Submitted rfc31.0: Single-threaded DG client and RPC_PREFERRED_PROTSEQ.
 * 	[1994/12/09  19:19 UTC  tatsu_s  /main/HPDCE02/1]
 * 
 * 	rfc31.0: Cleanup.
 * 	[1994/12/07  20:59 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/2]
 * 
 * 	rfc31.0: Initial version.
 * 	[1994/11/30  22:26 UTC  tatsu_s  /main/tatsu_s.st_dg.b0/1]
 * 
 * Revision 1.1.90.2  1994/05/27  15:35:49  tatsu_s
 * 	DG multi-buffer fragments.
 * 	[1994/04/29  18:55:37  tatsu_s]
 * 
 * Revision 1.1.90.1  1994/01/21  22:33:19  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:59:40  cbrooks]
 * 
 * Revision 1.1.7.1  1993/09/22  19:43:36  damon
 * 	Synched with ODE 2.1 based build
 * 	[1993/09/22  19:43:28  damon]
 * 
 * Revision 1.1.7.2  1993/09/14  16:47:20  tatsu_s
 * 	Bug 8103 - Enabled the atfork handler.
 * 	[1993/09/13  17:01:23  tatsu_s]
 * 
 * Revision 1.1.4.4  1993/01/03  22:56:46  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:56:27  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/23  20:09:10  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:59:31  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/10  15:09:36  sommerfeld
 * 	Clean up DEFAULT_NLSPATH definition.
 * 	[1992/10/26  20:57:10  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/01  13:37:39  rsalz
 * 	Added as part of rpc6 drop.
 * 	[1992/04/30  23:01:04  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:07:49  devrcs
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
**  This file contains all definitions specific to the HPUX hppa platform
**
**
*/

#ifndef _SYSCONF_H
#define _SYSCONF_H	1

/*****************************************************************************/

#include <dce/dce.h>

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

/* define some macros to override defaults */

#define RPC_C_SOCKET_GUESSED_RCVBUF    (8 * 1024)
#define RPC_C_SOCKET_GUESSED_SNDBUF    (8 * 1024)

#define RPC_C_SOCKET_MAX_RCVBUF (56 * 1024)
#define RPC_C_SOCKET_MAX_SNDBUF (56 * 1024)


/****************************************************************************/

/* define some macros to support atfork handler */

#define ATFORK_SUPPORTED

#define ATFORK(handler) rpc__cma_atfork(handler)

extern void rpc__cma_atfork _DCE_PROTOTYPE_ (( void * ));

/****************************************************************************/

#define	RPC_IS_SINGLE_THREADED(junk)	(! pthread_is_multithreaded_np())

#define RPC_C_DG_SOCK_MAX_PRIV_SOCKS	3

/****************************************************************************/

#ifndef UNIX
#define UNIX	1
#endif /* UNIX */

#ifndef BSD
#define BSD	1
#endif /* BSD */

#ifndef STDARG_PRINTF
#define STDARG_PRINTF	1
#endif /* STDARG_PRINTF */

#define srandom(seed)     srand ((int) seed)
#define random            rand 

#define RPC_DEFAULT_NLSPATH "/usr/lib/nls/C/%s.cat"

/**************************************************************************/
/*
 * This definition means the ioctl() call to get the interface
 * addresses in ipnaf_bsd.c in enumerate_interfaces() will be
 * called. This is because we want to returns all the addresses
 * associated with and interface.
 */
#define NO_SIOCGIFADDR 1

#endif /* _SYSCONF_H */
