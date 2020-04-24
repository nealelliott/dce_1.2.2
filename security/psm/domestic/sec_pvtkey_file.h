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
 * $Log: sec_pvtkey_file.h,v $
 * Revision 1.1.2.1  1996/10/04  14:26:34  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  14:02:11  arvind]
 *
 * $EndLog$
 */
/*
 *
 * Header file for HP File-based Private-Key Store.
 * Created 8/21/96, JCW (Digital) as part of keystore switch work.
 *
 */

#ifndef SEC_PVTKEY_FILE_INCLUDE_
#define SEC_PVTKEY_FILE_INCLUDE_

#include <fcntl.h>
#include <sys/stat.h>
#include <rgymacro.h>
#include <dce/dce_cf_const.h>
#include <dce/rgynbase.h>
#include <dce/binding.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pvtkey.h>
#include <dce/sec_pvtkey_base.h>
#include <sec_pk.h>
#include <sec_bsafe.h>


error_status_t sec_pvtkey_file_open(
    idl_char                  *name,
    sec_pk_domain_t           *domain_id,
    sec_pk_mechanism_handle_t *handle
);

error_status_t sec_pvtkey_file_close(
    sec_pk_mechanism_handle_t        handle  /* in */
);

error_status_t sec_pvtkey_file_store(
    sec_pk_mechanism_handle_t   handle,         /* in */
    idl_char                   	*pwd,           /* in */
    sec_pk_usage_flags_t  	key_usage,     /* in */
    unsigned32             	key_vno,        /* in */
    sec_pk_data_t       	*private_key,   /* in */
    sec_pk_data_t               *public_key     /* in, ignored in reference  */
);

error_status_t sec_pvtkey_file_get(
    sec_pk_mechanism_handle_t handle,                   /* in */
    idl_char                  *pwd,                     /* in */
    sec_pk_usage_flags_t      key_usage,                /* in */
    unsigned32                *key_vno,                 /* in, out */
    sec_pk_data_t             *private_key,             /* out */
    sec_pk_data_t             *public_key               /* out */
);

error_status_t sec_pvtkey_file_update(
    sec_pk_mechanism_handle_t handle,                    /* in */
    idl_char                  *current_pwd,              /* in */
    idl_char                  *new_pwd,                  /* in */
    sec_pk_usage_flags_t      key_usage,                /* in */
    unsigned32                *key_vno,                  /* in, out */ 
    sec_pk_data_t 	      *private_key,              /* in */ 
    sec_pk_data_t             *public_key                /* in */
);

error_status_t sec_pvtkey_file_capabilities(
    sec_pk_mechanism_handle_t handle,                  /* in */
    void                      *capability_parameter,   /* in */
    sec_pk_pvtkey_capability  *feature,                /* in */
    boolean32                 *supported               /* out */
);

error_status_t sec_pvtkey_file_delete(
    sec_pk_mechanism_handle_t   handle,                  /* in */
    idl_char                   	*pwd,                    /* in */
    sec_pk_usage_flags_t  	key_usage,              /* in */
    unsigned32             	key_vno                  /* in */
);

error_status_t sec_pvtkey_file_generate(
    sec_pk_mechanism_handle_t   handle,                  /* in */
    idl_char                   	*pwd,                    /* in */
    sec_pk_algorithm_id_t       *alg_id,                 /* in */
    unsigned32                  key_length,              /* in */
    sec_pk_usage_flags_t  	key_usage,               /* in */
    unsigned32             	key_vno,                 /* in */
    sec_pk_data_t       	*public_key             /* out */
);

error_status_t sec_pvtkey_file_update_generate(
    sec_pk_mechanism_handle_t handle,                    /* in */
    idl_char                  *current_pwd,              /* in */
    idl_char                  *new_pwd,                  /* in */
    sec_pk_algorithm_id_t     *alg_id,                   /* in */
    unsigned32                key_length,                /* in */
    sec_pk_usage_flags_t      key_usage,                /* in */
    unsigned32                *key_vno,                  /* in */
    sec_pk_data_t             *public_key                /* out */
);

#endif
