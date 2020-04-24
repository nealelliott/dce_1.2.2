/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Misc utility functions.
 * ______________________________________________________________________
 *
 * $Log: utils.c,v $
 * Revision 1.4  1996/10/18  19:39:35  bowe
 * Add object vector to all register/unregister calls.
 *
 * Revision 1.3  1996/10/17  15:15:24  bowe
 * Added function value_from_config_file().  Use more generalized
 * obj uuid (for exported object vector).
 *
 * Revision 1.2  1996/10/01  16:42:07  bowe
 * Implement cleanup of expired messages, idle timedout queues in
 * the maint thread.
 *
 * Revision 1.1  1996/09/27  18:07:16  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include "mosdefs.h"

#include <dce/uuid.h>
#include <dce/dceacl.h>
#include <dce/aclif.h>
#include <dce/keymgmt.h>

extern rdaclif_v1_0_epv_t dce_acl_v1_0_epv;

dce_db_handle_t qattr_db, qdata_db, acl_db;

sec_login_handle_t	login_ctx;

rpc_binding_vector_t	*mos_bind_vec;

/* ______________________________________________________________________ */
/* Get a value (as string) from the qmgr's config file.  The value is
 * copied into the caller-supplied buffer.  A pointer to that buffer is
 * the return value.  If no lines in the file match, return null.
 * Lines are of the form "Keyname: value".
 * Lines starting with '#' and blank lines are ignored.
 */
/* XXX - Yes, this is very inefficient if it is called more than once.
 * We should cache the stuff in memory.  Do this when this is called
 * in more than one place...
 */

char *
value_from_config_file(
  /* in */
    char *key,
  /* out */
    char *value
)
{
    FILE *fp;
    char buf[200], *cp;
    int  len;

    if ( !(fp=fopen(private_data, "r")) )
	return NULL;

    len = strlen(key);
    while (fgets(buf, 200, fp)) {
	if (buf[0] == '#' || buf[0] == '\n') continue;
	if (!strncmp(buf, key, len) && buf[len] == ':') {
	    if (cp = strchr(buf, '\n')) *cp = NULL;	/* trailing newline */
	    cp = strchr(buf, ':');
	    cp++;
	    while (*cp && isspace(*cp)) cp++;
	    if (cp) {
		strcpy(value, cp);
		return value;
	    }
	    break;
	}
    }
    fclose(fp);
}

/* ______________________________________________________________________ */

static boolean32
auth_fn(handle_t h, unsigned32 op, error_status_t *st)
{
    *st = error_status_ok;
    if (op == rpc_c_mgmt_stop_server_listen) { /* XXX */ }
	return TRUE;
}

/* ______________________________________________________________________ */

/* Log in to DCE. */

void
dce_login(boolean manage_key)
{
    sec_passwd_rec_t		*pwd;
    boolean32			reset;
    sec_login_auth_src_t	auth_src;
    char			*keyfile;
    error_status_t              st;

    keyfile = (char *)qmgr_keyfile;

    /* Get this server prin's key from keytab. */
    sec_key_mgmt_get_key(rpc_c_authn_dce_secret, keyfile, qmgr_prin, 0,
	(void **)&pwd, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "sec_key_mgmt_get_key", (long)st);

    /* set server's identity */
    sec_login_setup_identity(qmgr_prin, sec_login_no_flags, &login_ctx, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "sec_login_setup_identity", (long)st);
    sec_login_validate_identity(login_ctx, pwd, &reset, &auth_src, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "sec_login_validate_identity", (long)st);
    sec_key_mgmt_free_key(pwd, &st);

    sec_login_set_context(login_ctx, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "sec_login_set_context", (long)st);

    rpc_server_register_auth_info(qmgr_prin, rpc_c_authn_dce_secret, NULL,
	keyfile, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "rpc_server_register_auth_info", (long)st);

    /* spawn a thread to manage our key */
    if (manage_key) {
	void		*kmgmt_thread(void *);
	pthread_t	tid;
	if (pthread_create(&tid, pthread_attr_default, kmgmt_thread, NULL) < 0){
	    dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_ER_MSG,
		       "pthread_create", (long)errno);
	    return;
	}
	pthread_detach(&tid);
    }
}

/* ______________________________________________________________________ */

static uuid_vector_t	obj_vec;

/* Do all the server registration stuff. */

