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
 * $Log: pkc_certs.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:34:18  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:18:05  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:39 UTC  arvind
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
 * ABSTRACT:    This module implements low-level API for checking signatures,etc 
 *               
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

extern "C" {
#include <assert.h>
}
#define IF_NULL_NOW(x)     \
    utc_t now;             \
    if (x == NULL) {       \
       x = &now;           \
       utc_gettime(x);     \
    }                     


#ifndef UTC_OPERATORS
#define UTC_OPERATORS
#endif
#include <dce/pkc_certs.h>

char periods_overlap(utc_t p1_start, utc_t p1_end,
                     utc_t p2_start, utc_t p2_end) {

    if ((p1_start >= p2_start) && (p1_start <= p2_end)) return 1;
    if ((p2_start >= p1_start) && (p2_start <= p1_end)) return 1;
    return 0;
}
		     
static unsigned long usl_from_pointer(void * p) {
// This routine returns a hash of the presented pointer value.
// On platforms where sizeof(void *) > sizeof(unsigned long),
// this routine will have to be rewritten.

    unsigned long res = 0;

    assert(sizeof(void *) >= sizeof(unsigned long));

    memcpy(&res, &p, sizeof(void *));

    return res;

}

unsigned32 pkc_trusted_key_t::get_start_date(utc_t * ret_date) const {
    enum utc_cmptype res;

    if (!trusted) return pkc_s_key_not_trusted;
    *ret_date = start_date;
    if (certified) {
// modify the startdate if necessary
	if (utc_cmpmidtime(&res,
	                  (utc_t *)&certified_start_date,
	                  (utc_t *)&start_date)) return pkc_s_bad_time;
        if (res == utc_greaterThan) {
//	    certified_start_date > start_date
            *ret_date = certified_start_date;
	};
    };
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_end_date(utc_t * ret_date) const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_date = end_date;
    if (certified) {
// modify the enddate if necessary
	if (certified_end_date < end_date) {
            *ret_date = certified_end_date;
	};
    };
    if (revoked) {
	if (revocation_date < *ret_date) *ret_date = revocation_date;
	return pkc_s_certificate_revoked;
    };
    return 0;
}


unsigned32 pkc_trusted_key_t::get_key_start_date(utc_t * ret_date) const {
    enum utc_cmptype res;

    if (!trusted) return pkc_s_key_not_trusted;
    *ret_date = start_date;
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_key_end_date(utc_t * ret_date) const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_date = end_date;
    if (revoked) {
	if (revocation_date < *ret_date) *ret_date = revocation_date;
	return pkc_s_certificate_revoked;
    };
    return 0;
}

unsigned32 pkc_trusted_key_t::get_revalidation_date(utc_t * ret_date) const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_date = revalidation_date;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_revocation_date(utc_t * ret_date) const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_date = revocation_date;
    return 0;
}

unsigned long pkc_trusted_key_t::get_key_id(void) const {
    return key_id;
}

unsigned long pkc_trusted_key_t::get_ca_key_id(void) const {
    return ca_key_id;
}

char pkc_trusted_key_t::is_revoked(void) const {
    return revoked;
}

char pkc_trusted_key_t::is_trusted(void) const {
    return trusted;
}

char pkc_trusted_key_t::is_certified(void) const {
    return certified;
}


