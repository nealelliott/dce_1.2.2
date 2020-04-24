/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: scops.c,v $
 * Revision 1.1.4.1  1996/11/13  17:56:39  arvind
 * 	fix dced acl code returns wrong error on unauthorized
 * 	[1996/10/09  19:39 UTC  bartf  /main/bartf_pk_fix/1]
 *
 * 	fix incorrect invalid_permission error to be not_authorized
 * 	[1996/02/18  19:16:22  marty  1.1.2.2]
 *
 * Revision 1.1.2.2  1996/02/18  19:16:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:19  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  20:53:37  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted yet another (probably not last) fix for CHFts15852/OT12494.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/1  1995/08/23  21:14 UTC  tatsu_s
 * 	Added no_remote_update.
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/08  21:46 UTC  jss
 * 	Merge changes for CHFts14511, CHFts15038, CHFts14283.
 * 
 * 	HP revision /main/jss_mothra_fix3/1  1995/06/06  14:20 UTC  jss
 * 	Fix for OT12837. Fix lock usage problems
 * 	[1995/12/08  20:49:04  root]
 * 
 * Revision 1.1.2.26  1994/09/30  20:14:37  rsalz
 * 	Add srvrexec_data_t.sc_uuid and use it to restart servers (OT CR 12442).
 * 	[1994/09/30  20:11:06  rsalz]
 * 
 * Revision 1.1.2.25  1994/09/22  22:00:36  bowe
 * 	More/final svc work. [CR 11725]
 * 	[1994/09/22  21:42:57  bowe]
 * 
 * Revision 1.1.2.24  1994/09/22  13:37:33  bowe
 * 	Add errno to dced_s_sc_open_file_failed [CR 11725]
 * 	[1994/09/22  13:37:18  bowe]
 * 
 * Revision 1.1.2.23  1994/09/21  21:49:49  baum
 * 	Move dced_server_free_ptr() and srvr_file= NULL (OT 12102)
 * 	[1994/09/21  21:49:01  baum]
 * 
 * Revision 1.1.2.22  1994/09/20  21:25:58  bowe
 * 	Add/remote item from sx,sc name caches in when creating/deleting
 * 	that object type. [CR 12272]
 * 	[1994/09/20  21:25:35  bowe]
 * 
 * Revision 1.1.2.21  1994/09/17  08:05:20  rsalz
 * 	Merged with changes from 1.1.2.20
 * 	[1994/09/17  08:04:48  rsalz]
 * 
 * 	Properly test for I bit in create (OT CR 12032).
 * 	Instance is object UUID for srvrxec objects (OT CR 12162).
 * 	Ensure required fields exist in srvrconf_create (OT CR 12166).
 * 	[1994/09/17  07:58:04  rsalz]
 * 
 * Revision 1.1.2.20  1994/09/14  20:04:20  bowe
 * 	SVC work [CR 11725]
 * 	Delete ACL when deleting the object [CR 12030]
 * 	[1994/09/14  20:03:47  bowe]
 * 
 * Revision 1.1.2.19  1994/09/12  18:14:31  pwang
 * 	Turned on server_c_exec_running flag in srvrconf_start()
 * 	[OT#12087]
 * 
 * Revision 1.1.2.18  1994/09/12  15:37:54  rsalz
 * 	Add insert_priv ACL bit (part of OT CR 12032).
 * 	[1994/09/09  18:43:42  rsalz]
 * 
 * Revision 1.1.2.17  1994/09/06  19:56:25  rsalz
 * 	Create ACL and database objects for started servers (OT CR 12027).
 * 	[1994/09/06  19:34:20  rsalz]
 * 
 * Revision 1.1.2.16  1994/08/25  16:32:33  rsalz
 * 	Start boottime servers.
 * 	[1994/08/25  16:31:08  rsalz]
 * 
 * Revision 1.1.2.15  1994/08/24  19:51:54  bowe
 * 	Call srvr_add_to_table() when starting a server in srvrconf_start.
 * 	Pass env variable for keytab file. [CR 11791]
 * 	[1994/08/24  19:39:28  bowe]
 * 
 * Revision 1.1.2.14  1994/07/06  20:17:26  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:11:12  pwang]
 * 
 * Revision 1.1.2.13  1994/06/30  17:38:28  rsalz
 * 	Add dced_s_name_missing; use it in "create" operations (OT CR 10850).
 * 	[1994/06/30  17:34:43  rsalz]
 * 
 * Revision 1.1.2.12  1994/06/22  18:45:39  baum
 * 	Added cache_name_add() to svrconf_create() for OT#10978
 * 	[1994/06/22  18:45:25  baum]
 * 
 * Revision 1.1.2.11  1994/06/16  19:35:19  rsalz
 * 	Use new free routines.
 * 	[1994/06/16  19:32:23  rsalz]
 * 
 * Revision 1.1.2.10  1994/05/26  19:42:29  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:36:01  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:33:04  rsalz]
 * 
 * Revision 1.1.2.9  1994/05/12  19:30:35  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:34  rsalz]
 * 
 * Revision 1.1.2.8  1994/05/11  20:11:25  rsalz
 * 	Merged with changes from 1.1.2.7
 * 	[1994/05/11  20:11:03  rsalz]
 * 
 * 	More functionality; code cleanups.
 * 	[1994/05/11  20:08:28  rsalz]
 * 
 * Revision 1.1.2.7  1994/05/11  15:33:03  bowe
 * 	Correct args to dce_db_std_header_init() [CR 10603]
 * 	[1994/05/11  15:31:23  bowe]
 * 
 * Revision 1.1.2.6  1994/05/10  20:15:17  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:48  rsalz]
 * 
 * Revision 1.1.2.5  1994/05/05  20:07:45  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:01:06  rsalz]
 * 
 * Revision 1.1.2.4  1994/04/26  19:25:52  bowe
 * 	New params to dce_acl_is_client_authorized()
 * 	[1994/04/26  19:19:54  bowe]
 * 
 * Revision 1.1.2.3  1994/04/11  20:22:06  rsalz
 * 	Final set of RFC revision fixes.
 * 	[1994/04/11  20:08:25  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:09  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:05  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:59  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:28  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <libimpl.h>
#include <scimpl.h>
#include <kmimpl.h>
#include <dce/dced_util.h>

typedef struct server_to_file_state_s_t {
    idl_byte	*addr;
    FILE	*F;
} server_to_file_state_t;

/* ______________________________________________________________________ */

void
srvrconf_create(
  /* in */
    handle_t		h,
    server_t		*server,
  /* out */
    error_status_t	*st
)
{
    uuid_t		au;
    sec_acl_t		i_acl;
    server_data_t	new;
    uuid_t		Prin;
    sec_acl_permset_t	perms;
    error_status_t	xst;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Check ACL for permission */
    perms = dced_perm_insert;
    if (server->fixed.posix_gid == 0 || server->fixed.posix_uid == 0) {
	if (h != NULL && dced_is_remote_disabled) {
	    *st = sec_acl_not_authorized;
	    return;
	}
	perms |= dced_perm_insertpriv;
    }
    if (!dced_authorized(h, &sc_cont_type, &sc_acl, &Prin, perms, st))
	return;

    if (server->name == NULL
     || *server->name == '\0'
     || server->fixed.program == NULL
     || server->fixed.program == '\0') {
	*st = dced_s_name_missing;
	return;
    }

    /* Duplicate object? */
    dce_lock_riw(sc_table.lock, st);
    if (uuid_is_nil(&server->id, st))
	uuid_create(&server->id, st);
    else if (srvr_find_by_uuid(&sc_table, &server->id, st) != NULL
	  || srvr_find_by_name(&sc_table, server->name, st) != NULL) {
	dce_lock_unlock(sc_table.lock, st);
	*st = dced_s_already_exists;
	return;
    }

    /* Create an ACL for the object. */
    uuid_create(&au, st);
    if (*st != error_status_ok) {
	dce_lock_unlock(sc_table.lock, &xst);
	return;
    }
    dce_db_fetch_by_uuid(acl_db, &sc_do_acl, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_FETCH_IACL_MSG, "srvrconf_create",
		(long)*st);
	dce_lock_unlock(sc_table.lock, &xst);
	return;
    }
    dce_db_store_by_uuid(acl_db, &au, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG, "srvrconf_create",
		(long)*st);
	dce_lock_unlock(sc_table.lock, &xst);
	return;
    }

    /* Store the object. */
    srvr_qualify_prin_names(&server->prin_names, st);
    new.data = *server;
    new.flags = 0;
    dce_db_std_header_init(sc_db, &new.h, &new.data.id, &au, NULL, NULL, 0, st);
    CHECK_STAT_RET(*st);
    new.h.tagged_union.h.owner_id = Prin;
    new.used = TRUE;
    /* Don't need server_copy since srvr_add_to_table will do it for us. */
    new.data = *server;
    new.srvr_file = NULL;
    dce_db_store_by_uuid(sc_db, &new.data.id, &new, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG, "srvrconf_create",
		(long)*st);
	dce_lock_unlock(sc_table.lock, &xst);
	return;
    }

    dce_lock_write(sc_table.lock, st);
    srvr_add_to_table(&sc_table, &new, st);
    dce_lock_unlock(sc_table.lock, st);
    name_cache_add(sc_name_cache, (char *)server->name, &server->id);
}

