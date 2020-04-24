/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Queue mgt utilities.
 * ______________________________________________________________________
 *
 * $Log: q_utils.c,v $
 * Revision 1.13  1996/11/27  19:15:26  bowe
 * In enqueue_ack(), do not enqueue notice if enqueuing is disabled.
 *
 * Revision 1.12  1996/11/14  01:55:25  bowe
 * Rename to mos_datatype_ack.
 *
 * Revision 1.11  1996/11/13  17:25:13  bowe
 * Make ack_datatype extern.  #include mos_ack.h
 *
 * Revision 1.10  1996/11/11  22:05:02  bowe
 * In enqueue_ack(), call enqueue_item() with TRUE so it will store
 * the ack message persistently (if attribute says to).
 *
 * Revision 1.9  1996/11/11  21:58:58  bowe
 * Add arg to enqueue_ack() (makes it more general)
 *
 * Revision 1.8  1996/11/11  16:07:00  bowe
 * In find_item_by_selmask(), initialize prev_qi=NULL.
 *
 * Revision 1.7  1996/11/01  20:39:10  bowe
 * Added function move_items().
 * Check for null pointer in item_matches_selmask().
 *
 * Revision 1.6  1996/10/31  20:07:56  bowe
 * Add item_matches_selmask() (do do mask matching).
 * Rework find_item_by_selmask() to use item_matches_selmask().
 *
 * Revision 1.5  1996/10/22  14:49:54  bowe
 * In enqueue_ack, check that notice_dest is not null.
 *
 * Revision 1.4  1996/10/07  19:45:30  bowe
 * Fixes to find_item_by_selmask().
 *
 * Revision 1.3  1996/10/03  18:46:53  bowe
 * Adjust for new notice options symbols.
 * Start ack/reply to remote queues.
 *
 * Revision 1.2  1996/10/01  14:15:57  bowe
 * Initialze flag to 0 in cache_queue_info, enqueue_item.
 *
 * Revision 1.1  1996/09/27  18:07:30  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include "mosdefs.h"
#include <mos_ack.h>
#include <dce/idl_es.h>

/* ______________________________________________________________________ */

Queue  *the_queues=NULL, *last_queue=NULL;

mos_qattr_t			null_qattr = { 0 };
mos_mattr_t			null_mattr = { 0 };
mos_datatype_bytearray_t	null_body = { 0 };

/* ______________________________________________________________________ */

/* Store Queue structure in in-memory.  Copies the qattr and initializes
 * other parts (eg, mutexes, etc.).
 */

Queue *
cache_queue_info(
  /* in */
    idl_char		*name,
    mos_qattr_t		*qattr,
  /* out */
    error_status_t	*st )
{

    *st = error_status_ok;

    /* See if queue by that name already exists. */
    if (find_queue_by_name(name, st) != NULL) {
	*st = mos_queue_already_exists;
	return NULL;
    }
    *st = error_status_ok;

    /* Create new node at end of linked list */
    if (!the_queues) {
	the_queues = last_queue = NEW(Queue, 1);
    }
    else if (last_queue) {
	last_queue->next = NEW(Queue, 1);
	last_queue = last_queue->next;
    }
    last_queue->next = NULL;

    copy_qattr_local(&last_queue->qattr, qattr);
    last_queue->qi = NULL;
    last_queue->blocked = FALSE;

    last_queue->flags = 0;

    /* Create locks and mutexes */
    lock_create(&last_queue->lock, st);
	if (*st != error_status_ok) dce_svc_printf(MOS_S_MUTEX_LOCK_FAILED_MSG);
    if (pthread_cond_init(&last_queue->deq_cond, pthread_condattr_default) != 0)
	dce_svc_printf(MOS_S_COND_INIT_FAIL_MSG);
    if (pthread_mutex_init(&last_queue->deq_mutex, pthread_mutexattr_default) != 0)
    dce_svc_printf(MOS_S_MUTEX_INIT_FAIL_MSG);

    (void)utc_gettime(&last_queue->qattr.last_activity);

    return last_queue;
}

