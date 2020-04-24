/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: main.c,v $
 * Revision 1.1.12.2  1996/10/03  17:51:17  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:44 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	rsec_pk_cache version is now v1_1
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	merge fixes for secval deadlock and [de]activate failures
 * 	[1996/08/16  19:39 UTC  lmm  /main/DCE_1.2.2/lmm_dced_secval_merge/1]
 *
 * 	merge fix for CHFts17901 from HP branch
 *
 * 	Submitted the fix for CHFts17901.
 *
 * 	Workaround for dce_config. Tell secval if in bootstrap or not.
 * 	[1996/03/19  19:26 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/4]
 *
 * 	Check the return value from sec_clientd().
 * 	(Retry if true.)
 * 	[1996/03/19  15:05 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/3]
 *
 * 	Cleaned up the shutdown sequence.
 * 	Added the lock for the service tables.
 * 	Added wait_for_signel() to make infinite loops interruptible.
 * 	Some serviceability cleanup.
 * 	[1996/03/14  15:59 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/2]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Merge sec_pubkey and rsec_pk_cache
 * 	[1996/05/10  15:59 UTC  aha  /main/DCE_1.2/aha_pk2/2]
 *
 * 	Debug
 * 	[1996/05/07  20:08 UTC  aha  /main/DCE_1.2/aha_pk2/1]
 *
 * 	Additions for public key login: KDC public key cache service
 * 	[1996/01/03  19:38 UTC  psn  /main/DCE_1.2/1]
 *
 * 	Add KDC Public Key Retrieval to services table
 * 	for DCE Public Key Login
 * 	[1995/10/10  16:57 UTC  sommerfeld  /main/HPDCE02/13]
 *
 * 	Submitted the icmp monitor.
 * 	[1994/12/15  18:56 UTC  tatsu_s  /main/HPDCE02/tatsu_s_icmp_monitor/1]
 *
 * 	Call sigprocmask() in signal_catcher().
 * 	[1994/12/07  17:19 UTC  mk  /main/HPDCE02/1]
 *
 * 	Merge in changes from mk_mothra_dced/1.
 * 	[1994/11/29  15:16 UTC  mk  /main/mk_mothra_dced/1]
 *
 * 	Add rutc to services table.
 * 	[1994/10/26  19:54:40  rsalz  1.1.4.1]
 *
 * Revision 1.1.12.1  1996/07/08  18:20:51  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge sec_pubkey and rsec_pk_cache
 * 	[1996/05/10  15:59 UTC  aha  /main/DCE_1.2/aha_pk2/2]
 * 
 * 	Debug
 * 	[1996/05/07  20:08 UTC  aha  /main/DCE_1.2/aha_pk2/1]
 * 
 * 	Additions for public key login: KDC public key cache service
 * 	[1996/01/03  19:38 UTC  psn  /main/DCE_1.2/1]
 * 
 * 	Submitted the fix for CHFts16512.
 * 	[1995/09/29  22:38 UTC  tatsu_s  /main/HPDCE02/12]
 * 
 * 	Don't block in switch_service_mode().
 * 	[1995/09/23  00:39 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/2]
 * 
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/09/19  15:31 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/1]
 * 
 * 	Submitted the fix for CHFts16157.
 * 	[1995/09/08  15:01 UTC  tatsu_s  /main/HPDCE02/11]
 * 
 * 	Backed out dced_wait_for_register() change.
 * 	[1995/09/06  19:00 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_startup.b0/2]
 * 
 * 	Cleanup the namespace upon startup.
 * 	Let bootstrap_mgmt thread register auth_info.
 * 	[1995/09/06  03:25 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_startup.b0/1]
 * 
 * 	Submitted the fix for CHFts16168/CHFts16055.
 * 	[1995/08/29  22:27 UTC  tatsu_s  /main/HPDCE02/10]
 * 
 * 	Always call read_object_uuid().
 * 	[1995/08/29  20:07 UTC  tatsu_s  /main/HPDCE02/tatsu_s.psock_timeout.b0/2]
 * 
 * 	Added no_remote_update and write_dced_bindings().
 * 	Always initialize sc/sx_table.lock.
 * 	[1995/08/23  21:10 UTC  tatsu_s  /main/HPDCE02/tatsu_s.psock_timeout.b0/1]
 * 
 * 	Merge changes to touch /tmp/llbdbase.dat from mk_dced_as_rpcd/2.
 * 	[1995/07/27  23:24 UTC  mk  /main/HPDCE02/9]
 * 
 * 	Create /tmp/llbdbase.dat on startup and unlink on shutdown.  Also, start
 * 	signal_catcher thread even in do_ep_only mode.
 * 	[1995/07/27  23:20 UTC  mk  /main/HPDCE02/mk_dced_as_rpcd/2]
 * 
 * 	Merge dced-as-rpcd changes from mk_dced_as_rpcd/1.
 * 	[1995/07/20  22:01 UTC  mk  /main/HPDCE02/8]
 * 
 * 	Allow dced to run as rpcd and export only ep services.
 * 	[1995/07/11  15:20 UTC  mk  /main/HPDCE02/mk_dced_as_rpcd/1]
 * 
 * 	Submitted the fix for OT12494/CHFts13959 .
 * 	[1995/05/31  13:25 UTC  tatsu_s  /main/HPDCE02/7]
 * 
 * 	Removed guess_admin().
 * 	[1995/05/22  21:23 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_security.b0/4]
 * 
 * 	Merged up to .../HPDCE02/LATEST.
 * 	[1995/05/15  19:39 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_security.b0/3]
 * 
 * 	Merge fix to allow RPC_SUPPORTED_PROTSEQS=ncacn_ip_tcp from
 * 	mk_dced_udp_insistence/1.
 * 	[1995/05/10  20:49 UTC  mk  /main/HPDCE02/6]
 * 
 * 	Ignore status returned by rpc_network_is_protseq_valid("ncadg_ip_udp", ...)
 * 	so that dced can run with RPC_SUPPORTED_PROTSEQS=ncacn_ip_tcp.
 * 	[1995/05/09  21:38 UTC  mk  /main/HPDCE02/mk_dced_udp_insistence/1]
 * 
 * 	Adjusted umask().
 * 	[1995/05/13  01:51 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_security.b0/2]
 * 
 * 	OT12494 (CHFts13959).
 * 	[1995/05/12  21:21 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_security.b0/1]
 * 
 * 	Added umask(022).
 * 	[1995/05/12  15:00 UTC  tatsu_s  /main/HPDCE02/tatsu_s.scale_fix.b0/4]
 * 
 * 	Closed a small security hole in bootstrap phase.
 * 	Added subsys/dce/dced-admin and a guessed cell admin in the default ACL.
 * 	[1995/05/11  21:32 UTC  tatsu_s  /main/HPDCE02/tatsu_s.scale_fix.b0/3]
 * 
 * 	Submitted the fix for CHFts14942 and CHFts14945.
 * 	[1995/05/09  21:17 UTC  tatsu_s  /main/HPDCE02/5]
 * 
 * 	Conditionalized private thread pool creation.
 * 	[1995/05/08  15:05 UTC  tatsu_s  /main/HPDCE02/tatsu_s.scale_fix.b0/2]
 * 
 * 	Added the private thread pools.
 * 	[1995/05/05  20:39 UTC  tatsu_s  /main/HPDCE02/tatsu_s.scale_fix.b0/1]
 * 
 * 	Merge for CHFts13120
 * 	[1995/04/04  17:26 UTC  mdf  /main/HPDCE02/4]
 * 
 * 	Submitted dced shutdown fix.
 * 	[1995/01/04  18:13 UTC  tatsu_s  /main/HPDCE02/3]
 * 
 * 	Fixed the shutdown.
 * 	[1995/01/04  16:34 UTC  tatsu_s  /main/HPDCE02/tatsu_s.mothra_cn_fix.b0/1]
 * 
 * 	Submitted the icmp monitor.
 * 	[1994/12/22  19:06 UTC  tatsu_s  /main/HPDCE02/2]
 * 
 * 	Call sigprocmask() in signal_catcher().
 * 	[1994/12/15  18:56 UTC  tatsu_s  /main/HPDCE02/tatsu_s_icmp_monitor/1]
 * 
 * 	Merge in changes from mk_mothra_dced/1.
 * 	[1994/12/07  17:19 UTC  mk  /main/HPDCE02/1]
 * 
 * 	Add rutc to services table.
 * 	[1994/11/29  15:16 UTC  mk  /main/mk_mothra_dced/1]
 * 
 * 	Add KDC Public Key Retrieval to services
 * 
 * Revision 1.1.8.3  1996/02/18  19:16:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:33  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  21:27:54  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/13  1995/10/10  16:57 UTC  sommerfeld
 * 	Merge out from HPDCE02..
 * 	[1995/10/05  20:06 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/5]
 * 
 * 	Rename functions.
 * 	[1995/08/31  05:59 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/3]
 * 
 * 	Performance: let secidmap do the rca_site_bind iff it's needed; use
 * 	parse_name instead of pgo_name_to_id since it's cached; get cell and
 * 	principal UUID in one call.
 * 	[1995/08/14  15:18 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/12  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/23  00:39 UTC  tatsu_s
 * 	Don't block in switch_service_mode().
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/11  1995/09/08  15:01 UTC  tatsu_s
 * 	Submitted the fix for CHFts16157.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_startup.b0/2  1995/09/06  19:00 UTC  tatsu_s
 * 	Backed out dced_wait_for_register() change.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_startup.b0/1  1995/09/06  03:25 UTC  tatsu_s
 * 	Cleanup the namespace upon startup.
 * 	Let bootstrap_mgmt thread register auth_info.
 * 
 * 	HP revision /main/HPDCE02/10  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted the fix for CHFts16168/CHFts16055.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/2  1995/08/29  20:07 UTC  tatsu_s
 * 	Always call read_object_uuid().
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/1  1995/08/23  21:10 UTC  tatsu_s
 * 	Added no_remote_update and write_dced_bindings().
 * 	Always initialize sc/sx_table.lock.
 * 
 * 	HP revision /main/HPDCE02/9  1995/07/27  23:24 UTC  mk
 * 	Merge changes to touch /tmp/llbdbase.dat from mk_dced_as_rpcd/2.
 * 
 * 	HP revision /main/HPDCE02/mk_dced_as_rpcd/2  1995/07/27  23:20 UTC  mk
 * 	Create /tmp/llbdbase.dat on startup and unlink on shutdown.  Also, start
 * 	signal_catcher thread even in do_ep_only mode.
 * 
 * 	HP revision /main/HPDCE02/8  1995/07/20  22:01 UTC  mk
 * 	Merge dced-as-rpcd changes from mk_dced_as_rpcd/1.
 * 
 * 	HP revision /main/HPDCE02/mk_dced_as_rpcd/1  1995/07/11  15:20 UTC  mk
 * 	Allow dced to run as rpcd and export only ep services.
 * 
 * 	HP revision /main/HPDCE02/7  1995/05/31  13:25 UTC  tatsu_s
 * 	Submitted the fix for OT12494/CHFts13959 .
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/4  1995/05/22  21:23 UTC  tatsu_s
 * 	Removed guess_admin().
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/3  1995/05/15  19:39 UTC  tatsu_s
 * 	Merged up to .../HPDCE02/LATEST.
 * 
 * 	HP revision /main/HPDCE02/6  1995/05/10  20:49 UTC  mk
 * 	Merge fix to allow RPC_SUPPORTED_PROTSEQS=ncacn_ip_tcp from
 * 	mk_dced_udp_insistence/1.
 * 
 * 	HP revision /main/HPDCE02/mk_dced_udp_insistence/1  1995/05/09  21:38 UTC  mk
 * 	Ignore status returned by rpc_network_is_protseq_valid("ncadg_ip_udp", ...)
 * 	so that dced can run with RPC_SUPPORTED_PROTSEQS=ncacn_ip_tcp.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/2  1995/05/13  01:51 UTC  tatsu_s
 * 	Adjusted umask().
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_security.b0/1  1995/05/12  21:21 UTC  tatsu_s
 * 	OT12494 (CHFts13959).
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/4  1995/05/12  15:00 UTC  tatsu_s
 * 	Added umask(022).
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/3  1995/05/11  21:32 UTC  tatsu_s
 * 	Closed a small security hole in bootstrap phase.
 * 	Added subsys/dce/dced-admin and a guessed cell admin in the default ACL.
 * 
 * 	HP revision /main/HPDCE02/5  1995/05/09  21:17 UTC  tatsu_s
 * 	Submitted the fix for CHFts14942 and CHFts14945.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/2  1995/05/08  15:05 UTC  tatsu_s
 * 	Conditionalized private thread pool creation.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.scale_fix.b0/1  1995/05/05  20:39 UTC  tatsu_s
 * 	Added the private thread pools.
 * 
 * 	HP revision /main/HPDCE02/4  1995/04/04  17:26 UTC  mdf
 * 	Merge for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/3  1995/01/04  18:13 UTC  tatsu_s
 * 	Submitted dced shutdown fix.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.mothra_cn_fix.b0/1  1995/01/04  16:34 UTC  tatsu_s
 * 	Fixed the shuts
 * 
 * Revision 1.1.4.1  1994/10/26  19:54:40  rsalz
 * 	Add rpc_ss_enable_allocate call to signal_catcher (OT CR 12714?).
 * 	Call srvrexec_init for kill(2)-based pinging.
 * 	[1994/10/26  19:43:17  rsalz]
 * 
 * Revision 1.1.2.53  1994/10/17  19:31:03  sommerfeld
 * 	OT11548: fix hole in local preauth protocol
 * 	[1994/10/17  19:28:53  sommerfeld]
 * 
 * Revision 1.1.2.52  1994/10/03  19:58:42  rsalz
 * 	Put a mutex around secval operations; make explicit init func.
 * 	Redirect stdin/out/err for production work (OT CR 11725).
 * 	[1994/10/03  19:56:46  rsalz]
 * 
 * Revision 1.1.2.51  1994/09/27  19:57:08  rsalz
 * 	Add DCED to RDACL and SVC annotations (OT CR 12368).
 * 	[1994/09/27  18:47:27  rsalz]
 * 
 * Revision 1.1.2.50  1994/09/22  22:00:33  bowe
 * 	More (final) SVC work [CR 11725]
 * 	[1994/09/22  21:42:50  bowe]
 * 
 * Revision 1.1.2.49  1994/09/19  21:32:26  baum
 * 	Moved chdir() (OT 12249)
 * 	[1994/09/19  21:32:10  baum]
 * 
 * Revision 1.1.2.48  1994/09/17  08:05:14  rsalz
 * 	Store dced object UUID in a file (OT CR 12127).
 * 	Remote ep-compat fake server (OT CR 12140).
 * 	Don't allow purge if initializing (OT CR 12187).
 * 	Serviceability (OT CR 11725).
 * 	Complain less often about RPC NSI unexport (OT CR 12216).
 * 	[1994/09/17  07:57:14  rsalz]
 * 
 * Revision 1.1.2.47  1994/09/12  15:58:40  baum
 * 	Dced services now registered in security conscious manner (OT 12032)
 * 	[1994/09/12  15:45:12  baum]
 * 
 * Revision 1.1.2.46  1994/09/02  19:52:11  bowe
 * 	Merged with changes from 1.1.2.45
 * 	[1994/09/02  19:52:01  bowe]
 * 
 * 	Register an object for serviceability interface. [CR 11979]
 * 	[1994/09/02  19:45:47  bowe]
 * 
 * Revision 1.1.2.45  1994/09/02  15:24:49  baum
 * 	Always export dced obj bindings to NS (OT 11963)
 * 	[1994/09/02  15:24:11  baum]
 * 
 * Revision 1.1.2.44  1994/08/28  14:40:22  rsalz
 * 	Use dce_gettimeofday not dced_get_the_time (OT CR 11931).
 * 	[1994/08/28  14:34:44  rsalz]
 * 
 * Revision 1.1.2.43  1994/08/26  20:15:14  bowe
 * 	Add dced_svc_v1_0_epv [CR 11725]
 * 	[1994/08/26  20:10:10  bowe]
 * 
 * Revision 1.1.2.42  1994/08/25  16:32:20  rsalz
 * 	Start boottime servers.
 * 	[1994/08/25  16:30:20  rsalz]
 * 
 * Revision 1.1.2.41  1994/08/24  19:55:06  bowe
 * 	Merged with changes from 1.1.2.40
 * 	[1994/08/24  19:55:00  bowe]
 * 
 * 	Print usage msg, then exit if bad arg is given. [CR 11833]
 * 	[1994/08/24  19:53:28  bowe]
 * 
 * Revision 1.1.2.40  1994/08/24  16:41:06  baum
 * 	Merged with changes from 1.1.2.39
 * 	[1994/08/24  16:40:38  baum]
 * 
 * 	Full dced service on bootup.  Secval on by default (OT 11726 & 11656)
 * 	[1994/08/24  16:35:02  baum]
 * 
 * Revision 1.1.2.39  1994/08/20  18:03:14  bowe
 * 	SVC work [CR 11725]
 * 	[1994/08/20  17:52:40  bowe]
 * 
 * Revision 1.1.2.38  1994/08/02  05:45:35  rsalz
 * 	Set thread stacksize in background() (OT CR 11421).
 * 	[1994/08/02  05:17:00  rsalz]
 * 
 * Revision 1.1.2.37  1994/07/29  19:56:11  rsalz
 * 	Add local_ep_register for CN binding to dced (OT CR 11211).
 * 	Do all threads work after forking (OT CR 11421).
 * 	Allow THREAD_STACK_SIZE to set pthread_attr stacksize (OT CR 11421).
 * 	[1994/07/29  19:37:44  rsalz]
 * 
 * Revision 1.1.2.36  1994/07/18  18:51:25  rsalz
 * 	Tweak service names.
 * 	Delay and retry in export_to_namespace (OT CR 11211).
 * 	[1994/07/18  18:47:19  rsalz]
 * 
 * Revision 1.1.2.35  1994/07/07  19:14:59  bowe
 * 	Set new flag: privacy_required. [CR 11201]
 * 	[1994/07/07  19:13:42  bowe]
 * 
 * Revision 1.1.2.34  1994/07/05  16:03:20  rsalz
 * 	Make sure we have name to export to (OT CR 11171).
 * 	[1994/07/05  15:52:40  rsalz]
 * 
 * Revision 1.1.2.33  1994/06/30  19:25:03  pwang
 * 	Turned on the secval's namespace flag [OT#11027]
 * 	[1994/06/30  19:23:59  pwang]
 * 
 * Revision 1.1.2.32  1994/06/22  20:27:54  rsalz
 * 	Merged with changes from 1.1.2.31
 * 	[1994/06/22  20:27:46  rsalz]
 * 
 * 	Add "export into namespace" into acl_patcher.
 * 	[1994/06/22  20:14:06  rsalz]
 * 
 * Revision 1.1.2.31  1994/06/22  19:27:19  bowe
 * 	set status=OK if sec_rgy_site_open fails
 * 	[1994/06/22  19:26:50  bowe]
 * 
 * Revision 1.1.2.30  1994/06/21  20:39:13  rsalz
 * 	Properly inq_bindings (OT CR 11027).
 * 	[1994/06/21  20:38:56  rsalz]
 * 
 * Revision 1.1.2.29  1994/06/21  19:02:02  rsalz
 * 	Properly register protseqs if not doing endpoitns (OT CR 11005).
 * 	[1994/06/21  19:01:16  rsalz]
 * 
 * Revision 1.1.2.28  1994/06/17  19:38:00  rsalz
 * 	Cleaner server and database registration.
 * 	[1994/06/17  19:30:34  rsalz]
 * 
 * Revision 1.1.2.27  1994/06/16  19:35:14  rsalz
 * 	Set defaults if sec_rgy not available.
 * 	Final word on registering protocol sequences (OT CR 10940).
 * 	[1994/06/16  19:32:09  rsalz]
 * 
 * 	More work on register protocol sequences in the right way.
 * 	[1994/06/14  17:09:57  rsalz]
 * 
 * Revision 1.1.2.26  1994/06/13  20:20:52  rsalz
 * 	Really get all_protseqs right
 * 	[1994/06/13  20:20:46  rsalz]
 * 
 * Revision 1.1.2.25  1994/06/13  20:10:30  rsalz
 * 	Fix all_protseqs call
 * 	[1994/06/13  20:10:22  rsalz]
 * 
 * Revision 1.1.2.24  1994/06/13  20:05:33  rsalz
 * 	Move epdb_get_the_time to dced_get_the_time.
 * 	Add sec_clientd interfaces to registration.
 * 	[1994/06/13  20:04:54  rsalz]
 * 
 * Revision 1.1.2.23  1994/06/06  19:54:19  rsalz
 * 	Finish ACL patcher (OT CR 10858).
 * 	[1994/06/06  19:52:30  rsalz]
 * 
 * Revision 1.1.2.22  1994/06/03  19:17:12  rsalz
 * 	Add "acl patcher" (OT CR 10858).
 * 	[1994/06/03  19:10:54  rsalz]
 * 
 * Revision 1.1.2.21  1994/06/02  20:12:59  rsalz
 * 	Be more verbose reporting failures.
 * 	[1994/06/02  20:10:10  rsalz]
 * 
 * Revision 1.1.2.20  1994/06/01  16:22:53  rsalz
 * 	Fix -e flag; make init more robust
 * 	[1994/06/01  16:22:32  rsalz]
 * 
 * Revision 1.1.2.19  1994/05/31  20:24:03  rsalz
 * 	Add -e flag; fix some EP bugs
 * 	[1994/05/31  20:22:52  rsalz]
 * 
 * Revision 1.1.2.18  1994/05/27  16:31:17  rsalz
 * 	Fix bootstrap phase; cleanup EP and LLB compat code
 * 	[1994/05/27  16:30:29  rsalz]
 * 
 * Revision 1.1.2.17  1994/05/26  19:42:20  rsalz
 * 	EP functionality.
 * 	[1994/05/26  19:35:50  rsalz]
 * 
 * 	Checkpointing.
 * 	[1994/05/18  19:33:00  rsalz]
 * 
 * Revision 1.1.2.16  1994/05/12  19:30:32  rsalz
 * 	Add some dce_server functions.
 * 	Add and use dced_authorized
 * 	More srvr implementation; export their interfaces
 * 	[1994/05/12  19:26:30  rsalz]
 * 
 * Revision 1.1.2.15  1994/05/11  20:11:22  rsalz
 * 	More functionality; code cleanups.
 * 	[1994/05/11  20:08:24  rsalz]
 * 
 * Revision 1.1.2.14  1994/05/10  20:15:14  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:44  rsalz]
 * 
 * Revision 1.1.2.13  1994/05/05  20:07:39  rsalz
 * 	Initial server functionality drop.
 * 	[1994/05/05  20:01:02  rsalz]
 * 
 * Revision 1.1.2.12  1994/04/27  22:08:07  bowe
 * 	Adjust use of DCED_DIRECTORY
 * 	[1994/04/27  22:00:04  bowe]
 * 
 * Revision 1.1.2.11  1994/04/27  19:41:37  rsalz
 * 	Add serviceability files and init
 * 	[1994/04/27  19:36:50  rsalz]
 * 
 * Revision 1.1.2.10  1994/04/26  20:00:19  rsalz
 * 	Rewrite "fork into background" logic (courtesy <tatsu_s@apollo.hp.com>).
 * 	[1994/04/26  19:59:59  rsalz]
 * 
 * Revision 1.1.2.9  1994/04/25  19:18:28  rsalz
 * 	Restore changes from ODE-destroyed 1.1.2.7
 * 	[1994/04/25  17:58:58  rsalz]
 * 
 * Revision 1.1.2.8  1994/04/21  03:55:16  rsalz
 * 	Re-do arg-parsing and signal-catching.
 * 	[1994/04/21  03:54:55  rsalz]
 * 
 * Revision 1.1.2.6  1994/04/06  21:17:19  bowe
 * 	Set global thishost_prin_uuid
 * 	[1994/04/06  21:16:44  bowe]
 * 
 * Revision 1.1.2.5  1994/03/29  22:31:26  bowe
 * 	Added calls to global data and cache init functions.
 * 	Handle "-v" arg.  Added casts to shut up compiler warnings.
 * 	[1994/03/29  22:22:32  bowe]
 * 
 * Revision 1.1.2.4  1994/03/25  18:49:45  bowe
 * 	Set a real rpc_mgmt auth function.  Exit instead of return if databases
 * 	can't be opened.  Uncomment attr schema database opening code.
 * 	[1994/03/25  18:39:23  bowe]
 * 
 * Revision 1.1.2.3  1994/03/18  19:51:30  bowe
 * 	Pass flags to calls to dce_db_open() as a variable, so it's easier to
 * 	change.  Only do db_c_create when initializing (with -i arg to dced).
 * 	[1994/03/18  19:38:46  bowe]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:07  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:02  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:25:56  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:26  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <dce/pthread_exc.h>
