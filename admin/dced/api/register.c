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
 * $Log: register.c,v $
 * Revision 1.1.8.3  1996/02/18  19:15:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:57  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:26:57  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/28  22:15 UTC  jss
 * 	Merge fix for CHFts15336.
 * 
 * 	HP revision /main/jss_mothra_fix6/1  1995/07/27  17:13 UTC  jss
 * 	Fix for CHFts15336. Temporary hack. Use rpc_ep_register().
 * 	[1995/12/08  17:59:16  root]
 * 
 * Revision 1.1.4.2  1994/10/26  19:54:33  rsalz
 * 	Use srvrexec endpoints (OT CR 12061).
 * 	Fix last of OT CR 12568.
 * 	[1994/10/26  19:42:47  rsalz]
 * 
 * Revision 1.1.4.1  1994/10/25  21:49:57  max
 * 	CR 12568 general fixes
 * 	[1994/10/25  21:49:33  max]
 * 
 * Revision 1.1.2.7  1994/10/06  20:22:12  agd
 * 	Merged with changes from 1.1.2.6
 * 	[1994/10/06  20:22:06  agd]
 * 
 * 	expand copyright
 * 	[1994/10/06  14:11:49  agd]
 * 
 * 	expand copyright
 * 
 * Revision 1.1.2.6  1994/10/06  15:43:07  bowe
 * 	Fix bad loop index in dce_server_register() [CR 12534]
 * 	[1994/10/06  15:42:47  bowe]
 * 
 * Revision 1.1.2.5  1994/10/03  19:19:14  rsalz
 * 	Let use_protseq happen first via NULL handle.
 * 	Register endpoints (OT CR 12061).
 * 	[1994/10/03  19:17:43  rsalz]
 * 
 * Revision 1.1.2.4  1994/09/06  19:56:23  rsalz
 * 	Real fix for OT CR 11936.
 * 	Add srvrexec_exited; finish OT CR 11791.
 * 	[1994/09/06  19:34:10  rsalz]
 * 
 * Revision 1.1.2.3  1994/08/29  14:58:25  bowe
 * 	Temporarily comment out calls to srvr_{en,dis}able_service() [CR 11936]
 * 	[1994/08/29  14:58:05  bowe]
 * 
 * Revision 1.1.2.2  1994/08/28  17:56:16  rsalz
 * 	Add missing server convenience functions (OT CR 11791).
 * 	[1994/08/28  17:55:34  rsalz]
 * 
 * Revision 1.1.2.1  1994/08/25  16:32:07  rsalz
 * 	Add dce_server_register & co.
 * 	[1994/08/25  16:29:22  rsalz]
 * 
 * $EndLog$
 */
#include <dcedimpl.h>
#include <dce/stubbase.h>

#define RPC_IF_VERS_MAJOR(v)	(((v)      ) & 0xFFFF)
#define RPC_IF_VERS_MINOR(v)	(((v) >> 16) & 0xFFFF)

/*
**  Following line, if enabled, causes dce_server_register to use
**  rpc_ep_register calls.
*/
#define USE_EP_API


struct dce_server_handle_s_t {
    uuid_t			instance;
    boolean			configured;
    server_t			*sp;
    unsigned32			flags;
    dce_server_register_data_t	*data;
    rpc_binding_vector_t	*bv;
    unsigned32			where_bound;
    int				protseq_count;
    unsigned char		**protseq_list;
    uuid_vector_p_t		*uuidvec;
    srvrexec_ep_list_t		eplist;
    rpc_tower_vector_t		***tvecs;
};

/*
**  The API is designed to support "modular" servers as described in
**  DCE RFC 21.0.  For now there is only one server, however, so we
**  always use that.
*/
static struct dce_server_handle_s_t mainhandle;


