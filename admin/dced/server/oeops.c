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
 * $Log: oeops.c,v $
 * Revision 1.1.12.2  1996/11/21  14:13:09  arvind
 * 	OSF DCE 1.2.2 Drop5
 * 	[1996/11/21  14:10:53  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/20  16:21 UTC  arvind
 * 	Fix from OSF
 * 	[1996/11/20  14:25 UTC  arvind  /main/DCE_1.2.2/arvind_work/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/07/09  19:44 UTC  jrr
 * 	Fix memory leaks (OT13533).
 * 	[1996/07/09  19:39 UTC  jrr  /main/HPDCE02/jrr_perf_2/1]
 * 
 * Revision 1.1.9.3  1996/02/18  19:16:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:39  marty]
 * 
 * Revision 1.1.9.2  1995/12/08  20:54:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/9  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/8  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted the potential workaroud for CHFts15950.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/1  1995/08/23  21:13 UTC  tatsu_s
 * 	Reworked locking, removed epdb_un/lock(h) from ping_server().
 * 
 * 	HP revision /main/HPDCE02/7  1995/05/03  18:30 UTC  tatsu_s
 * 	Submitted the fix for CHFts15138.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.ep_mapper.b1/2  1995/04/28  21:16 UTC  tatsu_s
 * 	Fixed the memory leak.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.ep_mapper.b1/1  1995/04/28  20:21 UTC  tatsu_s
 * 	Moved epdb_un/lock(h) to ping_server() to avoid locking error.
 * 
 * 	HP revision /main/HPDCE02/6  1995/02/27  14:57 UTC  tatsu_s
 * 	Submitted the stale and lost endpoints fix.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.ep_mapper.b0/1  1995/02/17  16:06 UTC  tatsu_s
 * 	Better stale & lost ep detection.
 * 
 * 	HP revision /main/HPDCE02/5  1995/01/20  00:45 UTC  mk
 * 	Merge in changes from mk_mothra_dced/3.
 * 
 * 	HP revision /main/HPDCE02/mk_mothra_dced/3  1995/01/20  00:41 UTC  mk
 * 	Rework ep_ping1() fixes.
 * 
 * 	HP revision /main/HPDCE02/4  1995/01/19  00:47 UTC  mk
 * 	Merge in ep_ping1() fix from mk_mothra_dced.
 * 
 * 	HP revision /main/HPDCE02/mk_mothra_dced/2  1995/01/19  00:39 UTC  mk
 * 	Fix epdb list locking bug in ep_ping1() that crashed dced in system tests.
 * 	Wrap earlier fix in tower_to_string() with HPDCE conditionalization.
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/16  16:53 UTC  mk
 * 	Merge in tower_to_string() fix from mk_mothra_dced.
 * 
 * 	HP revision /main/HPDCE02/mk_mothra_dced/1  1995/01/16  16:51 UTC  mk
 * 	Fix tower_to_string() return value.
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/04  18:13 UTC  tatsu_s
 * 	Submitted dced shutdown fix.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.mothra_cn_fix.b0/1  1995/01/04  18:08 UTC  tatsu_s
 * 	Start ep_ping1 a little earlier.
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/22  19:06 UTC  tatsu_s
 * 	Submitted the icmp monitor.
 * 
 * 	HP revision /main/tatsu_s_icmp_monitor/4  1994/12/21  20:31 UTC  tatsu_s
 * 	Added dced_is_remote_handle().
 * 
 * 	HP revision /main/tatsu_s_icmp_monitor/3  1994/12/16  16:11 UTC  tatsu_s
 * 	Added sleep in the icmp monitor thread.
 * 
 * 	HP revision /main/tatsu_s_icmp_monitor/2  1994/12/09  04:17 UTC  tatsu_s
 * 	Fixed the debug message.
 * 
 * 	HP revision /main/tatsu_s_icmp_monitor/1  1994/12/09  03:09 UTC  tatsu_s
 * 	ICMP: unreachable port monitor.
 * 	[1995/12/08  20:49:16  root]
 * 
 * Revision 1.1.4.1  1994/10/26  19:54:43  rsalz
 * 	Add EPmap convenience abstractions (needed to fix OT CR 12061).
 * 	[1994/10/26  19:43:37  rsalz]
 * 
 * Revision 1.1.2.23  1994/10/04  18:33:20  rsalz
 * 	Use thread_attr, not pthread_attr_default (OT CR 12485).
 * 	[1994/10/04  18:25:25  rsalz]
 * 
 * Revision 1.1.2.22  1994/10/03  19:58:44  rsalz
 * 	Add some SVC calls (OT CR 11725); to debug OT CR 12384.
 * 	[1994/10/03  19:56:56  rsalz]
 * 
 * Revision 1.1.2.21  1994/09/28  20:05:00  rsalz
 * 	Don't ping outselves; redo way local flag is set (OT CR 12384).
 * 	[1994/09/28  19:40:57  rsalz]
 * 
 * Revision 1.1.2.20  1994/09/27  19:57:10  rsalz
 * 	Don't format towers if no debugging (OT CR 11725).
 * 	[1994/09/27  18:54:07  rsalz]
 * 
 * Revision 1.1.2.19  1994/09/26  17:49:22  rsalz
 * 	Convert to true SVC functions (OT CR 11725).
 * 	[1994/09/26  17:12:27  rsalz]
 * 
 * Revision 1.1.2.18  1994/09/17  08:05:18  rsalz
 * 	Merged with changes from 1.1.2.17
 * 	[1994/09/17  08:04:38  rsalz]
 * 
 * 	Rename epdb_sleep to dced_sleep.
 * 	Ignore deleted and local endpoints on restart.
 * 	[1994/09/17  07:57:54  rsalz]
 * 
 * 	Properly test "deleted" endpoints (OT CR 11408).
 * 	[1994/09/12  18:16:09  rsalz]
 * 
 * Revision 1.1.2.17  1994/09/12  15:58:42  baum
 * 	Fixed problems caused by adding obj uuidd to dced bindings
 * 	[1994/09/12  15:43:01  baum]
 * 
 * Revision 1.1.2.16  1994/08/30  21:23:14  baum
 * 	Added indirection to num_ents == 1
 * 	[1994/08/30  21:22:53  baum]
 * 
 * Revision 1.1.2.15  1994/08/30  20:09:05  rsalz
 * 	Ignore "local" EP entries during DG forwarding (OT CR 11954).
 * 	[1994/08/30  20:08:22  rsalz]
 * 
 * Revision 1.1.2.14  1994/08/28  14:40:25  rsalz
 * 	Use dce_gettimeofday not dced_get_the_time (OT CR 11931).
 * 	[1994/08/28  14:35:03  rsalz]
 * 
 * Revision 1.1.2.13  1994/08/25  16:32:29  rsalz
 * 	Some cleanups; purge #if 0'd server context.
 * 	[1994/08/25  16:30:56  rsalz]
 * 
 * Revision 1.1.2.12  1994/08/11  17:53:41  bowe
 * 	Do not delete local EPs. [CR 11567]
 * 	[1994/08/11  17:47:45  bowe]
 * 
 * Revision 1.1.2.11  1994/08/03  20:36:21  rsalz
 * 	Fix OT CR 11506.
 * 	[1994/08/03  20:32:53  rsalz]
 * 
 * Revision 1.1.2.7  1994/06/16  20:08:36  bowe
 * 	Properly set NIL ep entry.
 * 	[1994/06/16  20:08:23  bowe]
 * 
 * Revision 1.1.2.6  1994/06/16  19:35:17  rsalz
 * 	Don't store conformant structures in dce_db (see OT CR 10971).
 * 	[1994/06/16  19:32:19  rsalz]
 * 
 * Revision 1.1.2.5  1994/06/13  20:05:36  rsalz
 * 	Move epdb_get_the_time to dced_get_the_time
 * 	[1994/06/13  17:31:34  rsalz]
 * 
 * Revision 1.1.2.4  1994/06/09  18:15:39  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:20  annie]
 * 
 * Revision 1.1.2.3  1994/05/31  20:24:05  rsalz
 * 	Add -e flag; fix some EP bugs
 * 	[1994/05/31  20:22:55  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/27  16:31:21  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:38  rsalz]
 * 
 * Revision 1.1.2.1  1994/05/26  19:42:26  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:58  rsalz]
 * 
 * $EndLog$
 */
#include <oeimpl.h>
#include <comp.h>
#include <dce/mgmt.h>
#include <comtwr.h>
#include <comtwrflr.h>
#include <comtwrref.h>
#include <comfwd.h>
#include <oe.h>
#include <dced0_convert.h>

/*
**  Implementation of old EP support.  Most functions are private,
**  except for those at the end of this file and those needed by
**  the database list module.
*/


static struct epdb_handle_s_t	ep_data;
static epdb_handle_t		ep_handle = &ep_data;
static unsigned32		*my_addrs = NULL;
static int			my_addr_count = 0;

static void *icmp_monitor(void *arg);

pthread_cond_t	icmp_cond;
pthread_mutex_t	icmp_mutex;
volatile boolean32	icmp_active = false;
volatile boolean32	icmp_running = false;

/*
**  Stolen from rpcrand.h in rpc/runtime.
*/
#define RPC_RANDOM_GET(lower, upper) \
	(((rpc__random_get(lower, upper)) % ((upper) - (lower) + 1)) + (lower))
extern unsigned32 rpc__random_get(unsigned32, unsigned32);

/* ______________________________________________________________________ */

#define RETURN_IF_UNRECOVERABLE(s)                      \
    if ((s) == rpc_s_unknown_if) convertit = TRUE;      \
    else if ((s) != error_status_ok) return

