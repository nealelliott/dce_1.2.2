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
 * $Log: epops.c,v $
 * Revision 1.1.8.3  1996/02/18  19:16:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:28  marty]
 *
 * Revision 1.1.8.2  1995/12/08  20:53:47  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:28 UTC  jrr
 * 	Remove ep_scavenger work.
 * 
 * 	HP revision /main/HPDCE02/12  1995/10/20  21:58 UTC  lmm
 * 	merge ep_scavenger frequency increase fix
 * 
 * 	HP revision /main/HPDCE02/lmm_disable_atexit/1  1995/10/20  21:47 UTC  lmm
 * 	temporarily increase the frequency of
 * 	ep_scavenger for RPC exit handler fix
 * 
 * 	HP revision /main/HPDCE02/11  1995/10/10  16:57 UTC  sommerfeld
 * 	Merge out from HPDCE02..
 * 	[1995/10/05  20:07 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 * 
 * 	move compute_addr_weight back where it belongs.
 * 	[1995/09/08  22:36 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/10  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/9  1995/07/20  22:12 UTC  mk
 * 	Merge dced-as-rpcd changes from mk_dced_as_rpcd/1.
 * 
 * 	HP revision /main/HPDCE02/mk_dced_as_rpcd/1  1995/07/11  15:20 UTC  mk
 * 	Allow dced to run as rpcd and export only ep services.
 * 
 * 	HP revision /main/HPDCE02/8  1995/06/08  21:45 UTC  jss
 * 	Merge changes for CHFts14511, CHFts15038, CHFts14283.
 * 
 * 	HP revision /main/HPDCE02/jss_mothra_fix3/1  1995/06/06  14:20 UTC  jss
 * 	Fix for OT12837. Fix lock usage problems.
 * 
 * 	HP revision /main/HPDCE02/7  1995/05/31  13:25 UTC  tatsu_s
 * 	Submitted the fix for OT12494/CHFts13959 .
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/1  1995/05/18  17:29 UTC  tatsu_s
 * 	Return fwd_reject for unregistered rdacl & xattrschema EPs.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/09  21:17 UTC  tatsu_s
 * 	Submitted the fix for CHFts14942 and CHFts14945.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/1  1995/05/05  20:26 UTC  tatsu_s
 * 	Reduced the frequency of EP scavenger.
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/04  17:26 UTC  mdf
 * 	Merge for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/01  19:09 UTC  mk
 * 	Merge in fixes from mk_mothra_dced/1.
 * 
 * 	HP revision /main/HPDCE02/mk_mothra_dced/1  1995/02/01  18:52 UTC  mk
 * 	In dced_ept_fwd_map(), do not call llb_fwd() if old_ept_fwd_map()
 * 	has found an epdb entry with the local flag set.
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/25  20:12 UTC  tatsu_s
 * 	Submitted EP scavenger.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.local_rpc.b0/2  1995/01/25  18:04 UTC  tatsu_s
 * 	Increased the frequency of EP scavenger.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.local_rpc.b0/1  1995/01/24  22:43 UTC  tatsu_s
 * 	Added EP scavenger.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/22  19:06 UTC  tatsu_s
 * 	Submitted the icmp monitor.
 * 
 * 	HP revision /main/tatsu_s_icmp_monitor/3  1994/12/21  20:31 UTC  tatsu_s
 * 	Added dced_is_remote_handle().
 * 
 * 	HP revision /main/tatsu_s_icmp_monitor/2  1994/12/16  16:11 UTC  tatsu_s
 * 	Added sleep in the icmp monitor thread.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/14  22:30 UTC  tatsu_s
 * 	Submitted xattrschema fix.
 * 
 * 	HP revision /main/tatsu_s_icmp_monitor/1  1994/12/14  22:27 UTC  tatsu_s
 * 	Fixed is_for_me() for xattrschema.
 * 	[1995/12/08  20:49:09  root]
 * 
 * Revision 1.1.4.2  1994/10/27  18:44:56  rsalz
 * 	Don't clobber status code in new map functions (OT CR 12736).
 * 	[1994/10/27  18:33:58  rsalz]
 * 
 * Revision 1.1.4.1  1994/10/26  19:54:37  rsalz
 * 	Use srvrexec endpoints (OT CR 12061).
 * 	[1994/10/26  19:43:07  rsalz]
 * 
 * Revision 1.1.2.19  1994/09/28  20:04:58  rsalz
 * 	Don't ping outselves; redo way local flag is set (OT CR 12384).
 * 	[1994/09/28  19:40:35  rsalz]
 * 
 * Revision 1.1.2.18  1994/09/17  08:05:10  rsalz
 * 	Store dced object UUID in a file (OT CR 12127).
 * 	Remote ep-compat fake server (OT CR 12140).
 * 	[1994/09/17  07:56:41  rsalz]
 * 
 * Revision 1.1.2.17  1994/09/12  15:58:39  baum
 * 	Fixed problems caused by adding obj uuidd to dced bindings
 * 	[1994/09/12  15:42:59  baum]
 * 
 * 	Fixed dced binding lookup logic to handle Iobj UUIDS
 * 
 * Revision 1.1.2.16  1994/09/06  21:30:45  baum
 * 	Only put ep_object on SVC interface EP's (OT 12048)
 * 	[1994/09/06  21:30:31  baum]
 * 
 * Revision 1.1.2.15  1994/09/02  19:52:09  bowe
 * 	Merged with changes from 1.1.2.14
 * 	[1994/09/02  19:51:58  bowe]
 * 
 * 	Initialize entry obj to ep_object for local ep registration. [CR 11979]
 * 	[1994/09/02  19:45:41  bowe]
 * 
 * Revision 1.1.2.14  1994/09/02  15:24:48  baum
 * 	Always export dced obj bindings to NS (OT 11963)
 * 	[1994/09/02  15:24:13  baum]
 * 
 * Revision 1.1.2.13  1994/08/25  16:32:15  rsalz
 * 	Some cleanups; purge #if 0'd server context.
 * 	[1994/08/25  16:29:56  rsalz]
 * 
 * Revision 1.1.2.12  1994/08/12  17:36:55  bowe
 * 	Fix other botched indirection in is_for_me().  [CR 11567]
 * 	[1994/08/12  17:36:38  bowe]
 * 
 * Revision 1.1.2.11  1994/08/11  17:53:39  bowe
 * 	Fix botched indirection in is_for_me().  [CR 11567]
 * 	Set local flag (call old_ept_make_local()) in local_ep_register().
 * 	[1994/08/11  17:53:16  bowe]
 * 
 * Revision 1.1.2.10  1994/08/03  20:36:16  rsalz
 * 	Fix OT CR 11506.
 * 	[1994/08/03  20:32:04  rsalz]
 * 
 * Revision 1.1.2.9  1994/07/29  19:56:06  rsalz
 * 	Merged with changes from 1.1.2.8
 * 	[1994/07/29  19:55:35  rsalz]
 * 
 * 	Add local_ep_register for CN binding to dced (OT CR 11211).
 * 	Optimize lookup for dced's services (OT CR 10888).
 * 	[1994/07/29  19:36:54  rsalz]
 * 
 * Revision 1.1.2.8  1994/07/15  17:22:19  rsalz
 * 	Update LLB support; remove sources from src/rpc (OT CR 7435).
 * 	[1994/07/15  17:20:20  rsalz]
 * 
 * Revision 1.1.2.7  1994/06/17  19:37:57  rsalz
 * 	Clean up local address init; more work like 1.1.2.6.
 * 	[1994/06/17  19:30:24  rsalz]
 * 
 * Revision 1.1.2.6  1994/06/16  19:35:08  rsalz
 * 	Fix final(?) config bug -- don't clobber status in dced_ept_map.
 * 	[1994/06/16  19:31:46  rsalz]
 * 
 * Revision 1.1.2.5  1994/06/09  16:03:05  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:43  devsrc]
 * 
 * Revision 1.1.2.4  1994/05/31  20:23:59  rsalz
 * 	Add -e flag; fix some EP bugs
 * 	[1994/05/31  20:22:47  rsalz]
 * 
 * Revision 1.1.2.3  1994/05/27  16:31:12  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:20  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/26  19:42:10  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:40  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:32:56  rsalz]
 * 
 * Revision 1.1.2.1  1994/04/14  20:08:31  rsalz
 * 	Make server and API all compile.
 * 	[1994/04/14  19:45:09  rsalz]
 * 
 * $EndLog$
 */
