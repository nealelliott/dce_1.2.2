/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_acl.h,v $
 * Revision 1.1.11.1  1996/05/10  13:16:42  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  15:56 UTC  hanfei
 * 	multi-cell group work: modify macro GET_MGR_BY_DOMAIN to handle domain
 * 	foreign_person.
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 *
 * 	Merge work from greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:18 UTC  greg  /main/SEC_migrate/1]
 *
 * 	Add dynamic acl mgr configuration capability.
 * 	[1995/04/13  17:09 UTC  greg  /main/SEC_migrate/greg_sec_migrate_acls/1]
 *
 * Revision 1.1.4.6  1994/08/25  18:05:09  sekhar
 * 	[OT 11751 ] - Sec ACL Manager for SVCIF. Added 's' bit for
 * 	              SVC routing ( used by Policy ACL manager )
 * 	[1994/08/24  20:17:39  sekhar]
 * 
 * Revision 1.1.4.5  1994/06/02  21:16:09  mdf
 * 	Merged with changes from 1.1.4.4
 * 	[1994/06/02  21:15:59  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:22  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.4  1994/06/02  20:22:07  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:22  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.2  1994/04/22  19:13:52  rps
 * 	Add mgr_type arg to _get_id_from_item() for rgy polymorphism
 * 	[1994/04/21  00:07:10  rps]
 * 
 * Revision 1.1.4.1  1994/03/02  17:54:56  hanfei
 * 	Initial ERA drop
 * 	[1994/02/18  19:08:51  hanfei]
 * 
 * Revision 1.1.2.3  1992/12/29  13:25:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:12  zeliff]
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:57  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:57:56  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * RSDB_ACL.H: Macros and utilities supporting DACL manager.
 */

#ifndef rsdb_acl_h__included
#define rsdb_acl_h__included


#include <rsdb_pvt.h>
#include <rgy_obj_names.h>

/* Additional sec_acl permission definitions. */
#define sec_acl_perm_Delete_object  sec_acl_perm_unused_00000080
#define sec_acl_perm_name           sec_acl_perm_unused_00000100
#define sec_acl_perm_fullname       sec_acl_perm_unused_00000200
#define sec_acl_perm_mgmt_info      sec_acl_perm_unused_00000400
#define sec_acl_perm_auth_info      sec_acl_perm_unused_00000800
#define sec_acl_perm_user_info      sec_acl_perm_unused_00001000
#define sec_acl_perm_groups         sec_acl_perm_unused_00002000
#define sec_acl_perm_Member_list    sec_acl_perm_unused_00004000
#define sec_acl_perm_Admin          sec_acl_perm_unused_00008000
#define sec_acl_perm_Initialize     sec_acl_perm_unused_00010000

/*
 * Extended Registry Attributes support for additional Attribute
 * permission bits.
 */

#define sec_acl_perm_O     sec_acl_perm_unused_00020000
#define sec_acl_perm_P     sec_acl_perm_unused_00040000
#define sec_acl_perm_Q     sec_acl_perm_unused_00080000
#define sec_acl_perm_R     sec_acl_perm_unused_00100000
#define sec_acl_perm_S     sec_acl_perm_unused_00200000
#define sec_acl_perm_T     sec_acl_perm_unused_00400000
#define sec_acl_perm_U     sec_acl_perm_unused_00800000
#define sec_acl_perm_V     sec_acl_perm_unused_01000000
#define sec_acl_perm_W     sec_acl_perm_unused_02000000
#define sec_acl_perm_X     sec_acl_perm_unused_04000000
#define sec_acl_perm_Y     sec_acl_perm_unused_08000000
#define sec_acl_perm_Z     sec_acl_perm_unused_10000000
#define sec_acl_perm_svc   sec_acl_perm_unused_20000000


#define RSDB_ACL_PERMSET_RGY_V1_1             sec_acl_perm_read           | \
                                              sec_acl_perm_owner          | \
                                              sec_acl_perm_mgmt_info      | \
                                              sec_acl_perm_auth_info      | \
                                              sec_acl_perm_svc

#define RSDB_ACL_NUM_PERMS_RGY_V1_1   5

#define RSDB_ACL_PERMSET_RGY_V1_0_2           sec_acl_perm_read           | \
                                              sec_acl_perm_owner          | \
                                              sec_acl_perm_mgmt_info      | \
                                              sec_acl_perm_auth_info

#define RSDB_ACL_NUM_PERMS_RGY_V1_O_2  4