/* ______________________________________________________________________ */

/* Delete queue from memory and the backing store.  Also, delete its ACL.
 */

void
delete_queue(
    idl_char		*name,
    error_status_t	*st
)
{
    Queue	*q, *prev_q;

    *st = error_status_ok;

    /* Be sure queue exists */
    prev_q = NULL;
    for (q=the_queues; q; q=q->next) {
	if (!strcmp((char *)q->qattr.name, (char *)name))
	    break;
	prev_q = q;
    }
    if (q == NULL) {
	*st = mos_queue_not_found;
	return;
    }

    /* Take queue out of linked list. */
    if (q == the_queues)
	the_queues = the_queues->next;
    if (prev_q)
	prev_q->next = q->next;
    if (last_queue == q)
	last_queue = prev_q;

    dce_db_delete_by_uuid(acl_db, &q->qattr.acl, st);
    dce_db_delete_by_name(qattr_db, (char *)name, st);

    /* Clean up. */
    free(q->qattr.name);
    free(q);
}

#if 0
void
add_item_to_queue(
    Queue			*q,
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    error_status_t		*st )
{
    QItem	*qi;

    if (!q->qi) {
	q->qi = qi = NEW(QItem, 1);
    }
    else {	/* point qi to end of queue */
	for (qi=q->qi; qi->next; qi=qi->next)
	    /* do nothing */    ;
	qi->next = NEW(QItem, 1);
	qi = qi->next;
    }
    qi->next = NULL;
    qi->item.body.size = body->size;
    qi->item.body.data = (idl_char *)malloc((size_t)body->size);
    memcpy(qi->item.body.data, body->data, body->size);
}
#endif

/* ______________________________________________________________________ */

/* Find queue by name or ID.  Returns a pointer to that Queue structure.
 * If not found, return NULL and set status to mos_queue_not_found.
 */

Queue *
find_queue_by_name(
    idl_char		*name,
    error_status_t	*st
)
{
    Queue	*q;
    mos_string_t *a;
    unsigned32	i;

    *st = error_status_ok;
    /* Check primary names first. */
    for (q=the_queues; q; q=q->next) {
	if (!strcmp((char *)q->qattr.name, (char *)name)) return q;
    }
    /* Now aliases */
    for (q=the_queues; q; q=q->next) {
	if (q->qattr.aliases.count == 0) continue;
	a = q->qattr.aliases.list;
	for (i=0; i<q->qattr.aliases.count; i++)
	    if (!strcmp((char *)a[i], (char *)name)) return q;
    }
    *st = mos_queue_not_found;
    return NULL;
}

Queue *
find_queue_by_id(
    uuid_t		*id,
    error_status_t	*st
)
{
    Queue		*q;
    error_status_t	stx;

    *st = error_status_ok;
    for (q=the_queues; q; q=q->next) {
	if (uuid_equal(&q->qattr.id, id, &stx)) return q;
    }
    *st = mos_queue_not_found;
    return NULL;
}

/* ______________________________________________________________________ */

/* Place item (mattr and body) in queue pointed to by q.
 * This will allocate local storage and sets "flags" in QItem structure
 * appropriately.  Also writes to backing store, when appropriate.
 *
 * store_in_db says whether to write item to backing store.
 * FALSE at startup - when we're reading from backing store, storing items
 * into memory (they're alreadyt in backing store).
 * TRUE when doing enqueue operation, in which case we check attribute, set
 * by caller, to decide whether to write.
 */

