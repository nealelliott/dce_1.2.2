/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: sysconf.h,v $
 * Revision 1.1.62.2  1996/02/18  23:46:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:08  marty]
 *
 * Revision 1.1.62.1  1995/12/08  00:14:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/05  20:52 UTC  tatsu_s
 * 	Submitted the fix for CHFts14944.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/1  1995/05/03  21:25 UTC  tatsu_s
 * 	Added RPC_C_SOCKET_MAX_SND/RCVBUF.
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/16  19:51 UTC  mort
 * 	no longer need STRCAT and STRRCHR in 10.0
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 	[1995/12/07  23:55:46  root]
 * 
 * Revision 1.1.60.3  1994/06/10  20:54:14  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:00  devsrc]
 * 
 * Revision 1.1.60.2  1994/02/02  21:48:56  cbrooks
 * 	OT9855 code cleanup breaks KRPC
 * 	[1994/02/02  21:00:04  cbrooks]
 * 
 * Revision 1.1.60.1  1994/01/21  22:31:30  cbrooks
 * 	RPC Code Cleanup
 * 	[1994/01/21  20:33:05  cbrooks]
 * 
 * Revision 1.1.6.4  1993/10/28  20:54:52  robinson
 * 	Replace "DFS_UX_10" with "__hpux1000p" task.
 * 	[1993/10/28  13:40:26  robinson]
 * 
 * Revision 1.1.6.3  1993/08/27  12:59:25  tmm
 * 	UX 10 merge
 * 	[1993/08/27  12:38:35  tmm]
 * 
 * Revision 1.1.6.2  1993/07/08  09:26:50  root
 * 	Initial King Kong branch
 * 	[1993/07/08  09:26:24  root]
 * 
 * Revision 1.1.4.4  1993/05/21  19:57:12  kissel
 * 	Use the HPUX defined timespec (in <sys/timers.h>), as suggested in bug
 * 	CHFts07435.  Also, remove tests for HPUX since this is an HPUX specific file.
 * 	Finally, move <sys/socket.h> and <sys/uio.h> to comsoc_sys.h.
 * 	[1993/05/21  19:56:35  kissel]
 * 
 * Revision 1.1.4.3  1993/04/09  19:41:48  toml
 * 	Increase kernel packet pool size (rpc_c_dg_pkt_max) from 64 to 256.
 * 	[1993/03/29  22:44:21  toml]
 * 
 * Revision 1.1.4.2  1993/03/29  13:19:15  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.2.2  1992/06/18  18:35:37  tmm
 * 
 * 	06/16/92   tmm  Created from COSL drop, with some modifications.
 * 	[1992/06/18  18:31:00  tmm]
 * 
 * $EndLog$
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

/* #define DEBUG   1 */			/* get rid of this now... */

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
#define AUTH_NONE

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

#include <sys/param.h>
#include <sys/time.h>
#include <sys/timers.h>
#include <sys/errno.h>

extern int errno;	/* Needed because errno.h only defines it for non-kernel users. */

#ifdef HPUX
#include <h/assert.h> 
#else
#include <kern/assert.h>
#endif

/* 
 * kernel memory allocation routines for rpcmem_krpc.h (kalloc()/kfree())
 */

#ifdef HPUX
#include <h/malloc.h>
#else
#include <kern/kalloc.h>
#endif

#include <string.h>
#include <dce/ker/pthread_exc.h>

/****************************************************************************/

/* define some macros to override defaults */

#define RPC_C_SOCKET_MAX_RCVBUF (56 * 1024)
#define RPC_C_SOCKET_MAX_SNDBUF (56 * 1024)

/****************************************************************************/

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
#if (! __hpux1000p)
#define NEED_STRCAT
#define NEED_STRRCHR
#endif /* (! __hpux1000p) */
#define NEED_STRNCAT
#define NEED_STRSPN
#define NEED_RINDEX
#define NEED_MEMCPY
#define NEED_MEMCMP
#define NEED_MEMSET
#define NEED_MEMMOVE
#define NEED_ISXDIGIT
#define NEED_TOUPPER

/*
 * Misc. defines for portablility...
 */
#ifdef HPUX

#define	splhigh		spl6
#define uio_rw          uio_fpflags
#define	uio_segflg	uio_seg
#define	UIO_SYSSPACE	UIOSEG_KERNEL
#define POLLNORM        POLLIN
#define pkt_alloc       NCS_pkt_alloc
#define	NO_TSLEEP	1

#ifdef hp9000s300
#define	MICROTIME(tvp)	get_precise_time(tvp)

#elif defined( __hp9000s800)

#if __hpux1000p
extern struct timeval ms_gettimeofday();
#define MICROTIME(tvp)	*tvp = ms_gettimeofday();
#else /* __hpux1000p */
#define	MICROTIME(tvp)	kernel_gettimeofday(tvp)
#endif /* __hpux1000p */

#else 
#error "neither hp9000s300 nor __hp9000s800 defined!" 
#endif /* hp9000s300 */

#define assert(i)	DO_ASSERT((i),"")

/*
 * Redfine memory allocator functions to use hpux general memory
 * allocator.
 */
#define KALLOC(size)	kmalloc(size, (M_DYNAMIC), (M_WAITOK|M_ALIGN))
#define KFREE(a,s)   	kfree(a, (M_DYNAMIC))
#endif /* HPUX */

#endif /* _SYSCONF_H */

