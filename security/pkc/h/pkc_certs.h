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
 * $Log: pkc_certs.h,v $
 * Revision 1.1.2.1  1996/11/13  20:36:58  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:54:34  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:26 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:39 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:31 UTC  arvind  /main/arvind_capi/1]
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
 * ABSTRACT:    Data-Structures and proto-types for pkc_certs.cxx 
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


#ifndef PKC_CERTS_
#define PKC_CERTS_

#ifndef __cplusplus
#error "C++ must be used for direct certificate manipulation"
#endif

#include <dce/x509.h>
/*** #include <dce/pkc_platform_specific.h>  **/

extern "C" {
#include <stddef.h>
#include <dce/utc.h>
#include <dce/gssapi.h>
#include <dce/pkc_consts.h>
};

#ifdef UTC_OPERATORS

char periods_overlap(utc_t p1_start, utc_t p1_end,
                     utc_t p2_start, utc_t p2_end);
// Returns non-zero if p1 & p2 overlap

int operator == (const utc_t & o1, const utc_t & o2);
int operator != (const utc_t & o1, const utc_t & o2);
int operator <= (const utc_t & o1, const utc_t & o2);
int operator >= (const utc_t & o1, const utc_t & o2);
int operator < (const utc_t & o1, const utc_t & o2);
int operator > (const utc_t & o1, const utc_t & o2);

#endif


class pkc_generic_key_usage_t {
protected:
    virtual unsigned long pruned(unsigned long x) const;
public:
    unsigned long permitted;

    pkc_generic_key_usage_t(unsigned long permit_bits = 0xfffffffflu);
    char is_permitted(unsigned long check) const;
    char is_permitted(const pkc_generic_key_usage_t & check) const;
    void constrain(unsigned long constraint);
    void constrain(const pkc_generic_key_usage_t & constraint);
    void set(unsigned long constraints);
    pkc_generic_key_usage_t & operator = (unsigned long c);
};

class pkc_key_usage_t :  public pkc_generic_key_usage_t {
protected:
    virtual unsigned long pruned(unsigned long x) const;
public:
    pkc_key_usage_t & operator = (unsigned long c);
    pkc_key_usage_t(unsigned long c = 0xfffffffflu);
};

class pkc_ca_key_usage_t  : public pkc_generic_key_usage_t {
protected:
    virtual unsigned long pruned(unsigned long x) const;
public:
    pkc_ca_key_usage_t & operator = (unsigned long c);
    pkc_ca_key_usage_t(unsigned long c = 0xfffffffflu);
};

class pkc_key_policy_t {
public:
    gss_OID value;
    pkc_key_policy_t(void);
    virtual ~pkc_key_policy_t();
    pkc_key_policy_t & operator = (const pkc_key_policy_t & o);
    pkc_key_policy_t & operator = (const gss_OID & o);
};

class pkc_key_policies_t {
    char all_policies;
    gss_OID_set policies;  
public:
    pkc_key_policies_t(void);  // Initialize to "all policies OK".
    unsigned32 set(const pkc_key_policy_t & pol);
    unsigned32 set(const gss_OID pol);  // Adds an allowed policy
    unsigned32 set_none(void); // Sets no policies permitted
    unsigned32 set_all(void); // Sets all policies permitted
    unsigned32 constrain(const pkc_key_policies_t & pol);
    pkc_key_policies_t & operator = (const pkc_key_policies_t & pol);
};


#define PKC_NAME_CONSTRAINT_NONE 0ul
#define PKC_NAME_CONSTRAINT_CA_SUBORDINATE 1ul
#define PKC_NAME_CONSTRAINT_CA_SUPERIOR_SUBORDINATE 2ul


class pkc_name_subord_constraints_t;

class pkc_name_subord_constraint_t {
private:
    pkc_name_subord_constraints_t * root;
private:
    pkc_name_subord_constraint_t & operator = 
                      (const pkc_name_subord_constraint_t & o) {return *this;};
public:
    pkc_name_subord_constraint_t * next;
    pkc_name_subord_constraint_t * prev;