static void
convert_and_open(
    char                        *name,
    int                         oflags,
    dce_db_convert_func_t       func,
    dce_db_convert_func_t       func2,
    dce_db_handle_t             db,
    error_status_t              *st
)
{
    db_entry_t                  ent;
    db_entry0_t                 oldent;

    uuid_t                      *u;
    error_status_t              st2;
    boolean32                   convertit;
    char                        *p;

    /* Try to fetch the "first" item in the database.  If we failed
     * because of unknown interface, close and try to fetch under the
     * old interface version. */
    convertit = FALSE;
    dce_db_iter_start(db, st);
    RETURN_IF_UNRECOVERABLE(*st);
    dce_db_iter_next_by_uuid(db, &u, st);
    if (*st != db_s_no_more) {
        RETURN_IF_UNRECOVERABLE(*st);

        dce_db_fetch_by_uuid(db, u, &ent, st);    /* ep entries */
        RETURN_IF_UNRECOVERABLE(*st);
    }
    dce_db_iter_done(db, st);
    RETURN_IF_UNRECOVERABLE(*st);

    /* Succeeded; do we need to convert? */
    if (!convertit)
        return;

    /* Failed; just delete the old version of the file */
    dce_db_close(&db, &st2);
    unlink(name);                   /* delete the file of v 1.0 */
    *st = db_s_open_failed_enoent;  /* so as to recreate file */


}

/*------------------------------------------------------------------*/



/*
**  Towers.
*/
#if	defined(DCE_DEBUG)
static char *
tower_to_string(
    twr_t		*tower
)
{
    static char		HEX[] = "0123456789ABCDEF";
    char		*p;
    char		*save;
    unsigned char	*octet;
    unsigned32		i;

    save = p = malloc(3 * tower->tower_length + 1);
    octet = tower->tower_octet_string;
    for (i = 0; i < tower->tower_length; i++, octet++) {
	*p++ = HEX[(int)(*octet & 0xF0) >> 4];
	*p++ = HEX[(int)(*octet & 0x0F)     ];
	*p++ = ' ';
    }
    *p = '\0';
    return save;
}
#endif	/* defined(DCE_DEBUG) */

static void
tower_to_if_id(
    twr_t		*tower,
    rpc_if_id_t		*if_id,
    error_status_t	*st
)
{
    rpc_tower_ref_t	*tref;
    error_status_t	st2;
    char		*p;

    rpc__tower_to_tower_ref(tower, &tref, st);
    if (*st == error_status_ok) {
	if ((unsigned)tref->count < (unsigned)RPC_C_NUM_RPC_FLOORS)
	    *st = ept_s_invalid_entry;
	else
	    rpc__tower_flr_to_if_id(tref->floor[0], if_id, st);
	rpc__tower_ref_free(&tref, &st2);
    }

    if (*st != error_status_ok
     && DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 1)) {
	p = tower_to_string(tower);
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
	    "Bad tower in tower_to_if_id (length=%d) %s",
	    tower->tower_length, p));
	free(p);
    }
}


static void
tower_to_addr(
    twr_t		*tower,
    rpc_addr_p_t	*addr,
    error_status_t	*st
)
{
    char		*p;

    *addr = NULL;
    rpc__naf_tower_flrs_to_addr(tower->tower_octet_string, addr, st);
    if (*st != error_status_ok
     && DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 1)) {
	p = tower_to_string(tower);
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
	    "Bad tower in tower_to_addr (length=%d) %s",
	    tower->tower_length, p));
	free(p);
    }
}


/*
**  Fill in tower fields from tower.
*/
void
tower_to_fields(
    twr_t		*tower,
    twr_fields_t	*tfp,
    error_status_t	*st
)
{
    rpc_tower_ref_t	*tref;
    error_status_t	st2;
    char		*p;

    rpc__tower_to_tower_ref(tower, &tref, st);
    if (*st != error_status_ok)
	return;
    if ((unsigned)tref->count < (unsigned)(RPC_C_NUM_RPC_FLOORS + 1))
	*st = ept_s_invalid_entry;
    else if (*st == error_status_ok) {
	rpc__tower_ref_inq_protseq_id(tref, &tfp->protseq, st);
	if (*st == error_status_ok) {
	    rpc__tower_flr_to_if_id(tref->floor[0], &tfp->ifid, st);
	    if (*st == error_status_ok) {
		rpc__tower_flr_to_drep(tref->floor[1], &tfp->drep, st);
		if (*st == error_status_ok)
		    rpc__tower_flr_to_rpc_prot_id(tref->floor[2],
			&tfp->rpc_protocol, &tfp->rpc_protocol_vers_major,
			&tfp->rpc_protocol_vers_minor, st);
	    }
	}
    }
    rpc__tower_ref_free(&tref, &st2);

    if (*st != error_status_ok
     && DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 1)) {
	p = tower_to_string(tower);
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
	    "Bad tower in tower_to_fields (length=%d) %s",
	    tower->tower_length, p));
	free(p);
    }
}

/*
**  Backing store.
*/

static void
dce_db_status_to_ept_status(
    error_status_t	*st
)
{
    switch ((int)*st) {
    default:
	break;
    case db_s_fetch_failed:
	*st = ept_s_cant_access;
	break;
    case db_s_store_failed:
    case db_s_del_failed:
    case db_s_already_locked:
    case db_s_not_locked:
	*st = ept_s_update_failed;
	break;
    case db_s_not_open:
    case db_s_open_already_exists:
    case db_s_open_failed_enoent:
    case db_s_open_failed_eacces:
    case db_s_open_failed:
    case db_s_readonly:
    case db_s_mutex_init_fail:
    case db_s_mutex_lock_fail:
	*st = ept_s_cant_create;
    case db_s_no_header:
    case db_s_not_std_header:
	*st = ept_s_database_invalid;
	break;
    case db_s_key_not_found:
    case db_s_no_more:
	*st = ept_s_not_registered;
	break;
    }
}

void
epdb_lock(
    epdb_handle_t	h
)
{
    pthread_mutex_lock(&h->lock);
}

void
epdb_unlock(
    epdb_handle_t	h
)
{
    pthread_mutex_unlock(&h->lock);
}

void
epdb_database_read(
    epdb_handle_t	h,
    char		*name,
    error_status_t	*st
)
{
    db_entry_t		ent;
    db_entry_t		*new;
    uuid_t		*u;
    rpc_binding_handle_t	bh;
    boolean32		is_remote;
    error_status_t	st2;
    char		*p;

    dce_db_open(name, NULL, db_c_index_by_uuid,
	(dce_db_convert_func_t)dced_ep_convert, &h->db, st);

    if (*st != db_s_open_failed_enoent) {
    convert_and_open(name, db_c_index_by_uuid,
              (dce_db_convert_func_t)dced_ep_convert,
              (dce_db_convert_func_t)dced0_ep_convert, h->db, st);
    }

    if (*st == db_s_open_failed_enoent)
	dce_db_open(name, NULL, db_c_index_by_uuid | db_c_create,
	    (dce_db_convert_func_t)dced_ep_convert, &h->db, st);
    if (*st != error_status_ok)
	return;

    rpc_ss_enable_allocate();
    dce_db_iter_start(h->db, st);
    if (*st != error_status_ok) {
        rpc_ss_disable_allocate();
	return;
    }
    for ( ; ; ) {
        error_status_t alloc_st;
        dce_db_iter_next_by_uuid(h->db, &u, st);
        if (*st == db_s_no_more)
	    break;

	dce_db_fetch_by_uuid(h->db, u, &ent, st);
	if (*st != error_status_ok) {
	    dce_db_status_to_ept_status(st);
            rpc_ss_disable_allocate();
	    return;
	}
	if (ent.flags & db_entry_c_deleted) {
	    continue;
        }
	if (ent.flags & db_entry_c_local) {
	    continue;
        }

	/* Ignore a remote entry. */
	rpc_tower_to_binding(ent.tower->tower_octet_string, &bh, st);
	if (*st != error_status_ok
	    && DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 1)) {
	    p = tower_to_string(ent.tower);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
			   "Bad tower in epdb_database_read (length=%d) %s",
			   ent.tower->tower_length, p));
	    free(p);
	    *st = ept_s_invalid_entry;
            rpc_ss_disable_allocate();
	    return;
	}

	is_remote = dced_is_remote_handle(bh, st);
	rpc_binding_free(&bh, &st2);
	if (*st != error_status_ok
	    && DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 1)) {
	    p = tower_to_string(ent.tower);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
			   "Bad tower in epdb_database_read (length=%d) %s",
			   ent.tower->tower_length, p));
	    free(p);
	    *st = ept_s_invalid_entry;
            rpc_ss_disable_allocate();
	    return;
	}

	if (is_remote)
	    continue;

	if ((new = malloc(sizeof *new)) == NULL) {
	    *st = ept_s_cant_perform_op;
            rpc_ss_disable_allocate();
	    return;
	}
	*new = ent;
	dced_tower_copy(&new->tower, ent.tower, dced_copy_via_malloc, st);

	tower_to_addr(new->tower, &new->addr, st);
	if (*st != error_status_ok) {
	    *st = ept_s_invalid_entry;
            rpc_ss_disable_allocate();
	    return;
	}
	new->readers = 0;
	new->comm_failures = 0;
	dblist_add(&h->lists_mgmt, new);
    }
    dce_db_iter_done(h->db, st);
    dce_db_status_to_ept_status(st);
    rpc_ss_disable_allocate();
}

void
epdb_write_entry(
    epdb_handle_t	h,
    db_entry_t		*ep,
    error_status_t	*st
)
{
    rpc_addr_p_t	addr;
    db_lists_t		lists;
    static db_lists_t	nil_lists;

    addr = ep->addr;
    ep->addr = NULL;
    lists = ep->lists;
    ep->lists = nil_lists;

    dce_db_store_by_uuid(h->db, &ep->dbid, ep, st);
    dce_db_status_to_ept_status(st);

    ep->addr = addr;
    ep->lists = lists;
}

