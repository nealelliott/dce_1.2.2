/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: timing.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:11  marty]
 *
 * Revision 1.1.6.1  1995/12/11  21:52:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:54:07  root]
 * 
 * Revision 1.1.3.4  1993/03/04  20:39:49  cuti
 * 	Add defined(__hpux) in front of defined(SNI_SVR4_POSIX).  They are all POSIX.
 * 	[1993/03/04  20:02:28  cuti]
 * 
 * Revision 1.1.3.3  1992/12/31  17:36:06  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:51  htf]
 * 
 * Revision 1.1.3.2  1992/09/29  21:25:06  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/15  14:56:05  sekhar]
 * 
 * Revision 1.1  1992/01/19  04:08:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  timing.h V=3 04/29/91 //littl/prgy/src/client/tests_dir
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
** 
** TIMING: Code instrumenting package
**
**  This package allows an application to collect user, system and wall clock
**  elapsed time information.  An arbitraty number of clocks can be used
**  simultaneously.  Cumulative and immediate statistics can be obtained.
**
**  General Usage:
**
**      struct TIMINGinfo   module_foo;
**
**      module_foo = TIMINGregister("Module foo name");
**      . . .
**      TIMINGstart(module_foo);
**      . . . code to be instrumented
**      TIMINGstop(module_foo);
**
**      . . . print statistics
**      TIMINGprint(module_foo);
*/

#ifndef __TIMING_INCLUDED__
#define __TIMING_INCLUDED__

#include <sys/types.h>

#if defined(__hpux) || defined(SNI_SVR4_POSIX)
#include <sys/times.h>
#else
#include <sys/time.h>
#endif /* SNI_SVR4_POSIX */

#include <sys/resource.h>

/*
**  TIMINGregister
**      Create a timer.  Uses <blurb> as the identifying string when printing
**  statistics.
*/

struct TIMINGinfo * TIMINGregister (
#ifdef __STDC__
    char *blurb
#endif
    );


/*
**  TIMINGstart
**      Start a timer
*/

void TIMINGstart (
#ifdef __STDC__
    struct TIMINGinfo *timer
#endif
    );


/*
** TIMINGstop
**      Stop a timer.  Will print immediate statistics if TIMINGecho is set
**  to true.
*/

void TIMINGstop (
#ifdef __STDC__
    struct TIMINGinfo *timer
#endif
    );

/*
** TIMINGprint
**      Print cumulative information for this timer
*/

void TIMINGprint (
#ifdef __STDC__
    struct TIMINGinfo *timer
#endif
    );


/*
**  TIMINGecho
**      Enable/disable immediate printing of statistics upon a TIMINGstop.
*/

void TIMINGecho (
#ifdef __STDC__
    int flag
#endif
    );

#endif