#include <dcedimpl.h>
#include <dced_convert.h>
#include <scimpl.h>
#include <dce/rdaclifv0.h>
#include <dce/ep.h>
#include <dce/mgmt.h>
#define PTHREAD_EXC
#include <commonp.h>
#include <com.h>
#include <comp.h>
#include <comtwr.h>
#include <comtwrref.h>
#include <comfwd.h>
#include <oe.h>
#if	defined(WANT_LLB)
#include <lb.h>
#endif	/* defined(WANT_LLB) */
#include <sys/wait.h>

extern	pthread_cond_t	icmp_cond;
extern	pthread_mutex_t	icmp_mutex;
extern	boolean32	icmp_active;


/*
**  Following line, if enabled, causes dce_server_register to use
**  rpc_ep_register calls.
#define USE_EP_API
*/


/*
**  See if binding handle is for a remote client.  In case of any error,
**  we assume the client is remote.
*/
static boolean32
is_remote_handle(
    handle_t		h,
    error_status_t	*st
)
{
    boolean32		is_remote = TRUE;
    error_status_t	st1;
    handle_t		client;

    /* Get client handle from the binding handle. */
    rpc_binding_server_from_client(h, &client, st);
    if (*st != error_status_ok)
	return is_remote;

    is_remote = dced_is_remote_handle(client, st);
    rpc_binding_free(&client, &st1);

    return is_remote;
}