#define PTHREAD
#define NO_THREADS_HEADER

#define EXTERN
#include <dcedimpl.h>
#include <svimpl.h>
#include <dce/ep.h>
#if	defined(WANT_LLB)
#include <lb.h>
#endif	/* defined(WANT_LLB) */

#include <dce/stubbase.h>
#include <dce/rpcpvt.h>
#include <dce/rdaclifv0.h>
#include <dce/binding.h>
#include <dce/secidmap.h>
#include <dce/pgo.h>
#include <dce/acct.h>
#include <dce/misc.h>
#include <sys/wait.h>
#include <rsec_cert.h>
#include <rsec_login.h>
#include <dce/rsec_pk_cache.h>
#include <roverride.h>


/* Forward private definitions. */
/* Private thread pools */
static	rpc_thread_pool_handle_t tp_mgmt_pool = NULL;
static	rpc_thread_pool_handle_t tp_ept_pool = NULL;
static	rpc_thread_pool_handle_t tp_secval_pool = NULL;

#ifndef	HPDCE_LLB_AUDIO
static const char llbdbase_path[]="/tmp/llbdbase.dat";
#endif	/* HPDCE_LLB_AUDIO */

/* External refrences. */
extern rdaclif_v1_0_epv_t	dce_acl_v1_0_epv;
extern rdaclif_v0_0_epv_t	dce_acl_v0_0_epv;
extern ept_v3_0_epv_t		ept_v3_0_mgr_epv;
extern serviceability_v1_0_epv_t	dced_svc_v1_0_epv;

