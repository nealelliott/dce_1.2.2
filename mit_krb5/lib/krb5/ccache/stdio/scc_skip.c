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
 * $Log: scc_skip.c,v $
 * Revision 1.1.2.1  1996/06/05  20:45:33  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:46:07  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/ccache/stdio/scc_skip.c
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * This file contains the source code for reading variables from a
 * credentials cache.  These are not library-exported functions.
 */


#include "scc.h"

krb5_error_code
krb5_scc_skip_principal(context, id)
   krb5_context context;
   krb5_ccache id;
{
     krb5_error_code kret;
     krb5_principal princ;

     kret = krb5_scc_read_principal(context, id, &princ);
     if (kret != KRB5_OK)
	  return kret;

     krb5_free_principal(context, princ);
     return KRB5_OK;
}

     