boolean32
dced_is_remote_handle(
    handle_t		h,
    error_status_t	*st
)
{
    error_status_t	st1;
    unsigned char	*stb;
    unsigned char	*client_address;
    int			i;

    rpc_binding_to_string_binding(h, &stb, st);
    if (*st != error_status_ok)
	return TRUE;
    rpc_string_binding_parse(stb, NULL, NULL, &client_address, NULL, NULL, st);
    rpc_string_free(&stb, &st1);
    if (*st != error_status_ok)
	return TRUE;

    /* Compare the addresses with the client address. */
    for (i = 0; i < my_address_count; i++)
	if (strcmp((char *)client_address, (char *)my_addresses[i]) == 0)
	    break;
    rpc_string_free(&client_address, &st1);

    /* If we hit the end of the address list, he's foreign. */
    *st = rpc_s_ok;
    return i == my_address_count ? TRUE : FALSE;
}


static boolean32
dced_ept_valid_handle(
    handle_t			h,
    boolean32			local_only,
    error_status_t		*st
)
{
    uuid_t			object;
    boolean32			v;

    if (h == NULL)
	/* "Internal" handle; must be okay. */
	return TRUE;

    /* If there's an object in the binding handle, make sure it's the
     * right one. */
    rpc_binding_inq_object(h, &object, st);
    if (*st != error_status_ok)
	return FALSE;
    v = uuid_is_nil(&object, st);
    if (*st != error_status_ok)
	return FALSE;
    if (!v) {
	v = uuid_equal(&object, &dced_g_object, st);
	if (*st != error_status_ok)
	    return FALSE;
	if (!v) {
	    /* Too bad we did'nt have ept_s_wrong_object, but it's too
	     * late to add a status code now. */
	    *st = ept_s_cant_perform_op;
	    return FALSE;
	}
    }

    if (local_only && is_remote_handle(h, st)) {
	if (*st == error_status_ok)
	    *st = ept_s_cant_perform_op;
	return FALSE;
    }

    *st = error_status_ok;
    return TRUE;
}



void
local_ep_register(
    service_table_t		*sp,
    error_status_t		*st
)
{
    ept_entry_t			entry;
    rpc_if_rep_t		*if_rep;
    rpc_binding_rep_t		*bh_rep;
    error_status_t		st2;
    rpc_tower_ref_vector_t	*tv;
    unsigned32			i;
    unsigned32			j;

    /* Make sure we have at least one binding and that all valid bindings
     * have endpoints. */
    if_rep = (rpc_if_rep_t *)*sp->ifspec;
    for (j = dced_bindings->count, i = 0; i < dced_bindings->count; i++) {
        if (dced_bindings->binding_h[i] != NULL) {
            bh_rep = (rpc_binding_rep_t *)dced_bindings->binding_h[i];
            if (!bh_rep->addr_has_endpoint) {
                *st = rpc_s_invalid_binding;
                return;
            }
            j = i;
        }
    }
    if (j == dced_bindings->count) {
        *st = rpc_s_no_bindings;
        return;
    }

    /* Fill in initial fields. */
    memcpy(entry.annotation, sp->annote, sizeof entry.annotation);
    entry.object = dced_g_object;

    for (i = 0; i < dced_bindings->count; i++) {
        if (dced_bindings->binding_h[i] == NULL)
            continue;

        rpc__tower_ref_vec_from_binding(if_rep, dced_bindings->binding_h[i],
                &tv, st);
        if (*st != rpc_s_ok)
            break;

        for (j = 0; j < tv->count; j++) {
            rpc__tower_from_tower_ref(tv->tower[j], &entry.tower, st);
            if (*st != rpc_s_ok)
                break;
	    old_ept_insert((unsigned32)1, &entry, TRUE, TRUE, st);
            rpc__tower_free(&entry.tower, &st2);
            if (*st != rpc_s_ok)
                break;
        }

        rpc__tower_ref_vec_free(&tv, &st2);
        if (*st != rpc_s_ok)
            break;
    }
}

