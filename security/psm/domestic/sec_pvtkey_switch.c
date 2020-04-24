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
 * $Log: sec_pvtkey_switch.c,v $
 * Revision 1.1.2.3  1996/11/13  19:09:45  arvind
 * 	OSF DCE 1.2.2 Drop 5
 *
 * 	CHFts20483: in sec_pvtkey__register_keystore() set terminate flag before
 * 	looping through existing keystores clearing default flags.
 * 	[1996/11/04  18:00 UTC  aha  /main/DCE_1.2.2/aha_pk11/2]
 *
 * 	CHFts20483: in sec_pvtkey__register_keystore(), unset any previous
 * 	default mechanism if a new default is registered.
 * 	[1996/11/04  17:06 UTC  aha  /main/DCE_1.2.2/aha_pk11/1]
 *
 * 	CHFts20144: remove lookup of DCEPKPrivateKeyStorage ERA from krbtgt
 * 	principal as the default in sec_pvtkey__lookup_keystore()
 * 	[1996/10/18  20:08 UTC  sommerfeld  /main/DCE_1.2.2/4]
 *
 * 	CHFts20136: sec_pvtkey__lookup_keystore() now accepts only domains
 * 	registered with sec_pubkey_define_domain().
 * 	[1996/10/15  19:54 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * 	CHFts20361: references to uninit krbtgt_username
 * 	[1996/10/16  23:24 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_3/1]
 *
 * Revision 1.1.2.2  1996/10/15  20:54:00  arvind
 * 	Silence some compiler warnings.
 * 	Unfortunately, we can't silence all of them because uuid_equal and
 * 	uuid_to_string don't have their input-only parameters marked `const'..
 * 	[1996/10/09  18:43 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/3]
 * 
 * 	fix keystore id handling.
 * 	[1996/10/09  18:37 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/2]
 * 
 * 	Fix register_keystore to store the keystore_uuid
 * 	[1996/10/03  20:48 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/2]
 * 
 * 	Remove sec_pvtkey__lookup_keystore() checks for domain
 * 	[1996/10/03  19:52 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/1]
 * 
 * Revision 1.1.2.1  1996/10/04  16:33:47  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:33:31  arvind]
 * 
 * Revision /main/DCE_1.2.2/2  1996/09/16  20:31 UTC  aha
 * 	     CHFts19896: public key version support
 * 
 * Revision /main/DCE_1.2.2/aha_pk9_3/1  1996/09/13  23:54 UTC  aha
 * 	     Change sec_pk_domain_dce_pk_login to sec_pk_domain_dce_general
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */

#include <string.h>
#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/dce_svc.h>
#include <dce/dce_msg.h>
#include <pthread.h>
#include <dce/dcesecmsg.h>
#include <dce/dcepsmmsg.h>
#include "dcepsmsvc.h"
#include "dcepsmmac.h"
#include <dce/sec_pk.h>
#include <dce/sec_pubkey.h>
#include "sec_pvtkey_switch.h"
#include "sec_pvtkey_registered_keystores.h"
#include "sec_pvtkey_era.h"


dce_svc_handle_t psm_svc_handle;
static pthread_once_t once_block = pthread_once_init;
static int psm_msg_initialized = 0;
static int sec_pvtkey__initialized = 0;
static pthread_mutex_t keystore_mutex;

static void psm__once_routine(void) {
    error_status_t st;

    psm_msg_initialized = 1;
    psm_svc_handle = dce_svc_register(psm_svc_table,
                                      (idl_char *)"psm",
                                      &st);
    dce_msg_define_msg_table(psm__table,
                             sizeof(psm__table) / sizeof(psm__table[0]),
                             &st);

}

void sec_psm__init_svc(void) {
    
    if (!psm_msg_initialized) {
	pthread_once(&once_block, psm__once_routine);
    };

}

static void sec_pvtkey__once_routine(void) {
    error_status_t st;

    pthread_mutex_init(&keystore_mutex,
                       pthread_mutexattr_default);

    sec_pvtkey__initialized = 1;

}

