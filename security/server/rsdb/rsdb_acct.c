/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_acct.c,v $
 * Revision 1.1.32.5  1996/11/13  18:11:43  arvind
 * 	Implement bypass for attribute fetch from sec_pvtkey_switch.
 * 	[1996/11/05  02:16 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
 *
 * 	CHFts20483: in rsdb_acct_register_pvtkey_mechanism(), register
 * 	mechanism sec_psm_kdc_pk_init_mechanism as the default.  This will
 * 	cause the KDC call to sec_psm_open to pick up this mechanism even if
 * 	the krbtgt/<cell> principal does not yet have a key-pair or an
 * 	instance of the DCEPKPrivateKeyStorage ERA.
 * 	[1996/11/04  17:07 UTC  aha  /main/DCE_1.2.2/aha_pk11/1]
 *
 * 	Register public key domain sec_pk_domain_kdc_pk_init in
 * 	rsdb_acct_register_pvtkey_mechanism()
 * 	[1996/10/17  14:37 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * Revision 1.1.32.4  1996/10/15  20:56:40  arvind
 * 	CHFts20188: null password crumps secd.
 * 	[1996/10/09  18:35 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision 1.1.32.3  1996/10/04  16:49:34  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:49:15  arvind]
 * 
 * Revision /main/DCE_1.2.2/3  1996/10/02  18:28 UTC  cuti
 * 	Merge of cuti_pk_fix
 * 
 * Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/09/26  16:19 UTC  cuti
 * 	Make international build work
 * 
 * Revision /main/DCE_1.2.2/2  1996/09/16  20:29 UTC  aha
 * 	CHFts19896: public key version support
 * 
 * Revision /main/DCE_1.2.2/aha_pk9_3/1  1996/09/13  23:43 UTC  aha
 * 	Change sec_passwd_privkey -> sec_passwd_pubkey
 * 	[1996/09/10  21:07 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/3]
 * 
 * 	Add rsdb_acct_get_curr_version function to return current key
 * 	version
 * 	[1996/09/06  18:19 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 * 
 * 	Merge up from aha_pk8_1
 * 	[1996/08/29  15:41 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 * 
 * 	Merge up from aha_pk8
 * 	[1996/08/27  22:25 UTC  aha  /main/DCE_1.2.2/aha_pk8_1/1]
 * 
 * 	Changes for Public Key Login; support KDC retrieval of the KDC
 * 	digital signature private key via sec_psm.
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/07/18  17:33 UTC  aha
 * 	Merge Registry support for KDC private key storage
 * 
 * Revision /main/aha_pk6/4  1996/07/15  15:17 UTC  aha
 * 	Changes for Public Key Login; support Registry storage of KDC private key:
 * 	verify that version number for a sec_passwd_[gen]privkey is 1.
 * 
 * Revision /main/aha_pk6/3  1996/07/13  20:49 UTC  aha
 * 	Merge-out and bugfixes and Registry work
 * 
 * Revision /main/aha_pk6/2  1996/06/18  20:55 UTC  aha
 * 	Changes for Public Key Login; support Registry storage of KDC private key.
 * 
 * Revision /main/aha_pk6/1  1996/06/07  19:06 UTC  aha
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:43  marty]
 * 	 *
 * 
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/07/21  19:44 UTC  burati
 * 	Merge pgo copy fix from mb_mothra5 (& some cleanup of formatting errors)
 * 
 * 	HP revision /main/HPDCE02/mb_mothra5/1  1995/07/18  21:42 UTC  burati
 * 	Fix copying of person rsdb_named_item_t
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/15  21:10 UTC  greg
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/27  20:09 UTC  mdf
 * 	Fix CHFts14216
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_2/1  1995/01/26  18:23 UTC  mdf
 * 	Previous change to return status would break binary compatablity
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/15  03:28 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/15  00:32 UTC  rps
 * 	[OT12669] cell renaming
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/06  17:51 UTC  mdf
 * 	Fix return status to match man page and test suite.
 * 
 * 	HP revision /main/mdf_MOTHRA_0/1  1994/12/06  17:45 UTC  mdf
 * 	Fix error return for rsdb_acct_rename.
 * 	[1995/12/08  17:21:57  root]
 * 
 * Revision /main/DCE_1.2/aha_pk5/1  1996/06/05  16:14 UTC  aha
 * 	Changes for Public Key Login; support Registry storage of KDC private key
 * 
 * Revision /main/DCE_1.2/1  1996/01/03  19:18 UTC  psn
 * 	Merge pgo copy fix from mb_mothra5 (& some cleanup of formatting errors)
 * 	[1995/07/21  19:44 UTC  burati  /main/HPDCE02/5]
 * 
 * 	Fix copying of person rsdb_named_item_t
 * 	[1995/07/18  21:42 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	Fix CHFts14216
 * 	[1995/01/27  20:09 UTC  mdf  /main/HPDCE02/3]
 * 
 * 	Previous change to return status would break binary compatablity
 * 	[1995/01/26  18:23 UTC  mdf  /main/HPDCE02/mdf_MOTHRA_2/1]
 * 
 * 	[1994/12/15  03:28 UTC  rps  /main/HPDCE02/2]
 * 
 * 	[OT12669] cell renaming
 * 	[1994/12/15  00:32 UTC  rps  /main/rps_mothra/1]
 * 
 * 	Fix return status to match man page and test suite.
 * 	[1994/12/06  17:51 UTC  mdf  /main/HPDCE02/1]
 * 
 * 	Fix error return for rsdb_acct_rename.
 * 	[1994/12/06  17:45 UTC  mdf  /main/mdf_MOTHRA_0/1]
 * 
 * Revision 1.1.21.7  1994/09/21  20:32:50  max
 * 	CR 11961 fix.  In create_new_account fill passwd version OUT parm with
 * 	initial version.
 * 	[1994/09/21  20:32:25  max]
 * 
 * Revision 1.1.21.6  1994/08/22  17:03:37  max
 * 	Added crypt prototype for RIOS.  Eliminated unnecessary cast of crypt
 * 	return value.  Included <unistd.h> header file.
 * 	[1994/08/22  16:10:12  max]
 * 
 * Revision 1.1.21.5  1994/08/16  18:28:57  baum
 * 	OT 10575 - lookup_acct_id() status and return value should track one another
 * 	[1994/08/16  18:28:43  baum]
 * 
 * Revision 1.1.21.4  1994/08/08  14:23:37  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:24:28  mdf]
 * 
 * Revision 1.1.21.1  1994/04/22  19:14:04  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:17:42  rps]
 * 
 * Revision 1.1.19.2  94/03/29  18:42:40  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.18.1  1993/10/07  13:12:26  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/07  13:08:56  mccann]
 * 
 * Revision 1.1.16.2  1993/08/04  19:54:41  cuti
 * 	Change gen_unix_key to be non-ASCII centered.
 * 	[1993/08/04  18:25:04  cuti]
 * 
 * Revision 1.1.8.13  1993/03/31  22:55:26  hanfei
 * 	default pepper flag should be set to be true only if pepper pointer is NULL.
 * 	[1993/03/31  22:54:18  hanfei]
 * 
 * Revision 1.1.8.12  1993/03/25  21:31:48  burati
 * 	CR7509 Fix bad pepper_len calculation in rsdb_acct_key_get_next
 * 	[1993/03/25  01:10:30  burati]
 * 
 * Revision 1.1.8.11  1993/03/19  22:21:19  emartin
 * 	CR 7472  rsdb_acct_replace - takes gen_unix_passwd arg
 * 	[1993/03/19  18:54:22  emartin]
 * 
 * Revision 1.1.8.10  1993/03/11  15:14:00  emartin
 * 	7463 rsdb_acct_add_key_version - store account
 * 	[1993/03/11  15:10:24  emartin]
 * 
 * Revision 1.1.8.9  1993/02/19  16:20:08  ahop
 * 	7191: make gen_default_pepper PUBLIC for use by setup_locksmith
 * 	[1993/02/19  03:53:05  ahop]
 * 
 * Revision 1.1.8.8  1993/02/03  18:46:42  ahop
 * 	7085: generate and return default_pepper when appropriate in key_get_next
 * 	[1993/02/02  19:17:01  ahop]
 * 
 * Revision 1.1.8.7  1993/01/21  19:33:52  sommerfeld
 * 	Fix handling of zero-length "pepper": zero length pepper is *NOT* default.
 * 	[1993/01/21  03:47:59  sommerfeld]
 * 
 * Revision 1.1.8.6  1992/12/29  16:38:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:51  zeliff]
 * 
 * Revision 1.1.8.5  1992/10/19  15:28:19  burati
 * 	CR5134 secd should not allow keys with bad parity
 * 	[1992/10/16  22:46:37  burati]
 * 
 * 	CR5682 Make sure rsdb_acct_replace passes back new passwd version
 * 	[1992/10/16  20:54:25  burati]
 * 
 * Revision 1.1.8.4  1992/10/13  19:42:18  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:14:50  marty]
 * 
 * Revision 1.1.10.2  1992/10/01  20:48:17  garyf
 * 	conditionalize out crypt() prototype for OSF/1
 * 	[1992/09/29  20:11:04  garyf]
 * 
 * Revision 1.1.8.3  1992/10/07  20:37:56  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:20:11  sekhar]
 * 
 * Revision 1.1.9.2  1992/10/06  16:02:55  burati
 * 	Second replication drop: bl5
 * 	Incorporate rs_mkey calls.
 * 	Fix bug in rs_pgo_rename.
 * 	4790: fix cursor checking in get_projlist
 * 
 * Revision 1.1.8.2  1992/09/29  20:13:44  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:39:28  sekhar]
 * 
 * Revision 1.1.4.3  1992/07/13  20:03:29  ahop
 * 	typo in signature
 * 	[1992/07/01  18:14:07  ahop]
 * 
 * 	 3903: Add rsdb_acct_store_default_pepper, add pepper to get_auth_key params
 * 	[1992/06/30  22:33:36  ahop]
 * 
 * Revision 1.1.4.2  1992/06/08  15:57:34  burati
 * 	CR3910 Don't prohibit primary group from being included in projlist
 * 	[1992/06/05  20:50:58  burati]
 * 
 * Revision 1.1  1992/01/19  14:44:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1995
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/* 
 *      Registry Server - database interlude - Account management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$ID: $";
#endif


#include <unistd.h>
#include <stdio.h>
#include <un_strng.h>
#include <un_time.h>
#include <un_maloc.h>


#ifdef _AIX
/* RIOS without crypt proto in unistd.h */
char    *crypt (const char*,const char*);
#endif


#include <dce/dce.h>
#include <dce/id_base.h>

#include <rs_base.h>
#include <rs_util.h>
#include <rs_mkey.h>

#include <rsdb.h>
#include <rsdb_acct.h>
#include <rsdb_auth.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <rsdb_pgo.h>
#include <rsdb_pgo_util.h>
#include <rsdb_util.h>
#include <rsdb_policy.h>

#include <bstring.h>
#include <sec_krb_util.h>

/* Kerberos header files */
#include <krb5/kdb.h>  
#include <krb5/des_int.h>
#include <krb5/encryption.h>

#include <sec_svc.h>
#include <dce/sec_psm_base.h>
#include <dce/sec_pvtkey_base.h>
#include <dce/sec_pk.h>
#include <dce/passwd.h>
#include <dce/idlbase.h>
#include <dce/dce_cf.h>
#include <rgymacro.h>

/*
 * Local Global Variables
 */
static rsdb_acct_item_t    first;
static rsdb_acct_item_t    last;

/* Forward declarations */
PRIVATE error_status_t rsdb_acct_pvtkey_open(
    idl_char                  *name,      /* [in] */
    sec_pk_domain_t           *domain_id, /* [in] */
    sec_pk_mechanism_handle_t *handle     /* [out] */
);

PRIVATE error_status_t rsdb_acct_pvtkey_close(
    sec_pk_mechanism_handle_t handle /* [in] */
);

PRIVATE error_status_t rsdb_acct_pvtkey_capabilities(
    sec_pk_mechanism_handle_t handle,               /* [in] */
    idl_void_p_t              capability_parameter, /* [in] */
    sec_pk_pvtkey_capability  *feature,             /* [in] */
    boolean32                 *supported            /* [out] */
);

PRIVATE error_status_t rsdb_acct_pvtkey_get(
    sec_pk_mechanism_handle_t handle,       /* [in] */
    idl_char                  *pwd,         /* [in] */
    sec_pk_usage_flags_t      key_usage,    /* [in] */
    unsigned32                *key_vno,     /* [in, out] */
    sec_pk_data_t             *private_key, /* [out] */
    sec_pk_data_t             *public_key   /* [out] */
);

PRIVATE void purge_acct_by_id ( rsdb_pvt_id_t  id )
{
    rsdb_sequential_key_t  sequential_key;
    error_status_t         st;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_delete(rsdb_acct_db, (Pointer) &sequential_key,
                sizeof(sequential_key), &st);
}


/* 
 *  r s d b _ a c c t _ g e n _ d e f a u l t _ p e p p e r
 *
 *  Use principal name 'pname' and the default pepper algorithm
 *  to generate a pepper.
 *
 *  Caller must free pepper->data.
 */
