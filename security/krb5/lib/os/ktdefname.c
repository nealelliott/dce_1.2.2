/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktdefname.c,v $
 * Revision 1.1.5.2  1996/02/18  00:15:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:47  marty]
 *
 * Revision 1.1.5.1  1995/12/08  18:03:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:36  root]
 * 
 * Revision 1.1.3.3  1992/12/29  16:30:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:17:55  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  20:42:47  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:34:24  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:50:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  ktdefname.c V=2 10/24/91 //littl/prgy/krb5/lib/os
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/os/ktdefname.c,v $
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
 * Return default keytab file name.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_ktdefname_c[] =
"$Id: ktdefname.c,v 1.1.5.2 1996/02/18 00:15:26 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/osconf.h>

#include <krb5/ext-proto.h>

extern char *krb5_defkeyname;

krb5_error_code
krb5_kt_default_name(name, namesize)
char *name;
int namesize;
{
    strncpy(name, krb5_defkeyname, (size_t) namesize);
    if ((size_t) namesize < strlen(krb5_defkeyname))
	return KRB5_CONFIG_NOTENUFSPACE;
    else
	return 0;
}
    