void
register_server( void )
{
    extern serviceability_v1_0_epv_t	my_svc_v1_0_epv;
    extern mos_q_v1_0_epv_t		MOS_q_v1_0_epv;
    extern mos_mgmt_v1_0_epv_t		MOS_mgmt_v1_0_epv;
    error_status_t			st;

    obj_vec.count = 1;
    obj_vec.uuid[0] = &qmgr_object_uuid;

#if 0
    rpc_server_register_auth_info((unsigned_char_t *)qmgr_prin,
	rpc_c_authn_default, NULL, qmgr_keyfile, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_server_register_auth_info", (long)st);
#endif

    rpc_server_register_if(mos_q_v1_0_s_ifspec, NULL,
	(rpc_mgr_epv_t) &MOS_q_v1_0_epv, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_server_register_if", (long)st);
    rpc_server_register_if(mos_mgmt_v1_0_s_ifspec, NULL,
	(rpc_mgr_epv_t) &MOS_mgmt_v1_0_epv, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_server_register_if (mgmt)", (long)st);

    rpc_server_register_if(rdaclif_v1_0_s_ifspec, NULL,
	(rpc_mgr_epv_t) &dce_acl_v1_0_epv, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_server_register_if (rdaclif)", (long)st);

    rpc_server_register_if(serviceability_v1_0_s_ifspec, NULL,
	(rpc_mgr_epv_t) &my_svc_v1_0_epv, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_server_register_if (svc)", (long)st);

    rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_server_use_all_protseqs", (long)st);
    rpc_server_inq_bindings(&mos_bind_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_server_inq_bindings", (long)st);

    /* XXX - use rpc_ep_register_no_replace() ? */
    rpc_ep_register(mos_q_v1_0_s_ifspec, mos_bind_vec, &obj_vec,
	(unsigned_char_t *)"Q-mgr: Queue Data", &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_ep_register", (long)st);
    rpc_ep_register(mos_mgmt_v1_0_s_ifspec, mos_bind_vec, &obj_vec,
	(unsigned_char_t *)"Q-mgr: Queue Management", &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_ep_register (mgmt)", (long)st);
    rpc_ep_register(rdaclif_v1_0_s_ifspec, mos_bind_vec, &obj_vec,
	(unsigned_char_t *)"Q-mgr: ACL manager functions", &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_ep_register (rdaclif)", (long)st);
    rpc_ep_register(serviceability_v1_0_s_ifspec, mos_bind_vec, &obj_vec,
	(unsigned_char_t *)"Q-mgr: SVC functions", &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_ep_register (svc)", (long)st);

    rpc_ns_binding_export(rpc_c_ns_syntax_default, ns_entry,
	mos_q_v1_0_s_ifspec, mos_bind_vec, &obj_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_ns_binding_export", (long)st);
    rpc_ns_binding_export(rpc_c_ns_syntax_default, ns_entry,
	mos_mgmt_v1_0_s_ifspec, mos_bind_vec, &obj_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_ns_binding_export (mgmt)", (long)st);

    rpc_mgmt_set_authorization_fn(auth_fn, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"rpc_mgmt_set_authorization_fn", (long)st);
}

/* ______________________________________________________________________ */

void
unregister_server( void )
{
    error_status_t	st;

    rpc_ns_binding_unexport(rpc_c_ns_syntax_default, ns_entry,
        mos_q_v1_0_s_ifspec, NULL, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_SHUTDOWN_FUNCTION_FAIL_MSG,
		"rpc_ns_binding_unexport", (long)st);
    rpc_ns_binding_unexport(rpc_c_ns_syntax_default, ns_entry,
        mos_mgmt_v1_0_s_ifspec, NULL, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_SHUTDOWN_FUNCTION_FAIL_MSG,
		"rpc_ns_binding_unexport (mgmt)", (long)st);

    rpc_ep_unregister(mos_q_v1_0_s_ifspec, mos_bind_vec, &obj_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_SHUTDOWN_FUNCTION_FAIL_MSG,
		"rpc_ep_unregister", (long)st);
    rpc_ep_unregister(mos_mgmt_v1_0_s_ifspec, mos_bind_vec, &obj_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_SHUTDOWN_FUNCTION_FAIL_MSG,
		"rpc_ep_unregister (mgmt)", (long)st);
    rpc_ep_unregister(rdaclif_v1_0_s_ifspec, mos_bind_vec, &obj_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_SHUTDOWN_FUNCTION_FAIL_MSG,
		"rpc_ep_unregister (ACL)", (long)st);
    rpc_ep_unregister(serviceability_v1_0_s_ifspec, mos_bind_vec, &obj_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_SHUTDOWN_FUNCTION_FAIL_MSG,
		"rpc_ep_unregister (SVC)", (long)st);

    rpc_binding_vector_free(&mos_bind_vec, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_SHUTDOWN_FUNCTION_FAIL_MSG,
		"rpc_binding_vector_free", (long)st);
}

/* ______________________________________________________________________ */

/* Open databases.  The flag says whether to create them or not.
 * (It's set to true if the qmgr is run with "-i" (initialize).)
 */
void
open_databases(boolean create_db)
{
    error_status_t	st;

    if ( create_db ) {
	dce_db_open(qattrdbname, NULL, db_c_index_by_name | db_c_create,
	    (dce_db_convert_func_t)mos_qattr_convert, &qattr_db, &st);
	if (st != error_status_ok)
	    dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_db_open(qattrdb)", (long)st);
	dce_db_open(qdatadbname, NULL, db_c_index_by_uuid | db_c_create,
	    (dce_db_convert_func_t)mos_q_item_convert, &qdata_db, &st);
	if (st != error_status_ok)
	    dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_db_open(qdatadb)", (long)st);
	dce_db_open(acldbname, NULL, db_c_index_by_uuid | db_c_create,
	    (dce_db_convert_func_t)dce_rdacl_convert, &acl_db, &st);
	if (st != error_status_ok)
	    dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_db_open(acldb)", (long)st);
    }

    dce_db_open(qattrdbname, NULL, db_c_index_by_name,
	(dce_db_convert_func_t)mos_qattr_convert, &qattr_db, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_db_open(qattrdb)", (long)st);
    dce_db_open(qdatadbname, NULL, db_c_index_by_uuid,
	(dce_db_convert_func_t)mos_q_item_convert, &qdata_db, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_db_open(qdatadb)", (long)st);
    dce_db_open(acldbname, NULL, db_c_index_by_uuid,
	(dce_db_convert_func_t)dce_rdacl_convert, &acl_db, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_db_open(acldb)", (long)st);
}

void
close_databases(void)
{
    error_status_t	st;

    dce_db_close(&qattr_db, &st);
    dce_db_close(&qdata_db, &st);
    dce_db_close(&acl_db, &st);
}

/* ______________________________________________________________________ */

/* These allocate memory that will be freed when an RPC returns. */

idl_char *
strdup_for_client(
    idl_char		*p,
    error_status_t	*st
)
{
    idl_char            *new;

    new = rpc_sm_allocate(strlen((char *)p) + 1, st);
    if (*st != error_status_ok)
	return NULL;
    return (idl_char *)strcpy((char *)new, (char *)p);
}

void *
memdup_for_client(
    void		*p2,
    unsigned32		size,
    error_status_t	*st
)
{
    void		*p1;
    p1 = rpc_sm_allocate(size, st);
    if (*st != error_status_ok)
	return NULL;
    return memcpy(p1, p2, (size_t)size);
}

void *
memdup(
    void		*p2,
    unsigned32		size
)
{
    void		*p1;
    if ((p1=malloc((size_t)size)) == NULL)
	return NULL;
    return memcpy(p1, p2, (size_t)size);
}

/* ______________________________________________________________________ */

/* Maintenance thread.  Does various timeout checking tasks. */

void *
maint_thread(void *arg)
{
    struct timespec	delay;
    Queue		*q;
    QItem		*qi, *prev_qi;
    utc_t		delta;
    enum utc_cmptype	rel;
    error_status_t	st;

/* XXX - don't forget to add locking! */
/* XXX - decide how often to wake up */
    delay.tv_sec  = 60;
    delay.tv_nsec = 0;
    while (1) {
	pthread_delay_np(&delay);

	for (q=the_queues; q; q=q->next) {
	    if (q->qattr.len > 0) {
	    /* look for expired messages */
		for (qi=q->qi,prev_qi=NULL; qi; qi=qi->next,prev_qi=qi) {
		    if ( !(qi->item.mattr.flags & mos_c_use_expire_time) )
			continue;
		    if (utc_cmpmidtime(&rel, &qi->item.mattr.expire_time, NULL)
				< 0)
			continue;
		    if (rel == utc_lessThan) {
			if (prev_qi) prev_qi->next = qi->next;
			else q->qi = qi->next;
			/* free qi's storage */
			q->qattr.len--;
			free_qitem(qi, &st);
			if (st != error_status_ok)
			{}				/* XXX - report error */
			dce_svc_printf(MOS_S_MSG_EXPIRED_MSG, q->qattr.name);
		    }
		}
	    }

	    /* check empty queues - if empty_timeout has happened, remove 'em */
	    if (q->qattr.len > 0 || !(q->flags & mos_c_use_empty_timeout) == 0)
		continue;
	    if (utc_addtime(&delta, &q->qattr.last_activity,
			&q->qattr.empty_timeout) < 0)
		continue;
	    if (utc_cmpmidtime(&rel, &delta, NULL) < 0)
		continue;
	    if (rel == utc_lessThan) {
		MOS_q_delete_queue(NULL, q->qattr.name, 0, &st);
		{}				/* XXX - report error */
		dce_svc_printf(MOS_S_IDLE_QUEUE_REMOVED_MSG, q->qattr.name);
	    }

	}
    }
}

/* ______________________________________________________________________ */

/* Key management thread.  Just calls sec_key_mgmt_manage_key, which never
 * returns.
 */

void *
kmgmt_thread(void *arg)
{
    error_status_t	st;
    sec_key_mgmt_manage_key(rpc_c_authn_dce_secret, qmgr_keyfile, qmgr_prin, &st);
    dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
	"sec_key_mgmt_manage_key", (long)st);
}

/* ______________________________________________________________________ */
