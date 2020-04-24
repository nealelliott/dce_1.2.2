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
 * $Log: ktf_add.c,v $
 * Revision 1.1.2.1  1996/06/05  20:52:18  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:51:22  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/keytab/file/ktf_add.c
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
 * krb5_ktfile_add()
 */

#include "k5-int.h"
#include "ktfile.h"

krb5_error_code
krb5_ktfile_add(context, id, entry)
   krb5_context context;
   krb5_keytab id;
   krb5_keytab_entry *entry;
{
    krb5_error_code retval;

    if (retval = krb5_ktfileint_openw(context, id))
	return retval;
    if (fseek(KTFILEP(id), 0, 2) == -1)
	return KRB5_KT_END;
    retval = krb5_ktfileint_write_entry(context, id, entry);
    krb5_ktfileint_close(context, id);
    return retval;
}
