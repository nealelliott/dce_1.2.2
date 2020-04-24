/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sv_clientd.c,v $
 * Revision 1.1.7.4  1996/11/13  17:57:10  arvind
 * 	CHFts20688: rsec_pk_cache_kdc_key func test failures
 * 	[1996/11/06  21:43 UTC  aha  /main/DCE_1.2.2/aha_pk11/1]
 *
 * 	CHFts20688: copy public key data to output parameter in
 * 	rsec_pk_cache_kdc_key()
 * 	[1996/11/05  22:36 UTC  sommerfeld  /main/DCE_1.2.2/12]
 *
 * 	CHFts20401: memory leak in sec_pk_cache_kdc_key
 * 	plus compiler warning fixes.
 * 	[1996/11/05  02:20 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
 *
 * 	merge fix for Array BoundsWrite, for file_name in rsec_pk_cache_kdc_key
 * 	[1996/10/22  20:33 UTC  bartf  /main/DCE_1.2.2/bartf_pk_fix/1]
 *
 * 	fix file_name malloc len
 * 	[1996/09/16  20:30 UTC  aha  /main/DCE_1.2.2/10]
 *
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:44 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Principal for rsec_pk_cache_kdc_key() is krbtgt/<cell>
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 *
 * 	Support public key version numbers in rsec_pk_cache_kdc_key()
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Fix creation of cache file name in rsec_pk_cache_kdc_key()
 * 	[1996/08/27  14:18 UTC  aha  /main/DCE_1.2.2/9]
 *
 * 	Make key version number in rsec_pk_cache_kdc_key() call to
 * 	sec_pubkey_retrieve() an input/output variable.
 * 	[1996/08/23  14:43 UTC  aha  /main/DCE_1.2.2/7]
 *
 * 	Initialize rsec_pk_cache_kdc_key return parameter so prevent dced core dump
 * 	[1996/07/19  19:29 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 *
 * 	Changes for Public Key Login; initialize return parameter from
 * 	rsec_pk_cache_kdc_call() prior to return in case of error.
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/3]
 *
 * 	Bug fixes for rsec_pk_cache
 * 	[1996/07/13  20:50 UTC  aha  /main/DCE122_PK/aha_pk6/4]
 *
 * 	Merge to DCE122_PK
 * 	[1996/06/06  18:48 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 *
 * 	Include rca_pvt.h rather than rca_fileloc.h, since it
 * 	provides needed context.
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Merge bugfixes for dced and sec_pubkey based on code review
 * 	[1996/05/21  13:07 UTC  aha  /main/DCE_1.2/DCE12_PK/aha_pk3/1]
 *
 * 	Change KDC key cache file from DCELOCAL/KDC_PK_CACHE_DIR/<cell> to
 * 	DCELOCAL/KDC_PK_CACHE_DIR/<cell>/KDC_PK_CACHE_FILE, where <cell>
 * 	can be a hierarchical cell name containing /
 * 	[1996/05/16  14:19 UTC  aha  /main/DCE_1.2/DCE12_PK/1]
 *
 * 	Merge sec_pubkey and rsec_pk_cache
 * 	[1996/05/13  19:45 UTC  aha  /main/DCE_1.2/aha_pk2/4]
 *
 * 	Remove debug copy of sec_pubkey.c
 * 	[1996/05/13  16:31 UTC  aha  /main/DCE_1.2/aha_pk2/3]
 *
 * 	debug fixes
 * 	[1996/05/10  15:58 UTC  aha  /main/DCE_1.2/aha_pk2/2]
 *
 * 	Debug - includes copy of sec_pubkey.c
 * 	[1996/05/07  20:07 UTC  aha  /main/DCE_1.2/aha_pk2/1]
 *
 * 	Additions for public key login: KDC public key cache service
 * 	[17:16:36  17:16:36  aha (Anne Anderson)  1.2  96/04/30]
 *
 * 	Added note about need to convert cell-name to canonical form
 * 	[15:47:08  15:47:08  aha (Anne Anderson)  1.1  96/04/11]
 *
 * 	Initial revision
 * 	[15:47:08  15:47:08  aha (Anne Anderson)  1.1  96/04/11]
 *
 * 	Add KDC key cache module
 * 	[1995/10/10  16:57 UTC  sommerfeld  /main/HPDCE02/10]
 *
 * Revision 1.1.7.3  1996/10/03  17:51:31  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:44 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 * 
 * 	Support public key version numbers in rsec_pk_cache_kdc_key()
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 * 
 * 	Fix creation of cache file name in rsec_pk_cache_kdc_key()
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 * 
 * 	Principal for rsec_pk_cache_kdc_key() is krbtgt/<cell>
 * 	[1996/09/10  21:08 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 * 
 * 	Support public key version numbers in rsec_pk_cache_kdc_key()
 * 	[1996/09/06  18:20 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 * 
 * 	Fix creation of cache file name in rsec_pk_cache_kdc_key()
 * 	[1996/08/27  14:18 UTC  aha  /main/DCE_1.2.2/9]
 * 
 * 	CHFts19841,CHFts19836: eliminate memory leaks in rsec_pk_cache_kdc_key()
 * 	on pubkey_p output and on pubkey local variables
 * 	[1996/08/20  21:26 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/4]
 * 
 * 	Make key version number in rsec_pk_cache_kdc_key() call to
 * 	sec_pubkey_retrieve() an input/output variable.
 * 	[1996/08/23  20:18 UTC  aha  /main/DCE_1.2.2/8]
 * 
 * 	Initialize rsec_pk_cache_kdc_key return parameter so prevent dced core dump
 * 	[1996/07/19  20:28 UTC  aha  /main/DCE_1.2.2/4]
 * 
 * 	pass domains by reference.
 * 	clean up a couple bogons.
 * 	[1996/08/09  17:57 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	Add serviceability debug call to rsec_pk_cache_kdc_key()
 * 	[1996/08/20  18:41 UTC  aha  /main/DCE_1.2.2/5]
 * 
 * 	Add SVC call to help isolate AIX 0x00000002 status return from
 * 	rsec_pk_cache_kdc_key()
 * 	[1996/08/20  16:04 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/3]
 * 
 * 	Merge out
 * 	[1996/08/14  12:52 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/2]
 * 
 * 	Merge fixes for CHFts17901 and CHFts18513 from HP branch (secval threads)
 * 
 * 	Submitted the fix for CHFts18513.
 * 
 * 	Fixed a typo in resetup_machine_context().
 * 	[1996/04/18  18:18 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/8]
 * 
 * 	When activated, resetup the machine context.
 * 	[1996/04/18  17:57 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/7]
 * 
 * 	Submitted the fix for CHFts17901.
 * 
 * 	Fixed a typo in a debug logging.
 * 	[1996/03/29  17:56 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/6]
 * 
 * 	Consider the clock skew in keymgmt_handler().
 * 	[1996/03/29  16:33 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/5]
 * 
 * 	Backoff if login fails in keymgmt_handler().
 * 	[1996/03/26  18:52 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/4]
 * 
 * 	Always wake up keymgmt_handler() if maintain_machine_context() is signaled.
 * 	[1996/03/19  21:21 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/3]
 * 
 * 	Synchronized a password change.
 * 	(Be careful about the locking hierarchy.)
 * 	[1996/03/19  19:29 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/2]
 * 
 * 	Removed sec_clientd_get_machine_context() (unused) and
 * 	cancel_sv_threads().
 * 	No more pthread_cancel(). All three threads are now
 * 	controlled by their associated sv_thd_ctlblk_t variables.
 * 	keymgmt_handler() is reimplemeted by using the code stolen
 * 	from sec_key_mgmt_manage_key() (and its callees).
 * 	Three threads somewhat coordinate their work. For instance,
 * 	siteupdate_handler() suspends itself when the self context is
 * 	expired and waits for maintain_machine_context()'s
 * 	signal. keymgmt_handler() backs off when the password is
 * 	expired and waits for maintain_machine_context()'s signal.
 * 	Added the hacked version of keymgmt__bind_rgy() and
 * 	keymgmt_change_key() temporarily, to change the password with
 * 	the current context. (#ifndef NO_LOCAL_CHANGE_KEY)
 * 	Misc. code cleanup. Added volatile qualifiers for the
 * 	safe-optimization. (Though it causes a lot of compiler warnings.)
 * 
 * 	[1996/03/19  16:03 UTC  tatsu_s  /main/HPDCE02/tatsu_s.davis_fix.b1/1]
 * 
 * 	Initialize rsec_pk_cache_kdc_key return parameter so prevent dced core dump
 * 	[1996/07/19  20:28 UTC  aha  /main/DCE_1.2.2/4]
 * 
 * 	Initialize rsec_pk_cache_kdc_key return parameter so prevent dced core dump
 * 	[1996/07/19  20:28 UTC  aha  /main/DCE_1.2.2/4]
 * 
 * 	pass domains by reference.
 * 	clean up a couple bogons.
 * 	[1996/08/09  17:57 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_kdc/1]
 * 
 * 	Make key version number in rsec_pk_cache_kdc_key() call to
 * 	sec_pubkey_retrieve() an input/output vari
 * 
 * Revision 1.1.7.2  1996/08/09  11:54:31  arvind
 * 	Initialize rsec_pk_cache_kdc_key return parameter so prevent dced core dump
 * 	[1996/07/19  19:29 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
 * 
 * 	Changes for Public Key Login; initialize return parameter from
 * 	rsec_pk_cache_kdc_call() prior to return in case of error.
 * 	[1996/07/18  17:35 UTC  aha  /main/DCE_1.2.2/3]
 * 
 * 	Bug fixes for rsec_pk_cache
 * 	[1996/07/13  20:50 UTC  aha  /main/DCE122_PK/aha_pk6/4]
 * 
 * 	Changes for Public Key Login; merge changes for double-indirection->
 * 	single-indirection fixes.
 * 	[1996/06/19  20:33 UTC  aha  /main/DCE122_PK/aha_pk6/3]
 * 
 * 	Changes for Public Key Login; fix file descriptor leak in
 * 	rsec_pk_cache_write_key(), use sec_pubkey_keyfile_write().
 * 	[1996/06/18  20:56 UTC  aha  /main/DCE122_PK/aha_pk6/2]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  17:58 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Merge to DCE122_PK
 * 	[1996/06/06  18:48 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 * 
 * 	Include rca_pvt.h rather than rca_fileloc.h, since it
 * 	provides needed context.
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge bugfixes for dced and sec_pubkey based on code review
 * 	[1996/05/21  13:07 UTC  aha  /main/DCE_1.2/DCE12_PK/aha_pk3/1]
 * 
 * 	Change KDC key cache file from DCELOCAL/KDC_PK_CACHE_DIR/<cell> to
 * 	DCELOCAL/KDC_PK_CACHE_DIR/<cell>/KDC_PK_CACHE_FILE, where <cell>
 * 	can be a hierarchical cell name containing /
 * 	[1996/05/16  14:19 UTC  aha  /main/DCE_1.2/DCE12_PK/1]
 * 
 * 	Merge sec_pubkey and rsec_pk_cache
 * 	[1996/05/13  19:45 UTC  aha  /main/DCE_1.2/aha_pk2/4]
 * 
 * 	Remove debug copy of sec_pubkey.c
 * 	[1996/05/13  16:31 UTC  aha  /main/DCE_1.2/aha_pk2/3]
 * 
 * 	debug fixes
 * 	[1996/05/10  15:58 UTC  aha  /main/DCE_1.2/aha_pk2/2]
 * 
 * 	Debug - includes copy of sec_pubkey.c
 * 	[1996/05/07  20:07 UTC  aha  /main/DCE_1.2/aha_pk2/1]
 * 
 * 	Additions for public key login: KDC public key cache service
 * 	[17:16:36  17:16:36  aha (Anne Anderson)  1.2  96/04/30]
 * 
 * 	Added note about need to convert cell-name to canonical form
 * 	[15:47:08  15:47:08  aha (Anne Anderson)  1.1  96/04/11]
 * 
 * 	Initial revision
 * 	[15:47:08  15:47:08  aha (Anne Anderson)  1.1  96/04/11]
 * 
 * 	Add KDC key cache module
 * 	[1995/10/10  16:57 UTC  sommerfeld  /main/HPDCE02/10]
 * 
 * Revision 1.1.7.1  1996/07/08  18:21:04  arvind
 * 	fix compiler warnings; add more include files
 * 	[1996/06/11  21:33 UTC  sommerfeld  /main/DCE122_PK/sommerfeld_pk_indir/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/10  14:58 UTC  aha  /main/DCE122_PK/2]
 * 
 * 	Merge to DCE122_PK
 * 	[1996/06/06  18:48 UTC  aha  /main/DCE122_PK/aha_pk6/1]
 * 
 * 	Include rca_pvt.h rather than rca_fileloc.h, since it
 * 	provides needed context.
 * 	[1996/06/04  18:52 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Merge bugfixes for dced and sec_pubkey based on code review
 * 	[1996/05/21  13:07 UTC  aha  /main/DCE_1.2/DCE12_PK/aha_pk3/1]
 * 
 * 	Change KDC key cache file from DCELOCAL/KDC_PK_CACHE_DIR/<cell> to
 * 	DCELOCAL/KDC_PK_CACHE_DIR/<cell>/KDC_PK_CACHE_FILE, where <cell>
 * 	can be a hierarchical cell name containing /
 * 	[1996/05/16  14:19 UTC  aha  /main/DCE_1.2/DCE12_PK/1]
 * 
 * 	Merge sec_pubkey and rsec_pk_cache
 * 	[1996/05/13  19:45 UTC  aha  /main/DCE_1.2/aha_pk2/4]
 * 
 * 	Remove debug copy of sec_pubkey.c
 * 	[1996/05/13  16:31 UTC  aha  /main/DCE_1.2/aha_pk2/3]
 * 
 * 	debug fixes
 * 	[1996/05/10  15:58 UTC  aha  /main/DCE_1.2/aha_pk2/2]
 * 
 * 	Debug - includes copy of sec_pubkey.c
 * 	[1996/05/07  20:07 UTC  aha  /main/DCE_1.2/aha_pk2/1]
 * 
 * 	Additions for public key login: KDC public key cache service
 * 	[17:16:36  17:16:36  aha (Anne Anderson)  1.2  96/04/30]
 * 
 * 	Added note about need to convert cell-name to canonical form
 * 	[15:47:08  15:47:08  aha (Anne Anderson)  1.1  96/04/11]
 * 
 * 	Initial revision
 * 	[15:47:08  15:47:08  aha (Anne Anderson)  1.1  96/04/11]
 * 
 * 	Submitted the fix for CHFts16512.
 * 	[1995/09/29  22:38 UTC  tatsu_s  /main/HPDCE02/9]
 * 
 * 	Initial revision: dced splitting (libdcedpvt).
 * 	[1995/09/19  15:31 UTC  tatsu_s  /main/HPDCE02/tatsu_s.dced_local.b0/1]
 * 
 * 	Merge fix for CHFts16035 from mb_mothra6
 * 	[1995/09/08  18:17 UTC  burati  /main/HPDCE02/8]
 * 
 * 	Improve backoff
 * 	[1995/09/07  19:17 UTC  burati  /main/HPDCE02/mb_mothra6/2]
 * 
 * 	Fix key mgmt retries
 * 	[1995/08/21  21:30 UTC  burati  /main/HPDCE02/mb_mothra6/1]
 * 
 * 	Submitted the fix for CHFts16173.
 * 	[1995/08/29  22:27 UTC  tatsu_s  /main/HPDCE02/7]
 * 
 * 	Renamed write_bindings() to write_dced_bindings().
 * 	Added ACL and delay time range check in secval_update_pesite().
 * 	[1995/08/23  21:16 UTC  tatsu_s  /main/HPDCE02/tatsu_s.psock_timeout.b0/1]
 * 
 * 	Merge CHFts15534
 * 	[1995/06/21  18:40 UTC  mullan_s  /main/HPDCE02/6]
 * 
 * 	maintain_machine_context -avoid busy-wait loops by making sleep times incremental;
 * 	add svc error messages on thread exceptions; do not let thread exit unnecessarily.
 * 	(CHFts15534)
 * 	[1995/06/19  18:55 UTC  mullan_s  /main/HPDCE02/mullan_mothra_chfts15534/1]
 * 
 * 	Merge for CHFts13120
 * 	[1995/04/04  17:26 UTC  mdf  /main/HPDCE02/5]
 * 
 * 	Modifications after code review.
 * 	[1995/03/31  19:07 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_0/4]
 * 
 * 	Modifications based on suggested changes for CHFts13120
 * 	[1995/03/31  14:31 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_0/3]
 * 
 * 	Resolution of CHFts13120.
 * 	[1995/03/22  17:53 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_0/2]
 * 
 * 	Fix for CHFts14258, still needed.  Basicly sv_purge_context has to
 * 	call sec_login_export_from_pag() and sec_login_purge_context_exp()
 * 	to remove the current machine context.
 * 	[1995/03/10  17:14 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_0/1]
 * 
 * 	Fix dced panic
 * 	[1995/03/09  20:35 UTC  mdf  /main/HPDCE02/3]
 * 
 * 	Fix dced core dumps which break nightly tests.
 * 	[1995/03/09  19:29 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_0/1]
 * 
 * 	Fix nightly build problem.
 * 	[1995/03/09  19:28 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_0/1]
 * 
 * 	merge
 * 	[1995/02/09  11:53 UTC  mdf  /main/HPDCE02/2]
 * 
 * 	Changes for CHFts14258, which prevents kdestroy from being able to
 * 	remove the machine's credential files.
 * 	[1995/02/08  18:42 UTC  mdf  /main/HPDCE02/mdf_MOTHRA_5/1]
 * 
 * 	Fixes defect.
 * 	[1995/02/02  19:38 UTC  mdf  /main/HPDCE02/1]
 * 
 * 	Fixes defect CHFts13794, also known as OT12627.
 * 
 * 	The change is comprised of the change Bill Sommerfeld made for the
 * 	GTE Hotsite, and the one proposed by Rob Baum from the OSF defect report.
 * 
 * 	[1995/02/02  14:07 UTC  mdf  /main/mdf_MOTHRA_4/3]
 * 
 * 	Add KDC key cache module
 * 	[1995/10/10  16:57 UTC  sommerfeld  /main/HPDCE02/10]
 * 
 * Revision 1.1.5.2  1996/02/18  19:16:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:45  marty]
 * 
 * Revision 1.1.5.1  1995/12/08  21:27:58  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/10  1995/10/10  16:57 UTC  sommerfeld
 * 	Merge out from HPDCE02..
 * 	[1995/10/05  20:05 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 * 
 * 	on pe_site update, only check acl if delay != 0
 * 	[1995/09/08  22:35 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/9  1995/09/29  22:38 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/8  1995/09/08  18:17 UTC  burati
 * 	Merge fix for CHFts16035 from mb_mothra6
 * 
 * 	HP revision /main/HPDCE02/mb_mothra6/2  1995/09/07  19:17 UTC  burati
 * 	Improve backoff
 * 
 * 	HP revision /main/HPDCE02/mb_mothra6/1  1995/08/21  21:30 UTC  burati
 * 	Fix key mgmt retries
 * 
 * 	HP revision /main/HPDCE02/7  1995/08/29  22:27 UTC  tatsu_s
 * 	Submitted the fix for CHFts16173.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.psock_timeout.b0/1  1995/08/23  21:16 UTC  tatsu_s
 * 	Renamed write_bindings() to write_dced_bindings().
 * 	Added ACL and delay time range check in secval_update_pesite().
 * 
 * 	HP revision /main/HPDCE02/6  1995/06/21  18:40 UTC  mullan_s
 * 	Merge CHFts15534
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_chfts15534/1  1995/06/19  18:55 UTC  mullan_s
 * 	maintain_machine_context -avoid busy-wait loops by making sleep times incremental;
 * 	add svc error messages on thread exceptions; do not let thread exit unnecessarily.
 * 	(CHFts15534)
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/04  17:26 UTC  mdf
 * 	Merge for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_0/4  1995/03/31  19:07 UTC  mdf
 * 	Modifications after code review.
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_0/3  1995/03/31  14:31 UTC  mdf
 * 	Modifications based on suggested changes for CHFts13120
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_0/2  1995/03/22  17:53 UTC  mdf
 * 	Resolution of CHFts13120.
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_0/1  1995/03/10  17:14 UTC  mdf
 * 	Fix for CHFts14258, still needed.  Basicly sv_purge_context has to
 * 	call sec_login_export_from_pag() and sec_login_purge_context_exp()
 * 	to remove the current machine context.
 * 
 * 	HP revision /main/HPDCE02/3  1995/03/09  20:35 UTC  mdf
 * 	Fix dced panic
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_0/1  1995/03/09  19:29 UTC  mdf
 * 	Fix dced core dumps which break nightly tests.
 * 
 * 	HP revision /main/HPDCE02/mdf_mothra_defects_0/1  1995/03/09  19:28 UTC  mdf
 * 	Fix nightly build problem.
 * 
 * 	HP revision /main/HPDCE02/2  1995/02/09  11:53 UTC  mdf
 * 	merge
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_5/1  1995/02/08  18:42 UTC  mdf
 * 	Changes for CHFts14258, which prevents kdestroy from being able to
 * 	remove the machine's credential files.
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/02  19:38 UTC  mdf
 * 	Fixes defect.
 * 
 * 	HP revision /main/mdf_MOTHRA_4/3  1995/02/02  14:07 UTC  mdf
 * 	Fixes defect CHFts13794, also known as OT12627.
 * 
 * 	The change is comprised of the change Bill Sommerfeld made for the
 * 	GTE Hotsite, and the one proposed by Rob Baum from the OSF defect report.
 * 	[1995/12/08  18:00:02  root]
 * 
 * Revision 1.1.2.19  1994/10/17  19:31:05  sommerfeld
 * 	OT11548: fix hole in local preauth protocol
 * 	[1994/10/17  19:29:06  sommerfeld]
 * 
 * Revision 1.1.2.18  1994/10/03  19:58:45  rsalz
 * 	Put a mutex around secval operations; make explicit init func.
 * 	[1994/10/03  19:57:06  rsalz]
 * 
 * Revision 1.1.2.17  1994/09/22  21:33:50  bowe
 * 	Extraneous error report in sec_clientd_setup_first_context. [CR 12307]
 * 	[1994/09/22  21:33:39  bowe]
 * 
 * Revision 1.1.2.16  1994/09/14  20:04:22  bowe
 * 	SVC work (plus some minor cleanup) [CR 11725]
 * 	[1994/09/14  20:03:54  bowe]
 * 
 * Revision 1.1.2.15  1994/09/12  15:58:44  baum
 * 	Code cleanup/threads memory leak fix (OT11940)
 * 	[1994/09/12  15:45:50  baum]
 * 
 * Revision 1.1.2.14  1994/08/31  19:54:25  rsalz
 * 	Add scd bindings file to oride_base.idl and use it (OT CR 10976).
 * 	[1994/08/31  19:31:29  rsalz]
 * 
 * Revision 1.1.2.13  1994/08/28  14:40:26  rsalz
 * 	Use dce_gettimeofday not dced_get_the_time (OT CR 11931).
 * 	[1994/08/28  14:35:13  rsalz]
 * 
 * Revision 1.1.2.12  1994/08/24  16:41:13  baum
 * 	Full dced service on bootup.  Secval on by default (OT 11726 & 11656)
 * 	[1994/08/24  16:35:05  baum]
 * 
 * Revision 1.1.2.11  1994/08/12  21:34:55  bowe
 * 	In keymgmt_handler(), use correct host prin [CR 11656]
 * 	[1994/08/12  20:55:38  bowe]
 * 
 * Revision 1.1.2.10  1994/07/29  19:56:17  rsalz
 * 	Allow THREAD_STACK_SIZE to set pthread_attr stacksize (OT CR 11421).
 * 	Use dce_bindings_to_file (OT CR 11466).
 * 	[1994/07/29  19:38:33  rsalz]
 * 
 * Revision 1.1.2.9  1994/07/01  14:37:02  rsalz
 * 	Use <fcntl.h> not <sys/fcntl.h> (OT CR 11158).
 * 	[1994/07/01  14:35:04  rsalz]
 * 
 * Revision 1.1.2.8  1994/06/30  19:36:59  ohara
 * 	include sys/fcntl.h
 * 	[1994/06/15  18:47:11  ohara]
 * 
 * Revision 1.1.2.7  1994/06/13  20:05:38  rsalz
 * 	Code cleanup.
 * 	Server registration done in main now.
 * 	[1994/06/13  20:05:10  rsalz]
 * 
 * Revision 1.1.2.6  1994/06/06  19:44:57  bowe
 * 	Add ifspec code for rsec_login functionality. [CR 9768]
 * 	[1994/06/06  19:42:28  bowe]
 * 
 * Revision 1.1.2.5  1994/05/10  20:15:18  rsalz
 * 	Cleanup and more functionality
 * 	[1994/05/10  20:10:49  rsalz]
 * 
 * Revision 1.1.2.4  1994/04/20  19:09:29  rsalz
 * 	Add #include <time.h>; "struct tm" is there sometimes (OT CR 10403).
 * 	Add purge context.
 * 	Use dce_priv_proc_enable.
 * 	[1994/04/20  19:08:07  rsalz]
 * 
 * Revision 1.1.2.3  1994/04/11  20:22:07  rsalz
 * 	Final set of RFC revision fixes.
 * 	[1994/04/11  20:08:26  rsalz]
 * 
 * Revision 1.1.2.2  1994/03/11  15:00:10  bowe
 * 	Make #include's consistent. Fix some lint warnings
 * 	[1994/03/11  14:56:06  bowe]
 * 
 * Revision 1.1.2.1  1994/03/09  14:26:00  bowe
 * 	Initial checkin
 * 	[1994/03/09  14:24:29  bowe]
 * 
 * $EndLog$
 */

/*  sec_clientd.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1995, 1996
** Unpublished work. All Rights Reserved.
*/
#if	!defined(_BSD_SOURCE)
#define	_BSD_SOURCE
#endif	/* !defined(_BSD_SOURCE) */

#include <stdio.h>
#include <dce/dce.h>
#include <dce/dce_error.h>

#include <pthread.h>
#include <dce/exc_handling.h>
#include <sys/time.h>
#include <time.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <u_str.h>
#include <dce/sec_login.h>
#include <dce/keymgmt.h>
#ifndef	NO_LOCAL_CHANGE_KEY
#include <dce/rgybase.h>
#include <dce/binding.h>
#endif	/* NO_LOCAL_CHANGE_KEY */
#include <dce/dce_priv_proc.h>
#include <rsec_cert.h>
#include <rsec_login.h>
#include <roverride.h>
#include <dcedimpl.h>
#include <svimpl.h>
#include <rca_pvt.h>
#include <krb5/error_tables/krb5_err.h>
#include <dce/sec_base.h>
#include <dce/sec_pk_base.h>
#include <dce/sec_pk.h>
#include <dce/rsec_pk_cache.h>
#include <dce/sec_pubkey.h>

/* External refrences. */
/*
**  Stolen from rpcrand.h in rpc/runtime.
*/
#define RPC_RANDOM_GET(lower, upper) \
	(((rpc__random_get(lower, upper)) % ((upper) - (lower) + 1)) + (lower))
extern unsigned32 rpc__random_get(unsigned32, unsigned32);

/* External definitions. */
char			*sv_override_file_name;
char			*sv_group_override_file_name;
char			*sv_clientd_binding;
char			*sv_preauth_dir;

sv_thd_ctlblk_t		selfcntx_ctlblk;

/* Private definitions. */
static boolean		sv_initialized = FALSE;
static sv_thd_ctlblk_t	pesite_ctlblk;
static sv_thd_ctlblk_t	keymgmt_ctlblk;
#define KEY_CACHE_STALE_SECS 300

static signed32		pesite_delay_time = ONE_HOUR;

/*
 * Tack DCELOCAL in front of path
 */
static char *
glue( char *tail )
{
    char	*p;

    if ((p = malloc(strlen(dcelocal_path) + strlen(tail) + 1)) == NULL)
	return NULL;
    strcpy(p, dcelocal_path);
    strcat(p, tail);
    return p;
}

/*
 * Init global data needed by secval.
 */
void
init_secval(
    error_status_t		*st
)
{
    if (dce_priv_proc_enable() == -1) {
	*st = sec_login_s_privileged;
	return;
    }

    sv_override_file_name = glue(OVERRIDE_FILE);
    sv_group_override_file_name = glue(GROUP_OVERRIDE_FILE);
    sv_clientd_binding = glue(SCD_BINDINGS_FILE);
    sv_preauth_dir = glue(PREAUTH_DIR);
    if (sv_override_file_name == NULL || sv_group_override_file_name == NULL
	|| sv_clientd_binding == NULL || sv_preauth_dir == NULL) {
	*st = dced_s_no_memory;
	return;
    }

    init_sv_preauth(st);
    if (*st != error_status_ok)
	return;

    pesite_ctlblk.running = FALSE;
    pesite_ctlblk.stop = FALSE;
    pesite_ctlblk.suspend = FALSE;

    keymgmt_ctlblk.running = FALSE;
    keymgmt_ctlblk.stop = FALSE;
    keymgmt_ctlblk.suspend = FALSE;

    selfcntx_ctlblk.running = FALSE;
    selfcntx_ctlblk.stop = FALSE;
    selfcntx_ctlblk.suspend = FALSE;

    if (pthread_mutex_init(&pesite_ctlblk.m, pthread_mutexattr_default) < 0) {
	*st = dced_s_mutex_init_fail;
	return;
    }
    if (pthread_mutex_init(&keymgmt_ctlblk.m, pthread_mutexattr_default) < 0) {
	*st = dced_s_mutex_init_fail;
	return;
    }
    if (pthread_mutex_init(&selfcntx_ctlblk.m, pthread_mutexattr_default) < 0) {
	*st = dced_s_mutex_init_fail;
	return;
    }
    if (pthread_cond_init(&pesite_ctlblk.cv, pthread_condattr_default) < 0) {
	*st = dced_s_cond_init_fail;
	return;
    }
    if (pthread_cond_init(&keymgmt_ctlblk.cv, pthread_condattr_default) < 0) {
	*st = dced_s_cond_init_fail;
	return;
    }
    if (pthread_cond_init(&selfcntx_ctlblk.cv, pthread_condattr_default) < 0) {
	*st = dced_s_cond_init_fail;
	return;
    }

    sv_initialized = TRUE;

    *st = error_status_ok;
}

/*
 * sec_clientd_setup_first_context - Sets up initial machine context.
 *
 * Having this as a seperate function allows it to be called before the
 * maintain_context thread is launched for synchrononicity and error handling
 * reasons
 */
static void
sec_clientd_setup_first_context(
    error_status_t		*st
)
{
    sec_login_handle_t	context;
    boolean32		reset_passwd;
    boolean32		release_cntxt = FALSE;
    sec_login_auth_src_t	auth_src;
    error_status_t	st2;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Entered sec_clientd_setup_first_context"));

    sv_purge_context();

    /*
     * Set up & validate initial machine context
     */
    sec_login_setup_first(&context, st);
    if ( *st == error_status_ok ) {
	sec_login_validate_first(context, &reset_passwd, &auth_src, st);
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	       "scd_setup_first_cntxt: sec_login_validate_first st = 0x%08x",
		       *st));
	if ( *st != error_status_ok ) {
	    dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, *st);
	    sec_login_purge_context ( &context, &st2 );
	    if ( st2 != error_status_ok )
		dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, st2);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		"scd_setup_first_cntxt: sec_login_purge_context st2 = 0x%08x",
			   st2));
	}
	else {
	    release_cntxt = TRUE;
	    /* If (auth_src == sec_login_auth_src_network && reset_passwd),
	     * then the password is expired. */
	}
    }
    else
    {
	/* Setup first failed, if error is default_use, we're OK */
	if (*st != sec_login_s_default_use)  {
	    dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, *st);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "scd_setup_first_cntxt: sec_login_setup_first st = 0x%08x",
			   *st));
	}
	else
	{
	    /* The default machine context already exists, so we'll simply
	     * inherit and refresh it.
	     */
	    sec_login_get_current_context(&context, st);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	   "scd_setup_first_cntxt: sec_login_get_current_context st = 0x%08x",
			   *st));
	    if (*st == error_status_ok) {
		release_cntxt = TRUE;
		sec_login_refresh_identity(context, st);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	       "scd_setup_first_cntxt: sec_login_refresh_identity st = 0x%08x",
			       *st));
		if ( *st == error_status_ok ) {
		    sec_login_validate_first(context, &reset_passwd, &auth_src, st);
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		"scd_setup_first_cntxt: sec_login_validate_first st = 0x%08x",
				   *st));
		    /* If (*st == error_status_ok
		     *     && auth_src == sec_login_auth_src_network
		     *     && reset_passwd),
		     * then the password is expired. */
		    if (*st == sec_login_s_already_valid)
			*st = error_status_ok;
		    else if (*st != error_status_ok)
			dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, *st);
		}
		else
		    dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, *st);
	    }
	    else
		dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, *st);
	}
    }

    /* If we successfully got context, free structure */
    if ( release_cntxt ) {
	sec_login_release_context ( &context, &st2 );
	if ( st2 != error_status_ok )
	    dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, st2);
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	      "scd_setup_first_cntxt: sec_login_release_context st2 = 0x%08x",
		       st2));
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Exiting sec_clientd_setup_first_context"));
}


