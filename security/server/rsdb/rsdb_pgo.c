/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_pgo.c,v $
 * Revision 1.1.92.1  1996/05/10  13:25:15  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/5  1996/03/15  14:49 UTC  hanfei
 * 	when creating foreign_person database, set unix_id of
 * 	its root directory to be -1
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/4  1996/02/29  16:46 UTC  hanfei
 * 	work to make /.:/sec/foreign_principal and its subdirectory not
 * 	accessible.
 * 	treaks from code review.
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/02/20  15:10 UTC  hanfei
 * 	rsdb_pgo_get_members_all() : add a leading '/' for all foreign
 * 	principal names returned.
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/08  19:25 UTC  hanfei
 * 	take out domain from parameter list for rsdb_pgo_get_members_foreign_groupsets().
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  19:00 UTC  hanfei
 * 	multi-cell group work: codes to extend member operations on foreign
 * 	principal and modify codes in rsdb_pgo_add().
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:19 UTC  psn
 * 	[CHFts15883] prevent possible secd database corruption
 * 	[1995/07/28  19:51 UTC  rps  /main/HPDCE02/4]
 *
 * 	rsdb_pgo_move():  fix parental-pgo check
 * 	This disallows all moves into polymorphs; should be enhanced to be less restrictive.
 * 	[1995/07/28  19:38 UTC  rps  /main/HPDCE02/rps_moth3/1]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  20:41 UTC  burati  /main/HPDCE02/3]
 *
 * 	More alias work
 * 	[1995/07/13  20:22 UTC  burati  /main/HPDCE02/mb_mothra5/2]
 *
 * 	CHFts15622 (OT12915) Alias ACL bug
 * 	[1995/06/29  22:01 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	migration: conditionally repress pgodir creation
 * 	[1995/05/08  16:46 UTC  rps  /main/DCE1_1_WP/rps_wp/1]
 *
 * 	SEC migration Warranty Patch merge.
 * 	[1995/04/26  19:49 UTC  jrr  /main/DCE1_1_WP/1]
 *
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
 *
 * 	Merge greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:20 UTC  greg  /main/SEC_migrate/1]
 *
 * 	Convert references tio the former #define rsdb_acl_permset_*
 * 	to calls to the new rsdb_acl_permset_*() function.
 *
 * 	[1995/04/13  20:05 UTC  greg  /main/greg_sec_migrate_acls/1]
 *
 * Revision 1.1.83.6  1994/08/08  14:23:47  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:24:14  mdf]
 * 
 * Revision 1.1.85.7  94/05/26  15:43:08  mdf
 * 	hp_sec_to_osf_3 drop, fixed botched merge (again).
 * 
 * Revision 1.1.83.2  1994/04/22  19:14:19  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:26:15  rps]
 * 
 * 	Moved location of num_attrs and attr_list_id
 * 	[1994/03/30  16:45:40  rps]
 * 
 * Revision 1.1.81.2  1994/03/29  23:47:43  rps
 * 	Support rgy polymorphism
 * 
 * Revision 1.1.83.1  1994/03/02  17:56:08  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:52:43  hanfei]
 * 
 * 	HP revision /main/ODESSA_2/1  1994/02/10  04:51  ahop
 * 	 merge ODESSA_ERA4 up to ODESSA_2
 * 
 * 	HP revision /main/ODESSA_ERA4/1  1994/02/07  19:44  mdf
 * 	merge from ODESSA_ERA3 to ODESSA_ERA4.
 * 
 * 	HP revision /main/ODESSA_ERA3/1  1994/02/07  01:48  ahop
 * 	 merge it up
 * 
 * 	HP revision /main/ahop_odss_era_bl8/1  1994/02/05  22:16  ahop
 * 	initialize num_attrs and attr_list_id before storing new item
 * 
 * Revision 1.1.80.1  1993/10/07  13:12:32  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/07  13:09:47  mccann]
 * 
 * Revision 1.1.5.6  1993/02/26  23:56:13  emartin
 * 	em_bl6f: do rsdb_pgo_clear
 * 	[1993/02/22  22:55:36  emartin]
 * 
 * Revision 1.1.5.5  1992/12/29  16:39:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:49  zeliff]
 * 
 * Revision 1.1.5.4  1992/12/18  22:13:18  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:55:35  sekhar]
 * 
 * Revision 1.1.6.3  1992/12/16  19:00:11  emartin
 * 	bl6c: if num_members == 0, return good status
 * 
 * Revision 1.1.6.2  1992/12/11  21:55:12  emartin
 * 	bl6c: fix rsdb_pgo_get_members for num_members = 0 case.
 * 
 * Revision 1.1.5.3  1992/09/29  20:14:15  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:40:48  sekhar]
 * 
 * Revision 1.1.5.2  1992/08/31  18:28:34  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:13:03  sekhar]
 * 
 * Revision 1.1.3.2  1992/06/10  19:07:04  burati
 * 	CR3970 Fixed code that changes an alias to a primary in rsdb_pgo_replace
 * 	[1992/06/09  22:32:44  burati]
 * 
 * Revision 1.1  1992/01/19  14:44:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  rsdb_pgo.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Registry Server - Person, Group, Org Naming management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_pgo.c	31 - 10/23/91";
#endif

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <string.h>

#include <dce/dce.h>
#include <dce/uuid.h>

#include <rsdb_pvt.h>
#include <rsdb_util.h>
#include <rsdb_create.h>
#include <rsdb_pgo_util.h>
#include <rsdb_dir_util.h>
#include <rsdb_name_util.h>

#include <rsdb_pgo.h>
#include <rsdb_acl.h>
#include <rsdb_policy.h>
#include <rsdb_misc.h>
#include <rsdb_attr.h>
#include <bstring.h>
#include <rs_util.h>
#include <rs_base.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <sec_svc.h>


/*
 * internal functions
 */

/*
 *  i m p o r t _ p g o
 */
PRIVATE void import_pgo
  (
    rsdb_pgo_item_t     *pgo,
    sec_rgy_pgo_item_t  *item,
    boolean32           new
  )
{
    pgo->unix_id        = item->unix_num;
    pgo->fullname_len   = strlen( (char *) item->fullname);
    pgo->flags          = item->flags;
    strncpy((char *) pgo->fullname, (char *) item->fullname,
                pgo->fullname_len);
    pgo->quota          = item->quota;

    if (new) {
        /*
         * Make sure internal info is well defined.  If object isn't new,
         * this routine should have been called with new=false and any old
         * internal data will be preserved.
         */
        pgo->num_foreign_members      = 0;
        pgo->foreign_membership       = END_OF_LIST;
    }
}


/*
 *  d e l e t e _ a l i a s
 */
PRIVATE void delete_alias
  (
    sec_rgy_domain_t    domain,
    rsdb_named_item_t   *pgo
  )
{
    rsdb_named_item_t   pgo_alias;
    rsdb_credentials_t  credentials;
    error_status_t      st;

    if ( domain == sec_rgy_domain_foreign_person ) {
	/* delete this pgo's credential */
	rsdb_purge_credentials_by_pvt_id(domain_info[domain].db,
		RSDB_PGO(pgo).threads.id);
	return;
    }

    if (!rsdb_get_credentials_by_unix_id(domain_info[domain].db,
                                     RSDB_PGO(pgo).unix_id, &credentials)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_PGO_CREDS_MSG);
    }

    if (pgo->body.threads.id == credentials.id) {
        if (pgo->body.threads.alias == END_OF_LIST) {
            rsdb_purge_credentials(domain_info[domain].db, &credentials);
        } else {
            rsdb_change_credentials(domain_info[domain].db, &credentials,
                            &credentials.uid, credentials.unix_id,
                            pgo->body.threads.alias, credentials.acl_id);
        }
    } else {
        if (!rsdb_pgo_util_get_by_id(domain, credentials.id, &pgo_alias)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
        }
        while (true) {
            if (pgo_alias.body.threads.alias == pgo->body.threads.id) {
                pgo_alias.body.threads.alias = pgo->body.threads.alias;
                rsdb_util_store_item_by_id(domain, pgo_alias.body.threads.id,
                                           &pgo_alias);
                break;
            }
            if (!rsdb_pgo_util_get_next_alias(domain, &pgo_alias,
                                              &pgo_alias)) {
                dce_svc_printf(SEC_RSDB_CANT_WALK_ALIAS_CHAIN_MSG);
            }
        }
    }
}