PUBLIC void rsdb_acct_gen_default_pepper
  (
    sec_rgy_name_t              pname,
    krb5_data                   *pepper,
    error_status_t              *status
  )
{
    krb5_principal          principal = NULL; 
    char                    princ_name[(2 * sec_rgy_name_t_size)];
    char                    *realm;
    sec_rgy_properties_t    properties;

    CLEAR_STATUS(status);

    /* Use default pepper algorithm to generate pepper.
     * Get local realm to construct krb5 principal name.
     * Krb5_parse_name expects person_name@realm_name as input string. 
     * Don't store pepper.  It can be recreated with
     * default pepper algorithm. 
     */
    rsdb_properties_get_info( &properties, status );
    if (BAD_STATUS(status))
        return;

    /* Advance past the global directory root */
    realm = (char *)properties.realm;
    if (strncmp(GLOBAL_DIR_ROOT, realm, GLOBAL_DIR_ROOT_LEN) == 0)
        realm = realm + GLOBAL_DIR_ROOT_LEN;

    u_strcpy(princ_name, pname);
    u_strcat(princ_name, "@");
    u_strcat(princ_name, realm);
    *status = krb5_parse_name(princ_name, &principal);
    if(BAD_STATUS(status)) {
        return;
    }
    *status = krb5_principal2salt (principal, pepper);

    if (principal)
        krb5_free_principal(principal);

    return;
}


PUBLIC void rsdb_acct_create( void )
{
    bzero((Pointer) &first, sizeof(first));
    first.threads.id = START_OF_LIST;
    first.threads.next = END_OF_LIST;
    first.threads.prev = END_OF_LIST;

    store_acct_by_id(first.threads.id, &first);

    bzero((Pointer) &last, sizeof(last));
    last.threads.id = END_OF_LIST;
    last.threads.next = START_OF_LIST;
    last.threads.prev = START_OF_LIST;

    store_acct_by_id(last.threads.id, &last);
}


PUBLIC void rsdb_acct_init( void )
{
    (void) rsdb_get_acct_by_id(START_OF_LIST, &first);
    (void) rsdb_get_acct_by_id(END_OF_LIST, &last);
}

PRIVATE error_status_t rsdb_acct_pvtkey_open(
    idl_char                  *name,      /* [in] */
    sec_pk_domain_t           *domain_id, /* [in] */
    sec_pk_mechanism_handle_t *handle     /* [out] */
)
{
    *handle=NULL;
    return error_status_ok;
}

PRIVATE error_status_t rsdb_acct_pvtkey_close(
    sec_pk_mechanism_handle_t handle /* [in] */
)
{
    return error_status_ok;
}

PRIVATE error_status_t rsdb_acct_pvtkey_capabilities(
    sec_pk_mechanism_handle_t handle,               /* [in] */
    idl_void_p_t              capability_parameter, /* [in] */
    sec_pk_pvtkey_capability  *feature,             /* [in] */
    boolean32                 *supported            /* [out] */
)
{
    *supported=false;
    return error_status_ok;
}

/* rsdb_acct_pvtkey_get
 *
 * registered with sec_psm as a sec_pvtkey interface provider
 * for the sec_pvtkey_get() function.
 *
 * XXX-future optimizations: call rsdb_acct_lookup_acct_id() in pvtkey_open,
 * and include acct_id in the handle.  Call rsdb_auth_get_by_authkey_key()
 * directly from here, then rs_mkey_decrypt_with_master().
 */
PRIVATE error_status_t rsdb_acct_pvtkey_get(
    sec_pk_mechanism_handle_t handle,       /* [in] */
    idl_char                  *pwd,         /* [in] */
    sec_pk_usage_flags_t      key_usage,    /* [in] */
    unsigned32                *key_vno,     /* [in, out] */
    sec_pk_data_t             *private_key, /* [out] */
    sec_pk_data_t             *public_key   /* [out] */
)
{
    sec_rgy_login_name_t      login_name;
    krb5_keyblock             *auth_key_p=NULL;
    boolean32                 default_pepper;
    krb5_kvno                 mkey_version;
    char                      *cell_name=NULL;
    unsigned32                cur_key_vno;
    error_status_t            st=error_status_ok;

    /* Initialize return parameters */
    if (public_key)
        sec_pk_data_init(public_key);
    if (private_key)
        sec_pk_data_init(private_key);

    /* Look up the private key */
    dce_cf_get_cell_name(&cell_name, &st);
    if (GOOD_STATUS(&st))
    {
        if ((KRB_DIR_ROOT_LEN + strlen(cell_name) - GLOBAL_DIR_ROOT_LEN) > sizeof(login_name.pname))
        {
            st=sec_id_e_name_too_long;
        }
        else
        {
            CLEAR_LOGIN_NAME(&login_name);
            u_strcpy(login_name.pname, KRB_DIR_ROOT);
            u_strcat(login_name.pname, cell_name + GLOBAL_DIR_ROOT_LEN);

            /* look up current private key version */
            rsdb_acct_get_cur_key_version(&login_name,
                                          sec_passwd_pubkey,
                                          &cur_key_vno,
                                          &st);
            if (cur_key_vno == 0)
                /* No krbtgt key currently stored */
                return sec_pk_e_key_vers_unavailable;

            if (key_vno) {
                if ((*key_vno != 0) && (*key_vno != cur_key_vno))
                    return sec_pk_e_key_vers_unavailable;
                else if (*key_vno == 0)
                    *key_vno=cur_key_vno;
            }

            rsdb_acct_get_auth_key_by_name(login_name.pname,
                                           NULL,
                                           KEYTYPE_ENCRYPT_RSA_PRIV,
                                           cur_key_vno,
                                           &auth_key_p,
                                           &default_pepper,
                                           NULL,
                                           &mkey_version,
                                           &st);
        }
    }
    if (STATUS_OK(&st) && private_key)
    {
        private_key->data=auth_key_p->contents;
        private_key->len=auth_key_p->length;
        auth_key_p->length=0;
        auth_key_p->contents=NULL;
    }

    if (cell_name)
        free(cell_name);

    if (auth_key_p)
    {
        if (auth_key_p->contents)
            free(auth_key_p->contents);
        free(auth_key_p);
    }
    return st;
}

/*
 * rsdb_acct_pubkey_get
 *
 * public key retrieval function associated with
 * domain sec_pk_domain_kdc_pk_init;
 * this is merely a placeholder, since this
 * domain should never be used for public key
 * retrieval or storage.
 */
PRIVATE error_status_t rsdb_acct_pubkey_get(
    void *principal,
    sec_pk_domain_t *domain,
    sec_pk_usage_flags_t usage,
    unsigned32 *kvno,
    sec_pk_pubkey_t *pubkey_p
)
{
    /* This is a placeholder, since this domain
     * should never be used for public key
     * retrieval or storage; sec_pubkey_define_domain()
     * requires that entries have either a non-NULL
     * retrieve or a non-NULL store function.
     */
    if (pubkey_p)
        sec_pk_data_init(pubkey_p);
    return sec_pk_e_domain_unsupported;
}
    
PRIVATE error_status_t rsdb_acct_pvtkey_era_fetch (
    unsigned char *princ,
    const uuid_t *key_uuid,
    uuid_t *out_uuid
) 
{
    error_status_t status;
    sec_attr_t returned_attr;
    uuid_t obj_uuid;
    sec_rgy_login_name_t login_name;
    sec_rgy_name_t fullname;
    rsdb_util_rgy_obj_info_t obj_info;

    uuid_create_nil(out_uuid, &status);

    rsdb_name_util_complete_name(sec_rgy_domain_person, princ,
				 fullname, &status);

    if (status)
	return status;

    /* find the object in the database... */
    rsdb_util_get_obj_info_by_name(fullname, rsdb_pgo_type,
				   sec_acl_type_object, &obj_info, 
				   &obj_uuid, &status);

    if (status)
	return status;

    rsdb_attr_lookup1_by_uuid (&obj_info, key_uuid,
	rpc_ss_allocate, &returned_attr, &status);

    if (status)
	return status;

    /* this is "bug for bug" compatible with the remote interface.. */
    if (returned_attr.attr_value.attr_encoding != sec_attr_enc_uuid)
	return sec_pk_e_key_attr_read_failed;

    *out_uuid = returned_attr.attr_value.tagged_union.uuid;

    return error_status_ok;
}

/* rsdb_acct_register_pvtkey_mechanism()
 * 
 * Register the sec_psm_kdc_pk_init_mechanism and associated operations
 * with sec_psm as a private key retrieval mechanism.
 *
 * This mechanism is for use by the KDC in retrieving its own private key
 * as part of the pk-init authentication protocol.
 */
PUBLIC error_status_t rsdb_acct_register_pvtkey_mechanism(
)
{
    error_status_t        st;
    unsigned32            ks_err=0;
    sec_rgy_properties_t    cell_properties;

    CLEAR_STATUS(&st);

    rsdb_properties_get_info(&cell_properties, &st);
    if (BAD_STATUS(&st))
	return;

    if (ks_err)
        return sec_pk_e_rsdb_keystore_err;

    sec_pvtkey__era_fetch_bypass (cell_properties.realm,
				  rsdb_acct_pvtkey_era_fetch);
    
    st=sec_pubkey_define_domain(&sec_pk_domain_kdc_pk_init,
                                rsdb_acct_pubkey_get,
                                NULL);
    return st;
}

/*
 * r s d b _ a c c t _ g e t _ a u t h _ k e y _ b y _ n a m e 
 *
 * The current version of the "default" key type is stored with the backend
 * acct item proper.  Any other type of lookup (non-default type, non-current
 * version) requires an additional lookup. 
 * Note: strncpy's shouldn't be used to copy key data because the body
 * of encrypted keys may contain embedded NULLs.
 *
 * This call mallocs (*auth_key), (*auth_key)->contents, and 
 * pepper->data which must be freed by caller
 * after successful return of this call. 
 * Caller should memset (*auth_key)->contents to 0 before freeing.
 *
 */
PUBLIC boolean32 rsdb_acct_get_auth_key_by_name
  (
    sec_rgy_name_t      svr_pname,          /* [in] */
    char		*realm,		    /* [in] */
    unsigned32          key_type,           /* [in] */
    unsigned32          key_version,        /* [in] */
    krb5_keyblock       **auth_key,         /* [out] */
    boolean32           *default_pepper,    /* [out] */
    krb5_data           *pepper,            /* [out] */
    krb5_kvno           *mkey_version,      /* [out] */
    error_status_t      *st
  )
{
    sec_rgy_login_name_t    login_name;
    sec_rgy_cursor_t        cursor;
    sec_rgy_acct_key_t      name_parts;
    rsdb_named_item_t       person;
    rsdb_named_item_t       group;
    rsdb_named_item_t       org;
    rsdb_acct_item_t        account;
    rsdb_authkey_item_t     auth_item;
    long                    key_len;
    krb5_encrypted_keyblock ekey;
    boolean32               retval = true;

    CLEAR_STATUS(st);
    CLEAR_LOGIN_NAME(&login_name);
    ustrcpy(login_name.pname, svr_pname);
    cursor.valid = false;  
    *auth_key = NULL;
    ekey.contents = NULL;
    *default_pepper = true;
    if (pepper) {
	pepper->length = 0;
	pepper->data = NULL;
    }

    rsdb_acct_lookup_acct_id( &login_name, &cursor, &name_parts,
            &person, &group, &org, &account, st );
    if(BAD_STATUS(st)) {
        return false;
    }  
    /* If requested key is not the current key of the
     * default key_type, retrieve auth_key from database. 
     * Otherwise, use key stored in account record.
     */
    if((key_version != sec_passwd_c_version_none &&
        key_version != account.cur_key_version)  ||
        key_type != RSDB_AUTH_DEFAULT_KEY_TYPE) {
        if(!rsdb_auth_get_by_authkey_key(key_version, key_type,
            account.threads.id, &auth_item, st)) {
            return false;
        }
        key_len = auth_item.key_len;
    } else {
        key_len = account.key_len;
    }

    if(!(ekey.contents = (krb5_octet *)malloc(key_len))) {
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
        return false;
    }
    ekey.keytype = (krb5_keytype)key_type;
    ekey.length  = key_len;

    if((key_version != sec_passwd_c_version_none &&
        key_version != account.cur_key_version)  ||
        key_type != RSDB_AUTH_DEFAULT_KEY_TYPE) {
        bcopy( &(auth_item.buf[auth_item.pepper_len]), ekey.contents,
                 ekey.length);
        if(auth_item.default_pepper == false) {
            *default_pepper = false;
            if (pepper && (auth_item.pepper_len > 0)) {
                if(!(pepper->data = malloc(auth_item.pepper_len))) {
                    SET_STATUS(st, sec_rgy_cant_allocate_memory);
                    retval = false;
                    goto out;
                }
                pepper->length = auth_item.pepper_len;
                bcopy( auth_item.buf, pepper->data, pepper->length );
            }
        }
    } else {
        /* Here if current version requested.  Copy from acct_item. */
        bcopy(ACCT_KEY(&account), ekey.contents, ekey.length);
        if(account.default_pepper == false) {
            *default_pepper = false;
            if (pepper && (account.key_pepper_len > 0)) {
                if(!(pepper->data = malloc(account.key_pepper_len))) {
                    SET_STATUS(st, sec_rgy_cant_allocate_memory);
                    retval = false;
                    goto out;
                }
                pepper->length = account.key_pepper_len;
                bcopy( ACCT_PEPPER(&account), pepper->data, pepper->length );
            }
        }
    }

    /*
     * If the requested (primary) realm differs from the original
     * database cellname, the default pepper must be returned explicitly.
     * Passing NULL realm means "initiated locally"; must check for change.
     */
    if (pepper && (*default_pepper == true)) {
	char *prim_cellname = NULL;
	sec_rgy_name_t prop_realm;

	if (!realm) {
	    dce_cf_get_cell_name(&prim_cellname, st);
	    if (BAD_STATUS(st)) {
		retval = false;
		goto out;
	    }
	    realm = prim_cellname + GLOBAL_DIR_ROOT_LEN;
	}

	rsdb_properties_get_realm_name(prop_realm);

	if (u_strcmp(realm, prop_realm+GLOBAL_DIR_ROOT_LEN)) {
	    if (prim_cellname) {
		free(prim_cellname);
	    }

	    *default_pepper = false;
	    rsdb_acct_gen_default_pepper(svr_pname, pepper, st);
	    if (BAD_STATUS(st)) {
		retval = false;
		goto out;
	    }
	}
	else {
	    if (prim_cellname) {
		free(prim_cellname);
	    }
	}
    }

    if(!(*auth_key = (krb5_keyblock *)malloc(sizeof(krb5_keyblock)))) {
        SET_STATUS(st, sec_rgy_cant_allocate_memory);
        retval = false;
        goto err_out;
    }
    *mkey_version = account.mkey_version;
    rs_mkey_decrypt_with_master(account.mkey_version, &ekey, *auth_key, st);
    if(BAD_STATUS(st)) {
        retval = false;
        goto err_out;
    } else {
        goto out;
    }

err_out:
    if (*auth_key) {
        if((*auth_key)->contents) {
            memset((char *)(*auth_key)->contents, 0, (*auth_key)->length);
            free((char *)(*auth_key)->contents);  
        }
        free((char *)(*auth_key));
        *auth_key = NULL;
    }
    if (pepper && pepper->data) {
        free(pepper->data);
        pepper->length = 0; 
    }

out:
    if (ekey.contents) {
    memset((char *)ekey.contents, 0, ekey.length);
        free((char *)ekey.contents);  
    }
    return retval;
}


