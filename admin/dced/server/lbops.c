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
 * $Log: lbops.c,v $
 * Revision 1.1.7.2  1996/02/18  19:16:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:30  marty]
 *
 * Revision 1.1.7.1  1995/12/08  20:53:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/12/08  20:49:11  root]
 * 
 * Revision 1.1.2.12  1994/10/04  18:33:18  rsalz
 * 	Use thread_attr, not pthread_attr_default (OT CR 12485).
 * 	[1994/10/04  18:25:21  rsalz]
 * 
 * Revision 1.1.2.11  1994/09/17  08:05:13  rsalz
 * 	Rename epdb_sleep to dced_sleep.
 * 	[1994/09/17  07:57:01  rsalz]
 * 
 * Revision 1.1.2.10  1994/08/28  14:40:21  rsalz
 * 	Use dce_gettimeofday not dced_get_the_time (OT CR 11931).
 * 	[1994/08/28  14:34:34  rsalz]
 * 
 * Revision 1.1.2.9  1994/08/03  20:36:17  rsalz
 * 	Fix OT CR 11506.
 * 	[1994/08/03  20:32:14  rsalz]
 * 
 * Revision 1.1.2.6  1994/06/16  19:35:12  rsalz
 * 	Don't store conformant structures in dce_db (see OT CR 10971).
 * 	[1994/06/16  19:32:05  rsalz]
 * 
 * Revision 1.1.2.5  1994/06/13  20:05:31  rsalz
 * 	Move epdb_get_the_time to dced_get_the_time
 * 	[1994/06/13  17:31:30  rsalz]
 * 
 * Revision 1.1.2.4  1994/06/09  18:15:32  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:15  annie]
 * 
 * Revision 1.1.2.3  1994/05/31  20:24:01  rsalz
 * 	Add -e flag; fix some EP bugs
 * 	[1994/05/31  20:22:50  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/27  16:31:15  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:27  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/26  19:42:18  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:49  rsalz]
 * 
 * $EndLog$
 */
#include <oeimpl.h>
#include <comp.h>
#include <comtwr.h>
#include <comtwrflr.h>
#include <comtwrref.h>
#include <oe.h>
#include <lbimpl.h>

#define COPY_NEW_UUID_FROM_OLD(new, old)	\
	memcpy(&new, &old, sizeof new)
#define COPY_OLD_UUID_FROM_NEW(old, new) 	\
	memcpy(&old, &new, sizeof old)

static struct epdb_handle_s_t	llb_data;
static epdb_handle_t		llb_handle = &llb_data;
static llb_context_t		llb_contexts[llb_c_num_contexts];

/*
**  Context.
*/

static void
llb_ctx_init(
    void
)
{
    struct timeval	tv;
    unsigned16		c;
    int			i;

    dce_gettimeofday(&tv);
    c = (unsigned16)(tv.tv_sec & 0xFFFF);

    for (i = 0; i < llb_c_num_contexts; i++) {
	llb_contexts[i].free = TRUE;
	llb_contexts[i].llbh = LH_SET(i, c);
	llb_contexts[i].epth = NULL;
    }
}

static void
llb_ctx_free(
    lb_lookup_handle_t	*lookup_handle,
    llb_context_t	*chp
)
{
    if (*lookup_handle == chp->llbh)
	chp->free = TRUE;
    *lookup_handle = lb_default_lookup_handle;
}

/*
**  Garbage-collect "wanted" contexts; return a free one if there is one.
*/
static void
llb_ctx_gc(
    unsigned32		gc_secs,
    unsigned32		wanted,
    llb_context_t	**chpp
)
{
    struct timeval	now;
    llb_context_t	*chp;
    unsigned32		count;
    unsigned32		i;

    if (chpp != NULL)
	*chpp = NULL;

    dce_gettimeofday(&now);
    count = 0;
    for (i = 1, chp = &llb_contexts[i]; i < llb_c_num_contexts; i++, chp++) {
	if (!chp->free && now.tv_sec - chp->last_used >= gc_secs) {
	    dblist_ctx_delete(llb_handle, &chp->epth);
	    chp->free = TRUE;
	}

	if (chp->free && ++count >= wanted) {
	    if (chpp != NULL)
		*chpp = chp;
	    break;
	}
    }
}

