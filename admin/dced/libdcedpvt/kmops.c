/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kmops.c,v $
 * Revision 1.1.4.1  1996/11/13  17:56:12  arvind
 * 	fix dced acl code returns wrong error on unauthorized
 * 	[1996/10/09  19:39 UTC  bartf  /main/bartf_pk_fix/1]
 *
 * 	fix incorrect invalid_permission error to be not_authorized
 * 	[1996/02/18  19:16:16  marty  1.1.2.2]
 *
 * Revision 1.1.2.2  1996/02/18  19:16:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:13  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  20:53:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/1  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted the fix for OT13045.
 * 
 * 	HP revision /main/tatsu_s.psock_timeout.b0/1  1995/08/23  21:02 UTC  tatsu_s
 * 	Added no_remote_update and entry_c_data_imported flag.
 * 	[1995/12/08  20:48:59  root]
 * 
 * Revision 1.1.2.20  1994/09/30  18:06:38  bowe
 * 	Missed a misleading comment in cleanup. [CR 12030]
 * 	[1994/09/30  18:04:58  bowe]
 * 
 * Revision 1.1.2.19  1994/09/16  20:51:11  bowe
 * 	SVC work - cast error_status_t to (long) for dce_svc_printf [CR 11725]
 * 	[1994/09/16  20:50:43  bowe]
 * 
 * Revision 1.1.2.18  1994/09/14  20:04:18  bowe
 * 	SVC work [CR 11725]
 * 	Delete ACL when deleting the object [CR 12030]
 * 	[1994/09/14  20:03:43  bowe]
 * 
 * Revision 1.1.2.17  1994/09/02  20:58:47  bowe
 * 	SVC work [CR 11725].
 * 	[1994/09/02  20:58:01  bowe]
 * 
 * Revision 1.1.2.16  1994/08/24  19:27:04  bowe
 * 	Add rkeytab__get_keytab_filename().  [CR 11791]
 * 	Expunge key encryption and transmit_rep code. [CR 11773]
 * 	[1994/08/24  19:12:47  bowe]
 * 
 * Revision 1.1.2.15  1994/07/29  19:56:09  rsalz
 * 	Create keytab for host principal at init time (OT CR 11465).
 * 	[1994/07/29  19:37:24  rsalz]
 * 
 * Revision 1.1.2.14  1994/07/07  19:14:58  bowe
 * 	Added calls to dced_privacy_ok() for all operations. [CR 11201]
 * 	[1994/07/07  19:13:02  bowe]
 * 
 * Revision 1.1.2.13  1994/06/13  20:44:39  bowe
 * 	Check that name does not exist on create. [CR 10702]
 * 	[1994/06/13  20:14:56  bowe]
 * 
 * Revision 1.1.2.12  1994/05/26  19:42:14  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:45  rsalz]
 * 
 * Revision 1.1.2.11  1994/05/19  16:28:50  bowe
 * 	rkeytab_add_key() reports error if version is 0 or random key requested.
 * 	rkeytab_change_key() allows version 0 (auto choose next version) and
 * 	generate random key.  [9768]
 * 	rkeytab_get_keys() now returns all keys. [10691]
 * 	[1994/05/19  16:25:02  bowe]
 * 
 * Revision 1.1.2.10  1994/05/12  19:30:30  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:29  rsalz]
 * 
 * Revision 1.1.2.9  1994/05/11  20:11:21  rsalz
 * 	Merged with changes from 1.1.2.8
 * 	[1994/05/11  20:10:53  rsalz]
 * 
 * 	More functionality; code cleanups.
 * 	[1994/05/11  20:08:22  rsalz]
 * 
 * Revision 1.1.2.8  1994/05/11  15:33:02  bowe
 * 	Correct args to dce_db_std_header_init() [CR 10603]
 * 	[1994/05/11  15:31:26  bowe]
 * 
 * Revision 1.1.2.7  1994/05/10  21:08:51  bowe
 * 	Finish implemention of rkeytab operations
 * 	[1994/05/10  21:07:33  bowe]
 * 
 * Revision 1.1.2.6  1994/04/26  19:25:51  bowe
 * 	New params to dce_acl_is_client_authorized().
 * 	Set prin (owner) when creating a new entry.
 * 	[1994/04/26  19:24:09  bowe]
 * 
 * Revision 1.1.2.5  1994/04/07  20:54:26  bowe
 * 	Bad param was passed to sec_key_mgmt_{set,change}_key().
 * 	Added more checks for returned status.
 * 	[1994/04/07  20:52:13  bowe]
 * 
 * Revision 1.1.2.4  1994/03/29  22:31:28  bowe
 * 	Added name->uuid cache calls in create/delete
 * 	[1994/03/29  22:24:10  bowe]
 * 
 * Revision 1.1.2.3  1994/03/18  21:28:42  bowe
 * 	Fixed size when reallocating size of list (was wrong)
 * 	[1994/03/18  21:28:25  bowe]
 * 
 * Revision 1.1.2.2  1994/03/18  19:51:29  bowe
 * 	Adapt code to use ERA attributes.  A few misc adjustments for encrypting/
 * 	decrypting keys.
 * 	[1994/03/18  19:40:33  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:55  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:24  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <libimpl.h>
#include <kmimpl.h>
#include <dce/binding.h>
#include <dce/dced_util.h>

/* ______________________________________________________________________ */
/*  Create an entry for a keytab, and either create or import its keytab file.
 */

void
rkeytab_create(
  /* in */
    handle_t			h,
    dced_entry_t		*id,
    boolean32			just_import,
    dced_key_list_t		*klist,
  /* out */
    error_status_t		*st
)
{
    int				access_return;
    unsigned32			i;
    uuid_t			au;
    sec_acl_t			i_acl;
    dced_entry_data_t		id_d;
    uuid_t			Prin;	/* of caller */

    *st = error_status_ok;

    if (!dced_privacy_ok(h, st))
	return;
    if (!dced_authorized(h, &km_cont_type, &km_acl, &Prin, dced_perm_insert, st))
	return;

    /* See if this object exists first. */
    dce_db_fetch_by_uuid(km_db, &id->id, &id_d, st);
    if (*st == error_status_ok ||
	    name_cache_find_uuid(km_name_cache, (char *)id->name, &au) ) {
	*st = dced_s_already_exists;
	return;
    }

    access_return = access((char *)id->storage_tag, F_OK);
    if (just_import) {
	/* Importing - keytab file must exist (unless it's an internal import
	 * done during initialization.) */
	if (access_return != 0 && h != NULL) {
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
	    CHECK_STAT_RET(*st);
	}
    }

    /* create an ACL for this thing */
    uuid_create(&au, st);		/* uuid of the acl */
    CHECK_STAT_RET(*st);
    /* get initial obj acl */
    dce_db_fetch_by_uuid(acl_db, &km_do_acl, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_FETCH_IACL_MSG, "rkeytab_create", (long)*st);
	return;
    }

    dce_db_store_by_uuid(acl_db, &au, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG, "rkeytab_create", (long)*st);
	return;
    }

    dce_db_std_header_init(km_db, &id_d.h, &id->id, &au, NULL, NULL, 0, st);
    CHECK_STAT_RET(*st);
    id_d.h.tagged_union.h.owner_id = Prin;
    id_d.flags = 0;
    if (just_import)
	id_d.flags |= entry_c_data_imported;
    id_d.data = *id;
    dce_db_store_by_uuid(km_db, &id->id, &id_d, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG, "rkeytab_create",
		(long)*st);
	return;
    }

    /* Last, add to cached name->uuid mapping. */
    name_cache_add(km_name_cache, (char *)id->name, &id->id);
}

