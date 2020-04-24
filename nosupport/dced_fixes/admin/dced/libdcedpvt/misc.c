/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: misc.c,v $
BINRevision 1.1.2.1  1996/12/06  17:50:12  damon
BIN	Tandem dced fixes
BIN
 * Revision /main/gaz_dce1.2_tandem/1  1996/08/09  15:26 UTC  gaz
 * 	separated initialization of dce_g_info.dir_path from dced_init_global_data()
 * 	so that the dir_path could be initialized before "daemonizing" dced, by 
 * 	creating function dced_init_global_dir_path()
 * 
 * Revision 1.1.2.2  1996/02/18  19:16:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:17  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:32  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/4  1995/09/23  20:20 UTC  tatsu_s
 * 	Fixed server_c_exec_running check.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/3  1995/09/23  17:49 UTC  tatsu_s
 * 	Added anonymous hostdata directory.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  00:45 UTC  tatsu_s
 * 	Miscellaneous fixes.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/2  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted the fix for CHFts16168.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/1  1995/08/23  21:11 UTC  tatsu_s
 * 	Don't initialize sc/sx_table.lock.
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/20  22:05 UTC  mk
 * 	Merge dced-as-rpcd changes from mk_dced_as_rpcd/1.
 * 
 * 	HP revision /main/mk_dced_as_rpcd/1  1995/07/11  15:21 UTC  mk
 * 	Allow dced to run as rpcd and export only ep services.
 * 	[1995/12/08  20:49:02  root]
 * 
 * Revision 1.1.2.27  1994/09/30  19:37:10  bowe
 * 	Uncomment call to sec_attr_util_sch_ent_free_ptrs(). It was not
 * 	yet implemented when this code was orig written. [CR 10906]
 * 	[1994/09/30  19:36:58  bowe]
 * 
 * Revision 1.1.2.26  1994/09/22  22:00:35  bowe
 * 	More (final) SVC work [CR 11725]
 * 	[1994/09/22  21:42:53  bowe]
 * 
 * Revision 1.1.2.25  1994/09/17  08:05:15  rsalz
 * 	Merged with changes from 1.1.2.24
 * 	[1994/09/17  08:04:28  rsalz]
 * 
 * 	Store dced object UUID in a file (OT CR 12127).
 * 	Serviceability (OT CR 11725).
 * 	[1994/09/17  07:57:24  rsalz]
 * 
 * Revision 1.1.2.24  1994/09/14  20:04:21  bowe
 * 	SVC work [CR 11725]
 * 	[1994/09/14  20:03:50  bowe]
 * 
 * Revision 1.1.2.23  1994/09/12  15:58:41  baum
 * 	Fixed problems caused by adding obj uuidd to dced bindings
 * 	[1994/09/12  15:43:02  baum]
 * 
 * Revision 1.1.2.22  1994/09/02  19:52:07  bowe
 * 	Initiailze obj vector (used for registering svc obj). [CR 11979]
 * 	[1994/09/02  19:45:38  bowe]
 * 
 * Revision 1.1.2.21  1994/08/25  16:32:23  rsalz
 * 	Add dced_waitpid and fix hostdata postprocessor reaping.
 * 	[1994/08/25  16:30:32  rsalz]
 * 
 * Revision 1.1.2.20  1994/08/24  19:51:53  bowe
 * 	Merged with changes from 1.1.2.19
 * 	[1994/08/24  19:51:36  bowe]
 * 
 * 	In srvr_add_to_table() also copy srvr_file. [CR 11791]
 * 	[1994/08/24  19:39:25  bowe]
 * 
 * Revision 1.1.2.19  1994/08/24  16:41:09  baum
 * 	Full dced service on bootup.  Secval on by default (OT 11726 & 11656)
 * 	[1994/08/24  16:35:04  baum]
 * 
 * Revision 1.1.2.18  1994/07/29  19:56:12  rsalz
 * 	Add copy.c, split out from misc.c
 * 	Create ep-compat server in srvrexec (partial).
 * 	[1994/07/29  19:37:54  rsalz]
 * 
 * Revision 1.1.2.17  1994/07/07  19:15:01  bowe
 * 	Implement dced_privacy_ok(). [CR 11201]
 * 	[1994/07/07  19:14:11  bowe]
 * 
 * Revision 1.1.2.16  1994/07/06  20:17:23  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:10:40  pwang]
 * 
 * Revision 1.1.2.15  1994/06/30  19:36:58  ohara
 * 	define environ
 * 	[1994/06/30  18:52:14  ohara]
 * 
 * Revision 1.1.2.14  1994/06/16  19:35:15  rsalz
 * 	Use new free routines.
 * 	[1994/06/16  19:32:13  rsalz]
 * 
 * Revision 1.1.2.13  1994/06/09  16:03:07  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:45  devsrc]
 * 
 * Revision 1.1.2.12  1994/06/08  19:53:41  rsalz
 * 	Comment out free in name_cache_attr_init (OT CR 10906 work-around).
 * 	[1994/06/08  19:53:26  rsalz]
 * 
 * Revision 1.1.2.11  1994/06/07  20:31:37  rsalz
 * 	Really set count right in srvr_get_list.
 * 	[1994/06/07  18:52:14  rsalz]
 * 
 * 	Have srvr_qualify_prin_names use rpc_ns_entry_expand_name (OT CR 10874).
 * 	Set count right in srvr_get_list (OT CR 10875).
 * 	[1994/06/07  13:13:05  rsalz]
 * 
 * Revision 1.1.2.10  1994/06/03  19:17:13  rsalz
 * 	string_list_append didn't always allocate new space (OT CR 10857).
 * 	[1994/06/03  19:11:09  rsalz]
 * 
 * Revision 1.1.2.9  1994/06/02  20:13:04  rsalz
 * 	Don't lock in srvr_add_to_table (OT CR 10838).
 * 	Allow NULL strings in string_copy (OT CR 10847).
 * 	Ignore unused srvr_table entries in get routines (OT CR 10846).
 * 	[1994/06/02  20:10:21  rsalz]
 * 
 * Revision 1.1.2.8  1994/05/27  16:31:18  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:31  rsalz]
 * 
 * Revision 1.1.2.7  1994/05/26  19:42:21  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:52  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:33:01  rsalz]
 * 
 * Revision 1.1.2.6  1994/05/12  19:30:33  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:32  rsalz]
 * 
 * Revision 1.1.2.5  1994/05/11  20:11:23  rsalz
 * 	More functionality; code cleanups.
 * 	[1994/05/11  20:08:25  rsalz]
 * 
 * Revision 1.1.2.4  1994/05/10  20:15:15  rsalz
 * 	Merged with changes from 1.1.2.3
 * 	[1994/05/10  20:14:52  rsalz]
 * 
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:45  rsalz]
 * 
 * Revision 1.1.2.3  1994/05/10  15:39:04  bowe
 * 	Add missing * to dereference status ptr
 * 	[1994/05/10  15:38:46  bowe]
 * 
 * Revision 1.1.2.2  1994/05/05  20:32:45  rsalz
 * 	Fix merge problems from previous release
 * 	[1994/05/05  20:32:02  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/05  20:07:42  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:01:03  rsalz]
 * 
 * $EndLog$
 */