    unsigned long constraint;
    unsigned skipCerts;

    void unlink(void);

    ~pkc_name_subord_constraint_t();
    pkc_name_subord_constraint_t(pkc_name_subord_constraints_t * theRoot);
    pkc_name_subord_constraint_t(void);
    void set_constraint(unsigned long c);
    void set_skipCerts(unsigned c);
    void get_next_link_constraint(
                  pkc_name_subord_constraint_t ** new_constraint) const;
// Generate a new name-subord constraint that will be applicable to a 
// certificate issued by the subject of this constraint.

    char is_permitted(const x500name & issuer_name, 
                      const x500name & subject_name) const;
// Return value of 1 means permitted, 0 means forbidden, -1 means not 
// relevant, -2 means not relevant, but explicit permission is required from
// another subord constraint.


};


class pkc_name_subtree_constraints_t;

class pkc_name_subtree_constraint_t {
private:
    pkc_name_subtree_constraints_t * root;
private:
    pkc_name_subtree_constraint_t & operator =
                        (const pkc_name_subtree_constraint_t & o) {return *this;};
public:
    pkc_name_subtree_constraint_t * next;
    pkc_name_subtree_constraint_t * prev;

    x500name * base;
    x500name * chopBefore;
    x500name * chopAfter;
    unsigned minimum;
    unsigned maximum;

    void unlink(void);

    virtual ~pkc_name_subtree_constraint_t();
    pkc_name_subtree_constraint_t(pkc_name_subtree_constraints_t * theRoot);
    void set_base(const x500name & n);
    void set_chopBefore(const x500name & n);
    void set_chopAfter(const x500name & n);
    void set_minimum(unsigned n);
    void set_maximum(unsigned n);

    char is_permitted(const x500name & subject_name) const;
// Return value of 1 means permitted, 0 means forbidden, -1 means not 
// relevant, -2 means not relevant, but explicit permission is required from
// another subtree constraint.

    void get_next_link_constraint(
                  pkc_name_subtree_constraint_t ** new_constraint) const;
// Generate a new name-subtree constraint that will be applicable to a 
// certificate issued by the subject of this constraint.
};



class pkc_name_subord_constraints_t {
// A set of subordinate constraints on a name.
public:
    pkc_name_subord_constraints_t & operator =
                     (const pkc_name_subord_constraints_t & o);
public:
    pkc_name_subord_constraint_t * first;
    pkc_name_subord_constraint_t * last;
    pkc_name_subord_constraints_t(void);
    ~pkc_name_subord_constraints_t();
    char is_permitted(const x500name & ca_name, 
                      const x500name & subject_name) const;
    void get_next_link_constraint(
                  pkc_name_subord_constraints_t ** new_constraints) const;
    unsigned32 constrain(const pkc_name_subord_constraint_t & o);
    unsigned32 constrain(const pkc_name_subord_constraints_t & o);

};

class pkc_name_subtree_constraints_t {
// A set of subtree constraints on a name.
public:
    pkc_name_subtree_constraints_t & operator =
                        (const pkc_name_subtree_constraints_t & o);
public:
    pkc_name_subtree_constraint_t * first;
    pkc_name_subtree_constraint_t * last;
    pkc_name_subtree_constraints_t(void);
    virtual ~pkc_name_subtree_constraints_t();
    char is_permitted(const x500name & subject_name) const;
    void get_next_link_constraint(
                  pkc_name_subtree_constraints_t ** new_constraints) const;
    unsigned32 constrain(const pkc_name_subtree_constraint_t & o);
    unsigned32 constrain(const pkc_name_subtree_constraints_t & o);
};



class pkc_constraints_t {
// A class that expresses constraints on the names that can be certified by a 
// given key.  At the moment, there are three types of constraints that
// can be checked: total path-length, name subordination and subtree 
// constraints.

public:
    pkc_constraints_t & operator = (const pkc_constraints_t & o);
public:
    unsigned path_length;  // The maximum path-length that can be 
                           // certified by the key (if the entity can act 
                           // as a CA). 0xffffu means unlimited.
    pkc_name_subord_constraints_t subord_constraints;
    pkc_name_subtree_constraints_t subtree_constraints;

