/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kmlocal.c,v $
 * Revision 1.1.4.2  1996/02/18  19:15:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:52  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:26:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:59:14  root]
 * 
 * Revision 1.1.2.3  1994/08/24  19:27:00  bowe
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	[1994/08/24  19:09:15  bowe]
 * 
 * Revision 1.1.2.2  1994/08/03  22:06:33  pwang
 * 	Fixed several problems after tested with dcecp.
 * 	[1994/08/03  22:04:47  pwang]
 * 
 * Revision 1.1.2.1  1994/07/28  21:55:35  bowe
 * 	Initial checkin.  Local access to keytabs using rkeytab EPV. [CR 11280]
 * 	[1994/07/28  21:54:07  bowe]
 * 
 * $EndLog$
 */
/*
 *  This has similar functionality to dced/server/kmops.c, except it all
 *  works on local keytabs (files) and it does not store the keytab entries
 *  (instead, they are just kept in memory).  This is basically so that
 *  dcecp can manipulate keytab files like rgy_edit did in DCE 1.0.
 */
/* ______________________________________________________________________ */

#ifndef	_DEF_LOCAL_EPV_
#define	_DEF_LOCAL_EPV_
#endif

#include <dce/dce.h>
#include <dcedimpl.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>

/* ______________________________________________________________________ */

static dced_entry_t	*kentry;
static int		kentry_count;

/* Private functions to add, delete, and find in-core entries. */

static dced_entry_t *
ktab__find_by_name(char *name)
{
    int			i;
    dced_entry_t	*ep;

    for (i=kentry_count,ep=kentry; i--; ep++)
	if (!strcmp((char *)ep->name, name))
	    return ep;
    return NULL;
}

static dced_entry_t *
ktab__find_by_uuid(uuid_t *uuid)
{
    int			i;
    dced_entry_t	*ep;
    error_status_t	st;

    for (i=kentry_count,ep=kentry; i--; ep++)
	if (uuid_equal(&ep->id, uuid, &st))
	    return ep;
    return NULL;
}

static void
ktab__add(dced_entry_t *ep)
{
    int		i;

    i = kentry_count++;
    if (kentry_count == 1)
	kentry = malloc(sizeof(dced_entry_t));
    else
	kentry = realloc(kentry, sizeof(dced_entry_t) * kentry_count);
    kentry[i] = *ep;
    kentry[i].name = (dced_string_t)dce_strdup((char *)ep->name);
    /* description is optional */
    if (ep->description) {
    	kentry[i].description = 
			(dced_string_t)dce_strdup((char *)ep->description);
    }
    kentry[i].storage_tag = (dced_string_t)dce_strdup((char *)ep->storage_tag);
}

static void
ktab__del(uuid_t *uuid)
{
    int			i;
    dced_entry_t	*ep;
    error_status_t	st;

    if (kentry_count == 0) return;

    for (i=0,ep=kentry; i<kentry_count; ep++,i++) {
	if (uuid_equal(&ep->id, uuid, &st))
	    break;
    }
    if (i >= kentry_count)		/* see if not found */
	return;
    kentry_count--;
    free(kentry[i].name);
    /* description is optional */
    if (kentry[i].description) {
    	free(kentry[i].description);
    }
    free(kentry[i].storage_tag);
    /* If one to remove is last, just drop it off the end.  If not,
     * move last one to to this one's slot. */
    if (i < kentry_count)
	kentry[i] = kentry[kentry_count];
    if (kentry_count == 0)
	free(kentry);
    else
	kentry = realloc(kentry, sizeof(dced_entry_t) * kentry_count);
}

/* ______________________________________________________________________ */
/*  Create an entry for a keytab, and either create or import its keytab file.
 */

static void
dced__rkeytab_create(
  /* in */
    handle_t			h,
    dced_entry_t		*id,
    boolean32			just_import,
    dced_key_list_t		*klist,
  /* out */
    error_status_t		*st
)
{
    int                         access_return;
    unsigned32			i;

    *st = error_status_ok;

    /* See if this object exists first. */
    if (ktab__find_by_name((char *)id->name) ) {
	*st = dced_s_already_exists;
	return;
    }

    access_return = access((char *)id->storage_tag, F_OK);
    if (just_import) {
        /* Importing - keytab file must exist.  We do not write the file. */
        if (access_return != 0) {
            *st = dced_s_import_cant_access;
            return;
        }
    }
    else {
        /* Not importing - file must not exist.  We'll write the file. */
        if (access_return == 0) {
            *st = dced_s_import_already_exists;
            return;
        }

        /* write the key list to the keytab file */
        for (i=0; i<klist->count; i++) {
            sec_key_mgmt_set_key(klist->list[i].authn_service, id->storage_tag,
                        klist->list[i].principal, klist->list[i].version,
                        (void *)klist->list[i].passwd, st);
            CHECK_STATUS_RET(st);
        }
    }

    ktab__add(id);
}

