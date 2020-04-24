/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: preauth.c,v $
 * Revision 1.1.14.4  1996/11/13  18:08:35  arvind
 * 	CHFts20358: Add svc point for pk preauth..
 * 	CHFts20663: fix showstopper in process_pk_reply
 * 	[1996/11/05  22:17 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/2]
 *
 * 	Merge to DCE_1.2.2
 * 	[1996/11/01  21:43 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth3/1]
 *
 * 	CHFts20601: krb5_obtain_dce_padata: do not free rk contents on bad status.
 * 	[1996/10/31  19:36 UTC  mullan_s  /main/DCE_1.2.2/6]
 *
 * 	Merge to DCE 1.2.2
 * 	[1996/10/30  23:05 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth2/1]
 *
 * 	Fix CHFts20530: duplicate free of decrypt_key.
 * 	[1996/10/28  20:45 UTC  mullan_s  /main/DCE_1.2.2/5]
 *
 * 	Merge to DCE_1.2.2
 * 	[1996/10/25  19:10 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/9]
 *
 * 	A tad bit of cleanup.
 * 	[1996/10/24  18:09 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/8]
 *
 * 	Add preauth protocol specific data storage mechanism.
 * 	Fix UNIX_ENC_TIMESTAMPS padata to be compatible with old code -
 * 	add 32-bit length followed by encrypted timestamp.
 * 	[1996/10/21  16:56 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/7]
 *
 * 	code cleanup
 * 	[1996/10/17  14:54 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/6]
 *
 * 	Add decrypt_key parameter to obtain_padata.
 * 	[1996/10/15  13:27 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/4]
 *
 * 	Skip timestamps preauth if padata contains KRB5_PADATA_OSF_DCE.
 * 	[1996/10/10  17:15 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/3]
 *
 * 	Reinstate KRB5_PADATA_ENC_UNIX_TIME preauthentication method.
 * 	[1996/10/08  21:47 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/2]
 *
 * 	Fix KRB5_PADATA_OSF_DCE preauthentication method.
 * 	[1996/10/08  16:50 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	CHFts20148: should use MD5, not MD4.
 * 	Fix memory leak (of unused cell_name and cksum.contents).
 * 	[1996/10/16  23:18 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.14.3  1996/10/15  20:52:39  arvind
 * 	CHFts20157: misc client-side preauth cleanups.
 * 	Rip out printf.
 * 	[1996/10/04  22:21 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/2]
 * 
 * Revision 1.1.14.2  1996/10/04  16:55:53  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:55:37  arvind]
 * 
 * Revision /main/DCE_1.2.2/2  1996/10/03  18:48 UTC  sommerfeld
 * 	CHFts20135: reorder preauth entries appropriately.
 * 	[1996/10/03  17:08 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:30 UTC  sommerfeld
 * 	send error messages to stderr, and turn off one which blows up
 * 	dce_config.
 * 	these will need to change before the final drop.
 * 	[1996/09/25  21:18 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/3]
 * 
 * 	Use correct domain.
 * 	[1996/09/21  03:12 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/2]
 * 
 * 	Merge up to krb5beta6 preauth interface.
 * 	[1996/09/16  21:16 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * Revision 1.1.12.3  1996/02/18  00:14:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:58  marty]
 * 
 * Revision 1.1.12.2  1995/12/08  18:00:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:00:04  root]
 * 
 * Revision 1.1.8.1  1994/10/25  22:16:25  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes: modularize code
 * 	so that padata can be passed in from sec_login layer.
 * 	[1994/10/25  22:16:03  mullan_s]
 * 
 * Revision 1.1.2.5  1994/09/21  22:01:32  burati
 * 	CR10924 Always call krb5_finish_key(&eblock) in krb5_verify_padata()
 * 	[1994/09/21  19:17:23  burati]
 * 
 * Revision 1.1.2.4  1994/08/24  20:16:26  mullan_s
 * 	[OT11556] Update DCE preauth types to match Kerberos spec.
 * 	[1994/08/24  20:15:57  mullan_s]
 * 
 * Revision 1.1.2.3  1994/06/17  18:42:40  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:12:24  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/02  21:29:04  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:00  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.1  1994/05/11  19:23:41  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:21:11  ahop]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1994, 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
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
 * Sandia National Laboratories also makes no representations about the 
 * suitability of the modifications, or additions to this software for 
 * any purpose.  It is provided "as is" without express or implied warranty.
 *
 * Note: The internal interfaces to this routine are subject to change
 * and/or cleanup.  You should only expect the interfaces to
 * krb5_obtain_padata and krb5_verify_padata to have some chance of
 * staying stable.  [tytso:19920903.1544EDT]
 */

/*
 * This file contains routines for establishing, verifying, and any other
 * necessary functions, for utilizing the pre-authentication field of the 
 * kerberos kdc request, with various hardware/software verification devices.
 *
 * Note: At some point these functions may very well be split apart
 * into different files.... [tytso:19920903.1618EDT]
 */

#include <stdio.h>
#include <time.h>
#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/preauth.h>
#include <krb5/mit-des.h>

#include <syslog.h>

static krb5_error_code obtain_enc_ts_padata
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_etype_info,
	       krb5_keyblock *, 
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_creds *,
	       krb5_ccache,	       
	       krb5_kdc_req *,
#ifdef OSF_DCE_FEATURE
	       krb5_pa_misc_data *,
#endif
	       krb5_pa_data **));

