/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_acl.h,v $
 * Revision 1.1.7.2  1996/02/18  23:07:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:36  marty]
 *
 * Revision 1.1.7.1  1995/12/11  21:50:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:06  root]
 * 
 * Revision 1.1.5.3  1993/01/29  21:12:47  cuti
 * 	Change SEC_ACL_BIND_NO_OBJ to SEC_ACL_BIND_TO_ADDR
 * 	[1993/01/28  16:07:01  cuti]
 * 
 * Revision 1.1.5.2  1992/12/31  17:33:32  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:36  htf]
 * 
 * Revision 1.1.3.2  1992/05/15  18:30:34  cuti
 * 	 Add posix mask acl test.
 * 	[1992/05/14  20:33:43  cuti]
 * 
 * Revision 1.1  1992/01/19  04:08:10  devrcs
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
 * src/test/security/api/moretests/tsh_acl.h, , dce_osf, dce.75d
 *   
 *	Extracted	1/4/92 16:59:54
 *	from delta on	11/25/91 12:53:45
 *
 *  $LOG$
 *
 *
*/

#include <tsh_common.h>
#include <dce/aclbase.h>
#include <dce/daclif.h>
#include <dce/sec_login.h>
#include <dce/rgynbase.h>
#include <dce/rpc.h>

#ifndef NULL
#define NULL 0
#endif

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define SEC_ACL_BIND			100
#define SEC_ACL_BIND_TO_ADDR		101
#define SEC_ACL_BIND_GLOBAL_NAME_SP	102
#define SEC_ACL_GET_ACCESS		103
#define SEC_ACL_GET_MGR_TYPES		104
#define SEC_ACL_GET_PRINTSTRING		105
#define SEC_ACL_LOOKUP			106
#define SEC_ACL_RELEASE			107
#define SEC_ACL_REPLACE			108
#define SEC_ACL_TEST_ACCESS_TRUE	109
#define SEC_ACL_TEST_ACCESS_FALSE	110
#define SEC_ACL_TEST_ACCESS_BEHALF_TRUE	111
#define SEC_ACL_TEST_ACCESS_BEHALF_FALSE 112
#define SEC_ACL_RELEASE_NULL		113
#define SEC_ACL_GET_MGR_TYPES_SEMANTICS		114
#define SEC_ACL_CALC_MASK  			115

#define SEC_ACL_MGR_CONFIGURE		200
#define SEC_ACL_MGR_GET_ACCESS		201
#define SEC_ACL_MGR_GET_PRINTSTRING	202
#define SEC_ACL_MGR_IS_AUTHORIZED	203
#define SEC_ACL_MGR_LOOKUP		204
#define SEC_ACL_MGR_REPLACE		205

/* %*STOP*%  Stop translating for FILE.tshrc  */


#define SEC_ACL_MGR_TYPES_SIZE_AVAIL	5

#define SEC_ACL_PRINT_STR_SIZE_AVAIL	20

sec_acl_type_t SEC_ACL_TYPE = sec_acl_type_object;