void
enqueue_item(
    Queue			*q,
    mos_mattr_t			*mattr,
    mos_datatype_bytearray_t	*body,
    boolean			store_in_db
)
{
    QItem	*qi, *prev_qi;

    if (!q->qi) {		/* is this queue empty? */
	/* create sole item in this queue */
        q->qi = qi = NEW(QItem, 1);
	qi->next = NULL;
    }
    else {
	/* Find spot in queue after last item with this priority or less */
	prev_qi = NULL;
        for (qi=q->qi; qi; qi=qi->next) {
	    if (mattr->priority < qi->item.mattr.priority) {
    /* XXX - should (optionally?) also look at enqueue time */
		break;
	    }
	    prev_qi = qi;
	}
	if (prev_qi == NULL) {		/* at head of queue */
	    /* if prev_qi is NULL, new item will be first */
	    qi = NEW(QItem, 1);
	    qi->next = q->qi;
	    q->qi = qi;
	}
	else {
	    /* we add new item between prev_qi and qi */
	    prev_qi->next = NEW(QItem, 1);
	    prev_qi->next->next = qi;
	    qi = prev_qi->next;
	}
    }

    /* Keep ID of queue with item's attr so if we read it from backing store
     * (at startup), we know what queue it goes in.
     */
    mattr->qid = q->qattr.id;
    copy_mattr_local(&qi->item.mattr, mattr);
    qi->item.body.size = body->size;
    qi->item.body.data = memdup(body->data, body->size);

    qi->flags = 0;

    q->qattr.len++;
    (void)utc_gettime(&q->qattr.last_activity);

    if (store_in_db && mattr->persistence == mos_c_msg_attr_persistent) {
	mos_qitem_t qitem;
	error_status_t	st;
	qitem.body = *body;
	qitem.mattr = *mattr;
	dce_db_store_by_uuid(qdata_db, &mattr->id, &qitem, &st);
	if (st != error_status_ok)
	    dce_svc_printf(MOS_S_CANT_STORE_DATA_MSG,
			"enqueue_item", (long)st);
    }
}

/* ______________________________________________________________________ */
/* Move all items from one queue to another.
 * Strategy - for each item in "from" queue:
 *	- insert it into "to" queue
 *	- take it out of "from" queue
 *	- change it's mattr qid field to ID of "to" queue
 *	- increment/decrement item counts in queues
 *	- write to backing store (if appropriate)
 */

void
move_items(
    Queue			*q_from,
    Queue			*q_to
)
{
    QItem		*qi, *prev_qi, *qi2move;
    unsigned32		pr;
    mos_qitem_t		qitem;
    error_status_t	st;

    while (qi2move=q_from->qi) {
	if (!q_to->qi) {		/* is "to" queue empty? */
	    /* if so, make this new item the only one on that queue */
	    q_to->qi = qi2move;
	    q_from->qi = q_from->qi->next;
	    qi2move->next = NULL;
	}
	else {
	    /* Find spot in queue after last item with this priority or less */
	    prev_qi = NULL;
	    for (qi=q_to->qi; qi; qi=qi->next) {
		if (qi2move->item.mattr.priority < qi->item.mattr.priority) {
    /* XXX - should also look at enqueue time */
		    break;
		}
		prev_qi = qi;
	    }
	    /* set start of old queue to item after one we're moving */
	    q_from->qi = q_from->qi->next;
	    if (prev_qi == NULL) {		/* at head of queue */
		/* if prev_qi is NULL, new item will be first */
		qi2move->next = q_to->qi;
		q_to->qi = qi2move;
	    }
	    else {
		/* we add new item between prev_qi and qi */
		qi2move->next = prev_qi->next;
		prev_qi->next = qi2move;
	    }
	}
	/* set parent queue ID for this item */
	qi2move->item.mattr.qid = q_to->qattr.id;
	/* bounce counters appropriately */
	q_to->qattr.len++;
	q_from->qattr.len--;

	/* If persistent, save the item (the only reason we need this
	 * is becaue we just changed the parent "qid"). */
	if (qi2move->item.mattr.persistence == mos_c_msg_attr_persistent) {
	    qitem.mattr = qi2move->item.mattr;
	    qitem.body = qi2move->item.body;	/* copy len & data ptr */
	    dce_db_store_by_uuid(qdata_db, &qi2move->item.mattr.id, &qitem,&st);
	    if (st != error_status_ok)
		dce_svc_printf(MOS_S_CANT_STORE_DATA_MSG, "MOS_mgmt_move",
			(long)st);
	}
    }
}

/* ______________________________________________________________________ */

/* Read queue info from backing store. */

