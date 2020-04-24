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
 * $Log: oemisc.c,v $
 * Revision 1.1.4.2  1996/02/18  19:16:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:53:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:15  root]
 * 
 * Revision 1.1.2.10  1994/09/17  08:05:17  rsalz
 * 	Rename epdb_sleep to dced_sleep.
 * 	[1994/09/17  07:57:44  rsalz]
 * 
 * Revision 1.1.2.9  1994/08/28  14:40:23  rsalz
 * 	Use dce_gettimeofday not dced_get_the_time (OT CR 11931).
 * 	[1994/08/28  14:34:53  rsalz]
 * 
 * Revision 1.1.2.8  1994/08/03  20:36:20  rsalz
 * 	Fix OT CR 11506.
 * 	[1994/08/03  20:32:43  rsalz]
 * 
 * Revision 1.1.2.6  1994/06/16  19:35:16  rsalz
 * 	Don't store conformant structures in dce_db (see OT CR 10971).
 * 	[1994/06/16  19:32:16  rsalz]
 * 
 * Revision 1.1.2.5  1994/06/13  20:05:35  rsalz
 * 	Move epdb_get_the_time to dced_get_the_time
 * 	[1994/06/13  17:31:32  rsalz]
 * 
 * Revision 1.1.2.4  1994/06/09  18:15:37  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:19  annie]
 * 
 * Revision 1.1.2.3  1994/06/07  20:31:39  rsalz
 * 	Remove compiler warnings and a local variable in dblist_{first,next};
 * 	do the cast only at the return statement (OT CR 10883).
 * 	[1994/06/07  18:52:17  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/27  16:31:20  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:35  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/26  19:42:25  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:56  rsalz]
 * 
 * $EndLog$
 */
#include <oeimpl.h>

/*
**  Old EP database list and miscellaneous support.
*/


/*
**  Search context.
*/
typedef struct db_context_s_t {
    epdb_handle_t	h;
    dblist_type_t	list_type;
    db_entry_t		*ep;
    unsigned32		pass;
} *db_context_t;

/*
**  The following calculated offsets turn a list address into the start of
**  the full endpoint entry.
*/
static unsigned32	db_c_entries_offset;
static unsigned32	db_c_objects_offset;
static unsigned32	db_c_interfaces_offset;

/*
**  List modification.
*/

static void
add_to_list(
    dblist_t		*list,
    dblist_type_t	list_type,
    db_entry_t		*ep
)
{
    dblist_t		*lp;

    dce_assert(dhd_svc_handle, list_type == db_c_entry_list
	|| list_type == db_c_object_list || list_type == db_c_interface_list);
    switch (list_type) {
    case db_c_entry_list:
	lp = &ep->lists.entries;
	break;
    case db_c_object_list:
	lp = &ep->lists.objects;
	break;
    case db_c_interface_list:
	lp = &ep->lists.interfaces;
	break;
    }

    lp->fwd = NULL;
    lp->back = list->back;
    lp->back->fwd = lp;
    list->back = lp;
}

void
dblist_add(
    db_lists_mgmt_t	*lists_mgmt,
    db_entry_t		*ep
)
{
    int			bucket;
    error_status_t	st2;

    add_to_list(&lists_mgmt->entries, db_c_entry_list, ep);
    bucket = uuid_hash(&ep->object, &st2);
    bucket = db_hash2bucket(bucket);
    add_to_list(&lists_mgmt->objects[bucket], db_c_object_list, ep);
    bucket = uuid_hash(&ep->ifid.uuid, &st2);
    bucket = db_hash2bucket(bucket);
    add_to_list(&lists_mgmt->interfaces[bucket], db_c_interface_list, ep);
}

