/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: perf_c.h,v $
 * Revision 1.1.9.2  1996/02/18  23:07:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:56  marty]
 *
 * Revision 1.1.9.1  1995/12/11  20:15:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/03/06  15:47 UTC  tatsu_s
 * 	Submitted the pipe test.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s_func_test_b1/1  1994/10/10  13:49 UTC  tatsu_s
 * 	Added the pipe test.
 * 
 * 	HP revision /main/HPDCE02/1  1994/07/29  19:16 UTC  tatsu_s
 * 	Merged Mothra upto DCE 1.1 bata.
 * 	[1995/12/11  19:47:55  root]
 * 
 * Revision 1.1.4.3  1993/09/20  18:02:18  tatsu_s
 * 	Fix DSDe411738 in project "hpux.commands"
 * 	Added perf_{f}printf wrappers for {f}printf.
 * 	(Temporary fix for printf() mutex lock problem in libc_r.)
 * 	[1993/07/28  14:11:59  tatsu_s]
 * 
 * Revision 1.1.5.2  1993/01/13  19:56:40  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:24:30  bbelch]
 * 
 * Revision 1.1.2.3  1992/06/11  12:29:06  mishkin
 * 	Add decls for Zot type and object.
 * 	[1992/06/10  19:18:50  mishkin]
 * 
 * Revision 1.1.2.2  1992/05/01  15:41:10  rsalz
 * 	 08-oct-91 labossiere pthread_cancel_e 'broken cma4' workaround
 * 	 03-sep-91 mishkin   remove uuid_g_nil_uuid (not used).
 * 	 22-jul-91 mishkin   Add NilObj.
 * 	[1992/05/01  01:20:56  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:41:38  devrcs
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
**  NAME
**
**      perf_c.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Common header file used by perf client and server.
**
**
*/

#include <signal.h>
#include <stdio.h>
#include <math.h>

#include <dce/nbase.h>
#include <dce/rpc.h>

#include <perf.h>
#include <perfb.h>
#include <perfc.h>
#include <perfg.h>
#include <perfp.h>

#ifndef NO_TASKING
#  include <pthread.h>
#  include <dce/exc_handling.h>
#  ifdef BROKEN_CMA_EXC_HANDLING
#    define pthread_cancel_e  cma_e_alerted
#  endif
#endif

#include <dce/rpcexc.h>

#if defined(vms) || defined(SYS5)
#  define index strchr
#endif

extern char *error_text();

#if defined(vax) && ! (defined(vms) || defined(ultrix))
#  include <vax.h>
#  define MARSHALL_DOUBLE(d) d_to_g(d)
#  define UNMARSHALL_DOUBLE(d) g_to_d(d)
#else
#  define MARSHALL_DOUBLE(d)
#  define UNMARSHALL_DOUBLE(d)
#endif

extern uuid_old_t FooType, BarType, FooObj1, FooObj2, BarObj1, BarObj2;
extern uuid_t NilTypeObj, NilObj, ZotObj, ZotType;

extern char *authn_level_names[];
extern char *authn_names[];
extern char *authz_names[];

#define DEBUG_LEVEL   "0.1"
#define LOSSY_LEVEL   "4.99"

#ifdef CMA_INCLUDE
#define USE_PTHREAD_DELAY_NP
#endif

#if defined(__hpux) && !defined(HPDCE_PRINTF_FIX)
#define printf  perf_printf
#define fprintf  perf_fprintf
#endif /* HPDCE_PRINTF_FIX */

#ifdef USE_PTHREAD_DELAY_NP

#define SLEEP(secs) \
{ \
    struct timespec delay; \
    delay.tv_sec  = (secs); \
    delay.tv_nsec = 0; \
    pthread_delay_np(&delay); \
}

#else

#define SLEEP(secs) \
    sleep(secs)

#endif

#define VRprintf(level, stuff) \
{ \
    if (verbose >= (level)) \
        printf stuff; \
}