#define RSDB_ACL_PERMSET_PERSON               sec_acl_perm_read           | \
                                              sec_acl_perm_owner          | \
                                              sec_acl_perm_Delete_object  | \
                                              sec_acl_perm_name           | \
                                              sec_acl_perm_fullname       | \
                                              sec_acl_perm_mgmt_info      | \
                                              sec_acl_perm_auth_info      | \
                                              sec_acl_perm_user_info      | \
                                              sec_acl_perm_groups

#define RSDB_ACL_NUM_PERMS_PERSON   9

#define RSDB_ACL_PERMSET_GROUP                sec_acl_perm_read           | \
                                              sec_acl_perm_owner          | \
                                              sec_acl_perm_test           | \
                                              sec_acl_perm_Delete_object  | \
                                              sec_acl_perm_name           | \
                                              sec_acl_perm_fullname       | \
                                              sec_acl_perm_mgmt_info      | \
                                              sec_acl_perm_Member_list

#define RSDB_ACL_NUM_PERMS_GROUP    8

#define RSDB_ACL_PERMSET_ORG                  sec_acl_perm_read           | \
                                              sec_acl_perm_owner          | \
                                              sec_acl_perm_test           | \
                                              sec_acl_perm_Delete_object  | \
                                              sec_acl_perm_name           | \
                                              sec_acl_perm_fullname       | \
                                              sec_acl_perm_mgmt_info      | \
                                              sec_acl_perm_Member_list

#define RSDB_ACL_NUM_PERMS_ORG      8

#define RSDB_ACL_PERMSET_DIR                  sec_acl_perm_read           | \
                                              sec_acl_perm_owner          | \
                                              sec_acl_perm_insert         | \
                                              sec_acl_perm_delete         | \
                                              sec_acl_perm_Delete_object  | \
                                              sec_acl_perm_name
#define RSDB_ACL_NUM_PERMS_DIR      6

#define RSDB_ACL_PERMSET_REPLIST              sec_acl_perm_owner          | \
                                              sec_acl_perm_insert         | \
                                              sec_acl_perm_delete         | \
                                              sec_acl_perm_mgmt_info      | \
                                              sec_acl_perm_Admin          | \
                                              sec_acl_perm_Initialize
#define RSDB_ACL_NUM_PERMS_REPLIST  6

#define RSDB_ACL_PERMSET_ATTR_SCHEMA          sec_acl_perm_delete         | \
                                              sec_acl_perm_insert         | \
                                              sec_acl_perm_mgmt_info      | \
                                              sec_acl_perm_read           | \
                                              sec_acl_perm_owner
#define RSDB_ACL_NUM_PERMS_ATTR_SCHEMA  5

/*
 * Typedefs
 */
/* Acl Manager Types */
typedef enum rsdb_acl_mgr_type_t {
    rsdb_acl_mgr_type_rgy,
    rsdb_acl_mgr_type_person,
    rsdb_acl_mgr_type_group,
    rsdb_acl_mgr_type_org,
    rsdb_acl_mgr_type_dir,
    rsdb_acl_mgr_type_replist,
    rsdb_acl_mgr_type_attr_schema,
    rsdb_acl_mgr_type_last
} rsdb_acl_mgr_type_t;

/* Permission Type enums.  Used to index printstrings array. */
typedef enum rsdb_acl_mgr_perm_type_t {
    rsdb_acl_mgr_perm_read,
    rsdb_acl_mgr_perm_owner,
    rsdb_acl_mgr_perm_insert,
    rsdb_acl_mgr_perm_delete,
    rsdb_acl_mgr_perm_test,
    rsdb_acl_mgr_perm_Delete_object,
    rsdb_acl_mgr_perm_name,
    rsdb_acl_mgr_perm_fullname,
    rsdb_acl_mgr_perm_mgmt_info,
    rsdb_acl_mgr_perm_auth_info,
    rsdb_acl_mgr_perm_user_info,
    rsdb_acl_mgr_perm_groups,
    rsdb_acl_mgr_perm_Member_list,
    rsdb_acl_mgr_perm_Admin,
    rsdb_acl_mgr_perm_Initialize,
    rsdb_acl_mgr_perm_O,
    rsdb_acl_mgr_perm_P,
    rsdb_acl_mgr_perm_Q,
    rsdb_acl_mgr_perm_R,
    rsdb_acl_mgr_perm_S,
    rsdb_acl_mgr_perm_T,
    rsdb_acl_mgr_perm_U,
    rsdb_acl_mgr_perm_V,
    rsdb_acl_mgr_perm_W,
    rsdb_acl_mgr_perm_X,
    rsdb_acl_mgr_perm_Y,
    rsdb_acl_mgr_perm_Z,
    rsdb_acl_mgr_perm_svc,
    rsdb_acl_mgr_perm_last
} rsdb_acl_mgr_perm_type_t;