/*
 *  g e t _ p o l i c y _ d a t a
 */
PRIVATE boolean32 get_policy_data
  (
    rsdb_pvt_id_t   id,
    sec_rgy_plcy_t  *policy_data
  )
{
    rsdb_policy_key_t  policy_key;
    char               *data;
    int                data_len;
    error_status_t     st;

    bzero((Pointer) &policy_key, sizeof(policy_key));

    policy_key.record_type = POLICY_TYPE;
    policy_key.id = id;

    rsdb_fetch(domain_info[sec_rgy_domain_org].db, (Pointer) &policy_key,
                    sizeof(policy_key), &data, &data_len, &st);
    if (STATUS_EQUAL(&st, sec_rgy_object_not_found)) {
        return false;
    }

    if (data_len != sizeof(sec_rgy_plcy_t)) {
        dce_svc_printf(SEC_RSDB_BAD_POLICY_DATA_MSG);
    }

    bcopy(data, (Pointer) policy_data, sizeof(sec_rgy_plcy_t));

    return true;
}


/*
 *  s e t _ p o l i c y _ d a t a
 */
PRIVATE void set_policy_data
  (
    rsdb_pvt_id_t   id,
    sec_rgy_plcy_t  *policy_data
  )
{
    rsdb_policy_key_t  policy_key;
    error_status_t     st;

    if (id == END_OF_LIST) {
        dce_svc_printf(SEC_RSDB_BAD_POLICY_KEY_MSG);
    }

    bzero((Pointer) &policy_key, sizeof(policy_key));

    policy_key.record_type = POLICY_TYPE;
    policy_key.id = id;

    rsdb_replace(domain_info[sec_rgy_domain_org].db, (Pointer) &policy_key,
                    sizeof(policy_key), (Pointer) policy_data,
                    sizeof(sec_rgy_plcy_t), &st);
}


/*
 *  p u r g e _ p o l i c y _ d a t a
 */
PRIVATE void purge_policy_data
  (
    rsdb_pvt_id_t  id
  )
{
    rsdb_policy_key_t  policy_key;
    error_status_t     st;

    if (id == END_OF_LIST)
        return;

    bzero((Pointer) &policy_key, sizeof(policy_key));

    policy_key.record_type = POLICY_TYPE;
    policy_key.id = id;

    rsdb_delete(domain_info[sec_rgy_domain_org].db, (Pointer) &policy_key,
                    sizeof(policy_key), &st);
}


PRIVATE void add_creator_acl_entry
  (
    sec_rgy_domain_t        domain,     /* [in] */
    sec_rgy_name_t          name,       /* [in] */
    sec_rgy_foreign_id_t    *client_ids,/* [in] */
    error_status_t          *st
  )
{
    sec_rgy_name_t          full_name;
    sec_acl_type_t          acl_type;
    rsdb_acl_entry_t        *entry;
    uuid_t                  *mgr_type;
    sec_id_t                user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t          ids;
    rsdb_acl_t              *db_acl;

    CLEAR_STATUS(st);

    user_p = &user_obj; group_p = &group_obj;

    /* If client_ids->principal is not a nil uuid,
     * add an entry to the new pgo's acl giving the creator
     * (the client who made this call) maximum permissions.
     * If the creator's cell uuid matches acl default cell uuid,
     * create a user entry.  Otherwise, create a foreign_user entry.
     */
    if(!(uuid_is_nil(&(client_ids->principal), st))) {
        /* Lookup the acl for named pgo to get default realm
         * Set up some domain specific parameters
         */
        if(!rsdb_name_util_complete_name(domain, name, full_name, st))
            return;
        acl_type = sec_acl_type_object;
        switch(domain) {
        case sec_rgy_domain_person:
            mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);
            break;
        case sec_rgy_domain_group:
            mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_group]);
            break;
        case sec_rgy_domain_org:
            mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_org]);
            break;
        default:
            break;
        }
        rsdb_acl_get_ids_from_name(full_name, acl_type, mgr_type, &user_p,
                                   &group_p, &ids, st);
        if(BAD_STATUS(st)) {
            return;
        }
        /* Lookup acl in database using acl_id. */
        if(!rsdb_acl_get_by_id(ids.acl_id, &db_acl, st)) {
            return;
        }
        if(!(entry = (rsdb_acl_entry_t *)malloc(sizeof(rsdb_acl_entry_t)))) {
            SET_STATUS(st, sec_acl_cant_allocate_memory);
            return;
        }
        entry->name_buf[0] = '\0';
        /* Compare acl default realm with client's cell */
        if(uuid_equal(&(client_ids->cell), &(db_acl->default_realm.uuid),st)) {
            entry->entry_type  = sec_acl_e_type_user;
            entry->entry_info.id.uuid = client_ids->principal;
            entry->entry_info.id.name_len = 0;
        } else {
            /* Client is foreign */
            entry->entry_type = sec_acl_e_type_foreign_user;
            entry->entry_info.foreign_id.id.uuid = client_ids->principal;
            entry->entry_info.foreign_id.id.name_len = 0;
            entry->entry_info.foreign_id.realm.uuid = client_ids->cell;
            entry->entry_info.foreign_id.realm.name_len = 0;
        }
        switch(domain) {
        case sec_rgy_domain_person:
            entry->perms = rsdb_acl_permset_person();
            break;
        case sec_rgy_domain_group:
            entry->perms = rsdb_acl_permset_group();
            break;
        case sec_rgy_domain_org:
            entry->perms = rsdb_acl_permset_org();
            break;
        default:
            break;
        }
        rsdb_acl_replace_entry(domain, name, acl_type, mgr_type,
                               entry, st);
        free(entry);
        free((char *)db_acl);
    }
}



/*
 * End of Private functions
 */


/*
 *  r s d b _ p g o _ c r e a t e
 */
