/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pipec.h,v $
 * Revision 1.1.8.2  1996/03/09  20:51:16  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:05  marty]
 *
 * Revision 1.1.8.1  1995/12/13  21:55:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:15  root]
 * 
 * Revision 1.1.6.2  1993/07/09  12:53:36  root
 * 	Initial King Kong branch
 * 	[1993/07/09  12:53:09  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:30:53  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:56:09  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:03:23  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:13:50  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 
 * Copyright 1989 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 */
/* @(#)pipec.h	1.1
 *  NIDL TEST 
 *	Extracted	1/31/91 16:32:50
 *	from delta on	1/31/91 16:30:36
 */
#ifndef LINT
static char pipec_sccs_id[] = "@(#)pipec.h	1.1";
#endif

#define PIPE_IN               0
#define PIPE_PTR_IN           1
#define PIPE_OUT              2
#define PIPE_PTR_OUT          3
#define PIPE_IN_OUT           4
#define PIPE_ORDER_ERROR      5
#define PIPE_CLOSED_ERROR     6
#define PIPE_EMPTY_ERROR      7
#define PIPE_MEMORY_ERROR     8
