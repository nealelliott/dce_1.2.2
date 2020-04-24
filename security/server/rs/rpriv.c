/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpriv.c,v $
 * Revision 1.1.20.3  1996/02/18  00:17:09  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:07  marty]
 *
 * Revision 1.1.20.2  1995/12/08  17:50:05  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/31  18:42 UTC  rps
 * 	merge (revert)
 * 
 * 	HP revision /main/HPDCE02/rps_moth3/1  1995/07/31  15:41 UTC  rps
 * 	revert foreign cell account EPTGT check
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/15  20:02 UTC  rps
 * 	[CHFts14668] check cell account validity when doing intercell client auth
 * 	[1995/06/14  18:00 UTC  rps  /main/rps_moth2/1]
 * 
 * Revision 1.1.16.1  1994/10/25  20:23:36  kline_s
 * 	UHYP CR 12343: Enable Delegation audit points: utilize dce_aud_start_with_uuid()
 * 	to prevent audit from causing secd to go into an infinite loop.
 * 	[1994/10/25  20:19:53  kline_s]
 * 
 * Revision 1.1.12.6  1994/08/24  21:07:02  cuti
 * 	Modify audit points
 * 	[1994/08/24  20:07:49  cuti]
 * 
 * Revision 1.1.12.5  1994/07/15  15:01:06  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:18:47  mdf]
 * 
 * Revision 1.1.12.4  1994/06/10  15:07:19  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:49:43  greg]
 * 
 * Revision 1.1.12.3  1994/06/02  21:27:09  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:39  mdf]
 * 
 * Revision 1.1.12.2  1994/05/11  19:27:09  ahop
 * 	hp_sec_to_osf_2 drop
 * 	first cut at a real delegation token.
 * 	break common routines out into a separate utility module.
 * 	[1994/04/29  21:27:50  ahop]
 * 
 * Revision 1.1.12.1  1994/01/28  23:11:06  burati
 * 	Delegation changes - new mgr routines (dlg_bl1)
 * 	[1994/01/20  04:17:00  burati]
 * 
 * Revision 1.1.10.1  1993/10/14  20:02:24  sommerfeld
 * 	Workaround MIT-DCE incompatibility in tgs request
 * 	[1993/10/14  15:55:16  sommerfeld]
 * 
 * Revision 1.1.6.7  1993/04/16  19:58:21  sommerfeld
 * 	[OT 7706] avoid race conditons in KDC code.
 * 	[1993/04/16  19:49:40  sommerfeld]
 * 
 * 	CR7631 Fix group insertion into PAC in rpriv_krb_get_auth_info
 * 	[1993/04/01  19:56:22  burati]
 * 
 * Revision 1.1.6.5  1992/12/29  16:34:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:22:53  zeliff]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:15:43  sekhar]
 * 
 * Revision 1.1.7.2  1992/10/06  16:06:28  burati
 * 	Use BEGIN_READ at rpriv_get_ptgt (the public entry point to this module)
 * 	remove other READ_LOCKs
 * 
 * Revision 1.1.6.3  1992/09/29  20:12:32  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:36:24  sekhar]
 * 
 * Revision 1.1.6.2  1992/08/31  18:23:34  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:06  sekhar]
 * 
 * Revision 1.1.4.3  1992/06/25  22:28:33  burati
 * 	CR4461 Stop checking alias list as soon as membership verified
 * 	[1992/06/25  21:03:10  burati]
 * 
 * 	CR4461 Check alias list for group membership if primary fails.
 * 	[1992/06/25  20:50:06  burati]
 * 
 * Revision 1.1.4.2  1992/03/13  22:31:49  weisman
 * 	Pick up fix for quick & dirty bsubmit (3/13/92 sommerfeld using weisman's acct)
 * 	[1992/03/13  22:31:18  weisman]
 * 
 * Revision 1.1.2.2  1992/03/13  21:32:49  hsiao
 * 	Properly NULL-terminate last_req array. (3/13/92 sommerfeld)
 * 
 * Revision 1.1  1992/01/19  14:44:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993, 1994
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rpriv.c,v 1.1.20.3 1996/02/18 00:17:09 marty Exp $";
#endif

#include <rpriv_util.h>

#include <krb5/copyright.h>
#include <krb5/asn1.h>

#include <rpriv.h>
#include <rpriv_util.h>

#include <bstring.h>
#include <un_strng.h>
#include <un_maloc.h>

#include <rs_server.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_util.h>

#include <rsdb_pgo.h>

#include <sec_pkl.h>
#include <sec_id_pac_util.h>
#include <priv_krb_util.h>

#include <dce/audit.h>
#include <sec_aud_evts.h>

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/* 
 * PRIVATE ROUTINES
 */


/* 
 * free any existing string names in a pac 
 */
PRIVATE  void  clean_pac_names
(
    sec_id_pac_t  *pac
)
{
    int           i;

    if (pac->principal.name != NULL) {
        free( (char *) pac->principal.name);
        pac->principal.name = NULL;
    }
    
    if (pac->group.name != NULL) {
        free( (char *) pac->group.name);
        pac->group.name = NULL;
    }
    
    if (pac->realm.name != NULL) {
        free( (char *) pac->realm.name);
        pac->realm.name = NULL;
    }
    
    for (i = 0; (unsigned16) i < pac->num_groups; i++) {
    if (pac->groups[i].name != NULL) {
        free( (char *) pac->groups[i].name);
        pac->groups[i].name = NULL;
    }
    }
    
    for (i = 0; (unsigned16) i < pac->num_foreign_groups; i++) {
    if (pac->foreign_groups[i].id.name != NULL) {
        free( (char *) pac->foreign_groups[i].id.name);
        pac->foreign_groups[i].id.name = NULL;
    }
    if (pac->foreign_groups[i].realm.name != NULL) {
        free( (char *) pac->foreign_groups[i].realm.name);
        pac->foreign_groups[i].realm.name = NULL;
    }
    }
}


/*
 * p r i n t _ p a c 
 *
 * for debugging purposes
 */
PRIVATE void print_pac
(
    sec_id_pac_t  *p
)
{
    int                i;

    print_id(&p->realm ,"Realm:     ");

    print_id(&p->principal, "Principal: ");

    print_id(&p->group, "Group    ");

    printf("Local groupset :\n");
    for (i = 0; (unsigned16) i < p->num_groups; i++) {
        print_id(&p->groups[i], "     ");
    }
    
    printf("Foreign groupset :\n");
    for (i = 0; (unsigned16) i < p->num_foreign_groups; i++) {
        print_id(&p->foreign_groups[i].realm, "     Realm");
        print_id(&p->foreign_groups[i].id, "     Group");
    }
}
    

/* r p r i v _  k r b _ g e t _ p t g t
 * 
 * Using the information from the kerberos AP_REQ authentication 
 * header for the priv server session, contruct a privilege ticket
 * in krb5_kdc_rep form and encode it in ASN.1/BER kerberos response packet
 */
PRIVATE void rpriv_krb_get_ptgt
  (
   handle_t              handle,         /* [in] */
   krb5_data             *krb_req_data,  /* [in] */
   rpriv_pickle_t        **ptgt_rep,     /* [out] */
   error_status_t        *stp
  )
{

    rpriv_dce_authz_data_t  authz_data;
    rpriv_krb_req_info_t    *req_info = NULL;
    sec_id_pac_t            tmp_pac, granted_pac, *requested_pac = NULL;
    krb5_ticket             *ptgt = NULL;
    krb5_pa_data            **padata_vec = NULL;
    krb5_authdata           **authdata = NULL;

    dce_aud_rec_t           ard = NULL;
    error_status_t          aud_status;


    CLEAR_STATUS(stp);
    memset(&granted_pac, 0, sizeof(granted_pac));
    granted_pac.pac_type = sec_id_pac_format_v1;
    granted_pac.authenticated = true;
    memset(&tmp_pac, 0, sizeof(granted_pac));
    tmp_pac.pac_type = sec_id_pac_format_v1;
    tmp_pac.authenticated = true;

    /*
     * Decode and decrypt the KRB5_TGS_REQ
     */
    rpriv_util_get_krb_req_info(krb_req_data, &req_info, stp);
    if (BAD_STATUS(stp)) {
        goto audit_end;
    }
    
    /*
     * Decompose the krb5 elements of the request into their
     * corresponding DCE components.
     */

    rpriv_util_get_dce_authz_info(
	                         PRIV_GetPtgt,   /* audit event */
                                 handle,         /* used by audit */
                                 req_info, 
                                 &authz_data, 
                                 &requested_pac, 
                                 &ard,
                                 stp);

    if (!STATUS_OK(stp)) {
	rpriv_util_free_krb_req_info(&req_info);
        goto audit_end;
    }

    if (authz_data.local) {
        /* 
	 * client is from our local cell, so construct pac from info
         * in the local registry database
         */
	
	/*
	 * a pre-1.1 local ptgt request requires a PAC
	 * containing the requested privileges
	 */
	if (requested_pac == NULL) {
	    SET_STATUS(stp, sec_priv_s_invalid_request);
            goto get_ptgt_cleanup;
        }
	
        if (priv_debug) {
            printf("Priv Server received request for local principal:\n");
            print_pac(requested_pac);
        }
	
	/* 
	 * Verify the requested primary principal info
	 */


        rpriv_util_verify_local_princ(
                                      ard,
                                      authz_data.client_princ,
				      authz_data.client_cell, 
				      &requested_pac->principal,
                                      &requested_pac->group,
                                      &tmp_pac.principal,
			              &tmp_pac.group, 
				      &tmp_pac.realm,
				      stp);


	if (STATUS_OK(stp)) {
	    /*
	     * Construct granted_pac local groupset from those
	     * local groups in the requested_pac groupset that 
	     * are actually valid.
	     */


	    rpriv_util_verify_local_groups(
                                           ard,
                                           authz_data.client_princ,
					   requested_pac->num_groups,
					   requested_pac->groups,
					   &tmp_pac.num_groups,
					   &tmp_pac.groups,
					   stp);

	}

    } else { /* !authz_data.local */

	/* 
         * we've previously verified that the client in a
	 * foreign request is the architecturally defined
	 * privilege server principal, and the authorization
	 * data, if any, in the ticket used to authenticate 
	 * the request has been decoded
	 */
	
	/* 
	 * If there wasn't a PAC in the authorization data
         * the request is invalid.
	 */
	if (!FLAG_SET(authz_data.data_parts, priv_ad_part_pac)) {
	    SET_STATUS(stp, sec_priv_s_invalid_request);
	} else {
	    /* 
	     * check the cell uuid in the PAC presented by the foreign
	     * priv server against the uuid registered locally for that
	     * priv server's cell.  Reject if cell is not registered
	     * locally or the uuids don't match.
	     */
	    uuid_t  fc_uuid;
	    rsdb_pgo_name_to_uid(sec_rgy_domain_person, 
				 authz_data.client_tgt_princ,
				 &fc_uuid,
				 stp);
	    if (STATUS_OK(stp) 
	    && uuid_equal(&fc_uuid, &authz_data.v1_pac.realm.uuid, stp)) { 
		sec_id_pac_util_copy(&granted_pac, &authz_data.v1_pac);
	    } else {
		SET_STATUS(stp, sec_priv_s_invalid_request);
	    }
	}
    }

    if (!STATUS_OK(stp)) {
	goto get_ptgt_cleanup;
    }
    
    /*
     * copy tmp_pac to granted_pac, to produce a PAC generated with
     * the old PAC memeory management scheme.  Otherwise, we have
     * real problems deciding what deallocator routine to use.
     */
    if (authz_data.local) {
	sec_id_pac_util_copy(&granted_pac, &tmp_pac);
    }

    /*
     * allocate padata: we use it to return an encoded encrypted copy
     * of the pac sealed in the ptgt
     */
    padata_vec = calloc(2, sizeof(krb5_pa_data *));
    if (padata_vec == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
	goto get_ptgt_cleanup;
    }

    padata_vec[0] = (krb5_pa_data *) malloc(sizeof(krb5_pa_data));
    if (padata_vec[0] == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
	goto get_ptgt_cleanup;
    }
    memset(padata_vec[0], 0, sizeof(krb5_pa_data));

    /* 
     * Deal with PTGT authorization data up front.  We use it twice.  First, we
     * encode it, string names and all (if any), encrypt it, and stuff it in the
     * pa-data field of the generated kdc response message.  This allows the client
     * access to a copy of the PAC sealed in the ptgt. Next, we bonk the string
     * names (if any), and stuff it unencrypted into the authdata field of the
     * to-be-encrypted portion of the ptgt (we don't want to take up any more
     * space in the ticket than we have to, and names aren't trustworthy, so we
     * just get rid of them)
     */
    {
        idl_pkl_t       *ppac;
        unsigned32      ppac_len;
        krb5_enc_data   enc_authdata;
        krb5_data       *scratch;
    
	/* 
	 * first, the padata version
	 */
        sec_id_pac_t_pickle(&granted_pac, NULL, (void *(*)(unsigned32)) malloc,
			    (unsigned32) 8, &ppac, &ppac_len, stp);
	

	if (STATUS_OK(stp)) {
	    /* key version is 0 - we're using a session key */
	    enc_authdata.kvno = 0;
	    priv_krb_util_pac_to_enc_authdata(ppac, ppac_len, NULL, 
					      &req_info->eblock,
					      &enc_authdata, stp);
	    free((char *) ppac);
	}
	
	if (STATUS_OK(stp)) {
	    *stp = encode_krb5_enc_data(&enc_authdata, &scratch);
	    /* done with enc_authdata, in any event */
	    free(enc_authdata.ciphertext.data);
        }
	
	if (STATUS_OK(stp)) {
	    padata_vec[0]->pa_type = PRIV_KDC_REP_PADATA;
	    padata_vec[0]->length = scratch->length;
	    padata_vec[0]->contents = (unsigned char *) scratch->data;
	    
	    /* can free scratch now, but not scratch->data */
	    free( (char *) scratch);
	    
	    /* 
	     * now, the ticket authdata version 
	     */
	    
	    /* bonk the names, and stick the pac in the ptgt */
	    clean_pac_names(&granted_pac);
	    sec_id_pac_t_pickle(&granted_pac, NULL, (void *(*)(unsigned32)) malloc,
				(unsigned32) 8, &ppac, &ppac_len, stp);
	    if (STATUS_OK(stp)) {
		priv_krb_util_pac_to_authdata(ppac, ppac_len, 
		    req_info->authent->ticket->enc_part2->authorization_data,
		    &authdata, stp);
		free((char *) ppac);
	    }
	}
    }


    if (STATUS_OK(stp)) {
	/*
	 * build a PTGT
	 */
	rpriv_util_build_ptgt(req_info, 
		   req_info->authent->ticket->enc_part2->times.endtime,
		   authdata, &ptgt, stp);
    }

    if (STATUS_OK(stp)) {
	rpriv_util_build_ptgt_rep(req_info, padata_vec, ptgt,
				  ptgt_rep, stp);
    }

get_ptgt_cleanup:

    rpriv_util_free_krb_req_info(&req_info);
    rpriv_util_free_authz_data(&authz_data); 
    if (requested_pac) {
	sec_id_pac_util_free(requested_pac); 
	free((char *) requested_pac); 
    } 
    if (padata_vec)  krb5_free_pa_data(padata_vec);
    if (authdata) krb5_free_authdata(authdata);
    sec_id_pac_util_free(&granted_pac); 
    if (ptgt)  krb5_free_ticket(ptgt); 

audit_end:
#if defined(DCE_AUD_POINTS)
    if (ard) {
        if (GOOD_STATUS(stp)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                           aud_c_esl_cond_success, &aud_status);
        } else {
            if (rs_util_status_is_denial(*stp)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                               aud_c_esl_cond_failure, &aud_status);
            }
        }
    }
