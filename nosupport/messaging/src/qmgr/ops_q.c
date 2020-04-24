/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Queue manager: data operations
 * ______________________________________________________________________
 *
 * $Log: ops_q.c,v $
 * Revision 1.12  1996/11/26  17:18:29  bowe
 * Added flags param to MOS_q_move_item().
 *
 * Revision 1.11  1996/11/14  20:54:59  bowe
 * Initial implementation of mos_q_move_item().
 *
 * Revision 1.10  1996/11/11  21:58:10  bowe
 * Accomodate extra arg to enqueue_ack().
 *
 * Revision 1.9  1996/11/07  15:00:48  bowe
 * In mos_q_retrieve_mattr, handle empty queue correctly.
 *
 * Revision 1.8  1996/10/31  20:11:37  bowe
 * Rework mos_q_get_id_list() to take a selection mask.
 * Add mos_q_get_id_list_all(), which returns all IDs.
 *
 * Revision 1.7  1996/10/22  19:45:14  bowe
 * Fixe MOS_q_dequeue when using selection masks.
 *
 * Revision 1.6  1996/10/22  14:49:12  bowe
 * In MOS_q_purge, check for notice_dest before calling enqueue_ack().
 *
 * Revision 1.5  1996/10/17  15:16:54  bowe
 * Added enqueue_count,dequeue_count handling.
 * Send ack (via enqueue_ack()) when msg is purged.
 *
 * Revision 1.4  1996/10/07  19:50:07  bowe
 * Implement MOS_q_purge.
 * Minor fix to MOS_q_peek.
 *
 * Revision 1.3  1996/10/03  19:23:17  bowe
 * Don't set last_activity when getting qattr.  *Do* set it in ping.
 *
 * Revision 1.2  1996/10/03  18:46:17  bowe
 * Adjust for new notice options symbols.
 *
 * Revision 1.1  1996/09/27  18:05:02  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include "mosdefs.h"

/* ______________________________________________________________________ */
/* mos_q_enqueue  --  Enqueue a message.
 */

void
MOS_q_enqueue(
  /* [in] */
    handle_t			h,
    idl_char			*name,
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    unsigned32			flags,
  /* [out] */
    uuid_t			*id,
    error_status_t		*st
)
{
    Queue		*q;

#if DEBUG
fprintf(stderr, "OP: mos_q_enqueue %s: %ld bytes\n  [%s]\n",
 name, body->size, body->data);
#endif

    *st = error_status_ok;

    if ( (q = find_queue_by_name(name, st)) == NULL )
	return;

    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_enqueue, st))
	return;

    /* Enqueuing enabled? */
    if (!q->qattr.enq_enabled) {
	*st = mos_enqueue_disabled;
	return;
    }

    /* Check queue limits - length and size */
    if (q->qattr.max_qlen > 0 && q->qattr.len >= q->qattr.max_qlen) {
	*st = mos_queue_full;
	return;
    }
    if (q->qattr.max_msgsize > 0 && body->size >= q->qattr.max_msgsize) {
	*st = mos_msg_too_big;
	return;
    }

    find_initiator(h, &mattr->sender, st);

    /* if queue says no persistence and message wants, kick back error. */
    if (q->qattr.persistence == mos_c_que_attr_volatile &&
		mattr->persistence == mos_c_msg_attr_persistent) {
	*st = mos_no_persistence;
	return;
    }

    lock_write(q->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_LOCK_FAILED_MSG);

    (void)utc_gettime(&mattr->time_enqueued);
    if ( uuid_is_nil(&mattr->id, st) ) uuid_create(&mattr->id, st);
    *id = mattr->id;		/* ID is returned to caller, for convenience */

    if (q->qattr.persistence == mos_c_que_attr_persistent &&
		mattr->persistence != mos_c_msg_attr_persistent)
	mattr->persistence = mos_c_msg_attr_persistent;

    enqueue_item(q, mattr, body, TRUE);

    lock_unlock(q->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_UNLOCK_FAILED_MSG);
    lock_read(q->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_LOCK_FAILED_MSG);

    lock_unlock(q->lock, st);
    if (*st != error_status_ok)
	    dce_svc_printf(MOS_S_MUTEX_UNLOCK_FAILED_MSG);

    if (q->blocked) {
	pthread_cond_signal(&q->deq_cond);
    }
    enqueue_count++;
    if (mattr->notice_opts & mos_c_msg_attr_notice_enqueue &&
		mattr->notice_dest) {
fprintf(stderr, "Enqueue notice to: %s\n", mattr->notice_dest);
	/* XXX - send reply */
	enqueue_ack(mattr->notice_dest, mattr,
		mos_c_msg_attr_notice_enqueue, st);
    }
}