static boolean32
is_for_me(
    uuid_t		*interface
)
{
    error_status_t	st;
    rpc_if_rep_t	*if_rep;
    int			i;
    service_table_t	*sp;

    /* Check for Endpoint Interfaces */
    for (i = sizeof_ept_services_table, sp = ept_services; --i >= 0; sp++) {
	if_rep = (rpc_if_rep_t *)(*(sp->ifspec));
        if (uuid_equal(interface, &if_rep->id, &st))
            return TRUE;
    }

  if (!do_ep_only) {
    /* Check for all our other interfaces */
    for (i = sizeof_services_table, sp = services; --i >= 0; sp++) {
	if (!sp->registered
	 || sp->ifspec == &rs_attr_schema_v0_0_s_ifspec
	 || sp->ifspec == &rdaclif_v1_0_s_ifspec
	 || sp->ifspec == &rdaclif_v0_0_s_ifspec)
	  continue;
	if_rep = (rpc_if_rep_t *)(*(sp->ifspec));
	if (uuid_equal(interface, &if_rep->id, &st))
	    return TRUE;
    }
  }

    return FALSE;
}

static boolean32
is_for_my_object(
    uuid_t		*object,
    uuid_t		*interface
)
{
    error_status_t	st;

    /* Just like mgmt calls, these ifspecs are supported by
     * many servers. To avoid erroneous in-line forwarding,
     * we only accept our object UUID.
     * Note: The caller must use the dced object UUID to talk
     * to dced! */
    if (uuid_equal(interface,
		   &((rpc_if_rep_p_t)rs_attr_schema_v0_0_s_ifspec)->id,
		   &st)
	|| uuid_equal(interface,
		      &((rpc_if_rep_p_t)rdaclif_v1_0_s_ifspec)->id,
		      &st)
	|| uuid_equal(interface,
		      &((rpc_if_rep_p_t)rdaclif_v0_0_s_ifspec)->id,
		      &st)) {
	if (uuid_equal(object, &dced_g_object, &st))
	    return TRUE;
	else
	    return FALSE;
    }
    return TRUE;
}


static void
new_mgmt_map(
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_addr_p_t	addr,
    uuid_t		*actuuid,
    unsigned32		*num_ents,
    rpc_addr_p_t	*fwd_addr,
    error_status_t	*st
)
{
    unsigned16		h;
    srvr_hash_bucket_t	*bp;
    unsigned32		i;
    unsigned32		j;
    unsigned32		k;
    server_data_t	*sp;
    service_t		*svcp;
    srvr_hash_entry_t	*he;
    error_status_t	st2;
    twr_fields_t	tfp;
    sec_attr_twr_set_t	*tsp;

    h = uuid_hash(object, &st2);
    h = srvr_hash2bucket(h);
    bp = &sc_hash_table.list[h];

    for (he = bp->list, i = 0; i < bp->count; i++, he++) {
	sp = &sc_table.list[he->entry];
	if (!sp->used)
	    continue;
	if (he->object == 0)
	    /* No objects registered. */
	    continue;
	svcp = &sp->data.services.list[he->service];
	if (svcp->flags & service_c_disabled || svcp->towers.count == 0)
	    /* Disabled or no towers registered */
	    continue;

	if (ifid
	 && !(uuid_equal(&svcp->ifspec.uuid, &ifid->uuid, &st2)
	   && svcp->ifspec.vers_major == ifid->vers_major
	   && (unsigned)svcp->ifspec.vers_minor > (unsigned)ifid->vers_minor))
	    /* Not the interface we want. */
	    continue;

	if (!uuid_equal(&svcp->objects.list[he->object - 1], object, &st2))
	    /* Not the object we want. */
	    continue;

	/* Find compat tower in set. */
	for (j = 0; j < svcp->towers.count; j++) {
	    tsp = svcp->towers.list[j];
	    for (k = 0; k < tsp->count; k++) {
		tower_to_fields(tsp->towers[k], &tfp, &st2);
		if (st2 != error_status_ok)
		    continue;
		if (rpc_protocol == tfp.rpc_protocol
		 && rpc_protocol_vers_major == tfp.rpc_protocol_vers_major
		 && compatible_syntax_ids(drep, &tfp.drep)) {
		    rpc__naf_tower_flrs_to_addr(tsp->towers[i]->tower_octet_string, fwd_addr, st);
		    if (*st == error_status_ok) {
			*num_ents = 1;
			return;
		    }
		}
	    }
	}
    }

    *st = ept_s_not_registered;
}