static service_t *
ifhandle2service(
    dce_server_handle_t	h,
    rpc_if_handle_t	ifhandle,
    error_status_t	*st
)
{
    service_t		*sp;
    unsigned32		i;
    rpc_if_rep_t	*ifrep;

    ifrep = (rpc_if_rep_t *)ifhandle;
    for (sp = h->sp->services.list, i = 0;
	    i < h->sp->services.count; i++, sp++)
	if (uuid_equal(&sp->ifspec.uuid, &ifrep->id, st)
	 && sp->ifspec.vers_major == RPC_IF_VERS_MAJOR(ifrep->vers)
	 && sp->ifspec.vers_minor == RPC_IF_VERS_MINOR(ifrep->vers)) {
	    *st = error_status_ok;
	    return sp;
	}

    *st = ept_s_not_registered;
    return NULL;
}


static dce_server_register_data_t *
ifid2regdata(
    dce_server_handle_t		h,
    rpc_if_id_t			*ifid
)
{
    rpc_if_rep_t		*ifrep;
    error_status_t		st;
    dce_server_register_data_t	*rp;
    unsigned32			i;

    for (rp = h->data, i = 0; i < h->sp->services.count; i++, rp++) {
	ifrep = (rpc_if_rep_t *)rp->ifhandle;
	if (uuid_equal(&ifid->uuid, &ifrep->id, &st)
	 && ifid->vers_major == RPC_IF_VERS_MAJOR(ifrep->vers)
	 && ifid->vers_minor == RPC_IF_VERS_MINOR(ifrep->vers))
	    return rp;
    }
    return NULL;
}

static void
get_our_bindings(
    dce_server_handle_t	h,
    handle_t		*dh,
    error_status_t	*st
)
{
#if	0
    /* XXX?  See comment below. */
    rpc_binding_rep_t	*brep;
#endif	/* 0 */
    unsigned32		i;
    boolean		bound;

    rpc_server_inq_bindings(&h->bv, st);
    if (*st != error_status_ok)
	return;
    if (h->bv == NULL || h->bv->count == 0) {
        *st = rpc_s_no_bindings;
        return;
    }

    /* Ensure all bindings have endpoints.  Pick one of them and use it to
     * bind to dced. */
    for (bound = FALSE, i = 0; i < h->bv->count; i++)
        if (h->bv->binding_h[i] != NULL) {
#if	0
	    /* XXX?  We don't want to do this because it is too difficult
	     * to include the necessary internal RPC header file to get the
	     * binding_rep_t. */
	    brep = (rpc_binding_rep_t *)h->bv->binding_h[i];
	    if (!brep->addr_has_endpoint) {
                *st = rpc_s_invalid_binding;
                return;
            }
#endif	/* 0 */
	    if (!bound) {
		rpc_binding_to_epmap(h->bv->binding_h[i], dh, st);
		if (*st == error_status_ok) {
		    bound = TRUE;
		    h->where_bound = i;
		}
	    }
	}
    if (!bound) {
        *st = rpc_s_no_bindings;
        return;
    }
    *st = error_status_ok;
}



void
dce_server_use_protseq(
    dce_server_handle_t	h,
    unsigned char	*protseq,
    error_status_t	*st
)
{
    unsigned char	*p;

    if (h == NULL)
	h = &mainhandle;
    if (!rpc_network_is_protseq_valid(protseq, st))
	return;
    if (h->protseq_count == 0)
	h->protseq_list = malloc(sizeof *h->protseq_list);
    else
	h->protseq_list = realloc(h->protseq_list,
	    (h->protseq_count + 1) * sizeof *h->protseq_list);
    p = (unsigned char *)dce_strdup((char *)protseq);
    h->protseq_list[h->protseq_count++] = p;
    *st = error_status_ok;
}


