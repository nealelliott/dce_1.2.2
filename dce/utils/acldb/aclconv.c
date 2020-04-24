/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: aclconv.c,v $
 * Revision 1.1.6.1  1996/08/09  11:55:50  arvind
 * 	One minor fix to fix for OT13574.
 * 	[1996/07/25  21:46 UTC  psn  /main/psn_bugfix/7]
 *
 * 	Rename dce_acl__check_dup_id to dce_acl__check_duplicate.
 * 	[1996/07/25  17:34 UTC  psn  /main/psn_bugfix/6]
 *
 * 	Check for error in rpc_ss_malloc and other minor fixes to dce_acl_copy_acl().
 * 	[1996/07/24  20:27 UTC  psn  /main/psn_bugfix/5]
 *
 * 	Fix OTs 10632: dce_acl_obj_add_* should check for illegal entries.
 * 	[1996/07/23  20:04 UTC  psn  /main/psn_bugfix/4]
 *
 * 	Fix OTs 12929: The acldb library uses rpc_sm_client_free instead of
 * 	rpc_sm_free.
 * 	[1996/07/23  15:58 UTC  psn  /main/psn_bugfix/3]
 *
 * 	Fix OTs 13547: dce_acl_copy_acl() handles foreign_id incorrectly
 * 	[1996/07/23  15:39 UTC  psn  /main/psn_bugfix/2]
 *
 * 	Fix OTs 13031: dce_acl_obj_add_obj lacks support to add user_obj,
 * 	group_obj, mask_obj & other_obj.
 * 	[1996/07/23  14:34 UTC  psn  /main/psn_bugfix/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:19:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:21  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:35:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  18:07:07  root]
 * 
 * Revision 1.1.2.8  1994/07/15  17:27:04  bowe
 * 	Remove dce_acl_g_helpstring. [CR 11308]
 * 	[1994/07/15  17:26:20  bowe]
 * 
 * Revision 1.1.2.7  1994/06/10  22:26:13  cbrooks
 * 	CR10897 - only memset() sec_acl_entries once in incr__acl_ent_list
 * 	[1994/06/10  22:25:12  cbrooks]
 * 
 * 	CR 10897
 * 	[1994/06/10  21:11:31  cbrooks]
 * 
 * Revision 1.1.2.6  1994/06/03  13:59:46  bowe
 * 	Use rpc_sm_allocate()/rpc_sm_client_free() instead of malloc()/free().
 * 	[CR 10778]
 * 
 * Revision 1.1.2.5  1994/05/18  19:27:59  bowe
 * 	Check for NULL ptrs in dce_acl_inq_prin_and_group()
 * 	[1994/05/16  16:59:52  bowe]
 * 
 * Revision 1.1.2.4  1994/05/10  21:45:00  bowe
 * 	Adjust for obj header simplification
 * 	[1994/05/10  21:40:01  bowe]
 * 
 * Revision 1.1.2.3  1994/05/05  15:18:53  bowe
 * 	Renamed dce_acl_obj_init(), dce_acl_obj_free_entries() (new names).
 * 	Added some missing cases for delegation in _free_entries.  [CR 10001]
 * 	[1994/05/05  13:32:11  bowe]
 * 
 * Revision 1.1.2.2  1994/04/27  20:00:16  bowe
 * 	Check for NULL args in dce_acl_inq_prin_and_group().
 * 	[1994/04/27  19:59:58  bowe]
 * 
 * Revision 1.1.2.1  1994/04/26  19:11:25  bowe
 * 	Initial checkin (was part of dceaclimpl.c, now defunct)
 * 	[1994/04/26  19:09:18  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

/*  ACL convenience functions.  */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <aclimpl.h>

#include <dce/uuid.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <dce/dce_cf.h>

/* ______________________________________________________________________ */
/* Generic printstring for regular/basic/simple ACL manager.  Programmers
 * must define their own helpstring (which contains manager name).
 */

sec_acl_printstring_t dce_acl_g_printstring[] = {
    { "r",  "read",	sec_acl_perm_read	},
    { "w",  "write",	sec_acl_perm_write	},
    { "x",  "execute",	sec_acl_perm_execute	},
    { "c",  "control",	sec_acl_perm_control	},
    { "i",  "insert",	sec_acl_perm_insert	},
    { "d",  "delete",	sec_acl_perm_delete	},
    { "t",  "test",	sec_acl_perm_test	}
};

/* ______________________________________________________________________ */
/*  Create an ACL for an object of type mgr_type.  Caller passes in
 *  the (pointer to) ACL itself.  This will initialize the fields.
 */

void
dce_acl_obj_init(
    uuid_t		*mgr_type,
    sec_acl_t		*acl,
    error_status_t	*st
)
{
    unsigned_char_t	*cellname;
    sec_rgy_handle_t	rh;
    *st = error_status_ok;
    acl->sec_acl_manager_type	= *mgr_type;
    acl->num_entries		= 0;
    acl->sec_acl_entries	= NULL;

    /* First, null out the ACL structure. */
    memset(&acl->default_realm, 0, sizeof(sec_acl_id_t));

    /* Get the realm of the ACL */
    dce_cf_get_cell_name((char **)&cellname, st);
    STAT_CHECK_RET(*st);
    sec_rgy_site_open(cellname, &rh, st);
    if (*st != error_status_ok) {
	free(cellname);
	return;
    }
    sec_id_parse_name(rh, cellname, NULL, &acl->default_realm.uuid, NULL,
	NULL, st);
    free(cellname);
    STAT_CHECK_RET(*st);
    sec_rgy_site_close(rh, st);
    acl->default_realm.name	= NULL;
}

/* ______________________________________________________________________ */
/*  Free space used by an ACL (the entries), then NULL it out. */

void
dce_acl_obj_free_entries(
    sec_acl_t		*acl,
    error_status_t	*st
)
{
    int			i;
    sec_acl_entry_t	*ae;

    *st = error_status_ok;

    for (i=0; i<acl->num_entries; i++) {
	ae = &acl->sec_acl_entries[i];
	/* free each entry */

	switch (ae->entry_info.entry_type) {
	case sec_acl_e_type_mask_obj:
	case sec_acl_e_type_user_obj:
	case sec_acl_e_type_group_obj:
	case sec_acl_e_type_other_obj:
	case sec_acl_e_type_unauthenticated:
	case sec_acl_e_type_any_other:
	case sec_acl_e_type_user_obj_deleg:
	case sec_acl_e_type_group_obj_deleg:
	case sec_acl_e_type_other_obj_deleg:
	case sec_acl_e_type_any_other_deleg:
	    break;			/* no action */

	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:
	case sec_acl_e_type_user_deleg:
	case sec_acl_e_type_group_deleg:
	case sec_acl_e_type_for_other_deleg:
	    /* in most cases, names are null (uuid's are used only) */
	    if (ae->entry_info.tagged_union.id.name)
		rpc_sm_free(ae->entry_info.tagged_union.id.name, st);
	    break;

	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group:
	case sec_acl_e_type_for_user_deleg:
	case sec_acl_e_type_for_group_deleg:
	    /* in most cases, names are null (uuid's are used only) */

	    if (ae->entry_info.tagged_union.foreign_id.id.name)
		rpc_sm_free(ae->entry_info.tagged_union.foreign_id.id.name, st);
	    if (ae->entry_info.tagged_union.foreign_id.realm.name)
		rpc_sm_free(ae->entry_info.tagged_union.foreign_id.realm.name, st);
	    break;

	case sec_acl_e_type_extended:
            if (ae->entry_info.tagged_union.extended_info)
                rpc_sm_free(ae->entry_info.tagged_union.extended_info, st);
	    break;

	default:
	    break;
	}
    }

    if (acl->sec_acl_entries)
	rpc_sm_free(acl->sec_acl_entries, st);
    memset(acl, 0, sizeof(sec_acl_t));
    return ;
}

/* ______________________________________________________________________ */
/*  Routines for adding entries to ACLs. */

/*  Increase the size of the entry list by 1.  Does malloc() or realloc().
 *  Caller calls dce_acl_obj_free_entries() to free all the stuff for this ACL.
 */

static sec_acl_entry_t *
incr__acl_ent_list(
    sec_acl_t		*acl
)
{
    sec_acl_entry_t		*e, *ep, *ep2;
    unsigned32			i;
    error_status_t		st;

    if (acl->num_entries == 0) {
	acl->sec_acl_entries = rpc_sm_allocate(sizeof(sec_acl_entry_t), &st);
	if (st != error_status_ok)
	    return NULL;
	acl->num_entries = 1;
    }
    else {
	/* Allocate a new list, then copy entries.  If successful,
	 * then set sec_acl_entries within ACL struct. */

	e = rpc_sm_allocate((acl->num_entries + 1) * sizeof *e, &st);
	if (st != error_status_ok)
	    return NULL;

	for (i=0,ep=e,ep2=acl->sec_acl_entries; i<acl->num_entries; i++) 
	{
	    *ep++ = *ep2++;
	}

	rpc_sm_free(acl->sec_acl_entries, &st);
	if (st != error_status_ok)
	    return NULL;
	acl->sec_acl_entries = e;
	acl->num_entries++;
    }

    ep2 = &acl->sec_acl_entries[acl->num_entries-1];
    memset(ep2, 0, sizeof(sec_acl_entry_t));
    return ep2 ;
}

static void
dce_acl__check_perm_bits(
    sec_acl_t			*acl,
    sec_acl_permset_t		permset,
    error_status_t		*st
)
{
    acl_mgt_t			*am_info;

    /* Get max permission bits for this  */
    am_info = dce_acl__get_info_from_type(&acl->sec_acl_manager_type, st);
    STAT_CHECK_RET(*st);

    if ( (~am_info->max_perm) & permset)
	*st = sec_acl_invalid_permission;
}

static void
dce_acl__check_duplicate(
    sec_acl_t			*acl,
    sec_acl_entry_type_t	entry_type,
    uuid_t			*id,
    uuid_t			*realm,
    error_status_t		*st
)
{
    sec_acl_entry_t             *e = 0 ;
    unsigned32                  i;
    error_status_t		ignore_st;
    signed32                    res;

    *st = error_status_ok;

    for (i=0, e=acl->sec_acl_entries;
         i<acl->num_entries && *st == error_status_ok;
         i++,e++) {
        if (e->entry_info.entry_type == entry_type) {
            switch (entry_type) {
            /* id_entry type */
            case sec_acl_e_type_user:
            case sec_acl_e_type_group:
            case sec_acl_e_type_foreign_other:
            case sec_acl_e_type_user_deleg:
            case sec_acl_e_type_group_deleg:
            case sec_acl_e_type_for_other_deleg:
                if (id) {
                   if (uuid_equal(id,
                                  &(e->entry_info.tagged_union.id.uuid),
                                  &ignore_st))
                      *st = sec_acl_duplicate_entry;
                } else {
                   /* Caller of this routine should have passed a non-NULL
                      id for this entry_type */
                   *st = sec_acl_bad_parameter;
                }
                break;
            /* obj type */
            case sec_acl_e_type_mask_obj:
            case sec_acl_e_type_user_obj:
            case sec_acl_e_type_group_obj:
            case sec_acl_e_type_other_obj:
            case sec_acl_e_type_unauthenticated:
            case sec_acl_e_type_any_other:
            case sec_acl_e_type_user_obj_deleg:
            case sec_acl_e_type_group_obj_deleg:
            case sec_acl_e_type_other_obj_deleg:
            case sec_acl_e_type_any_other_deleg:
                *st = sec_acl_duplicate_entry;
                break;
            /* foreign_id type */
            case sec_acl_e_type_foreign_user:
            case sec_acl_e_type_foreign_group:
            case sec_acl_e_type_for_user_deleg:
            case sec_acl_e_type_for_group_deleg:
                if (realm && id) {
                   if (uuid_equal(realm,
                           &(e->entry_info.tagged_union.foreign_id.realm.uuid),
                           &ignore_st)
                        &&
                       uuid_equal(id,
                           &(e->entry_info.tagged_union.foreign_id.id.uuid),
                           &ignore_st))
                      *st = sec_acl_duplicate_entry;
                } else {
                   /* Caller of this routine should have passed a non-NULL
                      realm and id for this entry_type */
                   *st = sec_acl_bad_parameter;
                }
                break;
            default:
                break;
            }
        }
    }
}

/*  Add an ACL entry (user/grp, domestic/foreign) to the given ACL. */

void
dce_acl_obj_add_id_entry(
    sec_acl_t			*acl,
    sec_acl_entry_type_t	entry_type,
    sec_acl_permset_t		permset,
    uuid_t			*id,
    error_status_t		*st
)
{
    sec_acl_entry_t		*e = 0 ;

    *st = error_status_ok;

    switch (entry_type) {
    case sec_acl_e_type_user:
    case sec_acl_e_type_group:
    case sec_acl_e_type_foreign_other:
    case sec_acl_e_type_user_deleg:
    case sec_acl_e_type_group_deleg:
    case sec_acl_e_type_for_other_deleg:
	dce_acl__check_perm_bits(acl, permset, st);
	STAT_CHECK_RET(*st);
	dce_acl__check_duplicate(acl, entry_type, id, (uuid_t *)NULL, st);
	STAT_CHECK_RET(*st);
	if ((e = incr__acl_ent_list(acl)) == NULL)
	{
	    *st = rpc_s_no_memory ;
	    return;
	}
	e->perms			= permset;
	e->entry_info.entry_type	= entry_type;

	/* Just the ID */

	e->entry_info.tagged_union.id.uuid	= *id;
	e->entry_info.tagged_union.id.name	= NULL;
	break;

    default:
	*st = sec_acl_invalid_entry_type;
	break;
    }
}

/*  Add a user ACL entry to the given ACL. */

void
dce_acl_obj_add_user_entry(
    sec_acl_t		*acl,
    sec_acl_permset_t	permset,
    uuid_t		*user,
    error_status_t	*st
)
{
    dce_acl_obj_add_id_entry(acl, sec_acl_e_type_user, permset, user, st);
}

/*  Add a group ACL entry to the given ACL. */

void
dce_acl_obj_add_group_entry(
    sec_acl_t		*acl,
    sec_acl_permset_t	permset,
    uuid_t		*group,
    error_status_t	*st
)
{
    dce_acl_obj_add_id_entry(acl, sec_acl_e_type_group, permset, group, st);
}

/*  Add an "obj" ACL entry to the given ACL. */

void
dce_acl_obj_add_obj_entry(
    sec_acl_t			*acl,
    sec_acl_entry_type_t	entry_type,
    sec_acl_permset_t		permset,
    error_status_t		*st
)
{
    sec_acl_entry_t		*e = 0 ;

    *st = error_status_ok;
    if ((e = incr__acl_ent_list(acl)) == NULL)
    {
	*st = rpc_s_no_memory ;
	return;
    }

    switch (entry_type) {
    case sec_acl_e_type_mask_obj:
    case sec_acl_e_type_user_obj:
    case sec_acl_e_type_group_obj:
    case sec_acl_e_type_other_obj:
    case sec_acl_e_type_unauthenticated:
    case sec_acl_e_type_any_other:
    case sec_acl_e_type_user_obj_deleg:
    case sec_acl_e_type_group_obj_deleg:
    case sec_acl_e_type_other_obj_deleg:
    case sec_acl_e_type_any_other_deleg:

	dce_acl__check_perm_bits(acl, permset, st);
	STAT_CHECK_RET(*st);
	dce_acl__check_duplicate(acl, entry_type, (uuid_t *)NULL, (uuid_t *)NULL, st);
	STAT_CHECK_RET(*st);
	e->perms			= permset;
	e->entry_info.entry_type	= entry_type;
	break;

    default:
	*st = sec_acl_invalid_entry_type;
	break;
    }
}

/*  Add ACL entry for "unauthenticated" to the given ACL. */

void
dce_acl_obj_add_unauth_entry(
    sec_acl_t		*acl,
    sec_acl_permset_t	permset,
    error_status_t	*st
)
{
    dce_acl_obj_add_obj_entry(acl, sec_acl_e_type_unauthenticated, permset, st);
}

/*  Add ACL entry for "any_other" to the given ACL. */

void
dce_acl_obj_add_any_other_entry(
    sec_acl_t		*acl,
    sec_acl_permset_t	permset,
    error_status_t	*st
)
{
    dce_acl_obj_add_obj_entry(acl, sec_acl_e_type_any_other, permset, st);
}

/*  Add ACL entry for a foreign user or group to the given ACL. */

void
dce_acl_obj_add_foreign_entry(
    sec_acl_t			*acl,
    sec_acl_entry_type_t	entry_type,
    sec_acl_permset_t		permset,
    uuid_t			*realm,
    uuid_t			*id,
    error_status_t		*st
)
{
    sec_acl_entry_t		*e;
    sec_id_foreign_t		*fid;

    *st = error_status_ok;
    switch (entry_type) {
    case sec_acl_e_type_foreign_user:
    case sec_acl_e_type_foreign_group:
    case sec_acl_e_type_for_user_deleg:
    case sec_acl_e_type_for_group_deleg:
	dce_acl__check_perm_bits(acl, permset, st);
	STAT_CHECK_RET(*st);
	dce_acl__check_duplicate(acl, entry_type, id, realm, st);
	STAT_CHECK_RET(*st);
	if((e = incr__acl_ent_list(acl)) == NULL)
	{
	    *st = rpc_s_no_memory ;
	    return;
	}
	
	e->perms			= permset;
	e->entry_info.entry_type	= entry_type;

	fid = &e->entry_info.tagged_union.foreign_id;
	fid->realm.uuid = *realm;
	fid->id.uuid    = *id;
	fid->realm.name = fid->id.name = NULL;
	break;

    default:
	*st = sec_acl_invalid_entry_type;
	break;
    }
}

/* ______________________________________________________________________ */
/* Copy an ACL: acl1 -> acl2.
 * Space for acl2 is passed in, but the sec_acl_entries are allocated
 *  (just call free(acl2->sec_acl_entries) when done with it).
 */
void
dce_acl_copy_acl(
    sec_acl_t		*acl1,
    sec_acl_t		*acl2,
    error_status_t	*st
)
{
    int		i;
    idl_char	*cp;
    sec_acl_entry_t		*ae1, *ae2;

    acl2->default_realm		= acl1->default_realm;
    acl2->sec_acl_manager_type	= acl1->sec_acl_manager_type;
    acl2->sec_acl_entries	= (sec_acl_entry_t *)rpc_sm_allocate(
		sizeof(sec_acl_entry_t)*acl1->num_entries, st);
    STAT_CHECK_RET(*st);
    memset(acl2->sec_acl_entries, 0, sizeof(sec_acl_entry_t)*acl1->num_entries);

    for (i=0; *st == error_status_ok && i<acl1->num_entries; i++) {
	ae1 = &acl1->sec_acl_entries[i];
	ae2 = &acl2->sec_acl_entries[i];
        /* Copy perms and entry type */
	ae2->perms = ae1->perms;
	ae2->entry_info.entry_type = ae1->entry_info.entry_type;

	switch (ae1->entry_info.entry_type) {
	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:
	case sec_acl_e_type_user_deleg:
	case sec_acl_e_type_group_deleg:
	case sec_acl_e_type_for_other_deleg:
            ae2->entry_info.tagged_union.id.uuid =
	        ae1->entry_info.tagged_union.id.uuid;
	    if (ae1->entry_info.tagged_union.id.name) {
		cp = ae1->entry_info.tagged_union.id.name;
		ae2->entry_info.tagged_union.id.name =
			(idl_char *)rpc_sm_allocate(strlen((char *)cp) + 1, st);
                if (*st != error_status_ok)
                    break;
		strcpy((char *)ae2->entry_info.tagged_union.id.name, (char *)cp);
	    }
	    else
		ae2->entry_info.tagged_union.id.name = NULL;
	    break;

	case sec_acl_e_type_mask_obj:
	case sec_acl_e_type_user_obj:
	case sec_acl_e_type_group_obj:
	case sec_acl_e_type_other_obj:
	case sec_acl_e_type_unauthenticated:
	case sec_acl_e_type_any_other:
	case sec_acl_e_type_user_obj_deleg:
	case sec_acl_e_type_group_obj_deleg:
	case sec_acl_e_type_other_obj_deleg:
	case sec_acl_e_type_any_other_deleg:
	    break;			/* no action */

	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group:
	case sec_acl_e_type_for_user_deleg:
	case sec_acl_e_type_for_group_deleg:
            /* Copy foreign id */
	    ae2->entry_info.tagged_union.foreign_id.id.uuid =
	        ae1->entry_info.tagged_union.foreign_id.id.uuid;
            /* Copy foreign id name if not NULL */
	    if (ae1->entry_info.tagged_union.foreign_id.id.name) {
		cp = ae1->entry_info.tagged_union.foreign_id.id.name;
		ae2->entry_info.tagged_union.foreign_id.id.name =
			(idl_char *)rpc_sm_allocate(strlen((char *)cp) + 1, st);
                if (*st != error_status_ok)
                    break;
		strcpy((char *)ae2->entry_info.tagged_union.foreign_id.id.name, (char *)cp);
                
	    }
	    else
		ae2->entry_info.tagged_union.foreign_id.id.name = NULL;

            /* Copy foreign realm */
	    ae2->entry_info.tagged_union.foreign_id.realm.uuid =
		ae1->entry_info.tagged_union.foreign_id.realm.uuid;
            /* Copy foreign realm name if not NULL */
	    if (ae1->entry_info.tagged_union.foreign_id.realm.name) {
		cp = ae1->entry_info.tagged_union.foreign_id.realm.name;
		ae2->entry_info.tagged_union.foreign_id.realm.name =
			(idl_char *)rpc_sm_allocate(strlen((char *)cp) + 1, st);
                if (*st != error_status_ok)
                    break;
		strcpy((char *)ae2->entry_info.tagged_union.foreign_id.realm.name, (char *)cp);
	    }
	    else
		ae2->entry_info.tagged_union.foreign_id.realm.name = NULL;
	    break;
	case sec_acl_e_type_extended:
            if (ae1->entry_info.tagged_union.extended_info) {
                idl_size_t ext_len;
                sec_acl_extend_info_t *ae2e, *ae1e;

                ae1e = ae1->entry_info.tagged_union.extended_info;
                /* The size should be calculated using:
                 *      sizeof(sec_acl_extend_info_t) - 1 +
                 *      ae1e->num_bytes * sizeof(idl_byte)  
                 * The following was chosen to keep consistency with other
                 * modules of security code that deals with extended_info.
                 * The approach below could break if strcture field alignment
                 * or size changes.
                 */
                ext_len = sizeof(uuid_t) + sizeof(ndr_format_t) +
                          sizeof(unsigned32) + 
                          ae1e->num_bytes * sizeof(idl_byte);
                ae2e = ae2->entry_info.tagged_union.extended_info =
                    (sec_acl_extend_info_t *)rpc_sm_allocate(ext_len, st);
                if (*st != error_status_ok)
                    break;
                ae2e->extension_type = ae1e->extension_type;
                ae2e->format_label   = ae1e->format_label;
                ae2e->num_bytes      = ae1e->num_bytes;
	        memcpy(&(ae2e->pickled_data), &(ae1e->pickled_data),
                       ae1e->num_bytes);
            } else
                ae2->entry_info.tagged_union.extended_info = NULL;
	    break;

	default:
	    break;
	}
    }

    acl2->num_entries = acl1->num_entries;

    /* Free up allocated memory if we encounter failure during copy */
    if (*st != error_status_ok) {
        error_status_t dummy_st;
        dce_acl_obj_free_entries(acl2, &dummy_st);
    }
}

/* ______________________________________________________________________ */
/*  Retrieve the UUID of the appropriate ACL object from a standard
 *  database header.
 */

void
dce_acl_inq_acl_from_header(
    dce_db_header_t	dbh,
    sec_acl_type_t	sec_acl_type,
    uuid_t		*acl_uuid,
    error_status_t	*st
)
{
    dce_db_dataheader_t	*dhdr;

    *st = error_status_ok;
    dhdr = &dbh.tagged_union.h;

    switch (sec_acl_type) {
    default:
	*st = sec_acl_invalid_acl_type;
	break;

    case sec_acl_type_object:
	*acl_uuid = dhdr->acl_uuid;
	break;

    case sec_acl_type_default_object:
	*acl_uuid = dhdr->def_object_acl;
	break;

    case sec_acl_type_default_container:
	*acl_uuid = dhdr->def_container_acl;
	break;
    }
}

/* ______________________________________________________________________ */
/* Find the principal and group of the caller.  This is useful for filling
 * in the owner_id and group_id fields of standard data/obj headers.
 */

void
dce_acl_inq_prin_and_group(
    handle_t		h,
    uuid_t		*principal,
    uuid_t		*group,
    error_status_t	*st
)
{
    rpc_authz_cred_handle_t	cred_h;
    sec_cred_pa_handle_t	*cred_pa;
    sec_id_pa_t			*pa_p;

    *st = error_status_ok;

    if (!h) {
	if (principal) uuid_create_nil(principal, st);
	if (group)     uuid_create_nil(group, st);
	return;
    }
    /* Get the caller's credentials */
    dce_acl_inq_client_creds(h, &cred_h, st);
    STAT_CHECK_RET(*st);
    if (dce_acl_is_unauthenticated(cred_h)) {
	if (principal)	uuid_create_nil(principal, st);
	if (group)	uuid_create_nil(group, st);
	return;
    }
    cred_pa = sec_cred_get_initiator(cred_h, st);
    STAT_CHECK_RET(*st);
    pa_p = sec_cred_get_pa_data(cred_pa, st);
    if (principal)	*principal = pa_p->principal.uuid;
    if (group)		*group     = pa_p->group.uuid;
}

/* ______________________________________________________________________ */
