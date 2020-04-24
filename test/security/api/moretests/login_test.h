/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: login_test.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:48:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:26  root]
 * 
 * Revision 1.1.2.3  1993/01/29  21:12:02  cuti
 * 	Add VALID_CERT_IDENT
 * 	[1993/01/28  16:04:50  cuti]
 * 
 * Revision 1.1.2.2  1992/12/31  17:29:31  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:55:06  htf]
 * 
 * Revision 1.1  1992/01/19  04:08:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

#include <dce/nbase.h>
#include <dce/id_base.h>
#include <dce/sec_login.h>
/*#include "sec_login_pvt.h" */
/* #include <dce/misc.h> */

#include <stdio.h>

/* TSH include files */
 
#include <tsh_common.h>

#define sec_login_s_ok			0L

#define DONT_CARE               -1L

static char *last_op = "";
static int last_line = 0;

#define CALL_LOGIN(op) last_op = "op"; last_line = __LINE__; op

#define SETUP_IDENTITY		0
#define VALIDATE_IDENTITY	100
#define PURGE_CONTEXT		200
#define RELEASE_CONTEXT		300
#define GET_CURRENT_CONTEXT	400
#define GET_EXPIRATION		500
#define GET_GROUPS		600
#define SET_CONTEXT		700
#define GET_PWENT		800
#define NEWGROUPS		900
#define REFRESH_IDENTITY	1000
#define EXPORT_IMPORT_CONTEXT	1100
#define CHECK_PASSWORD	        1200
#define CERTIFY_IDENTITY	1300
#define VALID_CERT_IDENT    1400

#define BAD_OBJECT_FLAG         1
#define BAD_PASSWD_FLAG         2
                                