typedef struct rsdb_acl_ids_t {
    sec_rgy_domain_t        domain;
    rsdb_pvt_id_t           obj_id;
    rsdb_pvt_id_t           acl_id;
    uuid_t                  user_obj;
    uuid_t                  group_obj;
    unsigned32              acl_len;
} rsdb_acl_ids_t;

typedef struct rsdb_acl_mgr_info_t {
    unsigned32              num_perms;
    sec_acl_printstring_t   info_string;
} rsdb_acl_mgr_info_t;

/*
 * Global Variables
 */
/* defined in rsdb_acl.c */
extern uuid_t                   rsdb_acl_mgr_uuids[];
extern sec_acl_printstring_t    rsdb_acl_perm_strings[];
extern rsdb_acl_mgr_info_t      rsdb_acl_mgr_info[];


/*
 * Macros
 */
/* COPY_PRINT_STRING: (p) and (q) are pointers to type
 * sec_acl_printstring_t.  The values in (q) are copied to (p).
 */
#define COPY_PRINT_STRING(p, q) \
    u_strcpy((p)->printstring, (q)->printstring); \
    u_strcpy((p)->helpstring, (q)->helpstring); \
    (p)->permissions = (q)->permissions;


/* DB_ACL_ENTRY_LEN: calculates space required for an rsdb_acl_entry_t.
 * Now that names are no longer stored, this macro is pretty simple.
 * (e) is a pointer to sec_acl_entry_t
 */
#define DB_ACL_ENTRY_LEN(e) ( sizeof(rsdb_acl_entry_t) )


/* SEC_ACL_ENTRY_LEN: calculates space required for a sec_acl_entry_t.
 * (e) is a pointer to rsdb_acl_entry_t.
 * Extra char added to names for null-terminator.
 */
#define SEC_ACL_ENTRY_LEN(e) ( sizeof(sec_acl_entry_t) + \
    ((((e)->entry_type == sec_acl_e_type_foreign_user) || \
      ((e)->entry_type == sec_acl_e_type_foreign_group) || \
      ((e)->entry_type == sec_acl_e_type_for_user_deleg) || \
      ((e)->entry_type == sec_acl_e_type_for_group_deleg)) ? \
        ((e)->entry_info.foreign_id.id.name_len + 1 + \
         (e)->entry_info.foreign_id.realm.name_len + 1) : \
    ((((e)->entry_type == sec_acl_e_type_user) || \
      ((e)->entry_type == sec_acl_e_type_group) || \
      ((e)->entry_type == sec_acl_e_type_foreign_other) || \
      ((e)->entry_type == sec_acl_e_type_user_deleg) ||  \
      ((e)->entry_type == sec_acl_e_type_group_deleg) ||\
      ((e)->entry_type == sec_acl_e_type_for_other_deleg)) ? \
      ((e)->entry_info.id.name_len + 1) : 0 ) ) )


/* VALID_ENTRY_TYPE: (e) is a pointer to sec_acl_entry_t.
 * Note that sec_acl_e_type_extended is not considered VALID
 * to the registry acl manager.
 */
#define VALID_ENTRY_TYPE(e) ( \
    ( ((e)->entry_info.entry_type < sec_acl_e_type_user_obj)   || \
      ((e)->entry_info.entry_type > sec_acl_e_type_any_other_deleg)  || \
      ((e)->entry_info.entry_type == sec_acl_e_type_mask_obj)  || \
      ((e)->entry_info.entry_type == sec_acl_e_type_extended) ) ? \
      false : true )


/* KNOWN_ACL_TYPE: (a) is of type sec_acl_type_t.
 * Determine if (a) is a valid acl type.
 */
#define KNOWN_ACL_TYPE(a) ( ((a) < sec_acl_type_object && \
    (a) > sec_acl_type_default_container) ? false : true )

/* OBJECT_ACL_TYPE: (a) is of type sec_acl_type_t.
 * Determine if (a) is a valid acl type for an object-type
 * node (RGY, PERSON, GROUP, ORG).
 */
#define OBJECT_ACL_TYPE(a) ( (a) == sec_acl_type_object ? true : false )

/* CONTAINER_ACL_TYPE: (a) is of type sec_acl_type_t.
 * Determine if (a) is a valid acl type for a container-type node (DIR).
 */
#define CONTAINER_ACL_TYPE(a) KNOWN_ACL_TYPE((a))

