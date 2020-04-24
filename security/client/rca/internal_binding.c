/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: internal_binding.c,v $
 * Revision 1.1.29.2  1996/10/15  20:48:40  arvind
 * 	Initialize cache hint in binding handle.
 * 	[1996/10/11  00:01 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 *
 * 	Fix another binding spin in rca_check_binding_resolved(). CHFts19224.
 * 	[1996/07/29  15:03 UTC  jrr  /main/HPDCE02/jrr_davis_5/1]
 *
 * 	Add more comments to changes in rca_rebind().
 * 	[1996/03/28  20:12 UTC  jrr  /main/HPDCE02/jrr_davis_1/3]
 *
 * 	In rca_rebind, if the binding handle has the UPDATE flag set,
 * 	try the towers only and then fail. Back out the previous
 * 	insufficient fix for checking for the UPDATE flag in
 * 	rca_retry_status().
 * 	Remove extra retry macros from rca_site_resolve_update().
 * 	[1996/03/28  16:00 UTC  jrr  /main/HPDCE02/jrr_davis_1/2]
 *
 * 	Modify rca_retry_status() to only allow rebind on sec_rgy_rep_bad_state if
 * 	the binding handle doesn't have the SITE_UPDATEABLE flag set.
 * 	[1996/03/20  22:03 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 *
 * 	merge cell_name mem leak fix
 * 	[1996/08/08  19:34 UTC  bartf  /main/bartf_pk_1.2.2/1]
 *
 * 	CHFts19512: fix rca_site_bind_from_tower_vec has mem leak for cellname
 * 	[1996/02/18  00:07:14  marty  1.1.25.3]
 *
 * Revision 1.1.29.1  1996/10/03  14:46:09  arvind
 * 	Mark as volatile variables live across setjmp.
 * 	[1996/09/16  20:30 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	merge cell_name mem leak fix
 * 	[1996/08/08  19:34 UTC  bartf  /main/bartf_pk_1.2.2/1]
 * 
 * 	CHFts19512: fix rca_site_bind_from_tower_vec has mem leak for cellname
 * 	[1996/02/18  00:07:14  marty  1.1.25.3]
 * 
 * Revision 1.1.25.3  1996/02/18  00:07:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:19  marty]
 * 
 * Revision 1.1.25.2  1995/12/08  18:06:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/19  1995/10/30  15:20 UTC  mullan_s
 * 	Merge fix for CHFts15792
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15792/1  1995/10/30  14:43 UTC  mullan_s
 * 	CHFts15792: rca_retry_status: retry if sec_rgy_rep_bad_state
 * 	is returned.
 * 
 * 	HP revision /main/HPDCE02/18  1995/10/19  20:04 UTC  aha
 * 	Merge fixes for CHFts15451, CHFts16596, resubmit CHFts16482.
 * 	[1995/10/11  18:54 UTC  mullan_s  /main/HPDCE02/17]
 * 
 * 	Merge with burati's fix for CHFts15451
 * 	[1995/10/09  17:43 UTC  mullan_s  /main/HPDCE02/mullan_chts16596/2]
 * 
 * 	merge up to mainline
 * 	[1995/10/09  17:38 UTC  burati  /main/HPDCE02/mb_mothra7/2]
 * 
 * 	CHFts16596: rca_rebind should call rca_get_site_bindings to replenish
 * 	alt_handles if all have been tried already.
 * 	[1995/10/09  17:35 UTC  mullan_s  /main/HPDCE02/mullan_chts16596/1]
 * 
 * 	Backout CHFts16482
 * 	[1995/10/06  18:38 UTC  mullan_s  /main/HPDCE02/16]
 * 
 * 	Merge fix for CHFts16017
 * 	[1995/09/27  18:12 UTC  mullan_s  /main/HPDCE02/14]
 * 
 * 	rca_rebind: fallback to pe_site file if PATIENT_REBIND flag is not set.
 * 	[1995/09/19  17:25 UTC  mullan_s  /main/HPDCE02/mullan_chfts16017/1]
 * 
 * 	Retry for update binding
 * 	[1995/09/14  20:11 UTC  burati  /main/HPDCE02/mb_mothra6/1]
 * 
 * 	HP revision /main/HPDCE02/aha_mothra10/5  1995/10/19  19:09 UTC  aha
 * 	CHFts15769: control commands can bind to stopped sec server;
 * 	 add comment to binding_cache_lookup about apparently, but not, redundant code
 * 
 * 	HP revision /main/HPDCE02/aha_mothra10/4  1995/10/16  20:56 UTC  aha
 * 	CHFts15769: fix typo's
 * 
 * 	HP revision /main/HPDCE02/aha_mothra10/3  1995/10/16  20:16 UTC  aha
 * 	CHFts15769: control commands can bind to stopped security servers;
 * 	 in binding_cache_lookup(), return a context from the cache only
 * 	      if the requested_site_name matches (in addition to current tests)
 * 
 * 	HP revision /main/HPDCE02/aha_mothra10/2  1995/10/10  19:46 UTC  aha
 * 	merge out
 * 
 * 	HP revision /main/HPDCE02/mullan_chts16596/2  1995/10/09  17:43 UTC  mullan_s
 * 	Merge with burati's fix for CHFts16482
 * 
 * 	HP revision /main/HPDCE02/mb_mothra7/2  1995/10/09  17:38 UTC  burati
 * 	merge up to mainline
 * 
 * 	HP revision /main/HPDCE02/mullan_chts16596/1  1995/10/09  17:35 UTC  mullan_s
 * 	CHFts16596: rca_rebind should call rca_get_site_bindings to replenish
 * 	alt_handles if all have been tried already.
 * 
 * 	HP revision /main/HPDCE02/15  1995/09/28  19:45 UTC  aha
 * 	CHFts16482: fix infinite loop in rca_check_binding_resolved()
 * 
 * 	HP revision /main/HPDCE02/aha_mothra10/1  1995/09/26  13:36 UTC  aha
 * 	CHFts16482: infinite loop in rca_check_binding_resolved
 * 
 * 	HP revision /main/HPDCE02/14  1995/09/27  18:12 UTC  mullan_s
 * 	Merge fix for CHFts16017
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16017/1  1995/09/19  17:25 UTC  mullan_s
 * 	rca_rebind: fallback to pe_site file if PATIENT_REBIND flag is not set.
 * 
 * 	HP revision /main/HPDCE02/mb_mothra6/1  1995/09/14  20:11 UTC  burati
 * 	Retry for update binding
 * 
 * 	HP revision /main/HPDCE02/13  1995/07/12  21:22 UTC  mullan_s
 * 	Merge fix for CHFts15509
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15509/2  1995/07/12  21:11 UTC  mullan_s
 * 	Remove rpc_ns_set_exp_age(0->60) change from rca_nsi_binding_import.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15509/1  1995/07/10  17:09 UTC  mullan_s
 * 	CHFts15509: rca_rebind: avoid infinite loop in RETRY macro by setting
 * 	PATIENT rebind flag.
 * 
 * 	HP revision /main/HPDCE02/12  1995/06/29  18:44 UTC  mullan_s
 * 	Merge fix for CHFts15613
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_chfts15535/1  1995/06/29  18:34 UTC  mullan_s
 * 	In rca_rebind, break out of for(;;) loop if maximum number of
 * 	replicas have been tried.
 * 
 * 	HP revision /main/HPDCE02/11  1995/06/16  20:06 UTC  mdf
 * 	Memory leak at link at new_context->cell_name
 * 	[1995/06/15  19:02 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_6/1]
 * 
 * 	HP revision /main/HPDCE02/10  1995/05/24  19:27 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat10/1  1995/05/23  19:01 UTC  mullan_s
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 
 * 	HP revision /main/HPDCE02/9  1995/05/15  20:59 UTC  greg
 * 	Account for "patient" rebinds in rca_check_bie
 * 
 * Revision 1.1.21.2  1994/10/18  13:01:29  sekhar
 * 	[ OT 10455 ]  Fix uninitialized variable useage in rca_rebind().
 * 	[1994/10/17  20:15:41  sekhar]
 * 
 * Revision 1.1.21.1  1994/10/17  14:41:02  sekhar
 * 	[ OT 5750 ]  Log SVC messages for "Registry Server Unavailable"
 * 	[1994/10/17  14:30:56  sekhar]
 * 
 * 	HP revision /main/HPDCE02/1  1994/09/21  12:47 UTC  aha
 * 	Mothra fixes for internal_binding.c
 * 
 * 	HP revision /main/HPDCE01/aha_binding1/1  1994/09/16  16:08 UTC  aha
 * 	Remove cache-empty assertions in sec_rgy_site_close()
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:00  mdf
 * 	merge
 * 
 * Revision 1.1.18.3  1994/06/02  21:57:35  mdf
 * 	Merged with changes from 1.1.18.2
 * 	[1994/06/02  21:56:23  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  19:39:25  mdf]
 * 
 * Revision 1.1.18.2  1994/06/02  20:21:15  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  19:39:25  mdf]
 * 
 * Revision 1.1.14.2  1993/10/15  21:49:19  rps
 * 	[OT7758] rca_site_resolve_update:  don't stomp on input context rpc handle
 * 	[1993/10/15  21:48:05  rps]
 * 
 * 	[OT7758] rca_site_resolve_update:  don't stomp on input context rpc handle
 * 
 * Revision 1.1.14.1  1993/10/05  22:29:59  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:04:27  mccann]
 * 
 * Revision 1.1.10.4  1993/12/15  20:12:58  rps
 * 	rca_rebind_tower: ignore "" cell_name to avoid malformed princ realm
 * 	rca_site_bind_from_tower_vec:  canonicalize cell_name (ditto)
 * 	[1993/12/15  20:10:15  rps]
 * 
 * 	[OT8109/CHFts07857] sec_rgy_site_close:  free handle mutex
 * 	[CHFts08025] rca_rebind:  if nsi fails, fall back to pe_sites
 * 	[CHFts07628] When "no more bindings", force a cache flush for retry.
 * 	[OT7791] rca_site_check_readonly: don't free memory in compatibility case
 * 	[1993/08/18  22:57:18  rps]
 * 
 * Revision 1.1.10.3  1993/12/06  23:11:35  rps
 * 	[OT9488/CHFts09626]
 * 	rca_rebind_tower: set realm in auth_info principal name!
 * 	rca_site_resolve_update: check _bind_from_tower_vec return status
 * 	[OT9516/CHFts09622]
 * 	rca_nsi_binding_import: return value in error case; reset exp_age
 * 	[1993/12/02  16:06:41  rps]
 * 
 * Revision 1.1.10.2  1993/10/12  01:50:28  rps
 * 	[OT7758] rca_site_resolve_update:  don't stomp on input context rpc handle
 * 	[1993/10/12  01:47:50  rps]
 * 
 * Revision 1.1.8.3  1993/09/15  15:37:25  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.6.18  1993/03/22  21:35:04  burati
 * 	CR Free tmp_site_name with free(), not rpc_ss_client_free()
 * 	[1993/03/22  19:48:32  burati]
 * 
 * Revision 1.1.6.17  1993/03/17  22:52:02  rps
 * 	[OT7448] Make rca_site_bind_nsi_cell try all available ns bindings
 * 	[1993/03/17  22:04:42  rps]
 * 
 * Revision 1.1.6.16  1993/03/15  21:42:44  rps
 * 	[OT7469] Clear return status in rca_site_check_readonly
 * 	[1993/03/15  21:39:15  rps]
 * 
 * Revision 1.1.6.15  1993/03/11  23:10:48  rps
 * 	[OT7486] site_bind_update needs to deref output handle when setting flags
 * 	[1993/03/11  22:44:23  rps]
 * 
 * Revision 1.1.6.14  1993/03/09  18:33:59  rps
 * 	[OT7258] Add name parms to rca_site_bind_from_tower_vec
 * 	[1993/03/09  16:53:52  rps]
 * 
 * Revision 1.1.6.13  1993/03/08  17:54:59  rps
 * 	[OT7272] Implement sec_rgy_site_is_readonly()'s handle flags.
 * 	Fix previous CR6726 fix in pre-rep compatibility case &
 * 	Fix rca_site_resolve_update() in parameter "icontext"
 * 	[1993/03/03  20:26:18  rps]
 * 
 * Revision 1.1.6.12  1993/02/26  23:46:51  emartin
 * 	[OT7264] Assign cell and site names in rca_rebind_tower.
 * 	[OT7323] Add ept_s_not_registered as retryable status.
 * 	[1993/02/25  22:28:57  emartin]
 * 
 * Revision 1.1.6.11  1993/02/21  19:39:29  burati
 * 	CR6726 Plug leaking context in rca_site_resolve_update by using
 * 	separate [in] and [out] contexts, so the temporary one won't be
 * 	overwritten before the caller has a chance to free it.
 * 	[1993/02/21  00:22:47  burati]
 * 
 * Revision 1.1.6.10  1993/02/09  19:38:49  rps
 * 	[OT7166] rca_site_resolve_update: restore sec service uuid in tower bindings
 * 	rca_rebind_tower:  free rpc binding handle in error cases
 * 	[1993/02/08  23:37:53  rps]
 * 
 * Revision 1.1.6.9  1993/02/07  22:40:56  burati
 * 	CR6726 Fix leaking NSI handles
 * 	[1993/02/07  19:37:27  burati]
 * 
 * Revision 1.1.6.8  1993/02/05  21:38:07  sommerfeld
 * 	[OT5061] Fix typo in previous fix.
 * 	[1993/02/05  21:36:19  sommerfeld]
 * 
 * 	[OT5718] check auth_params for validity.
 * 	[OT5061] don't clobber cell name in binding handle.
 * 	[1993/02/04  22:16:57  sommerfeld]
 * 
 * Revision 1.1.6.7  1993/01/14  21:03:19  rps
 * 	[OT 5498] Expose auth_tkt_expired error
 * 	[OT 6577] Set rpc auth in rebind_tower only when specified
 * 	[1993/01/14  21:00:37  rps]
 * 
 * Revision 1.1.6.6  1992/12/29  13:02:39  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:24  zeliff]
 * 
 * Revision 1.1.6.5  1992/12/22  16:27:49  burati
 * 	CR6553 Get obj uuid correctly in bind_from_tower and bind_to_host
 * 	[1992/12/21  22:58:26  burati]
 * 
 * Revision 1.1.6.4  1992/12/18  23:01:18  rps
 * 	[CR6502] Redo cell_bind interface backward compatibility.
 * 	[1992/12/18  20:32:31  rps]
 * 
 * Revision 1.1.6.3  1992/12/16  22:53:33  sommerfe
 * 	[CR6502] Workaround: dike out attempt to look up rs_bind
 * 	[1992/12/16  21:20:37  sommerfe]
 * 
 * Revision 1.1.6.2  1992/12/08  19:00:24  rps
 * 	Add tower support.
 * 	Add replication update support.
 * 	Fix string binding.
 * 	Random cleanup.
 * 	[CR5601] in rca_inq_cell_name, don't assume failed rpc_mgmt call
 * 	zeros output parameter
 * 	OT4043 Need to rebind on more errors than just comm_failure
 * 	[1992/12/03  01:29:36  rps]
 * 
 * Revision 1.1.3.4  1992/06/18  21:18:48  sommerfeld
 * 	Always set the cell name in the handle, even if we're not authenticated.
 * 	Avoids null deference on PMAX.
 * 	[1992/06/18  18:30:59  sommerfeld]
 * 
 * 	Bugfix to last change: don't zero if_spec after we initialize it.
 * 	[1992/06/15  23:02:00  sommerfeld]
 * 
 * 	Make sure we properly initialize and dispose of new fields in the context.
 * 	Clean up misc. loose ends.
 * 	[1992/06/15  19:39:47  sommerfeld]
 * 
 * 	Allow rebinding from pe_site file (tested).
 * 	NB: If the first site on the list is catatonic, it typically gets timed
 * 	out *twice* (sigh) because there's no knowledge shared between
 * 	binding cache entries.
 * 	[1992/06/12  21:49:09  sommerfeld]
 * 
 * 	Now mostly works.
 * 	Massive rewrite; clean up much code.
 * 	Use the same code on initial and rebind paths.
 * 	Eliminate use of ..._len fields (just use rpc_stralloc to copy strings).
 * 	Cleanup debugging printfs
 * 	Add some more SEC_DBG_GPRINTF around rebinding.
 * 	[1992/06/10  22:44:59  sommerfeld]
 * 
 * 	Fix bug in previous change with respect to caching: delete "else"
 * 	clause when filling in cell_name.
 * 	[1992/06/09  20:10:19  sommerfeld]
 * 
 * Revision 1.1.3.3  1992/05/22  15:15:05  sommerfeld
 * 	Fix uninitialized status code.
 * 	[1992/05/21  23:55:49  sommerfeld]
 * 
 * 	Fix inter-cell binding (delay principal name expansion until later,
 * 	and allow sloppy principal name matching).
 * 	Fix some gcc compiler warnings.
 * 	Enable binding cache debug if DEBUG_SEC is > 1
 * 	[1992/05/21  21:19:28  sommerfeld]
 * 
 * Revision 1.1.3.2  1992/05/08  20:54:40  sommerfeld
 * 	Return the Tom G. Memorial status code when appropriate.
 * 	[1992/05/08  20:06:47  sommerfeld]
 * 
 * 	Add binding cache.
 * 	[1992/04/29  03:07:38  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:47:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  internal_binding.c
**
** Copyright (c) Hewlett-Packard Company 1991,1992,1993, 1995
** Unpublished work. All Rights Reserved.
**
*/
/* 
**      Passwd Etc - internal binding context management
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)internal_binding.c	33 - 12/19/91";
#endif

#define _rca_ALLOCATE_GLOBALS_

#include <un_maloc.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include <dce/dce.h>    
    
#include <sys/file.h>
#include <binding.h>
#include <rs_bind.h>
#include <secidmap.h>
#include <rsecidmap.h>
#include <rca_pvt.h>

#include <un_io.h>
#include <dce/dce_cf.h>
#include <u_str.h>
    
#include <sec_svc.h>
#include <sec_bind_util.h>
#include <sec_login_util.h>

#include <dce/exc_handling.h>
EXCEPTION rca_e_auth_tkt_expired;		/* fatal auth error */

#define Crs_bind_get_update_site(H,b,c,d,e,f,g)\
        (*rs_bind_v2_0_c_epv.rs_bind_get_update_site) (H,b,c,d,e,f,g)

static boolean32 rca_use_nsi = true;

#define SEC_CACHE_DEBUG 1
    
static pthread_once_t   def_con_mutex_once = pthread_once_init;
static boolean32        def_con_mutex_inited = false;
static pthread_mutex_t  def_con_mutex;

static pthread_once_t   cache_mutex_once = pthread_once_init;
static boolean32        cache_mutex_inited = false;
static pthread_mutex_t  cache_mutex;

static unsigned32	binding_clock = 0;
int rca_cache_lookups = 0;
int rca_cache_hits = 0;
int rca_cache_enters = 0;
int rca_cache_removes = 0;

#if SEC_CACHE_DEBUG
int rca_binding_cache_trace = 0;
#define SEC_DBG(n) (rca_binding_cache_trace >= (n))
#define SEC_DBG_GPRINTF(x) do {if (SEC_DBG(1)) printf x; } while(0)
#define SEC_DBG_PRINTF(n,x) do {if (SEC_DBG(n)) printf x; } while(0)
#else
#define SEC_DBG_PRINTF(n,x)
#define SEC_DBG_GPRINTF(x)
#endif

/* 
 * max number of unreferenced active binding handles..
 */
int rca_max_entries = 4;

/*
 * Since RPC only provides constants for 1sec min timeout (0) and
 * 30sec default timeout (5), we have to define our own for 4secs.
 */
#define RPC_C_BINDING_4SEC_TIMEOUT 2

static rca_pvt_handle_blk_t *cache_head;

#ifdef IDL_PROTOTYPES
#define PROTOTYPE(x) x
#else
#define PROTOTYPE(x) ()
#endif

PRIVATE boolean32 rca_get_site_bindings
    PROTOTYPE ((
	rca_pvt_handle_blk_t *context, 
	error_status_t *status
    ));

PRIVATE boolean32 rca_inq_cell_name 
    PROTOTYPE ((
	rca_pvt_handle_blk_t *context, 
	error_status_t *status
    ));

PRIVATE boolean32 rca_site_bind_nsi
    PROTOTYPE ((
	rca_pvt_handle_blk_t *context,
	error_status_t *status
    ));

PRIVATE boolean32 rca_site_bind_nsi_cell
    PROTOTYPE ((
	rca_pvt_handle_blk_t *context,
	sec_rgy_name_t          cell_name,
	error_status_t *status
    ));

PRIVATE boolean32 rca_site_bind_pe_site
    PROTOTYPE ((
	rca_pvt_handle_blk_t *context,
	error_status_t *status
    ));

PRIVATE boolean32 rca_bind_to_host
    PROTOTYPE ((
	unsigned char *name,
	rpc_binding_handle_t *handle,
	error_status_t *status
    ));

/*
 * Set up the RPC handle appropriately to make outgoing calls
 * authenticated as desired.
 */

PRIVATE boolean32 rca_setup_handle
    PROTOTYPE ((
	rca_pvt_handle_blk_t *context,
	error_status_t *status
    ));
    
PRIVATE void rca_create_cache_mutex
    PROTOTYPE ((
	void
    ));

PRIVATE void binding_cache_enter 
    PROTOTYPE ((
	rca_pvt_handle_blk_t *new_context
    ));

PRIVATE void binding_cache_remove
    PROTOTYPE ((
	rca_pvt_handle_blk_t *context
    ));

PUBLIC void rca_binding_cache_purge
    PROTOTYPE ((
	sec_login_handle_t *lhp
    ));

PRIVATE boolean32 binding_cache_lookup
    PROTOTYPE ((
        /* CHFts15769: add test for match of what would be the "requested_site_name"
         * of the requested binding with the requested_site_name of the cached
         * binding.
          */
        sec_rgy_name_t    req_site,  /* in */
        /* CHFts15769: end */
	unsigned_char_p_t site_name, /* in */
	sec_rgy_bind_auth_info_t *auth_info, /* in */
	unsigned char *princ,		/* in */
	unsigned char *cell,		/* in */
	rca_pvt_handle_blk_t    **context /* out */
    ));

/*
 * r c a _ c r e a t e _ c a c h e _ m u t e x
 */
PRIVATE void rca_create_cache_mutex()
{
   if (!cache_mutex_inited) {
        pthread_mutex_init(&cache_mutex, pthread_mutexattr_default);
        cache_mutex_inited = true;
    }
}

PRIVATE void binding_cache_enter (new_context)
    rca_pvt_handle_blk_t *new_context;
{
    assert(new_context->cache_next == NULL);
    assert(new_context->cache_prev == NULL);
    if (!cache_mutex_inited) {
	pthread_once(&cache_mutex_once, rca_create_cache_mutex);
    }
    P(cache_mutex);
    /* find entries to evict */
    rca_binding_cache_purge(NULL);

    SEC_DBG_PRINTF(3,("(binding_cache_enter) Adding %x to binding cache: site %s cell %s princ %s type %d\n", new_context,
	    new_context->site_name, new_context->cell_name, 
	    new_context->princ_name, new_context->auth_info.info_type
	));

    if (cache_head == NULL)
	cache_head = new_context;
    else
    {
	new_context->cache_next = cache_head;
	cache_head->cache_prev = new_context;
	cache_head = new_context;
    }
    new_context->refcount++;
    rca_cache_enters++;
    
    V(cache_mutex);
}

/* 
 * Remove context from cache.
 * Must be called with cache mutex locked.
 */

PRIVATE void binding_cache_remove (context)
    rca_pvt_handle_blk_t *context;    
{
    error_status_t lst;
    rca_pvt_handle_blk_t *next, *prev;
    
    assert (context->refcount == 1);
    assert (cache_head != NULL);
    
    next = context->cache_next;
    prev = context->cache_prev;
    
    if (cache_head == context) 
    {
	assert (prev == NULL);
	if (next)
	{
	    assert (next->cache_prev == context);
	    next->cache_prev = NULL;
	}
	cache_head = next;
    } 
    else
    {
	/* in middle of list; we must have a previous pointer */
	assert (prev != NULL);
	assert (prev->cache_next == context);
	prev->cache_next = next;
	if (next)
	{
	    assert (next->cache_prev == context);
	    next->cache_prev = prev;
	}
    }
    context->cache_prev = NULL;
    context->cache_next = NULL;
    sec_rgy_site_close(context, &lst);

    SEC_DBG_PRINTF(3,("Removed %x from binding cache\n", context));

    rca_cache_removes++;
}


/*
 * Remove excess "stale" entries from the binding cache.
 * Must be called with default context mutex locked.
 */

PUBLIC void rca_binding_cache_purge (lhp)
    sec_login_handle_t *lhp;
{
    int num_entries = 0;
    rca_pvt_handle_blk_t *oldest;
    unsigned32 oldest_time = ~0;
    rca_pvt_handle_blk_t *cur;
    
    if (!cache_head)
	return;
    
    if (lhp != NULL)
    {

	SEC_DBG_PRINTF(3,("flushing dead login handle %x\n", lhp));

	do 
	{
	    oldest = NULL;
	    for (cur = cache_head; cur != NULL; cur = cur->cache_next) 
	    {
		num_entries++;
		if ((cur->refcount == 1) &&
		    (cur->auth_info.info_type == sec_rgy_bind_auth_dce) &&
	            (cur->auth_info.tagged_union.dce_info.identity == lhp))
		{	
		    oldest = cur;
		    break;
		}
	    }
	    if (oldest) 
		binding_cache_remove(oldest);
	} while (oldest != NULL);
    }
    
    do 
    {
	oldest = NULL;

	SEC_DBG_PRINTF(3,("Purging binding cache\n"));

	num_entries = 0;
	for (cur = cache_head; cur != NULL; cur = cur->cache_next) 
	{
	    num_entries++;
	    if ((cur->refcount == 1) &&
		(oldest_time > cur->last_used))
	    {
		oldest_time = cur->last_used;
		oldest = cur;
	    } 
	}
	if ((oldest != NULL) && (num_entries > rca_max_entries))
	{
	    num_entries--;	    
	    binding_cache_remove(oldest);
	}
    } while ((num_entries > rca_max_entries) && (oldest != NULL));
}

/*
 * Look for preexisting binding handle matching the given parameters;
 * if found, return it, held, in *context, and return TRUE; otherwise
 * return FALSE.
 * 
 * The cache code doesn't check interface handles because a binding handle
 * refers to only one site, even with replication.
 * CHFts15769: add test for match of what would be the "requested_site_name"
 * of the requested binding with the requested_site_name of the cached
 * binding.
 */

PRIVATE boolean32 binding_cache_lookup (req_site, site_name, auth_info, princ, cell, context)
    sec_rgy_name_t    req_site;
/* CHFts15769: end */
    unsigned_char_p_t site_name;
    sec_rgy_bind_auth_info_t *auth_info;
    unsigned char *princ;
    unsigned char *cell;
    rca_pvt_handle_blk_t **context;
{
    rca_pvt_handle_blk_t *cur;
    if (!cache_mutex_inited) {
	pthread_once(&cache_mutex_once, rca_create_cache_mutex);
    }
    P(cache_mutex);

    /* CHFts15769: add test for match of what would be the "requested_site_name"
     * of the requested binding with the requested_site_name of the cached
     * binding.
     */
    SEC_DBG_PRINTF(2,("(binding_cache_lookup) Looking for site `%s', cell `%s', princ `%s', type %d, req. site `%s'\n",
	    site_name, cell, princ, auth_info->info_type, req_site));
    
    for (cur = cache_head; cur != NULL; cur = cur->cache_next) 
    {
	SEC_DBG_PRINTF(3,("(binding_cache_lookup) Looking at %x: site `%s', cell `%s', princ `%s', %d, req. site `%s'\n", cur,
		cur->site_name, cur->cell_name, cur->princ_name, 
		cur->auth_info.info_type, cur->requested_site_name));

        if (u_strcmp (cur->requested_site_name, req_site) != 0)
        {
	    SEC_DBG_PRINTF(3,("Requested site mismatch\n"));
	    continue;
        }
        /* The following tests for site_name and cell may appear to be redundant
         * now that the preceding test checks for a match on "requested_site_name",
         * but they make a difference in some uses of binding_cache_lookup for
         * reasons not yet understood.  For example, with the following tests 
         * removed, dcecp gets an invalid network address error the first time
         * it attempts a registry operation (if done with no credentials).
         * CHFts15769: end */
	if (site_name && site_name[0])
	{
	    if ((u_strcmp (cur->site_name, site_name) != 0) &&
		(u_strcmp (cur->cell_name, site_name) != 0))
	    {
		SEC_DBG_PRINTF(3,("Site mismatch\n"));
		continue;
	    }	
	}	
	if (cell && cell[0]) 
	{
	    if(u_strcmp (cur->cell_name, cell) != 0)
	    {
		SEC_DBG_PRINTF(3,("Cell name mismatch\n"));
		continue;
	    }
	}
	switch (auth_info->info_type) 
	{

	case sec_rgy_bind_auth_none:
	    if (cur->auth_info.info_type != sec_rgy_bind_auth_none)
	    {
		SEC_DBG_PRINTF(3,("info_type mismatch, none expected\n"));
		continue;
	    }
	    break;
	case sec_rgy_bind_auth_dce:
	    if (cur->auth_info.info_type != sec_rgy_bind_auth_dce)
	    {
		SEC_DBG_PRINTF(3,("info_type mismatch, dce expected\n"));
		continue;
	    }
	    /* if princ isn't set, we don't care */
	    if (princ && princ[0]) 
	    {
		if ((u_strcmp (cur->princ_name, princ) != 0) &&
		    (u_strcmp (cur->uncanon_princ_name, princ) != 0))
		{
		    SEC_DBG_PRINTF(3,("Princ name mismatch: %x(%s) or %s vs %x(%s)\n",
			cur->princ_name, cur->princ_name,
			cur->uncanon_princ_name,
			princ, princ));
		    continue;
		}
	    }
	    if (cur->auth_info.tagged_union.dce_info.authn_level 
		!= auth_info->tagged_union.dce_info.authn_level)
	    {
		SEC_DBG_PRINTF(3,("authn_level mismatch\n"));
		continue;
	    }
	    if (cur->auth_info.tagged_union.dce_info.authn_svc
		!= auth_info->tagged_union.dce_info.authn_svc)
	    {
		SEC_DBG_PRINTF(3,("authn_svc mismatch\n"));
		continue;
	    }
	    if (cur->auth_info.tagged_union.dce_info.authz_svc
		!= auth_info->tagged_union.dce_info.authz_svc)
	    {
		SEC_DBG_PRINTF(3,("authz_svc mismatch\n"));
		continue;
	    }
	    if (cur->auth_info.tagged_union.dce_info.identity
		!= auth_info->tagged_union.dce_info.identity)
	    {
		SEC_DBG_PRINTF(3,("identity mismatch: %x vs %x\n",
			cur->auth_info.tagged_union.dce_info.identity,
			auth_info->tagged_union.dce_info.identity
		    ));
		continue;
	    }
	    break;
	}
	SEC_DBG_PRINTF(2,("(binding_cache_lookup) Cache hit: returning %x\n", cur));
	cur->refcount++;
	*context = cur;
	V(cache_mutex);
	return true;
    }
    V(cache_mutex);    

    SEC_DBG_PRINTF(2,("(binding_cache_lookup) Cache miss\n"));
    return false;
    
}

    
 /*
  * r c a _ g e t _ s i t e _ b i n d i n g s
  *
  * Obtain the default site name.  In the current implementation a local
  * configuration file is used.  This facility is used as a backstop in the
  * event the naming system was unavailable.
  */
PRIVATE  boolean32    rca_get_site_bindings (context, status)
    rca_pvt_handle_blk_t *context;
    error_status_t *status;
{
    FILE            *fp;
    unsigned char *new_name;
    boolean32       canonical = false, bound = false, any_bound = false;
    unsigned_char_t *exp_name;
    sec_rgy_pname_t site_name;
    error_status_t  st, xst;
    int i;
    
    CLEAR_STATUS(status);

    u_strcpy(site_name, context->requested_site_name);
    
    if (*site_name != '\0'
	    && u_strncmp(site_name, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN) != 0
	    && u_strncmp(site_name, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) != 0) {
	SET_STATUS(status, sec_rgy_site_not_absolute);
	return false;
    }

    if (*site_name == '\0'
            || (u_strncmp(site_name, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN-1) == 0
              && (site_name[CELL_DIR_ROOT_LEN-1] == '\0' 
                  || site_name[CELL_DIR_ROOT_LEN-1] == PATH_COMPONENT_SEPARATOR)) 
            ) {
        rca_get_cell_name(&(context->auth_info),site_name);
        if (*site_name == '\0') {
            return false;
        }                 
        canonical = true;
    }

    for (i=0; i<RCA_MAX_HANDLES; i++) 
    {
	if (context->alt_handles[i] != NULL) 
	{
	    rpc_binding_handle_t h = context->alt_handles[i];
	    context->alt_handles[i] = NULL;
	    rpc_binding_free(&h, &xst);
	}
    }
    SEC_DBG_GPRINTF(("(rca_get_site_bindings) about to open config file\n"));    
    fp = fopen(RGY_CONFIG_FILE, "r");
    if (fp != NULL) {
	i=0;
	while ((!feof(fp)) && (i < RCA_MAX_HANDLES)) 
	{
	    dce_cf_find_name_by_key(fp, (char *) site_name, (char **)&new_name, &st);
	    if (!STATUS_EQUAL(&st, dce_cf_st_ok) && !canonical && !any_bound) {
		/* 
		 * we might have failed because the user input
		 * wasn't canonical, so canonicalize it and try again
		 */
		rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, site_name,
		    &exp_name,  &xst);
		if (GOOD_STATUS(&xst)) {
		    /* reset the file pointer */ 
		    SEC_DBG_GPRINTF(("(rca_get_site_bindings) Trying again with canonical name %s\n", exp_name));		    
		    canonical = true;
		    fseek(fp, 0, SEEK_SET);
		    u_strcpy(site_name, exp_name);
		    rpc_string_free(&exp_name, &xst);
		    continue;	/* try again.. */
		}
	    }
	    if (st == dce_cf_st_ok) 
	    {
		/* !!! return status codes? */
		SEC_DBG_GPRINTF(("(rca_get_site_bindings) Got binding %s\n", new_name));
		bound = rca_bind_to_host (new_name, &context->alt_handles[i], &xst);
		if (bound) 
		{
		    SEC_DBG_GPRINTF(("(rca_get_site_bindings) bind_to_host succeeded\n"));
		    any_bound = true;
		    i++;
		}
		free(new_name);
	    }
	}
        fclose(fp);
    }
    context->hindex = 0;
    return any_bound;
}


 /*
  * r c a _ g e t _ c e l l _ n a m e 
  *
  * Obtain the local cell name.
  */
