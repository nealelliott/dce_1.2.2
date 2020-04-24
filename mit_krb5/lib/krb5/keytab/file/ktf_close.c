/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ktf_close.c,v $
 * Revision 1.1.2.1  1996/06/05  20:52:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:51:30  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/keytab/file/ktf_close.c
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
 * "Close" a file-based keytab and invalidate the id.  This means
 * free memory hidden in the structures.
 */

#include "k5-int.h"
#include "ktfile.h"

krb5_error_code
krb5_ktfile_close(context, id)
    krb5_context context;
  krb5_keytab id;
  /*
   * This routine is responsible for freeing all memory allocated 
   * for this keytab.  There are no system resources that need
   * to be freed nor are there any open files.
   *
   * This routine should undo anything done by krb5_ktfile_resolve().
   */
{
    krb5_xfree(KTFILENAME(id));
    krb5_xfree(id->data);
    id->ops = 0;
    krb5_xfree(id);
    return (0);
}