unsigned32 pkc_trusted_key_t::get_usages(pkc_generic_key_usage_t * ret_usages) const {
    ret_usages->permitted = 0;
    if (!trusted) return pkc_s_key_not_trusted;
    ret_usages->permitted = ca_usages.permitted | user_usages.permitted; 
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_user_usages(pkc_key_usage_t * ret_usages) const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_usages = user_usages;
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_ca_usages(pkc_ca_key_usage_t * ret_usages)
const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_usages = ca_usages;
    if (certified) ret_usages->constrain(certified_usages);
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_key_policies(
				pkc_key_policies_t * ret_policies)
const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_policies = policies;
    if (certified) ret_policies->constrain(certified_policies);
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_constraints(
                                     pkc_constraints_t * ret_constraints)
const {
    if (!trusted) return pkc_s_key_not_trusted;
    *ret_constraints = constraints;
    if (certified) ret_constraints->constrain(certified_constraints);
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trusted_key_t::get_certifier(x500name & name) const {
    if (!trusted) return pkc_s_key_not_trusted;
    if (!certified) return pkc_s_key_not_certified;
    name = certified_by;
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}


pkc_trusted_key_t::pkc_trusted_key_t(void) {
    key_id = usl_from_pointer(this);
    ca_key_id = 0;
    trusted = 0;
    revoked = 0;
    certified = 0;
}

char pkc_trusted_key_t::valid_at(utc_t * time) const {
    utc_t start;
    utc_t end;
    unsigned32 res;
    IF_NULL_NOW(time);

    res = get_start_date(&start);
    if ((res != 0) && (res != pkc_s_certificate_revoked)) return 0;
    res = get_end_date(&end);
    if ((res != 0) && (res != pkc_s_certificate_revoked)) return 0;

    if (*time < start) return 0;
    if (*time > end) return 0;
    return 1;
}

pkc_trusted_key_t::pkc_trusted_key_t(utc_t startDate,
                                     utc_t endDate,
                                     pkc_ca_key_usage_t caUsages,
                                     pkc_key_usage_t userUsages,
                                     pkc_key_policies_t keyPolicies,
                                     pkc_constraints_t keyConstraints) {

    start_date = startDate;
    end_date = endDate;
    ca_usages = caUsages;
    user_usages = userUsages;
    policies = keyPolicies;
    constraints = keyConstraints;

    trusted = 1;
    revoked = 0;
    certified = 0;

    revalidation_date = startDate;

    key_id = usl_from_pointer(this);
    ca_key_id = 0;
}


void pkc_name_subord_constraint_t::unlink(void) {
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (root == NULL) return;
    if (this == root->first) root->first = next;
    if (this == root->last) root->last = prev;
}
						 
void pkc_name_subtree_constraint_t::unlink(void) {
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (root == NULL) return;
    if (this == root->first) root->first = next;
    if (this == root->last) root->last = prev;
}

pkc_name_subtree_constraint_t::~pkc_name_subtree_constraint_t() {
    unlink();
    if (base) delete base;
    if (chopBefore) delete chopBefore;
    if (chopAfter) delete chopAfter;
}

pkc_name_subtree_constraint_t::pkc_name_subtree_constraint_t(
                            pkc_name_subtree_constraints_t * theRoot) {

    root = theRoot;
    next = NULL;
    prev = root->last;
    if (prev) {
        prev->next = this;
    } else {
        root->first = this;
    };
    root->last = this;

    base = NULL;
    chopBefore = NULL;
    chopAfter = NULL;
    minimum = 0;
    maximum = 0xffffu;

}

void pkc_name_subtree_constraint_t::set_base(const x500name & n) {
    base = new x500name;
    *base = n;
}

void pkc_name_subtree_constraint_t::set_chopBefore(const x500name & n) {
    chopBefore = new x500name;
    *chopBefore = n;
}

void pkc_name_subtree_constraint_t::set_chopAfter(const x500name & n) {
    chopAfter = new x500name;
    *chopAfter = n;
}

void pkc_name_subtree_constraint_t::set_minimum(unsigned n) {
    minimum = n;
}

void pkc_name_subtree_constraint_t::set_maximum(unsigned n) {
    maximum = n;
}

						 
char pkc_name_subord_constraint_t::is_permitted(
                                      const x500name & ca_name,
                                      const x500name & subject_name) const {
    x500name * casSuperior = NULL;
    unsigned32 res;    

    if (skipCerts != 0) return 1;
// We're into the range where we have to apply this constraint.
    switch (constraint) {
    case PKC_NAME_CONSTRAINT_NONE:
	return 1;
    case PKC_NAME_CONSTRAINT_CA_SUBORDINATE:
	if (subject_name.is_descendant(ca_name)) return 1;
	else return 0;
    case PKC_NAME_CONSTRAINT_CA_SUPERIOR_SUBORDINATE:
	if (res = ca_name.make_ancestor(1, &casSuperior)) {
// Error, so fail constraint check
	    if (casSuperior) delete casSuperior;
	    return 0;
	};
	if (casSuperior == NULL) return 1;
	if (subject_name.is_descendant(*casSuperior)) {
	    delete casSuperior;
	    return 1;
	};
	delete casSuperior;
	return 0;
    default:
	return 0; // Error, so fail constraint check
    };    
}

char pkc_name_subord_constraints_t::is_permitted(
                                      const x500name & ca_name,
                                      const x500name & subject_name) const {
    pkc_name_subord_constraint_t * ptr;
    char explicit_permission_required = 0;
    char res;
    
    for (ptr = first; ptr != NULL; ptr = ptr->next) {
	res = ptr->is_permitted(ca_name, subject_name);
	if (res == 1) break;
	if (res == 0) return 0;
// Return value of -1 means not relevant; return value of -2 means not
// relevant, but explicit permission is required.
	if (res == -2) explicit_permission_required = 1;
    };
    if ((ptr == NULL) && explicit_permission_required) return 0;
    return 1;
}

char pkc_name_subtree_constraint_t::is_permitted(
                                      const x500name & subject_name) const {
    x500name * casSuperior = NULL;
    unsigned32 res;    

    if (base == NULL) return 1; // Null constraint - ignore

// For now, just check that we're in the proper subtree.  When we fully
// support V3 certs we can re-visit this area.

    if (subject_name.is_descendant(*base)) return 1;     

    return 0;
}

char pkc_name_subtree_constraints_t::is_permitted(
                                      const x500name & subject_name) const {
    pkc_name_subtree_constraint_t * ptr;
    char explicit_permission_required = 0;
    char res;
    
    for (ptr = first; ptr != NULL; ptr = ptr->next) {
	res = ptr->is_permitted(subject_name);
	if (res == 1) break;
	if (res == 0) return 0;
// Return value of -1 means not relevant; return value of -2 means not
// relevant, but explicit permission is required.
	if (res == -2) explicit_permission_required = 1;
    };
    if ((ptr == NULL) && explicit_permission_required) return 0;
    return 1;
}

char pkc_constraints_t::is_permitted(const x500name & ca_name,
                                     const x500name & subject_name) const {
    char res;
    char explicit_permission_required;
    pkc_name_subtree_constraint_t * subtree_ptr;

// Checks:
// 1) Path length not exceeded
// 2) Name subordination constraints OK
// 2) Subtree constraints OK

// Check 1:
    if (path_length == 0) return 0;
    
// Check 2:
    if (!subord_constraints.is_permitted(ca_name, subject_name)) return 0;

// Check 3:
    if (!subtree_constraints.is_permitted(subject_name)) return 0;

    return 1;
}

char pkc_trusted_key_t::may_certify(
            const x500name & subject, 
            const pkc_ca_key_usage_t & usage) const {
// Checks to make:
// 1) Is usage permitted by certifier?
// 2) Does subject name pass certifier's constraints?

    error_status_t res;
    pkc_ca_key_usage_t ca_usages;
    pkc_constraints_t constraints;
    
//Check 1:
    res = get_ca_usages(&ca_usages);
    if ((res != 0) && (res != pkc_s_certificate_revoked)) return 0;
    if (!ca_usages.is_permitted(usage)) return 0;

//Check 2:
    res = get_constraints(&constraints);
    if ((res != 0) && (res != pkc_s_certificate_revoked)) return 0;
    if (!constraints.is_permitted(owner, subject)) return 0;

// All checks passed, so the key is valid to certify the principal.
    return 1;
}


char pkc_trusted_key_t::may_certify(
            const x500name & subject, 
            unsigned long usages) const {

    pkc_ca_key_usage_t usage = usages;

    return may_certify(subject, usage);
}


void pkc_trust_list_t::empty(void) {
    while (first != NULL) delete first;
    revocation_list.empty();
}

unsigned32 pkc_trust_list_t::fixup_links(void) {
    pkc_trust_list_element_t * ptr1;
    pkc_trust_list_element_t * ptr2;
    for (ptr1 = first;
         ptr1 != NULL;
	 ptr1 = ptr1->next) {
	if (ptr1->old_key_id != ptr1->key_id) {
	    for (ptr2 = first;
	         ptr2 != NULL;
		 ptr2 = ptr2->next) {
                if (ptr2->ca_key_id == ptr1->old_key_id) {
		    ptr2->old_ca_key_id = ptr2->ca_key_id;
		    ptr2->ca_key_id = ptr1->key_id;
		};
            };
            ptr1->old_key_id != ptr1->key_id;
	};
	
    };
    return 0;
}

pkc_trust_list_t & pkc_trust_list_t::operator=(const pkc_trust_list_t & o) {
    pkc_trust_list_element_t * dst_ptr;
    pkc_trust_list_element_t * src_ptr;
    pkc_trusted_key_t * dst_key_ptr;

    empty();  // First delete any current data associatede with this object.

    revocation_list = o.revocation_list;  // Copy the revocation list
    revalidation_list = o.revalidation_list;  // Copy the revalidation list

    src_ptr = o.first;

    while (src_ptr != NULL) {
// create a new element read to receive the key...
	dst_ptr = new pkc_trust_list_element_t(*this);
// copy the key content...
	dst_ptr->copy(src_ptr);

	src_ptr = src_ptr->next;
    };
     
    fixup_links();

    return *this;
}

pkc_trusted_key_t & pkc_trusted_key_t::operator=(
                                         const pkc_trusted_key_t & o) {
    copy(&o);
    return *this;
}

unsigned32 pkc_trusted_key_t::copy(const pkc_trusted_key_t * o) {
    start_date = o->start_date;
    end_date = o->end_date;
    ca_usages = o->ca_usages;
    user_usages = o->user_usages;
    policies = o->policies;
    constraints = o->constraints;

    trusted = o->trusted;
    revoked = o->revoked;
    certified = o->certified;
    if (certified) {
        certified_by = o->certified_by;
        serial_number = o->serial_number;
    };
    
    certified_start_date = o->certified_start_date;
    certified_end_date = o->certified_end_date;
    certified_usages = o->certified_usages;
    certified_policies = o->certified_policies;
    certified_constraints = o->certified_constraints;

    revalidation_date = o->revalidation_date;
    
    old_key_id = o->key_id;
    ca_key_id = o->ca_key_id;
    key_id = usl_from_pointer(this);

    owner = o->owner;

    value.copy(o->value);

    return 0;
}

unsigned32 pkc_lookup_keys_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         pkc_trusted_key_list_t * & keys,
                         size_t * key_count,
                         const x500name & owner,
                         utc_t * key_time,
                         const pkc_generic_key_usage_t * usages) {
// Searches the specified trustlist for keys owned by the specified
// principal at the specified time for the specified usage (only a single 
// usage should be specified).  
    pkc_generic_key_usage_t theUsages;
    pkc_trust_list_element_t * ptr;
    pkc_trusted_key_list_element_t * theKey;
    unsigned32 status;
    *key_count = 0;

    keys = new pkc_trusted_key_list_t;
    
    for (ptr = trust_list->first; ptr != NULL; ptr = ptr->next) {
// For each key in the trustlist...

// Does this key belong to the specified owner?
	if (ptr->owner != owner) continue;

// Was this key valid at the specified time?
        if (key_time != NULL) {
            if (!ptr->valid_at(key_time)) continue;
        };

// Do the usages of this key permit the specified use?
        if (usages != NULL) {
            status = ptr->get_usages(&theUsages);
            if ((status != 0) && (status != pkc_s_certificate_revoked)) continue;
            if (!theUsages.is_permitted(*usages)) continue;
        };

// This is a key we're interested in.  Add it to the list.
        theKey = new pkc_trusted_key_list_element_t(keys);
        theKey->key = ptr;
        (*key_count)++;

    };
    return 0;
}

unsigned32 pkc_lookup_key_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         const pkc_trusted_key_t ** key,
			 unsigned long key_id) {
// Searches the specified trustlist for the specified key.
    pkc_trust_list_element_t * ptr;
    for (ptr = trust_list->first; ptr != NULL; ptr = ptr->next) {
	if (ptr->key_id == key_id) {
	    *key = ptr;
	    return 0;
	};
    };
    return pkc_s_key_not_found;
}