static void sec_pvtkey__init(void) {
    
    if (!sec_pvtkey__initialized) {
	pthread_once(&once_block, sec_pvtkey__once_routine);
    };

}


static void sec_pvtkey__lock_keystore_list(void) {
    sec_pvtkey__init();
    pthread_mutex_lock(&keystore_mutex);
}

static void sec_pvtkey__unlock_keystore_list(void) {
    sec_pvtkey__init();
    pthread_mutex_unlock(&keystore_mutex);
}

unsigned32 sec_pvtkey__lookup_keystore(const char * username,
                                       const sec_pk_domain_t * domain_id,
                                       const keystore_epv ** keystore) {

    unsigned32 st = 0;
    unsigned32 era_st = 0;
    int i;
    uuid_t keystore_id;
#if 0
    sec_rgy_name_t krbtgt_username;
#endif
    sec_rgy_name_t user_username;
    boolean32 matched;
    sec_pk_domain_def    *pkd_p;

    sec_psm__init_svc();

#ifdef DEBUG
    dce_svc_printf(SEC_PSM_I_LOOKUP_KEYSTORE_MSG, username);
#endif
    /* See if domain is one of those registered for DCE
     *
     * Note: currently only DCE domains are supported, and
     * they have an ERA for storing the pvtkey mechanism
     * each user should use.
     *
     * Other domains could be supported by providing a
     * mechanism here to look up the pvtkey mechanism
     * associated either with the domain or with the
     * user (for use in that domain).
     */
    if ((pkd_p=sec_pubkey_find_domain(domain_id)) == NULL) {
#ifdef DEBUG
        dce_svc_printf(SEC_PSM_I_BAD_DOMAIN_MSG, username);
#endif
	return sec_pk_e_domain_unsupported;
    };

/* 
 * Lock the keystore list.  From here on, every return path _must_ unlock
 * this list before returning.
 */
    sec_pvtkey__lock_keystore_list();


/*
 * Get uuid of matching keystore.
 * First, try to retrieve the DCEPKPrivateKeyStorage ERA from the principal.
 */

    (void) strncpy ((char *)user_username, username, sizeof (sec_rgy_name_t));
    era_st = sec_pvtkey__get_uuid_era_by_uuid(
                               user_username,
                               &sec_pvtkey_private_key_storage_era,
                               &keystore_id);

    if (era_st != 0) {

#ifdef DEBUG
	dce_svc_printf(SEC_PSM_I_NO_PRIN_KEYSTORE_ERA_MSG, username);
#endif

/*
 * Still can't find a keystore.  Search the list to see if we have a default
 * keystore registered...
 */
        for (i=0; i< MAX_KEYSTORES; i++) {

            if (installed_keystore[i].magic != SEC_PVTKEY_KEYSTORE_MAGIC) {

                dce_svc_printf(SEC_PSM_I_BAD_KEYSTORE_LIST_MSG, i);

                sec_pvtkey__unlock_keystore_list();
                return sec_psm_keystore_list_corrupt;
            };

            if ((installed_keystore[i].flags &
                 SEC_PVTKEY_SWITCH_FLAG_MATCHALL) &&
                (installed_keystore[i].flags &
                 SEC_PVTKEY_SWITCH_FLAG_ENABLED)) {
#ifdef DEBUG
                    dce_svc_printf(SEC_PSM_I_DEF_KEYSTORE_MSG, 
                                   username, 
                                   i,
                                   installed_keystore[i].keystore_name);
#endif
                    *keystore = &installed_keystore[i];
                    sec_pvtkey__unlock_keystore_list();
                    return 0;
            };

            if (installed_keystore[i].flags &
                SEC_PVTKEY_SWITCH_FLAG_TERMINATE) break;

        };
#ifdef DEBUG
        dce_svc_printf(SEC_PSM_I_NO_KEYSTORE_MSG, username);
#endif

        sec_pvtkey__unlock_keystore_list();
        return sec_psm_keystore_not_found;

    };


    if (era_st == 0) {
/*
 * We found an ERA.  Search the keystore list for it.
 */
#ifdef DEBUG
	{ 
             unsigned char * keystore_uuid_string = NULL;
             unsigned32 stat;
             uuid_to_string(&keystore_id,
                            &keystore_uuid_string,
                            &stat);
             dce_svc_printf(SEC_PSM_I_SEARCHING_EXPLICIT_MSG, 
                            username, keystore_uuid_string);
             rpc_string_free(&keystore_uuid_string, &stat);
        };
#endif
        for (i=0; i< MAX_KEYSTORES; i++) {

            if (installed_keystore[i].magic != SEC_PVTKEY_KEYSTORE_MAGIC) {

                dce_svc_printf(SEC_PSM_I_BAD_KEYSTORE_LIST_MSG, i);

                sec_pvtkey__unlock_keystore_list();
                return sec_psm_keystore_list_corrupt;
            };

            if (installed_keystore[i].flags &
                SEC_PVTKEY_SWITCH_FLAG_ENABLED) {

                matched = uuid_equal(installed_keystore[i].keystore_uuid,
                                     &keystore_id,
                                     &st);
		if (st != 0) {
                    dce_svc_printf(SEC_PSM_I_BAD_KEYSTORE_ID_MSG, 
                                   username, 
                                   i,
				   installed_keystore[i].keystore_name);
                };
                if (matched) {
#ifdef DEBUG
                    dce_svc_printf(SEC_PSM_I_CHOSEN_KEYSTORE_MSG, 
                                   username, 
                                   i,
				   installed_keystore[i].keystore_name);
#endif
                    *keystore = &installed_keystore[i];
                    sec_pvtkey__unlock_keystore_list();
                    return 0;
#ifdef DEBUG
                } else {
	           unsigned char * keystore_uuid_string = NULL;
                   unsigned32 stat;
	           uuid_to_string(installed_keystore[i].keystore_uuid,
                                  &keystore_uuid_string,
                                  &stat);
		    dce_svc_printf(SEC_PSM_I_DISCARDED_KEYSTORE_MSG,
                                   username, 
                                   i,
				   installed_keystore[i].keystore_name,
                                   keystore_uuid_string);
                   rpc_string_free(&keystore_uuid_string, &stat);
#endif
		};
 
            };

            if (installed_keystore[i].flags &
                SEC_PVTKEY_SWITCH_FLAG_TERMINATE) break;

        };
    };

/*
 * Either we didn't find an ERA, or we didn't find a matching keystore.
 * Perform last-chance search for MATCHALL keystore.
 */


    for (i=0; i< MAX_KEYSTORES; i++) {
#ifdef DEBUG
        dce_svc_printf(SEC_PSM_I_SEARCHING_MATCHALL_MSG, username);
#endif

	if (installed_keystore[i].magic != SEC_PVTKEY_KEYSTORE_MAGIC) {

            dce_svc_printf(SEC_PSM_I_BAD_KEYSTORE_LIST_MSG, i);

            sec_pvtkey__unlock_keystore_list();
	    return sec_psm_keystore_list_corrupt;
	};

	if (installed_keystore[i].flags &
	    SEC_PVTKEY_SWITCH_FLAG_ENABLED) {

            if (installed_keystore[i].flags &
                SEC_PVTKEY_SWITCH_FLAG_MATCHALL) {
#ifdef DEBUG
                dce_svc_printf(SEC_PSM_I_MATCHALL_KEYSTORE_MSG, 
                               username, 
                               i,
                               installed_keystore[i].keystore_name);
#endif
	        *keystore = &installed_keystore[i];
                sec_pvtkey__unlock_keystore_list();
	        return 0;
#ifdef DEBUG
            } else {
		    dce_svc_printf(SEC_PSM_I_DISCARDED_KEYSTORE_MSG,
                                   username, 
                                   i,
				   installed_keystore[i].keystore_name);
#endif
            };

	};

	if (installed_keystore[i].flags &
	    SEC_PVTKEY_SWITCH_FLAG_TERMINATE) break;

    };

#ifdef DEBUG
    dce_svc_printf(SEC_PSM_I_NO_KEYSTORE_MSG, username);
#endif

    sec_pvtkey__unlock_keystore_list();
    return sec_psm_keystore_not_found;
}