/*
**  Liveness.
*/
static boolean32
ping_server(
    db_entry_t			*ep,
    rpc_addr_p_t		addr,
    unsigned32			timeout,
    error_status_t		*status
)
{
    rpc_binding_handle_t	bh;
    boolean32			listening;
    error_status_t		st2;
    char			*p;

    rpc_tower_to_binding(ep->tower->tower_octet_string, &bh, status);
    if (*status != error_status_ok) {
	if (DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 1)) {
	    p = tower_to_string(ep->tower);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
		"Bad tower in ping_server (length=%d) %s",
		ep->tower->tower_length, p));
	    free(p);
	}
	/* XXX Should we diagnose *st and determine what to do? */
	return TRUE;
    }
    if (timeout != rpc_c_binding_default_timeout)
	rpc_mgmt_set_com_timeout(bh, timeout, status);
    if (icmp_active == false) {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
		       "(ping_server) waking icmp_monitor up"));
	pthread_cond_signal(&icmp_cond);
    }
    listening = rpc_mgmt_is_server_listening(bh, status);
    rpc_binding_free(&bh, &st2);
    /* If the server is too busy or listen backlog is full, it's alive.
       Ignore the result for DG and let the icmp monitor make the decision. */
    if (!listening) {
	if ((icmp_running == true
	     && addr->rpc_protseq_id == RPC_C_PROTSEQ_ID_NCADG_IP_UDP)
	    || *status == rpc_s_server_too_busy
	    || (addr->rpc_protseq_id == RPC_C_PROTSEQ_ID_NCACN_IP_TCP
		&& *status == rpc_s_connect_timed_out)) {
	    listening = true;
	    *status = error_status_ok;
	}
    }
    return listening;
}


/*
**  This task purges deleted entries and ping good servers; if a server
**  doesn't respond to a ping we hand it off to task2.
*/
static void *
ep_ping1(
    void		*arg
)
{
    epdb_handle_t	h;
    struct timeval	now;
    unsigned32		count;
    db_entry_t		*ep;
    db_entry_t		*next;
    boolean32		listening;
    error_status_t	status;
    rpc_addr_p_t	addr;
    time_t		waitsecs = ep_live_c_short_wait;

    /* Let other init stuff get done */
    dced_sleep(90);

    h = (epdb_handle_t)arg;
    dce_gettimeofday(&now);

    for ( ; ; ) {
	dced_sleep_until(&now, waitsecs);

	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
		       "(ep_ping1) awake"));
	epdb_lock(h);

	ep = dblist_first(&h->lists_mgmt, db_c_entry_list, NULL);
	for (count = 0; ep != NULL; ep = next) {
	    next = dblist_next(ep, db_c_entry_list);
	    /* don't delete local servers (ie, ones within dced) */
	    if (ep->flags & db_entry_c_local)
		continue;
	    if (count > ep_live_c_max_deletes) {
		count = 0;
		ep->readers++;
		epdb_unlock(h);
		dced_sleep(60);
		epdb_lock(h);
		ep->readers--;
                /*
                 * reset next, in case it was deleted or inserted
                 *   while we left the epdb unlocked
                 */
                next = dblist_next(ep, db_c_entry_list);
	    }
	    if (ep->flags & db_entry_c_deleted) {
		/* Marked for deletion by task2? */
		if (ep->readers == 0) {
		    epdb_delete_entry(h, ep, &status);
		    count++;
		}
	    }
	    else if (ep->comm_failures == 0) {
		ep->readers++;
		/* ep->addr can be temporarily nullified
		   by epdb_write_entry(). */
		addr = ep->addr;
		epdb_unlock(h);
		pthread_testcancel();
		listening = ping_server(ep, addr, ep_live_c_short_comm_timeout,
					&status);
		epdb_lock(h);
		ep->readers--;
                /*
                 * reset next, in case it was deleted or inserted
                 *   while we left the epdb unlocked
                 */
                next = dblist_next(ep, db_c_entry_list);
		if (!listening) {
		    if (ep->addr->rpc_protseq_id
			== RPC_C_PROTSEQ_ID_NCACN_IP_TCP
			&& status == rpc_s_connect_rejected) {
			/* Mark this guy dead immediately, to be reaped. */
			ep->flags |= db_entry_c_deleted;
			epdb_write_entry(h, ep, &status);
			DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
				       "(ep_ping1) server entry deleted (%d failure(s))",
				      ep->comm_failures));
		    }
		    else {
			ep->comm_failures++;
			pthread_cond_signal(&h->cv);
		    }
		}
	    }
	}

	epdb_unlock(h);
	waitsecs = RPC_RANDOM_GET((ep_live_c_long_wait
				   - ep_live_c_long_wait_space),
				  (ep_live_c_long_wait
				   + ep_live_c_long_wait_space));
    }
    return NULL;
}


/*
**  This task tries harder to reach an endpoint, eventually succeeding
**  or marking it for deletion.
*/
static void *
ep_ping2(
    void		*arg
)
{
    epdb_handle_t	h;
    struct timeval	now;
    struct timespec	waketime;
    unsigned32		waitsecs;
    boolean32		locked;
    db_entry_t		*ep;
    boolean32		listening;
    error_status_t	status;
    rpc_addr_p_t	addr;

    /* Let other init stuff get done */
    dced_sleep(300);

    h = (epdb_handle_t)arg;
    dce_gettimeofday(&now);
    waitsecs = ep_live_c_long_wait;

    epdb_lock(h);
    locked = TRUE;

    TRY
    {
	for ( ; ; ) {
	    waketime.tv_sec = now.tv_sec + waitsecs + 1;
	    waketime.tv_nsec = 0;

	    /* Release lock and wait for task2 event or timeout or cancel */
	    pthread_cond_timedwait(&h->cv, &h->lock, &waketime);
	    /* have lock now */

	    dce_gettimeofday(&now);
	    waitsecs = ep_live_c_long_wait;

	    ep = dblist_first(&h->lists_mgmt, db_c_entry_list, NULL);
	    for ( ; ep != NULL; ep = dblist_next(ep, db_c_entry_list)) {
		/* don't delete local servers (ie, ones within dced) */
		if (ep->flags & db_entry_c_local)
		    continue;

		if (ep->flags & db_entry_c_deleted || ep->comm_failures == 0)
		    continue;
		ep->readers++;
		/* ep->addr can be temporarily nullified
		   by epdb_write_entry(). */
		addr = ep->addr;
		locked = FALSE;
		epdb_unlock(h);
		pthread_testcancel();
		listening = ping_server(ep, addr,
					rpc_c_binding_default_timeout,
					&status);
		epdb_lock(h);
		locked = TRUE;
		ep->readers--;

		if (!listening) {
		    waitsecs = ep_live_c_short_wait;
		    ep->comm_failures++;
		    if ((ep->addr->rpc_protseq_id
			 == RPC_C_PROTSEQ_ID_NCACN_IP_TCP
			 && status == rpc_s_connect_rejected)
			|| (unsigned)ep->comm_failures >= (unsigned)ep_live_c_max_comm_failures) {
			/* Mark this guy dead, to be reaped. */
			ep->flags |= db_entry_c_deleted;
			epdb_write_entry(h, ep, &status);
			DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
				       "(ep_ping2) server entry deleted (%d failure(s))",
				      ep->comm_failures));
		    }
		}
		else
		    ep->comm_failures = 0;
	    }
	}
    }
    CATCH_ALL
    {
	if (locked)
	    epdb_unlock(h);
	RERAISE;
    }
    ENDTRY
    return NULL;
}


/*
**  Add an entry to the database and the lists.
*/
static void
epdb_add_entry(
    ept_entry_t		*xentry,
    twr_fields_t	*tfp,
    rpc_addr_p_t	addr,
    db_entry_t		**epp,
    error_status_t	*st
)
{
    static db_entry_t	nil_entry;
    db_entry_t		*ep;
    error_status_t	st2;

    *epp = NULL;
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
    dced_tower_copy(&ep->tower, xentry->tower, dced_copy_via_malloc, st);
    if (*st != error_status_ok) {
	*st = ept_s_cant_perform_op;
	free(ep);
	return;
    }
    ep->object = xentry->object;
    if (uuid_is_nil(&ep->object, &st2))
	ep->flags |= db_c_nil_object;
    ep->ifid = tfp->ifid;
    if (uuid_is_nil(&ep->ifid.uuid, &st2))
	ep->flags |= db_c_nil_ifspec;
    ep->drep_id = tfp->drep.id;
    ep->drep_vers_major = RPC_IF_VERS_MAJOR(tfp->drep.version);
    ep->drep_vers_minor = RPC_IF_VERS_MINOR(tfp->drep.version);
    ep->rpc_protocol = tfp->rpc_protocol;
    ep->rpc_protocol_vers_major = tfp->rpc_protocol_vers_major;
    ep->rpc_protocol_vers_minor = tfp->rpc_protocol_vers_minor;
    ep->addr = addr;
    memcpy(ep->annotation, xentry->annotation, sizeof ep->annotation);

    epdb_write_entry(ep_handle, ep, st);
    if (*st != error_status_ok)
	return;
    dblist_add(&ep_handle->lists_mgmt, ep);
    *epp = ep;
}

boolean32
compatible_syntax_ids(
    rpc_syntax_id_t	*d1,
    rpc_syntax_id_t	*d2
)
{
    error_status_t	st;

    return uuid_equal(&d1->id, &d2->id, &st)
	 && RPC_IF_VERS_MAJOR(d1->version) == RPC_IF_VERS_MAJOR(d2->version)
	 && RPC_IF_VERS_MINOR(d1->version) <= RPC_IF_VERS_MINOR(d2->version);
}


