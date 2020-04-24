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
 * $Log: pkc_plcy_api.h,v $
 * Revision 1.1.2.1  1996/11/13  20:38:39  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:55:47  arvind]
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
 * ABSTRACT:    Policy Switch-Layer definitions.
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
#include "pkc_plcy_int.h"

#define POLICY_OPEN 1
#define POLICY_CLOSED 0


class pkc_policy_int_t;

static pkc_policy_int_t * policy_list = NULL;
static char initialized = 0;
static pthread_mutex_t policylist_lock;
static pthread_mutexattr_t policylist_mutexattr;
static pthread_once_t once_block = pthread_once_init;

static void once_routine(void) {
    pthread_mutexattr_create(&policylist_mutexattr);
    pthread_mutexattr_setkind_np(&policylist_mutexattr, MUTEX_NONRECURSIVE_NP);
    pthread_mutex_init(&policylist_lock, policylist_mutexattr);
    initialized = 1;
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
        if (delete_pending) return 1;
        if (pthread_mutex_lock(&implementation_lock)) return -1;
                                                 // we already have the lock
        if (delete_pending) {
            pthread_mutex_unlock(&implementation_lock);
            return 1;
        };
        return 0;        
    };
    unlock(void) {
        pthread_mutex_unlock(&implementation_lock);
    };
    int check(gss_OID policy) {
        int result = 1;
        int already_locked;
        if ((already_locked = lock()) > 0) return 0;
        if (policy->length != implementation.policy_id.length) result = 0;
        else result = (memcmp(policy->elements,
                              implementation.policy_id.elements,
                              implementation.policy_id.length) == 0);
        if (!already_locked) unlock();
        return result;
    };    
    char * name(void) {
	return implementation.name();
    };
    unsigned32 open(void) {
        unsigned32 status = 0;
        int already_locked;
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


    unsigned32 establish_trustbase(const pkc_trust_list_t &trust,  
                            const utc_t * date,
			    pkc_key_usage_t desired_usage,
			    char initial_explicit_policy_required,
                            void ** trustbase) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
        if (implementation.retrieve_key == NULL) {
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
                                        desired_usage,
                                        initial_explicit_policy_required,
                                        trustbase);
        };
        if (!already_locked) unlock();
        return status;
    };


    unsigned32 retrieve_key(const void * trust,  
                            const x500name & subjectName,
                            const utc_t * date,
			    pkc_key_usage_t desired_usage,
			    char initial_explicit_policy_required,
                            pkc_trusted_key_t ** keys, size_t *no_of_keys) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_plcy_deleted;
        if (implementation.retrieve_key == NULL) {
            status = pkc_s_unimplemented;
        } else {
            if (state != POLICY_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.retrieve_key(
                                        &policy_context,
                                        trust,  
                                        subjectName,
                                        date,
                                        desired_usage,
                                        initial_explicit_policy_required,
                                        keys, no_of_keys);
        };
        if (!already_locked) unlock();
        return status;
    };
    pkc_policy_int_t(pkc_policy_t * imp) {
        if (!initialized) {
// We need to create the policy list-lock
            pthread_once(&once_block, once_routine);
        };
        pthread_mutex_lock(&policylist_lock);
        if (policy_list != NULL) {            
            policy_list->prev = this;
        };
        next = policy_list;
        prev = NULL;
        policy_list = this;

        pthread_mutexattr_create(&policy_mutexattr);
        pthread_mutexattr_setkind_np(&policy_mutexattr, MUTEX_NONRECURSIVE_NP);
        pthread_mutex_init(&implementation_lock, policy_mutexattr);
        pthread_mutexattr_delete(&policy_mutexattr);
        policy_context = NULL;
        state = POLICY_CLOSED;
        next = NULL;
        prev = NULL;
        implementation = *imp;
        pthread_mutex_unlock(&policylist_lock);
    };
    ~pkc_policy_int_t() {
        if (lock() > 0) return;
        delete_pending = 1;
        if (state == POLICY_OPEN) close();
        unlock();
        pthread_mutex_destroy(&implementation_lock);
    };
};



static unsigned32 find_policy(gss_OID oid,
                              pkc_policy_int_t * &policy) {
    unsigned32 status = 0;
    int done;

            
    if (!initialized) {
// We need to create the policy  list-lock
	pthread_once(&once_block, once_routine);
    };
    pthread_mutex_lock(&policylist_lock);

    done = ((policy  = policy_list) != NULL);

    while (!done) {
	if (policy->check(oid)) done = 1;
	else policy = policy->next;
    };

    if (policy == NULL) status = pkc_s_unsupported_policy;

    pthread_mutex_unlock(&policylist_lock);
    return status;
}



// #define PKC_REPLACE_NONE 0
// #define PKC_REPLACE_ENTRYPOINTS 1
// #define PKC_REPLACE_ALL 2

unsigned32 pkc_plcy_register_policy(pkc_policy_t * plcy,
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

    maj = gssdce_create_empty_oid_set(&min, oid_set);
    if (GSS_ERROR(maj)) return min;

    if (!initialized) {
// We need to create the policy list-lock
	pthread_once(&once_block, once_routine);
    };
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



unsigned32 pkc_plcy_retrieve_key(gss_OID policy,
                                 const pkc_trust_list_t & trust,  
                                 const x500name & subjectName,
                                 const utc_t * date,
                                 pkc_key_usage_t desired_usage,
                                 char initial_explicit_policy_required,
                                 pkc_key_information_t & key) {
    unsigned32 status;
    pkc_policy_int_t * plcy;

    if (status = find_policy(policy, plcy)) return status;
    return plcy->retrieve_key(trust,
                              subjectName,
                              date,
                              desired_usage,
                              initial_explicit_policy_required,
                              key);
}


void pkc_plcy_init(void) {

    int i;

    i = 0; /* mbg: */
    while (pkc_preloaded_policies[i] != NULL) {
	pkc_plcy_register_policy(pkc_preloaded_policies[i],
                                 PKC_REPLACE_ALL);
	i++;
    };
				       

}
