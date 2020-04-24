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
 * $Log: pkc_api.h,v $
 * Revision 1.1.2.1  1996/11/13  20:36:44  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:54:17  arvind]
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
 * FACILITY: Certification API 
 *
 * ABSTRACT:  User-API and Data-Structures for CAPI.
 *
 * AUTHOR:
 *            Mahesh B. Girkar & John Wray
 *            Distributed Processing Engineering (DPE)
 *
 *  
 * 
 */

#ifndef PKC_PLCY_API_
#define PKC_PLCY_API_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/utc.h>
#include <dce/gssapi.h>
#include <dce/pkc_consts.h>
#include <dce/sec_pk.h>


/*
 * High-level DCE certificate API.
 *
 *
 * This file exports the high-level DCE Certification API, a general-purpose
 * public-key retrieval facility.  The Certification API supports
 * replaceable policy and crypto implementations.  Default policies provide
 * a certificate-based hierarchical policy, similar in action to the DCE 
 * hierarchical cells mechanism, where certificates may be stored in either 
 * CDS or the X.500 GDS, and a non-certificate registry retrieval policy,
 *
 */

typedef struct {
void *handle;        /* will point to a pkc_trust_list_t item */
} trustlist_t;


typedef struct {
  int version;          /* Structure version.  Currently, must be version 0 */
  unsigned char *cert;  /* ASN.1 encoding of a Certificate */
  size_t size;          /* size of the encoding */
} cert_t;

typedef struct {
  int version;        /* Structure version.  Currently, must be version 0 */
  unsigned char *ca;  /* x500 char * version of Certification Authority
                       * whose key this is
                       * Name; for eg. /..../foo_cell/ca or
                       * /C=US/O=dec/CN=foo_cell/ca
                       */
  unsigned char *key;  /* ca's ASN.1 key */
  size_t size;         /* size of ASN.1 key */
  utc_t startDate;
  utc_t endDate;
} trusted_key_t;


#define IS_KEY		0   
#define IS_CERT		1   
#define CURRENT_KEY_VERSION	0
#define CURRENT_CERT_VERSION	0

/* structure to hold either a key, or a certificate */
typedef struct {
  int type;    /* Possible values of type are:
                  IS_KEY, or IS_CERT, anything else currently
                  is an error */
  union {
    trusted_key_t k;
    cert_t        c;
  } u;
} trustitem_t; 
 


/* this structure is defined for future enhancements so that
 * users can specify usages or other constraints on the key that 
 * is being retrieved.  In the current version, it is ignored. 
 */
typedef struct {
  void *reserved;
} selection_t;


/* 
 * structure to hold a trust base (the result of an establish_trustbase
 * call.  Callers do not need to create objects of this type; trustbase_t
 * objects are private to the certification API.
 */

typedef struct {
  gss_OID policy_oid;  /* policy OID used for computing trustbase. 
                        * Computing a trust base is a time-consuming
                        * operation, and hence, we cache this in
                        * this structure -- specifically in 
                        * trusbase_handle, where policy specific
                        * trustbase pointers are kept.  
                        */

  utc_t time;          /* time at which you want public keys to be valid
                        */

  selection_t *sel;    /* has to be 0; ignored for now; when used, will
                        * point to a copy of the sel ptr passed in by
                        * the user in pkc_init_trustbase
                        */

  void *trustbase_handle; /* this will be the last
                           * pointer returned by ->establish
                           * routine (see pkc_policy_t 
                           * structure). It points to a pkc_trust_list_t
                           * list.
                           */

} trustbase_t;


/*
 * keyinfo_t is returned to user when pkc_retrieve_keyinfo
 * is called, and contains information about a set of keys belonging
 * to a single principal.
 *
 * The user "interrogates" this handle for number of keys,
 * key value(s), certification path (incase of hierarchical policy) etc
 *
 * The structure contains a pointer to the trustbase_t that was used to
 * establish the trust from which these keys were derived, and a 
 * policy-specific handle that is used for the actual key information.
 * 
 * The user must ensure that the trustbase is not deleted until any
 * dependent keyinfo_t objects have been deleted.
 *
 * Callers do not need to create objects of this type; keyinfo_t
 * objects are private to the certification API.
 */

