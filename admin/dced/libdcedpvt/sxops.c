/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sxops.c,v $
 * Revision 1.1.2.2  1996/02/18  19:16:24  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:22  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:40  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/08  21:46 UTC  jss
 * 	Merge changes for CHFts14511, CHFts15038, CHFts14283.
 * 
 * 	HP revision /main/HPDCE02/jss_mothra_fix3/1  1995/06/06  14:20 UTC  jss
 * 	Fix for OT12837. Fix lock usage problems
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/27  14:57 UTC  tatsu_s
 * 	Submitted the stale and lost endpoints fix.
 * 
 * 	HP revision /main/tatsu_s.ep_mapper.b0/1  1995/02/17  16:26 UTC  tatsu_s
 * 	Fixed RPC_RANDOM_GET().
 * 	[1995/12/08  20:49:05  root]
 * 
 * Revision 1.1.5.2  1994/10/26  19:54:46  rsalz
 * 	Keep track of instance id for restart (OT CR 12714; fix by pwang).
 * 	[1994/10/26  19:54:14  rsalz]
 * 
 * 	Use srvrexec endpoints (OT CR 12061).
 * 	Add kill(2)-based pinging.
 * 	Implement RPC-based server-stopping.
 * 	[1994/10/26  19:43:47  rsalz]
 * 
 * Revision 1.1.5.1  1994/10/25  21:44:46  max
 * 	CR 12568 general fixes
 * 	[1994/10/25  21:43:13  max]
 * 
 * Revision 1.1.2.27  1994/10/03  19:58:47  rsalz
 * 	Add some SVC calls (OT CR 11725).
 * 	[1994/10/03  19:57:25  rsalz]
 * 
 * Revision 1.1.2.26  1994/09/30  20:14:38  rsalz
 * 	Add srvrexec_data_t.sc_uuid and use it to restart servers (OT CR 12442).
 * 	[1994/09/30  20:11:16  rsalz]
 * 
 * Revision 1.1.2.25  1994/09/28  17:37:49  baum
 * 	server_enable/disable change (OT 12380)
 * 	[1994/09/28  17:37:40  baum]
 * 
 * Revision 1.1.2.24  1994/09/23  14:06:09  bowe
 * 	svc work [CR 11725]
 * 	[1994/09/23  14:05:55  bowe]
 * 
 * Revision 1.1.2.23  1994/09/22  17:05:55  baum
 * 	Move dced_server_free_ptr() (OT 12287)
 * 	[1994/09/22  17:05:44  baum]
 * 
 * Revision 1.1.2.22  1994/09/21  21:49:47  baum
 * 	Added bit set to rpc branch in kill_server
 * 	[1994/09/21  21:49:25  baum]
 * 
 * Revision 1.1.2.21  1994/09/20  21:25:56  bowe
 * 	Remote item from sx name cache in remove_server [CR 12272]
 * 	[1994/09/20  21:25:31  bowe]
 * 
 * Revision 1.1.2.20  1994/09/17  08:05:21  rsalz
 * 	Remote ep-compat fake server (OT CR 12140).
 * 	Remove ACL when removing server (OT CR 12030).
 * 	[1994/09/17  07:58:14  rsalz]
 * 
 * Revision 1.1.2.19  1994/09/07  04:06:03  rsalz
 * 	Update backing store when objects change (OT CR 12066).
 * 	[1994/09/07  04:03:02  rsalz]
 * 
 * Revision 1.1.2.18  1994/09/06  19:56:26  rsalz
 * 	Add srvrexec_exited; finish OT CR 11791.
 * 	Create ACL and database objects for started servers (OT CR 12027).
 * 	Unlock mutex in dced_waitpid at right times (OT CR 12042).
 * 	[1994/09/06  19:34:23  rsalz]
 * 
 * Revision 1.1.2.17  1994/09/01  19:10:16  rsalz
 * 	Unlock the right server table in stop (OT CR 12005).
 * 	[1994/09/01  19:06:01  rsalz]
 * 
 * Revision 1.1.2.16  1994/08/28  17:56:17  rsalz
 * 	Rename some fields for clarity.
 * 	[1994/08/28  17:55:54  rsalz]
 * 
 * Revision 1.1.2.15  1994/08/28  00:58:40  rsalz
 * 	hd_post_proc passes in the item name through dced_fork (OT CR 11895).
 * 	[1994/08/26  19:14:33  rsalz]
 * 
 * Revision 1.1.2.14  1994/08/25  16:32:40  rsalz
 * 	Add dced_waitpid and fix hostdata postprocessor reaping.
 * 	[1994/08/25  16:31:31  rsalz]
 * 
 * Revision 1.1.2.13  1994/08/24  19:51:55  bowe
 * 	Fix reaper to clean up when exec'ed server exits.  [CR 11791]
 * 	[1994/08/24  19:39:32  bowe]
 * 
 * Revision 1.1.2.12  1994/07/29  19:56:20  rsalz
 * 	Create ep-compat server in srvrexec (partial).
 * 	[1994/07/29  19:38:42  rsalz]
 * 
 * Revision 1.1.2.11  1994/06/30  19:37:00  ohara
 * 	Merged with changes from 1.1.2.10
 * 	[1994/06/30  19:36:30  ohara]
 * 
 * 	defined **envrion
 * 	[1994/06/07  22:21:36  ohara]
 * 
 * Revision 1.1.2.10  1994/06/30  17:38:29  rsalz
 * 	Add dced_s_name_missing; use it in "create" operations (OT CR 10850).
 * 	[1994/06/30  17:34:46  rsalz]
 * 
 * Revision 1.1.2.9  1994/05/26  19:42:30  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:36:02  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:33:05  rsalz]
 * 
 * Revision 1.1.2.8  1994/05/12  19:30:36  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:36  rsalz]
 * 
 * Revision 1.1.2.7  1994/05/11  20:11:26  rsalz
 * 	More functionality; code cleanups.
 * 	[1994/05/11  20:08:29  rsalz]
 * 
 * Revision 1.1.2.6  1994/05/10  20:15:19  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:51  rsalz]
 * 
 * Revision 1.1.2.5  1994/05/05  20:07:49  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:01:09  rsalz]
 * 
 * Revision 1.1.2.4  1994/04/26  19:25:53  bowe
 * 	New params to dce_acl_is_client_authorized()
 * 	[1994/04/26  19:19:55  bowe]
 * 
 * Revision 1.1.2.3  1994/04/11  20:22:08  rsalz
 * 	Final set of RFC revision fixes.
 * 	[1994/04/11  20:08:28  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:15  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:13  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:26:05  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:35  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <libimpl.h>
#include <sximpl.h>
#include <sys/wait.h>


#define IFID_EQUAL(if1, if2, st)			\
	(uuid_equal(&(if1)->uuid, &(if2)->uuid, (st))	\
	 && (if1)->vers_major == (if2)->vers_major	\
	 && (if1)->vers_minor == (if2)->vers_minor)


/*
**  Stolen from rpcrand.h in rpc/runtime.
*/
#define RPC_RANDOM_GET(lower, upper) \
	(((rpc__random_get(lower, upper)) % ((upper) - (lower) + 1)) + (lower))
extern unsigned32 rpc__random_get(unsigned32, unsigned32);


/* ______________________________________________________________________ */

void
dced_invoke(
    char		**av,
    int			*status,
    error_status_t	*st
)
{
    extern char         **environ;
    pid_t		p;
    pid_t		pid;

    *st = error_status_ok;

    p = fork();
    if (p == -1) {
	*st = dced_s_postprocessor_spawn_fail;
	return;
    }
    if (p == 0) {
	execve(av[0], av, environ);
	_exit(1);
    }

    while (1) {
	/*
	 * For future reference, should this change get merged
	 * to OSF DCE, note that HP DCE has a non-process-blocking
	 * waitpid(), but OSF DCE does not.
	 */
	pid = waitpid(p, status, 0);
	if (pid < 0) {
	    if (errno == EINTR)
		continue;
	    if (errno != ECHILD) {
		*st = dced_s_postprocessor_spawn_fail;
		dce_svc_printf(DCED_S_SX_WAITPID_FAILED_MSG, strerror(errno));
	    }
	    return;
	}
	if (pid != p)
	    *st = dced_s_postprocessor_spawn_fail;
	break;
    }
}

void
dced_fork(
    char		**av,
    pid_t		*pp,
    error_status_t	*st
)
{
    extern char         **environ;
    pid_t		p;
    int			i;

    p = fork();
    if (p == -1) {
	*st = dced_s_postprocessor_spawn_fail;
	return;
    }
    if (p == 0) {
	execve(av[0], av, environ);
	_exit(1);
    }

    /* Add this PID to our process table. */
    dce_lock_justwrite(sx_pids.lock, st);
    for (i = 0; i < sx_pids.pids.count; i++)
	if (sx_pids.pids.list[i].pid == 0)
	    break;
    if (i == sx_pids.pids.count) {
	sx_pids.pids.count += 20;
	sx_pids.pids.list = realloc(sx_pids.pids.list,
		sx_pids.pids.count * sizeof *sx_pids.pids.list);
    }
    sx_pids.pids.list[i].pid = *pp = p;
    sx_pids.pids.list[i].done = 0;
    dce_lock_unlock(sx_pids.lock, st);
    *st = error_status_ok;
}

void
dced_waitpid(
    pid_t		pid,
    int			*status,
    error_status_t	*st
)
{
    unsigned32		i;

    pthread_mutex_lock(&sx_pids.m);
    for ( ; ; ) {
        pthread_cond_wait(&sx_pids.cv, &sx_pids.m);
        dce_lock_riw(sx_pids.lock, st);
        for (i = 0; i < sx_pids.pids.count; i++)
            if (
                (sx_pids.pids.list[i].pid == pid)
             && (sx_pids.pids.list[i].done)
            ) {
                dce_lock_write(sx_pids.lock, st);
                *status = sx_pids.pids.list[i].status;
                sx_pids.pids.list[i].pid = 0;
                dce_lock_unlock(sx_pids.lock, st);
                pthread_mutex_unlock(&sx_pids.m);
                return;
            }
        dce_lock_unlock(sx_pids.lock, st);
    }
}


/* ______________________________________________________________________ */

static void
remove_server(
    server_data_t	*sp,
    error_status_t	*st
)
{
    dce_svc_printf(DCED_S_SX_REAPING_SERVER_MSG, sp->data.name);
    dce_lock_write(sx_table.lock, st);
    /* remove its encoded server_t file */
    if (sp->srvr_file) {
	unlink((char *)sp->srvr_file);
	free(sp->srvr_file);
	sp->srvr_file = NULL;
    }
    sp->used = FALSE;
    srvr_hash_build(&sx_table, sx_table.hashtable, st);
    dce_db_delete_by_uuid(sx_db,
	&sp->data.exec_data.tagged_union.running_data.instance, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_OBJECT_MSG, "remove_server",
		(long)*st);
    dce_db_delete_by_uuid(acl_db, &sp->h.tagged_union.h.acl_uuid, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_DELETE_ACL_MSG, "remove_server", *st);
    dce_lock_unlock(sx_table.lock, st);
    name_cache_del(sx_name_cache, (char *)sp->data.name);
    dced_server_free_ptrs(&sp->data);
}

void
dced_srvrexec__reap(
    void
)
{
    pid_t			pid;
    int				wait_stat;
    server_data_t		*sp;
    error_status_t		st;
    unsigned32			i;
    server_fixedattr_t		fixed;
    dced_attr_list_t		attributes;
    uuid_t			id;

    for ( ; ; ) {
	/* Get next child. */
	pid = waitpid(-1, &wait_stat, WNOHANG);
	if (pid == 0)
	    return;
	if (pid < 0) {
	    if (errno != ECHILD)
		dce_svc_printf(DCED_S_SX_WAITPID_FAILED_MSG, strerror(errno));
	    return;
	}

	/* Find the server. */
	dce_lock_riw(sx_table.lock, &st);
	for (sp = sx_table.list, i = sx_table.count; i; i--, sp++)
	    if (sp->data.exec_data.tagged_union.running_data.posix_pid == pid)
		break;

	if (i == 0) {
	    /* Must be a hostdata postprocessor. */
	    dce_lock_unlock(sx_table.lock, &st);
	    dce_lock_justwrite(sx_pids.lock, &st);
	    for (i = 0; i < sx_pids.pids.count; i++)
		if (sx_pids.pids.list[i].pid == pid) {
		    sx_pids.pids.list[i].status = wait_stat;
		    sx_pids.pids.list[i].done = 1;
	    pthread_cond_broadcast(&sx_pids.cv);
		    break;
		}
	    dce_lock_unlock(sx_pids.lock, &st);
	    continue;
	}

	/* We were asked to shut this guy down, so do so. */
	if (sp->flags & server_c_exec__exiting) {
	    remove_server(sp, &st);
	    continue;
	}

	/* "Normal" exit? */
	if (WIFEXITED(wait_stat) && WEXITSTATUS(wait_stat) == 0) {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, ""), dhd_s_srvrexec,
		svc_c_sev_notice_verbose, dced_s_sx_server_exit,
		WEXITSTATUS(wait_stat));
	    remove_server(sp, &st);
	    continue;
	}
	/* Non-normal exit. */
	dce_svc_printf(DCED_S_SX_SERVER_EXIT_MSG,
		sp->data.name, WEXITSTATUS(wait_stat));

	/* Check restart-on-error flag */
	if (!(sp->data.fixed.startupflags & server_c_startup_on_failure)) {
	    remove_server(sp, &st);
	    continue;
	}

	/* XXX Open issue:
	 * we should check restart time to prevent thrashing. */
	dce_svc_printf(DCED_S_SX_STARTING_SERVER_MSG, sp->data.name);
	id = sp->data.exec_data.tagged_union.running_data.sc_uuid;
	fixedattr_copy(&fixed, &sp->data.fixed, dced_copy_via_malloc, &st);
	attr_list_copy(&attributes, &sp->data.attributes,
	    dced_copy_via_malloc, &st);
	remove_server(sp, &st);
	(*dced_srvrconf_v1_0_epv.srvrconf_start)
	    (NULL, &id, &fixed, &attributes, NULL, &st);
	dced_fixedattr_free_ptrs(&fixed);
	dced_attr_list_free_ptrs(&attributes);
    }
}