PUBLIC  void    rca_get_cell_name (
      sec_rgy_bind_auth_info_t *auth_info,
      sec_rgy_pname_t     cell_name
)
{
    error_status_t   st;
    char            *new_name;

    if (auth_info->info_type == sec_rgy_bind_auth_dce)  
    {
	char *cellp;
	sec_login_handle_t lh = auth_info->tagged_union.dce_info.identity;

	cellp = (char *)sec_login_util_get_cell(lh);
	if (cellp)
	{
	    u_strcpy(cell_name, cellp);
	    return;
	}
    }
    dce_cf_get_cell_name(&new_name, &st);
    if (st == dce_cf_st_ok) {
        strcpy((char *) cell_name, new_name);
        free(new_name);
    } else {
        cell_name[0] = '\0';
        return;
    }
    
}


/*
 * r c a _ g e t _ o b j _ i d
 * get the cell's security object id by lookup in the endpoint map
 */
PRIVATE void rca_get_obj_id (
    rpc_binding_handle_t        handle,
    uuid_t                      *obj_idP,
    error_status_t              *status
)    
{
    rpc_if_id_t         if_id;
    rpc_ep_inq_handle_t ep_inquire_handle;
    error_status_t      lst;

    rpc_if_inq_id(
	RCA_DEFAULT_SECIF,
	&if_id, 
	status);
    if (BAD_STATUS(status))
	return;

    rpc_mgmt_ep_elt_inq_begin(
	handle,
	rpc_c_ep_match_by_if,
	&if_id,
	rpc_c_vers_all,
	NULL,
	&ep_inquire_handle,
	status);
    if (BAD_STATUS(status))
	return;

    rpc_mgmt_ep_elt_inq_next(
	ep_inquire_handle,
	&if_id,
	NULL,
	obj_idP,
	NULL,
	status);
    if (BAD_STATUS(status))
	return;

    rpc_mgmt_ep_elt_inq_done(
	&ep_inquire_handle,
	&lst);
}