/*
**  If lookup_handle contains an existing context, point to its entry in the
**  llb_contexts array.  If its lb_default_lookup_handle, create one.
*/
static llb_context_t *
llb_ctx_get_ptr(
    lb_lookup_handle_t	*lookup_handle,
    error_status_t	*st
)
{
    llb_context_t	*chp;
    unsigned32		i;

    *st = lb_cant_access;
    if (lookup_handle == NULL)
	return NULL;

    if (*lookup_handle != lb_default_lookup_handle) {
	/* Allocated context -- validate it. */
	i = LH_GET_OFFSET(*lookup_handle);
	if (i < 1 || i >= llb_c_num_contexts)
	    return NULL;
	chp = &llb_contexts[i];
	if (chp->free || *lookup_handle != chp->llbh)
	    return NULL;
    }
    else {
	/* Find a free slot. */
	for (i = 1, chp = &llb_contexts[1]; i < llb_c_num_contexts; i++, chp++)
	    if (chp->free)
		break;
	if (i >= llb_c_num_contexts) {
	    /* All in use -- obnoxiously free one. */
	    llb_ctx_gc(llb_c_short_context, 1, &chp);
	    if (chp == NULL)
		return NULL;
	}

	chp->free = FALSE;
	chp->llbh = LH_SET_COOKIE(chp->llbh, LH_GET_COOKIE(chp->llbh) + 1);
	chp->epth = NULL;
    }

    *st = error_status_ok;
    return chp;
}

static void
llb_ctx_save(
    lb_lookup_handle_t	*lookup_handle,
    dblist_type_t	list_type,
    db_entry_t		*ep
)
{
    struct timeval	tv;
    llb_context_t	*chp;
    error_status_t	st2;

    if (lookup_handle == NULL)
	return;

    /* Return if lookup_handle wasn't allocated and no context needed. */
    if (*lookup_handle == lb_default_lookup_handle && ep == NULL)
	return;

    chp = llb_ctx_get_ptr(lookup_handle, &st2);
    if (*&st2 != error_status_ok) {
	*lookup_handle = lb_default_lookup_handle;
	return;
    }

    dblist_ctx_save(llb_handle, &chp->epth, list_type, ep, 1);
    if (chp->epth == NULL)
	/* No context to save so free slot. */
	llb_ctx_free(lookup_handle, chp);
    else {
	/* Context saved.  Set last_used to current time. */
	dce_gettimeofday(&tv);
	chp->last_used = tv.tv_sec;
	*lookup_handle = chp->llbh;
    }
}

static void
llb_ctx_get(
    lb_lookup_handle_t	*lookup_handle,
    dblist_type_t	*list_type,
    db_entry_t		**epp,
    error_status_t	*st
)
{
    llb_context_t	*chp;
    unsigned32		pass;

    if (lookup_handle == NULL || *lookup_handle == lb_default_lookup_handle) {
	*st = lb_cant_access;
	return;
    }

    chp = llb_ctx_get_ptr(lookup_handle, st);
    if (*st != error_status_ok) {
	*lookup_handle = lb_default_lookup_handle;
	return;
    }

    dblist_ctx_restore(llb_handle, &chp->epth, list_type, epp, &pass, st);
    if (*st != error_status_ok) {
	llb_ctx_free(lookup_handle, chp);
	return;
    }
}

/*
**  Cleanup tasks.
*/

static void
llb_delete_entry(
    db_entry_t		*ep,
    error_status_t	*st
)
{
    epdb_delete_entry(llb_handle, ep, st);
}

static void
llb_purge_ents(
    void
)
{
    unsigned32		count;
    db_entry_t		*ep;
    db_entry_t		*next;
    error_status_t	st;

    ep = dblist_first(&llb_handle->lists_mgmt, db_c_entry_list, NULL);
    for (count = 0; ep != NULL; ep = next) {
	next = dblist_next(ep, db_c_entry_list);
	if (ep->flags & db_entry_c_deleted && ep->readers == 0) {
	    llb_delete_entry(ep, &st);
	    if (++count > llb_live_c_max_deletes) {
		count = 0;
		ep->readers++;
		epdb_unlock(llb_handle);
		dced_sleep(60);
		epdb_lock(llb_handle);
		ep->readers--;
	    }
	}
    }
}