/*
**  Replace an existing entry; most fields stay the same.
*/
static void
epdb_replace_entry(
    ept_entry_t		*xentry,
    db_entry_t		*ep,
    error_status_t	*st
)
{
    ep->comm_failures = 0;
    ep->flags &= ~db_entry_c_deleted;
    memcpy(ep->annotation, xentry->annotation, sizeof ep->annotation);
    epdb_write_entry(ep_handle, ep, st);
}


/*
**  Delete an entry.
*/
void
epdb_delete_entry(
    epdb_handle_t	h,
    db_entry_t		*ep,
    error_status_t	*st
)
{
    if (ep->readers == 0) {
	dblist_remove(&h->lists_mgmt, ep);
	dce_db_delete_by_uuid(h->db, &ep->dbid, st);
	rpc__naf_addr_free(&ep->addr, st);
	free(ep->tower);
	free(ep);
	dce_db_status_to_ept_status(st);
    }
    else {
	ep->flags |= db_entry_c_deleted;
	epdb_write_entry(h, ep, st);
    }
}

/*
**  Is this entry a candidate for replacement?  Note that this is not
**  sufficient to determine actual replaceability: interface minor
**  version and network address must be considered as well.  See below.
*/
static boolean32
is_replace_candidate(
    db_entry_t		*ep,
    uuid_t		*object,
    twr_fields_t	*tfp,
    rpc_addr_p_t	addr
)
{
    error_status_t	st2;
    unsigned32		drep_vers_major;
    unsigned32		drep_vers_minor;

    drep_vers_major = RPC_IF_VERS_MAJOR(tfp->drep.version);
    drep_vers_minor = RPC_IF_VERS_MINOR(tfp->drep.version);

    return uuid_equal(object, &ep->object, &st2)
	&& uuid_equal(&tfp->ifid.uuid, &ep->ifid.uuid, &st2)
	&& tfp->ifid.vers_major == ep->ifid.vers_major
	&& addr->rpc_protseq_id == ep->addr->rpc_protseq_id
	&& uuid_equal(&tfp->drep.id, &ep->drep_id, &st2)
	&& drep_vers_major == ep->drep_vers_major
	&& drep_vers_minor == ep->drep_vers_minor
	&& tfp->rpc_protocol == ep->rpc_protocol
	&& tfp->rpc_protocol_vers_major == ep->rpc_protocol_vers_major
	&& tfp->rpc_protocol_vers_minor == ep->rpc_protocol_vers_minor;
}

/*
**  Delete entries which can be replaced by the new entry.
*/
static void
epdb_delete_replaceable_entries(
    uuid_t		*object,
    twr_fields_t	*tfp,
    rpc_addr_p_t	addr,
    error_status_t	*st
)
{
    unsigned char	*netaddr;
    unsigned char	*netaddr2;
    db_entry_t		*first;
    db_entry_t		*next;
    dblist_type_t	list_type;
    error_status_t	st2;
    db_entry_t		*ep;

    rpc__naf_addr_inq_netaddr(addr, &netaddr, st);
    if (*st != error_status_ok) {
	*st = ept_s_invalid_entry;
	return;
    }

    if (!uuid_is_nil(object, &st2)) {
	list_type = db_c_object_list;
	first = dblist_first(&ep_handle->lists_mgmt, list_type, object);
    }
    else {
	list_type = db_c_interface_list;
	first = dblist_first(&ep_handle->lists_mgmt, list_type,
		    &tfp->ifid.uuid);
    }

    /* Scan the list and see if there are any replace candidates with an
     * i/f minor version that's greater than the proposed new entry.  If
     * there are, the replace must fail. */
    for (ep = first; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (ep->flags & db_entry_c_deleted)
	    continue;
	if (is_replace_candidate(ep, object, tfp, addr)
	 && (unsigned)ep->ifid.vers_minor > (unsigned)tfp->ifid.vers_minor) {
	    rpc_string_free(&netaddr, st);
	    *st = ept_s_invalid_entry;
	    return;
	}
    }

    /* Scan the list again and delete all replace candidates that have
     * network address that matches the network entry of the new entry. */
    for (ep = first; ep != NULL; ep = next) {
	next = dblist_next(ep, list_type);
	if (ep->flags & db_entry_c_deleted)
	    continue;
	if (is_replace_candidate(ep, object, tfp, addr)) {
	    rpc__naf_addr_inq_netaddr(ep->addr, &netaddr2, &st2);
	    if (st2 != error_status_ok)
		continue;
	    if (strcmp((char *)netaddr, (char *)netaddr2) == 0)
		/* Matched in all fields except endpoint; delete it. */
		epdb_delete_entry(ep_handle, ep, &st2);
	    rpc_string_free(&netaddr2, &st2);
	}
    }

    rpc_string_free(&netaddr, st);
    *st = error_status_ok;
}

/*
**  Delete entries which match optional target object, interface, and addr.
*/
static void
epdb_delete_entries_by_obj_if_addr(
    boolean32		have_object,
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    rpc_addr_p_t	addr,
    error_status_t	*st
)
{
    unsigned32		count;
    db_entry_t		*next;
    db_entry_t		*ep;
    error_status_t	st2;

    ep = dblist_first(&ep_handle->lists_mgmt, db_c_interface_list, &ifid->uuid);
    for (count = 0; ep != NULL; ep = next) {
	next = dblist_next(ep, db_c_interface_list);
	if (ep->flags & db_entry_c_deleted)
	    continue;

	if ((!have_object || uuid_equal(object, &ep->object, &st2))
	 && uuid_equal(&ifid->uuid, &ep->ifid.uuid, &st2)
	 && ifid->vers_major == ep->ifid.vers_major
	 && ifid->vers_minor == ep->ifid.vers_minor
	 && rpc__naf_addr_compare(addr, ep->addr, &st2)) {
	    epdb_delete_entry(ep_handle, ep, st);
	    if (*st != error_status_ok)
		return;
	    count++;
	}
    }

    *st = count > 0 ? error_status_ok : ept_s_not_registered;
}

/*
**  Find an entry which exactly matches, even if deleted.
*/
static db_entry_t *
epdb_lookup_entry(
    ept_entry_t		*xentry
)
{
    db_entry_t		*ep;
    dblist_type_t	list_type;
    rpc_if_id_t		ifid;
    error_status_t	st2;

    if (!uuid_is_nil(&xentry->object, &st2)) {
	list_type = db_c_object_list;
	ep = dblist_first(&ep_handle->lists_mgmt, list_type, &xentry->object);
    }
    else {
	tower_to_if_id(xentry->tower, &ifid, &st2);
	list_type = db_c_interface_list;
	ep = dblist_first(&ep_handle->lists_mgmt, list_type, &ifid.uuid);
    }

    for (; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (uuid_equal(&xentry->object, &ep->object, &st2)
	 && xentry->tower->tower_length == ep->tower->tower_length
	 && memcmp(xentry->tower->tower_octet_string,
	      ep->tower->tower_octet_string, xentry->tower->tower_length) == 0)
	    return ep;
    }

    return NULL;
}

/*
**  Check an entry that is about to be inserted into the database.
**  All tower floors must be valid and non-nil (eg. nil interface uuid
**  is prohibited).  Also, check tower via rpc_tower_to_binding -- liveness
**  tasks will need to get binding handle.
*/
static void
epdb_chk_entry(
    ept_entry_t			*xentry,
    twr_fields_t		*tfp,
    rpc_addr_p_t		addr,
    error_status_t		*st
)
{
    rpc_binding_handle_t	bh;
    error_status_t		st2;

    if (uuid_is_nil(&tfp->ifid.uuid, &st2)
     || uuid_is_nil(&tfp->drep.id, &st2)
     || addr == NULL
     || addr->len == 0) {
	*st = ept_s_invalid_entry;
	return;
    }

    rpc_tower_to_binding(xentry->tower->tower_octet_string, &bh, st);
    if (*st != error_status_ok)
	return;

    rpc_binding_free(&bh, &st2);
}

/*
**  Add a new entry to the database.
*/
static void
epdb_insert(
    ept_entry_t		*xentry,
    boolean32		replace,
    boolean32		local,
    error_status_t	*st
)
{
    db_entry_t		*ep;
    twr_fields_t	twr_fields;
    rpc_addr_p_t	addr;
    error_status_t	st2;

    /* Pessimistic. */
    *st = ept_s_invalid_entry;
    addr = NULL;

    /* Parse the tower. */
    tower_to_fields(xentry->tower, &twr_fields, &st2);
    if (st2 != error_status_ok)
	return;
    tower_to_addr(xentry->tower, &addr, &st2);
    if (st2 != error_status_ok)
	return;
    epdb_chk_entry(xentry, &twr_fields, addr, &st2);
    if (st2 != error_status_ok) {
	if (addr != NULL)
	    rpc__naf_addr_free(&addr, &st2);
	return;
    }

    epdb_lock(ep_handle);

    if (replace) {
	epdb_delete_replaceable_entries(&xentry->object, &twr_fields, addr, st);
	if (*st != error_status_ok) {
	    rpc__naf_addr_free(&addr, &st2);
	    epdb_unlock(ep_handle);
	    return;
	}
    }

    if ((ep = epdb_lookup_entry(xentry)) == NULL)
	/* New entry. */
	epdb_add_entry(xentry, &twr_fields, addr, &ep, st);
    else {
	/* Existing entry. */
	rpc__naf_addr_free(&addr, &st2);
	epdb_replace_entry(xentry, ep, st);
    }

    if (local && *st == error_status_ok) {
	ep->flags |= db_entry_c_local;
	epdb_write_entry(ep_handle, ep, st);
    }

    epdb_unlock(ep_handle);

    if (*st != error_status_ok && addr != NULL)
	rpc__naf_addr_free(&addr, &st2);
}

