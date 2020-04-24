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
 * $Log: x509.h,v $
 * Revision 1.1.2.3  1996/11/17  04:45:04  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/17  04:43:42  arvind]
 *
 * Revision 1.1.2.2  1996/11/13  20:40:54  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:26 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * 	CAPI drop from DEC
 * 	[1996/10/17  00:21 UTC  arvind  /main/DCE_1.2.2/arvind_capi/1]
 * 	Revision 1.1.2.1  1996/10/04  19:57:32  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  19:57:14  arvind]
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

#ifndef X509_
#define X509_

#ifdef NOT_DCE
#include "asn.h"
#include "x500name.h"
#else
#include <dce/asn.h>
#include <dce/x500name.h>
#endif

// Version in X.509 is EXPLICIT [0] INTEGER DEFAULT 0
class x509_version : public asn_explicit {
public:
    asn_integer value;
    x509_version(void) : asn_explicit(0) {
        register_child(&value);
        value.set_default_value(0);
        value.set_value(0);
    };
};

template<unsigned tag> class tagged_bitstring : public asn_explicit {
public:
    asn_bitstring value;
    tagged_bitstring(void) : asn_explicit(tag) {
        register_child(&value);
    };
};

class AlgorithmIdentifier : public asn_sequence {
public:
    asn_objectidentifier algorithm;
    asn_any parameters;
    AlgorithmIdentifier(void) {
	register_child(&algorithm);
	register_child(&parameters);
    };
    int operator == (const AlgorithmIdentifier & o) const;
};

class Validity : public asn_sequence {
public:
    asn_utctime notBefore;
    asn_utctime notAfter;
    Validity(void) {
	notBefore.set_century_rollover(88);
	notBefore.set_ignore_seconds();
	notAfter.set_century_rollover(88);
	notAfter.set_ignore_seconds();
        register_child(&notBefore);
        register_child(&notAfter);
    };
};

class SubjectPublicKeyInfo : public asn_sequence {
public:
    AlgorithmIdentifier algorithm;
    asn_bitstring subjectPublicKey;
    SubjectPublicKeyInfo(void) {
        register_child(&algorithm);
        register_child(&subjectPublicKey);
    };
};

class Extension : public asn_sequence {
public:
    asn_objectidentifier extnID;
    asn_boolean critical;
    asn_octetstring extnValue;
    Extension(void) {
        critical.set_default_value(0);
        critical.set_value(0);
        register_child(&extnID);
        register_child(&critical);
        register_child(&extnValue);
    };
};

class Extensions : public asn_sequenceof<Extension> {
};

template<unsigned tag> class tagged_extensions : public asn_explicit {
public:
    Extensions value;
    tagged_extensions(void) : asn_explicit(tag) {
        register_child(&value);
    };
};

class CertificateInfo : public asn_sequence {
public:
    x509_version version;
    asn_integer serialNumber;
    AlgorithmIdentifier signatureAlgorithm;
    x500name issuer;
    Validity validity;
    x500name subject;
    SubjectPublicKeyInfo subjectPublicKeyInfo;
    tagged_bitstring<1> issuerUniqueIdentifier;
    tagged_bitstring<2> subjectUniqueIdentifier;
    tagged_extensions<3> extensions;
    CertificateInfo(void) {
        register_child(&version);
        register_child(&serialNumber);
        register_child(&signatureAlgorithm);
        register_child(&issuer);
        register_child(&validity);
        register_child(&subject);
        register_child(&subjectPublicKeyInfo);
        register_child(&issuerUniqueIdentifier);
        register_child(&subjectUniqueIdentifier);
        register_child(&extensions);
	issuerUniqueIdentifier.set_optional();
	subjectUniqueIdentifier.set_optional();
	extensions.set_optional();
    };
};

class Certificate : public asn_sequence {
    buffer_t cached_verifier_key;
protected:
    virtual void value_changed(void);
public:
    CertificateInfo certificateInfo;
    AlgorithmIdentifier signatureAlgorithm;
    asn_bitstring signature;
    Certificate(void) {
        register_child(&certificateInfo);
        register_child(&signatureAlgorithm);
        register_child(&signature);
    };
    unsigned32 check_signature(const r_buffer_t & publicKeyInfo) const;
    unsigned32 sign(const r_buffer_t & privateKey);
};

template <unsigned tag> class taggedCertificate : public asn_explicit {
public:
    Certificate cert;
    taggedCertificate(void) : asn_explicit(tag) {
	register_child(&cert);
    };
};

class CrossCertificatePair : public asn_sequence {
public:
    taggedCertificate<0> forward;
    taggedCertificate<1> reverse;
    CrossCertificatePair(void) {
	forward.set_optional();
	reverse.set_optional();
// A note in ISO 9594-8 says that at least one component shall be present.
// Since this restriction can't be expressed in ASN.1, it's up to the caller
// to verify that a succesful read operation actually consumed some bytes.
	register_child(&forward);
	register_child(&reverse);
    };    
};

class Revocation : public asn_sequence {
public:
    asn_integer certificateSerialNumber;
    asn_utctime revocationDate;
    Extensions extensions;
    Revocation(void) {
	revocationDate.set_century_rollover(88);
	revocationDate.set_ignore_seconds();
	extensions.set_optional();
	register_child(&certificateSerialNumber);
	register_child(&revocationDate);
	register_child(&extensions);
    };

};

class RevokedCerts : public asn_sequenceof<Revocation> {
};

class CertificateList : public asn_sequence {
public:
    x509_version version;
    AlgorithmIdentifier signatureAlgorithm;
    x500name issuer;
    asn_utctime thisUpdate;
    asn_utctime nextUpdate;
    RevokedCerts revokedCertificates;
    tagged_extensions<0> extensions;
    CertificateList(void) {
	thisUpdate.set_century_rollover(88);
	thisUpdate.set_ignore_seconds();
        nextUpdate.set_century_rollover(88);
        nextUpdate.set_ignore_seconds();
	version.set_optional();
	revokedCertificates.set_optional();
	extensions.set_optional();
	register_child(&version);
	register_child(&signatureAlgorithm);
	register_child(&issuer);
	register_child(&thisUpdate);
	register_child(&nextUpdate);
	register_child(&revokedCertificates);
	register_child(&extensions);
    };
};

class SignedCertificateList : public asn_sequence {
    buffer_t cached_verifier_key;
protected:
    virtual void value_changed(void);
public:
    CertificateList certificateList;
    AlgorithmIdentifier signatureAlgorithm;
    asn_bitstring signature;
    SignedCertificateList(void) {
        register_child(&certificateList);
        register_child(&signatureAlgorithm);
        register_child(&signature);
    };
    unsigned32 check_signature(const r_buffer_t & publicKeyInfo) const;
    unsigned32 sign(const r_buffer_t & privateKey);
};


#endif
