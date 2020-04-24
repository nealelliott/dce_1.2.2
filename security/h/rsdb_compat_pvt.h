/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_compat_pvt.h,v $
 * Revision 1.1.10.1  1996/05/10  13:17:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/03/15  15:51 UTC  hanfei
 * 	rename rsdb_c_db_version_dce12 to rsdb_c_db_version_dce122.
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/14  18:55 UTC  hanfei
 * 	increment rsdb_c_db_version_current for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 *
 * 	fix rs_replica_mgt_item_dce11_t in rsdb_replica_item_dce11_t.
 * 	[1995/02/17  17:15 UTC  hanfei  /main/SEC_migrate/4]
 *
 * 	merge
 * 	[1995/02/14  20:21 UTC  hanfei  /main/SEC_migrate/3]
 *
 * 	fix rs_replica_mgt_item_dce102_t in rsdb_replica_item_dce102_t.
 * 	[1995/02/14  20:19 UTC  hanfei  /main/SEC_migrate/2]
 *
 * 	merge migration codes
 * 	[1995/02/07  14:53 UTC  hanfei  /main/SEC_migrate/1]
 *
 * 	work for rsdb replica compatibility to dce1.1
 * 	[1995/02/06  06:35 UTC  hanfei  /main/hanfei_comp_bl2/1]
 *
 * Revision 1.1.4.2  1994/08/25  16:30:11  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:52:20  hanfei]
 * 
 * Revision 1.1.4.1  1994/06/17  18:42:07  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:44  devsrc]
 * 
 * Revision 1.1.2.4  1993/03/26  14:40:55  rps
 * 	CR 7472 - Add rs_log_passwd_item_dce102_t for 1.0.1 null unix passwords
 * 	[1993/03/24  21:45:09  rps]
 * 
 * Revision 1.1.2.3  1993/02/19  17:12:07  ahop
 * 	initial revision
 * 	[1993/02/19  08:41:09  ahop]
 * 
 * Revision 1.1.2.2  1993/02/10  17:21:45  ahop
 * 	Initial revision.
 * 	[1993/02/03  21:33:04  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 * Registry Database Backwards Compatibility - private types
 */

#ifndef rsdb_compat_pvt_h__included
#define rsdb_compat_pvt_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/uuid.h>
#include <dce/aclbase.h>
#include <dce/daclmgr.h>
#include <rsdb_pvt.h>
#include <rsdb_replica.h>
#include <rsdb_acl.h>
#include <rs_log.h>
#include <rs_log_acl.h>
#include <rs_log_util.h>

/* General Database Versions */
typedef signed32 rsdb_compat_db_version_t;

#define rsdb_c_db_version_dce101    1
#define rsdb_c_db_version_dce102    6
#define rsdb_c_db_version_pre_migrate_dce11     7
#define rsdb_c_db_version_dce11     8
#define rsdb_c_db_version_dce122    9
#define rsdb_c_db_version_current   rsdb_c_db_version_dce122

/* 
 * Historical File Versions
 */
#define rs_c_state_file_version_dce101  3
#define rs_c_state_file_version_dce102  6
#define rs_c_rgy_file_version_dce101    1
#define rs_c_log_file_version_dce101    4


/*
 * Historical Replica States
 */
#define  rs_c_st_dce101_unknown_to_master   1
#define  rs_c_st_dce101_uninitialized       2
#define  rs_c_st_dce101_in_service          3
#define  rs_c_st_dce101_in_maintenance      4
#define  rs_c_st_dce101_closed              5
#define  rs_c_st_dce101_deleted             6
#define  rs_c_st_dce101_initializing        7


/*
 * Historical Database States
 */
#define rsdb_c_st_dce101_consistent         1
#define rsdb_c_st_dce101_renaming           2
#define rsdb_c_st_dce101_mkey_changing      3


/*
 * Historical Data Structures
 */

/* Rgy_state
 */
#define rgy_state_nreserved_dce101      24

typedef struct rgy_state_file_dce101_t {
    short           version;
    short           rep_state;      /* Replica states */
    short           db_state;       /* Database states */
    boolean32       replist_readonly;
    boolean32       master_known;
    uuid_t          master_id;
    sec_timeval_t   master_ts;      /* ts when became master */
    uuid_t          rs_id;          /* Identity of this server */
    uuid_t          replist_id;     /* Identity of the set of replicas */
    short           flags;          /* indicates which reserved bytes
                                     * are valid */
    sec_timeval_t   init_ts;
    char            reserved[rgy_state_nreserved_dce101];
} rgy_state_file_dce101_t;

/* Rgy_info 
 */
typedef struct rsdb_rgy_file_info_dce101_t {
    long                           file_version;
    sec_rgy_properties_t           properties;
    sec_rgy_plcy_t                 policy;
    sec_rgy_plcy_auth_t            auth_policy;
    rsdb_pvt_id_t                  sequential_key;
    sec_timeval_sec_t              domain_cache_state[sec_rgy_domain_last];
    rsdb_pvt_id_t                  acl;
    sec_rgy_name_t                 myname;          /* my server name */
    rsdb_encrypt_key_t             enc_master_key;  /* current version */
    rsdb_encrypt_key_t             old_enc_master_key;
} rsdb_rgy_file_info_dce101_t;

/* Log structures
 */
#define rs_log_padding_size_dce101     226

typedef struct rs_log_base_rec_dce101_t {
    unsigned long                  log_len;
    sec_timeval_t                  update_ts;
    sec_timeval_t                  checkpt_ts;
    sec_timeval_t                  prop_ts;
    short                          version;
    char                           padding[rs_log_padding_size_dce101];
} rs_log_base_rec_dce101_t;

typedef struct rs_log_hdr_dce101_t {
    short                          upd_reclen;
    unsigned char                  upd_module;
    unsigned char                  upd_op;
    sec_timeval_t                  upd_ts;
} rs_log_hdr_dce101_t;

typedef struct rs_log_rec_dce101_t {
    rs_log_hdr_dce101_t            upd_hdr;
    unsigned_char_t                upd_args[VARYING];
} rs_log_rec_dce101_t;

typedef struct rs_log_acct_item_dce101_t {
    rs_acct_parts_t                data_parts;
    unsigned32                     person_name_len;
    unsigned32                     group_name_len;
    unsigned32                     org_name_len;
    sec_rgy_acct_key_t             key_parts;
} rs_log_acct_item_dce101_t;

typedef struct rs_log_acct_add_dce101_t {
    rs_log_hdr_dce101_t            upd_hdr;
    sec_rgy_foreign_id_t           client_ids;
    rs_log_acct_item_dce101_t      item;
} rs_log_acct_add_dce101_t;

typedef struct rs_log_acct_rename_dce101_t {
    rs_log_hdr_dce101_t            upd_hdr;
    sec_rgy_acct_key_t             new_key_parts;
    unsigned32                     old_pname_len;
    unsigned32                     old_gname_len;
    unsigned32                     old_oname_len;
    unsigned32                     new_pname_len;
    unsigned32                     new_gname_len;
    unsigned32                     new_oname_len;
    char                           name_buf[2 * ACCT_NAME_BUFSIZE];
} rs_log_acct_rename_dce101_t;

typedef rsdb_acl_fixed_t rs_log_acl_item_dce102_t;

typedef struct rs_log_acl_replace_dce101_t {
    rs_log_hdr_dce101_t            upd_hdr;
    sec_acl_mgr_handle_t           sec_acl_mgr;
    rsdb_acl_ids_t                 sec_acl_key;
    uuid_t                         manager_type;
    sec_acl_type_t                 sec_acl_type;
    rs_log_acl_item_dce102_t       item;
} rs_log_acl_replace_dce101_t;


/* 
 * DCE 1.0.2 Structures - "copied" here because originals
 * are private to other modules
 */
#define rs_c_log_file_version_dce102    5
#define rs_c_log_base_rec_pad_sz_dce102 200
#define rs_c_log_read_buffsize_dce102   1024


typedef struct rsdb_rgy_file_info_dce102_t {
    long                           file_version;
    sec_rgy_properties_t           properties;
    sec_rgy_plcy_t                 policy;
    sec_rgy_plcy_auth_t            auth_policy;
    rsdb_pvt_id_t                  sequential_key;
    sec_timeval_sec_t              domain_cache_state[sec_rgy_domain_last];
    rsdb_pvt_id_t                  policy_acl_id;
    rsdb_pvt_id_t                  replist_acl_id;
    sec_rgy_name_t                 myname; /* my server name */
    rsdb_encrypt_key_t             enc_master_key;
} rsdb_rgy_file_info_dce102_t;


typedef struct  rs_log_base_rec_dce102_t {
    unsigned32          log_len;
    unsigned32          version;
    rs_update_seqno_t   update_seqno;
    sec_timeval_t       update_ts;
    rs_update_seqno_t   base_prop_seqno;
    char                padding[rs_c_log_base_rec_pad_sz_dce102];
}   rs_log_base_rec_dce102_t;

typedef struct rs_log_acct_item_dce102_t {
    rs_acct_parts_t     data_parts;
    unsigned32          person_name_len;
    unsigned32          group_name_len;
    unsigned32          org_name_len;
} rs_log_acct_item_dce102_t;

typedef struct rs_log_passwd_item_dce102_t {
    unsigned32          key_pickle_len;
    unsigned32          key_len;
    unsigned32          checksum_len;
    sec_etype_t         enc_type;
    sec_passwd_type_t   enc_keytype;
    sec_passwd_version_t enc_key_version;
    sec_timeval_sec_t   passwd_dtm;
} rs_log_passwd_item_dce102_t;

typedef struct rs_log_acct_add_dce102_t {
    rs_log_hdr_t                upd_hdr;
    sec_rgy_foreign_id_t        client_ids;
    sec_passwd_type_t           key_type;
    rs_log_acct_item_dce102_t   item;
} rs_log_acct_add_dce102_t;

typedef struct rs_log_acct_rename_dce102_t {
    rs_log_hdr_t       upd_hdr;
    unsigned32         old_pname_len;
    unsigned32         old_gname_len;
    unsigned32         old_oname_len;
    unsigned32         new_pname_len;
    unsigned32         new_gname_len;
    unsigned32         new_oname_len;
    char               name_buf[2 * ACCT_NAME_BUFSIZE];
} rs_log_acct_rename_dce102_t;

typedef struct rs_log_acl_replace_dce102_t {
    rs_log_hdr_t                upd_hdr;
    sec_acl_mgr_handle_t        sec_acl_mgr;
    unsigned32                  component_name_len;
    uuid_t                      manager_type;
    sec_acl_type_t              sec_acl_type;
    rs_log_acl_item_dce102_t    item;
} rs_log_acl_replace_dce102_t;


/* 
 * Followings are data structures for converting
 * dce 1.0.3 (same as dce 1.0.2) rsdb database
 * to dce 1.1 .
 * The rs_c_state_file_version associated are
 * changed from 6 to 7.
 */
typedef enum named_item_type_dce102_t {
                    rsdb_pgo_dce102_type, rsdb_dir_dce102_type } 
named_item_type_dce102_t;


typedef struct rsdb_pgo_item_dce102_t {
    rsdb_thread_hdr_t      threads;
    rsdb_pvt_id_t          membership;
    long                   num_members;
    rsdb_pvt_id_t          info;
    long                   unix_id;
    long                   reserved1;
    rsdb_pvt_id_t          reserved2;
    short                  fullname_len;
    sec_rgy_pgo_flags_t    flags;
    signed32               quota;
    sec_rgy_name_t         fullname;
} rsdb_pgo_item_dce102_t;


typedef struct rsdb_dir_item_dce102_t {
    rsdb_thread_hdr_t   threads;        /* ID and link list info            */
    uuid_t              uid;            /* this directory's UUID            */
    rsdb_pvt_id_t       acl;            /* ACL for this directory           */
    rsdb_pvt_id_t       def_dir_acl;    /* new directory default ACL        */
    rsdb_pvt_id_t       def_obj_acl;    /* new object default ACL           */
    rsdb_pvt_id_t       reserved;       /* reserved for future use          */
} rsdb_dir_item_dce102_t;


typedef struct rsdb_named_item_dce102_t {
    named_item_type_dce102_t    type;      /* PGO or DIR                   */
    union { rsdb_thread_hdr_t   threads;
            rsdb_pgo_item_dce102_t     pgo;
            rsdb_dir_item_dce102_t     dir;
    }                       body;
} rsdb_named_item_dce102_t;


typedef struct rsdb_acct_item_dce11_t {
    rsdb_thread_hdr_t      threads;
    rsdb_pvt_id_t          person_id;
    rsdb_pvt_id_t          group_id;
    rsdb_pvt_id_t          org_id;
    rsdb_pvt_id_t          login_activity_id; /*  replaces long padding1;  */
    sec_rgy_acct_key_t     key_parts;
    sec_rgy_acct_admin_t   admin_part;
    rsdb_acct_user_item_t  user_part;
    sec_rgy_plcy_auth_t    auth_plcy;
    rsdb_acct_item_flags_t flags;              
    krb5_kvno              cur_key_version;    /* always refers to DES keytype */
    krb5_kvno              next_key_version;   /* next most recent version in list */
    short                  default_pepper;     /* true if def pepper alg used */
    short                  key_pepper_len;
    short                  key_len;
    krb5_kvno              mkey_version;        /* mkey version used for encryption */
    char                   string_buf[RSDB_ACCT_STRING_BUF_SIZE];
} rsdb_acct_item_dce11_t;


typedef enum named_item_type_dce11_t {
        rsdb_pgo_type_dce11 = 0x00000001,
        rsdb_dir_type_dce11 = 0x00000002
} named_item_type_dce11_t;


typedef struct rsdb_named_item_dce11_t {
    int				type;	/* set of named_item_type_dce11_t */
    union { rsdb_thread_hdr_t   threads;
            rsdb_pgo_item_t     pgo;
            rsdb_dir_item_t     dir;
	    rsdb_pgodir_item_t	pgodir;
    }                       body;              
} rsdb_named_item_dce11_t;


typedef struct rsdb_sequential_key_dce11_t {
    short           record_type;
    rsdb_pvt_id_t   id;
} rsdb_sequential_key_dce11_t;


typedef struct rsdb_name_key_dce11_t {
    short           record_type;
    rsdb_pvt_id_t   parent_id;
    long            name_len;
    sec_rgy_name_t  name;
} rsdb_name_key_dce11_t;


typedef struct rs_replica_mgt_item_dce102_t {
    uuid_t                      rep_id;          /* instance uuid */
    rs_replica_name_p_t         rep_name;        /* (global) name service name */
    rs_replica_twr_vec_p_t      rep_twrs;        /* protocol towers for comm. w. replica */
    rs_replica_prop_t           prop_type;       /* prop type (init, update, bad_sw_rev, or delete) */
    uuid_t                      init_id;
    rs_rep_mgt_item_alloc_t     alloc_type; 
    rs_rep_prop_mgt_handle_t    prop_mgt;   /* replica's prop mgt info (volatile) */
} rs_replica_mgt_item_dce102_t;


typedef struct rsdb_replica_item_dce102_t {   
    rs_replica_mgt_item_dce102_t       replica; 
    unsigned32                  rep_buffers_len;
    rs_replica_packed_buff_t    rep_buffers;    
} rsdb_replica_item_dce102_t;


typedef struct rs_replica_mgt_item_dce11_t {
    uuid_t                      rep_id;          /* instance uuid */
    rs_replica_name_p_t         rep_name;        /* (global) name service name */
    rs_replica_twr_vec_p_t      rep_twrs;        /* protocol towers for comm. w. replica */
    rs_replica_prop_t           prop_type;       /* prop type (init, update, bad_sw_rev, or delete) */
    rs_replica_prop_t           prev_prop_type;  /* prev prop type (only if prop type is bad_sw_rev) */
    uuid_t                      init_id;
    rs_rep_mgt_item_alloc_t     alloc_type; 
    rs_rep_prop_mgt_handle_t    prop_mgt;   /* replica's prop mgt info (volatile) */
} rs_replica_mgt_item_dce11_t;


typedef struct rsdb_replica_item_dce11_t {   
    rs_replica_mgt_item_dce11_t       replica; 
    unsigned32                  rep_buffers_len;
    rs_replica_packed_buff_t    rep_buffers;    
} rsdb_replica_item_dce11_t;


#endif /* rs_compat_pvt_h__included */