/*
**  Remove a matching entry from the database.
*/
static void
epdb_delete(
    ept_entry_t		*xentry,
    error_status_t	*st
)
{
    db_entry_t		*ep;

    epdb_lock(ep_handle);

    if ((ep = epdb_lookup_entry(xentry)) == NULL
     || ep->flags & db_entry_c_deleted)
	*st = ept_s_not_registered;
    else
	epdb_delete_entry(ep_handle, ep, st);

    epdb_unlock(ep_handle);
}

void
old_ept_mgmt_delete(
    boolean32		have_object,
    uuid_t		*object,
    twr_t		*tower,
    error_status_t	*st
)
{
    rpc_if_id_t		ifid;
    rpc_addr_p_t	addr;
    error_status_t	st2;

    tower_to_if_id(tower, &ifid, st);
    if (*st != error_status_ok)
	return;
    tower_to_addr(tower, &addr, st);
    if (*st != error_status_ok)
	return;

    epdb_lock(ep_handle);
    epdb_delete_entries_by_obj_if_addr(have_object, object, &ifid,
	addr, st);
    epdb_unlock(ep_handle);
    rpc__naf_addr_free(&addr, &st2);
}


static void
epdb_to_ept(
    db_entry_t		*ep,
    ept_entry_t		*xentry,
    error_status_t	*st
)
{
    xentry->object = ep->object;
    memcpy(xentry->annotation, ep->annotation, sizeof xentry->annotation);
    dced_tower_copy(&xentry->tower, ep->tower, dced_copy_for_stub, st);
}

/*
**  If entry matches, add it to the list of entries.
*/
static void
lookup_match(
    unsigned32		inquiry_type,
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    unsigned32		vers_option,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    ept_entry_t		entries[],
    unsigned32		list_type,
    db_entry_t		**epp,
    error_status_t	*st
)
{
    boolean32		match;
    db_entry_t		*ep;
    error_status_t	st2;
    unsigned32		count;

    count = *num_ents;
    for (ep = *epp; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (ep->flags & db_entry_c_deleted)
	    continue;

	match = FALSE;
	switch ((int)inquiry_type) {
	default:
	    *st = rpc_s_invalid_inquiry_type;
	    *num_ents = count;
	    return;
	case rpc_c_ep_all_elts:
	    match = TRUE;
	    break;
	case rpc_c_ep_match_by_if:
	    if (uuid_equal(&ifid->uuid, &ep->ifid.uuid, &st2))
		match = TRUE;
	    break;
	case rpc_c_ep_match_by_obj:
	    if (uuid_equal(object, &ep->object, &st2))
		match = TRUE;
	    break;
	case rpc_c_ep_match_by_both:
	    if (uuid_equal(&ifid->uuid, &ep->ifid.uuid, &st2)
	     && uuid_equal(object, &ep->object, &st2))
		match = TRUE;
	    break;
	}

	if (match) {
	    if (inquiry_type == rpc_c_ep_match_by_if
	     || inquiry_type == rpc_c_ep_match_by_both) {
		/* check interface version */
		match = FALSE;
		switch ((int)vers_option) {
		default:
		    *st = rpc_s_invalid_vers_option;
		    *num_ents = count;
		    return;
		case rpc_c_vers_all:
		    match = TRUE;
		    break;
		case rpc_c_vers_compatible:
		    if (ifid->vers_major == ep->ifid.vers_major
		     && (unsigned)ifid->vers_minor <= (unsigned)ep->ifid.vers_minor)
			match = TRUE;
		    break;
		case rpc_c_vers_exact:
		    if (ifid->vers_major == ep->ifid.vers_major
		     && ifid->vers_minor == ep->ifid.vers_minor)
			match = TRUE;
		    break;
		case rpc_c_vers_major_only:
		    if (ifid->vers_major == ep->ifid.vers_major)
			match = TRUE;
		    break;
		case rpc_c_vers_upto:
		    if ((unsigned)ifid->vers_major > (unsigned)ep->ifid.vers_major)
			match = TRUE;
		    else if (ifid->vers_major == ep->ifid.vers_major
		        && (unsigned)ifid->vers_minor >= (unsigned)ep->ifid.vers_minor)
			match = TRUE;
		    break;
		}
	    }
	}

	if (match) {
	    if (count >= max_ents)
		break;
	    epdb_to_ept(ep, &entries[count], st);
	    if (*st != error_status_ok) {
		*num_ents = count;
		return;
	    }
	    count++;
	}
    }

    *epp = ep;
    *num_ents = count;
    *st = error_status_ok;
}


static void
lookup(
    unsigned32		inquiry_type,
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    unsigned32		vers_option,
    ept_lookup_handle_t	*lookup_handle,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    ept_entry_t		entries[],
    error_status_t	*st
)
{
    unsigned32		pass;
    dblist_type_t	list_type;
    db_entry_t		*ep;
    unsigned32		i;
    error_status_t	st2;

    /* Have context? */
    if (lookup_handle == NULL || *lookup_handle == NULL) {
	/* No -- set search. */
	switch ((int)inquiry_type) {
	default:
	    *st = rpc_s_invalid_inquiry_type;
	    return;
	case rpc_c_ep_all_elts:
	    list_type = db_c_entry_list;
	    ep = dblist_first(&ep_handle->lists_mgmt, list_type, NULL);
	    break;
	case rpc_c_ep_match_by_if:
	case rpc_c_ep_match_by_both:
	    list_type = db_c_interface_list;
	    ep = dblist_first(&ep_handle->lists_mgmt, list_type, &ifid->uuid);
	    break;
	case rpc_c_ep_match_by_obj:
	    list_type = db_c_object_list;
	    ep = dblist_first(&ep_handle->lists_mgmt, list_type, object);
	    break;
	}
	pass = 1;
    }
    else {
	/* Yes -- restore and do some sanity checking. */
	dblist_ctx_restore(ep_handle, lookup_handle, &list_type, &ep, &pass, st);
	if (*st != error_status_ok) {
	    *lookup_handle = NULL;
	    return;
	}
	switch ((int) inquiry_type) {
	default:
	    *st = rpc_s_invalid_inquiry_type;
	    break;
	case rpc_c_ep_all_elts:
	    if (list_type != db_c_entry_list)
		*st = ept_s_invalid_context;
	    break;
	case rpc_c_ep_match_by_if:
	case rpc_c_ep_match_by_both:
	    if (list_type != db_c_interface_list)
		*st = ept_s_invalid_context;
	    break;
	case rpc_c_ep_match_by_obj:
	    if (list_type != db_c_object_list)
		*st = ept_s_invalid_context;
	    break;
	}
	if (*st != error_status_ok) {
	    dblist_ctx_delete(ep_handle, lookup_handle);
	    return;
	}
    }

    lookup_match(inquiry_type, object, ifid, vers_option, max_ents,
	num_ents, entries, list_type, &ep, st);
    if (*st != error_status_ok) {
	dblist_ctx_delete(ep_handle, lookup_handle);
	for (i = 0; i < *num_ents; i++)
	    rpc_sm_free(&entries[i].tower, &st2);
	*num_ents = 0;
	return;
    }

    dblist_ctx_save(ep_handle, lookup_handle, list_type, ep, pass);
    *st = *num_ents == 0 ? ept_s_not_registered : error_status_ok;
}

/*
**  Return entries that match filter spec'ed by inquiry_type, vers_option,
**  object, and interface.
*/
void
old_ept_lookup(
    unsigned32		inquiry_type,
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    unsigned32		vers_option,
    ept_lookup_handle_t	*lookup_handle,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    ept_entry_t		entries[],
    error_status_t	*st
)
{
    epdb_lock(ep_handle);

    if (entries == NULL) {
	dblist_ctx_delete(ep_handle, lookup_handle);
	*st = ept_s_invalid_entry;
	epdb_unlock(ep_handle);
	return;
    }

    lookup(inquiry_type, object, ifid, vers_option, lookup_handle, max_ents,
	num_ents, entries, st);

    epdb_unlock(ep_handle);
}


static void
map_match(
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_protseq_id_t	protseq,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    db_entry_t		*entries[],
    ept_lookup_handle_t	*map_handle,
    unsigned32		pass,
    dblist_type_t	list_type,
    db_entry_t		**epp,
    unsigned32		*st
)
{
    boolean32		nil_object;
    unsigned32		drep_vers_major;
    unsigned32		drep_vers_minor;
    unsigned32		count;
    unsigned32		st2;
    db_entry_t		*ep;

    nil_object = uuid_is_nil(object, &st2);
    drep_vers_major = RPC_IF_VERS_MAJOR(drep->version);
    drep_vers_minor = RPC_IF_VERS_MINOR(drep->version);

    count = *num_ents;
    for (ep = *epp; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (ep->flags & db_entry_c_deleted)
	    continue;

	if (((nil_object && ep->flags & db_c_nil_object)
	  || uuid_equal(object, &ep->object, &st2))
	 && uuid_equal(&ifid->uuid, &ep->ifid.uuid, &st2)
	 && ifid->vers_major == ep->ifid.vers_major
	 && (unsigned)ifid->vers_minor <= (unsigned)ep->ifid.vers_minor
	 && protseq == ep->addr->rpc_protseq_id
	 && uuid_equal(&drep->id, &ep->drep_id, &st2)
	 && drep_vers_major == ep->drep_vers_major
	 && (unsigned)drep_vers_minor <= (unsigned)ep->drep_vers_minor
	 && rpc_protocol == ep->rpc_protocol
	 && rpc_protocol_vers_major == ep->rpc_protocol_vers_major) {
	    /* All done? */
	    if (count >= max_ents)
		break;
	    entries[count++] = ep;
	    /* If no search context don't look for next matching entry. */
	    if (map_handle == NULL && count >= max_ents)
		break;
	}
    }

    *epp = ep;
    *num_ents = count;
    *st = error_status_ok;
}