/*
 * r s d b _ a c c t _ s t o r e _ d e f a u l t _ p e p p e r
 *
 *  Motivation: When the pepper for an auth key is constructed using
 *  the default pepper algorithm (which depends on the principal name)
 *  the pepper is not stored since it can be reconstructed easily
 *  and the storage space saved is significant.
 *  However, after the principal name of an account changes, the default
 *  pepper algorithm can no longer be used to reconstruct the pepper.
 *
 *  This operation reconstructs the pepper using the default pepper 
 *  algorithm which is based on the principal name.  The pepper is
 *  stored so that a subsequent name change may take place without
 *  invalidating the auth_key for this account.
 *
 *  If no account exists for the input principal, return error_status_ok.
 *  If the account default_pepper flag is not set, do nothing -- the
 *  pepper is already stored.
 *
 *  Input:
 *  pname - principal name for whose account the pepper will be stored.
 *
 */
PUBLIC void rsdb_acct_store_default_pepper
  (
    sec_rgy_name_t      pname,  
    error_status_t      *st
  )
{
    sec_rgy_login_name_t    login_name;
    sec_rgy_cursor_t        cursor;
    sec_rgy_acct_key_t      name_parts;
    rsdb_named_item_t       person;
    rsdb_named_item_t       group;
    rsdb_named_item_t       org;
    rsdb_acct_item_t        account;
    krb5_data               pepper;
    rsdb_authkey_item_t     auth_item;
    krb5_kvno               next_version;
    krb5_keytype            key_type = RSDB_AUTH_DEFAULT_KEY_TYPE;
    char                    tmp_buf[rsdb_auth_encrypt_des_key_max_len];

    CLEAR_STATUS(st);
    CLEAR_LOGIN_NAME(&login_name);
    u_strcpy(login_name.pname, pname);
    cursor.valid = false;  
    pepper.data = NULL;
    pepper.length = 0;

    /* Verify that account exists for pname.
     * If no account for pname, return without error.
     */
    if(!rsdb_acct_lookup_acct_id( &login_name, &cursor, &name_parts,
            &person, &group, &org, &account, st )) {
        CLEAR_STATUS(st);
        return;
    } else if(BAD_STATUS(st)) {
        return;
    }

    /* generate the pepper using the default pepper algorithm */
    rsdb_acct_gen_default_pepper( pname, &pepper, st );
    if(BAD_STATUS(st))
        return;

    /* For each auth_key stored, starting with the current key in the
     * account record, check the default_pepper flag.  
     * If the default_pepper flag is false, do nothing.  
     * If it is true, store the pepper and set the flag to false.
     */
    if(account.default_pepper == true) {
        /* Insert pepper into acct buffer, re-insert saved auth_key after it */
        bcopy( ACCT_KEY(&account), tmp_buf, account.key_len);
        bcopy( pepper.data, ACCT_PEPPER(&account), pepper.length);
        account.key_pepper_len = pepper.length;
        account.default_pepper = false;
        bcopy( tmp_buf, ACCT_KEY(&account), account.key_len);
        store_acct_by_id(account.threads.id, &account);
    }

    /* If this account supports multiple key versions,
     * check the pepper on remaining keys.
     */
    if( MULT_KEY_VERS_OK(&account) &&
        account.next_key_version != sec_passwd_c_version_none ) {
        next_version = account.next_key_version;
        while( next_version != sec_passwd_c_version_none ) {
            if(!rsdb_auth_get_by_authkey_key(next_version, key_type, 
                account.threads.id, &auth_item, st)) {
                goto out;
            }
            if(auth_item.default_pepper == true) {
                bcopy( &(auth_item.buf[auth_item.pepper_len]), tmp_buf,
                       auth_item.key_len);
                bcopy( pepper.data, &(auth_item.buf[0]), pepper.length);
                auth_item.default_pepper = false;
                auth_item.pepper_len = pepper.length;
                bcopy( tmp_buf, &(auth_item.buf[auth_item.pepper_len]), 
                       auth_item.key_len);
                rsdb_auth_store_by_authkey_key(next_version, key_type,
                    account.threads.id, &auth_item, st);
                if(BAD_STATUS(st))
                    goto out;
            }
            next_version = auth_item.next_version;
        }
    }

out:
    if (pepper.data)
        free((char *)pepper.data);
    return;
}


PUBLIC boolean32 rsdb_get_acct_by_id
  (
    rsdb_pvt_id_t      id,
    rsdb_acct_item_t   *acct
  )
{
    rsdb_sequential_key_t  sequential_key;
    char                   *data;
    int                    data_len;
    error_status_t         st;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    rsdb_fetch(rsdb_acct_db, (Pointer) &sequential_key, sizeof(sequential_key),
                        &data, &data_len, &st);
    if (BAD_STATUS(&st)) {
        /* Signal end of list */
        *acct = last;
        return false;
    }
    if (data_len <= sizeof(*acct)) {
        bcopy(data, (Pointer) acct, data_len);
    } else {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_buf_too_small);
    }
    return true;
}


PRIVATE void reset_next_acct
  (
    rsdb_pvt_id_t  item_id,
    rsdb_pvt_id_t  new_next
  )
{
    rsdb_acct_item_t  acct;

    if (item_id == first.threads.id) {
        first.threads.next = new_next;
    }

    (void) rsdb_get_acct_by_id(item_id, &acct);
    acct.threads.next = new_next;
    store_acct_by_id(item_id, &acct);
}


PRIVATE void reset_previous_acct
  (
    rsdb_pvt_id_t  item_id,
    rsdb_pvt_id_t  new_prev
  )
{
    rsdb_acct_item_t  acct;

    if (item_id == last.threads.id) {
        last.threads.prev = new_prev;
    }

    if (rsdb_get_acct_by_id(item_id, &acct)) {
        acct.threads.prev = new_prev;
        store_acct_by_id(item_id, &acct);
    } else {
        dce_svc_printf(SEC_RSDB_ACCT_RESET_MSG);
    }
}


PRIVATE void unthread_acct
  (
    rsdb_pgo_item_t    *person,
    rsdb_acct_item_t   *account
  )
{
    rsdb_acct_item_t   alias_acct;

    if (!rsdb_get_acct_by_id(person->info, &alias_acct)) {
        dce_svc_printf(SEC_RSDB_ACCT_CANT_GETID_MSG);
    }
    while (alias_acct.threads.alias != account->threads.id) {
        if (alias_acct.threads.alias == END_OF_LIST) {
            dce_svc_printf(SEC_RSDB_ACCT_NOALIASES_MSG);
        }
        if (!rsdb_get_acct_by_id(alias_acct.threads.alias, &alias_acct)) {
            dce_svc_printf(SEC_RSDB_ACCT_CANT_GETID_MSG);
        }
    }
    alias_acct.threads.alias = account->threads.alias;
    store_acct_by_id(alias_acct.threads.id, &alias_acct);
}


PRIVATE void rethread_acct_list
  (
    rsdb_named_item_t  *person,
    rsdb_acct_item_t   *acct
  )
{
    unthread_acct(&RSDB_PGO(person), acct);

    acct->threads.alias     = RSDB_PGO(person).info;
    RSDB_PGO(person).info   = acct->threads.id;

    rsdb_util_store_item_by_id(sec_rgy_domain_person, person->body.threads.id,
                               person);
    store_acct_by_id(acct->threads.id, acct);
}


PRIVATE void import_acct_user_part
  (
    rsdb_acct_item_t     *acct,
    sec_rgy_acct_user_t  *new,
    rs_acct_parts_t      parts,
    boolean32            pwd_val_touch  /* if false, make no assumptions about password validity */
  )
{
    char    tmp_buf[rsdb_auth_pepper_max_len + rsdb_auth_encrypt_key_max_len];

    /* First save the original pepper and key in string_buf */
    bcopy(ACCT_PEPPER(acct), tmp_buf, acct->key_pepper_len + acct->key_len);


    if (IS_SET(parts, rs_acct_part_user)) {
        acct->user_part.flags        = new->flags;
        acct->user_part.homedir_len  = strlen( (char *) new->homedir);
        acct->user_part.shell_len    = strlen( (char *) new->shell);
        acct->user_part.gecos_len    = strlen( (char *) new->gecos);

        strncpy(&(acct->string_buf[0]), (char *) new->homedir, 
                    acct->user_part.homedir_len);
        strncpy(ACCT_SHELL(acct), (char *)new->shell, 
                acct->user_part.shell_len);
        strncpy(ACCT_GECOS(acct), (char *)new->gecos, 
                acct->user_part.gecos_len);
        /* Restore the pepper and key */
        bcopy(tmp_buf, ACCT_PEPPER(acct), acct->key_pepper_len + acct->key_len);

    } else if (IS_SET(parts, rs_acct_part_passwd) && pwd_val_touch) {
        /*
        * If the user part is not changing, but the password is -
        * then we want to assert that the password is valid.  If both
        * parts are changing, then we want to allow the explicit value
        * hold.  (Someone may be changing the whole account structure 
        * including the password, but asserting that the password is 
        * invalid.)  Exception is where the private key is changing (pwd_val_touch
        * is false): here we do not want the password valid flag to be
        * altered from its existing value;
        */
        acct->user_part.flags |= sec_rgy_acct_user_passwd_valid;
    }

    if (IS_SET(parts, rs_acct_part_passwd)) {  
        strcpy((char *)acct->user_part.passwd, (char *)new->passwd);
        acct->user_part.passwd_dtm  = new->passwd_dtm;
    }
    /* Note: new->passwd_version_number is not imported because
     * it is a read-only value in the database.
     */
}


PUBLIC void export_acct_user_part
  (
    rsdb_acct_item_t     *acct,
    sec_rgy_acct_user_t  *user_part
  )
{
    strcpy((char *)user_part->passwd, (char *)acct->user_part.passwd);
    user_part->flags        = acct->user_part.flags;
    user_part->passwd_dtm   = acct->user_part.passwd_dtm; 
    user_part->passwd_version_number = acct->cur_key_version;

    strncpy((char *) user_part->homedir, &(acct->string_buf[0]), 
                acct->user_part.homedir_len);
    user_part->homedir[acct->user_part.homedir_len] = '\0';

    strncpy((char *) user_part->shell,
            ACCT_SHELL(acct),
            acct->user_part.shell_len);
    user_part->shell[acct->user_part.shell_len] = '\0';

    strncpy((char *) user_part->gecos,
            ACCT_GECOS(acct),
            acct->user_part.gecos_len);
    user_part->gecos[acct->user_part.gecos_len] = '\0';
}


PRIVATE void gen_unix_key
  (
    sec_passwd_rec_t            *passwd,
    sec_rgy_unix_passwd_buf_t   unix_key,
    error_status_t              *status
  )
{  
    char            salt[2];
    char            *p;
    int             i;
    struct timeval  tp;
    struct timezone tzp;  
    unsigned_char_p_t       plain_passwd;
    char *char_tbl="./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    CLEAR_STATUS(status);

    if (PASSWD_TYPE(passwd) == sec_passwd_des) {
        /*
         * Pre-crunched keys can't generate a UNIX rep.
         */
        strcpy((char *) unix_key, "CIPHER");
        return;

    } else if (PASSWD_TYPE(passwd) != sec_passwd_plain) {
        SET_STATUS(status, sec_rgy_not_implemented);
        return;

    } else {
        plain_passwd = PLAIN_PASSWD(passwd);
    }

    if ((plain_passwd == NULL) || (plain_passwd[0] == '\0')) {
        unix_key[0] = '\0';
        return;
    }
#ifdef SNI_SVR4_POSIX
    gettimeofday(&tp);
#else
    gettimeofday(&tp, &tzp);
#endif /* SNI_SVR4_POSIX */
    tp.tv_usec >>= 2;               /* get rid of unreliable low order bits */
    for (i = 0; i < 2; i++) {
        salt[i] = *(char_tbl + (tp.tv_usec & 0x3f)); /* take low order 6 bits  to index*/
        tp.tv_usec >>= 6;
    }
    p = crypt((char*)plain_passwd, salt);
    strcpy((char *)unix_key, p);
}