#include <libimpl.h>
#include <scimpl.h>
#include <sximpl.h>
#include <dce/dced_util.h>
#include <dce/sec_attr_util.h>
#include <dce/secidmap.h>

/* ______________________________________________________________________ */

typedef struct cache_entry_s_t {
    char		*name;
    uuid_t		id;
} cache_entry_t;


struct name_cache_s_t {
    dce_lock_t		lock;
    unsigned32		count;
    cache_entry_t	*list;
};

/* ______________________________________________________________________ */

static boolean32	privacy_required = TRUE;

/* ______________________________________________________________________ */


/*
** Initialize global directory value. This is done separately from the 
**  rest of global data so that it can used before dced daemonizes itself.
*/
void
dced_init_global_dir_path(
    error_status_t	*st
)
{
    dced_g_info.dir_path =
        malloc(strlen(dcelocal_path) + strlen(DCED_DIRECTORY) + 1 );
    if (dced_g_info.dir_path == NULL) {
        *st = dced_s_no_memory;
    }
    else {
        strcpy(dced_g_info.dir_path, dcelocal_path);
        strcat(dced_g_info.dir_path, DCED_DIRECTORY);
        *st = error_status_ok;
    }
}


/*
** Initialize global data values and structures.  Once set, most of these
**  are not changed.
*/
void
dced_init_global_data(
    boolean32		ep_only,
    boolean32		privacy,
    boolean32		is_server,
    boolean32		remote_update,
    error_status_t	*st
)
{
    int			i;
    extern char		**environ;
    char		*cp;

    privacy_required = privacy;
    dced_is_remote_disabled = !remote_update;
    dced_is_server = is_server;

    dced_g_anonymous_path =
	malloc(strlen(dced_g_info.dir_path)
	       + dced_c_service_hostdata_len + 3 );
    if (dced_g_anonymous_path == NULL) {
	*st = dced_s_no_memory;
	goto cleanup;
    }
    strcpy(dced_g_anonymous_path, dced_g_info.dir_path);
    strcat(dced_g_anonymous_path, "/");
    strcat(dced_g_anonymous_path, dced_c_service_hostdata);
    strcat(dced_g_anonymous_path, "/");

    dced_g_null_sec_attr.attr_value.attr_encoding = sec_attr_enc_void;
    dced_g_null_server.exec_data.execstate = server_c_exec_notrunning;

    uuid_create_nil(&dced_g_info.thishost_prin_uuid, st);
    uuid_create_nil(&dced_g_info.this_cell_uuid, st);

    /* Build a stringlist representing our environment. */
    for (i = 0; environ[i]; i++)
	continue;
    environ_string_list.count = i;
    environ_string_list.list =
	(idl_char **)malloc(i * sizeof *environ_string_list.list);
    if (environ_string_list.list == NULL) {
	*st = dced_s_no_memory;
	goto cleanup;
    }
    for (i = 0; environ[i]; i++)
	environ_string_list.list[i] = (idl_char *)dce_strdup(environ[i]);

    if (!ep_only) {
	dce_cf_get_cell_name(&dced_g_info.this_cell, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
			   "dce_cf_get_cell_name", (long)*st);
	    goto cleanup;
	}
    
	dce_cf_binding_entry_from_host(NULL, &dced_g_info.thishost_prin, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
			   "dce_cf_binding_entry_from_host", (long)*st);
	    goto cleanup;
	}
	dced_g_info.thishost_self =
	    malloc(strlen(dced_g_info.thishost_prin) + CELL_DIR_ROOT_LEN+1);
	if (dced_g_info.thishost_self == NULL) {
	    *st = dced_s_no_memory;
	    goto cleanup;
	}
	strcpy(dced_g_info.thishost_self, CELL_DIR_ROOT);
	strcat(dced_g_info.thishost_self, dced_g_info.thishost_prin);
    
	dce_cf_dced_entry_from_host(NULL, &cp, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
			   "dce_cf_dced_entry_from_host", (long)*st);
	    goto cleanup;
	}
	dced_g_info.this_dced = malloc(strlen(cp) + CELL_DIR_ROOT_LEN+1);
	if (dced_g_info.this_dced == NULL) {
	    *st = dced_s_no_memory;
	    goto cleanup;
	}
	strcpy(dced_g_info.this_dced, CELL_DIR_ROOT);
	strcat(dced_g_info.this_dced, cp);
	free(cp);
    }
    
    /* We must always initialize sx_table.lock because EP mapper uses it. */
    dce_lock_create(&sc_table.lock, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
	    "dce_lock_create(&sc_table.lock)", (long)*st);
	goto cleanup;
    }
    dce_lock_create(&sx_table.lock, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
	    "dce_lock_create(&sx_table.lock)", (long)*st);
	goto cleanup;
    }

    /* We always initialize sc/sx_hash_table.lock for simplicity. */
    dce_lock_create(&sc_hash_table.lock, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
	    "dce_lock_create(&sc_hash_table.lock)", (long)*st);
	goto cleanup;
    }
    dce_lock_create(&sx_hash_table.lock, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
	    "dce_lock_create(&sx_hash_table.lock)", (long)*st);
	goto cleanup;
    }

    pthread_cond_init(&sx_pids.cv, pthread_condattr_default);
    pthread_mutex_init(&sx_pids.m, pthread_mutexattr_default);
    dce_lock_create(&sx_pids.lock, st);
    sx_pids.pids.count = 20;
    sx_pids.pids.list = malloc(sx_pids.pids.count
				* sizeof *sx_pids.pids.list);
    if (sx_pids.pids.list == NULL) {
	*st = dced_s_no_memory;
	goto cleanup;
    }
    for (i = sx_pids.pids.count; --i >= 0; )
	sx_pids.pids.list[i].pid = 0;

    *st = error_status_ok;
    return;

