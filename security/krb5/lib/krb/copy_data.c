/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: copy_data.c,v $
 * Revision 1.1.8.2  1996/02/18  00:13:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:16  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:57:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:38  root]
 * 
 * Revision 1.1.4.2  1992/12/29  15:54:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:13:27  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/09  18:20:17  sommerfeld
 * 	Avoid calling malloc(0).
 * 	[1992/07/09  02:08:49  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:51:36  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  copy_data.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/copy_data.c,v $
 * $Author: marty $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 * 
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 *
 * krb5_copy_data()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_copy_data_c[] =
"$Id: copy_data.c,v 1.1.8.2 1996/02/18 00:13:33 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

/*
 * Copy a data structure, with fresh allocation.
 */
krb5_error_code
krb5_copy_data(indata, outdata)
const krb5_data *indata;
krb5_data **outdata;
{
    krb5_data *tempdata;
    int len;
    
    if (!(tempdata = (krb5_data *)malloc(sizeof(*tempdata))))
	return ENOMEM;

    len = indata->length;
    tempdata->length = len;
    if (len == 0)
	len = 1;
    
    if (!(tempdata->data = malloc(len))) {
	xfree(tempdata);
	return ENOMEM;
    }
    memcpy((char *)tempdata->data, (char *)indata->data, tempdata->length);
    *outdata = tempdata;
    return 0;
}