PUBLIC void rsdb_pgo_create
  (
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    sec_rgy_properties_t    *properties
  )
{
    sec_rgy_domain_t       d;
    rsdb_sequential_key_t  sequential_key;
    rsdb_name_key_t        name_key;
    int                    name_key_len;
    int                    item_data_len;
    rsdb_acl_mgr_type_t    mgr_type;
    error_status_t         st;

    domain_info[sec_rgy_domain_person].db  = rsdb_person_db;
    domain_info[sec_rgy_domain_group].db   = rsdb_group_db;
    domain_info[sec_rgy_domain_org].db     = rsdb_org_db;
    domain_info[sec_rgy_domain_foreign_person].db     = rsdb_foreign_person_db;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));
    bzero((Pointer) &name_key, sizeof(name_key));

    name_key.record_type  = DUMMY_TYPE;
    name_key.parent_id    = END_OF_LIST;

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = START_OF_LIST;

    for (d = sec_rgy_domain_person; d < sec_rgy_domain_invalid; d++) {
        bzero((Pointer) &domain_info[d].first, sizeof(domain_info[d].first));
        domain_info[d].first.type = rsdb_dir_type;
        item_data_len = NAMED_ITEM_LEN(&domain_info[d].first);
        domain_info[d].first.body.threads.id = START_OF_LIST;
        domain_info[d].first.body.threads.next = END_OF_LIST;
        domain_info[d].first.body.threads.prev = END_OF_LIST;

	RSDB_DIR(&domain_info[d].first).num_attrs = 0;
	RSDB_DIR(&domain_info[d].first).attr_list_id = END_OF_LIST;

        /* This top-level domain directory contains the default
         * ACLs for this domain.
         */
	if ( d != sec_rgy_domain_foreign_person ) {
            RSDB_DIR(&domain_info[d].first).acl =
                rsdb_acl_create_default_acl( rgy_creator, 
                    rgy_local_cell, rsdb_acl_mgr_type_dir );
    
            /* The def_obj_acl depends on the domain. */
            mgr_type = GET_MGR_BY_DOMAIN( d );
            RSDB_DIR(&domain_info[d].first).def_obj_acl =
                rsdb_acl_create_default_acl( rgy_creator, 
                    rgy_local_cell, mgr_type );
    
            RSDB_DIR(&domain_info[d].first).def_dir_acl =
                rsdb_acl_create_default_acl( rgy_creator, 
                    rgy_local_cell, rsdb_acl_mgr_type_dir );
	} else {
            RSDB_DIR(&domain_info[d].first).acl = 0;
            RSDB_DIR(&domain_info[d].first).def_obj_acl = 0;
            RSDB_DIR(&domain_info[d].first).def_dir_acl = 0;
	}

        /* Copy the top level directory name into name_key. */
        switch (d) {
            case sec_rgy_domain_person:
                strcpy((char *) name_key.name, SEC_PRINCIPAL_OBJ_NAME);
                name_key.name_len = strlen(SEC_PRINCIPAL_OBJ_NAME);
                break;
            case sec_rgy_domain_foreign_person:
                strcpy((char *) name_key.name, SEC_FOREIGN_PRINCIPAL_OBJ_NAME);
                name_key.name_len = strlen(SEC_FOREIGN_PRINCIPAL_OBJ_NAME);
                break;
            case sec_rgy_domain_group:
                strcpy((char *) name_key.name, SEC_GROUP_OBJ_NAME);
                name_key.name_len = strlen(SEC_GROUP_OBJ_NAME);
                break;
            case sec_rgy_domain_org:
            default:
                strcpy((char *) name_key.name, SEC_ORG_OBJ_NAME);
                name_key.name_len = strlen(SEC_ORG_OBJ_NAME);
                break;
        }
        name_key_len = NAME_LEN(&name_key);

        rsdb_store(domain_info[d].db, (Pointer) &name_key, name_key_len,
                        (Pointer) &domain_info[d].first, item_data_len, &st);
        rsdb_store(domain_info[d].db, (Pointer) &sequential_key,
                        sizeof(sequential_key), (Pointer) &name_key,
                        name_key_len, &st);
    }

    name_key.name_len     = 4;
    strcpy((char *) name_key.name, "last");
    name_key_len = NAME_LEN(&name_key);

    sequential_key.id = END_OF_LIST;

    for (d = sec_rgy_domain_person; d < sec_rgy_domain_invalid; d++) {
        bzero((Pointer) &domain_info[d].last, sizeof(domain_info[d].last));
        domain_info[d].last.type = rsdb_pgo_type;
        /* Copy in the low_unix_id into field for next assigned unix id. */
        switch (d) {
            case sec_rgy_domain_person:
                RSDB_PGO(&domain_info[d].last).unix_id =
		    properties->low_unix_id_person;
                break;
            case sec_rgy_domain_foreign_person:
                RSDB_PGO(&domain_info[d].last).unix_id = -1;
                break;
            case sec_rgy_domain_group:
                RSDB_PGO(&domain_info[d].last).unix_id =
		    properties->low_unix_id_group;
                break;
            case sec_rgy_domain_org:
                RSDB_PGO(&domain_info[d].last).unix_id =
		    properties->low_unix_id_org;
                break;
        }
        item_data_len = NAMED_ITEM_LEN(&domain_info[d].last);
        domain_info[d].last.body.threads.id = END_OF_LIST;
        domain_info[d].last.body.threads.next = START_OF_LIST;
        domain_info[d].last.body.threads.prev = START_OF_LIST;

        rsdb_store(domain_info[d].db, (Pointer) &name_key, name_key_len,
                        (Pointer) &domain_info[d].last, item_data_len, &st);
        rsdb_store(domain_info[d].db, (Pointer) &sequential_key,
                        sizeof(sequential_key), (Pointer) &name_key,
                        name_key_len, &st);
    }
}


/*
 *  r s d b _ p g o _ i n i t
 */
PUBLIC void rsdb_pgo_init(
    void
)
{
    int     i;

    domain_info[sec_rgy_domain_person].db  = rsdb_person_db;
    domain_info[sec_rgy_domain_group].db   = rsdb_group_db;
    domain_info[sec_rgy_domain_org].db     = rsdb_org_db;
    domain_info[sec_rgy_domain_foreign_person].db = rsdb_foreign_person_db;

    for (i = 0; i < sec_rgy_domain_invalid; i++) {
        rsdb_util_get_item_by_id(i, START_OF_LIST, &domain_info[i].first);
        rsdb_util_get_item_by_id(i, END_OF_LIST, &domain_info[i].last);
    }
} 


/*
 *  r s d b _ p g o _ c l e a r
 *
 * clear out domain_info
 * database is being re-initialized
 */
PUBLIC void rsdb_pgo_clear(
    void
)
{
    domain_info[sec_rgy_domain_person].db  = NULL;
    domain_info[sec_rgy_domain_group].db   = NULL;
    domain_info[sec_rgy_domain_org].db     = NULL;
    domain_info[sec_rgy_domain_foreign_person].db  = NULL;
}


/*
 *  e x p o r t _ p g o
 */
PUBLIC void export_pgo
  (
    sec_rgy_domain_t   domain,
    sec_rgy_name_t     name,
    sec_rgy_pgo_item_t *item,
    rsdb_pgo_item_t    *pgo
  )
{
    db_handle          db;
    rsdb_credentials_t credentials;
    long               name_len;
    error_status_t     status;

    item->unix_num     = pgo->unix_id;
    item->flags        = pgo->flags;
    strncpy((char *) item->fullname, (char *) pgo->fullname,
                pgo->fullname_len);
    item->fullname[pgo->fullname_len] = NULL;
    item->quota        = pgo->quota;

    db = domain_info[domain].db;

    if (!rsdb_get_credentials_by_unix_id(db, pgo->unix_id, &credentials)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_PGO_CREDS_MSG);
    } else {
        item->id = credentials.uid;
        rsdb_util_path_construct(domain, pgo->threads.id, name,
                                 &name_len, &status);
        if(BAD_STATUS(&status)) {
            dce_svc_printf(SEC_RSDB_CANT_CNTR_ITEM_NAME_MSG);
        }
    }
}


/*
 *  r s d b _ p g o _ a d d
 *  
 *  alias_acl - whether caller wants ALIAS ACL fix behavior
 *  *prop_acl - tell caller whether it should propagate the ACL for this
 *              entry in case a replica doesn't have the ALIAS ACL fix
 *              (this argument only used by rs_pgo_add()).
 */
