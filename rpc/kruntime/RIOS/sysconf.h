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
 * Revision 1.1.9.2  1996/02/18  23:46:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:16  marty]
 *
 * Revision 1.1.9.1  1995/12/08  00:14:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:13  root]
 * 
 * Revision 1.1.7.1  1994/02/02  21:49:00  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  21:00:16  cbrooks]
 * 
 * Revision 1.1.5.1  1993/09/30  14:39:53  sommerfeld
 * 	Add more includes to move machine-specific ifdefs out of
 * 	rpc/runtime/krbp.h
 * 	[1993/09/28  21:38:20  sommerfeld]
 * 
 * Revision 1.1.3.4  1993/01/03  22:36:06  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:52:09  bbelch]
 * 
 * Revision 1.1.3.3  1992/12/23  19:38:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:52:52  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/26  17:31:24  jaffe
 * 	Add new code to provide better RPC performance on kernel RCS sockets.
 * 	This change was made by Carl@IBM, and is being submitted by Transarc.
 * 	[1992/10/08  14:19:06  jaffe]
 * 
 * Revision 1.1  1992/01/19  03:15:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#ifndef SYSCONF_RIOS_H
#define SYSCONF_RIOS_H
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
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
**  This file contains all (well most) definitions specific to the OSF1 DEC
**  Mips based machines (DEC 3100 and DEC 2100).
**
**  %a%private_begin
**
**  MODIFICATION HISTORY: 
**  03-may-91 labossiere  portability work... file renames, kernel mem alloc
**  18-apr-91 labossiere updates for ker3
**  28-Feb-91 hermi NDR_LOCAL_INT_REP made little_endian for
**		    the DECstation.
**  27-Feb-91 hermi changes cribbed from Jan 15, drop port.
**  hermi - set the value of DEBUG to 1 because otherwise
**		was compilation of sys/socketvars.h was
**		failing due to syntax errors. The kernel
**		also uses DEBUG - name space pollution.
**		Phew !
**  hermi - copied from osc_m68k.h .
** 
**  04-feb-91 labossiere  Add NO_AUTOSTART_USE_PROTSEQ
**  23-nov-90 nacey       Include cma_exception.h.
**  26-sep-90 nacey       Add DO_NOT_ALLOW_HOSTNAMES.
**  11-jun-90 labossiere  Base version (from apollo_m68k.h)
**
**  %a%private_end  
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
#define LOSSY
#define MAX_DEBUG
#define DEBUG 1
#define RPC_DG_PLOG
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

#include <sys/syspest.h>
#include <sys/trchkid.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/socket.h>         /* should probably be in comsoc.h */
#include <sys/errno.h>
#include <sys/intr.h>
#include <sys/lockl.h>
#include <sys/malloc.h>
#include <sys/domain.h>
#include <sys/protosw.h>
extern int errno;
extern int brkpoint();

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
#define rpc__printf printf

/*
 * Must not call the kernel gettimeofday().
 */
#define gettimeofday    nck_gettimeofday

/*
 * Modify the default number of sockets allowed (64) to something reasonable.
 */
#define RPC_C_SERVER_MAX_SOCKETS 16

/*
 * Modify some of the default dg pkt buffer allocation constants to something
 * reasonable for the kernel.  These may have to be adjusted.
 */
#define RPC_C_DG_PKT_MAX        64

/*
 * Set the socket load to handle 4 concurrent calls at once on the same
 * socket. This should provide ample receive buffering.
 */
#define RPC_C_DG_SOCK_LOAD      4

/*
 * No environment to get debug switches from in the kernel :-)
 */
#define NO_GETENV

/*
 * Misc stuff for subr_krpc.c
 */
#define NEED_ISDIGIT
#define NEED_RINDEX
#define NEED_INDEX
#define NEED_ISXDIGIT
#define NEED_TOUPPER

#define KALLOC(size) xmalloc(size, 3, kernel_heap)
#define KFREE(addr, size) xmfree(addr, kernel_heap)
#endif

