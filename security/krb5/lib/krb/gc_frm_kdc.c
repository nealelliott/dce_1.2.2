/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gc_frm_kdc.c,v $
 * Revision 1.1.8.3  1996/02/18  00:13:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:34  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:59:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/06/26  16:02 UTC  rps
 * 	[CHFts15540] re-merge no-matching-cred => expired translation
 * 	[1995/06/26  15:53 UTC  rps  /main/HPDCE02/rps_moth2/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/18  23:19 UTC  rps
 * 	WP merge
 * 
 * 	HP revision /main/DCE1_1_WP/2  1995/04/25  15:49 UTC  jrr
 * 	Fix #ifdef inside macro call problem for AIX.
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/15  00:03 UTC  rps
 * 	deep-copy auth data into tgtq
 * 	[1995/04/13  23:56 UTC  rps  /main/MOTHRA_panic/3]
 * 
 * 	hand-merge to mothra_nightly
 * 	[1995/04/10  17:46 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:11 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	Add new routine: krb5_get_cred_from_dce - new arg tgt_authdata which is
 * 	used to match authdata on a TGT lookup.
 * 	[1995/03/20  14:43 UTC  mullan_s  /main/mullan_mothra_bin_compat2/1]
 * 
 * 	krb5b4p3 merge
 * 	[1995/03/08  21:44 UTC  rps  /main/rps_hc/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:11 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat2/1  1995/03/20  14:43 UTC  mullan_s
 * 	Add new routine: krb5_get_cred_from_dce - new arg tgt_authdata which is
 * 	used to match authdata on a TGT lookup.
 * 	[1995/12/08  16:59:48  root]
 * 
 * Revision 1.1.4.1  1994/05/11  19:21:50  ahop
 * 	hp_sec_to_osf_1 drop
 * 	include int-proto.h with <> instead of ""
 * 	[1994/05/03  19:32:31  ahop]
 * 
 * Revision 1.1.2.3  1993/02/16  22:35:56  sommerfeld
 * 	Zero TGT credentials before fetching.
 * 	[1993/02/12  17:59:24  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:41  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  gc_frm_kdc.c V=8 12/10/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/gc_frm_kdc.c,v $
 * $Author: marty $
 *
 * Copyright (c) 1994 by the Massachusetts Institute of Technology.
 * Copyright (c) 1994 CyberSAFE Corporation
 * Copyright (c) 1993 Open Computing Security Group
 * Copyright (c) 1990,1991 by the Massachusetts Institute of Technology.
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
 * permission.  Neither M.I.T., the Open Computing Security Group, nor 
 * CyberSAFE Corporation make any representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 * 
 * krb5_get_cred_from_kdc()
 * Get credentials from some KDC somewhere, possibly accumulating tgts
 * along the way.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gcfkdc_c[] =
"$Id: gc_frm_kdc.c,v 1.1.8.3 1996/02/18 00:13:53 marty Exp $";
#endif  /* !lint & !SABER */


#include <stdio.h>
#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <int-proto.h>

#ifdef OSF_DCE
#include <dce/dce_cf_const.h>
#endif

/*
 * Retrieve credentials for principal creds->client,
 * server creds->server, ticket flags creds->ticket_flags, possibly
 * second_ticket if needed by ticket_flags.
 * 
 * Credentials are requested from the KDC for the server's realm.  Any
 * TGT credentials obtained in the process of contacting the KDC are
 * returned in an array of credentials; tgts is filled in to point to an
 * array of pointers to credential structures (if no TGT's were used, the
 * pointer is zeroed).  TGT's may be returned even if no useful end ticket
 * was obtained.
 * 
 * The returned credentials are NOT cached.
 *
 * This routine should not be called if the credentials are already in
 * the cache.
 * 
 * If credentials are obtained, creds is filled in with the results;
 * creds->ticket and creds->keyblock->key are set to allocated storage,
 * which should be freed by the caller when finished.
 * 
 * returns errors, system errors.
 */

extern krb5_cksumtype krb5_kdc_req_sumtype;

/* helper macro: convert flags to necessary KDC options */

#define FLAGS2OPTS(flags) (flags & KDC_TKT_COMMON_MASK)
#define TGT_ETYPE \
      krb5_keytype_array[tgt.keyblock.keytype]->system->proto_enctype;


#ifdef __STDC__
krb5_error_code krb5_get_cred_from_kdc(
  krb5_ccache ccache,
  krb5_creds  *cred,
  krb5_creds  ***tgts
)
#else
krb5_error_code krb5_get_cred_from_kdc(ccache, cred, tgts)
  krb5_ccache ccache;
  krb5_creds  *cred;
  krb5_creds  ***tgts;
#endif
{
#ifdef OSF_DCE
    return krb5_get_cred_from_kdc_dce(ccache, cred, tgts, NULL);
}

/*
 * Specialized krb5_get_cred_from_kdc for DCE purposes
 *
 * This routine allows the caller to pass in authorization data
 * for the TGT which will be used as matching criteria in
 * krb5_cc_retrieve_cred lookups. If tgt_authdata is NULL, the
 * behavior is the same as krb5_get_cred_from_kdc. However, if
 * it is not NULL, KRB5_TC_MATCH_AUTHDATA flag is added to the
 * matchflags.
 *
 * tgt_authdata is necessary to locate DCE 1.1 PTGTs instead of
 * DCE 1.0 PTGTs so that the service tickets are sealed correctly.
 * For DCE 1.1, tgt_authdata will consist of a V1 pac followed by 
 * a version number.
 */
krb5_error_code
krb5_get_cred_from_kdc_dce (ccache, cred, tgts, tgt_authdata)
    krb5_ccache ccache;
    krb5_creds  *cred;
    krb5_creds ***tgts;
    krb5_authdata **tgt_authdata;
{
#endif /* OSF_DCE */
  krb5_creds      **ret_tgts = NULL;
  int             ntgts = 0;

  krb5_creds      tgt, tgtq;
  krb5_enctype    etype;
  krb5_error_code retval;
  krb5_principal  int_server = NULL;    /* Intermediate server for request */

  krb5_principal  *tgs_list = NULL;
  krb5_principal  *top_server = NULL;
  krb5_principal  *next_server = NULL;
  int             nservers = 0;
#ifdef OSF_DCE
  krb5_timestamp  now;
  int matchflags    = KRB5_TC_MATCH_SRV_NAMEONLY | KRB5_TC_MATCH_TIMES,
      expmatchflags = KRB5_TC_MATCH_SRV_NAMEONLY;
#endif

  /* in case we never get a TGT, zero the return */

  *tgts = NULL;
     
  memset((char *)&tgtq, 0, sizeof(tgtq));
  memset((char *)&tgt, 0, sizeof(tgt));

#ifdef OSF_DCE
  retval = krb5_timeofday(&now);
  if (retval)
	now = 0;

#endif
  /*
   * we know that the desired credentials aren't in the cache yet.
   *
   * To get them, we first need a tgt for the realm of the server.
   * first, we see if we have such a TGT in cache.  if not, then
   * we ask the kdc to give us one.  if that doesn't work, then
   * we try to get a tgt for a realm that is closest to the target.
   * once we have that, then we ask that realm if it can give us
   * tgt for the target.  if not, we do the process over with this
   * new tgt.
   */
  
  /*
   * (the ticket may be issued by some other intermediate
   *  realm's KDC; so we use KRB5_TC_MATCH_SRV_NAMEONLY)
   */
  if (retval = krb5_copy_principal(cred->client, &tgtq.client))
      goto cleanup;

  /* get target tgt from cache */
  if (retval = krb5_tgtname(krb5_princ_realm(cred->server),
                            krb5_princ_realm(cred->client),
                            &int_server)) {
      goto cleanup;
  }

  if (retval = krb5_copy_principal(int_server, &tgtq.server)) {
      goto cleanup;
  }

#ifdef OSF_DCE
  /* match on authdata, if requested */
  if (tgt_authdata) {
      if (retval = krb5_copy_authdata(tgt_authdata, &tgtq.authdata))
	  goto cleanup;
      matchflags |= KRB5_TC_MATCH_AUTHDATA;
      expmatchflags |= KRB5_TC_MATCH_AUTHDATA;
  }

  if (retval = krb5_cc_retrieve_cred(ccache,
				     matchflags,
                                     &tgtq,
                                     &tgt)) {
#else
  if (retval = krb5_cc_retrieve_cred(ccache,
                                     KRB5_TC_MATCH_SRV_NAMEONLY,
                                     &tgtq,
                                     &tgt)) {
#endif

    if (retval != KRB5_CC_NOTFOUND) {
	goto cleanup;
    }

    /*
     * Note that we want to request a TGT from our local KDC, even
     * if we already have a TGT for some intermediate realm.  The 
     * reason is that our local KDC may have a shortcut to the
     * destination realm, and if it does we want to use the
     * shortcut because it will provide greater security. - bcn
     */
  
    /*
     * didn't find it in the cache so it is time to get a local
     * tgt and walk the realms tree.
     */
    krb5_free_principal(int_server);
    int_server = NULL;
    if (retval = krb5_tgtname(krb5_princ_realm(cred->client),
                              krb5_princ_realm(cred->client),
                              &int_server)) {
	goto cleanup;
    }
  
    krb5_free_cred_contents(&tgtq);
    memset((char *)&tgtq, 0, sizeof(tgtq));
    if(retval = krb5_copy_principal(cred->client, &tgtq.client))
	goto cleanup;
    if(retval = krb5_copy_principal(int_server, &tgtq.server))
	goto cleanup;

#ifdef OSF_DCE
    /* match on authdata, if requested */
    if (tgt_authdata &&
	    (retval = krb5_copy_authdata(tgt_authdata, &tgtq.authdata)))
	goto cleanup;

    if (retval = krb5_cc_retrieve_cred(ccache,
				       matchflags,
                                       &tgtq,
                                       &tgt)) {

	if (retval == KRB5_CC_NOTFOUND) {
	    /* there's no current local TGT.  see if there's an expired one */
	    retval = krb5_cc_retrieve_cred(ccache,
					   expmatchflags,
					   &tgtq,
					   &tgt);
	    if (!retval) {
		retval = KRB5KRB_AP_ERR_TKT_EXPIRED;
	    }
	}
	goto cleanup;
    }
#else
    if (retval = krb5_cc_retrieve_cred(ccache,
                                       KRB5_TC_MATCH_SRV_NAMEONLY,
                                       &tgtq,
                                       &tgt)) {
	goto cleanup;
    }
#endif 
  
    /* get a list of realms to consult */
  
    if (retval = krb5_walk_realm_tree(krb5_princ_realm(cred->client),
                                      krb5_princ_realm(cred->server),
                                      &tgs_list, 
#ifdef OSF_DCE
				      PATH_COMPONENT_SEPARATOR)) {
#else
                                      KRB5_REALM_BRANCH_CHAR)) {
#endif
	goto cleanup;
    }
  
    for (nservers = 0; tgs_list[nservers]; nservers++)
      ;
  
    /* allocate storage for TGT pointers. */
    
    if (!(ret_tgts = (krb5_creds **) calloc(nservers+1, sizeof(krb5_creds)))) {
      retval = ENOMEM;
      goto cleanup;
    }
    *tgts = ret_tgts;
    
    /*
     * step one is to take the current tgt and see if there is a tgt for
     * krbtgt/realmof(target)@realmof(tgt).  if not, try to get one with
     * the tgt.
     *
     * if we don't get a tgt for the target, then try to find a tgt as
     * close to the target realm as possible. at each step if there isn't
     * a tgt in the cache we have to try and get one with our latest tgt.
     * once we have a tgt for a closer realm, we go back to step one.
     *
     * once we have a tgt for the target, we go try and get credentials.
     */
  
    for (top_server = tgs_list;
         top_server < tgs_list + nservers;
         top_server = next_server) {
    
      /* look in cache for a tgt for the destination */
    
      krb5_free_cred_contents(&tgtq);
      memset(&tgtq, 0, sizeof(tgtq));
      if(retval = krb5_copy_principal(tgt.client, &tgtq.client))
	  goto cleanup;

      krb5_free_principal(int_server);
      int_server = NULL;
      if (retval = krb5_tgtname(krb5_princ_realm(cred->server),
				krb5_princ_realm(*top_server),
				&int_server)) {
	  goto cleanup;
      }
    
      if(retval = krb5_copy_principal(int_server, &tgtq.server))
	  goto cleanup;

#ifdef OSF_DCE
      /* match on authdata, if requested */
      if (tgt_authdata &&
	    (retval = krb5_copy_authdata(tgt_authdata, &tgtq.authdata)))
	  goto cleanup;

      if (retval = krb5_cc_retrieve_cred(ccache,
					 matchflags,
					 &tgtq,
					 &tgt)) {
#else
      if (retval = krb5_cc_retrieve_cred(ccache,
					 KRB5_TC_MATCH_SRV_NAMEONLY,
					 &tgtq,
					 &tgt)) {
#endif
    
	if (retval != KRB5_CC_NOTFOUND) {
	    goto cleanup;
	}
  
	/* didn't find it in the cache so try and get one */
	/* with current tgt.                              */
    
	if (!valid_keytype(tgt.keyblock.keytype)) {
	    retval = KRB5_PROG_KEYTYPE_NOSUPP;
	    goto cleanup;
	}
    
	krb5_free_cred_contents(&tgtq);
	memset(&tgtq, 0, sizeof(tgtq));
	tgtq.times        = tgt.times;
	if (retval = krb5_copy_principal(tgt.client, &tgtq.client))
	    goto cleanup;
	if(retval = krb5_copy_principal(int_server, &tgtq.server))
	    goto cleanup;
	tgtq.is_skey      = FALSE;
	tgtq.ticket_flags = tgt.ticket_flags;
	etype             = TGT_ETYPE;
	if(retval = krb5_get_cred_via_tgt(&tgt,
					  FLAGS2OPTS(tgtq.ticket_flags),
					  etype,
					  krb5_kdc_req_sumtype,
					  &tgtq)) {
	      
       /*
	* couldn't get one so now loop backwards through the realms
	* list and try and get a tgt for a realm as close to the
	* target as possible. the kdc should give us a tgt for the
	* closest one it knows about, but not all kdc's do this yet.
	*/
  
	  for (next_server = tgs_list + nservers - 1;
	       next_server > top_server;
	       next_server--) {
	    krb5_free_cred_contents(&tgtq);
	    memset(&tgtq, 0, sizeof(tgtq));
	    if (retval = krb5_copy_principal(tgt.client, &tgtq.client))
		goto cleanup;

	    krb5_free_principal(int_server);
	    int_server = NULL;
	    if (retval = krb5_tgtname(krb5_princ_realm(*next_server),
				      krb5_princ_realm(*top_server),
				      &int_server)) {
		goto cleanup;
	    }
    
	    if(retval = krb5_copy_principal(int_server, &tgtq.server))
		goto cleanup;

#ifdef OSF_DCE
	    /* match on authdata, if requested */
	    if (tgt_authdata &&
		  (retval = krb5_copy_authdata(tgt_authdata, &tgtq.authdata)))
		goto cleanup;

	    if (retval = krb5_cc_retrieve_cred(ccache,
					       matchflags,
					       &tgtq,
					       &tgt)) {

		if (retval == KRB5_CC_NOTFOUND) {
		    /* there's no current local TGT; see if there's an expired one */
		    retval = krb5_cc_retrieve_cred(ccache,
						   expmatchflags,
						   &tgtq,
						   &tgt);
		}
#else
	    if(retval = krb5_cc_retrieve_cred(ccache,
					      KRB5_TC_MATCH_SRV_NAMEONLY,
					      &tgtq,
					      &tgt)) {
#endif
	      if (retval != KRB5_CC_NOTFOUND) {
		  goto cleanup;
	      }
  
	      /* not in the cache so try and get one with our current tgt. */
  
	      if (!valid_keytype(tgt.keyblock.keytype)) {
		  retval = KRB5_PROG_KEYTYPE_NOSUPP;
		  goto cleanup;
	      }
            
	      krb5_free_cred_contents(&tgtq);
	      memset(&tgtq, 0, sizeof(tgtq));
	      tgtq.times        = tgt.times;
	      if (retval = krb5_copy_principal(tgt.client, &tgtq.client))
		  goto cleanup;
	      if(retval = krb5_copy_principal(int_server, &tgtq.server))
		  goto cleanup;
	      tgtq.is_skey      = FALSE;
	      tgtq.ticket_flags = tgt.ticket_flags;
	      etype             = TGT_ETYPE;
	      if (retval = krb5_get_cred_via_tgt(&tgt,
                                                 FLAGS2OPTS(tgtq.ticket_flags),
                                                 etype,
                                                 krb5_kdc_req_sumtype,
                                                 &tgtq)) {
		  continue;
	      }
	      
	      /* save tgt in return array */
	      if (retval = krb5_copy_creds(&tgtq, &ret_tgts[ntgts])) {
		  goto cleanup;
	      }
	      
	      tgt = *ret_tgts[ntgts++];
	    }
          
	    /* got one as close as possible, now start all over */
  
	    break;
	  }
  
	  if (next_server == top_server) {
	      goto cleanup;
	  }
	  continue;
        }
 
	/*
	 * Got a tgt.  If it is for the target realm we can go try for the
	 * credentials.  If it is not for the target realm, then make sure it
	 * is in the realms hierarchy and if so, save it and start the loop
	 * over from there.  Note that we only need to compare the instance
	 * names since that is the target realm of the tgt.
	 */
    
	for (next_server = top_server; *next_server; next_server++) {
            krb5_data *realm_1 = krb5_princ_component(next_server[0], 1);
            krb5_data *realm_2 = krb5_princ_component(tgtq.server, 1);
            if (realm_1->length == realm_2->length &&
                !memcmp(realm_1->data, realm_2->data, realm_1->length)) {
		break;
            }
	}

	if (!next_server) {
	    retval = KRB5_KDCREP_MODIFIED;
	    goto cleanup;
	}

	if (retval = krb5_copy_creds(&tgtq, &ret_tgts[ntgts])) {
	    goto cleanup;
	}
    
        tgt = *ret_tgts[ntgts++];

#ifdef OSF_DCE
	}
      /* we're done if it is the target */

      if (!*next_server++) break;
#else
        /* we're done if it is the target */

        if (!*next_server++) break;
      }
#endif
    }
  }

  /* got/finally have tgt!  try for the creds */

  if (!valid_keytype(tgt.keyblock.keytype)) {
    retval = KRB5_PROG_KEYTYPE_NOSUPP;
    goto cleanup;
  }

  etype = TGT_ETYPE;
  if (cred->second_ticket.length) {
      retval = krb5_get_cred_via_2tgt(&tgt,
				      KDC_OPT_ENC_TKT_IN_SKEY |
				      FLAGS2OPTS(tgt.ticket_flags),
				      etype,
				      krb5_kdc_req_sumtype,
				      cred);
  }
  else {
      retval = krb5_get_cred_via_tgt(&tgt,
                                   FLAGS2OPTS(tgt.ticket_flags), 
                                   etype,
                                   krb5_kdc_req_sumtype,
                                   cred);
  }

  /* cleanup and return */

cleanup:

  if(tgs_list)  krb5_free_realm_tree(tgs_list);
  krb5_free_cred_contents(&tgtq); 
  if (int_server) krb5_free_principal(int_server); 
  if (ntgts == 0) {
      *tgts = NULL;
      if (ret_tgts)  free(ret_tgts);
      krb5_free_cred_contents(&tgt);
  }
  return(retval);
}
