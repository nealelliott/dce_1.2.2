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
 * $Log: decode_kdc.c,v $
 * Revision 1.1.2.1  1996/06/05  20:59:58  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:57:32  arvind]
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
 * $Log: decode_kdc.c,v $
 * Revision 1.1.2.1  1996/06/05  20:59:58  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:57:32  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  19:57:32  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:18 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:41 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:06 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * lib/krb5/krb/decode_kdc.c
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
 * krb5_decode_kdc_rep() function.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include "k5-int.h"

/*
 Takes a KDC_REP message and decrypts encrypted part using etype and
 *key, putting result in *rep.
 dec_rep->client,ticket,session,last_req,server,caddrs
 are all set to allocated storage which should be freed by the caller
 when finished with the response.

 If the response isn't a KDC_REP (tgs or as), it returns an error from
 the decoding routines.

 returns errors from encryption routines, system errors
 */

krb5_error_code
krb5_decode_kdc_rep(context, enc_rep, key, etype, dec_rep)
    krb5_context context;
    krb5_data * enc_rep;
    const krb5_keyblock * key;
    const krb5_enctype etype;
    krb5_kdc_rep ** dec_rep;
{
    krb5_error_code retval;
    krb5_kdc_rep *local_dec_rep;

    if (krb5_is_as_rep(enc_rep))
	retval = decode_krb5_as_rep(enc_rep, &local_dec_rep);
    else if (krb5_is_tgs_rep(enc_rep))
	retval = decode_krb5_tgs_rep(enc_rep, &local_dec_rep);
    else
	return KRB5KRB_AP_ERR_MSG_TYPE;

    if (retval)
	return retval;

#ifdef OSF_DCE
    /* ANL - The following check is being changed, since the
     * etype being tested is only a hint at what etype to expect.
     * See the krb5_keyblock definition in krb5.h.
     * Also see the XXX comment in krb5.h before krb5_keytype_array
     * This causes a problem with tickets obtained from DCE.
     * which suggest one etype, but may have another.
     * This problem was introduced in 5 beta 5 when a second etype
     * was added.
     */
    if (krb5_csarray[local_dec_rep->enc_part.etype]->system->proto_keytype
 	   != key->keytype) {
#else
    if (local_dec_rep->enc_part.etype != etype) {
#endif
	krb5_free_kdc_rep(context, local_dec_rep);
	return KRB5_WRONG_ETYPE;
    }
    retval = krb5_kdc_rep_decrypt_proc(context, key, 0, local_dec_rep);
    if (retval) {
	krb5_free_kdc_rep(context, local_dec_rep);
	return(retval);
    }
    *dec_rep = local_dec_rep;
    return 0;
}