/* ______________________________________________________________________ */

static void
null_out_reply( mos_mattr_t *mattr, mos_datatype_bytearray_t *body )
{
    if (mattr) *mattr = null_mattr;
    if (body) *body  = null_body;
}

void
dequeue_the_data(
  /* [in] */
    Queue			*q,
    mos_mask_list_t		*masks,
    unsigned32			flags,
  /* [out] */
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    error_status_t		*st
)
{
    QItem		*qi, *prev_qi;

    /* no mask - get first one */
    if (masks == NULL || masks->count == 0) {
	qi = q->qi;
	copy_mattr_for_client(mattr, &qi->item.mattr);
	body->size =  qi->item.body.size;
	body->data = memdup_for_client(qi->item.body.data,
		qi->item.body.size, st);
	q->qi = qi->next;
	q->qattr.len--;
	free_qitem(qi, st);
    }
    else {
	find_item_by_selmask(masks, q, &qi, &prev_qi, st);
	if (*st != error_status_ok || qi == NULL) {
	    null_out_reply(mattr, body);
	    return;
	}
	qi = q->qi;
	copy_mattr_for_client(mattr, &qi->item.mattr);
	body->size =  qi->item.body.size;
	body->data = memdup_for_client(qi->item.body.data,
		qi->item.body.size, st);
	/* XXX - take it out of middle of queue... */
	free_qitem(qi, st);
    }

    if (mattr->notice_opts & mos_c_msg_attr_notice_dequeue &&
		mattr->notice_dest) {
fprintf(stderr, "Dequeue notice to: %s\n", mattr->notice_dest);
	/* XXX - send reply */
	enqueue_ack(mattr->notice_dest, mattr,
		mos_c_msg_attr_notice_dequeue, st);
    }
}

static void
blocked_dequeue(
    Queue			*q,
    mos_mask_list_t		*masks,
    unsigned32			flags,
  /* [out] */
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    error_status_t		*st
)
{
    q->blocked = TRUE;
    if (pthread_cond_wait(&q->deq_cond, &q->deq_mutex) , 0) {
	dce_svc_printf(MOS_S_COND_WAIT_FAILED_MSG);
	*st = mos_s_cond_wait_failed;
	return;
    }
    dequeue_the_data(q, masks, flags, mattr, body, st);
    q->blocked = FALSE;
}

/* ______________________________________________________________________ */
/* mos_q_dequeue  --  dequeue a message, given a selection mask.
 */