unsigned32 pkc_lookup_element_in_trustlist(
                         pkc_trust_list_t * trust_list,
                         const pkc_trust_list_element_t ** key,
			 unsigned long key_id) {
// Searches the specified trustlist for the specified key.
    pkc_trust_list_element_t * ptr;
    for (ptr = trust_list->first; ptr != NULL; ptr = ptr->next) {
	if (ptr->key_id == key_id) {
	    *key = ptr;
	    return 0;
	};
    };
    return pkc_s_key_not_found;
}

unsigned32 pkc_check_cert_against_trustlist(
                         pkc_trust_list_t * trust_list,
                         const Certificate * cert,
                         int revoked_certs_permitted) {
// Checks the specified certificate against the specified list of trusted keys. 
// If the certificate is valid and can be verified from the trustlist, its
// content is added to the trustlist.
//
// A single certificate can result in multiple entries being made in the 
// trustlist, if more than one existing key entry can be used to certify the 
// key. Thus the trust model represented within the trust-list is always
// a simple tree-structure.  This makes pruning and revocation easy - whenever
// one key is invalidated or removed, all dependent entries can be treated the
// same way.
//
// Prior to adding the certificate, we check to see whether it has already 
// been revoked.  If so, we might still be able to add it if 
// revoked_certs_permitted is true, and if the revocation date is later
// than the goodSince date.
//
// When looking for a certifying key, the trustlist is scanned in reverse 
// order (last -> first), since this is likely to be most efficient for the 
// common case when certificates are inserted in the correct order.  
// Succesfully verified certificates will be added to the end of the list.
//
// For each key we inspect we apply the following checks:
// 0) If this key has been revoked, are we still allowed to consider it?
// 1) Does this key belong to the certificate issuer?
// 2) Is the key for the same algorithm as the certificate signature?
//    This check has been removed, so that e.g. a generic RSA key will be
//    usable to verify RSAwithMD2 signatures.  The crypto module is now
//    responsible for rejecting incompatibl key types.
// 3) Do the key constraints allow verification of this certificate?
// 4) Does this key validity (including revocations) overlap that of 
//    the certificate?
 
    int key_added = 0;
    pkc_constraints_t constraints;
    buffer_t key_buffer;
    unsigned32 res;    
    utc_t key_start;
    utc_t key_end;
    utc_t cert_start;
    utc_t cert_end;
    int certificate_revoked = 0;
    utc_t revoked_from;
    unsigned32 status;

    pkc_trust_list_element_t * ptr;
    pkc_trust_list_element_t * new_key;

    pkc_pending_revocation_t * crl_ptr;


// First checkwhether the new certificate is pre-revoked.
    status = trust_list->revocation_list.get_revocation_date(
                                       cert->certificateInfo.issuer,
                                       cert->certificateInfo.serialNumber,
                                       &revoked_from);
    if (status != pkc_s_certificate_not_revoked) {
	if (status != 0) return status;
        if (!revoked_certs_permitted) return pkc_s_certificate_revoked;
	certificate_revoked = 1;
    };


    for (ptr = trust_list->last; ptr != NULL; ptr = ptr->prev) {

// Check 0:
        if (ptr->revoked && !revoked_certs_permitted) continue;
// Check 1:
        if (cert->certificateInfo.issuer != ptr->owner) continue;

// Check 2:
//	if (ptr->value.algorithm.algorithm !=
//            cert->signatureAlgorithm.algorithm) continue;

// Check 3:
        res = ptr->get_constraints(&constraints);
	if ((res != 0) && (res != pkc_s_certificate_revoked)) continue;
        if (!constraints.is_permitted(cert->certificateInfo.issuer,
                                      cert->certificateInfo.subject)) continue;
// This key can't be used for this certificate.  


// Check 4:
	res = ptr->get_start_date(&key_start);
	if ((res != 0) && (res != pkc_s_certificate_revoked)) continue;
	res = ptr->get_end_date(&key_end);
	if ((res != 0) && (res != pkc_s_certificate_revoked)) continue;

	if (ptr->revoked) {
	    if (ptr->revocation_date < key_end) {
		key_end = ptr->revocation_date;
	    };
	};
	if (res = cert->certificateInfo.validity.notBefore.get_value(
                                                      cert_start)) continue;
	if (res = cert->certificateInfo.validity.notAfter.get_value(
                                                      cert_end)) continue;
	if (certificate_revoked) {
            if (cert_end > revoked_from) cert_end = revoked_from;
        };

	if (!periods_overlap(key_start, key_end, cert_start, cert_end)) {
	    continue;
	};

// All checks pass.  Now attempt to verify the certificate's signature.
// If signature verifies, add the certificate; if not continue.

	key_buffer.clear();
	if (res = ptr->value.write(key_buffer)) continue;
	if (res = cert->check_signature(key_buffer)) continue;
// The certificate verified.  Create a trusted key record for it, and
// add it to the list.

	new_key = new pkc_trust_list_element_t(*trust_list);
// Copy the key from the certificate into the trust-list-element...
// 
	cert->certificateInfo.validity.notBefore.get_value(new_key->start_date);
	cert->certificateInfo.validity.notAfter.get_value(new_key->end_date);

//	new_key->ca_usages = Not yet supported
//	new_key->user_usages = Not yet supported
//	new_key->policies = Not yet supported
//	new_key->constraints = Not yet supported
	new_key->trusted = 1;
	if ((new_key->revoked = certificate_revoked) != 0) {;
            new_key->revocation_date  = revoked_from;
	};
	new_key->certified = 1;
	new_key->certified_by = cert->certificateInfo.issuer;
	new_key->serial_number = cert->certificateInfo.serialNumber;

	ptr->get_start_date(&(new_key->certified_start_date));
	ptr->get_end_date(&(new_key->certified_end_date)); 
	ptr->get_ca_usages(&(new_key->certified_usages)); 
	ptr->get_key_policies(&(new_key->certified_policies)); 

//	new_key->key_id = Key-id was set by the constructor
	new_key->ca_key_id = ptr->key_id;

        new_key->owner = cert->certificateInfo.subject;
        new_key->value.copy(cert->certificateInfo.subjectPublicKeyInfo);

	key_added = 1;

//	New key entry added; loop to see if we need to do it again.

    };
    if (!key_added) return pkc_s_cert_not_added;

// We added the key from this certificate.  That means we might now be able to
// verify a pending revocation list.  See if we can...



    crl_ptr = trust_list->revocation_list.find_pending(
                                             cert->certificateInfo.subject);
    while (crl_ptr) {

// We found a pending crl for this principal.  Try to verify it...

        key_buffer.clear();
        cert->certificateInfo.subjectPublicKeyInfo.write(key_buffer);
        status = crl_ptr->crl.check_signature(key_buffer);
        if (status == 0) {
// We verified this CRL with the new key.  Process it.
            trust_list->process_crl(&(crl_ptr->crl));
// Now we've processed it, we can remove it from the pending list and
// discard it.
	    delete crl_ptr;
        };
// See if there are any more pending CRLs for this principal
        crl_ptr = trust_list->revocation_list.find_pending(
	                                   cert->certificateInfo.subject);
    };
    return 0;
}