PUBLIC void rsdb_pgo_add
  (
    sec_rgy_domain_t        domain,     /* [in] */
    sec_rgy_name_t          name,       /* [in] */
    sec_rgy_pgo_item_t      *(item),    /* [in] */
    uuid_t		    *cell_id,   /* [in] */
    sec_rgy_foreign_id_t    *client_ids,/* [in] */
    boolean32               alias_fix,  /* [in] */
    boolean32               *prop_acl,  /* [out] */
    error_status_t          *(status)   /* [out] */
  )
{
    rsdb_credentials_t  credentials;
    rsdb_credentials_foreign_t	fp_credentials;    
    rsdb_named_item_t	pgo, dir;
    rsdb_named_item_t   parent;
    rsdb_pvt_id_t       parent_id;
    boolean32           existing_pgo = false;
    rsdb_pvt_id_t       alias_id = END_OF_LIST;
    sec_rgy_name_t      pgo_name;
    sec_rgy_name_t      parent_name;
    long                name_len;
    long                parent_name_len;
    long                pgo_name_len;
    rsdb_pvt_id_t       acl_id;
    rsdb_name_key_t     name_key;
    long                name_key_len;
    error_status_t      tmp_status;

    CLEAR_STATUS(status);
    name_len = strlen( (char *) name);
    *prop_acl = true;                           /* Avoid old alias ACL bug */

    /*
     *   Return if a PGO item with the same name exists.
     */
    if (rsdb_pgo_util_get_by_name(domain, name, name_len, &pgo)) {
        SET_STATUS(status, sec_rgy_name_exists);
        return;
    }

    /*
     * If it's already a dir node, convert it to aggregate form
     */
    if (rsdb_dir_util_get_by_name(domain, name, name_len, &dir)) {
	if (!rs_sw_feature_test_support(rs_c_sw_feature_polymorph, status))
	    return;

	pgo.type = rsdb_dir_type | rsdb_pgo_type;
	memcpy(&RSDB_PGO(&pgo).threads, &dir.body.threads,
		sizeof(dir.body.threads));
	memcpy(&RSDB_DIR(&pgo), &RSDB_DIR(&dir), sizeof(dir.body.dir));
    }
    else {
	pgo.type = rsdb_pgo_type;
    }

    if ( domain == sec_rgy_domain_foreign_person ) {
	/* 
	 *  Adding a pseudo foreign principal for its membership in    
	 * this cell.  We add its credential indexed by its cell uuid 
	 * and principal uuid if this principal was not created yet.
	 */
	if ( rsdb_get_credentials_by_foreign_uuids(domain_info[domain].db, 
					cell_id, &item->id,
                                        &fp_credentials) ) {
            SET_STATUS(status, sec_rgy_object_exists);
            return;
	}
    } else
    if (rsdb_get_credentials_by_uid(domain_info[domain].db, &item->id,
                                        &credentials)) {
        existing_pgo = true;

        if (item->unix_num != credentials.unix_id) {
            SET_STATUS(status, sec_rgy_unix_id_changed);
            return;
        }
        else if (!RSDB_IS_AN_ALIAS(item)) {
            /*
             * Check to see if primary name is an alias!
             */
            if (!rsdb_pgo_util_get_by_id(domain, credentials.id, &pgo)) {
                dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
            }
            if (!RSDB_IS_AN_ALIAS(&RSDB_PGO(&pgo))) {
               SET_STATUS(status, sec_rgy_object_exists);
                return;
            } else {
                alias_id = pgo.body.threads.id;
            }
        }
    } else if (rsdb_get_credentials_by_unix_id(domain_info[domain].db,
                                            item->unix_num, &credentials)) {
        SET_STATUS(status, sec_rgy_object_exists);
        return;
    }

    /*
     * Strip the leaf component from name and put it in pgo_name. If name
     * contained only one component, it will now be empty indicating the
     * parent directory is the root of this domain. Otherwise, name now
     * represents parent directory; use it to get parent ID. Go ahead and
     * add the rest of the info.
     */

    /*
     * Use a copy of the name for the strip operation, so we don't destroy
     * the caller's version of the name
     */
    strcpy( (char *) parent_name, (char *) name);
    parent_name_len = strlen((char *) name);
    rsdb_name_util_strip_leaf( parent_name, &parent_name_len, pgo_name,
			      &pgo_name_len );
    if(parent_name_len == 0) {
        parent_id = START_OF_LIST;
        if(!rsdb_util_get_item_by_id(domain, parent_id, &parent)) {
            SET_STATUS(status, sec_rgy_dir_not_found);
            return;
        }
    } else {
        if(!rsdb_dir_util_get_by_name(domain, parent_name, parent_name_len,
				      &parent)) {
            SET_STATUS(status, sec_rgy_dir_not_found);
            return;
        }
        parent_id = parent.body.threads.id;
    }
    import_pgo(&RSDB_PGO(&pgo), item, true);
    pgo.body.threads.alias = alias_id;
    RSDB_PGO(&pgo).membership = END_OF_LIST;
    RSDB_PGO(&pgo).num_members = 0;
    RSDB_PGO(&pgo).foreign_membership = END_OF_LIST;
    RSDB_PGO(&pgo).num_foreign_members = 0;
    RSDB_PGO(&pgo).info = END_OF_LIST;
    RSDB_PGO(&pgo).num_attrs = 0;
    RSDB_PGO(&pgo).attr_list_id = END_OF_LIST;

    if (RSDB_IS_DIR(&pgo))
	rsdb_util_store_item_by_id(domain, pgo.body.threads.id, &pgo);
    else
	rsdb_util_store_new_named_item(domain, parent_id,
					pgo_name, pgo_name_len, &pgo);

    if (!existing_pgo) {
    	if ( domain == sec_rgy_domain_foreign_person ) {
	    /* add credential for this new foreign principal 
	     * currently unix_id is not used : set to be -1
	     * and acl_id is set to be 0. 
	     */
	    item->unix_num = -1;
	    acl_id = 0;
            rsdb_change_credentials_by_foreign_uuids(domain_info[domain].db, 
			     NULL_PTR, cell_id, &item->id, 
                             item->unix_num, pgo.body.threads.id, acl_id);
    	} else {
            acl_id = rsdb_acl_copy(RSDB_DIR(&parent).def_obj_acl, status);
            if(BAD_STATUS(status)) {
            	return;
            }
            rsdb_change_credentials(domain_info[domain].db, NULL_PTR, &item->id,
                             item->unix_num, pgo.body.threads.id, acl_id);

            /* Add an acl_entry to this PGO's acl giving the creator extra
    	     * privileges.  Since PGO is already added, don't return bad status
    	     * if this call fails
    	     */
            add_creator_acl_entry(domain, name, client_ids, &tmp_status);
	}
        *prop_acl = false;

    } else {

        /* Only if we're supporting backwards compatibility with the alias
         * ACL bug do we want to do the following:
         * 
         * Add an acl_entry to this PGO's acl giving the creator extra
         * privileges.  Since PGO is already added, don't return bad status
         * if this call fails
         */
        if (! alias_fix) {
            add_creator_acl_entry(domain, name, client_ids, &tmp_status);
        }

        if (alias_id != END_OF_LIST) {
            /* Adding a new primary name (current names are marked as aliases)
             * so change the head of the credentials list.
             */
            rsdb_change_credentials(domain_info[domain].db, &credentials,
                &item->id, item->unix_num, pgo.body.threads.id,
                credentials.acl_id);
        } else {
            /* Thread on end of alias chain */
            rsdb_named_item_t alias_pgo;

            rsdb_pgo_util_get_by_id(domain, credentials.id, &alias_pgo);
            while (rsdb_pgo_util_get_next_alias(domain,&alias_pgo,&alias_pgo));

            alias_pgo.body.threads.alias = pgo.body.threads.id;
            rsdb_util_store_item_by_id(domain, alias_pgo.body.threads.id,
                &alias_pgo);
        }
    }
}


/*
 *  r s d b _ p g o _ d e l e t e
 */
PUBLIC void rsdb_pgo_delete
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    long                name_len;

    CLEAR_STATUS(status);
    name_len = strlen( (char *) name);

    if (!rsdb_pgo_util_get_by_name(domain, name, name_len, &pgo)) {
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }

    /* Delete from Group and Org lists (will delete accounts) */
    if (domain == sec_rgy_domain_person) {
        rsdb_pgo_util_delete_person(&pgo);
    } else {
        rsdb_pgo_util_delete_member_list(domain, &pgo);
        if (domain == sec_rgy_domain_org) {
            purge_policy_data(RSDB_PGO(&pgo).info);
        }
    }

    /* Delete from alias chain and credentials */
    delete_alias(domain, &pgo);

    /* Delete this pgo's attribute list */
    rsdb_attr_list_delete(RSDB_PGO(&pgo).attr_list_id);

    /* Delete from sequential chain and primary key if not leaving dir */
    if (RSDB_IS_DIR(&pgo)) {
	rsdb_named_item_t	dir;

	dir.type = rsdb_dir_type;
	memcpy(&RSDB_DIR(&dir), &RSDB_DIR(&pgo),
		sizeof(pgo.body.dir));
	rsdb_util_store_item_by_id(domain, dir.body.threads.id, &dir);
    }
    else {
	rsdb_util_reset_next_item(domain, pgo.body.threads.prev,
				  pgo.body.threads.next);
	rsdb_util_reset_previous_item(domain, pgo.body.threads.next,
				      pgo.body.threads.prev);

	rsdb_pgo_util_purge_pgo(domain, &pgo);
    }
}


/*
 *  r s d b _ p g o _ r e p l a c e _ i n t e r n a l
 *
 * Replace an backend pgo item - this is an internal function for use by
 * operations that update backend storage (e.g., updating the quota count
 * for a principal during an update operation)
 */
PUBLIC void rsdb_pgo_replace_internal
  (
    sec_rgy_domain_t    domain,     /* [in] */
    rsdb_named_item_t   *pgo
  )
{
    rsdb_util_store_item_by_id(domain, pgo->body.threads.id, pgo);
}


/*
 *  r s d b _ p g o _ r e p l a c e
 */
