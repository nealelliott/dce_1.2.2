/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_store.c,v $
 * Revision 1.1.8.2  1996/10/03  15:01:14  arvind
 * 	Silence compiler warning.
 * 	[1996/09/16  22:15 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1996/01/03  19:14 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.1.8.1  1996/06/04  22:02:10  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1996/01/03  19:14 UTC  psn  /main/DCE_1.2/1]
 * 
 * 	CHFts14474: optimize krb credential cache
 * 	[1995/06/30  16:32 UTC  aha  /main/HPDCE02/1]
 * 
 * 	CHFts14474: optimize krb cache;
 * 	initialize "now" variable for checking expired tickets;
 * 	use krb5_fcc_read_cred() to re-read a matching ticket.
 * 	[1995/06/29  13:44 UTC  aha  /main/aha_mothra6/4]
 * 
 * 	CHFts14474: optimize Kerberos cache;
 * 	overwrite an unexpired ticket only if client, server, AND authdata match.
 * 	[1995/06/19  19:00 UTC  aha  /main/aha_mothra6/3]
 * 
 * 	CHFts14474: optimize krb54 cache;
 * 	re-align OSF_DCE conditionals.
 * 	[1995/06/14  18:10 UTC  aha  /main/aha_mothra6/2]
 * 
 * 	CHFts14474: optimize krb cred cache;
 * 	re-use matching (client/server)-paired ticket slots rather than always appending new tickets to end.
 * 	[1995/06/14  15:58 UTC  aha  /main/aha_mothra6/1]
 * 
 * Revision 1.1.6.2  1996/02/18  00:09:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:28  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:47:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/11/15  21:13 UTC  aha
 * 	CHFts16434: passwd_export sometimes gets rpc_s_auth_tkt_expired error;
 * 	change "krb5_fcc_retrieve" to "krb5_fcc_store" in debug message.
 * 	[1995/11/14  14:31 UTC  aha  /main/HPDCE02/aha_mothra12/3]
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/30  16:32 UTC  aha
 * 	CHFts14474: optimize krb credential cache
 * 
 * 	HP revision /main/aha_mothra6/4  1995/06/29  13:44 UTC  aha
 * 	CHFts14474: optimize krb cache;
 * 	 initialize "now" variable for checking expired tickets;
 * 	 use krb5_fcc_read_cred() to re-read a matching ticket.
 * 
 * 	HP revision /main/aha_mothra6/3  1995/06/19  19:00 UTC  aha
 * 	CHFts14474: optimize Kerberos cache;
 * 	 overwrite an unexpired ticket only if client, server, AND authdata match.
 * 
 * 	HP revision /main/aha_mothra6/2  1995/06/14  18:10 UTC  aha
 * 	CHFts14474: optimize krb54 cache;
 * 	 re-align OSF_DCE conditionals.
 * 
 * 	HP revision /main/aha_mothra6/1  1995/06/14  15:58 UTC  aha
 * 	CHFts14474: optimize krb cred cache;
 * 	 re-use matching (client/server)-paired ticket slots rather than always appending new tickets to end.
 * 	[1995/12/08  16:58:05  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:52  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:54:54  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_store.c,v $
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
 * This file contains the source code for krb5_fcc_store.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_store_c[] =
"$Id: fcc_store.c,v 1.1.8.2 1996/10/03 15:01:14 arvind Exp $";
#endif /* !lint && !SABER */

#include <errno.h>
#include "fcc.h"

#ifdef OSF_DCE
krb5_boolean    krb5_fcc_debug_store = FALSE;
#endif

/*
 * Modifies:
 * the file cache
 *
 * Effects:
 * stores creds in the file cred cache
 *
 * Errors:
 * system errors
 * storage failure errors
 */
krb5_error_code
krb5_fcc_store(id, creds)
   krb5_ccache id;
   krb5_creds *creds;
{
#define TCHECK(ret) if (ret != KRB5_OK) goto lose;
     krb5_error_code ret;
#ifdef OSF_DCE
     krb5_creds      filed_cred;
     krb5_cc_cursor  ccur;
     krb5_boolean    found_match = FALSE;
     off_t           match_off = 0; /* Offset in file to the outdated tkt */
     int             sizeof_creds = krb5_fcc_sizeof_cred(creds);
     int             sizeof_filed;
     krb5_timestamp  now = 0;

     if (krb5_fcc_debug_store) {
         krb5_fcc_debug_cred("krb5_fcc_store: TO BE STORED", creds, 0, 0, 0);
     }

     /* Cycle through the file, and learn whether or not
      * we need even bother with doing an insertion. Don't worry
      * about this frill if anything goes wrong.
      * Insertion (overwriting) is done only if client and server
      * match and creds size is same.  If ticket has not expired,
      * then authdata must also match.
      */
     if ((ret = krb5_fcc_start_seq_get(id, &ccur)) == KRB5_OK) {
       ret = krb5_timeofday(&now);
       if (ret == KRB5_OK) {
         do {
             /* Violate the cursor and find out where we are.  */
             match_off = ((krb5_fcc_cursor *) ccur)->pos;

             /* Read next credential */
             /* and compare client, server, size, and (if not expired) authdata */
             found_match = (!(ret = krb5_fcc_next_cred(id, &ccur, &filed_cred)))
             && krb5_principal_compare(filed_cred.client, creds->client)
             && krb5_principal_compare(filed_cred.server, creds->server)
             && (krb5_fcc_sizeof_cred(&filed_cred) == sizeof_creds)
             && ((now >= filed_cred.times.endtime)
                || authdata_match(filed_cred.authdata, creds->authdata));

             if (ret == KRB5_OK) {
                 if (krb5_fcc_debug_store) {
                     if (found_match) {
                         krb5_fcc_debug_cred("krb5_fcc_store: FOUND MATCH", &filed_cred, match_off, 0, 0);
                     } else {
                         krb5_fcc_debug_cred("krb5_fcc_store: FOUND NON-MATCH", &filed_cred, match_off, 0, 0);
                     }
                 }
                 (void) krb5_free_cred_contents(&filed_cred);
             }
         } while (!(found_match || ret == KRB5_CC_END));
       }
     }
     /*  Done with the cursor; drop it. This also allows us
      *  to reopen the file for writing, and hence get an
      *  exclusive lock on it.
      */
     (void) krb5_fcc_end_seq_get(id, &ccur);
#endif

     MAYBE_OPEN(id, FCC_OPEN_RDWR);

#ifdef OSF_DCE
     if (found_match) {
         /*  Well, we found a 'old' ticket. Now that we've got the
          *  exclusive lock on the file, double-check to see that
          *  the old ticket has remained in place during the lock juggle.
          */

         /*  Return to where the match was.  */
         ret = lseek(((krb5_fcc_data *) id->data)->fd, match_off, SEEK_SET);
         if (ret < 0) {
             MAYBE_CLOSE_IGNORE(id);
             return krb5_fcc_interpret(errno);
         }

         /*  Confirm, under the read/write lock, that the match
          *  hasn't been moved or destroyed or otherwise lost.
          */
         memset((char *)&filed_cred, 0, sizeof(filed_cred));
         found_match = (!(ret = krb5_fcc_read_cred(id, &filed_cred)))
         && krb5_principal_compare(filed_cred.client, creds->client)
         && krb5_principal_compare(filed_cred.server, creds->server)
         && ((now >= filed_cred.times.endtime)
            || authdata_match(filed_cred.authdata, creds->authdata))
         && (krb5_fcc_sizeof_cred(&filed_cred) == sizeof_creds);

         if (ret == KRB5_OK) {
             (void) krb5_free_cred_contents(&filed_cred);
         }

         if (found_match) {
             /*  Return again to the match's spot in the file.  */
             ret = lseek(((krb5_fcc_data *) id->data)->fd, match_off, SEEK_SET);
             if (ret < 0) {
                /*  This is unlikely to happen, but we'll play safe.  */
                MAYBE_CLOSE_IGNORE(id);
                return krb5_fcc_interpret(errno);
            }

            if (krb5_fcc_debug_store) {
                /*  Announce that we've taken the match path.  */
                /* CHFts16434: change msg text from "krb5_fcc_retrieve" to ..._store */
                krb5_fcc_debug_cred("krb5_fcc_store: INSERTING", creds, match_off, 0, 0);
            }
         }
     }

  if (!found_match) {
     /*  Ouch, all that work to find the match has been wasted,
      *  as the match ticket is no longer where it used to be,
      *  or was never found in the first place. Just append
      *  the ticket as we would always have done before.
      */
#endif

     /* Make sure we are writing to the end of the file */
     ret = lseek(((krb5_fcc_data *) id->data)->fd, 0, SEEK_END);
     if (ret < 0) {
	  MAYBE_CLOSE_IGNORE(id);
	  return krb5_fcc_interpret(errno);
     }
#ifdef OSF_DCE
     if (krb5_fcc_debug_store) {
        /*  Announce that we've taken the non-match path.  */
        krb5_fcc_debug_cred("krb5_fcc_store: APPENDING", creds, ret, 0, 0);
    }
 }
#endif

     ret = krb5_fcc_store_principal(id, creds->client);
     TCHECK(ret);
     ret = krb5_fcc_store_principal(id, creds->server);
     TCHECK(ret);
     ret = krb5_fcc_store_keyblock(id, &creds->keyblock);
     TCHECK(ret);
     ret = krb5_fcc_store_times(id, &creds->times);
     TCHECK(ret);
     ret = krb5_fcc_store_octet(id, creds->is_skey);
     TCHECK(ret);
     ret = krb5_fcc_store_int32(id, creds->ticket_flags);
     TCHECK(ret);
     ret = krb5_fcc_store_addrs(id, creds->addresses);
     TCHECK(ret);
     ret = krb5_fcc_store_authdata(id, creds->authdata);
     TCHECK(ret);
     ret = krb5_fcc_store_data(id, &creds->ticket);
     TCHECK(ret);
     ret = krb5_fcc_store_data(id, &creds->second_ticket);
     TCHECK(ret);

lose:
     MAYBE_CLOSE(id, ret);
     return ret;
#undef TCHECK
}
