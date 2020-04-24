/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: sec_pvtkey.c,v $
 * Revision 1.1.2.3  1996/10/04  16:35:20  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:35:05  arvind]
 *
 * Revision /main/DCE_1.2.2/3  1996/09/17  18:48 UTC  cuti
 * 	     Merge
 * 
 * Revision /main/DCE_1.2.2/2  1996/09/16  20:32 UTC  aha
 * 	     CHFts19896: public key version support
 * 
 * Revision /main/DCE_1.2.2/aha_pk9_3/1  1996/09/13  23:54 UTC  aha
 * 	     Merge up from aha_pk9_2
 * 
 * $EndLog$
 */
/*
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 */

/*
 * SEC_PVTKEY switch layer.
 *
 * This module sits between the sec_pvtkey.h header file and various
 * sec_pvtkey_XXX.c providers.  Choice of a particular keystore is
 * made on a per-user basis, according to ERA settings on the principal.
 * This module dispatches the routine invocations to the approriate 
 * keystore.
 */


#include <stddef.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include <dce/dcepsmmsg.h>
#include <sec_pvtkey_switch.h>


#define SEC_PVTKEY_MECH_HANDLE_MAGIC 0x572fa502ul

typedef struct {
    unsigned32 magic;
    keystore_epv * keystore;
    sec_pk_mechanism_handle_t mech_specific;
} sec_pvtkey_mechanism_desc_t;

error_status_t get_handle(sec_pk_mechanism_handle_t ext_handle,
                          sec_pvtkey_mechanism_desc_t ** int_handle) {
    *int_handle = (sec_pvtkey_mechanism_desc_t *)ext_handle;
    if (!(*int_handle)) {
	return sec_pvtkey_mechanism_not_init;
	}

    if ((*int_handle)->magic != SEC_PVTKEY_MECH_HANDLE_MAGIC) {
	*int_handle = NULL;
	return sec_psm_invalid_keystore_handle;
    };
    return 0;
}

error_status_t sec_pvtkey_open(idl_char *name,
                               sec_pk_domain_t *domain_id,
                               sec_pk_mechanism_handle_t *handle) {
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_desc;

    *handle = NULL;

    mech_desc = (sec_pvtkey_mechanism_desc_t *)
                            malloc(sizeof(sec_pvtkey_mechanism_desc_t));

    if (mech_desc == NULL) return sec_psm_no_more_memory;

    mech_desc->magic = SEC_PVTKEY_MECH_HANDLE_MAGIC;
    mech_desc->keystore = NULL;
    mech_desc->mech_specific = NULL;

    st = sec_pvtkey__lookup_keystore((char *)name,
                                     domain_id,
                                     &(mech_desc->keystore));

    if (st) return st;


/* Now we have the correct keystore.  Call its own Open() function... */

    st = mech_desc->keystore->open(name,
                                   domain_id,
                                   &(mech_desc->mech_specific));

    *handle = (sec_pk_mechanism_handle_t)(mech_desc);
    return st;
}


error_status_t sec_pvtkey_close(sec_pk_mechanism_handle_t handle) {
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_handle;

    st = get_handle(handle, &mech_handle);

    if (st) return st;

    if (mech_handle->keystore == NULL) return sec_psm_keystore_not_found;

    st = mech_handle->keystore->close(mech_handle->mech_specific);

    free(mech_handle);

    return st;
}

error_status_t sec_pvtkey_store(sec_pk_mechanism_handle_t handle,
                                idl_char *pwd,
                                sec_pk_usage_flags_t key_usage,
                                unsigned32 key_vno,
                                sec_pk_data_t *private_key,
                                sec_pk_data_t *public_key) {
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_handle;

    st = get_handle(handle, &mech_handle);

    if (st) return st;

    if (mech_handle->keystore == NULL) return sec_psm_keystore_not_found;

    st = mech_handle->keystore->store(mech_handle->mech_specific,
                                      pwd,
                                      key_usage,
                                      key_vno,
                                      private_key,
                                      public_key);

    return st;
}

error_status_t sec_pvtkey_delete(sec_pk_mechanism_handle_t handle,
                                 idl_char *pwd,
                                 sec_pk_usage_flags_t key_usage,
                                 unsigned32 key_vno) {
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_handle;

    st = get_handle(handle, &mech_handle);

    if (st) return st;

    if (mech_handle->keystore == NULL) return sec_psm_keystore_not_found;

    st = mech_handle->keystore->delete(mech_handle->mech_specific,
                                       pwd,
                                       key_usage,
                                       key_vno);

    return st;

}

error_status_t sec_pvtkey_generate(sec_pk_mechanism_handle_t handle,
                                   idl_char *pwd,
                                   sec_pk_algorithm_id_t *alg_id,
                                   unsigned32 key_length,
                                   sec_pk_usage_flags_t key_usage,
                                   unsigned32 key_vno,
                                   sec_pk_data_t *public_key){
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_handle;

    st = get_handle(handle, &mech_handle);

    if (st) return st;

    if (mech_handle->keystore == NULL) return sec_psm_keystore_not_found;

    st = mech_handle->keystore->generate(mech_handle->mech_specific,
                                         pwd,
                                         alg_id,
                                         key_length,
                                         key_usage,
                                         key_vno,
                                         public_key);
    return st;

}

error_status_t sec_pvtkey_get(sec_pk_mechanism_handle_t handle,
                              idl_char *pwd,
                              sec_pk_usage_flags_t key_usage,
                              unsigned32 *key_vno,
                              sec_pk_data_t *private_key,
                              sec_pk_data_t *public_key) {
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_handle;

    st = get_handle(handle, &mech_handle);

    if (st) return st;

    if (mech_handle->keystore == NULL) return sec_psm_keystore_not_found;

    st = mech_handle->keystore->get(mech_handle->mech_specific,
                                    pwd,
                                    key_usage,
                                    key_vno,
                                    private_key,
                                    public_key);

    return st;
}

error_status_t sec_pvtkey_update(sec_pk_mechanism_handle_t handle,
                                 idl_char *current_pwd,
                                 idl_char *new_pwd,
                                 sec_pk_usage_flags_t key_usage,
                                 unsigned32 *key_vno,
                                 sec_pk_data_t *private_key,
                                 sec_pk_data_t *public_key) {
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_handle;

    st = get_handle(handle, &mech_handle);

    if (st) return st;

    if (mech_handle->keystore == NULL) return sec_psm_keystore_not_found;

    st = mech_handle->keystore->update(mech_handle->mech_specific,
                                       current_pwd,
                                       new_pwd,
                                       key_usage,
                                       key_vno,
                                       private_key,
                                       public_key);

   return st;
}

error_status_t sec_pvtkey_update_generate(sec_pk_mechanism_handle_t handle,
                                          idl_char *current_pwd,
                                          idl_char *new_pwd,
                                          sec_pk_algorithm_id_t *alg_id,
                                          unsigned32 key_length,
                                          sec_pk_usage_flags_t key_usages,
                                          unsigned32 *key_vno,
                                          sec_pk_data_t *public_key) {
    error_status_t st;
    sec_pvtkey_mechanism_desc_t * mech_handle;

    st = get_handle(handle, &mech_handle);

    if (st) return st;

    if (mech_handle->keystore == NULL) return sec_psm_keystore_not_found;

    st = mech_handle->keystore->update_generate(mech_handle->mech_specific,
                                                current_pwd,
                                                new_pwd,
                                                alg_id,
                                                key_length,
                                                key_usages,
                                                key_vno,
                                                public_key);

    return st;
}