cleanup:
    if (dced_g_info.dir_path != NULL) {
	free(dced_g_info.dir_path);
	dced_g_info.dir_path = NULL;
    }
    if (dced_g_anonymous_path != NULL) {
	free(dced_g_anonymous_path);
	dced_g_anonymous_path = NULL;
    }
    if (environ_string_list.list) {
	free(environ_string_list.list);
	environ_string_list.list = NULL;
    }
    if (dced_g_info.thishost_self != NULL) {
	free(dced_g_info.thishost_self);
	dced_g_info.thishost_self = NULL;
    }
    if (dced_g_info.this_dced != NULL) {
	free(dced_g_info.this_dced);
	dced_g_info.this_dced = NULL;
    }
}

static void
init_object_uuid(
    uuid_t		*ouuid,
    error_status_t	*st
)
{
    FILE		*F;
    idl_char		*p;

    uuid_create(ouuid, st);
    if (*st != error_status_ok)
	return;
    uuid_to_string(ouuid, &p, st);
    if (*st != error_status_ok)
	return;

    if ((F = fopen(DCED_OBJECT_FILE, "w")) == NULL) {
	dce_svc_printf(DCED_S_CANT_OPEN_DATABASE_MSG, DCED_OBJECT_FILE, errno);
	*st = dced_s_data_unavailable;
	return;
    }
    fprintf(F, "%s\n", p);
    if (fclose(F) == EOF) {
	dce_svc_printf(DCED_S_CANT_OPEN_DATABASE_MSG, DCED_OBJECT_FILE, errno);
	*st = dced_s_data_unavailable;
	return;
    }
    *st = error_status_ok;
}

void
dced_read_object_uuid(
    uuid_t		*ouuid,
    error_status_t	*st
)
{
    FILE		*F;
    char		buff[128];
    char		*cp;

    if (access(DCED_OBJECT_FILE, F_OK) != 0) {
        init_object_uuid(ouuid, st);
        if (*st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "object UUID", (long)*st);
	return;
    }

    if ((F = fopen(DCED_OBJECT_FILE, "r")) == NULL
     || fgets(buff, sizeof buff, F) == NULL) {
	dce_svc_printf(DCED_S_CANT_OPEN_DATABASE_MSG, DCED_OBJECT_FILE, errno);
	*st = dced_s_data_unavailable;
	if (F)
	    fclose(F);
	return;
    }
    fclose(F);
    if ((cp = strchr(buff, '\n')) != NULL)
	*cp = '\0';
    uuid_from_string((idl_char *)buff, ouuid, st);
}

