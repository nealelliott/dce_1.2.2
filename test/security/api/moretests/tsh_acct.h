/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_acct.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:34  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:05  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:33:28  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:33  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:43  devrcs
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
 * src/test/security/api/moretests/tsh_acct.h, , dce_osf, dce.75d
 *   
 *	Extracted	1/4/92 16:56:20
 *	from delta on	11/13/91 14:19:09
 *
 *  $LOG$
 *
 *
*/

#include <tsh_common.h>
#include <dce/rgybase.h>
#include <dce/acct.h>

#ifndef NULL
#define NULL 0
#endif

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define ACCT_ADD		100
#define ACCT_ADD_2X		101
#define ACCT_ADD_NOT_MBR_GROUP	102
#define ACCT_ADD_NOT_MBR_ORG	103
#define ACCT_ADD_NOT_MBR_BOTH	104
#define ACCT_ADD_UNIQUE_KEY_PARTS 105

#define ACCT_DEL		200
#define ACCT_DEL_2X		201

#define ACCT_ADMIN_REPL		300
#define ACCT_ADMIN_REPL_UNIQUE_KEY_PARTS 301
#define ACCT_ADMIN_REPL_NOT_FOUND 302

#define ACCT_LOOKUP		400

#define ACCT_PASSWD		500
#define ACCT_PASSWD_NOT_FOUND	501

#define ACCT_RENAME		600
#define ACCT_RENAME_UNIQUE_KEY_PARTS 601
#define ACCT_RENAME_ALREADY_EXISTS 602
#define ACCT_RENAME_NOT_FOUND	603
#define ACCT_RENAME_WILDCARD	604

#define ACCT_REPL_ALL		700
#define ACCT_REPL_ALL_UNIQUE_KEY_PARTS 701
#define ACCT_REPL_ALL_NOT_FOUND 702

#define ACCT_USER_REPL		800
#define ACCT_USER_REPL_NOT_FOUND 801

/* %*STOP*%  Stop translating for FILE.tshrc  */

/* Constants for use with sec_rgy rpc's */

#define ACCT_PERSON_NAME (unsigned_char_p_t)"marnak\0"

#define ACCT_PERSON_PASSWD (unsigned_char_p_t)"softqual\0"

#define ACCT_PERSON_REPL_PASSWD (unsigned_char_p_t)"replpswd\0"

#define ACCT_GROUP_NAME	(unsigned_char_p_t)"locus\0"

#define ACCT_ORG_NAME	(unsigned_char_p_t)"quality\0"

#define ACCT_ALT_ORG_NAME (unsigned_char_p_t)"unique\0"

#define ACCT_KEY_ORG_NAME (unsigned_char_p_t)"newkey\0"

sec_rgy_login_name_t ACCT_LOGIN_NAME = {"marnak\0", "locus\0", "quality\0"};

sec_rgy_login_name_t ACCT_NEW_LOGIN_NAME = {"marsha\0", "locus\0", "quality\0"};

sec_rgy_login_name_t ACCT_ALT_LOGIN_NAME = {"marnak\0", "locus\0", "unique\0"};

sec_rgy_login_name_t ACCT_NON_LOGIN_NAME = {"marsha\0", "none\0", "none\0"};

sec_rgy_login_name_t ACCT_KEY_LOGIN_NAME = {"marnak\0", "locus\0", "newkey\0"};


