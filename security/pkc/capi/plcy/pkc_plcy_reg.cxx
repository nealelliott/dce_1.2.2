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
 * $Log: pkc_plcy_reg.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:33:28  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:27:45  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:26 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
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
 * ABSTRACT:    Switch Layer for switching between different policies.
 *              This provides thread safeness to policies that are 
 *              not thread-safe.	
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
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/gssapi.h>
}

#include <dce/pkc_plcy_reg.h>
#include <dce/pkc_common.h>
#include "pkc_plcy_int.h"

#define POLICY_OPEN 1
#define POLICY_CLOSED 0


class pkc_policy_int_t;

static pkc_policy_int_t * policy_list = NULL;
static char initialized = 0;
static pthread_mutex_t policylist_lock;
static pthread_mutexattr_t policylist_mutexattr;
static pthread_once_t once_block = pthread_once_init;

static unsigned32 pkc_plcy_int_register_policy(pkc_policy_t * plcy,
                                               int replacement_policy);

static void pkc_plcy_int_init(void) {

    int i;

    pkc_init_svc_handle();
    i = 0; /* mbg: */

    while (pkc_preloaded_policies[i] != NULL) {
	pkc_plcy_int_register_policy(pkc_preloaded_policies[i],
                                    PKC_REPLACE_ALL);
	i++;
    };
				       
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug5,
              "PKC_PLCY_REG_INIT: Registered %d pre-loaded policy implementations",
	      i));

}

static void once_routine(void) {
    pthread_mutexattr_create(&policylist_mutexattr);
    pthread_mutexattr_setkind_np(&policylist_mutexattr, MUTEX_NONRECURSIVE_NP);
    pthread_mutex_init(&policylist_lock, policylist_mutexattr);
    pkc_plcy_int_init();
    initialized = 1;
}

void pkc_plcy_init(void) {
    if (!initialized) {
        pthread_once(&once_block, once_routine);
    };
}

class pkc_policy_int_t {
public:
    pkc_policy_t implementation;
    char delete_pending;
    int state;
    void * policy_context;
    pkc_policy_int_t * next;
    pkc_policy_int_t * prev;
    pthread_mutex_t implementation_lock;
    pthread_mutexattr_t policy_mutexattr;
    int lock(void) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "policy_int::lock()"));
        if (delete_pending) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug3,
                          "policy_int::lock() - delete pending"));
	    return 1;
	};
        if (pthread_mutex_lock(&implementation_lock)) {
                 // we already have the lock
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug1,
                          "policy_int::lock() - already locked"));
	    return -1;
        };
        if (delete_pending) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug3,
                          "policy_int::lock() - delete pending post-lock"));
            pthread_mutex_unlock(&implementation_lock);
            return 1;
        };
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "policy_int::lock() - success"));
        return 0;        
    };
    void unlock(void) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "policy_int::unlock()"));
        pthread_mutex_unlock(&implementation_lock);
    };
    int check(gss_OID policy) {
        int result = 1;
        int already_locked;
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "policy_int::check()"));
#ifdef DCE_DEBUG	
	{   char * name = implementation.name();
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "policy is \"%s\"", name));
	    pkc_free(name);
	};
