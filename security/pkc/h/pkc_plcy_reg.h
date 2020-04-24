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
 * $Log: pkc_plcy_reg.h,v $
 * Revision 1.1.2.1  1996/11/13  20:39:30  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:56:34  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:27 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:40 UTC  arvind
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
 * ABSTRACT:    Policy Switch layer definitions.
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






#ifndef PKC_PLCY_REG_
#define PKC_PLCY_REG_

#ifndef __cplusplus
#error "C++ must be used for policy module registration"
#endif

extern "C" {
#include <stdlib.h>
#include <dce/dce.h>
#include <dce/dcepkcmsg.h>
#include <dce/gssapi.h>
#include <dce/sec_pk_base.h>
#include <dce/pkc_api.h>
};
#include <dce/pkc_common.h>
#include <dce/pkc_certs.h>
#include <dce/x500name.h>


typedef struct {
    char threadsafe;    /* non-zero if retrieve_keyinfo() may be    */
                        /* called simultaneously (within a single   */
                        /* session) by multiple threads.            */
    char multi_session; /* non-zero if the policy implementation    */
                        /* supports multiple simultaneous policy    */
                        /* sessions.                                */
} pkc_plcy_flags_t;


/* This data-type is used to register a new policy-module with the  */
/* certification API.  It describes a policy implementation, and    */
/* provides entry-points to its functions.                          */

typedef struct pkc_policy_struct_t {
    /* The version and alg_id fields are required for all versions  */
    /* of this data-structure.  Other fields may be                 */
    /* version-dependent.                                           */
    OM_uint32 version;      /* must be pkc_V1 for now               */
    gss_OID_desc policy_id;
    /* Object identifier that names the policy implementation       */

    pkc_plcy_flags_t flags;
    /* Describes the policy implementation capabilities             */

    char reserved[32 - sizeof(pkc_plcy_flags_t)];
    /* Reserve space for 32 bytes of flag data.                     */

    char * (* name)(void);
    /* Returns the policy name.  This will be used in               */
    /* diagnostic or auditing messages.  The name should be	    */
    /* returned in storage allocated using the pkc_malloc()	    */
    /* function defined in pkc_common.h.  This is the only	    */
    /* policy-module routine that may be called without having	    */
    /* first called the <open> routine.				    */


    /* NULL may be supplied as the address of any of the following  */
    /* routines, if the policy module does not provide or require   */
    /* the corresponding feature.  All policy modules should        */
    /* provide a <retrieve_keyinfo> function; other functions are   */
    /* optional.                                                    */

    unsigned32 (*open) (void** context);
    unsigned32 (*close) (void** context);
    /* Before invoking any policy routines (eg <retrieve_keyinfo>), */
    /* caller will establish a session with the policy              */
    /* implementation by invoking the implementation's open()       */
    /* function.  If the policy module maintains state information  */
    /* between calls, it may use the context parameter.             */
    /* This same context parameter will be passed to subsequent     */
    /* routine calls made by the certification facility.  Note that */
    /* once close() has been invoked, the certification facility    */
    /* will invoke open() again before making any further calls to  */
    /* the policy module.  If the open() routine stores any state   */
    /* in the context parameter, the close() routine should free    */
    /* this storage.                                                */

    unsigned32 (*establish_trustbase) (void **context,
                                const pkc_trust_list_t &initial_trust,  
                                const utc_t * date,
                                char initial_explicit_policy_required,
                                void ** trust_base_handle);

    /* This is a one-time call made by appln to initialize a trust */
    /* base. It returns the out_trust parameter, which contains an */
    /* extended trust list.                                        */
    /* After this call is made, the application can call           */
    /* retrieve_keyinfo to obtain the public keys of any           */
    /* particular principal.  The trustbase will not be changed    */
    /* as a rfesult of this call.                                  */
                                       
    unsigned32 (*retrieve_keyinfo) (
                            void ** context,
                            const void *trust_base_handle,  
                            const x500name & subjectName,
                            const utc_t * date,
                            const uuid_t & domain,
                            const pkc_generic_key_usage_t & desired_usage,
                            char initial_explicit_policy_req,
                            void ** keys_handle);
    /* A routine that retrieves the keys for the specified          */
    /* principal, stored in a policy-specific handle type.  The     */
    /* caller's initial trust is expressed in the <trust>           */
    /* parameter.  The desired subject name and key usage(s) are    */
    /* specified by the caller, along with an indication as to      */
    /* whether the initial certificate must explicitly contain the  */
    /* active policy in its Policies field.  The returned key       */
    /* information handle can be interrogated by various policy-    */
    /* specific pkc_cert_XXX functions to extract the actual key    */
    /* and determine the degree of trust that can be placed in that */
    /* key.                                                         */
    /* Note that some of these parameters relate to X.509 version 3 */
    /* certificates, support for which is not comitted for DCE 1.2. */
    /* The API has been designed with the intent that it be capable */
    /* of supporting all currently defined versions of X.509, so    */
    /* that it need not change when V3 support is added.            */

     unsigned32 (*delete_trustbase) (void **context,
                                     void **trust_base_handle
                                     );

     unsigned32 (*delete_keyinfo) (void **context,
                                   void **keys_handle
                                  );

/*
 * Interrogation routines
 */
 
    unsigned32 (*get_key_count) (void ** context,
                                 void * keys_handle,  
                                 size_t * key_count);

    unsigned32 (*get_key_data) (void ** context,
                                void * keys_handle,
                                unsigned key_index,
                                unsigned char ** key_data,
                                size_t * key_length);

    unsigned32 (*get_key_trust) (void ** context,
                                 void * keys_handle,
                                 unsigned key_index,
                                 certification_flags_t * flags,
                                 uuid_t * domain,
                                 pkc_generic_key_usage_t * usages);

    unsigned32 (*get_key_certifier_count) (void ** context,
                                           void * keys_handle,
                                           unsigned key_index,
                                           size_t * ca_count);

    unsigned32 (*get_key_certifier_info) (void ** context,
                                          void * keys_handle,
                                          unsigned key_index,
                                          unsigned ca_index,
                                          char ** ca_name,
                                          utc_t * certification_start,
                                          utc_t * certification_expiration,
                                          char  * is_crl_valid,
                                          utc_t * last_crl_seen,
                                          utc_t * next_crl_expected);

/*
 * etc.
 */


} pkc_policy_t;