/* OBJ_MGR_TYPE: (m) is of type rsdb_acl_mgr_type_t.
 * Return true if (m) is valid for acls of type sec_acl_type_object.
 */
#define OBJ_MGR_TYPE(m) ( (m) == rsdb_acl_mgr_type_rgy || \
    (m) == rsdb_acl_mgr_type_person || (m) == rsdb_acl_mgr_type_group || \
    (m) == rsdb_acl_mgr_type_org || (m) == rsdb_acl_mgr_type_dir || \
    (m) == rsdb_acl_mgr_type_replist || \
    (m) == rsdb_acl_mgr_type_attr_schema ? true : false )

/* DEF_OBJ_MGR_TYPE: (m) is of type rsdb_acl_mgr_type_t.
 * Return true if (m) is valid for acls of type sec_acl_type_default_object.
 */
#define DEF_OBJ_MGR_TYPE(m) ( (m) == rsdb_acl_mgr_type_person || \
    (m) == rsdb_acl_mgr_type_group || (m) == rsdb_acl_mgr_type_org ? \
    true : false )

/* DEF_CONT_MGR_TYPE: (m) is of type rsdb_acl_mgr_type_t.
 * Return true if (m) is valid for acls of type sec_acl_type_default_container.
 */
#define DEF_CONT_MGR_TYPE(m) ( (m) == rsdb_acl_mgr_type_dir ? true : false )

/* ACL_MGR_TYPES_MATCH: (a) is of type sec_acl_type_t.
 * (m) is of type rsdb_acl_mgr_type_t.
 * Return true if (m) is a valid mgr_type for the acl_type (a).
 */
#define ACL_MGR_TYPES_MATCH(a, m) ( (a) == sec_acl_type_object && \
    OBJ_MGR_TYPE(m) ? true : ((a) == sec_acl_type_default_object && \
    DEF_OBJ_MGR_TYPE(m) ? true : ((a) == sec_acl_type_default_container && \
    DEF_CONT_MGR_TYPE(m) ? true : false)) )


/* GET_DIR_ACL_ID: (i) is a pointer to a directory rsdb_named_item_t.
 * (a) is of type sec_acl_type_t.
 * Returns appropriate acl_id or NULL if invalid acl_type.
 */
#define GET_DIR_ACL_ID(i, a) ( RSDB_IS_DIR(i) ? \
    ((a) == sec_acl_type_object ? RSDB_DIR(i).acl : \
    ((a) == sec_acl_type_default_object ? RSDB_DIR(i).def_obj_acl : \
    ((a) == sec_acl_type_default_container ? RSDB_DIR(i).def_dir_acl : 0 ))) \
    : 0 )


/* GET_MGR_BY_DOMAIN: (d) is of type sec_rgy_domain_t.
 * This #define resolves to a type rsdb_acl_mgr_type_t.
 * If input domain is invalid, return an invalid mgr_type.
 */
#define GET_MGR_BY_DOMAIN(d) ( (d) == sec_rgy_domain_person ? \
    rsdb_acl_mgr_type_person : \
    ((d) == sec_rgy_domain_group ? rsdb_acl_mgr_type_group : \
    ((d) == sec_rgy_domain_org   ? rsdb_acl_mgr_type_org   : \
    ((d) == sec_rgy_domain_foreign_person   ? rsdb_acl_mgr_type_person   : \
    rsdb_acl_mgr_type_last))) )

/* KNOWN_MGR_TYPE: (m) is of type rsdb_acl_mgr_type_t
 * Return true if mgr type is in range of known values; false otherwise.
 */
#define KNOWN_MGR_TYPE(m) ( ((m) < rsdb_acl_mgr_type_rgy || \
    (m) >= rsdb_acl_mgr_type_last) ? false : true )


PUBLIC void rsdb_acl_create (
    void
);

PUBLIC void rsdb_acl_init (
    void
);


PUBLIC boolean32 rsdb_acl_get_mgr_type_from_uuid (
    uuid_t                  *uuid,
    rsdb_acl_mgr_type_t     *type,
    error_status_t          *st
);

PUBLIC boolean32 rsdb_acl_get_mgr_uuid_from_type (
    rsdb_acl_mgr_type_t     type,
    uuid_t                  *uuid,
    error_status_t          *st
);

PUBLIC void rsdb_acl_alloc_db_acl (
    sec_acl_t       *acl,
    rsdb_acl_t      **new_acl,
    error_status_t  *st
);

PUBLIC void rsdb_acl_alloc_sec_acl (
    rsdb_acl_t      *db_acl,
    sec_acl_t       **new_acl,
    unsigned32      *new_acl_len,
    error_status_t  *st
);

