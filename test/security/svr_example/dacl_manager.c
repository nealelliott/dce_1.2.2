/*
 * @OSF_COPYRIGHT@
 * 
 */
/*
 * HISTORY
 * $Log: dacl_manager.c,v $
 * Revision 1.1.6.3  1996/02/17  23:26:23  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:14  marty]
 *
 * Revision 1.1.6.2  1995/12/11  21:52:30  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/02  22:48 UTC  burati
 * 	Merge fix for CHFts14249
 * 
 * 	HP revision /main/burati_chfts14249/1  1995/03/01  19:37 UTC  burati
 * 	Fix for CHFts14249 (Delegation ACL check changes)
 * 	[1995/12/11  20:54:10  root]
 * 
 * Revision 1.1.2.2  1994/09/21  20:24:04  max
 * 	CR 11981 fix. When checking delegate access avoid user_obj_delagate
 * 	check if user_obj matched the uuid.  This is achieved by checking
 * 	access flag.
 * 	[1994/09/21  20:21:41  max]
 * 
 * Revision 1.1.2.1  1994/08/02  20:11:34  bhaim
 * 	move acl_server to test area
 * 	[1994/08/02  20:11:04  bhaim]
 * 
 * $EndLog$
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * The Example code is provided to you as "Pass-through Code".
 * Pass-through Code and its related Documentation are not warranteed,
 * supported, or indemnified by OSF, and are provided "AS IS".
 * You can use, sublicense, or disribute the same at your own risk.
 */

/* dacl_manager.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1993, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      sec_acl test server - wire and mgr implementation
 * 
 *
 * SEC_ACL dummy server interface routines, for testing/debugging purposes
 * only.  This was not intended to be an efficient use of ACL storage.
 * It's sole purpose it to test the client agent and clients.
 *
 * CONTENTS:
 *          Implementation of mgr interface  (daclmgr.idl) at top
 *          Implementation of wire interface (rdaclif.idl) at bottom
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)dacl_manager.c	20 - 01/02/92";
#endif

#include <dce/uuid.h>
#include <dce/sec_cred.h>
#include <rdaclif.h>
#include <rdaclifv0.h>
#include <daclmgr.h>

#include <stdio.h>
#include <string.h>
#include <macros.h>
#include <un_io.h>
#include <un_maloc.h>

/* Defined in dacl_server.c */
extern boolean32 debug_acl_svr;

void rdaclv0_lookup(
    handle_t h,
    sec_acl_component_name_t component_name,
    uuid_t *manager_type,
    sec_acl_type_t sec_acl_type,
    sec_acl_result_t *result
);
globaldef rdaclif_v0_0_epv_t rdaclif_v0_0_manager_epv = {
    rdaclv0_lookup,
    rdacl_replace,
    rdacl_get_access,
    rdacl_test_access,
    rdacl_test_access_on_behalf,
    rdacl_get_manager_types,
    rdacl_get_printstring,
    rdacl_get_referral,
    rdacl_get_mgr_types_semantics
};
globaldef rdaclif_v1_0_epv_t rdaclif_v1_0_manager_epv = {
    rdacl_lookup,
    rdacl_replace,
    rdacl_get_access,
    rdacl_test_access,
    rdacl_test_access_on_behalf,
    rdacl_get_manager_types,
    rdacl_get_printstring,
    rdacl_get_referral,
    rdacl_get_mgr_types_semantics
};

/* Private definitions, mostly for efficiency of auth checks */
#define ENTRY_NOT_FOUND -1
#define MAX_PERMISSIONS ~0

/*
 *     Internal structure for test database.  The actual internal
 * storage of sec_acl's will be determined by the individual ACL
 * manager implementors, for each manager (ie: rgy, filesystem...)
 * For testing purposes, we will use a static array of objects
 * and assume that they have no initial acls (ie: you must do a
 * replace before a lookup will return successfully), because the
 * acl editor will have to handle this case, as well as the case
 * where objects always have acls.
 */
#define OBJECT_ACL        0
#define DEFAULT_OBJECT    1
#define DEFAULT_CONTAINER 2
typedef struct {
    sec_acl_component_name_t    component_name;
    boolean32                   has_obj_acl, has_if_acl, has_id_acl;
    sec_acl_t                   *sec_obj_acl, *sec_if_acl, *sec_id_acl;
} my_object_t;

my_object_t test_objects[] = {
 {(sec_acl_component_name_t)"mike",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"joe",   false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"greg",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"anne",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"bill",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"liza",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"jim",   false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"terry", false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"tom",   false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"dale",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"nat",   false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"scott", false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"mark",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"paul",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)"rich",  false,  false, false, NULL, NULL, NULL },
 {(sec_acl_component_name_t)NULL,    false,  false, false, NULL, NULL, NULL }
};

PRIVATE sec_acl_mgr_handle_t sec_acl_mgr = NULL;

/*
 * INTERNAL TEST SERVER ROUTINES
 */


/* For testing purposes, we are using a single, global manager_type uuid */
PUBLIC uuid_t  mgr_uuid;

/* For testing purposes, convert the known uuid string to the manager_type */
PUBLIC void rdacl__get_manager_type( void )
{
    error_status_t  st;

    uuid_from_string((unsigned_char_p_t)"da1a8e10-78d8-11c9-ba32-08001e029155",
                        &mgr_uuid, &st);

    /* if we can't get a manager type, nothing else would work anyway so...*/
    if (! GOOD_STATUS(&st)) {
        fprintf(stderr, "INTERNAL ERROR: Could not get manager uuid %x\n",st);
        exit(1);
    }
}

PRIVATE void rdacl__validate_params (
    sec_acl_key_t   name,
    int             *index_p,
    error_status_t  *st_p
)
{
    error_status_t  st;
    boolean32       found;
    int             i;

    SET_STATUS(st_p, error_status_ok);

    /* Attempt to find the component_name */
    found = false;
    for (i = 0; (test_objects[i].component_name != NULL) &&
                (! found) ; i++)
        if (strcmp( (char *)test_objects[i].component_name,
                    (char *)name) == 0) {
            found = true;
            *index_p = i;
    }
    if (! found)
        SET_STATUS(st_p, sec_acl_object_not_found);
}