/* 
 *  g e n _ a u t h _ k e y 
 *
 *  Generate auth key and encrypt it under the master key.
 *  If input passwd contains a version number that
 *  is not available, return status sec_rgy_key_bad_version.
 *  If !(acct->flags & rsdb_acct_item_mult_versions_ok),
 *  multiple key versions are not allowed, put new auth key in acct.
 *  Else, copy current auth_key (if any) from acct into auth_item and store
 *  in database; gen new auth key and store in acct.
 *
 *  Private keys are always put into an auth_item and stored in the
 *  database.  Error returned unless (acct->flags &
 *  rsdb_auth_item_privkey_ok)
 */
PRIVATE void gen_auth_key
  (
    sec_rgy_name_t          pname,
    sec_passwd_rec_t        *passwd,
    rsdb_acct_item_t        *acct,
    sec_timeval_sec_t       *time_now,
    sec_passwd_version_t    *new_version_p,
    error_status_t          *status
  )
{
    krb5_keytype            keytype;
    krb5_keyblock           *key_p;
    krb5_encrypted_keyblock ekey;
    krb5_data               pwd;
    krb5_data               pepper;
    unsigned_char_p_t       plain_passwd;
    rsdb_authkey_item_t     auth_item;  
    sec_rgy_login_name_t    login_name;

    CLEAR_STATUS(status); 

    /* Don't bother doing anything if password type not supported */
    if (PASSWD_TYPE(passwd) == sec_passwd_plain)
        keytype=RSDB_AUTH_DEFAULT_KEY_TYPE;
    else {
        keytype=sec_to_krb_keytype(PASSWD_TYPE(passwd), status);
        if (BAD_STATUS(status)) {
            SET_STATUS(status, sec_rgy_not_implemented);
            return;
        }
    }

    /* Don't bother doing anything if password type not supported for
     * this account
     */
    if ((PASSWD_TYPE(passwd) == sec_passwd_pubkey) && !PK_AUTHKEY_OK(acct)) {
        SET_STATUS(status, sec_rsdb_acct_bad_keytype);
        return;
    }

    /* Don't bother doing anything if DES key has bad parity */
    if (PASSWD_TYPE(passwd) == sec_passwd_des) {
        if (!mit_des_check_key_parity(passwd->key.tagged_union.des_key)) {
            SET_STATUS(status, sec_s_bad_key_parity);
            return;
        }
    }

    /*
     * Pre-crunched keys don't need any special processing
     */
    ekey.contents = NULL;
    pepper.data = NULL;
    pepper.length = 0;

    /* If version_number specified, verify that it's not
     * the current version and that it doesn't already exist.
     * Check for bad values.
     */
    rsdb_auth_set_version_number(passwd, acct, time_now, new_version_p, status);
    if(BAD_STATUS(status))
        return;

    /* Handle existing key versions:
     * For non-default keytypes, we delete any existing
     * version; otherwise,
     * If multiple key versions allowed and this is not
     * the first key to be added, purge expired keys and
     * store the old key in the database before overwriting
     * acct with new key.
     */
    if (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE) {

        if (MULT_KEY_VERS_OK(acct) &&
               (acct->cur_key_version != sec_passwd_c_version_none)) {

            if(acct->next_key_version != sec_passwd_c_version_none) {
                rsdb_auth_purge_expired_keys(acct, time_now, status);
                if(BAD_STATUS(status)) {
                    return;
                }
            }

            auth_item.next_version    = acct->next_key_version;
            auth_item.default_pepper  = acct->default_pepper;
            auth_item.pepper_len      = acct->key_pepper_len;
            auth_item.key_len         = acct->key_len; 
            /* account login name required to set expire time */
            CLEAR_LOGIN_NAME(&login_name);
            ustrcpy(login_name.pname, pname); 
            rsdb_auth_set_expire_time(&login_name, time_now,
                                      &(auth_item.key_expire_time), status);
            bcopy( ACCT_PEPPER(acct), auth_item.buf,
                acct->key_pepper_len + acct->key_len);
            rsdb_auth_store_by_authkey_key(acct->cur_key_version,
                RSDB_AUTH_DEFAULT_KEY_TYPE, acct->threads.id, &auth_item, status);
            acct->next_key_version = acct->cur_key_version;
        }
        acct->cur_key_version = *new_version_p;

    } else if (PK_AUTHKEY_EXISTS(acct)) {
        rsdb_auth_purge_by_authkey_key(PK_PRIVKEY_VER_GET(acct), keytype, acct->threads.id, status);
        if (BAD_STATUS(status))
            return;
    }

    switch (PASSWD_TYPE(passwd)) {

    case sec_passwd_des:
        /* 
         * Store the input pepper (if any).
         */
        acct->default_pepper = (passwd->pepper == NULL);
        acct->key_pepper_len = ustrlen(passwd->pepper);
        bcopy((char *)passwd->pepper, ACCT_PEPPER(acct),
            acct->key_pepper_len );
        key_p = sec_to_krb_key(passwd, status);
        break;

    case sec_passwd_plain:
        plain_passwd = PLAIN_PASSWD(passwd);
        pepper.length = ustrlen(passwd->pepper); 
        if(passwd->pepper != 0) { 
            acct->default_pepper = false;

            pepper.data = malloc(pepper.length?pepper.length:1);
            if(!pepper.data) {
                SET_STATUS(status, sec_rgy_cant_allocate_memory);
                goto out;
            }
            bcopy((char *)passwd->pepper, (char *)pepper.data, pepper.length);
            acct->key_pepper_len = pepper.length;
            bcopy((char *)pepper.data, ACCT_PEPPER(acct), acct->key_pepper_len); 
        } else {
            /* Use default pepper algorithm to generate pepper.
             * Don't store pepper.  It can be recreated with
             * default pepper algorithm. 
             */
            acct->default_pepper = true;
            acct->key_pepper_len = 0;
            rsdb_acct_gen_default_pepper( pname, &pepper, status );
            if (BAD_STATUS(status))
                goto out;
        }

        pwd.length = u_strlen((char *)plain_passwd);
        pwd.data = (char *)plain_passwd;

        key_p=(krb5_keyblock *)malloc(sizeof(krb5_keyblock));
        rs_mkey_string_to_key(keytype, &pwd, &pepper, key_p, status);
        if (BAD_STATUS(status))
            goto out;
        break;

    case sec_passwd_pubkey:
        key_p = sec_to_krb_key(passwd, status);
        break;
    }

    rs_mkey_encrypt_with_master(key_p, &ekey, status);
    if (BAD_STATUS(status))
        goto out;
    rsdb_get_master_key_version(&(acct->mkey_version), status);
    if (BAD_STATUS(status))
        goto out;

    if (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE) {
        /*
         * Copy new key into account
         */
        acct->key_len = ekey.length;
        bcopy(ekey.contents, ACCT_KEY(acct), acct->key_len);
    } else {
        /*
         * Copy new key into authkey item
         */
        auth_item.next_version   = sec_passwd_c_version_none;
        auth_item.default_pepper = TRUE;
        auth_item.pepper_len     = 0;
        auth_item.key_len        = ekey.length;
        bcopy((char *)ekey.contents, &(auth_item.buf[auth_item.pepper_len]), ekey.length);
        rsdb_auth_store_by_authkey_key(*new_version_p,
                                       keytype,
                                       acct->threads.id,
                                       &auth_item,
                                       status);
        if (BAD_STATUS(status))
            goto out;
        PK_PRIVKEY_VER_SET(acct,*new_version_p);
    }

out:
    if (key_p) {
        if (key_p->contents) {
            memset((char *)(key_p->contents), 0, key_p->length); 
            free((char *)key_p->contents);
        }
        free((char *)key_p);
    }

    if (ekey.contents) {
        memset((char *)(ekey.contents), 0, ekey.length); 
        free((char *)ekey.contents);
    }
    if (pepper.data)
        free((char *)pepper.data);
}


PRIVATE void create_new_account
  (
    rsdb_named_item_t     *person,
    rsdb_named_item_t     *group,
    rsdb_named_item_t     *org,
    rsdb_pvt_id_t         last_account,
    sec_rgy_acct_key_t    key_parts,
    sec_rgy_acct_user_t   *user_part,
    sec_rgy_acct_admin_t  *admin_part,
    sec_rgy_name_t        pname,
    sec_passwd_rec_t      *passwd,
    boolean32             gen_unix_passwd,
    sec_passwd_version_t  *passwd_version_number,
    rsdb_acct_item_t      *acct,
    error_status_t        *status
  )
{
    rsdb_acct_item_t      old_acct; 

    /* Initialize lengths to avoid copy problems import_acct_user_part */
    acct->user_part.homedir_len = 0;
    acct->user_part.shell_len   = 0;
    acct->user_part.gecos_len   = 0;
    acct->key_pepper_len        = 0;
    acct->key_len               = 0;

    acct->flags      = NULL;
    acct->person_id  = person->body.threads.id;
    acct->group_id   = group->body.threads.id;
    acct->org_id     = org->body.threads.id;
    acct->key_parts  = key_parts;
    /*ACL
     * simple assignment of admin_part may not work when creator and
     * changer ids change from sids to foreign_id_t(s)
     */
    acct->admin_part = *admin_part;

    /* Generate UNIX encrypted passwd.  NOTE: NULL passwd
     * results in NULL unix key.
     */
    if (gen_unix_passwd) {
        gen_unix_key(passwd, user_part->passwd, status);
        if(BAD_STATUS(status)) {
            return;
        }
    }
    import_acct_user_part(acct, user_part, 
                          rs_acct_part_user | rs_acct_part_passwd,
                          gen_unix_passwd);

    /* auth policy fields are zero to indicate not set */
    acct->auth_plcy.max_ticket_lifetime = 0;
    acct->auth_plcy.max_renewable_lifetime = 0;

    acct->cur_key_version = sec_passwd_c_version_none; 
    acct->next_key_version = sec_passwd_c_version_none; 
    /* indicate whether multiple key versions are allowed in next_key_version */
    if(rsdb_auth_mult_key_vers_allowed(pname)) {
        acct->flags |= rsdb_acct_item_mult_versions_ok;
    }
    if (rsdb_auth_privkey_allowed(pname)) {
        acct->flags |= rsdb_acct_item_privkey_ok;
    }
    gen_auth_key(pname, passwd, acct, NULL, passwd_version_number, status);
    if(BAD_STATUS(status)) {
        return;
    }

    acct->threads.next   = last.threads.id;
    acct->threads.prev   = last.threads.prev;
    acct->threads.alias  = END_OF_LIST;
    acct->threads.id     = rsdb_new_sequential_id();

    /*
     *  assign a new id for login /journal activity records
     *  but do not actually create the records untill login
     *  activity requires it.
     *  
     *  Since the creation of records occurs during the login sequence 
     *  in which locks for account records would be difficult to acquire,
     *  we assign a sequential id at account generation and this
     *  id is used to add new records later.
     *  
     *  Deletion of an account will also delete the login/journal records.
     */
    acct->login_activity_id       = rsdb_new_login_id();

    /*
     * Make sure primary accounts appear at the beginning of the alias chain
     */
    if (RSDB_PGO(person).info == END_OF_LIST ||
        key_parts == sec_rgy_acct_key_person) {
        acct->threads.alias = RSDB_PGO(person).info;
        RSDB_PGO(person).info = acct->threads.id;
        rsdb_util_store_item_by_id(sec_rgy_domain_person, person->body.threads.id,
                                   person);
    } else {
        (void) rsdb_get_acct_by_id(last_account, &old_acct);
        old_acct.threads.alias = acct->threads.id;
        store_acct_by_id(last_account, &old_acct);
    }

    /*
     * Add the new account into the database
     */
    store_acct_by_id(acct->threads.id, acct);

    /*
     * Add the new login activity record into the database
     * and add the new journal record with the same id but into
     * the journal database
     * rsdb_store_login_activity_by_id(acct->login_activity_id, &login_activity);
     * rsdb_store_journal_activity_by_id(acct->login_activity_id, &journal);
     */


    /*
     * Insert into sequential list
     */
    reset_next_acct(acct->threads.prev, acct->threads.id);
    reset_previous_acct(acct->threads.next, acct->threads.id);
}