/* ______________________________________________________________________ */

void
srvrexec_started(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    srvrexec_ep_list_t		*eps,
  /* in,out */
    uuid_t			*instance,
  /* out */
    error_status_t		*st
)
{
    server_data_t		*sp;
    srvrexec_ep_t		*ep;
    service_t			*s;
    unsigned32			i;
    unsigned32			j;
    uuid_t			Prin;

    /* Set default output parameters. */
    if (instance == NULL || uuid_is_nil(instance, st)) {
	*st = dced_s_sx_missing_instance;
	return;
    }
    *st = error_status_ok;

    /* Find object. */
    dce_lock_riw(sx_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sx_table, instance, st)) == NULL) {
	dce_lock_unlock(sx_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission. */
    if (!dced_is_server) {
	dce_lock_unlock(sx_table.lock, st);
	*st = sec_acl_invalid_permission;
	return;
    }
    if (!dced_authorized(h, &sx_cont_type, &sx_acl, &Prin, dced_perm_insert, st)) {
	dce_lock_unlock(sx_table.lock, st);
	return;
    }

    if ( eps != NULL) { /* right now we pass NULL, so checking is a must */
	dce_lock_write(sx_table.lock, st);
	for (i = eps->count, ep = eps->list; i; i--, ep++)
	    for (j = sp->data.services.count, s = sp->data.services.list; j; j--, s++)
		if (IFID_EQUAL(&ep->ifid, &s->ifspec, st)) {
		    dced_tower_list_free_ptrs(&s->towers);
		    tower_list_copy(&s->towers, &ep->towers, dced_copy_via_malloc, st);
		    dce_db_store_by_uuid(sx_db,
					 &sp->data.exec_data.tagged_union.running_data.instance,
					 sp, st);
		    if (*st != error_status_ok)
			dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG,
				       "srvrexec_started", (long)*st);
		}
    }
    dce_lock_unlock(sx_table.lock, st);
}