    pkc_constraints_t(void);
    unsigned32 constrain(const pkc_constraints_t & o);
			   // Adds the specified constraints
    char is_permitted(const x500name & ca_name, const x500name & subject_name) 
        const;

    void get_next_link_constraint(
                  pkc_constraints_t ** new_constraints) const;
// Generate a new name-constraint that will be applicable to a 
// certificate issued by the subject of this constraint.
};




class pkc_trust_list_element_t;
class pkc_trusted_key_list_t;
class pkc_trusted_key_t {
friend class pkc_trust_list_t;
// A class that expresses trust in a public-key.  Very much like a certificate,
// but with trust pre-established, rather than based on a signature.

friend unsigned32 pkc_add_trusted_key(
                         pkc_trust_list_t * trust_list,
                         const pkc_trusted_key_t & key);
// Adds a key to a trustlist.

friend unsigned32 pkc_lookup_keys_in_trustlist(
                         pkc_trust_list_t * trust_list,     /* in */
                         pkc_trusted_key_list_t * & keys,   /* out */
                         size_t * key_count,		    /* out */
                         const x500name & owner,            /* in */
                         utc_t * key_time,                  /* in, optional */
                         const pkc_generic_key_usage_t * usages);
                                                            /* in, optional */
// Retrieves keys for the specified principal for the specified usages 
// that are valid at the specified time.  If usages is NULL, keys for any 
// usage are returned; if key-time is NULL, keys for the current time are 
// returned.  Note that this routine may return a success status even when
// no keys are actually found (in this case, an empty pkc_trusted_key_list_t 
// will have been returned via the keys parameter, and must be deleted).

friend unsigned32 pkc_lookup_key_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         const pkc_trusted_key_t ** key,
			 unsigned long key_id);
// Retrieves the specified key by index.

friend unsigned32 pkc_lookup_element_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         const pkc_trust_list_element_t ** key,
			 unsigned long key_id);
// Retrieves the specified key by index, returning it as a trust-list-element

friend unsigned32 pkc_check_cert_against_trustlist(
                         pkc_trust_list_t * trust_list,
                         const Certificate * cert,
                         int revoked_certs_permitted);
// Attempt to add a certificate to a trust-list.  The routine will succeed only
// if the trust-list contains a key that can be used to verify the certificate.
// revoked_certs_permitted specifies whether it is permissible to add a 
// certificate that appears on a verified CRL if there is a "window of validity"
// between the verifying key's start-time and its revocation time.


friend unsigned32 pkc_revoke_certificate(pkc_trust_list_t * trust_list,
                                         const x500name & issued_by,
                                         const asn_integer & serial_no,
                                         utc_t * invalidate_from);
// Revoke a single certificate.

friend unsigned32 pkc_revoke_certificates(
                    pkc_trust_list_t * trust_list,
                    const SignedCertificateList * crl);
// Revoke certificates based on a CRL.  If the signature on the CRL can
// be verified, it will be processed immediately.  If not, it will be saved
// and each certificate subsequently added will attempt to verify the
// CRL.  As soon as the CRL can be verified it will be applied.

friend unsigned32 pkc_prune_trustlist(pkc_trust_list_t * trust_list,
                         const utc_t * prune_before);
// Remove certificates that have expired at the given date (NULL means now).

friend unsigned32 pkc_delete_trustlist(pkc_trust_list_t * trust_list);
// Delete a trust-list.

friend unsigned32 pkc_copy_trustlist(
                         const pkc_trust_list_t * input_trust_list,
                         pkc_trust_list_t * output_trust_list);
// Copy a trust-list.


friend unsigned32 pkc_display_trustlist(
                         const pkc_trust_list_t * input_trust_list);