static void
new_map(
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_addr_p_t	addr,
    uuid_t		*actuuid,
    unsigned32		*num_ents,
    rpc_addr_p_t	*fwd_addr,
    error_status_t	*st
)
{
    unsigned16		h;
    srvr_hash_bucket_t	*bp;
    unsigned32		i;
    unsigned32		j;
    unsigned32		k;
    server_data_t	*sp;
    service_t		*svcp;
    srvr_hash_entry_t	*he;
    error_status_t	st2;
    sec_attr_twr_set_t	*tsp;
    twr_fields_t	tfp;

    if (!uuid_is_nil(object, &st2)) {
	new_mgmt_map(object, ifid, drep, rpc_protocol,
	    rpc_protocol_vers_major, rpc_protocol_vers_minor, addr,
	    actuuid, num_ents, fwd_addr, st);
	return;
    }

    /* Get the interface and object hash buckets. */
    h = uuid_hash(&ifid->uuid, &st2);
    h = srvr_hash2bucket(h);
    bp = &sc_hash_table.list[h];

    for (he = bp->list, i = 0; i < bp->count; i++, he++) {
	sp = &sc_table.list[he->entry];
	if (!sp->used)
	    continue;
	svcp = &sp->data.services.list[he->service];
	if (svcp->flags & service_c_disabled || svcp->towers.count == 0)
	    /* Disabled or no towers registered */
	    continue;

	/* Find compat tower in set. */
	for (j = 0; j < svcp->towers.count; j++) {
	    tsp = svcp->towers.list[j];
	    for (k = 0; k < tsp->count; k++) {
		tower_to_fields(tsp->towers[k], &tfp, &st2);
		if (st2 != error_status_ok)
		    continue;
		if (rpc_protocol == tfp.rpc_protocol
		 && rpc_protocol_vers_major == tfp.rpc_protocol_vers_major
		 && compatible_syntax_ids(drep, &tfp.drep)) {
		    rpc__naf_tower_flrs_to_addr(tsp->towers[i]->tower_octet_string, fwd_addr, st);
		    if (*st == error_status_ok) {
			*num_ents = 1;
			return;
		    }
		}
	    }
	}
    }

    *st = ept_s_not_registered;
}