void
MOS_q_dequeue(
  /* [in] */
    handle_t			h,
    idl_char			*name,
    mos_mask_list_t		*masks,
    unsigned32			flags,
  /* [out] */
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    error_status_t		*st
)
{
    Queue		*q;
    QItem		*qi, *prev_qi;

#if DEBUG
fprintf(stderr, "OP: mos_q_dequeue %s\n", name);
#endif

    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	null_out_reply(mattr, body);
	return;
    }
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_dequeue, st)) {
	null_out_reply(mattr, body);
	return;
    }
    /* Dequeuing enabled? */
    if (!q->qattr.deq_enabled) {
	null_out_reply(mattr, body);
	*st = mos_dequeue_disabled;
	return;
    }

    (void)utc_gettime(&q->qattr.last_activity);

    if (q->qattr.len == 0) {
	if (flags & mos_c_deqmode_wait) {
	    blocked_dequeue(q, masks, flags, mattr, body, st);
	}
	else {
	    null_out_reply(mattr, body);
	    *st = mos_empty_queue;
	}
	return;
    }
    *st = error_status_ok;

    /* no mask - get first one */
    if (masks == NULL || masks->count == 0) {
	qi = q->qi;
	copy_mattr_for_client(mattr, &qi->item.mattr);
	body->size =  qi->item.body.size;
	body->data = memdup_for_client(qi->item.body.data,
		qi->item.body.size, st);
	q->qi = qi->next;
	q->qattr.len--;
	free_qitem(qi, st);
    }
    else {
	find_item_by_selmask(masks, q, &qi, &prev_qi, st);
	if (*st != error_status_ok || qi == NULL) {
	    null_out_reply(mattr, body);
	    return;
	}

	/* take item out of linked list */
	if (prev_qi)		/* not the first in queue */
	    prev_qi->next = qi->next;
	else			/* first in queue */
	    q->qi = qi->next;
	q->qattr.len--;
	copy_mattr_for_client(mattr, &qi->item.mattr);
	body->size =  qi->item.body.size;
	body->data = memdup_for_client(qi->item.body.data,
		qi->item.body.size, st);
	free_qitem(qi, st);
    }

    if (mattr->notice_opts & mos_c_msg_attr_notice_dequeue &&
		mattr->notice_dest) {
	/* XXX - send reply */
fprintf(stderr, "Dequeue notice to: %s\n", mattr->notice_dest);
	enqueue_ack(mattr->notice_dest, mattr,
		mos_c_msg_attr_notice_dequeue, st);
    }
    dequeue_count++;
}

/* ______________________________________________________________________ */
/* mos_q_dequeue_next  --  dequeue the next message on a queue.
 * Sort of an optimized version of mos_q_dequeue.
 * If none available, return error.  (ie, this does not block.)
 */
void
MOS_q_dequeue_next(
  /* [in] */
    handle_t			h,
    idl_char			*name,
  /* [out] */
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    error_status_t		*st
)
{
    Queue		*q;
    QItem		*qi;
#if DEBUG
fprintf(stderr, "OP: mos_q_dequeue_next %s\n", name);
#endif
    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	null_out_reply(mattr, body);
	return;
    }
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_dequeue, st)) {
	null_out_reply(mattr, body);
	return;
    }
    /* Dequeuing enabled? */
    if (!q->qattr.deq_enabled) {
	null_out_reply(mattr, body);
	*st = mos_dequeue_disabled;
	return;
    }

    /* Is queue empty? */
    if(!q->qi) {
	null_out_reply(mattr, body);
	*st = mos_empty_queue;
	return;
    }

    qi = q->qi;
    q->qi = qi->next;
    q->qattr.len--;
    copy_mattr_for_client(mattr, &qi->item.mattr);
    body->size =  qi->item.body.size;
    body->data = memdup_for_client(qi->item.body.data, qi->item.body.size, st);
    free_qitem(qi, st);
    free(qi);
    (void)utc_gettime(&q->qattr.last_activity);

    if (mattr->notice_opts & mos_c_msg_attr_notice_dequeue &&
		mattr->notice_dest) {
	/* XXX - send reply */
fprintf(stderr, "Dequeue notice to: %s\n", mattr->notice_dest);
	enqueue_ack(mattr->notice_dest, mattr,
		mos_c_msg_attr_notice_dequeue, st);
    }
    dequeue_count++;
}

/* ______________________________________________________________________ */
/* mos_q_peek  --  peek at next matching message on queue, given selection mask.
 */
void
MOS_q_peek(
  /* [in] */
    handle_t			h,
    idl_char			*name,
    mos_mask_list_t		*masks,
    unsigned32			flags,
  /* [out] */
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    error_status_t		*st
)
{
    Queue		*q;
    QItem		*qi, *prev_qi;

#if DEBUG
fprintf(stderr, "OP: mos_q_peek %s\n", name);
#endif
    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	null_out_reply(mattr, body);
	return;
    }