/*
**  Purge entries marked as deleted.  Also purge inactive contexts.
*/
static void *
llb_ping1(
    void		*dummy
)
{
    struct timeval	now;

    dce_gettimeofday(&now);
    for ( ; ; ) {
	dced_sleep_until(&now, llb_c_live_long_wait);
	epdb_lock(llb_handle);
	llb_ctx_gc(llb_c_long_context, llb_c_num_contexts, NULL);
	llb_purge_ents();
	epdb_unlock(llb_handle);
    }
    return NULL;
}


static void
llb_insert_entry(
    lb_entry_t		*xentry,
    twr_t		*tower,
    rpc_addr_p_t	addr,
    error_status_t	*st
)
{
    static db_entry_t	nil_entry;
    db_entry_t		*ep;
    error_status_t	st2;

    if ((ep = malloc(sizeof *ep)) == NULL) {
	*st = ept_s_cant_perform_op;
	return;
    }
    *ep = nil_entry;
    uuid_create(&ep->dbid, st);
    if (*st != error_status_ok) {
	free(ep);
	return;
    }
    dced_tower_copy(&ep->tower, tower, dced_copy_via_malloc, st);
    if (*st != error_status_ok) {
	free(ep);
	*st = ept_s_cant_perform_op;
	return;
    }
    COPY_NEW_UUID_FROM_OLD(ep->object, xentry->object);
    COPY_NEW_UUID_FROM_OLD(ep->type, xentry->obj_type);
    COPY_NEW_UUID_FROM_OLD(ep->ifid.uuid, xentry->obj_interface);
    if (uuid_is_nil(&ep->object, &st2))
	ep->flags |= db_c_nil_object;
    if (uuid_is_nil(&ep->ifid.uuid, &st2))
	ep->flags |= db_c_nil_ifspec;
    ep->llb_flags = xentry->flags;
    ep->saddr_len = xentry->saddr_len;
    ep->addr = addr;
    memcpy(ep->annotation, xentry->annotation, sizeof ep->annotation);

    epdb_write_entry(llb_handle, ep, st);
    if (*st != error_status_ok)
	return;
    dblist_add(&llb_handle->lists_mgmt, ep);
}

/*
**  Return an entry that exactly matches xentry and addr except for
**  the endpoint.
*/
static db_entry_t *
llb_lookup_entry(
    lb_entry_t		*xentry,
    rpc_addr_p_t	addr,
    error_status_t	*st
)
{
    db_entry_t		*ep;
    dblist_type_t	list_type;
    unsigned char	*netaddr;
    unsigned char	*netaddr2;
    error_status_t	st2;

    rpc__naf_addr_inq_netaddr(addr, &netaddr, st);
    if (*st != error_status_ok) {
	*st = lb_bad_entry;
	return NULL;
    }

    if (!uuid_is_nil((uuid_t *)&xentry->object, &st2)) {
	list_type = db_c_object_list;
	ep = dblist_first(&llb_handle->lists_mgmt, list_type, (uuid_t *)&xentry->object);
    }
    else {
	list_type = db_c_interface_list;
	ep = dblist_first(&llb_handle->lists_mgmt, list_type, (uuid_t *)&xentry->obj_interface);
    }

    for (; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (ep->flags & db_entry_c_deleted)
	    continue;
	if (uuid_equal((uuid_t *)&xentry->object, &ep->object, &st2)
	 && uuid_equal((uuid_t *)&xentry->obj_type, &ep->type, &st2)
	 && uuid_equal((uuid_t *)&xentry->obj_interface, &ep->ifid.uuid, &st2)
	 && addr->rpc_protseq_id == ep->addr->rpc_protseq_id) {
	    rpc__naf_addr_inq_netaddr(ep->addr, &netaddr2, st);
	    if (*st != error_status_ok)
		break;
	    if (strcmp((char *)netaddr, (char *)netaddr2) == 0) {
		rpc_string_free(&netaddr2, &st2);
		break;
	    }
	    rpc_string_free(&netaddr2, &st2);
	}
    }

    rpc_string_free(&netaddr, &st2);
    return ep;
}