#endif
        if ((already_locked = lock()) > 0) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug3,
                          "policy_int::check() - policy locking violation"));
            return 0;
        };
        if (policy->length != implementation.policy_id.length) result = 0;
        else result = (memcmp(policy->elements,
                              implementation.policy_id.elements,
                              implementation.policy_id.length) == 0);
        if (!already_locked) unlock();
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "policy_int::check() returning %d", result));
        return result;
    };    
    char * name(void) {
	return implementation.name();
    };
    unsigned32 open(void) {
        unsigned32 status = 0;
        int already_locked;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Opening \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
        if (state != POLICY_OPEN) {
            if (implementation.open != NULL) {
                if ((status = implementation.open(&policy_context)) == 0)
                state = POLICY_OPEN;
            };
        };
        if (!already_locked) unlock();
        return status;
    };
    unsigned32 close(void) {
        unsigned32 status = 0;
        int already_locked;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Closing \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
        if (state != POLICY_CLOSED) {
            if (implementation.close != NULL) {
                if ((status = implementation.close(&policy_context)) == 0) 
                    state = POLICY_CLOSED;
            };
        };
        if (!already_locked) unlock();
        return status;
    };


    unsigned32 establish_trustbase(
                            const pkc_trust_list_t &trust,  
                            const utc_t * date,
			    char initial_explicit_policy_required,
                            void ** trustbase) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Establish_trustbase \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.establish_trustbase == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "Establish_trustbase is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.establish_trustbase(
                                        &policy_context,
                                        trust,  
                                        date,
                                        initial_explicit_policy_required,
                                        trustbase);
        };
        if (!already_locked) unlock();
        return status;
    };

    unsigned32 delete_trustbase(void ** trustbase) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Delete_trustbase \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.delete_trustbase == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "Delete_trustbase is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.delete_trustbase(
                                        &policy_context,
                                        trustbase);
        };
        if (!already_locked) unlock();
        return status;
    };

    unsigned32 delete_keyinfo(void ** keyinfo) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Delete_keyinfo \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.delete_keyinfo == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Delete_keyinfo in unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.delete_keyinfo(
                                        &policy_context,
                                        keyinfo);
        };
        if (!already_locked) unlock();
        return status;
    };


    unsigned32 retrieve_keyinfo(const void * trust,  
                                const x500name & subjectName,
                                const utc_t * date,
                                const uuid_t & domain,
                                const pkc_generic_key_usage_t & desired_usage,
                                char initial_explicit_policy_required,
                                void ** keys_handle) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Retrieve_kleyinfo \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.retrieve_keyinfo == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Retrieve_keyinfo is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.retrieve_keyinfo(
                                        &policy_context,
                                        trust,  
                                        subjectName,
                                        date,
                                        domain,
                                        desired_usage,
                                        initial_explicit_policy_required,
                                        keys_handle);
        };
        if (!already_locked) unlock();
        return status;
    };

    unsigned32 get_key_count(void *keys_handle,
                             size_t * key_count) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Get_Key_Count \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.get_key_count == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::Get_Key_Count is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.get_key_count(
                                        &policy_context,
                                        keys_handle,
                                        key_count);
        };
        if (!already_locked) unlock();
        return status;
    };

    unsigned32 get_key_data(void *keys_handle,
                            unsigned index,
			    unsigned char ** key_data,
			    size_t * key_length) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::get_key_data \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.get_key_data == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::get_key_data is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.get_key_data(
                                        &policy_context,
                                        keys_handle,
                                        index,
                                        key_data,
                                        key_length);
        };
        if (!already_locked) unlock();
        return status;
    };

    unsigned32 get_key_trust(void *keys_handle,
                             unsigned index,
			     certification_flags_t * flags,
			     uuid_t * domain,
                             pkc_generic_key_usage_t * usages) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::get_key_trust \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.get_key_trust == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::get_key_trust is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.get_key_trust(
                                        &policy_context,
                                        keys_handle,
                                        index,
                                        flags,
                                        domain,
                                        usages);
        };
        if (!already_locked) unlock();
        return status;
    };

    unsigned32 get_key_certifier_count(void *keys_handle,
                                       unsigned index,
                                       size_t * ca_count) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::get_key_certifier_count \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.get_key_certifier_count == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::get_key_certifier_count is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.get_key_certifier_count(
                                        &policy_context,
                                        keys_handle,
                                        index,
                                        ca_count);
        };
        if (!already_locked) unlock();
        return status;
    };

    unsigned32 get_key_certifier_info(void *keys_handle,
                                      unsigned key_index,
                                      unsigned ca_index,
                                      char ** ca_name,
                                      utc_t * certification_start,
                                      utc_t * certification_expiration,
                                      char * is_crl_valid,
                                      utc_t * last_crl_seen,
                                      utc_t * next_crl_expected) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::get_key_certifier_info \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif
        if (implementation.get_key_certifier_info == NULL) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                       "policy_int::get_key_certifier_info is unimplemented"));
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.get_key_certifier_info(
                                        &policy_context,
                                        keys_handle,
                                        key_index,
                                        ca_index,
                                        ca_name,
                                        certification_start,
                                        certification_expiration,
                                        is_crl_valid,
                                        last_crl_seen,
                                        next_crl_expected);
        };
        if (!already_locked) unlock();
        return status;
    };

    pkc_policy_int_t(pkc_policy_t * imp) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "policy_int::policy_int - creating a policy"));
