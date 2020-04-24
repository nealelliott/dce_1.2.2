/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: scalt.h,v $
 * Revision 1.1.8.2  1996/03/09  20:51:37  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:22  marty]
 *
 * Revision 1.1.8.1  1995/12/13  21:55:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:25  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:23:20  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:22:52  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:38:29  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:58:46  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:05:44  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:49  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 
 * Copyright 1990 by Apollo Computer, Inc.
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
/* @(#)scalt.h	1.1
 *  NIDL TEST 
 *	Extracted	1/31/91 16:33:01
 *	from delta on	1/31/91 16:31:00
 */
#ifndef LINT
static char scalt_sccs_id[] = "@(#)scalt.h	1.1";
#endif

#define SCALAR_SMALL_INT   0
#define SCALAR_USMALL_INT  1
#define SCALAR_SHORT_INT   2
#define SCALAR_USHORT_INT  3
#define SCALAR_LONG_INT    4
#define SCALAR_ULONG_INT   5
#define SCALAR_HYPER_INT   6
#define SCALAR_UHYPER_INT  7
#define SCALAR_BOOLEAN     8
#define SCALAR_BYTE        9
#define SCALAR_FLOAT      10
#define SCALAR_DOUBLE     11
#define SCALAR_CHAR       12
