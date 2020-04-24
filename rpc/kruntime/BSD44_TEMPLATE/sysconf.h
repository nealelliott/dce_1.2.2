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
 * Revision 1.1.8.2  1996/02/18  23:46:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:05  marty]
 *
 * Revision 1.1.8.1  1995/12/08  00:13:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:55:35  root]
 * 
 * Revision 1.1.6.1  1994/01/21  22:31:12  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:38:25  cbrooks]
 * 
 * Revision 1.1.4.4  1993/01/14  21:00:07  markar
 * 	    OT CR 6433 fix: Increase default DG packet pool size to 256.
 * 	[1993/01/14  19:19:40  markar]
 * 
 * Revision 1.1.4.3  1993/01/03  22:35:03  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:50:27  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:36:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:47:28  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:54:50  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  17:50:00  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:15:44  devrcs
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
**      osc_m68k.h (sysconf.h)
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all (well most) definitions specific to the 
**  OSF1 OSC 680x0 platform.  
**
**  The real scoop is that this file is for a *snap3* OSF1 m68k kernel.
**  Subsequent system specific config files should have names like:
**          sysconf_k<OS_NAME>_<HW_PLATFORM>.h
**  E.G
**      sysconf_kosf1_m68k.h, sysconf_kosf1_hppa.h, sysconf_kosf1_mips.h, 
**      sysconf_kaix_rs6000.h, sysconf_ksunos_sparc.h, ...
**
**  For a particular build, a link to the appropriate file is made
**  from "sysconf.h".
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
 * There is no naming service available in the kernel.
 */
#define NO_NS

/*
 * Tower support is not present in the kernel.
 */
#define RPC_NO_TOWER_SUPPORT

/*
 * Specify supported NAF, protseqs, and auth modules
 */
#define NAF_IP
#define PROT_NCADG
#define AUTH_NONE
#define AUTH_KRB

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
#define RPC_C_DG_PKT_MAX        256

/*
 * No environment to get debug switches from in the kernel :-)
 */
#define NO_GETENV

/*
 * Misc stuff for subr_krpc.c
 */
#define NEED_ISDIGIT
#define NEED_STRCAT
#define NEED_STRCPY
#define NEED_STRNCAT
#define NEED_STRNCMP
#define NEED_STRSPN
#define NEED_RINDEX
#define NEED_STRRCHR
#define NEED_MEMCPY
#define NEED_MEMCMP
#define NEED_MEMSET
#define NEED_ISXDIGIT

#endif /* _SYSCONF_H */