PUBLIC void rsdb_purge_account
  (
    boolean32          update_person,
    rsdb_named_item_t  *person,
    rsdb_acct_item_t   *account
  )
{
    sec_rgy_pgo_item_t  pgo;
    sec_rgy_name_t      pgo_name;
    error_status_t      st;

    if (update_person) {
        if (RSDB_PGO(person).info == account->threads.id) {
            RSDB_PGO(person).info = account->threads.alias;
            rsdb_util_store_item_by_id(sec_rgy_domain_person, 
                                       person->body.threads.id, person);
        } else {
            unthread_acct(&RSDB_PGO(person), account);
        }
    }

    reset_next_acct(account->threads.prev, account->threads.next);
    reset_previous_acct(account->threads.next, account->threads.prev); 
    rsdb_auth_purge_acct_keys(account, &st);

    rsdb_purge_login_activity_by_id(account->login_activity_id);
    rsdb_purge_journal_activity_by_id(account->login_activity_id);
    purge_acct_by_id(account->threads.id);

    /*
     * delete account from unix info db
     */
}   /* End rsdb_purge_account. */


PRIVATE sec_rgy_acct_key_t get_login_name_pgo
  (
    sec_rgy_login_name_t    *name,
    rsdb_named_item_t   *person,
    rsdb_named_item_t   *group,
    rsdb_named_item_t   *org,
    error_status_t      *status
  )
{
    char               name_buf[sizeof(sec_rgy_login_name_t) + 1];
    char               *p;
    char               *n;
    sec_rgy_acct_key_t retval;
    long               pname_len,
                       gname_len,
                       oname_len;


    retval = sec_rgy_acct_key_none;

    if (!CHECK_LOGIN_NAME(name)) {
        return retval;
    }


    person->body.threads.id = END_OF_LIST;
    group->body.threads.id = END_OF_LIST;
    org->body.threads.id = END_OF_LIST;


    if (IS_WILD(name->pname)) {
        rsdb_pgo_util_get_by_id(sec_rgy_domain_person, 
                        domain_info[sec_rgy_domain_person].first.body.threads.next,
                        person);
        if (person->body.threads.id == 
                domain_info[sec_rgy_domain_person].last.body.threads.id) {
            SET_STATUS(status, sec_rgy_no_more_entries);
            return retval;
        }
    } else {
        pname_len = strlen( (char *) name->pname);
        if (!rsdb_pgo_util_get_by_name(sec_rgy_domain_person, name->pname,
            pname_len, person)) {
            SET_STATUS(status, sec_rgy_object_not_found);
            return retval;
        }

	/* In cell name space, check all aliases for the account */
	if (rsdb_name_util_is_cell(name->pname, status)
		&& GOOD_STATUS(status) )
	{
	    rsdb_named_item_t tmp_person;
	    rsdb_credentials_t creds;

	    /* Anchor search at head of alias chain */
	    if (!rsdb_get_credentials_by_unix_id(domain_info[sec_rgy_domain_person].db,
			RSDB_PGO(person).unix_id, &creds))
		return retval;

	    if (!rsdb_pgo_util_get_by_id(sec_rgy_domain_person, creds.id,
                &tmp_person))
		return retval;

	    for (;;) {
		if (RSDB_PGO(&tmp_person).info != END_OF_LIST) {
		    memcpy(person, &tmp_person, NAMED_ITEM_LEN(&tmp_person));
		    break;
		}

		if (tmp_person.body.threads.alias == END_OF_LIST)
		    break;

		rsdb_pgo_util_get_by_id(sec_rgy_domain_person,
					tmp_person.body.threads.alias, &tmp_person);
	    }
	}
    }

    retval = sec_rgy_acct_key_person;
    if (IS_WILD(name->gname) && IS_WILD(name->oname)) {
	/* shut up purify: deterministically initialize group and org here */
	memset(group, 0, sizeof(*group));
	memset(org, 0, sizeof(*org));
        return retval;
    }
    
    if (!IS_WILD(name->gname)) {
        gname_len = strlen( (char *) name->gname);
        if (!rsdb_pgo_util_get_by_name(sec_rgy_domain_group, name->gname,
                                       gname_len, group)) {
            SET_STATUS(status, sec_rgy_object_not_found);
            return retval;
        }
        retval = sec_rgy_acct_key_group;
        if (IS_WILD(name->oname))
            return retval;
    }


    if (!IS_WILD(name->oname)) {
        if (retval == sec_rgy_acct_key_person) {
            /*group component was an embedded wildcard */
            SET_STATUS(status, sec_rgy_incomplete_login_name);
            return retval;
        }

        oname_len = strlen( (char *) name->oname);
        if (!rsdb_pgo_util_get_by_name(sec_rgy_domain_org, name->oname,
                                       oname_len, org)) {
            SET_STATUS(status, sec_rgy_object_not_found);
            return retval;
        }
        retval = sec_rgy_acct_key_org;
    }

    return retval;
}


/*
 * If gen_unix_passwd is true, generate a unix passwd, or not, depending on the
 * type of passwd  (plaintext can be used to generate unix passwords, 
 * ciphertext cannot).  If gen_unix_passwd is false, assumme the caller
 * has provided the unix passwd in the "passwd" field of the user_part
 * parameter
 */
PUBLIC void rsdb_acct_add
  (
    sec_rgy_login_name_t   *login_name,    /* [in] */
    sec_rgy_acct_key_t     key_parts,      /* [in] */
    sec_rgy_acct_user_t    *(user_part),   /* [in] */
    sec_rgy_acct_admin_t   *(admin_part),  /* [in] */
    sec_passwd_rec_t       *passwd,        /* [in] */
    boolean32              gen_unix_passwd,         /* [in] */
    sec_passwd_version_t   *passwd_version_number,  /* [out] */
    error_status_t         *(status)
  )
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    rsdb_pvt_id_t          last_account;
    sec_rgy_acct_key_t     name_parts;
    sec_rgy_pgo_item_t     pgo;
    sec_rgy_name_t         pgo_name;
    boolean32              first_account = false;
    rsdb_acct_item_t       acct;
    error_status_t         st;

    CLEAR_STATUS(status);

    name_parts = get_login_name_pgo(login_name, &person, &group, 
                                        &org, status);

    if (BAD_STATUS(status))
        return;

    if (name_parts != sec_rgy_acct_key_org) {
        SET_STATUS(status, sec_rgy_incomplete_login_name);
        return;
    }

    if (account_exists(&person, &group, &org, key_parts, &last_account)) {
        SET_STATUS(status, sec_rgy_object_exists);
        return;
    }

    if (!rsdb_util_is_member(sec_rgy_domain_group, RSDB_PGO(&group).membership,
                      person.body.threads.id, (rsdb_pvt_id_t *) NULL)) {
        SET_STATUS(status, sec_rgy_not_member_group);
        if (!rsdb_util_is_member(sec_rgy_domain_org, RSDB_PGO(&org).membership,
                                 person.body.threads.id,
                                 (rsdb_pvt_id_t *) NULL)) {
            SET_STATUS(status, sec_rgy_not_member_group_org);
        } 
        return;
    }
    if (!rsdb_util_is_member(sec_rgy_domain_org, RSDB_PGO(&org).membership,
                             person.body.threads.id,
                             (rsdb_pvt_id_t *) NULL)) {
        SET_STATUS(status, sec_rgy_not_member_org);
        return;
    }

    if (RSDB_PGO(&person).info == END_OF_LIST) {
        first_account = true;
    }

    create_new_account(&person, &group, &org, last_account, key_parts,
                       user_part, admin_part, login_name->pname,
                       passwd, gen_unix_passwd, passwd_version_number, 
                       &acct, status);

    if (BAD_STATUS(status)) {
        return;
    }

    if (key_parts == sec_rgy_acct_key_person) {
        /* Mark the group membership information with the fact that
         * this account exists.  This information will be used when
         * generating the UNIX group file information
         */
        rsdb_util_member_has_account(sec_rgy_domain_group, &RSDB_PGO(&group),
                           &RSDB_PGO(&person), true);
    }

    /*
     * If the account is being created for a cell principal,
     * always store the default pepper -- this so that potential
     * aliases are guaranteed to refer to the original pepper.
     */
    if (rsdb_name_util_is_cell(login_name->pname, status) && GOOD_STATUS(status) ) {
	rsdb_acct_store_default_pepper(login_name->pname, status);
	if (BAD_STATUS(status)) {
	    return;
	}
    }
}


PUBLIC void rsdb_acct_delete
  (
    sec_rgy_login_name_t   *login_name,    /* [in] */
    error_status_t         *(status)
  )
{
    rsdb_named_item_t  person;
    rsdb_named_item_t  group;
    rsdb_named_item_t  org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t   account;
    sec_rgy_cursor_t       cursor;

    CLEAR_STATUS(status);

    name_parts = get_login_name_pgo(login_name, &person, &group, 
                                        &org, status);

    if (BAD_STATUS(status))
        return;

    cursor.valid = false;
    if (find_account(true, &cursor, &RSDB_PGO(&person), &RSDB_PGO(&group),
                     &RSDB_PGO(&org), name_parts, &account)) {

        if (account.key_parts == sec_rgy_acct_key_person) {
            /* Update group info so that group now knows the account has
            * gone away.
            */
            if (group.body.threads.id != account.group_id) {
                rsdb_pgo_util_get_by_id(sec_rgy_domain_group, account.group_id,
                                        &group);
            }
            rsdb_util_member_has_account(sec_rgy_domain_group, &RSDB_PGO(&group),
                                             &RSDB_PGO(&person), false);
        }
        rsdb_purge_account(true, &person, &account);
    }
}   /* End rsdb_acct_delete. */


/*  
 *  r s d b _ a c c t _ r e n a m e 
 *
 *  Rename the account old_login_name to new_login_name.
 *  Only the group and org parts of the name may change.
 *  Person must already be a member of group and org
 *  for the operation to succeed.
 */