unsigned32 sec_pvtkey__register_keystore(
    const uuid_t * keystore_uuid, 
                            /* The UUID to which this pointer points        */
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
) {

    error_status_t status = 0;
    int i;
    boolean32 matched;
    keystore_epv * keystore = NULL;
    int terminate = 0;
    unsigned32 st;

    sec_psm__init_svc();

    sec_pvtkey__lock_keystore_list();

/* First, find where to put the new keystore... */
    for (i=0; i< MAX_KEYSTORES; i++) {
	if (installed_keystore[i].magic != SEC_PVTKEY_KEYSTORE_MAGIC) {

            dce_svc_printf(SEC_PSM_I_BAD_KEYSTORE_LIST_MSG, i);

	    status = sec_psm_keystore_list_corrupt;
	    goto end;
	};

        matched = uuid_equal(installed_keystore[i].keystore_uuid,
                             keystore_uuid,
                             &st);
        if (st != 0) {
            dce_svc_printf(SEC_PSM_I_BAD_KEYSTORE_ID1_MSG, 
                           i,
                           installed_keystore[i].keystore_name);
	    status = sec_psm_keystore_list_corrupt;
	    goto end;
        };
        if (matched) {
/* We've found a pre-existing implementation of this keystore.  We should */
/* either replace it, or fail, depending on the setting of 'replace'      */
	    if (replace) {
		terminate = installed_keystore[i].flags 
                            & SEC_PVTKEY_SWITCH_FLAG_TERMINATE;
                break;
            } else {
		status = sec_psm_keystore_already_registered;
		goto end;
            };
	};

/* If we get here, the keystore isn't one we care about.  Have we reached */
/* the end of the list yet?                                               */


	if (installed_keystore[i].flags &
	    SEC_PVTKEY_SWITCH_FLAG_TERMINATE) {
            if (i < MAX_KEYSTORES-1) {
                installed_keystore[i].flags &= 
                             ~SEC_PVTKEY_SWITCH_FLAG_TERMINATE;
	    };
	    terminate = 1;
	    i++;
	    break;
        };
    };    

    if (i >= MAX_KEYSTORES) {
/* We don't have any room left for a new keystore.  Fail.                   */
        status = sec_psm_keystore_list_full;
        goto end;
    };

/* Now we know where we want to put the new keystore.  Go ahead and do it. */

    keystore = &installed_keystore[i];

    keystore->magic = SEC_PVTKEY_KEYSTORE_MAGIC;

    keystore->flags = 0;
    if (enabled) keystore->flags |= SEC_PVTKEY_SWITCH_FLAG_ENABLED;
    if (terminate) keystore->flags |= SEC_PVTKEY_SWITCH_FLAG_TERMINATE;
    if (make_default) {
        /* CHFts20483: unset any previous default mechanisms when
         * a new default is stored
         */
        for (i=0; i < MAX_KEYSTORES; i++) {
            installed_keystore[i].flags ^= SEC_PVTKEY_SWITCH_FLAG_MATCHALL;
            if (installed_keystore[i].flags & SEC_PVTKEY_SWITCH_FLAG_TERMINATE)
                break;
        }
        keystore->flags |= SEC_PVTKEY_SWITCH_FLAG_MATCHALL;
    }

    strncpy(&keystore->keystore_name[0],
            keystore_name,
            sizeof(keystore->keystore_name[0]));
    keystore->keystore_uuid=keystore_uuid;

    keystore->open = open;
    keystore->close = close;
    keystore->capabilities = capabilities;
    keystore->store = store;
    keystore->delete = delete;
    keystore->generate = generate;
    keystore->get = get;
    keystore->update = update;
    keystore->update_generate = update_generate;

    status = 0;

end:
    sec_pvtkey__unlock_keystore_list();
    return status;

}
