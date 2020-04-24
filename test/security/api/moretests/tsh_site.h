/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_site.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:56  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:11  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:34:11  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:59:00  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:54  devrcs
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
 * src/test/security/api/moretests/tsh_site.h, , dce_osf, dce.75d
 *   
 *	Extracted	1/4/92 16:56:21
 *	from delta on	11/13/91 14:19:43
 *
 *  $LOG$
 *
 *
*/

#include <tsh_common.h>
#include <dce/rgybase.h>
#include <dce/binding.h>
#include <dce/sec_login.h>

#ifndef NULL
#define NULL 0
#endif

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define SITE_CLOSE			100
#define SITE_GET			101
#define SITE_IS_READONLY		102
#define SITE_OPEN			103
#define SITE_OPEN_DEFAULT		104
#define SITE_OPEN_QUERY			105
#define SITE_OPEN_UPDATE		106
#define SITE_OPEN_UPDATE_DEFAULT        107

#define SITE_BIND			200
#define SITE_BIND_DEFAULT		201
#define SITE_BIND_QUERY			202
#define SITE_BIND_UPDATE		203
#define SITE_BIND_UPDATE_DEFAULT        204
#define SITE_BIND_GET_INFO		205
#define SITE_BIND_GET_INFO_NULL_CELL_NAME	206
#define SITE_BIND_GET_INFO_NULL_SERV_NAME	207
#define SITE_BIND_GET_INFO_NULL_STRG_BIND	208
#define SITE_BIND_GET_INFO_NULL_AUTH_INFO	209

/* %*STOP*%  Stop translating for FILE.tshrc  */



#define SITE_BIND_USERNAME (unsigned_char_p_t)"flintstone"

#define SITE_BIND_PASSWD (unsigned_char_p_t)"yabadabado"

#define SITE_BIND_REALM	(unsigned_char_p_t)"master"