PUBLIC void rsdb_pgo_replace
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    sec_rgy_pgo_item_t  *(item),    /* [in] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    rsdb_credentials_t  credentials;
    boolean32           change_credentials  = false;
    boolean32           change_fullname     = false;
    boolean32           eq;
    rsdb_pvt_id_t       pgo_id;
    sec_rgy_pgo_item_t  to_change;
    long                name_len;

    CLEAR_STATUS(status);
    name_len    = strlen( (char *) name);

    if (!rsdb_pgo_util_get_by_name(domain, name, name_len, &pgo)) {
       SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }

    /*
     * Don't support uuid changes.
     * Don't support unix number changes unless the uuid doesn't
     * embed the unix id.
     */
    if (!rsdb_get_credentials_by_uid(domain_info[domain].db,
                                     &(item->id), &credentials)) {
        SET_STATUS(status, sec_rgy_not_implemented);
        return;
    }
    if (item->unix_num != credentials.unix_id &&
        rsdb_uuid_embeds_unix_id(&(item->id))) {
	SET_STATUS(status, sec_rgy_not_implemented);
        return;
    }

    if (strlen( (char *) item->fullname) != RSDB_PGO(&pgo).fullname_len ||
        strncmp((char *) item->fullname, (char *) RSDB_PGO(&pgo).fullname,
                                        RSDB_PGO(&pgo).fullname_len) != 0) {
        change_fullname = true;
    }

    /*
     * Make sure that we don't generate multiple entries with the
     * same unix id.
     */
    if (credentials.unix_id != item->unix_num) {
        rsdb_credentials_t existing_credentials;

        if (rsdb_get_credentials_by_unix_id(domain_info[domain].db,
                                                item->unix_num,
                                                &existing_credentials)) {
            SET_STATUS(status, sec_rgy_object_exists);
            return;
        }
    }

    /*
     * Check to see if we are making this entry a primary entry.
     * If so we must be sure that there is no primary entry already 
     * listed.
     *
     * Note that in the cell principal subspace we allow setting a
     * primary when there's already one, by swapping them atomically.
     */
    if ((credentials.id != pgo.body.threads.id) && !RSDB_IS_AN_ALIAS(item)) {
        rsdb_named_item_t primary_pgo, previous_pgo, *previous_pgo_p;

        rsdb_pgo_util_get_by_id(domain, credentials.id, &primary_pgo);
        if (!RSDB_IS_AN_ALIAS(&RSDB_PGO(&primary_pgo))) {

	    if (rsdb_name_util_is_cell(name, status) && GOOD_STATUS(status)) {

		long len;
		sec_rgy_name_t pname;
		sec_rgy_pgo_item_t pri_item;

		rsdb_util_path_construct(sec_rgy_domain_person, credentials.id,
						pname, &len, status);
		if (BAD_STATUS(status))
		    return;

		pri_item = *item;
		pri_item.flags |= sec_rgy_pgo_is_an_alias;

		/* We can recurse here because no mods have yet taken place */
		rsdb_pgo_replace(domain, pname, &pri_item, status);
		if (BAD_STATUS(status))
		    return;
	    }
	    else {
		SET_STATUS(status, sec_rgy_object_exists);
		return;
	    }
        }
        /* The pgo before this one on the list also needs to be updated */
        previous_pgo_p = NULL;
        pgo_id = credentials.id;
        do {
            rsdb_pgo_util_get_by_id(domain, pgo_id, &previous_pgo);
            if (previous_pgo.body.threads.alias == pgo.body.threads.id)
                previous_pgo_p = &previous_pgo;
            else
                pgo_id = previous_pgo.body.threads.alias;
        } while ((previous_pgo_p == NULL) &&
                 (previous_pgo.body.threads.alias != NULL));
        if (previous_pgo_p == NULL) {
            dce_svc_printf(SEC_RSDB_CORRUPT_ALIAS_CHAIN_MSG);
        }

        /*
         * Old entry was an alias, so go ahead and exchange their roles
         */
        previous_pgo.body.threads.alias = pgo.body.threads.alias;
        credentials.id = pgo.body.threads.id;
        pgo.body.threads.alias = primary_pgo.body.threads.id;

        rsdb_util_store_item_by_id(domain, previous_pgo.body.threads.id,
                                    &previous_pgo);
        rsdb_change_credentials(domain_info[domain].db, &credentials, 
                                    &credentials.uid, item->unix_num, 
                                    credentials.id, credentials.acl_id);

    } else if (credentials.unix_id != item->unix_num) {
        change_credentials = true;
        rsdb_change_credentials(domain_info[domain].db, &credentials, 
                                    &credentials.uid, item->unix_num, 
                                    credentials.id, credentials.acl_id);
    }
    /* The original pgo.acl_id is preserved in import_pgo. */
    import_pgo(&RSDB_PGO(&pgo), item, false);
    rsdb_util_store_item_by_id(domain, pgo.body.threads.id, &pgo);
}


 /*
  * r s d b _ p g o _ r e n a m e
  *
  * Change the name of the pgo item identified by the pathname old_name to
  * the component name new_name. Old_name is a fully qualified pathname
  * relative to domain. New_name is a simple, single-component name.
  *
  * This operation renames a PGO item within its parent directory. This
  * operation does not move a PGO item between directories.
  *
  * If new_name already exists, return sec_rgy_name_exists. If old_name does
  * not exist, return sec_rgy_object_not_found.
  */
PUBLIC void rsdb_pgo_rename
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      old_name,   /* [in] */
    sec_rgy_name_t      new_name,   /* [in] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    rsdb_named_item_t   dummy;
    rsdb_name_key_t     name_key;
    long                name_key_len;
    long                old_name_len;
    long                new_name_len;

    CLEAR_STATUS(status);

    old_name_len    = strlen( (char *) old_name);
    new_name_len    = strlen( (char *) new_name);

    /*
     *   Return if old_name can't be found.
     */
    if (!rsdb_pgo_util_get_by_name(domain, old_name, old_name_len, &pgo)) {
       SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }

    /*
     *   Can't move via rename so '/' is invalid in new_name
     */
    if (strchr((char *) new_name,'/') != NULL) {
        SET_STATUS(status, sec_rgy_bad_name);
        return;
    }

    /*
     *   Use pgo ID to get ID of parent directory for use in new name key.
     *   Return if new_name already exists for some PGO or DIR.
     */
    name_key_len = sizeof(name_key);
    rsdb_util_get_key_of_id(domain, pgo.body.threads.id,
                            (Pointer)&name_key, &name_key_len);
    if(rsdb_util_get_by_name_key(domain, name_key.parent_id, new_name,
                                 new_name_len, &dummy)) {
	SET_STATUS(status, sec_rgy_name_exists);
	return;
    }
    rsdb_util_change_name_key(domain, &pgo, name_key.parent_id,
                              new_name, new_name_len, status);

}   /* End rsdb_pgo_rename. */


 /*
  * r s d b _ p g o _ m o v e
  *
  * Move the pgo item identified by the pathname old_name to
  * new_name.  If the directory hierarchy
  * named by new_name does not yet exist, create the required
  * directories.  Add new_name, then delete old_name.
  *
  * Old_name and new_name are fully qualified pathnames relative to domain.
  *
  * This operation moves a PGO item between directories, perhaps changing
  * its leaf name.  Use rsdb_pgo_rename to rename a PGO within its
  * parent directory.
  *
  * Status Codes
  * sec_rgy_object_not_found - old_name does not exist.
  * sec_rgy_name_exists - new_name already exists.
  * sec_rgy_bad_name - component of new name identifies a pgo
  * sec_rgy_dir_could_not_create - couldn't create required dirs for new_name
  *
  */
