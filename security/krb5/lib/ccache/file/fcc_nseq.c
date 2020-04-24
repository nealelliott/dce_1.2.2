/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_nseq.c,v $
 * Revision 1.1.8.1  1996/10/03  15:01:01  arvind
 * 	Silence compiler warning.
 * 	[1996/09/16  22:15 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.6.2  1996/02/18  00:09:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:04  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:47:08  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/30  16:32 UTC  aha
 * 	CHFts14474: optimize krb credential cache
 * 
 * 	HP revision /main/aha_mothra6/4  1995/06/29  13:48 UTC  aha
 * 	CHFts14474: optimize krb cache;
 * 	 call krb5_fcc_debug_cred() if krb5_fcc_debug_nseq set.
 * 
 * 	HP revision /main/aha_mothra6/3  1995/06/15  16:18 UTC  aha
 * 	CHFts14474: optimize krb5 cache;
 * 	 properly include final free creds at end of next_cred_aux
 * 	 inside the if (kret != KRB5_OK) block.
 * 
 * 	HP revision /main/aha_mothra6/2  1995/06/14  17:47 UTC  aha
 * 	CHFts14474: optimize krb5 cache;
 * 	 fix spelling error.
 * 
 * 	HP revision /main/aha_mothra6/1  1995/06/14  15:56 UTC  aha
 * 	CHFts14474: optimize krb cred cache;
 * 	 add krb5_fcc_get_next_unexpired_cred(); re-org krb5_fcc_get_next_cred() so body is shared
 * 	 by krb5_fcc_get_next_unexpired_cred().
 * 	[1995/12/08  16:57:57  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:38  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:53:40  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_nseq.c,v $
 * $Author: arvind $
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
 * This file contains the source code for krb5_fcc_next_cred.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_nseq_c[] =
"$Id: fcc_nseq.c,v 1.1.8.1 1996/10/03 15:01:01 arvind Exp $";
#endif /* !lint && !SABER */

#include <errno.h>
#include "fcc.h"

#ifdef OSF_DCE
krb5_boolean krb5_fcc_debug_nseq = FALSE;
#endif

/*
 * Requires:
 * cursor is a krb5_cc_cursor originally obtained from
 * krb5_fcc_start_seq_get.
 *
 * Modifes:
 * cursor, creds
 * 
 * Effects:
 * Fills in creds with the "next" credentals structure from the cache
 * id.  The actual order the creds are returned in is arbitrary.
 * Space is allocated for the variable length fields in the
 * credentials structure, so the object returned must be passed to
#ifndef OSF_DCE
 * krb5_destroy_credential.
 *
 * The cursor is updated for the next call to krb5_fcc_next_cred.
#else
 * krb5_destroy_credential.  If the skipped flag is passed in as set,
 * expired tickets cause the next ticket to be fetched without
 * dropping the lock/open of the file.
 *
 * The cursor is updated for the next call to krb5_fcc_next_cred_aux().
#endif
 *
 * Errors:
 * system errors
 */
static
krb5_error_code
#ifdef OSF_DCE
krb5_fcc_next_cred_aux(id, cursor, creds, skip_expireds)
#else
krb5_fcc_next_cred(id, cursor, creds)
#endif
   krb5_ccache id;
   krb5_cc_cursor *cursor;
   krb5_creds *creds;
#ifdef OSF_DCE
   krb5_boolean skip_expireds;
#endif
{
     int ret;
     krb5_error_code kret;
     krb5_fcc_cursor *fcursor;
#ifdef OSF_DCE
     int curpos = 0;
     krb5_timestamp  now;

     if (skip_expireds) {
         if (krb5_timeofday(&now) != KRB5_OK)
             skip_expireds = FALSE;
     }
#endif

     memset((char *)creds, 0, sizeof(*creds));

     MAYBE_OPEN(id, FCC_OPEN_RDONLY);

     fcursor = (krb5_fcc_cursor *) *cursor;

     ret = lseek(((krb5_fcc_data *) id->data)->fd, fcursor->pos, SEEK_SET);
#ifdef OSF_DCE
     if (krb5_fcc_debug_nseq) {
         if (skip_expireds) {
             krb5_fcc_debug_cred("krb5_fcc_next_cred_aux: SEARCH SKIPPING EXPIRED", NULL, ret, 0, 0);
         } else {
             krb5_fcc_debug_cred("krb5_fcc_next_cred_aux: SEARCH ALL", NULL, ret, 0, 0);
         }
     }
#endif
     if (ret < 0) {
	 ret = krb5_fcc_interpret(errno);
	 MAYBE_CLOSE(id, ret);
	 return ret;
     }

#ifdef OSF_DCE
  do {
     if (krb5_fcc_debug_nseq) {
         curpos=lseek(((krb5_fcc_data *) id->data)->fd, 0, SEEK_CUR);
     }

     /* read the credential */
     if ((kret = krb5_fcc_read_cred(id, creds)) != KRB5_OK)
         goto lose;

     /*  If we're not skipping expired tickets, OR this ticket hasn't
      *  expired, then we've got our desired result.
      */
     if (!skip_expireds || creds->times.endtime > now) {
	 break;			/* Done looping and without errors. */
     }
     /*  Won't be needing this one...  */
     if (krb5_fcc_debug_nseq) {
         krb5_fcc_debug_cred("krb5_fcc_next_cred_aux: SKIPPING EXPIRED", creds, curpos, 0, 0);
     }
     krb5_free_cred_contents(creds);
     memset((char *)creds, 0, sizeof(*creds));     
   } while (1);			/* Errors goto out of this. */
#endif

     fcursor->pos = lseek(((krb5_fcc_data *) id->data)->fd, 0, SEEK_CUR);
     cursor = (krb5_cc_cursor *) fcursor;

#ifdef OSF_DCE
     if (krb5_fcc_debug_nseq) {
         krb5_fcc_debug_cred("krb5_fcc_next_cred_aux: RETURNING CRED", creds, curpos, 0, 0);
     }
#endif

lose:
     MAYBE_CLOSE(id, kret);		/* won't overwrite kret
					   if already set */
#ifdef OSF_DCE
     if (kret != KRB5_OK) {
         if (krb5_fcc_debug_nseq) {
             krb5_fcc_debug_cred("krb5_fcc_next_cred_aux: RETURNING ERROR", 0, curpos, kret, 0);
         }
	 krb5_free_cred_contents(creds);
     }
#else
     if (kret != KRB5_OK)
	 krb5_free_cred_contents(creds);
#endif
     return kret;
}

#ifdef OSF_DCE
/*
 * Requires:
 * cursor is a krb5_cc_cursor originally obtained from
 * krb5_fcc_start_seq_get.
 *
 * Modifes:
 * cursor, creds
 * 
 * Effects:
 * Fills in creds with the "next" credentals structure from the cache
 * id.  The actual order the creds are returned in is arbitrary.
 * Space is allocated for the variable length fields in the
 * credentials structure, so the object returned must be passed to
 * krb5_destroy_credential.
 *
 * The cursor is updated for the next call to krb5_fcc_next_cred()
 * or krb5_fcc_next_unexpired_cred().
 *
 * Errors:
 * system errors
 */
krb5_error_code
krb5_fcc_next_cred(id, cursor, creds)
   krb5_ccache id;
   krb5_cc_cursor *cursor;
   krb5_creds *creds;
{
  /* previous behavior is achieved by not skipping expired tickets. */
  return krb5_fcc_next_cred_aux(id, cursor, creds, FALSE);
}

/*
 * Requires:
 * cursor is a krb5_cc_cursor originally obtained from
 * krb5_fcc_start_seq_get.
 *
 * Modifes:
 * cursor, creds
 * 
 * Effects:
 * Fills in creds with the "next" credentals structure from the cache
 * id.  The actual order the creds are returned in is arbitrary.
 * Space is allocated for the variable length fields in the
 * credentials structure, so the object returned must be passed to
 * krb5_destroy_credential.
 *
 * The cursor is updated for the next call to krb5_fcc_next_cred()
 * or krb5_fcc_next_unexpired_cred().
 *
 * Errors:
 * system errors
 */
krb5_error_code
krb5_fcc_next_unexpired_cred(id, cursor, creds)
   krb5_ccache id;
   krb5_cc_cursor *cursor;
   krb5_creds *creds;
{  

  /* Extended, new behavior is achieved by skipping expired tickets. */
  return krb5_fcc_next_cred_aux(id, cursor, creds, TRUE);
}
#endif
