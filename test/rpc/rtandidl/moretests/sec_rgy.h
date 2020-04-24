/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_rgy.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:02  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:54  root]
 * 
 * Revision 1.1.2.2  1993/01/11  23:39:57  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:23  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright 1989 by Hewlett-Packard company 1991
 * Unpublished work.  All rights reserved.
 * 
 *   
 *	Extracted	1/4/92 16:54:12
 *	from delta on	11/8/91 16:58:19
 *
 *  $Log
 *
 *
*/

#include <stdio.h>
#include <dce/pthread_exc.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/acct.h>
#include <dce/misc.h>
#include <tsh_common.h>

#ifndef main_code
extern sec_rgy_handle_t  rgy_context;
extern char	*domain_text[sec_rgy_domain_last];
#else
sec_rgy_handle_t  rgy_context;
char	*domain_text[sec_rgy_domain_last];
#endif

#define CONTEXT (rgy_context)
#define UNIX_BASE_NUM 1000
     
static char *last_op = "";
static int last_line = 0;

#define CALL_RGY(op) last_op = "op"; last_line = __LINE__; op
   


#define PGO_ADD					0
#define PGO_GET_BY_NAME 		1
#define PGO_GET_BY_ID 	    	2
#define PGO_GET_BY_UNIX_NUM 	3
#define PGO_DELETE          	4
#define PGO_RENAME 				5	
#define PGO_REPLACE				6 
#define PGO_ADD_MEMBER      	7
#define PGO_DELETE_MEMBER   	8
#define PGO_IS_MEMBER       	9
#define PGO_GET_MEMBERS     	10
#define PGO_DELETE_NO_MEMBER    11

#define PGO_NAME_TO_ID			1
#define PGO_ID_TO_NAME			2
#define PGO_NAME_TO_UNIX_NUM	3
#define PGO_UNIX_NUM_TO_NAME	4
#define PGO_ID_TO_UNIX_NUM      5
#define PGO_UNIX_NUM_TO_ID		6


