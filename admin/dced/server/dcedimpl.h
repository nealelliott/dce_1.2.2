/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dcedimpl.h,v $
 * Revision 1.1.8.3  1996/02/18  23:32:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:30  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:27:52  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/10  1995/10/10  16:57 UTC  sommerfeld
 * 	Merge out from HPDCE02..
 * 	[1995/10/05  20:07 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 * 
 * 	remove compute_addr_weight; it shouldn't be used outside the pe_update
 * 	module (yet).
 * 	[1995/09/08  22:37 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/9  1995/09/29  22:37 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  00:38 UTC  tatsu_s
 * 	Include dcedpvt_server.h instead of dcedpvt.h.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:30 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/8  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted yet another (probably not last) fix for CHFts15852/OT12494.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/1  1995/08/23  21:00 UTC  tatsu_s
 * 	Added no_remote_update and write_dced_bindings().
 * 
 * 	HP revision /main/HPDCE02/7  1995/07/20  22:18 UTC  mk
 * 	Merge dced-as-rpcd changes from mk_dced_as_rpcd/1.
 * 
 * 	HP revision /main/HPDCE02/mk_dced_as_rpcd/1  1995/07/11  15:20 UTC  mk
 * 	Allow dced to run as rpcd and export only ep services.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/31  13:25 UTC  tatsu_s
 * 	Submitted the fix for OT12494/CHFts13959 .
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/1  1995/05/12  18:04 UTC  tatsu_s
 * 	OT12494 (CHFts13959).
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/2  1995/05/11  21:32 UTC  tatsu_s
 * 	Closed a small security hole in bootstrap phase.
 * 	Added subsys/dce/dced-admin and a guessed cell admin in the default ACL.
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/09  21:17 UTC  tatsu_s
 * 	Submitted the fix for CHFts14942 and CHFts14945.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/1  1995/05/05  20:34 UTC  tatsu_s
 * 	Added the private thread pools.
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/04  17:26 UTC  mdf
 * 	Merge for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/04  18:13 UTC  tatsu_s
 * 	Submitted dced shutdown fix.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.mothra_cn_fix.b0/1  1995/01/04  16:34 UTC  tatsu_s
 * 	Fixed the shutdown.
 * 
 * 	HP revision /main/HPDCE02/2  1994/12/22  19:06 UTC  tatsu_s
 * 	Submitted the icmp monitor.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s_icmp_monitor/1  1994/12/21  20:30 UTC  tatsu_s
 * 	Added dced_is_remote_handle().
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/07  17:16 UTC  mk
 * 	Merge in changes from mk_mothra_dced/1.
 * 
 * 	HP revision /main/mk_mothra_dced/1  1994/11/29  15:14 UTC  mk
 * 	Include rutc.h.
 * 	[1995/12/08  17:59:55  root]
 * 
 * Revision 1.1.4.1  1994/10/26  19:54:35  rsalz
 * 	Add EPmap convenience abstractions (needed to fix OT CR 12061).
 * 	[1994/10/26  19:42:57  rsalz]
 * 
 * Revision 1.1.2.36  1994/10/03  19:58:40  rsalz
 * 	Put a mutex around secval operations; make explicit init func.
 * 	[1994/10/03  19:56:35  rsalz]
 * 
 * Revision 1.1.2.35  1994/09/22  22:00:31  bowe
 * 	Remove unused macros. [CR 11725]
 * 	[1994/09/22  21:42:43  bowe]
 * 
 * Revision 1.1.2.34  1994/09/17  08:05:09  rsalz
 * 	Store dced object UUID in a file (OT CR 12127).
 * 	Remote ep-compat fake server (OT CR 12140).
 * 	[1994/09/17  07:56:32  rsalz]
 * 
 * Revision 1.1.2.33  1994/09/12  15:58:37  baum
 * 	Fixed problems caused by adding obj uuidd to dced bindings
 * 	[1994/09/12  15:43:03  baum]
 * 
 * Revision 1.1.2.32  1994/09/06  18:15:26  bowe
 * 	Added DCECF_POSTPROC. [CR 11705]
 * 	[1994/09/06  18:14:32  bowe]
 * 
 * Revision 1.1.2.31  1994/09/02  19:52:10  bowe
 * 	Declare obj vector (used for registering svc obj). [CR 11979]
 * 	[1994/09/02  19:45:44  bowe]
 * 
 * Revision 1.1.2.30  1994/08/28  14:40:20  rsalz
 * 	Use dce_gettimeofday not dced_get_the_time (OT CR 11931).
 * 	[1994/08/28  14:34:24  rsalz]
 * 
 * Revision 1.1.2.29  1994/08/28  00:58:36  rsalz
 * 	hd_post_proc passes in the item name through dced_fork (OT CR 11895).
 * 	[1994/08/26  19:13:55  rsalz]
 * 
 * Revision 1.1.2.28  1994/08/25  16:32:12  rsalz
 * 	Start boottime servers.
 * 	Add dced_waitpid and fix hostdata postprocessor reaping.
 * 	[1994/08/25  16:29:44  rsalz]
 * 
 * Revision 1.1.2.27  1994/08/24  19:51:45  bowe
 * 	Prototype for string_copy() (now non-staic). [CR 11791]
 * 	[1994/08/24  19:48:02  bowe]
 * 
 * Revision 1.1.2.26  1994/08/24  16:41:02  baum
 * 	Full dced service on bootup.  Secval on by default (OT 11726 & 11656)
 * 	[1994/08/24  16:34:59  baum]
 * 
 * Revision 1.1.2.25  1994/07/29  19:56:05  rsalz
 * 	Create keytab for host principal at init time (OT CR 11465).
 * 	Add local_ep_register for CN binding to dced (OT CR 11211).
 * 	Create ep-compat server in srvrexec (partial).
 * 	[1994/07/29  19:36:44  rsalz]
 * 
 * Revision 1.1.2.24  1994/07/07  19:14:57  bowe
 * 	Added privacy_required, dced_privacy_ok(). [CR 11201]
 * 	[1994/07/07  19:11:42  bowe]
 * 
 * Revision 1.1.2.23  1994/07/06  20:17:18  pwang
 * 	Reorg dced free routines [OT#11003]
 * 	[1994/07/06  20:10:14  pwang]
 * 
 * Revision 1.1.2.22  1994/06/22  20:27:53  rsalz
 * 	Add "export into namespace" into acl_patcher.
 * 	[1994/06/22  20:14:09  rsalz]
 * 
 * Revision 1.1.2.21  1994/06/16  19:35:07  rsalz
 * 	Add free functions (temporary until integrated into dced API).
 * 	[1994/06/16  19:31:42  rsalz]
 * 
 * Revision 1.1.2.20  1994/06/13  20:05:30  rsalz
 * 	Move epdb_get_the_time to dced_get_the_time.
 * 	Make pthread.h inclusion optional (for old EP files).
 * 	[1994/06/13  20:05:17  rsalz]
 * 
 * Revision 1.1.2.19  1994/06/03  19:17:11  rsalz
 * 	Add "acl patcher" (OT CR 10858).
 * 	[1994/06/03  19:10:44  rsalz]
 * 
 * Revision 1.1.2.18  1994/05/26  19:42:09  rsalz
 * 	Merged with changes from 1.1.2.17
 * 	[1994/05/26  19:41:46  rsalz]
 * 
 * 	EP functionality.
 * 	[1994/05/26  19:35:39  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:32:55  rsalz]
 * 
 * Revision 1.1.2.17  1994/05/18  21:00:21  bowe
 * 	Added #include <dced_util.h>
 * 	[1994/05/18  20:59:41  bowe]
 * 
 * Revision 1.1.2.16  1994/05/12  19:30:28  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:26  rsalz]
 * 
 * Revision 1.1.2.15  1994/05/11  20:11:16  rsalz
 * 	More functionality; code cleanups.
 * 	[1994/05/11  20:08:21  rsalz]
 * 
 * Revision 1.1.2.14  1994/05/10  20:15:09  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:38  rsalz]
 * 
 * Revision 1.1.2.13  1994/05/05  20:07:31  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:00:57  rsalz]
 * 
 * Revision 1.1.2.12  1994/04/27  22:08:10  bowe
 * 	Correct value of DCED_DIRECTORY
 * 	[1994/04/27  22:00:23  bowe]
 * 
 * Revision 1.1.2.11  1994/04/27  19:41:34  rsalz
 * 	Add dce_assert, set up headers right
 * 	[1994/04/27  19:38:07  rsalz]
 * 
 * Revision 1.1.2.10  1994/04/21  03:55:14  rsalz
 * 	Remove verbose variable; add sv_purge_context() declaration.
 * 	[1994/04/21  03:52:08  rsalz]
 * 
 * Revision 1.1.2.9  1994/04/18  15:05:36  bowe
 * 	New variable: dced_dir
 * 	[1994/04/18  15:00:27  bowe]
 * 
 * Revision 1.1.2.8  1994/04/11  20:22:04  rsalz
 * 	Final set of RFC revision fixes.
 * 	[1994/04/11  20:08:23  rsalz]
 * 
 * Revision 1.1.2.7  1994/04/11  18:45:55  rsalz
 * 	Update to final RFC revision.
 * 	[1994/04/11  18:26:27  rsalz]
 * 
 * Revision 1.1.2.6  1994/04/06  21:17:13  bowe
 * 	Added thishost_prin_uuid
 * 	[1994/04/06  21:15:14  bowe]
 * 
 * Revision 1.1.2.5  1994/04/06  15:12:11  bowe
 * 	Added includes for svc
 * 	[1994/04/06  15:10:44  bowe]
 * 
 * Revision 1.1.2.4  1994/03/29  22:31:24  bowe
 * 	Added typedefs, variables, and func prototype for name->uuid caches.
 * 	[1994/03/29  22:20:23  bowe]
 * 
 * Revision 1.1.2.3  1994/03/18  19:57:17  rsalz
 * 	Put permset bits in dced_aclbits.idl (OT CR 9768)
 * 	[1994/03/18  19:56:09  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:05  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:55:59  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:46  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:17  bowe]
 * 
 * $EndLog$
 */

