/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: svops.c,v $
 * Revision 1.1.7.1  1996/10/03  17:51:58  arvind
 * 	merge fixes for secval deadlock and [de]activate failures
 * 	[1996/08/16  20:32 UTC  lmm  /main/lmm_dced_secval_merge/1]
 *
 * 	merge fix for CHFts17901 and 18706 from HP branch (secval threads)
 *
 * 	Submitted the fix for CHFts18706.
 *
 * 	Added pthread_yield() in secval_enable() to synch with maintain_machine_context().
 * 	[1996/04/25  18:14 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/3]
 *
 * 	Submitted the fix for CHFts17901.
 *
 * 	Reverted to the old way of lying the initial secval state.
 * 	dce_config is severely broken and can't be fixed easily.
 * 	So we lie if dced is started in the bootstrap mode (-b).
 * 	[1996/03/19  19:24 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/2]
 *
 * 	Use selfcntx_ctlblk.
 * 	[1996/03/19  15:14 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/1]
 *
 * Revision 1.1.5.2  1996/02/18  19:16:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:50  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  20:54:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/03  22:30 UTC  mk
 * 	Create HPDCE02 branch and merge in fixes from mk_mothra_dced/1.
 * 
 * 	HP revision /main/mk_mothra_dced/1  1995/02/03  22:23 UTC  mk
 * 	Delete call to sv_purge_context() in secval_disable().
 * 	[1995/12/08  20:49:20  root]
 * 
 * Revision 1.1.2.8  1994/10/03  19:58:46  rsalz
 * 	Put a mutex around secval operations; make explicit init func.
 * 	[1994/10/03  19:57:16  rsalz]
 * 
 * Revision 1.1.2.7  1994/08/24  16:41:17  baum
 * 	Full dced service on bootup.  Secval on by default (OT 11726 & 11656)
 * 	[1994/08/24  16:35:06  baum]
 * 
 * Revision 1.1.2.6  1994/06/13  20:05:42  rsalz
 * 	Code cleanup.
 * 	Server registration done in main now.
 * 	[1994/06/13  20:05:03  rsalz]
 * 
 * Revision 1.1.2.5  1994/06/06  19:45:01  bowe
 * 	Allow null handles for no ACL check.
 * 	[1994/06/06  19:39:57  bowe]
 * 
 * Revision 1.1.2.4  1994/04/26  19:25:52  bowe
 * 	New params to dce_acl_is_client_authorized()
 * 	[1994/04/26  19:19:52  bowe]
 * 
 * Revision 1.1.2.3  1994/04/14  20:08:32  rsalz
 * 	Make server and API all compile.
 * 	[1994/04/14  19:45:10  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:13  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:10  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:26:03  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:33  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <dcedimpl.h>
#include <svimpl.h>

boolean	sv_first_enable_done = FALSE;

/* 
 * Return status of secval service
 */
void
secval_status
(
 /* in */  handle_t			h,
 /* out */ boolean32			*secval_is_active,	      
 /* out */ error_status_t		*st
)
{
    *st = error_status_ok;
    pthread_mutex_lock(&selfcntx_ctlblk.m);
    /* dce_config is severely broken. It expects us to lie. Otherwise,
     * it won't configure! To workaround it (until someone gets rid of
     * dce_config ;), we lie if bootstrapping. */
    if (!sv_first_enable_done) {
	*secval_is_active = sv_first_enable_done;
    }
    else {
	*secval_is_active = (!selfcntx_ctlblk.stop
			     && selfcntx_ctlblk.running
			     && !selfcntx_ctlblk.suspend);
    }
    pthread_mutex_unlock(&selfcntx_ctlblk.m);
    return;
}

/* ______________________________________________________________________ */
/*  Enable the secval service.
 */

void
secval_enable(
  /* in */
    handle_t			h,
  /* out */
    error_status_t		*st
)
{
    *st = error_status_ok;

    if (!dced_authorized(h, &dced_sv_obj_type, &dced_sv_acl,
			 NULL, dced_perm_execute, st))
	return;

    pthread_mutex_lock(&selfcntx_ctlblk.m);
    if (selfcntx_ctlblk.stop || !selfcntx_ctlblk.running) {
	*st = dced_s_sv_not_enabled;	/* Need a better status! */
    }
    else if (!selfcntx_ctlblk.suspend) {
	if (!sv_first_enable_done)
	    sv_first_enable_done = TRUE;
	else
	    *st = dced_s_sv_already_enabled;
    }
    else {
	selfcntx_ctlblk.suspend = FALSE;
	pthread_cond_signal(&selfcntx_ctlblk.cv);
	*st = error_status_ok;
	/* By unlocking the mutex and yielding, we give
	 * maintain_machine_context() the chance to re-setup the self
	 * context. */
	pthread_mutex_unlock(&selfcntx_ctlblk.m);
	pthread_yield();
	/* By locking the mutex again, we are synchronized at the
	 * points where maintain_machine_context() calls
	 * pthread_cond_timedwait() again. */
	pthread_mutex_lock(&selfcntx_ctlblk.m);
	/* We are not verifying if maintain_machine_context() has
	 * re-established the self context, or not, because we don't
	 * want to block the call executor thread forever when the
	 * registry is not available.
	 *
	 * Also, note that if there are concurrent
	 * secval_enable/disable()s, we can't guarantee the outcome,
	 * unless we serialize the requests (by using queue?) which
	 * potentially blocks all call executor threads. */
    }
    pthread_mutex_unlock(&selfcntx_ctlblk.m);
}

/* ______________________________________________________________________ */
/*  Disable the secval service.
 */

void
secval_disable(
  /* in */
    handle_t			h,
  /* out */
    error_status_t		*st
)
{
    *st = error_status_ok;

    if (!dced_authorized(h, &dced_sv_obj_type, &dced_sv_acl,
			 NULL, dced_perm_stop, st))
	return;

    pthread_mutex_lock(&selfcntx_ctlblk.m);
    if (selfcntx_ctlblk.stop
	|| !selfcntx_ctlblk.running
	|| selfcntx_ctlblk.suspend) {
	*st = dced_s_sv_not_enabled;
    }
    else {
	selfcntx_ctlblk.suspend = TRUE;
	pthread_cond_signal(&selfcntx_ctlblk.cv);
	*st = error_status_ok;
	/* We are not verifying if maintain_machine_context() is
	 * suspened or not. We trust its behavior when suspend ==
	 * TRUE. :) */
    }
    pthread_mutex_unlock(&selfcntx_ctlblk.m);
}

/* ______________________________________________________________________ */
/* request a key to talk to a server XXX not currently implemented */

void
secval_request_ticket(
  /* in */
    handle_t			h,
    twr_t			*tower,
    sec_timeval_period_t	how_long,
  /* out */
    error_status_t		*st
)
{
    boolean32		ok;

    if (!dced_authorized(h, &dced_sv_obj_type, &dced_sv_acl,
			 NULL, dced_perm_update, st))
	    return;
    if (!ok) {
	*st = sec_acl_invalid_permission;
	return;
    }
    *st = sec_s_not_implemented;
}

/* ______________________________________________________________________ */
/*  Validate the certification authority.
 */

void
secval_validate_cert_auth(
  /* in */
    handle_t			h,
  /* out */
    error_status_t		*st
)
{
    *st = error_status_ok;
    /* no action - since the RPC runtime does all the work */
}
/* ______________________________________________________________________ */

/*  See if the cell_aliases hostdata object needs to be updated.
 *  Not implemented in DCE1.1 because registry doesn't return the
 *  right timestamp (mod time of cell prin) in the cache data.
 *  N.B.:  This is a [maybe] operation for lower-overhead, hence no
 *  output params (would like to return sec_s_not_implemented :-).
 */
void
secval_check_cell_names(
  /* in */
    handle_t			h,
    rs_cache_data_t		*cache
)
{
}

/* ______________________________________________________________________ */

