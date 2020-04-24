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
 * Revision 1.1.13.2  1996/02/18  23:46:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:12  marty]
 *
 * Revision 1.1.13.1  1995/12/08  00:14:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:00  root]
 * 
 * Revision 1.1.11.1  1994/01/21  22:31:50  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:42:03  cbrooks]
 * 
 * Revision 1.1.9.6  1993/03/10  22:43:53  delgado
 * 	change rpc_printf to use dfs_icl_printf so that kprc debug
 * 	messages can use the dfs logging package.
 * 	[1993/03/10  22:43:23  delgado]
 * 
 * Revision 1.1.9.5  1993/01/14  20:58:02  markar
 * 	   OT CR 6433 fix: Increased default DG packet pool size to 256.
 * 	[1993/01/14  19:19:02  markar]
 * 
 * Revision 1.1.9.4  1993/01/03  22:35:35  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:51:17  bbelch]
 * 
 * Revision 1.1.9.3  1992/12/23  19:37:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:49:37  zeliff]
 * 
 * Revision 1.1.9.2  1992/12/10  22:28:09  marty
 * 	removed NEED_STRNCAT, NEED_STRCAT, NEED_MEMCPY for OSF 1.1.1 upgrade
 * 	[1992/10/22  14:24:03  marty]
 * 
 * Revision 1.1.2.4  1992/05/01  17:55:30  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  17:50:28  rsalz]
 * 
 * Revision 1.1.2.3  1992/04/27  18:41:37  toml
 * 	Remove #define NAF_IP and #define PROT_NCADG to quiet compiler.
 * 	[1992/04/27  18:39:33  toml]
 * 
 * Revision 1.1.2.2  1992/03/02  19:44:20  delgado
 * 	Add definition for rpc__uiomove for OSF1 to prevent tlb_miss
 * 	[1992/03/02  19:43:53  delgado]
 * 
 * Revision 1.1  1992/01/19  03:16:02  devrcs
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
**  This file contains all (well most) definitions specific to the OSF/1
**  KRPC.
**
*/

/*
 * !!!
 * Here are some things that would normally go on the compiler command line.
 * Unfortunately, gcc's cpp has a limit of 20 -D's (which we were exceeding)
 * so we provide some of these here.
 */

#define RPC_MUTEX_DEBUG
#define RPC_MUTEX_STATS
/* #define RPC_DG_LOSSY must be def'ed on cc cmd if desired - (for dglossy.c) */
#define MAX_DEBUG

#define DEBUG   1

/*
 * Do not include the autostart code in rpc_server_use_protseq_ep().
 */
#define NO_AUTOSTART_USE_PROTSEQ

/*
 * Do not allow hostnames in ipnaf.c.  The kernel has no way to translate
 * hostnames to ip addresses.
 */
#define DO_NOT_ALLOW_HOSTNAMES

/* 
 * There is no naming service available in the kernel right now.
 */
#define NO_NS

/*
 * Tower support is not present in the kernel.
 */
#define RPC_NO_TOWER_SUPPORT

/*
 * For time adjustments.
 */
#define UNIX

/*
 * For now, make everything visible to kdb
 */
#ifdef DEBUG
#  define INTERNAL
#endif

/*
 * Since OS doesn't already have this type (needed by pthread.h)...
 */
#define PTHREAD_NO_TIMESPEC

#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>         /* should probably be in comsoc_sys.h */
#include <sys/uio.h>            /* should probably be in comsoc_sys.h */
#include <sys/errno.h>
extern int errno;
#include <net/net_globals.h>
#include <kern/assert.h>

/* 
 * kernel memory allocation routines for rpcmem_krpc.h (kalloc()/kfree())
 */
#include <kern/kalloc.h>

#include <string.h>
#include <dce/ker/pthread_exc.h>

#ifndef UNIX
#  define UNIX
#endif


/*
 * Use some alternate common include files (see common/commonp.h)
 */
#define ALT_COMMON_INCLUDE  <alt_common_krpc.h>

/*
 * We provide the alternate function specific sscanf / sprintf routines.
 */
#define NO_SSCANF
#define NO_SPRINTF

/*
 * Can't use any of the available rpc__printf implementations.
 */
#define NO_RPC_PRINTF
#define rpc__printf dfs_icl_printf

/*
 * Must not call the kernel gettimeofday().
 */
#define gettimeofday    nck_gettimeofday
/*
 * must not call the kernel setsockopt()
 */

#define setsockopt      setsockopt_nck

/*
 * Modify the default number of sockets allowed (64) to something reasonable.
 */
#define RPC_C_SERVER_MAX_SOCKETS 16

/*
 * Modify some of the default dg pkt buffer allocation constants to something
 * reasonable for the kernel.  These may have to be adjusted.
 */
#define RPC_C_DG_PKT_MAX        256

/*
 * No environment to get debug switches from in the kernel :-)
 */
#define NO_GETENV

/*
 * OSF/1 uses the new, three-argument uiomove.
 */

#define rpc__uiomove(buf, len, dir, uio) ((uio)->uio_rw=(dir),(uiomove((buf), (len), (uio))))

/*
 * Misc stuff for subr_krpc.c
 */
#define NEED_ISDIGIT
#define NEED_STRSPN
#define NEED_RINDEX
#define NEED_STRRCHR
#define NEED_MEMCMP
#define NEED_MEMSET
#define NEED_ISXDIGIT
#define NEED_TOUPPER

#endif /* _SYSCONF_H */





