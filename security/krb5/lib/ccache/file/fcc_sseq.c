/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_sseq.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:27  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:47:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:03  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:50  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:54:41  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_sseq.c,v $
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
 * This file contains the source code for krb5_fcc_start_seq_get.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_sseq_c[] =
"$Id: fcc_sseq.c,v 1.1.6.2 1996/02/18 00:09:44 marty Exp $";
#endif /* !lint && !SABER */

#include "fcc.h"

/*
 * Effects:
 * Prepares for a sequential search of the credentials cache.
 * Returns and krb5_cc_cursor to be used with krb5_fcc_next_cred and
 * krb5_fcc_end_seq_get.
 *
 * If the cache is modified between the time of this call and the time
 * of the final krb5_fcc_end_seq_get, the results are undefined.
 *
 * Errors:
 * KRB5_CC_NOMEM
 * system errors
 */
krb5_error_code
krb5_fcc_start_seq_get(id, cursor)
   krb5_ccache id;
   krb5_cc_cursor *cursor;
{
     krb5_fcc_cursor *fcursor;
     int ret = KRB5_OK;
     
     fcursor = (krb5_fcc_cursor *) malloc(sizeof(krb5_fcc_cursor));
     if (fcursor == NULL)
	  return KRB5_CC_NOMEM;
     if (OPENCLOSE(id)) {
	  ret = krb5_fcc_open_file(id, FCC_OPEN_RDONLY);
	  if (ret) {
	      krb5_xfree(fcursor);
	      return ret;
	  }
     }
     else
	  /* seek after the version number */
	  lseek(((krb5_fcc_data *) id->data)->fd, sizeof(krb5_int16), SEEK_SET);

     /* Make sure we start reading right after the primary principal */

     krb5_fcc_skip_principal(id);
     fcursor->pos = lseek(((krb5_fcc_data *) id->data)->fd, 0, SEEK_CUR);
     *cursor = (krb5_cc_cursor) fcursor;

     MAYBE_CLOSE(id, ret);
     return ret;
}