/*
**  Do a little consistency checking an lb_entry
*/
static void
lb_chk_entry(
    lb_entry_t		*xentry,
    error_status_t	*st
)
{
    /* Pessimistic. */
    *st = lb_bad_entry;

    if (xentry->saddr_len < sizeof xentry->saddr.family
     || xentry->saddr_len > sizeof xentry->saddr)
	return;

    switch ((int)xentry->saddr.family) {
    default:
	return;
    case RPC_C_NAF_ID_DDS:
	if (!RPC_PROTSEQ_INQ_SUPPORTED(RPC_C_PROTSEQ_ID_NCADG_DDS))
	    return;
    	break;
    case RPC_C_NAF_ID_IP:
	if (!RPC_PROTSEQ_INQ_SUPPORTED(RPC_C_PROTSEQ_ID_NCADG_IP_UDP))
	    return;
    	break;
    }

    *st = error_status_ok;
}

static void
lb_entry_to_addr(
    lb_entry_t		*xentry,
    rpc_addr_p_t	*addr,
    error_status_t	*st
)
{
    rpc__naf_addr_from_sa((sockaddr_t *)&xentry->saddr, xentry->saddr_len,
	addr, st);
    if (*st != error_status_ok)
	return;

    switch ((int)xentry->saddr.family) {
    default:
	rpc__naf_addr_free(addr, st);
	*st = lb_bad_entry;
	return;
    case RPC_C_NAF_ID_DDS:
	(*addr)->rpc_protseq_id = RPC_C_PROTSEQ_ID_NCADG_DDS;
	break;
    case RPC_C_NAF_ID_IP:
	(*addr)->rpc_protseq_id = RPC_C_PROTSEQ_ID_NCADG_IP_UDP;
	break;
    }
}

/*
**  Create a tower and RPC address from an llb entry.
*/
static void
lb_entry_to_tower(
    lb_entry_t			*xentry,
    twr_t			**tower,
    rpc_addr_p_t		*addr,
    error_status_t		*st
)
{
    static rpc_syntax_id_t	ndr_syntax_id = {
	{0x8a885d04u, 0x1ceb, 0x11c9, 0x9f, 0xe8,
	 {0x8, 0x0, 0x2b, 0x10, 0x48, 0x60}},
	2
    };
    static rpc_if_rep_t		ifspec_template = {
	/* version	opcount	i/f version	uuid */
	    1,		4,	4,		{0},
	/* rt vers	epv		syntax vector */
	    2,		{0, NULL},	{1, &ndr_syntax_id},
	/* server_epv	mgr_epv */
	    NULL,	NULL
    };
    rpc_addr_p_t		addr2;
    rpc_protocol_id_t		prot_id;
    rpc_binding_rep_t		*bh;
    rpc_if_rep_t		ifspec;
    rpc_tower_ref_vector_t	*tower_vec;
    error_status_t		 st2;

    lb_entry_to_addr(xentry, addr, st);
    if (*st != error_status_ok)
	return;
    rpc__naf_addr_copy(*addr, &addr2, st);
    if (*st != error_status_ok)
	return;

    prot_id = RPC_PROTSEQ_INQ_PROT_ID((*addr)->rpc_protseq_id);
    bh = rpc__binding_alloc(FALSE, (uuid_t *)&xentry->object, prot_id,
		    addr2, st);
    if (*st != error_status_ok) {
	rpc__naf_addr_free(addr, &st2);
	rpc__naf_addr_free(&addr2, &st2);
	return;
    }

    /* Make up an ifspec from the template */
    ifspec = ifspec_template;
    COPY_NEW_UUID_FROM_OLD(ifspec.id, xentry->obj_interface);
    ifspec.vers = 0;

    /* Convert the binding handle to tower_ref vector and then to a tower */
    rpc__tower_ref_vec_from_binding(&ifspec, (rpc_binding_handle_t)bh,
	&tower_vec, st);
    if (*st != error_status_ok) {
	rpc__naf_addr_free(addr, &st2);
	rpc_binding_free((rpc_binding_handle_t *)&bh, &st2);
	return;
    }

    rpc__tower_from_tower_ref(tower_vec->tower[0], tower, st);
    if (*st != error_status_ok) {
	rpc__naf_addr_free(addr, &st2);
    }

    rpc_binding_free((rpc_binding_handle_t *)&bh, &st2);
    rpc__tower_ref_vec_free(&tower_vec, &st2);
}

