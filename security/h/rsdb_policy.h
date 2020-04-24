/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_policy.h,v $
 * Revision 1.1.10.3  1996/02/18  23:00:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:56  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:28:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:02 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	merge.
 * 	[1995/03/29  15:01 UTC  hanfei  /main/SEC_migrate/1]
 * 
 * 	add rsdb_activate_xattrschema_acl_dce11()
 * 	[1995/03/28  20:15 UTC  hanfei  /main/hanfei_migrate_bl2/1]
 * 
 * Revision 1.1.6.2  1994/07/15  14:59:50  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:04  mdf]
 * 
 * Revision 1.1.6.1  1994/03/02  17:55:03  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:56  hanfei]
 * 
 * Revision 1.1.4.4  1992/12/29  13:26:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:40  zeliff]
 * 
 * Revision 1.1.4.3  1992/11/20  15:51:35  ahop
 * 	Make file_version a pointer in get_rgy_info_file_version
 * 	[1992/11/18  21:35:11  ahop]
 * 
 * 	Add rsdb_get_rgy_info_file_version
 * 	[1992/11/16  16:37:43  ahop]
 * 
 * 	Remove unused get_enc_master_key_by_vno
 * 	[1992/11/10  23:05:56  ahop]
 * 
 * Revision 1.1.4.2  1992/08/31  18:20:17  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:58:18  sekhar]
 * 
 * Revision 1.1.2.2  1992/03/30  16:31:57  ahop
 * 	 support locksmith mode
 * 	[1992/03/27  23:43:03  ahop]
 * 
 * Revision 1.1  1992/01/19  14:43:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/* 
 *      Registry Server - Policy management - function defs
 */

#ifndef rsdb_policy__included
#define rsdb_policy__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rsbase.h>

#include <rsdb_pvt.h>   

/* Definitions */
#define rsdb_policy_field_not_set   -1

/* 
* FUNCTIONS 
*/               
void rsdb_get_rgy_info_file_version (
#ifdef __STDC__
    long     *file_version        /* [out] */
#endif
);

rsdb_pvt_id_t rsdb_get_rgy_acl_id (
#ifdef __STDC__ 
    void
#endif
);


rsdb_pvt_id_t rsdb_get_replist_acl_id (
#ifdef __STDC__ 
    void
#endif
);
rsdb_pvt_id_t rsdb_get_attr_schema_acl_id (
#ifdef __STDC__ 
    void
#endif
);


void rsdb_properties_get_info ( 
#ifdef __STDC__
    sec_rgy_properties_t   *(properties),  /* [out] */
    error_status_t         *(status)       /* [out] */
#endif 
);
 
void rsdb_properties_set_info ( 
#ifdef __STDC__
    sec_rgy_properties_t   *(properties),  /* [in] */
    error_status_t         *(status)       /* [out] */
#endif
);

/*
 * Use to set the realm field in the properties record.
 * rsdb_properties_set_info ignores the realm field.
 */
void rsdb_properties_set_realm ( 
#ifdef __STDC__
    sec_rgy_name_t   (realm_name),      /* [in] */
    uuid_t           *realm_uuid,       /* [in] */
    error_status_t   *(status)          /* [out] */
#endif
);
   
void rsdb_properties_get_realm_name ( 
#ifdef __STDC__
    sec_rgy_name_t   (realm_name)       /* [in, out] */
#endif
);
   
void rsdb_get_my_server_name ( 
#ifdef __STDC__
    sec_rgy_name_t   (my_name)       /* [out] */
#endif
);

boolean32 rsdb_is_readonly ( 
#ifdef __STDC__ 
    void
#endif
);
 
boolean32 rsdb_prop_flag_is_set ( 
#ifdef __STDC__ 
    unsigned32  properties_flag       /* [in] */
#endif
);

void rsdb_policy_get_effective ( 
#ifdef __STDC__
    sec_rgy_name_t    org_name,       /* [in] */
    sec_rgy_plcy_t    *(policy),      /* [out] */
    error_status_t    *(status)       /* [out] */
#endif
);

void rsdb_policy_get_info ( 
#ifdef __STDC__
    sec_rgy_name_t    organization,   /* [in] */
    sec_rgy_plcy_t    *(policy_data), /* [out] */
    error_status_t    *(status)       /* [out] */
#endif
);

void rsdb_policy_set_info ( 
#ifdef __STDC__
    sec_rgy_name_t  organization,   /* [in] */
    sec_rgy_plcy_t  *(policy_data), /* [in] */
    error_status_t  *(status)       /* [out] */
#endif
);

void rsdb_policy_upd_num_attrs (
    rsdb_pvt_id_t       obj_id,         /* [in] */
    signed32            num            /* [in] */
);

void rsdb_policy_get_num_attrs (
    rsdb_pvt_id_t       obj_id,         /* [in] */
    signed32            *num            /* [out] */
);

void rsdb_policy_set_attr_list_id (
    rsdb_pvt_id_t       obj_id,         /* [in] */
    rsdb_pvt_id_t       attr_list_id    /* [in] */
);

void rsdb_policy_get_attr_list_id (
    rsdb_pvt_id_t       obj_id,         /* [in] */
    rsdb_pvt_id_t       *attr_list_id   /* [out] */
);

void rsdb_policy_set_less_restrictive (
#ifdef __STDC__
    sec_rgy_name_t  org_name,       /* [in] */
    sec_rgy_plcy_t  *(policy),      /* [in] */
    error_status_t  *(status)
#endif
);

void rsdb_policy_set_override_info ( 
#ifdef __STDC__
    sec_rgy_name_t               category,
    rs_policy_override_flags_t   p_d,
    error_status_t               *st
#endif
);

void rsdb_policy_get_override_info ( 
#ifdef __STDC__
    sec_rgy_name_t              category,
    rs_policy_override_flags_t  *p_d,
    error_status_t              *st
#endif
);

void rsdb_auth_policy_get_effective ( 
#ifdef __STDC__
    sec_rgy_login_name_t  *account,         /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [out] */
    error_status_t        *(status)        /* [out] */
#endif
);

void rsdb_auth_policy_get_info ( 
#ifdef __STDC__
    sec_rgy_login_name_t  *account,         /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy),  /* [out] */
    error_status_t        *(status)        /* [out] */
#endif
);

void rsdb_auth_policy_set_info ( 
#ifdef __STDC__
    sec_rgy_login_name_t  *account,        /* [in] */
    sec_rgy_plcy_auth_t   *(auth_policy), /* [in] */
    error_status_t        *(status)       /* [out] */
#endif
);

void rsdb_store_enc_master_key (
#ifdef __STDC__
    rsdb_encrypt_key_t  *encrypted_mkey,
    error_status_t      *st
#endif
);

void rsdb_get_enc_master_key (
#ifdef __STDC__
    rsdb_encrypt_key_t  *encrypted_mkey,
    error_status_t      *st
#endif
);

void rsdb_get_master_key_version (
#ifdef __STDC__
    krb5_kvno       *mkey_version,
    error_status_t  *st
#endif
);

void rsdb_store_new_enc_master_key (
#ifdef __STDC__
    rsdb_encrypt_key_t  *encrypted_mkey,
    error_status_t      *st
#endif
);

void rsdb_get_new_master_key_version (
#ifdef __STDC__
    krb5_kvno       *new_mkey_version,
    error_status_t  *st
#endif
);

void rsdb_activate_xattrschema_acl_dce11 (
    sec_id_foreign_t                rgy_creator,
    error_status_t                  *st
);


#endif
