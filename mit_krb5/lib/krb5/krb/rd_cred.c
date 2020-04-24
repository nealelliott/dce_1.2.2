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
 * $Log: rd_cred.c,v $
 * Revision 1.1.2.1  1996/06/05  21:05:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:13  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rd_cred.c,v $
 * Revision 1.1.2.1  1996/06/05  21:05:29  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:02:13  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  20:02:13  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:19 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:44 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/3  1996/05/17  14:18 UTC  mullan_s
 * 	Remove #ifdef OSF_DCE - just a bug fix, not an enhancement.
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2  1996/05/16  16:36 UTC  mullan_s
 * 	krb5_rd_cred_basic: set retval to KRB5KRB_AP_ERR_BADADDR if addresses
 * 	don't match.
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:07 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
#include <k5-int.h>
#include "cleanup.h"
#include "auth_con.h"

#include <stddef.h>           /* NULL */
#include <stdlib.h>           /* malloc */
#include <errno.h>            /* ENOMEM */

/*-------------------- decrypt_credencdata --------------------*/

/*
 * decrypt the enc_part of a krb5_cred
 */
static krb5_error_code 
decrypt_credencdata(context, pcred, pkeyblock, pcredenc)
    krb5_context	  context;
    krb5_cred 		* pcred;
    krb5_keyblock 	* pkeyblock;
    krb5_cred_enc_part 	* pcredenc;
{
    krb5_cred_enc_part  * ppart;
    krb5_encrypt_block 	  eblock;
    krb5_error_code 	  retval;
    krb5_data 		  scratch;

    if (!valid_etype(pcred->enc_part.etype))
    	return KRB5_PROG_ETYPE_NOSUPP;

    /* put together an eblock for this decryption */
    krb5_use_cstype(context, &eblock, pcred->enc_part.etype);
    scratch.length = pcred->enc_part.ciphertext.length;
    
    if (!(scratch.data = (char *)malloc(scratch.length))) 
        return ENOMEM;

    /* do any necessary key pre-processing */
    if (retval = krb5_process_key(context, &eblock, pkeyblock)) 
    	goto cleanup;
    
    /* call the decryption routine */
    if (retval = krb5_decrypt(context, 
			      (krb5_pointer) pcred->enc_part.ciphertext.data,
                              (krb5_pointer) scratch.data,
                              scratch.length, &eblock, 0)) {
      	(void)krb5_finish_key(context, &eblock);
      	goto cleanup;
    }

    if (retval = krb5_finish_key(context, &eblock)) 
    	goto cleanup;

    /*  now decode the decrypted stuff */
    if (retval = decode_krb5_enc_cred_part(&scratch, &ppart))
    	goto cleanup_encpart;

    *pcredenc = *ppart;
    retval = 0;

cleanup_encpart:
    memset(ppart, 0, sizeof(*ppart));
    krb5_xfree(ppart);

cleanup:
    memset(scratch.data, 0, scratch.length);
    krb5_xfree(scratch.data);

    return retval;
}
/*----------------------- krb5_rd_cred_basic -----------------------*/