static void
llb_to_lb_entry(
    db_entry_t		*ep,
    lb_entry_t		*xentry
)
{
    rpc_addr_p_t	addr;

    COPY_OLD_UUID_FROM_NEW(xentry->object, ep->object);
    COPY_OLD_UUID_FROM_NEW(xentry->obj_type, ep->type);
    COPY_OLD_UUID_FROM_NEW(xentry->obj_interface, ep->ifid.uuid);
    xentry->flags = ep->llb_flags;
    xentry->saddr_len = ep->saddr_len;
    addr = ep->addr;
    memcpy(&xentry->saddr, &addr->sa, addr->len);
    memcpy(xentry->annotation, ep->annotation, sizeof xentry->annotation);
}



static void
do_llb_insert(
    lb_entry_t		*xentry,
    error_status_t	*st
)
{
    db_entry_t		*ep;
    twr_t		*tower;
    rpc_addr_p_t	addr;
    error_status_t	st2;

    lb_chk_entry(xentry, st);
    if (*st != error_status_ok)
	return;

    tower = NULL;
    addr = NULL;
    lb_entry_to_tower(xentry, &tower, &addr, st);
    if (*st != error_status_ok) {
	*st = lb_bad_entry;
	return;
    }

    epdb_lock(llb_handle);
    ep = llb_lookup_entry(xentry, addr, st);
    if (*st == error_status_ok) {
	if (ep)
	    /* Delete existing entry. */
	    llb_delete_entry(ep, st);
	if (*st == error_status_ok)
	    llb_insert_entry(xentry, tower, addr, st);
    }
    epdb_unlock(llb_handle);

    if (*st != error_status_ok)
	rpc__naf_addr_free(&addr, &st2);
    rpc__tower_free(&tower, &st2);
}


static void
do_llb_delete(
    lb_entry_t		*xentry,
    error_status_t	*st
)
{
    db_entry_t		*ep;
    rpc_addr_p_t	addr;
    error_status_t	st2;

    lb_chk_entry(xentry, st);
    if (*st != error_status_ok)
	return;

    addr = NULL;
    lb_entry_to_addr(xentry, &addr, st);
    if (*st != error_status_ok)
	return;

    epdb_lock(llb_handle);
    ep = llb_lookup_entry(xentry, addr, st);
    if (*st == error_status_ok && ep != NULL)
	/* Entry found, delete it */
	llb_delete_entry(ep, st);
    epdb_unlock(llb_handle);

    rpc__naf_addr_free(&addr, &st2);
}


/*
**  Return entries which match the specified object, obj_type, and interface.
**  If object is uuid_nil, return entries with any object
**  If obj_type is uuid_nil, return entries with any type
**  If interface is uuid_nil, return entries with any interface
*/
static void
do_llb_lookup(
    uuid_t		*object,
    uuid_t		*obj_type,
    uuid_t		*interface,
    lb_lookup_handle_t	*lookup_handle,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    lb_entry_t		*entries,
    error_status_t	*st
)
{
    boolean32		nil_object;
    boolean32		nil_type;
    boolean32		nil_ifspec;
    unsigned32		count;
    dblist_type_t	list_type;
    db_entry_t		*ep;
    error_status_t	st2;

    *num_ents = 0;
    if (entries == NULL || max_ents < 1) {
	*st = lb_bad_entry;
	return;
    }
    if (lookup_handle == NULL) {
	*st = lb_cant_access;
	return;
    }

    nil_object = object == NULL || uuid_is_nil(object, &st2);
    nil_type = obj_type == NULL || uuid_is_nil(obj_type, &st2);
    nil_ifspec = interface == NULL || uuid_is_nil(interface, &st2);

    epdb_lock(llb_handle);
    if (*lookup_handle == lb_default_lookup_handle) {
	if (!nil_object) {
	    list_type = db_c_object_list;
	    ep = dblist_first(&llb_handle->lists_mgmt, list_type, object);
	}
	else if (!nil_ifspec) {
	    list_type = db_c_interface_list;
	    ep = dblist_first(&llb_handle->lists_mgmt, list_type, interface);
	}
	else {
	    list_type = db_c_entry_list;
	    ep = dblist_first(&llb_handle->lists_mgmt, list_type, NULL);
	}
    }
    else {
	/* Get context associated with lookup_handle. */
	llb_ctx_get(lookup_handle, &list_type, &ep, st);
	if (*st != error_status_ok) {
	    epdb_unlock(llb_handle);
	    return;
	}
    }

    for (count = 0; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (ep->flags & db_entry_c_deleted)
	    continue;
	if ((nil_object || uuid_equal(object, &ep->object, &st2))
	 && (nil_type || uuid_equal(obj_type, &ep->type, &st2))
	 && (nil_ifspec || uuid_equal(interface, &ep->ifid.uuid, &st2))) {
	    /* Matched. Full? */
	    if (count >= max_ents)
		break;
	    llb_to_lb_entry(ep, &entries[count++]);
	}
    }

    llb_ctx_save(lookup_handle, list_type, ep);
    epdb_unlock(llb_handle);
    *num_ents = count;
    *st = count == 0 ? lb_not_registered : error_status_ok;
}


