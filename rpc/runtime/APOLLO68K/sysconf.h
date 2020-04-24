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
 * Revision 1.1.6.2  1996/02/18  23:46:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:45:31  marty]
 *
 * Revision 1.1.6.1  1995/12/08  00:16:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:47  root]
 * 
 * Revision 1.1.4.3  1993/01/03  22:56:39  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:56:15  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:08:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:58:55  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  16:04:30  rsalz
 * 	 15-Nov-91 markar    moved _POSIX_THREADS_SOURCE to c_switches
 * 	[1992/05/01  00:17:04  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:05:23  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef APOLLO_M68K_H
#define APOLLO_M68K_H
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      std.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  This file contains all definitions specific to the Apollo 680x0 platform.
**
**
*/

/******************************************************************************/

/*
 * Include this early so that in the case where it defines "wrappers" (e.g.,
 * CMA #define's "open" to "cma_open"), when some later file declares a
 * prototype for a wrapped function, it ends up declaring a prototype for
 * the wrapper.
 */

#include <dce/pthread_exc.h>

/******************************************************************************/

#include <sys/types.h>
#include <sys/time.h>
#include <sys/errno.h>
#include <signal.h>
#include <sys/file.h>
#undef R_OK
#undef W_OK
#undef X_OK
#undef F_OK
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/uio.h>

/******************************************************************************/

/*
 * A more useful (Sys V -like) "assert".
 */
#ifndef NDEBUG
#  define assert(EX) if (EX) ; else _assert("EX", __FILE__, __LINE__)
   extern _assert(...);
#else
#  define assert(EX)
#endif

/******************************************************************************/

#ifndef APOLLO_GLOBAL_LIBRARY

/*
 * Dummy prototypes to quiet C compiler info messages.
 */

extern bcopy(...);
extern bzero(...);
extern bcmp(...);

extern select(...);

extern socket(...);
extern bind(...);
extern connect(...);
extern accept(...);
extern listen(...);
extern sendmsg(...);
extern recvfrom(...);
extern recvmsg(...);
extern getsockname(...);
extern getpeername(...);
extern setsockopt(...);
extern getsockopt(...);
extern shutdown(...);
extern inet_addr(...);

extern ioctl(...);
extern fcntl(...);
extern gettimeofday(...);
extern random(...);
extern srandom(...);
extern ftruncate(...);
extern fsync(...);
        
#endif

/******************************************************************************/

#include <dce/exc_handling.h> 

/******************************************************************************/

/*
 * Our compiler knows how to do structure equality, so define UUID_EQ to
 * do so.
 */
#define UUID_EQ(u1, u2, st) (*(st) = 0, (u1) == (u2))
 
#define STDARG_PRINTF

/******************************************************************************/

/*
 * !!! We set lower value for the DDS NAF max TSDU until we've spread
 * around the newer DDS type manager that supports the larger value.
 */
#define rpc_c_dds_max_tsdu 1024

/*
 * !!! Without some work (i.e., writing code to look up the MTUs of
 * our local interfaces) we have to base these numbers on the smallest
 * MTU among all the interfaces we support.
 */
#define rpc_c_ip_udp_max_loc_unfrg_tpdu 1240
#define rpc_c_ip_udp_max_pth_unfrg_tpdu 1240

/******************************************************************************/
/* 
 * G L O B A L   L I B R A R Y  Specific defines
 *
 * Macros for use only when building for a post sr10.3 environment.
 */
#ifdef APOLLO_GLOBAL_LIBRARY

/*
 * At least for now, select is not a cancellable function.
 */
#define NON_CANCELLABLE_IO

/******************************************************************************/
/*
 * Tell the compiler to place all static data, declared within the scope
 * of a function, in a section named nck_pure_data$.  This section will
 * be loaded as a R/O, shared, initialized data section.  All other data,
 * global or statics at the file scope, will be loaded as R/W, per-process,
 * and zero-filled.
 */

#section( , nck_pure_data$)

/******************************************************************************/
/*
 * Redefine atfork to fit the scheme used by the Domain pthreads library.
 */

#define ATFORK_SUPPORTED

#define ATFORK(handler) apollo_atfork(handler)

extern void apollo_atfork(...);

/*
 * Use the native DOMAIN/OS memory allocators (supposedly better
 * and/or faster than cma_{malloc,free}).
 *
 * We want to but can't #include <apollo/base.h> because of conflicts
 * with "nbase.idl" and friends.  All we really need is status_$t, so
 * we snag it into here.  status_$t is needed by <apollo/rws.h>.
 */

typedef struct {long all;} status_$t;
#include <apollo/rws.h>

/******************************************************************************/
      
#ifdef malloc
#undef malloc
#endif

#define malloc(n) \
      rws_$alloc_heap_pool(rws_$std_pool, (long) (n))

#ifdef free
#undef free
#endif

#define free(p) \
{ \
      status_$t status; \
      rws_$release_heap_pool((p), rws_$std_pool, &status); \
}
#endif

/******************************************************************************/

#endif
