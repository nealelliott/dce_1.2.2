/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: seckey_util.h,v $
 * Revision 1.1.8.2  1996/02/18  23:01:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:54  marty]
 *
 * Revision 1.1.8.1  1995/12/08  17:30:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:30  root]
 * 
 * Revision 1.1.4.4  1993/02/10  19:21:53  burati
 * 	CR7182 Add update param to sec_key_mgmt__bind_rgy
 * 	[1993/02/10  18:38:08  burati]
 * 
 * Revision 1.1.4.3  1993/02/08  19:44:22  sommerfeld
 * 	[OT7153] Fix status code conversion.
 * 	[1993/02/08  18:30:40  sommerfeld]
 * 
 * Revision 1.1.4.2  1992/12/29  13:27:57  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:47:21  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/26  20:57:33  pato
 * 	OT#3620 default RPC key retrieval should not need keytab type prefix.
 * 	OT#3196 SEC_KEY_CHECK_AUTHN was returning not_implemented for bad
 * 	auth type instead of sec_key_mgmt_e_authn_invalid
 * 	[1992/05/22  13:57:57  pato]
 * 
 * Revision 1.1  1992/01/19  14:43:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
** Unpublished work. All Rights Reserved.
*/

/* 
 *  DCE Security key management interface - internal definitions
 *
 * $ID$
 */

#ifdef _SEC_KEY_ALLOCATE_GLOBALS_
#   define EXTERN
#   define INITVAL(X) X
#else
#   define EXTERN extern
#   define INITVAL(X)
#endif

#include <macros.h>
#include <dce/rgynbase.h>
#include <dce/binding.h>
#include <dce/sec_authn.h>
#include <krb5/krb5.h>
#include <krb5/func-proto.h>
#include <sec_krb_util.h>
#include <dce/keymgmt.h>

EXTERN boolean32            sec_key__inited INITVAL(= false);
EXTERN krb5_keyblock        sec_key__master_keyblock;
EXTERN krb5_encrypt_block   sec_key__master_encblock;

#define master_keyblock sec_key__master_keyblock
#define master_encblock sec_key__master_encblock


#define SEC_KEY_CHECK_INIT(ST) \
    if (!sec_key__inited) { \
        sec_key_mgmt__init(ST); \
    }

#define SEC_KEY_CHECK_AUTHN(AUTH, ST) \
    if (AUTH != rpc_c_authn_dce_secret) { \
        SET_STATUS(ST, sec_key_mgmt_e_authn_invalid); \
        return; \
    }

/*
 * Function Prototypes
 */

void sec_key_mgmt__init (
#ifdef __STDC__
    error_status_t  *st
#endif
);

boolean32 sec_key_mgmt__resolve_mode (
#ifdef __STDC__
    void            * arg,
    boolean32       readonly,
    krb5_keytab     * ktid,
    error_status_t  *st
#endif
);

boolean32 sec_key_mgmt__resolve (
#ifdef __STDC__
    void            * arg,
    krb5_keytab     * ktid,
    error_status_t  *st
#endif
);

void sec_key_mgmt__bind_rgy (
#ifdef __STDC__
    unsigned char                   * principal_name,
    sec_rgy_bind_auth_info_type_t   auth_type,
    sec_login_handle_t              lc,
    sec_rgy_name_t                  rel_princ_name,
    unsigned char                   * cell,
    boolean32                       update,
    sec_rgy_handle_t                * rgy_h,
    error_status_t                  * st
#endif
);

void sec_key_mgmt_get_key_type (
#ifdef __STDC__
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    sec_passwd_type_t           keytype,
    unsigned32                  key_vno,
    void                        ** keydata,
    error_status_t              * err 
#endif
);

void sec_key_mgmt_get_nth_key_internal (
#ifdef __STDC__
    sec_key_mgmt_authn_service  authn_service, 
    void                        * arg,
    unsigned char               * principal_name,
    signed32                    index,
    sec_passwd_type_t           keytype,
    unsigned32                  * key_vno,
    sec_passwd_rec_t            * keydata,
    unsigned char               ** name_out,
    error_status_t              * err 
#endif
);

/*
 * convert status code from that returned by keytab into something
 * acceptable for use above the API.
 */
error_status_t sec_key_mgmt__convert_status (
#ifdef __STDC__
    error_status_t		err
#endif
);