void
dce_server_register(
    unsigned32			flags,
    server_t			*server,
    dce_server_register_data_t	*data,
    dce_server_handle_t		*new,
    error_status_t		*st
)
{
    unsigned32			i;
    unsigned32			j;
    unsigned32			k;
    unsigned32			l;
    service_t			*sp;
    idl_char			*p;
    uuid_vector_t		*uv;
    dce_server_handle_t		h;
    handle_t			dh;
    error_status_t		st2;
    rpc_tower_vector_t		**tvec;
    dce_server_register_data_t	*rd;
    srvrexec_ep_list_t		*eps;
    sec_attr_twr_set_t		*set;
    srvrexec_ep_t		*el;

    /* Set up state. */
#if	0
    if ((h = malloc(sizeof *h)) == NULL) {
	*st = rpc_s_no_memory;
	return;
    }
#endif	/* 0 */
    h = &mainhandle;
    *new = h;
    dce_server_inq_uuids(NULL, &h->instance, st);
    if (*st == error_status_ok)
	h->configured = TRUE;
    else if (*st == dced_s_not_started_by_dced)
	h->configured = FALSE;
    else {
	if (h != &mainhandle)
	    free(h);
	return;
    }
    h->flags = flags;
    h->sp = server;
    h->data = data;
    if ((i = h->sp->services.count) != 0) {
	h->uuidvec = malloc(i * sizeof *h->uuidvec);
	for (i = 0; i < h->sp->services.count; i++)
	    h->uuidvec[i] = NULL;
    }

    /* Build server EP list. */
    h->eplist.count = h->sp->services.count;
    h->eplist.list = malloc(h->eplist.count * sizeof *h->eplist.list);
    h->tvecs = malloc(h->sp->services.count * sizeof *h->tvecs);
    for (sp = h->sp->services.list, i = 0;
	    i < h->sp->services.count; i++, sp++) {
	h->eplist.list[i].ifid = sp->ifspec;
	h->eplist.list[i].towers.count = 0;
    }


    /* Build UUID vectors. */
    for (sp = h->sp->services.list, i = 0;
	    i < h->sp->services.count; i++, sp++) {
	/* Build a UUID vector if needed. */
	if (sp->objects.count) {
	    h->uuidvec[i] = uv =
		malloc(sizeof *h->uuidvec[i] +
		    sp->objects.count * sizeof (uuid_t *));
	    uv->count = sp->objects.count;
	    for (j = 0; j < uv->count; j++)
		uv->uuid[j] = &sp->objects.list[j];
	}
    }

    /* Register interfaces. */
    for (sp = h->sp->services.list, i = 0;
	    i < h->sp->services.count; i++, sp++) {
	if ((sp->flags & service_c__registered) != 0)
	    continue;
	if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
	    continue;
	if (rd->num_types == 0) {
	    rpc_server_register_if(rd->ifhandle, NULL, rd->epv, st);
	    if (*st != error_status_ok)
		return;
	}
	else
	    for (j = 0; j < rd->num_types; j++) {
		rpc_server_register_if(rd->ifhandle, &rd->types[j].type,
		    rd->types[j].epv, st);
		if (*st != error_status_ok)
		    return;
	    }
	sp->flags |= service_c__registered;
    }

    /* Register protocol sequences. */
    if ((h->flags & dce_server_c_no_protseqs) == 0) {
	if (!h->protseq_count) {
	    rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, st);
	    if (*st != error_status_ok)
		return;
	}
	else
	    for (i = 0; i < h->protseq_count; i++) {
		rpc_server_use_protseq(h->protseq_list[i],
		    rpc_c_protseq_max_reqs_default, st);
		if (*st != error_status_ok)
		    return;
	    }
    }

    /* Get our created bindings. */
    get_our_bindings(h, &dh, st);
    if (*st != error_status_ok)
	return;
    eps = NULL;