static void
map_match_llb(
    uuid_t		*object,
    uuid_t		*interface,
    rpc_protseq_id_t	protseq,
    unsigned32		max_ents,
    unsigned32		*n_ents,
    db_entry_t		*entries[],
    ept_lookup_handle_t	*map_handle,
    unsigned32		pass,
    dblist_type_t	list_type,
    db_entry_t		**epp
)
{
    boolean32		nil_object;
    boolean32		nil_ifspec;
    boolean32		match;
    error_status_t	st2;
    db_entry_t		*ep;

    nil_object = uuid_is_nil(object, &st2);
    nil_ifspec = uuid_is_nil(interface, &st2);
    for (ep = *epp; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (ep->flags & db_entry_c_deleted)
	    continue;
	if (pass == 3) {
	    if ((nil_object || ep->flags & db_c_nil_object)
	     && !(nil_object && ep->flags & db_c_nil_object)
	     && ((nil_ifspec && ep->flags & db_c_nil_ifspec)
	       || uuid_equal(interface, &ep->ifid.uuid, &st2))
	      && protseq == ep->addr->rpc_protseq_id)
		match = TRUE;
	    else
		match = FALSE;
	}
	else {
	    if (((nil_object && ep->flags & db_c_nil_object) || uuid_equal(object, &ep->object, &st2))
	     && ((nil_ifspec && ep->flags & db_c_nil_ifspec) || uuid_equal(interface, &ep->ifid.uuid, &st2))
	     && protseq == ep->addr->rpc_protseq_id)
		match = TRUE;
	    else
		match = FALSE;
	}

	if (match) {
	    /* Found.  Full? */
	    if (*n_ents >= max_ents)
		break;
	    entries[*n_ents] = ep;
	    (*n_ents)++;
	    /* If no context and we're full, we're done. */
	    if (map_handle == NULL && *n_ents >= max_ents)
		break;
	}
    }

    *epp = ep;
}

/*
 *  Search the llb database looking for entries which match the target object,
 *  interface, and protseq.  Target interface uuid must be non-nil.
 *      pass 1 - match object and interface (MOI)
 *          if target object is non-nil,
 *              search objectq for exact object/interface match
 *          if target object is nil,
 *              search interfaceq for exact interface match/object nil
 *      pass 2 - match object (MO)
 *          if target object is non-nil,
 *              search objectq for object match/interface nil
 *      pass 3 - match interface (MI)
 *          search interfaceq for interface match/target object XOR entry object nil
 */
