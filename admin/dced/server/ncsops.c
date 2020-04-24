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
 * $Log: ncsops.c,v $
 * Revision 1.1.4.2  1996/02/18  19:16:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:53:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:49:12  root]
 * 
 * Revision 1.1.2.4  1994/07/15  17:22:22  rsalz
 * 	Update LLB support; remove sources from src/rpc (OT CR 7435).
 * 	[1994/07/15  17:20:32  rsalz]
 * 
 * Revision 1.1.2.3  1994/06/09  16:03:08  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:46  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/26  19:42:22  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:53  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/06  20:00:06  rsalz
 * 	Add files for NCS1.5 mgmt interface support.
 * 	[1994/05/06  19:59:53  rsalz]
 * 
 * $EndLog$
 */

/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  Operations for NCS1.5 management interface.
*/

#include <commonp.h>
#include <mgmtp.h>
#include <com.h>
#include <dce/rrpc.h>
#include <lb.h>


/*
**  Manager routine for rrpc_are_you_there.
*/
static void
are_you_there(
    handle_t		h,
    error_status_t	*st
)
{
    error_status_t	st2;

    if (!rpc__mgmt_authorization_check(h, rpc_c_mgmt_is_server_listen,
	    true, &st2)) {
	/* Sort of pointless, since we're answering anyway */
	*st = rrpc_op_not_allowed;
	return;
    }
    *st = 0;
}


/*
**  Manager routine for rrpc_inq_stats.
*/
static void 
inq_stats(
    handle_t		h,
    unsigned32		max_stats,
    rrpc_stat_vec_t	stats,
    signed32		*l_stat,
    error_status_t	*st
)
{
    error_status_t	st2;
    rpc_stats_vector_t	*nstats;

    if (!rpc__mgmt_authorization_check(h, rpc_c_mgmt_inq_stats, true, &st2)) {
	*st = rrpc_op_not_allowed;
	*l_stat = -1;
	return;
    }
    rpc_mgmt_inq_stats(NULL, &nstats, &st2);
    if (st2 != rpc_s_ok) {
	*st = rrpc_cant_get_stats;
	*l_stat = -1;
	return;
    }

    if (rrpc_sv_calls_in <= max_stats)
	stats[rrpc_sv_calls_in] = nstats->stats[rpc_c_stats_calls_in];
    if (rrpc_sv_calls_out <= max_stats)
	stats[rrpc_sv_calls_out] = nstats->stats[rpc_c_stats_calls_out];
    if (rrpc_sv_rcvd <= max_stats)
	stats[rrpc_sv_rcvd] = nstats->stats[rpc_c_stats_pkts_in];
    if (rrpc_sv_sent <= max_stats)
	stats[rrpc_sv_sent] = nstats->stats[rpc_c_stats_pkts_out];
    if (rrpc_sv_frag_resends <= max_stats)
	stats[rrpc_sv_frag_resends] = 0;
    if (rrpc_sv_dup_frags_rcvd <= max_stats)
	stats[rrpc_sv_dup_frags_rcvd] = 0;
    *l_stat = MIN(rpc_c_stats_pkts_out, max_stats);
    rpc_mgmt_stats_vector_free(&nstats, &st2);
    *st = 0;
}


/*
**  Manager routine for rrpc_inq_interfaces.
*/
static void 
inq_interfaces(
    handle_t			h,
    unsigned32			max_ifs,
    rrpc_interface_vec_t	ifs,
    signed32			*l_if,
    error_status_t		*st
)
{
    error_status_t		st2;
    rpc_if_id_vector_t		*nifs;
    signed32			i;
    int				j;

    if (!rpc__mgmt_authorization_check(h, rpc_c_mgmt_inq_if_ids, true, &st2)) {
	*st = rrpc_op_not_allowed;
	*l_if = -1;
	return;
    }
    rpc_mgmt_inq_if_ids(NULL, &nifs, &st2);
    if (st2 != rpc_s_ok) {
	*st = rrpc_cant_get_ifs;
	*l_if = -1;
	return;
    }

    *l_if = MIN(nifs->count - 1, max_ifs);
    for (i = 0; i <= *l_if; i++) {
	ifs[i].vers = nifs->if_id[i]->vers_minor << 16
		    | nifs->if_id[i]->vers_major;
	ifs[i].id = *(uuid_old_t *)&nifs->if_id[i]->uuid;
	ifs[i].opcnt = 0;
	for (j = 0; j < 32; j++)
	    ifs[i].port[j] = 0;
    }
    rpc_if_id_vector_free(&nifs, &st2);
    *st = 0;
}

/*
**  Register the RRPC manager EPV.
*/

void
ncs_mgmt_if_register_epv(
    error_status_t	*st
)
{
    static rrpc_v0_0_epv_t rrpc_v0_0_mgr_epv = {
	are_you_there, inq_stats, inq_interfaces,
	rpc__mgmt_stop_server_lsn_mgr
    };

    /* Register the remote management interface with the runtime
     * as an internal interface. */
    rpc__server_register_if_int(
	(rpc_if_handle_t)rrpc_v0_0_s_ifspec, NULL,
	(rpc_mgr_epv_t)&rrpc_v0_0_mgr_epv, true, st);
}