static krb5_error_code obtain_enc_unix_ts_padata
    PROTOTYPE((krb5_context,
               krb5_pa_data *,
               krb5_etype_info,
               krb5_keyblock *,
               krb5_error_code ( * )(krb5_context,
                                     krb5_const krb5_enctype,
                                     krb5_data *,
                                     krb5_const_pointer,
                                     krb5_keyblock **),
               krb5_const_pointer,
               krb5_creds *,
               krb5_ccache,
               krb5_kdc_req *,
#ifdef OSF_DCE_FEATURE
	       krb5_pa_misc_data *,
#endif
               krb5_pa_data **));

static krb5_error_code process_pw_salt
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_kdc_req *,
	       krb5_kdc_rep *,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_keyblock *,
				     krb5_const_pointer,
				     krb5_kdc_rep * ),
	       krb5_keyblock **,
	       krb5_creds *, 
	       krb5_ccache,	       
	       krb5_int32 *,
#ifdef OSF_DCE_FEATURE
	       krb5_pa_misc_data **,
#endif
	       krb5_int32 *));

#ifdef OSF_DCE_FEATURE
static krb5_error_code obtain_pk_padata
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_etype_info,
	       krb5_keyblock *, 
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_creds *,
	       krb5_ccache,	       
	       krb5_kdc_req *,
	       krb5_pa_misc_data *,
	       krb5_pa_data **));

static krb5_error_code process_pk_preauth
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_kdc_req *,
	       krb5_kdc_rep *,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_keyblock *,
				     krb5_const_pointer,
				     krb5_kdc_rep * ),
	       krb5_keyblock **,
	       krb5_creds *, 
	       krb5_ccache,	       
	       krb5_int32 *,
	       krb5_pa_misc_data **,
	       krb5_int32 *));

static krb5_error_code obtain_dce_padata
    PROTOTYPE((krb5_context,
               krb5_pa_data *,
               krb5_etype_info,
               krb5_keyblock *,
               krb5_error_code ( * )(krb5_context,
                                     krb5_const krb5_enctype,
                                     krb5_data *,
                                     krb5_const_pointer,
                                     krb5_keyblock **),
               krb5_const_pointer,
               krb5_creds *,
               krb5_ccache,
               krb5_kdc_req *,
	       krb5_pa_misc_data *,
               krb5_pa_data **));

static krb5_error_code process_dce_preauth
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_kdc_req *,
	       krb5_kdc_rep *,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_keyblock *,
				     krb5_const_pointer,
				     krb5_kdc_rep * ),
	       krb5_keyblock **,
	       krb5_creds *, 
	       krb5_ccache,	       
	       krb5_int32 *,
	       krb5_pa_misc_data **,
	       krb5_int32 *));
#endif

#if 0
static krb5_error_code obtain_sam_padata
    PROTOTYPE((krb5_context,
	       krb5_pa_data *,
	       krb5_etype_info,
	       krb5_keyblock *, 
	       krb5_error_code ( * )(krb5_context,
				     krb5_const krb5_enctype,
				     krb5_data *,
				     krb5_const_pointer,
				     krb5_keyblock **),
	       krb5_const_pointer,
	       krb5_creds *,
	       krb5_kdc_req *,
	       krb5_pa_data **));
#endif

static krb5_preauth_ops preauth_systems[] = {
#ifdef OSF_DCE_FEATURE
    {
	KRB5_PADATA_ROOT_CERT,
        0,
	obtain_pk_padata,
	0,
    },
    {
	KRB5_PADATA_PUBLIC_REP,
        0,
	0,
	process_pk_preauth,
    },
    {
	KRB5_PADATA_OSF_DCE,
	0,
	obtain_dce_padata,
	process_dce_preauth,
    },
#endif
    {
	KRB5_PADATA_ENC_TIMESTAMP,
        0,
        obtain_enc_ts_padata,
        0,
    },
    {
	KRB5_PADATA_ENC_UNIX_TIME,
	0,
	obtain_enc_unix_ts_padata,
	0,
    },
    {
	KRB5_PADATA_PW_SALT,
        0,
        0,
        process_pw_salt,
    },
    {
	KRB5_PADATA_AFS3_SALT,
        0,
        0,
        process_pw_salt,
    },
#if 0
    {
	KRB5_PADATA_SAM_CHALLENGE,
        0,
        obtain_sam_padata,
        0,
    },
#endif
    { -1 }
};

static krb5_error_code find_pa_system
    PROTOTYPE((int type, krb5_preauth_ops **Preauth_proc));

/* some typedef's for the function args to make things look a bit cleaner */

typedef krb5_error_code (*git_key_proc) PROTOTYPE((krb5_context,
						   const krb5_enctype,
						   krb5_data *,
						   krb5_const_pointer,
						   krb5_keyblock **));

typedef krb5_error_code (*git_decrypt_proc) PROTOTYPE((krb5_context,
						       const krb5_keyblock *,
						       krb5_const_pointer,
						       krb5_kdc_rep * ));