PUBLIC void rsdb_acct_rename
  (
    sec_rgy_login_name_t  *(old_login_name),    /* [in] */
    sec_rgy_login_name_t  *(new_login_name),    /* [in] */
    sec_rgy_acct_key_t    *(new_key_parts),     /* [in, out] */
    error_status_t        *(status)
  )
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      old_group;
    rsdb_named_item_t      new_group;
    rsdb_named_item_t      old_org;
    rsdb_named_item_t      new_org;
    sec_rgy_acct_key_t     full_key_parts;
    rsdb_pvt_id_t          last_account;
    sec_rgy_acct_key_t     name_parts;
    sec_rgy_pgo_item_t     pgo;
    sec_rgy_name_t         pgo_name;
    boolean32              first_account = false;
    rsdb_acct_item_t       account;
    error_status_t         st;

    CLEAR_STATUS(status);

    /* Make sure principal name hasn't changed */
    if(ustrcmp(old_login_name->pname, new_login_name->pname)) {
        SET_STATUS(status, sec_rgy_bad_name);
        return;
    }

    name_parts = get_login_name_pgo(new_login_name, &person, &new_group, 
                                        &new_org, status);

    if (BAD_STATUS(status))
        return;

    if (name_parts != sec_rgy_acct_key_org) {
        SET_STATUS(status, sec_rgy_incomplete_login_name);
        return;
    }
    /* To check if new account name exists, compare against full name (key_parts) */
    full_key_parts = sec_rgy_acct_key_org;
    if (account_exists(&person, &new_group, &new_org, full_key_parts, &last_account)) {
        SET_STATUS(status, sec_rgy_object_exists);
        return;
    }

    if (!rsdb_util_is_member(sec_rgy_domain_group, RSDB_PGO(&new_group).membership,
                      person.body.threads.id, (rsdb_pvt_id_t *) NULL)) {
        SET_STATUS(status, sec_rgy_not_member_group);
        if (!rsdb_util_is_member(sec_rgy_domain_org, RSDB_PGO(&new_org).membership,
                                 person.body.threads.id,
                                 (rsdb_pvt_id_t *) NULL)) {
            SET_STATUS(status, sec_rgy_not_member_group_org);
        } 
        return;
    }
    if (!rsdb_util_is_member(sec_rgy_domain_org, RSDB_PGO(&new_org).membership,
                             person.body.threads.id,
                             (rsdb_pvt_id_t *) NULL)) {
        SET_STATUS(status, sec_rgy_not_member_org);
        return;
    }
    name_parts = get_login_name_pgo(new_login_name, &person, &old_group, 
                                        &old_org, status);
    if (BAD_STATUS(status))
        return;

    if (name_parts != sec_rgy_acct_key_org) {
        SET_STATUS(status, sec_rgy_incomplete_login_name);
        return;
    }
    if (!account_exists(&person, &old_group, &old_org, *new_key_parts, &last_account)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    if(!rsdb_get_acct_by_id(last_account, &account)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    account.group_id = new_group.body.threads.id;
    account.org_id   = new_org.body.threads.id;
    store_acct_by_id(last_account, &account);

    return;
}


PRIVATE boolean32 embedded_wildcards
  (
    sec_rgy_login_name_t  *name,
    rsdb_named_item_t     *person,
    rsdb_named_item_t     *group,
    rsdb_named_item_t     *org,
    error_status_t        *status
  )
{
    long                  pname_len,
                          gname_len,
                          oname_len;
    CLEAR_STATUS(status);

    if (!CHECK_LOGIN_NAME(name)) {
        return false;
    }

    /* END_OF_LIST id indicates a wild_carded component.  We initialize the
     * pgo component ids here.  Non-wildcarded components will get overwritten
     * by the get_pgo_by_name calls below
     */
    person->body.threads.id = END_OF_LIST;
    group->body.threads.id = END_OF_LIST;
    org->body.threads.id = END_OF_LIST;

    /*
     * If the name is of the form p= NULL,g=group,o=NULL or
     * p=NULL,g=NULL,o=org or p=NULL,g=NULL,o=NULL then there are embedded
     * wildcards (p=NULL,g=NULL,o=NULL is a special case).  Otherwise the name
     * must be p=name,g=NULL,o=org (p=name,g=NULL,o=NULL and
     * p=name,g=group,o=NULL do not have embedded wild cards)  So we just
     * check to see if the name is not a wild card and that if the group is
     * then the org is as well - which is sufficient to determine that there
     * are no embedded wildcards. 
     */
    if ( (!IS_WILD(name->pname)) &&
         (!IS_WILD(name->gname) || IS_WILD(name->oname)) ) {
            return false;
        }


    if (!IS_WILD(name->pname)) {
        pname_len = strlen( (char *) name->pname);
        if (!rsdb_pgo_util_get_by_name(sec_rgy_domain_person, name->pname,
                                       pname_len, person)) {

            SET_STATUS(status, sec_rgy_object_not_found);
            return false;
        }
    }

    if (!IS_WILD(name->gname)) {
        gname_len = strlen( (char *) name->gname);
        if (!rsdb_pgo_util_get_by_name(sec_rgy_domain_group, name->gname,
                                       gname_len, group)) {
            SET_STATUS(status, sec_rgy_object_not_found);
            return false;
        }
    }

    if (!IS_WILD(name->oname)) {
        oname_len = strlen( (char *) name->oname);
        if (!rsdb_pgo_util_get_by_name(sec_rgy_domain_org, name->oname,
                                       oname_len, org)) {
            SET_STATUS(status, sec_rgy_object_not_found);
            return false;
        }
    }

    return true;
}


PUBLIC void get_name_component
  (
    sec_rgy_domain_t       domain,
    rsdb_pvt_id_t      id,
    sec_rgy_acct_key_t name_parts,
    char               **p,
    rsdb_named_item_t  *pgo
  )
{
    sec_rgy_name_t     component_name;
    sec_rgy_acct_key_t test_part;
    long               len;
    error_status_t     status;

    switch (domain) {
        case sec_rgy_domain_person:
            test_part = sec_rgy_acct_key_person;
            break;

        case sec_rgy_domain_group:
            test_part = sec_rgy_acct_key_group;
            break;

        case sec_rgy_domain_org:
            test_part = sec_rgy_acct_key_org;
            break;
    }

    if (name_parts < test_part) {
        rsdb_pgo_util_get_pgo_and_name_by_id(domain, id, pgo,
                                             component_name, &len);
        strncpy(*p, (char *) component_name, len);
        (*p)[len] = '\0';
    } else {
        rsdb_util_path_construct(domain, id, component_name, &len, &status);
        strncpy(*p, (char *) component_name, len);
        (*p)[len] = '\0';
    }

    *p += len;
}


PUBLIC boolean32 rsdb_acct_lookup_acct_id
  (
    sec_rgy_login_name_t  *login_name,    /* [in, out] */
    sec_rgy_cursor_t      *(cursor),      /* [in, out] */
    sec_rgy_acct_key_t    *name_parts,
    rsdb_named_item_t     *person,
    rsdb_named_item_t     *group,
    rsdb_named_item_t     *org,
    rsdb_acct_item_t      *account,
    error_status_t        *(status)
  )
{
    boolean32             got_account = false;
    rsdb_pvt_id_t         account_id;

    if (embedded_wildcards(login_name, person, group, org,
                                status)) {
        /*
         * Force the name recovery code (below) to extract the complete name
         */
        *name_parts = sec_rgy_acct_key_none;

        /*
         * Resume search at cursor position or at the first account
         */
        if (cursor->valid) {
            account_id      = cursor->handle;
            cursor->valid   = false;
        } else {
            account_id      = first.threads.id;
        }

        SET_STATUS(status, sec_rgy_no_more_entries);

        if ((account_id != last.threads.id)
                && rsdb_get_acct_by_id(account_id, account)) {


            while ((account->threads.next != last.threads.id)
                    && rsdb_get_acct_by_id(account->threads.next, account)) {
            /*
             * If this account meets the matching criteria (either the p,g,o
             * fields match, or the login string specified a wildcard for that
             * field) 
             */
            if ((person->body.threads.id == END_OF_LIST 
                            || account->person_id == person->body.threads.id)
                        && (group->body.threads.id == END_OF_LIST
                            || account->group_id == group->body.threads.id)
                        && (org->body.threads.id == END_OF_LIST
                            || account->org_id == org->body.threads.id)) {

                    cursor->handle  = account->threads.id;
                    cursor->valid   = true;
                    got_account     = true;
                    CLEAR_STATUS(status);
                    break;

                }
            }
        }

    } else if (STATUS_OK(status)) {

        *name_parts = get_login_name_pgo(login_name, person, group, org,
                                         status);

        if (STATUS_OK(status)
                && find_account(false, cursor, &RSDB_PGO(person),
                                &RSDB_PGO(group), &RSDB_PGO(org),
                                *name_parts, account)) {
            got_account = true;
        }
    }

    if ( (got_account == FALSE) && STATUS_OK( status ) )
      SET_STATUS ( status, sec_rgy_object_not_found );

    return got_account;
}


PUBLIC void rsdb_acct_lookup
  (
    sec_rgy_login_name_t   *login_name,     /* [in, out] */
    sec_rgy_cursor_t       *(cursor),      /* [in, out] */
    sec_rgy_sid_t          *(sid),         /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid),    /* [out] */
    sec_rgy_acct_key_t     *(key_parts),   /* [out] */
    sec_rgy_acct_user_t    *(user_part),   /* [out] */
    sec_rgy_acct_admin_t   *(admin_part),  /* [out] */
    error_status_t         *(status)
  )
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    rsdb_pvt_id_t          account_id;
    unsigned char          *p;

    CLEAR_STATUS(status);

    if (rsdb_acct_lookup_acct_id (login_name,  cursor,
                                  &name_parts, &person,
                                  &group, &org,
                                  &account, status)) {
        p = login_name->pname;
        get_name_component(sec_rgy_domain_person, account.person_id, name_parts,
                                (char **) &p, &person);

        p = login_name->gname;
        get_name_component(sec_rgy_domain_group, account.group_id, name_parts,
                                (char **) &p, &group);

        p = login_name->oname;
        get_name_component(sec_rgy_domain_org, account.org_id, name_parts,
                                (char **) &p, &org);

        *key_parts      = account.key_parts;
        *admin_part     = account.admin_part;
        export_acct_user_part(&account, user_part);

        rsdb_util_get_sid(&RSDB_PGO(&person), &RSDB_PGO(&group),
                         &RSDB_PGO(&org), unix_sid, sid);

    } else if (STATUS_OK(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
    }
}


/* rsdb_acct_key_get_next 
 * 
 * Get the next key for an account.  When called with an initialized
 * cursor, this routine must *always* return a "current" key.  The
 * replica initialization code depends on it.  
 *
 * This routine was written to support replica initialization
 * and is called only by surrogate master when initializing
 * a slave.  As initialization occurs under a global lock,
 * we can be sure that the database will not change between
 * calls to this routine.  This simplifies cursor management
 * considerably.  This routine *should not* be used in
 * circumstances where the database may have changed between
 * calls.  
 *
 * Curently only DES keys are supported.  When multiple key types
 * are supported, this routine will need to be rewritten.
 * Caller must free key->pepper if not NULL on successful return.
 *
 * ERRORS:
 *     sec_rgy_no_more_entries
 *     sec_rgy_object_not_found
 */

PUBLIC void rsdb_acct_key_get_next
  (
    sec_rgy_login_name_t   *name,             /* [in] */
    sec_rgy_cursor_t       *cursor,           /* [in, out] */
    sec_passwd_rec_t       *key,              /* [out] */
    boolean32              *current,          /* [out] */
    sec_timeval_sec_t      *garbage_collect,  /* [out] */
    error_status_t         *status
  )
{
    rsdb_acct_authkey_get_next(name, RSDB_AUTH_DEFAULT_KEY_TYPE,
                               cursor, key, current,
                               garbage_collect, status);
}

/* r s d b _ a c c t _ a u t h k e y _ g e t _ n e x t
 *
 * Get next authkey of a given keytype; generalization of
 * rsdb_acct_key_get_next()
 *
 * In Parameters:
 *     name                   - principal name
 *     keytype                - keytype to be retrieved
 *
 * In/Out Parameters:
 *     cursor                 - pointer to a rgy cursor,
 *                              initialized as cursor->valid=false
 *
 * Out Parameters:
 *     key                    - pointer to returned key
 *     current                - returned true if returned key
 *                              is the current version of this keytype
 *     garbage_collect        - pointer to returned key expire time
 *     status                 - error status
 *
 * Errors Returned:
 *     sec_rgy_object_not_found:     account for named principal not found
 *     sec_rgy_no_more_entries:      no more keys of this keytype
 *     sec_rgy_cant_allocate_memory: unable to allocate memory required
 *     sec_rgy_key_bad_type:         key found is not a supported type
 *
 * Errors Passed Through From:
 *     get_login_name_pgo()
 *     rsdb_auth_get_by_authkey_key()
 *     rs_mkey_decrypt_with_master()
 *     krb_to_sec_keytype()
 *     rsdb_acct_gen_default_pepper()
 */
PUBLIC void rsdb_acct_authkey_get_next
  (
    sec_rgy_login_name_t   *name,             /* [in] */
    krb5_keytype           keytype,            /* [in] */
    sec_rgy_cursor_t       *cursor,           /* [in, out] */
    sec_passwd_rec_t       *key,              /* [out] */
    boolean32              *current,          /* [out] */
    sec_timeval_sec_t      *garbage_collect,  /* [out] */
    error_status_t         *status
  )
{

    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    sec_rgy_cursor_t       tmp_cursor; 
    short                  key_len, pepper_len;
    rsdb_authkey_item_t    auth_item;
    short                  default_pepper;  
    krb5_encrypted_keyblock ekey;
    krb5_keyblock          krb5_key;
    krb5_data              pepper;


    *garbage_collect = 0;

    /* lookup the account information */
    name_parts = get_login_name_pgo(name, &person, &group, &org, status);
    if (GOOD_STATUS(status)) {
        tmp_cursor.valid = false;
        if (!find_account(true, &tmp_cursor, &RSDB_PGO(&person), &RSDB_PGO(&group),
                          &RSDB_PGO(&org), name_parts, &account)) {
            SET_STATUS(status, sec_rgy_object_not_found);
        }
    }

    if (BAD_STATUS(status)) {
        return;
    }

    if (cursor->valid) { 
        if (cursor->handle == sec_passwd_c_version_none) {
            SET_STATUS(status, sec_rgy_no_more_entries);
         } else {
            /*
             * find the next key using the version number stored in the
             * cursor
             */ 
            (void) rsdb_auth_get_by_authkey_key(cursor->handle, 
                                                keytype,
                                                account.threads.id, &auth_item,
                                                status);
            if (GOOD_STATUS(status)) {
                key_len = auth_item.key_len;  
                default_pepper = auth_item.default_pepper;
                *current = false; 
                *garbage_collect = auth_item.key_expire_time;
                key->version_number = cursor->handle;
            }
        }

    } else if (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE) {
       /* use the current key stored with the account data */  
       key_len = account.key_len;
       default_pepper = account.default_pepper;
       *current = true;
       key->version_number = account.cur_key_version;

    } else if (keytype == KEYTYPE_ENCRYPT_RSA_PRIV) {

        if (!PK_AUTHKEY_EXISTS(&account)) {
            SET_STATUS(status, sec_rgy_no_more_entries);
        } else {
            /* find the private key using version # 1 */
            (void) rsdb_auth_get_by_authkey_key(1,
                                                keytype,
                                                account.threads.id, &auth_item,
                                                status);
            if (GOOD_STATUS(status)) {
                key_len = auth_item.key_len;
                default_pepper = auth_item.default_pepper;
                *current = true;
                *garbage_collect = auth_item.key_expire_time;
                key->version_number = 1;
            }
        }
    }

    if (GOOD_STATUS(status)) {
        ekey.contents =  (krb5_octet *)malloc(key_len);
        if (ekey.contents == NULL) {
            SET_STATUS(status, sec_rgy_cant_allocate_memory);
        } else {
            ekey.length = key_len;
            ekey.keytype = keytype;
        }
    }

    if (BAD_STATUS(status)) {
        return;
    } 

    if (*current && (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE)) {
        memcpy(ekey.contents,
               &(account.string_buf[account.user_part.homedir_len +
                account.user_part.shell_len + account.user_part.gecos_len +
                account.key_pepper_len]), 
                ekey.length);
    } else {
        memcpy(ekey.contents, &(auth_item.buf[auth_item.pepper_len]), 
                 ekey.length);
    }

    /* decrypt the key using the master_key */
    rs_mkey_decrypt_with_master(account.mkey_version, &ekey, &krb5_key, status);

    /* done with ekey, in any event... */
    memset((char *)ekey.contents, 0, ekey.length);
    free((char *)ekey.contents);

    if (BAD_STATUS(status)) {
        return;
    }

    PASSWD_TYPE(key) = krb_to_sec_keytype(krb5_key.keytype, status);  

    /* copy the key data */
    switch(PASSWD_TYPE(key)) {
    case sec_passwd_des:
        memcpy(DES_PASSWD(key), krb5_key.contents, 
               sec_passwd_c_des_key_size);
        break;
    case sec_passwd_pubkey:
        PUBKEY_PASSWD(key).len = krb5_key.length;
        PUBKEY_PASSWD(key).data = (byte *)malloc(krb5_key.length);
        if (PUBKEY_PASSWD(key).data == NULL)
            SET_STATUS(status, sec_rgy_cant_allocate_memory);
        else
            memcpy(PUBKEY_PASSWD(key).data, krb5_key.contents, krb5_key.length);
        break;
    default:
        SET_STATUS(status, sec_rgy_key_bad_type);
    }

    /* done with krb5_key */
    memset((char *)krb5_key.contents, 0, krb5_key.length);
    free((char *)krb5_key.contents);

    if (BAD_STATUS(status)) {
        return;
    }

    key->pepper = NULL;
    if (PASSWD_TYPE(key) == sec_passwd_des) {
        /* copy the pepper field */
        if (!default_pepper) {
            /* use non_default pepper stored with key */
            *current ? (pepper_len = account.key_pepper_len)
                     : (pepper_len = auth_item.pepper_len); 

            key->pepper = (unsigned_char_t *)malloc(pepper_len + 1); 
            if (key->pepper == NULL) {
                SET_STATUS(status, sec_rgy_cant_allocate_memory);
                memset(DES_PASSWD(key), 0, sec_passwd_c_des_key_size);
            } else {
                if (*current) {
                    memcpy((char *)key->pepper,
                           &(account.string_buf[account.user_part.homedir_len +
                           account.user_part.shell_len +
                           account.user_part.gecos_len]),
                           pepper_len );
                } else {
                    memcpy((char *)key->pepper, auth_item.buf, pepper_len); 
                }
                key->pepper[pepper_len] = '\0';
            }
        } else {
            pepper.data = NULL;
            pepper.length = 0;

            /* generate the pepper using the default pepper algorithm */
            rsdb_acct_gen_default_pepper( name->pname, &pepper, status );
            if(BAD_STATUS(status)) {
                return;
            }

            key->pepper = (unsigned_char_t *)malloc(pepper.length + 1); 
            if (key->pepper == NULL) {
                SET_STATUS(status, sec_rgy_cant_allocate_memory);
                memset(DES_PASSWD(key), 0, sec_passwd_c_des_key_size);
            } else {
                memcpy((char *)key->pepper, pepper.data, pepper.length); 
                key->pepper[pepper.length] = '\0';
            }
            if(pepper.data)
                free(pepper.data);
        }
    }

    if (GOOD_STATUS(status)) {
        /* advance the cursor handle to the next key_version */
        if (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE)
            *current ? (cursor->handle = account.next_key_version)
                     : (cursor->handle = auth_item.next_version);
        else
            cursor->handle = sec_passwd_c_version_none;
        cursor->valid = true;
    }
}


