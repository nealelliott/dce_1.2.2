/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_group.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:12:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:52  root]
 * 
 * Revision 1.1.2.3  1993/01/25  19:29:28  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:46:18  bbelch]
 * 
 * Revision 1.1.2.2  1993/01/13  19:48:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:49  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:37:45  devrcs
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



/* ??? These definitions must correspond with those in the
 *	group.tshrc file.  Eventually both these #defines
 *	and the *.tsh set commands should be generated
 *	automatically from some master file.
 */

/*
 *  SubCase values for testing rpc_ns_group_mbr_add() and
 *  rpc_ns_group_mbr_remove().
 */
#define GROUP_ADD		0
#define GROUP_REPEAT_ADD	1
#define GROUP_ADD_WITH_CLEANUP	2
#define GROUP_DEL		3
#define GROUP_DEL_WITH_SETUP	4

/*
 *  Subcase values for testing rpc_ns_group_inq_{begin,next,end}().
 *  The constant name indicates the expected order of operations; the
 *  _T_ preceeds the function actually being tested.
 */
#define GROUP_T_BEGIN			0
#define GROUP_T_BEGIN_DONE		1
#define GROUP_BEGIN_T_NEXT		2
#define GROUP_BEGIN_T_NEXT_DONE		3
#define GROUP_BEGIN_NEXT_T_NEXT_DONE	4
#define GROUP_BEGIN_INVALIDATE_T_NEXT	5
#define GROUP_BEGIN_T_DONE		6
#define GROUP_BEGIN_INVALIDATE_T_DONE	7
#define GROUP_BEGIN_T_NEXT_null_DONE	8


/* SetupAction values */
#define GRP_SETUP_NOP		0	/* No special setup needed  */
#define GRP_SETUP_GROUP		1	/* Create group before test */
#define GRP_SETUP_NONGROUP	2	/* Create non-group "   "   */
#define GRP_SETUP_NULL_CONTEXT	3	/* Cause rpc_ns_group_inq_begin() to */
					/*  fail, returning a null inquiry */
					/*  context. */