unsigned32 pkc_revoke_certificate(pkc_trust_list_t * trust_list,
                                  const x500name & issued_by,
                                  const asn_integer & serial_no,
                                  utc_t * invalidate_from) {
    unsigned32 status;
    pkc_trust_list_element_t * key;
// Applies the specified revocation to the specified trustlist (i.e. revokes
// a key and all dependent keys).  If invalidate_from is NULL, the key is
// completely revoked; if a valid UTC-time is provided, the key is revoked from
// that time on.
    trust_list->revocation_list.add_revocation(issued_by, 
                                               serial_no, 
                                               invalidate_from);
// Find all key entries certified by the revoked certificate and set their
// certifier-revoked information, then and propagate the revocation
// throughout the chain.

    status = trust_list->find_certified_key(issued_by, serial_no, &key);
    if (status == pkc_s_key_not_found) return 0;
    if (status) return status;

// We have (at least one) entry that is affected by the revocation.

    while (status != pkc_s_key_not_found) {
	key->apply_revocation(invalidate_from);
        status = trust_list->find_next_certified_key(&key);
	if ((status != 0) && (status != pkc_s_key_not_found)) return status;
    };    
    
    return 0;
}

unsigned32 pkc_revoke_certificates(
                  pkc_trust_list_t * trust_list,
                  const SignedCertificateList * crl) {
    unsigned32 status = 0;
    utc_t crl_date;
    pkc_trusted_key_list_t * key_list = NULL;
    pkc_trusted_key_list_element_t * key_ptr = NULL;
    pkc_generic_key_usage_t usages;
    buffer_t key_buffer;
    size_t key_count;

// Attempt to verify the signature on the CRL.    
// If the signature can be verified, call pkc_revoke_certificate for
// each certificate in the CRL; if not, add the CRL to the pending revocations
// list.
    
    status = crl->certificateList.thisUpdate.get_value(crl_date);
    if (status) return status;

    usages = PKC_CAKEY_USAGE_OFFLINE_CRL_SIGN;
    
    status = pkc_lookup_keys_in_trustlist(trust_list,
                         key_list,
                         &key_count,
                         crl->certificateList.issuer,
                         &crl_date,
			 &usages);
    
    if (status != pkc_s_key_not_found) {
        if (status) return status;

        key_ptr = key_list->first;
        status = 1;
        while (key_ptr) {
            key_buffer.clear();
            key_ptr->key->value.write(key_buffer);
            status = crl->check_signature(key_buffer);
            if (status == 0) break;
            key_ptr = key_ptr->next;
        };    
        delete key_list;
    };

    if (status != 0) {
// We didn't manage to find a key that could verify the crl, so store
// it in the pending revocations list.
	new pkc_pending_revocation_t(*crl,
                                     &(trust_list->revocation_list));
	return 0;
    }

    
// We verified the CRL signature, so process its contents now.

    return trust_list->process_crl(crl);
}


unsigned32 pkc_trust_list_t::process_crl(
                  const SignedCertificateList * crl) {

    unsigned32 status;
    const RevokedCerts * certs;
    size_t child_count;
    int i;
    utc_t revocation_date;
    utc_t thisUpdate;
    utc_t nextUpdate;

// Processing a CRL is simple: First we make a note that we've seen the
// CRL for this entry, then we revoke the certificates it contains.
// The caller is assumed to have already verified the signature on the
// CRL.


    status = crl->certificateList.thisUpdate.get_value(thisUpdate);
    if (status) return status;

    status = crl->certificateList.nextUpdate.get_value(nextUpdate);
    if (status) return status;

    status = revalidation_list.add_crl(crl->certificateList.issuer,
                                       thisUpdate,
                                       nextUpdate);
    if (status) return status;

    certs = &(crl->certificateList.revokedCertificates);
    
    if (certs->is_present()) {
        child_count = certs->child_count();
        for (i=0; i<child_count; i++) {
            (*certs)[i]->revocationDate.get_value(revocation_date);
            status = pkc_revoke_certificate(
                                  this,
                                  crl->certificateList.issuer,
                                  (*certs)[i]->certificateSerialNumber,
                                  &revocation_date);
            if (status) break;
        };
    };

    return status;

}

unsigned32 pkc_prune_trustlist(pkc_trust_list_t * trust_list,
                               const utc_t * prune_before) {
// Prunes keys that expired before the stated time from a trustlist.  If
// prune_before is NULL, all keys that have expired are pruned.
    return 0;
}

unsigned32 pkc_delete_trustlist(pkc_trust_list_t * trust_list) {
// Delete a trust-list.
    delete trust_list;
    return 0;
}

unsigned32 pkc_copy_trustlist(
                         const pkc_trust_list_t * input_trust_list,
                         pkc_trust_list_t * output_trust_list) {
// Copy a trust-list.
    *output_trust_list = *input_trust_list;
    return 0;
}


unsigned32 pkc_add_trusted_key(
                         pkc_trust_list_t * trust_list,
                         const pkc_trusted_key_t & key) {

    pkc_trust_list_element_t * new_key =
                                 new pkc_trust_list_element_t(*trust_list);
    
    new_key->start_date = key.start_date;
    new_key->end_date = key.end_date;

    new_key->ca_usages = key.ca_usages;
    new_key->user_usages = key.user_usages;
    new_key->policies = key.policies;
    new_key->constraints = key.constraints;
    new_key->trusted = 1;
    new_key->revoked = 0;
    new_key->certified = 0;
//    new_key->serial_number = key.serial_number;
    new_key->certified_start_date = key.certified_start_date;
    new_key->certified_end_date = key.certified_end_date;
    new_key->certified_usages = key.certified_usages; 
    new_key->certified_policies = key.certified_policies;
    new_key->ca_key_id = 0;

    new_key->owner = key.owner;
    new_key->value.copy(key.value);

    return 0;
}

pkc_constraints_t & pkc_constraints_t::operator= (const pkc_constraints_t & o)
{

    path_length = o.path_length;
    subord_constraints = o.subord_constraints;
    subtree_constraints = o.subtree_constraints;

    return *this;
}

pkc_name_subord_constraints_t & pkc_name_subord_constraints_t ::operator=
                                    (const pkc_name_subord_constraints_t & o) {
// Release any current constraints, and copy the new ones

    pkc_name_subord_constraint_t * srcConstraint;
    pkc_name_subord_constraint_t * newConstraint;

    while (first != NULL) delete first;

    srcConstraint = o.first;
    
    while (srcConstraint != NULL) {
       newConstraint = new pkc_name_subord_constraint_t(this);
       newConstraint->constraint = srcConstraint->constraint;
       newConstraint->skipCerts = srcConstraint->skipCerts;
       srcConstraint = srcConstraint->next;
    };

    return *this;
}

pkc_name_subord_constraints_t::pkc_name_subord_constraints_t(void) {
    first = NULL;
    last = NULL;
}

pkc_name_subord_constraints_t::~pkc_name_subord_constraints_t() {
    while (first != NULL) {
        delete first;
    };
}

pkc_name_subtree_constraints_t & pkc_name_subtree_constraints_t ::operator=
                                   (const pkc_name_subtree_constraints_t & o) {
// Release any current constraints, and copy the new ones

    pkc_name_subtree_constraint_t * srcConstraint;
    pkc_name_subtree_constraint_t * newConstraint;

    while (first != NULL) delete first;

    srcConstraint = o.first;
    
    while (srcConstraint != NULL) {
       newConstraint = new pkc_name_subtree_constraint_t(this);
       newConstraint->base = srcConstraint->base;
       newConstraint->chopBefore = srcConstraint->chopBefore;
       newConstraint->chopAfter = srcConstraint->chopAfter;
       newConstraint->minimum = srcConstraint->minimum;
       newConstraint->maximum = srcConstraint->maximum;

       srcConstraint = srcConstraint->next;
    };

    return *this;
}