// Now lock the list...
        pthread_mutex_lock(&policylist_lock);
// If there are elements already on the list, set the prev ptr of the first
// element to point to this one.
        if (policy_list != NULL) {            
            policy_list->prev = this;
        };
// Set our own prev & next ptrs...
        next = policy_list;
        prev = NULL;
// And finally put us at the start of the queue
        policy_list = this;

// Create the lock for this policy 
        pthread_mutexattr_create(&policy_mutexattr);
        pthread_mutexattr_setkind_np(&policy_mutexattr, MUTEX_NONRECURSIVE_NP);
        pthread_mutex_init(&implementation_lock, policy_mutexattr);
        pthread_mutexattr_delete(&policy_mutexattr);
        policy_context = NULL;
        state = POLICY_CLOSED;
	delete_pending = 0;
        implementation = *imp;
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::policy_int - Registered \"%s\"",
                          name));
	    pkc_free(name);
	};
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                      "Policy OID length: %d octets",
		      implementation.policy_id.length));
#endif
        pthread_mutex_unlock(&policylist_lock);
    };
    ~pkc_policy_int_t() {
#ifdef DCE_DEBUG
	{   char * name = implementation.name();
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                          "policy_int::~policy_int \"%s\"",
                          name));
	    pkc_free(name);
	};
#endif

        if (lock() > 0) {
            DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug1,
                          "policy_int::~policy_int - Can't acquire lock!"));
	    return;
	};
        delete_pending = 1;
        if (state == POLICY_OPEN) close();
        unlock();
        pthread_mutex_lock(&policylist_lock);
	if (prev) prev->next = next;
	if (next) next->prev = prev;
        pthread_mutex_destroy(&implementation_lock);
        pthread_mutex_unlock(&policylist_lock);
    };
};



static unsigned32 find_policy(gss_OID oid,
                              pkc_policy_int_t * &policy) {
    unsigned32 status = 0;
    int done;
    int pcount = 0;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug9,
                  "Hunting for a policy implementation..."));
            
    pthread_mutex_lock(&policylist_lock);

    done = ((policy = policy_list) == NULL);

    while ((policy != NULL) && !done) {
	pcount++;
	if (policy->check(oid)) done = 1;
	else policy = policy->next;
    };

    if (policy == NULL) {
       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_general, svc_c_debug2,
                     "Didn't find desired policy in list of %d", pcount));
	status = pkc_s_unsupported_policy;
    };
    
    pthread_mutex_unlock(&policylist_lock);
    return status;
}



// #define PKC_REPLACE_NONE 0
// #define PKC_REPLACE_ENTRYPOINTS 1
// #define PKC_REPLACE_ALL 2

unsigned32 pkc_plcy_register_policy(pkc_policy_t * plcy,
                                    int replacement_policy) {
    pkc_plcy_init();
    return pkc_plcy_int_register_policy(plcy, replacement_policy);
}

static unsigned32 pkc_plcy_int_register_policy(pkc_policy_t * plcy,
                                               int replacement_policy) {
    pkc_policy_int_t * ptr;


    switch (replacement_policy) {
    case PKC_REPLACE_ALL:
        new pkc_policy_int_t(plcy);
	// That's all, as the new implementation will go at the head of the
	// queue, making any previous implementations invisible.    
    case PKC_REPLACE_NONE:
	if (find_policy(&(plcy->policy_id), ptr) == 0) {
	    return pkc_s_plcy_already_registered;
	};
        new pkc_policy_int_t(plcy);
    default:
	return pkc_s_unsupported;
    };
    return 0;    
}



unsigned32 pkc_plcy_lookup_policy(gss_OID oid,
                                  pkc_policy_t * details) {

    pkc_policy_int_t * policy;
    unsigned32 status;
    int already_locked;

    pkc_plcy_init();

    if (status = find_policy(oid, policy)) return status;
    if ((already_locked = policy->lock()) > 0) return pkc_s_locking_violation;
    *details = policy->implementation;
    if (!already_locked) policy->unlock();
    return 0;
}    