void
dced_setup_global_uuids(
    error_status_t	*st
)
{
    sec_rgy_bind_auth_info_t	auth;
    sec_rgy_handle_t		rh;
    sec_rgy_name_t		prin_in_rgy;
    error_status_t		lst;

    auth.info_type = sec_rgy_bind_auth_none;
    sec_rgy_site_bind(NULL, &auth, &rh, st);
    if (*st != error_status_ok)
	return;

    /* Get HOST/self principal uuid. */
    strcpy((char *)prin_in_rgy, dced_g_info.thishost_prin);
    sec_rgy_pgo_name_to_id(rh, sec_rgy_domain_person, prin_in_rgy,
			   &dced_g_info.thishost_prin_uuid, st);
    if (*st != error_status_ok) {
	sec_rgy_site_close(rh, &lst);
	return;
    }

    /* Get cell uuid */
    sec_id_parse_name(rh, (idl_char *)dced_g_info.this_cell, NULL,
		      &dced_g_info.this_cell_uuid, NULL, NULL, st);
    if (*st != error_status_ok) {
	sec_rgy_site_close(rh, &lst);
	return;
    }

    sec_rgy_site_close(rh, st);
}

/*
**  Wrapper for all authorization checks.  Also gets invoking principal,
**  if desired.
*/
boolean32
dced_authorized(
    handle_t		h,
    uuid_t		*aclmgr,
    uuid_t		*acl,
    uuid_t		*Prin,
    sec_acl_permset_t	perms,
    error_status_t	*st
)
{
    boolean32		ok;

    *st = error_status_ok;

    /* Bootstrap mode -- everyone allowed. */
    if (dced_is_bootstrap) {
	if (Prin)
	    *Prin = uuid_g_nil_uuid;
	return TRUE;
    }

    /* Local operation? */
    if (h == NULL) {
	if (Prin)
	    *Prin = dced_g_info.thishost_prin_uuid;
	return TRUE;
    }

    if (!dced_is_acl_enabled) {
	*st = sec_acl_invalid_permission;
	return FALSE;
    }

    /* Need the client identity? */
    if (Prin) {
	dce_acl_inq_prin_and_group(h, Prin, NULL, st);
	if (*st != error_status_ok)
	    return FALSE;
    }

    dce_acl_is_client_authorized(h, aclmgr, acl, Prin, NULL, perms, &ok, st);
    if (!ok) {
	*st = sec_acl_invalid_permission;
	return FALSE;
    }

    return TRUE;
}


/*
**  Wrapper to ensure that client is using privacy (if required).
*/
boolean32
dced_privacy_ok(
    handle_t			h,
    error_status_t		*st
)
{
    rpc_authz_cred_handle_t	cred_h;
    unsigned32			protect;

    if (h == NULL || !privacy_required)
	return TRUE;

    rpc_binding_inq_auth_caller(h, &cred_h, NULL, &protect, NULL, NULL, st);
    if (*st != error_status_ok)
    	return FALSE;
    if (protect == rpc_c_protect_level_pkt_privacy && *st == error_status_ok)
	return TRUE;
    *st = dced_s_need_privacy;
    return FALSE;
}



server_data_t *
srvr_find_by_uuid(
    srvr_table_t	*t,
    uuid_t		*id,
    error_status_t	*st
)
{
    server_data_t	*sp;
    unsigned32		i;

    for (i = t->count, sp = t->list; i; i--, sp++)
	if (sp->used && uuid_equal(&sp->data.id, id, st))
	    return sp;
    return NULL;
}


server_data_t *
srvr_find_by_name(
    srvr_table_t	*t,
    idl_char		*name,
    error_status_t	*st
)
{
    server_data_t	*sp;
    unsigned32		i;

    *st = error_status_ok;
    for (i = t->count, sp = t->list; i; i--, sp++)
	if (sp->used && strcmp((char *)sp->data.name, (char *)name) == 0)
	    return sp;
    return NULL;
}


void
srvr_get_list(
    srvr_table_t	*t,
    dced_entry_list_t	*items,
    error_status_t	*st
)
{
    static idl_char	*NIL = (idl_char *)"";
    unsigned32		i;
    dced_entry_t	*ep;
    server_data_t	*sp;

    dce_lock_read(t->lock, st);
    items->count = 0;
    items->list = (dced_entry_t *)
	rpc_sm_allocate(t->count * sizeof *items->list, st);
    for (sp = t->list, ep = items->list, i = t->count; i; i--, sp++) {
	if (!sp->used)
	    continue;
	ep->id = sp->data.id;
	ep->name = strdup_for_client(sp->data.name ? sp->data.name : NIL, st);
	ep->description = strdup_for_client((idl_char *)"A server", st);
	ep->storage_tag = strdup_for_client((idl_char *)"#Internal", st);
	ep++;
	items->count++;
    }
    dce_lock_unlock(t->lock, st);
}


void
srvr_add_to_table(
    srvr_table_t	*t,
    server_data_t	*new,
    error_status_t	*st
)
{
    server_data_t	*sp;
    unsigned32		i;

    for (sp = t->list, i = t->count; i && sp->used; i--, sp++)
	continue;
    if (i == 0) {
	t->count++;
	t->list = (server_data_t *)realloc(t->list, t->count * sizeof *t->list);
	sp = &t->list[t->count - 1];
    }

    sp->used = TRUE;
    sp->h = new->h;
    string_copy(&sp->srvr_file, &new->srvr_file, dced_copy_via_malloc, st);
    server_copy(&sp->data, &new->data, dced_copy_via_malloc, st);
    srvr_hash_build(t, t->hashtable, st);
}