/* ______________________________________________________________________ */
/*  Delete the entry for a keytab, and optionally its keytab file.
 */

static void
dced__rkeytab_delete(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    boolean32			remove_object,
  /* out */
    error_status_t		*st
)
{
    dced_entry_t	*ep;

    *st = error_status_ok;

    if ((ep = ktab__find_by_uuid(id_uuid)) == NULL) {
	*st = db_s_key_not_found;
	return;
    }

    if (remove_object) {
	/* remove object (file) itself */
	if (unlink((char *)ep->storage_tag) < 0) {
	    *st = dced_s_cant_remove_storage_file;
	}
    }
    ktab__del(id_uuid);
}

/* ______________________________________________________________________ */
/*  Get keys in a keytab file.
 */

static void
dced__rkeytab_get_keys(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
  /* in,out */
    dced_key_list_t		*klist,
  /* out */
    error_status_t		*st
)
{
    void			*cursor;
    sec_passwd_type_t		keytype;
    error_status_t		st2;
    char			*keytab_file;
    int				i;
    idl_char			*prin;
    int				n_keys;
    unsigned32			key_vno;
    sec_passwd_rec_t		*pwd;
    dced_entry_t	*ep;

    *st = error_status_ok;

    klist->count = 0;
    klist->list = NULL;

    if ((ep = ktab__find_by_uuid(id_uuid)) == NULL) {
	*st = db_s_key_not_found;
	return;
    }

    n_keys = 0;
    keytype = sec_passwd_des;
    keytab_file = (char *)ep->storage_tag;
    sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_secret, keytab_file,
	NULL, &keytype, &cursor, st);
    CHECK_STATUS_RET(st);
    for ( ; ; ) {
	sec_key_mgmt_get_next_key(cursor, NULL, &key_vno, NULL, st);
	if (*st != error_status_ok)
	    break;
	n_keys++;
    }
    /* Games to return the "more important" error, if any */
    sec_key_mgmt_release_cursor(&cursor, &st2);
    /* Check for end of list, which is a "good thing" */
    if (*st != sec_key_mgmt_e_key_unavailable)
	return;
    if (st2 != error_status_ok) {
	*st = st2;
	return;
    }
    *st = error_status_ok;

    if (!n_keys) {
	return;
    }

    /* Allocate space for the returned list.  We know how many we need. */
    MALLOC_RESET_RET(klist->list, dced_key_t, n_keys, st);

    sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_secret, keytab_file,
	NULL, &keytype, &cursor, st);
    if (!GOOD_STATUS(st)) {
	free(klist->list);
	return;
    }
    for (i=0; i<n_keys; i++) {
	sec_key_mgmt_get_next_key(cursor, &prin, &key_vno,
		    (void **)&pwd, st);
	if (!GOOD_STATUS(st)) {
		dced_key_list_free_ptrs(klist);
		return;
	}
	klist->list[i].principal = (dced_string_t)dce_strdup((char *)prin);
	klist->list[i].version = pwd->version_number;
	klist->list[i].authn_service = rpc_c_authn_dce_secret;
	MALLOC_RESET(klist->list[i].passwd, sec_passwd_rec_t, 1, st);
	if (!GOOD_STATUS(st)) {
		dced_key_list_free_ptrs(klist);
		return;
	}
	/* the keydata in pwd is des-key */
	*klist->list[i].passwd		= *pwd;
    }
    sec_key_mgmt_release_cursor(&cursor, &st2);
    /* Check for end of list, which is a "good thing" */
    if (*st == sec_key_mgmt_e_key_unavailable)
	*st = error_status_ok;
    if (GOOD_STATUS(st) && !GOOD_STATUS(&st2)) {
	dced_key_list_free_ptrs(klist);
	SET_STATUS(st, st2);
	return;
    }
    klist->count = n_keys;
}

/* ______________________________________________________________________ */
/*  Add a key to a keytab file.
 */

static void
dced__rkeytab_add_key(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    dced_key_t			*key,
  /* out */
    error_status_t		*st
)
{
    dced_entry_t	*ep;

    *st = error_status_ok;

    if ((ep = ktab__find_by_uuid(id_uuid)) == NULL) {
	*st = db_s_key_not_found;
	return;
    }

    if ((key->passwd) && (key->version != key->passwd->version_number)) {
	*st = dced_s_key_version_mismatch;
	return;
    }

    if (key->version == 0) {
	*st = dced_s_key_v0_not_allowed;
	return;
    }

    if (DCED_IS_RANDOM_KEY(key)) {
	*st = dced_s_random_key_not_allowed;
	return;
    }

    /* add key to list */
    sec_key_mgmt_set_key(key->authn_service, ep->storage_tag,
	key->principal, key->version, (void *)key->passwd, st);
    CHECK_STATUS_RET(st);

}

/* ______________________________________________________________________ */
/*  Delete a key from a keytab file.
 */

