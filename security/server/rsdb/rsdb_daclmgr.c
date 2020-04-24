/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_daclmgr.c,v $
 * Revision 1.1.15.1  1996/05/10  13:24:49  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/03/22  15:27 UTC  greg
 * 	1.2.2 migration fixes.
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  19:10 UTC  hanfei
 * 	fix bug in foreign group checks in sec__acl_mgr_access_check().
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:19 UTC  psn
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 *
 * 	Merge fix for CHFts14249
 * 	[1995/03/02  22:46 UTC  burati  /main/HPDCE02/1]
 *
 * 	Fix for CHFts14249 (Delegation ACL check changes)
 * 	[1995/03/01  19:51 UTC  burati  /main/burati_chfts14249/1]
 *
 * Revision 1.1.7.8  1994/09/20  22:25:35  max
 * 	CR 11303 fix.  Changed secd acl managers help and print strings
 * 	according to new guidelines.
 * 	[1994/09/20  22:23:49  max]
 * 
 * Revision 1.1.7.7  1994/09/20  15:31:19  burati
 * 	CR10924 Free cursor when done in sec_acl_mgr_get_access()
 * 	[1994/09/20  15:30:42  burati]
 * 
 * Revision 1.1.7.6  1994/08/04  16:14:44  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:44  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * 	hp_sec_to_osf_3 drop, removing cruft.
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  12:11:08  mdf]
 * 
 * Revision 1.1.7.3  1994/04/22  19:14:13  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:22:11  rps]
 * 
 * Revision 1.1.8.2  94/03/29  18:43:28  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.7.2  1994/03/02  17:56:05  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:52:36  hanfei]
 * 
 * Revision 1.1.7.1  1994/01/28  23:11:26  burati
 * 	Delegation changes - New acl entry types (dlg_bl1)
 * 	[1994/01/20  03:45:01  burati]
 * 
 * Revision 1.1.5.4  1992/12/29  16:38:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:17  zeliff]
 * 
 * Revision 1.1.5.3  1992/09/29  20:13:58  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:40:03  sekhar]
 * 
 * Revision 1.1.5.2  1992/08/31  18:28:17  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:12:49  sekhar]
 * 
 * Revision 1.1.2.3  1992/05/06  21:59:08  ahop
 * 	 CR 2893: Initialize certain [out]s to NULL so they aren't marshalled on error return
 * 	[1992/05/06  17:11:45  ahop]
 * 
 * Revision 1.1.2.2  1992/03/30  21:41:26  ahop
 * 	 fix CR 765: replace invalid_entry_name, name_resolution_failed with sec_acl_object_not_found
 * 	[1992/03/30  19:04:04  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1994, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * RSDB_DACLMGR: Back-end ACL data store and management
 *
 * Implementation notes: The sec_acl_key used in the following operations
 * is a pointer to an rsdb_acl_ids_t.  This structure is initialized
 * with a call to rsdb_acl_get_mgr_ids with input component name, acl_type,
 * (? and manager type).  Some of the input parameters in the following
 * calls are ignored because the interesting information has already
 * been processed and put into the key rsdb_acl_ids_t.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_daclmgr.c	25 - 12/18/91";
#endif

#include <dce/dce.h>
#include <dce/assert.h>
#include <sec_daclmgr.h>
#include <dce/uuid.h>
#include <dce/sec_cred.h>
#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_util.h>
#include <rsdb_policy.h>
#include <rs_util.h>
#include <rs_ver_pvt.h>
#include <rs_ver.h>
#include <bstring.h>
#include <stdio.h>
#include <u_str.h>
#include <initstring.h>
#include <sec_svc.h>

/* String constants for use in rsdb_acl_mgr_info[] */
#if defined(__STDC__) && !defined(_K_R_PORT_CHECK_)
    /* Acl Manager Print strings */
#   define SEC_POLICY_PSTRING       SEC_POLICY_OBJ_NAME
#   define SEC_PRINCIPAL_PSTRING    SEC_PRINCIPAL_OBJ_NAME
#   define SEC_GROUP_PSTRING        SEC_GROUP_OBJ_NAME
#   define SEC_ORG_PSTRING          "organization"
#   define SEC_DIR_PSTRING          "secdirectory"
#   define SEC_REPLIST_PSTRING      SEC_REPLIST_OBJ_NAME
#   define SEC_ATTR_SCHEMA_PSTRING  SEC_ATTR_SCHEMA_OBJ_NAME

    /* Acl Manager Help strings */
#   define SEC_POLICY_HSTRING       "Registry policy object ACL"
#   define SEC_PRINCIPAL_HSTRING    "Registry principal object ACLs"
#   define SEC_GROUP_HSTRING        "Registry group object ACLs"
#   define SEC_ORG_HSTRING          "Registry organization object ACLs"
#   define SEC_DIR_HSTRING          "Registry directory object ACLs"
#   define SEC_REPLIST_HSTRING      "Registry replica list object ACL"
#   define SEC_ATTR_SCHEMA_HSTRING  "Registry extended attribute schema object ACL"

#else
char *rsdb_acl_policy_pstring = NULL;
#   define SEC_POLICY_PSTRING \
    STRING_CONCAT(rsdb_acl_policy_pstring, "", SEC_POLICY_OBJ_NAME, "")

char *rsdb_acl_principal_pstring = NULL;
#   define SEC_PRINCIPAL_PSTRING \
    STRING_CONCAT(rsdb_acl_principal_pstring, "", SEC_PRINCIPAL_OBJ_NAME, "")

char *rsdb_acl_group_pstring = NULL;
#   define SEC_GROUP_PSTRING \
    STRING_CONCAT(rsdb_acl_group_pstring, "", SEC_GROUP_OBJ_NAME, "")

char *rsdb_acl_org_pstring = NULL;
#   define SEC_ORG_PSTRING \
    STRING_CONCAT(rsdb_acl_org_pstring, "", "organization" , "")

char *rsdb_acl_dir_pstring = NULL;
#   define SEC_DIR_PSTRING \
    STRING_CONCAT(rsdb_acl_dir_pstring, "", "secdirectory" , "")

char *rsdb_acl_replist_pstring = NULL;
#   define SEC_REPLIST_PSTRING \
    STRING_CONCAT(rsdb_acl_replist_pstring, "", SEC_REPLIST_OBJ_NAME, "")

char *rsdb_acl_attr_schema_pstring = NULL;
#   define SEC_ATTR_SCHEMA_PSTRING \
    STRING_CONCAT(rsdb_acl_attr_schema_pstring, "", SEC_ATTR_SCHEMA_OBJ_NAME, "")

char *rsdb_acl_policy_hstring = NULL;
#   define SEC_POLICY_HSTRING \
    STRING_CONCAT(rsdb_acl_policy_hstring, "", "Registry policy object ACL", "")

char *rsdb_acl_principal_hstring = NULL;
#   define SEC_PRINCIPAL_HSTRING \
    STRING_CONCAT(rsdb_acl_principal_hstring, "", "Registry principal object ACLs", "")

char *rsdb_acl_group_hstring = NULL;
#   define SEC_GROUP_HSTRING \
    STRING_CONCAT(rsdb_acl_group_hstring, "", "Registry group object ACLs", "")

char *rsdb_acl_org_hstring = NULL;
#   define SEC_ORG_HSTRING \
    STRING_CONCAT(rsdb_acl_org_hstring, "", "Registry organization object ACLs", "")

char *rsdb_acl_dir_hstring = NULL;
#   define SEC_DIR_HSTRING \
    STRING_CONCAT(rsdb_acl_dir_hstring, "", "Registry directory object ACLs", "")

char *rsdb_acl_replist_hstring = NULL;
#   define SEC_REPLIST_HSTRING \
    STRING_CONCAT(rsdb_acl_replist_hstring, "", "Registry list object ACL", "")

char *rsdb_acl_attr_schema_hstring = NULL;
#   define SEC_ATTR_SCHEMA_HSTRING \
    STRING_CONCAT(rsdb_acl_attr_schema_hstring, "", "Registry extended attribute schema object ACL" , "")
#endif

#define NUM_ACL_MGRS rsdb_acl_mgr_type_last
PUBLIC rsdb_acl_mgr_info_t  rsdb_acl_mgr_info[NUM_ACL_MGRS];

typedef struct permset_version_info_t {
    unsigned32         num_perms;
    sec_acl_permset_t  permset;
} permset_version_info_t;

/*
 * The indices of this array correspond to the software version constants
 * defined in rs_ver_pvt.h.  0 = unknown, 1 = 1.0.2, 2 = 1.1.  Given a
 * software version, one can find the permset info associated with that
 * software version by using the version as an index into the
 * permset_version_info_vec_t.
 */
typedef permset_version_info_t permset_version_info_vec_t[rs_c_num_software_vers];

typedef struct acl_mgr_version_info_t {
    unsigned32                  min_sw_version;
    unsigned32                  max_sw_version;
    permset_version_info_vec_t  permset_info;
} acl_mgr_version_info_t;
    
/* 
 * There is a permset version vector for each acl manager type.
 * As of 1.1, only the rgy acl manager type supports different 
 * permission sets at 1.1 than at 1.0.2. The array is indexed
 * by radb_acl_mgr_type.
 */
static acl_mgr_version_info_t sec_acl_mgr_type_info[NUM_ACL_MGRS] =
{
    /* rsdb_acl_mgr_type_rgy */
    {    
	rs_c_software_vers_1_0_2,
	rs_c_default_max_sw_vers,
	{ 
	    { 0, 0 }, 
	    { RSDB_ACL_NUM_PERMS_RGY_V1_O_2, RSDB_ACL_PERMSET_RGY_V1_0_2 },
	    { RSDB_ACL_NUM_PERMS_RGY_V1_1, RSDB_ACL_PERMSET_RGY_V1_1 },
	    { RSDB_ACL_NUM_PERMS_RGY_V1_1, RSDB_ACL_PERMSET_RGY_V1_1 }
	}
    },

    /* rsdb_acl_mgr_type_person */
    {
	rs_c_software_vers_1_0_2,
	rs_c_default_max_sw_vers,
	{
	    { 0, 0 }, 
	    { RSDB_ACL_NUM_PERMS_PERSON, RSDB_ACL_PERMSET_PERSON },
	    { RSDB_ACL_NUM_PERMS_PERSON, RSDB_ACL_PERMSET_PERSON },
	    { RSDB_ACL_NUM_PERMS_PERSON, RSDB_ACL_PERMSET_PERSON }
	}
    },

    /* rsdb_acl_mgr_type_group */
    {
	rs_c_software_vers_1_0_2,
	rs_c_default_max_sw_vers,
	{
	    { 0, 0 }, 
	    { RSDB_ACL_NUM_PERMS_GROUP, RSDB_ACL_PERMSET_GROUP },
	    { RSDB_ACL_NUM_PERMS_GROUP, RSDB_ACL_PERMSET_GROUP },
	    { RSDB_ACL_NUM_PERMS_GROUP, RSDB_ACL_PERMSET_GROUP }
	}
    },

    /* rsdb_acl_mgr_type_org */
    {
	rs_c_software_vers_1_0_2,
	rs_c_default_max_sw_vers,
	{
	    { 0, 0 }, 
	    { RSDB_ACL_NUM_PERMS_ORG, RSDB_ACL_PERMSET_ORG },
	    { RSDB_ACL_NUM_PERMS_ORG, RSDB_ACL_PERMSET_ORG },
	    { RSDB_ACL_NUM_PERMS_ORG, RSDB_ACL_PERMSET_ORG }
	}
    },

    /* rsdb_acl_mgr_type_dir */
    {
	rs_c_software_vers_1_0_2,
	rs_c_default_max_sw_vers,
	{
	    { 0, 0 }, 
	    { RSDB_ACL_NUM_PERMS_DIR, RSDB_ACL_PERMSET_DIR },
	    { RSDB_ACL_NUM_PERMS_DIR, RSDB_ACL_PERMSET_DIR },
	    { RSDB_ACL_NUM_PERMS_DIR, RSDB_ACL_PERMSET_DIR }
	}
    },

    /* rsdb_acl_mgr_type_replist */
    {
	rs_c_software_vers_1_0_2,
	rs_c_default_max_sw_vers,
	{
	    { 0, 0 }, 
	    { RSDB_ACL_NUM_PERMS_REPLIST, RSDB_ACL_PERMSET_REPLIST },
	    { RSDB_ACL_NUM_PERMS_REPLIST, RSDB_ACL_PERMSET_REPLIST },
	    { RSDB_ACL_NUM_PERMS_REPLIST, RSDB_ACL_PERMSET_REPLIST }
	}
    },

    /* rsdb_acl_mgr_type_attr_schema */
    {
	rs_c_software_vers_1_1,
	rs_c_default_max_sw_vers,
	{
	    { 0, 0 }, 
	    { RSDB_ACL_NUM_PERMS_ATTR_SCHEMA, RSDB_ACL_PERMSET_ATTR_SCHEMA },
	    { RSDB_ACL_NUM_PERMS_ATTR_SCHEMA, RSDB_ACL_PERMSET_ATTR_SCHEMA },
	    { RSDB_ACL_NUM_PERMS_ATTR_SCHEMA, RSDB_ACL_PERMSET_ATTR_SCHEMA }
	}
    }
};
    
/*
 * software version supported by acl manager.
 * Initialized to "unknown" and set by data
 * supplied in the sec_acl_mgr_configure call
 */
static sec_acl_mgr_software_version = rs_c_software_vers_unknown;

#define CHECK_ACL_MGR_VERSION(type,stp) \
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown); \
    ((sec_acl_mgr_software_version < sec_acl_mgr_type_info[(type)].min_sw_version) \
    || (sec_acl_mgr_software_version > sec_acl_mgr_type_info[(type)].max_sw_version)) \
    ? SET_STATUS((stp), sec_acl_unknown_manager_type) \
    : CLEAR_STATUS((stp))

