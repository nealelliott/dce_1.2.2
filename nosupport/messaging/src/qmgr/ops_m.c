/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Queue manager: Management operations
 * ______________________________________________________________________
 *
 * $Log: ops_m.c,v $
 * Revision 1.6  1996/11/01  20:37:23  bowe
 * Implement MOS_mgmt_move() operation.
 *
 * Revision 1.5  1996/10/31  20:10:50  bowe
 * In MOS_mgmt_create_queue(), check that aliases of new queue
 * are not in use already.
 *
 * Revision 1.4  1996/10/17  15:18:21  bowe
 * Return more info in MOS_mgmt_info().
 *
 * Revision 1.3  1996/10/16  00:42:29  bowe
 * Set create time in qattr in MOS_mgmt_create. (was missing)
 *
 * Revision 1.2  1996/10/15  01:26:11  bowe
 * Added an arg to acl_for_new_queue)().
 *
 * Revision 1.1  1996/09/27  18:05:02  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include "mosdefs.h"

/* ______________________________________________________________________ */
/* mos_mgmt_create_queue  --  create queue with given attributes.
 */

void
MOS_mgmt_create_queue(
  /* [in] */
    handle_t		h,
  /* [in,out] */
    mos_qattr_t		*qattr,
  /* [out] */
    error_status_t	*st
)
{
    Queue		*q;
    mos_string_t	*ap;
    uuid_t		prin;
    unsigned32		i;

    *st = error_status_ok;
#if DEBUG
fprintf(stderr, "OP: mos_create_queue: %s\n", qattr->name);
#endif

    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_create, st)) {
	*qattr = null_qattr;
	return;
    }

    if ( uuid_is_nil(&qattr->id, st) ) uuid_create(&qattr->id, st);

    /* If name is null (or ""), we make up a name. */
    if (qattr->name == NULL || qattr->name[0] == NULL) {
	/* uuid_to_string allocates the string for us (with malloc) */
	uuid_to_string(&qattr->id, &qattr->name, st);
    }
    /* see if queue by that name already exists */
    if ( q = find_queue_by_name(qattr->name, st) ) {
	*st = mos_queue_already_exists;
	*qattr = q->qattr;
	return;
    }
    /* check uniqueness amongst alias names */
    for (i=0,ap=qattr->aliases.list; i<qattr->aliases.count; i++, ap++) {
	if ( q=find_queue_by_name(*ap, st) ) {
	    *st = mos_queue_already_exists;
	    *qattr = q->qattr;
	    return;
	}
    }

    /* Initialize non-user-writable attributes. */
    uuid_create(&qattr->acl, st);
    (void)utc_gettime(&qattr->created);
    qattr->len = 0;

    q = cache_queue_info(qattr->name, qattr, st);
    if (*st != error_status_ok) return;

    /* Store queue attributes in backing store */
    dce_db_store_by_name(qattr_db, (char *)qattr->name, qattr, st);
    if (*st != error_status_ok)
	dce_svc_printf(MOS_S_CANT_STORE_DATA_MSG,
		"mos_mgmt_create_queue", (long)*st);

    dce_acl_inq_prin_and_group(h, &prin, NULL, st);
/* XXX - handle error */
    if (*st != error_status_ok) return;
    acl_for_new_queue(h, &qattr->acl, &prin, st);
    if (*st != error_status_ok)
	dce_svc_printf(MOS_S_CANT_STORE_ACL_MSG,
		"mos_mgmt_create_queue", (long)*st);

    dce_svc_printf(MOS_S_QUEUE_CREATED_MSG, (char *)qattr->name);
    create_count++;
}

/* ______________________________________________________________________ */
/* mos_mgmt_get_id_list  --  return a list of IDs of all queues
 */

void
MOS_mgmt_get_id_list(
  /* [in] */
    handle_t		h,
  /* [out] */
    mos_uuid_list_t	*idlist,
    error_status_t	*st
)
{
    Queue		*q;
    unsigned32		n;

    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_rdattr, st))
	return;

    *st = error_status_ok;
    /* count queues */
    for (n=0,q=the_queues; q; q=q->next)
	n++;

    idlist->count = n;
    if ( n == 0 ) return;

    /* XXX - can we use NEW_client() here? */
    idlist->list = rpc_sm_allocate( (sizeof(*idlist->list) * n), st);
#if DEBUG
fprintf(stderr, "OP: mos_mgmt_get_id_list %ld\n", n);
#endif
    for (n=0,q=the_queues; q; q=q->next,n++)
	idlist->list[n] = q->qattr.id;
}

/* ______________________________________________________________________ */
/* mos_mgmt_get_name_list  --  return a list of names of all queues
 */

void
MOS_mgmt_get_name_list(
  /* [in] */
    handle_t		h,
  /* [out] */
    mos_string_list_t	*names,
    error_status_t	*st
)
{
    Queue		*q;
    unsigned32		n;

    if (!mos_authorized(h, &mgmt_type, &mgmt_acl, mos_perm_rdattr, st))
	return;

    *st = error_status_ok;
    /* count queues */
    for (n=0,q=the_queues; q; q=q->next)
	n++;

    names->count = n;
    if ( n == 0 ) return;

    names->list = rpc_sm_allocate( (sizeof(*names->list) * n), st);
#if DEBUG
fprintf(stderr, "OP: mos_mgmt_get_name_list %ld\n", n);
#endif
    for (n=0,q=the_queues; q; q=q->next,n++)
	names->list[n] = strdup_for_client(q->qattr.name, st);;
}