pkc_name_subtree_constraints_t::pkc_name_subtree_constraints_t(void) {
    first = NULL;
    last = NULL;
}

pkc_name_subtree_constraints_t::~pkc_name_subtree_constraints_t() {
    while (first != NULL) {
        delete first;
    };
}

unsigned32 pkc_revocation_list_t::get_revocation_date(
                                      const x500name & issuer,
                                      const asn_integer & serialNumber,
                                      utc_t * revocationDate) const {
    pkc_revocation_t * p = first;
    while (p) {
	if (p->certIssuer == issuer &&
	    p->certSerialNumber == serialNumber) {
            *revocationDate = p->certRevocationDate;
	    return 0;
	};
	p = p->next;
    };
    return pkc_s_certificate_not_revoked;
}



unsigned32 pkc_revocation_list_t::get_revocation_date(
        const pkc_revocation_t & o,
        utc_t * revocationDate) const {
    return get_revocation_date(o.certIssuer,
                               o.certSerialNumber,
                               revocationDate);
}

unsigned32 pkc_revocation_list_t::add_revocation(
                                        const x500name & issuer,
                                        const asn_integer & serialNumber,
                                        utc_t * revocationDate) {
    pkc_revocation_t * p = first;

    IF_NULL_NOW(revocationDate);

    while (p) {
	if (p->certIssuer == issuer &&
	    p->certSerialNumber == serialNumber) {
	    break;
	};
	p = p->next;
    };
    if (p) {
// We have to extend a pre-existing revocation.
        if (*revocationDate < p->certRevocationDate) {
// The new date is prior to the existing date, so alter the existing date.
            p->certRevocationDate = *revocationDate;
	};
    } else {
// We didn't find the revocation - create it.
        new pkc_revocation_t(issuer, serialNumber, *revocationDate, this);
    };
    return 0;
}



unsigned32 pkc_revocation_list_t::add_revocation(const pkc_revocation_t & o) {
    return add_revocation(o.certIssuer,
                          o.certSerialNumber,
                          (utc_t *)(&o.certRevocationDate));
}

unsigned32 pkc_revocation_list_t::add_key(pkc_trust_list_t * trust_list,
                                          const SubjectPublicKeyInfo & key,
                                          const x500name & subject,
                                          const utc_t & start_date,
                                          const utc_t & end_date,
                                          const pkc_ca_key_usage_t * usages) {
// Tell the revocation list that a key has been added to the trust-list.
// This may allow one or more pending revocations to become real revocations,
// which in turn may allow some keys to be pruned from the trust-list.  Such
// pruning will happen because this routine will invoke pkc_revoke_certificate
// for each certificate that's found in the newly-verified CRL

// TBD

    return 0;
}

void pkc_revocation_list_t::empty() {
    while (first) delete first;    // Delete the confirmed revocations.
    while (first_pending) delete first_pending;	// Delete the provisional
						// revocations.
}

pkc_revocation_list_t::~pkc_revocation_list_t() {
    empty();
}

pkc_revocation_list_t::pkc_revocation_list_t(void) {
    first = NULL;
    last = NULL;
    first_pending = NULL;
    last_pending = NULL;
}

pkc_pending_revocation_t * pkc_revocation_list_t::find_pending(
                                             const x500name & issuer) const {
    pkc_pending_revocation_t * ptr = first_pending;
    while (ptr) {
        if (ptr->crl.certificateList.issuer == issuer) return ptr;
	ptr = ptr->next;
    };
    return NULL;
}



void pkc_crl_seen_date_list_t::empty() {
    while (first) delete first;    // Delete the confirmed revocations.
}

pkc_crl_seen_date_list_t::~pkc_crl_seen_date_list_t() {
    empty();
}

pkc_crl_seen_date_list_t::pkc_crl_seen_date_list_t(void) {
    first = NULL;
    last = NULL;
}

pkc_revocation_t::pkc_revocation_t(const x500name & issuer,
                     const asn_integer & serialNumber,
                     utc_t revocationDate,
                     pkc_revocation_list_t * the_root ) {
    root = the_root;
    next = NULL;
    prev = root->last;
    if (prev) {
        prev->next = this;
    } else {
        root->first = this;
    };
    root->last = this;
    certIssuer = issuer;
    certSerialNumber = serialNumber;
    certRevocationDate = revocationDate;
}

pkc_revocation_t::~pkc_revocation_t(void) {
    unlink();
}

void pkc_revocation_t::unlink(void) {
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (root == NULL) return;
    if (this == root->first) root->first = next;
    if (this == root->last) root->last = prev;
}   

pkc_revocation_t & pkc_revocation_t::operator = (const pkc_revocation_t & o) {
    certIssuer = o.certIssuer;
    certSerialNumber = o.certSerialNumber;
    certRevocationDate = o.certRevocationDate;
    return *this;
}

pkc_crl_seen_date_t::pkc_crl_seen_date_t(const x500name & issuer,
                                         const utc_t & seenDate,
                                         const utc_t & nextDate,
                                         pkc_crl_seen_date_list_t * the_root ) {
    root = the_root;
    next = NULL;
    prev = root->last;
    if (prev) {
        prev->next = this;
    } else {
        root->first = this;
    };
    root->last = this;

    crlIssuer = issuer;
    crlDate = seenDate;
    crlNextDate = nextDate;
}

pkc_crl_seen_date_t::~pkc_crl_seen_date_t(void) {
    unlink();
}

void pkc_crl_seen_date_t::unlink(void) {
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (root == NULL) return;
    if (this == root->first) root->first = next;
    if (this == root->last) root->last = prev;
}   

pkc_crl_seen_date_t & pkc_crl_seen_date_t::operator = (const pkc_crl_seen_date_t & o) {
    crlIssuer = o.crlIssuer;
    crlDate = o.crlDate;
    crlNextDate = o.crlNextDate;
    return *this;
}

pkc_crl_seen_date_t * pkc_crl_seen_date_list_t::find(const x500name & issuer)
const {
    pkc_crl_seen_date_t * ptr = first;
    while (ptr) {
        if (ptr->crlIssuer == issuer) return ptr;
        ptr = ptr->next;
    };
    return NULL;
}

unsigned32 pkc_crl_seen_date_list_t::get_last_crl_seen_date(
                                      const x500name & issuer,
                                      utc_t * crlSeenDate) const {
    pkc_crl_seen_date_t * ptr;
    ptr = find(issuer);
    if (ptr == NULL) return pkc_s_key_not_found;
    if (crlSeenDate) *crlSeenDate = ptr->crlDate;
    return 0;
}

unsigned32 pkc_crl_seen_date_list_t::get_next_crl_expected_date(
                                      const x500name & issuer,
                                      utc_t * crlExpectedDate) const {
    pkc_crl_seen_date_t * ptr;
    ptr = find(issuer);
    if (ptr == NULL) return pkc_s_key_not_found;
    if (crlExpectedDate) *crlExpectedDate = ptr->crlNextDate;
    return 0;
}

unsigned32 pkc_crl_seen_date_list_t::add_crl(
                                      const x500name & issuer,
                                      const utc_t & crlSeenDate,
                                      const utc_t & crlExpectedDate) {
    pkc_crl_seen_date_t * ptr;
    ptr = find(issuer);
    if (ptr) {
	if (crlSeenDate > ptr->crlDate) {
            ptr->crlDate = crlSeenDate;
            ptr->crlNextDate = crlExpectedDate;
	}
    } else {
	ptr = new pkc_crl_seen_date_t(issuer,
                                      crlSeenDate,
                                      crlExpectedDate,
                                      this);
    };
    return 0;
}


pkc_crl_seen_date_list_t & pkc_crl_seen_date_list_t::operator = (
                                          const pkc_crl_seen_date_list_t & o) {
    pkc_crl_seen_date_t * p = o.first;
    while (p) {
	new pkc_crl_seen_date_t(p->crlIssuer,
                                p->crlDate,
                                p->crlNextDate,
                                this);
        p = p->next;
    };
    return *this;
}