PRIVATE void rdacl__free_dacl (
    sec_acl_t *sec_acl_p
)
{
    sec_acl_entry_t *entry_p;
    int             i;

    free((char *) sec_acl_p->default_realm.name);
    for (i = 0; i < sec_acl_p->num_entries; i++) {
        entry_p = &(sec_acl_p->sec_acl_entries[i]);
        switch (entry_p->entry_info.entry_type)  {
            case sec_acl_e_type_user:
            case sec_acl_e_type_group:
            case sec_acl_e_type_foreign_other:
            case sec_acl_e_type_user_deleg:
            case sec_acl_e_type_group_deleg:
            case sec_acl_e_type_for_other_deleg:
                free((char *)
                    entry_p->entry_info.tagged_union.id.name);
                break;
            case sec_acl_e_type_foreign_user:
            case sec_acl_e_type_foreign_group:
            case sec_acl_e_type_for_user_deleg:
            case sec_acl_e_type_for_group_deleg:
                free((char *)
                    entry_p->entry_info.tagged_union.foreign_id.id.name);
                break;
            case sec_acl_e_type_extended:
            default:
                break;
        }
    }
}


/* SEC_ACL Manager routines (test implementation)
 * 
 * Would normally be implemented separately in a sec_acl_mgr, but since
 * this is a test/semi-reference implementation, it's all here...
 */

/* s e c _ a c l _ m g r _ c o n f i g u r e
 *
 *  Configure the dacl manager.  This operation provides a handle that
 *  refers to the particular acl database in use and is necessary for all
 *  other sec_acl manager operations.  The db_name identifies which acl
 *  database to use (and generally refers to the file system object that
 *  represents the persistent storage for the database).  The config_info
 *  provides information on how to configure this database.
 */
PUBLIC void sec_acl_mgr_configure (
    sec_acl_mgr_config_t    config_info,
    unsigned_char_p_t       db_name,
    sec_acl_mgr_handle_t    *sec_acl_mgr,
    error_status_t          *st_p
)
{
    int i;
    CLEAR_STATUS(st_p);

    /* Don't bother with a real handle, since the test server only
        implements one type of acl storage (volatile) */
    *sec_acl_mgr = NULL;

}

/* access_granted
 *
 * Private routine to mask entry permset against mask_obj if necessary
 * and compare the result of that operation with the desired_access
 * permset, to determine if access should be granted
 */

PRIVATE boolean32 access_granted (
    sec_acl_entry_t     entry,
    sec_acl_permset_t   mask_perms,
    sec_acl_permset_t   desired,
    sec_acl_permset_t   *granted
)
{
    boolean32 grant_access = false;

    switch (entry.entry_info.entry_type) {
        case sec_acl_e_type_user_obj:
        case sec_acl_e_type_user_obj_deleg:
        case sec_acl_e_type_other_obj:
        case sec_acl_e_type_other_obj_deleg:
        case sec_acl_e_type_mask_obj:
        case sec_acl_e_type_unauthenticated:
            *granted |= (entry.perms & desired);
            break;
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
            *granted |= ((entry.perms & mask_perms) & desired);
            break;
        default:
	    break;
    }
    if ( *granted == desired )
        grant_access = true;

    return grant_access;
}


/* s e c _ a c l _ m g r _ i s _ a u t h o r i z e d
 *
 *  The basic operation in the authorization package, this function will
 *  yield true if the principal (as described in the privilege attribute
 *  certificate referred to by "accessor_info") is authorized to perform
 *  the requested operation.  The dacl controlling this decision is 
 *  not passed directly to this function, but is referred to via the 
 *  sec_acl_key and the sec_acl_type parameters (Generally objects will
 *  only be protected by a dacl_type_object dacl, but specifying the
 *  sec_acl_type on the call will allow for future enhancement)
 */

/* Compare default realm with accessor's realm */
#define default_realm_eq(entry) uuid_equal(&paP->realm.uuid, \
                                        &sec_acl_p->default_realm.uuid, \
                                        &st)

/* Compare foreign realm with accessor's realm */
#define foreign_realm_eq(entry) uuid_equal(&paP->realm.uuid, \
  &sec_acl_p->sec_acl_entries[entry].entry_info.tagged_union.foreign_id.realm.uuid, \
  &st)

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

