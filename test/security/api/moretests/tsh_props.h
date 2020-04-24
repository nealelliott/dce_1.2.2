/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_props.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:53  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:11  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:34:06  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:56  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:52  devrcs
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
 * src/test/security/api/moretests/tsh_props.h, , dce_osf, dce.75d
 *   
 *	Extracted	1/4/92 16:56:21
 *	from delta on	11/13/91 14:19:35
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

/* %*START*% Start the body of code that will get translated into the FILE.tshrc */

#define SET_PROPS			100
#define SET_PROPS_NOT_AUTHZ		101

#define GET_PROPS			200

/* %*STOP*%  Stop translating for FILE.tshrc  */

/* Constants for use with sec_rgy rpc's */

#define PROPS_VAL 100

sec_rgy_properties_t PROPS = {PROPS_VAL, PROPS_VAL, PROPS_VAL, PROPS_VAL};

