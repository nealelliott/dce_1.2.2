/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ccdefname.c,v $
 * Revision 1.1.4.2  1996/02/18  00:15:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:33  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:03:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:29  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:01:01  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:17  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:50:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ccdefname.c V=2 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/ccdefname.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Return default cred. cache name.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_defname_c[] =
"$Id: ccdefname.c,v 1.1.4.2 1996/02/18 00:15:10 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>

#include <stdio.h>
#include <krb5/ext-proto.h>

char *krb5_cc_default_name()
{
    char *name = getenv ("KRB5CCNAME");
    static char *name_buf;
    
    if (name == 0) {
	if (name_buf == 0)
	    name_buf = malloc (35);
	
	sprintf(name_buf, "FILE:/tmp/krb5cc_%d", getuid());
	name = name_buf;
    }
    return name;
}
    