/* Workaround for dce_config with secval status/activate. */
extern boolean	sv_first_enable_done;

/* Forward references. */
static void	parse_args(int, char **);
static void	register_servers(service_table_t*, int,error_status_t*);
static void	unregister_server(service_table_t*, error_status_t*);
static void	*signal_catcher(void *);
static void	background(void);
static void 	*bootstrap_mgmt(void *arg);
static void	dced_my_addrs_init(error_status_t*);
static void	pool_lookup(uuid_p_t, rpc_if_id_p_t if_id,
			    unsigned32, rpc_thread_pool_handle_t*,
			    unsigned32*);
static void	switch_service_mode(void);

service_table_t ept_services[] = {
    { &ept_v3_0_s_ifspec, FALSE, TRUE, "Endpoint Resolution",
      (rpc_mgr_epv_t)&ept_v3_0_mgr_epv, &tp_ept_pool },
#  if defined(WANT_LLB)
    { &llb__v4_0_s_ifspec, FALSE, TRUE, "NCS1.5 Local Location Broker",
	(rpc_mgr_epv_t)&llb_v4_0_mgr_epv, NULL }
#  endif /* defined(WANT_LLB) */
};

static const int num_secval_services = 5;

service_table_t		services[] = {
    /* Don't change order of first five services! */
    { &rsec_cert_v1_0_s_ifspec, TRUE, FALSE, "Security Certification",
      NULL, &tp_secval_pool },
    { &rsec_login_v2_0_s_ifspec, TRUE, FALSE, "Login Pre-Authentication",
      NULL, &tp_secval_pool },
    { &roverride_v1_0_s_ifspec, TRUE, FALSE, "Password Override",
      NULL, &tp_secval_pool },
    { &secval_v1_0_s_ifspec, TRUE, FALSE, "Security Validation",
      NULL, &tp_secval_pool },
    { &rsec_pk_cache_v1_1_s_ifspec, TRUE, FALSE, "KDC Key Cache",
      NULL, &tp_secval_pool },
    { &hostdata_v1_0_s_ifspec, TRUE, FALSE, "Host Data",
      (rpc_mgr_epv_t)&dced_hostdata_v1_0_epv, NULL },
    { &rkeytab_v1_0_s_ifspec, TRUE, FALSE, "Remote Keytable",
      (rpc_mgr_epv_t)&dced_rkeytab_v1_0_epv, NULL },
    { &rs_attr_schema_v0_0_s_ifspec, FALSE, TRUE, "Extended Attribute Schema",
      (rpc_mgr_epv_t)&dced_rs_attr_schema_v0_0_epv, NULL },
    { &srvrconf_v1_0_s_ifspec, TRUE, FALSE, "Server Configuration",
      (rpc_mgr_epv_t)&dced_srvrconf_v1_0_epv, NULL },
    { &srvrexec_v1_0_s_ifspec, TRUE, FALSE, "Server Execution",
      (rpc_mgr_epv_t)&dced_srvrexec_v1_0_epv, NULL },
    { &rdaclif_v1_0_s_ifspec, FALSE, TRUE, "DCED ACL Manager",
      (rpc_mgr_epv_t)&dce_acl_v1_0_epv, NULL },
    { &rdaclif_v0_0_s_ifspec, FALSE, TRUE, "Pre-1.1 DCED ACL Manager",
      (rpc_mgr_epv_t)&dce_acl_v0_0_epv, NULL },
    { &rutc_v0_0_s_ifspec, FALSE, TRUE, "Remote UTC",
      NULL, NULL },
    { &serviceability_v1_0_s_ifspec, TRUE, FALSE, "DCED Serviceability Control",
      (rpc_mgr_epv_t)&dced_svc_v1_0_epv, NULL }
};
int sizeof_services_table = sizeof services / sizeof services[0];

