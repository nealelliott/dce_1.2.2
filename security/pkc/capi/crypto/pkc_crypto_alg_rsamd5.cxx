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
 * $Log: pkc_crypto_alg_rsamd5.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:31:38  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:21:05  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:38 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:32 UTC  arvind  /main/arvind_capi/1]
 * 
 * $EndLog$
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 * 
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 * 
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 * 
 * FACILITY:    Certification API	
 * 
 * ABSTRACT:    Crypto switch routines 
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              John Wray 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */



#include <string.h>
#include <pkc_crypto_alg_rsamd5.h>
#include <sec_bsafe.h>

#define ALGORITHM_NAME "RSAwithMD5"

unsigned char rsa_md5_oid[5] = {
0x01, 0x02, 0x03, 0x04, 0x05
};

extern "C" {

static char * name_rtn(void) {
    char * ret_val;
    ret_val = (char *)pkc_malloc(strlen(ALGORITHM_NAME)+1);
    strcpy(ret_val, ALGORITHM_NAME);
    return ret_val;
}

static unsigned32 verify_rtn (void ** context,
                              sec_pk_gen_data_t data,
                              sec_pk_data_t public_key,
                              sec_pk_data_t signature) {

    return sec_bsafe_verify_data(sec_sign_md5rsa,
                                 &data,
                                 &public_key,
				 &signature);
}		       

}

pkc_signature_algorithm_t pkc_crypto_alg_rsamd5 = {
    pkc_V1,		    // Version
    {sizeof(rsa_md5_oid),   // OID.length
     rsa_md5_oid	    // OID.elements
    },
    {0,			    // flags.threadsafe
     0			    // flags.multi_session
    },
    {0},		    // reserved
    name_rtn,		    // name
    NULL,		    // open
    NULL,		    // close
    verify_rtn,		    // verify
    NULL,		    // sign
    NULL		    // generate_keypair
};