static void
map_llb(
    uuid_t		*object,
    uuid_t		*interface,
    rpc_protseq_id_t	protseq,
    ept_lookup_handle_t	*map_handle,
    unsigned32		max_ents,
    unsigned32		*n_ents,
    db_entry_t		*entries[],
    error_status_t	*st
)
{
    boolean32		nil_object;
    unsigned32		pass;
    dblist_type_t	list_type;
    db_entry_t		*ep;
    error_status_t	st2;

    *st = error_status_ok;
    nil_object = uuid_is_nil(object, &st2);

    if (map_handle == NULL || *map_handle == NULL) {
	pass = 1;
	if (nil_object) {
	    list_type = db_c_interface_list;
	    ep = dblist_first(&llb_handle->lists_mgmt, list_type, interface);
	}
	else {
	    list_type = db_c_object_list;
	    ep = dblist_first(&llb_handle->lists_mgmt, list_type, object);
	}
    }
    else {
	dblist_ctx_restore(llb_handle, map_handle, &list_type, &ep, &pass, st);
	if (*st != error_status_ok)
	    return;
    }

    /*  search for object/interface exact match */
    if (pass == 1) {
	map_match_llb(object, interface, protseq, max_ents, n_ents, entries,
	    map_handle, pass, list_type, &ep);

	if (*n_ents >= max_ents && (ep != NULL || map_handle == NULL)) {
	    /* Full and we're either done or there's no context. */
	    dblist_ctx_save(llb_handle, map_handle, list_type, ep, pass);
	    return;
	}

	/* Get organized for next pass. */
	if (!nil_object) {
	    pass = 2;
	    list_type = db_c_object_list;
	    ep = dblist_first(&llb_handle->lists_mgmt, list_type, object);
	}
	else {
	    pass = 3;
	    list_type = db_c_interface_list;
	    ep = dblist_first(&llb_handle->lists_mgmt, list_type, interface);
	}
    }

    /* Search for object match, interface nil */
    if (pass == 2) {
	map_match_llb(object, &uuid_g_nil_uuid, protseq, max_ents, n_ents,
	    entries, map_handle, pass, list_type, &ep);
	if (*n_ents >= max_ents && (ep != NULL || map_handle == NULL)) {
	    /* Full and we're either done or there's no context. */
	    dblist_ctx_save(llb_handle, map_handle, list_type, ep, pass);
	    return;
	}

	/* Get organized for next pass. */
	pass = 3;
	list_type = db_c_interface_list;
	ep = dblist_first(&llb_handle->lists_mgmt, list_type, interface);
    }

    /* Search for interface match, target object XOR entry object nil. */
    if (pass == 3) {
	map_match_llb(object, interface, protseq, max_ents, n_ents, entries,
	    map_handle, pass, list_type, &ep);
    }

    dblist_ctx_save(llb_handle, map_handle, list_type, ep, pass);

    *st = *n_ents == 0 ? ept_s_not_registered : error_status_ok;
}


/*
**  Invoke map_llb to do a sequence of searches through the llb database
**  for appropriate entries to forward to and return pointers to these
**  entries.  Copy the entries' addr info into fwd_addrs.
**
**  This routine is invoked from the ncs2.0 side which automagically
**  manages client/server context handles so the context handles in these
**  routines don't need additional llb mgmt.
*/
void
llb_fwd(
    uuid_t		*object,
    uuid_t		*interface,
    rpc_addr_p_t	addr,
    unsigned32		*num_addrs,
    rpc_addr_p_t	fwd_addrs[],
    error_status_t	*st
)
{
    db_entry_t		*entry;

    if (fwd_addrs == NULL || *num_addrs > 1) {
	*st = ept_s_cant_perform_op;
	epdb_unlock(llb_handle);
	return;
    }

    epdb_lock(llb_handle);
    map_llb(object, interface, addr->rpc_protseq_id, NULL, 1,
	num_addrs, &entry, st);
    if (*st != error_status_ok) {
	epdb_unlock(llb_handle);
	return;
    }

    if (*num_addrs == 1) {
	rpc__naf_addr_copy(entry->addr, &fwd_addrs[0], st);
	if (*st != error_status_ok) {
	    *num_addrs = 0;
	    *st = ept_s_cant_perform_op;
	}
    }
    else {
	dce_assert(dhd_svc_handle, *num_addrs == 0);
	*st = ept_s_cant_perform_op;
    }

    epdb_unlock(llb_handle);
}