/*
**  Does addr match any of the entries's addresses?
*/
static boolean32
map_mgmt_endpt_unique(
    rpc_addr_p_t	addr,
    unsigned32		num_ents,
    db_entry_t		*entries[]
)
{
    unsigned32		i;
    error_status_t	st2;

    for (i = 0; i < num_ents; i++)
	if (rpc__naf_addr_compare(addr, entries[i]->addr, &st2))
	    return FALSE;
    return TRUE;
}


static void
map_mgmt_match(
    uuid_t		*object,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_protseq_id_t	protseq,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    db_entry_t		*entries[],
    ept_lookup_handle_t	*map_handle,
    unsigned32		pass,
    dblist_type_t	list_type,
    db_entry_t		**epp,
    unsigned32		*st
)
{
    unsigned32		drep_vers_major;
    unsigned32		drep_vers_minor;
    unsigned32		count;
    error_status_t	st2;
    db_entry_t		*ep;

    drep_vers_major = RPC_IF_VERS_MAJOR(drep->version);
    drep_vers_minor = RPC_IF_VERS_MINOR(drep->version);

    count = *num_ents;
    for (ep = *epp; ep != NULL; ep = dblist_next(ep, list_type)) {
	if (ep->flags & db_entry_c_deleted)
	    continue;

	if (uuid_equal(object, &ep->object, &st2)
	&& protseq == ep->addr->rpc_protseq_id
	&& uuid_equal(&drep->id, &ep->drep_id, &st2)
	&& drep_vers_major == ep->drep_vers_major
	&& (unsigned)drep_vers_minor <= (unsigned)ep->drep_vers_minor
	&& rpc_protocol == ep->rpc_protocol
	&& rpc_protocol_vers_major == ep->rpc_protocol_vers_major
	&& map_mgmt_endpt_unique(ep->addr, count, entries)) {
	    /* All done? */
	    if (count >= max_ents)
		break;
	    entries[count++] = ep;
	    /* If no search context don't look for next matching entry. */
	    if (map_handle == NULL && count >= max_ents)
		break;
	}
    }

    *epp = ep;
    *num_ents = count;
    *st = error_status_ok;
}

/*
**  Find an endpoint for the RPC management interface.  Return only one
**  entry per process.
*/
static void
map_mgmt(
    uuid_t		*object,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_protseq_id_t	protseq,
    ept_lookup_handle_t	*map_handle,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    db_entry_t		*entries[],
    unsigned32		*st
)
{
    unsigned32		pass;
    dblist_type_t	list_type;
    db_entry_t		*ep;
    error_status_t	st2;

    if (uuid_is_nil(object, &st2)) {
	dblist_ctx_delete(ep_handle, map_handle);
	*st = ept_s_invalid_entry;
	return;
    }

    if (map_handle == NULL || *map_handle == NULL) {
	pass = 1;
	list_type = db_c_object_list;
	ep = dblist_first(&ep_handle->lists_mgmt, list_type, object);
    }
    else {
	dblist_ctx_restore(ep_handle, map_handle, &list_type, &ep, &pass, st);
	if (*st != error_status_ok)
	    return;
    }

    map_mgmt_match(object, drep, rpc_protocol,
	rpc_protocol_vers_major, rpc_protocol_vers_minor, protseq,
	max_ents, num_ents, entries, map_handle, pass, list_type, &ep, st);
    if (*st != error_status_ok) {
	dblist_ctx_delete(ep_handle, map_handle);
	return;
    }

    dblist_ctx_save(ep_handle, map_handle, list_type, ep, pass);

    *st = *num_ents == 0 ? ept_s_not_registered : error_status_ok;
}

/*
**
**  Search the endpoint database for matching entries.
**
**  If the target object is non-nil, do a two-pass search.  First,
**  search the object list looking for exact object/interface matches.
**  Second, search the interface list for interface match/object nil
**  If the target object is nil, search the interface list for
**  interface match/object nil (same as pass two above).
**
**  The following table gives some examples of applying the above algorithm
**  to various map entriess, assuming the other fields (protseq, etc) already
**  match:
**	<a,b>	= <object uuid, interface uuid>
**	Moi	= Match on Object and Interface
**	Mi	= Match on Interface
**	-	= No match
**     \   Map	|
**	\ entry	|
**  key  \	| <nil,I1> <nil,I2> <O1,I1> <O1,I2> <O2,I1>
**  ------------|-------------------------------------------
**  <nil,I1>	|   Moi       -        -       -      -
**  <O1,I1>	|   Mi        -       Moi      -      -
*/
static void
map(
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_protseq_id_t	protseq,
    ept_lookup_handle_t	*map_handle,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    db_entry_t		*entries[],
    error_status_t	*st
)
{
    unsigned32		pass;
    dblist_type_t	list_type;
    db_entry_t		*ep;
    error_status_t	st2;

    if (map_handle == NULL || *map_handle == NULL) {
	if (uuid_is_nil(object, &st2)) {
	    pass = 2;
	    list_type = db_c_interface_list;
	    ep = dblist_first(&ep_handle->lists_mgmt, list_type, &ifid->uuid);
	}
	else {
	    pass = 1;
	    list_type = db_c_object_list;
	    ep = dblist_first(&ep_handle->lists_mgmt, list_type, object);
	}
    }
    else {
	dblist_ctx_restore(ep_handle, map_handle, &list_type, &ep, &pass, st);
	if (*st != error_status_ok)
	    return;
    }

    /*  search objectq for object/interface match */
    if (pass == 1) {
	map_match(object, ifid, drep, rpc_protocol,
	    rpc_protocol_vers_major, rpc_protocol_vers_minor, protseq,
	    max_ents, num_ents, entries, map_handle, pass, list_type, &ep, st);
	if (*st != error_status_ok) {
	    dblist_ctx_delete(ep_handle, map_handle);
	    return;
	}

	if (*num_ents >= max_ents && (ep != NULL || map_handle == NULL)) {
	    dblist_ctx_save(ep_handle, map_handle, list_type, ep, pass);
	    *st = error_status_ok;
	    return;
	}

	/* Get organized for next pass */
	pass = 2;
	list_type = db_c_interface_list;
	ep = dblist_first(&ep_handle->lists_mgmt, list_type, &ifid->uuid);
    }

    /* search interfaceq for interface match/object nil */
    if (pass == 2) {
	map_match(&uuid_g_nil_uuid, ifid, drep, rpc_protocol,
	    rpc_protocol_vers_major, rpc_protocol_vers_minor, protseq,
	    max_ents, num_ents, entries, map_handle, pass, list_type, &ep, st);
	if (*st != error_status_ok) {
	    dblist_ctx_delete(ep_handle, map_handle);
	    return;
	}
    }

    dblist_ctx_save(ep_handle, map_handle, list_type, ep, pass);
    *st = *num_ents == 0 ? ept_s_not_registered : error_status_ok;
}

static void
check_map_entry(
    twr_fields_t	*tfp,
    error_status_t	*st
)
{
    error_status_t	st2;

    if (uuid_is_nil(&tfp->ifid.uuid, &st2)
     || uuid_is_nil(&tfp->drep.id, &st2)
     || !RPC_PROTSEQ_INQ_SUPPORTED(tfp->protseq))
	*st = ept_s_invalid_entry;
    else
	*st = error_status_ok;
}

/*
**  Invoke a map function to search through the map; copy the tower into
**  fwd_towers.  This is used for CN forwading.
*/
void
old_ept_map(
    uuid_t		*object,
    twr_t		*map_tower,
    ept_lookup_handle_t	*map_handle,
    unsigned32		max_ents,
    unsigned32		*num_ents,
    twr_t		*fwd_towers[],
    error_status_t	*st
)
{
    rpc_if_rep_t	*mgmt_if_rep;
    twr_fields_t	fields;
    db_entry_t		**entries;
    unsigned32		start_ent;
    unsigned32		i;
    error_status_t	st2;

    if (dblist_ctx_different(ep_handle, map_handle, st))
	return;
    epdb_lock(ep_handle);

    if (fwd_towers == NULL || max_ents == 0 || *num_ents > max_ents) {
	dblist_ctx_delete(ep_handle, map_handle);
	*st = ept_s_cant_perform_op;
	epdb_unlock(ep_handle);
	return;
    }

    tower_to_fields(map_tower, &fields, st);
    if (*st != error_status_ok) {
	dblist_ctx_delete(ep_handle, map_handle);
	*st = ept_s_invalid_entry;
	epdb_unlock(ep_handle);
	return;
    }

    check_map_entry(&fields, st);
    if (*st != error_status_ok) {
	dblist_ctx_delete(ep_handle, map_handle);
	*st = ept_s_invalid_entry;
	epdb_unlock(ep_handle);
	return;
    }

    entries = (db_entry_t **)malloc(max_ents * sizeof *entries);
    if (entries == NULL) {
	dblist_ctx_delete(ep_handle, map_handle);
	*st = ept_s_cant_perform_op;
	epdb_unlock(ep_handle);
	return;
    }

    mgmt_if_rep = (rpc_if_rep_t *)mgmt_v1_0_s_ifspec;
    start_ent = *num_ents;
    if (uuid_equal(&fields.ifid.uuid, &mgmt_if_rep->id, &st2))
	map_mgmt(object, &fields.drep, fields.rpc_protocol,
	    fields.rpc_protocol_vers_major, fields.rpc_protocol_vers_minor,
	    fields.protseq, map_handle, max_ents, num_ents, entries, st);
    else
	map(object, &fields.ifid, &fields.drep, fields.rpc_protocol,
	    fields.rpc_protocol_vers_major, fields.rpc_protocol_vers_minor,
	    fields.protseq, map_handle, max_ents, num_ents, entries, st);
    if (*st != error_status_ok) {
	free(entries);
	epdb_unlock(ep_handle);
	return;
    }

    for (i = start_ent; i < *num_ents; i++) {
	dced_tower_copy(&fwd_towers[i], entries[i]->tower, dced_copy_for_stub, st);
	if (*st != error_status_ok) {
	    /* Return what we got but kill context. */
	    dblist_ctx_delete(ep_handle, map_handle);
	    *num_ents = i;
	    *st = *num_ents > 0 ? error_status_ok : ept_s_cant_perform_op;
	    break;
	}
    }

    free(entries);
    epdb_unlock(ep_handle);
}


