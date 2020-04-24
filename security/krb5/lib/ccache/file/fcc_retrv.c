/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_retrv.c,v $
 * Revision 1.1.8.3  1996/02/18  00:09:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:20  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:47:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/11/13  16:03 UTC  mullan_s
 * 	Merge fix for CHFts16827
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16698/1  1995/11/13  15:59 UTC  mullan_s
 * 	CHFts16827: KRB5_TC_MATCH_SRV_NAMEONLY logic broken - should not match
 * 	the realm which granted ticket.
 * 
 * 	HP revision /main/HPDCE02/3  1995/06/30  16:32 UTC  aha
 * 	CHFts14474: optimize krb credential cache
 * 
 * 	HP revision /main/HPDCE02/aha_mothra6/4  1995/06/29  13:46 UTC  aha
 * 	CHFts14474: optimize krb cache;
 * 	 add krb5_fcc_debug_cred() to display ticket and state;
 * 	 use syslog rather than fprintf in debug displays.
 * 
 * 	HP revision /main/HPDCE02/aha_mothra6/3  1995/06/19  19:00 UTC  aha
 * 	CHFts14474: optimize kerberos cache;
 * 	 export authdata_match() for use by krb5_fcc_store().
 * 
 * 	HP revision /main/HPDCE02/aha_mothra6/2  1995/06/14  18:02 UTC  aha
 * 	CHFts14474: optimize krb54 cache;
 * 	 fix indentation.
 * 
 * 	HP revision /main/HPDCE02/aha_mothra6/1  1995/06/14  15:57 UTC  aha
 * 	CHFts14474: optimize krb cred cache;
 * 	 if krb5_fcc_retrieve() query is matching times, redirect to krb5_fcc_get_next_unexpired_cred()
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/15  00:01 UTC  rps
 * 	merge up to nightly
 * 	[1995/04/10  17:43 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:11 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat2/1  1995/03/20  14:42 UTC  mullan_s
 * 	authdata_match: if DCE authdata, call specialized match routine
 * 	to recognize if a V1 PAC has been placed in authdata.
 * 	[1995/12/08  16:58:01  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:44  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:54:12  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_retrv.c,v $
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
 * This file contains the source code for krb5_fcc_retrieve.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_retrv_c[] =
"$Id: fcc_retrv.c,v 1.1.8.3 1996/02/18 00:09:39 marty Exp $";
#endif /* !lint && !SABER */


#include "fcc.h"
#ifdef OSF_DCE
#include <syslog.h>
#endif

#ifdef OSF_DCE
krb5_boolean       krb5_fcc_debug_retrieve = FALSE;
#endif

#define set(bits) (whichfields & bits)
#define flags_match(a,b) (a & b == a)
#define times_match_exact(t1,t2) (memcmp((char *)(t1), (char *)(t2), sizeof(*(t1))) == 0)

static krb5_boolean times_match PROTOTYPE((krb5_int32 now,
                                           const krb5_ticket_times *,
					   const krb5_ticket_times *));
static krb5_boolean standard_fields_match
    PROTOTYPE((const krb5_creds *,
	       const krb5_creds *));

static krb5_boolean srvname_match
    PROTOTYPE((const krb5_creds *,
	       const krb5_creds *));

static krb5_boolean
data_match(data1, data2)
register const krb5_data *data1, *data2;
{
    if (!data1) {
	if (!data2)
	    return TRUE;
	else
	    return FALSE;
    }
    if (!data2) return FALSE;

    if (data1->length != data2->length)
	return FALSE;
    else
	return memcmp(data1->data, data2->data, data1->length) ? FALSE : TRUE;
}


int krb5_cc_match(now, whichfields, mcreds, fcreds)
    krb5_int32 now;
    krb5_flags whichfields;
    krb5_creds *mcreds;
    krb5_creds *fcreds;
{
    return (((set(KRB5_TC_MATCH_SRV_NAMEONLY) &&
		   srvname_match(mcreds, fcreds)) ||
	       standard_fields_match(mcreds, fcreds))
	      &&
	      (! set(KRB5_TC_MATCH_IS_SKEY) ||
	       mcreds->is_skey == fcreds->is_skey)
	      &&
	      (! set(KRB5_TC_MATCH_FLAGS_EXACT) ||
	       mcreds->ticket_flags == fcreds->ticket_flags)
	      &&
	      (! set(KRB5_TC_MATCH_FLAGS) ||
	       flags_match(mcreds->ticket_flags, fcreds->ticket_flags))
	      &&
	      (! set(KRB5_TC_MATCH_TIMES_EXACT) ||
	       times_match_exact(&mcreds->times, &fcreds->times))
	      &&
	      (! set(KRB5_TC_MATCH_TIMES) ||
	       times_match(now, &mcreds->times, &fcreds->times))
	      &&
	      ( ! set(KRB5_TC_MATCH_AUTHDATA) ||
	       authdata_match(mcreds->authdata, fcreds->authdata))
	      &&
	      (! set(KRB5_TC_MATCH_2ND_TKT) ||
	       data_match (&mcreds->second_ticket, &fcreds->second_ticket))
    );
}

#ifdef OSF_DCE
/*
 * krb5_fcc_debug_cred: syslog a message (usually calling function),
 *                      client, server, and size for a ticket
 */
void
krb5_fcc_debug_cred(msg, creds, offset, error, matchfields)
    char       *msg;
    krb5_creds *creds;
    int offset;
    krb5_error_code error;
    krb5_flags matchfields;
{
    char *cname, *sname;
    krb5_error_code kret=0;
    krb5_timestamp   now=0;

    if (creds) {
        if (!(creds->server) || (krb5_unparse_name(creds->server, &sname)))
            sname = (char *) NULL;
        if (!(creds->client) || (krb5_unparse_name(creds->client, &cname)))
            cname = (char *) NULL;
        kret=krb5_timeofday(&now);
        syslog(LOG_DEBUG, "%s: %s ticket for %s, size %d, expires %d, now %d, offset %d, error 0x%x, matchfields 0x%x",
            msg,
            cname ? cname : "<unknown client>",
            sname ? sname : "<unknown server>",
            krb5_fcc_sizeof_cred(creds),
            creds->times.endtime,
            now,
            offset,
            error,
            matchfields);
        if (cname) free(cname);
        if (sname) free(sname);
    } else {
        syslog(LOG_DEBUG, "%s: offset %d, error 0x%x, matchfields 0x%x",
               msg, offset, error, matchfields);
    }
}
#endif


/*
 * Effects:
 * Searches the file cred cache is for a credential matching mcreds,
 * with the fields specified by whichfields.  If one if found, it is
 * returned in creds, which should be freed by the caller with
 * krb5_free_credentials().
 * 
 * The fields are interpreted in the following way (all constants are
 * preceded by KRB5_TC_).  MATCH_IS_SKEY requires the is_skey field to
 * match exactly.  MATCH_TIMES requires the requested lifetime to be
 * at least as great as that specified; MATCH_TIMES_EXACT requires the
 * requested lifetime to be exactly that specified.  MATCH_FLAGS
 * requires only the set bits in mcreds be set in creds;
 * MATCH_FLAGS_EXACT requires all bits to match.
 *
 * Errors:
 * system errors
 * permission errors
 * KRB5_CC_NOMEM
 */
krb5_error_code
krb5_fcc_retrieve(id, whichfields, mcreds, creds)
   krb5_ccache id;
   krb5_flags whichfields;
   krb5_creds *mcreds;
   krb5_creds *creds;
{
     /* This function could be considerably faster if it kept indexing */
     /* information.. sounds like a "next version" idea to me. :-) */

     krb5_cc_cursor cursor;
     krb5_error_code kret;
     krb5_creds fetchcreds;
     krb5_timestamp now;
#ifdef OSF_DCE
     krb5_error_code (*advance_cursor)
        PROTOTYPE((krb5_ccache id, krb5_cc_cursor *cursor, krb5_creds *creds)) = krb5_fcc_next_cred;
     int   pos;

     if (krb5_fcc_debug_retrieve) {
         krb5_fcc_debug_cred("krb5_fcc_retrieve: TRYING TO MATCH", mcreds, 0, 0, whichfields);
     }
#endif
     kret = krb5_timeofday(&now);
     if (kret != KRB5_OK)
	 return kret;

#ifdef OSF_DCE
     if (set(KRB5_TC_MATCH_TIMES)) {
         /*  An implementation coincidence is that these requests can never
          *  match if we fetch an expired ticket. So, heck, load that
          *  knowledge into the flow of control and avoid the overhead
          *  of repeated OPENCLOSE activity that the cursor advancements
          *  would produce. Note that TIMES_MATCH_EXACT is distinct here,
          *  and we'll leave those queries to return in the old way.
          */
         advance_cursor = krb5_fcc_next_unexpired_cred;
     }

#endif
     kret = krb5_fcc_start_seq_get(id, &cursor);
     if (kret != KRB5_OK)
	  return kret;

#ifdef OSF_DCE
     while (pos=((krb5_fcc_cursor *)cursor)->pos,
            (kret = (*advance_cursor)(id, &cursor, &fetchcreds)) == KRB5_OK) {
         if (krb5_fcc_debug_retrieve) {
             krb5_fcc_debug_cred("krb5_fcc_retrieve: EXAMINING", &fetchcreds, pos, 0, 0);
         }
#else
     while ((kret = krb5_fcc_next_cred(id, &cursor, &fetchcreds)) == KRB5_OK) {
#endif
	 if (krb5_cc_match (now, whichfields, mcreds, &fetchcreds))
	 {
#ifdef OSF_DCE
               if (krb5_fcc_debug_retrieve) {
                   krb5_fcc_debug_cred("krb5_fcc_retrieve: RETURNING MATCH", &fetchcreds,
                                        ((krb5_fcc_cursor *)cursor)->pos,0,0);
               }
#endif
	       krb5_fcc_end_seq_get(id, &cursor);
	       *creds = fetchcreds;
	       return KRB5_OK;
	  }

#ifdef OSF_DCE
          if (krb5_fcc_debug_retrieve) {
              krb5_fcc_debug_cred("krb5_fcc_retrieve: NOT A MATCH", 0, 0, 0, 0);
          }
#endif
	  /* This one doesn't match */
	  krb5_free_cred_contents(&fetchcreds);
     }

     /* If we get here, a match wasn't found */
     krb5_fcc_end_seq_get(id, &cursor);
#ifdef OSF_DCE
     if (krb5_fcc_debug_retrieve) {
         krb5_fcc_debug_cred("krb5_fcc_retrieve: RETURNING KRB5_CC_NOTFOUND", 0, 0, 0, 0);
     }
#endif
     return KRB5_CC_NOTFOUND;
}

static krb5_boolean
times_match(now, t1, t2)
krb5_int32 now;    
register const krb5_ticket_times *t1;
register const krb5_ticket_times *t2;
{
    if (t1->renew_till) {
	if (t1->renew_till > t2->renew_till)
	    return FALSE;		/* this one expires too late */
    }

    if (t2->renew_till) {
	if (now >= t2->renew_till)
	    return FALSE;
    }

    if (t1->endtime) {
	if (t1->endtime > t2->endtime)
	    return FALSE;		/* this one expires too late */
    }
    if (now >= t2->endtime)
	return FALSE;

    /* only care about expiration on a times_match */
    return TRUE;
}

static krb5_boolean
standard_fields_match(mcreds, creds)
register const krb5_creds *mcreds, *creds;
{
    return (krb5_principal_compare(mcreds->client,creds->client) &&
	    krb5_principal_compare(mcreds->server,creds->server));
}

/* only match the server name portion, not the server realm portion */

static krb5_boolean
srvname_match(mcreds, creds)
register const krb5_creds *mcreds, *creds;
{
    krb5_boolean retval;
    retval = krb5_principal_compare(mcreds->client,creds->client);
    if (retval != TRUE)
	return retval;
    return krb5_principal_compare(&(mcreds->server[1]),&(creds->server[1]));
}

krb5_boolean
authdata_match(mdata, data)
    register krb5_authdata * const *mdata, * const *data;
{
    register const krb5_authdata *mdatap, *datap;

    if (mdata == data)
      return TRUE;

    if (mdata == NULL)
	return *data == NULL;
	
    if (data == NULL)
	return *mdata == NULL;
    
    while ((mdatap = *mdata) && (datap = *data)) {
#ifdef OSF_DCE
    if (mdatap->ad_type == KRB5_AUTHDATA_OSF_DCE) {
        if (!priv_krb_util_match_dce_authdata(mdatap, datap))
            return FALSE;
    } else
#endif
      if ((mdatap->ad_type != datap->ad_type) ||
          (mdatap->length != datap->length) ||
          (memcmp ((char *)mdatap->contents,
		 (char *)datap->contents, mdatap->length) != 0))
          return FALSE;
      mdata++;
      data++;
    }
    return (*mdata == NULL) && (*data == NULL);
}

#ifdef KRB5_BETA1
/* !!!! this function does not belong here!! */
int krb5_princ_size(krb5_const_principal p)
{
    krb5_const_principal t;
    p++;
    t = p;
    while (*t)
	t++;
    return t - p;
}

#endif
