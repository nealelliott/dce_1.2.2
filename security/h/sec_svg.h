/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg.h,v $
 * Revision 1.1.60.2  1996/10/03  14:52:32  arvind
 * 	CHFts19728: support salvage of krbtgt private key values
 * 	[1996/08/27  16:02 UTC  aha  /main/DCE_1.2.2/aha_dbpk1_1/1]
 *
 * 	CHFts19728: support salvaging of krbtgt private key
 * 	[1996/08/27  15:33 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	Modifications to allow salvage of krbtgt private key
 * 	[1996/05/24  23:15 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:46 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	mutli-cell group work: add cell and principal uuid in prt_list_item_t
 * 	and rec_member_list_item_t for foreign principal member pgo.
 * 	[1996/03/14  18:50 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:43  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/07/11  18:21 UTC  jrr  /main/HPDCE02/2]
 *
 * 	merge
 * 	HP 	[1994/10/27  13:47 UTC  hanfei  /main/hanfei_svg_bl1/1]
 *
 * 	work for ERA entries.
 * 	[1995/12/08  16:54:26  root]
 *
 * Revision 1.1.60.1  1996/05/10  13:18:20  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/14  18:50 UTC  hanfei
 * 	mutli-cell group work: add cell and principal uuid in prt_list_item_t
 * 	and rec_member_list_item_t for foreign principal member pgo.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Fix memory corruption problems
 * 	[1995/07/06  18:06 UTC  jrr  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	merge
 * 	[1994/10/27  15:31 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	work for ERA entries.
 * 	[1994/10/27  13:47 UTC  hanfei  /main/hanfei_svg_bl1/1]
 * 
 * Revision 1.1.54.1  1994/10/26  15:17:27  hanfei
 * 	ERA enhancement work.
 * 	[1994/10/25  18:31:05  hanfei]
 * 
 * Revision 1.1.52.1  1994/06/17  18:42:10  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:49  devsrc]
 * 
 * Revision 1.1.50.1  1993/10/05  22:30:24  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:10:51  mccann]
 * 
 * Revision 1.1.3.3  1993/03/23  23:07:39  hanfei
 * 	add codes for salvager reconstruct
 * 	[1993/03/23  22:50:28  hanfei]
 * 
 * Revision 1.1.3.2  1993/03/05  18:28:54  frisco
 * 	initial salvager functionality
 * 	[1993/03/02  21:01:35  frisco]
 * 
 * Revision 1.1.1.3  1992/11/25  20:57:36  frisco
 * 	Fix Revision History
 * 
 * Revision 1.1.1.2  1992/11/25  20:46:02  frisco
 * 	  Initial Revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

/*
 *      Registry Database Salvager Include file
 */

#ifndef sec_salvage__included
#define sec_salvage__included

#include <dce/rgynbase.h>
#include <rsdb_pvt.h>
#include <rs_rep_pvt.h>
#include <rsdb_acl.h>

#define TIME_STRING_LEN 17
#define LIFETIME_STRING_LEN 1024
#define MAX_ACL_PRINTSTRINGS 25
#define TOWER_MOD 24
#define TOWER_STRING_MAX_LEN 48
#define MAX_SIZE_ACL_ENTRY_STRING 1024

/*  .prt file names */

#define DB_BASE_DIRECTORY DCELOCAL_PATH "/var/security/"
#define PRT_DIRECTORY "rgy_print/"
#define DB_DIRECTORY "rgy_data/"

#define PRT_FILE_PRINC "princ.prt"
#define PRT_FILE_GROUP "group.prt"
#define PRT_FILE_ORG "org.prt"
#define PRT_FILE_MKEY ".mkey.prt"
#define PRT_FILE_POLICY "policy.prt"
#define PRT_FILE_RGY_STATE "rgy_state.prt"
#define PRT_FILE_REPLICAS "replicas.prt"
#define PRT_FILE_INFO "info.prt"
#define PRT_FILE_ATTR_SCHEMA "attr_schema.prt"
#define SCHEMA_NAME ""

#define INIT_HEAD_LIST(head, item) {head=NEW(item); head->next=NULL;}

PUBLIC sec_rgy_pname_t db_base_path;
PUBLIC sec_rgy_pname_t db_path;
PUBLIC sec_rgy_pname_t prt_base_path;
PUBLIC sec_rgy_pname_t prt_path;
PUBLIC rsdb_name_key_t  root_name_keys[sec_rgy_domain_last];

PUBLIC sec_rgy_properties_t properties;

PUBLIC boolean32             rs_verbose;


typedef struct prt_list_item_t {
    rsdb_pvt_id_t internal_id;
    sec_rgy_name_t name;
    long           name_len;
    uuid_t	   cell_uuid;
    uuid_t	   princ_uuid;
    struct prt_list_item_t *ptr;
}  prt_list_item_t;

typedef struct prt_rep_list_item_t {
    rs_replica_mgt_item_t *rep_info;
    struct prt_rep_list_item_t *ptr;
}  prt_rep_list_item_t;

typedef struct prt_authkey_list_item_t {
    krb5_kvno version;
    char pepper[rsdb_auth_pepper_max_len];
    short key_len;
    char key[rsdb_auth_encrypt_key_max_len];
    struct prt_authkey_list_item_t *ptr;
}  prt_authkey_list_item_t;

typedef struct rsdb_acl_string_t {
    sec_rgy_name_t realm;
    uuid_t realm_uuid;
    rsdb_acl_mgr_type_t mgr_type;
    int num_entries;
    char *entries[MAX_NUM_ACL_ENTRIES];
} rsdb_acl_string_t;

typedef struct rec_acl_list_item_t {
    rsdb_pvt_id_t acl_id;
    rsdb_acl_string_t *acl;
    struct rec_acl_list_item_t *ptr;
}  rec_acl_list_item_t;

typedef struct rec_acct_authkey_item_t {
    sec_passwd_rec_t           passwd;
    krb5_keyblock              dkey;
    sec_timeval_sec_t          expiration_date;
    struct rec_acct_authkey_item_t   *next;
} rec_acct_authkey_item_t;

typedef struct rec_acct_list_item_t {
    krb5_kvno               mkey_version;
    krb5_keyblock           ckey;
    sec_rgy_login_name_t    login_name;
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;
    sec_passwd_rec_t        passwd; 
    boolean32               gen_unix_passwd;
    sec_passwd_version_t    passwd_version_number;
    unsigned_char_p_t       creator_name;
    unsigned_char_p_t       changer_name;
    sec_rgy_plcy_auth_t     auth_policy;
    rec_acct_authkey_item_t      *auth_keys;
    struct rec_acct_list_item_t  *next;
} rec_acct_list_item_t;

typedef struct rec_member_list_item_t {
     sec_rgy_domain_t domain;
     char             *domain_name;
     char             *name;
     uuid_p_t	      cell_uuidp;
     uuid_p_t	      princ_uuidp;
     struct rec_member_list_item_t  *next;
} rec_member_list_item_t;

typedef struct rec_princ_list_item_t {
     unsigned_char_p_t               	name;
     uuid_t                 		id;
     struct rec_princ_list_item_t  	*next;
} rec_princ_list_item_t;

#endif