int sizeof_ept_services_table = sizeof ept_services / sizeof ept_services[0];


/* Private definitions. */
static boolean		in_foreground;		/* run in foreground? */
static boolean		endpoints = TRUE;
static char		**protseqs;
static boolean		debug = FALSE;
static boolean		initialize_databases = FALSE;
static boolean		do_secval = TRUE;
static boolean		purging = FALSE;
static boolean		remote_update = FALSE;
static boolean		privacy_required = TRUE;
static volatile boolean	server_is_ready = FALSE;
static volatile boolean	full_service = TRUE;

/* server_is_shutdown can be set to TRUE by any thread without the
 * lock, but can't be reset to FALSE. */
static volatile boolean	server_is_shutdown = FALSE;

/* The lock for the service tables.
 *
 * The service tables need to be protected because there are potentially
 * three threads which may modify them, i.e., main, signal_catcher and
 * call executor (running mgmt call), and one thread which reads them,
 * RPC runtime's listener. (Note: The signal_catcher and the call
 * executor threads call prepare_stop_listening() because if the endpoint
 * mapper stops listening rpc_ns_binding_unexport() may fail. The RPC
 * runtime's listener thread can't take the lock because we can't block
 * it.) */
static dce_lock_t	service_table_lock;

/* Synchronization primitive for wait_for loop. */
static pthread_mutex_t	wait_for_mutex;
static pthread_cond_t	wait_for_cond;


/* ______________________________________________________________________ */

static boolean
wait_for_signal(time_t secs)
{
    struct timeval	now;
    struct timespec	ts;
    int			i;
    boolean		is_signaled = FALSE;

    dce_gettimeofday(&now);
    ts.tv_sec = now.tv_sec + secs;
    ts.tv_nsec = 0;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug2,
		   "(wait_for_signal) sleeping for %d second(s)...", secs));

    pthread_mutex_lock(&wait_for_mutex);
    i = pthread_cond_timedwait(&wait_for_cond, &wait_for_mutex, &ts);
    if (i == 0) {	/* signaled */
	is_signaled = TRUE;
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug2,
		       "(wait_for_signal) signaled"));
    }
#ifdef	PTHREAD_EXC
    else if (i == EAGAIN) {	/* timeout */
#else
    else if (i == -1 && errno == EAGAIN) {	/* timeout */
#endif	/* PTHREAD_EXC */
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug2,
		       "(wait_for_signal) awake (timeout)"));
    }
    else {
	/* Otherwise, ignore for now */
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		       "(wait_for_signal) awake (%d, (%d) %s)",
		       i, errno, strerror(errno)));
    }
    pthread_mutex_unlock(&wait_for_mutex);

    return is_signaled;
}