/*
  * r c a _ b i n d _ t o _ h o s t
  *
  * Attempt to get a handle via the specified string binding.
  */

PRIVATE boolean32 rca_bind_to_host (name, handle, status)
    unsigned char *name;
    rpc_binding_handle_t * handle;
    error_status_t *status;
{
    unsigned_char_p_t   str_obj             = NULL;
    unsigned_char_p_t   str_protseq         = NULL;
    unsigned_char_p_t   str_netaddr         = NULL;
    unsigned_char_p_t   str_endpoint        = NULL;
    unsigned_char_p_t   str_options         = NULL;
    unsigned_char_p_t   str_binding         = NULL;
    unsigned_char_p_t   str_binding_real    = name;
    unsigned_char_p_t   str_protseq_real;
    unsigned_char_p_t   str_endpoint_real;
    boolean32           bound               = false;
    error_status_t xst;
    uuid_t                  obj_uuid;
    
    /* Don't even attempt to use null pointer */
    if (! name) 
    {
	SEC_DBG_GPRINTF(("(rca_bind_to_host) Null string binding\n"));
        return false;
    }

    rpc_string_binding_parse(name, &str_obj, &str_protseq,
                                &str_netaddr, &str_endpoint,
                                &str_options, status);
    if (BAD_STATUS(status))
	goto exit_free;

    if (GOOD_STATUS(status)) {
	/* !!! if *handle is non-null, free it.. */
        /*
         * If a simple name was used (unadorned with the proper protocol
         * sequence) we try a default sequence. 
         */
        if (NULL_STRING(str_protseq)) {
            str_protseq_real = (unsigned_char_p_t) "ip";
        } else {
            str_protseq_real = str_protseq;
        }

        if (NULL_STRING(str_endpoint)) {
            str_endpoint_real = (unsigned_char_p_t) "";
        } else {
            str_endpoint_real = str_endpoint;
        }

        rpc_string_binding_compose(str_obj, str_protseq_real, str_netaddr,
                                    str_endpoint_real, str_options,
                                    &str_binding, status);
        if (GOOD_STATUS(status)) {
            str_binding_real = str_binding;
        }

        rpc_binding_from_string_binding(str_binding_real, 
	    handle, status);

        if (GOOD_STATUS(status) && NULL_STRING(str_obj)) {
            SEC_DBG_GPRINTF(("(rca_bind_to_host) Null object uuid in string binding\n"));	
            rca_get_obj_id(*handle, &obj_uuid, status);
	    if (BAD_STATUS(status))
	        goto exit_free;

            rpc_binding_set_object(*handle, &obj_uuid, status);
            if (BAD_STATUS(status))
	        goto exit_free;
        }

        if (GOOD_STATUS(status)) 
	{
            SEC_DBG_GPRINTF(("(rca_bind_to_host) Successful bind.\n"));
	    bound = true;
	}

    }

exit_free:
    /*
     * Free the strings that may have been allocated by the RPC runtime
     */
    if (str_obj != NULL) {
        rpc_string_free(&str_obj, &xst);
    }
    if (str_protseq != NULL) {
        rpc_string_free(&str_protseq, &xst);
    }
    if (str_netaddr != NULL) {
        rpc_string_free(&str_netaddr, &xst);
    }
    if (str_endpoint != NULL) {
        rpc_string_free(&str_endpoint, &xst);
    }
    if (str_options != NULL) {
        rpc_string_free(&str_options, &xst);
    }
    if (str_binding != NULL) {
        rpc_string_free(&str_binding, &xst);
    }
    return bound;
}