pkc_revocation_list_t & pkc_revocation_list_t::operator = (
                                          const pkc_revocation_list_t & o) {
    pkc_revocation_t * p = o.first;
    pkc_pending_revocation_t * pp = o.first_pending;
    while (p) {
	new pkc_revocation_t(p->certIssuer,
                             p->certSerialNumber,
                             p->certRevocationDate,
                             this);
        p = p->next;
    };
    while (pp) {
	new pkc_pending_revocation_t(pp->crl,
                                     this);
        pp = pp->next;
    };
    return *this;
}

void pkc_trust_list_element_t::unlink(void) {
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (root == NULL) return;
    if (this == root->first) root->first = next;
    if (this == root->last) root->last = prev;
}

pkc_trust_list_element_t::pkc_trust_list_element_t(
                                             pkc_trust_list_t & the_root) {
    root = &the_root;
    next = NULL;
    prev = root->last;
    if (prev) {
        prev->next = this;
    } else {
        root->first = this;
    }
    root->last = this;
}

pkc_trust_list_element_t::pkc_trust_list_element_t(pkc_trust_list_t & the_root,
                             utc_t startDate,
                             utc_t endDate,
                             pkc_ca_key_usage_t caUsages,
                             pkc_key_usage_t userUsages,
                             pkc_key_policies_t keyPolicies,
                             pkc_constraints_t keyConstraints)
    : pkc_trusted_key_t(startDate,
                        endDate,
                        caUsages,
                        userUsages,
                        keyPolicies,
                        keyConstraints) {
    root = &the_root;
    next = NULL;
    prev = root->last;
    if (prev) {
        prev->next = this;
    } else {
        root->first = this;
    };
    root->last = this;
}
 
pkc_trust_list_element_t::~pkc_trust_list_element_t() {
    unlink();
};

pkc_trust_list_t::pkc_trust_list_t(void) {
    first = NULL;
    last = NULL;
}

pkc_trust_list_t::~pkc_trust_list_t() {
    empty();
}

unsigned32 pkc_trust_list_t::find_certified_key(
                                  const x500name & certifier,
                                  const asn_integer & certifying_serial_no,
                                  pkc_trust_list_element_t ** key) {
    *key = first;
    while (*key) {
        if (((*key)->certified_by == certifier) &&
            ((*key)->serial_number == certifying_serial_no)) {
            return 0;
        };
	*key = (*key)->next;
    };
    return pkc_s_key_not_found;
}

unsigned32 pkc_trust_list_t::find_next_certified_key(
                                   pkc_trust_list_element_t ** key) {
    pkc_trust_list_element_t * orig_key = *key;

    if (orig_key == NULL) return pkc_s_key_not_found;

    (*key) = (*key)->next;
    while (*key) {
        if (((*key)->certified_by == orig_key->certified_by) &&
            ((*key)->serial_number == orig_key->serial_number)) {
            return 0;
        };
	*key = (*key)->next;
    };
    return pkc_s_key_not_found;
}

pkc_key_policy_t & pkc_key_policy_t::operator = (const pkc_key_policy_t & o) {
    OM_uint32 min_stat;
    if (value != NULL) gssdce_release_oid(&min_stat, &value);
    value = NULL;
    if (o.value == NULL) return *this;  
    value = (gss_OID)malloc(sizeof(gss_OID_desc));
    value->length = o.value->length;
    value->elements = (void *)malloc(value->length);   
    memcpy(value->elements, o.value->elements, value->length); 
    return *this;
}

pkc_key_policy_t & pkc_key_policy_t::operator = (const gss_OID & o) {
    OM_uint32 min_stat;
    if (value != NULL) gssdce_release_oid(&min_stat, &value);
    value = NULL;
    if (o == NULL) return *this;    
    value = (gss_OID)malloc(sizeof(gss_OID_desc));
    value->length = o->length;
    value->elements = (void *)malloc(value->length); 
    memcpy(value->elements, o->elements, value->length); 
    return *this;
}


pkc_key_policies_t::pkc_key_policies_t(void) {
    policies = NULL;
    all_policies = 1;
}

unsigned32 pkc_key_policies_t::set(const pkc_key_policy_t & pol) {
    OM_uint32 min_stat = 0;
    all_policies = 0;
    if (policies == NULL) {
        gssdce_create_empty_oid_set(&min_stat, &policies);
    };
    gssdce_add_oid_set_member(&min_stat,
                              pol.value,
                              &policies);
    return min_stat;  

}

unsigned32 pkc_key_policies_t::set(const gss_OID pol) {
    OM_uint32 min_stat = 0;
    all_policies = 0;
    if (policies == NULL) {
        gssdce_create_empty_oid_set(&min_stat, &policies);
    };
    gssdce_add_oid_set_member(&min_stat,
                              pol,
                              &policies);
    return min_stat;  

}


unsigned32 pkc_key_policies_t::set_none(void) {
    OM_uint32 min_stat = 0;
    all_policies = 0;
    if (policies != NULL) {
        gss_release_oid_set(&min_stat, &policies);
    };
    policies = NULL;
    return min_stat;
}

unsigned32 pkc_key_policies_t::set_all(void) {
    OM_uint32 min_stat = 0;
    all_policies = 1;
    if (policies != NULL) {
        gss_release_oid_set(&min_stat, &policies);
    };
    policies = NULL;
    return min_stat;
}

pkc_key_policies_t & pkc_key_policies_t::operator = (
                                          const pkc_key_policies_t & pol) {
    int i;
    set_none();
    for (i=0;i<pol.policies->count;i++) set(&pol.policies->elements[i]);
    return *this;
}

unsigned32 pkc_key_policies_t::constrain(const pkc_key_policies_t & pol) {
    int i;
    int present;
    OM_uint32 min_stat = 0;
    gss_OID_set temp_policies;

    if (all_policies) {
        *this = pol;
        return 0;
    };
    if (policies == NULL) {
        return 0;
    };
    temp_policies = policies;
    policies = NULL;
    for (i=0;i<pol.policies->count;i++) {
        gssdce_test_oid_set_member(&min_stat,
                                   &pol.policies->elements[i], 
                                   temp_policies,
                                   &present);
        if (present) set(&pol.policies->elements[i]);
    };
    gss_release_oid_set(&min_stat, &temp_policies);
    return min_stat;
}

pkc_key_policy_t::pkc_key_policy_t(void) {
    value = NULL;
}

pkc_key_policy_t::~pkc_key_policy_t() {
    OM_uint32 min_stat;
    if (value != NULL) gssdce_release_oid(&min_stat, &value);
}

pkc_name_subord_constraint_t::~pkc_name_subord_constraint_t() {
    unlink();
}

pkc_name_subord_constraint_t::pkc_name_subord_constraint_t(
                                    pkc_name_subord_constraints_t * theRoot) {
    root = theRoot;
    next = NULL;
    prev = root->last;
    if (prev) {
        prev->next = this;
    } else {
        root->first = this;
    };
    root->last = this;

}

pkc_name_subord_constraint_t::pkc_name_subord_constraint_t(void) {
    skipCerts = 0;
    constraint = PKC_NAME_CONSTRAINT_NONE;
}

void pkc_name_subord_constraint_t::set_constraint(unsigned long c) {
    constraint = c;
}

void pkc_name_subord_constraint_t::set_skipCerts(unsigned c) {
    skipCerts = c;
}

unsigned32 pkc_name_subtree_constraints_t::constrain(
                           const pkc_name_subtree_constraint_t & o){
    pkc_name_subtree_constraint_t * c = new pkc_name_subtree_constraint_t(this);
    if (o.base) c->set_base(*o.base);
    if (o.chopBefore) c->set_chopBefore(*o.chopBefore);
    if (o.chopAfter) c->set_chopAfter(*o.chopAfter);
    c->set_minimum(o.minimum);
    c->set_maximum(o.maximum);
    return 0;    
}			   