/* ______________________________________________________________________ */

void
srvrexec_create(
  /* in */
    handle_t			h,
    server_t			*server,
  /* out */
    uuid_t			*instance,
    error_status_t		*st
)
{
    uuid_t			Prin;
    server_data_t		new;
    sec_acl_t			i_acl;
    uuid_t			au;

    /* Set default output parameters. */
    if (instance == NULL || uuid_is_nil(instance, st)) {
	*st = dced_s_sx_missing_instance;
	return;
    }
    if (srvr_find_by_uuid(&sx_table, instance, st) != NULL) {
	*st = dced_s_already_exists;
	return;
    }
    *st = error_status_ok;

    /* Check ACL for permission */
    if (!dced_is_server) {
	*st = sec_acl_invalid_permission;
	return;
    }
    if (!dced_authorized(h, &sx_cont_type, &sx_acl, &Prin, dced_perm_insert, st))
	return;

    if (server->name == NULL || *server->name == '\0') {
	*st = dced_s_name_missing;
	return;
    }

    /* Create an ACL for this object */
    uuid_create(&au, st);
    CHECK_STAT_RET(*st);
    dce_db_fetch_by_uuid(acl_db, &sx_do_acl, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_FETCH_IACL_MSG, "srvrexec_create",
		(long)*st);
	return;
    }
    dce_db_store_by_uuid(acl_db, &au, &i_acl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_CANT_STORE_ACL_MSG, "srvrexec_create",
		(long)*st);
	return;
    }

    /* Store the object. */
    dce_db_std_header_init(sx_db, &new.h, &new.data.id, &au, NULL, NULL, 0, st);
    CHECK_STAT_RET(*st);
    new.h.tagged_union.h.owner_id = Prin;
    new.used = TRUE;
    new.flags = 0;
    /* Don't need server_copy since srvr_add_to_table will do it for us. */
    srvr_qualify_prin_names(&server->prin_names, st);
    new.data = *server;
    new.data.exec_data.tagged_union.running_data.instance = *instance;
    dce_db_store_by_uuid(sx_db,
	&new.data.exec_data.tagged_union.running_data.instance, server, st);
    if (*st != error_status_ok) {
	error_status_t		xst;
	dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG, "srvrexec_create",
		(long)*st);
	dce_lock_unlock(sx_table.lock, &xst);
	return;
    }

    /* Add it to the table. */
    dce_lock_write(sx_table.lock, st);
    srvr_add_to_table(&sx_table, &new, st);
    dce_lock_unlock(sx_table.lock, st);
}