static void
dced_wait_for_sec(
    uuid_t			*admin_uuid,
    uuid_t			*admin_group_uuid,
    error_status_t		*st
)
{
    sec_rgy_bind_auth_info_t	auth;
    void			*key;
    error_status_t		st2;

    /* Initialize */
    if (admin_uuid != NULL)
	uuid_create_nil(admin_uuid, &st2);
    if (admin_group_uuid != NULL)
	uuid_create_nil(admin_group_uuid, &st2);

    auth.info_type = sec_rgy_bind_auth_none;

    /* Wait for HOST/self principal to be available. */
    for ( ; ; ) {
	sec_id_global_parse_name (&auth,
	    (unsigned_char_p_t)dced_g_info.thishost_prin, NULL,
	    &dced_g_info.this_cell_uuid, NULL,
	    &dced_g_info.thishost_prin_uuid, st);
	if (*st == error_status_ok)
	    break;
	dce_svc_printf(DCED_S_WAITING_FOR_SOMETHING_MSG,
		       "machine principal", (long)*st);
	if (wait_for_signal(10)) {
	    *st = dced_s_startup_interrupted;
	    return;
	}
    }

    /* Wait for HOST/self keytab to be available */
    for ( ; ; ) {
	sec_key_mgmt_get_key(rpc_c_authn_dce_secret, NULL,
	    (unsigned_char_p_t)dced_g_info.thishost_prin,
            0, &key, st);
	if (*st == error_status_ok) {
	    sec_key_mgmt_free_key(key, st);
	    break;
	}
	dce_svc_printf(DCED_S_WAITING_FOR_SOMETHING_MSG,
		       "machine keytab", (long)*st);
	if (wait_for_signal(10)) {
	    *st = dced_s_startup_interrupted;
	    return;
	}
    }

    if (admin_group_uuid != NULL) {
	sec_id_global_parse_group(&auth, (unsigned_char_p_t)"subsys/dce/dced-admin",
			    NULL, NULL, NULL, admin_group_uuid, &st2);
	if (st2 != error_status_ok) {
	    dce_svc_printf(DCED_S_INIT_FUNCTION_FAIL_MSG,
			   "lookup subsys/dce/dced-admin", (long)st2);
	    uuid_create_nil(admin_group_uuid, &st2);
	}
	else {
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_aclmgr, svc_c_debug1,
			   "subsys/dce/dced-admin found"));
	}
    }
    if (admin_uuid != NULL) {
	/* No way to obtain admin principal. Hope in future... */
    }
}

static void
dced_wait_for_register(
    error_status_t	*st
)
{
    for ( ; ; ) {
	rpc_server_register_auth_info((idl_char *)dced_g_info.thishost_self,
	    rpc_c_authn_dce_secret, NULL, NULL, st);
	if (*st == error_status_ok)
	    break;
	dce_svc_printf(DCED_S_WAITING_FOR_SOMETHING_MSG,
		       "rpc_server_register_auth_info", (long)*st);
	if (wait_for_signal(10)) {
	    *st = dced_s_startup_interrupted;
	    return;
	}
    }
}


static void
check_protseq_error(
    error_status_t	st,
    char		*ps
)
{
    if (st == error_status_ok)
	return;
    dce_svc_printf(DCED_S_CANT_USE_PROTSEQ_MSG, ps, strerror(errno));
    exit(1);
}
/* qsort helper function */

static int charpsort (const void *a, const void *b)
{
    char  **pa = (char **)a;
    char  **pb = (char **)b;

    return strcmp(*pa, *pb);
}

static boolean32
mgmt_authorization(
    rpc_binding_handle_t	h,
    unsigned32			op,
    error_status_t		*st
)
{
    boolean32			ok = FALSE;

    *st = error_status_ok;

    /* Since we are here, we are listening. However, if the
     * check_listener hasn't noticed it, don't stop, yet.
     * Note: Once the check_listener sets server_is_ready flag, only
     * the main thread can change it. */
    if (dced_is_bootstrap) {
	if (op == rpc_c_mgmt_stop_server_listen) {
	    if (server_is_ready) {
		prepare_stop_listening();
		server_is_shutdown = TRUE;
		ok = TRUE;
	    }
	}
	else
	    ok = TRUE;
	return ok;
    }

    ok = dced_server_auth(h, op, st);
    if (*st == error_status_ok && ok && op == rpc_c_mgmt_stop_server_listen) {
	if (server_is_ready) {
	    prepare_stop_listening();
	    server_is_shutdown = TRUE;
	}
	else
	    ok = FALSE;
    }

    return ok;
}

int
main(int ac, char **av)
{
    error_status_t	st;
    pthread_t		tid;
    int			i;
    char		**pp;
    char *ourname = NULL ;
    const char *rpcdname = "rpcd";

    do_ep_only = FALSE;

    ourname = strrchr(av[0], '/') ;
    if (ourname) 
	ourname++;
    else
	ourname = av[0];

    if (strcmp(ourname, rpcdname) == 0) {
        do_ep_only = TRUE;
	full_service = FALSE;
    }

    /* Initialize */
    (void)umask(S_IWGRP|S_IWOTH);

    /* Basic setup. */
    dce_svc_set_progname("dced", &st);
#if 0
    dce_msg_define_msg_table(dced_svc_g_table,
	sizeof dced_svc_g_table / sizeof(dce_msg_table_t), &st);
    dhd_svc_handle = dce_svc_register(dhd_svc_table, (idl_char *)"dhd", &st);
#endif
    parse_args(ac, av);

    dced_init_global_data(do_ep_only, privacy_required,
			  TRUE, remote_update, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "global data", (long)st);
    if (purging || do_secval) {
      sv_purge_context();
      remove_bindings_file();
    }

    if (!debug && chdir(dced_g_info.dir_path) < 0)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "working directory", (long)errno);
    if (!in_foreground)
	background();

    /* read_object_uuid() will create a new uuid, if can't be read. */
    dced_read_object_uuid(&dced_g_object, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "reading object UUID", (long)st);

    dced_g_object_vector.count   = 1;
    dced_g_object_vector.uuid[0] = &dced_g_object;

#if	defined(THREAD_STACK_SIZE)
    pthread_attr_create(&thread_attr);
    pthread_attr_setstacksize(&thread_attr, THREAD_STACK_SIZE);
#else
    thread_attr = pthread_attr_default;
#endif	/* defined(THREAD_STACK_SIZE) */

    rpc_network_is_protseq_valid((idl_char *)"ncadg_ip_udp", &st);
    /*
     * this rpc_network_is_protseq_valid() call is solely to initialize
     *   the RPC runtime, so ignore the returned status
     */

    dce_lock_create(&service_table_lock, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "dce_lock_create(&service_table_lock)", (long)st);
    if (pthread_mutex_init(&wait_for_mutex, pthread_mutexattr_default) < 0)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "pthread_mutex_init(&wait_for_mutex)",
		       (long)dced_s_mutex_init_fail);
    if (pthread_cond_init(&wait_for_cond, pthread_condattr_default) < 0)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
		       "pthread_cond_init(&wait_for_cond)",
		       (long)dced_s_cond_init_fail);

    if (!do_ep_only) {
	dced_open_databases(initialize_databases, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "open databases", (long)st);
    }
#ifndef	HPDCE_LLB_AUDIO
    /*
     * Create /tmp/llbdbase.dat if it does not already exist,
     * to placate old Audio applications that have a wired in
     * dependency on this file.
     */
    if (endpoints && access(llbdbase_path, F_OK) != 0) {
        int f;
	if ((f = creat(llbdbase_path, 0444)) == -1) {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, "%s%s"), dhd_s_general,
		svc_c_sev_warning, dced_s_init_create_file_fail, llbdbase_path,
		strerror(errno));
	}
        else {
            close(f);
        }
    }
