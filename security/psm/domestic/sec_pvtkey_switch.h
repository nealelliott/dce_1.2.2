/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pvtkey_switch.h,v $
 * Revision 1.1.2.2  1996/11/13  18:09:55  arvind
 * 	Add/modify copyright info
 * 	[1996/09/09  21:50 UTC  arvind  /main/DCE_1.2.2/1]
 *
 * Revision 1.1.2.1  1996/10/04  16:00:47  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:00:26  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:50 UTC  arvind
 * 	     Merge PKSS to DCE_1.2.2
 * 
 * $EndLog$
 */
/*
 * (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */
/*
 * Copyright (c) Hewlett-Packard Company 1996
 * Unpublished work.  All Rights Reserved.
 *
 */

#ifndef SEC_PVTKEY_SWITCH_H_
#define SEC_PVTKEY_SWITCH_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include <dce/dcepsmmsg.h>


#define SEC_PVTKEY_SWITCH_FLAG_MATCHALL	    1
#define SEC_PVTKEY_SWITCH_FLAG_TERMINATE    2
#define SEC_PVTKEY_SWITCH_FLAG_ENABLED	    4

typedef struct {

    unsigned32 magic;

    unsigned32 flags;

    const uuid_t * keystore_uuid;

    char keystore_name[32];  /* Name must be 32 characters or less,       */
                             /* and NULL-terminated if less.              */

    error_status_t (* open)(idl_char *name,
                            sec_pk_domain_t *domain_id,
                            sec_pk_mechanism_handle_t *handle);

    error_status_t (* close)(sec_pk_mechanism_handle_t handle);

    error_status_t (* capabilities)(sec_pk_mechanism_handle_t handle,
                                    idl_void_p_t capability_parameter,
                                    sec_pk_pvtkey_capability * feature,
                                    idl_boolean * supported);

    error_status_t (* store)(sec_pk_mechanism_handle_t handle,
                             idl_char *pwd,
                             sec_pk_usage_flags_t key_usages,
                             unsigned32 key_vno,
                             sec_pk_data_t *private_key,
                             sec_pk_data_t *public_key);

    error_status_t (* delete)(sec_pk_mechanism_handle_t handle,
                              idl_char *pwd,
                              sec_pk_usage_flags_t key_usages,
                              unsigned32 key_vno);

    error_status_t (* generate)(sec_pk_mechanism_handle_t handle,
                                idl_char *pwd,
                                sec_pk_algorithm_id_t *alg_id,
                                unsigned32 key_length,
                                sec_pk_usage_flags_t key_usage,
                                unsigned32 key_vno,
                                sec_pk_data_t *public_key);

    error_status_t (* get)(sec_pk_mechanism_handle_t handle,
                           idl_char *pwd,
                           sec_pk_usage_flags_t key_usage,
                           unsigned32 *key_vno,
                           sec_pk_data_t *private_key,
                           sec_pk_data_t *public_key);

    error_status_t (* update)(sec_pk_mechanism_handle_t handle,
                              idl_char *current_pwd,
                              idl_char *new_pwd,
                              sec_pk_usage_flags_t key_usages,
                              unsigned32 *key_vno,
                              sec_pk_data_t *private_key,
                              sec_pk_data_t *public_key);

    error_status_t (* update_generate)(sec_pk_mechanism_handle_t handle,
                                       idl_char *current_pwd,
                                       idl_char *new_pwd,
                                       sec_pk_algorithm_id_t *alg_id,
                                       unsigned32 key_length,
                                       sec_pk_usage_flags_t key_usages,
                                       unsigned32 *key_vno,
                                       sec_pk_data_t *public_key);

} keystore_epv;



/*
 *  s e c _ p v t k e y _ _ g e t _ u u i d _ e r a _ b y _ u u i d
 *
 * Retrieve a UUID-valued ERA from a specific principal entry in the 
 * registry.  The desired ERA is specified by its UUID.
 */

unsigned32 sec_pvtkey__get_uuid_era_by_uuid(sec_rgy_name_t principal_name,
                                            const uuid_t * uuid,
                                            uuid_t * era_value);





/*
 * s e c _ p v t k e y _ _ l o o k u p _ k e y s t o r e 
 *
 * Lookup the keystore associated with the specified user's DCE login
 * key.  Returns a pointer to the internal EPV for that keystore, or
 * NULL if the keystore was not found.
 */

extern unsigned32 sec_pvtkey__lookup_keystore(const char * username,
                                              const sec_pk_domain_t * domain_id,
                                              const keystore_epv ** keystore);

