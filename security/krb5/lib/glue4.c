/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: glue4.c,v $
 * Revision 1.1.4.2  1996/02/18  00:12:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:27  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:53:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:05  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:29:06  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:09:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:49:17  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  glue4.c V=1 05/07/90 //littl/prgy/krb5/lib
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
#include <krb5/krb5.h>

krb5_data string_list[3] = {
{11, "FOO.MIT.EDU"},
{6, "jtkohl"},
};

krb5_data *princ[] = {&string_list[0], &string_list[1], 0};

krb5_data string_list2[3] = {
{11, "FOO.MIT.EDU"},
{4, "rcmd"},
{13, "lycus.mit.edu"},
};

krb5_data *princ2[] = {&string_list2[0], &string_list2[1], &string_list2[2], 0};
				   
krb5_last_req_entry lrentries[] = { {32000, 1}, {0, 3}, {10, 2} };
krb5_last_req_entry *lrfoo1[] = {&lrentries[0], &lrentries[1], &lrentries[2], 0};
