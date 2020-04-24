/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_ops.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:07  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:47:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:58  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:40  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:53:49  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_ops.c,v $
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
 * This file contains the structure krb5_fcc_ops.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_ops_c[] =
"$Id: fcc_ops.c,v 1.1.6.2 1996/02/18 00:09:33 marty Exp $";
#endif /* !lint && !SABER */


#include "fcc.h"

krb5_cc_ops krb5_fcc_ops = {
     "FILE",
     krb5_fcc_get_name,
     krb5_fcc_resolve,
     krb5_fcc_generate_new,
     krb5_fcc_initialize,
     krb5_fcc_destroy,
     krb5_fcc_close,
     krb5_fcc_store,
     krb5_fcc_retrieve,
     krb5_fcc_get_principal,
     krb5_fcc_start_seq_get,
     krb5_fcc_next_cred,
     krb5_fcc_end_seq_get,
     NULL, /* XXX krb5_fcc_remove, */
     krb5_fcc_set_flags,
};




     