/* XXX - is this ACL perm correct??? */
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_dequeue, st)) {
	null_out_reply(mattr, body);
	return;
    }

    (void)utc_gettime(&q->qattr.last_activity);

    if (q->qattr.len == 0) {
	null_out_reply(mattr, body);
	*st = mos_empty_queue;
	return;
    }

    *st = error_status_ok;
    /* If no selection mask, return first one on queue. */
    if (masks == NULL || masks->count == 0) {
	*mattr = q->qi->item.mattr;
	*body  = q->qi->item.body;
	return;
    }

    find_item_by_selmask(masks, q, &qi, &prev_qi, st);
    if (*st != error_status_ok) {
	null_out_reply(mattr, body);
	*st = mos_item_not_found;
	return;
    }
    *mattr = qi->item.mattr;
    *body  = qi->item.body;
}

/* ______________________________________________________________________ */
/* mos_q_purge  --  purge/delete matching message on queue
 */
void
MOS_q_purge(
  /* [in] */
    handle_t			h,
    idl_char			*name,
    uuid_t			*id,
  /* [out] */
    error_status_t		*st
)
{
    Queue		*q;
    QItem		*qi,*prev_qi;

#if DEBUG
fprintf(stderr, "OP: mos_q_purge %s\n", name);
#endif
    if ( (q = find_queue_by_name(name, st)) == NULL )
	return;
    for (qi=q->qi,prev_qi=NULL; qi; prev_qi=qi,qi=qi->next) {
	if (uuid_equal(&qi->item.mattr.id, id, st)) {
	    /* take item out of linked list */
	    if (prev_qi)		/* not the first in queue */
		prev_qi->next = qi->next;
	    else			/* first in queue */
		q->qi = qi->next;
	    q->qattr.len--;
	    if (qi->item.mattr.notice_opts & mos_c_msg_attr_notice_dequeue &&
			qi->item.mattr.notice_dest) {
		enqueue_ack(qi->item.mattr.notice_dest, &qi->item.mattr,
			mos_c_msg_attr_notice_dequeue, st);
	    }
	    free_qitem(qi, st);
	    dequeue_count++;
	    break;
	}
    }
}

/* ______________________________________________________________________ */
/* mos_q_retrieve_mattr  --  get attributes of a next message matching mask.
 */

void
MOS_q_retrieve_mattr(
  /* [in] */
    handle_t		h,
    idl_char		*name,
    mos_mask_list_t	*masks,
    unsigned32		flags,
  /* [out] */
    mos_mattr_t		*mattr,
    error_status_t	*st
)
{
    Queue		*q;
    QItem		*qi, *prev_qi;

printf("OP: mos_q_retrieve_mattr: %s\n", name);
    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	null_out_reply(mattr, NULL);
	return;
    }
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_rdmattr, st)) {
	null_out_reply(mattr, NULL);
	return;
    }

    (void)utc_gettime(&q->qattr.last_activity);

    *st = error_status_ok;
    if (masks == NULL || masks->count == 0) {
	if (q->qattr.len == 0) {
	    null_out_reply(mattr, NULL);
	    *st = mos_empty_queue;
	}
	else
	    *mattr = q->qi->item.mattr;
	return;
    }

    find_item_by_selmask(masks, q, &qi, &prev_qi, st);
    if (*st != error_status_ok) {
	null_out_reply(mattr, NULL);
	return;
    }
    *mattr = qi->item.mattr;
}

/* mos_q_retrieve_mattr_by_id  --  get attribute of message with given ID
 */
