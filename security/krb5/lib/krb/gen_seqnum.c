/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gen_seqnum.c,v $
 * Revision 1.1.6.2  1996/02/18  00:13:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:03:37  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:59:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:59:50  root]
 * 
 * Revision 1.1.2.2  1992/12/29  15:55:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:14:15  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:51:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  gen_seqnum.c V=1 06/06/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/gen_seqnum.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
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
 * Routine to automatically generate a starting sequence number.
 * We do this by getting a random key and encrypting something with it,
 * then taking the output and slicing it up.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_gen_seqnum_c[] =
"$Id: gen_seqnum.c,v 1.1.6.2 1996/02/18 00:13:57 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

krb5_error_code
krb5_generate_seq_number(key, seqno)
const krb5_keyblock *key;
krb5_int32 *seqno;
{
    krb5_pointer random_state;
    krb5_encrypt_block eblock;
    krb5_keyblock *subkey;
    krb5_error_code retval;
    struct tval {
	krb5_int32 seconds;
	krb5_int32 microseconds;
    } timenow;
    krb5_octet *intmp, *outtmp;
    int esize;
    char *outseqno;

    if (!valid_keytype(key->keytype))
	return KRB5_PROG_KEYTYPE_NOSUPP;

    krb5_use_keytype(&eblock, key->keytype);

    if (retval = krb5_init_random_key(&eblock, key, &random_state))
	return(retval);
	
    if (retval = krb5_random_key(&eblock, random_state, &subkey)) {
	(void) krb5_finish_random_key(&eblock, random_state);
	return retval;
    }	
    /* ignore the error if any, since we've already gotten the key out */
    if (retval = krb5_finish_random_key(&eblock, &random_state)) {
	krb5_free_keyblock(subkey);
	return retval;
    }

    esize = krb5_encrypt_size(sizeof(timenow), eblock.crypto_entry);
    intmp = (krb5_octet *)malloc(esize);
    if (!intmp) {
	krb5_free_keyblock(subkey);
	return ENOMEM;
    }
    outtmp = (krb5_octet *)malloc(esize);
    if (!outtmp) {
	xfree(intmp);
	krb5_free_keyblock(subkey);
	return ENOMEM;
    }
    if (retval = krb5_process_key(&eblock, subkey)) {
	goto cleanup;
    }
    outseqno = (char *)seqno;

    if (retval = krb5_us_timeofday(&timenow.seconds,
				   &timenow.microseconds)) {
	goto cleanup;
    }
    memcpy((char *)intmp, (char *)&timenow, sizeof(timenow));

    while (outseqno < (char *)(seqno+1)) {
	memset((char *)intmp, 0, esize);

	if (retval = krb5_encrypt((krb5_pointer)intmp,
				  (krb5_pointer)outtmp,
				  sizeof(timenow),
				  &eblock,
				  0)) {
	    (void) krb5_finish_key(&eblock);
	    goto cleanup;
	}
	memcpy(outseqno, (char *)outtmp, MIN((char *)(seqno+1)-outseqno,
					     esize));
	outseqno += MIN((char *)(seqno+1)-outseqno, esize);
	/* chain along */
	memcpy((char *)intmp,(char *)outtmp,esize);
    }
    (void) krb5_finish_key(&eblock);
 cleanup:
    krb5_free_keyblock(subkey);
    xfree(intmp);
    xfree(outtmp);
    return retval;
}