/*
 * r c a _ c r e a t e _ d e f _ c o n _ m u t e x
 */
PRIVATE void rca_create_def_con_mutex (void)
{
   if (!def_con_mutex_inited) {
        pthread_mutex_init(&def_con_mutex, pthread_mutexattr_default);
        def_con_mutex_inited = true;
    }
}

/*
 * r c a _ c r e a t e _ d e f a u l t _ c o n t e x t
 * 
 * Assume global lock
 */
PRIVATE void rca_create_default_context (
    error_status_t  *stp
)
{
    sec_rgy_bind_auth_info_t    auth_info;

    auth_info.info_type = sec_rgy_bind_auth_dce;
    auth_info.tagged_union.dce_info.authn_level =
                                        rpc_c_protect_level_pkt_integ;
    auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
    auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
    auth_info.tagged_union.dce_info.identity  = NULL;

    rca_site_bind((unsigned_char_p_t) "", &auth_info, rca_NSI_cell,
	NULL, SEC_RGY_SERVER_NAME,
	"", (sec_rgy_handle_t *)&rca_default_context_block_p, stp);

    if (STATUS_OK(stp)) {
        /*
         * This handle should be marked as rebindable
         */
        SET(HANDLE_FLAGS(rca_default_context_block_p),
                    RCA_HANDLE_FLAGS_REBINDABLE);
	/* 
	 * bump the reference one more to prevent it from being gc'ed 
	 */
	rca_default_context_block_p->refcount++;
    } else {
       rca_default_context_block_p = NULL;
    }
}


/*
 * !!! this really should bump the refcount on the handle, but that
 * would require changing all callers to do a "site_close" afterwards..
 */

PUBLIC void rca_expand_default_handle ( context_p, status ) 
    sec_rgy_handle_t    *context_p;
    error_status_t      *status;
{
    rca_pvt_handle_blk_t *context;
    
    CLEAR_STATUS(status);
    
    context = (rca_pvt_handle_blk_t *)*context_p;
    if (context != sec_rgy_default_handle) 
	return;

    /* If default context doesn't exist, then create one */

    if (rca_default_context_block_p == NULL) 
    {
	if (!def_con_mutex_inited)
	    pthread_once(&def_con_mutex_once, rca_create_def_con_mutex);
	
	P(def_con_mutex);
	
	if (rca_default_context_block_p == NULL)
	    rca_create_default_context(status);
	
	V(def_con_mutex);
    }

    if (STATUS_OK(status)) 
	*context_p = (sec_rgy_handle_t) rca_default_context_block_p;
    else
	*context_p = NULL;
}

/*
 * r c a _ l o c k _ h a n d l e
 *
 *  Put a lock (via semaphore) on this sec_rgy_handle_t.
 *  If it's the default context, and that hasn't been created yet,
 *  then do so first.
 */
PUBLIC void rca_lock_handle ( context_p, status ) 
    sec_rgy_handle_t    *context_p;
    error_status_t      *status;
{
    rca_pvt_handle_blk_t *context;
    
    rca_expand_default_handle(context_p, status);

    context = (rca_pvt_handle_blk_t *)*context_p;
    SEC_DBG_PRINTF(5,("(rca_lock_handle) locking %x\n", *context));
    if (context != NULL) 
    {
	if (pthread_mutex_lock(&context->mutex)) {
	    SET_STATUS(status, sec_rgy_bad_handle);
	    return;
	}
	/* 
	 * reset "import count" for this transaction..
	 */
	context->import_count = 0;
    }
}

PUBLIC void rca_unlock_handle (context_p)
    sec_rgy_handle_t  context_p;
{
    rca_pvt_handle_blk_t *context = (rca_pvt_handle_blk_t *)context_p;
    SEC_DBG_PRINTF(5, ("(rca_unlock_handle) unlocking %x\n", context));    
    pthread_mutex_unlock (&context->mutex);
}

/*
 * r c a _ n s i _ b i n d i n g _ i m p o r t
 * 
 * (Re)initialize the NS import context, and import the first binding from it.
 */

PRIVATE boolean32 rca_nsi_binding_import (
    rca_pvt_handle_blk_t        *context,
    error_status_t *status
)
{
    boolean32               bound = false;
    error_status_t xst;
    boolean32               prev_authn;
    
    rpc_ns_set_authn(false, &prev_authn, status); 
    if (BAD_STATUS(status)) {
        return bound;
    }

    context->import_count++;

    if (context->import_handle) 
    {
	rpc_ns_binding_import_done(&context->import_handle, &xst);
	context->import_handle = NULL;
	UNSET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_IMPORT_H_VALID);
    }

    if (context->requested_site_name && rca_use_nsi && 
	FLAG_SET(context->import_flags, rca_NSI_name)) 
    {
        bound = rca_site_bind_nsi(context, status);
        if (bound) {
            if (BAD_STATUS(status)) {
                CLEAR_STATUS(status);
	    }
        }
    }

    if (!bound && rca_use_nsi && 
	FLAG_SET(context->import_flags, rca_NSI_cell)) {
        /*
         * If cell wasn't specified, use supplied data, or the local cell
         * as cell name
         */
	unsigned_char_p_t   cell_name = context->cell_name;

	/* 
	 * we copy this here because context->site_name may be freed before
	 * rca_site_bind_nsi_cell gets a chance to set the cell name.
	 */
	if (cell_name == NULL)
		cell_name = context->site_name ? context->site_name :
		    context->requested_site_name;
	cell_name = rpc_stralloc(cell_name);
	bound = rca_site_bind_nsi_cell(context, cell_name, status);

	if (!bound && STATUS_EQUAL(status, rpc_s_no_more_bindings)) {
	    unsigned32 exp_age;
	    /*
	     * Retry after flushing the directory cache
	     */
	    SEC_DBG_GPRINTF(("(rca_nsi_binding_import) attempting to re-import\n"));
	    rpc_ns_mgmt_inq_exp_age(&exp_age, status);
	    if (BAD_STATUS(status))
		exp_age = rpc_c_ns_default_exp_age;

	    rpc_ns_mgmt_set_exp_age(0, status);
	    if (BAD_STATUS(status)) {
		SEC_DBG_GPRINTF(("(rca_nsi_binding_import) rpc ns consistency failed -- status %x\n", *status));
	    } else {
		bound = rca_site_bind_nsi_cell(context, cell_name, status);
		rpc_ns_mgmt_set_exp_age(exp_age, &xst);
	    }
	}

        if (bound) 
	{
	    /* 
	     * we got a binding.
	     */
	    CLEAR_STATUS(status);
        }
	else
	{
	    rpc_string_free (&cell_name, &xst);
	}
    }

    if (!bound) {
	bound = rca_site_bind_pe_site(context, status);
	if (bound)
	    CLEAR_STATUS(status);
    }
    
    rpc_ns_set_authn(prev_authn, &prev_authn, &xst);
    
    return bound;
}


/*
 * r c a _ s e t u p _ h a n d l e
 * Set up the RPC handle after a new handle has been imported.
 */

PRIVATE boolean32 rca_setup_handle(context, status)
    rca_pvt_handle_blk_t *context;
    error_status_t *status;
{
    unsigned_char_p_t tmp_name_p;
    error_status_t st;
    sec_rgy_name_t  rgy_princ_name;
    sec_rgy_bind_auth_info_t    *auth_info=&context->auth_info;
    int len;
    
    CLEAR_STATUS(status);

    SEC_DBG_GPRINTF(("(rca_setup_handle) bind succeeded -- setting up handle\n"));
    SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID);

    /* Set timeout based on whether we're in Phase I or II of rebinding */
    if (FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND)) {
	rpc_mgmt_set_com_timeout(HANDLE(context),rpc_c_binding_default_timeout,
					status);
    } else {
	rpc_mgmt_set_com_timeout(HANDLE(context),RPC_C_BINDING_4SEC_TIMEOUT,
					status);
    }
    
    rpc_ns_binding_inq_entry_name(context->rpc_handle,
	rpc_c_ns_syntax_dce,
	&tmp_name_p,
	&st);
    
    SEC_DBG_GPRINTF(("(rca_setup_handle) req. site name is %s, status %x\n",
	context->requested_site_name, st));

    if (GOOD_STATUS(&st)) 
    {
	if (context->site_name)
	    DISPOSE(context->site_name);
	SEC_DBG_GPRINTF(("(rca_setup_handle) Replace entry name with %s\n", tmp_name_p));	
	context->site_name = tmp_name_p;
    }
    else if (context->requested_site_name != NULL) 
    {
	if (context->site_name)
	    DISPOSE(context->site_name);
	SEC_DBG_GPRINTF(("(rca_setup_handle) No entry name -- status %x.  Falling back to site == %s\n", st, context->requested_site_name));
	context->site_name = rpc_stralloc(context->requested_site_name);
    }    
    else if (context->cell_name != NULL)
    {
	/*
	 * We didn't get an entry name, so use the cell name
	 * as the server name.
	 */
	if (context->site_name)
	    DISPOSE(context->site_name);
	SEC_DBG_GPRINTF(("(rca_setup_handle) No entry name -- status %x.  Falling back to site == cell\n", st));
	context->site_name = rpc_stralloc(context->cell_name);
    }
    
    if ((context->cell_name == NULL) || (context->cell_name[0] == '\0'))
    {
	if (!rca_inq_cell_name(context, status))  
	{
	    SEC_DBG_GPRINTF(("(rca_setup_handle) rca_inq_cell_name failed, status %x\n",
		*status));
	    return false;
	}
    }

    /*
     * If we're not trying to authenticate, we're done.
     */
    if (auth_info->info_type != sec_rgy_bind_auth_dce) 
	return true;
    
    /*
     * Set the auth info on the new handle 
     * 
     * Use the kerberos "princ@realm" name format to avoid
     * infinite recursion through sec_id_parse_name in the rpc runtime.
     */
    if (context->princ_name == NULL) 
    {
	u_strcpy(rgy_princ_name, context->uncanon_princ_name);
	if (context->cell_name) {
	    unsigned_char_p_t  ucp;

	    u_strcat(rgy_princ_name, "@");
	    ucp = (context->cell_name + GLOBAL_DIR_ROOT_LEN); /* !!! barf */
	    u_strcat(rgy_princ_name, ucp);    
	}
	/* 
	 * stash the principal name in the context for future 
	 * reference.
	 */
	len = u_strlen(rgy_princ_name);	
	context->princ_name = (unsigned_char_p_t) malloc(1 + len);
	if (context->princ_name == NULL) 
	{
	    SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    return false;
	}
	u_strcpy(context->princ_name, rgy_princ_name);
    }
  
    SEC_DBG_GPRINTF(("(rca_setup_handle) Auth DCE, principal '%s'\n", 
	context->princ_name));
	    
    SEC_DBG_PRINTF(2,("(rca_setup_handle) rpc_binding_set_auth_info, identity %x, handle %x\n",
	auth_info->tagged_union.dce_info.identity, context->rpc_handle));
    rpc_binding_set_auth_info(context->rpc_handle,
	context->princ_name,
	auth_info->tagged_union.dce_info.authn_level,
	auth_info->tagged_union.dce_info.authn_svc,
	auth_info->tagged_union.dce_info.identity,
	auth_info->tagged_union.dce_info.authz_svc,
	status);  
    SEC_DBG_PRINTF(2,("(rca_setup_handle) end set_auth_info identity %x\n",
	auth_info->tagged_union.dce_info.identity));
    
    if (BAD_STATUS(status)) {
	SEC_DBG_GPRINTF(("(rca_setup_handle) rpc_binding_set_auth_info returned status %x\n", *status));
	if (STATUS_EQUAL(status, rpc_s_auth_tkt_expired))
	    RAISE(rca_e_auth_tkt_expired);
	return false;
    }
	
    return true;
}