/*
**  Prepend the cell name if local principal given.  Modify the list
**  using auto-reclaimed memory.
*/
void
srvr_qualify_prin_names(
    dced_string_list_t	*names,
    error_status_t	*st
)
{
    unsigned32		i;
    idl_char		*new;
    unsigned32		len;
    idl_char		**p;

    for (len = strlen(dced_g_info.this_cell), i = names->count,
	     p = names->list; i; p++) {
	i--;
	if (p[0][0] == '/')
	    continue;
	new = dced_copy_for_stub(len + 1 + strlen((char *)p[0]) + 1);
	strcpy((char *)new, (char *)dced_g_info.this_cell);
	new[len] = '/';
	strcpy((char *)&new[len + 1], (char *)p[0]);
	p[0] = new;
    }
}



/*
**  Return an item from the cache.  Assumes caller has locked it.
*/
static cache_entry_t *
name_cache_find(
    name_cache_t	cache,
    char		*name
)
{
    unsigned32		n;
    cache_entry_t	*cp;

    for (cp = cache->list, n = cache->count; n; n--, cp++)
	if (strcmp(cp->name, name) == 0)
	    return cp;
    return NULL;
}


boolean32
name_cache_find_uuid(
    name_cache_t	cache,
    char		*name,
    uuid_t		*id
)
{
    cache_entry_t	*cp;
    error_status_t	st;

    dce_lock_read(cache->lock, &st);
    if ((cp = name_cache_find(cache, name)) != NULL) {
	*id = cp->id;
	dce_lock_unlock(cache->lock, &st);
	return TRUE;
    }
    dce_lock_unlock(cache->lock, &st);
    return FALSE;
}


/*
**  Add an item to the cache.
*/
void
name_cache_add(
    name_cache_t	cache,
    char		*name,
    uuid_t		*id
)
{
    cache_entry_t	*cp;
    error_status_t	st;

    /* First, check if name is already in list.  If so, use its slot. */
    dce_lock_riw(cache->lock, &st);
    dce_lock_write(cache->lock, &st);
    if ((cp = name_cache_find(cache, name)) != NULL)
	cp->id = *id;
    else {
	cache->list = (cache_entry_t *)realloc(cache->list,
	    (cache->count + 1) * sizeof *cache->list);
	cp = &cache->list[cache->count++];
	cp->name = dce_strdup(name);
	cp->id   = *id;
    }
    dce_lock_unlock(cache->lock, &st);
}


/*
**  Delete an item from the cache.
*/
void
name_cache_del(
    name_cache_t	cache,
    char		*name
)
{
    cache_entry_t	*cp;
    error_status_t	st;

    /* First, check if name is already in list.  If so, use it's slot. */
    dce_lock_riw(cache->lock, &st);
    if ((cp = name_cache_find(cache, name)) != NULL) {
	/* Move the last one to the slot found and decrement the count.
	 * Don't realloc down -- not worth the cost, since we will
	 * probably just add an element soon anyway. */
	dce_lock_write(cache->lock, &st);
	free(cp->name);
	cache->count--;
	*cp = cache->list[cache->count];
    }
    dce_lock_unlock(cache->lock, &st);
}



static void
name_cache_schema_init(
    dce_db_handle_t	db,
    name_cache_t	cache,
    error_status_t	*st
)
{
    unsigned32		n;
    uuid_t		*u;
    attr_schema_data_t	S;
    cache_entry_t	*cp;

    dce_db_inq_count(db, &n, st);
    if (n == 0) {
	cache->count = 0;
	cache->list = malloc(sizeof *cache->list);
	return;
    }

    cache->count = n;
    cache->list = (cache_entry_t *)malloc(n * sizeof *cache->list);
    if (cache->list == NULL) {
	*st = dced_s_no_memory;
	return;
    }
    dce_db_iter_start(db, st);
    CHECK_STAT_RET(*st);
    for (cp = cache->list; ; cp++) {
        dce_db_iter_next_by_uuid(db, &u, st);
        if (*st == db_s_no_more) break;
	CHECK_STAT_RET(*st);
	dce_db_fetch_by_uuid(db, u, &S, st);
	CHECK_STAT_RET(*st);
	cp->name = dce_strdup((char *)S.data.attr_name);
	cp->id   = S.data.attr_id;
	sec_attr_util_sch_ent_free_ptrs(&S.data);
    }
    dce_db_iter_done(db, st);
}


static void
name_cache_entry_init(
    dce_db_handle_t	db,
    name_cache_t	cache,
    error_status_t	*st
)
{
    unsigned32		n;
    uuid_t		*u;
    dced_entry_data_t	E;
    cache_entry_t	*cp;

    dce_db_inq_count(db, &n, st);
    if (n == 0) {
	cache->count = 0;
	cache->list = malloc(sizeof *cache->list);
	return;
    }

    cache->count = n;
    cache->list = (cache_entry_t *)malloc(n * sizeof *cache->list);
    if (cache->list == NULL) {
	*st = dced_s_no_memory;
	return;
    }
    dce_db_iter_start(db, st);
    CHECK_STAT_RET(*st);
    for (cp = cache->list; ; cp++) {
        dce_db_iter_next_by_uuid(db, &u, st);
        if (*st == db_s_no_more) break;
	CHECK_STAT_RET(*st);
	dce_db_fetch_by_uuid(db, u, &E, st);
	CHECK_STAT_RET(*st);
	cp->name = dce_strdup((char *)E.data.name);
	cp->id   = E.data.id;
	entry_free_ptrs(&E.data, dced_sm_client_free);
    }
    dce_db_iter_done(db, st);
}


