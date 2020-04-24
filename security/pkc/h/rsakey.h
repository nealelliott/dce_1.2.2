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
 * $Log: rsakey.h,v $
 * Revision 1.1.2.2  1996/11/13  20:39:58  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/17  00:16 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 *
 * Revision 1.1.2.1  1996/10/04  19:49:23  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  19:49:01  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:26 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  16:11 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */


#ifndef RSAKEY_
#define RSAKEY_

#include <dce/asn.h>
#include <dce/x509.h>
#include <dce/x501.h>


class RSAPrivateKey : public asn_sequence{
public:
    asn_integer version;
    asn_integer modulus;
    asn_integer publicExponent;
    asn_integer privateExponent;
    asn_integer prime1;
    asn_integer prime2;
    asn_integer exponent1;
    asn_integer exponent2;
    asn_integer coefficient;
    RSAPrivateKey(void) {
	register_child(&version);
        register_child(&modulus);
        register_child(&publicExponent);
        register_child(&privateExponent);
        register_child(&prime1);
        register_child(&prime2);
        register_child(&exponent1);
        register_child(&exponent2);
        register_child(&coefficient);
    };
};

class RSAPublicKey : public asn_sequence {
public:
    asn_integer modulus;
    asn_integer publicExponent;
    RSAPublicKey(void) {
	register_child(&modulus);
	register_child(&publicExponent);
    };
};


class Attributes : public asn_setof<Attribute> {
};


class PrivateKeyInfo : public asn_sequence {
public:
    asn_integer version;
    AlgorithmIdentifier privateKeyAlgorithm;
    asn_octetstring privateKey;
    Attributes attributes;
    PrivateKeyInfo(void) {
	register_child(&version);
	register_child(&privateKeyAlgorithm);
	register_child(&privateKey);
	register_child(&attributes);
	attributes.set_optional();
    };    
};

#endif