#endif	/* HPDCE_LLB_AUDIO */
    if (!do_ep_only) {
	dced_setup_acls(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "setup ACLs", (long)st);
	dced_init_name_caches(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "initialize caches", (long)st);
	init_secval(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "initialize secval data", (long)st);
    }

    if (initialize_databases) {
	dced_init_databases(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "Databases", (long)st);
	return 0;
    }

    if (!do_ep_only) {
	dced_hd_read_post_processors(&st);
	if (st == dced_s_no_postprocessors)
	    dce_svc_printf(DCED_S_POSTPROCESSORS_NOT_SET_MSG);
	else if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "load post-processors", (long)st);

	rpc_mgmt_set_authorization_fn(mgmt_authorization, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "RPC authorization", (long)st);
    }

    /* Register protseqs and endpoints. */
    if (!endpoints) {
	rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default, &st);
	check_protseq_error(st, "*all*");
    }
    else if (protseqs)
	for (pp = protseqs; *pp; pp++) {
	    rpc_server_use_protseq_if((idl_char *)*pp,
		rpc_c_protseq_max_reqs_default, ept_v3_0_s_ifspec, &st);
	    check_protseq_error(st, *pp);
	}
    else {
	rpc_server_use_all_protseqs_if(rpc_c_protseq_max_reqs_default,
	    ept_v3_0_s_ifspec, &st);
	check_protseq_error(st, "*all*");
    }
    rpc_server_inq_bindings(&dced_bindings, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "get bindings", (long)st);
    dced_my_addrs_init(&st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "get bindings", (long)st);

    /* Register services. */
    if (endpoints) {
	dced_ept_init(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "EP service", (long)st);
	register_servers(ept_services, sizeof_ept_services_table, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "register EP service", (long)st);
    }

    if (!do_ep_only) {
	i = do_secval ? 0 : num_secval_services;
	register_servers(&services[i], sizeof_services_table - i, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "register services", (long)st);
	/* Start our background threads. */
	dced_init_srvrexec(thread_attr, &st);
	dced_start_boottime_servers(&st);
	pthread_create(&tid, thread_attr, bootstrap_mgmt, NULL);
	pthread_detach(&tid);
    }
    if (!debug) {
	pthread_create(&tid, thread_attr, signal_catcher, NULL);
	pthread_detach(&tid);
    }

    if (!dced_is_bootstrap && !do_ep_only) {
        dced_wait_for_register(&st);
        if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "wait_for_register", (long)st);
    }

    /* Create private thread pools */
    rpc_server_create_thread_pool(1, &tp_mgmt_pool, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "mgmt pool", (long)st);
    rpc_server_set_thread_pool_qlen(tp_mgmt_pool, 40, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "mgmt pool", (long)st);

    if (endpoints) {
	rpc_server_create_thread_pool(5, &tp_ept_pool, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "ept pool", (long)st);
	rpc_server_set_thread_pool_qlen(tp_ept_pool, 100, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "ept pool", (long)st);
    }

    if (do_secval) {
	rpc_server_create_thread_pool(num_secval_services, &tp_secval_pool, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "secval pool", (long)st);
	rpc_server_set_thread_pool_qlen(tp_secval_pool, 64, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "secval pool", (long)st);
    }

    rpc_server_set_thread_pool_fn(&pool_lookup, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "pool lookup", (long)st);

    while (!server_is_shutdown) {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
	"About to listen."));
	rpc_server_listen(5, &st);
	server_is_ready = FALSE;
	if (st != error_status_ok) {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general,
			   svc_c_sev_fatal, dced_s_cant_init_something,
			   "start listening", (long)st);
	    break;
	}
	if (server_is_shutdown || do_ep_only)
	    break;

	/* Switching the service mode. */
	switch_service_mode();
	server_is_ready = TRUE;
    }
    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		   "About to shutdown."));

    /* Destroy private thread pools */
    rpc_server_set_thread_pool_fn(NULL, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
		       "pool lookup", (long)st);

    if (tp_mgmt_pool != NULL) {
	rpc_server_free_thread_pool(&tp_mgmt_pool, true, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "mgmt pool", (long)st);
    }

    if (endpoints && tp_ept_pool != NULL) {
	rpc_server_free_thread_pool(&tp_ept_pool, true, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "ept pool", (long)st);
    }

    if (do_secval && tp_secval_pool != NULL) {
	rpc_server_free_thread_pool(&tp_secval_pool, true, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "secval pool", (long)st);
    }

    /* We're done.  Clean up and exit. */
    if (do_secval)
	remove_bindings_file();

    if (!do_ep_only && full_service) {
	dced_close_databases(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "close databases", (long)st);
    }
#ifndef	HPDCE_LLB_AUDIO
    /*
     * Delete /tmp/llbdbase.dat.
     */
    if (endpoints) {
	unlink(llbdbase_path);
    }
#endif	/* HPDCE_LLB_AUDIO */
    return 0;
}

/* ______________________________________________________________________ */

static void *
signal_catcher(void *arg)
{
    static int		catch_sigs[] = {
#if	defined(SIGUSR1)
	SIGUSR1,
#endif	/* defined(SIGUSR1) */
#if	defined(SIGCHLD)
	SIGCHLD,
#endif	/* defined(SIGCHLD) */
#if	defined(SIGDANGER)
	SIGDANGER,
#endif	/* defined(SIGDANGER) */
	SIGHUP, SIGINT, SIGQUIT, SIGTERM
    };
    sigset_t		catch_these;
    int			i;
    error_status_t	st;

    rpc_ss_enable_allocate();
    sigemptyset(&catch_these);
    for (i = 0; i < sizeof catch_sigs / sizeof catch_sigs[0]; i++)
	sigaddset(&catch_these, catch_sigs[i]);
    if (do_ep_only) {
#if	defined(SIGCHLD)
	(void)sigdelset(&catch_these, SIGCHLD);
#endif	/* defined(SIGCHLD) */
#if	defined(SIGUSR1)
	(void)sigdelset(&catch_these, SIGUSR1);
#endif	/* defined(SIGUSR1) */
    }

    sigprocmask(SIG_BLOCK, &catch_these, NULL);
    for ( ; ; ) {
	i = sigwait(&catch_these);
	/* Note continue below, to re-do the loop. */
	switch (i) {
	default:
	    if (i == SIGTERM)
		dce_svc_printf(DCED_S_CAUGHT_SIGNAL_MSG, i);
	    else
		dce_svc_printf(DCE_SVC(dhd_svc_handle, "%d"), dhd_s_general,
		    svc_c_sev_warning, dced_s_caught_signal,
		    i);
	    if (server_is_ready) {
		prepare_stop_listening();
		server_is_shutdown = TRUE;
		rpc_mgmt_stop_server_listening(NULL, &st);
	    }
	    else {
		/* Two possibilities: The main thread is in
		 * dced_wait_for_register(), or switching the service mode. */
		server_is_shutdown = TRUE;
		pthread_cond_broadcast(&wait_for_cond);
	    }
	    break;
#if	defined(SIGCHLD)
	case SIGCHLD:
            if (full_service)
   	        dced_srvrexec__reap();
	    continue;
#endif	/* defined(SIGCHLD) */
#if	defined(SIGUSR1)
	case SIGUSR1:
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, "%d"), dhd_s_general,
			   svc_c_sev_notice_verbose, dced_s_caught_signal,
			   i);
	    if (server_is_ready) {
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
			       "About to stop listening."));
#if	defined(SIGCHLD)
		if (full_service)
		    (void)sigdelset(&catch_these, SIGCHLD);
		else
		    (void)sigaddset(&catch_these, SIGCHLD);
#endif	/* defined(SIGCHLD) */
		rpc_mgmt_stop_server_listening(NULL, &st);
	    }
	    break;
#endif	/* defined(SIGUSR1) */
	}
    }
    sigprocmask(SIG_UNBLOCK, &catch_these, NULL);
    return NULL;
}


/*
**  Call rpc_mgmt_is_server_listening to see if we're listening.  The local
**  function, rpc__server_is_listening, returns true even if rpc_server_listen
**  was entered, but fails.  So we don't call that. :-)
*/
static void *
check_listener(void *arg)
{
    int				*pipes;
    int				pfd;
    int				i;
    struct timespec		interval;
    boolean			listening;
    rpc_binding_vector_p_t	bv;
    error_status_t		st;

    /* Get our pipes back. */
    pipes = (int *)arg;
    pfd = pipes[1];

    /* Get some bindings. */
    do {
	interval.tv_sec = 2;
	interval.tv_nsec = 0;
	pthread_delay_np(&interval);

	rpc_server_inq_bindings(&bv, &st);
    } while (st == rpc_s_no_bindings);;

    /* Wait forever since if rpc_server_listen failed main() will exit. */
    do {
	interval.tv_sec = 2;
	interval.tv_nsec = 0;
	pthread_delay_np(&interval);

	for (i = 0; i < bv->count; i++) {
	    listening = rpc_mgmt_is_server_listening(bv->binding_h[i], &st);
	    if (st == error_status_ok && listening)
		break;
	}
    } while (!listening);

    rpc_binding_vector_free(&bv, &st);

    /* Tell the parent we're ready by closing the pipe so he gets EOF
     * (he's blocked in read after the pipe). */
    close(pfd);

    server_is_ready = TRUE;

    dce_svc_printf(DCED_S_IS_LISTENING_MSG);
    return NULL;
}

static void
background(
    void
)
{
    /* The "pipes" variable must be static because its address is passed
     * into a thread routine. */
    static int		pipes[2];
    pid_t		pid;
    char		dummy;
    pthread_t		tid;
    int			i;
    int			child;
    pthread_attr_t	my_attr;

    if (pipe(pipes) < 0)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "pipe", (long)errno);

    pid = fork();
    if (pid < 0)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "fork", (long)errno);

    if (pid > 0) {
	/* We're the parent.  Block in read() until the child exits. */
	close(pipes[1]);
	read(pipes[0], &dummy, sizeof dummy);

	/* How is the child? */
	child = waitpid(pid, &i, WNOHANG);
	if (child == 0) {
	    /* No child exited, so dced is still running.  Good. */
	    _exit(0);
	}

	if (child == pid)
	    dce_svc_printf(DCED_S_BACKGROUND_EXIT_MSG, (long)pid, i);
	else
	    dce_svc_printf(DCED_S_BACKGROUND_WAIT_EXIT_MSG,
		child, (long)pid, strerror(errno));
	fflush(stderr);
	_exit(1);
    }

    /* Child.  Close pipe, spawn listener-checker. */
    close(pipes[0]);