/*
 * sv_purge_context - Remove current machine context with extreme prejudice
 */
void
sv_purge_context( void )
{
    sec_login_handle_t		context;
    error_status_t		st;
    unsigned32 buf_len;
    ndr_byte buf[1024];
    const signed32 MAXS32 = ~(1<<31);

    sec_login_export_from_pag(-1, sizeof(buf), buf, &buf_len, NULL, &st);
    if (st != error_status_ok) {
	dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, st);
	return;
    }
    sec_login_purge_context_exp(buf_len, buf, MAXS32, &st);
    if ( st != error_status_ok )
	dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, st);
}

#ifndef	NO_LOCAL_CHANGE_KEY
/* Local copy of sec_key_mgmt_change_key() which uses the current
 * default context, instead of establishing a new context which can't
 * be done if the password is expired. */

#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)

static void keymgmt__bind_rgy
  (
    unsigned char                   * principal_name,
    sec_rgy_bind_auth_info_type_t   auth_type,
    sec_login_handle_t              lc,
    sec_rgy_name_t                  rel_princ_name,
    unsigned char                   * cell,
    boolean32                       update,
    sec_rgy_handle_t                * rgy_h,
    error_status_t                  * st
  )
{
    sec_rgy_bind_auth_info_t    auth_info;
    error_status_t              lerr;
    sec_rgy_name_t              lcell;

    if (cell == NULL) {
        cell = lcell;
    }

    /*
     * Parse the global name into a <cell> <principal> pair
     */
    auth_info.info_type = sec_rgy_bind_auth_none;
    sec_rgy_cell_bind((unsigned_char_p_t)"", &auth_info, rgy_h, st);
    if (GOOD_STATUS(st)) {
        sec_id_parse_name(*rgy_h, principal_name, cell, NULL, rel_princ_name,
                            NULL, st);
        sec_rgy_site_close(*rgy_h, &lerr);

        /*
         * bind to the principal's home cell
         */
        switch (auth_type) {
            case sec_rgy_bind_auth_dce:
                auth_info.info_type = sec_rgy_bind_auth_dce;
                auth_info.tagged_union.dce_info.identity = lc;
                auth_info.tagged_union.dce_info.authn_level =
                                            rpc_c_authn_level_pkt_integrity;
                auth_info.tagged_union.dce_info.authn_svc =
                                            rpc_c_authn_dce_secret;
                auth_info.tagged_union.dce_info.authz_svc =
                                            rpc_c_authz_dce;
                break;

            default:
                auth_info.info_type = sec_rgy_bind_auth_none;
                break;
        }
        if (update) {
            sec_rgy_site_bind_update(cell, &auth_info, rgy_h, st);
        } else {
            sec_rgy_cell_bind(cell, &auth_info, rgy_h, st);
        }
    }
}