krb5_error_code krb5_obtain_padata(
    krb5_context		context,
    krb5_pa_data **		preauth_to_use,
    git_key_proc 		key_proc,
    krb5_const_pointer		key_seed,
    krb5_creds *		creds,
    krb5_ccache			ccache,
#ifdef OSF_DCE_FEATURE
    krb5_pa_misc_data		**preauth_misc_data,
#endif
    krb5_kdc_req *		request
)
{
    krb5_error_code		retval;
    krb5_etype_info	    	etype_info = 0;
    krb5_pa_data **		pa;
    krb5_pa_data **		send_pa_list;
    krb5_pa_data **		send_pa;
    krb5_preauth_ops 		*ops;
    krb5_keyblock *		def_enc_key = 0;
    krb5_enctype 		enctype;
    krb5_data 			salt;
    krb5_data			scratch;
    int				size;
    int				f_salt = 0;
#ifdef OSF_DCE_FEATURE
    int				skip_timestamps_pa = 0;
    krb5_pa_misc_data		**pa_misc;
#endif

    if (preauth_to_use == NULL)
	return 0;

    for (pa = preauth_to_use, size=0; *pa; pa++, size++) {
	if ((*pa)->pa_type == KRB5_PADATA_ETYPE_INFO) {
	    scratch.length = (*pa)->length;
	    scratch.data = (char *) (*pa)->contents;
	    retval = decode_krb5_etype_info(&scratch, &etype_info);
	    if (retval)
		return retval;
	}
    }

    if ((send_pa_list = malloc((size+1) * sizeof(krb5_pa_data *))) == NULL)
	return ENOMEM;

    send_pa = send_pa_list;
    *send_pa = 0;

    enctype = request->etype[0];
    salt.data = 0;
    salt.length = -1;
    if (etype_info) {
	enctype = etype_info[0]->etype;
	salt.data = (char *) etype_info[0]->salt;
	salt.length = etype_info[0]->length;
    }
    if (salt.length == -1) {
	if ((retval = krb5_principal2salt(/*context, */request->client, &salt)))
	    return(retval);
	f_salt = 1;
    }
    
    if ((retval = (*key_proc)(context, enctype, &salt, key_seed,
			      &def_enc_key)))
	goto cleanup;
    
#ifdef OSF_DCE_FEATURE
    for (pa = preauth_to_use, pa_misc = preauth_misc_data; *pa; pa++, pa_misc++) {
#else
    for (pa = preauth_to_use; *pa; pa++) {
#endif
	if (find_pa_system((*pa)->pa_type, &ops))
	    continue;

	if (ops->obtain == 0)
	    continue;
	
#ifdef OSF_DCE_FEATURE
	/* See comments below */
	if (skip_timestamps_pa && ((*pa)->pa_type == KRB5_PADATA_ENC_UNIX_TIME
		|| (*pa)->pa_type == KRB5_PADATA_ENC_TIMESTAMP))
	    continue;
#endif
	retval = ((ops)->obtain)(context, 
				 *pa, 
				 etype_info, 
				 def_enc_key,
				 key_proc, 
				 key_seed, 
				 creds, 
				 ccache,
				 request, 
#ifdef OSF_DCE_FEATURE
				 *pa_misc,				 
#endif
				 send_pa);
	if (retval)
	    goto cleanup;

#ifdef OSF_DCE_FEATURE
	/*
	 * If padata of KRB5_PADATA_OSF_DCE can be constructed, skip any
	 * entries of type KRB5_PADATA_UNIX_ENC_TIME or KRB5_PADATA_ENC_TIMESTAMP
         * in the preauth_to_use array. We do not want to weaken the security of DCE 
         * third party preauthentication by sending other types of padata which makes 
         * the user's password more vulnerable to offline attacks. We assume that the 
         * caller has ordered the requested preauth types by priority.
	 */
	if (((*pa)->pa_type == KRB5_PADATA_OSF_DCE) && (*send_pa))
	    skip_timestamps_pa = 1;
#endif

	if (*send_pa)
	    send_pa++;
	*send_pa = 0;
    }

    retval = 0;

    if (send_pa_list[0]) {
	request->padata = send_pa_list;
	send_pa_list = 0;
    }

cleanup:
    if (f_salt)
	krb5_xfree(salt.data);
    if (send_pa_list)
	krb5_free_pa_data(/*context, */send_pa_list);
    if (def_enc_key)
	krb5_free_keyblock(/*context, */def_enc_key);
    if (etype_info)
        krb5_free_etype_info(context, etype_info);
    return retval;
    
}

krb5_error_code
krb5_process_padata(
    krb5_context	context,
    krb5_kdc_req *	request,
    krb5_kdc_rep *	as_reply,
    git_key_proc	key_proc,
    krb5_const_pointer	keyseed,
    git_decrypt_proc	decrypt_proc,
    krb5_keyblock **	decrypt_key,
    krb5_creds *	creds,
    krb5_ccache		ccache,
#ifdef OSF_DCE_FEATURE
    krb5_pa_misc_data   **preauth_misc_data,
#endif
    krb5_int32 *	do_more
)
{
    krb5_error_code		retval = 0;
    krb5_preauth_ops * 		ops;
    krb5_pa_data **		pa;
    krb5_int32			done = 0;
    
    *do_more = 0;		/* By default, we don't need to repeat... */
    if (as_reply->padata == 0)
#ifdef OSF_DCE_FEATURE
	goto dce_preauth;
#else
	return 0;
#endif

    /* XXX - Will this screw up pre-1.1 servers using padata for salt? - check! */
    for (pa = as_reply->padata; *pa; pa++) {
	if (find_pa_system((*pa)->pa_type, &ops))
	    continue;

	if (ops->process == 0)
	    continue;

	retval = ((ops)->process)(context, 
				  *pa, 
				  request, 
				  as_reply,
				  key_proc, 
				  keyseed, 
				  decrypt_proc,
				  decrypt_key, 
				  creds, 
				  ccache, 
				  do_more, 
#ifdef OSF_DCE_FEATURE
				  preauth_misc_data,
#endif
				  &done);
	if (retval)
	    goto cleanup;
	if (done)
	    break;
    }

#ifdef OSF_DCE_FEATURE
dce_preauth:
    /*
     * Set decryption key to DCE Third Party reply-random-key
     */
    if (!done)
        retval = process_dce_preauth(context, NULL, request,
                                     as_reply, key_proc,
                                     keyseed, decrypt_proc,
                                     decrypt_key, creds,
                                     ccache, do_more,
                                     preauth_misc_data, &done);
#endif

cleanup:
    return retval;
}

/*
 * This routine is the "obtain" function for the ENC_TIMESTAMP
 * preauthentication type.  It take the current time and encrypts it
 * in the user's key.
 */
static krb5_error_code
obtain_enc_ts_padata(
    krb5_context		context,
    krb5_pa_data *		in_padata,
    krb5_etype_info		etype_info,
    krb5_keyblock *		def_enc_key,
    git_key_proc 		key_proc,
    krb5_const_pointer		key_seed,
    krb5_creds *		creds,
    krb5_ccache			ccache,
    krb5_kdc_req *		request,
#ifdef OSF_DCE_FEATURE
    krb5_pa_misc_data		*pa_misc,
#endif
    krb5_pa_data **		out_padata
)
{
    krb5_pa_enc_ts		pa_enc;
    krb5_error_code		retval;
    krb5_data *			scratch;
    krb5_enc_data 		enc_data;
    krb5_pa_data *		pa;
    

    enc_data.ciphertext.data = 0;

    retval = krb5_us_timeofday(/*context,*/ &pa_enc.patimestamp, &pa_enc.pausec);
    if (retval)
	return retval;

    if ((retval = encode_krb5_pa_enc_ts(&pa_enc, &scratch)) != 0)
	return retval;

    if ((retval = krb5_encrypt_data(context, def_enc_key, 0, scratch,
				    &enc_data)))
	goto cleanup;

    krb5_free_data(/*context,*/scratch);
    scratch = 0;
    
    if ((retval = encode_krb5_enc_data(&enc_data, &scratch)) != 0)
	goto cleanup;

    if ((pa = malloc(sizeof(krb5_pa_data))) == NULL) {
	retval = ENOMEM;
	goto cleanup;
    }

#ifndef OSF_DCE_ARCHAISM
    pa->magic = KV5M_PA_DATA;
#endif
    pa->pa_type = KRB5_PADATA_ENC_TIMESTAMP;
    pa->length = scratch->length;
    pa->contents = (krb5_octet *) scratch->data;

    *out_padata = pa;

    krb5_xfree(scratch);
    scratch = 0;

    retval = 0;
    
cleanup:
    if (scratch)
	krb5_free_data(/*context, */scratch);
    if (enc_data.ciphertext.data)
	krb5_xfree(enc_data.ciphertext.data);
    return retval;
}

static krb5_error_code
process_pw_salt(
    krb5_context		context,
    krb5_pa_data *		padata,
    krb5_kdc_req *		request,
    krb5_kdc_rep *		as_reply,
    git_key_proc		key_proc,
    krb5_const_pointer		keyseed,
    git_decrypt_proc		decrypt_proc,
    krb5_keyblock **		decrypt_key,
    krb5_creds *		creds,
    krb5_ccache 		ccache,
    krb5_int32 *		do_more,
#ifdef OSF_DCE_FEATURE
    krb5_pa_misc_data		**pa_misc,
#endif
    krb5_int32 *		done
)
{
    krb5_error_code	retval;
    krb5_data		salt;
    
    if (*decrypt_key != 0)
	return 0;

    salt.data = (char *) padata->contents;
    salt.length = 
      (padata->pa_type == KRB5_PADATA_AFS3_SALT)?(-1):(padata->length);
    
    if ((retval = (*key_proc)(context, as_reply->enc_part.etype,
			      &salt, keyseed, decrypt_key))) {
	*decrypt_key = 0;
	return retval;
    }

    return 0;
}
    
static krb5_error_code
find_pa_system(type, preauth)
    int			type;
    krb5_preauth_ops	**preauth;
{
    krb5_preauth_ops *ap = preauth_systems;
    
    while ((ap->type != -1) && (ap->type != type))
	ap++;
    if (ap->type == -1)
	return(KRB5_PREAUTH_BAD_TYPE);
    *preauth = ap;
    return 0;
} 


extern const char *krb5_default_pwd_prompt1;

static krb5_error_code
sam_get_pass_from_user(context, etype_info, key_proc, key_seed, request,
		       new_enc_key, prompt)
    krb5_context		context;
    krb5_etype_info		etype_info;
    git_key_proc 		key_proc;
    krb5_const_pointer		key_seed;
    krb5_kdc_req *		request;
    krb5_keyblock **		new_enc_key;
    const char *		prompt;
{
    krb5_enctype 		enctype;
    krb5_error_code		retval;
    const char *oldprompt;

    /* enctype = request->ktype[0]; */
    enctype = ENCTYPE_DES_CBC_MD5;
/* hack with this first! */
    oldprompt = krb5_default_pwd_prompt1;
    krb5_default_pwd_prompt1 = prompt;
    {
      krb5_data newpw;
      newpw.data = 0; newpw.length = 0;
      /* we don't keep the new password, just the key... */
      retval = (*key_proc)(context, enctype, 0, 
			   (krb5_const_pointer)&newpw, new_enc_key);
      krb5_xfree(newpw.data);
    }
    krb5_default_pwd_prompt1 = oldprompt;
    return retval;
}
static 
char *handle_sam_labels(sc)
     krb5_sam_challenge *sc;
{
    char *label = sc->sam_challenge_label.data;
    int label_len = sc->sam_challenge_label.length;
    char *prompt = sc->sam_response_prompt.data;
    int prompt_len = sc->sam_response_prompt.length;
    char *challenge = sc->sam_challenge.data;
    int challenge_len = sc->sam_challenge.length;
    char *prompt1, *p;
    char *sep1 = ": [";
    char *sep2 = "]\n";
    char *sep3 = ": ";

    if (sc->sam_cksum.length == 0) {
      /* or invalid -- but lets just handle presence now XXX */
      switch (sc->sam_type) {
      case PA_SAM_TYPE_ENIGMA:	/* Enigma Logic */
	label = "Challenge for Enigma Logic mechanism";
	break;
      case PA_SAM_TYPE_DIGI_PATH: /*  Digital Pathways */
	label = "Challenge for Digital Pathways mechanism";
	break;
      case PA_SAM_TYPE_SKEY_K0:	/*  S/key where  KDC has key 0 */
	label = "Challenge for Enhanced S/Key mechanism";
	break;
      case PA_SAM_TYPE_SKEY:	/*  Traditional S/Key */
	label = "Challenge for Traditional S/Key mechanism";
	break;
      case PA_SAM_TYPE_SECURID:	/*  Security Dynamics */
	label = "Challenge for Security Dynamics mechanism";
	break;
      }
      prompt = "Passcode";
      label_len = strlen(label);
      prompt_len = strlen(prompt);
    }

    /* example:
       Challenge for Digital Pathways mechanism: [134591]
       Passcode: 
     */
    p = prompt1 = malloc(label_len + strlen(sep1) +
			 challenge_len + strlen(sep2) +
			 prompt_len+ strlen(sep3) + 1);
    strncpy(p, label, label_len); p += label_len;
    strcpy(p, sep1); p += strlen(sep1);
    strncpy(p, challenge, challenge_len); p += challenge_len;
    strcpy(p, sep2); p += strlen(sep2);
    strncpy(p, prompt, prompt_len); p += prompt_len;
    strcpy(p, sep3); /* p += strlen(sep3); */
    return prompt1;
}

#if 0

/*
 * This routine is the "obtain" function for the SAM_CHALLENGE
 * preauthentication type.  It presents the challenge...
 */
static krb5_error_code
obtain_sam_padata(context, in_padata, etype_info, def_enc_key,
		  key_proc, key_seed, creds, ccache, request, out_padata)
    krb5_context		context;
    krb5_pa_data *		in_padata;
    krb5_etype_info		etype_info;
    krb5_keyblock *		def_enc_key;
    git_key_proc 		key_proc;
    krb5_const_pointer		key_seed;
    krb5_creds *		creds;
    krb5_ccache			ccache;
    krb5_kdc_req *		request;
    krb5_pa_data **		out_padata;
{
    krb5_pa_enc_ts		pa_enc;
    krb5_error_code		retval;
    krb5_data *			scratch;
    krb5_data			tmpsam;
    krb5_pa_data *		pa;
    krb5_sam_challenge		*sam_challenge = 0;
    krb5_sam_response		sam_response;
    /* these two get encrypted and stuffed in to sam_response */
    krb5_enc_sam_response_enc	enc_sam_response_enc;
    krb5_keyblock *		sam_use_key = 0;
    char * prompt;

    tmpsam.length = in_padata->length;
    tmpsam.data = (char *) in_padata->contents;
    retval = decode_krb5_sam_challenge(&tmpsam, &sam_challenge);
    if (retval)
      return retval;

    if (sam_challenge->sam_flags & KRB5_SAM_MUST_PK_ENCRYPT_SAD) {
      return KRB5_SAM_UNSUPPORTED;
    }

    enc_sam_response_enc.sam_nonce = sam_challenge->sam_nonce;
    if (sam_challenge->sam_flags & KRB5_SAM_SEND_ENCRYPTED_SAD) {
      /* encrypt passcode in key by stuffing it here */
      int pcsize = 256;
      char *passcode = malloc(pcsize+1);
      prompt = handle_sam_labels(sam_challenge);
      retval = krb5_read_password(context, prompt, 0, passcode, &pcsize);
      free(prompt);

      if (retval) {
	free(passcode);
	return retval;
      }
      enc_sam_response_enc.sam_passcode.data = passcode;
      enc_sam_response_enc.sam_passcode.length = pcsize;
    } else if (sam_challenge->sam_flags & KRB5_SAM_USE_SAD_AS_KEY) {
      if (sam_challenge->sam_nonce) {
	/* use nonce in the next AS request? */
      } else {
	retval = krb5_us_timeofday(context, 
				   &enc_sam_response_enc.sam_timestamp,
				   &enc_sam_response_enc.sam_usec);
	sam_response.sam_patimestamp = enc_sam_response_enc.sam_timestamp;
      }
      if (retval)
	return retval;      
      prompt = handle_sam_labels(sam_challenge);
      retval = sam_get_pass_from_user(context, etype_info, key_proc, 
				      key_seed, request, &sam_use_key,
				      prompt);
      free(prompt);
      if (retval)
	return retval;      
      enc_sam_response_enc.sam_passcode.length = 0;
    } else {
      /* what *was* it? */
      return KRB5_SAM_UNSUPPORTED;
    }

    /* so at this point, either sam_use_key is generated from the passcode
     * or enc_sam_response_enc.sam_passcode is set to it, and we use 
     * def_enc_key instead. */
    /* encode the encoded part of the response */
    if ((retval = encode_krb5_enc_sam_response_enc(&enc_sam_response_enc,
						   &scratch)) != 0)
      return retval;

    if ((retval = krb5_encrypt_data(context, 
				    sam_use_key?sam_use_key:def_enc_key, 
				    0, scratch,
				    &sam_response.sam_enc_nonce_or_ts)))
      goto cleanup;

    krb5_free_data(/*context, */scratch);
    scratch = 0;

    /* sam_enc_key is reserved for future use */
    sam_response.sam_enc_key.ciphertext.length = 0;

    /* copy things from the challenge */
    sam_response.sam_nonce = sam_challenge->sam_nonce;
    sam_response.sam_flags = sam_challenge->sam_flags;
    sam_response.sam_track_id = sam_challenge->sam_track_id;
    sam_response.sam_type = sam_challenge->sam_type;
#ifndef OSF_DCE_ARCHAISM
    sam_response.magic = KV5M_SAM_RESPONSE;
#endif

    if ((retval = encode_krb5_sam_response(&sam_response, &scratch)) != 0)
	return retval;
    
    if ((pa = malloc(sizeof(krb5_pa_data))) == NULL) {
	retval = ENOMEM;
	goto cleanup;
    }

#ifndef OSF_DCE_ARCHAISM
    pa->magic = KV5M_PA_DATA;
#endif
    pa->pa_type = KRB5_PADATA_SAM_RESPONSE;
    pa->length = scratch->length;
    pa->contents = (krb5_octet *) scratch->data;
    scratch = 0;		/* so we don't free it! */

    *out_padata = pa;

    retval = 0;
    
cleanup:
    if (scratch)
	krb5_free_data(/*context, */scratch);
    if (sam_challenge)
        krb5_xfree(sam_challenge);
    return retval;
}
#endif

#ifdef OSF_DCE_FEATURE

void
krb5_free_pa_misc_data(val)
krb5_pa_misc_data **val;
{
    register krb5_pa_misc_data **temp;

    for (temp = val; *temp; temp++) {
	if ((*temp)->data && (*temp)->free)
	    (*temp)->free((*temp)->data);
        xfree(*temp);
    }
    xfree(val);
    return;
}

#include <dce/dce.h>
#include <dce/dce_svc.h>
#include <dce/dcesecsvc.h>
#include <dce/dcesecmac.h>
#include <dce/sec_pk.h> 
#include <dce/sec_psm.h>
#include <dce/dce.h>
#include <sec_svc.h>

error_status_t compute_kdc_body_cksum(krb5_cksumtype sumtype,
				      krb5_kdc_req *request,
				      krb5_checksum *sum)
{
    krb5_data *scratch;
    error_status_t errcode;
    
    errcode = encode_krb5_kdc_req_body(request, &scratch);
    if (errcode) {
	return(errcode);
    }

    if (!(sum->contents = (krb5_octet *)
	  malloc(krb5_checksum_size(sumtype)))) {
	krb5_free_data(scratch);
	return ENOMEM;
    }

    errcode = krb5_calculate_checksum(sumtype,
				     scratch->data,
				     scratch->length,
				     NULL,
				     0,
				     sum);
    krb5_free_data(scratch);    

    if (errcode != 0) {
	xfree(sum->contents);
	sum->contents = NULL;
    }
    return errcode;
}


extern krb5_error_code pwd_keyproc(krb5_context ctxt,
				   krb5_const krb5_enctype type,
				   krb5_data *salt,
				   krb5_const_pointer keyseed,
				   krb5_keyblock **key);

static krb5_error_code
obtain_pk_padata(context, in_padata, etype_info, def_enc_key, key_proc, 
		 key_seed, creds, ccache, request, pa_misc, out_padata)
    krb5_context		context;
    krb5_pa_data *		in_padata;
    krb5_etype_info		etype_info;
    krb5_keyblock *		def_enc_key;
    git_key_proc 		key_proc;
    krb5_const_pointer		key_seed;
    krb5_creds *		creds;
    krb5_ccache			ccache;
    krb5_kdc_req *		request;
    krb5_pa_misc_data		*pa_misc;
    krb5_pa_data **		out_padata;
{
    krb5_principal kdc;
    krb5_checksum cksum;
    krb5_int32 cusec = 0 /* !!! */;
    krb5_timestamp ctime = 0 /* !!! */;
    krb5_int32 nonce = request->nonce;
    sec_psm_handle_t psm_h = NULL;
    error_status_t st;
    char *princ_name = NULL;
    char *password;
    sec_pk_domain_t domain_id;
    krb5_pa_data *pa = NULL;
    const char *errloc = NULL;
    
    /* Assume we can't produce this type of authdata.. */

    *out_padata = NULL;

    cksum.contents = NULL;

    /*
     * N.B. A non-zero error return from this function aborts the *entire*
     * ticket request, which is a big hammer; this is why we always
     * return `0' and lose the reason for the preauth failure.
     * A servicability point would be good here..
     */

    /* !!! possibly bogus assumption: key_seed->data points at
       null-terminated string containing password. */
    if (key_proc == pwd_keyproc)
	password = ((krb5_data*)(key_seed))->data;
    else
	return 0;
    
    domain_id = sec_pk_domain_dce_general;
    
    sec_dce_pair_from_krb_princ(request->client,
				NULL, /* cell name; unneeded */
				&princ_name,
				&st);
    if (st != 0) {
	errloc = "name_cvt";
	goto cleanup;
    }

    /* well, do we have psm handy?  if so, go for it.. */
    /* !!! what about non-local principals? */
    st = sec_psm_open(princ_name,
		      (idl_char *)password,
		      &domain_id,
		      &psm_h);
    if (princ_name)
	free(princ_name);
    if (st != 0) {
	errloc = "sec_psm_open";
	goto cleanup;
    }
    krb5_timeofday(&ctime); /* !!! should pull from somewhere else.. */

    st = compute_kdc_body_cksum(CKSUMTYPE_RSA_MD5, request, &cksum);
    if (st == 0) {
	if ((pa = malloc(sizeof(krb5_pa_data))) == NULL) {
	    st = ENOMEM;
	    errloc = "malloc";
	    goto cleanup;
	}
	st = krb5_pkinit_sign_as_req (psm_h, request->server, &cksum,
					   cusec, ctime, request->nonce,
					   pa);
 	errloc = "request_signature";
    } else {
	errloc = "request_checksum";
    }
  cleanup:
    if ((st != 0) && (errloc != NULL) && (st != sec_pk_e_kf_open_err)) {
	dce_svc_printf(SEC_PK_LOGIN_CLIENT_FAIL_MSG, error_message(st), errloc);
    }
    if (cksum.contents) {
        free(cksum.contents);
        cksum.contents = NULL;
    }
    if (st == 0) {
	*out_padata = pa;
        pa_misc->data = psm_h;
	pa_misc->free = (krb5_pa_misc_data_free_proc *) sec_psm_close;
    } else {
	if (pa)
	    free(pa);		/* !!! what about ptrs in *pa?? */
	if (psm_h)
	    sec_psm_close(psm_h);
    }
    return 0;
}


static krb5_error_code
process_pk_preauth(context, padata, request, as_reply,
		   key_proc, keyseed, decrypt_proc, decrypt_key, 
		   creds, ccache, do_more, pa_misc, done)
    krb5_context		context;
    krb5_pa_data *		padata;
    krb5_kdc_req *		request;
    krb5_kdc_rep *		as_reply;
    git_key_proc		key_proc;
    krb5_const_pointer		keyseed;
    git_decrypt_proc		decrypt_proc;
    krb5_keyblock **		decrypt_key;
    krb5_creds *		creds;
    krb5_ccache			ccache;
    krb5_int32 *		do_more;
    krb5_pa_misc_data **	pa_misc;
    krb5_int32 *		done;
{
    sec_psm_handle_t psm_h = NULL;
    krb5_error_code errcode;
    krb5_keyblock reply_key;
    krb5_int32 peer_nonce;
    krb5_keyblock *rkp;
    krb5_pa_misc_data **pa_miscp;

    errcode = KRB5_PREAUTH_FAILED;
    /* Locate psm handle in misc. preauth data */
    for (pa_miscp = pa_misc; *pa_miscp; pa_miscp++) {
	if ((*pa_miscp)->pa_type == KRB5_PADATA_ROOT_CERT) {
	    psm_h = (sec_psm_handle_t)(*pa_miscp)->data;
	    errcode = 0;
	    break;
	}
    }

    if (errcode == 0) {
	errcode = krb5_pkinit_decode_as_rep (psm_h,
					     padata,
					     request->server,
					     request->client,
					     &reply_key,
					     &peer_nonce);
    }
    
    if (errcode != 0) {
	dce_svc_printf(SEC_PK_LOGIN_CLIENT_FAIL_MSG,
		       error_message(errcode), "decode_pk_response");
    } else {
	/* !!! check peer_nonce vs request->nonce */
	rkp = (krb5_keyblock *)malloc (sizeof(krb5_keyblock));
	*rkp = reply_key;
	if (*decrypt_key)
	    krb5_free_keyblock(*decrypt_key);
	*decrypt_key = rkp;
	/*
         * Do not process other padata in reply, because it may cause
	 * decrypt_key to be overwritten.
	 */
	*done = 1;
    }

    return errcode;
}

static krb5_error_code
obtain_dce_padata(context, in_padata, etype_info, def_enc_key, key_proc, 
		  key_seed, creds, ccache, request, pa_misc, out_padata)
    krb5_context                context;
    krb5_pa_data *              in_padata;
    krb5_etype_info             etype_info;
    krb5_keyblock *             def_enc_key;
    git_key_proc                key_proc;
    krb5_const_pointer          key_seed;
    krb5_creds *                creds;
    krb5_ccache                 ccache;
    krb5_kdc_req *              request;
    krb5_pa_misc_data *		pa_misc;
    krb5_pa_data **             out_padata;
{
    krb5_error_code		retval = 0;
    krb5_keyblock		*rk = NULL;
    krb5_pa_data 		*pa = NULL;
    char			*errtext;

    /* initialize */
    *out_padata = NULL;
    if ((pa = malloc(sizeof(krb5_pa_data))) == NULL) {
        retval = ENOMEM;
	errtext = "malloc";
        goto cleanup;
    }
    if ((rk = malloc(sizeof(krb5_keyblock))) == NULL) {
        retval = ENOMEM;
	errtext = "malloc";
        goto cleanup;
    }

    /* a non-zero error return from this function aborts the *entire*
       ticket request, which is a big hammer; this is why we always
       return `0' */

    if ((retval = krb5_obtain_dce_tp_preauth(context, 
			OSF_DCE_THIRD_PARTY,
		        request->client,
		        def_enc_key,
			key_seed,
		        rk,
		        pa))) {
	errtext = "krb5_obtain_dce_tp_preauth";
	goto cleanup;
    }

cleanup:
    if (retval == 0) {
	*out_padata = pa;
	pa_misc->data = rk;
	pa_misc->free = (krb5_pa_misc_data_free_proc *) krb5_free_keyblock;
    } else {
	if (pa)
	    free(pa);
	if (rk)
	    free(rk);
	/* Need to log via SVC? */
	dce_svc_printf(DCE_SVC(sec_svc_handle, "%s%s"),
		sec_s_login, svc_c_sev_error | svc_c_action_none,
		sec_sys_errno_text, errtext,
		error_message(retval));
    }

    return 0;
}

static krb5_error_code
process_dce_preauth(context, padata, request, as_reply,
                    key_proc, keyseed, decrypt_proc, decrypt_key,
                    creds, ccache, do_more, pa_misc, done)
    krb5_context                context;
    krb5_pa_data *              padata;
    krb5_kdc_req *              request;
    krb5_kdc_rep *              as_reply;
    git_key_proc                key_proc;
    krb5_const_pointer          keyseed;
    git_decrypt_proc            decrypt_proc;
    krb5_keyblock **            decrypt_key;
    krb5_creds *                creds;
    krb5_ccache                 ccache;
    krb5_int32 *                do_more;
    krb5_pa_misc_data **        pa_misc;
    krb5_int32 *                done;
{
    krb5_pa_misc_data		**pa_miscp;

    /* May need to subtype (*pa_miscp)->data in future */

    /* Locate reply-random-key in misc. preauth data */
    for (pa_miscp = pa_misc; *pa_miscp; pa_miscp++) {
        if ((*pa_miscp)->pa_type == KRB5_PADATA_OSF_DCE) {
	    if ((*pa_miscp)->data) {
                if (*decrypt_key)
	            krb5_free_keyblock(*decrypt_key);
                if ((*decrypt_key = malloc(sizeof(krb5_keyblock))) == NULL)
                    return ENOMEM;
		krb5_copy_keyblock_contents((krb5_keyblock *)(*pa_miscp)->data, *decrypt_key);
	    }
            break;
        }
    }
    return 0;
}

#endif

/*
 * Format is:   8 bytes of random confounder,
 *              1 byte version number (currently 0),
 *              4 bytes: number of seconds since Jan 1, 1970, in MSB order.
 */
int seeded = 0 ; /* Used by srand below */

static krb5_error_code
obtain_enc_unix_ts_padata(
    krb5_context                context,
    krb5_pa_data *              in_padata,
    krb5_etype_info             etype_info,
    krb5_keyblock *             def_enc_key,
    git_key_proc                key_proc,
    krb5_const_pointer          key_seed,
    krb5_creds *                creds,
    krb5_ccache                 ccache,
    krb5_kdc_req *              request,
#ifdef OSF_DCE_FEATURE
    krb5_pa_misc_data		*pa_misc,
#endif
    krb5_pa_data **             out_padata
)
{
    krb5_enc_data		enc_data;
    krb5_timestamp 		kdc_time;
    krb5_error_code     	retval = 0;
    int         		i;
    unsigned char 		*tmp = NULL;
    krb5_data			*scratch = NULL;
    krb5_pa_data		*pa = NULL;

    /* initialize */
    memset(&enc_data, 0, sizeof(enc_data));

    retval = krb5_timeofday(&kdc_time);
    if (retval)
        return retval;
    if (!seeded) {
        seeded = kdc_time + getpid();
        srand(seeded);
    }

    if ((scratch = malloc(sizeof(krb5_data))) == NULL)
	return(ENOMEM);

    scratch->length = 13; 
    scratch->data = malloc(scratch->length);
    tmp = (unsigned char *) scratch->data;
    if (!tmp) {
	krb5_xfree(scratch);
        return(ENOMEM);
    }

    for (i=0; i < 8; i++)
        *tmp++ = rand() & 255;

    *tmp++ = 0;
    *tmp++ = (kdc_time >> 24) & 255;
    *tmp++ = (kdc_time >> 16) & 255;
    *tmp++ = (kdc_time >> 8) & 255;
    *tmp++ = kdc_time & 255;

    if ((retval = krb5_encrypt_data(context, def_enc_key, 0, scratch,
                                    &enc_data)))
	goto cleanup;

    krb5_free_data(scratch);
    scratch = 0;

    if ((scratch = malloc(sizeof(krb5_data))) == NULL) {
	retval = ENOMEM;
	goto cleanup;
    }
    scratch->length = enc_data.ciphertext.length + 4;
    if (!(scratch->data = malloc(scratch->length))) {
	retval = ENOMEM;
	goto cleanup;
    }

    scratch->data[0] = 13 >> 24;
    scratch->data[1] = 13 >> 16;
    scratch->data[2] = 13 >> 8;
    scratch->data[3] = 13;
    memcpy((scratch->data + 4), enc_data.ciphertext.data, enc_data.ciphertext.length);

    if ((pa = malloc(sizeof(krb5_pa_data))) == NULL) {
	retval = ENOMEM;
	goto cleanup;
    }

    pa->pa_type = KRB5_PADATA_ENC_UNIX_TIME;
    pa->length = scratch->length;
    pa->contents = (krb5_octet *) scratch->data;

    *out_padata = pa;

    krb5_xfree(scratch);
    scratch = 0;

    retval = 0;

cleanup:
    if (scratch)
        krb5_free_data(scratch);
    if (enc_data.ciphertext.data)
        krb5_xfree(enc_data.ciphertext.data);
    return(retval);
}