static void
dced__rkeytab_delete_key(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    dced_key_t			*key,
  /* out */
    error_status_t		*st
)
{
    error_status_t		st2;
    void			*cursor;
    idl_char			*prin;
    unsigned32			key_vno;
    sec_passwd_type_t		keytype;
    char			*keytab_file;
    dced_entry_t	*ep;

    *st = error_status_ok;

    if ((ep = ktab__find_by_uuid(id_uuid)) == NULL) {
	*st = db_s_key_not_found;
	return;
    }

    /* If version is 0, delete all versions of that prin's key.
     * We can't delete a key in the "cursor loop", since kerberos
     * locks the keyfile.  So we loop through, remembering the
     * version numbers, release the cursor, then delete those keys.
     * NOTE: sec_key_mgmt_ code says kerberos version numbers are 1 byte.
     */
    keytab_file = (char *)ep->storage_tag;
    if (key->version == 0) {
	unsigned32	vv[256];	/* version vector */
	unsigned32	nv, i;

	keytype = sec_passwd_des;
	sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_secret, keytab_file,
	    key->principal, &keytype, &cursor, st);
	CHECK_STATUS_RET(st);
	for (i=0; i<0xFF; i++)
	    vv[i] = 0;
	for (nv = 0; ; ) {
	    sec_key_mgmt_get_next_key(cursor, &prin, &key_vno, NULL, st);
	    if (*st != error_status_ok)
		break;
	    vv[nv++] = key_vno;
	}
	sec_key_mgmt_release_cursor(&cursor, &st2);
	for (i=0; i<nv; i++) {
	    sec_key_mgmt_delete_key(rpc_c_authn_dce_secret, keytab_file,
		prin, vv[i], st);
	    if (*st != error_status_ok)
		break;
	}
	/* XXX - do we have to free prin? */
    }
    else {
	sec_key_mgmt_delete_key(key->authn_service, keytab_file,
		key->principal, key->version, st);
	CHECK_STATUS_RET(st);
    }
}

/* ______________________________________________________________________ */
/*  Change a key in the registry.
 */

static void
dced__rkeytab_change_key(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    dced_key_t			*key,
  /* out */
    error_status_t		*st
)
{
    sec_timeval_period_t	garbage_collect_time;	/* ignored */
    sec_passwd_type_t		keytype;
    sec_passwd_rec_t		*pwd;
    dced_entry_t		*ep;
    error_status_t		st2;

    *st = error_status_ok;

    if ((ep = ktab__find_by_uuid(id_uuid)) == NULL) {
	*st = db_s_key_not_found;
	return;
    }

    if ( key->passwd && (key->version != key->passwd->version_number) ) {
	*st = dced_s_key_version_mismatch;
	return;
    }

    if (DCED_IS_RANDOM_KEY(key)) {
	keytype = sec_passwd_des;
	sec_key_mgmt_gen_rand_key(key->authn_service, ep->storage_tag,
		key->principal, &keytype, key->version, (void **)&pwd, st);
	CHECK_STATUS_RET(st);
	sec_key_mgmt_change_key(key->authn_service, ep->storage_tag,
		key->principal, key->version, (void *)pwd,
		&garbage_collect_time, st);
	sec_key_mgmt_free_key(pwd, &st2);
	return;
    }

    /* Note: version=0 means use the appropriate/latest key */
    sec_key_mgmt_change_key(key->authn_service, ep->storage_tag,
	key->principal, key->version, (void *)key->passwd,
	&garbage_collect_time, st);
}

/* ______________________________________________________________________ */
/*  Return the list of entries corresponding to keytab files.
 */

static void
dced__rkeytab_get_list(
  /* in */
    handle_t			h,
  /* in,out */
    dced_entry_list_t		*items,
  /* out */
    error_status_t		*st
)
{
    int				i;
    dced_entry_t		*ep;

    *st = error_status_ok;
    items->count = 0;
    items->list  = NULL;

    if (!kentry_count) {
	return;
    }

    MALLOC_RESET_RET(items->list, dced_entry_t, kentry_count, st);

    for (i=0, ep=kentry; i<kentry_count; i++, ep++) {
	items->list[i] = *ep;
	items->list[i].name = (dced_string_t)dce_strdup((char *) ep->name);
	if (ep->description) {
		items->list[i].description = 
			(dced_string_t)dce_strdup((char *) ep->description);
	}
	items->list[i].storage_tag = 
			(dced_string_t)dce_strdup((char *) ep->storage_tag);
    }
    items->count = kentry_count;
}

/* ______________________________________________________________________ */

/* Caller should use this vector instead of the idl-generated one that
 * talks to DCED.
 */
rkeytab_v1_0_epv_t rkeytab_v1_0_l_epv = {
    dced__rkeytab_get_list,
    dced__rkeytab_create,
    dced__rkeytab_delete,
    dced__rkeytab_get_keys,
    dced__rkeytab_add_key,
    dced__rkeytab_delete_key,
    dced__rkeytab_change_key
};

/* ______________________________________________________________________ */