/* ______________________________________________________________________ */
/* mos_mgmt_rename  --  rename a queue
 */
void
MOS_mgmt_rename(
  /* [in] */
    handle_t		h,
    idl_char		*from_name,
    idl_char		*to_name,
/*    unsigned32		flags,*/
  /* [out] */
    error_status_t	*st
)
{
    Queue	*q;
    *st = error_status_ok;
#if DEBUG
fprintf(stderr, "OP: mos_mgmt_rename: %s -> %s\n", from_name, to_name);
#endif

    if ( (q = find_queue_by_name(from_name, st)) == NULL )
	return;

    if ( find_queue_by_name(to_name, st) ) {
	*st = mos_queue_already_exists;
	return;
    }

    /* rename in memory */
    free(q->qattr.name);
    q->qattr.name = (idl_char *)strdup((char *)to_name);

    /* rename in backing store */
    dce_db_delete_by_name(qattr_db, (char *)from_name, st);
    dce_db_store_by_name(qattr_db, (char *)to_name, &q->qattr, st);
}

/* ______________________________________________________________________ */
/* mos_mgmt_info  --  return a bunch of info about a qmgr.
 */

void
MOS_mgmt_info(
  /* [in] */
    handle_t		h,
  /* [out] */
    mos_kv_pair_list_t
			*info,
    error_status_t	*st
)
{
    Queue		*q;
    unsigned32		i, n;

    *st = error_status_ok;
    /* Number must be at least the amount of space used below! */
    info->count = 9;
    info->list = NEW_client(mos_kv_pair_t, info->count, st);
    if (*st != error_status_ok) {
	info->count = 0;
	return;
    }

    info->list[n].key   = (idl_char *)"Qmgr-Prin";
    info->list[n].value.type   = mos_c_info_string;
    info->list[n].value.tagged_union.s = qmgr_prin;
    n++;

    info->list[n].key   = (idl_char *)"NS-Entry";
    info->list[n].value.type   = mos_c_info_string;
    info->list[n].value.tagged_union.s = ns_entry;
    n++;

    info->list[n].key   = (idl_char *)"Key-File";
    info->list[n].value.type   = mos_c_info_string;
    info->list[n].value.tagged_union.s = qmgr_keyfile;
    n++;

    info->list[n].key   = (idl_char *)"Start-Time";
    info->list[n].value.type   = mos_c_info_utc;
    info->list[n].value.tagged_union.t = start_time;
    n++;

    for (i=0,q=the_queues; q; q=q->next) i++;
    info->list[n].key   = (idl_char *)"Queues";
    info->list[n].value.type   = mos_c_info_u32;
    info->list[n].value.tagged_union.i = i;
    n++;

    info->list[n].key   = (idl_char *)"Create-Count";
    info->list[n].value.type   = mos_c_info_u32;
    info->list[n].value.tagged_union.i = create_count;
    n++;

    info->list[n].key   = (idl_char *)"Enqueue-Count";
    info->list[n].value.type   = mos_c_info_u32;
    info->list[n].value.tagged_union.i = enqueue_count;
    n++;

    info->list[n].key   = (idl_char *)"Dequeue-Count";
    info->list[n].value.type   = mos_c_info_u32;
    info->list[n].value.tagged_union.i = dequeue_count;
    n++;

    info->list[n].key   = (idl_char *)"Object-UUID";
    info->list[n].value.type   = mos_c_info_uuid;
    info->list[n].value.tagged_union.uuid = qmgr_object_uuid;
    n++;

}

/* ______________________________________________________________________ */
/* mos_mgmt_move  --  move queue's items to another queue
 */

void
MOS_mgmt_move(
  /* [in] */
    handle_t		h,
    idl_char		*from_name,
    idl_char		*to_name,
/*    unsigned32		flags,*/
  /* [out] */
    error_status_t	*st
)
{
    Queue		*q_from, *q_to;
    QItem		*qi;
    mos_mattr_t		*mattr;

    *st = error_status_ok;
#if DEBUG
fprintf(stderr, "OP: mos_mgmt_move: %s -- %s\n", from_name, to_name);
#endif

    if ( (q_from = find_queue_by_name(from_name, st)) == NULL )
	return;
    if ( (q_to = find_queue_by_name(to_name, st)) == NULL ) {
	/* Does not exist.  Create it. */
	/* XXX - we may want a flag to say what to do here */
	return;		/* XXX - just return for now */
    }

    lock_write(q_from->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_UNLOCK_FAILED_MSG);
    lock_write(q_to->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_UNLOCK_FAILED_MSG);

    move_items(q_from, q_to);

    lock_unlock(q_from->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_UNLOCK_FAILED_MSG);
    lock_unlock(q_to->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_UNLOCK_FAILED_MSG);
}

/* ______________________________________________________________________ */

/* Entry-point vector - so the qmgr can call other qmgrs. */
mos_mgmt_v1_0_epv_t MOS_mgmt_v1_0_epv = {
    MOS_mgmt_create_queue,
    MOS_mgmt_get_id_list,
    MOS_mgmt_get_name_list,
    MOS_mgmt_rename,
    MOS_mgmt_info,
    MOS_mgmt_move
};

/* ______________________________________________________________________ */

/*
Print_queues()
{
    Queue	*q;
    for (q=the_queues; q; q=q->next) {
	printf("++++++++++++++++\n");
	Print_qattr(&q->qattr);
    }
}
*/