/* k e y m g m t _ c h a n g e _ k e y
 *
 * Perform all activities necessary to update a principal's key to
 * the specified value.  This includes updating any local storage
 * for the principal's key and also performing any remote operations
 * needed to keep the authentication service current.  Old keys for the
 * principal are garbage collected if appropriate.
 *
 * Argument Usage:
 *      INPUT
 *  authn_service   **  select the underlying authentication scheme
 *  arg             **  key acquisition function argument (same value
 *                      that is passed to rpc_server_register_auth_info.)
 *  principal_name  **  names the principal whose key is to be changed
 *  key_vno             the new key version number. (0 may be specified
 *                      to indicate that the runtime should select the
 *                      next appropriate key version number)
 *  keydata         **  the new key value.
 *
 *  ** see NOTE on authenticated RPC related arguments above.
 *
 *      OUTPUT
 *  garbage_collect_time    informs the caller of how many seconds in the
 *                      future the garbage collection function must be
 *                      called to get rid of any remaining obsolete keys.
 *                      a 0 return value indicates there is no need to
 *                      call the garbage collection function.
 *  err                 error status if any.
 *
 * Any error condition will leave the key state unchanged.
 *
 * Errors:
 *  sec_key_mgmt_e_key_unavailable      Old key not present, unable to use
 *                                      it to set a client side authentication
 *                                      context.
 *  sec_key_mgmt_e_authn_invalid        Authentication service invalid.
 *  sec_key_mgmt_e_authn_unavailable    Authentication service unavailable
 *                                      unable to update network database or
 *                                      obtain necessary network credentials.
 *  sec_key_mgmt_e_unauthorized         Caller unauthorized to perform op.
 *  sec_key_mgmt_e_key_unsupported      Key type not supported.
 *  sec_key_mgmt_e_key_version_exists   Key with this version number already
 *                                      exists.
 */