void
read_queues(error_status_t *st)
{
    char		*key;
    uuid_t		*id;
    error_status_t	stx;
    Queue		*q;
    mos_qattr_t		qattr;
    mos_qitem_t		qitem;

    /* Read queue attributes, storing into memory. */
    dce_db_iter_start(qattr_db, st);
    if (*st != error_status_ok) return;

    while (1) {
	dce_db_iter_next_by_name(qattr_db, &key, st);
	if (*st == db_s_no_more) break;
	if (*st != error_status_ok) return;
	dce_db_fetch_by_name(qattr_db, key, &qattr, st);
	if (*st != error_status_ok) return;
#if DEBUG
fprintf(stderr, "    %s\n", qattr.name);
#endif
	q = cache_queue_info(qattr.name, &qattr, st);
	if (*st != error_status_ok) return;
    }

    dce_db_iter_done(qattr_db, st);
    if (*st != error_status_ok) return;

    /* _____________________________________________ */

    /* Now, read queue items, storing them intgo memory. */
    dce_db_iter_start(qdata_db, st);
    if (*st != error_status_ok) return;

    while (1) {
	dce_db_iter_next_by_uuid(qdata_db, &id, st);
	if (*st == db_s_no_more) break;
	if (*st != error_status_ok) break;
	dce_db_fetch_by_uuid(qdata_db, id, &qitem, st);
	if (*st != error_status_ok) return;
	if ( (q = find_queue_by_id(&qitem.mattr.qid, &stx)) == NULL) {
printf("Orphaned message\n");
Print_uuid("   Message-ID: ", &qitem.mattr.id);
Print_uuid("   Queue-ID: ", &qitem.mattr.qid);
 /*
	    dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"missing queue / orphan message", (long)*st);
 */
	    continue;
	}
/*Print_uuid("\titem: ", &qitem.mattr.id);*/
	enqueue_item(q, &qitem.mattr, &qitem.body, FALSE);
	free(qitem.body.data);
	if (qitem.mattr.notice_dest) free(qitem.mattr.notice_dest);
    }

    dce_db_iter_done(qdata_db, st);
    if (*st != error_status_ok) return;
}

/* ______________________________________________________________________ */
/* Copy a mos_string_list_t structure, l2 to l1 (like strcpy). */

void
copy_string_list_for_client(mos_string_list_t *l1, mos_string_list_t *l2)
{
    int			i;
    error_status_t	st;

    l1->count = l2->count;
    if (l2->count == 0) {
	l1->list = NULL;
	return;
    }

    l1->list = NEW_client(mos_string_t, l2->count, &st);
    for (i=0; i<l2->count; i++)
	l1->list[i] = strdup_for_client(l2->list[i], &st);
}

void
copy_string_list_local(mos_string_list_t *l1, mos_string_list_t *l2)
{
    int			i;

    l1->count = l2->count;
    if (l2->count == 0) {
	l1->list = NULL;
	return;
    }

    l1->list = NEW(mos_string_t, l2->count);
    for (i=0; i<l2->count; i++)
	l1->list[i] = (idl_char *)strdup((char *)l2->list[i]);
}

void
free_string_list_local(mos_string_list_t *l)
{
    int			i;

    if (l->count == 0) return;
    for (i=0; i<l->count; i++)
	free(l->list[i]);
    free(l->list);
}

/* ______________________________________________________________________ */
/* Copy qattr structure, a2 to a1 (like strcpy). */

void
copy_qattr_for_client(mos_qattr_t *a1, mos_qattr_t *a2)
{
    error_status_t	st;

    *a1 = *a2;
    a1->name = strdup_for_client(a2->name, &st);
    if (a2->annotation)
	a1->annotation = strdup_for_client(a2->annotation, &st);
    copy_string_list_for_client(&a1->aliases, &a2->aliases);
/*
    a2->aliases.count = a1->aliases.count;
    if (a2->aliases.count > 0) {
	int i;
	a1->aliases.list =
	    NEW_client(mos_string_t, a2->aliases.count, &st);
	for (i=0; i<a2->aliases.count; i++)
	    a1->aliases.list[i] = strdup_for_client(a2->aliases.list[i], &st);
    }
*/
}