PUBLIC void rsdb_acct_login_lookup
  (
    sec_rgy_login_name_t   *login_name,    /* [in, out] */
    sec_rgy_cursor_t       *(cursor),      /* [in, out] */
    sec_rgy_sid_t          *(sid),         /* [out] */
    sec_rgy_unix_sid_t     *(unix_sid),    /* [out] */
    sec_rgy_acct_key_t     *(key_parts),   /* [out] */
    sec_rgy_acct_user_t    *(user_part),   /* [out] */
    sec_rgy_acct_admin_t   *(admin_part),  /* [out] */
    error_status_t         *(status)
  )
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    boolean32              got_account = false;
    rsdb_pvt_id_t          account_id;
    unsigned char          *p;

    CLEAR_STATUS(status);

    if (embedded_wildcards(login_name, &person, &group, &org,
                                status)) {
        /*
         * Login queries can't specify embedded wildcards!
         */
        SET_STATUS(status, sec_rgy_object_not_found);

    } else if (BAD_STATUS(status)) {

        return;

    } else {
        name_parts = get_login_name_pgo(login_name, &person, 
                                        &group, &org, status);

        if (BAD_STATUS(status))
            return;

        if (find_login_account(cursor, &RSDB_PGO(&person),
                               &RSDB_PGO(&group), &RSDB_PGO(&org),
                               name_parts, &account)) {
            got_account = true;
        }
    }

    if (got_account) {
        p = login_name->pname;
        get_name_component(sec_rgy_domain_person, account.person_id, name_parts,
                                (char **) &p, &person);

        p = login_name->gname;
        get_name_component(sec_rgy_domain_group, account.group_id, name_parts,
                                (char **) &p, &group);

        p = login_name->oname;
        get_name_component(sec_rgy_domain_org, account.org_id, name_parts,
                                (char **) &p, &org);

        *key_parts      = account.key_parts;
        *admin_part     = account.admin_part;
        export_acct_user_part(&account, user_part);

        rsdb_util_get_sid(&RSDB_PGO(&person), &RSDB_PGO(&group),
                         &RSDB_PGO(&org), unix_sid, sid);

    } else {
        SET_STATUS(status, sec_rgy_object_not_found);
    }
}


PUBLIC void rsdb_acct_replace
  (
    sec_rgy_login_name_t   *login_name,    /* [in] */
    sec_rgy_acct_key_t     key_parts,      /* [in] */
    rs_acct_parts_t        modify_parts,   /* [in] */
    sec_rgy_acct_user_t    *(user_part),   /* [in] */
    sec_rgy_acct_admin_t   *(admin_part),  /* [in] */
    sec_passwd_rec_t       *passwd,        /* [in] */
    boolean32              gen_unix_passwd,/* [in] */
    sec_timeval_sec_t      *time_now,      /* [in] */
    sec_passwd_version_t   *passwd_version_number,  /* [out] */
    error_status_t         *(status)
  )
{
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    rsdb_acct_item_t       existing_account;
    sec_rgy_cursor_t       cursor;
    sec_rgy_pgo_item_t     pgo;

    CLEAR_STATUS(status);

    if (modify_parts == 0)
        return;

    name_parts = get_login_name_pgo(login_name, &person, &group, 
                                    &org, status);

    if (BAD_STATUS(status))
        return;

    cursor.valid = false;
    if (find_account(true, &cursor, &RSDB_PGO(&person), &RSDB_PGO(&group),
                     &RSDB_PGO(&org), name_parts, &account)) {
        if (IS_SET(modify_parts, rs_acct_part_login_name) &&
            account.key_parts != key_parts) {

            /*
             * Make sure we don't modify login_name to an already existing
             * account name.  (if we are making the login name less general we
             * already know that this account is unique!) 
             */
            if (key_parts < account.key_parts) {
                cursor.valid = false;
                if (find_account(true, &cursor, &RSDB_PGO(&person),
                                 &RSDB_PGO(&group), &RSDB_PGO(&org),
                                 key_parts, &existing_account)) {
                    /*
                     * Check if the existing account is already using the
                     * requested abbreviation.  The existing account might
                     * have a shorter abbreviation which should not force us
                     * to reject the change being requested, but would be
                     * returned by find_account(). 
                     */
                    if (existing_account.key_parts == key_parts) {
                        SET_STATUS(status, sec_rgy_name_exists);
                        return;
                    }
                }
            }
            /* Adjust group account info */

            if (account.key_parts == sec_rgy_acct_key_person) {
                if (group.body.threads.id != account.group_id) {
                    rsdb_pgo_util_get_by_id(sec_rgy_domain_group,
                                                account.group_id, &group);
                }
                rsdb_util_member_has_account(sec_rgy_domain_group,
                                             &RSDB_PGO(&group),
                                             &RSDB_PGO(&person), false);
            } else if (key_parts == sec_rgy_acct_key_person) {
                if (group.body.threads.id != account.group_id) {
                    rsdb_pgo_util_get_by_id(sec_rgy_domain_group, account.group_id,
                                            &group);
                }
                rsdb_util_member_has_account(sec_rgy_domain_group,
                                             &RSDB_PGO(&group),
                                             &RSDB_PGO(&person), true);
            }
            account.key_parts = key_parts;
        }
        /*
         * Re-thread alias chain if a primary account does not appears at the
         * beginning of the chain. (it should) 
         */
        if (account.key_parts == sec_rgy_acct_key_person
            && RSDB_PGO(&person).info != account.threads.id) {

            rethread_acct_list(&person, &account);

        }
        if (IS_SET(modify_parts, rs_acct_part_user) ||
            IS_SET(modify_parts, rs_acct_part_passwd)) {

            if (IS_SET(modify_parts, rs_acct_part_passwd)) {
                /* Generate UNIX encrypted passwd. */
                if (gen_unix_passwd) {
                    gen_unix_key(passwd, user_part->passwd, status);
                    if (BAD_STATUS(status)) {
                        return;
                    }
                }
                /* Generate auth key. */
                gen_auth_key(login_name->pname, passwd,
                             &account, time_now, passwd_version_number, status);
                if (BAD_STATUS(status)) {
                    return;
                }
            }

            /* If no unix password was generated, we do not touch
             * the password valid flag unless it was explicitly
             * set via a change to flags
             */
            import_acct_user_part(&account, user_part, modify_parts, gen_unix_passwd);
        }
        if (IS_SET(modify_parts, rs_acct_part_admin)) {
            /*
             * Preserve the creation time and creator info 
             */
            admin_part->creator = account.admin_part.creator;
            admin_part->creation_date = account.admin_part.creation_date;
            account.admin_part = *admin_part;
        }
        /* Note: auth policy is not changed/overwritten in this operation. */
        store_acct_by_id(account.threads.id, &account);

    } else {
        SET_STATUS(status, sec_rgy_object_not_found);
    }
}

/* rsdb_acct_get_cur_key_version()
 *
 */
PUBLIC void rsdb_acct_get_cur_key_version (
    sec_rgy_login_name_t   *login_namep, /* in */
    sec_passwd_type_t      key_type,     /* in */
    unsigned32             *key_version, /* out */
    error_status_t         *st_p         /* out */
)
{
    krb5_keytype           keytype;
    rsdb_named_item_t      person;
    rsdb_named_item_t      group;
    rsdb_named_item_t      org;
    sec_rgy_acct_key_t     name_parts;
    rsdb_acct_item_t       account;
    sec_rgy_cursor_t       cursor;
    error_status_t         lst;

    CLEAR_STATUS(st_p);
    if (!key_version) {
        SET_STATUS(st_p, sec_s_null_param_illegal);
        return;
    }
    *key_version=sec_passwd_c_version_none;
    keytype=sec_to_krb_keytype(key_type, &lst);
    if (BAD_STATUS(&lst)) {
        SET_STATUS(st_p,sec_rgy_key_bad_type);
        return;
    }

    /* Lookup the account information */
    name_parts = get_login_name_pgo(login_namep, &person, &group,
                                    &org, st_p);
    if (BAD_STATUS(st_p))
        return;
    cursor.valid = false;
    if (!find_account(true, &cursor, &RSDB_PGO(&person), &RSDB_PGO(&group),
                      &RSDB_PGO(&org), name_parts, &account)) {
        SET_STATUS(st_p, sec_rgy_object_not_found);
        return;
    }

    if (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE)
        *key_version=account.cur_key_version;
    else if (keytype == KEYTYPE_ENCRYPT_RSA_PRIV)
        *key_version=PK_PRIVKEY_VER_GET(&account);
    else {
        *key_version=sec_passwd_c_version_none;
        SET_STATUS(st_p,sec_rgy_key_bad_type);
    }
}

/* rsdb_acct_add_key_version
 *
 * Add a specific version of an account key.  If current is true
 * the key is added as the current version and "version_expiration"
 * is ignored (current keys are never garbage collected).  If
 * current is false, the key is stored as a back-version of the 
 * account's key using "version_expiration."  This routine accepts
 * only ciphertext keys.  Keys that arrived in plain_text are
 * converted to ciphertext by the 
 *
 * This routine was written to provide backend support for the  
 * rs_prop_acct_add_key_version operation, which is invoked by
 * a surrogate master when initializing a slave. This routine is 
 * never invoked during  "normal" key updates.
 *
 * DCE 1.0.2 restrictions:
 *
 * At 1.0.2, only a single keytype (DES) is supported.  This
 * means that this routine will never be called with current 
 * = true (the current DES key must have been supplied with the 
 * rs_prop_acct_add operation).  We make use of this situation
 *  to simplify the implementation but, when multiple key types 
 * are supported, this routine will have to be rewritten.
 *
 * DCE 1.2.2 loosening of restrictions:
 *
 * At 1.2.2, a single alternate keytype (RSA private key) is supported.
 * There must be only one version of this key per account, and the
 * version must be 1.  To indicate that an account has such an
 * associated keytype, the "rsdb_acct_item_privkey_exists" bit is set
 * in acct->flags.
 * 
 * ERRORS:
 *    sec_rgy_key_bad_version  - key version already exists or there is already
 *                                  a current version of this type key
 *    sec_rgy_key_bad_type     - key type not supported at this server. 
 *    sec_rgy_object_not_found - account does not exist
 */