static void
srvr_hash_table_add(
    srvr_hash_table_t	*ht,
    uuid_t		*up,
    srvr_hash_entry_t	*value,
    error_status_t	*st
)
{
    unsigned16		h;
    srvr_hash_bucket_t	*bp;

    h = uuid_hash(up, st);
    h = srvr_hash2bucket(h);
    bp = &ht->list[h];
    if (bp->size == 0) {
	bp->size = srvr_c_hash_bucket_init;
	bp->list = (srvr_hash_entry_t *)malloc(bp->size * sizeof *bp->list);
	if (bp->list == NULL) {
	    *st = dced_s_no_memory;
	    return;
	}
    }
    else if (bp->count >= bp->size - 1) {
	bp->size += srvr_c_hash_bucket_delta;
	bp->list = (srvr_hash_entry_t *)
			realloc(bp->list, bp->size * sizeof *bp->list);
	if (bp->list == NULL) {
	    *st = dced_s_no_memory;
	    return;
	}
    }
    bp->list[bp->count++] = *value;
}

/*
**  (Re)build the hash tables.  Caller must have locks held.
*/
void
srvr_hash_build(
    srvr_table_t	*t,
    srvr_hash_table_t	*ht,
    error_status_t	*st
)
{
    unsigned32		i;
    unsigned32		j;
    unsigned32		end;
    service_t		*svcp;
    server_data_t	*sp;
    srvr_hash_bucket_t	*bp;
    srvr_hash_entry_t	value;
    uuid_t		*up;

    /* Empty the buckets. */
    for (bp = ht->list, i = srvr_c_hash_table_size; i; i--, bp++)
	bp->count = 0;

    /* Loop over all servers. */
    value.entry = 0;
    for (i = t->count, sp = t->list; i; sp++, i--, value.entry++) {
	if (!sp->used)
	    continue;
	/* Process each service. */
	svcp = sp->data.services.list;
	value.object = 0;
	for (end = sp->data.services.count, value.service = 0;
		(unsigned32)value.service < end;
		value.service++, svcp++) {
	    srvr_hash_table_add(ht, &svcp->ifspec.uuid, &value, st);
	    for (j = svcp->objects.count, up = svcp->objects.list; j;
		    j--, up++) {
		value.object++;
		srvr_hash_table_add(ht, up, &value, st);
	    }
	}
    }
}


static void
name_cache_server_init(
    dce_db_handle_t	db,
    name_cache_t	cache,
    srvr_table_t	*t,
    srvr_hash_table_t	*ht,
    error_status_t	*st
)
{
    unsigned32		n;
    uuid_t		*u;
    server_data_t	S;
    cache_entry_t	*cp;
    server_data_t	*sp;

    if (t->setup)
	return;
    t->setup = TRUE;

    t->hashtable = ht;
    dce_db_inq_count(db, &n, st);
    if (n == 0) {
	cache->count = 0;
	cache->list = malloc(sizeof *cache->list);
	t->count = 0;
	t->list = malloc(sizeof *t->list);
	return;
    }

    cache->count = n;
    cache->list = (cache_entry_t *)malloc(n * sizeof *cache->list);
    if (cache->list == NULL) {
	*st = dced_s_no_memory;
	return;
    }
    t->count = n;
    t->list = (server_data_t *)malloc(n * sizeof *t->list);
    if (t->list == NULL) {
	free(cache->list);
	*st = dced_s_no_memory;
	return;
    }
    dce_db_iter_start(db, st);
    CHECK_STAT_RET(*st);
    for (cp = cache->list, sp = t->list; ; cp++, sp++) {
        dce_db_iter_next_by_uuid(db, &u, st);
        if (*st == db_s_no_more) break;
	CHECK_STAT_RET(*st);
	dce_db_fetch_by_uuid(db, u, &S, st);
	CHECK_STAT_RET(*st);
	cp->name = dce_strdup((char *)S.data.name);
	cp->id   = (S.data.exec_data.execstate & server_c_exec_running)
		     ? S.data.exec_data.tagged_union.running_data.instance
		     : S.data.id;
	*sp = S;
    }
    dce_db_iter_done(db, st);

    dce_lock_riw(ht->lock, st);
    dce_lock_write(ht->lock, st);
    srvr_hash_build(t, ht, st);
    dce_lock_unlock(ht->lock, st);
}


static void
name_cache_create(
    name_cache_t	*cp,
    error_status_t	*st
)
{
    name_cache_t	new;

    if ((new = malloc(sizeof *new)) == NULL) {
	*st = dced_s_no_memory;
	return;
    }
    dce_lock_create(&new->lock, st);
    CHECK_STAT_RET(*st);
    *cp = new;
}