void
MOS_q_retrieve_mattr_by_id(
  /* [in] */
    handle_t		h,
    idl_char		*name,
    uuid_t		*id,
  /* [out] */
    mos_mattr_t		*mattr,
    error_status_t	*st
)
{
    Queue		*q;
    QItem		*qi;

printf("OP: mos_q_retrieve_mattr_by_id: %s ", name); Print_uuid("", id);
    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	null_out_reply(mattr, NULL);
	return;
    }
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_rdmattr, st)) {
	null_out_reply(mattr, NULL);
	return;
    }

    *mattr = null_mattr;
    (void)utc_gettime(&q->qattr.last_activity);
    for (qi=q->qi; qi; qi=qi->next) {
	if (uuid_equal(&qi->item.mattr.id, id, st)) {
	    *mattr = qi->item.mattr;
	    *st = error_status_ok;
	    return;
	}
    }
    null_out_reply(mattr, NULL);
    *st = mos_item_not_found;
}

/* ______________________________________________________________________ */
/* mos_q_get_id_list  --  get list of matching message IDs, given queue name.
 * mos_q_get_id_list_all  --  get list of all message IDs, given queue name.
 */

void
MOS_q_get_id_list(
  /* [in] */
    handle_t		h,
    idl_char		*name,
    mos_mask_list_t	*masks,
  /* [out] */
    mos_uuid_list_t	*idlist,
    error_status_t	*st
)
{
    Queue		*q;
    QItem		*qi;
    unsigned32		i;

    *st = error_status_ok;
    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	idlist->count = 0;
	return;
    }
/*
XXX what perms needed for this?  mos_perm_rdmattr?
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_rdmattr, st)) {
	null_out_reply(mattr, NULL);
	return;
    }
*/

    (void)utc_gettime(&q->qattr.last_activity);
#if DEBUG
fprintf(stderr, "OP: mos_q_get_id_list %s\n", name);
#endif
    idlist->list = rpc_sm_allocate((sizeof(*idlist->list) * q->qattr.len), st);
    for (i=0,qi=q->qi; qi; qi=qi->next) {
	if (item_matches_selmask(masks, qi)) {
	    idlist->list[i] = qi->item.mattr.id;
	    i++;
	}
    }
    idlist->count = i;
}

void
MOS_q_get_id_list_all(
  /* [in] */
    handle_t		h,
    idl_char		*name,
  /* [out] */
    mos_uuid_list_t	*idlist,
    error_status_t	*st
)
{
    Queue		*q;
    QItem		*qi;
    unsigned32		i;

    *st = error_status_ok;
    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	idlist->count = 0;
	return;
    }
/*
XXX what perms needed for this?  mos_perm_rdmattr?
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_rdmattr, st)) {
	null_out_reply(mattr, NULL);
	return;
    }
*/

    (void)utc_gettime(&q->qattr.last_activity);
#if DEBUG
fprintf(stderr, "OP: mos_q_get_id_list_all %s\n", name);
#endif
    idlist->list = rpc_sm_allocate((sizeof(*idlist->list) * q->qattr.len), st);
    idlist->count = q->qattr.len;
    for (i=0,qi=q->qi; qi; qi=qi->next,i++)
	idlist->list[i] = qi->item.mattr.id;
}

/* ______________________________________________________________________ */
/* mos_q_delete_queue  --  delete a queue
 */

void
MOS_q_delete_queue(
  /* [in] */
    handle_t		h,
    idl_char		*name,
    mos_que_delete_op_t	flag,
  /* [out] */
    error_status_t	*st
)
{
    Queue		*q;

    *st = error_status_ok;
#if DEBUG
fprintf(stderr, "OP: mos_delete_queue: %s\n", name);
#endif

    if ( (q = find_queue_by_name(name, st)) == NULL )
	return;
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_delete, st))
	return;

    /* XXX */
    /* Check if queue is empty and what flag says about non-empty queue. */
    if (flag == mos_c_delop_preserve && q->qattr.len > 0) {
	*st = mos_queue_not_empty;
	return;
    }

    /* OK, caller specified mos_c_delop_force OR the queue is empty. */

    /* empty the queue items */
    if (q->qattr.len > 0) {
	/* Empty the queue (free the items). */
	QItem	*qi, *qix;
	qi = q->qi;
	while (qi) {
	    /* delete backing store item, free memory */
/*	    dce_db_delete_by_uuid(qdata_db, &qi->item.mattr.id, st);*/
	    /* XXX - report error somehow, then keep on going */
	    qix = qi;
	    qi = qi->next;
	    free_qitem(qix, st);
	}
    }

    delete_queue(name, st);
}