/*
**  DG forwarding.
**  Do not forward to an entry with a nil interface id AND a nil object
**  id.  Allowing such registrations will result in an inexact lookup match
**  (due to the nil interface id) for operations that are really for
**  the server implementing the endpoint map operations.  If this ever
**  becomes desireable, it will likely require visiting (fixing) the
**  protocol service implementations that use DG style forwarding.  The
**  implementations may try to forward first through the map and only
**  if the forward map fails, then attempt to perform normal pkt
**  handling.
*/
static void
dced_ept_fwd_map(
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_addr_p_t	addr,
    uuid_t		*actuuid,
    rpc_addr_p_t	*fwd_addr,
    rpc_fwd_action_t	*fwd_action,
    error_status_t	*st
)
{
    rpc_if_rep_t	*mgmt_if_rep;
    error_status_t	st2;
    unsigned32		num_ents;
    boolean32		is_mgmt_if;
    boolean32		is_nil_obj;

    /* Various pessimisms. */
    dce_assert(dhd_svc_handle, fwd_addr != NULL);
    mgmt_if_rep = (rpc_if_rep_t *)mgmt_v1_0_s_ifspec;
    is_mgmt_if = uuid_equal(&ifid->uuid, &mgmt_if_rep->id, &st2);
    is_nil_obj = uuid_is_nil(object, &st2);
    if (is_mgmt_if && is_nil_obj) {
	*st = ept_s_invalid_entry;
	return;
    }
    *fwd_action = rpc_e_fwd_drop;
    *st = error_status_ok;

    if (is_for_me(&ifid->uuid))
        return;

    /* RPC runtime wants NIL object UUID for forwarder mgmt calls 
     * (see rpc/runtime/dglsn.c). */
    if (is_mgmt_if && uuid_equal(object, &dced_g_object, &st2)) {
      *object = uuid_g_nil_uuid;
      return;
    }

    num_ents = 0;
    old_ept_fwd_map(object, ifid, is_mgmt_if, drep, rpc_protocol,
	rpc_protocol_vers_major, rpc_protocol_vers_minor, addr, actuuid,
	&num_ents, fwd_addr, st);

    /*
     * If old_ept_fwd_map() finds an epdb entry with the local flag set,
     * it returns a status of ept_s_not_registered and an entry count of 1,
     * and we should look no further.
     */
    if (num_ents == 0
     && !do_ep_only
     && (*st == ept_s_not_registered || *st == error_status_ok)) {
	dce_lock_read(sc_hash_table.lock, &st2);
	if (is_mgmt_if)
	    new_mgmt_map(object, NULL, drep, rpc_protocol,
		rpc_protocol_vers_major, rpc_protocol_vers_minor,
		addr, actuuid, &num_ents, fwd_addr, st);
	else
	    new_map(object, ifid, drep, rpc_protocol,
		rpc_protocol_vers_major, rpc_protocol_vers_minor,
		addr, actuuid, &num_ents, fwd_addr, st);
	dce_lock_unlock(sc_hash_table.lock, &st2);
    }

#if	defined(WANT_LLB)
    if (num_ents == 0
     && (*st == ept_s_not_registered || *st == error_status_ok))
	llb_fwd(object, &ifid->uuid, addr, &num_ents, fwd_addr, st);
#endif	/* defined(WANT_LLB) */

    dce_assert(dhd_svc_handle, num_ents < 2);
    if (*st == ept_s_not_registered) {
	*st = error_status_ok;
	if (num_ents == 1) {
	    /* It must be a "local" entry, do "in-line" forwarding. */
	    return;
	}
	if (is_for_my_object(object, &ifid->uuid)) {
	    /* do "in-line" forwarding. */
	    return;
	}
	*fwd_action = rpc_e_fwd_reject;
	return;
    }
    if (*st != error_status_ok)
	return;

    if (num_ents == 1) {
	*fwd_action = rpc_e_fwd_forward;
	if (icmp_active == false)
	    pthread_cond_signal(&icmp_cond);
    }
}


static void
dced_ept_insert(
  /* in */
    handle_t			h,
    unsigned32			num_ents,
    ept_entry_t			entries[],
    boolean32			replace,
  /* out */
    error_status_t		*st
)
{
    error_status_t		st2;

    /* Check binding handle. */
    if (!dced_ept_valid_handle(h, TRUE, st))
	return;

    dce_lock_justwrite(sx_table.lock, &st2);
    old_ept_insert(num_ents, entries, replace, FALSE, st);
    dce_lock_unlock(sx_table.lock, &st2);
}


static void
dced_ept_delete(
  /* in */
    handle_t			h,
    unsigned32			num_ents,
    ept_entry_t			entries[],
  /* out */
    error_status_t		*st
)
{
    error_status_t		st2;

    /* Check binding handle. */
    if (!dced_ept_valid_handle(h, TRUE, st))
	return;

    dce_lock_justwrite(sx_table.lock, &st2);
    old_ept_delete(num_ents, entries, st);
    dce_lock_unlock(sx_table.lock, &st2);
}


static void
dced_ept_lookup(
  /* in */
    handle_t			h,
    unsigned32			inquiry_type,
    uuid_t			*object,
    rpc_if_id_t			*ifid,
    unsigned32			vers_option,
  /* in,out */
    ept_lookup_handle_t		*lookup_handle,
  /* in */
    unsigned32			max_ents,
  /* out */
    unsigned32			*num_ents,
    ept_entry_t			entries[],
    error_status_t		*st
)
{
    /* Set default output parameters. */
    *num_ents = 0;

    /* Check binding handle and input parameters. */
    if (!dced_ept_valid_handle(h, FALSE, st))
	return;
    old_ept_lookup(inquiry_type, object, ifid, vers_option,
	lookup_handle, max_ents, num_ents, entries, st);
}


