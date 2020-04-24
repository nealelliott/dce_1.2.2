/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktf_get_na.c,v $
 * Revision 1.1.6.2  1996/02/18  00:12:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:02:37  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:55:04  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:17  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:02:06  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:56:33  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/keytab/file/ktf_get_na.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
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
 * Get the name of the file containing a file-based keytab.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_ktfile_get_name_c[] =
"$Id: ktf_get_na.c,v 1.1.6.2 1996/02/18 00:12:42 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>

#include "ktfile.h"

krb5_error_code
krb5_ktfile_get_name(id, name, len)
  krb5_keytab id;
  char *name;
  int len;
  /* 
   * This routine returns the name of the name of the file associated with
   * this file-based keytab.  name is zeroed and the filename is truncated
   * to fit in name if necessary.
   */
{
    memset(name, 0, len);
    strncpy(name, KTFILENAME(id), len);
    return(0);
}