PUBLIC void rsdb_pgo_move
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      old_name,   /* [in] */
    sec_rgy_name_t      new_name,   /* [in] */
    error_status_t      *(status)
  )
{
    long                old_name_len;
    long                new_name_len;
    sec_rgy_name_t      old_parent_name;
    sec_rgy_name_t      new_parent_name;
    long                new_parent_len;
    rsdb_pvt_id_t       new_parent_id;
    rsdb_named_item_t   old_pgo;
    rsdb_named_item_t   new_item;
    rsdb_name_key_t     name_key;
    long                name_key_len = sizeof(name_key);
    short               prefix_type;
    error_status_t      tmp_st;

    CLEAR_STATUS(status);
    old_name_len    = ustrlen(old_name);
    new_name_len    = ustrlen(new_name);

    /*
    *   Return if new_name already exists as pgo or dir
    */
    if (rsdb_util_get_by_name(domain, new_name, new_name_len, &new_item)) {
        SET_STATUS(status, sec_rgy_name_exists);
        return;
    }
    /* Make sure parent of new_name doesn't name a pgo.
     * If parent of new_name doesn't exist, create required directories.
     * Get new_parent_id and store new leaf name in name_key.
     */
    ustrcpy(new_parent_name, new_name);
    new_parent_len = ustrlen(new_parent_name);
    rsdb_name_util_strip_leaf(new_parent_name, &new_parent_len,
                              name_key.name, &name_key.name_len);
    if(new_parent_len == 0) {
        new_parent_id = START_OF_LIST;
    }
    else {
        if (rsdb_util_get_by_name(domain, new_parent_name,
                                  new_parent_len, &new_item)) {
            /* If parent is a pgo, return error */
            if (RSDB_IS_PGO(&new_item)) {
                SET_STATUS(status, sec_rgy_bad_name);
                return;
            }
        } else {
            rsdb_util_build_dirs(domain, new_parent_name,
                                 new_parent_len, status);
            if (!rsdb_dir_util_get_by_name(domain, new_parent_name,
                                  new_parent_len, &new_item)) {
                SET_STATUS(status, sec_rgy_dir_could_not_create);
                return;
            }
        }
        new_parent_id = new_item.body.threads.id;
    }
    /*  Return if old_name pgo doesn't exist.  Calling routine should
     *  ensure this is never the case.
     *  Retrieve old_pgo after build_dirs is called so that thread ids are good.
     */
    if (!rsdb_pgo_util_get_by_name(domain, old_name, old_name_len, &old_pgo)) {
        /* delete empty directories we may have created above. */
        rsdb_util_delete_dirs_if_empty(domain, new_parent_name, &tmp_st);
        SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    rsdb_util_change_name_key(domain, &old_pgo, new_parent_id,
                              name_key.name, name_key.name_len, status);
    if(STATUS_OK(status)) {
        /* If move resulted in empty directories in old_name, delete them. */
        if(rsdb_name_util_get_parent(old_name, old_parent_name)) {
            rsdb_util_delete_dirs_if_empty(domain, old_parent_name, &tmp_st);
        }
    }
}


/*
 *  r s d b _ p g o _ g e t _ b y _ n a m e
 */
PUBLIC void rsdb_pgo_get_by_name
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    sec_rgy_pgo_item_t  *(item),    /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    long                name_len;

    CLEAR_STATUS(status);
    name_len = strlen( (char *) name);


    if (!rsdb_pgo_util_get_by_name(domain, name, name_len, &pgo)) {
        cursor->valid = false;
        SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        export_pgo(domain, name, item, &RSDB_PGO(&pgo));
        cursor->valid = true;
        cursor->handle = pgo.body.threads.next;
    }
}


/*
 *  r s d b _ p g o _ g e t _ b y _ u i d
 */
PUBLIC void rsdb_pgo_get_by_uid
  (
    sec_rgy_domain_t    domain,         /* [in] */
    uuid_t              *(uid),         /* [in] */
    boolean32           allow_aliases,  /* [in] */
    sec_rgy_cursor_t    *(cursor),      /* [in, out] */
    sec_rgy_name_t      name,           /* [out] */
    sec_rgy_pgo_item_t  *(item),        /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_credentials_t credentials;

    CLEAR_STATUS(status);

    if (!rsdb_get_credentials_by_uid(domain_info[domain].db,
				     uid, &credentials)) {
        cursor->valid = false;
        SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        rsdb_pgo_util_get_an_alias(domain, &credentials, allow_aliases,
                                   cursor, name, item, status);
    }
}


/*
 *  r s d b _ p g o _ g e t _ b y _ u n i x _ n u m
 */
PUBLIC void rsdb_pgo_get_by_unix_num
  (
    sec_rgy_domain_t    domain,         /* [in] */
    long                unix_num,       /* [in] */
    boolean32           allow_aliases,  /* [in] */
    sec_rgy_cursor_t    *(cursor),      /* [in, out] */
    sec_rgy_name_t      name,           /* [out] */
    sec_rgy_pgo_item_t  *(item),        /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_credentials_t credentials;

    CLEAR_STATUS(status);
    *name = NULL;

    if (!rsdb_get_credentials_by_unix_id(domain_info[domain].db, unix_num,
					 &credentials)) {
        cursor->valid = false;
       SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        rsdb_pgo_util_get_an_alias(domain, &credentials, allow_aliases,
                                   cursor, name, item, status);
    }
}


/*
 *  r s d b _ p g o _ g e t _ n e x t _ i d
 */
PUBLIC void rsdb_pgo_get_next_id
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    sec_rgy_name_t      name,       /* [out] */
    sec_rgy_pgo_item_t  *(item),    /* [out] */
    rsdb_pvt_id_t       *id,        /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    boolean32           item_is_dir = TRUE;

    CLEAR_STATUS(status);

    *name = NULL;

    /*
     * Get next id that identifies a PGO, not a DIR item.
     */
    while(item_is_dir) {
        rsdb_util_get_next_by_id( domain, cursor, &pgo, id, status );
        if(BAD_STATUS(status))
            return;
	if (RSDB_IS_PGO(&pgo))
            item_is_dir = FALSE;
    }
    export_pgo(domain, name, item, &RSDB_PGO(&pgo));
}


/*
 *  r s d b _ p g o _ g e t _ n e x t
 */
PUBLIC void rsdb_pgo_get_next
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_cursor_t    *(cursor),  /* [in, out] */
    sec_rgy_name_t      name,       /* [out] */
    sec_rgy_pgo_item_t  *(item),    /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_pvt_id_t       id;

    rsdb_pgo_get_next_id(domain, cursor, name, item, &id, status);
}


 /*
  * r s d b _ p g o _ g e t _ p r e v _ m a r k e r
  *
  * get_prev_marker: get a marker for the previous object (if it exists.  If
  * not then just return the same marker.)
  */
PUBLIC void rsdb_pgo_get_prev_marker
  (
    sec_rgy_domain_t   domain,         /* [in] */
    sec_rgy_cursor_t   *(cursor),      /* [in] */
    sec_rgy_cursor_t   *(prev_cursor)
  )
{
    rsdb_named_item_t  pgo;

    prev_cursor->valid = false;

    if (cursor->valid &&
        rsdb_pgo_util_get_by_marker(domain, cursor->handle, &pgo)) {
        prev_cursor->valid = true;
        prev_cursor->handle = pgo.body.threads.prev;
    }
}


/*
 * r s d b _ p g o _ u i d _ o f _ n a m e d _ i t e m
 *
 * yield the UUID associated with the named item.
 */
PUBLIC void rsdb_pgo_uid_of_named_item
  (
    sec_rgy_domain_t    domain,     /* [in] */
    rsdb_named_item_t   *named,     /* [in] */
    uuid_t              *(uid)      /* [out] */
  )
{
    rsdb_credentials_t  credentials;

    if (!rsdb_get_credentials_by_unix_id(domain_info[domain].db,
                                            RSDB_PGO(named).unix_id,
                                            &credentials)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_PGO_CREDS_MSG);
    }
    *uid = credentials.uid;
}


/*
 * r s d b _ p g o _ n a m e _ t o _ u i d
 */
PUBLIC void rsdb_pgo_name_to_uid
  (
    sec_rgy_domain_t    domain,     /* [in] */
    sec_rgy_name_t      name,       /* [in] */
    uuid_t              *(uid),     /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    long                name_len;

    CLEAR_STATUS(status);
    name_len = strlen( (char *) name);

    if (!rsdb_pgo_util_get_by_name(domain, name, name_len, &pgo)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        rsdb_pgo_uid_of_named_item(domain, &pgo, uid);
    }
}



/*
 * r s d b _ p g o _ n a m e _ t o _ u n i x _ n u m
 */
PUBLIC void rsdb_pgo_name_to_unix_num
  (
    sec_rgy_domain_t    domain,         /* [in] */
    sec_rgy_name_t      name,           /* [in] */
    long                *(unix_num),    /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    long                name_len;

    CLEAR_STATUS(status);
    name_len = strlen( (char *) name);

    if (!rsdb_pgo_util_get_by_name(domain, name, name_len, &pgo)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        *unix_num = RSDB_PGO(&pgo).unix_id;
    }
}


/*
 *  r s d b _ p g o _ u i d _ t o _ n a m e
 */
PUBLIC void rsdb_pgo_uid_to_name
  (
    sec_rgy_domain_t    domain,       /* [in] */
    uuid_t              *(uid),       /* [in] */
    sec_rgy_name_t      name,         /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_credentials_t  credentials;
    long                name_len;

    CLEAR_STATUS(status);

    if (!rsdb_get_credentials_by_uid(domain_info[domain].db, uid,
				     &credentials)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        rsdb_util_path_construct(domain, credentials.id,
                                 name, &name_len, status);
	name[name_len] = NULL;
    }
}


/*
 *  r s d b _ p g o _ u n i x _ n u m _ t o _ n a m e
 */