unsigned32 pkc_name_subtree_constraints_t::constrain(
                           const pkc_name_subtree_constraints_t & o){
    unsigned32 status;
    const pkc_name_subtree_constraint_t * c = o.first;
    while (c) {
        if (status = constrain(*c)) return status;
	c = c->next;
    };
    return 0;
}			   

unsigned32 pkc_name_subord_constraints_t::constrain(
                           const pkc_name_subord_constraint_t & o){
    pkc_name_subord_constraint_t * c = new pkc_name_subord_constraint_t(this);
    c->constraint = o.constraint;
    c->skipCerts = o.skipCerts;
    return 0;    
}			   

unsigned32 pkc_name_subord_constraints_t::constrain(
                           const pkc_name_subord_constraints_t & o){
    unsigned32 status;
    const pkc_name_subord_constraint_t * c = o.first;
    while (c) {
        if (status = constrain(*c)) return status;
	c = c->next;
    };
    return 0;
}			   

pkc_constraints_t::pkc_constraints_t(void) {
    path_length = 0xffffu;
}

unsigned32 pkc_constraints_t::constrain(const pkc_constraints_t & o) {
    unsigned32 status;
    if (o.path_length < path_length) path_length = o.path_length;
    if (status = subord_constraints.constrain(o.subord_constraints))
        return status;
    return subtree_constraints.constrain(o.subtree_constraints);
}

int operator == (const utc_t & o1, const utc_t & o2) {
    enum utc_cmptype rel;
    if (utc_cmpmidtime(&rel, (utc_t *)&o1, (utc_t *)&o2) != 0) return 0;
    return (rel == utc_equalTo);
}

int operator != (const utc_t & o1, const utc_t & o2) {
    enum utc_cmptype rel;
    if (utc_cmpmidtime(&rel, (utc_t *)&o1, (utc_t *)&o2) != 0) return 0;
    return (rel != utc_equalTo);
}

int operator <= (const utc_t & o1, const utc_t & o2) {
    enum utc_cmptype rel;
    if (utc_cmpmidtime(&rel, (utc_t *)&o1, (utc_t *)&o2) != 0) return 0;
    return ((rel == utc_lessThan) || (rel == utc_equalTo));
}

int operator >= (const utc_t & o1, const utc_t & o2) {
    enum utc_cmptype rel;
    if (utc_cmpmidtime(&rel, (utc_t *)&o1, (utc_t *)&o2) != 0) return 0;
    return ((rel == utc_greaterThan) || (rel == utc_equalTo));
}

int operator < (const utc_t & o1, const utc_t & o2) {
    enum utc_cmptype rel;
    if (utc_cmpmidtime(&rel, (utc_t *)&o1, (utc_t *)&o2) != 0) return 0;
    return (rel == utc_lessThan);
}

int operator > (const utc_t & o1, const utc_t & o2) {
    enum utc_cmptype rel;
    if (utc_cmpmidtime(&rel, (utc_t *)&o1, (utc_t *)&o2) != 0) return 0;
    return (rel == utc_greaterThan);
}

unsigned32 pkc_trust_list_element_t::apply_revocation(utc_t * revoke_from) {
    unsigned32 status;
    pkc_trust_list_element_t * key;
    if (revoke_from == NULL) revoke_from = & start_date;
    if (revoked) {
// We only need to modify the key if the new revocation date supercedes the
// current one - ie if it is earlier than the current date.
        if (*revoke_from >= revocation_date) return 0;
    };
// Either the key was not previously revoked, or the new date should
// supercede the current date.
    revoked = 1;
    revocation_date = *revoke_from;

// Scan the list, looking for entries certified by this key entry.  If we find
// such entries, revoke them recursively.

    status = root->find_certified_key_by_id(key_id, &key);
    if (status == pkc_s_key_not_found) return 0;
    if (status) return status;

// We have (at least one) follow-on entry that is affected by the revocation.

    while (status != pkc_s_key_not_found) {
	key->apply_revocation(revoke_from);
        status = root->find_next_certified_key_by_id(&key);
        if (status) return status;
    };    
    
    return 0;
    
}

unsigned32 pkc_trust_list_element_t::get_certifier_key(
                                   const pkc_trusted_key_t ** ca) const {
    unsigned32 status;
    unsigned long ca_id;
    if (!trusted) return pkc_s_key_not_trusted;
    if (!certified) return pkc_s_key_not_certified;

    ca_id = ca_key_id;

    status = pkc_lookup_key_in_trustlist(root,
                                         ca,
                                         ca_id);
    
    if (status) return status;					 
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}

unsigned32 pkc_trust_list_element_t::get_certifier(
                                   const pkc_trust_list_element_t ** ca) const {
    unsigned32 status;
    unsigned long ca_id;
    if (!trusted) return pkc_s_key_not_trusted;
    if (!certified) return pkc_s_key_not_certified;

    ca_id = ca_key_id;

    status = pkc_lookup_element_in_trustlist(root,
                                             ca,
                                             ca_id);
    
    if (status) return status;					 
    if (revoked) return pkc_s_certificate_revoked;
    return 0;
}


unsigned32 pkc_trust_list_t::find_certified_key_by_id(
                                  unsigned long key_id,
                                  pkc_trust_list_element_t ** key) {
    *key = first;
    while (*key) {
        if ((*key)->ca_key_id == key_id) {
            return 0;
        };
	*key = (*key)->next;
    };
    return pkc_s_key_not_found;
}

unsigned32 pkc_trust_list_t::find_next_certified_key_by_id(
                                   pkc_trust_list_element_t ** key) {
    pkc_trust_list_element_t * orig_key = *key;

    if (orig_key == NULL) return pkc_s_key_not_found;

    (*key) = (*key)->next;
    while (*key) {
        if ((*key)->ca_key_id == orig_key->ca_key_id) {
            return 0;
        };
	*key = (*key)->next;
    };
    return pkc_s_key_not_found;
}

pkc_generic_key_usage_t::pkc_generic_key_usage_t(unsigned long permit_bits) {
    set(permit_bits);
}

char pkc_generic_key_usage_t::is_permitted(unsigned long check) const {
    return ((check & permitted) == check);
}

char pkc_generic_key_usage_t::is_permitted(
                           const pkc_generic_key_usage_t & check)  const {
    return ((check.permitted & permitted) == check.permitted);
}

void pkc_generic_key_usage_t::constrain(unsigned long constraint) {
    permitted = permitted & constraint;
}

void pkc_generic_key_usage_t::constrain(
                            const pkc_generic_key_usage_t & constraint) {
    permitted = permitted & constraint.permitted;
}

void pkc_generic_key_usage_t::set(unsigned long constraints) {
    permitted = pruned(constraints);
}

pkc_generic_key_usage_t & pkc_generic_key_usage_t::operator = 
                                                     (unsigned long c) {
    set(c);
    return *this;
}

unsigned long pkc_generic_key_usage_t::pruned(unsigned long x) const {
    return x;
}


unsigned long pkc_key_usage_t::pruned(unsigned long x) const {
    return (x & PKC_KEY_USAGE_MASK);
}

pkc_key_usage_t & pkc_key_usage_t::operator = (unsigned long c) {
    *((pkc_generic_key_usage_t *)this) = c;
    return *this;
}

pkc_key_usage_t::pkc_key_usage_t (unsigned long c) {
    set(c);
}

unsigned long pkc_ca_key_usage_t::pruned(unsigned long x) const {
    return (x & PKC_CAKEY_USAGE_MASK);
}

pkc_ca_key_usage_t & pkc_ca_key_usage_t::operator = (unsigned long c) {
    *((pkc_generic_key_usage_t *)this) = c;
    return *this;
}