#define PKC_REPLACE_NONE 0
#define PKC_REPLACE_ENTRYPOINTS 1
#define PKC_REPLACE_ALL 2

extern "C" unsigned32 pkc_plcy_register_policy(pkc_policy_t * plcy,
                                               int replacement_policy);

    /* Register an implementation of a policy.                      */
    /* The <replacement_policy> argument determines how the PKC     */
    /* facility will handle the case where an implementation of the */
    /* policy is already registered:                                */
    /*                                                              */
    /*     PKC_REPLACE_NONE:    An error will be returned if an     */
    /*                          implementation of the policy is     */
    /*                          already registered.                 */
    /*     PKC_REPLACE_ENTRYPOINTS: Only entrypoints that the       */
    /*                          original implementation did not     */
    /*                          provide will be replaced.           */
    /*     PKC_REPLACE_ALL:     The new implementation will replace */
    /*                          the existing one.                   */
    /* The use of PKC_REPLACE_ENTRYPOINTS is not currently          */
    /* supported.                                                   */


extern "C" unsigned32 pkc_plcy_lookup_policy(gss_OID oid,
                                             pkc_policy_t * details);
    /* The preferred way of obtaining information about a           */
    /* registered policy implementation, this function returns      */
    /* a policy implementation descriptor block for the             */
    /* specified policy, and leaves the policy list unlocked.       */
    /* The complete list of registered polcicies may be obtained    */
    /* by invoking pkc_get_registered_policies().                   */

extern "C" unsigned32 pkc_plcy_get_registered_policies(
                                         gss_OID_set * oid_set);
    /* Returns an OID-set describing the currently-registered       */
    /* policy implementations.                                      */




extern "C" unsigned32 pkc_plcy_establish_trustbase (
                            gss_OID policy,
                            const pkc_trust_list_t &initial_trust,  
                            const utc_t * date,
                            char initial_explicit_policy_required,
                            void ** trust_base_handle);

extern "C" unsigned32 pkc_plcy_retrieve_keyinfo(
                            gss_OID policy,
                            const void *trust_base_handle,  
                            const x500name & subjectName,
                            const utc_t * date,
                            const uuid_t & desired_domain,
                            const pkc_generic_key_usage_t & desired_usage,
                            char initial_explicit_policy_required,
                            void ** keys_handle);

extern "C" unsigned32 pkc_plcy_delete_trustbase(
                            gss_OID policy,
                            void **trust_base_handle);


extern "C" unsigned32 pkc_plcy_delete_keyinfo(
                            gss_OID policy,
                            void **keys_handle);



extern "C" unsigned32 pkc_plcy_get_key_count (gss_OID policy,
                                              void * keys_handle,  
                                              size_t * key_count);

extern "C" unsigned32 pkc_plcy_get_key_data (gss_OID policy,
                                             void * keys_handle,
                                             unsigned key_index,
                                             unsigned char ** key_data,
                                             size_t * key_length);

extern "C" unsigned32 pkc_plcy_get_key_trust (gss_OID policy,
                                              void * keys_handle,
                                              unsigned key_index,
                                              certification_flags_t * flags,
                                              uuid_t * domain,
                                              pkc_generic_key_usage_t * usages);

extern "C" unsigned32 pkc_plcy_get_key_certifier_count (
                                              gss_OID policy,
                                              void * keys_handle,
                                              unsigned key_index,
                                              size_t * ca_count);

extern "C" unsigned32 pkc_plcy_get_key_certifier_info (
                                              gss_OID policy,
                                              void * keys_handle,
                                              unsigned key_index,
                                              unsigned ca_index,
                                              char ** certifier_name,
                                              utc_t * certification_start,
                                              utc_t * certification_expiration,
                                              char  * is_crl_valid,
                                              utc_t * last_crl_seen,
                                              utc_t * next_crl_expected);

    /* The above functions search the list of registered policies   */
    /* for implementations of the specified policy. If found, the   */
    /* implementation will be opened if necessary and its           */
    /* corresponding member function will be invoked.  These        */
    /* routines provide the necessary mutex protection around       */
    /* non-threadsafe policy implementations.                       */



#endif