#endif  /* defined(DCE_AUD_POINTS) */

}

/* 
 * MANAGER  ROUTINES 
 */


/* r p r i v _ g e t _ p t g t
 * 
 * Privilege server manager routine to construct and return a
 * a privilege certificate to the ticket-granting service.
 * The caller supplies the group set, and the privilege server
 * seals the group set in the authorization portion of a privilege
 * ticket-granting ticket, after first rejecting any groups that
 * are not legitimately part of the caller credentials.  A group 
 * will be rejected if the caller is not a member of the group,
 * or the group is not allowed on project lists (the projlist_ok
 * flag is not set).
 *
 */
PUBLIC void rpriv_get_ptgt
  (
   handle_t        handle,
   unsigned32      authn_svc,
   unsigned32      authz_svc,
   rpriv_pickle_t  *ptgt_req,
   rpriv_pickle_t  **ptgt_rep,
   error_status_t  *status
  )
{
    krb5_data      *krb_rep, krb_req; 
    int            pickle_size;                                    

    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(status, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(status, sec_priv_s_invalid_authn_svc);
        return;
    }

    *ptgt_rep = NULL;

    krb_req.length = ptgt_req->num_bytes;
    krb_req.data = (char *) ptgt_req->bytes;

    BEGIN_READ(lock_db, status) {

        rpriv_krb_get_ptgt(handle, &krb_req, ptgt_rep, status);

    } END_READ
}
