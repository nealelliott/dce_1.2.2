/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pattrc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:49  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:08:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:22  root]
 * 
 * Revision 1.1.2.4  1993/02/03  18:21:47  cuti
 * 	Add CONTEXT_HANDLE subcase to do context handle call
 * 	[1993/02/01  22:39:21  cuti]
 * 
 * Revision 1.1.2.3  1993/01/25  18:06:41  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:42:24  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/11  23:38:12  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:11:11  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:36:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/

#define PATTR_AUTO_COMM_STAT               0
#define PATTR_COMM_STAT                    1
#define PATTR_INT_REF_IN                   2
#define PATTR_PTR_DATA                     3
#define PATTR_PTR_NON_NULL_TO_NON_NULL     4
#define PATTR_PTR_NULL_TO_NON_NULL         5
#define PATTR_PTR_DATA_TO_NULL             6 
#define CONTEXT_HANDLE					   7