#if	defined(THREAD_STACK_SIZE)
    pthread_attr_create(&my_attr);
    pthread_attr_setstacksize(&my_attr, THREAD_STACK_SIZE);
#else
    my_attr = pthread_attr_default;
#endif	/* defined(THREAD_STACK_SIZE) */
    if (pthread_create(&tid, my_attr, check_listener, (void *)pipes) < 0)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "check_listener", (long)errno);
    pthread_detach(&tid);

    /* Go away, tty. */
    if ((i = open("/dev/null", O_RDONLY)) >= 0) {
	(void)dup2(i, 0);
	(void)close(i);
    }
    if ((i = open("dced.log", O_WRONLY|O_CREAT|O_APPEND,
		  S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) >= 0) {
	(void)dup2(i, 1);
	(void)dup2(i, 2);
	(void)close(i);
    }
    setsid();
}

static void
dced_wait_for_secval(
    error_status_t	*st
)
{
    /* Wait for secval to show up. */
    while (sec_clientd(st)) {
	if (wait_for_signal(10)) {
	    *st = dced_s_startup_interrupted;
	    return;
	}
    }
}

/*
**  Wait for sec to come up.  Patch ACL's.  Start secval if required.
**  If bootstrap, register to do authenticated RPC and EP register our
**  services.  Export to namespace.
*/
static void *
bootstrap_mgmt(
    void		*arg
)
{
    error_status_t	st;
    unsigned32		i;
    uuid_t		admin_group_uuid;

    dced_wait_for_sec(NULL, &admin_group_uuid, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "wait_for_sec", (long)st);

    dced_acl_patcher(NULL, &admin_group_uuid, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "ACL patcher", (long)st);

    if (do_secval) {
	dced_wait_for_secval(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "wait_for_secval", (long)st);
	i = 0;
    }
    else
	i = num_secval_services;

    if (dced_is_bootstrap) {
	dced_is_bootstrap = FALSE;
	dced_wait_for_register(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "wait_for_register", (long)st);
	register_servers(&services[i], sizeof_services_table - i, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "register servers", (long)st);
    }

    if (do_secval) {
        write_dced_bindings(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
                           "write_dced_bindings", (long)st);
    }
    export_to_namespace(&services[i], sizeof_services_table - i, &st);
    if (st != error_status_ok)
	dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG, "NS export", (long)st);

    return NULL;
}

static void
parse_args(int ac, char **av)
{
    int			c;
    error_status_t	st;

    dced_is_bootstrap = FALSE;
    purging = FALSE;
    privacy_required = TRUE;
    remote_update = FALSE;
    sv_first_enable_done = TRUE;

    if (do_ep_only) {
        do_secval = FALSE;
	while ((c = getopt(ac, av, "dfuw:")) != -1) {
	    switch (c) {
	    default:
		dce_fprintf(stderr, dced_s_usage);
		exit(1);
		/* NOTREACHED */
	    case 'd':	debug = TRUE;			break;
	    case 'f':	in_foreground = TRUE;		break;
	    case 'u':
		dce_printf(dced_s_usage);
		exit(0);
		/* NOTREACHED */
	    case 'w':
		dce_svc_routing((idl_char *)optarg, &st);
		if (st != error_status_ok) {
		    dce_pgm_fprintf(stderr, st);
		    exit(1);
		}
		break;
	    }
	}
    }
    else {
        do_secval = TRUE;
	while ((c = getopt(ac, av, "rbdefhipsw:c")) != -1) {
	    switch (c) {
	    default:
		dce_fprintf(stderr, dced_s_usage);
		exit(1);
		/* NOTREACHED */
	    case 'r':	remote_update = TRUE;		break;
	    case 'b':	dced_is_bootstrap = TRUE;
		/* If bootstrapping, lie the secval state. */
		sv_first_enable_done = FALSE;
		break;
	    case 'd':	debug = TRUE;			break;
	    case 'e':	endpoints = FALSE;		break;
	    case 'f':	in_foreground = TRUE;		break;
	    case 'h':
		dce_printf(dced_s_usage);
		exit(0);
		/* NOTREACHED */
	    case 'i':	initialize_databases = TRUE;	break;
	    case 'p':	purging = TRUE;			break;
	    case 's':	do_secval = FALSE;		break;
	    case 'c':	privacy_required = FALSE;	break;
	    case 'w':
		dce_svc_routing((idl_char *)optarg, &st);
		if (st != error_status_ok) {
		    dce_pgm_fprintf(stderr, st);
		    exit(1);
		}
		break;
	    }
	}
    }

    ac -= optind;
    av += optind;
    if (av[0])
	for (protseqs = av; *av; av++) {
	    rpc_network_is_protseq_valid((idl_char *)*av, &st);
	    if (st != error_status_ok)
		dce_svc_printf(DCED_S_BAD_PROTSEQ_MSG, *av);
	}
    if ((purging && (!do_secval || initialize_databases))
     || (protseqs && !endpoints)) {
	dce_pgm_fprintf(stderr, dced_s_usage);
	exit(1);
    }
    if (initialize_databases || debug)
	in_foreground = TRUE;
      
}

void
export_to_namespace(
    service_table_t	*sp,
    int			size,
    error_status_t	*st
)
{
    uuid_vector_t	ovec;
    uuid_t		ouuid;
    rpc_if_id_t		if_id;
    rpc_ns_handle_t	oc;
    unsigned32		i;
    time_t		delay;
    boolean		ook;

    /* Unexport stale bindings */
    for (i = 0; i < sizeof_services_table; i++) {
	rpc_if_inq_id(*(services[i].ifspec), &if_id, st);
	if (*st != error_status_ok)
	    continue;
	delay = 2;
	for ( ; ; ) {
	    rpc_ns_mgmt_binding_unexport(rpc_c_ns_syntax_default,
					 (idl_char *)dced_g_info.this_dced,
					 &if_id,
					 rpc_c_vers_compatible,
					 NULL, st);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug2,
			   "export_to_namespace()::unexport %s 0x%08x",
			   services[i].annote, *st));
	    if (*st != rpc_s_name_service_unavailable)
		break;
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, "%s"), dhd_s_general,
			   svc_c_sev_notice_verbose, dced_s_cant_nsi_unexport,
			   services[i].annote);
	    delay = MIN(600, (delay<<1));
	    if (wait_for_signal(delay)) {
		*st = dced_s_startup_interrupted;
		return;
	    }
	}
	if (*st == rpc_s_entry_not_found
	    || *st == rpc_s_incomplete_name
	    || *st == rpc_s_not_rpc_entry
	    || *st == rpc_s_invalid_name_syntax
	    || *st == rpc_s_unsupported_name_syntax)
	    break;
    }
    for ( ; --size >= 0; sp++) {
	if (!sp->namespace)
	    continue;
	delay = 2;
	for ( ; ; ) {
	    rpc_ns_binding_export(rpc_c_ns_syntax_default,
		(idl_char *)dced_g_info.this_dced, *sp->ifspec, dced_bindings,
		&dced_g_object_vector, st);
	    if (*st == error_status_ok)
		break;
	    dce_svc_printf(DCED_S_CANT_EXPORT_YET_MSG);
	    delay = MIN(600, (delay<<1));
	    if (wait_for_signal(delay)) {
		*st = dced_s_startup_interrupted;
		return;
	    }
	}
	sp->exported = TRUE;
    }
    /* Unexport stale objects */
    rpc_ns_entry_object_inq_begin(rpc_c_ns_syntax_default,
				  (idl_char *)dced_g_info.this_dced,
				  &oc, st);
    if (*st != error_status_ok)
	return;

    for ( ; ; ) {
	rpc_ns_entry_object_inq_next(oc, &ouuid, st);
	if (*st == rpc_s_no_more_members || *st != error_status_ok)
	    break;

	for (ook = FALSE, i = 0; i < dced_g_object_vector.count; i++) {
	    if (uuid_equal(&ouuid, dced_g_object_vector.uuid[i], st))
		ook = TRUE;
	}

	if (ook == FALSE) {
	    ovec.count   = 1;
	    ovec.uuid[0] = &ouuid;

	    rpc_ns_binding_unexport(rpc_c_ns_syntax_default,
				    (idl_char *)dced_g_info.this_dced,
				    NULL,
				    &ovec, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(DCE_SVC(dhd_svc_handle, "%s"), dhd_s_general,
			       svc_c_sev_notice_verbose,
			       dced_s_cant_nsi_unexport,
			       "object");
	    }
	}
    }

    rpc_ns_entry_object_inq_done(&oc, st);
}