static krb5_error_code 
krb5_rd_cred_basic(context, pcreddata, pkeyblock, local_addr, remote_addr,
		   replaydata, pppcreds)
    krb5_context          context;
    krb5_data		* pcreddata;
    krb5_keyblock 	* pkeyblock;
    krb5_address  	* local_addr;
    krb5_address  	* remote_addr;
    krb5_replay_data    * replaydata;
    krb5_creds        *** pppcreds;
{
    krb5_error_code       retval;
    krb5_cred 		* pcred;
    krb5_int32 		  ncreds;
    krb5_int32 		  i = 0;
    krb5_cred_enc_part 	  encpart;

    /* decode cred message */
    if (retval = decode_krb5_cred(pcreddata, &pcred)) 
    	return retval;

    if (retval = decrypt_credencdata(context, pcred, pkeyblock, &encpart)) 
	goto cleanup_cred;

#ifdef OSF_DCE
    /*
     * Do not check sender address if it is NULL. This fixes interoperability
     * problem between pre-beta 5 rlogin and beta 5 rlogind. A pre-beta 5
     * rlogin did not fill in the sender addresses when forwarding creds.
     * A beta 5 rlogind enforces it. (Sent this as a bug report to MIT 5/96)
     */
    if (encpart.s_address && 
	!krb5_address_compare(context, remote_addr, encpart.s_address)) {
#else
    if (!krb5_address_compare(context, remote_addr, encpart.s_address)) {
        KRB5KRB_AP_ERR_BADADDR;
#endif
        retval = KRB5KRB_AP_ERR_BADADDR;
        goto cleanup_cred;
    }

    if (encpart.r_address) {
        if (local_addr) {
            if (!krb5_address_compare(context, local_addr, encpart.r_address)) {
                retval = KRB5KRB_AP_ERR_BADADDR;
                goto cleanup_cred;
            }
        } else {
            krb5_address **our_addrs;

            if (retval = krb5_os_localaddr(&our_addrs)) {
                goto cleanup_cred;
            }
            if (!krb5_address_search(context, encpart.r_address, our_addrs)) {
                krb5_free_addresses(context, our_addrs);
                retval =  KRB5KRB_AP_ERR_BADADDR;
                goto cleanup_cred;
            }
            krb5_free_addresses(context, our_addrs);
        }
    }

    replaydata->timestamp = encpart.timestamp;
    replaydata->usec = encpart.usec;
    replaydata->seq = encpart.nonce;

   /*
    * Allocate the list of creds.  The memory is allocated so that
    * krb5_free_tgt_creds can be used to free the list.
    */
    for (ncreds = 0; pcred->tickets[ncreds]; ncreds++);
	
    if ((*pppcreds = 
        (krb5_creds **)malloc((size_t)(sizeof(krb5_creds *) * ncreds + 1))) == NULL) {
        retval = ENOMEM;
        goto cleanup_cred;
    }

    /*
     * For each credential, create a strcture in the list of
     * credentials and copy the information.
     */
    while (i < ncreds) {
        krb5_cred_info 	* pinfo;
        krb5_creds 	* pcur;
	krb5_data	* pdata;

        if ((pcur = (krb5_creds *)malloc(sizeof(krb5_creds))) == NULL) {
	    retval = ENOMEM;
	    goto cleanup;
        }

        (*pppcreds)[i] = pcur;
        pinfo = encpart.ticket_info[i++];
        memset(pcur, 0, sizeof(krb5_creds));

        if (retval = krb5_copy_principal(context, pinfo->client, &pcur->client))
	    goto cleanup;

        if (retval = krb5_copy_principal(context, pinfo->server, &pcur->server))
	    goto cleanup;

      	if (retval = krb5_copy_keyblock_contents(context, pinfo->session,
						 &pcur->keyblock))
	    goto cleanup;

        if (retval = krb5_copy_addresses(context, pinfo->caddrs, 
					 &pcur->addresses)) 
	    goto cleanup;

        if (retval = encode_krb5_ticket(pcred->tickets[i - 1], &pdata))
	    goto cleanup;

	pcur->ticket = *pdata;
        /* krb5_free_data(context, pdata); ANL */
        /* above frees both the krb5_data and what it points at */
        /* need to only free the krb5_data, since we copied the pointer */
        /* to pcur->ticket  This was the source of the ASN.1 problem */
        /* with forwarded credentials on some machines */
        krb5_xfree(pdata);  /* ANL */

        pcur->is_skey = FALSE;
        pcur->magic = KV5M_CREDS;
        pcur->times = pinfo->times;
        pcur->ticket_flags = pinfo->flags;
        pcur->authdata = NULL;   /* not used */
        memset(&pcur->second_ticket, 0, sizeof(pcur->second_ticket));
    }

    /*
     * NULL terminate the list
     */
    (*pppcreds)[i] = NULL;

cleanup:
    if (retval)
	while (i >= 0)
	    free((*pppcreds)[i--]);

cleanup_cred:
    krb5_free_cred(context, pcred);

    return retval;
}

/*----------------------- krb5_rd_cred -----------------------*/

extern krb5_deltat krb5_clockskew;
#define in_clock_skew(date) (labs((date)-currenttime) < krb5_clockskew)

/*
 * This functions takes as input an KRB_CRED message, validates it, and
 * outputs the nonce and an array of the forwarded credentials.
 */
krb5_error_code INTERFACE
krb5_rd_cred(context, auth_context, pcreddata, pppcreds, outdata)
    krb5_context          context;
    krb5_auth_context   * auth_context;
    krb5_data 		* pcreddata;       
    krb5_creds        *** pppcreds;
    krb5_replay_data  	* outdata;
{
    krb5_error_code       retval;
    krb5_keyblock       * keyblock;
    krb5_replay_data      replaydata;

    /* Get keyblock */
    if ((keyblock = auth_context->local_subkey) == NULL)
        if ((keyblock = auth_context->remote_subkey) == NULL)
            keyblock = auth_context->keyblock;

    if (((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME) ||
      (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) &&
      (outdata == NULL))
        /* Need a better error */
        return KRB5_RC_REQUIRED;

    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) &&
      (auth_context->rcache == NULL))
        return KRB5_RC_REQUIRED;

{
    krb5_address * premote_fulladdr = NULL;
    krb5_address * plocal_fulladdr = NULL;
    krb5_address remote_fulladdr;
    krb5_address local_fulladdr;
    CLEANUP_INIT(2);

    if (auth_context->local_addr) {
    	if (auth_context->local_port) {
            if (!(retval = krb5_make_fulladdr(context,auth_context->local_addr,
                                 	      auth_context->local_port, 
					      &local_fulladdr))){
                CLEANUP_PUSH(local_fulladdr.contents, free);
	        plocal_fulladdr = &local_fulladdr;
            } else {
	        return retval;
            }
	} else {
            plocal_fulladdr = auth_context->local_addr;
        }
    }

    if (auth_context->remote_addr) {
    	if (auth_context->remote_port) {
            if (!(retval = krb5_make_fulladdr(context,auth_context->remote_addr,
                                 	      auth_context->remote_port, 
					      &remote_fulladdr))){
                CLEANUP_PUSH(remote_fulladdr.contents, free);
	        premote_fulladdr = &remote_fulladdr;
            } else {
	        return retval;
            }
	} else {
            premote_fulladdr = auth_context->remote_addr;
        }
    }

    if (retval = krb5_rd_cred_basic(context, pcreddata, keyblock,
      				    plocal_fulladdr, premote_fulladdr,
      				    &replaydata, pppcreds)) {
        CLEANUP_DONE();
	return retval;
    }

    CLEANUP_DONE();
}


    if (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_TIME) {
        krb5_donot_replay replay;
        krb5_timestamp currenttime;

        if (retval = krb5_timeofday(context, &currenttime))
            goto error;

        if (!in_clock_skew(replaydata.timestamp)) {
            retval =  KRB5KRB_AP_ERR_SKEW;
            goto error;
        }

        if (retval = krb5_gen_replay_name(context, auth_context->remote_addr,
                                          "_forw", &replay.client))
            goto error;

        replay.server = "";             /* XXX */
        replay.cusec = replaydata.usec;
        replay.ctime = replaydata.timestamp;
        if (retval = krb5_rc_store(context, auth_context->rcache, &replay)) {
            krb5_xfree(replay.client);
            goto error;
        }
        krb5_xfree(replay.client);
    }

    if (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_DO_SEQUENCE) {
        if (auth_context->remote_seq_number != replaydata.seq) {
            retval =  KRB5KRB_AP_ERR_BADORDER;
            goto error;
        }
        auth_context->remote_seq_number++;
    }

    if ((auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_TIME) ||
      (auth_context->auth_context_flags & KRB5_AUTH_CONTEXT_RET_SEQUENCE)) {
        outdata->timestamp = replaydata.timestamp;
        outdata->usec = replaydata.usec;
        outdata->seq = replaydata.seq;
    }

error:;
    if (retval)
    	krb5_xfree(*pppcreds);
    return retval;
}