unsigned32 pkc_plcy_get_registered_policies(gss_OID_set * oid_set) {
    pkc_policy_int_t * policy;
    OM_uint32 maj;
    OM_uint32 min;
    unsigned32 status;

    pkc_plcy_init();

    maj = gssdce_create_empty_oid_set(&min, oid_set);
    if (GSS_ERROR(maj)) return min;

    pthread_mutex_lock(&policylist_lock);

    policy = policy_list;
    status = 0;
    while (policy != NULL) {
	maj = gssdce_add_oid_set_member(&min,
                                        &(policy->implementation.policy_id),
					oid_set);
        if (GSS_ERROR(maj)) status = min;
	policy = policy->next;
    };

    pthread_mutex_unlock(&policylist_lock);
    return status;
    

}



unsigned32 pkc_plcy_retrieve_keyinfo(
                               gss_OID policy,
                               const void *trust,  
                               const x500name & subjectName,
                               const utc_t * date,
                               const uuid_t & domain,
                               const pkc_generic_key_usage_t & desired_usage,
                               char initial_explicit_policy_required,
                               void ** keys_handle) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->retrieve_keyinfo(trust,
                                  subjectName,
                                  date,
                                  domain,
                                  desired_usage,
                                  initial_explicit_policy_required,
                                  keys_handle);
}


unsigned32 pkc_plcy_establish_trustbase (
                                 gss_OID policy,
                                 const pkc_trust_list_t & trust,  
                                 const utc_t * date,
                                 char initial_explicit_policy_required,
                                 void **trust_base_handle) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->establish_trustbase(trust,
                                     date,
                                     initial_explicit_policy_required,
                                     trust_base_handle);
}

unsigned32 pkc_plcy_delete_trustbase (
                        gss_OID policy,
                        void **trustbase) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->delete_trustbase(trustbase); 
}
                                      

unsigned32 pkc_plcy_delete_keyinfo (
                        gss_OID policy,
                        void **keys_handle) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->delete_keyinfo(keys_handle); 
}
                                      

unsigned32 pkc_plcy_get_key_count (
                        gss_OID policy,
                        void *keys_handle,
			size_t * key_count) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->get_key_count(keys_handle, key_count); 
}
                                      

unsigned32 pkc_plcy_get_key_data (
                        gss_OID policy,
                        void *keys_handle,
                        unsigned key_index,
			unsigned char ** key_data,
			size_t * key_length) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->get_key_data(keys_handle,
                              key_index,
                              key_data,
                              key_length); 
}
                                      


unsigned32 pkc_plcy_get_key_trust (
                        gss_OID policy,
                        void *keys_handle,
                        unsigned key_index,
			certification_flags_t * flags,
                        uuid_t * domain,
                        pkc_generic_key_usage_t * usages) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->get_key_trust(keys_handle,
                              key_index,
                              flags,
                              domain,
                              usages);
}
                                      


unsigned32 pkc_plcy_get_key_certifier_count (
                        gss_OID policy,
                        void *keys_handle,
                        unsigned key_index,
                        size_t * ca_count) {

    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->get_key_certifier_count (keys_handle,
                                          key_index,
                                          ca_count);
}
                                      

unsigned32 pkc_plcy_get_key_certifier_info (
                        gss_OID policy,
                        void *keys_handle,
                        unsigned key_index,
                        unsigned ca_index,
                        char ** certifier_name,
                        utc_t * certification_start,
                        utc_t * certification_expiration,
                        char * is_crl_valid,
                        utc_t * last_crl_seen,
                        utc_t * next_crl_expected) {

    unsigned32 status;
    pkc_policy_int_t * plcy;

    pkc_plcy_init();

    if (status = find_policy(policy, plcy)) return status;
    return plcy->get_key_certifier_info(keys_handle,
                                        key_index,
                                        ca_index,
                                        certifier_name,
                                        certification_start,
                                        certification_expiration,
                                        is_crl_valid,
                                        last_crl_seen,
                                        next_crl_expected);

}
                                      

