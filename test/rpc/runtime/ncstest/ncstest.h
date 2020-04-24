/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ncstest.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:53  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:14:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:31  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:55:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:23:00  bbelch]
 * 
 * Revision 1.1.2.3  1992/05/28  17:51:46  garyf
 * 	fix compilation warning
 * 	[1992/05/28  17:44:31  garyf]
 * 
 * Revision 1.1.2.2  1992/05/01  13:49:11  rsalz
 * 	 17-sep-91 labossiere    fix user space includes
 * 	[1992/04/30  23:23:14  rsalz]
 * 
 * Revision 1.2  1992/01/19  22:14:13  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
 */
/*
 *  OSF DCE Version 1.0 
 */

/*
 * Misc stuff to make the various compilation modes happy...
 */

#if defined (_AIX)
#include <sys/trchkid.h>
#endif

#ifndef KERNEL_TEST_DRIVER
#  include <dce/rpc.h>
#  ifndef _KERNEL
#    include <pthread.h>
#    include <dce/exc_handling.h>
#  else
#    include <dce/ker/exc_handling.h>
#  endif
#  include <dce/rpcexc.h>
#  include <dce/dce_error.h>
#endif

#ifndef NULL
#  define NULL 0
#endif

#ifdef KERNEL_TEST_DRIVER
typedef unsigned long  error_status_t;
#endif

/*
 * kernel test driver client / server test arguments.
 */

#define NCSTEST_MAX_ARGC            20
#define NCSTEST_MAX_ARGVBYTES       256

typedef struct {
    int                 argc;
    char                argvchars[NCSTEST_MAX_ARGVBYTES];
} ncstest_inargs_t;

typedef struct {
    unsigned long st;
} ncstest_outargs_t;

typedef union {
    ncstest_inargs_t  inargs;
    ncstest_outargs_t outargs;
} ncstest_args_t;

/*****************************************************************************/ 

/*
 * NCS device test jig operations for ncstest (see ncsdev.h).
 */

#ifdef KERNEL_TEST_DRIVER
#  include <sys/file.h>
#  include <sys/ioctl.h>
#  define NCSDEVICE  "/dev/ncs"
#endif

#if defined(_KERNEL) || defined(KERNEL_TEST_DRIVER)
#undef NIOC
#  ifdef OLD_STYLE_IOWR
#    define NIOC    N
#  else
#    define NIOC    'N'
#  endif

#  define NIOCNTC           _IOWR(NIOC, 9, ncstest_args_t)     /* run ncstest client */
#  define NIOCNTS           _IOWR(NIOC, 10, ncstest_args_t)    /* run ncstest server */
#endif

#ifdef _KERNEL
/*
 * The real linkage between the kernel "ncsdev" test jig and ncstest.
 */
#if !defined (_AIX)
globalref void ncstest_c_doit();
globalref void ncstest_s_doit();
#else
globalref int ncstest_c_doit();
globalref int ncstest_s_doit();
#endif
#endif

/*****************************************************************************/

/*
 * The default set of ncstest tests to include...
 */
#ifndef NCSTESTS_NO_DEFAULT_TESTS
#define NCSTEST_SIMPLE /**/
#define NCSTEST_AFS /**/
#endif