typedef struct {
  trustbase_t * base;
  char *name;
  utc_t key_date;
  selection_t *sel; 
  void *handle;  /* handle points to a policy specific structure */
} keyinfo_t;


typedef enum {
   UNTRUSTED,     // No trust (e.g. unauthenticated)
   DIRECT_TRUST,  // Direct trust via third party (e.g. authenticated registry)
   CERTIFIED_TRUST    // Trust certified by caller's trustbase.
} trust_type_t;

/*
 * A structure that describes the trust that can be placed in a returned key.
 *
 */
typedef struct {
  trust_type_t trust_type;  // The style of trust 
  char missing_crls;	    // True if one or more CRLs were missing
  char revoked;             // True if any certificate has been revoked (even
                            // if it were still valid at the retrieval time).
} certification_flags_t;

/* 
 * pkc_get_registered_policies
 * returns a set of OID's, all of which point to pre-loaded policies.
 * An appln will call this once during the life-time of the appln.
 * An appln can then choose to use these oids in 
 * pkc_init_trustbase, etc.
 */

extern unsigned32 pkc_get_registered_policies (gss_OID_set * oid_set);







/*
 * Function pkc_init_trustlist
 *
 * Creates an empty trustlist
 * If tr_list is empty, return pkc_s_asn_bad_param;
 * If we cannot malloc, returns pkc_s_nomem;
 * Otherwise, returns pkc_s_success.
 * On succesful return, tr_list will contain a PKC-allocated
 * data structure which must be released with pkc_free_trustlist()
 */

extern unsigned32 pkc_init_trustlist (trustlist_t **tr_list);







/*
 * Function pkc_append_to_trustlist:
 * Appends to pkc_trust_list_t (described by (*tr_list)->handle)
 */
 

extern unsigned32 pkc_append_to_trustlist (trustlist_t ** tr_list, 
                                           trustitem_t *tr_item, 
                                           size_t no_of_tr_items);






/*
 * pkc_init_trustbase: This is to initialize the initial trust
 * list to include all the certificates we initially trust, given
 * the initial set of trusted certificates.
 * This routine will also store the cross-cert pair certificates
 * found during the creationi of the trustbase.
 *
 * Users will normally call the pkc_ API in the following order:
 *
 *  1. pkc_get_registered_policies  (One Time for duration of Appln)
 *
 *  2. pkc_create_empty_trustlist, 
 *  3. One or more pkc_append_to_trustlist
 *
 *  2 & 3 together build up an initial trust list.
 *
 *  4. pkc_init_trustbase.
 *
 *  4 computes a trustbase given an initial trust list.
 *
 *  5. One or more pkc_retrieve_keyinfo  for each individual's public
 *     key that needs to be looked up.
 *  6. pkc_free_trustlist
 *  7. pkc_free_trustbase
 *
 *
 * tr_list: trust list presented to API
 * policy_oid: distinguishes which policy to use
 * date: time at which the public key is to be valid. Can be 0.
 * sel: has to be set to 0. (this is present for a future version &
 *      ignored currently)
 * base: extended trust list + other things..
 *
 * On successful return, base will contain a PKC-allocated structure,
 * which should be released with pkc_free_trustbase()
 */


extern unsigned32 pkc_init_trustbase (trustlist_t **tr_list, 
                                      gss_OID policy_oid, 
                                      utc_t *time,
                                      selection_t *sel,
                                      trustbase_t **base);






/*
 * pkc_retrieve_keyinfo 
 *
 * Returns a keyinfo_t structure describing the set of trusted
 * keys that are valid for the specified principal at the specified
 * date, under any additional constraints specified in key_domain,
 * key_usages or sel.
 *
 * key_domain specifies the domain of the public key(s) to be retrieved.
 * If specified as either NULL or sec_pk_domain_unspecified, keys for
 * all available domains will be returned.
 *
 * key_usages specifies the intended use(s) to which the key will be put.
 * Key usage values are expressed as a bit-mask made up from the various
 * PKC_KEY_USAGE_xxx and PKC_CAKEY_USAGE_xxx constants defined in file
 * pkc_consts.h.  If key_usages is specified as NULL, keys for all usages
 * are returned.
 *
 * Note that the key_domain and key_usage parameters are considered as
 * advisory only.  Some policies may not distinguish keys according to
 * these properties.  The key_usage and domain parameters of returned
 * keys (output from pkc_get_key_trust_info) should always be checked 
 * before attempting to use a key.
 *
 * On successful return, keyinfobase will contain a PKC-allocated structure
 * which must be released with pkc_free_keyinfo().
 */