static void keymgmt_change_key
  (
    sec_key_mgmt_authn_service  authn_service,
    void                        * arg,
    unsigned char               * principal_name,
    unsigned32                  key_vno,
    void                        * keydata,
    sec_timeval_period_t        * garbage_collect_time,
    error_status_t              * err
  )
{
    unsigned32                  old_vno = sec_passwd_c_version_none, next_vno;
    sec_passwd_rec_t            *oldkey;
    sec_passwd_rec_t            *newkey = (sec_passwd_rec_t *) keydata;
    sec_login_handle_t          lc;
    error_status_t              lst;
    sec_rgy_login_name_t        login_name;
    sec_rgy_handle_t            rgy_h = NULL;
    sec_rgy_name_t              cell;
    char                        *prim_princ;

    /* Use the current context, instead of login as the specified
     * principal */
    sec_login_get_current_context(&lc, err);
    /* We should verify that the current context's principal_name and
     * authn_service match with the requested ones. */

    if (GOOD_STATUS(err)) {
        /* If key_vno was specified, attempt to use it */
        if (key_vno != sec_passwd_c_version_none) {
            next_vno = key_vno;
        } else {
            /* Didn't ask for next_vno from sec_key_mgmt__login because it
	     * doesn't yet ensure that we bound to the master for the login,
	     * thus it could have used an older key to validate us with.
	     * So, we must ask for next_vno from the master now.
             */
            sec_key_mgmt_get_next_kvno(authn_service, arg, principal_name,
                &newkey->key.key_type, &old_vno, &next_vno, err);

            /* Next vno picked by server, so there can't be an existing valid
             * key with this vno.  Attempt a delete for this vno to make sure.
             */
            sec_key_mgmt_delete_key(authn_service, arg, principal_name,
                                    next_vno, &lst);
        }

        if (GOOD_STATUS(err)) {
            ((sec_passwd_rec_t *)keydata)->version_number = next_vno;

            /*
             * Bind to principal's home cell
             */
            keymgmt__bind_rgy(principal_name, sec_rgy_bind_auth_dce, lc,
			      login_name.pname, cell, true, &rgy_h, err);
        }

        if (GOOD_STATUS(err)) {
            sec_krb_string_from_dce_pair((char *) cell,
                                            (char *) login_name.pname,
                                            &prim_princ, err);
            if (GOOD_STATUS(err)) {
                /* Set the key locally */
                sec_key_mgmt_set_key(authn_service, arg,
                                        (unsigned char *) prim_princ,
                                        next_vno, keydata, err);
                free(prim_princ);

                if (GOOD_STATUS(err)) {
                    login_name.gname[0] = login_name.oname[0] = '\0';

                    /*
                     * If asked for garbage_collect_time return
                     * max_ticket_lifetime or error
                     */
                    if (garbage_collect_time) {
                        sec_rgy_plcy_auth_t     auth_policy;

                        sec_rgy_auth_plcy_get_effective(rgy_h, &login_name,
                                                        &auth_policy, err);
                        if (GOOD_STATUS(err)) {
                            *garbage_collect_time =
                                    auth_policy.max_ticket_lifetime;
                        }
                    }

                    /* Get the current key, since validate_ident free'd it */
                    if (GOOD_STATUS(err)) {
                        sec_key_mgmt_get_key(authn_service, arg, principal_name,
                                                old_vno, (void **) &oldkey, err);
                    }

                    /* Set the key in the registry */
                    if (GOOD_STATUS(err)) {
                        sec_rgy_acct_passwd(rgy_h, &login_name, oldkey,
                                            (sec_passwd_rec_t *) keydata,
                                            sec_passwd_des, &next_vno, err);
                        sec_key_mgmt_free_key(oldkey, &lst);
                    }
                }
            } /* set_key succeeded */

            sec_rgy_site_close(rgy_h, &lst);
        } /* cell_bind succeeded */
    } /* setup_ident and validate_ident succeeded */

    if (GOOD_STATUS(err)) {
        /* Perform any necessary garbage collection */
        sec_key_mgmt_garbage_collect(authn_service, arg, principal_name, err);
    }
}
#undef	GOOD_STATUS
#endif	/* NO_LOCAL_CHANGE_KEY */

