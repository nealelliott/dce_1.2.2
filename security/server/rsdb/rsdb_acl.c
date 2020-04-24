/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_acl.c,v $
 * Revision 1.1.20.3  1996/02/18  00:20:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:50  marty]
 *
 * Revision 1.1.20.2  1995/12/08  17:58:27  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:10 UTC  greg
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge greg_sec_migrate_acls branch.
 * 	[1995/04/18  18:20 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Convert references tio the former #define rsdb_acl_permset_*
 * 	to calls to the new rsdb_acl_permset_*() function.
 * 
 * 	[1995/04/13  20:05 UTC  greg  /main/greg_sec_migrate_acls/2]
 * 
 * 	Add dynamic acl mgr configuration capability.
 * 	[1995/04/13  17:10 UTC  greg  /main/greg_sec_migrate_acls/1]
 * 
 * Revision 1.1.16.1  1994/10/17  18:18:22  cuti
 * 	fix case that didn't return boolean in rsdb_acl_mgr_perms_valid
 * 	[1994/10/14  16:16:20  cuti]
 * 
 * Revision 1.1.11.5  1994/08/25  21:25:07  sekhar
 * 	[OT 11751 ] - Sec ACL Manager for SVCIF . Added 's' bit for
 * 	              SVC routing ( used by Policy ACL manager )
 * 	[1994/08/24  20:22:34  sekhar]
 * 
 * Revision 1.1.11.4  1994/08/04  16:14:20  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:04  mdf]
 * 
 * Revision 1.1.11.2  1994/04/22  19:14:08  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 	[1994/04/21  00:18:12  rps]
 * 
 * Revision 1.1.9.2  94/03/29  18:43:11  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * 	HP revision /main/ODESSA_2/2  1994/04/26  17:32 UTC  burati
 * 	merge from secd_auth
 * 
 * 	HP revision /main/ODESSA_2/secd_auth/1  1994/04/22  19:45 UTC  burati
 * 	New entry types for 1.1
 * 
 * 	HP revision /main/ODESSA_ERA2/3  1994/01/31  16:54  ahop
 * 	 merge forward
 * 
 * 	HP revision /main/ODESSA_ERA2/ahop_odss_era_bl5/1  1994/01/31  16:52  ahop
 * 	set read_perm for attr_schema acl_entries[1-3] in create_default
 * 
 * 	HP revision /main/ODESSA_ERA2/2  1994/01/31  14:28  ahop
 * 	 merge forward
 * 
 * 	HP revision /main/ODESSA_ERA/ODESSA_ERA1/ahop_odss_era_bl4/2  1994/01/29  21:27  ahop
 * 	rework rsdb_acl_get_ids_from_name to call rsdb_util_get_obj_info_by_name
 * 
 * 	HP revision /main/ODESSA_ERA/ODESSA_ERA1/ahop_odss_era_bl4/1  1994/01/29  00:02  ahop
 * 	align attr status codes
 * 
 * Revision 1.1.8.2  1993/08/02  20:38:25  hanfei
 * 	fixed for ot 7947 : fix a memory leak of rsdb_acl_t which is
 * 	allocated when rsdb_acl_delete() calls rsdb_acl_get_by_id().
 * 	[1993/08/02  20:37:16  hanfei]
 * 
 * Revision 1.1.5.6  1993/02/19  17:14:35  ahop
 * 	dbcp: add rsdb_acl_get_name_from_ids; fix mem leak (redundant malloc call)
 * 	      in create_default_acl.
 * 	[1993/02/19  08:44:05  ahop]
 * 
 * Revision 1.1.5.5  1992/12/29  16:38:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:58  zeliff]
 * 
 * Revision 1.1.5.4  1992/11/20  15:54:17  ahop
 * 	Make acl_create safe to call more than once
 * 	[1992/11/10  23:13:53  ahop]
 * 
 * Revision 1.1.5.3  1992/09/29  20:13:48  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:39:39  sekhar]
 * 
 * Revision 1.1.5.2  1992/08/31  18:28:01  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:12:30  sekhar]
 * 
 * Revision 1.1.2.2  1992/03/30  21:34:18  ahop
 * 	 fix CR 763: replace sec_acl_name_resolution_failed with sec_acl_object_not_found
 * 	[1992/03/30  19:03:03  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *  ACL database routines.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
	"$Id: rsdb_acl.c,v 1.1.20.3 1996/02/18 00:20:55 marty Exp $";
#endif

#include <dce/dce.h>
#include <dce/uuid.h>
#include <dce/aclbase.h>

#include <stdio.h>
#include <rsdb_pvt.h>
#include <rsdb_create.h>
#include <rsdb_acl.h>
#include <rsdb.h>
#include <rsdb_name_util.h>
#include <rsdb_util.h>
#include <rsdb_policy.h>
#include <rs_util.h>
#include <bstring.h>

#include <sec_svc.h>

/*
 * Local Global Variables
 *
 * first and last are not rsdb_acl_fixed_t because only the threads and
 * acl_len fields are used.  The entries field is never accessed.
 */
PRIVATE rsdb_acl_t  *first = 0;
PRIVATE rsdb_acl_t  *last = 0;

/* Use rsdb_acl_mgr_type_t as array index */
PUBLIC uuid_t   rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_last] = {
      { /* 06ab8f10-0191-11ca-a9e8-08001e039d7d ** rsdb_acl_mgr_type_rgy    */
        0x06ab8f10,
        0x0191,
        0x11ca,
        0xa9,
        0xe8,
        {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      { /* 06ab9320-0191-11ca-a9e8-08001e039d7d ** rsdb_acl_mgr_type_person */
        0x06ab9320,
        0x0191,
        0x11ca,
        0xa9,
        0xe8,
        {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      { /* 06ab9640-0191-11ca-a9e8-08001e039d7d ** rsdb_acl_mgr_type_group  */
        0x06ab9640,
        0x0191,
        0x11ca,
        0xa9,
        0xe8,
        {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      { /* 06ab9960-0191-11ca-a9e8-08001e039d7d ** rsdb_acl_mgr_type_org    */
        0x06ab9960,
        0x0191,
        0x11ca,
        0xa9,
        0xe8,
        {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      { /* 06ab9c80-0191-11ca-a9e8-08001e039d7d ** rsdb_acl_mgr_type_dir    */
        0x06ab9c80,
        0x0191,
        0x11ca,
        0xa9,
        0xe8,
        {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      { /* 2ac24970-60c3-11cb-b261-08001e039d7d ** rsdb_acl_mgr_type_replist */
        0x2ac24970,
        0x60c3,
        0x11cb,
        0xb2,
        0x61,
        {0x08, 0x00, 0x1e, 0x03, 0x9d, 0x7d}
      },
      { /* 755cd9ce-ded3-11cc-8d0a-080009353559 rsdb_acl_mrg_type_attr_schema */
        0x755cd9ce,
        0xded3,
        0x11cc,
        0x8d,
        0x0a,
        {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
      }
	
};

/* Printstrings for all Registry permissions. */
PUBLIC sec_acl_printstring_t rsdb_acl_perm_strings[] = {
    { "r",  "read",                 sec_acl_perm_read           },
    { "c",  "control",              sec_acl_perm_owner          },
    { "i",  "insert",               sec_acl_perm_insert         },
    { "d",  "delete",               sec_acl_perm_delete         },
    { "t",  "test",                 sec_acl_perm_test           },
    { "D",  "Delete object",        sec_acl_perm_Delete_object  },
    { "n",  "name",                 sec_acl_perm_name           },
    { "f",  "fullname",             sec_acl_perm_fullname       },
    { "m",  "management info",      sec_acl_perm_mgmt_info      },
    { "a",  "authentication info",  sec_acl_perm_auth_info      },
    { "u",  "user info",            sec_acl_perm_user_info      },
    { "g",  "groups",               sec_acl_perm_groups         },
    { "M",  "Member list",          sec_acl_perm_Member_list    },
    { "A",  "Administration",       sec_acl_perm_Admin          },
    { "I",  "Initialize",           sec_acl_perm_Initialize     },
/* -------------------------------------------------------------- */
    { "O",  "generic",		    sec_acl_perm_O      	},
    { "P",  "generic",		    sec_acl_perm_P      	},
    { "Q",  "generic",		    sec_acl_perm_Q      	},
    { "R",  "generic",		    sec_acl_perm_R      	},
    { "S",  "generic",		    sec_acl_perm_S      	},
    { "T",  "generic",		    sec_acl_perm_T      	},
    { "U",  "generic",		    sec_acl_perm_U      	},
    { "V",  "generic",		    sec_acl_perm_V      	},
    { "W",  "generic",		    sec_acl_perm_W      	},
    { "X",  "generic",		    sec_acl_perm_X      	},
    { "Y",  "generic",		    sec_acl_perm_Y      	},
    { "Z",  "generic",		    sec_acl_perm_Z      	},
    { "s",  "Serviceability",       sec_acl_perm_svc            }
};


/* r s d b _ a c l _ r e s e t _ n e x t
 *
 * Reset the next pointer of acl (with id acl_id) to new_next.
 * WARNING: This routine, using balanced_trees as the underlying
 * database manager, makes data modifications directly
 * to stored data.
 */
PRIVATE void rsdb_acl_reset_next (
    rsdb_pvt_id_t   acl_id,
    rsdb_pvt_id_t   new_next
)
{
    rsdb_sequential_key_t   sequential_key;
    Pointer                 data;
    int                     data_len;
    rsdb_acl_t              *acl;
    error_status_t          status;
    error_status_t          *st = &status;

    /* Update the global "first" if necessary. */
    if (acl_id == first->threads.id) {
        first->threads.next = new_next;
    }

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = acl_id;

    rsdb_fetch(rsdb_acl_db, (Pointer) &sequential_key, sizeof(sequential_key),
                        &data, &data_len, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(SEC_RSDB_FETCH_ERROR_MSG);
    }
    acl = (rsdb_acl_t *)data;
    acl->threads.next = new_next;

}


/* r s d b _ a c l _ r e s e t _ p r e v
 *
 * Reset the prev pointer of acl (with id acl_id) to new_prev.
 * WARNING: This routine, using balanced_trees as the underlying
 * database manager, makes data modifications directly
 * to stored data.
 */
PRIVATE void rsdb_acl_reset_prev (
    rsdb_pvt_id_t   acl_id,
    rsdb_pvt_id_t   new_prev
)
{
    rsdb_sequential_key_t   sequential_key;
    Pointer                 data;
    int                     data_len;
    rsdb_acl_t              *acl;
    error_status_t          status;
    error_status_t          *st = &status;

    /* Update the global "last" if necessary. */
    if (acl_id == last->threads.id) {
        last->threads.prev = new_prev;
    }

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = acl_id;

    rsdb_fetch(rsdb_acl_db, (Pointer) &sequential_key, sizeof(sequential_key),
                        &data, &data_len, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(SEC_RSDB_FETCH_ERROR_MSG);
    }
    acl = (rsdb_acl_t *)data;
    acl->threads.prev = new_prev;
}


/* p u r g e _ a c l _ b y _ i d
 *
 * Delete the acl identified by acl_id from the acl database.
 *
 * Status Codes:
 *      sec_acl_no_acl_found            if acl not found with acl_id.
 *      error_status_ok                 otherwise.
 */
PRIVATE void purge_acl_by_id (
    rsdb_pvt_id_t   acl_id,
    error_status_t  *st
)
{
    rsdb_sequential_key_t  sequential_key;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = acl_id;

    rsdb_delete(rsdb_acl_db, (Pointer) &sequential_key,
                sizeof(sequential_key), st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_acl_no_acl_found);
    }
}


/* d u p l i c a t e _ s e c _ a c l
 *
 * Sec_acl points to a sec_acl_t to be duplicated in
 * new_acl.  New_acl must point to adequate storage
 * for the fixed part of a sec_acl_t.  New_var_p
 * points to adequate storage for the variable-length
 * parts of the acl.  After copying the variable length
 * parts, new_var_p is advanced past the copied data to
 * point at unused storage area.
 *
 * Status Codes:
 *      error_status_ok
 */
PRIVATE void duplicate_sec_acl (
    sec_acl_t           *sec_acl,
    sec_acl_t           *new_acl,
    unsigned_char_p_t   *new_var_p
)
{
    int                 i;
    unsigned_char_p_t   buf;
    sec_id_t            *sec_id, *new_id;
    sec_id_foreign_t    *sec_for_id, *new_for_id;

    /* Initialize the sec_acl_entries pointer */
    buf = (unsigned_char_p_t)new_acl;
    new_acl->sec_acl_entries = (sec_acl_entry_t *)(buf + sizeof(sec_acl_t));

    new_acl->default_realm.uuid = sec_acl->default_realm.uuid;
    new_acl->sec_acl_manager_type = sec_acl->sec_acl_manager_type;
    new_acl->num_entries = sec_acl->num_entries;
    new_acl->default_realm.name = *new_var_p;
    u_strcpy(new_acl->default_realm.name, sec_acl->default_realm.name);
    *new_var_p += u_strlen(new_acl->default_realm.name) + 1;

    for(i = 0; i < new_acl->num_entries; i++) {
        new_acl->sec_acl_entries[i].perms =
            sec_acl->sec_acl_entries[i].perms;
        new_acl->sec_acl_entries[i].entry_info.entry_type =
            sec_acl->sec_acl_entries[i].entry_info.entry_type;
        switch (new_acl->sec_acl_entries[i].entry_info.entry_type) {
        case sec_acl_e_type_user_obj:
        case sec_acl_e_type_group_obj:
        case sec_acl_e_type_other_obj:
        case sec_acl_e_type_unauthenticated:
        case sec_acl_e_type_any_other:
        case sec_acl_e_type_user_obj_deleg:
        case sec_acl_e_type_group_obj_deleg:
        case sec_acl_e_type_other_obj_deleg:
        case sec_acl_e_type_any_other_deleg:
            /* ... nothing to copy ... */
            break;

        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
            /* Copy id */
            sec_id = &(sec_acl->sec_acl_entries[i].entry_info.tagged_union.id);
            new_id = &(new_acl->sec_acl_entries[i].entry_info.tagged_union.id);
            new_id->uuid = sec_id->uuid;
            new_id->name = *new_var_p;
            u_strcpy(new_id->name, sec_id->name);
            *new_var_p += u_strlen(new_id->name) + 1;
            break;

        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:
            /* Copy foreign_id.id */
            sec_for_id =
                &(sec_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id);
            new_for_id =
                &(new_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id);
            new_for_id->id.uuid = sec_for_id->id.uuid;
            new_for_id->id.name = *new_var_p;
            u_strcpy(new_for_id->id.name, sec_for_id->id.name);
            *new_var_p += u_strlen(new_for_id->id.name) + 1;

            /* Copy foreign_id.realm */
            new_for_id->realm.uuid = sec_for_id->realm.uuid;
            new_for_id->realm.name = *new_var_p;
            u_strcpy(new_for_id->realm.name, sec_for_id->realm.name);
            *new_var_p += u_strlen(new_for_id->realm.name) + 1;
            break;

        case sec_acl_e_type_extended:
        default:
            break;
        } /* end switch */
    }     /* end for loop */
}


/* r s d b _ a c l _ c r e a t e
 *
 *  Create and store the acl list head and tail.
 *  This routine either succeeds or fails with fatal error.
 *  This operation is called in sec_create_db when creating
 *  the initial database; in this case, first and last are NULL.
 *  It may also be called in secd during re-initialization of
 *  a slave, in which case, first and last are already allocated.
 *
 */
PUBLIC void rsdb_acl_create (
    void
)
{
    if (first == 0 && last == 0) {
        if (!(first = (rsdb_acl_t *) malloc(sizeof(rsdb_acl_fixed_t)))) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_s_no_memory);
        }
        if (!(last = (rsdb_acl_t *) malloc(sizeof(rsdb_acl_fixed_t)))) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_s_no_memory);
        }
    }

    bzero((Pointer)first, sizeof(rsdb_acl_fixed_t));
    first->threads.id = START_OF_LIST;
    first->threads.next = END_OF_LIST;
    first->threads.prev = END_OF_LIST;
    first->acl_len = sizeof(rsdb_acl_fixed_t);

    rsdb_acl_store_by_id(first->threads.id, first);

    bzero((Pointer) last, sizeof(rsdb_acl_fixed_t));
    last->threads.id = END_OF_LIST;
    last->threads.next = START_OF_LIST;
    last->threads.prev = START_OF_LIST;
    last->acl_len = sizeof(rsdb_acl_fixed_t);

    rsdb_acl_store_by_id(last->threads.id, last);
}


/* r s d b _ a c l _ i n i t
 *
 * Retrieve from database and initialize the acl list head and tail.
 * This routine either succeeds or fails with fatal error.
 */
PUBLIC void rsdb_acl_init (
    void
)
{
    error_status_t          status;
    error_status_t          *st = &status;

    if (!rsdb_acl_get_by_id(START_OF_LIST, &first, st)) {
        dce_svc_printf(SEC_RSDB_CANT_INIT_ACL_MSG);
    }
    if (!rsdb_acl_get_by_id(END_OF_LIST, &last, st)) {
        dce_svc_printf(SEC_RSDB_CANT_INIT_ACL_MSG);
    }
}


/* r s d b _ a c l _ g e t _ m g r _ t y p e _ f r o m _ u u i d
 *
 * Use input acl manager type uuid to determine
 * output acl manager type.
 *
 * Status Codes:
 *      sec_acl_unknown_manager_type - input manager type uuid not found.
 *      error_status_ok.
 */
PUBLIC boolean32 rsdb_acl_get_mgr_type_from_uuid (
    uuid_t                  *uuid,
    rsdb_acl_mgr_type_t     *type,
    error_status_t          *st
)
{
    char                    *uuid_str;
    rsdb_acl_mgr_type_t     i;
    error_status_t          tmp_status;

    CLEAR_STATUS(st);

    for (i = rsdb_acl_mgr_type_rgy; i < rsdb_acl_mgr_type_last; i++) {
        if (UUID_EQUAL(uuid, &rsdb_acl_mgr_uuids[i], &tmp_status)) {
            *type = i;
            return true;
        }
    }
    SET_STATUS(st, sec_acl_unknown_manager_type);
    return false;
}


/* r s d b _ a c l _ g e t _ m g r _ u u i d _ f r o m _ t y p e
 *
 * Use input acl manager type to determine
 * output acl manager type uuid.
 *
 * Status Codes:
 *      sec_acl_unknown_manager_type - input manager type not legal value.
 *      error_status_ok.
 */
PUBLIC boolean32 rsdb_acl_get_mgr_uuid_from_type (
    rsdb_acl_mgr_type_t     type,
    uuid_t                  *uuid,
    error_status_t          *st
)
{
    CLEAR_STATUS(st);

    if (!KNOWN_MGR_TYPE(type)) {
        SET_STATUS(st, sec_acl_unknown_manager_type);
        return false;
    }
    *uuid = rsdb_acl_mgr_uuids[type];

    return true;
}


/* r s d b _ a c l _ a l l o c _ d b _ a c l
 *
 * Malloc an rsdb_acl_t buffer large enough to hold the fixed parts of an
 * rsdb_acl_t and the variable length parts of the input sec_acl_t.
 * If sec_acl has no entries, keep one rsdb_acl_entry for access to
 * variable length data in rsdb_acl_t.
 * Names are no longer being stored in db_acls, so don't
 * account for their lengths.
 * Set the acl length field.
 * Buffer allocated for new_acl must be freed by caller.
 *
 * Status Codes:
 *      sec_acl_invalid_entry_type      if bad entry in input acl.
 *      sec_acl_cant_allocate_memory    if malloc fails.
 *      error_status_ok                 otherwise.
 */
PUBLIC void rsdb_acl_alloc_db_acl (
    sec_acl_t       *acl,
    rsdb_acl_t      **new_acl,
    error_status_t  *st
)
{
    int                 i;
    int                 acl_len;
    unsigned_char_p_t   new_buf;

    CLEAR_STATUS(st);
    *new_acl = (rsdb_acl_t *)NULL;

    acl_len = sizeof(rsdb_acl_fixed_t);

    if (acl->num_entries > 0) {
        acl_len -= sizeof(rsdb_acl_entry_t);
    }
    for(i = 0; i < acl->num_entries; i++ ) {
        if (VALID_ENTRY_TYPE(&(acl->sec_acl_entries[i]))) {
            acl_len += DB_ACL_ENTRY_LEN(&(acl->sec_acl_entries[i]));
        } else {
            SET_STATUS(st, sec_acl_invalid_entry_type);
            return;
        }
    }

    if (!(new_buf = (unsigned_char_p_t) malloc(acl_len))) {
        SET_STATUS(st, sec_acl_cant_allocate_memory);
        return;
    }
    *new_acl = (rsdb_acl_t *) new_buf;
    bzero((Pointer)(*new_acl), sizeof(rsdb_acl_fixed_t));
    /* store total length of new_acl buffer. */
    (*new_acl)->acl_len = acl_len;

    return;
}


/* r s d b _ a c l _ a l l o c _ s e c _ a c l
 *
 * Allocate a sec_acl_t buffer large enough to hold the fixed parts of a
 * sec_acl_t and the variable length parts of the input rsdb_acl_t.
 * Initialize (*new_acl)->sec_acl_entries.
 * new_acl must be freed by caller.
 *
 * Status Codes:
 *      sec_acl_cant_allocate_memory    if malloc fails.
 *      error_status_ok                 otherwise.
 */
PUBLIC void rsdb_acl_alloc_sec_acl (
    rsdb_acl_t      *db_acl,
    sec_acl_t       **new_acl,
    unsigned32      *new_acl_len,
    error_status_t  *st
)
{
    int                 i;
    unsigned_char_p_t   new_buf;

    CLEAR_STATUS(st);
    *new_acl = (sec_acl_t *)NULL;

    *new_acl_len = sizeof(sec_acl_t);
    *new_acl_len += (db_acl->default_realm.name_len + 1);

    for(i = 0; i < db_acl->num_entries; i++ ) {
        /* Assume entry_type is valid in stored rsdb_acl so don't check. */
        *new_acl_len += SEC_ACL_ENTRY_LEN(&(db_acl->entries[i]));
    }

    if (!(new_buf = (unsigned_char_p_t)malloc(*new_acl_len))) {
        *new_acl_len = 0;
        SET_STATUS(st, sec_acl_cant_allocate_memory);
    } else {
        *new_acl = (sec_acl_t *)new_buf;
        (*new_acl)->sec_acl_entries =
                        (sec_acl_entry_t *)(new_buf + sizeof(sec_acl_t));
    }
    return;
}


/* r s d b _ a c l _ c o p y _ l i s t _ t o _ r p c _ b u f
 *
 * Use rpc_ss_allocate to allocate a buffer for the input sec_acl_list.
 * Copy the list and each acl on the list into the new buffer.
 * This routine should be called in rdacl_lookup after sec_acl_mgr_lookup
 * so that the sec_acl_list to be marshalled will be automatically freed.
 * NOTE: current implementation assumes only one acl on list.
 *
 * Status Codes:
 *      sec_acl_bad_acl_syntax          if acl_list has more/less than one acl.
 *      sec_acl_cant_allocate_memory    if rpc_ss_allocate fails.
 *      error_status_ok                 otherwise.
 */
PUBLIC void rsdb_acl_copy_list_to_rpc_buf (
    rsdb_acl_ids_t  *acl_ids,
    sec_acl_list_t  *sec_acl_list,
    sec_acl_list_t  **new_acl_list,
    error_status_t  *st
)
{
    int                 i;
    unsigned_char_p_t   buf;
    unsigned32          list_size;
    sec_acl_t           *sec_acl, *new_acl;
    unsigned_char_p_t   new_var_p;

    CLEAR_STATUS(st);
    *new_acl_list = (sec_acl_list_t *)NULL;
    if (sec_acl_list->num_acls != 1) {
        SET_STATUS(st, sec_acl_bad_acl_syntax);
        return;
    }

    /* Allocate buffer large enough for the new_acl_list header
     * and the acl it contains.
     */
    list_size = sizeof(sec_acl_list_t);
    if (!(buf =
        (unsigned_char_p_t)rpc_ss_allocate(list_size + acl_ids->acl_len))) {
        SET_STATUS(st, sec_acl_cant_allocate_memory);
        return;
    }
    *new_acl_list = (sec_acl_list_t *)buf;
    (*new_acl_list)->num_acls = sec_acl_list->num_acls;
    (*new_acl_list)->sec_acls[0] = (sec_acl_t *)(buf + list_size);

    /* Copy the acl from sec_acl_list to new_acl_list */
    sec_acl = sec_acl_list->sec_acls[0];
    new_acl = (*new_acl_list)->sec_acls[0];
    new_var_p = (unsigned_char_p_t)new_acl + sizeof(sec_acl_t) +
            (sizeof(sec_acl_entry_t) * sec_acl->num_entries);
    duplicate_sec_acl(sec_acl, new_acl, &new_var_p);
    return;
}


/* r s d b _ a c l _ f r e e _ l i s t
 *
 * Free the buffers for each acl on the acl_list.
 * Free the acl_list buffer.
 *
 */
PUBLIC void rsdb_acl_free_list (
    sec_acl_list_t  **sec_acl_list
)
{
    sec_acl_t       *sec_acl;
    int             i;

    if (!(*sec_acl_list)) {
        return;
    }

    for ( i = 0; i < (*sec_acl_list)->num_acls; i++ ) {
        if ( sec_acl = (*sec_acl_list)->sec_acls[i] ) {
            free((char *)sec_acl);
        }
    }
    free((char *)(*sec_acl_list));
    *sec_acl_list = (sec_acl_list_t *)NULL;
}


/* r s d b _ a c l _ c r e a t e _ d e f a u l t _ a c l
 *
 * Given input mgr_type and using hard-coded rules for initial default acls,
 * create (or reference) an ACL and return its ID.
 * The rules are as follow:
 *
 * Mgr_type    Entries (entry_type, [entry_info,] permissions)
 * --------    -----------------------------------------------
 * POLICY      e_type_unauth,                      r-------------
 *             e_type_any_other,                   r-------------
 *             e_type_other_obj,                   r-------------
 *             e_type_user,        <rgy_creator>,  rc------ma---s
 *
 * PRINCIPAL   e_type_unauth,                      r-------------
 *             e_type_any_other,                   r-------------
 *             e_type_other_obj,                   r----------g--
 *             e_type_user,        <rgy_creator>,  rc---Dnfmaug--
 *             e_type_user_obj,                    r-------f-ug--
 *
 * GROUP       e_type_unauth,                      r---t---------
 *             e_type_any_other,                   r---t---------
 *             e_type_other_obj,                   r---t---------
 *             e_type_user,        <rgy_creator>,  rc--tDnfm---M-
 *             e_type_group_obj,                   r---t---------
 *
 * ORG         e_type_unauth,                      r---t---------
 *             e_type_any_other,                   r---t---------
 *             e_type_other_obj,                   r---t---------
 *             e_type_user,        <rgy_creator>,  rc--tDnfm---M-
 *
 * DIR         e_type_unauth,                      r-------------
 *             e_type_any_other,                   r-------------
 *             e_type_other_obj,                   r-------------
 *             e_type_user,        <rgy_creator>,  rcid-Dn-------
 *
 * REPLIST     e_type_user,        <rgy_creator>,  -cid----m----A
 *
 * Current implementation:
 * Check validity of input parameters.  Allocate an rsdb_acl_t
 * large enough for the desired acl based on above rules.
 * Create the acl, store it and return its acl_id to the caller.
 * No name strings are currently stored in rsdb_acls.
 *
 * Future implementation:
 * Lookup a suitable default ACL, increment its reference count and
 * return its id.
 *
 * This operation is fatal if not successful.
 *
 * Note: This operation should only be called in rgy_create, not in rgyd.
 * The local_cell and rgy_creator globals will not be valid in rgyd.
 */
PUBLIC rsdb_pvt_id_t rsdb_acl_create_default_acl (
    sec_id_t            *rgy_creator,
    sec_id_t            *rgy_local_cell,
    rsdb_acl_mgr_type_t mgr_type
)
{
    rsdb_acl_t          *db_acl;
    rsdb_pvt_id_t       acl_id;
    sec_acl_t           sec_acl;
    unsigned32          acl_len;
    unsigned32          cell_name_len;
    unsigned32          creator_len;
    unsigned32          num_entries;
    unsigned_char_p_t   buf_p;
    error_status_t      status;
    error_status_t      *st = &status;

    if (!KNOWN_MGR_TYPE(mgr_type)) {
        dce_svc_printf(SEC_RSDB_UNKNOWN_ACLMGR_TYPE_MSG);
    }
    /* Calculate required size of new acl. */
    /* Don't store names. */
    cell_name_len = 0;
    creator_len   = 0;

    /* num_entries = 5 for principal and group only. */
    if (mgr_type == rsdb_acl_mgr_type_person ||
       mgr_type == rsdb_acl_mgr_type_group ) {
        num_entries = 5;
    } else if (mgr_type == rsdb_acl_mgr_type_replist) {
        num_entries = 1;
    } else {
        num_entries = 4;
    }

    acl_len = sizeof(rsdb_acl_fixed_t) + cell_name_len +
              ((num_entries - 1) * sizeof(rsdb_acl_entry_t)) + creator_len;
    if (!(db_acl = (rsdb_acl_t *)malloc(acl_len))) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_s_no_memory);
    }

    /* Set up the acl header. */
    db_acl->default_realm.uuid      = rgy_local_cell->uuid;
    /* Copy the cell name into the name buffer of the last entry. */
    db_acl->default_realm.name_len  = cell_name_len;
    buf_p = db_acl->entries[num_entries - 1].name_buf;
    u_strncpy(buf_p, rgy_local_cell->name, cell_name_len);
    buf_p += cell_name_len;

    if (!rsdb_acl_get_mgr_uuid_from_type(mgr_type,
	&(db_acl->sec_acl_manager_type), st)) {
            dce_svc_printf(SEC_RSDB_UNKNOWN_ACLMGR_TYPE_MSG);
    }
    db_acl->num_entries             = num_entries;
    db_acl->acl_len                 = acl_len;

    /* Set up acl entries.  Use mgr_type as a guide to rules.
     * Handle mgr_type_replist separately since it doesn't have
     * as much in common with the others.
     * Initialize parts of entries common to all mgr types.
     */
    db_acl->entries[0].entry_type   = sec_acl_e_type_user;
    /* Copy creator identity. */
    db_acl->entries[0].entry_info.id.uuid       = rgy_creator->uuid;
    db_acl->entries[0].entry_info.id.name_len   = creator_len;
    u_strncpy(buf_p, rgy_creator->name, creator_len);
    buf_p += creator_len;

    if (mgr_type != rsdb_acl_mgr_type_replist) {
        db_acl->entries[1].entry_type   = sec_acl_e_type_any_other;
        db_acl->entries[2].entry_type   = sec_acl_e_type_other_obj;
        db_acl->entries[3].entry_type   = sec_acl_e_type_unauthenticated;
    }
    switch( mgr_type ) {
        case rsdb_acl_mgr_type_rgy:
            db_acl->entries[0].perms        = rsdb_acl_permset_rgy();
            db_acl->entries[1].perms        = sec_acl_perm_read;
            db_acl->entries[2].perms        = sec_acl_perm_read;
            db_acl->entries[3].perms        = sec_acl_perm_read;
            break;
        case rsdb_acl_mgr_type_person:
            db_acl->entries[0].perms        = rsdb_acl_permset_person();
            db_acl->entries[1].perms        = sec_acl_perm_read;
            db_acl->entries[2].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_groups;
            db_acl->entries[3].perms        = sec_acl_perm_read;
            db_acl->entries[4].entry_type   = sec_acl_e_type_user_obj;
            db_acl->entries[4].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_fullname     |
                                              sec_acl_perm_user_info    |
                                              sec_acl_perm_groups;
            break;
        case rsdb_acl_mgr_type_group:
            db_acl->entries[0].perms        = rsdb_acl_permset_group();
            db_acl->entries[1].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_test;
            db_acl->entries[2].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_test;
            db_acl->entries[3].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_test;
            db_acl->entries[4].entry_type   = sec_acl_e_type_group_obj;
            db_acl->entries[4].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_test;
            break;
        case rsdb_acl_mgr_type_org:
            db_acl->entries[0].perms        = rsdb_acl_permset_org();
            db_acl->entries[1].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_test;
            db_acl->entries[2].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_test;
            db_acl->entries[3].perms        = sec_acl_perm_read         |
                                              sec_acl_perm_test;
            break;
        case rsdb_acl_mgr_type_dir:
            db_acl->entries[0].perms        = rsdb_acl_permset_dir();
            db_acl->entries[1].perms        = sec_acl_perm_read;
            db_acl->entries[2].perms        = sec_acl_perm_read;
            db_acl->entries[3].perms        = sec_acl_perm_read;
            break;
        case rsdb_acl_mgr_type_replist:
            db_acl->entries[0].perms        = sec_acl_perm_owner        |
                                              sec_acl_perm_insert       |
                                              sec_acl_perm_delete       |
                                              sec_acl_perm_mgmt_info    |
                                              sec_acl_perm_Admin;
            break;
        case rsdb_acl_mgr_type_attr_schema:
	    db_acl->entries[0].perms        = sec_acl_perm_delete	|
					      sec_acl_perm_insert	|
					      sec_acl_perm_mgmt_info	|
					      sec_acl_perm_read		|
					      sec_acl_perm_owner;
            db_acl->entries[1].perms        = sec_acl_perm_read;
            db_acl->entries[2].perms        = sec_acl_perm_read;
            db_acl->entries[3].perms        = sec_acl_perm_read;
            break;
    }

    rsdb_acl_store_new_acl( db_acl, &acl_id );
    free(db_acl);
    return(acl_id);
}


/* r s d b _ a c l _ c o p y
 *
 * Lookup the acl identified by the input acl_id.
 * Create a new acl identical to it.
 * Store the new acl and return its id.
 * Return NULL if unsuccessful.
 */
PUBLIC rsdb_pvt_id_t rsdb_acl_copy (
    rsdb_pvt_id_t       acl_id,
    error_status_t      *st
)
{
    rsdb_acl_t          *new_acl;
    rsdb_pvt_id_t       new_acl_id;

    CLEAR_STATUS(st);
    rsdb_acl_get_by_id(acl_id, &new_acl, st);
    if (BAD_STATUS(st)) {
        return NULL;
    }
    /* Store_new_acl is fatal if not successful. */
    rsdb_acl_store_new_acl(new_acl, &new_acl_id);

    /* Free acl buffer allocated within rsdb_acl_get_by_id. */
    free(new_acl);

    return(new_acl_id);
}


/* r s d b _ a c l _ s t o r e _ n e w _ a c l
 *
 * Store the input rsdb_acl_t in the ACL database.
 * Return the new acl_id generated for it.
 * This routine either succeeds or fails with fatal error.
 *
 */
PUBLIC void rsdb_acl_store_new_acl (
    rsdb_acl_t      *acl,
    rsdb_pvt_id_t   *acl_id
)
{
    error_status_t          status;
    error_status_t          *st = &status;
    rsdb_sequential_key_t   sequential_key;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    /* Create a new sequential ID and store the acl data with it. */
    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = rsdb_new_sequential_id();
    acl->threads.id = *acl_id = sequential_key.id;

    /* Thread the new ACL on the sequential list. */
    acl->threads.next = last->threads.id;
    acl->threads.prev = last->threads.prev;

    rsdb_store(rsdb_acl_db, (Pointer) &sequential_key, sizeof(sequential_key),
               (Pointer)acl, acl->acl_len, st);
    /* Don't bother to check status; any error is fatal in rsdb_store. */

    /* Update threads of next and previous acls in list. */
    rsdb_acl_reset_next(acl->threads.prev, acl->threads.id);
    rsdb_acl_reset_prev(acl->threads.next, acl->threads.id);
}


/* r s d b _ a c l _ s t o r e _ b y _ i d
 *
 * Store the input rsdb_acl_t in the ACL database.
 * This routine either succeeds or fails with fatal error.
 *
 */
PUBLIC void rsdb_acl_store_by_id (
    rsdb_pvt_id_t   acl_id,
    rsdb_acl_t      *acl
)
{
    error_status_t          status;
    error_status_t          *st = &status;
    rsdb_sequential_key_t   sequential_key;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = acl_id;
    rsdb_replace(rsdb_acl_db, (Pointer) &sequential_key,
                 sizeof(sequential_key), (Pointer) acl, acl->acl_len, st);
    /* Don't bother to check status; any error is fatal in rsdb_replace. */
}


/* r s d b _ a c l _ g e t _ b y _ i d
 *
 * Retrieve the acl using the input acl_id as the key.
 * Return true if acl found, false otherwise.
 * Buffer allocated for acl must be freed by caller.
 *
 * Status Codes:
 *      sec_acl_no_acl_found            if acl not found with acl_id.
 *      sec_acl_cant_allocate_memory    if malloc fails.
 *      error_status_ok                 otherwise.
 */
PUBLIC boolean32 rsdb_acl_get_by_id (
    rsdb_pvt_id_t   acl_id,
    rsdb_acl_t      **acl,
    error_status_t  *st
)
{
    rsdb_sequential_key_t   sequential_key;
    Pointer                 data;
    int                     data_len;

    CLEAR_STATUS(st);
    bzero((Pointer) &sequential_key, sizeof(sequential_key));

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = acl_id;

    rsdb_fetch(rsdb_acl_db, (Pointer) &sequential_key, sizeof(sequential_key),
                        &data, &data_len, st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_acl_no_acl_found);
        *acl = (rsdb_acl_t *)NULL;
        return false;
    }
    if (!(*acl = (rsdb_acl_t *)malloc(data_len))) {
        SET_STATUS(st, sec_acl_cant_allocate_memory);
        return false;
    }
    bcopy(data, (Pointer)(*acl), data_len);
    return true;
}


/* r s d b _ a c l _ d e l e t e
 *
 * Delete the acl, identified by acl_id, from the acl database
 * and reset prev and next acl pointers.
 *
 * Status Codes:
 *      sec_acl_no_acl_found            if acl not found with acl_id.
 *      error_status_ok                 otherwise.
 */
PUBLIC void rsdb_acl_delete (
    rsdb_pvt_id_t   acl_id,
    error_status_t  *st
)
{
    rsdb_acl_t          *acl;

    CLEAR_STATUS(st);
    rsdb_acl_get_by_id(acl_id, &acl, st);
    if (BAD_STATUS(st)) {
        return;
    }

    /* Update threads of next and previous acls in list. */
    rsdb_acl_reset_next(acl->threads.prev, acl->threads.next);
    rsdb_acl_reset_prev(acl->threads.next, acl->threads.prev);

    purge_acl_by_id(acl_id, st);
    free(acl);
}


/* r s d b _ a c l _ g e t _ n a m e _ f r o m _ i d s
 *
 * Use the obj_id and domain fields in acl_ids to construct the component_name.
 */
PUBLIC void rsdb_acl_get_name_from_ids (
    rsdb_acl_ids_t              *acl_ids,
    sec_rgy_name_t              name,
    error_status_t              *st
)
{
    rsdb_name_key_t             name_key;
    signed32                    name_key_len = sizeof(name_key);
    unsigned_char_p_t           p;
    signed32                    plen;

    CLEAR_STATUS(st);

    /* First, see if obj_id matches the policy or replist objects */
    if (acl_ids->obj_id == RSDB_POLICY_OBJ_ID) {
        u_strcpy(name, SEC_POLICY_OBJ_NAME);
        return;
    }
    if (acl_ids->obj_id == RSDB_REPLIST_OBJ_ID) {
        u_strcpy(name, SEC_REPLIST_OBJ_NAME);
        return;
    }
    if (acl_ids->obj_id == RSDB_ATTR_SCHEMA_OBJ_ID) {
        u_strcpy(name, SEC_ATTR_SCHEMA_OBJ_NAME);
        return;
    }
    /* If the domain field is not valid, return with error */
    if (!VALID_DOMAIN(acl_ids->domain)) {
        SET_STATUS(st, sec_rgy_bad_domain);
        return;
    }
    /* get the domain name */
    rsdb_util_get_key_of_id(acl_ids->domain, START_OF_LIST,
                            (Pointer)&name_key, &name_key_len);
    u_strncpy(name, name_key.name, name_key.name_len);
    /* construct the rest of the object component name */
    name[name_key.name_len] = '/';
    p = &name[name_key.name_len + 1];
    rsdb_util_path_construct(acl_ids->domain, acl_ids->obj_id,
                             p, &plen, st);
}


/* r s d b _ a c l _ g e t _ i d s _ f r o m _ n a m e
 *
 * Resolve the input name to determine the object it identifies.
 * Fill in the acl_ids structure with the internal info
 * that the rsdb layer uses to identify and operate on the object.
 *
 * The input name should have one of the following forms:
 *      policy
 *      replist
 *      attr_schema
 *      <domain_dir>                i.e., person OR group OR org
 *      <domain_dir>/<pgo>          i.e., person/ahop
 *      <domain_dir>/<dir>/<pgo>    i.e., person/ccd/ahop
 *      etc.
 * The name must not begin with a slash '/'.
 *
 * Return an error if mgr_type is unknown or invalid for the object.
 *
 * If that object is a PGO in the person domain, store the pgo uuid
 * in user_obj and set *group_obj to NULL.
 * If that object is a PGO in the group domain, store the pgo uuid
 * in group_obj and set *user_obj to NULL.
 * In either case, the name pointer within user_obj and group_obj
 * is set to NULL -- only the UUID is required.
 * Otherwise, set both *user_obj and *group_obj to NULL.
 * Note, the caller must have allocated the user_obj and group_obj
 * sec_id_t structures (without malloced space in the name field).
 *
 *  Status Codes:
 *      sec_acl_object_not_found - name didn't identify any object.
 *      sec_acl_unknown_manager_type - input manager type uuid not legal.
 *      sec_acl_invalid_manager_type - input manager type not valid for object.
 *      sec_acl_invalid_acl_type - acl_type invalid for named object.
 *      sec_acl_cant_allocate_memory - malloc failure during operation.
 *      sec_acl_name_resolution_failed - invalid name.
 */
PUBLIC void rsdb_acl_get_ids_from_name (
    sec_acl_component_name_t    name,
    sec_acl_type_t              acl_type,
    uuid_t                      *mgr_type,
    sec_id_t                    **user_obj,
    sec_id_t                    **group_obj,
    rsdb_acl_ids_t              *acl_ids,
    error_status_t              *st
)
{
    unsigned_char_t     	*p;
    rsdb_acl_mgr_type_t 	mtype;
    rsdb_util_rgy_obj_info_t    rgy_obj_info;
    uuid_t			obj_uuid;
    error_status_t      	tmp_st;
    named_item_type_t		obj_type;

    CLEAR_STATUS(st);
    uuid_create_nil(&(acl_ids->user_obj), &tmp_st);
    uuid_create_nil(&(acl_ids->group_obj), &tmp_st);

    p = (unsigned_char_t *)name;

    /* Check that mgr_type is within legal range. */
    if ( !rsdb_acl_get_mgr_type_from_uuid(mgr_type, &mtype, st) ) {
        return;
    }
    /* Check that acl_type and mgr_type are compatible. */
    if (!ACL_MGR_TYPES_MATCH(acl_type, mtype)) {
        SET_STATUS(st, sec_acl_invalid_acl_type);
        return;
    }

    /*
     * Divine the type of object to be fetched from a polymorph --
     * dir requests are either explicit or implicit, in the case of
     * [mgr==<domain> and acl==default-initial-object]
     */
    if ((mtype == rsdb_acl_mgr_type_dir)
	    || (acl_type == sec_acl_type_default_object))
	obj_type = rsdb_dir_type;
    else
	obj_type = rsdb_pgo_type;

    /* Get rgy object ids from the name  and acl_type */
    rsdb_util_get_obj_info_by_name(p, obj_type, acl_type,
					&rgy_obj_info, &obj_uuid, st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_acl_object_not_found);
	return;
    }
    if (rgy_obj_info.acl_id == NULL) {
        SET_STATUS(st, sec_acl_invalid_acl_type);
        return;
    }

    acl_ids->domain = rgy_obj_info.domain;
    acl_ids->obj_id = rgy_obj_info.obj_id;
    acl_ids->acl_id = rgy_obj_info.acl_id;

    /* Handle the special cases of POLICY, REPLIST, ATTR_SCHEMA objects. */
    if ( rgy_obj_info.obj_id == RSDB_POLICY_OBJ_ID	||
	rgy_obj_info.obj_id == RSDB_REPLIST_OBJ_ID	||
	rgy_obj_info.obj_id == RSDB_ATTR_SCHEMA_OBJ_ID ) {
	/* NOTE: if attr_schema becomes a container, these
	 * checks will be incorrect for it.
         */
	if (mtype != rgy_obj_info.acl_mgr_type) {
            SET_STATUS(st, sec_acl_invalid_manager_type);
            return;
	}
        if (acl_type != sec_acl_type_object) {
            SET_STATUS(st, sec_acl_invalid_acl_type);
            return;
        }
	*user_obj = *group_obj = NULL;
        return;
    }

    /* Check that acl_type is valid for node type. */
    if ( (rgy_obj_info.obj_type == rsdb_pgo_type &&
	 !OBJECT_ACL_TYPE(acl_type))		||
        (rgy_obj_info.obj_type == rsdb_dir_type &&
	 !CONTAINER_ACL_TYPE(acl_type)) ) {
        SET_STATUS(st, sec_acl_invalid_acl_type);
        return;
    }

    if ( rgy_obj_info.obj_type == rsdb_pgo_type &&
	acl_ids->domain != sec_rgy_domain_org) {
        if (acl_ids->domain == sec_rgy_domain_person) {
            (*user_obj)->uuid = obj_uuid;
            (*user_obj)->name = NULL;
            *group_obj = NULL;
            acl_ids->user_obj = obj_uuid;
        } else {
            (*group_obj)->uuid = obj_uuid;
            (*group_obj)->name = NULL;
            *user_obj = NULL;
            acl_ids->group_obj = obj_uuid;
        }
    } else {
        *user_obj = *group_obj = NULL;
    }
}


/* r s d b _ a c l _ g e t _ i d _ f r o m _ i t e m
 *
 * If item is a directory object, get acl_id from the item structure.
 * If item is a pgo object, look up the credentials to get acl_id.
 */
PUBLIC boolean32 rsdb_acl_get_id_from_item (
    sec_rgy_domain_t        domain,
    rsdb_named_item_t       *item,
    sec_acl_type_t          acl_type,
    rsdb_acl_mgr_type_t     mgr_type,
    rsdb_pvt_id_t           *acl_id,
    error_status_t          *st
)
{
    rsdb_credentials_t      credentials;

    CLEAR_STATUS(st);

    if (RSDB_IS_PGO(item) && OBJECT_ACL_TYPE(acl_type)
	    && (mgr_type != rsdb_acl_mgr_type_dir))
    {
        if(!rsdb_get_credentials_by_unix_id(domain_info[domain].db,
				RSDB_PGO(item).unix_id, &credentials)) {
            dce_svc_printf(SEC_RSDB_CANT_GET_PGO_CREDS_MSG);
        }
        *acl_id = credentials.acl_id;
        return true;
    } else if (RSDB_IS_DIR(item)) {
        *acl_id = GET_DIR_ACL_ID(item, acl_type);
        return true;
    } else {
        return false;
    }
}


/* r s d b _ a c l _ e x p o r t
 *
 * Copy data from rsdb_acl buffer to corresponding fields in sec_acl.
 * It is assumed that the sec_acl buffer has already been allocated
 * sufficient space for all fixed and variable length data (i.e., that
 * it has been allocated with rsdb_acl_alloc_sec_acl or equivalent).
 * It is assumed that sec_acl->sec_acl_entries has been alloc-ed/initialized.
 *
 * Status Codes:
 *      error_status_ok
 */
PUBLIC void rsdb_acl_export (
    sec_acl_t       *sec_acl,
    rsdb_acl_t      *rsdb_acl,
    error_status_t  *st
)
{
    int                 i;
    unsigned_char_p_t   sec_var_p;
    unsigned_char_p_t   rsdb_var_p;

    CLEAR_STATUS(st);

    sec_acl->default_realm.uuid = rsdb_acl->default_realm.uuid;
    sec_acl->sec_acl_manager_type = rsdb_acl->sec_acl_manager_type;
    sec_acl->num_entries = rsdb_acl->num_entries;

    /* Set rsdb_var_p to point at the beginning of variable length data
     * in rsdb_acl.  As data is copied, rsdb_var_p is updated to point
     * at unread data.
     */
    if (rsdb_acl->num_entries > 0) {
        i = rsdb_acl->num_entries - 1;
    } else {
        i = 0;
    }
    rsdb_var_p = (unsigned_char_p_t)(rsdb_acl->entries[i].name_buf);

    /* Set sec_var_p to point at the beginning of variable length data
     * in sec_acl. As data is copied, sec_var_p is advanced to point
     * at unused buffer space.
     */
    sec_var_p = (unsigned_char_p_t)sec_acl + sizeof(sec_acl_t) +
                (sizeof(sec_acl_entry_t) * sec_acl->num_entries);

    sec_acl->default_realm.name = sec_var_p;
    u_strncpy(sec_var_p, rsdb_var_p, rsdb_acl->default_realm.name_len);
    sec_var_p  += rsdb_acl->default_realm.name_len;
    rsdb_var_p += rsdb_acl->default_realm.name_len;
    *sec_var_p++ = '\0';


    for (i = 0; i < sec_acl->num_entries; i++) {
        sec_acl->sec_acl_entries[i].perms =
            rsdb_acl->entries[i].perms;
        sec_acl->sec_acl_entries[i].entry_info.entry_type =
            rsdb_acl->entries[i].entry_type;
        switch (sec_acl->sec_acl_entries[i].entry_info.entry_type) {
        case sec_acl_e_type_user_obj:
        case sec_acl_e_type_group_obj:
        case sec_acl_e_type_other_obj:
        case sec_acl_e_type_unauthenticated:
        case sec_acl_e_type_any_other:
        case sec_acl_e_type_user_obj_deleg:
        case sec_acl_e_type_group_obj_deleg:
        case sec_acl_e_type_other_obj_deleg:
        case sec_acl_e_type_any_other_deleg:
            /* ... nothing to copy ... */
            break;

        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
            /* Copy id */
            sec_acl->sec_acl_entries[i].entry_info.tagged_union.id.uuid =
                rsdb_acl->entries[i].entry_info.id.uuid;
            sec_acl->sec_acl_entries[i].entry_info.tagged_union.id.name =
                sec_var_p;
            u_strncpy(sec_var_p, rsdb_var_p,
                rsdb_acl->entries[i].entry_info.id.name_len);
            sec_var_p  += rsdb_acl->entries[i].entry_info.id.name_len;
            rsdb_var_p += rsdb_acl->entries[i].entry_info.id.name_len;
            *sec_var_p++ = '\0';
            break;

        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:
            /* Copy foreign_id.id */
            sec_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id.uuid =
                rsdb_acl->entries[i].entry_info.foreign_id.id.uuid;
            sec_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id.name =
                sec_var_p;
            u_strncpy(sec_var_p, rsdb_var_p,
                rsdb_acl->entries[i].entry_info.foreign_id.id.name_len);
            sec_var_p  +=
		rsdb_acl->entries[i].entry_info.foreign_id.id.name_len;
            rsdb_var_p +=
		rsdb_acl->entries[i].entry_info.foreign_id.id.name_len;
            *sec_var_p++ = '\0';

            /* Copy foreign_id.realm */
            sec_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id.realm.uuid =
                rsdb_acl->entries[i].entry_info.foreign_id.realm.uuid;
            sec_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id.realm.name =
                sec_var_p;
            u_strncpy(sec_var_p, rsdb_var_p,
                rsdb_acl->entries[i].entry_info.foreign_id.realm.name_len);
            sec_var_p  +=
		rsdb_acl->entries[i].entry_info.foreign_id.realm.name_len;
            rsdb_var_p +=
		rsdb_acl->entries[i].entry_info.foreign_id.realm.name_len;
            *sec_var_p++ = '\0';
            break;

        case sec_acl_e_type_extended:
        default:
            break;
        } /* end switch */
    }     /* end for loop */
}


/* r s d b _ a c l _ i m p o r t
 *
 * Pack data from sec_acl into rsdb_acl buffer.
 * It is assumed that rsdb_acl contains enough space for all fixed and variable
 * length data (i.e., that it has been allocated with rsdb_acl_alloc_db_acl or
 * equivalent).
 * Don't copy names, only uuids are stored.
 *
 * Status Codes:
 *      error_status_ok.
 */
PUBLIC void rsdb_acl_import (
    rsdb_acl_t      *rsdb_acl,
    sec_acl_t       *sec_acl,
    error_status_t  *st
)
{
    int i;

    CLEAR_STATUS(st);

    rsdb_acl->default_realm.uuid = sec_acl->default_realm.uuid;
    rsdb_acl->sec_acl_manager_type = sec_acl->sec_acl_manager_type;
    rsdb_acl->num_entries = sec_acl->num_entries;

    /* Set rsdb_var_p to point at the beginning of variable length data
     * in rsdb_acl.  As data is copied, rsdb_var_p is advanced to point
     * at unused buffer space.
     */
    if (rsdb_acl->num_entries > 0) {
        i = rsdb_acl->num_entries - 1;
    } else {
        i = 0;
    }

    rsdb_acl->default_realm.name_len = 0;

    for (i = 0; i < sec_acl->num_entries; i++) {
        rsdb_acl->entries[i].perms =
            sec_acl->sec_acl_entries[i].perms;
        rsdb_acl->entries[i].entry_type =
            sec_acl->sec_acl_entries[i].entry_info.entry_type;
        switch (rsdb_acl->entries[i].entry_type) {
        case sec_acl_e_type_user_obj:
        case sec_acl_e_type_group_obj:
        case sec_acl_e_type_other_obj:
        case sec_acl_e_type_unauthenticated:
        case sec_acl_e_type_any_other:
        case sec_acl_e_type_user_obj_deleg:
        case sec_acl_e_type_group_obj_deleg:
        case sec_acl_e_type_other_obj_deleg:
        case sec_acl_e_type_any_other_deleg:
            /* ... nothing to copy ... */
            break;

        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
        case sec_acl_e_type_user_deleg:
        case sec_acl_e_type_group_deleg:
        case sec_acl_e_type_for_other_deleg:
            /* Copy id */
            rsdb_acl->entries[i].entry_info.id.uuid =
                sec_acl->sec_acl_entries[i].entry_info.tagged_union.id.uuid;
            rsdb_acl->entries[i].entry_info.id.name_len = 0;
            break;

        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group:
        case sec_acl_e_type_for_user_deleg:
        case sec_acl_e_type_for_group_deleg:
            /* Copy foreign_id.id */
            rsdb_acl->entries[i].entry_info.foreign_id.id.uuid =
                sec_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id.id.uuid;
            rsdb_acl->entries[i].entry_info.foreign_id.id.name_len = 0;

            /* Copy foreign_id.realm */
            rsdb_acl->entries[i].entry_info.foreign_id.realm.uuid =
                sec_acl->sec_acl_entries[i].entry_info.tagged_union.foreign_id.realm.uuid;
            rsdb_acl->entries[i].entry_info.foreign_id.realm.name_len = 0;
            break;

        case sec_acl_e_type_extended:
        default:
            break;
        } /* end switch */
    }     /* end for loop */
}


/* r s d b _ a c l _ s e m a n t i c _ c h e c k
 *
 *  Check that the input acl is semantically correct for the given mgr_type.
 *  The acl header, each acl entry and each entry's permissions are checked
 *  for validity.
 *
 *  Following is a table plotting the semantic rules.
 *
 *                Valid           Invalid
 * Mgr_type    Permissions      Entry Types
 * --------    -----------      -----------
 * POLICY      rc-----ma----s   e_type_user_obj
 *                              e_type_group_obj
 *
 * PERSON      rc--Dnfmaug---   e_type_group_obj
 *
 * GROUP       rc--Dnfm---M--   e_type_user_obj
 *
 * ORG         rc--Dnfm---M--   e_type_user_obj
 *                              e_type_group_obj
 *
 * DIR         rcidDn--------   e_type_user_obj
 *                              e_type_group_obj
 *
 * REPLIST     -cid---m----AI   e_type_user_obj
 *                              e_type_group_obj
 *
 * Additional Rules:
 *  - An acl must have at least one entry.
 *  - An acl must have at least one entry containing the c-bit.
 *  - An acl must not contain any entries of types not supported
 *    by the Registry (mask_obj, extended).
 *  - An acl must not contain entries with duplicate keys
 *    (check not yet implemented).
 *
 *  Status Codes:
 *      sec_acl_unknown_manager_type - acl's manager type not legal value
 *      sec_acl_invalid_manager_type - acl's manager type didn't match input
 *                                     mgr_type
 *      sec_acl_invalid_entry_type - an acl entry not valid for input
 *                                   mgr_type.
 *      sec_acl_invalid_permission - an acl entry contains a permission bit
 *                                   not supported by this mgr_type.
 *      sec_acl_no_owner - no entry had the owner bit set.
 *      sec_acl_bad_acl_syntax - a syntax error was encountered while parsing
 *                               input acl.
 */
PUBLIC void rsdb_acl_semantic_check (
    sec_acl_t                   *acl,
    sec_acl_type_t              acl_type,
    rsdb_acl_mgr_type_t         mgr_type,
    rsdb_acl_ids_t              *acl_ids,
    error_status_t              *st
)
{
    unsigned32                  i;
    rsdb_acl_mgr_type_t         mgr_type2;
    boolean32                   owner_bit_set = false;
    sec_acl_entry_type_t        e_type;
    sec_acl_entry_type_t        irrelevant_e_type = -1;
    uuid_t                      *implied_obj_uuid;
    sec_acl_permset_t           valid_perms;
    error_status_t              tmp_st;

    CLEAR_STATUS(st);

    /* Check the mgr_type in the acl header. */
    if (!rsdb_acl_get_mgr_type_from_uuid(&(acl->sec_acl_manager_type),
	&mgr_type2, st)) {
        return;
    }
    if (mgr_type2 != mgr_type) {
        SET_STATUS(st, sec_acl_invalid_manager_type);
        return;
    }
    /* Check that the acl_type is consistent with the mgr_type. */
    if (!ACL_MGR_TYPES_MATCH(acl_type, mgr_type)) {
        SET_STATUS(st, sec_acl_invalid_acl_type);
        return;
    }
    if (acl->num_entries < 1) {
        SET_STATUS(st, sec_acl_bad_acl_syntax);
        return;
    }
    switch(mgr_type) {
    case rsdb_acl_mgr_type_rgy:
        valid_perms = rsdb_acl_permset_rgy();
        break;
    case rsdb_acl_mgr_type_person:
        valid_perms = rsdb_acl_permset_person();
        break;
    case rsdb_acl_mgr_type_group:
        valid_perms = rsdb_acl_permset_group();
        break;
    case rsdb_acl_mgr_type_org:
        valid_perms = rsdb_acl_permset_org();
        break;
    case rsdb_acl_mgr_type_dir:
        valid_perms = rsdb_acl_permset_dir();
        break;
    case rsdb_acl_mgr_type_replist:
        valid_perms = rsdb_acl_permset_replist();
        break;
    case rsdb_acl_mgr_type_attr_schema:
        valid_perms = rsdb_acl_permset_attr_schema();
        break;
    }
    /* Determine if there is an irrelevant entry.
     * For example, if this is the object acl on person/root,
     * and there are both a user_obj entry (implied user root)
     * and a user:root entry, the user:root entry is irrelevant.
     * The acl check algorithm matches the user_obj entry and
     * disregards the user:root entry.  The same applies for
     * the similar group_obj case.
     */
    if (acl_type == sec_acl_type_object) {
        if (!(uuid_is_nil(&(acl_ids->user_obj), &tmp_st))) {
            irrelevant_e_type = sec_acl_e_type_user;
            implied_obj_uuid = &(acl_ids->user_obj);
        } else if (!(uuid_is_nil(&(acl_ids->group_obj), &tmp_st))) {
            irrelevant_e_type = sec_acl_e_type_group;
            implied_obj_uuid = &(acl_ids->group_obj);
        }
    }

    /* For each acl entry,
     * - Check that the entry type and permissions are
     *   valid for the mgr_type.
     */
    for (i = 0; i < acl->num_entries; i++) {
        /* Reject bad entry types */
        if (!(VALID_ENTRY_TYPE(&(acl->sec_acl_entries[i])))) {
            SET_STATUS(st, sec_acl_invalid_entry_type);
            return;
        }
        e_type = acl->sec_acl_entries[i].entry_info.entry_type;
        /* If entry type is user_obj or group_obj, check validity. */
        if ((e_type == sec_acl_e_type_user_obj &&
             mgr_type != rsdb_acl_mgr_type_person) ||
            (e_type == sec_acl_e_type_group_obj &&
             mgr_type != rsdb_acl_mgr_type_group) ) {
            SET_STATUS(st, sec_acl_invalid_entry_type);
            return;
        }
        if (acl->sec_acl_entries[i].perms & ~(valid_perms)) {
            SET_STATUS(st, sec_acl_invalid_permission);
            return;
        }
        /* Maintain c-bit information */
        /* Remember if the control bit is set. */
        if (IS_SET(acl->sec_acl_entries[i].perms, sec_acl_perm_owner)) {
            /* ignore unauthenticated and "irrelevant" entries */
            if ( e_type != sec_acl_e_type_unauthenticated ) {
                if ( e_type != irrelevant_e_type ) {
                    owner_bit_set = true;
                } else if ( !(uuid_equal(
                    &(acl->sec_acl_entries[i].entry_info.tagged_union.id.uuid),
                    implied_obj_uuid, &tmp_st)) ) {
                        owner_bit_set = true;
                }
            }
        }
    }
    /* Error if there was not at least one entry with the owner bit set. */
    if ( owner_bit_set ) {
        /* At least one c-bit is effective.  Return with SUCCESS */
        return;
    } else {
        SET_STATUS(st, sec_acl_no_owner);
        return;
    }
}


/*  r s d b _ a c l _ r e p l a c e _ e n t r y
 *
 *  Use input domain, name, acl_type and mgr_type to
 *  identify and retrieve an rsdb_acl.
 *  Modify the old acl (or create a new one if necessary)
 *  to contain the input new_entry and store the acl.
 *  If new_entry contains any key data, it is in name_buf
 *  as a single, null-terminated string for computing ease.
 */
PUBLIC void rsdb_acl_replace_entry (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              name,
    sec_acl_type_t              acl_type,
    uuid_t                      *mgr_type,
    rsdb_acl_entry_t            *new_entry,
    error_status_t              *st
)
{
    boolean32       add_bits = true;

    CLEAR_STATUS(st);

    rsdb_acl_update_entry( domain, name, acl_type, mgr_type,
                           new_entry, add_bits, st );
}


/*  r s d b _ a c l _ u p d a t e _ e n t r y
 *
 *  Use input domain, name, acl_type and mgr_type to
 *  identify and retrieve an rsdb_acl.
 *  Update the specified entry of the old acl to add (add_bits == true)
 *  or delete (add_bits = false) the permission bits on the input entry.
 *  Create a new entry if an entry with the same key doesn't exist.
 *  Store the acl.
 *  If new_entry contains any key data, it is in name_buf
 *  as a single, null-terminated string for computing ease.
 */
PUBLIC void rsdb_acl_update_entry (
    sec_rgy_domain_t            domain,
    sec_rgy_name_t              name,
    sec_acl_type_t              acl_type,
    uuid_t                      *mgr_type,
    rsdb_acl_entry_t            *new_entry,
    boolean32                   add_bits,
    error_status_t              *st
)
{
    sec_rgy_name_t              full_name;
    sec_id_t                    user_obj, group_obj, *user_p, *group_p;
    rsdb_acl_ids_t              ids;
    rsdb_acl_t                  *old_acl, *new_acl;
    unsigned32                  new_acl_len;
    rsdb_acl_entry_t            *old_entry;
    int                         i;

    CLEAR_STATUS(st);

    if (!rsdb_name_util_complete_name(domain, name, full_name, st)) {
        SET_STATUS(st, sec_acl_name_resolution_failed);
        return;
    }

    user_p = &user_obj; group_p = &group_obj;
    rsdb_acl_get_ids_from_name(full_name, acl_type, mgr_type,
                               &user_p, &group_p, &ids, st);
    if (BAD_STATUS(st)) {
        return;
    }
    rsdb_acl_get_by_id(ids.acl_id, &old_acl, st);
    if (BAD_STATUS(st)) {
        return;
    }

    for ( i = 0; i < old_acl->num_entries; i++ ) {
        old_entry = &(old_acl->entries[i]);
        if ( rsdb_acl_match_entries(new_entry, old_entry)) {
            /* New entry matches old entry. Replace old entry.
             * Since entry keys match, no need to change name_buf.
             * No dynamic length data (like names) are stored
             */
            if (add_bits) {
                old_entry->perms |= new_entry->perms;
            } else {
                old_entry->perms &= ~(new_entry->perms);
            }
            rsdb_acl_store_by_id(ids.acl_id, old_acl);
            free(old_acl);
            return;
        }
    }
    /* We are here if new_entry did not match any entries in acl.
     * Create a new acl containing all old entries and new_entry.
     */
    new_acl_len = old_acl->acl_len + sizeof(rsdb_acl_entry_t);
    if (!(new_acl = (rsdb_acl_t *)malloc(new_acl_len))) {
        SET_STATUS(st, sec_acl_cant_allocate_memory);
        free(old_acl);
        return;
    }
    bcopy((char *)old_acl, (char *)new_acl, sizeof(rsdb_acl_fixed_t));
    new_acl->num_entries += 1;
    new_acl->acl_len = new_acl_len;
    for ( i = 0; i < old_acl->num_entries; i++ ) {
        new_acl->entries[i] = old_acl->entries[i];
    }
    new_acl->entries[i] = *new_entry;
    rsdb_acl_store_by_id(ids.acl_id, new_acl);
    free(old_acl);
    free(new_acl);
}


/*  r s d b _ a c l _ m a t c h _ e n t r i e s
 *
 *  Return true if the two entries have the same entry_type
 *  and, where applicable, have identical entry keys.
 *  (Compare uuids, not names, to compare keys.)
 */
PUBLIC boolean32 rsdb_acl_match_entries (
    rsdb_acl_entry_t            *entry1,
    rsdb_acl_entry_t            *entry2
)
{
    unsigned32			stp;

    if ( entry1->entry_type != entry2->entry_type )
        return false;

    switch( entry2->entry_type ) {
    case sec_acl_e_type_user_obj:
    case sec_acl_e_type_group_obj:
    case sec_acl_e_type_other_obj:
    case sec_acl_e_type_unauthenticated:
    case sec_acl_e_type_any_other:
    case sec_acl_e_type_user_obj_deleg:
    case sec_acl_e_type_group_obj_deleg:
    case sec_acl_e_type_other_obj_deleg:
    case sec_acl_e_type_any_other_deleg:
        /* ... no key data to compare ... */
        return true;

    case sec_acl_e_type_user:
    case sec_acl_e_type_group:
    case sec_acl_e_type_foreign_other:
    case sec_acl_e_type_user_deleg:
    case sec_acl_e_type_group_deleg:
    case sec_acl_e_type_for_other_deleg:
        /* Compare id */
	if ( !uuid_equal(&(entry1->entry_info.id.uuid),
                         &(entry2->entry_info.id.uuid), &stp) )
            return false;
        return true;

    case sec_acl_e_type_foreign_user:
    case sec_acl_e_type_foreign_group:
    case sec_acl_e_type_for_user_deleg:
    case sec_acl_e_type_for_group_deleg:
        /* Compare foreign_id */
        if (!uuid_equal(&(entry1->entry_info.foreign_id.id.uuid),
                        &(entry2->entry_info.foreign_id.id.uuid), &stp) )
            return false;
        else if (!uuid_equal(&(entry1->entry_info.foreign_id.realm.uuid),
                             &(entry2->entry_info.foreign_id.realm.uuid),&stp))
            return false;
        return true;

    case sec_acl_e_type_extended:
    default:
        return true;
    } /* end switch */
}


/* rsdb_acl_mgr_perms_valid
 *
 * Added for Extended Registry Support, this function will verify
 * that the acl manager type is a valid registry object acl_mgr_type.
 * If not status is set to sec_acl_invalid_manager_type.
 * If the permisions are not valid for this mgr_type, then the status is
 * set to sec_acl_bad_permset.
 * Errors:
 * 	sec_acl_unknown_manager_type
 *	sec_acl_invalid_manager_type
 *	sec_acl_bad_permset
 */
PUBLIC boolean32 rsdb_acl_mgr_perms_valid (
    uuid_t			*acl_mgr_type,
    sec_acl_permset_t		permset,
    error_status_t 		*st_p
)
{
    sec_acl_permset_t	valid_perms;
    rsdb_acl_mgr_type_t	mgr_type;

    CLEAR_STATUS(st_p);

    if (!rsdb_acl_get_mgr_type_from_uuid(acl_mgr_type, &mgr_type, st_p)) {
	return false;
    }

    if (!OBJ_MGR_TYPE(mgr_type)) {
	SET_STATUS(st_p, sec_acl_invalid_manager_type);
	return false;
    }

    switch(mgr_type) {
    case rsdb_acl_mgr_type_rgy:
        valid_perms = rsdb_acl_permset_rgy();
        break;
    case rsdb_acl_mgr_type_person:
        valid_perms = rsdb_acl_permset_person();
        break;
    case rsdb_acl_mgr_type_group:
        valid_perms = rsdb_acl_permset_group();
        break;
    case rsdb_acl_mgr_type_org:
        valid_perms = rsdb_acl_permset_org();
        break;
    case rsdb_acl_mgr_type_dir:
        valid_perms = rsdb_acl_permset_dir();
        break;
    case rsdb_acl_mgr_type_replist:
        valid_perms = rsdb_acl_permset_replist();
        break;
    case rsdb_acl_mgr_type_attr_schema:
        valid_perms = rsdb_acl_permset_attr_schema();
        break;
    }

    if ( permset & ~(valid_perms)) {
	SET_STATUS(st_p, sec_acl_bad_permset);
	return false;
    }
    return true;
}