/* Private definitions, mostly for efficiency of auth checks */
#define ENTRY_NOT_FOUND -1
#define MAX_PERMISSIONS ~0

/* Compare default realm with accessor's realm */
#define default_realm_eq(entry) uuid_equal(&paP->realm.uuid, \
                                        &sec_acl_p->default_realm.uuid, \
                                        &st)

/* Compare foreign realm with accessor's realm */
#define foreign_realm_eq(entry) uuid_equal(&paP->realm.uuid, \
  &sec_acl_p->sec_acl_entries[entry].entry_info.tagged_union.foreign_id.realm.uuid, \
  &st)

static  void  config_mgr_info (
    rsdb_acl_mgr_type_t   mgr_type,    /* [in] */
    unsigned32            version,     /* [in] */
    char *                infostring,  /* [in] */
    char *                helpstring   /* [in] */
);

/* g e t _ a c c e s s
 *
 * Private routine to mask entry permset against specified mask
 * and return the set of granted permissions.  If no permissions are
 * granted, return false; otherwise return true.
 */
PRIVATE boolean32 get_access (
    sec_acl_entry_t     entry,
    sec_acl_permset_t   mask_perms,
    sec_acl_permset_t   *granted
)
{
    switch (entry.entry_info.entry_type) {
        case sec_acl_e_type_group_obj:
        case sec_acl_e_type_any_other:
        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_group_obj_deleg:
        case sec_acl_e_type_any_other_deleg:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:
        case sec_acl_e_type_extended:
            *granted |= (entry.perms & mask_perms);
            break;
        case sec_acl_e_type_user_obj:
        case sec_acl_e_type_other_obj:
        case sec_acl_e_type_unauthenticated:
        case sec_acl_e_type_user_obj_deleg:
        case sec_acl_e_type_other_obj_deleg:
            *granted |= entry.perms;
            break;
        default:
	    break;
    }
    if ( *granted == (sec_acl_permset_t) 0 ) {
        return false;
    } else {
        return true;
    }
}


/* s e c _ a c l _ m g r _ c o n f i g u r e
 *
 *  Configure the acl manager.  This operation provides a handle that
 *  refers to the particular acl database in use and is necessary for all
 *  other acl manager operations.  The db_name identifies which acl
 *  database to use (and generally refers to the file system object that
 *  represents the persistent storage for the database).  The config_info
 *  provides information on how to configure this database.
 */
/* Implementation Note:  Since there is only one acl manager and only
 * one acl database in the current implementation, this operation is
 * a no-op.  config_info is ignored, db_name is ignored, and a NULL
 * sec_acl_mgr_handle is returned.
 */