protected:
// Fields from the certificate:
    utc_t start_date;
    utc_t end_date;
    pkc_ca_key_usage_t ca_usages;
    pkc_key_usage_t user_usages;
    pkc_key_policies_t policies;
    pkc_constraints_t constraints;

// Flags.  
// Is this entry trusted (a-priori)?
    char trusted;
// Is it certified by another entry?
    char certified;

    x500name certified_by;	// Name of the CA that certified this key.
    asn_integer serial_number;	// Serial no of certifying certificate 

// The following fields are copied from the certifying key entry
    utc_t certified_start_date;
    utc_t certified_end_date;

    pkc_ca_key_usage_t certified_usages;
    pkc_key_policies_t certified_policies;
    pkc_constraints_t certified_constraints;

    char revoked; // non-zero if the certifying certificate 
                  // has been revoked
    utc_t revocation_date; // Date from which certifier revocation 
                           // is effective.
    utc_t revalidation_date; // Most recent CRL we've seen that doesn't
                             // contain this certificate.


    unsigned long key_id;	// An ID identifying this Key entry
    unsigned long ca_key_id;    // The ID of the key that certified this one.
                                // 0 means direct trust.

    unsigned long old_key_id;	// Temporary storage for use while copying
    unsigned long old_ca_key_id;    

public:

    SubjectPublicKeyInfo value;  /* mbg */
    x500name owner;

    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_trusted_key_t(void);
    pkc_trusted_key_t(utc_t startDate,
                      utc_t endDate,
                      pkc_ca_key_usage_t caUsages,
                      pkc_key_usage_t userUsages,
                      pkc_key_policies_t keyPolicies,
                      pkc_constraints_t keyConstraints);
    unsigned32 get_start_date(utc_t * start_date) const;
    unsigned32 get_end_date(utc_t * start_date) const;
    unsigned32 get_key_start_date(utc_t * start_date) const;
    unsigned32 get_key_end_date(utc_t * start_date) const;
    unsigned32 get_usages(pkc_generic_key_usage_t * all_usages) const;
    unsigned32 get_user_usages(pkc_key_usage_t * user_usages) const;
    unsigned32 get_ca_usages(pkc_ca_key_usage_t * ca_usages) const;
    unsigned32 get_key_policies(pkc_key_policies_t * policies) const;
    unsigned32 get_constraints(pkc_constraints_t * constraints) const;
    unsigned32 get_certifier(x500name & name) const;
    unsigned32 get_revalidation_date(utc_t * start_date) const;
    unsigned32 get_revocation_date(utc_t * start_date) const;
    unsigned long get_key_id(void) const;
    unsigned long get_ca_key_id(void) const;
    char is_revoked(void) const;
    char is_trusted(void) const;
    char is_certified(void) const;
    char valid_at(utc_t * time) const;

    pkc_trusted_key_t & operator = (const pkc_trusted_key_t & o);
    unsigned32 copy (const pkc_trusted_key_t * src);

    char may_certify(
            const x500name & subject, 
            unsigned long usage = PKC_CAKEY_USAGE_KEY_CERT_SIGN) const;

    char may_certify(
            const x500name & subject, 
            const pkc_ca_key_usage_t & usage) const;

};


class pkc_revocation_t;
class pkc_pending_revocation_t;
class pkc_revocation_list_t {
friend class pkc_trust_list_t;
// A list of revoked certificates and their dates.
private:
    pkc_revocation_list_t & operator = 
                      (const pkc_revocation_list_t & o);
public:
    pkc_revocation_t * first;
    pkc_revocation_t * last;
    pkc_pending_revocation_t * first_pending;
    pkc_pending_revocation_t * last_pending;
    unsigned32 get_revocation_date(const pkc_revocation_t & o,
                                   utc_t * revocationDate) const;
    unsigned32 get_revocation_date(const x500name & issuer,
                                   const asn_integer & serialNumber,
                                   utc_t * revocationDate) const;
    unsigned32 add_revocation(const pkc_revocation_t & o);
    unsigned32 add_revocation(const x500name & issuer,
                              const asn_integer & serialNumber,
                              utc_t * revocationDate);
    unsigned32 add_key(pkc_trust_list_t * trust_list,
                       const SubjectPublicKeyInfo & key,
                       const x500name & subject,
                       const utc_t & start_date,
                       const utc_t & end_date,
                       const pkc_ca_key_usage_t * usages = NULL);
    pkc_pending_revocation_t * find_pending(const x500name & issuer) const;
    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_revocation_list_t(void);
    virtual ~pkc_revocation_list_t();
    void empty(void);
};