PRIVATE sec__acl_mgr_match_entries (
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
            case sec_acl_e_type_mask_obj:
                entry_pos[MASK_POS] = i;
                *mask_perms_p = sec_acl_p->sec_acl_entries[i].perms;
                break;
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
PRIVATE boolean32 sec__acl_mgr_access_check (
    sec_acl_t         *sec_acl_p,
    sec_acl_permset_t mask_perms,
    sec_id_pa_t       *paP,
    sec_id_t          *user_obj,
    sec_id_t          *group_obj,
    sec_acl_permset_t desired_access,
    boolean32         auth,
    error_status_t    *st_p
)
{
    error_status_t      st;
    sec_acl_permset_t   granted = (sec_acl_permset_t) 0, group_access;
    int                 i;              /* For traversing entry list. */
    unsigned int        j;              /* For traversing entry list. */
    sec_id_t            *group_id, *realm_id;
    boolean32           chk_loc_groups = false, one_group_found = false;
    boolean32           access = false;
    
    if (debug_acl_svr) {
        unsigned_char_p_t uid_str;
        uuid_to_string(&paP->principal.uuid, &uid_str, &st);
        printf("DEBUG: Initiator Access Check: %s\n", uid_str);
    }
    
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
            if (access_granted(
                    sec_acl_p->sec_acl_entries[entry_pos[USER_OBJ_ENTRY]],
                    mask_perms, desired_access, &granted))
                access = true;
            else
                return false;       /* implied denial rights */
        }
    }

    /* USER check */
    if ((! access) && (entry_pos[USER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(sec_acl_p->sec_acl_entries[entry_pos[USER_ENTRY]],
                            mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }

    /* FOREIGN_USER check */
    if ((! access) && (entry_pos[FOREIGN_USER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_USER_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }

    /* GROUP checks */
    group_access = (sec_acl_permset_t) 0;
    for (i = 0;((! access) && (i < sec_acl_p->num_entries)); i++) {
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
                        if (access_granted(sec_acl_p->sec_acl_entries[i],
                                            mask_perms, desired_access,
                                            &granted))
                            access = true;
                        else
                            group_access = (group_access | granted);
                    }
                    /* CHECK LOCAL GROUPS */
                    for (j = 0;
                         ((! access) && (j < paP->num_groups));
                         j++) {

                        if (uuid_equal(&paP->groups[j].uuid,
                                        &group_id->uuid, &st) ) {

                            one_group_found = true;
                            /* check the perms to see if access granted */
                            if (access_granted(sec_acl_p->sec_acl_entries[i],
                                                mask_perms, desired_access,
                                                &granted))
                                access = true;
                            else
                                group_access = (group_access | granted);
                        }
                    }
                } else {
                    /* CHECK FOREIGN GROUPS */
		    int k;
		    /* For each foreign groupset that the caller belongs to */
                    for (j = 0;
                         (! access) && (j < paP->num_foreign_groupsets);
                         j++) {

                      /* If the entry's realm matches this groupset's realm */
                      if (uuid_equal(&paP->foreign_groupsets[j].realm.uuid,
                                      &group_id->uuid, &st))

                        /* Then check each group within that foreign realm */
                        for (k = 0; ((!access) &&
                             (k < paP->foreign_groupsets[j].num_groups));k++) {
                            if (uuid_equal(
                        	&paP->foreign_groupsets[j].realm.uuid,
                                &realm_id->uuid, &st)) {

                                one_group_found = true;
                                /* check the perms to see if access granted */
                                if (access_granted(
                                        sec_acl_p->sec_acl_entries[i],
                                        mask_perms, desired_access, &granted))
                                    access = true;
                                else
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
        if ((group_access & desired_access) == desired_access)
            access = true;
    } /* GROUP check */

    /* If at least 1 group found and !access, then deny any access */
    if (!access && one_group_found)
        return false;

    /* OTHER_OBJ check */
    if ((! access) && (entry_pos[OTHER_OBJ_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[OTHER_OBJ_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }

    /* FOREIGN_OTHER check */
    if ((! access) && (entry_pos[FOREIGN_OTHER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_OTHER_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }

    /* ANY_OTHER check */
    if ((! access) && (entry_pos[ANY_OTHER_ENTRY] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[ANY_OTHER_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }

    /* UNAUTHENTICATED check
     * If pa_data isn't authenticated and access was granted by one of the
     * above checks, then desired_access must be masked by the unauthenticated
     * entry.
     */
    if ((access) && (! auth)) {
        if (entry_pos[UNAUTH_ENTRY] == ENTRY_NOT_FOUND)
            access = false;
        else if (! access_granted(
                    sec_acl_p->sec_acl_entries[entry_pos[UNAUTH_ENTRY]],
                    mask_perms, desired_access, &granted))
            access = false;
    }
    return access;
}


/*
 * Internal routine to perform actual access checking against deleg entries.
 */
PRIVATE boolean32 sec__acl_mgr_deleg_access_check(
    sec_acl_t         *sec_acl_p,
    sec_acl_permset_t mask_perms,
    sec_id_pa_t       *paP,
    sec_id_t          *user_obj,
    sec_id_t          *group_obj,
    sec_acl_permset_t desired_access,
    boolean32         auth,
    error_status_t    *st_p
)
{
    error_status_t      st;
    sec_acl_permset_t   granted = (sec_acl_permset_t) 0, group_access;
    int                 i;              /* For traversing entry list. */
    unsigned int        j;              /* For traversing entry list. */
    sec_id_t            *group_id, *realm_id;
    boolean32           chk_loc_groups = false, one_group_found = false;
    boolean32           access = false;

    if (debug_acl_svr) {
        unsigned_char_p_t uid_str;
        uuid_to_string(&paP->principal.uuid, &uid_str, &st);
        printf("DEBUG: Delegate Access Check: %s\n", uid_str);
    }

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
            if (access_granted(
                    sec_acl_p->sec_acl_entries[entry_pos[USER_OBJ_ENTRY]],
                    mask_perms, desired_access, &granted))
                access = true;
            else if (entry_pos[USER_OBJ_DELEG] == ENTRY_NOT_FOUND)
                return false;       /* implied denial rights */
        }
    }

    /* USER_OBJ_DELEGATE check */
    if ((! access) && (entry_pos[USER_OBJ_DELEG] != ENTRY_NOT_FOUND)) {

        /* If e_type_user_obj_deleg entry exists then user_obj can't be NULL */
        if (user_obj == NULL) {
            SET_STATUS(st_p, sec_acl_expected_user_obj);
            return false;
        }

        /* If the id assoc w/ user_obj matches the principal id */
        if (uuid_equal(&paP->principal.uuid,
                        &user_obj->uuid,
                        &st)) {
            /* then check the permsets to see if access is granted */
            if (access_granted(
                    sec_acl_p->sec_acl_entries[entry_pos[USER_OBJ_DELEG]],
                    mask_perms, desired_access, &granted))
                access = true;
            else
                return false;       /* implied denial rights */
        }
    }
    /* Not pretty, but best performance without rewriting entire algorithm */
    if (access) goto DlgGotAccess;

    /* USER check */
    if (entry_pos[USER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (access_granted(sec_acl_p->sec_acl_entries[entry_pos[USER_ENTRY]],
                            mask_perms, desired_access, &granted))
            access = true;
        else if (entry_pos[USER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }
    /* USER_DELEG check */
    if ((! access) && (entry_pos[USER_DELEG] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(sec_acl_p->sec_acl_entries[entry_pos[USER_DELEG]],
                            mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }
    if (access) goto DlgGotAccess;

    /* FOREIGN_USER check */
    if (entry_pos[FOREIGN_USER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_USER_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else if (entry_pos[F_USER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }
    /* FOREIGN_USER_DELEG check */
    if ((! access) && (entry_pos[F_USER_DELEG] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[F_USER_DELEG]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }
    if (access) goto DlgGotAccess;

    /* GROUP checks */
    group_access = (sec_acl_permset_t) 0;
    for (i = 0;((! access) && (i < sec_acl_p->num_entries)); i++) {
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
                        if (access_granted(sec_acl_p->sec_acl_entries[i],
                                            mask_perms, desired_access,
                                            &granted))
                            access = true;
                        else
                            group_access = (group_access | granted);
                    }
                    /* CHECK LOCAL GROUPS */
                    for (j = 0;
                         ((! access) && (j < paP->num_groups));
                         j++) {

                        if (uuid_equal(&paP->groups[j].uuid,
                                        &group_id->uuid, &st) ) {

                            one_group_found = true;
                            /* check the perms to see if access granted */
                            if (access_granted(sec_acl_p->sec_acl_entries[i],
                                                mask_perms, desired_access,
                                                &granted))
                                access = true;
                            else
                                group_access = (group_access | granted);
                        }
                    }
                } else {
                    /* CHECK FOREIGN GROUPS */
		    int k;
		    /* For each foreign groupset that the caller belongs to */
                    for (j = 0;
                         (! access) && (j < paP->num_foreign_groupsets);
                         j++) {

                      /* If the entry's realm matches this groupset's realm */
                      if (uuid_equal(&paP->foreign_groupsets[j].realm.uuid,
                                      &group_id->uuid, &st))

                        /* Then check each group within that foreign realm */
                        for (k = 0; ((!access) &&
                             (k < paP->foreign_groupsets[j].num_groups));k++) {
                            if (uuid_equal(
                        	&paP->foreign_groupsets[j].realm.uuid,
                                &realm_id->uuid, &st)) {

                                one_group_found = true;
                                /* check the perms to see if access granted */
                                if (access_granted(
                                        sec_acl_p->sec_acl_entries[i],
                                        mask_perms, desired_access, &granted))
                                    access = true;
                                else
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
        if ((group_access & desired_access) == desired_access)
            access = true;
    } /* GROUP check */

    /* If at least 1 group found and !access, then deny any access */
    if (!access && one_group_found)
        return false;
    if (access) goto DlgGotAccess;

    /* OTHER_OBJ check */
    if (entry_pos[OTHER_OBJ_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[OTHER_OBJ_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else if (entry_pos[OTHER_OBJ_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }
    /* OTHER_OBJ_DELEG check */
    if ((! access) && (entry_pos[OTHER_OBJ_DELEG] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[OTHER_OBJ_DELEG]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }
    if (access) goto DlgGotAccess;

    /* FOREIGN_OTHER check */
    if (entry_pos[FOREIGN_OTHER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[FOREIGN_OTHER_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else if (entry_pos[F_OTHER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }
    /* FOREIGN_OTHER_DELEG check */
    if ((! access) && (entry_pos[F_OTHER_DELEG] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[F_OTHER_DELEG]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }
    if (access) goto DlgGotAccess;

    /* ANY_OTHER check */
    if (entry_pos[ANY_OTHER_ENTRY] != ENTRY_NOT_FOUND) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[ANY_OTHER_ENTRY]],
                mask_perms, desired_access, &granted))
            access = true;
        else if (entry_pos[ANY_OTHER_DELEG] == ENTRY_NOT_FOUND)
            return false;           /* implied denial rights */
    }
    /* ANY_OTHER_DELEG check */
    if ((! access) && (entry_pos[ANY_OTHER_DELEG] != ENTRY_NOT_FOUND)) {
        /* check the permsets to see if access is granted */
        if (access_granted(
                sec_acl_p->sec_acl_entries[entry_pos[ANY_OTHER_DELEG]],
                mask_perms, desired_access, &granted))
            access = true;
        else
            return false;           /* implied denial rights */
    }

DlgGotAccess:
	
    /* UNAUTHENTICATED check
     * If pa_data isn't authenticated and access was granted by one of the
     * above checks, then desired_access must be masked by the unauthenticated
     * entry.
     */
    if ((access) && (! auth)) {
        if (entry_pos[UNAUTH_ENTRY] == ENTRY_NOT_FOUND)
            access = false;
        else if (! access_granted(
                    sec_acl_p->sec_acl_entries[entry_pos[UNAUTH_ENTRY]],
                    mask_perms, desired_access, &granted))
            access = false;
    }
    return access;
}


PUBLIC boolean32 sec_acl_mgr_is_authorized (
    sec_acl_mgr_handle_t    sec_acl_mgr,
    sec_acl_permset_t       desired_access,
    rpc_authz_cred_handle_t *accessor_info,
    sec_acl_key_t           sec_acl_key,
    uuid_t                  *manager_type_p,
    sec_id_t                *user_obj,     
    sec_id_t                *group_obj,    
    error_status_t          *st_p
)
{
    sec_acl_list_t      *sec_acl_list;
    sec_acl_t           *sec_acl_p;
    sec_id_pac_t        *pacP;
    sec_id_pa_t         *paP;
    error_status_t      st;
    sec_acl_permset_t   mask_perms;
    boolean32           auth, access = false; /* Keep running tab on access */
    sec_cred_pa_handle_t pa_h;
    sec_cred_cursor_t    cursor;
    int                 entries[sec_acl_e_type_any_other_deleg];

    auth = sec_cred_is_authenticated(*accessor_info, st_p);
    if (BAD_STATUS(st_p))
        return false;
    if (debug_acl_svr)
        fprintf(stderr, "DEBUG: sec_cred_is_authorized returned %s\n",
                ((auth) ? "true" : "false"));

    pa_h = sec_cred_get_initiator(*accessor_info, st_p);
    if (BAD_STATUS(st_p))
        return false;
    paP = sec_cred_get_pa_data(pa_h, st_p);
    if (BAD_STATUS(st_p))
        return false;
        
    /* Retrieve the sec_acl for this key */
    sec_acl_mgr_lookup(sec_acl_mgr, sec_acl_key, manager_type_p,
                        sec_acl_type_object, &sec_acl_list, st_p);
    if (BAD_STATUS(st_p))
        return false;
    else
        sec_acl_p = sec_acl_list->sec_acls[0];

    /* Find entries matching the initiator's privilege attributes */
    sec__acl_mgr_match_entries(false, &mask_perms, sec_acl_p, paP);

    /* Check Initiator First
     * Now that we know which entries match the user described in the PAs,
     * check the permissions corresponding to each entry until access is
     * granted by one of them.
     */
    access = sec__acl_mgr_access_check(sec_acl_p, mask_perms, paP,
        user_obj, group_obj, desired_access, auth, st_p);

    /* If the initiator doesn't have access, then don't bother
     * checking delegates.
     */
    if (! access) return false;

    /* Check Delegates */
    sec_cred_initialize_cursor(&cursor, st_p);
    if (BAD_STATUS(st_p))
	return false;
    do {
        /* Access must be granted to each delegate, so assume no
         * access yet each time through the loop
         */
        pa_h = sec_cred_get_delegate(*accessor_info, &cursor, st_p);
        if (GOOD_STATUS(st_p)) {
            access = false;
            paP = sec_cred_get_pa_data(pa_h, st_p);
            if (GOOD_STATUS(st_p)) {
                /* Find the ACL entries that match this delegate */
                sec__acl_mgr_match_entries(true, &mask_perms, sec_acl_p, paP);

                /* Check the delegate's access */
                access = sec__acl_mgr_deleg_access_check(sec_acl_p, mask_perms,
                    paP, user_obj, group_obj, desired_access, auth, st_p);
            }
        }
    } while (GOOD_STATUS(st_p) && access);

    /* Any error code other than end of delegate list is fatal */
    if (STATUS_EQUAL(st_p, sec_cred_s_no_more_entries))
	CLEAR_STATUS(st_p);
    else if (BAD_STATUS(st_p))
	return false;

    return access;
}


/* s e c _ a c l _ m g r _ r e p l a c e
 *
 *  Replace the dacl associated with the key.  This package treats dacls as
 *  immutable objects - the old dacl is thrown away and the new one created
 *  Some implementations of this interface may choose to optimize storage
 *  and share dacl representation for many objects - in which case the real
 *  dacl storage must be reference counted, but that is of no concern to
 *  the consumer of the interface.
 *
 *  NOTE: This implementation of sec_acl_mgr_replace implements USER_OBJ,
 *  GROUP_OBJ and EXTENDED types *only* to provide a complete reference.
 *  These types are not used by this sec_acl manager.  A real sec_acl_mgr
 *  implementation should return the error sec_acl_invalid_entry_type,
 *  for any one of these types passed in, that is not supported by the mgr.
 *  (ie: if there's no user or group owner stored with an object, then
 *   it does not make sense to support user_obj and group_obj types)
 */
PUBLIC void sec_acl_mgr_replace (
    sec_acl_mgr_handle_t    sec_acl_mgr,
    sec_acl_key_t           sec_acl_key,
    uuid_t                  *manager_type_p,
    sec_acl_type_t          sec_acl_type,
    sec_acl_list_t          *sec_acl_list_p,
    error_status_t          *st_p
)
{
    int             index, i, j, num_bytes;
    sec_acl_t       *nacl_p, *sec_acl_p;
    sec_acl_entry_t *new_e, *my_e;

    rdacl__validate_params(sec_acl_key, &index, st_p);
    if (GOOD_STATUS(st_p)) {

        /* We know that there's only one acl in the list */
        sec_acl_p = sec_acl_list_p->sec_acls[0];

        /* If replacing an existing acl, remove the old one.
         * Make space for the new acl and set has_acl to true.
         */
        switch (sec_acl_type) {
            case sec_acl_type_object:
                if (! test_objects[index].has_obj_acl)
                    test_objects[index].has_obj_acl = true;
                else
                    rdacl__free_dacl(test_objects[index].sec_obj_acl);
                nacl_p = test_objects[index].sec_obj_acl = 
                            (sec_acl_t *) malloc(sizeof(sec_acl_t));
                break;
            case sec_acl_type_default_object:
                if (! test_objects[index].has_if_acl)
                    test_objects[index].has_if_acl = true;
                else
                    rdacl__free_dacl(test_objects[index].sec_if_acl);
                nacl_p = test_objects[index].sec_if_acl = 
                            (sec_acl_t *) malloc(sizeof(sec_acl_t));
                break;
            case sec_acl_type_default_container:
                if (! test_objects[index].has_id_acl)
                    test_objects[index].has_id_acl = true;
                else
                    rdacl__free_dacl(test_objects[index].sec_id_acl);
                nacl_p = test_objects[index].sec_id_acl = 
                            (sec_acl_t *) malloc(sizeof(sec_acl_t));
                break;
        }

        /* Copy the acl information into the new sec_acl */
        nacl_p->default_realm.uuid = sec_acl_p->default_realm.uuid;
        nacl_p->default_realm.name = (unsigned_char_p_t)
                malloc((strlen((char *) sec_acl_p->default_realm.name)+1) *
                sizeof(char));
        strcpy((char *) nacl_p->default_realm.name, 
                (char *) sec_acl_p->default_realm.name);
        nacl_p->sec_acl_manager_type = sec_acl_p->sec_acl_manager_type;
        nacl_p->num_entries = sec_acl_p->num_entries;

        /* The storage of the sec_acl_entry's that the stub passed in is
         * not guaranteed to stick around, so we must allocate our own
         * storage, as opposed to pointing to existing storage...
         */
        nacl_p->sec_acl_entries = (sec_acl_entry_t *)
                malloc(sec_acl_p->num_entries * sizeof(sec_acl_entry_t));
        for (i = 0; i < sec_acl_p->num_entries; i++) {
            /* Copy the pointers to the storage entry and the incoming
             * entry, to make this more readable (ie: <= 80 columns)
             */
            my_e = &(nacl_p->sec_acl_entries[i]);
            new_e = &(sec_acl_p->sec_acl_entries[i]);

            my_e->perms = new_e->perms;
            my_e->entry_info.entry_type = new_e->entry_info.entry_type;

            switch (new_e->entry_info.entry_type) {
                case sec_acl_e_type_user:
                case sec_acl_e_type_group:
                case sec_acl_e_type_foreign_other:
                case sec_acl_e_type_user_deleg:
                case sec_acl_e_type_group_deleg:
                case sec_acl_e_type_for_other_deleg:

                    /* Copy the id's uuid */
                    my_e->entry_info.tagged_union.id.uuid = 
                        new_e->entry_info.tagged_union.id.uuid;

                    /* Make room for the name, and copy it */
                    my_e->entry_info.tagged_union.id.name = (unsigned_char_p_t)
                        malloc(sizeof(unsigned_char_t) * (1 + strlen((char *)
                            new_e->entry_info.tagged_union.id.name)));

                    strcpy((char *) my_e->entry_info.tagged_union.id.name,
                            (char *) new_e->entry_info.tagged_union.id.name);
                    break;

                case sec_acl_e_type_foreign_user:
                case sec_acl_e_type_foreign_group:
                case sec_acl_e_type_for_user_deleg:
                case sec_acl_e_type_for_group_deleg:
                    /* Copy the foreign id's uuids */
                    my_e->entry_info.tagged_union.foreign_id.id.uuid = 
                        new_e->entry_info.tagged_union.foreign_id.id.uuid;
                    my_e->entry_info.tagged_union.foreign_id.realm.uuid = 
                        new_e->entry_info.tagged_union.foreign_id.realm.uuid;

                    /* Make room for the names, and copy them */
                    my_e->entry_info.tagged_union.foreign_id.id.name =
                        (unsigned_char_p_t) malloc (sizeof(unsigned_char_t) *
                            (1 + strlen((char *)
                            new_e->entry_info.tagged_union.foreign_id.id.name
                            )) );
                    strcpy(
                    (char *) my_e->entry_info.tagged_union.foreign_id.id.name,
                    (char *) new_e->entry_info.tagged_union.foreign_id.id.name
                    );

                    my_e->entry_info.tagged_union.foreign_id.realm.name =
                        (unsigned_char_p_t) malloc (sizeof(unsigned_char_t) *
                          (1 + strlen((char *)
                          new_e->entry_info.tagged_union.foreign_id.realm.name
                          )) );
                    strcpy((char *)
                        my_e->entry_info.tagged_union.foreign_id.realm.name,
                        (char *)
                        new_e->entry_info.tagged_union.foreign_id.realm.name
                    );
                    break;

                case sec_acl_e_type_extended:
                  num_bytes = 
                    new_e->entry_info.tagged_union.extended_info->num_bytes;

                  my_e->entry_info.tagged_union.extended_info =
                    (sec_acl_extend_info_t *) malloc(
                        sizeof(uuid_t) + sizeof(ndr_format_t) +
                        sizeof(unsigned32) + (num_bytes * sizeof(ndr_byte))
                    );

                  my_e->entry_info.tagged_union.extended_info->extension_type=
                  new_e->entry_info.tagged_union.extended_info->extension_type;

                  my_e->entry_info.tagged_union.extended_info->format_label =
                   new_e->entry_info.tagged_union.extended_info->format_label;

                  my_e->entry_info.tagged_union.extended_info->num_bytes =
                   new_e->entry_info.tagged_union.extended_info->num_bytes;

                for (j = 0; j < num_bytes; j++)
                 my_e->entry_info.tagged_union.extended_info->pickled_data[j]=
                 new_e->entry_info.tagged_union.extended_info->pickled_data[j];

                break;

            } /* switch entry_type */
        }   /* for number of entries, copy ... */
    }   /* if good_status */
}


/* s e c _ a c l _ m g r _ l o o k u p
 *
 *  Extract the dacl associated with the key.
 *  In this test version the space for a sec_acl_t is already allocated
 *  so don't bother malloc'ing anything... but in a real acl manager,
 *  the acl is most likely going to be retrieved from a separate data-store,
 *  so pass the sec_acl as ** to allow full allocation of the buffer by
 *  sec_acl_mgr_lookup(...).
 */
PUBLIC void sec_acl_mgr_lookup (
    sec_acl_mgr_handle_t    sec_acl_mgr,
    sec_acl_key_t           sec_acl_key,
    uuid_t                  *manager_type_p,
    sec_acl_type_t          sec_acl_type,
    sec_acl_list_t          **sec_acl_list_p,
    error_status_t          *st_p
)
{
    error_status_t  st;
    int             index;

    /* We only have 1 acl to pass back because there's <= 32 permission bits
     * and since NIDL allocates 1 array entry for open arrays, we don't need
     * to malloc any space for it.
     */
    rdacl__validate_params(sec_acl_key, &index, st_p);
    if (GOOD_STATUS(st_p)) {
        /* Allocate space for the list */
        *sec_acl_list_p =
            (sec_acl_list_t *) rpc_ss_allocate(sizeof(sec_acl_list_t));

        switch (sec_acl_type) {
            case sec_acl_type_object:
                if (! test_objects[index].has_obj_acl)
                    SET_STATUS(st_p, sec_acl_no_acl_found);
                else
                    (*sec_acl_list_p)->num_acls = 1;
                    (*sec_acl_list_p)->sec_acls[0] =
                                    test_objects[index].sec_obj_acl;
                break;
            case sec_acl_type_default_object:
                if (! test_objects[index].has_if_acl)
                    SET_STATUS(st_p, sec_acl_no_acl_found);
                else
                    (*sec_acl_list_p)->num_acls = 1;
                    (*sec_acl_list_p)->sec_acls[0] =
                                    test_objects[index].sec_if_acl;
                break;
            case sec_acl_type_default_container:
                if (! test_objects[index].has_id_acl)
                    SET_STATUS(st_p, sec_acl_no_acl_found);
                else
                    (*sec_acl_list_p)->num_acls = 1;
                    (*sec_acl_list_p)->sec_acls[0] =
                                    test_objects[index].sec_id_acl;
                break;
        }
    }
}


/*
 * TEST IMPLEMENTATION OF THE SEC_ACL WIRE INTERFACE ROUTINES
 */

/*  r d a c l _ l o o k u p
 *
 *  retrieve an acl associated with the object referred to in the handle
 *  parameter.  The component_name argument is used to further identify
 *  the entity being protected by the sec_acl.
 *
 *  Comparable to POSIX acl_read()
 */

PUBLIC void rdacl_lookup (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_type_t              sec_acl_type,
    sec_acl_result_t            *sec_acl_result_p
)
{
    error_status_t  st;
    if (debug_acl_svr)
        fprintf(stderr, "DEBUG: Entered rdacl_lookup\n");
    sec_acl_mgr_lookup(sec_acl_mgr, (sec_acl_key_t) component_name,
                        manager_type_p, sec_acl_type,
                        &sec_acl_result_p->tagged_union.sec_acl_list, &st);
    if (GOOD_STATUS(&st)) {
        SET_STATUS(&(sec_acl_result_p->st), error_status_ok);
    } else
        SET_STATUS(&(sec_acl_result_p->st), st);
}

/*  r d a c l v 0 _ l o o k u p
 *
 *  retrieve an acl associated with the object referred to in the handle
 *  parameter.  The component_name argument is used to further identify
 *  the entity being protected by the sec_acl.
 *
 *  Comparable to POSIX acl_read()
 */

PUBLIC void rdaclv0_lookup (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_type_t              sec_acl_type,
    sec_acl_result_t            *sec_acl_result_p
)
{
    error_status_t  st;
    sec_acl_t       *aclP;
    int             i,j;
    sec_acl_extend_info_t *einfoP;

/* Generate a dummy extended info for now, should really use
 * rpc_ss_allocate here so it will be free()'d automagically,
 * but when we do the real code for this routine, there's going
 * to be lots of pickling going on for each type... so it'll be
 * done correctly at that time.  This is just a filler to make
 * sure no new types get passed back to an old acl_edit.
 */
    einfoP = (sec_acl_extend_info_t *) malloc(sizeof(uuid_t) +
        sizeof(ndr_format_t) + sizeof(unsigned32) +
	(1 * sizeof(ndr_byte)) );

    uuid_create(&einfoP->extension_type, &st);
    einfoP->format_label.int_rep = 0;
    einfoP->format_label.char_rep = 0;
    einfoP->format_label.float_rep = 0;
    einfoP->format_label.reserved = 0;
    einfoP->num_bytes = 1;
    einfoP->pickled_data[0] = 10;

    if (debug_acl_svr)
        fprintf(stderr, "DEBUG: Entered rdaclv0_lookup\n");

    sec_acl_mgr_lookup(sec_acl_mgr, (sec_acl_key_t) component_name,
                        manager_type_p, sec_acl_type,
                        &sec_acl_result_p->tagged_union.sec_acl_list, &st);
    if (GOOD_STATUS(&st)) {
        /* Make sure we're not passing back any non-v0 entry types */
        for (i =  0; i < sec_acl_result_p->tagged_union.sec_acl_list->num_acls;
             i++) {
            aclP = sec_acl_result_p->tagged_union.sec_acl_list->sec_acls[i];
            for (j = 0; j < aclP->num_entries; j++) {
                switch (aclP->sec_acl_entries[j].entry_info.entry_type) {
                case sec_acl_e_type_user_obj_deleg:
                case sec_acl_e_type_group_obj_deleg:
                case sec_acl_e_type_other_obj_deleg:
                case sec_acl_e_type_any_other_deleg:
                    /* ... just the permset_t ... */
                    aclP->sec_acl_entries[j].entry_info.entry_type =
                        sec_acl_e_type_extended;
                    aclP->sec_acl_entries[j].entry_info.tagged_union.extended_info = einfoP;
                    break;
                case sec_acl_e_type_user_deleg:
                case sec_acl_e_type_group_deleg:
                case sec_acl_e_type_for_other_deleg:
		    /* sec_id_t */
                    aclP->sec_acl_entries[j].entry_info.entry_type =
                        sec_acl_e_type_extended;
                    aclP->sec_acl_entries[j].entry_info.tagged_union.extended_info = einfoP;
		    break;
                case sec_acl_e_type_for_user_deleg:
                case sec_acl_e_type_for_group_deleg:
                    /* sec_id_foreign_t */
		    
                    aclP->sec_acl_entries[j].entry_info.entry_type =
                        sec_acl_e_type_extended;
                    aclP->sec_acl_entries[j].entry_info.tagged_union.extended_info = einfoP;
                    break;
                }
            }
	}
        SET_STATUS(&(sec_acl_result_p->st), error_status_ok);
    } else {
        SET_STATUS(&(sec_acl_result_p->st), st);
    }
}

/*  r d a c l _ r e p l a c e
 *
 * Replace the acl associated with the object referred to in the handle.
 * ACLs are immutable, the replace operation takes the new acl and throws
 * away the old acl associated with the object.  The component_name
 * argument is used to further identify the entity being protected by the
 * acl.
 *
 * Comparable to POSIX acl_write()
 */

PUBLIC void rdacl_replace (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_type_t              sec_acl_type,
    sec_acl_list_t              *sec_acl_list_p,
    error_status_t              *st_p
)
{
    sec_acl_mgr_replace(sec_acl_mgr, (sec_acl_key_t) component_name,
                        manager_type_p, sec_acl_type, sec_acl_list_p, st_p);
}


/* r d a c l _ t e s t _ a c c e s s
 *
 * Determine if the caller has the requested access.
 */

PUBLIC boolean32 rdacl_test_access (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_acl_permset_t           desired_permset,
    error_status_t              *st_p
)
{
    error_status_t          st;
    sec_id_pac_t            unauth_pac;
    sec_id_t                *user_obj = NULL;
    sec_id_t                *group_obj = NULL;
    rpc_authz_cred_handle_t privs;
    unsigned_char_p_t       server_princ_name;
    unsigned32              authn_level;
    unsigned32              authn_svc;
    unsigned32              authz_svc; 
    unsigned_char_t         dummy_name = '\0';

    if (! uuid_equal(&mgr_uuid, manager_type_p, &st))
        SET_STATUS(st_p, sec_acl_unknown_manager_type);
    else {
        CLEAR_STATUS(st_p);
    }
    
    if (debug_acl_svr) {
        unsigned_char_p_t str;
        rpc_binding_to_string_binding(h, &str, &st);
        fprintf(stderr, "DEBUG: rdacl_test_access called via %s\n", str);
    }

    /* inquire the runtime as to who called us */
    rpc_binding_inq_auth_caller(h, &privs, &server_princ_name, &authn_level,
                                &authn_svc, &authz_svc, st_p);
    if (! GOOD_STATUS(st_p))
        return false;
    else {

        /* May need to generate an unauthenticated (dummy) PAC */
        if (authz_svc != rpc_c_authz_dce) {
            unauth_pac.pac_type = sec_id_pac_format_v1;
            unauth_pac.authenticated = false;
            uuid_create_nil(&unauth_pac.realm.uuid, &st);
            unauth_pac.principal.uuid = unauth_pac.realm.uuid;
            unauth_pac.group.uuid = unauth_pac.realm.uuid;
            unauth_pac.realm.name = &dummy_name;
            unauth_pac.principal.name = &dummy_name;
            unauth_pac.group.name = &dummy_name;
            unauth_pac.num_groups = 0;
            unauth_pac.num_foreign_groups = 0;
            unauth_pac.groups = NULL;
            unauth_pac.foreign_groups = NULL;
	    /* XXX WHAT TO DO? */
	    fprintf(stderr, "FATAL - internal error unauth not working yet\n");
	    exit(sec_login_s_internal_error);
        }

        return(sec_acl_mgr_is_authorized (sec_acl_mgr, desired_permset, &privs,
                                            (sec_acl_key_t) component_name,
                                            manager_type_p, user_obj,
                                            group_obj, st_p));
    }
}


/* r d a c l _ t e s t _ a c c e s s _ o n _ b e h a l f
 *
 * Determine if the subject has the requested access.  This function
 * returns true if the access is available to both the caller and
 * the subject identified in the call.
 */

PUBLIC boolean32 rdacl_test_access_on_behalf (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p,
    sec_id_pac_t                *subject_p,
    sec_acl_permset_t           desired_permset,
    error_status_t              *st_p
)
{
    SET_STATUS(st_p, sec_rgy_not_implemented);
}


/* r d a c l _ g e t _ m a n a g e r _ t y p e s
 *
 * Determine the types of acls protecting an object.  ACL editors/browsers
 * use this operation to determine the acl manager types that a particular
 * reference monitor is using to protect a selected entity.
 */

/*  NOTE:   For test purposes in this initial version, there is 1 manager
 *          type and it has been hardcoded.
 */

PUBLIC void rdacl_get_manager_types (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    sec_acl_type_t              acl_type,
    unsigned32                  size_avail,
    unsigned32                  *size_used_p,
    unsigned32                  *num_types_p,
    uuid_t                      *manager_types,
    error_status_t              *st_p
)
{
    *num_types_p = 1;
    if (size_avail < 1) {
        *size_used_p = 0;
    } else {
        *size_used_p = 1;       /* hardcoded for initial test version */
        *manager_types = mgr_uuid;
    }
    SET_STATUS(st_p, error_status_ok);
}


/* r d a c l _ g e t _ m g r _ t y p e s _ s e m a n t i c s
 *
 * Determine the types of acls protecting an object.  ACL editors/browsers
 * use this operation to determine the acl manager types that a particular
 * reference monitor is using to protect a selected entity.
 */
/*  NOTE:   For test purposes in this initial version, there is 1 manager
 *          type and it has been hardcoded.
 */
PUBLIC void rdacl_get_mgr_types_semantics (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    sec_acl_type_t              acl_type,
    unsigned32                  size_avail,
    unsigned32                  *size_used_p,
    unsigned32                  *num_types_p,
    uuid_t                      manager_types[],
    sec_acl_posix_semantics_t   posix_semantics[],
    error_status_t              *st_p
)
{
    *num_types_p = 1;
    if (size_avail < 1) {
        *size_used_p = 0;
    } else {
        *size_used_p = 1;       /* hardcoded for initial test version */
        manager_types[0]   = mgr_uuid;
        posix_semantics[0] = sec_acl_posix_mask_obj ;
    }
    SET_STATUS(st_p, error_status_ok);
}


/*  r d a c l _ g e t _ p r i n t s t r i n g
 *
 *  Retrieve printable representations for each permission bit that 
 *  the acl manager will support.  There may be aliases for common
 *  permission combinations - by convention simple entries should appear
 *  at the beginning of the array, and combinations should appear at the
 *  end.  When false the tokenize flag indicates that permission
 *  printstrings are unambiguous and therefore printstrings for various 
 *  permissions can be concatenated.  When true, however,  this property 
 *  does not hold and the strings should be tokenized before input or output.
 */

/*  NOTE:   This version is for testing only and thus has hardcoded
 *          printstrings.  Real ACL managers need to have their own
 *          method of determining printstrings (most likely being more
 *          extensible/parameterized than is given here).  For this utility,
 *          if you modify the number of printstrings in the hardcoded
 *          array, you must change the hardcoded NUM_PSTRS so the
 *          stub will pass the correct number of printstrings back.
 */
#define NUM_PSTRS 7

static sec_acl_printstring_t hardcoded_printstrings[] = {
    { "c",  "control",  sec_acl_perm_owner      },
    { "r",  "read",     sec_acl_perm_read       },
    { "w",  "write",    sec_acl_perm_write      },
    { "x",  "execute",  sec_acl_perm_execute    },
    { "i",  "insert",   sec_acl_perm_insert     },
    { "d",  "delete",   sec_acl_perm_delete     },
    { "t",  "test",     sec_acl_perm_test       }
};

static sec_acl_printstring_t hardcoded_manager_info = {
    "acl_test_server", "Sample ACL manager.",
    (sec_acl_perm_owner | sec_acl_perm_read | sec_acl_perm_write |
     sec_acl_perm_execute | sec_acl_perm_insert | sec_acl_perm_delete |
     sec_acl_perm_test)
};

PUBLIC void rdacl_get_printstring (
    handle_t                h,
    uuid_t                  *manager_type_p,
    unsigned32              size_avail,
    uuid_t                  *manager_type_chain,
    sec_acl_printstring_t   *manager_info,
    boolean32               *tokenize_p,
    unsigned32              *total_num_printstrings_p,
    unsigned32              *size_used_p,
    sec_acl_printstring_t   printstrings[],
    error_status_t          *st_p
)
{
    error_status_t  st;
    int             i;

    *total_num_printstrings_p = NUM_PSTRS;
    *size_used_p = (size_avail < NUM_PSTRS) ? size_avail : NUM_PSTRS;
    *manager_info = hardcoded_manager_info;
    uuid_create_nil(manager_type_chain, &st);

    if (! uuid_equal(&mgr_uuid, manager_type_p, &st))
        SET_STATUS(st_p, sec_acl_unknown_manager_type);
    else {
        SET_STATUS(st_p, error_status_ok);
        *tokenize_p = false;
        for (i = 0; i < *size_used_p; i++)
            printstrings[i] = hardcoded_printstrings[i];
    }
}

/*  r d a c l _ g e t _ r e f e r r a l
 *
 * Obtain a referral to an acl update site.  This function is used when
 * the current acl site yields a sec_acl_site_readonly error.  Some
 * replication managers will require all updates for a given object to
 * be directed to a given replica.  Clients of the generic acl interface may
 * know they are dealing with an object that is replicated in this way. 
 * This function allows them to recover from this problem and rebind to
 * the proper update site.
 */
PUBLIC void rdacl_get_referral (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type_p, 
    sec_acl_type_t              sec_acl_type,
    sec_acl_tower_set_t         *towers_p,
    error_status_t              *st_p
)
{
    SET_STATUS(st_p, sec_acl_not_implemented);
}


/*  r d a c l _ g e t _ a c c e s s 
 *
 *  Determine the caller's access to the specified object.  This is 
 *  useful for implenting operations like the conventional UNIX access
 *  function.
 */
PUBLIC void rdacl_get_access (
    handle_t                    h,
    sec_acl_component_name_t    component_name,
    uuid_t                      *manager_type,
    sec_acl_permset_t           *net_rights,
    error_status_t              *st_p
)
{
    SET_STATUS(st_p, sec_acl_not_implemented);
}