/*
**  Invoke a map function to search through the map for DG forwarding; copy
**  the adress info into fwd_addrs.  Save context unless there were errors.
**  Perform the forwarding map algorithm to produce an rpc_addr to the
**  selected endpoint.
**
**  Eventually, we probably want to get all packets from a single activity
**  to a single server (assuming that we can figure out how take advantage
**  of selecting different potential servers in the face of stale entries).
*/
void
old_ept_fwd_map(
    uuid_t		*object,
    rpc_if_id_t		*ifid,
    boolean32		is_mgmt_if,
    rpc_syntax_id_t	*drep,
    rpc_protocol_id_t	rpc_protocol,
    unsigned32		rpc_protocol_vers_major,
    unsigned32		rpc_protocol_vers_minor,
    rpc_addr_p_t	addr,
    uuid_t		*actuuid,
    unsigned32		*num_ents,
    rpc_addr_p_t	fwd_addrs[],
    error_status_t	*st
)
{
    db_entry_t		*entry;

    epdb_lock(ep_handle);
    if (is_mgmt_if)
	map_mgmt(object, drep, rpc_protocol, rpc_protocol_vers_major,
	    rpc_protocol_vers_minor, addr->rpc_protseq_id, NULL,
	    1, num_ents, &entry, st);
    else
	map(object, ifid, drep, rpc_protocol, rpc_protocol_vers_major,
	    rpc_protocol_vers_minor, addr->rpc_protseq_id, NULL,
	    1, num_ents, &entry, st);
    if (*st == error_status_ok
     && *num_ents == 1
     && entry->flags & db_entry_c_local)
	/* Ignore "local" entries that were inserted for CN; DG wants to
	 * get failures so that the runtime will do "in-line" forwarding. */
	*st = ept_s_not_registered;
    if (*st != error_status_ok) {
	epdb_unlock(ep_handle);
	return;
    }
    if (*num_ents == 0) {
	*st = ept_s_cant_perform_op;
	epdb_unlock(ep_handle);
	return;
    }

    rpc__naf_addr_copy(entry->addr, &fwd_addrs[0], st);
    if (*st != error_status_ok) {
	*num_ents = 0;
	*st = ept_s_cant_perform_op;
    }
    epdb_unlock(ep_handle);
}

static void
epdb_delete_lookup_handle(
    ept_lookup_handle_t	*lookup_handle
)
{
    epdb_lock(ep_handle);
    dblist_ctx_delete(ep_handle, lookup_handle);
    epdb_unlock(ep_handle);
}

/*
**  External interface to EP interface.
*/
void
old_ept_insert(
    unsigned32		num_ents,
    ept_entry_t		entries[],
    boolean32		replace,
    boolean32		local,
    error_status_t	*st
)
{
    ept_entry_t		*ep;
    unsigned32		i;
    error_status_t	st2;

    for (i = num_ents, ep = entries; i; i--, ep++) {
	epdb_insert(ep, replace, local, st);
	if (*st != error_status_ok) {
	    for (ep = entries; i < num_ents; i++, ep++)
		epdb_delete(ep, &st2);
	    return;
	}
    }
}


void
old_ept_delete(
    unsigned32		num_ents,
    ept_entry_t		entries[],
    error_status_t	*st
)
{
    ept_entry_t		*ep;
    unsigned32		i;

    for (i = 0, ep = entries; i < num_ents; i++, ep++) {
	epdb_delete(ep, st);
	if (*st != error_status_ok)
	    return;
    }
}


void
old_ept_lookup_handle_free(
    ept_lookup_handle_t	*lookup_handle,
    error_status_t	*st
)
{
    epdb_delete_lookup_handle(lookup_handle);
    *st = error_status_ok;
}


void
old_ept_lookup_handle_t_rundown(
    ept_lookup_handle_t		lookup_handle
)
{
    epdb_delete_lookup_handle(&lookup_handle);
}


/*
** Initialization.
*/
void
old_ept_init(
    uuid_t		*u,
    error_status_t	*st
)
{
    int			i;
    int			j;
    unsigned char	*stb;
    unsigned char	*saddr;
    unsigned32		iaddr;
    error_status_t	st1;
    pthread_t		icmp_task;

    my_addrs =
	(unsigned32 *)malloc(dced_bindings->count * sizeof(*my_addrs));
    if (my_addrs == NULL) {
	*st = ept_s_no_memory;
	return;
    }

    /* Get the network address of each binding. */
    for (my_addr_count = 0, i = 0; i < dced_bindings->count; i++) {
	rpc_binding_to_string_binding(dced_bindings->binding_h[i], &stb, st);
	if (*st != error_status_ok)
	    return;
	rpc_string_binding_parse(stb, NULL, NULL, &saddr, NULL, NULL, st);
	rpc_string_free(&stb, &st1);
	if (*st != error_status_ok) {
	    rpc_string_free(&saddr, &st1);
	    return;
	}
	iaddr = inet_addr(saddr);
	rpc_string_free(&saddr, &st1);

	/* Compare against already cached addresses. */
	for (j = 0; j < my_addr_count; j++) {
	    if (my_addrs[j] == iaddr)
		break;
	}
	if (j == my_addr_count)
	    my_addrs[my_addr_count++] = iaddr;
    }
    /* Open the endpoint database.  Create it if necessary. */
    dblist_init(ep_handle);
    epdb_database_read(ep_handle, EP_DB_NAME, st);
    if (*st != error_status_ok) {
	fprintf(stderr, "Failed opening %s, 0x%8.8lx\n", EP_DB_NAME, (long)*st);
	exit(1);
    }
    pthread_cond_init(&ep_handle->cv, pthread_condattr_default);
    pthread_create(&ep_handle->sliv_task1, thread_attr, ep_ping1, ep_handle);
    pthread_create(&ep_handle->sliv_task2, thread_attr, ep_ping2, ep_handle);
    pthread_mutex_init(&icmp_mutex, pthread_mutexattr_default);
    pthread_cond_init(&icmp_cond, pthread_condattr_default);
    pthread_create(&icmp_task, thread_attr, icmp_monitor, ep_handle);
    pthread_detach(&icmp_task);
}


#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>

#ifdef  MAX_DEBUG
static void *
hexdump(
    unsigned char *p,
    int len
)
{
    int i;

    for (i = 0; i < len; i++)
    {
        if ((i&0x0f) == 0)
            fprintf(stderr, "\n%04x:", i);
        fprintf(stderr, " %02x", p[i]);
    }
    fprintf(stderr, "\n");
}
#endif

/*
**  Delete entries which match endpoint.
*/
static unsigned32
epdb_delete_entries_by_endpoint(
    unsigned16		port,
    error_status_t	*st
)
{
    unsigned32		count;
    db_entry_t		*next;
    db_entry_t		*ep;
    error_status_t	st2;
    struct sockaddr_in  *saddr;

    *st = error_status_ok;

    ep = dblist_first(&ep_handle->lists_mgmt, db_c_entry_list, NULL);
    for (count = 0; ep != NULL; ep = next) {
	next = dblist_next(ep, db_c_entry_list);
	/* don't delete local servers (ie, ones within dced) */
	if (ep->flags & db_entry_c_local
	    || ep->flags & db_entry_c_deleted)
	    continue;

	if (ep->addr->rpc_protseq_id == RPC_C_PROTSEQ_ID_NCADG_IP_UDP
	    && ((struct sockaddr_in *) &ep->addr->sa)->sin_port == port) {
	    epdb_delete_entry(ep_handle, ep, &st2);
	    if (st2 == error_status_ok)
		count++;
	    else if (*st == error_status_ok) {
		*st = st2;
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug3,
      "(epdb_delete_entries_by_endpoint) Can't delete entry, port %d 0x%08x",
			       port, st2));
	    }
	}
    }

    if (*st == error_status_ok)
	*st = count > 0 ? error_status_ok : ept_s_not_registered;

    return count;
}

#define	icmp_monitor_c_max_n_ports	10
#define	icmp_monitor_c_long_wait	(5 * 60)
#define	icmp_monitor_c_short_wait	1
#define	icmp_monitor_c_active_time	5