class pkc_pending_revocation_t {
friend class pkc_revocation_list_t;
    pkc_revocation_list_t * root;
    pkc_pending_revocation_t * prev;
    pkc_pending_revocation_t * next;
public:
    SignedCertificateList crl;
    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_pending_revocation_t(const SignedCertificateList & crl,
                             pkc_revocation_list_t * the_root = NULL);
    pkc_pending_revocation_t & operator = (const pkc_pending_revocation_t & o);
    virtual void unlink(void);
    virtual ~pkc_pending_revocation_t();
};

class pkc_trusted_key_list_element_t {
private:
    pkc_trusted_key_list_t * root;
public:
    pkc_trusted_key_t * key;
    pkc_trusted_key_list_element_t * next;
    pkc_trusted_key_list_element_t * prev;
    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_trusted_key_list_element_t(pkc_trusted_key_list_t * theRoot);
    virtual ~pkc_trusted_key_list_element_t();
    virtual void unlink(void);
};

class pkc_trusted_key_list_t {
public:
    pkc_trusted_key_list_element_t * first;
    pkc_trusted_key_list_element_t * last;
public:
    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_trusted_key_list_t(void);
    void empty(void);
    virtual ~pkc_trusted_key_list_t(void);
};


class pkc_revocation_t {
friend class pkc_revocation_list_t;
    pkc_revocation_list_t * root;
    pkc_revocation_t * prev;
    pkc_revocation_t * next;
public:
    x500name certIssuer;
    asn_integer certSerialNumber;
    utc_t certRevocationDate;

    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_revocation_t(const x500name & issuer,
                     const asn_integer & serialNumber,
                     utc_t revocationDate,
		     pkc_revocation_list_t * the_root = NULL);
    pkc_revocation_t & operator = (const pkc_revocation_t & o);

    virtual void unlink(void);
    virtual ~pkc_revocation_t();
};


class pkc_crl_seen_date_t;
class pkc_crl_seen_date_list_t {
friend class pkc_trust_list_t;
// A list of "CRL-seen" dates.
private:
    pkc_crl_seen_date_list_t & operator = 
                      (const pkc_crl_seen_date_list_t & o);
protected:
    pkc_crl_seen_date_t * find(const x500name & issuer) const;
public:
    pkc_crl_seen_date_t * first;
    pkc_crl_seen_date_t * last;
    unsigned32 get_last_crl_seen_date(const x500name & issuer,
                                      utc_t * crlSeenDate) const;
    unsigned32 get_next_crl_expected_date(const x500name & issuer,
                                          utc_t * crlExpectedDate) const;
    unsigned32 add_crl(const x500name & issuer,
                       const utc_t & crlSeenDate,
                       const utc_t & crlExpectedDate);
    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_crl_seen_date_list_t(void);
    virtual ~pkc_crl_seen_date_list_t();
    void empty(void);
};

class pkc_crl_seen_date_t {
friend class pkc_crl_seen_date_list_t;
    pkc_crl_seen_date_list_t * root;
    pkc_crl_seen_date_t * prev;
    pkc_crl_seen_date_t * next;
public:
    x500name crlIssuer;
    utc_t crlDate;
    utc_t crlNextDate;

    virtual unsigned32 display(buffer_t & buffer) const;
    pkc_crl_seen_date_t(const x500name & issuer,
                        const utc_t & seenDate,
                        const utc_t & nextDate,
		        pkc_crl_seen_date_list_t * the_root = NULL);
    pkc_crl_seen_date_t & operator = (const pkc_crl_seen_date_t & o);