/* ______________________________________________________________________ */
/*  Delete the entry for a keytab, and optionally its keytab file.
 */

void
rkeytab_delete(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    boolean32			remove_object,
  /* out */
    error_status_t		*st
)
{
    dced_entry_data_t		id_d;
    error_status_t		st2;
    uuid_t			Prin;	/* of caller */

    *st = error_status_ok;

    dce_db_fetch_by_uuid(km_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_privacy_ok(h, st))
	return;
    if (remove_object) {
	if (h != NULL && dced_is_remote_disabled) {
	    *st = sec_acl_not_authorized;
	    return;
	}
	if (id_d.flags & entry_c_data_imported) {
	    *st = sec_acl_invalid_permission;
	    return;
	}
	if (!dced_authorized(h, &km_obj_type,
		&id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_purge, st))
	    return;
	/* remove object (file) itself */
	if (unlink((char *)id_d.data.storage_tag) < 0) {
	    *st = dced_s_cant_remove_storage_file;
	}
    }
    else {
	if (!dced_authorized(h, &km_obj_type,
		&id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_delete, st))
	    return;
    }
    dce_db_delete_by_uuid(acl_db, &id_d.h.tagged_union.h.acl_uuid, &st2);
    if (st2 != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_ACL_MSG, "rkeytab_delete", (long)st2);
    dce_db_delete_by_uuid(km_db, id_uuid, &st2);
    if (st2 != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_OBJECT_MSG, "rkeytab_delete",
		(long)st2);
    /* delete error more important than unlink? */
    if (st2 != error_status_ok && *st == error_status_ok)
	*st = st2;

    /* Last, delete cached name->uuid mapping. */
    name_cache_del(km_name_cache, (char *)id_d.data.name);
}

/* ______________________________________________________________________ */
/*  Get keys in a keytab file.
 */