void
copy_qattr_local(mos_qattr_t *a1, mos_qattr_t *a2)
{
    *a1 = *a2;
    a1->name = (idl_char *)strdup((char *)a2->name);
    if (a2->annotation)
	a1->annotation = (idl_char *)strdup((char *)a2->annotation);
    copy_string_list_local(&a1->aliases, &a2->aliases);
/*
    a2->aliases.count = a1->aliases.count;
    if (a2->aliases.count > 0) {
	int i;
	a1->aliases.list = NEW(mos_string_t, a2->aliases.count);
	for (i=0; i<a2->aliases.count; i++) {
	    a1->aliases.list[i] = (idl_char *)strdup((char *)a2->aliases.list[i]);
	}
    }
*/
}

/* ______________________________________________________________________ */
/* Copy a2 to a1 (like strcpy). */
void
copy_mattr_for_client(mos_mattr_t *a1, mos_mattr_t *a2)
{
    error_status_t	st;

    *a1 = *a2;
    if (a2->notice_dest)
	a1->notice_dest = strdup_for_client(a2->notice_dest, &st);

    /* copy "lower" pieces of sec_id_pa_t */
    if (a2->sender.num_groups) {
	int i;
	a1->sender.groups =
		NEW_client(sec_id_t, a2->sender.num_groups, &st);
	for (i=0; i<a1->sender.num_groups; i++) {
	    a1->sender.groups[i] = a2->sender.groups[i];
	}
    }
    if (a2->sender.num_foreign_groupsets) {
	int i;
	a1->sender.foreign_groupsets = NEW_client(sec_id_foreign_groupset_t,
			a2->sender.num_foreign_groupsets, &st);
/* XXX - this is incomplete!!!!! */
	for (i=0; i<a2->sender.num_foreign_groupsets; i++) {
	    a1->sender.foreign_groupsets[i] = a2->sender.foreign_groupsets[i];
	}
    }
}

/* Copy a2 to a1 (like strcpy). */
void
copy_mattr_local(mos_mattr_t *a1, mos_mattr_t *a2)
{
    *a1 = *a2;
    if (a2->notice_dest)
	a1->notice_dest = (idl_char *)strdup((char *)a2->notice_dest);

    /* copy "lower" pieces of sec_id_pa_t */
    if (a2->sender.num_groups) {
	int i;
	a1->sender.groups = NEW(sec_id_t, a2->sender.num_groups);
	for (i=0; i<a2->sender.num_groups; i++) {
	    a1->sender.groups[i] = a2->sender.groups[i];
	}
    }
    if (a2->sender.num_foreign_groupsets) {
	int i;
	a1->sender.foreign_groupsets = NEW(sec_id_foreign_groupset_t,
			a2->sender.num_foreign_groupsets);
/* XXX - this is incomplete!!!!! */
	for (i=0; i<a2->sender.num_foreign_groupsets; i++) {
	    a1->sender.foreign_groupsets[i] = a2->sender.foreign_groupsets[i];
	}
    }
}

/* ______________________________________________________________________ */
/* Free storage - in-core and backing store - for a queue item. */

void
free_qitem(QItem *qi, error_status_t *st)
{
    if (qi->item.mattr.persistence == mos_c_msg_attr_persistent) {
	dce_db_delete_by_uuid(qdata_db, &qi->item.mattr.id, st);
	/* XXX - report error somehow; keep on going */
    }

    if (qi->item.mattr.notice_dest)
	free(qi->item.mattr.notice_dest);
    if (qi->item.body.size)
	free(qi->item.body.data);
    free(qi);
}

/* ______________________________________________________________________ */
/* Determing whether a queue item matches a selection mask.
 * Returns TRUE/FALSE.
 */