    virtual void unlink(void);
    virtual ~pkc_crl_seen_date_t();
};

class pkc_trust_list_t {
friend class pkc_trust_list_element_t;
public:

    pkc_revocation_list_t revocation_list;  // List of revocations
    pkc_crl_seen_date_list_t revalidation_list;  // List of revalidations

    pkc_trust_list_element_t * first;
    pkc_trust_list_element_t * last;
    pkc_trust_list_t(void);
    virtual unsigned32 display(buffer_t & buffer) const;
    void empty(void);
    virtual ~pkc_trust_list_t();
    pkc_trust_list_t & operator = (const pkc_trust_list_t & o);
    unsigned32 fixup_links(void);
    unsigned32 find_certified_key(const x500name & certifier,
                                  const asn_integer & certifying_serial_no,
                                  pkc_trust_list_element_t ** key);
// Returns the first key entry that was created from the specified certificate.
// Call find_next_certified_key() to return the next such key.

    unsigned32 find_next_certified_key(pkc_trust_list_element_t ** key);
// Returns the next key entry that was created from the same certificate as
// the current entry.  "key" is both an input and an output.

    unsigned32 find_certified_key_by_id(unsigned long ca_key_id,
                                        pkc_trust_list_element_t ** key);
// Returns the first key entry that was certified by the specified key-id
// Call find_next_certified_key_by_id() to return the next such key.

    unsigned32 find_next_certified_key_by_id(pkc_trust_list_element_t ** key);
// Returns the next key entry that was certified by the same key as
// the current entry.  "key" is both an input and an output.

    unsigned32 process_crl(const SignedCertificateList *crl);
// Applies a previously verifed CRL to the trust-list.
};


class pkc_trust_list_element_t : public pkc_trusted_key_t {
friend class pkc_trust_list_t;
// A list of trusted keys.
private:
    pkc_trust_list_element_t & operator = 
                      (const pkc_trust_list_element_t & o) {return *this;};
private:
    pkc_trust_list_t * root;
public:
    pkc_trust_list_element_t * next;
    pkc_trust_list_element_t * prev;
    void unlink(void);
    pkc_trust_list_element_t(pkc_trust_list_t & the_root);
    pkc_trust_list_element_t(pkc_trust_list_t & the_root, 
                             utc_t startDate,
                             utc_t endDate,
                             pkc_ca_key_usage_t caUsages,
                             pkc_key_usage_t userUsages,
                             pkc_key_policies_t keyPolicies,
                             pkc_constraints_t keyConstraints);
    virtual ~pkc_trust_list_element_t();
    unsigned32 apply_revocation(utc_t * revocation_date);
// Apply a revocation to this key, starting at the specified date.
// If revocation_date is NULL, the key is completely revoked.
// This key, and all keys dependent on it will be revoked.

    unsigned32 get_certifier_key(const pkc_trusted_key_t ** ca) const;
    unsigned32 get_certifier(const pkc_trust_list_element_t ** ca) const;

};

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// The trustlist is the fundamental object within the certificate manipulation 
// facility.  A trustlist is a set of keys which are trusted, plus a list
// of revoked certificate serial numbers.  Keys are inserted into a trustlist 
// either directly (via the pkc_add_trusted_key() function) or indirectly 
// (via the pkc_check_cert_against_trustlist() function).  The latter routine
// will only add keys if the certificate signature can be verified by a
// key already in the trustlist, and if the certificate has not been revoked.
//
// Currently, trustlists are relatively static objects - once a key is
// inserted, its trust properties don't change.  If, for example, a 
// key is added that is capable of extending the trust in another key
// within the list, the second key isn't automatically updated.  
// Auto-update is a matter for the next version.
// 
// The way that a policy module is expected to use this stuff is to first
// create an initial trustlist containing the directly trusted keys, that
// is, the start point(s) of all valid trust chains.  Typically, this set of
// keys will be used for multiple certificate chain evaluations;  If the
// policy wishes to impose additional path constraints over the constraints
// expressed within the certificates, it must maintain a master copy of the 
// original trustlist and clone it to create a modifiable version
// for each chain the policy module wants to verify.  After verification of
// a candidate chain, the cloned trustlist must be discarded so that the
// next trial verification starts from a known state.
//
// Using the initial trustlist as a starting point, the policy module
// retrieves a chain of certificates and adds them to the trustlist one
// by one, starting with the certificate(s) closest to the start point(s).
// Multiple chains may be evaluated simultaneously using a single trustlist
// for policies that do not wish to impose additional constraints on the
// trust chain;  however the policy module must ensure that for each 
// trust-chain, certificates are added in the correct order.  A future 
// auto-update enhancement may lift this requirement.
//
// ---------------------------------------------------------------------------