#ifndef _DCEDIMPL_H
#define _DCEDIMPL_H

/* System header files. */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/param.h>
#if	!defined(NO_THREADS_HEADER)
#include <pthread.h>
#endif	/* !defined(NO_THREADS_HEADER) */

/* DCE facilities we use. */
#include <dce/dce.h>
#include <dce/dce_utils.h>
#define DCE_ASSERT
#include <dce/assert.h>
#include <dce/dce_msg.h>
#include <dce/dbif.h>
#include <dce/dce_cf.h>
#include <dce/dceacl.h>
#include <dce/aclif.h>

/* DCE facilities we provide. */
#include <dce/dced_data.h>
#include <hostdata.h>
#include <rkeytab.h>
#include <srvrconf.h>
#include <srvrexec.h>
#include <secval.h>
#include <rs_attr_schema.h>
#include <dce/rutc.h>

/* ACL definitions. */
#include <dce/dced_aclbits.h>

/* Implementation details. */
#include <dce/dcedhdmsg.h>
#include <dcedhdsvc.h>
#include <dcedhdmac.h>
#include <dce/dce_lock.h>
#include <dce/dced_util.h>

/* Global variables. */
#include <dce/dcedpvt_server.h>

/* ______________________________________________________________________ */

/* for convenience */
#define CHECK_STAT_RET(st)	if ((st) != error_status_ok) return;