/*
 * r c a _ r e b i n d _ t o w e r
 */
PRIVATE void rca_rebind_tower (
    rca_pvt_handle_blk_t        *context,
    error_status_t		*status
)
{
    unsigned32			start, wrap, i;
    error_status_t		xst;
    uuid_t			obj_uuid;
    sec_rgy_bind_auth_info_t    *auth_info = &context->auth_info;
	
    CLEAR_STATUS(status);
    context->rpc_handle = NULL;

    start = context->tower_vec_index;
    wrap = context->tower_vec->num_towers;

    for (i=0; i < wrap; i++) {

	int index = context->tower_vec_index = (start+i) % wrap;
	twr_p_t tower = context->tower_vec->towers[index];
	if (tower == NULL)
	    continue;

	/* Clean up prior loop failure */
	if (context->rpc_handle)
	    rpc_binding_free(&context->rpc_handle, &xst);

	rpc_tower_to_binding(tower->tower_octet_string,
				&context->rpc_handle, status);
	if (BAD_STATUS(status))
	    continue;

	/*
	 * Set the auth info on the new handle 
	 * [common code above; pasted for simplicity]
	 * 
	 * Use the kerberos "princ@realm" name format to avoid
	 * infinite recursion through sec_id_parse_name in the rpc runtime.
	 */
	if (context->princ_name == NULL) {
	    sec_rgy_name_t  rgy_princ_name;
	    int len;

	    u_strcpy(rgy_princ_name, SEC_RGY_SERVER_NAME);
	    if (context->cell_name && *context->cell_name) {
		unsigned_char_p_t  ucp;

		u_strcat(rgy_princ_name, "@");
		ucp = (context->cell_name + GLOBAL_DIR_ROOT_LEN);
		u_strcat(rgy_princ_name, ucp);
	    }
	    /*
	     * stash the principal name in the context for future
	     * reference.
	     */
	    len = u_strlen(rgy_princ_name);
	    context->princ_name = (unsigned_char_p_t) malloc(1 + len);
	    if (context->princ_name == NULL) {
		SET_STATUS(status, sec_rgy_cant_allocate_memory);
		return;
	    }
	    u_strcpy(context->princ_name, rgy_princ_name);
	}

	if (auth_info->info_type == sec_rgy_bind_auth_dce) {
	    rpc_binding_set_auth_info(context->rpc_handle,
				context->princ_name,
				auth_info->tagged_union.dce_info.authn_level,
				auth_info->tagged_union.dce_info.authn_svc,
				auth_info->tagged_union.dce_info.identity,
				auth_info->tagged_union.dce_info.authz_svc,
				status);  
	    if (BAD_STATUS(status)) {
		rpc_binding_free(&context->rpc_handle, &xst);
		if (STATUS_EQUAL(status, rpc_s_auth_tkt_expired))
		    RAISE(rca_e_auth_tkt_expired);
		continue;
	    }
	}

	rca_get_obj_id(context->rpc_handle, &obj_uuid, status);
	if (BAD_STATUS(status)) {
	    continue;
	}

	rpc_binding_set_object(context->rpc_handle, &obj_uuid, status);
	if (BAD_STATUS(status)) {
	    continue;
	}

	SET(HANDLE_FLAGS(context),
		RCA_HANDLE_FLAGS_VALID | RCA_HANDLE_FLAGS_REBINDABLE);
	return;
    }

    /* We ran out of towers to try */
    if (GOOD_STATUS(status))
	SET_STATUS(status, sec_rgy_tower_rebind_failed);

    if (context->rpc_handle)
	rpc_binding_free(&context->rpc_handle, &xst);
}


/*
 * r c a _ r e b i n d
 * 
 */
	
PUBLIC boolean32 rca_rebind (
    rca_pvt_handle_blk_t        *context,
    error_status_t *status
)
{
    error_status_t xst;
    int i;
    rpc_binding_handle_t h = NULL ;
    unsigned char *msgp = NULL ;
    boolean32 alt_handle_found = false;

    /* 
     * !!! should free up context->rpc_handle here
     */

    /*
     * Try towers if they've been specified. 
     */
    if (context->tower_vec != NULL) {
	rca_rebind_tower(context, status);
	return GOOD_STATUS(status) ? true : false;
    }

    /* 
     * If this is a context handle to the update site, give up
     * now. Otherwise we will end up binding to a random server 
     * below.
     *
     * NOTE: This needs to be modified to handle the case where
     * the master changes. Currently any client with a binding
     * handle to the master is hosed when a change master is
     * performed.
     * Changes should use the alt_handles and/or the NSI to try
     * and find a responding replica that can tell us the location
     * of the new master.
     */
    if (FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_SITE_UPDATEABLE)) {
	SET_STATUS(status, sec_rgy_server_unavailable);
        return false;
    }

    /* 
     * Fall back on additional handles, if any..
     */

    for (;;)
    {
	/* bail out if maximum number of handles have been tried */
	if (context->hindex >= RCA_MAX_HANDLES) {
            SEC_DBG_GPRINTF(("(rca_rebind) Maximum number of binding handles exceeded\n"));
	    SET_STATUS(status, sec_rgy_server_unavailable);
	    return false;
        }
	for (i = context->hindex; i<RCA_MAX_HANDLES; i++) 
	{
	    h = context->alt_handles[i];
	    
	    if (h != NULL) 
	    {
		context->alt_handles[i] = NULL;
		context->hindex = i+1;
	        alt_handle_found = true;
		break;
	    }
	}
	
	if (h == NULL)
	{
	    /* back to the drawing board.. */
	    if (!alt_handle_found && 
		!FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_IMPORT_H_VALID)) {
	        /*
        	 * Replenish the alt_handles by re-reading the pe_site file. 
                 * It's possible the binding was originally obtained via the
                 * pe_site and the pe_site has been updated since that time OR
                 * servers which were down have been brought back up. (During 
                 * initial DCE startup, there is a race condition between dced 
                 * and secd if secd has not registered its endpoints before dced
                 * tries to bind to secd. This will fix that.) This case is only
                 * tried if the alt_handle_found flag is false (which means that
                 * all alt_handles have been previously tried) and NSI is not 
                 * valid.
        	 */
	        boolean32 bound;
                /* 
                 * If we have gone through all the alt_handles, set the
                 * PATIENT_BINDING flag. This will prevent an infinite
                 * binding loop if security and CDS aren't available.
                 */
                SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND);
	        bound = rca_get_site_bindings(context, status);
	        if (bound)
		    continue;
	    }
	    break;
	}
	if (h != NULL) 
	{
	    boolean32 bound;
	    SEC_DBG_GPRINTF(("(rca_rebind) Took alt_handle %d from context\n", i));
	    context->rpc_handle = h;
	    SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID);
	    bound = rca_setup_handle(context, status);
	    if (bound) 
		return true;
	}
    }

    /* Attempt to (re)bind using NSI */
    if (FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_IMPORT_H_VALID)) {
	if (context->rpc_handle) 
	{
	    rpc_binding_free (&context->rpc_handle, &xst);
	    context->rpc_handle = NULL;
	}
	SEC_DBG_GPRINTF(("(rca_rebind) Attempting to rebind\n"));

	rpc_ns_binding_import_next(context->import_handle, 
	    &(context->rpc_handle), status);
	
	if (STATUS_EQUAL(status, rpc_s_no_more_bindings) &&
	    (context->import_count == 0)) 
	{
	    SEC_DBG_GPRINTF(("(rca_rebind) attempting to re-import\n"));
	    SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND);
	    rca_nsi_binding_import(context, status);
	}
	
	if (BAD_STATUS(status))
	{
	    boolean32 bound;
	    bound = rca_site_bind_pe_site(context, status);
	    if (bound)
		CLEAR_STATUS(status);
	}

	if (BAD_STATUS(status)) 
	{
            dce_svc_printf(SEC_RCA_OP_STATUS_MSG , context->site_name , msgp=dce_sprintf(*status));
            if (msgp) free(msgp) ;
	    SET_STATUS(status, sec_rgy_server_unavailable);
	    return false;
	}

	return rca_setup_handle(context, status);
    } else {
	/* Can't rebind without NSI yet.  Returning false will allow
	 * caller to terminate loop, and resetting handle to valid will
	 * allow application to try again (maybe it was intermittent)
	 * without coming through here again right away.
         *
         * CHFts15509: added RCA_HANDLE_FLAGS_PATIENT_REBIND flag to
         * avoid inifinite loop when secd is down. The infinite loop
         * occurs in the RETRY_CASE macro. Previously, the PATIENT flag
         * was not set, so the retry counter was never decremented.
	 */
	SEC_DBG_GPRINTF(("(rca_rebind) Non-nsi rebind failed\n"));
	rpc_binding_reset (context->rpc_handle, &xst);
	SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID);
        SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND);
        dce_svc_printf(SEC_RCA_OP_STATUS_MSG , context->site_name , msgp=dce_sprintf(*status));
        if (msgp) free(msgp) ;
	SET_STATUS(status, sec_rgy_server_unavailable);
	return false;
    }
}

/* rca_check_binding_resolved
 *
 * Similar to rca_check_binding, but in addition
 * ensures that all DG binding handles are fully
 * resolved.  Will loop, decrementing retry count
 * until the handle selected is either a CN handle
 * or a fully bound DG handle, or the retry count
 * is exhausted.
 */ 
boolean32 rca_check_binding_resolved (
    sec_rgy_handle_t    *context_p,
    rca_operation_t     which,
    signed32            *retry_count,
    error_status_t      *status
)
{
    rca_pvt_handle_blk_t *context = (rca_pvt_handle_blk_t *) *context_p;
    boolean32            bound = false;

    while ((*retry_count > 0) && !bound) {
	if (bound = rca_check_binding(context_p, which, status)) {
	    /* 
	     * If the binding we've selected is a DG binding handle, we
	     * need to fully resolve it before use.  If we don't, the
	     * DG runtime will direct the next rpc to the endpoint map 
	     * forwarding mechanism, and if the interface in question
	     * isn't registered in the endpoint map, the client will
	     * get back a rpc_s_comm_failure status.  By fully resolving the
	     * binding here, we ensure that the next rpc will be sent
	     * directly to the server.   If the server has not registered
	     * the interface in question with the local rpc runtime, the
	     * the client will get an rpc_s_unknown_if error status and 
	     * can take whatever compatibility action is appropriate.
	     * We resolve the handle using the secidmap interface because
	     * every security server exports it.
             *
             * It is not necessary to resolve Connection-oriented
             * bindings because the CN runtime always returns
             * the appropriate status when an endpoint is not
	     * registered in the endpoint map--it never misleads
             * a client by returning rpc_s_comm_failure.  Resolving
             * CN bindings here using a generic interface would actually be 
	     * inefficient.  Better to perform the resolution later
             * using a specific interface.
	     */
	    bound = sec_bind_util_resolve_binding(HANDLE(context), 
					  RCA_DEFAULT_SECIF,
					  (unsigned_char_p_t) "ncadg_ip_udp",
					  status);
        }
        /* CHFts16482: infinite loop in rca_check_binding_resolved();
         * following block of code was nested one level too deeply
         */
        if (!bound) {
            unsigned char *msgp = NULL;

            dce_svc_printf(SEC_RCA_OP_STATUS_MSG , context->site_name , msgp=dce_sprintf(*status));
            if (msgp) free(msgp) ;
            UNSET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID);
            if (FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND)) 
                (*retry_count) -= 1;
        }
    }
	
    if (bound) {
	CLEAR_STATUS(status);
    } else {
	SET_STATUS(status, sec_rgy_server_unavailable);
    }

    return bound;
}	