PUBLIC  void  rsdb_acct_add_key_version
  (
    sec_rgy_login_name_t      *login_name,          /* [in] */
    sec_passwd_rec_t          *passwd,              /* [in] */
    boolean32                 current,              /* [in] */
    sec_timeval_sec_t         *version_expiration,  /* [in] */
    error_status_t            *status
  )
{
    rsdb_named_item_t         person;
    rsdb_named_item_t         group;
    rsdb_named_item_t         org;
    sec_rgy_acct_key_t        name_parts;
    rsdb_acct_item_t          account;
    sec_rgy_cursor_t          cursor;
    rsdb_authkey_item_t       auth_item;
    krb5_keytype              keytype;
    krb5_keyblock             key;
    krb5_encrypted_keyblock   ekey;

    keytype = sec_to_krb_keytype(PASSWD_TYPE(passwd), status);
    if (BAD_STATUS(status)) {
        /*
         * if the specified key type isn't supported then bail out immediately
         * Note that this includes a key type of sec_passwd_plain.  This routine
         * can only be used to add key versions that are already in ciphertext.
         */
        SET_STATUS(status, sec_rgy_key_bad_type);
    } else if (passwd->version_number == sec_passwd_c_version_none) {
        /* only specific key versions can be added by this routine */
        SET_STATUS(status, sec_rgy_key_bad_version);
    } else if ((PASSWD_TYPE(passwd) == sec_passwd_pubkey) && passwd->version_number != 1) {
        SET_STATUS(status, sec_rgy_key_bad_version);
    } else {
        /* lookup the account information */
        name_parts = get_login_name_pgo(login_name, &person, &group,
                    &org, status);
        if (GOOD_STATUS(status)) {
            cursor.valid = false;
            if (!find_account(true, &cursor, &RSDB_PGO(&person), &RSDB_PGO(&group),
                              &RSDB_PGO(&org), name_parts, &account)) {
                SET_STATUS(status, sec_rgy_object_not_found);
            }
        }
    }

    if (GOOD_STATUS(status)) {
        /*
         * if the key is to be added as the current key, but there is already
         * a current default key (only sec_passwd_des keys are
         * currently supported for default keys), then return an error status.
         */
        if ((keytype == RSDB_AUTH_DEFAULT_KEY_TYPE) &&
            current && (account.cur_key_version != sec_passwd_c_version_none)) {
            SET_STATUS(status, sec_rgy_key_bad_version);
        } else {  
            /* 
             * if a key with the specified version and keytype already exists,
             * then return an error status
             */
            if (!rsdb_auth_version_ok(passwd->version_number,
                                      keytype,
                                      account.threads.id, NULL,
                                      status)) {
                SET_STATUS(status, sec_rgy_key_bad_version);
            }
        }
    }

    if (BAD_STATUS(status)) {
        return;
    }

    /* 
     * We're done error checking, so let's get down to work! 
     */
    if (current) {
        /* 
         * This never happens in DCE 1.0.2 or DCE 1.2.2.  There is only a single
         * default key type supported and the account must have been previously
         * added with the current version of that key.  This makes 
         * threading versions much easier, since we can simply do it in 
         * arrival order, but this whole issue will have to be revisited 
         * if and when we add support for multiple versions of multiple key types.
         * It might be easiest to require that the first key of any type to arrive be
         * the current key.
         *
         * For now, panic because if we actually end up in this block, then
         * the slave's database is in a bad state.
         */
         dce_svc_printf(SEC_RSDB_ACCT_ADD_CURKEY_MSG);
    } else {
        /* 
         * This is either a back-version of the default keytype,
         * or is a private key.
         */
        if (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE) {
            auth_item.next_version = account.next_key_version;
            auth_item.pepper_len = ustrlen(passwd->pepper);
            auth_item.default_pepper = (passwd->pepper == NULL);
            auth_item.key_expire_time = *version_expiration;
            bcopy((char *)passwd->pepper, &auth_item.buf[0], auth_item.pepper_len);
            key.length = sec_passwd_c_des_key_size;
            key.contents = DES_PASSWD(passwd);
        } else {
            auth_item.next_version = sec_passwd_c_version_none;
            auth_item.pepper_len = 0;
            auth_item.default_pepper = true;
            auth_item.key_expire_time = *version_expiration;
            key.length = PUBKEY_PASSWD(passwd).len;
            key.contents = PUBKEY_PASSWD(passwd).data;
        }

        /* encrypt the key under the master_key */
        key.keytype = keytype;
        rs_mkey_encrypt_with_master(&key, &ekey, status);
        rsdb_get_master_key_version(&(account.mkey_version), status); 

        if (GOOD_STATUS(status)) {
            auth_item.key_len = ekey.length;  
            bcopy(ekey.contents, &auth_item.buf[auth_item.pepper_len],
                  ekey.length);
            rsdb_auth_store_by_authkey_key(passwd->version_number,
                                           keytype, 
                                           account.threads.id, &auth_item, 
                                           status);
            if (GOOD_STATUS(status)) {
                if (keytype == RSDB_AUTH_DEFAULT_KEY_TYPE)
                    account.next_key_version = passwd->version_number;
                store_acct_by_id(account.threads.id, &account);
            }
        }
    }
}


PRIVATE boolean32 is_legal_projlist_member
  (
    rsdb_acct_item_t  account,
    rsdb_member_id_t  member
  )
{
    rsdb_named_item_t group;

    rsdb_pgo_util_get_by_id(sec_rgy_domain_group, member.id, &group);
    if (!RSDB_IS_PROJLIST_OK(&RSDB_PGO(&group))) {
        return false;
    }

    return true;
}


PUBLIC void rsdb_acct_get_projlist
  (
    sec_rgy_login_name_t    *name,               /* [in] */
    sec_rgy_cursor_t        *(projlist_cursor),  /* [in, out] */
    signed32                max_num,             /* [in] */
    signed32                *(num_returned),     /* [out] */
    uuid_t                  *projlist,           /* [out] */
    signed32                *unix_projlist,      /* [out] */
    signed32                *num_projects,       /* [out] */
    error_status_t          *(status)
  )
{
    sec_rgy_acct_key_t          name_parts;
    rsdb_named_item_t           person;
    rsdb_named_item_t           group;
    rsdb_named_item_t           org;
    rsdb_credentials_t          credentials;
    rsdb_max_membership_list_t  list;
    rsdb_pvt_id_t               list_id;
    sec_rgy_cursor_t            acct_cursor;
    rsdb_acct_item_t            account;
    long                        physical_cursor,
                                logical_cursor,
                                index,
                                logical_index,
                                mem_count;

    CLEAR_STATUS(status);
    *num_returned = 0;

    name_parts = get_login_name_pgo(name, &person, &group, &org, status);
    if (BAD_STATUS(status))
        return;

    if (name_parts < sec_rgy_acct_key_person) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }

#ifdef notdef
    /*
     * This functionality has been disabled until the client side sets the 
     * information correctly.
     */
    if (!RSDB_IS_PROJLIST_OK(&RSDB_PGO(&person))) {
        /* This person does not support projlist queries */
        return;
    }
#endif

    acct_cursor.valid = false;
    if (!find_login_account(&acct_cursor, &RSDB_PGO(&person),
                            &RSDB_PGO(&group), &RSDB_PGO(&org),
                            name_parts, &account)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }

    /* assume initially that all groups this person is a member of can 
     * appear on a project list.  we'll decrement later whenever we
     * find a group that's disallowed on projlists.
     */
    *num_projects = RSDB_PGO(&person).num_members;
    list_id = RSDB_PGO(&person).membership;
    mem_count = 0;

    rsdb_util_get_member_list(sec_rgy_domain_person, list_id, &list);

    if (projlist_cursor->valid) {

        if (projlist_cursor->handle >= RSDB_PGO(&person).num_members) {
            SET_STATUS(status, sec_rgy_no_more_entries);
            return;
        }
 
        /* loop past all members already returned by a previous call with
         * this cursor. 
         *
         * the cursor starts as an offset into a logical  membership list 
         * that is actually a concatenation of one or more physical
         * rsdb_max_membership_list_t(s).  As we discover physical lists that
         * have  already been completely  processed by a previous query using
         * this cursor, we subtract the count of members in that list from the
         * cursor.  When we reach a physical list that has not been
         * (fully) processed by a previous call, the cursor is an offset
         * into that specific physical list
         */
        logical_cursor = projlist_cursor->handle;

        while ( mem_count < projlist_cursor->handle ) {

            if (list_id == END_OF_LIST) {
                dce_svc_printf(SEC_RSDB_ACCT_END_LIST_MSG);
            }

            mem_count += list.count;

            if (mem_count < projlist_cursor->handle) {

                /* check the current membership list for groups
                 * that don't belong on projlists
                 */
                for (index = 0; index < list.total_count; index++) {
                    if (list.member[index].deleted) {
                        continue;
                    }
                    if (!is_legal_projlist_member(account, 
                                                  list.member[index])) {
                        *num_projects -= 1;
                    }
                }

                list_id = list.next;
                rsdb_util_get_member_list(sec_rgy_domain_person, list_id, &list);
                logical_cursor -= list.count;

            }  /* else exit before next iteration */
        }  /* end while */

        /* now the cursor is relative to a specific physical membership
         * list, but it's still "logical" in the sense that some of
         * elements of that list may be "deleted" and thus need to be 
         * ignored when using the cursor as an index.  To obtain a 
         * physical cursor, we increment the logical_cursor for every deleted
         * element that appears before the logical cursor position 
         */
        for (physical_cursor = logical_cursor, index = 0, logical_index = 0;
             index < list.total_count, logical_index < logical_cursor;
             index++) {

            if (list.member[index].deleted) {
                physical_cursor += 1;
            } else {
                logical_index += 1;
                if (logical_index < logical_cursor) {
                    /* previously returned member */
                    if (!is_legal_projlist_member(account, 
                                                  list.member[index])) {
                        *num_projects--;
                    }
                }
            }
        }

    } else {  /* projlist cursor invalid - start with first member list */
        physical_cursor = 0;
        projlist_cursor->handle = physical_cursor;
    }

    projlist_cursor->valid = true;

    /* loop through the elements of the remaining physical membership
     * lists.  We have to loop through all of them even after we've 
     * filled the caller's projlist buffers because we have to return
     * a count of the total number of projects on every call.  This allows
     * the caller to recognize when they've gotten a complete projlist
     * set without possibly wasting a remote call to determine the
     * end of list, but it forces us to process the entire membership
     * list on every call.
     */ 
    while (list_id != END_OF_LIST) {

        for (index = physical_cursor; index < list.total_count; index++) {

            if (list.member[index].deleted == 1)
                continue;

            if (!is_legal_projlist_member(account, list.member[index])) {

                if (*num_returned < max_num) {
                    projlist_cursor->handle += 1;
                }

                *num_projects -= 1;
                continue;
            }

            if (*num_returned < max_num) {

                projlist_cursor->handle += 1;

                rsdb_pgo_util_get_by_id(sec_rgy_domain_group, list.member[index].id,
                                        &group);
                *unix_projlist++ = RSDB_PGO(&group).unix_id;

                if (!rsdb_get_credentials_by_unix_id(
                            domain_info[sec_rgy_domain_group].db, 
                            RSDB_PGO(&group).unix_id, &credentials)) {
                    dce_svc_printf(SEC_RSDB_CANT_GET_GROUP_CREDS_MSG);
                } else {
                    *projlist++ = credentials.uid;
                }

                *num_returned += 1;
            }
        } /* end for */

        list_id = list.next;
        if (list_id != END_OF_LIST) {
            rsdb_util_get_member_list(sec_rgy_domain_person, list_id, &list);
            physical_cursor = 0;
        }
    }
}


/*
 *  r s d b _ a c c t _ a u t h _ p o l i c y _ g e t _ i n f o
 *
 *  Get the auth policy for this account.
 *
 *  Status Codes:
 *      sec_rgy_object_not_found - auth policy not set for this account OR 
 *                                 this account doesn't exist
 *
 */
PUBLIC void rsdb_acct_auth_policy_get_info
  (
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [out] */
    error_status_t        *(status) 
  )
{
    sec_rgy_cursor_t      cursor;
    sec_rgy_acct_key_t    name_parts;
    rsdb_named_item_t     person;
    rsdb_named_item_t     group;
    rsdb_named_item_t     org;
    rsdb_acct_item_t      acct_item;

    CLEAR_STATUS(status);
    cursor.valid = false;  

    rsdb_acct_lookup_acct_id( account, &cursor, &name_parts,
            &person, &group, &org, &acct_item, status );
    if(BAD_STATUS(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    if(acct_item.auth_plcy.max_ticket_lifetime == 0 &&
       acct_item.auth_plcy.max_renewable_lifetime == 0) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    *auth_policy = acct_item.auth_plcy;
    return;
}
 

/*
 *  r s d b _ a c c t _ a u t h _ p o l i c y _ s e t _ i n f o
 *
 *  Set the auth policy for this account.
 *
 *  Note: future version may want to set acct last_changer, change_date.
 *
 *  Status Codes:
 *      sec_rgy_object_not_found - this account doesn't exist
 *
 */
PUBLIC void rsdb_acct_auth_policy_set_info
  (
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [in] */
    error_status_t        *(status) 
  )
{
    sec_rgy_cursor_t      cursor;
    sec_rgy_acct_key_t    name_parts;
    rsdb_named_item_t     person;
    rsdb_named_item_t     group;
    rsdb_named_item_t     org;
    rsdb_acct_item_t      acct_item;

    CLEAR_STATUS(status);
    cursor.valid = false;  

    rsdb_acct_lookup_acct_id( account, &cursor, &name_parts,
            &person, &group, &org, &acct_item, status );
    if(BAD_STATUS(status)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    acct_item.auth_plcy = *auth_policy;
    store_acct_by_id(acct_item.threads.id, &acct_item);
    return;
}