/* ______________________________________________________________________ */

typedef struct service_table_s_t {
    rpc_if_handle_t	*ifspec;
    boolean32		namespace;
    boolean32		register_at_bootstrap;
    char		*annote;
    rpc_mgr_epv_t	mgrepv;
    void		*pool;
    boolean		registered;
    boolean		exported;
} service_table_t;

/*
**  An "unpacked" tower.
*/
typedef struct twr_fields_s_t {
    rpc_if_id_t		ifid;
    rpc_syntax_id_t	drep;
    unsigned32		rpc_protocol;
    unsigned32		rpc_protocol_vers_major;
    unsigned32		rpc_protocol_vers_minor;
    unsigned32		protseq;
} twr_fields_t;
extern void tower_to_fields(twr_t*, twr_fields_t*, error_status_t*);
extern boolean32 compatible_syntax_ids(rpc_syntax_id_t*, rpc_syntax_id_t*);


/* ______________________________________________________________________ */

#if     !defined(EXTERN)
#define EXTERN extern
#endif  /* !defined(EXTERN) */

EXTERN unsigned char	**my_addresses;
EXTERN int		my_address_count;

EXTERN boolean		do_ep_only;

EXTERN rpc_binding_vector_t	*dced_bindings;
EXTERN pthread_attr_t	thread_attr;		/* For threads we create */

/* for initializing data */

EXTERN uuid_t		dced_g_object;
EXTERN uuid_vector_t	dced_g_object_vector;

extern int		sizeof_services_table;
extern int		sizeof_bootservices_table;
extern service_table_t	services[];
extern service_table_t	bootservices[];

extern int		sizeof_ept_services_table;
extern service_table_t	ept_services[];

/* simple function prototypes */
int		main(int, char**);
#ifndef	HPDCE_DCED_FIX_OT12494
void		acl_patcher(uuid_t*, uuid_t*, error_status_t*);
#else
void		acl_patcher(error_status_t *);
#endif	/* HPDCE_DCED_FIX_OT12494 */
int		compute_addr_weight (unsigned_char_p_t);
void		dced_ept_init(error_status_t*);
void		export_to_namespace(service_table_t*, int, error_status_t*);
void		local_ep_register(service_table_t*, error_status_t*);
void		init_secval(error_status_t*);
void		remove_bindings_file(void);
void		sv_purge_context(void);
boolean32	dced_is_remote_handle(handle_t, error_status_t*);
void		prepare_stop_listening(void);

/* security functions */
void		write_dced_bindings(error_status_t *);

#endif