/*
**  Invoke map_llb to do a sequence of searches through the llb database
**  for appropriate entries to forward to and return pointers to these
**  entries.  Copy the entries' tower info into fwd_towers.
**
**  This routine is invoked from the ncs2.0 side which automagically
**  manages client/server context handles so the context handles in these
**  routines don't need additional llb mgmt.
*/
void
llb_map(
    uuid_t		*object,
    twr_t		*map_tower,
    ept_lookup_handle_t	*lookup_handle,
    unsigned32		max_towers,
    unsigned32		*num_towers,
    twr_t		*fwd_towers[],
    error_status_t	*st
)
{
    twr_fields_t	fields;
    db_entry_t		**entries;
    unsigned32		start_ent;
    int			i;

    if (dblist_ctx_different(llb_handle, lookup_handle, st))
	return;
    epdb_lock(llb_handle);

    if (fwd_towers == NULL || max_towers == 0 || *num_towers > max_towers) {
	dblist_ctx_delete(llb_handle, lookup_handle);
	*st = ept_s_cant_perform_op;
	epdb_unlock(llb_handle);
	return;
    }

    tower_to_fields(map_tower, &fields, st);
    if (*st != error_status_ok) {
	dblist_ctx_delete(llb_handle, lookup_handle);
	*st = ept_s_invalid_entry;
	epdb_unlock(llb_handle);
	return;
    }

    entries = (db_entry_t **)malloc(max_towers * sizeof *entries);
    if (entries == NULL) {
	dblist_ctx_delete(llb_handle, lookup_handle);
	*st = ept_s_cant_perform_op;
	epdb_unlock(llb_handle);
	return;
    }

    start_ent = *num_towers;
    map_llb(object, &fields.ifid.uuid, fields.protseq, lookup_handle, max_towers,
	num_towers, entries, st);
    if (*st != error_status_ok) {
	free(entries);
	epdb_unlock(llb_handle);
	return;
    }

    for (i = start_ent; i < *num_towers; i++) {
	dced_tower_copy(&fwd_towers[i], entries[i]->tower, dced_copy_for_stub, st);
	if (*st != error_status_ok) {
	    /* Return what we got but kill context. */
	    dblist_ctx_delete(llb_handle, lookup_handle);
	    *num_towers = i;
	    *st = *num_towers > 0 ? error_status_ok : ept_s_cant_perform_op;
	    break;
	}
    }

    free(entries);
    epdb_unlock(llb_handle);
}

/*
**  External interface for the LLB interface.
*/

static void
ept_status_to_llb_status(
    error_status_t	*st
)
{
    switch ((int)*st) {
    default:
	/* For 100% compatability, perhaps we should map
	 * all DCE 1.0 codes to NCS 1.5.1 codes.  For now, just
	 * leave the code as is. */
	break;
    case ept_s_cant_perform_op:
    case ept_s_no_memory:
	*st = lb_cant_access;
	break;
    case ept_s_database_invalid:
    case ept_s_cant_create:
	*st = lb_database_invalid;
	break;
    case ept_s_cant_access:
	*st = lb_cant_access;
	break;
    case ept_s_database_already_open:
	*st = lb_database_busy;
	break;
    case ept_s_invalid_entry:
	*st = lb_bad_entry;
	break;
    case ept_s_update_failed:
	*st = lb_update_failed;
	break;
    case ept_s_invalid_context:
	*st = lb_cant_access;
	break;
    case ept_s_not_registered:
	*st = lb_not_registered;
	break;
    case ept_s_server_unavailable:
	*st = lb_server_unavailable;
	break;
    }
}

static void
dced_llb_insert(
    handle_t		h,
    lb_entry_t		*xentry,
    error_status_t	*st
)
{
    do_llb_insert(xentry, st);
    ept_status_to_llb_status(st);
}

static void
dced_llb_delete(
    handle_t		h,
    lb_entry_t		*xentry,
    error_status_t	*st
)
{
    do_llb_delete(xentry, st);
    ept_status_to_llb_status(st);
}


static void
dced_llb_lookup(
    handle_t		h,
    uuid_t		*object,
    uuid_t		*obj_type,
    uuid_t		*obj_interface,
    lb_lookup_handle_t	*lookup_handle,
    unsigned32		max_num_results,
    unsigned32		*num_results,
    lb_entry_t		result_entries[],
    error_status_t	*st
)
{
    do_llb_lookup(object, obj_type, obj_interface, lookup_handle,
	max_num_results, num_results, result_entries, st);
    ept_status_to_llb_status(st);
}

void
llb_init(
    error_status_t	*st
)
{
    /* Read the LLB database. */
    dblist_init(llb_handle);
    llb_ctx_init();
    epdb_database_read(llb_handle, LLB_DB_NAME, st);
    if (*st != error_status_ok)
	return;
    pthread_create(&llb_handle->sliv_task1, thread_attr, llb_ping1, llb_handle);
    ncs_mgmt_if_register_epv(st);
}

llb__v4_0_epv_t llb_v4_0_mgr_epv = {
    dced_llb_insert, dced_llb_delete, dced_llb_lookup
};