/*
 * keymgmt_handler - Thread routine to manage machine keytab
 *
 * This includes a copy of sec_key_mgmt_manage_key() (and its
 * callees). We are not calling it because it is not cancel-safe
 * and there is no API to stop it. (When
 * sec_key_mgmt_manage_key() becomes safely interruptible, we
 * should use it.)
 */
static void *
keymgmt_handler( void *arg )
{
    volatile error_status_t		st;
    volatile boolean			passwd_expired = FALSE;
    volatile boolean			has_exp = TRUE;
    volatile time_t			backoff = 0;
    unsigned32				old_kvno = sec_passwd_c_version_none;
    sec_timeval_period_t		garbage_collect_time;
    volatile sec_passwd_rec_t		*keydata;
    sec_passwd_type_t			keytype = sec_passwd_des;
    volatile sec_login_net_info_t	net_info;
    volatile sec_login_handle_t		lc;
    volatile error_status_t		lst;
    volatile struct timespec		ts;
    volatile struct timeval		now;
    volatile boolean			do_change = FALSE;
    volatile boolean32			reset;
    volatile sec_login_auth_src_t	auth_src;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Entering keymgmt_handler"));

    /* Let other init stuff get done */
    dced_sleep(120);

    pthread_mutex_lock(&keymgmt_ctlblk.m);
    keymgmt_ctlblk.running = TRUE;
    while (!keymgmt_ctlblk.stop) {
	TRY {
	    st = error_status_ok;
	    ts.tv_sec = 0;
	    ts.tv_nsec = 0;

	    /* We could skip login if the last change_key() attempt
             * was failed (do_change == TRUE). However, the password
             * expiration policy might be changed while we were
             * asleep. Thus, check it again. */
	    if (!keymgmt_ctlblk.suspend) {
		passwd_expired = FALSE;
		/* login as the specified principal */
		sec_login_setup_identity((unsigned_char_p_t)dced_g_info.thishost_prin,
					 (sec_login_machine_princ
					  | sec_login_master_rgy),
					 (sec_login_handle_t *)&lc,
					 (error_status_t *)&st);
		if (st == error_status_ok) {
		    sec_login_valid_from_keytable(lc, rpc_c_authn_dce_secret,
						  NULL,
						  sec_passwd_c_version_none,
						  &old_kvno,
						  (boolean32 *)&reset,
						  (sec_login_auth_src_t *)&auth_src,
						  (error_status_t *)&st);
		    if (st == error_status_ok) {
			if (auth_src == sec_login_auth_src_network && reset) {
			    passwd_expired = TRUE;
			    do_change = FALSE;
			}
		    }
		    else {
			sec_login_purge_context((sec_login_handle_t *)&lc,
						(error_status_t *)&lst);
			if (lst != error_status_ok)
			    dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, lst);
		    }
		}

		if (st == error_status_ok) {
		    sec_login_inquire_net_info(lc,
					       (sec_login_net_info_t *)&net_info,
					       (error_status_t *)&st);

		    if (st == error_status_ok ||
			st == sec_login_s_not_certified) {
			st = error_status_ok;
			has_exp = (net_info.passwd_expiration_date != 0);

			/* What time is it? */
			dce_gettimeofday((struct timeval *)&now);

			if (!has_exp
			    || ((net_info.passwd_expiration_date - now.tv_sec)
				> (2*TEN_MINUTES))) {
			    /* Long (>20 min.) or no expiration, but
			     * only sleep for 10 +/- 2 minutes and
			     * check again, in case things change. In
			     * the worst case, we have 8 minutes (~7
			     * retries) to change the password. */
			    do_change = FALSE;
			}
			else if ((net_info.passwd_expiration_date - now.tv_sec)
				 > TEN_MINUTES) {
			    /* It expires within the next 10~20 mins,
			     * but only sleep for half so that we can
			     * retry in case of failures. In the worst
			     * case, we have 5 minutes (~4 retries) to
			     * change the password. */
			    do_change = FALSE;
			    ts.tv_sec = (net_info.passwd_expiration_date
					 - now.tv_sec) / 2;
			}
			else {
			    /* Otherwise, it expires within the next
			     * 10 mins, change it now.
			     *
			     * Note: If passwd_expiration_date < now,
			     * we end up here. It probably means the
			     * clock skew between us and the registry,
			     * or the password has expired right after
			     * the successful login. By trying the
			     * password change, we'll know the
			     * cause. */
			    do_change = TRUE;
			}
			sec_login_free_net_info((sec_login_net_info_t *)&net_info);
		    }
		    sec_login_purge_context ((sec_login_handle_t *)&lc,
					     (error_status_t *)&lst);
		    if (lst != error_status_ok)
			dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, lst);
		}
		if (st ==  KRB5KDC_ERR_KEY_EXP) {
		    passwd_expired = TRUE;
		    do_change = FALSE;
		}
#ifndef	NO_LOCAL_CHANGE_KEY
		if (passwd_expired) {
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	   "(keymgmt_handler) changing the password with the current context"));
		    sec_key_mgmt_gen_rand_key(rpc_c_authn_dce_secret, NULL,
					      (unsigned_char_p_t)dced_g_info.thishost_prin,
					      &keytype, sec_passwd_c_version_none,
					      (void **) &keydata, (error_status_t *)&st);
		    if (st == error_status_ok) {
			/* We synchronize with
			 * maintain_machine_context() so that it won't
			 * use the half-way updated password. Note the
			 * locking hierarchy. It means that
			 * maintain_machine_context() can not lock
			 * keymgmt_ctlblk.m without releasing
			 * selfcntx_ctlblk.m first! I hope that
			 * keymgmt_change_key() doesn't block
			 * forever. */
			pthread_mutex_lock(&selfcntx_ctlblk.m);
			TRY {
			    /* let change_key choose the next key
                             * version number */
			    keymgmt_change_key(rpc_c_authn_dce_secret, NULL,
					       (unsigned_char_p_t)dced_g_info.thishost_prin,
					       sec_passwd_c_version_none,
					       (sec_passwd_rec_t *)keydata,
					       &garbage_collect_time, (error_status_t *)&st);
			}
			FINALLY {
			    pthread_mutex_unlock(&selfcntx_ctlblk.m);
			}
			ENDTRY
			sec_key_mgmt_free_key((void *)keydata, (error_status_t *)&lst);
			if (lst != error_status_ok)
			    dce_svc_printf(DCED_S_SV_SEC_KEY_MGMT_FAIL_MSG,
					   lst);
			if (st == error_status_ok) {
			    passwd_expired = FALSE;
			    do_change = FALSE;
			    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval,
					   svc_c_debug1,
				   "(keymgmt_handler) changed the password"));
			}
		    }
		}