pkc_ca_key_usage_t::pkc_ca_key_usage_t(unsigned long c) {
    set(c);
}

pkc_pending_revocation_t::pkc_pending_revocation_t(
                              const SignedCertificateList & theCrl,
			      pkc_revocation_list_t * the_root) {
    root = the_root;
    next = NULL;
    prev = root->last_pending;
    if (prev) {
        prev->next = this;
    } else {
        root->first_pending = this;
    };
    root->last_pending = this;
    crl.copy(theCrl);
}

pkc_pending_revocation_t & pkc_pending_revocation_t::operator = (
                                       const pkc_pending_revocation_t & o) {
    crl.copy(o.crl);
    return *this;

}

void pkc_pending_revocation_t::unlink(void) {
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (root == NULL) return;
    if (this == root->first_pending) root->first_pending = next;
    if (this == root->last_pending) root->last_pending = prev;
}

pkc_pending_revocation_t::~pkc_pending_revocation_t() {
    unlink();
}


void pkc_trusted_key_list_element_t::unlink(void) {
    if (prev) prev->next = next;
    if (next) next->prev = prev;
    if (root == NULL) return;
    if (this == root->first) root->first = next;
    if (this == root->last) root->last = prev;
}

pkc_trusted_key_list_element_t::~pkc_trusted_key_list_element_t() {
    unlink();
}

pkc_trusted_key_list_element_t::pkc_trusted_key_list_element_t(
					pkc_trusted_key_list_t * theRoot) {
    root = theRoot;
    next = NULL;
    prev = root->last;
    if (prev) {
        prev->next = this;
    } else {
        root->first = this;
    };
    root->last = this;

}

pkc_trusted_key_list_t::pkc_trusted_key_list_t(void) {
    first = NULL;
    last = NULL;
}

void pkc_trusted_key_list_t::empty(void) {
    while (first != NULL) first->unlink();
}

pkc_trusted_key_list_t::~pkc_trusted_key_list_t() {
    empty();
}


// Display functions

static unsigned32 utc_display(const utc_t & t, buffer_t & buffer) {
    char string[UTC_MAX_STR_LEN+1];

    if (utc_ascgmtime(string,
                      sizeof(string),
                      (utc_t *)&t) != 0) {
        strcpy(string,"<Invalid-UTC>");
    };    
    return buffer.append(string);
}

unsigned32 pkc_trusted_key_t::display(buffer_t & buffer) const {
    buffer.append("KEY { Owner = ");
    owner.display(buffer);
    buffer.append("\nFlags: ");
    if (trusted) buffer.append("Trusted ");
    if (certified) {
        buffer.append("Certified (by ");
	certified_by.display(buffer, 1);
	buffer.append(" (");
        buffer.append_hex(ca_key_id);
        buffer.append(")) ");
        if (revoked) buffer.append("Revoked ");
        buffer.append("\nSerial#: "); serial_number.display(buffer);
    };
    buffer.append("\nStart: ");
    utc_display(start_date, buffer);
    buffer.append("\nEnd: ");
    utc_display(end_date, buffer);
    if (certified) {
        buffer.append("\nCertified start: ");
	utc_display(start_date, buffer);
	buffer.append("\nCertified End: ");
	utc_display(end_date, buffer);
    };
    if (revoked) {
	buffer.append("\nRevocation date: ");
	utc_display(revocation_date, buffer);
	buffer.append("\nRevalidation date: ");
	utc_display(revalidation_date, buffer);
    };
    buffer.append("\nID: "); buffer.append_hex(key_id);
    buffer.append("\n}\n");   
    return 0;
}

unsigned32 pkc_revocation_list_t::display(buffer_t & buffer) const {
    pkc_pending_revocation_t * pending;
    pkc_revocation_t * rev;
    buffer.append("RL {\n");
    if ((rev = first) == NULL) {
        buffer.append("No active revocations\n");
    } else {
        buffer.append("REVOCATIONS {\n");
        while (rev) {
            rev->display(buffer);
            rev = rev->next;
            buffer.append("\n");
	};
        buffer.append("}\n");
    };
    if ((pending = first_pending) == NULL) {
        buffer.append("No pending revocations\n");
    } else {
        buffer.append("PENDING {\n");
        while (pending) {
            pending->display(buffer);
            pending = pending->next;
            buffer.append("\n");
	};
        buffer.append("}\n");
    };
    buffer.append("\n}\n");   
    return 0;
}

unsigned32 pkc_pending_revocation_t::display(buffer_t & buffer) const {
    buffer.append("PENDING CRL\n");
    return 0;
}

unsigned32 pkc_revocation_t::display(buffer_t & buffer) const {
    buffer.append("REVOCATION {");
    buffer.append("\nIssuer: "); certIssuer.display(buffer);
    buffer.append("\nSerial#: "); certSerialNumber.display(buffer);
    buffer.append("\nRevoked: "); utc_display(certRevocationDate, buffer);
    buffer.append("\n}\n");   
    return 0;
}


unsigned32 pkc_crl_seen_date_list_t::display(buffer_t & buffer) const {
    pkc_crl_seen_date_t * crl;
    buffer.append("CRL-DATE-LIST {\n");
    if ((crl = first) == NULL) {
        buffer.append("No CRLs\n");
    } else {
        while (crl) {
            crl->display(buffer);
            crl = crl->next;
            buffer.append("\n");
	};
    };
    buffer.append("\n}\n");   
    return 0;
}

unsigned32 pkc_crl_seen_date_t::display(buffer_t & buffer) const {
    buffer.append("CRL-DATE {");
    buffer.append("Issuer: "); crlIssuer.display(buffer);
    buffer.append("\nDate: "); utc_display(crlDate, buffer);
    buffer.append("\nNext CRL: "); utc_display(crlNextDate, buffer);
    buffer.append("\n}\n");   
    return 0;
}

unsigned32 pkc_trusted_key_list_element_t::display(buffer_t & buffer) const {
    return key->display(buffer);
}

unsigned32 pkc_trusted_key_list_t::display(buffer_t & buffer) const {
    pkc_trusted_key_list_element_t * ptr = first;
    buffer.append("KEY-LIST {");
    while (ptr != NULL) {
        ptr->display(buffer);
        ptr = ptr->next;
    };
    buffer.append("\n}\n");   
    return 0;
}

unsigned32 pkc_trust_list_t::display(buffer_t & buffer) const {
    pkc_trust_list_element_t * ptr = first;
    buffer.append("TRUST-LIST {\n");
    buffer.append("Keys: {\n");
    while (ptr != NULL) {
	ptr->display(buffer);
	buffer.append("\n");
	ptr = ptr->next;
    };
    buffer.append("\n},");   

    buffer.append("\nRevocations: {\n");
    revocation_list.display(buffer);
    buffer.append("\n},");   

    buffer.append("\nCrls seen: {\n");
    revalidation_list.display(buffer);
    buffer.append("\n}");   

    buffer.append("\n}\n");   
    return 0;
}

unsigned32 pkc_display_trustlist(const pkc_trust_list_t * input_trust_list) {
    buffer_t buffer;

    if (input_trust_list) {
        input_trust_list->display(buffer);
        fprintf(stdout, "Trustlist:\n");
        fprintf(stdout, "%.*s\n\n", buffer.data_length, buffer.data);
    } else {
	fprintf(stdout, "Trustlist: <EMPTY-TRUSTLIST>\n\n");
    };
    fflush(stdout);
    return 0;
}

unsigned32 pkc_display_time(const utc_t & t) {
    char string[UTC_MAX_STR_LEN+1];

    if (utc_ascgmtime(string,
                      sizeof(string),
                      (utc_t *)&t) != 0) {
        strcpy(string, "<Invalid UTC>");
    };    
    fprintf(stdout, string);
    fprintf(stdout, "\n\n");
    fflush(stdout);
    return 0;
}