/* ______________________________________________________________________ */

void
srvrconf_delete(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
  /* out */
    error_status_t	*st
)
{
    server_data_t	*sp;
    error_status_t	st2;
    uuid_t		Prin;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Object exist? */
    dce_lock_riw(sc_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sc_table, id_uuid, st)) == NULL) {
	dce_lock_unlock(sc_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (!dced_authorized(h, &sc_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_delete, st)) {
	dce_lock_unlock(sc_table.lock, &st2);
	return;
    }

    dce_lock_write(sc_table.lock, st);
    sp->used = FALSE;
    dce_lock_unlock(sc_table.lock, st);
    dce_db_delete_by_uuid(acl_db, &sp->h.tagged_union.h.acl_uuid, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_ACL_MSG, "srvrconf_delete", (long)*st);
    dce_db_delete_by_uuid(sc_db, id_uuid, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_OBJECT_MSG, "srvrconf_delete", (long)*st);
    name_cache_del(sc_name_cache, (char *)sp->data.name);
    dced_server_free_ptrs(&sp->data);
}

/* ______________________________________________________________________ */

void
srvrconf_attr_replace(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
    dced_attr_list_t	*attributes,
  /* out */
    error_status_t	*st
)
{
    server_data_t	*sp;
    error_status_t	st2;
    uuid_t		Prin;
    sec_acl_permset_t	perms = dced_perm_write;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Object exist? */
    dce_lock_riw(sc_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sc_table, id_uuid, st)) == NULL) {
	dce_lock_unlock(sc_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (sp->data.fixed.posix_gid == 0 || sp->data.fixed.posix_uid == 0) {
	if (h != NULL && dced_is_remote_disabled) {
	    dce_lock_unlock(sc_table.lock, &st2);
	    *st = sec_acl_not_authorized;
	    return;
	}
	perms |= dced_perm_flag;
    }
    if (!dced_authorized(h, &sc_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, perms, st)) {
	dce_lock_unlock(sc_table.lock, &st2);
	return;
    }

    /* Update the object. */
    dced_attr_list_free_ptrs(&sp->data.attributes);
    attr_list_copy(&sp->data.attributes, attributes, dced_copy_via_malloc, st);
    dce_lock_unlock(sc_table.lock, st);
    dce_db_store_by_uuid(sc_db, &sp->data.id, sp, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG, "srvrconf_attr_replace",
		(long)*st);
}

/* ______________________________________________________________________ */

void
srvrconf_fixattr_replace(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
    server_fixedattr_t	*fixed,
  /* out */
    error_status_t	*st
)
{
    server_data_t	*sp;
    error_status_t	st2;
    uuid_t		Prin;
    sec_acl_permset_t	perms = dced_perm_write;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Object exist? */
    dce_lock_riw(sc_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sc_table, id_uuid, st)) == NULL) {
	dce_lock_unlock(sc_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (fixed->posix_gid == 0 || fixed->posix_uid == 0
	|| sp->data.fixed.posix_gid == 0 || sp->data.fixed.posix_uid == 0) {
	if (h != NULL && dced_is_remote_disabled) {
	    dce_lock_unlock(sc_table.lock, &st2);
	    *st = sec_acl_not_authorized;
	    return;
	}
	perms |= dced_perm_flag;
    }
    if (!dced_authorized(h, &sc_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, perms, st)) {
	dce_lock_unlock(sc_table.lock, &st2);
	return;
    }

    /* Update object. */
    dced_fixedattr_free_ptrs(&sp->data.fixed);
    fixedattr_copy(&sp->data.fixed, fixed, dced_copy_via_malloc, st);
    dce_db_store_by_uuid(sc_db, &sp->data.id, sp, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG, "srvrconf_fixattr_replace",
		(long)*st);
    dce_lock_unlock(sc_table.lock, st);
}

/* ______________________________________________________________________ */

void
srvrconf_get_list(
  /* in */
    handle_t		h,
  /* out */
    dced_entry_list_t	*items,
    error_status_t	*st
)
{
    uuid_t		Prin;

    /* Set default output parameters. */
    items->count = 0;
    *st = error_status_ok;

    /* Check ACL for permission */
    if (!dced_authorized(h, &sc_cont_type, &sc_acl, &Prin, dced_perm_read, st))
	return;

    srvr_get_list(&sc_table, items, st);
}

/* ______________________________________________________________________ */

void
srvrconf_get(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
  /* out */
    server_t		*value,
    error_status_t	*st
)
{
    server_data_t	*sp;
    error_status_t	st2;
    uuid_t		Prin;

    /* Set default output parameters. */
    *value = dced_g_null_server;
    *st = error_status_ok;

    /* Object exist? */
    dce_lock_riw(sc_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sc_table, id_uuid, st)) == NULL) {
	dce_lock_unlock(sc_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (!dced_authorized(h, &sc_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_read, st)) {
	dce_lock_unlock(sc_table.lock, &st2);
	return;
    }

    dce_lock_write(sc_table.lock, st);
    server_copy(value, &sp->data, dced_copy_for_stub, st);
    dce_lock_unlock(sc_table.lock, st);
}

/* ______________________________________________________________________ */
static void
to_file_allocate(
    idl_void_p_t		param,
    idl_byte			**buf,
    idl_ulong_int		*size
)
{
    server_to_file_state_t	*state;

    state = (server_to_file_state_t *)param;
    /* From the IDL usage notes.  Ugly, but it gets an 8-byte aligned buffer
     * and fills in size with the aligned size. */
    state->addr = malloc(*size + 8);
    *buf = (idl_byte *)(((state->addr - (idl_byte *)0) + 7) & ~7);
    *size -= *buf - state->addr;
    *size &= ~7;
}


static void
to_file_write(
    idl_void_p_t		param,
    idl_byte			*buf,
    idl_ulong_int		size
)
{
    server_to_file_state_t	*state;

    state = (server_to_file_state_t *)param;
    fwrite(buf, 1, size, state->F);
    free(state->addr);
}


static void
server_to_file(
    server_t			*s,
    char			*name,
    error_status_t		*st
)
{
    server_to_file_state_t	state;
    idl_es_handle_t		h;

    if ((state.F = fopen(name, "w")) == NULL) {
	dce_svc_printf(DCED_S_SC_OPEN_FILE_FAILED_MSG, errno);
	*st = dced_s_sc_open_file_failed;
	return;
    }
    idl_es_encode_incremental(&state, to_file_allocate, to_file_write, &h, st);
    dced_server_env_encode(h, s, st);
    fclose(state.F);
    idl_es_handle_free(&h, st);
}


void
srvrconf_start(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    server_fixedattr_t		*fixed,
    dced_attr_list_t		*attributes,
  /* in,out */
    uuid_t			*instance,
  /* out */
    error_status_t		*st
)
{
    server_data_t		*sp;
    server_data_t		new;
    error_status_t		st2;
    uuid_t			Prin;
    uuid_t			dummy;
    unsigned32			i;
    sec_attr_t			*ap;
    sec_attr_enc_str_array_t	*sa;
    char			**env;
    dced_string_list_t		newenv;
    char			buff[256];
    char			temp[256];
    idl_char			*u1;
    idl_char			*u2;
    dced_string_list_t		sl;
    dced_string_t		ptrs[3];	/* 3 env variables to add */
    pid_t			pid;
    char			**argv;
    dced_string_t		keyfile;
    uuid_t			au;
    sec_acl_t			i_acl;
    idl_char			*newname;

    /* Set default output parameters. */
    if (instance == NULL) {
	uuid_create(&dummy, st);
	instance = &dummy;
    }
    else if (uuid_is_nil(instance, st))
	uuid_create(instance, st);
    *st = error_status_ok;

    /* Find object. */
    dce_lock_riw(sc_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sc_table, id_uuid, st)) == NULL) {
	dce_lock_unlock(sc_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (!dced_is_server) {
	dce_lock_unlock(sc_table.lock, &st2);
	*st = sec_acl_invalid_permission;
	return;
    }
    if (!dced_authorized(h, &sc_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_execute, st)) {
	dce_lock_unlock(sc_table.lock, &st2);
	return;
    }
    if ((fixed || attributes)
     && !dced_authorized(h, &sc_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_flag, st)) {
	dce_lock_unlock(sc_table.lock, &st2);
	return;
    }
    if (((fixed && (fixed->posix_gid == 0 || fixed->posix_uid == 0))
	|| ((fixed || attributes)
	    && (sp->data.fixed.posix_gid == 0
		|| sp->data.fixed.posix_uid == 0)))
	&& h != NULL && dced_is_remote_disabled) {
	dce_lock_unlock(sc_table.lock, &st2);
	*st = sec_acl_not_authorized;
	return;
    }

    dce_lock_write(sc_table.lock, st);
    server_copy(&new.data, &sp->data, dced_copy_via_malloc, st);
    dce_lock_unlock(sc_table.lock, st);
    new.data.exec_data.execstate = server_c_exec_running;
    new.data.exec_data.tagged_union.running_data.instance = *instance;

    /* Set up attributes. */
    if (fixed) {
	dced_fixedattr_free_ptrs(&new.data.fixed);
	fixedattr_copy(&new.data.fixed, fixed, dced_copy_via_malloc, st);
    }
    if (attributes) {
	dced_attr_list_free_ptrs(&new.data.attributes);
	attr_list_copy(&new.data.attributes, attributes, dced_copy_via_malloc, st);
    }

    /* Set up base environment. */
    string_list_copy(&newenv, &environ_string_list, dced_copy_for_stub, st);

    /* Add the two known dced environment entries. */
    uuid_to_string(id_uuid, &u1, st);
    new.data.exec_data.tagged_union.running_data.sc_uuid = *id_uuid;
    uuid_to_string(&new.data.exec_data.tagged_union.running_data.instance, &u2, st);
    new.flags = server_c_exec__starting;
    sprintf(buff, "%s=%s %s", dced_uuid_env_name, (char *)u1, (char *)u2);
    rpc_string_free(&u1, st);
    rpc_string_free(&u2, st);

    sl.list = ptrs;
    ptrs[0] = strdup_for_client((idl_char *)buff, st);
    (void)sprintf(temp, "%s/scXXXXXX", dced_g_info.dir_path);
    (void)mktemp(temp);
    new.srvr_file = (idl_char *)temp;
    server_to_file(&new.data, temp, st);
    if (*st == error_status_ok) {
	sprintf(buff, "%s=%s", dced_conf_env_name, temp);
	ptrs[1] = strdup_for_client((idl_char *)buff, st);
	sl.count = 2;
    }
    else
	sl.count = 1;

    /* Get keytab filename - use the first one in the list. */
    if (new.data.fixed.keytables.count > 0) {
	rkeytab__get_keytab_filename(&new.data.fixed.keytables.list[0],
		&keyfile, st);
	if (*st == error_status_ok) {
	    sprintf(buff, "%s=%s", dced_keytab_env_name, keyfile);
	    free(keyfile);
	}
    }
    else
	sprintf(buff, "%s=", dced_keytab_env_name);	/* null string */
    ptrs[sl.count] = strdup_for_client((idl_char *)buff, st);
    sl.count++;

    string_list_append(&newenv, &sl, dced_copy_for_stub, st);

    /* Anything to add to the environment?. */
    sa = NULL;
    for (ap = new.data.attributes.list, i = new.data.attributes.count; i; i--, ap++)
	if (uuid_equal(&ap->attr_id, &dced_g_uuid_envaddattr, st)) {
	    /* Found it.  Right type? */
	    if (ap->attr_value.attr_encoding
		    != sec_attr_enc_printstring_array) {
		dced_server_free_ptrs(&new.data);
		*st = dced_s_sc_invalid_attr_type;
		return;
	    }
	    /* Duplicate? */
	    if (sa) {
		dced_server_free_ptrs(&new.data);
		*st = dced_s_sc_invalid_attr_type;
		return;
	    }
	    sa = ap->attr_value.tagged_union.string_array;
	}

    /* Add server-specific values? */
    if (sa) {
	sl.list = sa->strings;
	sl.count = sa->num_strings;
	string_list_append(&newenv, &sl, dced_copy_for_stub, st);
    }

    /* Turn it from a stringlist into an environment. */
    env = (char **)dced_copy_for_stub((newenv.count + 1) * sizeof *env);
    memcpy(env, newenv.list, newenv.count * sizeof *env);
    env[newenv.count] = NULL;

    /* Build argv. */
    argv = (char **)
	    dced_copy_for_stub((1 + new.data.fixed.arguments.count + 1) * sizeof *argv);
    argv[0] = (char *)new.data.fixed.program;
    for (i = 0; i < new.data.fixed.arguments.count; i++)
	argv[i + 1] = (char *)new.data.fixed.arguments.list[i];
    argv[i + 1] = NULL;

    pid = fork();
    if (pid < 0) {
	dced_server_free_ptrs(&new.data);
	*st = dced_s_sc_cant_fork;
	return;
    }
    if (pid == 0) {
	if (setgid((gid_t)new.data.fixed.posix_gid) < 0)
	    dce_svc_printf(DCED_S_SC_SETID_FAILED_MSG, "setgid", errno);
	if (setuid((uid_t)new.data.fixed.posix_uid) < 0)
	    dce_svc_printf(DCED_S_SC_SETID_FAILED_MSG, "setuid", errno);
	if (new.data.fixed.posix_dir
	 && chdir((char *)new.data.fixed.posix_dir) < 0) {
	    dce_svc_printf(DCED_S_SC_CHDIR_FAILED_MSG,
			(char *)new.data.fixed.posix_dir, errno);
	    if (chdir((char *)dcelocal_path) < 0)
		dce_svc_printf(DCED_S_SC_CHDIR_FAILED_MSG,
			(char *)dcelocal_path, errno);
	}
	execve(argv[0], argv, env);
	dce_svc_printf(DCED_S_SC_EXEC_FAILED_MSG, argv[0], errno);
	_exit(1);
    }

    new.data.exec_data.tagged_union.running_data.posix_pid = pid;

    /* Create an ACL for the object. */
    uuid_create(&au, st);
    CHECK_STAT_RET(*st);
    dce_db_fetch_by_uuid(acl_db, &sx_do_acl, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_FETCH_IACL_MSG, "srvrconf_start",
		(long)*st);
	return;
    }
    dce_db_store_by_uuid(acl_db, &au, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG, "srvrconf_start",
		(long)*st);
	return;
    }

    new.data.id = *instance;
    dce_db_std_header_init(sx_db, &new.h, &new.data.id, &au, NULL, NULL, 0, st);
    CHECK_STAT_RET(*st);
    dce_lock_riw(sx_table.lock, st);
    /* Append process ID if not is not unique. */
    if (srvr_find_by_name(&sx_table, new.data.name, st) != NULL) {
	newname = dced_copy_for_stub(strlen((char *)new.data.name) + 1 + 20 + 1);
	sprintf((char *)newname, "%s.%ld", new.data.name, (long)pid);
	new.data.name = newname;
    }
    dce_db_store_by_uuid(sx_db, &new.data.id, &new, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG, "srvrconf_start",
		(long)*st);
	dce_lock_unlock(sx_table.lock, st);
	return;
    }

    dce_lock_write(sx_table.lock, st);
    srvr_add_to_table(&sx_table, &new, st);
    dce_lock_unlock(sx_table.lock, st);

    name_cache_add(sx_name_cache, (char *)new.data.name, &new.data.id);
}

/* ______________________________________________________________________ */
void
dced_start_boottime_servers(
    error_status_t		*st
)
{
    server_data_t		*sp;
    unsigned32			i;

    dce_lock_read(sc_table.lock, st);
    for (sp = sc_table.list, i = sc_table.count; i; i--, sp++)
	if (sp->data.fixed.startupflags & server_c_startup_at_boot) {
	    srvrconf_start(NULL, &sp->data.id, NULL, NULL, NULL, st);
	    if (*st != error_status_ok)
		dce_svc_printf(DCED_S_SC_BOOT_START_FAILED_MSG, sp->data.name,
			(long)*st);
	}
    dce_lock_unlock(sc_table.lock, st);
}

srvrconf_v1_0_epv_t dced_srvrconf_v1_0_epv = {
    srvrconf_get_list,
    srvrconf_get,
    srvrconf_create,
    srvrconf_delete,
    srvrconf_attr_replace,
    srvrconf_fixattr_replace,
    srvrconf_start
};