static void
remove_from_list(
    dblist_t		*list,
    dblist_type_t	list_type,
    db_entry_t		*ep
)
{
    dblist_t		*lp;

    dce_assert(dhd_svc_handle, list_type == db_c_entry_list
	|| list_type == db_c_object_list || list_type == db_c_interface_list);
    switch (list_type) {
    case db_c_entry_list:
	lp = &ep->lists.entries;
	break;
    case db_c_object_list:
	lp = &ep->lists.objects;
	break;
    case db_c_interface_list:
	lp = &ep->lists.interfaces;
	break;
    }

    lp->back->fwd = lp->fwd;
    if (lp->fwd != NULL)
	lp->fwd->back = lp->back;
    else
	list->back = lp->back;		/* remove from end of list */
    lp->fwd = NULL;
    lp->back = NULL;
}

void
dblist_remove(
    db_lists_mgmt_t	*lists_mgmt,
    db_entry_t		*ep
)
{
    int			bucket;
    error_status_t	st2;

    remove_from_list(&lists_mgmt->entries, db_c_entry_list, ep);
    bucket = uuid_hash(&ep->object, &st2);
    bucket = db_hash2bucket(bucket);
    remove_from_list(&lists_mgmt->objects[bucket], db_c_object_list, ep);
    bucket = uuid_hash(&ep->ifid.uuid, &st2);
    bucket = db_hash2bucket(bucket);
    remove_from_list(&lists_mgmt->interfaces[bucket], db_c_interface_list, ep);
}

/*
**  Traversal.
*/
db_entry_t *
dblist_first(
    db_lists_mgmt_t	*lists_mgmt,
    dblist_type_t	list_type,
    uuid_t		*id
)
{
    int			bucket;
    error_status_t	st2;
    char		*p;

    dce_assert(dhd_svc_handle, list_type == db_c_entry_list
	|| list_type == db_c_object_list || list_type == db_c_interface_list);
    switch (list_type) {
    case db_c_entry_list:
	if ((p = (char *)lists_mgmt->entries.fwd) != NULL)
	    p -= db_c_entries_offset;
	break;
    case db_c_object_list:
	bucket = uuid_hash(id, &st2);
	bucket = db_hash2bucket(bucket);
	if ((p = (char *)lists_mgmt->objects[bucket].fwd) != NULL)
	    p -= db_c_objects_offset;
	break;
    case db_c_interface_list:
	bucket = uuid_hash(id, &st2);
	bucket = db_hash2bucket(bucket);
	if ((p = (char *)lists_mgmt->interfaces[bucket].fwd) != NULL)
	    p -= db_c_interfaces_offset;
	break;
    }

    return (db_entry_t *)p;
}

db_entry_t *
dblist_next(
    db_entry_t		*old,
    dblist_type_t	list_type
)
{
    char		*p;

    dce_assert(dhd_svc_handle, list_type == db_c_entry_list
	|| list_type == db_c_object_list || list_type == db_c_interface_list);
    switch (list_type) {
    case db_c_entry_list:
	if ((p = (char *)old->lists.entries.fwd) != NULL)
	    p -= db_c_entries_offset;
	break;
    case db_c_object_list:
	if ((p = (char *)old->lists.objects.fwd) != NULL)
	    p -= db_c_objects_offset;
	break;
    case db_c_interface_list:
	if ((p = (char *)old->lists.interfaces.fwd) != NULL)
	    p -= db_c_interfaces_offset;
	break;
    }

    return (db_entry_t *)p;
}

/*
**  Context.
*/


/*
**  Context is bad if there's no place to store the handle (i.e., caller
**  didn't want it), among other things.
*/
static boolean32
valid_context(
    epdb_handle_t	h,
    ept_lookup_handle_t	*lookup_handle
)
{
    db_context_t	ctx;

    if (lookup_handle == NULL)
	return FALSE;
    if (*lookup_handle == NULL)
	return TRUE;
    ctx = (db_context_t)*lookup_handle;
    if (ctx->h != h)
	return FALSE;

    switch (ctx->list_type) {
    default:
	return FALSE;
    case db_c_entry_list:
    case db_c_object_list:
    case db_c_interface_list:
	break;
    }

    return TRUE;
}