PUBLIC void sec_acl_mgr_configure (
    sec_acl_mgr_config_t    config_info,
    unsigned32              version,
    unsigned_char_p_t       db_name,
    sec_acl_mgr_handle_t    *sec_acl_mgr,
    error_status_t          *st
)
{
    CLEAR_STATUS(st);
    *sec_acl_mgr = NULL;

    /* 
     * If the caller gave us a bad software version it's an 
     * unrecoverable internal programming error and so we die.
     */
    dce_assert(sec_svc_handle, version >= rs_c_min_software_vers && version <= rs_c_max_software_vers);

    /*
     * Set static global version variable so other routines know 
     * the current version.  No mutex needed because we assume that
     * all calls to the acl mgr library are protected by database
     * locks at the rs_ layer
     */
    sec_acl_mgr_software_version = version;

    /*
     * Initialize acl manager info
     */
    config_mgr_info(rsdb_acl_mgr_type_rgy, version,
		    SEC_POLICY_PSTRING, SEC_POLICY_HSTRING);
    
    config_mgr_info(rsdb_acl_mgr_type_person, version,
		    SEC_PRINCIPAL_PSTRING, SEC_PRINCIPAL_HSTRING);

    config_mgr_info(rsdb_acl_mgr_type_group, version,
		    SEC_PRINCIPAL_PSTRING, SEC_PRINCIPAL_HSTRING);

    config_mgr_info(rsdb_acl_mgr_type_org, version, 
		    SEC_ORG_PSTRING, SEC_ORG_HSTRING);

    config_mgr_info(rsdb_acl_mgr_type_dir, version,
		    SEC_DIR_PSTRING, SEC_DIR_HSTRING);

    config_mgr_info(rsdb_acl_mgr_type_replist, version,
		    SEC_REPLIST_PSTRING, SEC_REPLIST_HSTRING);

    config_mgr_info(rsdb_acl_mgr_type_attr_schema, version,
		    SEC_ATTR_SCHEMA_PSTRING, SEC_ATTR_SCHEMA_HSTRING);

    return;
}


/*
 * In order to prevent having to traverse the list of ACL entries more
 * than once per identity, we go through the entire list once per identity
 * and keep track of where we find the following types, by storing their
 * index from the ACL entry array in a locally defined array of indices
 */

/* Which array index corresponds to which entry type: */
#define MASK_POS            0
#define USER_OBJ_ENTRY      1
#define USER_ENTRY          2
#define FOREIGN_USER_ENTRY  3
#define OTHER_OBJ_ENTRY     4
#define FOREIGN_OTHER_ENTRY 5
#define ANY_OTHER_ENTRY     6
#define UNAUTH_ENTRY        7
#define USER_OBJ_DELEG      8
#define USER_DELEG          9
#define F_USER_DELEG        10
#define OTHER_OBJ_DELEG     11
#define F_OTHER_DELEG       12
#define ANY_OTHER_DELEG     13
#define POS_ARRAY_LEN       14
signed32 entry_pos[POS_ARRAY_LEN];

PRIVATE  void  sec__acl_mgr_match_entries
    (
    boolean32         delegates,
    sec_acl_permset_t *mask_perms_p,
    sec_acl_t         *sec_acl_p,
    sec_id_pa_t       *paP
    )
{
    int                 i;              /* For traversing entry list. */
    error_status_t      st;

    /* If masks isn't explicitly set, then it should have no effect
        when masking, so turn all perm bits on by default */
    *mask_perms_p = MAX_PERMISSIONS;

    /* PRE-PROCESS the acl entries so we only have to loop 
     * through once looking for specific types of entries
     *
     * Note, the accessor may be a member of multiple {foreign_}groups.
     * Therefore, the *group checks must be done below, in a separate
     * pass through the entry list.
     */
    memset(entry_pos, ENTRY_NOT_FOUND, POS_ARRAY_LEN*sizeof(*entry_pos));
    for (i = 0; i < sec_acl_p->num_entries; i++) {

        /* Check for existence of each type of entry, and keep track
         * of where each type was found in the entry list.
         * Don't mark type as found if the associated realm id's
         * are not the same
         */
        switch(sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
            case sec_acl_e_type_user_obj:
                if (default_realm_eq(i))
                    entry_pos[USER_OBJ_ENTRY] = i;
                break;
            case sec_acl_e_type_user:
                if ((uuid_equal(&paP->principal.uuid,
                                &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id.uuid,
                                &st)) && (default_realm_eq(i)))
                    entry_pos[USER_ENTRY] = i;
                break;
            case sec_acl_e_type_foreign_user:
                if ((uuid_equal(&paP->principal.uuid,
                                &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id.uuid,
                                &st)) && (foreign_realm_eq(i)))
                    entry_pos[FOREIGN_USER_ENTRY] = i;
                break;
            case sec_acl_e_type_other_obj:
                if (default_realm_eq(i))
                    entry_pos[OTHER_OBJ_ENTRY] = i;
                break;
            case sec_acl_e_type_foreign_other:
                if ( uuid_equal( &paP->realm.uuid,
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id.uuid,
                        &st))
                   entry_pos[FOREIGN_OTHER_ENTRY] = i;
                break;
            case sec_acl_e_type_any_other:
                entry_pos[ANY_OTHER_ENTRY] = i;
                break;
            case sec_acl_e_type_unauthenticated:
                entry_pos[UNAUTH_ENTRY] = i;
                break;
            default:
                break;
        }
        if (delegates) {
            switch(sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
            case sec_acl_e_type_user_obj_deleg:
                if (default_realm_eq(i))
                    entry_pos[USER_OBJ_DELEG] = i;
                break;
            case sec_acl_e_type_user_deleg:
                if ((uuid_equal(&paP->principal.uuid,
                    &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id.uuid,
                    &st)) && (default_realm_eq(i)))
                    entry_pos[USER_DELEG] = i;
                break;
            case sec_acl_e_type_for_user_deleg:
                if ((uuid_equal(&paP->principal.uuid,
                    &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id.uuid,
                    &st)) && (foreign_realm_eq(i)))
                    entry_pos[F_USER_DELEG] = i;
                break;
            case sec_acl_e_type_other_obj_deleg:
                if (default_realm_eq(i))
                    entry_pos[OTHER_OBJ_DELEG] = i;
                break;
            case sec_acl_e_type_for_other_deleg:
                if ( uuid_equal( &paP->realm.uuid,
                    &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id.uuid,
                    &st))
                    entry_pos[F_OTHER_DELEG] = i;
                break;
            case sec_acl_e_type_any_other_deleg:
                entry_pos[ANY_OTHER_DELEG] = i;
                break;
            }
        }
    }
}


/*
 * Internal routine to perform actual access checking against entries.
 */
