/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_dacl.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:27  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:25:05  root
 * 	Submit
 * 	[1995/12/11  15:14:39  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:07:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_dacl.h V=3 04/29/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      Registry Server - sec_acl wire interface routines, header file
** 
*/

#ifndef rs_dacl__included
#define rs_dacl__included

#include <dce/nbase.h>

/*
 * Initialize data needed in the sec_acl wire interface routines
 */
void rs_dacl_init(
#ifdef __STDC__
error_status_t  *st_p
#endif
);

#endif  /* rs_dacl__included */

