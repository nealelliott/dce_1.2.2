/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_policy.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:50  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:10  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:34:01  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:53  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
*/ 
/* 
 * Copyright 1991 by Apollo Computer, Inc.
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
 * src/test/security/api/moretests/tsh_policy.h, , dce_osf, dce.75d
 *   
 *	Extracted	1/4/92 16:56:21
 *	from delta on	11/13/91 14:19:31
 *
 *  $LOG$
 *
 *
*/

#include <tsh_common.h>
#include <dce/rgybase.h>
#include <dce/policy.h>

#ifndef NULL
#define NULL 0
#endif

#define NORMAL_CASES 0
#define NOT_FOUND_CASES 1
#define RGY_CASES 2
#define NOT_AUTHZ_CASES 3
#define WILDCARD_CASES  4

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define SET_POLICY			100
#define SET_POLICY_NOT_FOUND		101
#define SET_POLICY_RGY			102
#define SET_POLICY_NOT_AUTHZ		103

#define GET_POLICY			200
#define GET_POLICY_NOT_FOUND		201
#define GET_POLICY_RGY			202
#define GET_POLICY_WILDCARD		204

#define GET_EFF_POLICY			300
#define GET_EFF_POLICY_NOT_FOUND	301
#define GET_EFF_POLICY_RGY		302
#define GET_EFF_POLICY_WILDCARD		304

/* %*STOP*%  Stop translating for FILE.tshrc  */

/* Constants for use with sec_rgy rpc's */

#define POLICY_ORG_NAME (unsigned_char_p_t)"locus\0"

#define POLICY_RGY_NAME (unsigned_char_p_t)""

#define POLICY_RGY_VAL 100

#define POLICY_ORG_VAL 50

#define RGY_PASSWD_LEN 6

#define ORG_PASSWD_LEN 8

sec_rgy_plcy_t POLICY_RGY = {RGY_PASSWD_LEN, POLICY_RGY_VAL, POLICY_RGY_VAL, POLICY_RGY_VAL, 0};

sec_rgy_plcy_t POLICY_ORG = {ORG_PASSWD_LEN, POLICY_ORG_VAL, POLICY_ORG_VAL, POLICY_ORG_VAL, 0};