PRIVATE boolean32 sec__acl_mgr_access_check(
    sec_acl_t         *sec_acl_p,
    sec_acl_permset_t mask_perms,
    sec_id_pa_t       *paP,
    sec_id_t          *user_obj,
    sec_id_t          *group_obj,
    sec_acl_permset_t desired,
    sec_acl_permset_t *net_rights,
    boolean32         auth,
    error_status_t    *st_p
)
{
    error_status_t      st;
    sec_acl_permset_t   group_access, granted = (sec_acl_permset_t)0;
    int                 i;              /* For traversing entry list. */
    unsigned int        j;              /* For traversing entry list. */
    sec_id_t            *group_id, *realm_id;
    boolean32           chk_loc_groups = false, one_group_found = false;
    boolean32           got_access = false;
    
    /* USER_OBJ check */
    if (entry_pos[USER_OBJ_ENTRY] != ENTRY_NOT_FOUND) {

        /* If e_type_user_obj entry exists, then user_obj can't be NULL */
        if (user_obj == NULL) {
            SET_STATUS(st_p, sec_acl_expected_user_obj);
            return false;
        }

        /* If the id assoc w/ user_obj matches the principal id */
        if (uuid_equal(&paP->principal.uuid,
                        &user_obj->uuid,
                        &st)) {
            /* then check the permsets to see if access is granted */
            if (get_access(
                    sec_acl_p->sec_acl_entries[entry_pos[USER_OBJ_ENTRY]],
                    mask_perms, &granted))
                got_access = true;
            else {
                return false;       /* implied denial rights */
            }
        }
    }

    /* USER check */
    if ((! got_access) && (entry_pos[USER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see what access is granted */
        if (get_access(sec_acl_p->sec_acl_entries[entry_pos[USER_ENTRY]],
                            mask_perms, &granted))
            got_access = true;
        else {
            return false;           /* implied denial rights */
        }
    }

    /* FOREIGN_USER check */
    if ((! got_access) && (entry_pos[FOREIGN_USER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_USER_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }

    /* GROUP checks */
    group_access = (sec_acl_permset_t) 0;
    for (i = 0;((! got_access) && (i < sec_acl_p->num_entries)); i++) {
        switch(sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
            case sec_acl_e_type_group_obj:
            case sec_acl_e_type_group:
            case sec_acl_e_type_foreign_group:
                if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type
                                                == sec_acl_e_type_group_obj) {
                    /* If group_obj entry exists then group_obj cant be NULL */
                    if (group_obj == NULL) {
                        SET_STATUS(st_p, sec_acl_expected_group_obj);
                        return false;
                    }
                    group_id = group_obj;   /* check against group_obj param */
                    realm_id = &sec_acl_p->default_realm;
                    chk_loc_groups = default_realm_eq(i);
                } else if (sec_acl_p->sec_acl_entries[i].entry_info.entry_type
                                                    == sec_acl_e_type_group) {
                    chk_loc_groups = default_realm_eq(i);
                    group_id = 
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id;
                    realm_id = &sec_acl_p->default_realm;
                } else {
                    chk_loc_groups = foreign_realm_eq(i);
                    group_id = 
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id;
                    realm_id =
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.realm;
                }
                /* Check either the local groups or the foreign groups */
                if (chk_loc_groups) {
                    /* CHECK PAC GROUP */
                    if (uuid_equal(&paP->group.uuid,
                                    &group_id->uuid, &st) ) {

                        one_group_found = true;
                        /* then check the perms to see if access is granted */
                        if (get_access(sec_acl_p->sec_acl_entries[i],
                                            mask_perms, &granted))
                            group_access = (group_access | granted);
                    }
                    /* CHECK LOCAL GROUPS */
                    for (j = 0; j < paP->num_groups; j++) {

                        if (uuid_equal(&paP->groups[j].uuid,
                                        &group_id->uuid, &st) ) {

                            one_group_found = true;
                            /* check the perms to see if access granted */
                            if (get_access(sec_acl_p->sec_acl_entries[i],
                                                mask_perms, &granted))
                                group_access = (group_access | granted);
                        }
                    }
                } else {
                    /* CHECK FOREIGN GROUPS */
		    int k;
		    /* For each foreign groupset that the caller belongs to */
                    for (j = 0; j < paP->num_foreign_groupsets; j++) {

                      /* If the entry's realm matches this groupset's realm */
                      if (uuid_equal(&paP->foreign_groupsets[j].realm.uuid,
                                      &realm_id->uuid, &st))

                        /* Then check each group within that foreign realm */
                        for (k = 0; k < paP->foreign_groupsets[j].num_groups;
                             k++) {
                            if (uuid_equal(
                                &paP->foreign_groupsets[j].groups[k].uuid,
                                &group_id->uuid, &st)) {

                                one_group_found = true;
                                /* check the perms to see if access granted */
                                if (get_access(sec_acl_p->sec_acl_entries[i],
                                        mask_perms, &granted))
                                    group_access = (group_access | granted);
                            }

                        }
		    }
		}
                break;
            default:
                break;
        } /* switch (entry type) */

        /* See if the union of multiple group entries granted access */
        if ((group_access & desired) == desired) {
            got_access = true;
            granted = group_access;
        }
    } /* GROUP check */

    /* If at least 1 group found, and no group permissions
     * were granted, then deny any access
     */
    if (!got_access && one_group_found) {
        if (group_access == (sec_acl_permset_t)0) {
            return false;
        } else {
            got_access = true;
            granted = group_access;
        }
    }

    /* OTHER_OBJ check */
    if ((! got_access) && (entry_pos[OTHER_OBJ_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[OTHER_OBJ_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }

    /* FOREIGN_OTHER check */
    if ((!got_access) && (entry_pos[FOREIGN_OTHER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_OTHER_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }

    /* ANY_OTHER check */
    if ((! got_access) && (entry_pos[ANY_OTHER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[ANY_OTHER_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }

    /* UNAUTHENTICATED check
     * If pa_data isn't authenticated and access was granted by one of the
     * above checks, then desired_access must be masked by the unauthenticated
     * entry.
     */
    if ((got_access) && (! auth)) {
        if (entry_pos[UNAUTH_ENTRY] == ENTRY_NOT_FOUND) {
            got_access = false;
        } else {		
            granted &=
                sec_acl_p->sec_acl_entries[entry_pos[UNAUTH_ENTRY]].perms;
        }
    }
    if (got_access) {
        *net_rights = granted;
    }
    return got_access;
}


/*
 * Internal routine to perform actual access checking against entries.
 */
PRIVATE boolean32 sec__acl_mgr_deleg_access_check(
    sec_acl_t         *sec_acl_p,
    sec_acl_permset_t mask_perms,
    sec_id_pa_t       *paP,
    sec_id_t          *user_obj,
    sec_id_t          *group_obj,
    sec_acl_permset_t desired_access,
    sec_acl_permset_t *net_rights,
    boolean32         auth,
    error_status_t    *st_p
)
{
    error_status_t      st;
    sec_acl_permset_t   group_access, granted = (sec_acl_permset_t)0;
    int                 i;              /* For traversing entry list. */
    unsigned int        j;              /* For traversing entry list. */
    sec_id_t            *group_id, *realm_id;
    boolean32           chk_loc_groups = false, one_group_found = false;
    boolean32           got_access = false;

    /* USER_OBJ check */
    if (entry_pos[USER_OBJ_ENTRY] != ENTRY_NOT_FOUND) {

        /* If e_type_user_obj entry exists, then user_obj can't be NULL */
        if (user_obj == NULL) {
            SET_STATUS(st_p, sec_acl_expected_user_obj);
            return false;
        }

        /* If the id assoc w/ user_obj matches the principal id */
        if (uuid_equal(&paP->principal.uuid,
                        &user_obj->uuid,
                        &st)) {
            /* then check the permsets to see if access is granted */
            if (get_access(
                    sec_acl_p->sec_acl_entries[entry_pos[USER_OBJ_ENTRY]],
                    mask_perms, &granted))
                got_access = true;
		
            /* If no access granted and no user_obj_delegate, bail out now */
            else if (entry_pos[USER_OBJ_DELEG] == ENTRY_NOT_FOUND)
                return false;       /* implied denial rights */
        }
    }

    /* USER_OBJ_DELEGATE check */
    if (entry_pos[USER_OBJ_DELEG] != ENTRY_NOT_FOUND) {

        /* If e_type_user_obj_deleg entry exists then user_obj can't be NULL */
        if (user_obj == NULL) {
            SET_STATUS(st_p, sec_acl_expected_user_obj);
            return false;
        }

        /* If the id assoc w/ user_obj matches the principal id */
        if (uuid_equal(&paP->principal.uuid, &user_obj->uuid, &st)) {
            /* then check the permsets to see if access is granted */
            if (get_access(
                    sec_acl_p->sec_acl_entries[entry_pos[USER_OBJ_DELEG]],
                    mask_perms, &granted))
                got_access = true;
            else
                return false;       /* implied denial rights */
        }
    }
    /* Not pretty, but best performance without rewriting entire algorithm */
    if (got_access) goto DlgGotAccess;

    /* USER check */
    if (entry_pos[USER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(sec_acl_p->sec_acl_entries[entry_pos[USER_ENTRY]],
                            mask_perms, &granted))
            got_access = true;
        else if (entry_pos[USER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }

    /* USER_DELEG check */
    if (entry_pos[USER_DELEG] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(sec_acl_p->sec_acl_entries[entry_pos[USER_DELEG]],
                            mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }
    if (got_access) goto DlgGotAccess;

    /* FOREIGN_USER check */
    if (entry_pos[FOREIGN_USER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_USER_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else if (entry_pos[F_USER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }

    /* FOREIGN_USER_DELEG check */
    if (entry_pos[F_USER_DELEG] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[F_USER_DELEG]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }
    if (got_access) goto DlgGotAccess;

    /* GROUP checks */
    group_access = (sec_acl_permset_t) 0;
    for (i = 0; ((!got_access) && (i < sec_acl_p->num_entries)); i++) {
        switch(sec_acl_p->sec_acl_entries[i].entry_info.entry_type) {
            case sec_acl_e_type_group_obj:
            case sec_acl_e_type_group:
            case sec_acl_e_type_foreign_group:
            case sec_acl_e_type_group_obj_deleg:
            case sec_acl_e_type_group_deleg:
            case sec_acl_e_type_for_group_deleg:
                if ((sec_acl_p->sec_acl_entries[i].entry_info.entry_type
                    == sec_acl_e_type_group_obj) ||
                    (sec_acl_p->sec_acl_entries[i].entry_info.entry_type
                    == sec_acl_e_type_group_obj_deleg)) {

                    /* If group_obj entry exists then group_obj cant be NULL */
                    if (group_obj == NULL) {
                        SET_STATUS(st_p, sec_acl_expected_group_obj);
                        return false;
                    }
                    group_id = group_obj;   /* check against group_obj param */
                    realm_id = &sec_acl_p->default_realm;
                    chk_loc_groups = default_realm_eq(i);
                } else if ((sec_acl_p->sec_acl_entries[i].entry_info.entry_type
                            == sec_acl_e_type_group) ||
                           (sec_acl_p->sec_acl_entries[i].entry_info.entry_type
                            == sec_acl_e_type_group_deleg)) {
                    chk_loc_groups = default_realm_eq(i);
                    group_id = 
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.id;
                    realm_id = &sec_acl_p->default_realm;
                } else {
                    chk_loc_groups = foreign_realm_eq(i);
                    group_id = 
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id;
                    realm_id =
                        &sec_acl_p->sec_acl_entries[i].entry_info.tagged_union.foreign_id.realm;
                }
                /* Check either the local groups or the foreign groups */
                if (chk_loc_groups) {
                    /* CHECK PAC GROUP */
                    if (uuid_equal(&paP->group.uuid,
                                    &group_id->uuid, &st) ) {

                        one_group_found = true;
                        /* then check the perms to see if access is granted */
                        if (get_access(sec_acl_p->sec_acl_entries[i],
                                            mask_perms, &granted))
                            group_access = (group_access | granted);
                    }
                    /* CHECK LOCAL GROUPS */
                    for (j = 0; j < paP->num_groups; j++) {

                        if (uuid_equal(&paP->groups[j].uuid,
                                        &group_id->uuid, &st) ) {

                            one_group_found = true;
                            /* check the perms to see if access granted */
                            if (get_access(sec_acl_p->sec_acl_entries[i],
                                                mask_perms, &granted))
                                group_access = (group_access | granted);
                        }
                    }
                } else {
                    /* CHECK FOREIGN GROUPS */
		    int k;
		    /* For each foreign groupset that the caller belongs to */
                    for (j = 0; j < paP->num_foreign_groupsets; j++) {

                      /* If the entry's realm matches this groupset's realm */
                      if (uuid_equal(&paP->foreign_groupsets[j].realm.uuid,
                                      &group_id->uuid, &st))

                        /* Then check each group within that foreign realm */
                        for (k = 0; k < paP->foreign_groupsets[j].num_groups;
                             k++) {
                            if (uuid_equal(
                        	&paP->foreign_groupsets[j].realm.uuid,
                                &realm_id->uuid, &st)) {

                                one_group_found = true;
                                /* check the perms to see if access granted */
                                if (get_access(sec_acl_p->sec_acl_entries[i],
                                        mask_perms, &granted))
                                    group_access = (group_access | granted);
                            }

                        }
		    }
		}
                break;
            default:
                break;
        } /* switch (entry type) */

        /* See if the union of multiple group entries granted access */
        if ((group_access & desired_access) == desired_access) {
            got_access = true;
            granted = group_access;
        }
    } /* GROUP check */

    /* If at least 1 group found, and no group permissions
     * were granted, then deny any access
     */
    if (!got_access && one_group_found) {
        if (group_access == (sec_acl_permset_t)0) {
            return false;
        } else {
            got_access = true;
            granted = group_access;
        }
    }
    if (got_access) goto DlgGotAccess;

    /* OTHER_OBJ check */
    if (entry_pos[OTHER_OBJ_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[OTHER_OBJ_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else if (entry_pos[OTHER_OBJ_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }
    
    /* OTHER_OBJ_DELEG check */
    if (entry_pos[OTHER_OBJ_DELEG] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[OTHER_OBJ_DELEG]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }
    if (got_access) goto DlgGotAccess;

    /* FOREIGN_OTHER check */
    if (entry_pos[FOREIGN_OTHER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_OTHER_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else if (entry_pos[F_OTHER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }

    /* FOREIGN_OTHER_DELEG check */
    if (entry_pos[F_OTHER_DELEG] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[F_OTHER_DELEG]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }
    if (got_access) goto DlgGotAccess;

    /* ANY_OTHER check */
    if (entry_pos[ANY_OTHER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[ANY_OTHER_ENTRY]],
                mask_perms, &granted))
            got_access = true;
        else if (entry_pos[ANY_OTHER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }

    /* ANY_OTHER_DELEG check */
    if (entry_pos[ANY_OTHER_DELEG] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (get_access(
                sec_acl_p->sec_acl_entries[entry_pos[ANY_OTHER_DELEG]],
                mask_perms, &granted))
            got_access = true;
        else
            return false;           /* implied denial rights */
    }

DlgGotAccess:
	
    /* UNAUTHENTICATED check
     * If pa_data isn't authenticated and access was granted by one of the
     * above checks, then desired_access must be masked by the unauthenticated
     * entry.
     */
    if ((got_access) && (! auth)) {
        if (entry_pos[UNAUTH_ENTRY] == ENTRY_NOT_FOUND) {
            got_access = false;
        } else {
            granted &=
                sec_acl_p->sec_acl_entries[entry_pos[UNAUTH_ENTRY]].perms;
        }
    }
    return got_access;
}


/* s e c _ a c l _ m g r _ g e t _ a c c e s s
 *
 * Determine the caller's access to the specified object.
 */
PUBLIC void sec_acl_mgr_get_access (
    sec_acl_mgr_handle_t    sec_acl_mgr,
    rpc_authz_cred_handle_t accessor_handle,
    sec_acl_key_t           sec_acl_key,
    uuid_t                  *manager_type,
    sec_id_t                *user_obj,
    sec_id_t                *group_obj,
    sec_acl_permset_t       *net_rights,
    error_status_t          *stp
)
{
    sec_acl_list_t          *sec_acl_list_p;
    sec_acl_t               *sec_acl_p;
    unsigned int            i,j;            /* For traversing entry list. */
    int                     mask,           /* For keeping track of entries. */
                            user_obj_entry,
                            user_entry,
                            foreign_user_entry,
                            other_obj_entry,
                            foreign_other_entry,
                            any_other_entry,
                            unauth_entry;
    sec_id_t                *group_id, *realm_id;
    sec_acl_permset_t       mask_perms, granted, group_access;
    boolean32               chk_loc_groups = false, one_group_found = false;
    boolean32               auth, got_access = false;
    rsdb_acl_mgr_type_t     rsdb_mgr_type;
    sec_acl_permset_t       max_perms;
    error_status_t          tmp_status;
    sec_id_pa_t             *paP;
    sec_cred_pa_handle_t    pa_h;
    sec_cred_cursor_t       cursor;

    CLEAR_STATUS(stp);

    /* Find out if the credentials are authenticated.
     * Then get the initiator's credentials from the cred handle.
     * If we can't even do that, then don't bother continuing on.
     */
    auth = sec_cred_is_authenticated(accessor_handle, stp);
    if (BAD_STATUS(stp))
        return;
    pa_h = sec_cred_get_initiator(accessor_handle, stp);
    if (BAD_STATUS(stp))
        return;
    paP = sec_cred_get_pa_data(pa_h, stp);
    if (BAD_STATUS(stp))
        return;

    *net_rights = (sec_acl_permset_t) 0;
    /* get the max_perms for this manager type */
    if(!(rsdb_acl_get_mgr_type_from_uuid(manager_type, &rsdb_mgr_type, stp))) {
        return;
    }
    CHECK_ACL_MGR_VERSION(rsdb_mgr_type, stp);
    if (BAD_STATUS(stp)) return;

    max_perms = rsdb_acl_mgr_info[rsdb_mgr_type].info_string.permissions;

    /* Retrieve the sec_acl for this key.  If this call has good status,
     * the sec_acl_list must be freed.
     */
    sec_acl_mgr_lookup(sec_acl_mgr, sec_acl_key, manager_type,
                        sec_acl_type_object, &sec_acl_list_p, stp);
    if (BAD_STATUS(stp))
        return;
    else {
        sec_acl_p = sec_acl_list_p->sec_acls[0];
    }

    /* Find entries matching the initiator's privilege attributes */
    sec__acl_mgr_match_entries(false, &mask_perms, sec_acl_p, paP);

    /* Check Initiator First
     * Now that we know which entries match the user described in the PAs,
     * check the permissions corresponding to each entry until access is
     * granted by one of them.
     */
    got_access = sec__acl_mgr_access_check(sec_acl_p, mask_perms, paP,
        user_obj, group_obj, max_perms, &granted, auth, stp);
    if ((! got_access) || BAD_STATUS(stp)) {
        rsdb_acl_free_list(&sec_acl_list_p);
        return;
    }
    if (got_access) {
        *net_rights = granted;
    }

    /* Check Delegates */
    sec_cred_initialize_cursor(&cursor, stp);
    if (BAD_STATUS(stp))
	return;
    do {
        /* Access must be granted to each delegate, so assume no
         * access yet each time through the loop
         */
        pa_h = sec_cred_get_delegate(accessor_handle, &cursor, stp);
        if (GOOD_STATUS(stp)) {
            got_access = false;
            paP = sec_cred_get_pa_data(pa_h, stp);
            if (GOOD_STATUS(stp)) {
                /* Find the ACL entries that match this delegate */
                sec__acl_mgr_match_entries(true, &mask_perms, sec_acl_p, paP);

                /* Check the delegate's access */
                got_access = sec__acl_mgr_deleg_access_check(sec_acl_p,
                    mask_perms, paP, user_obj, group_obj, max_perms,
		    &granted, auth, stp);
                if (GOOD_STATUS(stp) && got_access) {
                    *net_rights = (*net_rights & granted);
                }
            }
        }
    } while (GOOD_STATUS(stp) && got_access);

    sec_cred_free_cursor(&cursor, &tmp_status);
    rsdb_acl_free_list(&sec_acl_list_p);
    /* Any error code other than end of delegate list is fatal */
    if (STATUS_EQUAL(stp, sec_cred_s_no_more_entries)) {
        CLEAR_STATUS(stp);
    }
}


/* s e c _ a c l _ m g r _ r e p l a c e
 *
 *  Replace the acl associated with the key.  This package treats acls as
 *  immutable objects - the old acl is thrown away and the new one created
 *  Some implementations of this interface may choose to optimize storage
 *  and share acl representation for many objects - in which case the real
 *  acl storage must be reference counted, but that is of no concern to
 *  the consumer of the interface.
 *
 *  The sec_acl_key points to an rsdb_acl_ids_t whose domain and
 *  obj_id are used to identify the node whose acl is being replaced.
 *  If the obj_id field is set to RSDB_POLICY_OBJ_ID, the POLICY node is
 *  indicated.
 *  If the obj_id field is set to RSDB_REPLIST_OBJ_ID, the REPLIST node is
 *  indicated.
 *  If the obj_id field is set to RSDB_ATTR_SCHEMA_OBJ_ID, the ATTR_SCHEMA
 *  node is indicated.
 *  The domain field of the rsdb_acl_ids_t is ignored in these two cases.
 *  Otherwise, the domain and obj_id  identify the PGO/DIR object whose ACL
 *  is being replaced.
 *  The input manager_type and acl_type further identify the acl to be replaced
 *  Ignore the acl_id in the rsdb_acl_ids_t.  Let the node, mgr_type
 *  and acl_type identify the acl to replace.
 *  The sec_acl_mgr handle is ignored in the  current implementation but could
 *  be used in the future to help determine the acl to replace.
 *
 *  NOTE: there is no authorization checking performed on this operation -
 *          it is stricly local.  We assume that the application will
 *          have called sec_acl_mgr_is_authorized with desired access set
 *          to "permission setting" or "owner" or some equivalent notion
 *          that implies the ability to set the acl before actually
 *          attempting the replace.
 *
 *  Status Codes:
 *      sec_acl_object_not_found - sec_acl_key didn't identify any object.
 *      sec_acl_unknown_manager_type - sec_acl_key contained unknown mgr_type.
 *      sec_acl_invalid_acl_type - sec_acl_type invalid for object or mgr_type.
 *      sec_acl_cant_allocate_memory - malloc failed in alloc_db_acl.
 *      sec_acl_bad_key - format error in sec_acl_key.
 *      (The following errors are returned by rsdb_acl_semantic_check.)
 *      sec_acl_invalid_manager_type - input acl's manager type didn't
 *                                     match input mgr_type
 *      sec_acl_invalid_entry_type - an acl entry not valid for input
 *                                   mgr_type and acl_type.
 *      sec_acl_invalid_permission - an acl entry contains a permission bit
 *                                   not supported for this mgr and acl type.
 *      sec_acl_no_owner - no acl entry had the owner bit set.
 *      sec_acl_bad_acl_syntax - a syntax error was encountered while parsing
 *                               input acl.
 */
PUBLIC void sec_acl_mgr_replace (
    sec_acl_mgr_handle_t    sec_acl_mgr,
    sec_acl_key_t           sec_acl_key,
    uuid_t                  *manager_type,
    sec_acl_type_t          sec_acl_type,
    sec_acl_list_t          *sec_acl_list,
    error_status_t          *st
)
{
    rsdb_acl_ids_t          *key_ids;
    rsdb_acl_mgr_type_t     mgr_type;
    rsdb_named_item_t       item;
    rsdb_pvt_id_t           acl_id;
    rsdb_acl_t              *db_acl;
    rsdb_acl_t              *old_db_acl;
    sec_acl_t               *sec_acl;

    /* sec_acl_mgr is ignored in this implementation. */
    /* This implementation supports only one acl in the acl_list. */

    CLEAR_STATUS(st);
    key_ids = (rsdb_acl_ids_t *)sec_acl_key;

    if (sec_acl_list->num_acls != 1) {
        SET_STATUS(st, sec_acl_bad_acl_syntax);
        return;
    }
    if (!rsdb_acl_get_mgr_type_from_uuid( manager_type, &mgr_type, st)) {
        return;
    }
    CHECK_ACL_MGR_VERSION(mgr_type, st);
    if (BAD_STATUS(st)) return;

    /* Check that the new acl is internally consistent for this mgr_type. */
    sec_acl = sec_acl_list->sec_acls[0];
    rsdb_acl_semantic_check(sec_acl, sec_acl_type, mgr_type, key_ids, st);
    if (BAD_STATUS(st)) {
        return;
    }
    /* Process used to determine acl_id depends on the mgr_type.
     * For POLICY, REPLIST and ATTR_SCHEMA mgr_type, get the rgy_info record.
     * For PGO or DIR mgr type, look up the object.
     */
    /* RSDB_POLICY_OBJ_ID obj_id indicates the POLICY node. */
    if (key_ids->obj_id == RSDB_POLICY_OBJ_ID) {
        if (mgr_type != rsdb_acl_mgr_type_rgy) {
            SET_STATUS(st, sec_acl_invalid_manager_type);
            return;
        }
        acl_id = rsdb_get_rgy_acl_id();
    } else if (key_ids->obj_id == RSDB_REPLIST_OBJ_ID) {
        if(mgr_type != rsdb_acl_mgr_type_replist) {
            SET_STATUS(st, sec_acl_invalid_manager_type);
            return;
        }
        acl_id = rsdb_get_replist_acl_id();
    } else if (key_ids->obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
        if (mgr_type != rsdb_acl_mgr_type_attr_schema) {
            SET_STATUS(st, sec_acl_invalid_manager_type);
            return;
        }
        acl_id = rsdb_get_attr_schema_acl_id();
    } else {    /* look up PGO/DIR object to get acl_id */
        if (!rsdb_util_get_item_by_id(key_ids->domain,
	    key_ids->obj_id, &item)) {
            SET_STATUS(st, sec_acl_object_not_found);
            return;
        }                                

        /* Get the acl_id from the item record. */ 
        if(!rsdb_acl_get_id_from_item(key_ids->domain, &item, 
			sec_acl_type, mgr_type, &acl_id, st)) {
            SET_STATUS(st, sec_acl_invalid_acl_type);
            return;
        }
    }
    /* Now that we have the acl_id, fetch it from the database */
    if (!rsdb_acl_get_by_id(acl_id, &old_db_acl, st)) {
        SET_STATUS(st, sec_acl_object_not_found);
        return;
    }
    /* Pack the acl into an rsdb_acl, 'db_acl', suitable for storing. */
    rsdb_acl_alloc_db_acl( sec_acl, &db_acl, st);
    if (BAD_STATUS(st)) {
        free((char *)old_db_acl);
        return;
    }
    rsdb_acl_import(db_acl, sec_acl, st);
    if (BAD_STATUS(st)) {
        free((char *)old_db_acl);
        free((char *)db_acl);
        return;
    }
    db_acl->threads.id = acl_id;
    db_acl->threads.next = old_db_acl->threads.next;
    db_acl->threads.prev = old_db_acl->threads.prev;
    rsdb_acl_store_by_id(acl_id, db_acl);
    free((char *)old_db_acl);
    free((char *)db_acl);
}


/* s e c _ a c l _ m g r _ l o o k u p
 *
 *  Extract the acl associated with the acl_id in the key.
 *  Space for the sec_acl_list and each acl on the list must be freed by caller
 *  Current implementation places only one acl on the list.
 *
 *  Status Codes:
 *      sec_acl_no_acl_found - sec_acl_key->acl_id didn't identify any acl.
 *      sec_acl_cant_allocate_memory - malloc failed in alloc_sec_acl.
 *      sec_acl_bad_key - bad acl_id or format error in sec_acl_key.
 *      error_status_ok - otherwise.
 */
PUBLIC void sec_acl_mgr_lookup (
    sec_acl_mgr_handle_t    sec_acl_mgr,
    sec_acl_key_t           sec_acl_key,
    uuid_t                  *manager_type,
    sec_acl_type_t          sec_acl_type,
    sec_acl_list_t          **sec_acl_list,
    error_status_t          *st
)
{
    rsdb_acl_ids_t          *key_ids;
    rsdb_named_item_t       item;
    sec_rgy_name_t          item_name;
    sec_rgy_name_t          resolved;
    rsdb_acl_t              *db_acl;
    sec_acl_t               *sec_acl;
    unsigned32              size, acl_len;
    rsdb_acl_mgr_type_t     mgr_type;

    /* sec_acl_mgr is ignored in this implementation. */

    CLEAR_STATUS(st);
    *sec_acl_list = (sec_acl_list_t *)NULL;
    db_acl = (rsdb_acl_t *)NULL;
    sec_acl = (sec_acl_t *)NULL;
    key_ids = (rsdb_acl_ids_t *)sec_acl_key;

    if (key_ids->acl_id == NULL) {
        SET_STATUS(st, sec_acl_bad_key);
        return;
    }

    if (!rsdb_acl_get_mgr_type_from_uuid( manager_type, &mgr_type, st)) {
        return;
    }
    CHECK_ACL_MGR_VERSION(mgr_type, st);
    if (BAD_STATUS(st)) return;

    /* Lookup acl in database using acl_id. */
    if (!rsdb_acl_get_by_id(key_ids->acl_id, &db_acl, st)) {
        return;
    }
    size = sizeof(sec_acl_list_t);
    /* Allocate the sec_acl_list header. */
    if (!(*sec_acl_list = (sec_acl_list_t *)malloc(size))) {
        SET_STATUS(st, sec_acl_cant_allocate_memory);
        free((char *)db_acl);
        return;
    }
    /* Allocate buffer for output acl and export acl info.
     * sec_acl allocated in rsdb_acl_alloc_sec_acl must be freed by caller.
     */
    rsdb_acl_alloc_sec_acl(db_acl, &sec_acl, &acl_len, st);
    if (BAD_STATUS(st)) {
        free((char *)(*sec_acl_list));
        free((char *)db_acl);
        return;
    }
    /* Store the acl length in key_ids for later use by
     * rsdb_acl_copy_list_to_rpc_buf.
     */
    key_ids->acl_len = acl_len;
    rsdb_acl_export(sec_acl, db_acl, st);
    if (BAD_STATUS(st)) {
        free((char *)(*sec_acl_list));
        free((char *)db_acl);
        free((char *)sec_acl);
        return;
    }
    (*sec_acl_list)->num_acls = 1;
    (*sec_acl_list)->sec_acls[0] = sec_acl;
    free((char *)db_acl);
    return;
}


/* s e c _ a c l _ m g r _ g e t _ m a n a g e r _ t y p e s
 *
 * Determine the types of acls protecting an object. ACL editors/browsers
 * use this operation to determine the acl manager types that a particular
 * reference monitor is using to manage the acls of a particular type
 * on a selected entity.
 *
 * num_types is the total number of acl manager types supported by this
 * entity for the given acl_type - if it is greater than the size avail,
 * this function should be invoked again with a buffer of the appropriate size.
 *
 * If num_types is greater than 1, the object is polymorphic - i.e., the
 * named object supports acl manager types for each form of the selected
 * entity.
 *
 * The sec_acl_key is the name of the object.  This routine will
 * (1) resolve the input name to determine the object it identifies,
 * (2) return a list of the manager types of the object's acl(s)
 * corresponding to the input acl_type.
 *
 *  Status Codes:
 *      sec_acl_object_not_found - name didn't identify any object.
 *      sec_acl_invalid_acl_type - Acl_type not found on named object.
 *      sec_acl_invalid_entry_name - bad domain component in name.
 *      sec_acl_unknown_manager_type - object's manager type not valid.
 */
PUBLIC void sec_acl_mgr_get_manager_types (
    sec_acl_mgr_handle_t        sec_acl_mgr,
    sec_acl_key_t               sec_acl_key,
    sec_acl_type_t              sec_acl_type,
    unsigned32                  size_avail,
    unsigned32                  *size_used,
    unsigned32                  *num_types,
    uuid_t                      manager_types[],
    error_status_t              *st
)
{
    unsigned_char_t     *p;
    rsdb_named_item_t   obj;
    sec_rgy_domain_t    domain;
    rsdb_credentials_t  credentials;
    rsdb_acl_mgr_type_t mtype;

    CLEAR_STATUS(st);
    *size_used = 0;
    *num_types = 0;

    p = (unsigned_char_t *)sec_acl_key;
    /* Initial '/' is illegal */
    if (*p == '/') {
        SET_STATUS(st, sec_acl_object_not_found);
        return;
    }
    /* Handle the special case of SEC_POLICY_OBJ_NAME first. */
    if (u_strcmp(p, SEC_POLICY_OBJ_NAME) == 0) {
	if (!OBJECT_ACL_TYPE(sec_acl_type)) {
	    SET_STATUS(st, sec_acl_invalid_acl_type);
	    return;
	}
	CHECK_ACL_MGR_VERSION(rsdb_acl_mgr_type_rgy, st);
	if (STATUS_OK(st)) {
	    *num_types = 1;
	    mtype = rsdb_acl_mgr_type_rgy;
	    if (size_avail > 0) {
		if (!rsdb_acl_get_mgr_uuid_from_type(mtype, manager_types, st)) {
		    dce_svc_printf(SEC_RSDB_CANT_GET_MGR_TYPUUID_MSG);
		}
		*size_used = 1;
	    }
	}
        return;
    }
    /* Handle the special case of the Replist Object. */
    if (u_strcmp(p, SEC_REPLIST_OBJ_NAME) == 0) {
	if (!OBJECT_ACL_TYPE(sec_acl_type)) {
	    SET_STATUS(st, sec_acl_invalid_acl_type);
	    return;
	}
	CHECK_ACL_MGR_VERSION(rsdb_acl_mgr_type_replist, st);
	if (STATUS_OK(st)) {
	    *num_types = 1;
	    mtype = rsdb_acl_mgr_type_replist;
	    if (size_avail > 0) {
		if (!rsdb_acl_get_mgr_uuid_from_type(mtype, manager_types, st)) {
		    dce_svc_printf(SEC_RSDB_CANT_GET_MGR_TYPUUID_MSG);
		}
		*size_used = 1;
	    }
	}
	return;
    }
    /* Handle the special case of the Attr Schema Object. */
    if (u_strcmp(p, SEC_ATTR_SCHEMA_OBJ_NAME) == 0) {
	if (!OBJECT_ACL_TYPE(sec_acl_type)) {
	    SET_STATUS(st, sec_acl_invalid_acl_type);
	    return;
	}
	CHECK_ACL_MGR_VERSION(rsdb_acl_mgr_type_attr_schema, st);
	if (STATUS_OK(st)) {
	    *num_types = 1;
	    mtype = rsdb_acl_mgr_type_attr_schema;
	    if (size_avail > 0) {
		if (!rsdb_acl_get_mgr_uuid_from_type(mtype, manager_types, st)) {
		    dce_svc_printf(SEC_RSDB_CANT_GET_MGR_TYPUUID_MSG);
		}
		*size_used = 1;
	    }
	}
        return;
    }
    /* Handle case where name identifies PERSON, GROUP, ORG or DIR node */
    rsdb_util_get_domain_obj_by_name(p, &domain, &obj, st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_acl_object_not_found);
        return;
    }
    /*
     * The mgr_type can be determined, without retrieving the acl itself,
     * by the object type, domain, and acl_type.
     */
    if (RSDB_IS_PGO(&obj)) {
	if(!OBJECT_ACL_TYPE(sec_acl_type)) {
	    SET_STATUS(st, sec_acl_invalid_acl_type);
	    return;
	}
        mtype = GET_MGR_BY_DOMAIN(domain);
	CHECK_ACL_MGR_VERSION(mtype, st);
	if (STATUS_OK(st)) {
	    *num_types += 1;
	    if (size_avail > *size_used) {
		if (!rsdb_acl_get_mgr_uuid_from_type(mtype,
						     &manager_types[*size_used], st)) {
		    dce_svc_printf(SEC_RSDB_CANT_GET_MGR_TYPUUID_MSG);
		}
		*size_used += 1;
	    }
	}
    }
    if (RSDB_IS_DIR(&obj)) {
        if(!CONTAINER_ACL_TYPE(sec_acl_type)) {
            SET_STATUS(st, sec_acl_invalid_acl_type);
            return;
        }
        if(sec_acl_type == sec_acl_type_default_object) {
            mtype = GET_MGR_BY_DOMAIN(domain);
        } else {
            mtype = rsdb_acl_mgr_type_dir;
        }
	CHECK_ACL_MGR_VERSION(mtype, st);
	if (STATUS_OK(st)) {
	    *num_types += 1;
	    if (size_avail > *size_used) {
		if (!rsdb_acl_get_mgr_uuid_from_type(mtype,
						     &manager_types[*size_used], st)) {
		    dce_svc_printf(SEC_RSDB_CANT_GET_MGR_TYPUUID_MSG);
		}
		*size_used += 1;
	    }
	}
    }
    if (*num_types == 0) {
        SET_STATUS(st, sec_acl_unknown_manager_type);
        return;
    }
}


/* s e c _ a c l _ m g r _ g e t _ p r i n t s t r i n g
 *
 * Retrieve printable representations for each permission bit that the
 * sec_acl manager will support.  There may be aliases for common
 * permission combinations - by convention simple entries should
 * appear at the beginning of the array, and combinations should
 * appear at the end.  When false the tokenize flag indicates that
 * permission printstrings are unambiguous and therefore printstrings
 * for various permissions can be concatenated.  When true, however,
 * this property does not hold and the strings should be tokenized
 * before input or output. The manager_info string provides a name and
 * help info for the manager type as well as the complete set of
 * supported permission bits. total_num_printstrings is the total
 * number of acl printstrings supported by this acl manager type - if
 * it is greater than the size avail, this function should be invoked
 * again with a buffer of the appropriate size.
 *
 * If the acl for the target object contains more than 32 permission
 * bits, multiple manager types can be used - one for each 32 bit wide
 * slice of permissions.  When this is the case the manager_type_chain
 * parameter is set to the uuid of the next manager type in the set.  The
 * final result for the chain returns uuid_nil in the manager_type_chain
 * parameter.
 *
 * Status Codes:
 *  sec_acl_unknown_manager_type - input manager type uuid not legal.
 */
PUBLIC void sec_acl_mgr_get_printstring (
    sec_acl_mgr_handle_t    sec_acl_mgr,
    uuid_t                  *manager_type,
    unsigned32              size_avail,
    uuid_t                  *manager_type_chain,
    sec_acl_printstring_t   *manager_info,
    boolean32               *tokenize,
    unsigned32              *total_num_printstrings,
    unsigned32              *size_used,
    sec_acl_printstring_t   printstrings[],
    error_status_t          *st
)
{
    int                         i;
    sec_acl_permset_t           mgr_permset;
    rsdb_acl_mgr_type_t         mgr_type;
    register rsdb_acl_mgr_perm_type_t    perm_index;

    CLEAR_STATUS(st);
    *tokenize = false;
    *size_used = 0;
    *total_num_printstrings = 0;
    manager_info->printstring[0] = '\0';
    manager_info->helpstring[0]  = '\0';
    uuid_create_nil(manager_type_chain, st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_acl_rpc_error);
        return;
    }

    /* Check that manager_type is within legal range. */
    if (!rsdb_acl_get_mgr_type_from_uuid(manager_type, &mgr_type, st)) {
        return;
    }
    CHECK_ACL_MGR_VERSION(mgr_type, st);
    if (!STATUS_OK(st)) return;

    *total_num_printstrings = rsdb_acl_mgr_info[mgr_type].num_perms;
    *size_used = (size_avail < *total_num_printstrings) ?
                 size_avail : *total_num_printstrings;

    COPY_PRINT_STRING( manager_info,
                       &(rsdb_acl_mgr_info[mgr_type].info_string) );

    mgr_permset = rsdb_acl_mgr_info[mgr_type].info_string.permissions;
    for (i = 0, perm_index = rsdb_acl_mgr_perm_read;
         (i < *size_used) && (perm_index < rsdb_acl_mgr_perm_last);
         i++, perm_index++) {
        while ( !(IS_SET(mgr_permset,
                       rsdb_acl_perm_strings[perm_index].permissions)) &&
               perm_index < rsdb_acl_mgr_perm_last ) {
            perm_index++;
        }
        if (perm_index <= rsdb_acl_mgr_perm_last) {
            COPY_PRINT_STRING(&printstrings[i],
                              &rsdb_acl_perm_strings[perm_index]);
        }
    }
}

static  void  config_mgr_info (
    rsdb_acl_mgr_type_t   mgr_type,     /* [in] */
    unsigned32            version,      /* [in] */
    char                  *printstring,  /* [in] */
    char                  *helpstring   /* [in] */
)
{
    rsdb_acl_mgr_info[mgr_type].num_perms = 
	 sec_acl_mgr_type_info[mgr_type].permset_info[version].num_perms;

    rsdb_acl_mgr_info[mgr_type].info_string.permissions = 
	sec_acl_mgr_type_info[mgr_type].permset_info[version].permset;

    u_strcpy(rsdb_acl_mgr_info[mgr_type].info_string.printstring, printstring);
    u_strcpy(rsdb_acl_mgr_info[mgr_type].info_string.helpstring, helpstring);
}

sec_acl_permset_t rsdb_acl_permset_rgy ( void )
{
    /* can't call this unless the acl manager has been configured */
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown);
    return rsdb_acl_mgr_info[rsdb_acl_mgr_type_rgy].info_string.permissions;
}

sec_acl_permset_t rsdb_acl_permset_replist ( void )
{
    /* can't call this unless the acl manager has been configured */
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown);
    return rsdb_acl_mgr_info[rsdb_acl_mgr_type_replist].info_string.permissions;
}

sec_acl_permset_t rsdb_acl_permset_attr_schema ( void )
{
    /* can't call this unless the acl manager has been configured */
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown);
    return rsdb_acl_mgr_info[rsdb_acl_mgr_type_attr_schema].info_string.permissions;
}
   
sec_acl_permset_t rsdb_acl_permset_person ( void )
{
    /* can't call this unless the acl manager has been configured */
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown);
    return rsdb_acl_mgr_info[rsdb_acl_mgr_type_person].info_string.permissions;
}

sec_acl_permset_t rsdb_acl_permset_group ( void )
{
    /* can't call this unless the acl manager has been configured */
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown);
    return rsdb_acl_mgr_info[rsdb_acl_mgr_type_group].info_string.permissions;
}

sec_acl_permset_t rsdb_acl_permset_org ( void )
{
    /* can't call this unless the acl manager has been configured */
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown);
    return rsdb_acl_mgr_info[rsdb_acl_mgr_type_org].info_string.permissions;
}

sec_acl_permset_t rsdb_acl_permset_dir ( void )
{
    /* can't call this unless the acl manager has been configured */
    dce_assert(sec_svc_handle, sec_acl_mgr_software_version != rs_c_software_vers_unknown);
    return rsdb_acl_mgr_info[rsdb_acl_mgr_type_dir].info_string.permissions;
}