#if	!defined(USE_EP_API)
    if ((h->flags & dce_server_c_no_endpoints) == 0) {
	eps = &h->eplist;

	/* Fill in the eplist for each service. */
	for (el = h->eplist.list, sp = h->sp->services.list, i = 0;
		i < h->sp->services.count; i++, sp++) {
	    if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
		continue;

	    /* Get a tower vector for each binding. */
	    h->tvecs[i] = tvec = malloc(h->bv->count * sizeof *tvec);
	    for (j = 0, k = 0; j < h->bv->count; j++) {
		rpc_tower_vector_from_binding(rd->ifhandle, h->bv->binding_h[j],
		    &tvec[j], st);
		if (*st != error_status_ok)
		    return;
		k += tvec[j]->count;
	    }

	    /* Linearize the list of vectors. */
	    set = malloc(sizeof *set + k * sizeof *set->towers);
	    set->count = k;
	    for (j = 0, k = 0; j < h->bv->count; j++)
		for (l = 0; l < tvec[j]->count; l++)
		    set->towers[k++] = tvec[j]->tower[l];
	    el->towers.count = 1;
	    el->towers.list = malloc(sizeof *el->towers.list);
	    el->towers.list[0] = set;
	}

	if (!h->configured)
	    /* Attach the towers to the service.  Assumes they're in the right
	     * order, which we did above. */
	    for (sp = h->sp->services.list, i = 0;
		    i < h->sp->services.count; i++, sp++)
		sp->towers = h->eplist.list[i].towers;
	    h->sp->exec_data.tagged_union.running_data.posix_pid = getpid();

    }
#endif	/* defined(USE_EP_API) */

    /* Register with endpoint mapper. */
    if (h->configured)
	(*srvrexec_v1_0_c_epv.srvrexec_started)(dh, NULL, eps, &h->instance, st);
    else
	(*srvrexec_v1_0_c_epv.srvrexec_create)(dh, h->sp, &h->instance, st);

#if	defined(USE_EP_API)
    if ((h->flags & dce_server_c_no_endpoints) == 0) {
	for (sp = h->sp->services.list, i = 0;
		i < h->sp->services.count; i++, sp++) {
	    if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
		continue;
	    rpc_ep_register(rd->ifhandle, h->bv, h->uuidvec[i], sp->annotation, st);
	    if (*st != error_status_ok)
		return;
	}
    }
#endif	/* defined(USE_EP_API) */

    rpc_binding_free(&dh, &st2);

    /* Export to namespace. */
    if ((h->flags & dce_server_c_ns_export) != 0) {
	for (sp = h->sp->services.list, i = 0;
		i < h->sp->services.count; i++, sp++) {
	    if (sp->flags & dce_service_c_no_ns_export)
		continue;
	    if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
		continue;
	    if ((p = sp->entryname) == NULL
	     && (p = h->sp->entryname) == NULL)
		continue;

	    rpc_ns_binding_export(rpc_c_ns_syntax_dce, p,
		rd->ifhandle, h->bv, h->uuidvec[i], st);
	    if (*st != error_status_ok)
		return;
	}
    }

    *st = error_status_ok;
}