void
dced_init_name_caches(
    error_status_t	*st
)
{
    *st = error_status_ok;

    name_cache_create(&hd_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_create(&km_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_create(&as_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_create(&sc_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_create(&sx_name_cache, st);
    CHECK_STAT_RET(*st);

    name_cache_entry_init(hd_db, hd_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_entry_init(km_db, km_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_schema_init(as_db, as_name_cache, st);
    CHECK_STAT_RET(*st);

    name_cache_server_init(sc_db, sc_name_cache,
	&sc_table, &sc_hash_table, st);
    CHECK_STAT_RET(*st);
    name_cache_server_init(sx_db, sx_name_cache,
	&sx_table, &sx_hash_table, st);
    CHECK_STAT_RET(*st);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		   "Initialized name caches."));
}

void
dced_reload_name_caches(
    error_status_t	*st
)
{
    name_cache_t	*n_caches[] = {
	&hd_name_cache,
	&km_name_cache,
	&as_name_cache,
	&sc_name_cache
    };
    name_cache_t	cache;
    cache_entry_t	*cp;
    unsigned32		i;
    unsigned32		n;

    *st = error_status_ok;

    /* Flush caches. */
    for (i = 0; i < sizeof(n_caches)/sizeof(n_caches[0]); i++) {
	cache = *(n_caches[i]);
	dce_lock_riw(cache->lock, st);
	dce_lock_write(cache->lock, st);
	for (cp = cache->list, n = cache->count; n; n--, cp++) {
	    free(cp->name);
	}
	cache->count = 0;
	free(cache->list);
	cache->list = NULL;
	dce_lock_unlock(cache->lock, st);
    }

    /* We can't flush srvrexec. */
    cache = sc_name_cache;
    dce_lock_riw(cache->lock, st);
    dce_lock_write(cache->lock, st);
    for (cp = cache->list, n = cache->count; n; n--, cp++) {
	free(cp->name);
    }
    cache->count = 0;
    free(cache->list);
    cache->list = NULL;
    free(sc_table.list);
    sc_table.list = NULL;
    dce_lock_unlock(cache->lock, st);
    sc_table.setup = FALSE;

    name_cache_entry_init(hd_db, hd_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_entry_init(km_db, km_name_cache, st);
    CHECK_STAT_RET(*st);
    name_cache_schema_init(as_db, as_name_cache, st);
    CHECK_STAT_RET(*st);

    name_cache_server_init(sc_db, sc_name_cache,
	&sc_table, &sc_hash_table, st);
    CHECK_STAT_RET(*st);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		   "Reloaded name caches."));
}

/* ______________________________________________________________________ */

void
dced_sleep(
    time_t		secs
)
{
    struct timespec	ts;

    ts.tv_sec = secs;
    ts.tv_nsec = 0;
    pthread_delay_np(&ts);
}


/*
**  Sleep until a specific time (if not already past) and return the
**  current time.
*/
void
dced_sleep_until(
    struct timeval	*when,
    time_t		until
)
{
    struct timeval	now;
    time_t		secs;

    secs = when->tv_sec + until;
    dce_gettimeofday(&now);
    if (secs > now.tv_sec)
	dced_sleep(secs - now.tv_sec);
    dce_gettimeofday(when);
}

/* ______________________________________________________________________ */

static void
open_database(
    char			*name,
    int				oflags,
    dce_db_convert_func_t	func,
    dce_db_handle_t		*dbh,
    error_status_t		*st
)
{
    dce_db_open(name, NULL, oflags, func, dbh, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_OPEN_DATABASE_MSG, name, errno);
    }
}

void
dced_open_databases(
    boolean32		ic,
    error_status_t	*st
)
{
    unsigned32		oflags, oflags_a;
    mode_t		omode;
    char		*fname;
    char		*cp;
    int			len;
    error_status_t	lst;

    *st = error_status_ok;

    if (!dced_is_server) {
	dced_register_local_mgrs(&dced_hostdata_v1_0_epv,
				 &dced_rkeytab_v1_0_epv,
				 &dced_srvrconf_v1_0_epv,
				 &dced_srvrexec_v1_0_epv,
				 st);
	CHECK_STAT_RET(*st);
    }

    /* For now, ATTR_SCHEMA_DB_NAME is longest. */
    len = strlen(dced_g_info.dir_path);
    fname = malloc(len + 1 + strlen(ATTR_SCHEMA_DB_NAME) + 1);
    if (fname == NULL) {
	*st = dced_s_no_memory;
	return;
    }

    if (dced_is_server)
	cp = fname;
    else
    {
	strcpy(fname, dced_g_info.dir_path);
	cp = fname + len;
	*cp++= '/';
    }

    /* See what flags we need.  Only create if initializing (or else
     * dced will happily go on with empty databases). */
    oflags   = db_c_lock_nowait | db_c_index_by_uuid | db_c_std_header;
    oflags_a = db_c_lock_nowait | db_c_index_by_uuid;
    if (ic) {
	oflags   |= db_c_create;
	oflags_a |= db_c_create;
	omode = umask(S_IXUSR|S_IRWXG|S_IRWXO);
    }

    strcpy(cp, HOSTDATA_DB_NAME);
    open_database(fname, oflags,
		  (dce_db_convert_func_t)dced_entry_convert, &hd_db, st);
    if (*st != error_status_ok) {
	hd_db = NULL;
	goto cleanup;
    }

    strcpy(cp, RKEYTAB_DB_NAME);
    open_database(fname, oflags,
		  (dce_db_convert_func_t)dced_entry_convert, &km_db, st);
    if (*st != error_status_ok) {
	km_db = NULL;
	goto cleanup;
    }

    strcpy(cp, ATTR_SCHEMA_DB_NAME);
    open_database(fname, oflags,
		  (dce_db_convert_func_t)dced_attr_schema_convert, &as_db, st);
    if (*st != error_status_ok) {
	as_db = NULL;
	goto cleanup;
    }

    strcpy(cp, SRVRCONF_DB_NAME);
    open_database(fname, oflags,
	(dce_db_convert_func_t)dced_server_convert, &sc_db, st);
    if (*st != error_status_ok) {
	sc_db = NULL;
	goto cleanup;
    }

    strcpy(cp, ACL_DB_NAME);
    open_database(fname, oflags_a,
	(dce_db_convert_func_t)dce_rdacl_convert, &acl_db, st);
    if (*st != error_status_ok) {
	acl_db = NULL;
	goto cleanup;
    }

    /* Open the endpoint database.  Create it if necessary. */
    strcpy(cp, SRVREXEC_DB_NAME);
    dce_db_open(fname, NULL, oflags,
	(dce_db_convert_func_t)dced_server_convert, &sx_db, st);
    if (*st == db_s_open_failed_enoent)
	open_database(fname, oflags | db_c_create,
	    (dce_db_convert_func_t)dced_server_convert, &sx_db, st);
    if (*st != error_status_ok)
	sx_db = NULL;

cleanup:
    if (ic)
	(void)umask(omode);

    (void)free(fname);

    if (*st != error_status_ok) {
	if (acl_db != NULL)
	    dce_db_close(&acl_db, &lst);
	if (hd_db != NULL)
	    dce_db_close(&hd_db, &lst);
	if (km_db != NULL)
	    dce_db_close(&km_db, &lst);
	if (sc_db != NULL)
	    dce_db_close(&sc_db, &lst);
	if (sx_db != NULL)
	    dce_db_close(&sx_db, &lst);
	if (as_db != NULL)
	    dce_db_close(&as_db, &lst);
	acl_db = NULL;
	hd_db = NULL;
	km_db = NULL;
	sc_db = NULL;
	sx_db = NULL;
	as_db = NULL;
    }
    else {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		       "Opened databases."));
    }
}