boolean
item_matches_selmask(
  /* in */
    mos_mask_list_t	*masks,
    QItem		*qitem
)
{
    error_status_t	st;
    mos_mask_t		*mask;
    mos_mattr_t		*a;
    boolean		match;
    int			m;
    unsigned32		pr;
    mos_msg_selmask_op_t rel;
    uuid_t		*u;

    if (masks == NULL || masks->count == 0) return TRUE;
    a = &qitem->item.mattr;
    for (match=TRUE,m=masks->count,mask=masks->list; --m>=0; mask++) {
	rel = mask->relation;
	if (rel == mos_c_mask_op_none) continue;
	switch (mask->mask_info.mask_type) {
	case mos_c_mask_type_msgid:
	    u = &mask->mask_info.tagged_union.id;
	    if ( (rel == mos_c_mask_op_equal &&
		    !uuid_equal(u, &a->id, &st))
	      || (rel == mos_c_mask_op_not_equal &&
		    uuid_equal(u, &a->id, &st)) )
		match = FALSE;
	    break;
	case mos_c_mask_type_msgtype:
	    if ( (rel == mos_c_mask_op_equal &&
		    mask->mask_info.tagged_union.msgtype != a->msgtype)
	      || (rel == mos_c_mask_op_not_equal &&
		    mask->mask_info.tagged_union.msgtype == a->msgtype) )
		match = FALSE;
	    break;
	case mos_c_mask_type_priority:
	    pr = mask->mask_info.tagged_union.priority;
	    if ( (rel == mos_c_mask_op_equal && a->priority != pr)
	      || (rel == mos_c_mask_op_not_equal && a->priority == pr)
	      || (rel == mos_c_mask_op_less_than && a->priority >= pr)
	      || (rel == mos_c_mask_op_eq_less_than && a->priority > pr)
	      || (rel == mos_c_mask_op_greater_than && a->priority <= pr)
	      || (rel == mos_c_mask_op_eq_greater_than && a->priority < pr))
		match = FALSE;
	    break;
	case mos_c_mask_type_datatype:
	    u = &mask->mask_info.tagged_union.datatype;
	    if ( (rel == mos_c_mask_op_equal &&
		    !uuid_equal(u, &a->datatype, &st))
	      || (rel == mos_c_mask_op_not_equal &&
		    uuid_equal(u, &a->datatype, &st)) )
		match = FALSE;
	    break;
	case mos_c_mask_type_protectlvl:
	    if ( (rel == mos_c_mask_op_equal &&
		    mask->mask_info.tagged_union.protectlvl != a->protectlvl)
	      || (rel == mos_c_mask_op_not_equal &&
		    mask->mask_info.tagged_union.protectlvl == a->protectlvl) )
		match = FALSE;
	    break;
	case mos_c_mask_type_sender_prin:
	    u = &mask->mask_info.tagged_union.sender;
	    if ( (rel == mos_c_mask_op_equal &&
		    !uuid_equal(u, &a->sender.principal.uuid, &st))
	      || (rel == mos_c_mask_op_not_equal &&
		    uuid_equal(u, &a->sender.principal.uuid, &st)) )
		match = FALSE;
	    break;
	case mos_c_mask_type_ttr:
	    /* XXX */
	    break;
	}
	if (!match) return FALSE;
    }
    return match;
}

/* ______________________________________________________________________ */
/* Find matching item in given queue.
 * Returns pointers to matching queue-item, and the prev queue-item (so
 * caller can take the item out of the linked list).
 */

void
find_item_by_selmask(
  /* in */
    mos_mask_list_t	*masks,
    Queue		*q,
  /* out */
    QItem		**qitem,
    QItem		**prev_qitem,
    error_status_t	*st
)
{
    QItem		*qi, *prev_qi;

    for (qi=q->qi,prev_qi=NULL; qi; prev_qi=qi,qi=qi->next) {
	if ( item_matches_selmask(masks, qi) ) {
	    *qitem = qi;
	    *prev_qitem = prev_qi;
	    *st = error_status_ok;
	    return;
	}
    }
    *qitem = NULL;
    *prev_qitem = NULL;
    *st = mos_item_not_found;
}

/* ______________________________________________________________________ */
/* send a reply to the given queue */

