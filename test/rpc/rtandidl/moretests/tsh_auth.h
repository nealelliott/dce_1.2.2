/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: tsh_auth.h,v $
 * Revision 1.1.6.2  1996/02/18  23:06:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:08  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:11:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:42  root]
 * 
 * Revision 1.1.4.3  1993/01/11  23:42:00  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:15:28  bbelch]
 * 
 * Revision 1.1.4.2  1992/10/14  13:27:25  cuti
 * 	Add #define VALID_DATA_NULL_SERVNAME for extra test subcase.
 * 	[1992/10/13  21:11:00  cuti]
 * 
 * Revision 1.1.2.2  1992/07/15  15:31:49  smith
 * 	added:
 * 		#define CELL_ADMIN "cell_admin"
 * 		#define ADMIN_PASSWD "-dce-"
 * 
 * 	changed:
 * 		#define BINDING_DATA_FNAME
 * 			(unsigned_char_p_t)"sofserv.binding.data"
 * 	to:
 * 		#define BINDING_DATA_FNAME
 * 			(unsigned_char_p_t)"/tmp/sofserv.binding.data"
 * 
 * 		because the sofservclient couldn't find the file.
 * 	[1992/07/15  15:30:16  smith]
 * 
 * Revision 1.1  1992/01/19  03:37:39  devrcs
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
 *
 * src/test/rpc/rtandidl/moretests/tsh_auth.h, , dce_osf, dce.75d
 *   
 *	Extracted	1/4/92 16:54:03
 *	from delta on	11/8/91 16:55:46
 *
 *  $LOG$
 *
 *
*/


#include <tsh_common.h>

#ifndef NULL
#define NULL 0
#endif

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define SET_AUTH_INFO		0
#define INQ_AUTH_INFO		1
#define IF_REG_AUTH_INFO	2
#define INQ_AUTH_CLIENT		3
#define MGMT_INQ_DEF_AUTH_LEVEL 4
#define MGMT_SET_AUTHZ_FN	5
#define SVR_REG_AUTH_INFO	6
#define INQ_AUTH_INFO_WO_SETUP	7
#define INQ_AUTH_CLIENT_WO_SETUP 8 
#define ERR_INVALID_BINDING 9
#define VALID_DATA_NULL_SERVNAME 10

#define AUTHN_SVC_NONE		0
#define AUTHN_SVC_DCE_PRIVATE	1
#define AUTHN_SVC_DCE_PUBLIC	2
#define AUTHN_SVC_DEFAULT	3
#define AUTHN_SVC_INVALID	9999
#define AUTHN_SVC_UNKNOWN	9999

#define AUTHN_LEVEL_DEFAULT     0
#define AUTHN_LEVEL_NONE	1
#define AUTHN_LEVEL_CONNECT	2
#define AUTHN_LEVEL_CALL	3
#define AUTHN_LEVEL_PKT		4
#define AUTHN_LEVEL_PKT_INTEGRITY 5
#define AUTHN_LEVEL_PKT_PRIVACY	6
#define AUTHN_LEVEL_INVALID	9999

#define AUTHZ_NONE		0
#define AUTHZ_NAME		1
#define AUTHZ_DCE		2
#define AUTHZ_INVALID		9999

#define NULL_GET_KEY_FUNC	0
#define USE_GET_KEY_FUNC	1

#define NULL_ARG_VALUE		0
#define USE_ARG_VALUE		1

#define NULL_AUTHZ_FUNC		0
#define USE_AUTHZ_FUNC		1

#define NULL_SERVNAME		0
#define USE_SERVNAME		1 
         
#define NULL_AUTHN_LEVEL	0
#define USE_AUTHN_LEVEL	    1
                
#define NULL_AUTHZ_SVC		0
#define USE_AUTHZ_SVC		1

#define NULL_AUTHN_SVC		0
#define USE_AUTNZ_SVC		1

#define NULL_AUTH_ID		0
#define USE_AUTH_ID			1



/* Constants for use with authentication rpc's */

#define CLIENT_UNIX_NUM		998
#define SERVER_UNIX_NUM     999

#define BINDING_DATA_FNAME (unsigned_char_p_t)"/tmp/sofserv.binding.data"

#define CELL_ADMIN "cell_admin"
#define ADMIN_PASSWD "-dce-"