/* ______________________________________________________________________ */
/* mos_q_get_qattr_by_name  --  get queue attributes, given its name.
 */

void
MOS_q_get_qattr_by_name(
  /* [in] */
    handle_t		h,
    idl_char		*name,
  /* [out] */
    mos_qattr_t		*qattr,
    error_status_t	*st
)
{
    Queue		*q;

#if DEBUG
fprintf(stderr, "OP: mos_q_get_qattr_by_name\n");
#endif
    *st = error_status_ok;

    if ( (q = find_queue_by_name(name, st)) == NULL ) {
	*qattr = null_qattr;
	return;
    }
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_rdqattr, st)) {
	*qattr = null_qattr;
	return;
    }

    copy_qattr_for_client(qattr, &q->qattr);
}

/* mos_q_get_qattr_by_id  --  get queue attributes, given its ID.
 */

void
MOS_q_get_qattr_by_id(
  /* [in] */
    handle_t		h,
    uuid_t		*id,
  /* [out] */
    mos_qattr_t		*qattr,
    error_status_t	*st
)
{
    Queue	*q;

Print_uuid("OP: mos_q_get_qattr_by_id: ", id);
    *st = error_status_ok;

    if ( (q = find_queue_by_id(id, st)) == NULL ) {
	*qattr = null_qattr;
	return;
    }

    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_rdqattr, st)) {
	*qattr = null_qattr;
	return;
    }

    copy_qattr_for_client(qattr, &q->qattr);
}

/* ______________________________________________________________________ */
/* mos_q_set_qattr_by_name  --  set queue attributes, given its name.
 */

static void
set_qattr(
    Queue		*q,
    mos_qattr_t		*qattr,
    error_status_t	*st
)
{
    unsigned32	i;

printf("========= set_qattr\n");
Print_qattr(qattr);
    /* don't overwrite some fields */
    qattr->name = q->qattr.name;
    qattr->id = q->qattr.id;
    qattr->acl = q->qattr.acl;
    qattr->last_activity = q->qattr.last_activity;
    qattr->created = q->qattr.created;

    /* set attr in memory */
    /* XXX - free stuff from old attr in memory - aliases, annot, notice_dest */
    free(q->qattr.annotation);
    for (i=0; i<q->qattr.aliases.count; i++) {
	free(q->qattr.aliases.list[i]);
    }
    q->qattr = *qattr;
    /* dup all the strings, since what was passed in will be auto-freed */
    if (qattr->annotation)
	q->qattr.annotation = (idl_char *)strdup((char *)qattr->annotation);
    copy_string_list_local(&q->qattr.aliases, &qattr->aliases);
/*
    for (i=0; i<q->qattr.aliases.count; i++)
	q->qattr.aliases.list[i] =
		(idl_char *)strdup((char *)qattr->aliases.list[i]);
*/
Print_qattr(qattr);

    dce_db_store_by_name(qattr_db, (char *)qattr->name, qattr, st);
}

void
MOS_q_set_qattr_by_name(
  /* [in] */
    handle_t		h,
    idl_char		*name,
    mos_qattr_t		*qattr,
  /* [out] */
    error_status_t	*st
)
{
    Queue	*q;

    *st = error_status_ok;
#if DEBUG
fprintf(stderr, "OP: mos_q_set_qattr_by_name: %s\n", name);
#endif

    if ( (q = find_queue_by_name(name, st)) == NULL )
	return;
    (void)utc_gettime(&q->qattr.last_activity);
    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_manage, st))
	return;

    set_qattr(q, qattr, st);
    return;
}