#endif	/* NO_LOCAL_CHANGE_KEY */
	    }

	    /* Only change if above succeeded, the exp time wasn't 0
	     * (forever) and it's not expired. */
	    if (!keymgmt_ctlblk.suspend
		&& st == error_status_ok && do_change && !passwd_expired) {
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			       "(keymgmt_handler) changing the password"));
		sec_key_mgmt_gen_rand_key(rpc_c_authn_dce_secret, NULL,
					  (unsigned_char_p_t)dced_g_info.thishost_prin,
					  &keytype, sec_passwd_c_version_none,
					  (void **) &keydata,
					  (error_status_t *)&st);
		if (st == error_status_ok) {
		    /* We synchronize with maintain_machine_context()
		     * so that it won't use the half-way updated
		     * password. Note the locking hierarchy. It means
		     * that maintain_machine_context() can not lock
		     * keymgmt_ctlblk.m without releasing
		     * selfcntx_ctlblk.m first! I hope that
		     * sec_key_mgmt_change_key() doesn't block
		     * forever. */
		    pthread_mutex_lock(&selfcntx_ctlblk.m);
		    TRY {
			/* let change_key choose the next key version number */
			sec_key_mgmt_change_key(rpc_c_authn_dce_secret, NULL,
						(unsigned_char_p_t)dced_g_info.thishost_prin,
						sec_passwd_c_version_none,
						(sec_passwd_rec_t *)keydata,
						&garbage_collect_time,
						(error_status_t *)&st);
		    }
		    FINALLY {
			pthread_mutex_unlock(&selfcntx_ctlblk.m);
		    }
		    ENDTRY
		    sec_key_mgmt_free_key((sec_passwd_rec_t *)keydata,
					  (error_status_t *)&lst);
		    if (lst != error_status_ok)
			dce_svc_printf(DCED_S_SV_SEC_KEY_MGMT_FAIL_MSG, lst);
		    if (st == error_status_ok) {
			do_change = FALSE;
			DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval,
				       svc_c_debug1,
			       "(keymgmt_handler) changed the password"));
		    }
		}
		if (st ==  KRB5KDC_ERR_KEY_EXP) {
		    passwd_expired = TRUE;
		    do_change = FALSE;
		}
	    }

	    if (st != error_status_ok)
		dce_svc_printf(DCED_S_SV_SEC_KEY_MGMT_FAIL_MSG, st);
	    else
		backoff = 0;

	    if (keymgmt_ctlblk.suspend || passwd_expired) {
		/* Wait until someone wakes us up.
		 *
		 * In case of the password expiration, we expect that an
		 * administrator notifies us after setting up a new
		 * password. If he/she forgets it and the new password's
		 * lifetime is shorter than 6 hours, we'll lose it again! */
		ts.tv_sec = (6 * ONE_HOUR);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval,
			       svc_c_debug1, "(keymgmt_handler) %s",
			       (keymgmt_ctlblk.suspend?
				"suspended":
				"The password is expired")));
	    }
	    else if (st != error_status_ok && st != KRB5KDC_ERR_KEY_EXP) {
		/* Retry.
		 *
		 * If the password change failed or login failed with
		 * the possibly-expire'able password (last known
		 * status), then retry aggressively. */
		if (do_change || has_exp)
		    ts.tv_sec = ONE_MINUTE;
		else {
		    /* Otherwise, backoff. */
		    ts.tv_sec =
			(backoff < TEN_MINUTES)? backoff+=ONE_MINUTE : backoff;
		}
	    }
	    else if (ts.tv_sec == 0) {
		/* If we have successfully changed the password, we
                 * end up here. If the password's lifetime is less
                 * than 8 (12) minutes, we always (sometimes) lose it!
                 * On the other hand, spinning the thread (keep
                 * changing the password) is not acceptable.
                 * */
		ts.tv_sec = RPC_RANDOM_GET(8*60, 12*60); /* 10+/-2 mins. */
	    }

	    dce_gettimeofday((struct timeval *)&now);
	    ts.tv_sec += now.tv_sec;
	    if (pthread_cond_timedwait(&keymgmt_ctlblk.cv, &keymgmt_ctlblk.m,
				       (struct timespec *)&ts) == 0) {
		/* Signaled. If passwd_expired is true, probably a new
                   password is ready. */
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			       "(keymgmt_handler) signaled."));
		passwd_expired = FALSE;
		backoff = 0;
	    }
	}
	CATCH (pthread_cancel_e) {
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "(keymgmt_handler) cancelled."));
	}
	CATCH_ALL {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, ""), dhd_s_secval,
			   svc_c_sev_error | svc_c_action_none,
			   sec_thr_exit_exc);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "(keymgmt_handler) got exception 0x%p", THIS_CATCH));
	}
	ENDTRY;
    }
    keymgmt_ctlblk.running = FALSE;
    pthread_mutex_unlock(&keymgmt_ctlblk.m);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Exiting keymgmt_handler"));
    return NULL;
}


/*
 * siteupdate_handler - Thread routine to manage pe_site file updates.
 *
 * This thread uses the self context maintained by
 * maintain_machine_context() and doesn't login by itself. Thus, if
 * the self context is expired, it waits until
 * maintain_machine_context() (or somebody else) successfully
 * establishes it and notifies.
 */
static void *
siteupdate_handler (void *arg)
{
    volatile error_status_t	st;
    volatile struct timeval 	now;
    volatile struct timespec	ts;
    volatile boolean		selfcntx_expired = FALSE;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Entering siteupdate_handler"));

    /* Let other init stuff get done */
    dced_sleep(30);

    pthread_mutex_lock(&pesite_ctlblk.m);
    pesite_ctlblk.running = TRUE;
    while (!pesite_ctlblk.stop) {
	TRY {
	    st = error_status_ok;

	    if (!pesite_ctlblk.suspend) {
		selfcntx_expired = FALSE;
		st = scd_update_pesite("", RGY_CONFIG_FILE, NULL);
		if (st != error_status_ok) {
		    dce_svc_printf(DCED_S_SV_UPDATE_PE_SITE_FAIL_MSG, st);
		    switch(st) {
		    case rpc_s_auth_tkt_expired:
		    case sec_login_s_no_current_context:
			/* ?? case sec_rgy_cant_authenticate: */
			/* ?? case KRB5_FCC_NOFILE: */
			/* Any other statuses? */
			selfcntx_expired = TRUE;
			break;
		    }
		}
	    }

	    dce_gettimeofday((struct timeval *)&now);
	    ts.tv_nsec = 0;
	    if (pesite_ctlblk.suspend || selfcntx_expired) {
		/* Wait until someone wakes us up.
		 *
		 * In case of the self context expiration, we expect
		 * that maintain_machine_context() notifies us after
		 * establishing a new context. However, it may forget
		 * the notification because it doesn't know our
		 * state. We consider it non-critical. (If it becomes
		 * critical, we'll need to tell
		 * maintain_machine_context() that we are waiting.) */
		ts.tv_sec = now.tv_sec + (24 * ONE_HOUR);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			       "(siteupdate_handler) %s",
			       (pesite_ctlblk.suspend?
				"suspended":
				"The self context is expired")));
	    }
	    else {
		ts.tv_sec = now.tv_sec
		    + RPC_RANDOM_GET((pesite_delay_time - TWO_MINUTES),
				     (pesite_delay_time + TWO_MINUTES));
	    }
	    if (pthread_cond_timedwait(&pesite_ctlblk.cv, &pesite_ctlblk.m,
				       (struct timespec *)&ts) == 0) {
		/* Signaled. If selfcntx_expired is true, probably a
                   new self context is ready. */
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			       "(siteupdate_handler) signaled."));
		selfcntx_expired = FALSE;
	    }
	}
	CATCH (pthread_cancel_e) {
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "(siteupdate_handler) cancelled."));
	}
	CATCH_ALL {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, ""), dhd_s_secval,
			   svc_c_sev_error | svc_c_action_none,
			   sec_thr_exit_exc);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "(siteupdate_handler) got exception 0x%p",
			   THIS_CATCH));
	}
	ENDTRY;
    }
    pesite_ctlblk.running = FALSE;
    pthread_mutex_unlock(&pesite_ctlblk.m);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Exiting siteupdate_handler"));
    return NULL;
}

/*  Update pe_site file.
 */

void
secval_update_pesite(
    handle_t                    h,
    signed32			delay,			/* delay in seconds */
    error_status_t              *st
)
{
    *st = error_status_ok;

    if (delay != 0)
    {
	if (!dced_authorized(h, &dced_sv_obj_type, &dced_sv_acl,
			     NULL, dced_perm_update, st))
	    return;
    }

    pthread_mutex_lock(&pesite_ctlblk.m);
    if (pesite_ctlblk.stop || !pesite_ctlblk.running) {
	*st = dced_s_sv_not_enabled;
	pthread_mutex_unlock(&pesite_ctlblk.m);
	return;
    }

    if (delay > 0) {
	/* Why TEN_MINUTES? It must be > TWO_MINUTES. */
	pesite_delay_time = MAX(TEN_MINUTES,delay);
	pesite_ctlblk.suspend = FALSE;
    }
    else if (delay < 0) {
	pesite_ctlblk.suspend = TRUE;
    }

    pthread_cond_signal(&pesite_ctlblk.cv);
    pthread_mutex_unlock(&pesite_ctlblk.m);
}


/* Resetup the SELF context.
 *
 * Caller must hold the selfcntx_ctlblk.m mutex. */