unsigned32 pkc_add_trusted_key(
                         pkc_trust_list_t * trust_list,  /* mbg: */
                         const pkc_trusted_key_t & key);
// Adds the specified key to the specified trustlist.

unsigned32 pkc_lookup_keys_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         const pkc_trusted_key_t ** key,
                         size_t * key_count,
                         const x500name & owner,
                         utc_t * key_time = NULL,
                         const pkc_generic_key_usage_t * usages = NULL);
// Searches the specified trustlist for keys owned by the specified
// principal at the specified time for the specified usage.  The keys are
// returned in an array of pointers to pkc_trusted_key_t objects, which is
// allocated on the heap; the pointers point to elements within the trust-list,
// thus the caller should copy into allocatead storage if they are expected
// to remain valid after the deletion of the trust-list.

unsigned32 pkc_lookup_key_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         const pkc_trusted_key_t ** key,
			 unsigned long key_id);
// Searches the specified trustlist for the specified key.

unsigned32 pkc_lookup_element_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         const pkc_trust_list_element_t ** key,
			 unsigned long key_id);
// Searches the specified trustlist for the specified key.

unsigned32 pkc_check_cert_against_trustlist(
                         pkc_trust_list_t * trust_list,
                         const Certificate * cert,
                         int revoked_certs_permitted);
// Checks the specified certificate against the specified list of trusted keys. 
// If the certificate is valid and can be verified from the trustlist, its
// content is added to the trustlist.  revoked_certs_permitted is a flag
// that say whether revoked certificates should still be trusted for dates 
// prior to their revocation date.


unsigned32 pkc_revoke_certificate(pkc_trust_list_t * trust_list,
                                  const x500name & issued_by,
                                  const asn_integer & serial_no,
                                  utc_t * invalidate_from = NULL);
// Applies the specified revocation to the specified trustlist (i.e. revokes
// a key and all dependent keys).  If invalidate_from is NULL, the key is
// completely revoked; if a valid UTC-time is provided, the key is revoked from
// that time on.  The revocation is stored within the trustlist, and any 
// subsequent attempts to add the certificate will be rejected.

unsigned32 pkc_revoke_certificates(
                    pkc_trust_list_t * trust_list,
                    const SignedCertificateList * crl);
// Processes a certificate revocation list.  The revocations within
// the CRL are applied to the specified trustlist (i.e. revoking
// a set of keys and all dependent keys).  The revocations are stored 
// within the trustlist, and any subsequent attempts to add a revoked 
// certificate will be rejected.  In addition, the CRL as a whole
// serves to revalidate certificates stored within the sphere of
// responsibility specified by <revalidate>.


unsigned32 pkc_prune_trustlist(pkc_trust_list_t * trust_list,
                               const utc_t * prune_before = NULL);
// Prunes keys that expired before the stated time from a trustlist.  If
// prune_before is NULL, all keys that have expired are pruned.


unsigned32 pkc_display_trustlist(
                         const pkc_trust_list_t * input_trust_list);
// Prints the contents of the trustlist to stdout.  Intended for use
// when debugging policy modules.


#endif