/*
 * r c a _ c h e c k _ b i n d i n g 
 *
 * Validate an rpc binding in a sec_rgy_handle_t.  If the binding is invalid and
 * the handle is rebindable, select a new site.  This routine assumes the context
 * mutex is locked.  
 */
PUBLIC boolean32 rca_check_binding (
    sec_rgy_handle_t    *context_p,
    rca_operation_t     which,
    error_status_t      *status
)
{
    rca_pvt_handle_blk_t        *context = (rca_pvt_handle_blk_t *) *context_p;
    unsigned_char_p_t           tmp_name_p = NULL;
    error_status_t              st;

    SEC_DBG_PRINTF(5, ("(rca_check_binding) checking %x\n", context));
    /* 
     * only work with real live contexts.  Caller should have called
     * rca_lock_handle first to convert the default handle to a real 
     * one
     */
    if (context == sec_rgy_default_handle) {
        SET_STATUS(status, sec_rgy_bad_handle);
        return false;
    }
    /* set last time used */
    context->last_used = binding_clock++;

    SEC_DBG_PRINTF(6, ("(rca_check_binding) flags is %x\n", context->handle_flags));
    
    if (! FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID)) {
	if (FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_REBINDABLE)) {
            dce_svc_printf(SEC_RCA_SITE_REBIND_MSG);
            if (rca_rebind(context, status))  {
                dce_svc_printf(SEC_RCA_SITE_REBIND_SUCC_MSG, context->site_name);
                return true ;
            }
            else {
                dce_svc_printf(SEC_RCA_SITE_REBIND_FAIL_MSG);
                return false ;
            }
        }
	else  
	{
	    error_status_t xst;
	    SEC_DBG_GPRINTF(("(rca_check_binding) Not rebindable.. resetting valid flag just in case\n"));
	    
	    SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID);
	    rpc_binding_reset (context->rpc_handle, &xst);
            /* N.B. Error which caused the rebind is already logged by 
             *      rca_retry_status() 
             */
	    SET_STATUS(status, sec_rgy_server_unavailable);
	    return false;
	}
    }

    CLEAR_STATUS(status);
    return true;
}

/*
 * Fetch bindings from pe_site file..
 */

PRIVATE boolean32 rca_site_bind_pe_site (context, status)
    rca_pvt_handle_blk_t *context;
    error_status_t *status;
{
    boolean32               bound = false;
    /* 
     * If we're here, then context->requested_site_name will give us 
     * a good binding if
     *    A) it matches an entry in the client agent backstop file, or
     *    B) it's a valid string binding to a registry site
     */
    /* 
     * This converts the input string (server name) to a string
     * binding, if the caller provided a cell name that matches an
     * entry in the client agent backstop file.
     */

    SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND);
    SEC_DBG_GPRINTF(("(rca_site_bind_pe_site) calling get_site_bindings\n"));
    bound = rca_get_site_bindings(context, status);
    if (bound)
    {
	SEC_DBG_GPRINTF(("(rca_site_bind_pe_site) returned successfully from rca_get_site_bindings\n"));
	bound = rca_rebind(context, status);
	if (bound) 
	{
	    SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_REBINDABLE);
	}
    }
    if (!bound) 
    {
	/*
	 * Ok, fall back to assuming what was passed in is a string
	 * binding, and use *that*..
	 */
	SEC_DBG_GPRINTF(("(rca_site_bind_pe_site) assuming %s is string binding\n", context->requested_site_name));
	bound = rca_bind_to_host (context->requested_site_name, &context->rpc_handle, status);
	if (bound)
	    return rca_setup_handle(context, status);
    }

    return bound;
}


PRIVATE boolean32 rca_site_bind_nsi (context, status)
    rca_pvt_handle_blk_t *context;
    error_status_t *status;
{
    error_status_t          st;
    boolean32               bound = false;
    unsigned_char_p_t       tmp_name_p = NULL;

    SEC_DBG_GPRINTF(("(rca_site_bind_nsi) Attempting to bind to site %s\n",
	context->requested_site_name));
    
    rpc_ns_entry_inq_resolution(rpc_c_ns_syntax_dce,
                                context->requested_site_name,
                                &context->site_name,
                                (unsigned_char_p_t *) NULL,
                                status);

    /* attempt to import a sec_rgy binding handle from this name */
    if (STATUS_EQUAL(status, rpc_s_partial_results)) 
	/* keep going in order to accept principal names; revisit someday */;
    else if (BAD_STATUS(status)) 
    {
	SEC_DBG_GPRINTF(("(rca_site_bind_nsi) inq_resolution failed, status %x\n", *status));
	return false;    
    }

    rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce, 
	context->site_name,
	context->if_spec, (uuid_p_t)NULL,
	&context->import_handle, status);
    
    if (BAD_STATUS(status)) 
    {
	SEC_DBG_GPRINTF(("(rca_site_bind_nsi) import_begin failed, status %x\n", *status));
	return false;
    }
    
    rpc_ns_binding_import_next(context->import_handle, &context->rpc_handle, status);

    if (BAD_STATUS(status)) 
    {
	SEC_DBG_GPRINTF(("(rca_site_bind_nsi) import_next failed, status %x\n", *status));
        rpc_ns_binding_import_done(&context->import_handle, &st);
	return false;
    }
    SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_IMPORT_H_VALID | RCA_HANDLE_FLAGS_REBINDABLE);
    SEC_DBG_GPRINTF(("(rca_site_bind_nsi) calling rca_setup_handle\n"));
    return rca_setup_handle(context, status);
}


PRIVATE void rca_site_bind_nsi_cell_if (
    rca_pvt_handle_blk_t	*context,	/* [in] */
    sec_rgy_name_t		cell_name,	/* [in] */
    rpc_if_handle_t		if_spec,	/* [in] */
    error_status_t		*status		/* [out] */
)
{
    sec_rgy_pname_t         tmp_string;
    error_status_t          st;

    sprintf((char *)tmp_string, "%s/%s", (char *)cell_name,
            CELL_PROFILE_NAME);

    rpc_ns_binding_import_begin(rpc_c_ns_syntax_dce,
	(unsigned_char_p_t) tmp_string, 
	if_spec, (uuid_p_t) NULL,
	&context->import_handle, status);

    if (BAD_STATUS(status)) {
	SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) rs_bind import_begin failed, status %x\n", *status));
	return;
    }
    if (FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_PATIENT_REBIND)) {
	rpc_ns_mgmt_handle_set_exp_age(context->import_handle, 60, &st);
	if (BAD_STATUS(&st)) {
		SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) rpc_ns_mgmt_handle_set_exp_age failed, status %x\n", st));
	}
    }

    do {
	rpc_ns_binding_import_next(context->import_handle, 
		&context->rpc_handle, status);

	if (BAD_STATUS(status)) {
	    SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) rs_bind import_next failed, status %x\n", *status));
	    break;
	}

	SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_IMPORT_H_VALID
		| RCA_HANDLE_FLAGS_REBINDABLE);

	SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) about to call rca_setup_handle; rpc_handle==%x\n", context->rpc_handle));
	if (rca_setup_handle(context, status)) {
	    if (context->cell_name && *context->cell_name)
		free(context->cell_name);
	    context->cell_name = cell_name;
	    return;
	}
    } while (BAD_STATUS(status));

    rpc_ns_binding_import_done(&context->import_handle, &st);
    context->import_handle = NULL;
    UNSET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_IMPORT_H_VALID);
}


PRIVATE boolean32 rca_site_bind_nsi_cell(context, cell_name, status)
    rca_pvt_handle_blk_t *context;
    sec_rgy_name_t          cell_name;
    error_status_t *status;
{
    SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) Attempting to bind to cell %s\n", cell_name));

    /*
     * For compatibility when running new clients in old cells, we try the
     * replication bind interface before falling back to the 1.0.1 interface.
     */
    CLEAR_STATUS(status);

    if (context->if_spec == RCA_DEFAULT_SECIF) {
        SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) Trying rs_bind interface\n"));
	rca_site_bind_nsi_cell_if(context, cell_name,
		rs_bind_v2_0_c_ifspec, status);
    }

    if (context->if_spec != RCA_DEFAULT_SECIF ||
	STATUS_EQUAL(status, rpc_s_no_more_bindings)) {
        SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) Trying caller-specified interface\n"));
	rca_site_bind_nsi_cell_if(context, cell_name,
		context->if_spec, status);
    }

    if (GOOD_STATUS(status))
	return true;
    else {
	SEC_DBG_GPRINTF(("(rca_site_bind_nsi_cell) failed, status %x\n", *status));
	return false;
    }
}

/*
 * r c a _ c h e c k _ e n v _ v a r s
 * 
 * Check environment variables (once) for the appropriate settings.
 */
PRIVATE void rca_check_env_vars (void)
{
    static boolean32        checked_env = false;
    char           *bypass;
    
    if (checked_env) 
	return;

    checked_env = true;

    bypass = getenv ("BIND_PE_SITE");
    if ((bypass != NULL) && (bypass[0] != '0'))
    {
	rca_use_nsi = false;
    }
    bypass = getenv("DEBUG_SEC");
    
    if ((bypass != NULL) && (bypass[0] != '0'))
    {
	if (isdigit(bypass[0]))
	    rca_binding_cache_trace = atoi(bypass);
    }
}

 
/*
 * given a binding handle, acquire the principal name from the
 * rpc runtime, and extract the cell component.
 */ 
PRIVATE  boolean32  rca_inq_cell_name ( context, status) 
    rca_pvt_handle_blk_t *context;
    error_status_t *status;
{
    error_status_t        st;
    int                   len1, len2;
    unsigned_char_p_t     cp;
    unsigned_char_p_t	  princ_name = NULL;
    
    if (context->uncanon_princ_name == NULL 
	|| context->uncanon_princ_name[0] == '\0') {
	return false;
    }
    
    rpc_mgmt_inq_server_princ_name(context->rpc_handle, 
	rpc_c_authn_dce_secret, 
	&princ_name, status);

    if (STATUS_EQUAL(status, rpc_s_binding_incomplete)) {
	princ_name = NULL;
        rpc_ep_resolve_binding(context->rpc_handle, context->if_spec, &st);
        rpc_mgmt_inq_server_princ_name(context->rpc_handle, 
	    rpc_c_authn_dce_secret, &princ_name, status);
    }
    if (BAD_STATUS(status)) 
    {
	princ_name = NULL;
	SEC_DBG_GPRINTF(("(rca_inq_cell_name) rpc_mgmt_inq_server_princ_name failed -- status %x\n", *status));
        goto fail;
    }
    
    len1 = u_strlen(princ_name); 
    if (len1 > sizeof(sec_rgy_name_t)-1)
        goto fail;
    
    len2 = u_strlen(context->uncanon_princ_name);
    if (len1 < len2 + 1) 	 /* couldn't be the server we want */
        goto fail;
    
    cp = princ_name + len1 - len2;
    if (u_strcmp(cp, context->uncanon_princ_name) == 0) {
        *(--cp) = '\0';  /* back up one to get rid of (/) separator */
	/* 
	 * inherit the storage. 
	 */
	context->cell_name = princ_name;
        return true;
    }
 fail:
    if (princ_name)
	rpc_string_free(&princ_name, &st);
    return false;
}
   

PUBLIC boolean32 sec_rgy_enable_nsi (
    boolean32   flag
)
{
    boolean32   old_val;

    rca_check_env_vars();

    old_val = rca_use_nsi;
    rca_use_nsi = flag;

    return old_val;
}