/*
 * s e c _ p s m _ _ i n i t _ s v c
 *
 * Initialize PSM serviceability messaging.
 * This routine might be better moved elsewhere; however currently
 * the sec_pvtkey_XXX routines are the only ones that use PSM serviceability
 * messages, so fo the moment leave it here.
 * 
 * The routine is threadsafe, and can be invoked multiple times without
 * ill effects.
 *
 */
extern void sec_psm__init_svc(void);



/*
 * s e c _ p v t k e y _ _ r e g i s t e r _ k e y s t o r e 
 *
 * Register a new keystore implementation for the current image.
 * Keystores are registered either statically (for all images) via 
 * sec_pvtkey_registered_keystores.c, or dynamically (for the current
 * image only) by calling this routine.  All parameters are input.
 * The routine adds the new keystore implementation to the list of
 * keystores.  The implementation of this list is fairly limited:
 * 
 * 1.  There is a fixed-length list of keystores, and attempts to 
 *     register more keystores than will fit in the list will return 
 *     an error (code TBD).
 * 2.  Keystore implementations cannot be deleted from the list.  However,
 *     apart for releasing the slot within the keystore list, the effect
 *     of keystore deletion can be achieved by re-registering the keystore 
 *     you want to delete, setting replace = true and enabled = false.
 *
 */

extern unsigned32 sec_pvtkey__register_keystore(
    uuid_t * keystore_uuid, /* The UUID to which this pointer points        */
                            /* mustn't disappear!                           */
    const char * keystore_name,  
                            /* Name should be 32 characters or less.        */
                            /* Used in serviceability messages.  This is    */
                            /* copied into the keystore list, so it is OK   */
                            /* for it to disappear after the                */
                            /* register_keystore call returns.              */
    boolean32 enabled,      /* If true, the keystore will be enabled.       */
                            /* If false, the keystore will be disabled.     */
    boolean32 make_default, /* If true, this keystore will become the new   */
                            /* default keystore, to be used if the registry */
                            /* doesn't tell use which one to use.           */
    boolean32 replace,      /* If true, this keystore will replace any      */
                            /* pre-existing keystore(s) with the same UUID. */
                            /* If false, attempts to re-register an         */
                            /* already-existing keystore will fail.         */
    error_status_t (* open)(idl_char *name,
                            sec_pk_domain_t *domain_id,
                            sec_pk_mechanism_handle_t *handle),
    error_status_t (* close)(sec_pk_mechanism_handle_t handle),
    error_status_t (* capabilities)(sec_pk_mechanism_handle_t handle,
                                    idl_void_p_t capability_parameter,
                                    sec_pk_pvtkey_capability * feature,
                                    idl_boolean * supported),
    error_status_t (* store)(sec_pk_mechanism_handle_t handle,
                             idl_char *pwd,
                             sec_pk_usage_flags_t key_usages,
                             unsigned32 key_vno,
                             sec_pk_data_t *private_key,
                             sec_pk_data_t *public_key),
    error_status_t (* delete)(sec_pk_mechanism_handle_t handle,
                              idl_char *pwd,
                              sec_pk_usage_flags_t key_usages,
                              unsigned32 key_vno),
    error_status_t (* generate)(sec_pk_mechanism_handle_t handle,
                                idl_char *pwd,
                                sec_pk_algorithm_id_t *alg_id,
                                unsigned32 key_length,
                                sec_pk_usage_flags_t key_usage,
                                unsigned32 key_vno,
                                sec_pk_data_t *public_key),
    error_status_t (* get)(sec_pk_mechanism_handle_t handle,
                           idl_char *pwd,
                           sec_pk_usage_flags_t key_usage,
                           unsigned32 *key_vno,
                           sec_pk_data_t *private_key,
                           sec_pk_data_t *public_key),
    error_status_t (* update)(sec_pk_mechanism_handle_t handle,
                              idl_char *current_pwd,
                              idl_char *new_pwd,
                              sec_pk_usage_flags_t key_usages,
                              unsigned32 *key_vno,
                              sec_pk_data_t *private_key,
                              sec_pk_data_t *public_key),
    error_status_t (* update_generate)(sec_pk_mechanism_handle_t handle,
                                       idl_char *current_pwd,
                                       idl_char *new_pwd,
                                       sec_pk_algorithm_id_t *alg_id,
                                       unsigned32 key_length,
                                       sec_pk_usage_flags_t key_usages,
                                       unsigned32 *key_vno,
                                       sec_pk_data_t *public_key)
);




#ifdef __cplusplus
}
#endif

#endif