void
dce_server_unregister(
    dce_server_handle_t		*hp,
    error_status_t		*st
)
{
    unsigned32			i;
    unsigned32			j;
    service_t			*sp;
    idl_char			*p;
    dce_server_handle_t		h;
    handle_t			dh;
    error_status_t		st2;
    dce_server_register_data_t	*rd;
    srvrexec_ep_t		*el;

    h = *hp;

    /* Remove from namespace. */
    if ((h->flags & dce_server_c_ns_export) != 0) {
	for (sp = h->sp->services.list, i = 0;
		i < h->sp->services.count; i++, sp++) {
	    if (sp->flags & dce_service_c_no_ns_export)
		continue;
	    if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
		continue;
	    if ((p = sp->entryname) == NULL
	      && (p = h->sp->entryname) == NULL)
		continue;

	    rpc_ns_binding_unexport(rpc_c_ns_syntax_dce, p, rd->ifhandle,
		h->uuidvec[i], st);
	    if (*st != error_status_ok)
		return;
	}
    }

    /* Say that we're going away. */
    rpc_binding_to_epmap(h->bv->binding_h[h->where_bound], &dh, st);
    if (*st != error_status_ok)
	return;
    (*srvrexec_v1_0_c_epv.srvrexec_exited)(dh, &h->instance, st);
    rpc_binding_free(&dh, &st2);

#if	!defined(USE_EP_API)
    /* Free eplist structures. */
    for (el = h->eplist.list, sp = h->sp->services.list, i = 0;
	    i < h->sp->services.count; i++, sp++) {
	if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
	    continue;
	free(el->towers.list[0]);
	free(el->towers.list);
	for (j = 0; j < h->bv->count; j++)
	    rpc_tower_vector_free(&h->tvecs[i][j], st);
	free(h->tvecs[i]);
    }
    free(h->tvecs);
#else
    /* Remove old-style endpoints. */
    if ((h->flags & dce_server_c_no_endpoints) == 0) {
	for (sp = h->sp->services.list, i = 0;
		i < h->sp->services.count; i++, sp++) {
	    if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
		continue;
	    rpc_ep_unregister(rd->ifhandle, h->bv, h->uuidvec[i], st);
	    if (*st != error_status_ok)
		return;
	}
    }
#endif	/* !defined(USE_EP_API) */

    /* Unregister interfaces. */
    for (sp = h->sp->services.list, i = 0;
	    i < h->sp->services.count; i++, sp++) {
	if ((sp->flags & service_c__registered) == 0)
	    continue;
	if ((rd = ifid2regdata(h, &sp->ifspec)) == NULL)
	    continue;
	if (rd->num_types == 0)
	    rpc_server_unregister_if(rd->ifhandle, NULL, st);
	else
	    for (j = 0; j < rd->num_types; j++)
		rpc_server_unregister_if(rd->ifhandle, &rd->types[i].type, st);
	sp->flags &= ~service_c__registered;
    }

    /* Free UUID vector list, and any vectors in it, if they exist. */
    if (h->sp->services.count != 0) {
	for (i = 0; i < h->sp->services.count; i++)
	    if (h->uuidvec[i])
		free(h->uuidvec[i]);
	free(h->uuidvec);
    }

    /* Free other allocated fields. */
    if (h->protseq_count) {
	for (i = 0; i < h->protseq_count; i++)
	    free(h->protseq_list[i]);
	free(h->protseq_list);
    }
    rpc_binding_vector_free(&h->bv, st);
    if (h != &mainhandle)
	free(h);

    *hp = NULL;
    *st = error_status_ok;
}


void
dce_server_disable_service(
    dce_server_handle_t	h,
    rpc_if_handle_t	ifhandle,
    error_status_t	*st
)
{
    service_t		*sp;
    handle_t		dh;
    error_status_t	st2;

    /* Find the right service. */
    if ((sp = ifhandle2service(h, ifhandle, st)) == NULL)
	return;

    if (sp->flags & service_c__registered) {
	rpc_server_unregister_if(ifhandle, NULL, st);
	if (*st != error_status_ok)
	    return;
	sp->flags &= ~service_c__registered;
    }

    if ((sp->flags & service_c_disabled) == 0) {
	rpc_binding_to_epmap(h->bv->binding_h[h->where_bound], &dh, st);
	if (*st != error_status_ok)
	    return;
	(*srvrexec_v1_0_c_epv.srvrexec_disable_service)(dh,
	    &h->instance, &sp->ifspec, st);
	if (*st == error_status_ok)
	    sp->flags |= service_c_disabled;
	rpc_binding_free(&dh, &st2);
    }
}

void
dce_server_enable_service(
    dce_server_handle_t	h,
    rpc_if_handle_t	ifhandle,
    error_status_t	*st
)
{
    unsigned32		i;
    service_t		*sp;
    handle_t		dh;
    error_status_t	st2;

    /* Find the right service. */
    if ((sp = ifhandle2service(h, ifhandle, st)) == NULL)
	return;

    i = sp - h->sp->services.list;
    if ((sp->flags & service_c__registered) == 0) {
	rpc_server_register_if(ifhandle, NULL, h->data[i].epv, st);
	if (*st != error_status_ok)
	    return;
	sp->flags |= service_c__registered;
    }

    if ((sp->flags & service_c_disabled) != 0) {
	rpc_binding_to_epmap(h->bv->binding_h[h->where_bound], &dh, st);
	if (*st != error_status_ok)
	    return;
	(*srvrexec_v1_0_c_epv.srvrexec_enable_service)(dh,
	    &h->instance, &sp->ifspec, st);
	if (*st == error_status_ok)
	    sp->flags &= ~service_c_disabled;
	rpc_binding_free(&dh, &st2);
    }
}