void
rkeytab_get_keys(
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
    dced_entry_data_t		id_d;
    char			*keytab_file;
    int				i;
    idl_char			*prin;
    int				n_keys;
    unsigned32			key_vno;
    sec_passwd_rec_t		*pwd;
    uuid_t			Prin;	/* of caller */

    *st = error_status_ok;
    klist->count = 0;

    dce_db_fetch_by_uuid(km_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_privacy_ok(h, st))
	return;
    if (!dced_authorized(h, &km_obj_type,
	    &id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_read, st))
	return;

    n_keys = 0;
    keytype = sec_passwd_des;
    keytab_file = (char *)id_d.data.storage_tag;
    sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_secret, keytab_file,
	NULL, &keytype, &cursor, st);
    CHECK_STAT_RET(*st);
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

    /* Allocate space for the returned list.  We know how many we need. */
    klist->list = rpc_sm_allocate(n_keys * sizeof(dced_key_t), st);
    CHECK_STAT_RET(*st);

    sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_secret, keytab_file,
	NULL, &keytype, &cursor, st);
    CHECK_STAT_RET(*st);
    for (i=0; i<n_keys; i++) {
	sec_key_mgmt_get_next_key(cursor, &prin, &key_vno,
		    (void **)&pwd, st);
	/* Copy key for transmission back to client, then free our copy. */
	klist->list[i].principal	= strdup_for_client(prin, st);
	CHECK_STAT_RET(*st);
	klist->list[i].version		= pwd->version_number;
	klist->list[i].authn_service	= rpc_c_authn_dce_secret;
	klist->list[i].passwd		=
		rpc_sm_allocate(sizeof(sec_passwd_rec_t), st);
	*klist->list[i].passwd		= *pwd;
	if (dced_is_remote_disabled) {
	    if (pwd->pepper != NULL)
		memset(klist->list[i].passwd->pepper, 0,
		       strlen((char *)klist->list[i].passwd->pepper));
	    switch(pwd->key.key_type) {
            case sec_passwd_plain:
		if (pwd->key.tagged_union.plain != NULL)
		    memset(klist->list[i].passwd->key.tagged_union.plain, 0,
			   strlen((char *)klist->list[i].passwd->key.tagged_union.plain));
		break;
            case sec_passwd_des:
		memset(&klist->list[i].passwd->key.tagged_union.des_key, 0,
		       sec_passwd_c_des_key_size);
		break;
	    }
	}
	sec_key_mgmt_free_key(pwd, &st2);
    }
    sec_key_mgmt_release_cursor(&cursor, &st2);
    /* Check for end of list, which is a "good thing" */
    if (*st == sec_key_mgmt_e_key_unavailable)
	*st = error_status_ok;
    CHECK_STAT_RET(*st);
    if (st2 != error_status_ok) {
	*st = st2;
	return;
    }
    klist->count = n_keys;
}

/* ______________________________________________________________________ */
/*  Add a key to a keytab file.
 */