/*
**  (CN) endpoint resolution.
*/
static void
dced_ept_map(
  /* in */
    handle_t			h,
    uuid_t			*object,
    twr_t			*map_tower,
  /* in,out */
    ept_lookup_handle_t		*lookup_handle,
  /* in */
    unsigned32			max_towers,
  /* out */
    unsigned32			*num_towers,
    twr_t			*towers[],
    error_status_t		*st
)
{
    error_status_t		st2;
    twr_fields_t		fields;
    rpc_if_rep_t		*mgmt_if_rep;

    /* Set default output parameters. */
    *num_towers = 0;

    /* Check binding handle. */
    if (!dced_ept_valid_handle(h, FALSE, st))
	return;

    dce_lock_read(sx_table.lock, &st2);
    old_ept_map(object, map_tower, lookup_handle, max_towers, num_towers,
	towers, st);

    /* This of this as "found_nothing || room_for_more" */
    if (*st == ept_s_not_registered || *st == ept_s_invalid_context
     || (*st == rpc_s_ok
      && (*num_towers < max_towers
       || (lookup_handle != NULL && *lookup_handle == NULL)))) {

	tower_to_fields(map_tower, &fields, &st2);
	mgmt_if_rep = (rpc_if_rep_t *)mgmt_v1_0_s_ifspec;
	if (uuid_equal(&fields.ifid.uuid, &mgmt_if_rep->id, &st2))
	    /* Map MGMT interface */
	    ;
	else
	    /* Standard map. */
	    ;

	/* XXX copy the tower. */
    }

#if	defined(WANT_LLB)
    if (*st == ept_s_not_registered || *st == ept_s_invalid_context
     || (*st == rpc_s_ok
      && (*num_towers < max_towers
       || (lookup_handle != NULL && *lookup_handle == NULL)))) {
	llb_map(object, map_tower, lookup_handle, max_towers, num_towers,
	    towers, st);
    }
#endif	/* defined(WANT_LLB) */
    dce_lock_unlock(sx_table.lock, &st2);
}


static void
dced_ept_lookup_handle_free(
  /* in */
    handle_t			h,
  /* in,out */
    ept_lookup_handle_t		*lookup_handle,
  /* out */
    error_status_t		*st
)
{
    old_ept_lookup_handle_free(lookup_handle, st);
}

static void
dced_ept_inq_object(
  /* in */
    handle_t			h,
  /* out */
    uuid_t			*object,
    error_status_t		*st
)
{
    /* Set default output parameters. */
    *object = uuid_g_nil_uuid;

    /* Check binding handle. */
    if (!dced_ept_valid_handle(h, FALSE, st))
	return;

    *object = dced_g_object;
    *st = error_status_ok;
}


static void
dced_ept_mgmt_delete(
  /* in */
    handle_t			h,
    boolean32			object_speced,
    uuid_t			*object,
    twr_t			*tower,
  /* out */
    error_status_t		*st
)
{
    error_status_t		st2;

    /* Check binding handle. */
    if (!dced_ept_valid_handle(h, TRUE, st))
	return;

    dce_lock_justwrite(sx_table.lock, &st2);
    old_ept_mgmt_delete(object_speced, object, tower, st);
    dce_lock_unlock(sx_table.lock, &st2);
}


ept_v3_0_epv_t	ept_v3_0_mgr_epv = {
    dced_ept_insert,
    dced_ept_delete,
    dced_ept_lookup,
    dced_ept_map,
    dced_ept_lookup_handle_free,
    dced_ept_inq_object,
    dced_ept_mgmt_delete
};


void
ept_lookup_handle_t_rundown(
    ept_lookup_handle_t		lookup_handle
)
{
    old_ept_lookup_handle_t_rundown(lookup_handle);
}

void
dced_ept_init(
    error_status_t	*st
)
{
    int			i;
    unsigned char	*stb;
    error_status_t	st1;

    rpc__server_register_fwd_map(dced_ept_fwd_map, st);

    old_ept_init(&dced_g_object, st);
#if	defined(WANT_LLB)
    llb_init(st);
#endif	/* defined(WANT_LLB) */
}
