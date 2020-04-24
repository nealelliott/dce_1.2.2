/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dcedpvt_server.h,v $
 * Revision 1.1.2.2  1996/02/18  23:32:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:20  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:53:14  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  22:40 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/23  00:41 UTC  tatsu_s
 * 	Initial revision.
 * 	[1995/12/08  20:48:53  root]
 * 
 * $EndLog$
 */

#ifndef	_DCED_PVTSVR_H
#define	_DCED_PVTSVR_H

#include <dce/dcedpvt.h>

/* ______________________________________________________________________ */

#define DCECF_POSTPROC		"dcecf_postproc"

/* ______________________________________________________________________ */

/* "copy constructor" memory allocator.
 */
typedef void *(*dced_alloc_func_t)(
    idl_size_t	/* size */
);

/* ______________________________________________________________________ */

/* Global definitions. */

extern volatile boolean32	dced_is_bootstrap;

extern uuid_t		uuid_g_nil_uuid;

/* for ACLs */

extern uuid_t		dced_config_type;
extern uuid_t		dced_config_acl;
extern uuid_t		dced_sv_obj_type;
extern uuid_t		dced_sv_acl;

/* for manager EPVs */

extern hostdata_v1_0_epv_t		dced_hostdata_v1_0_epv;
extern rkeytab_v1_0_epv_t		dced_rkeytab_v1_0_epv;
extern rs_attr_schema_v0_0_epv_t	dced_rs_attr_schema_v0_0_epv;
extern srvrconf_v1_0_epv_t		dced_srvrconf_v1_0_epv;
extern srvrexec_v1_0_epv_t		dced_srvrexec_v1_0_epv;

/* copying functions. */
extern	void	*dced_copy_via_malloc(idl_size_t);
extern	void	*dced_copy_for_stub(idl_size_t);
extern	void	dced_tower_copy(twr_t**, twr_t*,
				dced_alloc_func_t, error_status_t*);

/* simple function prototypes */

extern	void	dced_init_srvrexec(pthread_attr_t, error_status_t*);
extern	void	dced_start_boottime_servers(error_status_t*);
extern	void	dced_acl_patcher(uuid_t*, uuid_t*, error_status_t*);
extern	void	dced_fork(char**, pid_t*, error_status_t*);
extern	void	dced_waitpid(pid_t, int*, error_status_t*);
extern	void	dced_invoke(char**, int*, error_status_t*);
extern	void	dced_srvrexec__reap(void);
extern	void	dced_sleep(time_t);
extern	void	dced_sleep_until(struct timeval*, time_t);

/* security functions */
extern	boolean32	dced_authorized(handle_t, uuid_t*, uuid_t*, uuid_t*,
	sec_acl_permset_t, error_status_t*);
extern	boolean32	dced_server_auth(rpc_binding_handle_t, unsigned32,
	error_status_t*);

#endif	/* _DCED_PVTSVR_H */