PUBLIC void rsdb_pgo_unix_num_to_name
  (
    sec_rgy_domain_t    domain,       /* [in] */
    long                unix_num,     /* [in] */
    sec_rgy_name_t      name,         /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_credentials_t  credentials;
    long                name_len;

    CLEAR_STATUS(status);

    if (!rsdb_get_credentials_by_unix_id(domain_info[domain].db, unix_num,
				     &credentials)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        rsdb_util_path_construct(domain, credentials.id,
                                 name, &name_len, status);
	name[name_len] = NULL;
    }
}


/*
 *  r s d b _ p g o _ u i d _ t o _ u n i x _ n u m
 */
PUBLIC void rsdb_pgo_uid_to_unix_num
  (
    sec_rgy_domain_t    domain,       /* [in] */
    uuid_t              *(uid),       /* [in] */
    long                *(unix_num),  /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_credentials_t credentials;

    CLEAR_STATUS(status);

    if (!rsdb_get_credentials_by_uid(domain_info[domain].db, uid,
				     &credentials)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        *unix_num = credentials.unix_id;
    }
}


/*
 *  r s d b _ p g o _ u n i x _ n u m _ t o _ u i d
 */
PUBLIC void rsdb_pgo_unix_num_to_uid
  (
    sec_rgy_domain_t    domain,     /* [in] */
    long                unix_num,   /* [in] */
    uuid_t              *(uid),     /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_credentials_t credentials;

    CLEAR_STATUS(status);

    if (!rsdb_get_credentials_by_unix_id(domain_info[domain].db, unix_num,
					 &credentials)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        *uid = credentials.uid;
    }
}


/*
 * r s d b _ p g o _ a d d _ m e m b e r
 */
PUBLIC void rsdb_pgo_add_member
  (
    sec_rgy_domain_t    domain,         /* [in] */
    sec_rgy_name_t      go_name,        /* [in] */
    sec_rgy_name_t      person_name,    /* [in] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   go;
    rsdb_named_item_t   person;
    sec_rgy_pgo_item_t  xgo;
    sec_rgy_name_t      xgo_name;
    long                go_name_len,
                        person_name_len;
    sec_rgy_domain_t	domain_person = sec_rgy_domain_person;
    sec_rgy_name_t	*p_name;
    boolean32		islocal=true;

    CLEAR_STATUS(status);

    go_name_len = strlen( (char *) go_name);
    
    islocal =  rsdb_name_util_is_foreign_pgo((char *)person_name)==false;
    if ( ! islocal ) {
	domain_person = sec_rgy_domain_foreign_person;
	/* skip leading '/' */
	p_name = (sec_rgy_name_t *) &person_name[1];
    } else {
	p_name = (sec_rgy_name_t *) &person_name[0];
    }
    person_name_len = strlen( (char *) p_name);

    if (domain == sec_rgy_domain_person) {
	SET_STATUS(status, sec_rgy_bad_domain);
    } else {
        if (!rsdb_pgo_util_get_by_name(domain, go_name, go_name_len, &go)
            || !rsdb_pgo_util_get_by_name(domain_person, 
		*p_name, person_name_len, &person)) {
	    SET_STATUS (status, sec_rgy_object_not_found);
        } else {
            rsdb_pgo_util_member_list_add(domain, 
			domain_person, &go, &person);
            if (domain == sec_rgy_domain_group) {
                rsdb_pgo_util_member_list_add(domain_person, domain,
			&person, &go);
            }
        }
    }
}


/*
 *  r s d b _ p g o _ d e l e t e _ m e m b e r
 */
PUBLIC void rsdb_pgo_delete_member
  (
    sec_rgy_domain_t    domain,         /* [in] */
    sec_rgy_name_t      go_name,        /* [in] */
    sec_rgy_name_t      person_name,    /* [in] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   aggregate;
    rsdb_named_item_t   member;
    sec_rgy_pgo_item_t  xgo;
    sec_rgy_name_t      xgo_name;
    long                go_name_len,
                        person_name_len;
    sec_rgy_domain_t	domain_person = sec_rgy_domain_person;
    sec_rgy_name_t	*p_name;
    boolean32		islocal=true;

    CLEAR_STATUS(status);
    go_name_len = strlen( (char *) go_name);

    islocal =  rsdb_name_util_is_foreign_pgo((char *)person_name)==false;

    if ( ! islocal ) {
	domain_person = sec_rgy_domain_foreign_person;
	p_name = (sec_rgy_name_t *) &person_name[1];
    } else {
	p_name = (sec_rgy_name_t *) &person_name[0];
    }
    person_name_len = strlen( (char *) p_name);


    if (domain == sec_rgy_domain_person) {
	SET_STATUS(status, sec_rgy_bad_domain);
    } else {
        if (!rsdb_pgo_util_get_by_name(domain, go_name, go_name_len,
                                       &aggregate)) {
	    SET_STATUS(status, sec_rgy_object_not_found);
            return;
        }
        if (!rsdb_pgo_util_get_by_name(domain_person, 
			*p_name, person_name_len, &member)) {
	    SET_STATUS(status, sec_rgy_object_not_found);
            return;
        }

        rsdb_pgo_util_delete_member(domain, &aggregate, &member);

        if (domain == sec_rgy_domain_group) {
            /* delete group from project list */
            rsdb_pgo_util_delete_member(domain_person, &member, &aggregate);
	    if ( ( ! islocal ) && ( RSDB_PGO(&member).num_members == 0 ) ) {
		/* 
		 * is no longer a member of any group, so delete this
		 * foreign principal ( and its dir if empty ) from the 
		 * database.
		 */
		rs_util_pgo_delete(domain_person, *p_name, 
			status);
	    }
        }

	if ( islocal ) {
            rsdb_pgo_util_delete_member_accounts(domain, &aggregate, &member);
	}
    }
}


/*
  * r s d b _ p g o _ g e t _ m e m b e r s _ a l l
  *
  * Note about membership marker - The marker handle corresponds to the n'th
  * active member.
  */
PRIVATE void rsdb_pgo_get_members_all
  (
    boolean32           unix_semantics,      /* [in] */
    sec_rgy_domain_t    domain,              /* [in] */
    rsdb_named_item_t	*pgo,                /* [in] */
    sec_rgy_cursor_t    *(member_cursor),    /* [in, out] */
    signed32            max_members,         /* [in] */
    sec_rgy_member_t    member_list[],       /* [out] */
    signed32            *(number_supplied),  /* [out] */
    signed32            *(number_members),   /* [out] */
    error_status_t      *(status)
  )
{
    int                 count = 0;
    int                 element;
    int                 num_slots;
    int                 num;
    int                 i;
    rsdb_pvt_id_t       list_id;
    long                member_len;
    sec_rgy_domain_t    member_domain;
    sec_rgy_domain_t	data_domain;
    sec_rgy_name_t	member_name;
    rsdb_max_membership_list_t list;

    CLEAR_STATUS(status);

    *number_supplied = 0;

    /*
     * domain specifies the target object domain that we want its
     * member to be retrieved.  Membership id list are stored in this
     * domain.  After getting the list, we need to retrieve members
     * for more information ( e.g. name ) from its member database, hence
     * using the member_domain.  data_domain is introduced for multi-cell
     * group work.  It indicates that for a group members retrieveal,
     * if this is set to be sec_rgy_domain_foreign_person then we are
     * getting the group foreign principal members.  The purpose of
     * this variable is serving as a flag so we can reuse the codes
     * of the following two loops.
     */
    data_domain = domain;

    if (domain == sec_rgy_domain_person || 
		domain == sec_rgy_domain_foreign_person ) {
        member_domain = sec_rgy_domain_group;
    } else {
        member_domain = sec_rgy_domain_person;
    }

    *number_members = RSDB_PGO(pgo).num_members + 
	      		RSDB_PGO(pgo).num_foreign_members;
    if(*number_members == 0) {
        return;
    }

    if (member_cursor->valid) {
	/* 
	 * not the first time this operation for the same pgo, 
	 * find where stop last time.
	 */
        if (member_cursor->handle > *number_members) {
	    SET_STATUS(status, sec_rgy_no_more_entries);
            return;
        }
        for (list_id = RSDB_PGO(pgo).membership; ;
                list_id = list.next,
                count += list.count) {

            if (list_id == END_OF_LIST) {
		if ( data_domain == sec_rgy_domain_group ) {
		    list_id = RSDB_PGO(pgo).foreign_membership;
		    data_domain = member_domain = sec_rgy_domain_foreign_person;
		}
		if ( list_id == END_OF_LIST ) {
                    dce_svc_printf(SEC_RSDB_END_MEMB_LIST_MSG);
		}
	    }

            rsdb_util_get_member_list(domain, list_id, &list);

            if ((count + list.count) >= member_cursor->handle)
                break;
        }
        num_slots = member_cursor->handle - count;
        i = 0;
        for (element = 0; element < list.total_count, i < num_slots; element++) {
            if (list.member[element].deleted == 0) {
                i++;
            }
        }
    }
    else {
        rsdb_util_get_member_list(domain, RSDB_PGO(pgo).membership, &list);
        element = 0;
        member_cursor->handle = 0;
    }
    member_cursor->valid = true;
    num = 0;
    while (num < max_members) {
        for (i = element; i < list.total_count; i++) {

            if (list.member[i].deleted == 1)
                continue;

            member_cursor->handle++;

            if (unix_semantics && list.member[i].has_acct == 1)
                continue;

            rsdb_util_path_construct(member_domain, list.member[i].id,
                                     member_name, &member_len, status);
	    if ( member_domain == sec_rgy_domain_foreign_person ) {
		member_list[num][0] = '/';
		memcpy(&member_list[num][1], member_name, member_len);
		member_len++;
	    } else {
		memcpy(member_list[num], member_name, member_len);
	    }
            member_list[num][member_len] = NULL;
            num++;

            if (num == max_members)
                break;
        }
        if (num != max_members) {
	    rsdb_pvt_id_t	list_next=list.next;
            if (list_next == END_OF_LIST) {
		if ( data_domain == sec_rgy_domain_group ) {
		    /* check any foreign principal member existed for group */
		    list_next = RSDB_PGO(pgo).foreign_membership;
		    data_domain = member_domain = sec_rgy_domain_foreign_person;
		}
		if ( list_next == END_OF_LIST ) {
		    member_cursor->valid = false;
                    break;
		}
            }
            rsdb_util_get_member_list(domain, list_next, &list);
            element = 0;
        }
    }

    *number_supplied = num;
}