static void
register_server(
    service_table_t	*sp,
    error_status_t	*st
)
{
    if ((dced_is_bootstrap && !sp->register_at_bootstrap) || sp->registered) {
	*st = error_status_ok;
	return;
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug2,
	 "EP Registering %s", sp->annote));

    rpc_server_register_if(*sp->ifspec, NULL, sp->mgrepv, st);
    CHECK_STAT_RET(*st);
    if (endpoints)
 	local_ep_register(sp, st);
    else
	rpc_ep_register(*sp->ifspec, dced_bindings, &dced_g_object_vector,
	    (idl_char *)sp->annote, st);
    CHECK_STAT_RET(*st);
    sp->registered = TRUE;
}


static void
register_servers(
    service_table_t	*sp,
    int			i,
    error_status_t	*st
)
{
    dce_lock_justwrite(service_table_lock, st);
    if (*st != error_status_ok)
	return;

    for ( ; --i >= 0; sp++) {
	register_server(sp, st);
	if (*st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_REGISTER_SERVER_MSG, sp->annote);
    }

    dce_lock_unlock(service_table_lock, st);
}


static void
unregister_server(
    service_table_t	*sp,
    error_status_t	*st
)
{
    if (!sp->registered) {
	*st = error_status_ok;
	return;
    }
    if (sp->exported) {
	rpc_ns_binding_unexport(rpc_c_ns_syntax_default,
				(idl_char *)dced_g_info.this_dced,
				*sp->ifspec, NULL, st);
	if (*st != error_status_ok && *st != rpc_s_name_service_unavailable)
	    dce_svc_printf(DCED_S_CANT_NSI_UNEXPORT_MSG, sp->annote);
	sp->exported = FALSE;
    }
    if (!endpoints) {
	rpc_ep_unregister(*sp->ifspec, dced_bindings, &dced_g_object_vector, st);
	CHECK_STAT_RET(*st);
    }

    rpc_server_unregister_if(*sp->ifspec, NULL, st);
    CHECK_STAT_RET(*st);
    sp->registered = FALSE;
}

/* ______________________________________________________________________ */

void
prepare_stop_listening(
    void
)
{
    error_status_t	st;
    int			i;

    dce_lock_justwrite(service_table_lock, &st);
    if (st != error_status_ok)
	/* If it can't be locked, we simply return and continue. */
	return;

    for (i = 0; i < sizeof_services_table; i++) {
	unregister_server(&services[i], &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "unregister service", (long)st);
    }
    for (i = 0; i < sizeof_ept_services_table; i++) {
	unregister_server(&ept_services[i], &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "unregister EP service", (long)st);
    }

    dce_lock_unlock(service_table_lock, &st);
}

/* Should be called by main() thread only. */
static void
switch_service_mode(
    void
)
{
    service_table_t	*sp;
    error_status_t	st;
    int			i;

    if (full_service) {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		       "Switching to the partial service mode."));

	dce_lock_justwrite(service_table_lock, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "dce_lock_justwrite(service_table_lock)", (long)st);

	for (i = num_secval_services; i < sizeof_services_table; i++) {
	    sp = &services[i];
	    if (!sp->registered)
		continue;

	    if (!endpoints) {
		rpc_ep_unregister(*sp->ifspec, dced_bindings,
				  &dced_g_object_vector, &st);
		/* Ignore status -- not worth the cost, since we will
		 * probably just re-register EP soon anyway. */
	    }

	    rpc_server_unregister_if(*sp->ifspec, NULL, &st);
	    if (st != error_status_ok)
		dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			       "unregister service I/F", (long)st);
	    sp->registered = FALSE;
	}

	dce_lock_unlock(service_table_lock, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "dce_lock_unlock(service_table_lock)", (long)st);

	dced_unregister_acls(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "unregister ACLs", (long)st);

	dced_close_databases(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_STOP_SOMETHING_MSG,
			   "close databases", (long)st);

	full_service = FALSE;

	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		       "Entered into the partial service mode."));
    }
    else {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		       "Switching to the full service mode."));

	dced_open_databases(FALSE, &st);
	if (st != error_status_ok) {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, "%s%lx"), dhd_s_general,
			   svc_c_sev_error, dced_s_cant_init_something,
			   "open databases", (long)st);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
			   "Still in the partial service mode."));
	    return;
	}

	dced_register_acls(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "register ACLs", (long)st);

	dced_reload_name_caches(&st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "reload name caches", (long)st);

	register_servers(&services[num_secval_services],
			 sizeof_services_table - num_secval_services, &st);
	if (st != error_status_ok)
	    dce_svc_printf(DCED_S_CANT_INIT_SOMETHING_MSG,
			   "register services", (long)st);

	full_service = TRUE;

	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_general, svc_c_debug1,
		       "Entered into the full service mode."));
    }
}

/* ______________________________________________________________________ */

static void
dced_my_addrs_init(error_status_t	*st)
{
    int 		i;
    unsigned char	*stb;
    error_status_t	st1;

    my_addresses =
	(unsigned char **)malloc(dced_bindings->count * sizeof *my_addresses);
    if (my_addresses == NULL) {
	*st = ept_s_no_memory;
	return;
    }

    /* Get the network address of each binding. */
    for (i = 0; i < dced_bindings->count; i++) {
	rpc_binding_to_string_binding(dced_bindings->binding_h[i], &stb, st);
	if (*st != error_status_ok)
	    return;
	rpc_string_binding_parse(stb, NULL, NULL, &my_addresses[i], NULL,
	    NULL, st);
	rpc_string_free(&stb, &st1);
	if (*st != error_status_ok)
	    return;
    }
    my_address_count = dced_bindings->count;

    /* do a "sort -u" on my_addresses... */
    qsort(my_addresses, my_address_count, sizeof(unsigned char *), charpsort);
    for (i=0; i<my_address_count-1; i++) 
    {
	while ((i<my_address_count-1) &&(strcmp((const char *)my_addresses[i], (const char *)my_addresses[i+1]) == 0))
	{
	    error_status_t _;
	    rpc_string_free (&my_addresses[i], &_);
	    memmove(&my_addresses[i], &my_addresses[i+1],
		    sizeof(char *) * (my_address_count-i));
	    my_address_count--;
	}
    }
    my_addresses[my_address_count++] = NULL;
}

/*
**  Thread pool dispatcher.
**  Note: Service tables are protected by service_table_lock.
**  Even though they are protected, we can't block the
**  rpc runtime's listener thread by taking the
**  lock. Thus, service tables can not be altered
**  while listening. (But, we do alter registered flags. ;)
*/
static void pool_lookup(
    /* [in] */ uuid_p_t obj_uuid,
    /* [in] */ rpc_if_id_p_t if_id,
    /* [in] */ unsigned32 opnum,
    /* [out] */ rpc_thread_pool_handle_t *phandle,
    /* [out] */ unsigned32 *status
)
{
    extern rpc_if_rep_p_t	mgmt_v1_0_s_ifspec;
    service_table_t		*sp;
    int				i;
    unsigned32			st;

    *phandle = NULL;	/* default pool */
    *status = rpc_s_ok;

    if (uuid_equal(&if_id->uuid, &mgmt_v1_0_s_ifspec->id, &st)) {
	*phandle = tp_mgmt_pool;
	return;
    }

    for (sp = &ept_services[0], i = 0; i < sizeof_ept_services_table; i++) {
	if (sp->registered == TRUE
	    && uuid_equal(&if_id->uuid,
			  &((rpc_if_rep_p_t)(*sp->ifspec))->id,
			  &st)) {
	    if (sp->pool != NULL)
		*phandle = *(rpc_thread_pool_handle_t *)(sp->pool);
	    return;
	}
    }

    for (sp = &services[0], i = 0; i < sizeof_services_table; i++) {
	if (sp->registered == TRUE
	    && uuid_equal(&if_id->uuid,
			  &((rpc_if_rep_p_t)(*sp->ifspec))->id,
			  &st)) {
	    if (sp->pool != NULL)
		*phandle = *(rpc_thread_pool_handle_t *)(sp->pool);
	    return;
	}
    }

    /* Unknown or unregistered service */
}
