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
 * $Log: pkc_crypto_reg.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:32:05  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:22:48  arvind]
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
 * ABSTRACT:    Crypto switch routines 
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


#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/pkc_crypto_reg.h>
#include "pkc_crypto_int.h"

#define ALG_OPEN 1
#define ALG_CLOSED 0


class pkc_signature_algorithm_int_t;

static pkc_signature_algorithm_int_t * algorithm_list = NULL;
static char initialized = 0;
static pthread_mutex_t alglist_lock;
static pthread_mutexattr_t alglist_mutexattr;
static pthread_once_t once_block = pthread_once_init;

static unsigned32 pkc_crypto_register_signature_alg_int(
                        pkc_signature_algorithm_t * alg,
                        int replacement_policy);

static void once_routine(void) {
    int i = 0;

    pthread_mutexattr_create(&alglist_mutexattr);
    pthread_mutexattr_setkind_np(&alglist_mutexattr, MUTEX_NONRECURSIVE_NP);
    pthread_mutex_init(&alglist_lock, alglist_mutexattr);

    while (pkc_preloaded_algorithms[i] != NULL) {
	pkc_crypto_register_signature_alg_int(pkc_preloaded_algorithms[i],
                                              PKC_REPLACE_ALL);
	i++;
    };

    initialized = 1;
}

void pkc_crypto_init(void) {

    if (!initialized) {
// We need to create the algorithm list-lock
        pthread_once(&once_block, once_routine);
    };				       

}

class pkc_signature_algorithm_int_t {
public:
    pkc_signature_algorithm_t implementation;
    char delete_pending;
    int state;
    void * alg_context;
    pkc_signature_algorithm_int_t * next;
    pkc_signature_algorithm_int_t * prev;
    pthread_mutex_t implementation_lock;
    pthread_mutexattr_t alg_mutexattr;
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
    void unlock(void) {
        pthread_mutex_unlock(&implementation_lock);
    };
    int check(gss_OID alg) {
        int result = 1;
        int already_locked;
        if ((already_locked = lock()) > 0) return 0;
        if (alg->length != implementation.alg_id.length) result = 0;
        else result = (memcmp(alg->elements,
                              implementation.alg_id.elements,
                              implementation.alg_id.length) == 0);
        if (!already_locked) unlock();
        return result;
    };    
    char * name(void) {
	return implementation.name();
    };
    unsigned32 open(void) {
        unsigned32 status = 0;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_alg_deleted;
        if (state != ALG_OPEN) {
            if (implementation.open != NULL) {
                if ((status = implementation.open(&alg_context)) == 0)
                state = ALG_OPEN;
            };
        };
        if (!already_locked) unlock();
        return status;
    };
    unsigned32 close(void) {
        unsigned32 status = 0;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_alg_deleted;
        if (state != ALG_CLOSED) {
            if (implementation.close != NULL) {
                if ((status = implementation.close(&alg_context)) == 0) 
                    state = ALG_CLOSED;
            };
        };
        if (!already_locked) unlock();
        return status;
    };
    unsigned32 verify(sec_pk_gen_data_t data,
                      sec_pk_data_t public_key,
                      sec_pk_data_t signature) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_alg_deleted;
        if (implementation.verify == NULL) {
            status = pkc_s_unimplemented;
        } else {
            if (state != ALG_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.verify(&alg_context,
                                           data,
                                           public_key,
                                           signature);
        };
        if (!already_locked) unlock();
        return status;
    };
    unsigned32 sign(sec_pk_gen_data_t data,
                    sec_pk_data_t private_key,
                    sec_pk_data_t * signature) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_alg_deleted;
        if (implementation.sign == NULL) {
            status = pkc_s_unimplemented;
        } else {
            if (state != ALG_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.sign(&alg_context,
                                         data,
                                         private_key,
                                         signature);
        };
        if (!already_locked) unlock();
        return status;
    };
    unsigned32 generate_keypair(unsigned32 size,
                                void * alg_info,
                                sec_pk_data_t * private_key,
                                sec_pk_data_t * public_key) {
        unsigned32 status;
        int already_locked;
        if ((already_locked = lock()) > 0) return pkc_s_alg_deleted;
        if (implementation.generate_keypair == NULL) {
            status = pkc_s_unimplemented;
        } else {
            if (state != ALG_OPEN) open();
            if (implementation.flags.threadsafe) {
                if (!already_locked) unlock();
                already_locked = -1;
            };
            status = implementation.generate_keypair(&alg_context,
                                                     size,
                                                     alg_info,
                                                     private_key,
                                                     public_key);
        };
        if (!already_locked) unlock();
        return status;
    };
    pkc_signature_algorithm_int_t(pkc_signature_algorithm_t * imp) {

        pthread_mutex_lock(&alglist_lock);
        if (algorithm_list != NULL) {            
            algorithm_list->prev = this;
        };
        next = algorithm_list;
        prev = NULL;
        algorithm_list = this;
	delete_pending = 0;
        pthread_mutexattr_create(&alg_mutexattr);
        pthread_mutexattr_setkind_np(&alg_mutexattr, MUTEX_NONRECURSIVE_NP);
        pthread_mutex_init(&implementation_lock, alg_mutexattr);
        pthread_mutexattr_delete(&alg_mutexattr);
        alg_context = NULL;
        state = ALG_CLOSED;
        next = NULL;
        prev = NULL;
        implementation = *imp;
        pthread_mutex_unlock(&alglist_lock);
    };
    ~pkc_signature_algorithm_int_t() {
        if (lock() > 0) return;
        delete_pending = 1;
        if (state == ALG_OPEN) close();
        unlock();
        pthread_mutex_destroy(&implementation_lock);
    };
};
 