/* mos_q_set_qattr_by_id  -- set queue attributes, given its ID.
*/

void
MOS_q_set_qattr_by_id(
  /* [in] */
    handle_t		h,
    uuid_t		*id,
    mos_qattr_t		*qattr,
  /* [out] */
    error_status_t	*st
)
{
    Queue	*q;

Print_uuid("OP: mos_q_set_qattr_by_id: ", id);
    *st = error_status_ok;
    if ( (q = find_queue_by_id(id, st)) == NULL )
	return;
    (void)utc_gettime(&q->qattr.last_activity);

    if (!mos_authorized(h, &q_obj_type, &q->qattr.acl, mos_perm_manage, st))
	return;

    set_qattr(q, qattr, st);
    return;
}

/* ______________________________________________________________________ */
/* mos_q_ping  --  see if queue exists.  Supports the API's "attach" call.
 */

void
MOS_q_ping(
  /* [in] */
    handle_t		h,
    idl_char		*name,
  /* [out] */
    uuid_t		*id,
    error_status_t	*st
)
{
    Queue		*q;
    error_status_t	xst;
#if DEBUG
fprintf(stderr, "OP: mos_q_ping: %s\n", name);
#endif
    if ( (q = find_queue_by_name(name, st)) == NULL )
	uuid_create_nil(id, &xst);
    else {
	*id = q->qattr.id;
	(void)utc_gettime(&q->qattr.last_activity);
    }
}

/* ______________________________________________________________________ */

void
MOS_q_move_item(
  /* [in] */
    handle_t		h,
    idl_char		*from_name,
    idl_char		*to_name,
    uuid_t		*id,
    unsigned32		flags,
  /* [out] */
    error_status_t	*st
)
{
    Queue		*q_from, *q_to;
    QItem		*qi, *prev_qi, *qi2move;

    if ( (q_from = find_queue_by_name(from_name, st)) == NULL )
	return;
    if ( (q_to = find_queue_by_name(to_name, st)) == NULL )
	return;

    (void)utc_gettime(&q_from->qattr.last_activity);
    (void)utc_gettime(&q_to->qattr.last_activity);
    for (prev_qi=NULL,qi2move=q_from->qi; qi2move; qi2move=qi2move->next) {
	if (uuid_equal(&qi2move->item.mattr.id, id, st))
	    break;
	prev_qi = qi2move;
    }
    if (qi2move == NULL) {
	*st = mos_item_not_found;
	return;
    }

    /* remove from "from" queue */
    if (prev_qi == NULL)
	q_from->qi = NULL;
    else
	prev_qi->next = qi2move->next;
    q_from->qattr.len--;

    /* add to "to" queue */
    if (q_to->qi == NULL)
	q_to->qi = qi2move;
    else {
/* XXX - incomplete - ran out of time today.  Add to end of queue for now. */
	for (qi=q_to->qi; qi; qi=qi->next) {
	    if (qi->next == NULL) {
		qi->next = qi2move;
		qi2move->next = NULL;
		break;
	    }
	}
    }
    q_to->qattr.len++;
}

/* ______________________________________________________________________ */

/* Entry-point vector - so the qmgr can call other qmgrs. */
mos_q_v1_0_epv_t MOS_q_v1_0_epv = {
    MOS_q_enqueue,
    MOS_q_dequeue,
    MOS_q_dequeue_next,
    MOS_q_peek,
    MOS_q_purge,
    MOS_q_retrieve_mattr,
    MOS_q_retrieve_mattr_by_id,
    MOS_q_get_id_list,
    MOS_q_get_id_list_all,
    MOS_q_delete_queue,
    MOS_q_get_qattr_by_name,
    MOS_q_get_qattr_by_id,
    MOS_q_set_qattr_by_name,
    MOS_q_set_qattr_by_id,
    MOS_q_ping,
    MOS_q_move_item
};

/* ______________________________________________________________________ */