static void *
icmp_monitor(
    void		*arg
)
{
    int s;
    unsigned char packet[64];
    int len;
    struct ip *p_ipp;
    struct ip *d_ipp;
    struct icmp *p_icmpp;
    struct udphdr *d_udphp;
    error_status_t st;
    unsigned32 n;
    int i;
    int j;
    char msg[160];
    char *cp;
    struct timeval now;
    struct timeval signaled_time;
    struct timespec waketime;
    int count = 0;
    int n_ports = 0;
    unsigned16 dport;
    unsigned16 ports[icmp_monitor_c_max_n_ports];
#ifdef  MAX_DEBUG
    char dbuf[1024];
#endif

    signaled_time.tv_sec = 0;
    signaled_time.tv_usec = 0;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
		   "(icmp_monitor) starting up..."));

    s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (s == -1) {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
		       "(icmp_monitor) Can't create socket, (%d) %s",
		       errno, strerror(errno)));
        return NULL;
    }

    if (fcntl(s, F_SETFD, 1) == -1) {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
		       "(icmp_monitor) Can't set close-on-exec flag, (%d) %s",
		       errno, strerror(errno)));
        return NULL;
    }

    if (fcntl(s, F_SETFL, O_NONBLOCK) == -1) {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
		       "(icmp_monitor) Can't set non-blocking mode, (%d) %s",
		       errno, strerror(errno)));
        return NULL;
    }

    pthread_mutex_lock(&icmp_mutex);
    icmp_active = false;
    icmp_running = true;

    for (;;) {
        len = recv(s, &packet, sizeof(packet), 0);
        if (len == 0) {
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
			   "(icmp_monitor) End-of-File"));
            break;
        }
        if (len == -1) {
	    switch (errno) {
	    case EINTR:
		continue;
		break;

	    case EAGAIN:
		dce_gettimeofday(&now);
		if (icmp_active
		    && signaled_time.tv_sec != 0
		    && (now.tv_sec <
			signaled_time.tv_sec + icmp_monitor_c_active_time)) {
		    waketime.tv_sec = now.tv_sec + icmp_monitor_c_short_wait;
		    waketime.tv_nsec = 0;
		}
		else {
		    icmp_active = false;
		    n_ports = 0;
		    waketime.tv_sec = now.tv_sec + icmp_monitor_c_long_wait;
		    waketime.tv_nsec = 0;
		}
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
			       "(icmp_monitor) got %d; sleeping (%s)...",
			       count, (icmp_active?"short":"long")));
		count = 0;
		i = pthread_cond_timedwait(&icmp_cond, &icmp_mutex, &waketime);
#ifdef	PTHREAD_EXC
		if (i == 0) {	/* signaled */
		    icmp_active = true;
		    dce_gettimeofday(&signaled_time);
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
				   "(icmp_monitor) signaled"));
		    continue;
		}
		else if (i == EAGAIN) {	/* timeout */
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
				   "(icmp_monitor) awake (timeout)"));
		    continue;
		}
#else
		if (i == 0) {	/* signaled */
		    icmp_active = true;
		    dce_gettimeofday(&signaled_time);
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
				   "(icmp_monitor) signaled"));
		    continue;
		}
		else if (i == -1 && errno == EAGAIN) {	/* timeout */
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
				   "(icmp_monitor) awake (timeout)"));
		    continue;
		}
#endif	/* PTHREAD_EXC */
		/* Otherwise, ignore for now */
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
			       "(icmp_monitor) awake (%d, (%d) %s)",
			       i, errno, strerror(errno)));
		continue;
		break;

	    default:
		break;
	    }
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
			   "(icmp_monitor) Can't read socket, (%d) %s",
			   errno, strerror(errno)));
            break;
        }
	else if (len < 56)
        {
	    /* 20(ip header)+8(icmp header)+20(ip header)+8(udp header) */
#ifdef  MAX_DEBUG
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
			   "(icmp_monitor) packet length shorter %d < 56",
			   len));
#endif
            continue;
        }

	count++;
        p_ipp = (struct ip *)&packet;
        
#ifdef  MAX_DEBUG
	if (DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 3)) {
            hexdump(packet, len);

	    cp = dbuf;
	    *cp = '\0';
	    sprintf(cp,
		    "IP Header:\nip_v:   %d\nip_hl:  %d\nip_tos: %d\n"
		    "ip_len: %d\nip_id:  %d\nip_off: %d\nip_ttl: 0x%02x\n"
		    "ip_p:   %d\nip_sum: 0x%04x\nip_src: %s\n",
		    p_ipp->ip_v, p_ipp->ip_hl, p_ipp->ip_tos,
		    ntohs(p_ipp->ip_len), p_ipp->ip_id, p_ipp->ip_off,
		    p_ipp->ip_ttl, p_ipp->ip_p, p_ipp->ip_sum,
		    inet_ntoa(p_ipp->ip_src));
            
            DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug3,
			   "%sip_dst: %s", dbuf, inet_ntoa(p_ipp->ip_dst)));
        }
#endif
        
        if (p_ipp->ip_v != IPVERSION) {
#ifdef  MAX_DEBUG
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
			   "(icmp_monitor) IP version mismatch %d != %d",
			   p_ipp->ip_v, IPVERSION));
#endif
            continue;
        }
        if (p_ipp->ip_p != IPPROTO_ICMP) {
#ifdef  MAX_DEBUG
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
			   "(icmp_monitor) IP protocol mismatch %d != %d",
			   p_ipp->ip_p, IPPROTO_ICMP));
#endif
            continue;
        }
        if (ntohs(p_ipp->ip_len) < 36) {
#ifdef  MAX_DEBUG
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
			   "(icmp_monitor) IP length shorter %d < 36",
			   ntohs(p_ipp->ip_len)));
#endif
            continue;
        }
        
        p_icmpp = (struct icmp *)&packet[p_ipp->ip_hl<<2];
        
#ifdef  MAX_DEBUG
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug3,
"  ICMP Header:\n  icmp_type:  %d\n  icmp_code:  %d\n  icmp_cksum: 0x%02x\n  icmp_void:  0x%04x",
		       p_icmpp->icmp_type, p_icmpp->icmp_code,
		       p_icmpp->icmp_cksum, p_icmpp->icmp_void));
#endif
        
        if (p_icmpp->icmp_type != ICMP_UNREACH
            || p_icmpp->icmp_code != ICMP_UNREACH_PORT) {
#ifdef  MAX_DEBUG
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
		   "(icmp_monitor) Non-unreachable UDP port message (%d,%d)",
			   p_icmpp->icmp_type, p_icmpp->icmp_code));
#endif
            continue;
	}
        
        d_ipp = (struct ip *)&p_icmpp->icmp_data;

#ifdef  MAX_DEBUG
	if (DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 3)) {
	    cp = dbuf;
	    *cp = '\0';
	    sprintf(cp,
		    "    IP Header:\n    ip_v:   %d\n    ip_hl:  %d\n"
		    "    ip_tos: %d\n    ip_len: %d\n    ip_id:  %d\n"
		    "    ip_off: %d\n    ip_ttl: 0x%02x\n    ip_p:   %d\n"
		    "    ip_sum: 0x%04x\n    ip_src: %s\n",
		    d_ipp->ip_v, d_ipp->ip_hl, d_ipp->ip_tos,
		    ntohs(d_ipp->ip_len), d_ipp->ip_id, d_ipp->ip_off,
		    d_ipp->ip_ttl, d_ipp->ip_p, d_ipp->ip_sum,
		    inet_ntoa(d_ipp->ip_src));
            
            DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug3,
			   "%s    ip_dst: %s",
			   dbuf, inet_ntoa(d_ipp->ip_dst)));
        }
#endif
        
        if (d_ipp->ip_p != IPPROTO_UDP) {
#ifdef  MAX_DEBUG
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
			   "(icmp_monitor) IP protocol mismatch %d != %d",
			   d_ipp->ip_p, IPPROTO_UDP));
#endif
            continue;
        }
        
        d_udphp = (struct udphdr *)((unsigned char *)d_ipp + (d_ipp->ip_hl<<2));
	dport = ntohs(d_udphp->uh_dport);

#ifdef  MAX_DEBUG
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug3,
		       "      UDP Header:\n      uh_sport: %d\n"
		       "      uh_dport: %d\n      uh_ulen:  %d\n"
		       "      uh_sum:   0x%02x",
		       ntohs(d_udphp->uh_sport), dport,
		       ntohs(d_udphp->uh_ulen), d_udphp->uh_sum));
#endif
	if (ntohs(d_udphp->uh_ulen) < 88) {
	    /* 8(udp header)+80(dg header) */
#ifdef  MAX_DEBUG
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
		   "(icmp_monitor) failed udp packet length shorter %d < 88",
			   ntohs(d_udphp->uh_ulen)));
#endif
            continue;
	}

	if (DCE_SVC_DEBUG_ATLEAST(dhd_svc_handle, dhd_s_ep, 1)) {
	    cp = msg;
	    *cp = '\0';
	    sprintf(cp, "(icmp_monitor) Unreachable UDP port: %s:%d", 
		    inet_ntoa(d_ipp->ip_src), ntohs(d_udphp->uh_sport));
	    cp += strlen(cp);
	    sprintf(cp, " -> %s:%d;",
		    inet_ntoa(d_ipp->ip_dst), dport);
	    cp += strlen(cp);
	}

	for (i = 0; i < my_addr_count; i++) {
 	    if (d_ipp->ip_dst.s_addr == my_addrs[i]) {
		for (j = 0; j < n_ports; j++) {
		    if (ports[j] == dport)
			break;
		}
		if (n_ports != 0 && j != n_ports) {
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
				   "%s Ignored, cache hit", msg));
		    break;
		}

		/* Cache the port. */
		if (n_ports < icmp_monitor_c_max_n_ports)
		    ports[n_ports++] = dport;

		epdb_lock(ep_handle);
		n = epdb_delete_entries_by_endpoint(dport, &st);
		epdb_unlock(ep_handle);

		if (n > 0) {
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
				   "%s %d entry(s) deleted", msg, n));
		}
		else {
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug2,
				   "%s Ignored, 0 entry deleted", msg));
		}
		break;
	    }
	}
    }

    icmp_running = false;

    pthread_mutex_unlock(&icmp_mutex);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_ep, svc_c_debug1,
		   "(icmp_monitor) shutting down..."));

    return NULL;
}