/*
**  Save list search context.  Database must be locked.
*/
void
dblist_ctx_save(
    epdb_handle_t	h,
    ept_lookup_handle_t	*lookup_handle,
    dblist_type_t	list_type,
    db_entry_t		*ep,
    unsigned32		pass
)
{
    db_context_t	ctx;

    /* If the entry list pointer is NULL, the search is done -- delete
     * context and return. */
    if (ep == NULL) {
	dblist_ctx_delete(h, lookup_handle);
	return;
    }

    /* Check that context handle is ok */
    if (!valid_context(h, lookup_handle))
	return;

    if (*lookup_handle != NULL) {
	/* Have a context; mark we're no longer reading the old entry. */
	ctx = (db_context_t)*lookup_handle;
	ctx->ep->readers--;
    }
    else {
	/* Get a context. */
	if ((ctx = malloc(sizeof *ctx)) == NULL)
	    return;
	*lookup_handle = (ept_lookup_handle_t *)ctx;
    }

    /* Bump readers, checking for overflow. */
    ep->readers++;
    if (ep->readers >= db_c_max_readers) {
	dblist_ctx_delete(h, lookup_handle);
	return;
    }

    /* Save state. */
    ctx->h = h;
    ctx->list_type = list_type;
    ctx->ep = ep;
    ctx->pass = pass;
}


void
dblist_ctx_restore(
    epdb_handle_t		h,
    ept_lookup_handle_t		*lookup_handle,
    dblist_type_t		*list_type,
    db_entry_t			**epp,
    unsigned32			*pass,
    error_status_t		*st
)
{
    db_context_t		ctx;

    if (!valid_context(h, lookup_handle) || *lookup_handle == NULL) {
	*st = ept_s_invalid_context;
	return;
    }

    ctx = (db_context_t)*lookup_handle;
    *list_type = ctx->list_type;
    *epp = ctx->ep;
    *pass = ctx->pass;
    *st = error_status_ok;
}


void
dblist_ctx_delete(
    epdb_handle_t	h,
    ept_lookup_handle_t	*lookup_handle
)
{
    db_context_t	ctx;

    if (!valid_context(h, lookup_handle) || *lookup_handle == NULL)
	return;
    ctx = (db_context_t)*lookup_handle;
    ctx->ep->readers--;
    free(ctx);
    *lookup_handle = NULL;
}


/*
**  Does lookup_handle point to a context for this database?
*/
boolean32
dblist_ctx_different(
    epdb_handle_t		h,
    ept_lookup_handle_t		*lookup_handle,
    error_status_t		*st
)
{
    db_context_t		ctx;

    *st = error_status_ok;

    if (lookup_handle == NULL)
	return FALSE;

    if (*lookup_handle != NULL) {
	ctx = (db_context_t)*lookup_handle;
	if (ctx->h != h) {
	    *st = ept_s_invalid_context;
	    return TRUE;
	}
    }

    return FALSE;
}



void
dblist_init(
    epdb_handle_t	h
)
{
    int			i;

    db_c_entries_offset = offsetof(db_entry_t, lists.entries);
    db_c_objects_offset = offsetof(db_entry_t, lists.objects);
    db_c_interfaces_offset = offsetof(db_entry_t, lists.interfaces);

    h->lists_mgmt.entries.fwd = NULL;
    h->lists_mgmt.entries.back = &h->lists_mgmt.entries;
    for (i = 0; i < db_c_nbucket; i++) {
	h->lists_mgmt.objects[i].fwd = NULL;
	h->lists_mgmt.objects[i].back = &h->lists_mgmt.objects[i];
	h->lists_mgmt.interfaces[i].fwd = NULL;
	h->lists_mgmt.interfaces[i].back = &h->lists_mgmt.interfaces[i];
    }

    pthread_mutex_init(&h->lock, pthread_mutexattr_default);
}