void
dced_close_databases(
    error_status_t	*st
)
{
    error_status_t	lst;

    *st = error_status_ok;

    dce_db_close(&hd_db, &lst);
    if (lst != error_status_ok) {
	dce_svc_printf(
	    DCE_SVC(dhd_svc_handle, ""), dhd_s_general,
	    svc_c_sev_fatal, dced_s_cant_stop_something, HOSTDATA_DB_NAME,
	    (long)lst);
	if (*st == error_status_ok)
	    *st = lst;
    }

    dce_db_close(&km_db, &lst);
    if (lst != error_status_ok) {
	dce_svc_printf(
	    DCE_SVC(dhd_svc_handle, ""), dhd_s_general,
	    svc_c_sev_fatal, dced_s_cant_stop_something, RKEYTAB_DB_NAME,
	    (long)lst);
	if (*st == error_status_ok)
	    *st = lst;
    }

    dce_db_close(&as_db, &lst);
    if (lst != error_status_ok) {
	dce_svc_printf(
	    DCE_SVC(dhd_svc_handle, ""), dhd_s_general,
	    svc_c_sev_fatal, dced_s_cant_stop_something, ATTR_SCHEMA_DB_NAME,
	    (long)lst);
	if (*st == error_status_ok)
	    *st = lst;
    }

    dce_db_close(&sc_db, &lst);
    if (lst != error_status_ok) {
	dce_svc_printf(
	    DCE_SVC(dhd_svc_handle, ""), dhd_s_general,
	    svc_c_sev_fatal, dced_s_cant_stop_something, SRVRCONF_DB_NAME,
	    (long)lst);
	if (*st == error_status_ok)
	    *st = lst;
    }

    dce_db_close(&sx_db, &lst);
    if (lst != error_status_ok) {
	dce_svc_printf(
	    DCE_SVC(dhd_svc_handle, ""), dhd_s_general,
	    svc_c_sev_fatal, dced_s_cant_stop_something, SRVREXEC_DB_NAME,
	    (long)lst);
	if (*st == error_status_ok)
	    *st = lst;
    }

    dce_db_close(&acl_db, &lst);
    if (lst != error_status_ok) {
	dce_svc_printf(
	    DCE_SVC(dhd_svc_handle, ""), dhd_s_general,
	    svc_c_sev_fatal, dced_s_cant_stop_something, ACL_DB_NAME,
	    (long)lst);
	if (*st == error_status_ok)
	    *st = lst;
    }

    if (*st == error_status_ok) {
	acl_db = NULL;
	hd_db = NULL;
	km_db = NULL;
	sc_db = NULL;
	sx_db = NULL;
	as_db = NULL;
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		       "Closed databases."));
    }
}
