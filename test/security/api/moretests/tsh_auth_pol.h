/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_auth_pol.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:39  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:07  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:33:37  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:40  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:45  devrcs
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
 * src/test/security/api/moretests/tsh_auth_pol.h, , dce_osf, dce.75d
 *   
 *	Extracted	1/4/92 16:56:20
 *	from delta on	11/13/91 14:19:13
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

#define SET_AUTH_POL			100
#define SET_AUTH_POL_NOT_FOUND		101
#define SET_AUTH_POL_RGY		102
#define SET_AUTH_POL_NOT_AUTHZ		103

#define GET_AUTH_POL			200
#define GET_AUTH_POL_NOT_FOUND		201
#define GET_AUTH_POL_RGY		202
#define GET_AUTH_POL_WILDCARD		204

#define GET_EFF_AUTH_POL		300
#define GET_EFF_AUTH_POL_NOT_FOUND	301
#define GET_EFF_AUTH_POL_RGY		302
#define GET_EFF_AUTH_POL_WILDCARD	304

/* %*STOP*%  Stop translating for FILE.tshrc  */

/* Constants for use with sec_rgy rpc's */

#define PERSON_NAME_STRING "marnak\0"

#define AUTH_POL_PERSON_NAME (unsigned_char_p_t)PERSON_NAME_STRING

#define AUTH_POL_PERSON_PASSWD (unsigned_char_p_t)"qualjob1\0"

sec_rgy_login_name_t AUTH_POL_LOGIN_NAME =
                           {PERSON_NAME_STRING, "none\0", "none\0"};

sec_rgy_login_name_t AUTH_POL_LOGIN_WILDCARD =
                           {PERSON_NAME_STRING, " \0", " \0"};

sec_rgy_login_name_t AUTH_POL_LOGIN_RGY = {0};

#define AUTH_POL_RGY_VAL 100

#define AUTH_POL_USER_VAL 50

sec_rgy_plcy_auth_t AUTH_POL_RGY = {AUTH_POL_RGY_VAL, AUTH_POL_RGY_VAL};

sec_rgy_plcy_auth_t AUTH_POL_USER = {AUTH_POL_USER_VAL, AUTH_POL_USER_VAL};