/* ______________________________________________________________________ */
void
srvrexec_exited(
  /* in */
    handle_t			h,
    uuid_t			*instance,
  /* out */
    error_status_t		*st
)
{
    error_status_t		st2;
    server_data_t		*sp;
    uuid_t			Prin;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Find object. */
    dce_lock_read(sx_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sx_table, instance, st)) == NULL) {
	dce_lock_unlock(sx_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (!dced_is_server) {
	dce_lock_unlock(sx_table.lock, &st2);
	*st = sec_acl_invalid_permission;
	return;
    }
    if (!dced_authorized(h, &sx_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_stop, st)) {
	dce_lock_unlock(sx_table.lock, &st2);
	return;
    }

    sp->flags |= server_c_exec__exiting;
    dce_lock_unlock(sx_table.lock, &st2);
}

/* ______________________________________________________________________ */
static void
get_random_binding(
    server_data_t		*sp,
    rpc_binding_handle_t	*bh,
    error_status_t		*st
)
{
    service_t			*s;
    unsigned32			i;
    sec_attr_twr_set_p_t	ts;
    uuid_t                      *op;
    uuid_t                      o;

    /* Pick a random service. */
    if (sp->data.services.count == 0) {
	*st = rpc_s_no_bindings;
	return;
    }
    i = RPC_RANDOM_GET(0, sp->data.services.count - 1);
    s = &sp->data.services.list[i];

    /* Pick a random towerset. */
    if (s->towers.count == 0) {
	*st = rpc_s_no_bindings;
	return;
    }
    i = RPC_RANDOM_GET(0, s->towers.count - 1);
    ts = s->towers.list[i];

    /* Pick a random tower. */
    if (ts->count == 0) {
	*st = rpc_s_no_bindings;
	return;
    }
    i = RPC_RANDOM_GET(0, ts->count - 1);
    rpc_tower_to_binding(ts->towers[i]->tower_octet_string, bh, st);
    if (*st != error_status_ok)
	return;

    /* Pick a random object if current obj uuid is nil */
    rpc_binding_inq_object(*bh, &o, st);
    if ( (s->objects.count == 0) || 
	 (*st != error_status_ok) ||
	 (!uuid_is_nil(&o, st)) ) {
	return;
    }
    i = RPC_RANDOM_GET(0, s->objects.count - 1);
    op = &s->objects.list[i];
    rpc_binding_set_object(*bh, op, st);
}

static void
kill_server(
    server_data_t		*sp,
    srvrexec_stop_method_t	method,
    error_status_t		*st
)
{
    int				i;
    int				s;
    pid_t			pid;
    rpc_binding_handle_t	bh;
    error_status_t		st2;

    sp->flags |= server_c_exec__exiting;

    if (method == srvrexec_stop_rpc) {
	get_random_binding(sp, &bh, st);
	if (*st != error_status_ok)
	    return;
	if (sp->data.prin_names.count > 0) {
	    rpc_binding_set_auth_info(bh, sp->data.prin_names.list[0],
		rpc_c_protect_level_default, rpc_c_authn_dce_secret,
		NULL, rpc_c_authz_dce, st);
	    if (*st != error_status_ok)
		return;
	}
	rpc_mgmt_stop_server_listening(bh, st);
	rpc_binding_free(&bh, &st2);
    } 
    else {
	switch ((int)method) {
	default:			s = SIGTERM;	break;
	case srvrexec_stop_hard:	s = SIGKILL;	break;
	case srvrexec_stop_error:	s = SIGABRT;	break;
	}
	pid = sp->data.exec_data.tagged_union.running_data.posix_pid;
	i = kill((pid_t)pid, s);
	if (i < 0)
	    dce_svc_printf(DCED_S_SX_KILL_FAILED_MSG, (long)pid, s,
		strerror(errno));
    }
}

void
srvrexec_stop(
  /* in */
    handle_t			h,
    uuid_t			*id_uuid,
    srvrexec_stop_method_t	method,
  /* out */
    error_status_t		*st
)
{
    server_data_t		*sp;
    uuid_t			Prin;
    error_status_t		st2;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Find object. */
    dce_lock_read(sx_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sx_table, id_uuid, st)) == NULL) {
	dce_lock_unlock(sx_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (!dced_is_server) {
	dce_lock_unlock(sx_table.lock, &st2);
	*st = sec_acl_invalid_permission;
	return;
    }
    if (!dced_authorized(h, &sx_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_stop, st)) {
	dce_lock_unlock(sx_table.lock, &st2);
	return;
    }

    kill_server(sp, method, st);
    dce_lock_unlock(sx_table.lock, &st2);
}

/* ______________________________________________________________________ */

void
srvrexec_shutdown(
  /* in */
    handle_t			h,
  /* out */
    error_status_t		*st
)
{
    server_data_t		*sp;
    uuid_t			Prin;
    unsigned16			i;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Check ACL for permission */
    if (!dced_is_server) {
	*st = sec_acl_invalid_permission;
	return;
    }
    dce_lock_read(sx_table.lock, st);
    for (sp = sx_table.list, i = sx_table.count; i; i--, sp++) {
	if (!sp->used)
	    continue;
	if (!dced_authorized(h, &sx_obj_type,
		&sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_stop, st))
	    break;
    }
    if (i != 0) {
	dce_lock_unlock(sx_table.lock, st);
	*st = sec_acl_invalid_permission;
	return;
    }

    for (sp = sx_table.list, i = sx_table.count; i; i--, sp++) {
	if (sp->used)
	    kill_server(sp, srvrexec_stop_rpc, st);
    }
    dce_lock_unlock(sx_table.lock, st);
    dced_sleep(5);

    /* Now really kill them. */
    dce_lock_read(sx_table.lock, st);
    for (sp = sx_table.list, i = sx_table.count; i; i--, sp++) {
	if (sp->used)
	    kill_server(sp, srvrexec_stop_hard, st);
    }
    dce_lock_unlock(sx_table.lock, st);

    /* And we exit. */
    rpc_mgmt_stop_server_listening(NULL, st);
}

/* ______________________________________________________________________ */

static void
srvrexec_munge_service(
  /* in */
    handle_t			h,
    uuid_t			*id,
    rpc_if_id_t			*ifid,
    unsigned32			flags,
  /* out */
    error_status_t		*st
)
{
    server_data_t		*sp;
    uuid_t			Prin;
    service_t			*s;
    unsigned32			i;
    error_status_t		st2;

    /* Set default output parameters. */
    *st = error_status_ok;

    /* Find object. */
    dce_lock_riw(sx_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sx_table, id, st)) == NULL) {
	dce_lock_unlock(sx_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (!dced_is_server) {
	dce_lock_unlock(sx_table.lock, &st2);
	*st = sec_acl_invalid_permission;
	return;
    }
    if (!dced_authorized(h, &sx_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_write, st)) {
	dce_lock_unlock(sx_table.lock, &st2);
	return;
    }

    /* Find interface ID, set flags. */
    *st = dced_s_not_found;
    dce_lock_write(sx_table.lock, &st2);
    for (i = sp->data.services.count, s = sp->data.services.list; i; i--, sp++)
	if (IFID_EQUAL(&s->ifspec, ifid, &st2)) {
	    s->flags = flags;
	    dce_db_store_by_uuid(sx_db,
		&sp->data.exec_data.tagged_union.running_data.instance, sp, st);
	    if (*st != error_status_ok)
		dce_svc_printf(DCED_S_CANT_STORE_OBJECT_MSG,
			"srvrexec_munge_service", (long)*st);
	    break;
	}
    dce_lock_unlock(sx_table.lock, &st2);
    
}


void
srvrexec_enable_service(
  /* in */
    handle_t			h,
    uuid_t			*id,
    rpc_if_id_t			*ifid,
  /* out */
    error_status_t		*st
)
{
    srvrexec_munge_service(h, id, ifid, (unsigned32)0, st);
}

void
srvrexec_disable_service(
  /* in */
    handle_t			h,
    uuid_t			*id,
    rpc_if_id_t			*ifid,
  /* out */
    error_status_t		*st
)
{
    srvrexec_munge_service(h, id, ifid, service_c_disabled, st);
}

/* ______________________________________________________________________ */

void
srvrexec_get_list(
  /* in */
    handle_t			h,
  /* out */
    dced_entry_list_t		*items,
    error_status_t		*st
)
{
    uuid_t			Prin;

    /* Set default output parameters. */
    items->count = 0;
    *st = error_status_ok;

    /* Check ACL for permission */
    if (!dced_authorized(h, &sx_cont_type, &sx_acl, &Prin, dced_perm_read, st))
	return;

    srvr_get_list(&sx_table, items, st);
}

/* ______________________________________________________________________ */

void
srvrexec_get(
  /* in */
    handle_t		h,
    uuid_t		*id_uuid,
  /* out */
    server_t		*server,
    error_status_t	*st
)
{
    server_data_t	*sp;
    uuid_t		Prin;
    error_status_t	st2;

    /* Set default output parameters. */
    *st = error_status_ok;
    *server = dced_g_null_server;

    /* Find object. */
    dce_lock_read(sx_table.lock, st);
    if ((sp = srvr_find_by_uuid(&sx_table, id_uuid, st)) == NULL) {
	dce_lock_unlock(sx_table.lock, st);
	*st = dced_s_not_found;
	return;
    }

    /* Check ACL for permission */
    if (!dced_authorized(h, &sx_obj_type,
	    &sp->h.tagged_union.h.acl_uuid, &Prin, dced_perm_read, st)) {
	dce_lock_unlock(sx_table.lock, &st2);
	return;
    }

    /* Set return value. */
    server_copy(server, &sp->data, dced_copy_for_stub, st);
    dce_lock_unlock(sx_table.lock, st);
}

/* ______________________________________________________________________ */
static void *
ping_via_kill(
    void		*arg
)
{
    pid_t		pid;
    error_status_t	st;
    server_data_t	*sp;
    unsigned32		i;

    for ( ; ; ) {
	dced_sleep(10);
	dce_lock_riw(sx_table.lock, &st);
	for (sp = sx_table.list, i = sx_table.count; i; i--, sp++) {
	    if (!sp->used)
		continue;
	    pid = sp->data.exec_data.tagged_union.running_data.posix_pid;
	    if (kill(pid, 0) < 0 && errno == ESRCH) {
		remove_server(sp, &st);
		dced_sleep(5);
		dce_lock_riw(sx_table.lock, &st);
	    }
	}
	dce_lock_unlock(sx_table.lock, &st);
    }
    return NULL;
}


void
dced_init_srvrexec(
    pthread_attr_t	thread_attr,
    error_status_t	*st
)
{
    pthread_t		tid;

    pthread_create(&tid, thread_attr, ping_via_kill, NULL);
    pthread_detach(&tid);
}

srvrexec_v1_0_epv_t dced_srvrexec_v1_0_epv = {
    srvrexec_get_list,
    srvrexec_get,
    srvrexec_started,
    srvrexec_create,
    srvrexec_exited,
    srvrexec_stop,
    srvrexec_shutdown,
    srvrexec_enable_service,
    srvrexec_disable_service
};