extern uuid_t mos_datatype_ack;

void enqueue_ack_remote(idl_char *, mos_mattr_t *, mos_datatype_bytearray_t *);

void
enqueue_ack(
    idl_char		*name,
    mos_mattr_t		*data_mattr,
    unsigned32		notice_opt,
    error_status_t	*st
)
{
    Queue			*q;
    mos_mattr_t			mattr;
    mos_datatype_bytearray_t	body;
    mos_datatype_ack_t		ack;
    idl_es_handle_t		eh;
    idl_ulong_int		esize;
    idl_byte			*ep;
    boolean			remote_queue=FALSE;

    if (name == NULL || *name == '\0') {
	*st = mos_queue_not_found;
	return;
    }

    /* XXX
     * Check if queue is external to us.  If so, we can't send it (yet).
     * We'd have to spawn a thread, connect to qmgr, enqueue it, ...
     */

    if (name[0] == '/') {
	remote_queue = TRUE;
    }
    else {
	q = find_queue_by_name(name, st);
	if (*st != error_status_ok) return;
    }

    /* XXX - should an error be returned?  But it will be to the recipient,
     * not the sender.  The sender will never know.  Open question.
     * For now, silently return.
     */
    if (!q->qattr.enq_enabled)
	return;

    mattr = *data_mattr;	/* copy most of original msg's attributes */
    body = null_body;

    (void)utc_gettime(&mattr.time_enqueued);
    uuid_create(&mattr.id, st);
    mattr.msgtype = mos_c_msg_attr_msgtype_notice;
    mattr.sender.principal.uuid = qmgr_prin_uuid;;
    mattr.datatype = mos_datatype_ack;
    mattr.notice_opts = mos_c_msg_attr_notice_none;
    mattr.notice_dest = NULL;
    mattr.qid = q->qattr.id;
/*    mattr.expire_time = XXX;*/
/*    mattr.valid_time = XXX;*/

    ack.msg_id = data_mattr->id;
    ack.notice_option = notice_opt;
    ack.status = error_status_ok;

    /* call encoding services to (what else) encode the data */

    idl_es_encode_dyn_buffer(&ep, &esize, &eh, st);
    if (*st != error_status_ok) return;

    mos_ack_convert(eh, &ack, st);
    if (*st != error_status_ok) {
	error_status_t x_st;
	idl_es_handle_free(&eh, &x_st);
	return;
    }

    body.data = ep;
    body.size = esize;

    if (remote_queue)
	enqueue_ack_remote(name, &mattr, &body);
    else
	enqueue_item(q, &mattr, &body, TRUE);

    idl_es_handle_free(&eh, st);
}

/* ______________________________________________________________________ */

void *
ack_enq_thread(void *arg)
{
    idl_char	*name;
    name = arg;
fprintf(stderr, "ACK - enqueue in %s.\n", name);
}


void
enqueue_ack_remote(
    idl_char		*name,
    mos_mattr_t		*mattr,
    mos_datatype_bytearray_t
			*body
)
{
    pthread_t	tid;

    /* Allocate/copy mattr and body.  (Has to be copied, since all the
     * space was allocated by the RPC/thread that's about to go return.
     */

    if (pthread_create(&tid, pthread_attr_default, ack_enq_thread, name) < 0)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_ER_MSG,
			"pthread_create", (long)errno);
    pthread_detach(&tid);
}

/* ______________________________________________________________________ */

#if 0
Print_acl_db()
{
    uuid_t	*id;
    sec_acl_t	acl;
    error_status_t	st;

    dce_db_iter_start(acl_db, &st);
    if (st != error_status_ok) return;
    while (1) {
	dce_db_iter_next_by_uuid(acl_db, &id, &st);
	if (st == db_s_no_more) break;
	if (st != error_status_ok) break;
	dce_db_fetch_by_uuid(acl_db, id, &acl, &st);
	if (st != error_status_ok) return;
	Print_acl(&acl, id);
    }
    dce_db_iter_done(acl_db, &st);
}
#endif
