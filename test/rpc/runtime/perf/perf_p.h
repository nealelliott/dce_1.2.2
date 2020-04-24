/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: perf_p.h,v $
 * Revision 1.1.6.2  1996/02/18  23:07:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:58  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:15:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:47:57  root]
 * 
 * Revision 1.1.4.2  1993/01/13  19:56:45  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:24:36  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:41:14  rsalz
 * 	Part of rpc6 code drop.
 * 	[1992/05/01  01:21:02  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:41:40  devrcs
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
**      perf_p.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definitions of types/constants internal to performance and system
**  exerciser.
**
**
*/

extern perfg_v1_0_epv_t foo_perfg_epv, bar_perfg_epv;
extern perf_v2_0_epv_t perf_epv;

extern idl_boolean use_reserved_threads;
extern int teardown_thread_pools();


