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
 * $Log: x509.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:30:25  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:57:38  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:26 UTC  arvind
 * 	     Cert-API drop 2 from DEC
 * 	     [1996/11/03  23:22 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:37 UTC  arvind
 * 	     CAPI drop from DEC
 * 	     [1996/10/16  20:24 UTC  arvind  /main/arvind_capi/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module: X509.CXX
 *
 * Purpose: Implementation of X.509 certificates
 *
 */
 

#include <dce/x509.h>
#include <stdlib.h>
#include <dce/gssapi.h>
#include <dce/pkc_crypto_reg.h>


int AlgorithmIdentifier::operator == (const AlgorithmIdentifier & o) const {
    if (!(algorithm == o.algorithm)) return 0;
    return 1==1; // For the moment, until we want to really compare parameters
//    return (parameters == o.parameters);
}

void Certificate::value_changed(void) {
    cached_verifier_key.empty();
    asn_sequence::value_changed();
}

unsigned32 Certificate::check_signature(const r_buffer_t & publicKeyInfo)
const {

    Certificate * this_var = (Certificate *)this;
    
    unsigned32 res;
    gss_OID alg_oid = NULL;
    OM_uint32 min_stat;
    buffer_t toBeSigned;
    sec_pk_gen_data_t to_be_signed;
    sec_pk_data_t key;
    sec_pk_data_t sig;
    unsigned32 signature_bit_len;
        
    if (publicKeyInfo.data_length == 0) return pkc_s_invalid_public_key;

    if ((publicKeyInfo.data_length == cached_verifier_key.data_length) &&
        (memcmp(publicKeyInfo.data,
                cached_verifier_key.data,
                publicKeyInfo.data_length) == 0)) {
	// No need to re-verify certificate signature, as we know this key
	// works.
		return 0;
    } else if (cached_verifier_key.data_length != 0) {
	// This key is different to the one we know works, so fail.
	return pkc_s_cert_not_verified_signature;
    };
// If we reach here we have to actually do crypto.
// First, check that the two signatureAlgorithm fields match...
    if (!(signatureAlgorithm == certificateInfo.signatureAlgorithm)) {
	return pkc_s_cert_invalid_algid;
    };

// Extract the algorithm OID from the certificate...
    res = certificateInfo.signatureAlgorithm.algorithm.get_value(alg_oid);

// Next the certificateInfo field into a buffer

    if (res = certificateInfo.write(toBeSigned)) return res;
    to_be_signed.data = toBeSigned.data;
    to_be_signed.len = toBeSigned.data_length;

    key.data = publicKeyInfo.data;    
    key.len = publicKeyInfo.data_length;    

    signature.get_value(sig.data, signature_bit_len);
    sig.len = signature_bit_len/8;

    res = pkc_crypto_verify_signature(alg_oid,
                                      to_be_signed,
                                      key,
                                      sig);

    gssdce_release_oid(&min_stat, &alg_oid);

    if (res) return res;

// The signature checked out - retain the key to speed up repeat checks
    this_var->cached_verifier_key.clear();
    this_var->cached_verifier_key.append(publicKeyInfo);

    return 0;
}

unsigned32 Certificate::sign(const r_buffer_t & privateKey) {
    return pkc_s_unimplemented;
}

void SignedCertificateList::value_changed(void) {
    cached_verifier_key.empty();
    asn_sequence::value_changed();
}

unsigned32 SignedCertificateList::check_signature(
                              const r_buffer_t & publicKeyInfo)
const {

    SignedCertificateList * this_var = (SignedCertificateList *)this;
    
    unsigned32 res;
    gss_OID alg_oid = NULL;
    OM_uint32 min_stat;
    buffer_t toBeSigned;
    sec_pk_gen_data_t to_be_signed;
    sec_pk_data_t key;
    sec_pk_data_t sig;
    unsigned32 signature_bit_len;
        
    if (publicKeyInfo.data_length == 0) return pkc_s_invalid_public_key;

    if ((publicKeyInfo.data_length == cached_verifier_key.data_length) &&
        (memcmp(publicKeyInfo.data,
                cached_verifier_key.data,
                publicKeyInfo.data_length) == 0)) {
	// No need to re-verify certificateList signature, as we know this key
	// works.
		return 0;
    } else if (cached_verifier_key.data_length != 0) {
	// This key is different to the one we know works, so fail.
	return pkc_s_cert_not_verified_signature;
    };
// If we reach here we have to actually do crypto.
// First, check that the two signatureAlgorithm fields match...
    if (!(signatureAlgorithm == certificateList.signatureAlgorithm)) {
	return pkc_s_cert_invalid_algid;
    };

// Extract the algorithm OID from the certificateList...
    res = certificateList.signatureAlgorithm.algorithm.get_value(alg_oid);

// Next the certificateList field into a buffer

    if (res = certificateList.write(toBeSigned)) return res;
    to_be_signed.data = toBeSigned.data;
    to_be_signed.len = toBeSigned.data_length;

    key.data = publicKeyInfo.data;    
    key.len = publicKeyInfo.data_length;    

    signature.get_value(sig.data, signature_bit_len);
    sig.len = signature_bit_len/8;

    res = pkc_crypto_verify_signature(alg_oid,
                                      to_be_signed,
                                      key,
                                      sig);

    gssdce_release_oid(&min_stat, &alg_oid);

    if (res) return res;

// The signature checked out - retain the key to speed up repeat checks
    this_var->cached_verifier_key.clear();
    this_var->cached_verifier_key.append(publicKeyInfo);

    return 0;
}

unsigned32 SignedCertificateList::sign(const r_buffer_t & privateKey) {
    return pkc_s_unimplemented;
}

