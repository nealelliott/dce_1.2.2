/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_eseq.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:49  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:46:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:53  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:31  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:52:36  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_eseq.c,v $
 * $Author: marty $
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
 * This file contains the source code for krb5_fcc_end_seq_get.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_eseq_c[] =
"$Id: fcc_eseq.c,v 1.1.6.2 1996/02/18 00:09:20 marty Exp $";
#endif /* !lint && !SABER */

#include "fcc.h"

/*
 * Requires:
 * cursor is a krb5_cc_cursor originally obtained from
 * krb5_fcc_start_seq_get.
 *
 * Modifies:
 * id, cursor
 *
 * Effects:
 * Finishes sequential processing of the file credentials ccache id,
 * and invalidates the cursor (it must never be used after this call).
 */
/* ARGSUSED */
krb5_error_code
krb5_fcc_end_seq_get(id, cursor)
   krb5_ccache id;
   krb5_cc_cursor *cursor;
{
     int kret = KRB5_OK;
     
     /* don't close; it may be left open by the caller,
	and if not, fcc_start_seq_get and/or fcc_next_cred will do the
	MAYBE_CLOSE.
     MAYBE_CLOSE(id, kret); */
     krb5_xfree((krb5_fcc_cursor *) *cursor);

     return kret;
}