PRIVATE rca_pvt_handle_blk_t *rca_handle_init (
    sec_rgy_bind_auth_info_t	*auth_info,	/* [in]  */
    char			*princ,		/* [in]  */
    sec_rgy_name_t		cell_name,	/* [in]  */
    error_status_t		*status		/* [out] */
)
{
    rca_pvt_handle_blk_t        *new_context;
    int i;

    CLEAR_STATUS(status);

    /*
     * We now have to build a new context.
     * 
     * To allow us to share code between original binding and
     * rebinding, we allocate it first, assuming that the binding will succeed.
     * 
     * If it fails, we'll trash it later.
     */
    
    new_context = NEW(rca_pvt_handle_blk_t);
    if (new_context == NULL) 
    {
	/* oops. */
	SET_STATUS(status, sec_rgy_cant_allocate_memory);
	return new_context;
    }
    new_context->cache_next = NULL;
    new_context->cache_prev = NULL;
    new_context->refcount = 1;
    new_context->last_used = binding_clock++;
    new_context->handle_flags = RCA_HANDLE_FLAGS_NONE;
    new_context->import_flags = 0;
    new_context->rpc_handle = 0;
    new_context->import_handle = NULL;
    new_context->if_spec = RCA_DEFAULT_SECIF;
    new_context->site_name = 0;
    new_context->princ_name = NULL;
    new_context->uncanon_princ_name = NULL;
    new_context->requested_site_name = NULL;
    new_context->tower_vec = NULL;
    new_context->tower_vec_index = 0;
    new_context->hindex = 0;
    new_context->import_count = 0;
    new_context->last_cachehint.person_dtm = 0;
    new_context->last_cachehint.group_dtm = 0;
    new_context->last_cachehint.org_dtm = 0;
    
    for (i=0; i<RCA_MAX_HANDLES; i++) 
    {
	new_context->alt_handles[i] = NULL;
    }

    pthread_mutex_init(&new_context->mutex, pthread_mutexattr_default);
    
    if (princ) 
    {
	new_context->uncanon_princ_name = rpc_stralloc((unsigned_char_p_t)princ);
	if (new_context->uncanon_princ_name == NULL) 
	{
	    SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    return NULL; /* XXX!!! mem leak on new_context here */
	}
    }

    if (cell_name && cell_name[0]) {
	new_context->cell_name = rpc_stralloc(cell_name);
	
	if (new_context->cell_name == NULL) 
	{
	    SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    return NULL; /* XXX!!! mem leak on new_context here */
	}
    } else {
	new_context->cell_name = NULL;
    }
    
    new_context->auth_info = *auth_info;
    if (new_context->auth_info.info_type == sec_rgy_bind_auth_dce) 
    {
	error_status_t lst;
	rpc_auth_identity_handle_t ident;
	ident = auth_info->tagged_union.dce_info.identity;
	SEC_DBG_PRINTF(3,("(rca_site_bind) bumping ref on identity %x\n", ident));
	sec_login_util_reference (ident, (void *)&ident, &lst);
	assert (lst == error_status_ok);
	new_context->auth_info.tagged_union.dce_info.identity = ident;
    }

    return new_context;
}


/*
 * t o w e r _ v e c _ d u p -- allocate and copy tower vector
 */
PRIVATE rs_replica_twr_vec_p_t tower_vec_dup (
    rs_replica_twr_vec_p_t	towerv,		/* [in] */
    error_status_t		*status		/* [out] */
)
{
    unsigned32 i;
    rs_replica_twr_vec_p_t	new_towerv;

    CLEAR_STATUS(status);

    new_towerv = malloc(sizeof(*towerv) +
			sizeof(*towerv->towers) * (towerv->num_towers-1) );
    if (new_towerv == NULL) {
	SET_STATUS(status, sec_rgy_cant_allocate_memory);
	return NULL;
    }
    new_towerv->num_towers = towerv->num_towers;

    for (i=0; i<towerv->num_towers; i++) {
	twr_p_t ts = towerv->towers[i];
	twr_p_t td = malloc(sizeof(*ts) + ts->tower_length-1);
	if (td == NULL) {
	    new_towerv->num_towers = i;
	    TOWER_VEC_FREE(free, new_towerv);
	    SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    return NULL;
	}
	new_towerv->towers[i] = td;
	td->tower_length = ts->tower_length;
	memcpy(td->tower_octet_string,
			ts->tower_octet_string, ts->tower_length);
    }

    return new_towerv;
}


PUBLIC void rca_site_bind_from_tower_vec (
    rpc_tower_vector_p_t        towerv,     /* [in]  */
    unsigned_char_t             *cell_name, /* [in]  */
    unsigned_char_t             *site_name, /* [in]  */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status     /* [out] */
)
{
    rs_replica_twr_vec_p_t	tv = (rs_replica_twr_vec_p_t) towerv;
    rca_pvt_handle_blk_t	*new_context;
    unsigned_char_p_t           ucp = NULL;
    error_status_t		xst;

    *context = NULL;
    CLEAR_STATUS(status);

    if (tv == NULL || tv->num_towers == 0 || auth_info == NULL) {
	SET_STATUS(status, sec_s_null_param_illegal);
	return;
    }

    /* Canonicalize given cell name */
    if (cell_name && *cell_name) {
	rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, cell_name, &ucp, status);
	if (BAD_STATUS(status)) {
            return;
	}
    }

    /*
     * bind the identity now... 
     */
    
    if ((auth_info->info_type == sec_rgy_bind_auth_dce) &&
	(auth_info->tagged_union.dce_info.identity == NULL)) 
    {
	auth_info->tagged_union.dce_info.identity = 
	    (sec_login_handle_t)sec_login_pvt_get_default_context(status);
	if (BAD_STATUS(status))
            if (ucp != NULL)
                rpc_string_free(&ucp, &xst);
            return;
    }
    
    new_context = rca_handle_init(auth_info, NULL, ucp, status);
    if (ucp != NULL)
        rpc_string_free(&ucp, &xst);
    if (BAD_STATUS(status))
	return;

    if (site_name) {
	new_context->site_name = rpc_stralloc(site_name);
	if (new_context->site_name == NULL) {
	    sec_rgy_site_close((sec_rgy_handle_t) new_context, &xst);
	    SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    return;
	}
    }

    new_context->tower_vec = tower_vec_dup(tv, status);
    if (BAD_STATUS(status)) {
	sec_rgy_site_close((sec_rgy_handle_t) new_context, &xst);
	return;
    }

    EXCEPTION_INIT (rca_e_auth_tkt_expired);
    TRY
	rca_rebind_tower(new_context, status);
    CATCH (rca_e_auth_tkt_expired)
	SET_STATUS(status, rpc_s_auth_tkt_expired);
    ENDTRY

    if (BAD_STATUS(status)) {
	sec_rgy_site_close((sec_rgy_handle_t) new_context, &xst);
	return;
    }    

    *context = (sec_rgy_handle_t) new_context;
}

PUBLIC void rca_site_bind (
    unsigned_char_p_t           site_name,  /* [in] */
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    long                        flags,
    rpc_if_handle_t             if_spec,
    char                        *princ,
    char                        *cell,      /* May be NULL */
    sec_rgy_handle_t            *context,   /* [out] */
    error_status_t              *status
)
{
    static boolean32        checked_env = false;
    sec_rgy_name_t          lsite;      /* local copy of site_name */
    volatile boolean32			bound = false;
    sec_rgy_name_t          cell_name;
    error_status_t          st;
    rca_pvt_handle_blk_t    *new_context /* = NULL */;
    unsigned_char_p_t       tmp_name_p;
    unsigned char           *msgp = NULL ;
    
    CLEAR_STATUS(status);
    
    if (auth_info == NULL) 
    {
	*status = sec_s_null_param_illegal;
	*context = NULL;
	return;
    }
    
    if (!checked_env) {
        rca_check_env_vars();
        checked_env = true;
    }
    
    /*
     * bind the identity now... 
     */
    
    if (auth_info->info_type == sec_rgy_bind_auth_dce)
    {
	/* 
	 * validate authentication parameters.
	 * auth_level is an unsigned32, so comparing < 0 is nonsensical.
	 * !!! this should perhaps be broken out into a common routine.
	 */
	if ((auth_info->tagged_union.dce_info.authn_level > rpc_c_protect_level_pkt_privacy) ||
	    ((auth_info->tagged_union.dce_info.authn_svc != rpc_c_authn_default) &&
	     (auth_info->tagged_union.dce_info.authn_svc != rpc_c_authn_dce_secret) &&
	     (auth_info->tagged_union.dce_info.authn_svc != rpc_c_authn_none)) ||
	    ((auth_info->tagged_union.dce_info.authz_svc > rpc_c_authz_dce)))
	{
	    SET_STATUS(status, sec_s_invalid_auth_params);
	    return;
	}
	    
	    
	if (auth_info->tagged_union.dce_info.identity == NULL)
	{
	    auth_info->tagged_union.dce_info.identity = 
		(sec_login_handle_t)sec_login_pvt_get_default_context(status);
	    if (BAD_STATUS(status))
		return;
	}
    } else if (auth_info->info_type != sec_rgy_bind_auth_none) 
    {
	SET_STATUS (status, sec_s_invalid_auth_params);
	return;
    }

    cell_name[0] = '\0';
    if (cell)
	u_strcpy(cell_name, cell);
    
    if (cell_name[0]) {
	unsigned_char_p_t  ucp;
	
	rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce,
	    cell_name, &tmp_name_p, status);
	if (GOOD_STATUS(status)) {
	    u_strcpy(cell_name, tmp_name_p);
	    rpc_string_free(&tmp_name_p, &st);
	}
    }
    

    /*
     * if the caller passed in a NULL site_name, use either the cell name passed in,
     * the client user's default cell, or (failing that) whatever the local machine's cell is.
     */
    if (!(site_name && *site_name)) {
	if (cell_name[0]) 
	{
	    site_name = lsite;
	    u_strcpy(site_name, cell_name);
	} else {
	    rca_get_cell_name(auth_info, lsite);          
	    site_name = lsite;
	    u_strcpy(cell_name, site_name);
	}
    } else
    {
        /*
         * If the user input begins with a component separator, then it is a 
         * global name of some sort.  We canonicalize it up front because 
         * it makes life easier in a myriad of little ways.
         */
        if (*site_name == PATH_COMPONENT_SEPARATOR) {   
            unsigned_char_p_t  exp_name;
               
            rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, site_name, &exp_name, status);
            if (GOOD_STATUS(status)) {
                u_strncpy(lsite, exp_name, sizeof(lsite));
                lsite[sizeof(lsite)-1] = '\0';
		site_name = lsite;
                rpc_string_free(&exp_name, &st);
            }
        } else {
            u_strncpy(lsite, site_name, sizeof(lsite));
            lsite[sizeof(lsite)-1] = '\0';
        }
    }

    if (BAD_STATUS(status)) {
        return;
    }

    
    /*
     * check our cache.
     */
    
    if (rca_max_entries &&
        /* CHFts15769: add test for match of what would be the "requested_site_name"
         * of the requested binding with the requested_site_name of the cached
         * binding.
         */
	binding_cache_lookup(lsite, site_name, auth_info,
		(unsigned_char_p_t)princ, cell_name, &new_context))
        /* CHFts15769: end */
    {
	SEC_DBG_GPRINTF(("returning cached context %x\n", new_context));
	*context = (sec_rgy_handle_t) new_context;
	CLEAR_STATUS(status);
	return;
    }
    
    /*
     * We now have to build a new context.
     */
    new_context = rca_handle_init(auth_info, princ, cell_name, status);
    if (BAD_STATUS(status))
	return;

    if (if_spec)
	new_context->if_spec = if_spec;
    new_context->import_flags = flags;
    new_context->requested_site_name = rpc_stralloc(lsite);

    EXCEPTION_INIT (rca_e_auth_tkt_expired);
    TRY
	bound = rca_nsi_binding_import (new_context, status);
    CATCH (rca_e_auth_tkt_expired)
	SET_STATUS(status, rpc_s_auth_tkt_expired);
    ENDTRY

    if (!bound) 
    {
	if (! STATUS_EQUAL(status, rpc_s_auth_tkt_expired)) {
            dce_svc_printf(SEC_RCA_OP_STATUS_MSG , new_context->site_name , msgp=dce_sprintf(*status));
            if (msgp) free(msgp) ;
	    SET_STATUS(status, sec_rgy_server_unavailable);
        }
    }
    else
    {
        /*
         * Success. 
	 */
	CLEAR_STATUS(status);

	/*
	 * Try pinging the site to verify availability 
	 *
	 * NOT YET IMPLEMENTED - just assume it's valid
	 */
	SET(HANDLE_FLAGS(new_context), RCA_HANDLE_FLAGS_VALID);

	*context = (sec_rgy_handle_t) new_context;

	if (SEC_DBG(1)) {
	    unsigned_char_t *str_b = NULL;
	    error_status_t  st;

	    SEC_DBG_GPRINTF(("(rca_site_bind) Bound to Cell '%s'\n", new_context->cell_name));
	    SEC_DBG_GPRINTF(("(rca_site_bind) Bound to Site '%s'\n", new_context->site_name));
	    sec_rgy_site_binding_get_info(*context, NULL, NULL, 
		&str_b, NULL, &st);
	    SEC_DBG_GPRINTF(("(rca_site_bind) String Binding  '%s'\n",
		(char *)str_b ? (char *)str_b : ""));
	}
    }

    if (BAD_STATUS(status)) {
	if (new_context)
	    sec_rgy_site_close((sec_rgy_handle_t) new_context, &st);
        *context = NULL;
    }           

    if (GOOD_STATUS(status))
	binding_cache_enter (new_context);

}