/*
  * r s d b _ p g o _ g e t _ m e m b e r s _ f o r e i g n _ g r o u p s e t s
  *
  * Note about membership marker - The marker handle corresponds to the n'th
  * active member.
  */
PUBLIC void  rsdb_pgo_get_foreign_groupsets
  (
    boolean32           unix_semantics,      /* [in] */
    uuid_t		*cell_uuid,	     /* [in] */
    uuid_t		*princ_uuid,	     /* [in] */
    sec_rgy_cursor_t    *(group_cursor),     /* [in, out] */
    signed32            max_groups,          /* [in] */
    sec_rgy_member_t    group_list[],        /* [out] */
    signed32            *(number_supplied),  /* [out] */
    signed32            *(number_groups),    /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    rsdb_pvt_id_t	id;
    rsdb_credentials_foreign_t cred;

    if ( ! rsdb_get_credentials_by_foreign_uuids(
		domain_info[sec_rgy_domain_foreign_person].db, 
		cell_uuid, princ_uuid, &cred) ) {
	SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }
    id = cred.id;
    if (!rsdb_pgo_util_get_by_id(sec_rgy_domain_foreign_person, 
		id, &pgo)) {
	SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }

    rsdb_pgo_get_members_all(unix_semantics, sec_rgy_domain_foreign_person, 
	&pgo, group_cursor, max_groups, group_list, number_supplied, 
	number_groups, status);

}



/*
  * r s d b _ p g o _ g e t _ m e m b e r s
  *
  * Note about membership marker - The marker handle corresponds to the n'th
  * active member.
  */
PUBLIC void rsdb_pgo_get_members
  (
    boolean32           unix_semantics,      /* [in] */
    sec_rgy_domain_t    domain,              /* [in] */
    sec_rgy_name_t      pgo_name,             /* [in] */
    sec_rgy_cursor_t    *(member_cursor),    /* [in, out] */
    signed32            max_members,         /* [in] */
    sec_rgy_member_t    member_list[],       /* [out] */
    signed32            *(number_supplied),  /* [out] */
    signed32            *(number_members),   /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   pgo;
    long                pgo_name_len;

    CLEAR_STATUS(status);

    pgo_name_len = strlen( (char *) pgo_name);
    if (!rsdb_pgo_util_get_by_name(domain, pgo_name, pgo_name_len, &pgo)) {
	SET_STATUS(status, sec_rgy_object_not_found);
        return;
    }

    rsdb_pgo_get_members_all(unix_semantics, domain, &pgo, member_cursor,
	max_members, member_list, number_supplied, number_members, status);

}


/*
 *  r s d b _ p g o _ i s _ m e m b e r
 */
PUBLIC boolean32 rsdb_pgo_is_member
  (
    sec_rgy_domain_t    domain,         /* [in] */
    sec_rgy_name_t      go_name,        /* [in] */
    sec_rgy_name_t      person_name,    /* [in] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   go;
    rsdb_named_item_t   person;
    boolean32           result = false;
    long                go_name_len,
                        person_name_len;
    sec_rgy_domain_t	domain_person = sec_rgy_domain_person;
    sec_rgy_name_t	*p_name;
    boolean32		islocal=true;

    CLEAR_STATUS(status);
    go_name_len = strlen( (char *) go_name);


    islocal =  rsdb_name_util_is_foreign_pgo((char *)person_name)==false;

    if ( ! islocal ) {
	domain_person = sec_rgy_domain_foreign_person;
	p_name = (sec_rgy_name_t *) &person_name[1];
    } else {
	p_name = (sec_rgy_name_t *) &person_name[0];
    }
    person_name_len = strlen( (char *) p_name);

    if (domain == sec_rgy_domain_person) {
	SET_STATUS(status, sec_rgy_bad_domain);
    } else if (!rsdb_pgo_util_get_by_name(domain, go_name, go_name_len, &go)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else if (!rsdb_pgo_util_get_by_name(domain_person, *p_name,
				person_name_len, &person)) {
	SET_STATUS(status, sec_rgy_object_not_found);
    } else {
	if ( islocal ) {
	    result = rsdb_util_is_member(domain, RSDB_PGO(&go).membership,
                                     person.body.threads.id,
                                     (rsdb_pvt_id_t *) NULL);
	} else {
	    result = rsdb_util_is_member(domain, RSDB_PGO(&go).foreign_membership,
                                     person.body.threads.id,
                                     (rsdb_pvt_id_t *) NULL);
	}
    }

    return result;
}


/*
 *  r s d b _ p g o _ p o l i c y _ g e t _ i n f o
 */
PUBLIC void rsdb_pgo_policy_get_info
  (
    sec_rgy_name_t      org_name,       /* [in] */
    sec_rgy_plcy_t      *(policy_data), /* [out] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   org;
    long                org_name_len;

    CLEAR_STATUS(status);
    org_name_len = strlen( (char *) org_name);

    if (rsdb_pgo_util_get_by_name(sec_rgy_domain_org, org_name, org_name_len, &org)) {
        if (!get_policy_data(RSDB_PGO(&org).info, policy_data)) {
	    SET_STATUS(status, sec_rgy_object_not_found);
        }
    } else {
	SET_STATUS(status, sec_rgy_object_not_found);
    }
}


/*
 *  r s d b _ p g o _ p o l i c y _ s e t _ i n f o
 */
PUBLIC void rsdb_pgo_policy_set_info
  (
    sec_rgy_name_t      org_name,       /* [in] */
    sec_rgy_plcy_t      *(policy_data), /* [in] */
    error_status_t      *(status)
  )
{
    rsdb_named_item_t   org;
    long                org_name_len;

    CLEAR_STATUS(status);
    org_name_len = strlen( (char *) org_name);

    if (rsdb_pgo_util_get_by_name(sec_rgy_domain_org, org_name,
                                      org_name_len, &org)) {
        if (RSDB_PGO(&org).info == END_OF_LIST) {
            RSDB_PGO(&org).info = rsdb_new_sequential_id();
            rsdb_util_store_item_by_id(sec_rgy_domain_org,
                                       org.body.threads.id, &org);
        }
        set_policy_data(RSDB_PGO(&org).info, policy_data);
    } else {
	SET_STATUS(status, sec_rgy_object_not_found);
    }

}   /* End rsdb_pgo_policy_set_info. */