static void
resetup_machine_context(
    sec_login_handle_t		*context,
    const error_status_t	in_st,
    const signed32		max_backoff
)
{
    error_status_t	st, st2;
    time_t		backoff = 0;
    struct timeval	now;
    struct timespec	ts;
    boolean		passwd_expired = FALSE;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Entered resetup_machine_context"));

    if (in_st != error_status_ok)
	dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, in_st);
    if (*context != NULL) {
	sec_login_release_context(context, &st2);
	if (st2 != error_status_ok)
	    dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, st2);
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
       "(resetup_machine_context) sec_login_release_context st2 = 0x%08x",
		       st2));
    }
    while (!selfcntx_ctlblk.stop) {
	if (!selfcntx_ctlblk.suspend) {
	    passwd_expired = FALSE;

	    sec_clientd_setup_first_context(&st);
	    if (st == error_status_ok) {
		sec_login_get_current_context(context, &st);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
       "(resetup_machine_context) sec_login_get_current_context st = 0x%08x",
			       st));
		if (st == error_status_ok) {
		    /* If we have retried, wake up
		     * siteupdate_handler() which might be waiting for
		     * the self context. */
		    if (backoff != 0)
			pthread_cond_signal(&pesite_ctlblk.cv);
		    break;
		}
		else {
		    dce_svc_printf(DCED_S_SV_SEC_LOGIN_FAIL_MSG, st);
		}
	    }
	    if (st == KRB5KDC_ERR_KEY_EXP)
		passwd_expired = TRUE;
	}

	dce_gettimeofday(&now);
	ts.tv_nsec = 0;
	if (selfcntx_ctlblk.suspend) {
	    /* Wait until someone wakes us up. */
	    ts.tv_sec = now.tv_sec + (30 * 24 * ONE_HOUR);
	    backoff = 0;	/* Restart. */
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "(resetup_machine_context) suspended"));
	}
	else if (passwd_expired) {
	    /* Wait until someone wakes us up. We expect that an
	     * administrator notifies us after setting up a new
	     * password. */
	    ts.tv_sec = now.tv_sec + ONE_HOUR;
	    backoff = 0;	/* Restart. */
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "(resetup_machine_context) The self password is expired"));
	}
	else {
	    if (backoff < max_backoff)
		backoff += ONE_MINUTE;
	    ts.tv_sec = now.tv_sec + backoff;
	}
	if (pthread_cond_timedwait(&selfcntx_ctlblk.cv, &selfcntx_ctlblk.m,
				   &ts) == 0) {
	    /* Signaled. If passwd_expired is true, probably a new
	       password is ready. */
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "(resetup_machine_context) signaled."));
	    /* Always wake up keymgmt_handler() because secval
             * de/activation means some sort of state change has
             * happened to the host's account and we'd like
             * keymgmt_handler() to pick it up, too. We expect that an
             * administrator won't do that often. */
	    pthread_cond_signal(&keymgmt_ctlblk.cv);
	    backoff = 0;
	}
    }

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Exiting resetup_machine_context"));
}

/*
 * maintain_machine_context - Thread routine to maintain SELF context
 */
static void *
maintain_machine_context( void *arg )
{
    volatile error_status_t	st = error_status_ok;
    volatile sec_login_handle_t	context = NULL;
    boolean32			reset_passwd;
    sec_login_auth_src_t	auth_src;
    volatile signed32		expiration, valid_time;
    volatile struct timeval	now;
    volatile struct timespec	ts;

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Entering maintain_machine_context"));

    pthread_mutex_lock(&selfcntx_ctlblk.m);
    selfcntx_ctlblk.running = TRUE;
    /*
     * If machine context does not exist, set it up.
     */
    sec_login_get_current_context((sec_login_handle_t *)&context,
				  (error_status_t *)&st);
    if (st != error_status_ok) {
	DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
       "maintain_machine_cntxt: sec_login_get_current_context st = 0x%08x",
		       st));
	context = NULL;
	resetup_machine_context((sec_login_handle_t *)&context,
				st, TEN_MINUTES);
    }

    while (!selfcntx_ctlblk.stop) {
	TRY {
	    dce_gettimeofday((struct timeval *)&now);

	    ts.tv_nsec = 0;
	    if (selfcntx_ctlblk.suspend) {
		/* Wait until someone wakes us up. */
		ts.tv_sec = now.tv_sec + (30 * 24 * ONE_HOUR);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			       "maintain_machine_cntxt: suspended"));
	    }
	    else {
		/*
		 * If call fails, loop until we can recreate the creds
		 * and get the new context.
		 */
		sec_login_get_expiration(context, (signed32 *)&expiration,
					 (error_status_t *)&st);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	       "maintain_machine_cntxt: sec_login_get_expiration st = 0x%08x",
			       st));
		if (st != error_status_ok) {
		    resetup_machine_context((sec_login_handle_t *)&context,
					    st, FIVE_MINUTES);
		    continue;
		}

		if (expiration < now.tv_sec) {
		    /* The self context is expired (or will be within
		     * the clock skew). Resetup now. */
		    ts.tv_sec = 0;
		}
		else {
		    /*
		     * Sleep for valid_time - MIN((valid_time / 2), 10m )
		     */
		    valid_time = expiration - now.tv_sec;
		    ts.tv_sec = expiration - (MIN(TEN_MINUTES, valid_time / 2));
		}
	    }

	    if (ts.tv_sec > 0) {
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			       "maintain_machine_cntxt: sleeping for %d seconds",
			       (ts.tv_sec - now.tv_sec)));
		if (pthread_cond_timedwait(&selfcntx_ctlblk.cv,
					   &selfcntx_ctlblk.m,
					   (struct timespec *)&ts) == 0) {
		    /* Signaled. Suspended or resumed? */
		    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
				   "maintain_machine_cntxt: signaled."));
		    /* If resumed, resetup the machine context to pick
                     * up the state change. */
		    if (!selfcntx_ctlblk.stop && !selfcntx_ctlblk.suspend) {
			resetup_machine_context((sec_login_handle_t *)&context,
						error_status_ok, FIVE_MINUTES);
		    }
		    /* Always wake up keymgmt_handler() because secval
		     * de/activation means some sort of state change
		     * has happened to the host's account and we'd
		     * like keymgmt_handler() to pick it up, too. We
		     * expect that an administrator won't do that
		     * often. */
		    pthread_cond_signal(&keymgmt_ctlblk.cv);
		    continue;
		}
	    }

	    if (!selfcntx_ctlblk.stop && !selfcntx_ctlblk.suspend) {
		/*
		 * If either of these following 2 calls fail, loop until we
		 * can recreate the creds and get the new context.
		 */

/* 1 */
		sec_login_refresh_identity((sec_login_handle_t)context,
					   (error_status_t *)&st);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	       "maintain_machine_cntxt: sec_login_refresh_ident st = 0x%08x",
			       st));
		if (st != error_status_ok) {
		    resetup_machine_context((sec_login_handle_t *)&context, st, FIVE_MINUTES);
		    continue;
		}

/* 2 */
		sec_login_validate_first(context, &reset_passwd, &auth_src,
					 (error_status_t *)&st);
		DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	       "maintain_machine_cntxt: sec_login_validate_first st = 0x%08x",
			       st));
		if (st != error_status_ok) {
		    resetup_machine_context((sec_login_handle_t *)&context,
					    st, FIVE_MINUTES);
		}
		/* If (st == error_status_ok
		 *     && auth_src == sec_login_auth_src_network
		 *     && reset_passwd),
		 * then the password is expired. */
	    }
	}
	CATCH (pthread_cancel_e) {
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "maintain_machine_cntxt: cancelled."));
	}
	CATCH_ALL {
	    dce_svc_printf(DCE_SVC(dhd_svc_handle, ""), dhd_s_secval,
			   svc_c_sev_error | svc_c_action_none, sec_thr_exit_exc);
	    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
			   "maintain_machine_cntxt: got exception 0x%p",
			   THIS_CATCH));
	}
	ENDTRY;
    }
    selfcntx_ctlblk.running = FALSE;
    pthread_mutex_unlock(&selfcntx_ctlblk.m);

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
		   "Exiting maintain_machine_context"));
    return NULL;
}


/*
 * remove_bindings_file - Remove dced sec_client string bindings file
 */
void
remove_bindings_file(void)
{

    unlink(sv_clientd_binding);
}

/*
 *  write_bindings - Record bindings in local file.
 */
void
write_dced_bindings( error_status_t	*st )
{
    int	fd;
    FILE	*F;

    fd = open(sv_clientd_binding, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
	dce_svc_printf(DCED_S_SV_OPEN_BINDINGS_FAIL_MSG, strerror(errno));
	return;
    }

    /* Force proper file ownership and access modes */
    fchown(fd, 0, 0);
    fchmod(fd, 0644);
    if ((F = fdopen(fd, "w")) == NULL) {
	dce_svc_printf(DCED_S_SV_OPEN_BINDINGS_FAIL_MSG, strerror(errno));
	(void)close(fd);
	return;
    }
    dce_bindings_to_file(F, dced_bindings, st);
    if (*st != error_status_ok)
	dce_svc_printf(DCED_S_SV_WRITE_BINDINGS_FAIL_MSG, *st);
    fclose(F);
}

/*
 * sec_clientd - Start dced sec_clientd functionality
 *
 * Return TRUE, if non-fatal error. Otherwise, return FALSE
 * (success or fatal). */
boolean32
sec_clientd( error_status_t *st )
{
    pthread_t	tid;

    if (!sv_initialized) {
	*st = -1;	/* Need a status! */
	return FALSE;
    }

    sec_clientd_setup_first_context(st);
    /* password expiration is fatal. */
    if (*st == KRB5KDC_ERR_KEY_EXP)
	return FALSE;
    else if (*st != error_status_ok)
	return TRUE;

    load_override_files(st);
    if (*st != error_status_ok) {
	dce_svc_printf(DCED_S_SV_LOAD_OVERRIDE_FAIL_MSG, *st);
	return FALSE;
    }

    if (pthread_create(&tid, thread_attr,
		       maintain_machine_context, NULL) < 0) {
	dce_svc_printf(DCED_S_THREAD_ERROR_MSG, strerror(errno));
	*st = dced_s_cannot_create_key_mgmt_thr;
	return FALSE;
    }
    pthread_detach(&tid);

    if (pthread_create(&tid, thread_attr,
		       keymgmt_handler, NULL) < 0) {
	dce_svc_printf(DCED_S_THREAD_ERROR_MSG, strerror(errno));
	*st = dced_s_cannot_create_key_mgmt_thr;
	return FALSE;
    }
    pthread_detach(&tid);

    if (pthread_create(&tid, thread_attr,
		       siteupdate_handler, NULL) < 0) {
	dce_svc_printf(DCED_S_THREAD_ERROR_MSG, strerror(errno));
	*st = dced_s_cannot_create_key_mgmt_thr;
	return FALSE;
    }
    pthread_detach(&tid);

    return FALSE;
}

/*
 * rsec_login_validate_cert_auth - RPC for certifying authentication.
 */