/*
 * s e c _ r g y _ s i t e _ c l o s e 
 *
 * Destroys the site binding associated with the context parameter.  Subsequent
 * use of the context causes an error. 
 */

PUBLIC void sec_rgy_site_close (
    sec_rgy_handle_t    ext_context,    /* [in] */
    error_status_t      *status
)
{
    rca_pvt_handle_blk_t    *context;
    int i;
    error_status_t xst;
    
    CLEAR_STATUS(status);
    
    context = (rca_pvt_handle_blk_t *) ext_context;

    if (context != sec_rgy_default_handle) {

	rca_lock_handle(&ext_context, status);
	if (STATUS_OK(status)) {

	    SEC_DBG_PRINTF(3,("%x: Dropping site refcount from %d", context, context->refcount));
	    assert (context->refcount > 0);
	    context->refcount--;
	    SEC_DBG_PRINTF(3,(" to %d\n", context->refcount));

	    if (context->refcount != 0) {
		rca_unlock_handle(ext_context);
		return;
	    }
	    else {
                /* CHFts13082: remove erroneous "assert cache_next,cache_prev
                 *  == NULL" because that is true only if entry is the only 
                 * entry in the cache, which is not necessarily the case.
                 */

		if (context->auth_info.info_type == sec_rgy_bind_auth_dce) 
		{
		    SEC_DBG_PRINTF(3,("Drop ref to identity %x from binding cache\n", 
			context->auth_info.tagged_union.dce_info.identity));
		    sec_login_util_release 
			(context->auth_info.tagged_union.dce_info.identity);
		    context->auth_info.tagged_union.dce_info.identity = 0;
		}
		    
		if (context->site_name != NULL)
		    free(context->site_name);

		if (context->cell_name != NULL)
		    free(context->cell_name);

		if (context->uncanon_princ_name != NULL)
		    free (context->uncanon_princ_name);
		
		if (context->princ_name != NULL) 
		    free(context->princ_name);
		
		if (context->requested_site_name != NULL)
		    free (context->requested_site_name);
		
		if (context->tower_vec != NULL)
		    TOWER_VEC_FREE(free, context->tower_vec);

		for (i=0; i<RCA_MAX_HANDLES; i++) 
		{
		    if (context->alt_handles[i] != NULL) 
			rpc_binding_free(&(context->alt_handles[i]), &xst);
		}

		if (FLAG_SET(HANDLE_FLAGS(context),
		    RCA_HANDLE_FLAGS_IMPORT_H_VALID)) 
		{
		    rpc_ns_binding_import_done(&context->import_handle,status);
		}                  
		if (context->rpc_handle != NULL) {
		    rpc_binding_free(&context->rpc_handle, status);
		}
		rca_unlock_handle(ext_context);
		pthread_mutex_destroy(&context->mutex);
	    }
	    DISPOSE(context);
	}
    }
}


PUBLIC void rca_site_resolve_update (
    sec_rgy_bind_auth_info_t    *auth_info, /* [in]  */
    sec_rgy_handle_t            icontext,   /* [in]  */
    sec_rgy_handle_t            *ocontext,  /* [out] */
    error_status_t              *status     /* [out] */
)
{
    boolean32			update_site;
    sec_rgy_name_t		cell_name;
    rs_replica_name_p_t		site_name;
    uuid_t			srvuuid, instuuid;
    rs_replica_twr_vec_p_t	towerv;
    volatile unsigned32 i;

    rca_lock_handle(&icontext, status);
    if (BAD_STATUS(status))
	return;

    I_SETUP_RETRY(rca_op_read, status) {
        Crs_bind_get_update_site(HANDLE(icontext), cell_name, &update_site,
            &site_name, &instuuid, &towerv, status);
        I_NORMAL_CASE;
        I_RETRY_CASE;
    }
    I_RETRY_END;

    /*
     * Halt without error if we're in a pre-replication cell; need to bump
     * the reference count because our caller will close the input handle.
     */
    if (STATUS_EQUAL(status, rpc_s_unknown_if)) {
	CLEAR_STATUS(status);
	SET(HANDLE_FLAGS(icontext), RCA_HANDLE_FLAGS_SITE_UPDATEABLE);
	SET(HANDLE_FLAGS(icontext), RCA_HANDLE_FLAGS_PATIENT_REBIND);
	((rca_pvt_handle_blk_t *)icontext)->refcount++;
	*ocontext = icontext;
	goto unlock_exit;
    }
    if (BAD_STATUS(status))
	goto unlock_exit;

    /* Keep service uuid for later */
    rpc_binding_inq_object(HANDLE(icontext), &srvuuid, status);
    if (BAD_STATUS(status) || uuid_is_nil(&srvuuid, status) || BAD_STATUS(status))
	rca_get_obj_id(HANDLE(icontext), &srvuuid, status);
    if (BAD_STATUS(status) || uuid_is_nil(&srvuuid, status) || BAD_STATUS(status))
	goto unlock_exit;

    /*
     * Normally we'll be done, but we may have been referred to another site.
     */
    while (! update_site) {
	for (i=0; i<towerv->num_towers; i++) {
	    twr_p_t			tower = towerv->towers[i];
	    rpc_binding_handle_t	tmp_rpch;
            unsigned char               *rpch_string = NULL;
            unsigned char               *msgp = NULL ;
	    rs_replica_twr_vec_p_t	tmp_tv;
	    error_status_t		xst;

	    /* Try next tower if can't get binding */
	    rpc_tower_to_binding(tower->tower_octet_string,
		&tmp_rpch, status);
	    if (BAD_STATUS(status))
		continue;

	    rpc_binding_set_object(tmp_rpch, &srvuuid, status);
	    if (BAD_STATUS(status)) {
		rpc_binding_free(&tmp_rpch, &xst);
		continue;
	    }

	    /* Proceed with next tower vector, freeing previous */
            Crs_bind_get_update_site(tmp_rpch, cell_name, &update_site,
                                     &site_name, &instuuid, &tmp_tv, status);
	    rpc_binding_free(&tmp_rpch, &xst);

	    if (GOOD_STATUS(status)) {
		TOWER_VEC_FREE(rpc_ss_client_free, towerv);
		towerv = tmp_tv;
		break;
	    }

            if (BAD_STATUS(status)) {
                rpc_binding_to_string_binding(tmp_rpch, &rpch_string, &xst);
                dce_svc_printf(SEC_RCA_OP_STATUS_MSG , 
                               rpch_string, 
                               msgp=dce_sprintf(*status));
                if (msgp) 
                    free(msgp);
                if (rpch_string) 
                    rpc_string_free(&rpch_string, &xst);
            }
	}
	/* Finished cycling through tower vector */
	if (BAD_STATUS(status)) {
	    TOWER_VEC_FREE(rpc_ss_client_free, towerv);
            SET_STATUS(status, sec_rgy_server_unavailable);
	    goto unlock_exit;
	}

    }

    if (GOOD_STATUS(status)) {

	unsigned char		sep[] = {PATH_COMPONENT_SEPARATOR, '\0'};
	unsigned_char_p_t	tmp_site_name = (unsigned_char_p_t)
		malloc(u_strlen(cell_name)+1 +u_strlen(site_name) +1);

	if (tmp_site_name == NULL) {
	    SET_STATUS(status, sec_rgy_cant_allocate_memory);
	    goto unlock_exit;
	}
	u_strcpy(tmp_site_name, cell_name);
	u_strcat(tmp_site_name, sep);
	u_strcat(tmp_site_name, site_name);

	rca_site_bind_from_tower_vec((rpc_tower_vector_p_t) towerv,
			cell_name, tmp_site_name, auth_info, ocontext, status);

	if (GOOD_STATUS(status)) {
	    SET(HANDLE_FLAGS(*ocontext), RCA_HANDLE_FLAGS_SITE_UPDATEABLE);
	    SET(HANDLE_FLAGS(*ocontext), RCA_HANDLE_FLAGS_PATIENT_REBIND);
	}

	rpc_ss_client_free(site_name);
	free((char *)tmp_site_name);
    }
    TOWER_VEC_FREE(rpc_ss_client_free, towerv);

unlock_exit:
	rca_unlock_handle(icontext);
}


PUBLIC boolean32 rca_site_check_readonly (
    sec_rgy_handle_t            context,    /* [in] */
    error_status_t              *status     /* [out] */
)
{
    boolean32			update_site;
    sec_rgy_name_t		cell_name;
    rs_replica_name_p_t		site_name;
    uuid_t			instuuid;
    rs_replica_twr_vec_p_t	towerv;

    CLEAR_STATUS(status);

    if (!FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_SITE_READONLY) &&
	!FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_SITE_UPDATEABLE)) {

        SETUP_RETRY(rca_op_read, status) {
            Crs_bind_get_update_site(HANDLE(context), cell_name, &update_site,
                &site_name, &instuuid, &towerv, status);
            NORMAL_CASE;
            RETRY_CASE;
        }
        RETRY_END;

	if (GOOD_STATUS(status)) {
	    rpc_ss_client_free(site_name);
	    TOWER_VEC_FREE(rpc_ss_client_free, towerv);
	}
	else if (STATUS_EQUAL(status, rpc_s_unknown_if)) {
	    CLEAR_STATUS(status);
	    update_site = TRUE;
	}

	if (BAD_STATUS(status))
	    return false;

	SET(HANDLE_FLAGS(context), update_site ?
		RCA_HANDLE_FLAGS_SITE_UPDATEABLE :
		RCA_HANDLE_FLAGS_SITE_READONLY);
    }

    return FLAG_SET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_SITE_READONLY);
}


/*
 * Return true if it is appropriate to retry an operation that
 * received status "op_st".
 * Return false if it is not appropriate to retry the operation.
 */
PUBLIC boolean32 rca_retry_status (
    sec_rgy_handle_t context,
    error_status_t op_st
)
{
    boolean32   retval;
    unsigned char *msgp = NULL ;
    rca_pvt_handle_blk_t   *context_p ;

    switch(op_st) { 
        case error_status_ok:
            retval = false;
            break;

        case rpc_s_wrong_boot_time:
        case rpc_s_comm_failure:
        case rpc_s_endpoint_not_found:	/* ??? */
        case rpc_s_connection_closed:
        case rpc_s_connect_timed_out:
        case rpc_s_connect_rejected:
        case rpc_s_network_unreachable:
        case rpc_s_connect_no_resources:
        case rpc_s_rem_network_shutdown:
        case rpc_s_too_many_rem_connects:
        case rpc_s_no_rem_endpoint:
        case rpc_s_rem_host_down:
        case rpc_s_host_unreachable:
        case rpc_s_access_control_info_inv: /* ??? */
        case rpc_s_connect_closed_by_rem:
        case rpc_s_rem_host_crashed:
        case rpc_s_tsyntaxes_unsupported:
        case rpc_s_cant_getpeername:
        case rpc_s_unknown_reject:	/* ??? */
        case rpc_s_wrong_kind_of_binding:
        case rpc_s_call_timeout:
        case rpc_s_server_too_busy:
        case rpc_s_unknown_error:
        case rpc_s_socket_failure:
        case rpc_s_auth_skew:
        case rpc_s_not_listening:
        case rpc_s_rpcd_comm_failure:
        case rpc_s_object_not_found:
        case rpc_s_cthread_not_found:
        case rpc_s_invalid_rpc_protseq:	/* ??? */
        case rpc_s_max_descs_exceeded:	/* ??? */
        case rpc_s_unknown_if:
        case rpc_s_unsupported_type:
        case rpc_s_loc_connect_aborted:
        case rpc_s_unknown_mgr_type:
	case ept_s_not_registered:
	case sec_rgy_rep_bad_state:

            context_p = (rca_pvt_handle_blk_t *) context ;
            dce_svc_printf(SEC_RCA_OP_STATUS_MSG, context_p->site_name, 
                           msgp=dce_sprintf(op_st));
            if (msgp)
                free(msgp) ;

	    /* we may wish to take additional action here */
	    UNSET(HANDLE_FLAGS(context), RCA_HANDLE_FLAGS_VALID);

            retval = true;
            break;

        default:
            retval = false;
            break;
    }

    return retval;
}


PUBLIC rca_global_info_t * rca_access_global_info (
    boolean32           update
)
{
    return NULL;
}

PUBLIC void rca_release_global_info (
    rca_global_info_t *info
)
{
}