void
rkeytab_add_key(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    dced_key_t			*key,
  /* out */
    error_status_t		*st
)
{
    dced_entry_data_t		id_d;
    uuid_t			Prin;	/* of caller */

    *st = error_status_ok;

    dce_db_fetch_by_uuid(km_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_privacy_ok(h, st))
	return;
    if (!dced_authorized(h, &km_obj_type,
	    &id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_append, st))
	return;

    if ( key->version != key->passwd->version_number ) {
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
    sec_key_mgmt_set_key(key->authn_service, id_d.data.storage_tag,
	key->principal, key->version, (void *)key->passwd, st);
    CHECK_STAT_RET(*st);

}

/* ______________________________________________________________________ */
/*  Delete a key from a keytab file.
 */

void
rkeytab_delete_key(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    dced_key_t			*key,
  /* out */
    error_status_t		*st
)
{
    dced_entry_data_t		id_d;
    error_status_t		st2;
    void			*cursor;
    idl_char			*prin;
    unsigned32			key_vno;
    sec_passwd_type_t		keytype;
    char			*keytab_file;
    uuid_t			Prin;	/* of caller */

    *st = error_status_ok;

    dce_db_fetch_by_uuid(km_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_privacy_ok(h, st))
	return;
    if (!dced_authorized(h, &km_obj_type,
	    &id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_expunge, st))
	return;

    /* If version is 0, delete all versions of that prin's key.
     * We can't delete a key in the "cursor loop", since kerberos
     * locks the keyfile.  So we loop through, remembering the
     * version numbers, release the cursor, then delete those keys.
     * NOTE: sec_key_mgmt_ code says kerberos version numbers are 1 byte.
     */
    keytab_file = (char *)id_d.data.storage_tag;
    if (key->version == 0) {
	unsigned32	vv[256];	/* version vector */
	unsigned32	nv, i;

	keytype = sec_passwd_des;
	sec_key_mgmt_initialize_cursor(rpc_c_authn_dce_secret, keytab_file,
	    key->principal, &keytype, &cursor, st);
	CHECK_STAT_RET(*st);
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
    }
    else {
	sec_key_mgmt_delete_key(key->authn_service, keytab_file,
		key->principal, key->version, st);
	CHECK_STAT_RET(*st);
    }
}

/* ______________________________________________________________________ */
/*  Change a key in the registry.
 */

void
rkeytab_change_key(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    dced_key_t			*key,
  /* out */
    error_status_t		*st
)
{
    dced_entry_data_t		id_d;
    sec_timeval_period_t	garbage_collect_time;	/* ignored */
    uuid_t			Prin;	/* of caller */
    sec_passwd_type_t		keytype;
    sec_passwd_rec_t		*pwd;

    *st = error_status_ok;

    dce_db_fetch_by_uuid(km_db, id_uuid, &id_d, st);
    CHECK_STAT_RET(*st);

    if (!dced_privacy_ok(h, st))
	return;
    if (!dced_authorized(h, &km_obj_type,
	    &id_d.h.tagged_union.h.acl_uuid, &Prin, dced_perm_append, st))
	return;

    if ( key->passwd && (key->version != key->passwd->version_number) ) {
	*st = dced_s_key_version_mismatch;
	return;
    }

    if (DCED_IS_RANDOM_KEY(key)) {
	keytype = sec_passwd_des;
	sec_key_mgmt_gen_rand_key(key->authn_service, id_d.data.storage_tag,
		key->principal, &keytype, key->version, (void **)&pwd, st);
	CHECK_STAT_RET(*st);
	sec_key_mgmt_change_key(key->authn_service, id_d.data.storage_tag,
		key->principal, key->version, (void *)pwd,
		&garbage_collect_time, st);
	CHECK_STAT_RET(*st);
	sec_key_mgmt_free_key(pwd, st);
	return;
    }

    /* Note: version=0 means use the appropriate/latest key */
    sec_key_mgmt_change_key(key->authn_service, id_d.data.storage_tag,
	key->principal, key->version, (void *)key->passwd,
	&garbage_collect_time, st);
}

/* ______________________________________________________________________ */
/*  Return the list of entries corresponding to keytab files.
 */

void
rkeytab_get_list(
  /* in */
    handle_t			h,
  /* in,out */
    dced_entry_list_t		*items,
  /* out */
    error_status_t		*st
)
{
    void			*key;
    error_status_t		st2;
    unsigned32			n;
    dced_entry_data_t		id_d;
    uuid_t			Prin;	/* of caller */

    *st = error_status_ok;
    items->count = 0;
    items->list  = NULL;

    if (!dced_privacy_ok(h, st))
	return;
    if (!dced_authorized(h, &km_cont_type, &km_acl, &Prin, dced_perm_read, st))
	return;

    dce_db_inq_count(km_db, &n, st);
    CHECK_STAT_RET(*st);
    items->list  = rpc_sm_allocate(n * sizeof(dced_entry_t), st);
    CHECK_STAT_RET(*st);

    dce_db_iter_start(km_db, st);
    CHECK_STAT_RET(*st);
    do {
	dce_db_iter_next(km_db, &key, st);
	if (*st == db_s_no_more) {
	    *st = error_status_ok;
	    break;
	}
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_ITER_FAIL_MSG, "rkeytab_get_list", (long)*st);
	    dce_db_iter_done(km_db, &st2);
	    return;
	}
	dce_db_fetch(km_db, key, &id_d, st);
	if (*st != error_status_ok) {
	    dce_db_iter_done(km_db, &st2);
	    return;
	}
	items->list[items->count] = id_d.data;
	items->count++;
    } while (TRUE);
    dce_db_iter_done(km_db, st);
}

/* ______________________________________________________________________ */
/* Get a keytab's filename.
 * Assumes it's called from within a server operation, so memory for the
 * dced_entry_data_t need not be freed.
 * Returns a copy of the storage_tag - caller should free that.
 */

void
rkeytab__get_keytab_filename(
  /* in */
    uuid_t		*id,
  /* out */
    dced_string_t	*storage_tag,
    error_status_t	*st
)
{
    dced_entry_data_t		id_d;

    dce_db_fetch_by_uuid(km_db, id, &id_d, st);
    if (*st != error_status_ok) return;
    *storage_tag = (dced_string_t)dce_strdup((char *)id_d.data.storage_tag);
}

/* ______________________________________________________________________ */

rkeytab_v1_0_epv_t dced_rkeytab_v1_0_epv = {
    rkeytab_get_list,
    rkeytab_create,
    rkeytab_delete,
    rkeytab_get_keys,
    rkeytab_add_key,
    rkeytab_delete_key,
    rkeytab_change_key
};