static unsigned32 find_alg(gss_OID oid,
                           pkc_signature_algorithm_int_t * &algorithm) {
    unsigned32 status = 0;
    int done;

    pthread_mutex_lock(&alglist_lock);

    done = ((algorithm = algorithm_list) != NULL);

    while (!done) {
	if (algorithm->check(oid)) done = 1;
	else algorithm = algorithm->next;
    };

    if (algorithm == NULL) status = pkc_s_unsupported_algorithm;

    pthread_mutex_unlock(&alglist_lock);
    return status;
}



// #define PKC_REPLACE_NONE 0
// #define PKC_REPLACE_ENTRYPOINTS 1
// #define PKC_REPLACE_ALL 2

unsigned32 pkc_crypto_register_signature_alg(
                        pkc_signature_algorithm_t * alg,
                        int replacement_policy) {
    pkc_crypto_init();

    return pkc_crypto_register_signature_alg_int(alg, replacement_policy);
}


static unsigned32 pkc_crypto_register_signature_alg_int(
                        pkc_signature_algorithm_t * alg,
                        int replacement_policy) {
// This routine must only be called if the layer initialization has already
// been performed.

    pkc_signature_algorithm_int_t * ptr;

    switch (replacement_policy) {
    case PKC_REPLACE_ALL:
        new pkc_signature_algorithm_int_t(alg);
	// That's all, as the new implementation will go at the head of the
	// queue, making any previous implementations invisible.    
    case PKC_REPLACE_NONE:
	if (find_alg(&(alg->alg_id), ptr) == 0) {
	    return pkc_s_alg_already_registered;
	};
        new pkc_signature_algorithm_int_t(alg);
    default:
	return pkc_s_unsupported;
    };
    return 0;    
}



unsigned32 pkc_crypto_lookup_algorithm(
                           gss_OID oid,
                           pkc_signature_algorithm_t * details) {

    pkc_signature_algorithm_int_t * alg;
    unsigned32 status;
    int already_locked;

    pkc_crypto_init();

    if (status = find_alg(oid, alg)) return status;
    if ((already_locked = alg->lock()) > 0) return pkc_s_locking_violation;
    *details = alg->implementation;
    if (!already_locked) alg->unlock();
    return 0;
}    

unsigned32 pkc_crypto_get_registered_algorithms(gss_OID_set * oid_set) {
    pkc_signature_algorithm_int_t * algorithm;
    OM_uint32 maj;
    OM_uint32 min;
    unsigned32 status;

    pkc_crypto_init();

    maj = gssdce_create_empty_oid_set(&min, oid_set);
    if (GSS_ERROR(maj)) return min;

    pthread_mutex_lock(&alglist_lock);

    algorithm = algorithm_list;
    status = 0;
    while (algorithm != NULL) {
	maj = gssdce_add_oid_set_member(&min,
                                        &(algorithm->implementation.alg_id),
					oid_set);
        if (GSS_ERROR(maj)) status = min;
	algorithm = algorithm->next;
    };

    pthread_mutex_unlock(&alglist_lock);
    return status;
    

}



unsigned32 pkc_crypto_verify_signature(gss_OID algorithm,
                                       sec_pk_gen_data_t data,
                                       sec_pk_data_t public_key,
                                       sec_pk_data_t signature) {
    unsigned32 status;
    pkc_signature_algorithm_int_t * alg;

    pkc_crypto_init();

    if (status = find_alg(algorithm, alg)) return status;
    return alg->verify(data, public_key, signature);
}

unsigned32 pkc_crypto_sign(gss_OID algorithm,
                           sec_pk_gen_data_t data,
                           sec_pk_data_t private_key,
                           sec_pk_data_t * signature) {
    unsigned32 status;
    pkc_signature_algorithm_int_t * alg;

    pkc_crypto_init();

    if (status = find_alg(algorithm, alg)) return status;
    return alg->sign(data, private_key, signature);
}

unsigned32 pkc_crypto_generate_keypair(gss_OID algorithm,
                                       unsigned32 size,
                                       void * alg_info,
                                       sec_pk_data_t * private_key,
                                       sec_pk_data_t * public_key) {
    unsigned32 status;
    pkc_signature_algorithm_int_t * alg;

    pkc_crypto_init();

    if (status = find_alg(algorithm, alg)) return status;
    return alg->generate_keypair(size, alg_info, private_key, public_key);
}