void
rsec_login_validate_cert_auth(
    handle_t                    handle,
    error_status_t              *st
)
{
    *st = error_status_ok;
}

/* pk_cache_check_file
 *
 * Function:
 *     Checks to see if a cache file of the correct name
 *     exists.  If it does, does a "stat" to see if it
 *     has been at least 5 minutes since file was updated.
 *     If file does not exist, file is created.
 *
 * In Parameters:
 *     cell     - /.../<cell_name>
 *                cell for which the krbtgt key is being
 *                cached; caller must ensure this is in
 *                canonical form
 *
 * Out Parameters:
 *     file_name - name of cache file is stored in
 *                 array allocated by caller
 *
 * Errors:
 *     dced_s_pkcache_cell_too_long - cell name is too long
 *                 for length of allocated file buffer
 *     dced_s_pkcache_mkdir_error
 *     dced_s_pkcache_cell_invalid - internal error
 *     dced_s_pkcache_key_fresh - key was updated within 5 min.
 */
static error_status_t pk_cache_check_file(
    sec_rgy_name_t       cell,
    char                 **file_name_p
)
{
    char                    *s1; /* cell not yet copied to file_name */
    char                    *s2; /* pointer to next / or last char */
    error_status_t          st;
    int                     est;
    struct stat             stat_buf;
    unsigned32              flen;
           
    /* validate parameters */
    /* File name will be: /opt/dcelocal/etc/security/kdc_pk_auth/<cell>/.key */
    flen=KDC_PUBKEY_CACHE_DIR_LEN + u_strlen(cell) - GLOBAL_DIR_ROOT_LEN + \
         strlen("/") + KDC_PUBKEY_CACHE_FILE_LEN + 1;
    *file_name_p=malloc(flen);
    if (*file_name_p==NULL)
        return dced_s_no_memory;

    /* Make sure base directory exists */        
    strcpy(*file_name_p, KDC_PUBKEY_CACHE_DIR);
    if (mkdir(*file_name_p, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)) {
        if (errno != EEXIST) {
            dce_svc_printf(DCED_S_PKCACHE_MKDIR_ERROR_MSG, strerror(errno));
            return dced_s_pkcache_mkdir_error;
        }
    }

    /* Create each component of cell name directory path */
    /* s1 points to start of new name component */
    /* s2 points to end of new name component */
    /* *file_name_p contains entire name up to new component */
    s1=(char *)cell+GLOBAL_DIR_ROOT_LEN;
    while (*s1) {
        if ((s2=strchr(s1, '/')) == NULL)
            /* no more name components; pick up rest of string */
            s2=s1+strlen(s1)-1;
        strncat(*file_name_p, s1, s2-s1+1);
        if (mkdir(*file_name_p, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH)) {
            if (errno != EEXIST) {
                dce_svc_printf(DCED_S_PKCACHE_MKDIR_ERROR_MSG, strerror(errno));
                return dced_s_pkcache_mkdir_error;
            }
        }
        s1=s2+1;
    }

    /* Append keyfile name */
    u_strcat(*file_name_p, "/");
    u_strcat(*file_name_p, KDC_PUBKEY_CACHE_FILE);

#if 0
    /* XXX-remove for production */
    if (u_strlen(*file_name_p) != flen)
        return dced_s_pkcache_cell_invalid;
#endif

    /* If file already exists, return key fresh if
     * file is less than KEY_CACHE_STALE_SECS old
     */    
    if (stat(*file_name_p, &stat_buf)) {

        /* unable to stat file
         * Ignore error: if something is wrong with the file, we will try to read it
         */
        est=errno;

    } else if (difftime(time(NULL), stat_buf.st_mtime) < KEY_CACHE_STALE_SECS) {

        /* File has been updated within past five minutes
         *
         * Note that we could cache the time the file was
         * written in a static variable in memory, saving
         * the stat calls.  This was not done because there
         * can be an unlimited number of cache files for
         * the krbtgt keys of various cells.  Thus
         * we would either need to cache only the local
         * krbtgt key cache time (and check that cell was
         * local before using memory cache), or we would
         * need to have a fixed array of cached cell names
         * and cache times (and check for a match before
         * using memory cache).
         *
         * Since abuse of this interface affects only
         * the local node, the optimization was not
         * done, but actual timings may prove that
         * caching at least the local krbtgt time is
         * worthwhile.
         */
        return dced_s_pkcache_key_fresh;
    }

    return error_status_ok;
}

/* r s e c _ p k _ c a c h e _ k d c _ k e y
 *
 * rsec_pk_cache_kdc_key - this routine requests dced to
 * obtain and cache, or refresh cache, of public key of
 * the KDC of a specified cell.  The key is cached in
 * the file ${KDC_PUBKEY_CACHE_DIR}/<cell>/${KDC_PUBKEY_CACHE_FILE}
 * (KDC_PUBKEY_CACHE_DIR and _FILE  defined in security/h/rca_fileloc.h).
 *
 * Keys may be retrieved from this file via the
 * sec_pubkey_retrieve() interface using domain sec_pk_domain_pk_kdc_cache
 * or sec_pk_domain_pk_kdc_cache_refresh.  The
 * sec_pubkey_retrieve() interface invokes the
 * rsec_pk_cache_kdc_key interface if the cache is empty or
 * a refresh request is being made.
 *
 * rsec_pk_cache_kdc_key() refuses to refresh an existing
 * cache file if it has been written within the past five
 * minutes.  This is intended to stall denial-of-service
 * attacks.
 *
 * In Parameters
 *   cell           - canonical cell name (/.../cell_name)
 *   usage          - must be "sec_pk_usage_digitalSignature" in the
 *                    reference implementation.
 *   kvno           - pointer to key version number desired
 *                    if NULL, no version number is returned;
 *                    if 0, current version is returned.
 * Out Parameters
 *   kvno           - pointer to key version number being
 *                    returned, or NULL
 *   pubkey_p       - pointer to DER-encoded public key value
 *                    of type sec_pk_pubkey_t
 *
 * Returns:
 *   dced_s_pkcache_cellname_invalid - bad cellname
 *   dced_s_pkcache_usage_unsupp - usage value unsupported
 *   dced_s_pkcache_key_fresh - key was cached recently
 *            (within KEY_CACHE_STALE_SECS)
 */
error_status_t rsec_pk_cache_kdc_key (
    handle_t                handle,     /* [in] */
    sec_rgy_name_t          cell,       /* [in] */
    sec_pk_usage_flags_t    usage,      /* [in] */
    unsigned32              *kvno,      /* [in,out] */
    sec_pk_pubkey_t         *pubkey_p   /* [out] */
)
{
    error_status_t          st,lst;
    int                     est;
    dce_error_string_t      err_string;
    size_t                  cell_name_len;
    sec_rgy_name_t          prin_name;
    char                    *file_name=NULL;  /* allocated in pk_cache_check_file */
    sec_pk_pubkey_t         pubkey;
    
    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_pkcache, svc_c_debug1,
	"ENTER rsec_pk_cache_kdc_key"));

    /* Initialize return parameters */
    st=error_status_ok;
    sec_pk_data_init(pubkey_p);
    sec_pk_data_init(&pubkey);

    /* Verify parameters */
    /* Verify krbtgt principal name created from input cell will
     * fit into a sec_rgy_name_t
     */
    cell_name_len=strlen((char *)cell);
    if ((KRB_DIR_ROOT_LEN + cell_name_len - GLOBAL_DIR_ROOT_LEN)
        > sizeof(sec_rgy_name_t))
        st=dced_s_pkcache_cell_invalid;

    else if (usage != sec_pk_usage_digitalSignature)
        st=dced_s_pkcache_usage_unsupp;

    /* Check cache file name and age */
    if (st==error_status_ok)
        st=pk_cache_check_file(cell, &file_name);

    if (st==error_status_ok) {

        /* Retrieve public key from registry */
        u_strcpy(prin_name, KRB_DIR_ROOT);
        u_strcat(prin_name, cell+GLOBAL_DIR_ROOT_LEN);
        st=sec_pubkey_retrieve((void *)prin_name,
                               &sec_pk_domain_dce_general,
                               usage,
                               kvno,
                               &pubkey);
        if (st != error_status_ok)
            DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_pkcache, svc_c_debug1,
            "rsec_pk_cache_kdc_key: sec_pubkey_retrieve() returned %x", st));
    }

    if (st==error_status_ok) {

        /* Write key to cache file with standard public key keyfile header */
        lst=sec_pubkey_write_keyfile((idl_char *)file_name, &pubkey, kvno, NULL);

        /* Failure to cache the key is not fatal, but we want to know
         * about it if we care to check
         */
        if (lst != error_status_ok) {
            dce_error_inq_text(lst, err_string, &est);
            dce_svc_printf(DCED_S_PKCACHE_KEY_WRITE_FAILURE_MSG, err_string);
        }
   
        /* Set ownership and permissions to ensure file is publicly readable
         */
        if (chown(file_name, getuid(), getgid()) != 0) {
                dce_svc_printf(DCED_S_PKCACHE_KEY_WRITE_FAILURE_MSG, strerror(errno));
                st=dced_s_pkcache_key_write_failure;
        }
        if (chmod(file_name, (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) != 0) {
                dce_svc_printf(DCED_S_PKCACHE_KEY_WRITE_FAILURE_MSG, strerror(errno));
                /* OK for st to overwrite here - same value */
                st=dced_s_pkcache_key_write_failure;
        }
    }

    if (pubkey.data) {
	if (st == error_status_ok) {
	    size_t len = pubkey.len;
	    idl_void_p_t data = rpc_ss_allocate(len);
	    if (data == NULL) {
		st = dced_s_pkcache_key_write_failure; /* little white lie.. */
	    } else {
		pubkey_p->data = data;
		pubkey_p->len = len;
                memcpy(pubkey_p->data, pubkey.data, pubkey.len);
	    }
	}
	sec_pk_data_free(&pubkey);
    }
    /* Free filename */
    if (file_name)
        free(file_name);

    return st;
}