extern unsigned32 pkc_retrieve_keyinfo (trustbase_t *base,  
                                        char *name, 
                                        utc_t *key_date,
                                        uuid_t * key_domain,
                                        unsigned long * key_usages,
                                        selection_t *sel, 
                                        keyinfo_t **keyinfobase);







/*
 * pkc_get_key_count
 *
 * Return the number of keys within a keyinfo_t structure
 *
 */

extern unsigned32 pkc_get_key_count (keyinfo_t *keyinfobase, 
                                     size_t *key_count);







/*
 * pkc_get_key_data
 *
 * Extracts an encoded public key from a keyinfo_t structure.  
 * <Index> is the index of the key (ranging from 0..key_count-1).
 * On return, key_data will contain PKC-allocated storage which must
 * be released with pkc_free().
 */

extern unsigned32 pkc_get_key_data (keyinfo_t *keyinfobase, 
                                    unsigned key_index,
                                    unsigned char ** key_data,
                                    size_t * key_length);








/*
 * Miscellaneous key interrogation functions
 *
 */

/*
 * pkc_get_key_trust_info
 *
 * Returns a set of flags describing the trust that can be placed in the
 * key.  See the definition of certification_flags_t for more information.
 *
 * If key_domain and key_usages are passed as non-NULL pointers, on a 
 * succesfull return these parameters will describe the domain and permitted
 * usage(s) of the specified key.  Policies that do not distinguish keys
 * according to domain will indicate a domain of sec_pk_domain_unspecified;
 * policies that do not distinguish keys according to usage will indicate
 * all usages are permitted. 
 *
 */
extern unsigned32 pkc_get_key_trust_info (keyinfo_t  *keyinfobase, 
                                          unsigned key_index,
                                          certification_flags_t * flags,
                                          uuid_t * key_domain,
                                          unsigned long * key_usages);






/*
 * pkc_get_key_certifier_count
 *
 * Returns the number of CAs in the certification path of the
 * specified key.  
 *
 */

extern unsigned32 pkc_get_key_certifier_count (keyinfo_t  *keyinfobase, 
                                               unsigned key_index,
                                               size_t * ca_count);






/*
 * pkc_get_key_certifier_info
 *
 * Returns information about a specific certifier from a key's
 * certification path.  Certifier 0 is the CA that vouched for
 * the key; certifier 1 is the CA that vouched for certifier 0, etc.
 * The total number of certifiers for a given key is returned by
 * pkc_get_key_certifier_count above.  Any of the return parameters
 * may be passed as NULL if the corresponding information is not
 * required.  On return, the certifier_name parameter will contain
 * PKC-allocated storage which must be released with pkc_free().
 */

extern unsigned32 pkc_get_key_certifier_info (keyinfo_t *keyinfobase,
                                              unsigned key_index,
                                              unsigned certifier_index,
                                              char ** certifier_name,
                                              utc_t * certification_start,
                                              utc_t * certification_expiration,
                                              char * crl_valid,
                                              utc_t * crl_last_seen,
                                              utc_t * next_crl_expected);







/*
 * pkc_free_trustlist
 *
 */

extern unsigned32 pkc_free_trustlist(trustlist_t **tr_list);




/*
 * pkc_free_trustbase
 *
 * Note that a trustbase should not be freed until any keyinfo_t
 * structures that were derived from it have been freed.
 */

extern unsigned32 pkc_free_trustbase(trustbase_t **base);




/*
 * pkc_free_keyinfo
 * 
 */

extern unsigned32 pkc_free_keyinfo(keyinfo_t **keybase);




/*
 * pkc_free
 *
 * This routine should be used to release contiguous storage returned 
 * by any of the pkc_XXX routines.
 *
 */

extern void pkc_free(void *);




#ifdef __cplusplus
}
#endif

#endif
