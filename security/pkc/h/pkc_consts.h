/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, I
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: pkc_consts.h,v $
 * Revision 1.1.2.1  1996/11/13  20:37:22  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:54:52  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/11/04  22:26 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:27 UTC  arvind  /main/arvind_capi2/1]
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
 * ABSTRACT:    Various constants, common to multiple layers
 *
 *
 * ENVIRONMENT: portable
 *
 * AUTHOR:
 *              John Wray
 *              Distributed Processing Engineering (DPE)
 *
 * DATE:        March 1996
 *
 */

#ifndef PKC_CONSTS_H_
#define PKC_CONSTS_H_

#include <dce/nbase.h>
#include <stddef.h>

extern uuid_t sec_pk_domain_unspecified;

#define PKC_KEY_USAGE_MASK 0xfffflu
#define PKC_CAKEY_USAGE_MASK 0xffff0000lu
// Allow up to 16 distinct usages per key-type

#define PKC_KEY_USAGE_AUTHENTICATION 0x01lu
#define PKC_KEY_USAGE_INTEGRITY 0x02lu
#define PKC_KEY_USAGE_KEY_ENCIPHERMENT 0x04lu
#define PKC_KEY_USAGE_DATA_ENCIPHERMENT 0x08lu
#define PKC_KEY_USAGE_KEY_AGREEMENT 0x10lu
#define PKC_KEY_USAGE_NONREPUDIATION 0x20lu

#define PKC_CAKEY_USAGE_KEY_CERT_SIGN 0x010000lu
#define PKC_CAKEY_USAGE_OFFLINE_CRL_SIGN 0x020000lu
#define PKC_CAKEY_USAGE_TRANSACTION_SIGN 0x040000lu
 
extern const unsigned char * RSA_LOW_ASSURANCE_ROOT_KEY;
extern const size_t RSA_LOW_ASSURANCE_ROOT_KEY_LENGTH;
extern const char RSA_LOW_ASSURANCE_ROOT_NAME[];

#endif