PUBLIC void rsdb_acl_copy_list_to_rpc_buf (
    rsdb_acl_ids_t  *acl_ids,
    sec_acl_list_t  *sec_acl_list,
    sec_acl_list_t  **new_acl_list,
    error_status_t  *st
);

PUBLIC void rsdb_acl_free_list (
    sec_acl_list_t  **sec_acl_list
);


PUBLIC rsdb_pvt_id_t rsdb_acl_create_default_acl (
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    rsdb_acl_mgr_type_t     mgr_type
);

PUBLIC rsdb_pvt_id_t rsdb_acl_copy (
    rsdb_pvt_id_t       acl_id,
    error_status_t      *st
);

PUBLIC void rsdb_acl_store_new_acl (
    rsdb_acl_t      *acl,
    rsdb_pvt_id_t   *acl_id
);

PUBLIC void rsdb_acl_store_by_id (
    rsdb_pvt_id_t   acl_id,
    rsdb_acl_t      *acl
);

PUBLIC boolean32 rsdb_acl_get_by_id (
    rsdb_pvt_id_t   acl_id,
    rsdb_acl_t      **acl,
    error_status_t  *st
);

PUBLIC void rsdb_acl_delete (
    rsdb_pvt_id_t   acl_id,
    error_status_t  *st
);

PUBLIC void rsdb_acl_get_ids_from_name (
    sec_acl_component_name_t    name,
    sec_acl_type_t              acl_type,
    uuid_t                      *mgr_type,
    sec_id_t                    **user_obj,
    sec_id_t                    **group_obj,
    rsdb_acl_ids_t              *acl_ids,
    error_status_t              *st
);

PUBLIC boolean32 rsdb_acl_get_id_from_item (
    sec_rgy_domain_t        domain,
    rsdb_named_item_t       *item,
    sec_acl_type_t          acl_type,
    rsdb_acl_mgr_type_t     mgr_type,
    rsdb_pvt_id_t           *acl_id,
    error_status_t          *st
);

PUBLIC void rsdb_acl_export (
    sec_acl_t       *sec_acl,
    rsdb_acl_t      *rsdb_acl,
    error_status_t  *st
);

PUBLIC void rsdb_acl_import (
    rsdb_acl_t      *rsdb_acl,
    sec_acl_t       *sec_acl,
    error_status_t  *st
);

PUBLIC void rsdb_acl_semantic_check (
    sec_acl_t                   *acl,
    sec_acl_type_t              acl_type,
    rsdb_acl_mgr_type_t         mgr_type,
    rsdb_acl_ids_t              *acl_ids,
    error_status_t              *st
);

PUBLIC void rsdb_acl_replace_entry (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              name,
    sec_acl_type_t              acl_type,
    uuid_t                      *mgr_type,
    rsdb_acl_entry_t            *new_entry,
    error_status_t              *st
);

PUBLIC void rsdb_acl_update_entry (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              name,
    sec_acl_type_t              acl_type,
    uuid_t                      *mgr_type,
    rsdb_acl_entry_t            *new_entry,
    boolean32                   add_bits,
    error_status_t              *st
);



PUBLIC boolean32 rsdb_acl_match_entries (
    rsdb_acl_entry_t            *entry1,
    rsdb_acl_entry_t            *entry2
);

PUBLIC boolean32 rsdb_acl_mgr_perms_valid (
        uuid_t			*acl_mgr_type,
        sec_acl_permset_t       permset,
        error_status_t          *st_p
);


/*
 * The following functions return the permission set
 * currently configured for an acl mgr type.
 * These used to be #defines rather than functions
 * but the need to conditionally and dynamically 
 * assign different permission sets to an acl manager
 * put a big dent in that world view.
 *
 * NOTE WELL:  The acl  mgr type in question must have
 * been configured prior to calling these functions.  
 * It is a programmimg error to do otherwise, and your process 
 * will die.
 *
 * Also, it's up to the caller to provide concurrency
 * control, but if you are referencing rsdb layer
 * abstractions, you're probably already
 * providing it.
 */
sec_acl_permset_t  rsdb_acl_permset_rgy ( void );
sec_acl_permset_t  rsdb_acl_permset_person ( void );
sec_acl_permset_t  rsdb_acl_permset_group ( void );
sec_acl_permset_t  rsdb_acl_permset_org ( void );
sec_acl_permset_t  rsdb_acl_permset_dir ( void );
sec_acl_permset_t  rsdb_acl_permset_replist ( void );
sec_acl_permset_t  rsdb_acl_permset_attr_schema ( void );

#endif
