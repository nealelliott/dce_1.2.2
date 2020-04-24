/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_pvt.c,v $
 * Revision 1.1.53.6  1996/11/13  17:59:29  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/24  17:30 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/7]
 *
 * 	Store local rgy TGT encrypted in user's key.
 * 	[1996/10/21  16:58 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/6]
 *
 * 	Fix bug allocating patype array entries.
 * 	[1996/10/16  21:11 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/5]
 *
 * 	Code cleanup
 * 	[1996/10/15  14:56 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/4]
 *
 * 	Fix local rgy to work with new preauth structure.
 * 	[1996/10/15  13:23 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/3]
 *
 * 	Move translation of krb5 error codes after call to krb5_get_in_tkt.
 * 	[1996/10/11  14:28 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/2]
 *
 * 	Fix DCE Third Party preauthentication.
 * 	Reorder requested preauth types, remove sec_login_pvt_get_preauth.
 * 	[1996/10/09  19:49 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce12_tp_preauth/1]
 *
 * 	Backout fix for OT 13596.
 * 	[1996/07/09  19:47 UTC  jrr  /main/DCE_1.2.2/3]
 *
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:29 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Merge fix for CHFts15754
 * 	HP 	[1995/11/01  17:48 UTC  mullan_s  /main/HPDCE02/mullan_chfts15754/1]
 *
 * 	CHFts15754: get_trusted_preauth - fix bad free of random_key.
 * 	HP 	[1995/10/23  17:29 UTC  burati  /main/HPDCE02/36]
 *
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 	HP 	[1995/10/23  15:35 UTC  burati  /main/HPDCE02/mb_mothra7/4]
 *
 * 	Add changes for login context owner uid (CHFts16467)
 * 	HP 	[1995/10/11  21:50 UTC  burati  /main/HPDCE02/mb_mothra7/3]
 *
 * 	save a couple of instructions...
 * 	HP 	[1995/10/11  21:02 UTC  burati  /main/HPDCE02/mb_mothra7/2]
 *
 * 	save lcp->flags across do_as_request
 * 	HP 	[1995/10/10  23:28 UTC  burati  /main/HPDCE02/mb_mothra7/1]
 *
 * 	Allow bind to master on login
 * 	HP 	[1995/10/04  19:17 UTC  mullan_s  /main/HPDCE02/32]
 *
 * 	Merge fixes for CHFts16496, CHFts16546
 * 	HP 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/3]
 *
 * 	allocate_login_context: put back local rgy fallback because if
 * 	sec_id_global_parse_name is passed a global name and misses
 * 	in the cache, it will go remote.
 * 	HP 	[1995/09/29  20:01 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/2]
 *
 * 	sec_login_pvt_lrgy_to_lc: convert stored pac to epac and store
 * 	in EPAC_CHAIN(lcp) - allows PTGT requests to pass from login contexts
 * 	obtained via local rgy.
 * 	HP 	[1995/09/29  15:37 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 *
 * 	get_and_process_krb_tgt: fetch login context info from local rgy
 * 	if krb5_do_as_request returns KDC_UNREACH (comm failure).
 * 	Remove local rgy lookup from allocate_login_context because the
 * 	call no longer goes remote.
 * 	HP 	[1995/09/28  16:20 UTC  rose_l  /main/HPDCE02/31]
 *
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	[merge to Mainline]
 *
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	HP 	[1995/09/18  18:21 UTC  rps  /main/HPDCE02/30]
 *
 * 	merge
 * 	HP 	[1995/09/18  18:02 UTC  rps  /main/HPDCE02/rps_moth4/1]
 *
 * 	CHFts16415: sec_login_pvt_is_host_cell & cell aliases
 * 	HP 	[1995/09/11  21:13 UTC  aha  /main/HPDCE02/29]
 *
 * 	CHFts15902: allow login to a remote cell only if
 * 	the surrogate account for the remote cell is valid in this cell
 * 	HP 	[1995/09/08  16:39 UTC  aha  /main/HPDCE02/aha_mothra9/2]
 *
 * 	CHFts15902: clean up change code based on review comments
 * 	HP 	[1995/09/07  19:45 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 *
 * 	CHFts15902: allow login to a remote cell only if the
 * 	surrogate account for the remote cell is valid
 * 	HP 	[1995/09/08  17:26 UTC  sommerfeld  /main/HPDCE02/28]
 *
 * 	Merge fix for CHFts15637
 * 	HP 	[1995/08/09  14:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 *
 * 	Call priv_krb_util_construct_v1_stkt_authdata in certify_context.
 * 	HP 	[1995/07/28  15:46 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	synch_refresh_context: remove epacs from .db file before renaming creds file
 * 	to avoid mismatched seals.
 * 	HP 	[1995/07/12  21:22 UTC  mullan_s  /main/HPDCE02/26]
 *
 * 	Merge fix for CHFts15509
 * 	HP 	[1995/07/11  13:34 UTC  mullan_s  /main/HPDCE02/mullan_chfts15509/1]
 *
 * 	H
 *
 * Revision 1.1.53.5  1996/10/15  20:48:15  arvind
 * 	Initialize pag to squelch purify warning.
 * 	[1996/10/10  23:58 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * 	Backout fix for OT 13596.
 * 	[1996/07/09  19:47 UTC  jrr  /main/DCE_1.2.2/3]
 * 
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:29 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge fix for CHFts15754
 * 	HP 	[1995/11/01  17:48 UTC  mullan_s  /main/HPDCE02/mullan_chfts15754/1]
 * 
 * 	CHFts15754: get_trusted_preauth - fix bad free of random_key.
 * 	HP 	[1995/10/23  17:29 UTC  burati  /main/HPDCE02/36]
 * 
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 	HP 	[1995/10/23  15:35 UTC  burati  /main/HPDCE02/mb_mothra7/4]
 * 
 * 	Add changes for login context owner uid (CHFts16467)
 * 	HP 	[1995/10/11  21:50 UTC  burati  /main/HPDCE02/mb_mothra7/3]
 * 
 * 	save a couple of instructions...
 * 	HP 	[1995/10/11  21:02 UTC  burati  /main/HPDCE02/mb_mothra7/2]
 * 
 * 	save lcp->flags across do_as_request
 * 	HP 	[1995/10/10  23:28 UTC  burati  /main/HPDCE02/mb_mothra7/1]
 * 
 * 	Allow bind to master on login
 * 	HP 	[1995/10/04  19:17 UTC  mullan_s  /main/HPDCE02/32]
 * 
 * 	Merge fixes for CHFts16496, CHFts16546
 * 	HP 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/3]
 * 
 * 	allocate_login_context: put back local rgy fallback because if
 * 	sec_id_global_parse_name is passed a global name and misses
 * 	in the cache, it will go remote.
 * 	HP 	[1995/09/29  20:01 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/2]
 * 
 * 	sec_login_pvt_lrgy_to_lc: convert stored pac to epac and store
 * 	in EPAC_CHAIN(lcp) - allows PTGT requests to pass from login contexts
 * 	obtained via local rgy.
 * 	HP 	[1995/09/29  15:37 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 * 
 * 	get_and_process_krb_tgt: fetch login context info from local rgy
 * 	if krb5_do_as_request returns KDC_UNREACH (comm failure).
 * 	Remove local rgy lookup from allocate_login_context because the
 * 	call no longer goes remote.
 * 	HP 	[1995/09/28  16:20 UTC  rose_l  /main/HPDCE02/31]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	[merge to Mainline]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	HP 	[1995/09/18  18:21 UTC  rps  /main/HPDCE02/30]
 * 
 * 	merge
 * 	HP 	[1995/09/18  18:02 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	CHFts16415: sec_login_pvt_is_host_cell & cell aliases
 * 	HP 	[1995/09/11  21:13 UTC  aha  /main/HPDCE02/29]
 * 
 * 	CHFts15902: allow login to a remote cell only if
 * 	the surrogate account for the remote cell is valid in this cell
 * 	HP 	[1995/09/08  16:39 UTC  aha  /main/HPDCE02/aha_mothra9/2]
 * 
 * 	CHFts15902: clean up change code based on review comments
 * 	HP 	[1995/09/07  19:45 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 * 
 * 	CHFts15902: allow login to a remote cell only if the
 * 	surrogate account for the remote cell is valid
 * 	HP 	[1995/09/08  17:26 UTC  sommerfeld  /main/HPDCE02/28]
 * 
 * 	Merge fix for CHFts15637
 * 	HP 	[1995/08/09  14:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata in certify_context.
 * 	HP 	[1995/07/28  15:46 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	synch_refresh_context: remove epacs from .db file before renaming creds file
 * 	to avoid mismatched seals.
 * 	HP 	[1995/07/12  21:22 UTC  mullan_s  /main/HPDCE02/26]
 * 
 * 	Merge fix for CHFts15509
 * 	HP 	[1995/07/11  13:34 UTC  mullan_s  /main/HPDCE02/mullan_chfts15509/1]
 * 
 * 	HP 	[1994/09/29  12:44 UTC  mullan_s  /main/HPDCE01/mullan_junk/1]
 * 
 * 	Fix memory allocation bug in sec_login_pvt_get_namecache.
 * 	HP 	[1994/05/16  19:53 UTC  rose_l  /main/HPDCE01/2]
 * 
 * 	merge sec_login PAG inheritance changes into Rodan(mainline)
 * 	HP 	[1994/05/16  19:21 UTC  rose_l  /main/HPDCE01/rodan_initial/3]
 * 
 * 	clean-up inherit PAG work.
 * 	HP 	[1994/05/11  13:29 UTC  rose_l  /main/HPDCE01/rodan_initial/2]
 * 
 * 	HP 	[1994/04/21  12:37 UTC  rose_l  /main/HPDCE01/rodan_initial/1]
 * 
 * 	inherit PAG from environment option in ...pvt_set_context()
 * 	HP 	[1994/01/26  19:51  mdf  /main/HPDCE01/1]
 * 
 * 	Added check to see if ppac in import_context_data is valid before trying
 * 	to free it.
 * 	[1993/12/17  19:24:05  mdf]
 * 
 * 	Resubmitting fix for DTS bug CHFts09882
 * 	Allow a passwd_override file entry to
 * 
 * Revision 1.1.53.4  1996/10/03  14:45:27  arvind
 * 	fix braino with respect to where the plaintext password really lives.
 * 	[1996/09/25  21:19 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/3]
 * 
 * 	Silence compiler warnings; mark as volatile variables which are
 * 	live across setjmp.
 * 	Deal with kerberos API changes.
 * 	Rip out third-party preauth support for now.
 * 	[1996/09/16  20:08 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	Backout fix for OT 13596.
 * 	[1996/07/09  19:47 UTC  jrr  /main/DCE_1.2.2/3]
 * 
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:29 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge fix for CHFts15754
 * 	HP 	[1995/11/01  17:48 UTC  mullan_s  /main/HPDCE02/mullan_chfts15754/1]
 * 
 * 	CHFts15754: get_trusted_preauth - fix bad free of random_key.
 * 	HP 	[1995/10/23  17:29 UTC  burati  /main/HPDCE02/36]
 * 
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 	HP 	[1995/10/23  15:35 UTC  burati  /main/HPDCE02/mb_mothra7/4]
 * 
 * 	Add changes for login context owner uid (CHFts16467)
 * 	HP 	[1995/10/11  21:50 UTC  burati  /main/HPDCE02/mb_mothra7/3]
 * 
 * 	save a couple of instructions...
 * 	HP 	[1995/10/11  21:02 UTC  burati  /main/HPDCE02/mb_mothra7/2]
 * 
 * 	save lcp->flags across do_as_request
 * 	HP 	[1995/10/10  23:28 UTC  burati  /main/HPDCE02/mb_mothra7/1]
 * 
 * 	Allow bind to master on login
 * 	HP 	[1995/10/04  19:17 UTC  mullan_s  /main/HPDCE02/32]
 * 
 * 	Merge fixes for CHFts16496, CHFts16546
 * 	HP 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/3]
 * 
 * 	allocate_login_context: put back local rgy fallback because if
 * 	sec_id_global_parse_name is passed a global name and misses
 * 	in the cache, it will go remote.
 * 	HP 	[1995/09/29  20:01 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/2]
 * 
 * 	sec_login_pvt_lrgy_to_lc: convert stored pac to epac and store
 * 	in EPAC_CHAIN(lcp) - allows PTGT requests to pass from login contexts
 * 	obtained via local rgy.
 * 	HP 	[1995/09/29  15:37 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 * 
 * 	get_and_process_krb_tgt: fetch login context info from local rgy
 * 	if krb5_do_as_request returns KDC_UNREACH (comm failure).
 * 	Remove local rgy lookup from allocate_login_context because the
 * 	call no longer goes remote.
 * 	HP 	[1995/09/28  16:20 UTC  rose_l  /main/HPDCE02/31]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	[merge to Mainline]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	HP 	[1995/09/18  18:21 UTC  rps  /main/HPDCE02/30]
 * 
 * 	merge
 * 	HP 	[1995/09/18  18:02 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	CHFts16415: sec_login_pvt_is_host_cell & cell aliases
 * 	HP 	[1995/09/11  21:13 UTC  aha  /main/HPDCE02/29]
 * 
 * 	CHFts15902: allow login to a remote cell only if
 * 	the surrogate account for the remote cell is valid in this cell
 * 	HP 	[1995/09/08  16:39 UTC  aha  /main/HPDCE02/aha_mothra9/2]
 * 
 * 	CHFts15902: clean up change code based on review comments
 * 	HP 	[1995/09/07  19:45 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 * 
 * 	CHFts15902: allow login to a remote cell only if the
 * 	surrogate account for the remote cell is valid
 * 	HP 	[1995/09/08  17:26 UTC  sommerfeld  /main/HPDCE02/28]
 * 
 * 	Merge fix for CHFts15637
 * 	HP 	[1995/08/09  14:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata in certify_context.
 * 	HP 	[1995/07/28  15:46 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	synch_refresh_context: remove epacs from .db file before renaming creds file
 * 	to avoid mismatched seals.
 * 	HP 	[1995/07/12  21:22 UTC  mullan_s  /main/HPDCE02/26]
 * 
 * 	Merge fix for CHFts15509
 * 	HP 	[1995/07/11  13:34 UTC  mullan_s  /main/HPDCE02/mullan_chfts15509/1]
 * 
 * 	HP 	[1994/09/29  12:44 UTC  mullan_s  /main/HPDCE01/mullan_junk/1]
 * 
 * 	Fix memory allocation bug in sec_login_pvt_get_namecache.
 * 	HP 	[1994/05/16  19:53 UTC  rose_l  /main/HPDCE01/2]
 * 
 * 	merge sec_login PAG inheritance changes into Rodan(mainline)
 * 	HP 	[1994/05/16  19:21 UTC  rose_l  /main/HPDCE01/rodan_initial/3]
 * 
 * 	clean-up inherit PAG work.
 * 	HP 	[1994/05/11  13:29 UTC  rose_l  /main/HPDCE01/rodan_initial/2]
 * 
 * 	HP 	[1994/04/21  12:37 UTC  rose_l  /main/HPDCE01/rodan_initial/1]
 * 
 * 	inherit PAG from environment optio
 * 
 * Revision 1.1.53.3  1996/08/09  12:03:22  arvind
 * 	Remove the unneded status check from the previous fix.
 * 	[1996/04/16  19:49 UTC  jrr  /main/HPDCE02/jrr_davis_2/2]
 * 
 * 	Fix memory leak in sec_login_pvt_synch_refresh_context().
 * 	[1996/04/05  18:09 UTC  jrr  /main/HPDCE02/jrr_davis_2/1]
 * 
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:29 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge fix for CHFts15754
 * 	HP 	[1995/11/01  17:48 UTC  mullan_s  /main/HPDCE02/mullan_chfts15754/1]
 * 
 * 	CHFts15754: get_trusted_preauth - fix bad free of random_key.
 * 	HP 	[1995/10/23  17:29 UTC  burati  /main/HPDCE02/36]
 * 
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 	HP 	[1995/10/23  15:35 UTC  burati  /main/HPDCE02/mb_mothra7/4]
 * 
 * 	Add changes for login context owner uid (CHFts16467)
 * 	HP 	[1995/10/11  21:50 UTC  burati  /main/HPDCE02/mb_mothra7/3]
 * 
 * 	save a couple of instructions...
 * 	HP 	[1995/10/11  21:02 UTC  burati  /main/HPDCE02/mb_mothra7/2]
 * 
 * 	save lcp->flags across do_as_request
 * 	HP 	[1995/10/10  23:28 UTC  burati  /main/HPDCE02/mb_mothra7/1]
 * 
 * 	Allow bind to master on login
 * 	HP 	[1995/10/04  19:17 UTC  mullan_s  /main/HPDCE02/32]
 * 
 * 	Merge fixes for CHFts16496, CHFts16546
 * 	HP 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/3]
 * 
 * 	allocate_login_context: put back local rgy fallback because if
 * 	sec_id_global_parse_name is passed a global name and misses
 * 	in the cache, it will go remote.
 * 	HP 	[1995/09/29  20:01 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/2]
 * 
 * 	sec_login_pvt_lrgy_to_lc: convert stored pac to epac and store
 * 	in EPAC_CHAIN(lcp) - allows PTGT requests to pass from login contexts
 * 	obtained via local rgy.
 * 	HP 	[1995/09/29  15:37 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 * 
 * 	get_and_process_krb_tgt: fetch login context info from local rgy
 * 	if krb5_do_as_request returns KDC_UNREACH (comm failure).
 * 	Remove local rgy lookup from allocate_login_context because the
 * 	call no longer goes remote.
 * 	HP 	[1995/09/28  16:20 UTC  rose_l  /main/HPDCE02/31]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	[merge to Mainline]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	HP 	[1995/09/18  18:21 UTC  rps  /main/HPDCE02/30]
 * 
 * 	merge
 * 	HP 	[1995/09/18  18:02 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	CHFts16415: sec_login_pvt_is_host_cell & cell aliases
 * 	HP 	[1995/09/11  21:13 UTC  aha  /main/HPDCE02/29]
 * 
 * 	CHFts15902: allow login to a remote cell only if
 * 	the surrogate account for the remote cell is valid in this cell
 * 	HP 	[1995/09/08  16:39 UTC  aha  /main/HPDCE02/aha_mothra9/2]
 * 
 * 	CHFts15902: clean up change code based on review comments
 * 	HP 	[1995/09/07  19:45 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 * 
 * 	CHFts15902: allow login to a remote cell only if the
 * 	surrogate account for the remote cell is valid
 * 	HP 	[1995/09/08  17:26 UTC  sommerfeld  /main/HPDCE02/28]
 * 
 * 	Merge fix for CHFts15637
 * 	HP 	[1995/08/09  14:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata in certify_context.
 * 	HP 	[1995/07/28  15:46 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	synch_refresh_context: remove epacs from .db file before renaming creds file
 * 	to avoid mismatched seals.
 * 	HP 	[1995/07/12  21:22 UTC  mullan_s  /main/HPDCE02/26]
 * 
 * 	Merge fix for CHFts15509
 * 	HP 	[1995/07/11  13:34 UTC  mullan_s  /main/HPDCE02/mullan_chfts15509/1]
 * 
 * 	HP 	[1994/09/29  12:44 UTC  mullan_s  /main/HPDCE01/mullan_junk/1]
 * 
 * 	Fix memory allocation bug in sec_login_pvt_get_namecache.
 * 	HP 	[1994/05/16  19:53 UTC  rose_l  /main/HPDCE01/2]
 * 
 * 	merge sec_login PAG inheritance changes into Rodan(mainline)
 * 	HP 	[1994/05/16  19:21 UTC  rose_l  /main/HPDCE01/rodan_initial/3]
 * 
 * 	clean-up inherit PAG work.
 * 	HP 	[1994/05/11  13:29 UTC  rose_l  /main/HPDCE01/rodan_initial/2]
 * 
 * 	HP 	[1994/04/21  12:37 UTC  rose_l  /main/HPDCE01/rodan_initial/1]
 * 
 * 	inherit PAG from environment option in ...pvt_set_context()
 * 	HP 	[1994/01/26  19:51  mdf  /main/HPDCE01/1]
 * 
 * 	merge
 * 
 * 	One more time, trying to avoid calling free with a
 * 	NULL ppac
 * 	[1993/12/29  21:00:24  mdf]
 * 
 * 	Added check to see if ppac in import_context_data is val
 * 
 * Revision 1.1.53.2  1996/06/04  21:57:43  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:29 UTC  mullan_s  /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1]
 * 
 * 	Merge policy_user_to_user -> sec_rgy_acct_auth_user_to_user flag
 * 	manipulation from mb_u2u
 * 	[1996/05/06  20:45 UTC  burati  /main/DCE_1.2/4]
 * 
 * 	merge u2u work
 * 	[1996/04/29  21:31 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:12 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * 	last endgame sec fix fron hp -mcg
 * 	[1996/04/16  21:00:55  root]
 * 
 * 	*
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  22:58:03  marty]
 * 
 * 	Merge fix for CHFts15754
 * 	HP 	[1995/11/01  17:48 UTC  mullan_s  /main/HPDCE02/mullan_chfts15754/1]
 * 
 * 	CHFts15754: get_trusted_preauth - fix bad free of random_key.
 * 	HP 	[1995/10/23  17:29 UTC  burati  /main/HPDCE02/36]
 * 
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 	HP 	[1995/10/23  15:35 UTC  burati  /main/HPDCE02/mb_mothra7/4]
 * 
 * 	Add changes for login context owner uid (CHFts16467)
 * 	HP 	[1995/10/11  21:50 UTC  burati  /main/HPDCE02/mb_mothra7/3]
 * 
 * 	save a couple of instructions...
 * 	HP 	[1995/10/11  21:02 UTC  burati  /main/HPDCE02/mb_mothra7/2]
 * 
 * 	save lcp->flags across do_as_request
 * 	HP 	[1995/10/10  23:28 UTC  burati  /main/HPDCE02/mb_mothra7/1]
 * 
 * 	Allow bind to master on login
 * 	HP 	[1995/10/04  19:17 UTC  mullan_s  /main/HPDCE02/32]
 * 
 * 	Merge fixes for CHFts16496, CHFts16546
 * 	HP 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/3]
 * 
 * 	allocate_login_context: put back local rgy fallback because if
 * 	sec_id_global_parse_name is passed a global name and misses
 * 	in the cache, it will go remote.
 * 	HP 	[1995/09/29  20:01 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/2]
 * 
 * 	sec_login_pvt_lrgy_to_lc: convert stored pac to epac and store
 * 	in EPAC_CHAIN(lcp) - allows PTGT requests to pass from login contexts
 * 	obtained via local rgy.
 * 	HP 	[1995/09/29  15:37 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 * 
 * 	get_and_process_krb_tgt: fetch login context info from local rgy
 * 	if krb5_do_as_request returns KDC_UNREACH (comm failure).
 * 	Remove local rgy lookup from allocate_login_context because the
 * 	call no longer goes remote.
 * 	HP 	[1995/09/28  16:20 UTC  rose_l  /main/HPDCE02/31]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	[merge to Mainline]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	HP 	[1995/09/18  18:21 UTC  rps  /main/HPDCE02/30]
 * 
 * 	merge
 * 	HP 	[1995/09/18  18:02 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	CHFts16415: sec_login_pvt_is_host_cell & cell aliases
 * 	HP 	[1995/09/11  21:13 UTC  aha  /main/HPDCE02/29]
 * 
 * 	CHFts15902: allow login to a remote cell only if
 * 	the surrogate account for the remote cell is valid in this cell
 * 	HP 	[1995/09/08  16:39 UTC  aha  /main/HPDCE02/aha_mothra9/2]
 * 
 * 	CHFts15902: clean up change code based on review comments
 * 	HP 	[1995/09/07  19:45 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 * 
 * 	CHFts15902: allow login to a remote cell only if the
 * 	surrogate account for the remote cell is valid
 * 	HP 	[1995/09/08  17:26 UTC  sommerfeld  /main/HPDCE02/28]
 * 
 * 	Merge fix for CHFts15637
 * 	HP 	[1995/08/09  14:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata in certify_context.
 * 	HP 	[1995/07/28  15:46 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	synch_refresh_context: remove epacs from .db file before renaming creds file
 * 	to avoid mismatched seals.
 * 	HP 	[1995/07/12  21:22 UTC  mullan_s  /main/HPDCE02/26]
 * 
 * 	Merge fix for CHFts15509
 * 	HP 	[1995/07/11  13:34 UTC  mullan_s  /main/HPDCE02/mullan_chfts15509/1]
 * 
 * 	HP 	[1994/09/29  12:44 UTC  mullan_s  /main/HPDCE01/mullan_junk/1]
 * 
 * 	Fix memory allocation bug in sec_login_pvt_get_namecache.
 * 	HP 	[1994/05/16  19:53 UTC  rose_l  /main/HPDCE01/2]
 * 
 * 	merge sec_login PAG inheritance changes into Rodan(mainline)
 * 	HP 	[1994/05/16  19:21 UTC  rose_l  /main/HPDCE01/rodan_initial/3]
 * 
 * 	clean-up inherit PAG work.
 * 	HP 	[1994/05/11  13:29 UTC  rose_l  /main/HPDCE01/rodan_initial/2]
 * 
 * 	HP 	[1994/04/21  12:37 UTC  rose_l  /main/HPDCE01/rodan_initial/1]
 * 
 * 	inherit PAG from environment option in
 * 
 * Revision 1.1.53.1  1996/05/10  13:13:43  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/12  20:13 UTC  pdd
 * 	Fix several functions that fall off the end without returning a value.
 * 	[1996/02/29  22:00 UTC  pdd  /main/HPDCE02/pdd_branch_CHFts17478/1]
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:07 UTC  psn
 * 	Merge fix for CHFts15754
 * 	[1995/11/02  21:29 UTC  mullan_s  /main/HPDCE02/37]
 * 
 * 	CHFts15754: get_trusted_preauth - fix bad free of random_key.
 * 	[1995/11/01  17:48 UTC  mullan_s  /main/HPDCE02/mullan_chfts15754/1]
 * 
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 	[1995/10/23  17:29 UTC  burati  /main/HPDCE02/36]
 * 
 * 	Add changes for login context owner uid (CHFts16467)
 * 	[1995/10/23  15:35 UTC  burati  /main/HPDCE02/mb_mothra7/4]
 * 
 * 	save a couple of instructions...
 * 	[1995/10/11  21:50 UTC  burati  /main/HPDCE02/mb_mothra7/3]
 * 
 * 	save lcp->flags across do_as_request
 * 	[1995/10/11  21:02 UTC  burati  /main/HPDCE02/mb_mothra7/2]
 * 
 * 	Allow bind to master on login
 * 	[1995/10/10  23:28 UTC  burati  /main/HPDCE02/mb_mothra7/1]
 * 
 * 	Merge fixes for CHFts16496, CHFts16546
 * 	[1995/10/04  19:17 UTC  mullan_s  /main/HPDCE02/32]
 * 
 * 	allocate_login_context: put back local rgy fallback because if
 * 	sec_id_global_parse_name is passed a global name and misses
 * 	in the cache, it will go remote.
 * 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/3]
 * 
 * 	sec_login_pvt_lrgy_to_lc: convert stored pac to epac and store
 * 	in EPAC_CHAIN(lcp) - allows PTGT requests to pass from login contexts
 * 	obtained via local rgy.
 * 	[1995/09/29  20:01 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/2]
 * 
 * 	get_and_process_krb_tgt: fetch login context info from local rgy
 * 	if krb5_do_as_request returns KDC_UNREACH (comm failure).
 * 	Remove local rgy lookup from allocate_login_context because the
 * 	call no longer goes remote.
 * 	[1995/09/29  15:37 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	[merge to Mainline]
 * 
 * 	CHFts16312: OT13110: core dump in sec_login_valid_and_cert_ident() for a refr. tgt.
 * 	[1995/09/28  16:06 UTC  rose_l  /main/HPDCE02/rose_l_CHFts16312/1]
 * 
 * 	merge
 * 	[1995/09/18  18:21 UTC  rps  /main/HPDCE02/30]
 * 
 * 	CHFts16415: sec_login_pvt_is_host_cell & cell aliases
 * 	[1995/09/18  18:02 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	CHFts15902: allow login to a remote cell only if
 * 	the surrogate account for the remote cell is valid in this cell
 * 	[1995/09/11  21:13 UTC  aha  /main/HPDCE02/29]
 * 
 * 	CHFts15902: clean up change code based on review comments
 * 	[1995/09/08  16:39 UTC  aha  /main/HPDCE02/aha_mothra9/2]
 * 
 * 	CHFts15902: allow login to a remote cell only if the
 * 	surrogate account for the remote cell is valid
 * 	[1995/09/07  19:45 UTC  aha  /main/HPDCE02/aha_mothra9/1]
 * 
 * 	make generate_export_token faster:
 * 	add fast-path for token_buf_size == 0
 * 	cache the results, and validate by checking for modified .data
 * 	file.
 * 	[1995/08/31  05:55 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/4]
 * 
 * 	Rename sec_id1 to sec_id_global
 * 	[1995/08/31  00:56 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/3]
 * 
 * 	Eliminate the uuid request from sec_id1_parse_name.
 * 	[1995/08/22  17:29 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 * 
 * 	Performance: let secidmap do the rca_site_bind iff it's needed.
 * 	Support for default.nc system-wide unauthenticated name cache.
 * 	Fix some compiler warnings.
 * 	[1995/08/10  03:10 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:06 UTC  mullan_s  /main/HPDCE02/27]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata in certify_context.
 * 	[1995/08/09  14:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	synch_refresh_context: remove epacs from .db file before renaming creds file
 * 	to avoid mismatched seals.
 * 	[1995/07/28  15:46 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	Merge fix for CHFts15509
 * 	[1995/07/12  21:22 UTC  mullan_s  /main/HPDCE02/26]
 * 
 * 	Translate KRB5KDC_ERR_UNREACH errors from krb5_do_as_request to
 * 	sec_rgy_server_unavailable.
 * 	[1995/07/11  13:34 UTC  mullan_s  /main/HPDCE02/mullan_chfts15509/1]
 * 
 * 	add sec_login_pvt_unset_context().
 * 	[1995/05/02  1
 * 
 * Revision 1.1.39.3  1994/10/25  22:19:52  mullan_s
 * 	[OT12495] Plaintext third party preauth fixes: add code to compose
 * 	new padata and also handle retry requests from server.
 * 	[1994/10/25  22:19:19  mullan_s]
 * 
 * Revision 1.1.39.2  1994/10/24  18:32:48  greg
 * 	CR12159: Add a generation uuid to the login context and incorporate it
 * 	into the login context export token.  This allows the  CDS clerk to
 * 	recognize changes to the *fffffff* host machine credentials immediately.
 * 	[1994/10/24  18:05:38  greg]
 * 
 * Revision 1.1.39.1  1994/10/21  14:23:29  burati
 * 	CR12659 sec_login_pvt_allocate_first() wasn't initializing v1_1 data
 * 	[1994/10/21  14:20:55  burati]
 * 
 * Revision 1.1.24.23  1994/10/17  19:30:49  sommerfeld
 * 	OT11548: fix hole in local preauth protocol
 * 	[1994/10/17  19:27:40  sommerfeld]
 * 
 * Revision 1.1.24.22  1994/09/28  21:34:43  burati
 * 	CR12179 (slowness) Call new rtn krb5_get_credentials_dce()
 * 	[1994/09/28  21:12:35  burati]
 * 
 * Revision 1.1.24.21  1994/09/21  19:07:45  sommerfeld
 * 	[OT9212] when purging context, let kernel know, too, so it can purge
 * 	its caches.
 * 	[1994/09/21  19:06:26  sommerfeld]
 * 
 * Revision 1.1.24.20  1994/09/19  15:00:23  burati
 * 	CR12234 Fix off by 1 on memcpy in sec_login_pvt_get_namecache()
 * 	[1994/09/17  00:03:57  burati]
 * 
 * Revision 1.1.24.19  1994/09/16  21:51:46  sommerfeld
 * 	[OT11915] store pag in login context.
 * 	[1994/09/16  21:46:44  sommerfeld]
 * 
 * Revision 1.1.24.18  1994/09/07  18:23:29  sekhar
 * 	[ OT 11687 ] In sec_login_pvt_get_trusted_preauth()  use open()
 * 	instead of creat() ; also close file descriptor.
 * 
 * 	[ OT 12041 ] Fix memory leaks in sec_login_pvt_trusted_preauth().
 * 	[1994/09/06  18:58:58  sekhar]
 * 
 * Revision 1.1.24.17  1994/08/26  18:47:29  mullan_s
 * 	[OT11557] Subtype DCE preauth data.
 * 	[1994/08/25  18:41:19  mullan_s]
 * 
 * Revision 1.1.24.16  1994/08/24  20:00:23  mullan_s
 * 	[OT11556] Update DCE preauth types to match Kerberos spec.
 * 	[1994/08/24  19:59:34  mullan_s]
 * 
 * Revision 1.1.24.15  1994/08/23  15:11:29  burati
 * 	CR9493 Fix cleanup handling at end of sec_login_pvt_setup_refresh_context()
 * 	[1994/08/23  00:22:40  burati]
 * 
 * Revision 1.1.24.14  1994/08/22  17:03:33  max
 * 	Added crypt prototype for RIOS.  Elimnated unnecessary cast of cyrpt
 * 	return value.
 * 	[1994/08/22  16:05:21  max]
 * 
 * Revision 1.1.24.13  1994/08/19  17:59:14  mdf
 * 	Merged with changes from 1.1.24.12
 * 	[1994/08/19  17:58:48  mdf]
 * 
 * 	Bug fixes from Hewlett Packard
 * 	[1994/08/19  17:53:13  mdf]
 * 
 * Revision 1.1.24.12  1994/08/19  14:58:17  greg
 * 	Fix typo: sec_id_pac_set_encode --> sec_id_epac_set_encode
 * 	[1994/08/19  14:17:14  greg]
 * 
 * Revision 1.1.24.12  1994/08/19  14:58:17  greg
 * 	Fix typo: sec_id_pac_set_encode --> sec_id_epac_set_encode
 * 	[1994/08/19  14:17:14  greg]
 * 
 * Revision 1.1.24.11  1994/08/18  20:25:09  greg
 * 	Make sec_login_pvt_get_ecreds() use a v1 compatibility
 * 	PAC, if that is all that is available, to generate the
 * 	v1_1 EPAC chain.  Otherwise, the sec_login_cred_*
 * 	functions would fail if the security servers were
 * 	pre-1.1.
 * 	[1994/08/18  19:50:22  greg]
 * 
 * 	Minor mods.
 * 	[1994/08/16  18:05:28  greg]
 * 
 * Revision 1.1.24.10  1994/08/04  16:12:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:08:58  mdf]
 * 
 * Revision 1.1.24.9  1994/07/15  14:58:45  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:07  mdf]
 * 
 * Revision 1.1.24.7  1994/06/10  15:07:03  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:47:42  greg]
 * 
 * Revision 1.1.24.6  1994/06/02  21:57:28  mdf
 * 	Merged with changes from 1.1.24.5
 * 	[1994/06/02  21:55:45  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:55  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, fix merge problem
 * 
 * Revision 1.1.24.5  1994/06/02  20:21:01  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:55  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, fix merge problem
 * 
 * Revision 1.1.24.3  1994/05/11  16:49:59  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Remove dead code: get_krb_tgt and process_krb_tgt.
 * 	Do sec_login_set_extended_attrs work.
 * 	Add epac to login context
 * 	Translate KRB5KRB_AP_ERR_BAD_INTEGRITY from krb5_do_as_request
 * 	to sec_rgy_passwd_invalid.
 * 	Added check to see if ppac in import_context_data is valid before trying
 * 	to free it.
 * 	Allow a passwd_override file entry to have null gecos information and still
 * 	be considered a valid entry.
 * 	[1994/04/29  20:45:43  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Remove dead code: get_krb_tgt and process_krb_tgt.
 * 	Do sec_login_set_extended_attrs work.
 * 	Add epac to login context
 * 	Translate KRB5KRB_AP_ERR_BAD_INTEGRITY from krb5_do_as_request
 * 	to sec_rgy_passwd_invalid.
 * 	Added check to see if ppac in import_context_data is valid before trying
 * 	to free it.
 * 	Allow a passwd_override file entry to have null gecos information and still
 * 	be considered a valid entry.
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Remove dead code: get_krb_tgt and process_krb_tgt.
 * 	Do sec_login_set_extended_attrs work.
 * 	Add epac to login context
 * 	Translate KRB5KRB_AP_ERR_BAD_INTEGRITY from krb5_do_as_request
 * 	to sec_rgy_passwd_invalid.
 * 	Added check to see if ppac in import_context_data is valid before trying
 * 	to free it.
 * 	Allow a passwd_override file entry to have null gecos information and still
 * 	be considered a valid entry.
 * 
 * Revision 1.1.24.2  1994/02/04  20:38:16  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:21:34  devsrc]
 * 
 * Revision 1.1.24.1  1994/01/28  23:10:22  burati
 * 	Delegation changes/additions (dlg_bl1)
 * 	[1994/01/19  22:12:02  burati]
 * 
 * Revision 1.1.16.6  1993/11/03  23:26:22  rps
 * 	On behalf of Michael D. Fairbrother <mdf@ch.hp.com>
 * 	It's ok if the status from sec_login_pvt_check_exp is
 * 	rpc_s_auth_tkt_expired.
 * 	[1993/11/03  22:52:21  rps]
 * 
 * Revision 1.1.16.5  1993/10/20  11:23:00  mdf
 * 	Merged with latest changes from backing tree
 * 	[1993/10/20  11:22:16  mdf]
 * 
 * 	CR7776
 * 	Allow for persistent data to be updated in update_idenity_exp().
 * 	[1993/10/18  11:15:26  mdf]
 * 
 * Revision 1.1.16.3  1993/10/15  12:29:27  sekhar
 * 	Merged with changes from 1.1.16.2
 * 	[1993/10/15  12:29:07  sekhar]
 * 
 * 	[OT 7498] Create the namecache file during login (func
 * 		  sec_login_set_context() ) so user can delete it using
 * 		  kdestroy.
 * 	[1993/10/14  20:46:35  sekhar]
 * 
 * Revision 1.1.16.2  1993/10/14  20:02:12  sommerfeld
 * 	Latent support for authentication forwarding
 * 	[1993/10/14  15:56:33  sommerfeld]
 * 
 * Revision 1.1.16.1  1993/10/05  22:30:14  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:06:44  mccann]
 * 
 * Revision 1.1.14.3  1993/08/19  15:00:13  rps
 * 	[CHFts08266] Enforce bsd chown root-only behavior on all systems
 * 	[1993/08/18  22:57:28  rps]
 * 
 * Revision 1.1.14.2  1993/08/16  20:00:48  hanfei
 * 	[8167] incorrect assignment statement for return status.
 * 	[1993/08/16  19:59:18  hanfei]
 * 
 * Revision 1.1.6.21  1993/03/18  16:41:07  burati
 * 	CR7304 Make sure LOCK_DEFAULT_CONTEXT works in if/else statements
 * 	[1993/03/18  16:39:57  burati]
 * 
 * Revision 1.1.6.20  1993/03/17  18:29:47  burati
 * 	CR7497 In sec_login_pvt_allocate_login_context, don't blow away
 * 	the cache file on errout: if a cache_name was passed in.
 * 	[1993/03/17  17:05:29  burati]
 * 
 * Revision 1.1.6.19  1993/03/16  22:45:20  sommerfeld
 * 	[7144] Inform DFS when login context is ready.
 * 	[1993/03/16  22:33:02  sommerfeld]
 * 
 * Revision 1.1.6.18  1993/03/02  21:29:19  sommerfeld
 * 	[7379] fix "binary search to zero" behavior of sec_clientd by
 * 	updating timeout during ticket refresh.
 * 	[1993/03/02  20:19:18  sommerfeld]
 * 
 * Revision 1.1.6.17  1993/02/22  16:42:34  burati
 * 	Optimize the sec_login_pvt_get_default_context() routine, because
 * 	it's being used much more frequently within NSI now.
 * 	[1993/02/22  16:24:31  burati]
 * 
 * Revision 1.1.6.16  1993/02/16  22:41:32  sommerfeld
 * 	[OT6556] partial fix.
 * 	Speed up expiration checking.
 * 	Add sec_login_pvt_check_exp() for quick identity expiration checking.
 * 	Still doesn't update the identity_expiration field in the on-disk
 * 	structure.
 * 	[1993/02/09  02:36:01  sommerfeld]
 * 
 * Revision 1.1.6.15  1993/02/07  22:32:26  burati
 * 	CR6726 Fix as_reply leak Bill found in sec_login_pvt_process_krb_tgt
 * 	[1993/02/07  02:03:58  burati]
 * 
 * Revision 1.1.6.14  1993/02/04  19:51:51  burati
 * 	CR6726 When purging context, purge it from the cache too
 * 	[1993/02/04  18:30:04  burati]
 * 
 * Revision 1.1.6.13  1992/12/29  13:01:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:39:55  zeliff]
 * 
 * Revision 1.1.6.12  1992/12/15  20:16:45  burati
 * 	CR4851 Addendum to previous fix to make it work with DFS clients too
 * 	[1992/12/15  20:13:39  burati]
 * 
 * Revision 1.1.6.11  1992/12/15  18:18:18  burati
 * 	CR4851 New rtn ...set_context with contents of set_default_context
 * 	       and new param to specify whether to set it as default or not
 * 	[1992/12/15  17:14:53  burati]
 * 
 * Revision 1.1.6.10  1992/12/09  19:50:11  burati
 * 	CR5641 Wasn't cleaning up empty cache on failure in
 * 	       sec_login_pvt_allocate_login_context.
 * 	CR5482 Fix from sommerfeld so it won't fall back to machine princ
 * 	       if the ccache specified in KRB5CCNAME isn't readable.
 * 	[1992/12/09  18:41:33  burati]
 * 
 * Revision 1.1.6.9  1992/12/08  18:24:43  burati
 * 	CR6109 Infinite loop in sec_login_pvt_ccache_genname() after
 * 	26 login contexts are created by one pid.  Replace with the
 * 	technique used by the pag credential cache naming for >> 26
 * 	[1992/12/07  23:46:48  burati]
 * 
 * Revision 1.1.6.8  1992/12/04  21:28:31  burati
 * 	CR5978 Handle OS dependent passwd struct correctly
 * 	[1992/12/04  16:43:41  burati]
 * 
 * Revision 1.1.6.7  1992/11/16  23:13:09  burati
 * 	5902 Need to init overridden [out] arg in sec_login_pvt_get_login_info
 * 	[1992/11/16  19:26:51  burati]
 * 
 * Revision 1.1.6.6  1992/10/21  16:51:57  burati
 * 	CR5734 fix memory leak in sec_login_pvt_inq_pac()
 * 	[1992/10/21  14:30:18  burati]
 * 
 * Revision 1.1.6.5  1992/10/15  15:28:46  burati
 * 	CR3610 Flush .nc file when purging context
 * 	[1992/10/14  18:57:35  burati]
 * 
 * Revision 1.1.6.4  1992/10/07  20:23:57  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:06  sekhar]
 * 
 * Revision 1.1.7.2  1992/10/06  15:26:11  burati
 * 	Second replication code drop: bl5
 * 	Add sec_login_pvt_create_raw_context
 * 
 * Revision 1.1.6.3  1992/09/29  21:15:35  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:19:37  sekhar]
 * 
 * Revision 1.1.6.2  1992/08/31  18:16:27  sekhar
 * 	Merge in aix compile fixes
 * 	[1992/08/31  14:56:31  sekhar]
 * 
 * Revision 1.1.2.16  1992/08/12  21:15:21  sekhar
 * 	Pick up the following fix from HP :
 * 
 * 	Adjust context refresh to avoid leaving empty credential cache files.
 * 	This fix should be revisited for DCE 1.0.2
 * 	[1992/08/12  21:14:39  sekhar]
 * 
 * Revision 1.1.2.15  1992/07/13  19:59:40  ahop
 * 	 3903: don't malloc when pepper length is zero in process_krb_tgt
 * 	[1992/07/06  22:50:04  ahop]
 * 
 * 	 3903: args to memcpy of pepper were interchanged in process_krb_tgt
 * 	[1992/06/30  22:26:39  ahop]
 * 
 * Revision 1.1.2.14  1992/07/08  01:16:54  burati
 * 	CR4535 Fix simulated pag generation code.
 * 	[1992/07/06  22:42:46  burati]
 * 
 * Revision 1.1.2.13  1992/06/24  22:03:11  sommerfeld
 * 	CR4212: Remove bogus param passed to VSEM macro.
 * 	[1992/06/24  20:44:23  sommerfeld]
 * 
 * Revision 1.1.2.12  1992/06/17  18:34:52  burati
 * 	CR3482 Fix passing of pointer to num_groups
 * 	[1992/06/17  16:15:35  burati]
 * 
 * Revision 1.1.2.11  1992/06/10  14:23:54  ahop
 * 	 4141: set file mode to 0600 in sec_login_pvt_ccache_gennew
 * 	[1992/06/09  21:18:29  ahop]
 * 
 * Revision 1.1.2.10  1992/06/03  19:00:33  ahop
 * 	 2143: In gennew, filename is now of size SEC_LOGIN_TKT_NAME_MAX_LENGTH
 * 	[1992/06/03  00:13:09  ahop]
 * 
 * 	 CR2143 Replace krb gen_new with sec_login_pvt_ccache_gennew
 * 	[1992/05/22  19:40:40  ahop]
 * 
 * Revision 1.1.2.9  1992/05/22  19:43:48  burati
 * 	Fix unterminated comment, so status is checked on next line.
 * 	Pass back sec_override_fields_t to caller of pvt_get_login_info
 * 	[1992/05/22  19:16:55  burati]
 * 
 * Revision 1.1.2.8  1992/05/14  18:55:58  burati
 * 	Mods to support overrides
 * 	[1992/05/14  01:49:00  burati]
 * 
 * Revision 1.1.2.7  1992/05/08  20:52:49  sommerfeld
 * 	Fix testing of KRB5KDC return codes..
 * 	[1992/05/04  22:28:44  sommerfeld]
 * 
 * 	Remove #if 0'ed code.
 * 	Add call to rca_binding_cache_purge in sec_login_pvt_purge_refresh_context.
 * 	Remove any pretense of sec_login_pvt_free_login_context working if
 * 	the reference count is nonzero.
 * 	Add namecache support hooks.
 * 	Add misplaced sec_rgy_site_close() calls to avoid memory leaks.
 * 	[1992/04/29  03:22:49  sommerfeld]
 * 
 * 	Merge forward from mainline.
 * 	[1992/04/17  22:18:04  sommerfeld]
 * 
 * 	More context cache: fix sec_login_allocate_first,
 * 	sec_login_pvt_create_restricted_context, and
 * 	sec_login_pvt_setup_refresh_context to also enter their contexts
 * 	into the context cache.
 * 	[1992/04/17  15:38:25  sommerfeld]
 * 
 * Revision 1.1.2.6  1992/04/27  19:32:08  burati
 * 	CR2419 Cleanup previous fix (make sure caller status is cleared).
 * 	[1992/04/27  19:28:16  burati]
 * 
 * Revision 1.1.2.5  1992/04/03  21:32:51  burati
 * 	CR2419 Correct status code from allocate_context.
 * 	CR2487 Can't destroy ccache in allocate_context on error.
 * 	[1992/04/02  20:30:07  burati]
 * 
 * Revision 1.1.2.4  1992/03/31  21:37:03  burati
 * 	CR2419 Merged in local registry code
 * 	[1992/03/31  01:28:24  burati]
 * 
 * Revision 1.1.2.3  1992/03/26  16:53:27  burati
 * 	CR2405 Free lcp->identity (was only freeing contents)
 * 	[1992/03/26  01:38:09  burati]
 * 
 * Revision 1.1.2.2  1992/03/24  18:23:53  burati
 * 	CR2106 make_passwd_context wasn't always 'return'ing a value.
 * 	[1992/03/23  21:00:30  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993, 1995
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

/*
 *      DCE Security Credential Management - Private routines
 */

#if !defined(LINT) && !defined(apollo)
static char *VersionID =
    "$Id: sec_login_pvt.c,v 1.1.53.6 1996/11/13 17:59:29 arvind Exp $";
#endif

#include <pthread.h>
#include <un_maloc.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <sys/file.h>
#include <dce/dce.h>
#include <dce/exc_handling.h>

#include <un_strng.h>
#include <bstring.h>
#include <un_time.h>
#include <un_pass.h>
#include <un_stat.h>
#include <un_io.h>

#include <rgymacro.h>

#include <dce/rpc.h>
#include <dce/id_epac.h>
#include <dce/acct.h>
#include <dce/policy.h>
#include <dce/misc.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <dce/secidmap.h>
#include <dce/priv.h>
#include <dce/dce_cf.h>
#include <dce/override.h>
#include <rsecidmap.h>
#include <rsec_login.h>
#include <dce/keymgmt.h>
#include <dce/sec_rgy_attr.h>

#define SEC_LOGIN_PVT_MAIN
#include <sec_login_pvt.h>
#include <sec_login_np.h>
#include <rca_pub.h>
#include <sec_pkl.h>
#include <sec_id_pac_util.h>
#include <sec_krb_util.h>
#include <priv_krb_util.h>
#include <u_str.h>
#include <initstring.h>
#include <sec_lrgy.h>
#include <sec_encode.h>
#include <sec_cred_internal.h>
#include <sec_login_util.h>
#include <krb5/los-proto.h>
#include <krb5/asn1.h>
#include <sec_login_db.h>

#include <sec_svc.h>

#ifdef _AIX
/* RIOS without crypt proto in unistd.h */
char    *crypt (const char*,const char*);
#endif

#ifdef SNI_SVR4
/*
 * have to shorten the suffix to help ensure that the filename
 * stays < 13 chars long (1 for safety so you can see if the
 * filename gets too long since SVR4 silently truncates after
 * the 14th char on s5 filesystems).
 * the #define's for CREDFNAME, SEC_LOGIN_ENV_NAME_LENGTH, and
 * DATAFILE_SUFFIX all have to kept in sync with each other -- rcc
 */

#define DATAFILE_SUFFIX    ".dt"
#else
#define DATAFILE_SUFFIX    ".data"
#endif
#define NAMECACHE_SUFFIX ".nc"

#define DATAFILE_NAME(dfname, ccache) \
    sec_util_string_init(&(dfname), "", \
             krb5_cc_get_name((ccache)), \
             DATAFILE_SUFFIX)
#define NAMECACHE_NAME(ncname, ccache) \
    sec_util_string_init(&(ncname), "", \
             krb5_cc_get_name((ccache)), \
             NAMECACHE_SUFFIX)


/* primitive semaphore operations */
#define P(mutex) pthread_mutex_lock((&mutex))

#define V(mutex)  pthread_mutex_unlock((&mutex))


/* semaphore with exception handling - useful only if the semaphore is acquired
 * and freed within the same lexical scope (the VSEM macro takes no parameter to
 * emphasize this contraint).  Can be nested within other exception
 * handling scopes, but may not "straddle" any other such scope.
 */
#define PSEM(mutex) { \
            pthread_mutex_t  this_mutex; \
            this_mutex = mutex; \
            P(this_mutex); \
            TRY {

#define VSEM \
            } CATCH_ALL { \
                V(this_mutex); \
                RERAISE; \
            } \
            ENDTRY \
            V(this_mutex); \
        }

/* Default context mutex */
static pthread_mutex_t  def_con_mutex;
static boolean32        def_con_mutex_inited = false;
static pthread_once_t   def_con_mutex_once = pthread_once_init;

#define LOCK_DEFAULT_CONTEXT { \
    if (!def_con_mutex_inited) { \
        pthread_once(&def_con_mutex_once, init_def_con_mutex); \
     } \
    PSEM(def_con_mutex);

#define UNLOCK_DEFAULT_CONTEXT  VSEM }


/* Login context mutex */
static pthread_mutex_t  lc_mutex;
static boolean32	lc_mutex_inited = false;
static pthread_once_t	lc_mutex_once = pthread_once_init;


/* Bind to master flag mutex */
static pthread_mutex_t  btm_mutex;
static boolean32	btm_mutex_inited = false;
static pthread_once_t	btm_mutex_once = pthread_once_init;
unsigned32              btm_flag;

#define INC_BTM_FLAG { \
    if (!btm_mutex_inited) { \
        pthread_once(&btm_mutex_once, init_btm_mutex); \
    } \
    PSEM(btm_mutex); \
    btm_flag++; \
    VSEM \
    }

#define DEC_BTM_FLAG { \
    PSEM(btm_mutex); \
    btm_flag--; \
    VSEM \
    }

/* Useful macro to cleanup contents of a krb5_keyblock */
#define cleanup_key(key) { \
    if ((key)->contents) { \
        memset((char *)((key)->contents), 0, (key)->length); \
        free((key)->contents); \
    } \
}

/*
 * Forward declarations
 */
PRIVATE void sec_login_pvt_ccache_swap (
    sec_login_context_t   *lcp,
    sec_login_context_t   *rcp,
    error_status_t        *stp
);

PRIVATE  void  sec_login_pvt_ccache_genname (
    char                 **name,
    error_status_t       *stp
);

PRIVATE void update_identity_exp (
    sec_login_context_t *lcp,
    error_status_t *stp
);

PRIVATE void copy_creds (
    krb5_creds      *in,
    krb5_creds      *out,
    error_status_t  *stp
);

PRIVATE void copy_tgt_data (
    tgt_request_data_t	*stgt_data,
    tgt_request_data_t	**dtgt_data,
    error_status_t	*stp
);

/*
 * End of forward declarations
 */

PRIVATE void  init_def_con_mutex ( void )
{
    pthread_mutex_init(&def_con_mutex, pthread_mutexattr_default);
    def_con_mutex_inited = true;
}

PRIVATE void  init_lc_mutex ( void )
{
    pthread_mutex_init(&lc_mutex, pthread_mutexattr_default);
    lc_mutex_inited = true;
}

PRIVATE void init_btm_mutex ( void )
{
    pthread_mutex_init(&btm_mutex, pthread_mutexattr_default);
    btm_flag = 0;
    btm_mutex_inited = true;
}


PRIVATE void init_db_mutex(
    pthread_mutex_t		*db_mutex,
    error_status_t		*stp
)
{
    CLEAR_STATUS(stp);

    if (pthread_mutex_init(db_mutex, pthread_mutexattr_default) == -1) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s%s"),
                       sec_s_login, svc_c_sev_error | svc_c_action_none,
		       sec_sys_errno_text, "pthread_mutex_init",
		       strerror(errno));
        SET_STATUS(stp, sec_login_s_internal_error);
    }
}


/*
 * Define an expected value for the number of groups in a groupset.  On some
 * systems there is an NGROUPS macro (which is usually 8). If so, we use it
 * otherwise we define our own
 */
#ifdef NGROUPS   /* might already be defined in a system include file */
#    define SECLOGIN_NGROUPS   NGROUPS
#else
#    define SECLOGIN_NGROUPS   8
#endif

/*
 * static storage for default context handle, set/retrieved via
 * sec_login_pvt_get/set_default_context
 */
typedef struct sec_login_pvt_default_handle_t {
    boolean32           valid;
    sec_login_handle_t  handle;
} sec_login_pvt_default_handle_t;

static sec_login_pvt_default_handle_t  sec_login_pvt_default_context = {
    false,
    sec_login_default_handle
};

/* Global state used to find a starting point for unused ccache names */
static  unsigned32      unique_cred_val = 0;
static  boolean32       sec_login_ucv__inited = false;
static  pthread_mutex_t sec_login_ucv__mutex;

#define RGY_AUTH_INFO_DCE(ai) (ai).tagged_union.dce_info

#define PW_MAX_NAME_LEN (sizeof(sec_rgy_name_t))
#define PW_MAX_PASSWD_LEN (sizeof(sec_rgy_unix_passwd_buf_t))
#define PW_MAX_CLASS_LEN (sizeof(sec_rgy_unix_passwd_buf_t))
#define PW_MAX_GECOS_LEN (sizeof(sec_rgy_name_t))
#define PW_MAX_DIR_LEN (sizeof(sec_rgy_name_t))
#define PW_MAX_SHELL_LEN (sizeof(sec_rgy_name_t))

/* Login set identifier */
static uuid_t login_set_id = { /* 6d8d97bc-dd2d-11cc-b1cc-080009353559 */
    0x6d8d97bc,
    0xdd2d,
    0x11cc,
    0xb1,
    0xcc,
    {0x08, 0x00, 0x09, 0x35, 0x35, 0x59}
};

/* Pathname of DCE Configuration file */
#define DCE_CONFIG_FILE DCELOCAL_PATH "/dce_cf.db"

/*
 * Thread-safe wrapper around sec_login_cache_enter().
 * Ensures that 2 threads cannot enter 2 login contexts
 * in the cache using the same underlying credential files.
 */
PRIVATE void lookup_and_cache_enter(
    sec_login_context_t		*lcp	/* [in, out] */
)
{
    sec_login_context_t		*tmp_lcp;

    /* Initialize lc_mutex */
    if (!lc_mutex_inited) {
        pthread_once(&lc_mutex_once, init_lc_mutex);
    }

    pthread_mutex_lock(&lc_mutex);

    /* check if another thread already entered this context in the cache */
    tmp_lcp = sec_login_cache_lookup(lcp->context_name_len, lcp->context_name, 
				     &lcp->generation_id);
    if (tmp_lcp) {
	/* free the login context which was passed in */
	sec_login_pvt_free_login_context(lcp);
	/* assign lcp to the new tmp_lcp */
	lcp = tmp_lcp;
    } else {
        lcp->refcount = 1;
        sec_login_cache_enter(lcp);
        lcp->magic = lcp;
    }

    pthread_mutex_unlock(&lc_mutex);
}

/*
 * compute the effective account expiration date
 */
PRIVATE  sec_timeval_sec_t  effective_acct_expiration
  (
    sec_rgy_acct_admin_t  *admin,
    sec_rgy_plcy_t        *policy
  )
{
    sec_timeval_sec_t  acct_exp;

    if (policy->acct_lifespan == 0 /* live forever */) {
        acct_exp = admin->expiration_date;
    } else if (admin->expiration_date == 0  /* never expire */) {
        acct_exp = admin->creation_date + policy->acct_lifespan;
    } else {
        acct_exp = MIN(admin->expiration_date,
                      (admin->creation_date + policy->acct_lifespan));
    }

    return acct_exp;
}


/*
 * compute the effective passwd expiration date
 */
PRIVATE  sec_timeval_sec_t  effective_passwd_expiration
  (
    sec_rgy_acct_user_t  *user,
    sec_rgy_plcy_t       *policy
  )
{
    sec_timeval_sec_t    passwd_exp, tmp_exp;

    if (policy->passwd_lifetime == 0) {
        /* Use expiration date, unless password's been changed since then */
        passwd_exp = (policy->passwd_exp_date > user->passwd_dtm) ?
                                                policy->passwd_exp_date : 0;
    } else if (policy->passwd_exp_date == 0) {
        passwd_exp = user->passwd_dtm + policy->passwd_lifetime;
    } else {

        /* Expiration date, unless password's been changed since then */
        tmp_exp = (policy->passwd_exp_date > user->passwd_dtm) ?
                                                policy->passwd_exp_date : 0;
        if (tmp_exp == 0) {
            passwd_exp = user->passwd_dtm + policy->passwd_lifetime;
        } else {
            passwd_exp = MIN(tmp_exp,
                                (user->passwd_dtm + policy->passwd_lifetime));
        }
    }

    return passwd_exp;
}


/*
 *  f r e e _ p a s s w d _ c o n t e x t
 *
 *  Free memory used by a passwd struct.
 */
PRIVATE  void free_passwd_context
  (
    struct sec_passwd  **context_pwent
  )
{
    if (*context_pwent != NULL) {
        if ((*context_pwent)->pw_name != NULL) {
            free((*context_pwent)->pw_name);
            (*context_pwent)->pw_name = NULL;
        }
        free( (char *) *context_pwent);
        *context_pwent = NULL;
    }
}


/*
 *  a l l o c _ p w e n t _ s t r i n g _ b u f f e r s
 *
 * allocate buffers for all pwent character strings as a single block with
 * pw_name pointing to the head of the block, and all successive string fields
 * pointing to the appropriate offset within the allocated block.  pw_comment
 * is set to NULL.
 */
PRIVATE void alloc_pwent_string_buffers
  (
    struct sec_passwd  *pwent,
    error_status_t     *stp
  )
{
    CLEAR_STATUS(stp);

    pwent->pw_name = malloc(PW_MAX_NAME_LEN + PW_MAX_PASSWD_LEN +
                            PW_MAX_CLASS_LEN +  PW_MAX_GECOS_LEN +
                            PW_MAX_DIR_LEN + PW_MAX_SHELL_LEN);

    if (pwent->pw_name == NULL) {
        MEM_FAILURE(stp);
        return;
    }

    pwent->pw_passwd = pwent->pw_name + PW_MAX_NAME_LEN;
    pwent->pw_class = pwent->pw_passwd + PW_MAX_PASSWD_LEN;
    pwent->pw_gecos = pwent->pw_class + PW_MAX_CLASS_LEN;
    pwent->pw_dir = (pwent->pw_gecos + PW_MAX_GECOS_LEN);
    pwent->pw_shell = (pwent->pw_dir + PW_MAX_DIR_LEN);

}


/*
 *  m a k e _ p a s s w d _ c o n t e x t
 *
 *  Allocate a passwd struct and stuff it with input data.
 */
PRIVATE  boolean32  make_passwd_context
  (
    struct sec_passwd       **context_pwent,        /* [out] */
    unsigned_char_p_t       name,                   /* [in]  */
    sec_rgy_unix_sid_t      *unix_sid,              /* [in]  */
    sec_rgy_acct_user_t     *user_part,             /* [in]  */
    sec_rgy_acct_admin_t    *admin_part,            /* [in]  */
    sec_rgy_plcy_t          *policy,                /* [in]  */
    unsigned_char_p_t       class,                  /* [in]  */
    error_status_t          *stp
  )
{
    char                        *name_ptr;
    char                        *colon_ptr;
    signed32                    name_len = u_strlen(name);

    *context_pwent =  (struct sec_passwd *) malloc(sizeof(struct sec_passwd));
    if (*context_pwent == NULL) {
        SET_STATUS(stp, sec_login_s_no_memory);
        return false;
    }

    alloc_pwent_string_buffers(*context_pwent, stp);
    if (BAD_STATUS(stp)) {
        free_passwd_context(context_pwent);
        return false;
    }

    if(name_len > PW_MAX_NAME_LEN - 1) {
        SET_STATUS(stp, sec_rgy_bad_data);
        free_passwd_context(context_pwent);
        return false;
    }
    strncpy((*context_pwent)->pw_name, (char *)name, name_len);
    (*context_pwent)->pw_name[name_len] = '\0';

    /* Store print representation of passwd. */
    strncpy((*context_pwent)->pw_passwd, (char *) user_part->passwd,
            PW_MAX_PASSWD_LEN);
    (*context_pwent)->pw_passwd[PW_MAX_PASSWD_LEN - 1] = '\0';

    /* Currently, class is not supported by rgy. */
#ifdef NOTDEF
    strncpy((*context_pwent)->pw_class, (char *)class, PW_MAX_CLASS_LEN);
    (*context_pwent)->pw_class[PW_MAX_CLASS_LEN - 1] = '\0';
#endif
    (*context_pwent)->pw_class[0] = '\0';

    strncpy((*context_pwent)->pw_gecos, (char *)user_part->gecos, PW_MAX_GECOS_LEN);
    (*context_pwent)->pw_gecos[PW_MAX_GECOS_LEN - 1] = '\0';

    strncpy((*context_pwent)->pw_dir, (char *)user_part->homedir, PW_MAX_DIR_LEN);
    (*context_pwent)->pw_dir[PW_MAX_DIR_LEN - 1] = '\0';

    strncpy((*context_pwent)->pw_shell, (char *)user_part->shell, PW_MAX_SHELL_LEN);
    (*context_pwent)->pw_shell[PW_MAX_SHELL_LEN - 1] = '\0';

    (*context_pwent)->pw_uid    = unix_sid->person;
    (*context_pwent)->pw_gid    = unix_sid->group;

    (*context_pwent)->pw_change =
        (time_t)effective_passwd_expiration(user_part, policy);

    (*context_pwent)->pw_expire =
        (time_t) effective_acct_expiration(admin_part, policy);

    return true;
}


PRIVATE void  allocate_identity
  (
    login_info_t    **id_p,
    error_status_t  *stp
  )
{
    CLEAR_STATUS(stp);

    *id_p = (login_info_t *) malloc(sizeof(login_info_t));
    if (*id_p == NULL) {
        SET_STATUS(stp, sec_login_s_no_memory);
        return;
    }

    memset(*id_p, 0, sizeof(login_info_t));
    (*id_p)->net_creds.pac_type  = sec_id_pac_format_v1;
}

PRIVATE  void  free_identity
  (
    login_info_t    **id_p
  )
{
    if (id_p == NULL) {
        return;
    }

    if ((*id_p) != NULL) {
        sec_id_pac_util_free(&(*id_p)->net_creds);
        free_passwd_context(&((*id_p)->unix_info.pwent));
        if ((*id_p)->unix_info.groups != NULL) {
            free( (char *) (*id_p)->unix_info.groups);
        }
	if ((*id_p)->unix_info.os_pwent != NULL) {
	    free( (char *) (*id_p)->unix_info.os_pwent);
	}
        free((char *)*id_p);
        *id_p = NULL;
    }
}


PRIVATE void allocate_krb_request_info
  (
    tgt_request_data_t **rinfo,
    error_status_t     *stp
  )
{
    CLEAR_STATUS(stp);

    *rinfo = (tgt_request_data_t *) malloc(sizeof(tgt_request_data_t));
    if (*rinfo == NULL) {
        SET_STATUS(stp, sec_login_s_no_memory);
        return;
    }

   memset(*rinfo, 0, sizeof(tgt_request_data_t));
}


PRIVATE void free_krb_request_info
  (
    tgt_request_data_t **rinfo
  )
{
    if (*rinfo == NULL) {
        return;
    }

    if ((*rinfo)->reply.length != 0 && (*rinfo)->reply.data != NULL) {
        free((*rinfo)->reply.data);
    }

    krb5_free_cred_contents(&(*rinfo)->creds);

    if ((*rinfo)->addrs != NULL) {
        krb5_free_addresses((*rinfo)->addrs);
    }

    free((char *) *rinfo);
    *rinfo = NULL;
}


PRIVATE  void  allocate_krb_info
  (
    krb_info_t           *krb_info_p,
    char                 *cache_name,
    error_status_t       *stp
  )
{

    memset(krb_info_p, 0, sizeof(*krb_info_p));

    if (cache_name != NULL) {
        /*
         * allocate a cache handle for "cache_name".  cache_name
         * must include a type prefix (FILE:).
         */
        *stp = krb5_cc_resolve (cache_name, &krb_info_p->cache);
    } else {
        /*
         * allocate a new kerberos cache
         *
         */
        sec_login_pvt_ccache_gennew(&krb_info_p->cache, stp);
    }
}


PRIVATE  void free_krb_info
  (
    krb_info_t  *krb_info_p
  )
{

    if (krb_info_p != NULL) {

        if (krb_info_p->cache != NULL) {
            krb5_cc_close(krb_info_p->cache);
        }

        if (krb_info_p->pepper.length && krb_info_p->pepper.contents) {
            free(krb_info_p->pepper.contents);
        }

        if (krb_info_p->tgt_data != NULL) {
            free_krb_request_info(&krb_info_p->tgt_data);
        }

        if (krb_info_p->num_caches != 0 && krb_info_p->prev_caches != NULL) {
            int i;
            for (i = 0; i < krb_info_p->num_caches; i++) {
                free(krb_info_p->prev_caches[i]);
            }
            free((char *) krb_info_p->prev_caches);
       }

        memset(krb_info_p, 0, sizeof(*krb_info_p));
    }
}

PRIVATE  void  copy_id (
    sec_id_t        *dest,
    sec_id_t        *src,
    error_status_t  *stp
)
{

    CLEAR_STATUS(stp);
    dest->uuid = src->uuid;
    if (src->name != NULL) {
	u_dynstrcpy(dest->name, src->name);
	if (dest->name == NULL) {
	    SET_STATUS(stp, sec_login_s_no_memory);
	    return;
	}
    }
}


PRIVATE  void  copy_restrictions (
    sec_id_restriction_set_t  *dest,
    sec_id_restriction_set_t  *src,
    error_status_t            *stp
)
{
    unsigned16 i;
    sec_id_restriction_t *srp, *drp;

    CLEAR_STATUS(stp);
    if (!src || !src->num_restrictions)
	return;

    dest->restrictions = 
	(sec_id_restriction_t *) malloc(src->num_restrictions);
    if (dest->restrictions == NULL) {
	SET_STATUS(stp, sec_login_s_no_memory);
	return;
    }

    memset(dest->restrictions, 0, 
	   sizeof(sec_id_restriction_t) * src->num_restrictions);

    srp = &src->restrictions[0];
    drp = &src->restrictions[0];

   for (i = 0; 
    i < src->num_restrictions && STATUS_OK(stp); 
    i++, drp++, srp++) {
	switch ( srp->entry_info.entry_type) {
	case sec_rstr_e_type_user:
	case sec_rstr_e_type_group:
	case sec_rstr_e_type_foreign_other:
	    copy_id(&drp->entry_info.tagged_union.id,
		    &srp->entry_info.tagged_union.id, 
		    stp);
	    break;
	    
	case sec_rstr_e_type_foreign_user:
	case sec_rstr_e_type_foreign_group:
	    copy_id(&drp->entry_info.tagged_union.foreign_id.id,
		    &srp->entry_info.tagged_union.foreign_id.id, 
		    stp);
	    if (GOOD_STATUS(stp)) {
		copy_id(&drp->entry_info.tagged_union.foreign_id.realm,
			&srp->entry_info.tagged_union.foreign_id.realm, 
			stp);
	    }
	}
    }
}

PRIVATE void copy_dlg_token_set (
    sec_dlg_token_set_t  *dest,
    sec_dlg_token_set_t  *src,
    error_status_t       *stp
)
{
    unsigned32           i;

    CLEAR_STATUS(stp);
    memset(dest, 0, sizeof(*dest));

    if (!src || !src->num_tokens || !src->tokens) return;

    dest->tokens = (sec_dlg_token_t *) malloc(src->num_tokens * sizeof(sec_dlg_token_t));
    if (dest->tokens == NULL) {
	SET_STATUS(stp, sec_login_s_no_memory);
	return;
    }
    
    dest->num_tokens = src->num_tokens;
    memset( dest->tokens, 0, src->num_tokens * sizeof(sec_dlg_token_t));
    for (i = 0; i < dest->num_tokens; i++) {
	dest->tokens[i].token_bytes.bytes = 
	    (idl_byte *) malloc(src->tokens[i].token_bytes.num_bytes);
	if (dest->tokens[i].token_bytes.bytes == NULL) {
	    SET_STATUS(stp, sec_login_s_no_memory);
	    break;
	}
	dest->tokens[i].token_bytes.num_bytes =
	    src->tokens[i].token_bytes.num_bytes;
	memcpy(dest->tokens[i].token_bytes.bytes,
	       src->tokens[i].token_bytes.bytes,
	       dest->tokens[i].token_bytes.num_bytes);
	dest->tokens[i].expiration = src->tokens[i].expiration;
    }
    
}

PRIVATE void copy_dlg_request_info (
    sec_id_delegation_type_t    dlg_type,       /* [in] */
    sec_id_restriction_set_t    *dlg_rstrs,     /* [in] */
    sec_id_restriction_set_t    *tgt_rstrs,     /* [in] */
    sec_id_opt_req_t            *opt_rstrs,     /* [in] */
    sec_id_opt_req_t            *req_rstrs,     /* [in] */
    sec_bytes_t                 *dlg_chain,     /* [in] */
    sec_dlg_token_set_t         *dlg_token_set, /* [in] */
    sec__login_dlg_req_info_t   *req_info,     /* [out] */
    error_status_t              *stp            /* [out] */  
)
{
    sec__login_dlg_req_info_t   *rip = req_info;   

    memset(rip, 0, sizeof(*rip));

    rip->dlg_type = dlg_type;
    
    if (dlg_chain && dlg_chain->num_bytes && dlg_chain->bytes) {
	rip->dlg_chain.bytes = (idl_char *) malloc(dlg_chain->num_bytes);
	if (rip->dlg_chain.bytes == NULL) {
	    MEM_FAILURE(stp);
	    goto failure;
	}
	rip->dlg_chain.num_bytes = dlg_chain->num_bytes;
	memcpy(rip->dlg_chain.bytes, dlg_chain->bytes, dlg_chain->num_bytes);
    } 
    
    if (dlg_token_set && dlg_token_set->num_tokens && dlg_token_set->tokens) {
	copy_dlg_token_set(&rip->dlg_token_set, dlg_token_set, stp);
	if (BAD_STATUS(stp)) {
	    goto failure;
	}	   
    }
    
    if (opt_rstrs && opt_rstrs->restriction_len) {
	rip->opt_rstrs.restrictions = 
	    (idl_byte *) malloc(opt_rstrs->restriction_len);
	if (rip->opt_rstrs.restrictions == NULL) {
	    MEM_FAILURE(stp);
	    goto failure;
	}
	rip->opt_rstrs.restriction_len = opt_rstrs->restriction_len;
    }
    
    if (req_rstrs && req_rstrs->restriction_len) {
	rip->req_rstrs.restrictions = 
	    (idl_byte *) malloc(req_rstrs->restriction_len);
	if (rip->req_rstrs.restrictions == NULL) {
	    MEM_FAILURE(stp);
	    goto failure;
	}
	rip->req_rstrs.restriction_len = req_rstrs->restriction_len;
   }
    
    if (tgt_rstrs && tgt_rstrs->num_restrictions) {
	copy_restrictions(&rip->tgt_rstrs, tgt_rstrs, stp);
	if (BAD_STATUS(stp)) {
	    goto failure;
	}	   
    }
    
    if (dlg_rstrs && dlg_rstrs->num_restrictions) {
	copy_restrictions(&rip->dlg_rstrs, dlg_rstrs, stp);
    }
    
    if (STATUS_OK(stp)) {
	return;
    }
   
failure:
   if (rip->dlg_token_set.tokens) 
       sec_encode_dlg_token_set_free(free, &rip->dlg_token_set);
   if (rip->dlg_chain.bytes) 
       free(rip->dlg_chain.bytes);
   if (rip->opt_rstrs.restrictions)
       sec_encode_opt_req_free(free, &rip->opt_rstrs);
   if (rip->req_rstrs.restrictions)
       sec_encode_opt_req_free(free, &rip->req_rstrs);
   if (rip->tgt_rstrs.restrictions)
       sec_encode_restriction_set_free(free, &rip->tgt_rstrs);
   if (rip->dlg_rstrs.restrictions)
       sec_encode_restriction_set_free(free, &rip->dlg_rstrs);
   
   }

/*
 * copy_tgt_data
 *
 * Copies TGT data from src tgt to dest tgt
 * typically useful for copying the refreshed 
 * TGT data to a callers login context TGT
 *
 */
PRIVATE void copy_tgt_data (
    tgt_request_data_t	*stgt_data,
    tgt_request_data_t	**dtgt_data,
    error_status_t	*stp
    )
{
    krb5_data *srep, *drep;

    CLEAR_STATUS(stp);

    /* allocate the destination */
    allocate_krb_request_info(dtgt_data, stp);
    if (BAD_STATUS(stp)) {
	return;
    }

    /* copy address */
    if (stgt_data->addrs != NULL) {
	*stp = krb5_copy_addresses(stgt_data->addrs, &((*dtgt_data)->addrs));
	if (BAD_STATUS(stp)) {
	    free_krb_request_info(dtgt_data);
	    return;
	}
    }

    /* copy credentials */
    copy_creds(&(stgt_data->creds), &((*dtgt_data)->creds), stp);
    if (BAD_STATUS(stp)) {
	free_krb_request_info(dtgt_data);
	return;
    }

    /* copy reply data */
    srep = &(stgt_data->reply);
    if (srep->length != 0) {
    	drep = &((*dtgt_data)->reply);
	drep->data = malloc(srep->length);
	    if (drep->data == NULL) {
		SET_STATUS(stp, sec_login_s_no_memory);
		free_krb_request_info(dtgt_data);
		return;
	    }
	memcpy(drep->data, srep->data, srep->length);
	drep->length = srep->length;
    }

    /* ...and the rest */
    (*dtgt_data)->options   = stgt_data->options;
    (*dtgt_data)->enc_type  = stgt_data->enc_type;
    (*dtgt_data)->nonce	    = stgt_data->nonce;
}

/*
 * f r e e _ l o g i n _ c o n t e x t
 *
 * Free memory associated with a login context.
 * If the kerberos credential cache handle is not NULL, we take the
 * least destructive step of closing the cache file, and freeing the cache
 * handle.  If stronger measures are needed (as in a "purge"), the caller
 * takes whatever action is appropriate.  If such action invalidates the
 * cache handle, the caller should be careful to set the cache handle field
 * to NULL before calling this function.
 */
PUBLIC void  sec_login_pvt_free_login_context
  (
    sec_login_context_t   *lcp    /* [in, out] */
  )
{
    if (lcp != NULL) {
	assert (lcp->refcount == 0);
	lcp->magic = 0;
        free_identity(&lcp->identity);
        free_krb_info(&lcp->krb_info);
        if (lcp->context_name)
            free(lcp->context_name);
        if (lcp->namecache_name)
            free(lcp->namecache_name);
        if (lcp->namecache)
            sec__id_free_cache((sec_login_handle_t *)lcp);
	
	/* v1.1 info */
	sec_encode_v1_1_lc_info_free(free, &lcp->v1_1_info);

        /* destroy db mutex */
        (void) pthread_mutex_destroy(&lcp->db_mutex);

	if (lcp->exptoken)
	    free(lcp->exptoken);

	/*
	 * there are no outstanding references to this handle
	 */
	memset(lcp, 0, sizeof(sec_login_context_t));
	free( (char *) lcp);
    }
}


/*
 * s e c _ l o g i n _ p v t _ i s _ h o s t _ c e l l
 *
 * return true if the input realm is the host cell, otherwise false.
 */
PUBLIC  boolean32 sec_login_pvt_is_host_cell (
    unsigned_char_p_t  cell,
    error_status_t     *stp
)
{
    char        *host_cell;
    boolean32   retval = false;
    char        **aliases;

    dce_cf_get_cell_name(&host_cell, stp);
    if (GOOD_STATUS(stp)) {
        if (strcmp(host_cell, (char *) cell) == 0) {
            retval = true;
        }
        free(host_cell);
    }
    if (retval)
	return retval;

    dce_cf_get_cell_aliases(&aliases, stp);

    if (GOOD_STATUS(stp) && aliases) {
	char            **alias;
	error_status_t  xst;

	for (alias = aliases; *alias; alias++) {
	    if (! u_strcmp(*alias, cell)) {
		retval = true;
		break;
	    }
	}
	dce_cf_free_cell_aliases(aliases, &xst);
    }

    return retval;
}

/*
 * As of now, when setting the default context, we create a copy
 * of the credential cache.  In order to cleanup properly when a context
 * is purged, we need to maintain a list of the previous credential cache
 * files.  This routine maintains such a list.
 */
PRIVATE  void  update_prev_cache_list
  (
    sec_login_context_t  *lcp,
    char                 *cache_name,
    error_status_t       *stp
  )
{
    krb_info_t           *krb_info = &KRB_INFO(lcp);
    char                 **newlist, **freelist;

    CLEAR_STATUS(stp);

    newlist = (char **) malloc((krb_info->num_caches + 1) * sizeof(char *));
    if (newlist != NULL) {
        int i;

        /* copy the existing entries from the old list to the new */
        for (i=0; i<krb_info->num_caches; i++) {
            newlist[i] = malloc(strlen(krb_info->prev_caches[i]) + 1);
            if (newlist[i]== NULL) {
                SET_STATUS(stp, sec_login_s_no_memory);
                break;
            }
            strcpy(newlist[i], krb_info->prev_caches[i]);
        }

        /* and add the new entry */
        if (GOOD_STATUS(stp)) {
            newlist[i] = malloc(strlen(cache_name) + 1);
            if (newlist[i] != NULL) {
                strcpy(newlist[i], cache_name);
            } else {
                SET_STATUS(stp, sec_login_s_no_memory);
            }
        }

        if (BAD_STATUS(stp)) {
            /* free up new list storage */
            freelist = newlist;
        }  else {
            /* free up the old list storage */
            freelist = krb_info->prev_caches;

            krb_info->prev_caches = newlist;
            krb_info->num_caches += 1;
    }

        if (freelist != NULL) {
            for(i--; i > -1; i--) {
                free(freelist[i]);
            }
            free((char **) freelist);
        }

    } else {
    SET_STATUS(stp, sec_login_s_no_memory);
    }

}



/* Assume the caller has provided appropriate concurrency control */

/* w r i t e _ c o n t e x t
 *
 * Update the file in a temporary location - and then rename it
 * atomically to the target location.  This avoids the need to
 * lock the target filename and provide cross-process concurrency
 * controls.
 *
 * As of DCE 1.1 and beyond, all new login context data will be
 * stored using the DCE Backing Store Library routines in a new
 * file. See (src/security/idl/sec_login_encode.idl for more
 * information). The old file will be used to store DCE 1.0.x
 * information.
 */
PRIVATE  void  write_context
  (
    sec_login_context_t  *lcp,
    boolean32            update,
    error_status_t       *stp
  )
{
    char                 *filename;
    int                  dfd;
    int                  nbytes;
    Pointer              varying;
    lc_fixed_data_t      fixed;
    char                 *dfn;
    struct stat 	 stbuf;
    int 		 set_owner = 0;
    sec_login_db_entry_t *db_entry;

    CLEAR_STATUS(stp);
    if (stat (DATAFILE_NAME(dfn, KRB_INFO(lcp).cache), &stbuf) == 0)
	set_owner = 1;
    free(dfn);

    sec_login_pvt_ccache_genname(&filename, stp);
    if (GOOD_STATUS(stp)) {
        dfd = open(filename, O_TRUNC | O_WRONLY, 0);

        /*
         * if we're here, then we need to package up the context data for
         * persistent storage, and write the contents to disk
         */
        sec_login_db_import_context_data(lcp, &fixed, &varying, &db_entry, stp);
        if (BAD_STATUS(stp)) {
            close(dfd);
            unlink(filename);
            free(filename);
            return;
        }

        /* write fixed-length portion */
        nbytes = write(dfd, (char *) &fixed, sizeof(fixed));
        if (nbytes != sizeof(fixed)) {
	    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		"Write failed : Requested no of bytes (%d) not written. errno = %d\n",
                sizeof(fixed), errno));
            SET_STATUS(stp, sec_login_s_internal_error);
        }

        /* write variable length portion */
        if (GOOD_STATUS(stp)) {
            nbytes = write(dfd, varying, fixed.var_len);
            if (nbytes != fixed.var_len) {
	        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                    "Write failed : requested no bytes (%d) not written. errno = %d\n",
                    fixed.var_len, errno));
                SET_STATUS(stp, sec_login_s_internal_error);
            }
        }

        free(varying);
        close(dfd);

        /*
         * Store array of db_entry structs to new (1.1+) context data file
         */
        if (GOOD_STATUS(stp)) {
            sec_login_util_store_db_entries((sec_login_handle_t *) lcp,
		DB_DATAFILE_NAME(dfn, KRB_INFO(lcp).cache), 
	    	NUM_LC_DB_TYPES, db_entry, stp);
            if (dfn)
                free(dfn);
        }
        sec_login_db_free_entries(free, db_entry, NUM_LC_DB_TYPES);

        if (GOOD_STATUS(stp)) {
	    if (set_owner)
	    {
		(void) chown (filename, stbuf.st_uid, stbuf.st_gid);
	    }
	    rename(filename, DATAFILE_NAME(dfn, KRB_INFO(lcp).cache));
            free(dfn);
        }

        free(filename);
    }
}

/*
 * read the fixed and varying segments of login context
 * from disk, without interpretation.  Any NULL
 * varying input parameters parameters mean the
 * caller is not interested in that piece of the
 * data, and so no attempt is made to read it.
 * The caller must free the storage for any non-NULL
 * varying parameters.
 *
 * !!!BINARY COMPATIBILITY!!!
 *
 * As of DCE 1.1, any new persistent fields in the
 * login context are stored in a new file which uses
 * the DCE Backing Store Library routines to maintain
 * binary compatibility. Please see 
 * src/security/idl/sec_login_encode.idl for details.
 */
static void read_context_raw (
    sec_login_context_t  *lcp,
    lc_fixed_data_t      *fixed,
    Pointer              *varying, 
    sec_login_db_data_t  *db_data,
    error_status_t       *stp
)
{
    char                 *datafile;
    int                  dfd;
    int                  nbytes;

    CLEAR_STATUS(stp);
    if (varying) *varying = NULL;

    dfd = open(DATAFILE_NAME(datafile, KRB_INFO(lcp).cache), O_RDONLY, 0);
    free(datafile);
    if (dfd < 0) {
        if (errno == ENOENT) {
           SET_STATUS(stp, sec_login_s_no_current_context);
        } else {
            SET_STATUS(stp, errno);
        }
        return;
    }

    /* read fixed length portion of data */
    nbytes = read(dfd, (char *) fixed, sizeof(*fixed));
    if (nbytes != sizeof(*fixed)) {
        close(dfd);
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                       "Read failed : Requested bytes (%d) not read. errno = %d\n",
                       sizeof(*fixed), errno));
        SET_STATUS(stp, sec_login_s_internal_error);
        return;
    }

    if (varying) {
	*varying = malloc(fixed->var_len);
	if (*varying == NULL) {
	    close(dfd);
	    SET_STATUS(stp, sec_login_s_no_memory);
	    return;
	}

	/* which tells us how much variable length data to read */
	nbytes = read(dfd, *varying, fixed->var_len);
	if (nbytes != fixed->var_len) {
	    close(dfd);
	    free(*varying); *varying = NULL;
	    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
			   "Read failed : Requested bytes (%d) not read. errno = %d\n",
			   fixed->var_len, errno));
	    SET_STATUS(stp, sec_login_s_internal_error);
	    return;
	}
    } else {
	off_t  offset;

	/* caller doesn't want varying segment, so just seek past it */
	offset = lseek(dfd, fixed->var_len, SEEK_CUR);
	if (offset == -1) {
	    close(dfd);
	    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
			   "lseek to offset (%d) failed. errno = %d\n",
			   sizeof(*fixed) + fixed->var_len, errno));
	    SET_STATUS(stp, sec_login_s_internal_error);
	    return;
	}	    
    }

   /*
    * no need for an lseek past the varying2 segment since its
    * the last thing in the file that we could comprehend, hence
    * no else clause.
    */
    close(dfd);

    sec_login_util_fetch_db_entries((sec_login_handle_t *) lcp,
		DB_DATAFILE_NAME(datafile, KRB_INFO(lcp).cache),
        	NUM_LC_DB_TYPES, lc_db_types,
	        &db_data->db_entry, &db_data->num_fetched, 
       		&db_data->num_not_fetched, &db_data->not_fetched, stp); 
    if (datafile)
        free(datafile);
    if (BAD_STATUS(stp)) {
        return;
    }
}
    

/*
 * assume caller has supplied appropriate concurrency control.
 * input context must have been allocated, and contain a valid
 * kerberos cache handle.  This routine will fill in the rest
 * from persistent storage.
 */
PRIVATE  void  read_context
  (
    sec_login_context_t  *lcp,
    error_status_t       *stp
  )
{
    Pointer              varying;
    lc_fixed_data_t      fixed;
    sec_login_db_data_t  db_data;

    CLEAR_STATUS(stp);

    read_context_raw(lcp, &fixed, &varying, &db_data, stp);
    if (GOOD_STATUS(stp)) {
	sec_login_db_export_context_data(&fixed, varying, &db_data, lcp, stp);
	free(varying);
        sec_login_db_free_entries(rpc_ss_client_free, db_data.db_entry, NUM_LC_DB_TYPES);
        if (db_data.not_fetched)
            free(db_data.not_fetched);
    }
}


PRIVATE  boolean32  bind_to_rgy
  (
    unsigned_char_p_t site,            /* [in] */
    unsigned32         authn_svc,       /* [in] */
    unsigned32         authz_svc,       /* [in] */
    sec_login_handle_t auth_identity,   /* [in] */
    sec_rgy_handle_t   *rgy_context_p,  /* [out] */
    error_status_t     *stp,            /* [out] */
    error_status_t     *astp            /* [out] */
  )
{
    sec_rgy_bind_auth_info_t  auth_info;

    CLEAR_STATUS(stp);
    CLEAR_STATUS(astp);

    if (auth_identity ==  (void *) (-1))
    {
        *astp = 0;
    } else {

        /*
         * we've been handed an valid identity, so try using it, first with
         * with packet integrity, and, if that fails,  then with auth level none
         */

        auth_info.info_type = sec_rgy_bind_auth_dce;
        RGY_AUTH_INFO_DCE(auth_info).authn_level = rpc_c_authn_level_pkt_integrity;
        RGY_AUTH_INFO_DCE(auth_info).authn_svc = authn_svc;
        RGY_AUTH_INFO_DCE(auth_info).authz_svc = authz_svc;
        RGY_AUTH_INFO_DCE(auth_info).identity = auth_identity;

        rca_site_bind(site, &auth_info, 0 | rca_NSI_cell, secidmap_v1_0_c_ifspec,
                      SEC_RGY_SERVER_NAME, (char *)site, rgy_context_p,
                       stp);


        if (GOOD_STATUS(stp))
            return true;

        *astp = *stp;
    }

    /* try unauthenticated */
    auth_info.info_type = sec_rgy_bind_auth_none;
    rca_site_bind(site, &auth_info, 0 | rca_NSI_cell, secidmap_v1_0_c_ifspec,
                  SEC_RGY_SERVER_NAME, (char *) site, rgy_context_p, stp);

    return false;
}


PRIVATE  void allocate_projlist_buffers
  (
    signed32        num_groups,
    uuid_t          **id_projlist,
    signed32        **unix_projlist,
    error_status_t  *stp
  )
{

    CLEAR_STATUS(stp);
    *id_projlist = NULL;
    *unix_projlist = NULL;

    if (num_groups == 0) {
        return;
    }

    *id_projlist = (uuid_t *) malloc(num_groups * sizeof(uuid_t));
    if (*id_projlist == NULL) {
        MEM_FAILURE(stp);
        return;
    }

    *unix_projlist = (long *) malloc(num_groups * sizeof(signed32));
    if (*unix_projlist == NULL) {
        free ((char *) *id_projlist);
        *id_projlist = NULL;
        MEM_FAILURE(stp);
        return;
    }

}


PRIVATE  void  check_info_state
  (
    sec_login_context_t  *lcp,        /* [in, out] */
    boolean32            *get_info,   /* [out] */
    boolean32            *free_info,  /* [out] */
    error_status_t       *stp
  )
{
    CLEAR_STATUS(stp);
    *get_info = false;
    *free_info = false;

    switch (lcp->state) {

    case certified:
        switch (lcp->identity->state) {
        case info_certified:
            break;                                  /* do nothing */
        case info_auth:
            lcp->identity->state = info_certified;
            break;
        case info_unauth:
            *get_info = true;
            *free_info = true;
            break;
        case info_none:
            *get_info = true;
            break;
        }
        break;

    case valid:
        switch (lcp->identity->state) {
        case info_certified:
            SET_STATUS(stp, sec_login_s_context_invalid);   /* shouldn't ever happen */
            break;
        case info_auth:
            break;                                          /* do nothing */
        case info_unauth:
            *get_info = true;
            *free_info = true;
            break;
        case info_none:
            *get_info = true;
            break;
        }
        break;

    case setup:
    case semisetup:
        switch (lcp->identity->state) {
        case info_certified:
            /*FALLTHROUGH*/
        case info_auth:
            SET_STATUS(stp, sec_login_s_context_invalid);   /* shouldn't ever happen */
            break;
        case info_unauth:
            break;                                          /* do nothing */
        case info_none:
            *get_info = true;
            break;
        }
        break;

    case allocated:
        switch (lcp->identity->state) {
        case info_certified:
            /*FALLTHROUGH*/
        case info_auth:
            SET_STATUS(stp, sec_login_s_context_invalid);   /* shouldn't ever happen */
            break;
        case info_unauth:
            break;                                          /* do nothing */
        case info_none:
            *get_info = true;
            break;
        }
        break;

    default:
        SET_STATUS(stp, sec_login_s_context_invalid);
        break;
    }
}


/* s e t _ p o l i c y
 *
 * calculate effective account and passwd expiration dates from registry
 * lifespan and expiration data.  Store the relevant admin and user flags
 * too
 */
PRIVATE  void  set_policy
  (
    sec_login_context_t   *lcp,
    sec_rgy_acct_admin_t  *admin,
    sec_rgy_acct_user_t   *user,
    sec_rgy_plcy_t        *policy
  )
{
    struct timezone       tz;
    sec_timeval_sec_t     passwd_exp, acct_exp, tmp_exp;
    time_t now;

    (void) time(&now);

    acct_exp = effective_acct_expiration(admin, policy);
    passwd_exp = effective_passwd_expiration(user, policy);

    POLICY_INFO(lcp).pflags = 0;

    /*
     * If the password has an effective expiration time then either
     * the expiration time must be in the future, or the passwd
     * modification time must be later than the expiration time, or
     * the password is not valid.  If the pasword has been marked invalid
     * by an administrator, then  the password is invalid regardless
     * of the expiration date.
     */
    if (FLAG_SET(user->flags, sec_rgy_acct_user_passwd_valid)) {
        if (passwd_exp != 0) {
            if (passwd_exp > now || user->passwd_dtm > passwd_exp) {
                SET(POLICY_INFO(lcp).pflags, policy_passwd_valid);
            }
        } else {
            SET(POLICY_INFO(lcp).pflags, policy_passwd_valid);
        }
    }

    if (FLAG_SET(admin->flags, sec_rgy_acct_admin_valid)) {
        if (acct_exp != 0) {
            if (acct_exp > now ) {
                SET(POLICY_INFO(lcp).pflags, policy_acct_valid);
            }
        } else  {
            SET(POLICY_INFO(lcp).pflags, policy_acct_valid);
        }
    }

    if (FLAG_SET(admin->authentication_flags,sec_rgy_acct_auth_user_to_user)) {
        SET(POLICY_INFO(lcp).pflags, policy_user_to_user);
    }

    POLICY_INFO(lcp).passwd_exp = passwd_exp;
    POLICY_INFO(lcp).acct_exp = acct_exp;
}


PRIVATE  void  get_local_unix_info
  (
    unsigned_char_p_t  cell,
    unsigned_char_p_t  user,
    struct sec_passwd  **pwent,
    unsigned16         *num_groups,
    signed32           **groups,
    error_status_t     *stp
  )
{
   sec_rgy_name_t      name;
   char                sep_str[2];
   boolean32           local;
   unsigned16          ngrps;

    CLEAR_STATUS(stp);

    *pwent =  (struct sec_passwd *) malloc(sizeof(struct sec_passwd));
    if (*pwent != NULL) {
        alloc_pwent_string_buffers(*pwent, stp);
        if (BAD_STATUS(stp)) {
            free_passwd_context(pwent);
            return;
        }
    } else {
        SET_STATUS(stp, sec_login_s_no_memory);
        return;
    }

    *name = '\0';
    if (cell != NULL && *cell != '\0') {
        local = sec_login_pvt_is_host_cell(cell, stp);
        if (BAD_STATUS(stp)) {
            return;
        }
        if (!local) {
            unsigned_char_p_t   ucp;

            u_strcpy(name, cell);
            ucp = (name + u_strlen(name));
            *ucp++ = PATH_COMPONENT_SEPARATOR;
            *ucp = '\0';
        }
    }

    if (user != NULL && *user != '\0') {
        u_strcat(name, user);
    }

    /*
     * try to find the principal in the local registry.  For now, this is
     * just /etc/password.
     */
    ngrps = (*num_groups);
    sec_login_pvt_get_local_login_info((char *)name,
                                       (*pwent)->pw_passwd,
                                       &(*pwent)->pw_uid,
                                       &(*pwent)->pw_gid,
                                       (*pwent)->pw_gecos,
                                       (*pwent)->pw_dir,
                                       (*pwent)->pw_shell,
                                       &ngrps,
                                       groups,
                                       stp);
    *num_groups = (unsigned16)(ngrps & 0xffff);
    if (BAD_STATUS(stp)) {
        return;
    }

    strcpy((*pwent)->pw_name, (char *)name);
    /* not supported by bsd4.3 passwd file */
    *((*pwent)->pw_class) = '\0';
    (*pwent)->pw_expire = 0;
    (*pwent)->pw_change = 0;
}


PRIVATE void sec_login_pvt_lrgytgt_to_lc
  (
    sec_lrgy_tgt_entry_t    *ltgtp,
    sec_login_context_t     *lcp
  )
{
    KRB_REQUEST_INFO(lcp)->options      = ltgtp->options;
    KRB_REQUEST_INFO(lcp)->enc_type     = ltgtp->enc_type;
    KRB_REQUEST_INFO(lcp)->nonce        = ltgtp->nonce;
    KRB_REQUEST_INFO(lcp)->reply.length = ltgtp->reply_len;
    KRB_REQUEST_INFO(lcp)->reply.data   = ltgtp->reply;
}


/*
 * s e c _ l o g i n _ p v t _ l r g y _ t o _ l c
 */
PRIVATE void sec_login_pvt_lrgy_to_lc
  (
    boolean32           override,
    sec_lrgy_entry_t    *entryp,
    sec_login_context_t *lcp,
    error_status_t      *stp
  )
{
    sec_rgy_unix_passwd_buf_t   pw_passwd;
    sec_rgy_name_t              pw_gecos;
    sec_rgy_name_t              pw_dir;
    sec_rgy_name_t              pw_shell;
    sec_override_fields_t       overridden;

    overridden = sec_override_none;
    CLEAR_STATUS(stp);
    if (override) {
        override_get_login_info((unsigned_char_p_t) entryp->principal,
                                &entryp->pw_uid, &entryp->pw_gid, pw_passwd,
                                pw_gecos, pw_dir, pw_shell, &overridden, stp);
            /* If no override warning, proceed with original data */
        if (STATUS_EQUAL(stp, sec_login_s_ovrd_ent_not_found)||
            STATUS_EQUAL(stp, sec_login_s_no_override_info) ) {
            CLEAR_STATUS(stp);

            /* Any other bad status means we can't tell if the account
             * should be overridden so it must fail.
             */
        } else if (BAD_STATUS(stp)) {
            return;
        }
    }

    u_strcpy(lcp->cell, entryp->cell);
    free(entryp->cell);
    u_strcpy(lcp->principal, entryp->principal);
    free(entryp->principal);

    UNIX_CREDS(lcp).num_groups  = entryp->num_unix_groups;
    UNIX_CREDS(lcp).groups      = (signed32 *) entryp->groups;
    UNIX_CREDS(lcp).os_pwent    = NULL;

    POLICY_INFO(lcp).acct_exp       = entryp->acct_exp;
    POLICY_INFO(lcp).passwd_exp     = entryp->passwd_exp;
    POLICY_INFO(lcp).identity_exp   = entryp->identity_exp;
    POLICY_INFO(lcp).pflags         =
        entryp->passwd_valid ? (policy_passwd_valid & policy_acct_valid) :
                               (policy_acct_valid);

    UNIX_CREDS(lcp).pwent =
	(struct sec_passwd *) malloc(sizeof(struct sec_passwd));
    if (UNIX_CREDS(lcp).pwent != NULL) {
        alloc_pwent_string_buffers(UNIX_CREDS(lcp).pwent, stp);

        u_strcpy(UNIX_CREDS(lcp).pwent->pw_name,   entryp->pw_name);
        if (overridden && FLAG_SET(overridden, sec_override_pw_passwd)) {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_passwd, pw_passwd);
        } else {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_passwd, entryp->pw_passwd);
        }
        u_strcpy(UNIX_CREDS(lcp).pwent->pw_class,  entryp->pw_class);
        if (overridden && FLAG_SET(overridden, sec_override_pw_gecos)) {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_gecos, pw_gecos);
        } else {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_gecos,  entryp->pw_gecos);
        }
        if (overridden && FLAG_SET(overridden, sec_override_pw_dir)) {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_dir, pw_dir);
        } else {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_dir,    entryp->pw_dir);
        }
        if (overridden && FLAG_SET(overridden, sec_override_pw_shell)) {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_shell, pw_shell);
        } else {
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_shell,  entryp->pw_shell);
        }
        UNIX_CREDS(lcp).pwent->pw_uid       = entryp->pw_uid;
        UNIX_CREDS(lcp).pwent->pw_gid       = entryp->pw_gid;
        UNIX_CREDS(lcp).pwent->pw_change    = entryp->pw_change;
        UNIX_CREDS(lcp).pwent->pw_expire    = entryp->pw_expire;
    } else {
        SET_STATUS(stp, sec_login_s_no_memory);
    }

    if (GOOD_STATUS(stp)) {
        sec_id_pac_t_unpickle((idl_pkl_t *) entryp->ppac,
                                (void *(*)(unsigned32)) malloc,
                                &NET_CREDS(lcp), stp);
    }

    /* Convert V1 PAC to EPAC CHAIN */
    if (GOOD_STATUS(stp)) {
	sec_attr_t	login_attr;
        login_attr.attr_id = login_set_id;
        login_attr.attr_value.attr_encoding = sec_attr_enc_void;
	pac_to_epac_chain_bytes(&NET_CREDS(lcp),
	    &EPAC_CHAIN(lcp), 1, &login_attr,
	    sec_id_deleg_type_none,
	    sec_id_compat_mode_initiator, stp);
    }

    free(entryp->ppac);
    free(entryp->pw_name);
    free(entryp->pw_passwd);
    free(entryp->pw_class);
    free(entryp->pw_gecos);
    free(entryp->pw_dir);
    free(entryp->pw_shell);
}


/*
 * PUBLIC (to sec_login module code) routines
 */


PUBLIC void sec_login_pvt_lrgy_store
  (
    sec_login_context_t *lcp,   /* [in]  */
    error_status_t      *stp    /* [out] */
  )
{
    sec_lrgy_entry_t        entry;
    sec_lrgy_tgt_entry_t    tgt_entry;
    idl_pkl_t               *ppac;
    unsigned32              ppac_len;

    entry.pw_uid            = UNIX_CREDS(lcp).pwent->pw_uid;
    entry.pw_gid            = UNIX_CREDS(lcp).pwent->pw_gid;
    entry.pw_change         = UNIX_CREDS(lcp).pwent->pw_change;
    entry.pw_expire         = UNIX_CREDS(lcp).pwent->pw_expire;
    entry.num_unix_groups   = UNIX_CREDS(lcp).num_groups;

    entry.acct_exp      = POLICY_INFO(lcp).acct_exp;
    entry.passwd_exp    = POLICY_INFO(lcp).passwd_exp;
    entry.identity_exp  = POLICY_INFO(lcp).identity_exp;
    entry.passwd_valid  = (POLICY_INFO(lcp).pflags & policy_passwd_valid) ? true: false;


    /* pickle the context pac */
    sec_id_pac_t_pickle(&NET_CREDS(lcp), NULL, (void *(*)(unsigned32)) malloc,
                            (unsigned32) 8, &ppac, &ppac_len, stp);
    if (!GOOD_STATUS(stp)) {
        return;
    }

    entry.pw_name       = UNIX_CREDS(lcp).pwent->pw_name;
    entry.pw_passwd     = UNIX_CREDS(lcp).pwent->pw_passwd;
    entry.pw_class      = UNIX_CREDS(lcp).pwent->pw_class;
    entry.pw_gecos      = UNIX_CREDS(lcp).pwent->pw_gecos;
    entry.pw_dir        = UNIX_CREDS(lcp).pwent->pw_dir;
    entry.pw_shell      = UNIX_CREDS(lcp).pwent->pw_shell;
    entry.groups        = (char *)UNIX_CREDS(lcp).groups;
    entry.ppac          = (char *)ppac;
    entry.ppac_len      = ppac_len;
    tgt_entry.cell      = entry.cell = (char *)lcp->cell;
    tgt_entry.principal = entry.principal = (char *)lcp->principal;
    tgt_entry.options   = KRB_REQUEST_INFO(lcp)->options;
    tgt_entry.enc_type  = KRB_REQUEST_INFO(lcp)->enc_type;
    tgt_entry.nonce     = KRB_REQUEST_INFO(lcp)->nonce;
    tgt_entry.reply_len = KRB_REQUEST_INFO(lcp)->reply.length;
    tgt_entry.reply   = (char *)(KRB_REQUEST_INFO(lcp)->reply.data);

    sec_lrgy_entry_store(&entry, stp);
    if (GOOD_STATUS(stp)) {
        sec_lrgy_tgt_store(&tgt_entry, stp);
    }
    if (ppac) {
        free(ppac);
    }
}


/*  s e c _ l o g i n _ p v t _ c h e c k _ h a n d l e
 *
 * Test  an instance of a sec_login_handle_t to see it has been allocated, setup,
 * or validated, as indicated by the state parameter.
 *
 * Because a handle is just an address, we can't provide much in the
 * way of robustness; if the caller gives us garbage, we'll just
 * dereference it and die.
 */
PUBLIC  sec_login_context_t *  sec_login_pvt_check_handle
  (
    sec_login_handle_t   handle,   /* [in]  */
    context_state_t      state,    /* [in]  */
    error_status_t       *stp      /* [out] */
  )
{
    sec_login_context_t *lcp = (sec_login_context_t *) handle;

    CLEAR_STATUS(stp);

    /* reject default handle outright */
    if (DEFAULT_HANDLE(handle)) {
       SET_STATUS(stp, sec_login_s_default_use);
       return lcp;
    }

    if (lcp->magic != lcp) {
	SET_STATUS(stp, sec_login_s_context_invalid);
	return lcp;
    }

    /*
     * if the caller is certain the context is valid just return a pointer
     * to the internal context data structure
     */
     if (state == none) {
         return lcp;
     }

    /*
     * make use of the fact that C enums have ordinal values to
     * speed up the check
     */
     if (state > lcp->state) {
         SET_STATUS(stp, sec_login_s_context_invalid);
     }

    return ( GOOD_STATUS(stp) ? lcp : NULL );
}


/* s e c _ l o g i n _ p v t _ h a n d l e _ e q u a l
 *
 * return true if the two handles are the same (meaning that they really are
 * the same handle, not distinct handles representing the same credentials)
 */
PUBLIC  boolean32  sec_login_pvt_handle_equal ( handle1, handle2 )
    sec_login_handle_t  handle1;
    sec_login_handle_t  handle2;
{
    return (handle1 == handle2);
}


/*
 *  s e c _ l o g i n _ p v t _ a l l o c a t e _ l o g i n _ c o n t e x t
 *
 * Malloc space for a login context structure and initialize fields
 * appropriately.
 *
 * If the cache_name parameter is NULL, and the user parameter is not
 * NULL, then the principal and cell info are derived from the user parameter
 * and a new credential cache is created and initialized with the krb5_principal
 * name derived from the user parameter.  The result is an "allocated" context.
 *
 * If the cache_name parameter is non-NULL, then a credential cache
 * handle is allocated for the existing "cache_name", and the context
 * credentials are filled in with PTGT info from the cache.   In this case
 * the result is a "validated" context (but there's no guarantee that the
 * credentials it references haven't expired).
 */
PUBLIC void sec_login_pvt_allocate_login_context
  (
    boolean32           override,        /* [in] */
    unsigned_char_p_t   user,            /* [in] */
    char                *cache_name,     /* [in] */
    sec_login_context_t **lcap,          /* [out] */
    error_status_t      *stp
  )
{
    krb5_principal       princ = NULL;
    char                 *pp, *cp, *dfn;
    sec_login_context_t  *lcp;

    /* allocate the space for the login context struct */
    *lcap = NULL;
    lcp = (sec_login_context_t *)malloc(sizeof(sec_login_context_t));
    if (lcp == NULL) {      /* can't allocate any space so bail out */
        MEM_FAILURE(stp);
        return;
    } else {
        CLEAR_STATUS(stp);
    }
    memset(lcp, 0, sizeof(sec_login_context_t));
    lcp->refcount = 0;
    lcp->mark = 0;
    lcp->cache_link_next = NULL;
    lcp->cache_link_prev = NULL;
    lcp->namecache = 0;
    lcp->namecache_name = 0;
    sec_login_pvt_init_v1_1_info(NULL, NULL, &lcp->v1_1_info);

    allocate_identity(&lcp->identity, stp);
    if (BAD_STATUS(stp)) {
        free( (char *) lcp);
        return;
    }
    UNIX_CREDS(lcp).os_pwent = NULL;

    /*
     * will create a new cache file if cache name is NULL, otherwise will
     * create a handle on the existing file
     */
    allocate_krb_info(&lcp->krb_info, cache_name, stp);
    if (BAD_STATUS(stp))
        goto errout;
    if (cache_name) {
        int len = strlen(cache_name)+1;
        char *name_copy = malloc (len);
	struct stat stb;
        memcpy(name_copy, cache_name, len);
        lcp->context_name = name_copy;
        lcp->context_name_len = len;
        /*
         * Find out who owns the credential cache file and set the
         * owner_uid to it.
         */
        if (stat(krb5_cc_get_name(KRB_INFO(lcp).cache), &stb) == -1) {
            SET_STATUS(stp, sec_login_s_no_current_context);
	    goto errout;
        }
	lcp->owner_uid = stb.st_uid;
    } else {
        /*
         * !!! should extract this from the context, and update it on
         * the fly as the context gets "set".  We can fix this later.
         */
        lcp->context_name_len = -1;
        lcp->context_name = NULL;
        /*
         * Since this is a brand new cache file, set the credential
         * cache owner to the effective uid.
         */
        lcp->owner_uid = geteuid();
    }

    /* Initialize db_mutex */
    init_db_mutex(&lcp->db_mutex, stp);
    if (BAD_STATUS(stp)) {
	goto errout;
    }

    if (user != NULL) {
        /*
	 * break the user up into principal and cell components and store
	 * the result in the principal and realm components of the context
	 * credentials.
	 *
	 * [1995/08/22  12:05:11  sommerfeld]
	 * We don't ask for a uuid (we used to), since we'll find out
	 * soon enough if the principal doesn't really exist, and
	 * asking for a uuid *guarantees* that we'll need to go remote
	 * if our cache is cold.
	 */
        sec_rgy_bind_auth_info_t  auth_binding;
        auth_binding.info_type = sec_rgy_bind_auth_none;

	sec_id_global_parse_name(&auth_binding, user, lcp->cell, NULL,
			  lcp->principal, NULL, stp);

	if (GOOD_STATUS(stp)) {
	    if (lcp->principal[0] == '\0') {
		/*
		 * caller supplied a global cell name (/.../cellname),
		 * which yields no principal component from parse_name.
		 * In the unlikely event that the caller really wants
		 * to log in as a cell principal, they should do so as
		 * /.../cellname/krbtgt/cellname
		 */
		SET_STATUS(stp, sec_rgy_incomplete_login_name);
            }
        }

        if (BAD_STATUS(stp)) {

            /*LOCAL_REGISTRY
             * Try to find the user in the local registry.  If they're
             * there, seed the context with locally obtained info
             * and set lcp->auth_src = sec_login_auth_src_local.
             */
            error_status_t      lst;

	    sec_login_pvt_get_lrgy_login_info(override, lcp, &lst);
            if (BAD_STATUS(&lst))
                goto errout;
            else
                CLEAR_STATUS(stp);
	}

        if (GOOD_STATUS(stp)) {
            sec_krb_princ_from_dce_pair((char *) lcp->cell,
                                        (char *) lcp->principal,
                                        &princ, stp);

            lcp->local = sec_login_pvt_is_host_cell(lcp->cell, stp);

            /* initialize the cache */
            *stp = krb5_cc_initialize(lcp->krb_info.cache, princ);
            krb5_free_principal(princ);

            if (BAD_STATUS(stp))
                goto errout;
            lcp->state = allocated;
            if (lcp->auth_src != sec_login_auth_src_local)
                lcp->auth_src = sec_login_auth_src_network;

	    /* get a "generation" id */
	    uuid_create(&lcp->generation_id, stp);
	    if (BAD_STATUS(stp)) {
		goto errout;
	    }
        }
    } else if (cache_name != NULL) {

        /*
	 * we got a cache name, but no user name, so construct the context
	 * from persistent storage.  first we need to allocate basic unix
	 * info structures
	 */
        UNIX_CREDS(lcp).pwent =
	    (struct sec_passwd *) malloc(sizeof(struct sec_passwd));
        if (UNIX_CREDS(lcp).pwent == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
        }

        if (GOOD_STATUS(stp)) {
            alloc_pwent_string_buffers(UNIX_CREDS(lcp).pwent, stp);
        }

        if (GOOD_STATUS(stp)) {
            read_context(lcp, stp);
        }
    }

    if (BAD_STATUS(stp))
        goto errout;

    lookup_and_cache_enter(lcp);
    *lcap = lcp;
    return;

 errout:
    /* We know there's no one using the empty ccache at this point, so destroy
     * it (ie: deletes the file too) rather than just free-ing it later
     */
    if ((cache_name == NULL) && (lcp->krb_info.cache != NULL)) {
        krb5_cc_destroy(lcp->krb_info.cache);
        lcp->krb_info.cache = (krb5_ccache)NULL;
    }
    sec_login_pvt_free_login_context(lcp);
    *lcap = NULL;
    return;
}


/*
 *  s e c _ l o g i n _ p v t _ a l l o c a t e _ f i r s t
 */
PUBLIC void sec_login_pvt_allocate_first
  (
    char                *cache_name,     /* [in] */
    sec_login_context_t **lcap,          /* [out] */
    error_status_t      *stp
  )
{
    krb5_principal       princ = NULL;
    char                 *host_name = NULL, *pp = NULL, *cp = NULL;
    sec_login_context_t *lcp;

    /* allocate the space for the login context struct */
    lcp = (sec_login_context_t *)malloc(sizeof(sec_login_context_t));
    if (lcp == NULL) {      /* can't allocate any space so bail out */
        MEM_FAILURE(stp);
        return;
    } else {
        CLEAR_STATUS(stp);
    }
    memset(lcp, 0, sizeof(sec_login_context_t));
    lcp->refcount = 0;
    lcp->mark = 0;
    lcp->cache_link_next = NULL;
    lcp->cache_link_prev = NULL;
    lcp->namecache = NULL;
    lcp->namecache_name = NULL;
    sec_login_pvt_init_v1_1_info(NULL, NULL, &lcp->v1_1_info);
    /*
     * Initialize pag to SEC_LOGIN_NO_PAG. This is a useful
     * indicator that this login context belongs to the machine
     * principal. The namecache code uses this to determine if the
     * namecache file should be owned by root.
     */
    lcp->v1_1_info.pag = SEC_LOGIN_NO_PAG;

    allocate_identity(&lcp->identity, stp);
    if (BAD_STATUS(stp)) {
        free( (char *) lcp);
        return;
    }

    allocate_krb_info(&lcp->krb_info, cache_name, stp);
    if (BAD_STATUS(stp))
        goto errout;

    /* Initialize db_mutex */
    init_db_mutex(&lcp->db_mutex, stp);
    if (BAD_STATUS(stp)) {
	goto errout;
    }

    /* get a "generation" id */
    uuid_create(&lcp->generation_id, stp);
    if (BAD_STATUS(stp)) {
	goto errout;
    }

    /*
     * construct principal info from host configuration info
     */
    dce_cf_prin_name_from_host(NULL, &host_name, stp);

    if (GOOD_STATUS(stp)) {
        if (strncmp(CELL_DIR_ROOT, host_name, CELL_DIR_ROOT_LEN) == 0) {
            pp = host_name + CELL_DIR_ROOT_LEN;
        } else {
            pp = host_name;
        }
        dce_cf_get_cell_name(&cp, stp);
    }

    if (GOOD_STATUS(stp)) {
        strcpy( (char *) lcp->principal, pp);
        strcpy( (char *) lcp->cell, cp);
        sec_krb_princ_from_dce_pair((char *) lcp->cell,
                                    (char *) lcp->principal,
                                    &princ, stp);
        if (GOOD_STATUS(stp)) {
            /* initialize the cache */
            *stp = krb5_cc_initialize(lcp->krb_info.cache, princ);
            krb5_free_principal(princ);
        }
    }

    if (cp != NULL) {
        free(cp);
    }

    if (host_name != NULL) {
        free(host_name);
    }

    if (BAD_STATUS(stp))
        goto errout;

    lcp->local = true;
    lcp->state = allocated;
    lcp->auth_src = sec_login_auth_src_network; /* must be */

    lookup_and_cache_enter(lcp);
    *lcap = lcp;
    return;
 errout:
    sec_login_pvt_free_login_context(lcp);
    *lcap = NULL;
}


/* s e c _ l o g i n _ p v t _ u n s e t _ d e f a u l t _ c o n t e x t
 * blow away references to the current default context
 */

PUBLIC void sec_login_pvt_unset_default_context ( void ) 
{
    LOCK_DEFAULT_CONTEXT {
	/* !!! drop reference to hold handle? */
	sec_login_pvt_default_context.handle = NULL;
	sec_login_pvt_default_context.valid = 0;
	unsetenv("KRB5CCNAME");	/* !!!??? */
	resetpag();
    } UNLOCK_DEFAULT_CONTEXT; 
}



/* s e c _ l o g i n _ p v t _ s e t _  c o n t e x t
 *
 * Save the specified login context in a credential cache.
 * If asked, then also make it the default context.
 */
PUBLIC  void  sec_login_pvt_set_context
  (
    sec_login_handle_t  handle,
    boolean32           set_default,
    error_status_t      *stp
  )
{
    sec_login_context_t  *lcp;
    char                 pagenv[SEC_LOGIN_PAG_NAME_LENGTH];
    char                 env[SEC_LOGIN_PAG_NAME_LENGTH+SEC_LOGIN_ENV_NAME_LENGTH];
    unsigned32           pag;
    krb5_ccache          old_cache = NULL, new_cache = NULL;
    char                 *prev_cfn, *new_cfn, *prev_dfn = NULL, *new_dfn = NULL, 
                         *prev_dbfn = NULL, *new_dbfn = NULL;
    boolean32            first_default_use = false, gen_pag;
    char                 *ncfn;
    int                  ncfd;

    lcp = sec_login_pvt_check_handle(handle, valid, stp);

    if (GOOD_STATUS(stp)) {
        prev_cfn = krb5_cc_get_name(KRB_INFO(lcp).cache);
        sprintf(env, "%s=", SEC_LOGIN_CCACHE_ENV);

	pag = -42;		/* obviously wrong value.. */

        if (set_default) {
            switch (sec_login_pvt_new_pag(lcp, &pag)) {
            case 0:
                gen_pag = false;
                sprintf(pagenv, SEC_LOGIN_CCACHE_FROM_PAG_FMT, pag);
                break;
            case -1:
                gen_pag = true;
                break;
            default:
                DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                               "sec_login_pvt_new_pag() failed \n"));
                SET_STATUS(stp, sec_login_s_internal_error);
                return;
	    }
	} else {
	    gen_pag = true;
	}

        /*
         * create links to the previous cache and datafile names
         * using the new cache and datafile names
         */
        sec_login_pvt_link_ccache(pag, gen_pag, pagenv, prev_cfn, stp);

        /*
         * construct a new cache handle that references the
         * new cache file name
         */
        if (GOOD_STATUS(stp)) {
            *stp = krb5_cc_resolve(pagenv, &new_cache);
        }

	/*
	 * change ownership of cache file if it is being created
	 * via proxy (default ownership is by euid).
	 * Also, must change the login context owner uid whenever
	 * we chown the krb5 cache file.
	 */		
	if (GOOD_STATUS(stp)) {
	    if ((FLAG_SET(lcp->flags, sec_login_proxy_cred)) &&
	        (UNIX_CREDS(lcp).pwent != NULL) &&
		(geteuid() == 0)) {
		    if (chown(prev_cfn, UNIX_CREDS(lcp).pwent->pw_uid, UNIX_CREDS(lcp).pwent->pw_gid) == -1)
			*stp = errno;
		    lcp->owner_uid = UNIX_CREDS(lcp).pwent->pw_uid;
	    }
            /*
             * save pag in login context.
	     */
	    lcp->v1_1_info.pag = pag;
	}
    }

    if (BAD_STATUS(stp)) {
        return;
    }

    new_cfn = krb5_cc_get_name(new_cache);

    if (GOOD_STATUS(stp)) {
        /*
         * if there are no names on the previous cache list
         * then this is the first set_context with this handle,
         * and we create the first datafile with the new name
         * (below), otherwise we need to link back to the
         * old data file
         */
        if (KRB_INFO(lcp).num_caches != 0) {
            if (link(DATAFILE_NAME(prev_dfn, KRB_INFO(lcp).cache),
                    DATAFILE_NAME(new_dfn, new_cache)) == (-1)) {
               *stp = errno;
            }
        } else {
	    error_status_t lst;
            first_default_use = true;
            /*
             *  create the namecache file here to guarantee that the
             *  name cache file will be owned by the process id of the
             *  user (possibly non-root) logging in. If this is not
             *  created here, then this may be created by cdsclerk
             *  and owned by root, thus preventing a user (who is non root)
             *  from deleting the name cache file ( via kdestroy).
             */
            lcp->namecache_name = (NAMECACHE_NAME(ncfn, new_cache)) ;
	    sec_login_pvt_open_namecache(lcp, lcp->namecache_name, (O_CREAT | O_TRUNC),
					 &ncfd, &lst);
	    /* if status is bad, continue on error */
            if (GOOD_STATUS(&lst)) {
               (void) close(ncfd);
            }
        }
    }

    /*
     * Link new (1.1+) data cache file. It is possible that
     * the old one does not exist, because we may have inherited
     * this login context from a 1.0.x library. This is ok,
     * because the code in sec_login_db.c is written to handle
     * nonexistant filenames and initialize the login context
     * appropriately.
     *
     * NOTE: you may think we should only link this file if
     * KRB_INFO(lcp).num_caches != 0. However, the .db file
     * contains more than login context data. It also can contain
     * EPACs. We need to ensure that this information is carried
     * forward.       
     */
    if (GOOD_STATUS(stp) &&
        link(DB_DATAFILE_NAME(prev_dbfn, KRB_INFO(lcp).cache),
             DB_DATAFILE_NAME(new_dbfn, new_cache)) == (-1)) {
        if (errno != ENOENT)
            *stp = errno;
    }

    if (GOOD_STATUS(stp)) {
        /* update the list of previous cache files */
        update_prev_cache_list(lcp, prev_cfn, stp);
    }

    if (GOOD_STATUS(stp)) {
        /*
         * assign the new cache handle. Do this before (possibly)
         * calling write_context to ensure that the data file is created
         * with the name derived from the new cache file name.  Save the
         * old cache handle so we can flush it or restore it later, depending
         * on whether or not everything turns out ok.
         */
        old_cache = KRB_INFO(lcp).cache;
        KRB_INFO(lcp).cache = new_cache;

        if (first_default_use) {
            /* create the data file */
            write_context(lcp, false, stp);
            /*
             * datafile should be readable/writeable only by root and user.
             */
            if (GOOD_STATUS(stp)) {
                if (INFO_STATE(lcp) == info_certified
		&& UNIX_CREDS(lcp).pwent != NULL
		&& geteuid() == 0) {
		    if (chown(DATAFILE_NAME(new_dfn, new_cache),
                                 UNIX_CREDS(lcp).pwent->pw_uid, -1) == -1)
			*stp = errno;
                    if (GOOD_STATUS(stp) &&
                        chown(DB_DATAFILE_NAME(new_dbfn, new_cache),
                                 UNIX_CREDS(lcp).pwent->pw_uid, -1) == -1)
                        *stp = errno;
                }
            }
        } else {
            /* update the existing datafile */
            write_context(lcp, true, stp);
        }
    }

    if (GOOD_STATUS(stp)) {
         /* we don't need the old cache handle anymore */
        (void) krb5_cc_close(old_cache);

	if (set_default) {
	    LOCK_DEFAULT_CONTEXT {
	        unsigned32 expiration;
		/* !!! drop reference to old handle? */
		sec_login_pvt_default_context.handle = handle;
		/* !!! bump refcount of new handle? */
		/*
		 * Set the tkt cache environment variable so children of this
		 * process will inherit auth context
		 */
		setenv(env, pagenv, 1);
		sec_login_pvt_default_context.valid = true;
 		expiration = POLICY_INFO(lcp).identity_exp;
 		sec_login_pag_new_tgt (0, expiration);

	    } UNLOCK_DEFAULT_CONTEXT;
	}
    } else {
        /*
         * undo as much of what we've done as is necessary
         */
        pag = 0;
        if (new_cache != NULL) {
            if (new_cfn != NULL) {
                (void) unlink(new_cfn);
            }
            (void) krb5_cc_close(new_cache);
        }

        if (old_cache != NULL) {
            KRB_INFO(lcp).cache = old_cache;
        }

        if (new_dfn != NULL) {
            (void) unlink(new_dfn);
        }
        if (new_dbfn != NULL) {
            (void) unlink(new_dbfn);
        }
    }

    if (prev_dfn != NULL) {
        free(prev_dfn);
    }
    if (new_dfn != NULL) {
        free(new_dfn);
    }
    if (prev_dbfn != NULL) {
        free(prev_dbfn);
    }
    if (new_dbfn != NULL) {
        free(new_dbfn);
    }
}


/* s e c _ l o g i n _ p v t _ s e t _ d e f a u l t _ c o n t e x t
 *
 * Associate the default context with a validated login context supplied by
 * the caller.   A handle on the default context data can be retrieved with
 * sec_login_pvt_get_default_context
 */
PUBLIC  void  sec_login_pvt_set_default_context
  (
    sec_login_handle_t  handle,
    error_status_t      *stp
  )
{
    sec_login_pvt_set_context(handle, true, stp);
}


/* s e c _ l o g i n _ p v t _ g e t _ d e f a u l t _ c o n t e x t
 *
 * Retrieve a handle on the current login context (must have been established
 * via sec_login_pvt_set_default_context).
 */
PUBLIC  sec_login_handle_t  sec_login_pvt_get_default_context
  (
    error_status_t      *stp
  )
{
    sec_login_handle_t  handle = NULL;
    char                pagenv[SEC_LOGIN_PAG_NAME_LENGTH];
    uid_t               euid;
    sec_login_context_t **lcap = (sec_login_context_t **) &handle;
    char                *env_context;


    CLEAR_STATUS(stp);

    /*
     * Check to see if the handle is already valid, and if so obtain
     * it without bothering to lock access.
     * Rationale:
     *  The default context is invalid IFF it has not yet been initialized
     *  or it has been destroyed.
     *  How can we be misled if we don't check the flag under a lock?
     *      1) The context may be in the process of being created.  If this is
     *         true, then the context valid flag will only be set to true if
     *         in fact the entire context has been created - the flag is NOW
     *         (in this function and in sec_login_pvt_set_context) only set
     *         to true as the final operation just before the lock is released
     *         So in fact the context is valid, and there is no error.
     *
     *      2) The context may be valid but about to be destroyed.  In this
     *         case the purge_default_context function is holding the lock,
     *         but has yet to clear the flag.  This case is equivalent to
     *         taking the lock and setting the handle parameter, then
     *         releasing the lock and having the purge thread scheduled to
     *         purge the context before the handle is actually returned to
     *         the caller (or in fact for the purge to occur at any time
     *         after the caller gets the handle).  So there is no advantage
     *         in this case to operating under the lock.
     *
     *  We conclude, therefore, that it is an acceptable optimization to
     *  get return default context if the context is marked valid and we have
     *  checked this state without locking the mutex.
     */
    if (sec_login_pvt_default_context.valid) {

            handle = sec_login_pvt_default_context.handle;

    } else LOCK_DEFAULT_CONTEXT {

        if (sec_login_pvt_default_context.valid) {

            /* use the stored handle */
            handle = sec_login_pvt_default_context.handle;

        } else {
            /* see if we can construct one from the process environment */
            euid = geteuid();
            if (euid == 0) {
                sprintf(pagenv, SEC_LOGIN_CCACHE_FROM_PAG_FMT,
                        SEC_LOGIN_NO_PAG);
            }

            /* get the cache name from the environment */
            env_context = (char *)getenv(SEC_LOGIN_CCACHE_ENV);

            if (env_context == NULL) {
               /*
                * if there is no default context, and the process is
                * privileged, then try using the host machine context
                */
                if (euid == 0) {
                    env_context = pagenv;
                } else {
                    SET_STATUS(stp, sec_login_s_no_current_context);
                }
            }

             /* Perhaps this context is still in the cache, maybe
              * because it was once the default context.
              */
	    if (GOOD_STATUS(stp)) { 
		 char *cname;
		 int cname_len;
		 uuid_t  generation_id;

		 sec_login_pvt_parse_export_token((ndr_char *)env_context, 
						  strlen(env_context)+1,
						  &cname, &cname_len, 
						  &generation_id, stp);
		 if (GOOD_STATUS(stp)) {
		     *lcap = sec_login_cache_lookup(cname_len, cname, 
						     &generation_id);
		 }
	     }

/*            if (GOOD_STATUS(stp) && (*lcap == NULL || (*lcap)->krb_info.cache == NULL)) { */
	    if (GOOD_STATUS(stp) && (*lcap == NULL  || env_context == NULL)) {
                struct stat  stat_buf;
                char         *cfn;

                /* strip the cache type specifier prefix from the
                 * kerberos ccache environment variable to get the filename of the
                 * file-based credential cache (the follwing stat call will
                 * fail horribly if the environment variable is not properly
                 * formatted)
                 */
                cfn = strchr(env_context, ':');
                cfn++;
                /*
                 * stat the credential file.  If it's empty, then don't bother trying
                 * to construct anything.  This protects against attempts to
                 * to inherit the default context established by sec_login_init_first
                 * before it's been validated.
                 */
                if (stat(cfn, &stat_buf) == -1) {
                    SET_STATUS(stp, sec_login_s_no_current_context);
                } else if (stat_buf.st_size == 0) {
                    SET_STATUS(stp, sec_login_s_no_current_context);
                }

                if (GOOD_STATUS(stp)) {
                    /*
                     * allocate a new login context using the ticket cache
                     * name obtained from the process environment
                     */
                    sec_login_pvt_allocate_login_context(false,
                                                         (unsigned_char_p_t) NULL,
                                                         env_context,
                                                         lcap,
                                                         stp);

                    if (BAD_STATUS(stp)) {
                        SET_STATUS(stp, sec_login_s_no_current_context);
                    }
                }
            }

            if (BAD_STATUS(stp)) {
                handle = NULL;
            } else {
                sec_login_pvt_default_context.handle = handle;
                /*
                 * if we ended up inheriting the host machine context
                 * for some reason, then set the environment variable
                 * so attempts to inherit credentials via krb5_cc_default()
                 * will work correctly (klist, in particular)
                 */
                if (euid == 0 && env_context == pagenv) {
                    char env[SEC_LOGIN_PAG_NAME_LENGTH+SEC_LOGIN_ENV_NAME_LENGTH];
                    sprintf(env, "%s=", SEC_LOGIN_CCACHE_ENV);
                    setenv(env, pagenv, 1);
                }
                sec_login_pvt_default_context.valid = true;
            }
        }

    } UNLOCK_DEFAULT_CONTEXT;

    return handle;


}


/* s e c _ l o g i n _ p v t _ p u r g e _ d e f a u l t _ c o n t e x t
 *
 * Provided the handle provided by the caller really is the default context
 * handle, ensure that future attempts to get the default context come up
 * empty.  If the handle provided is not a handle on the default context
 * then just return false.
 */
PUBLIC  boolean32  sec_login_pvt_purge_default_context
  (
    sec_login_handle_t  handle
  )
{
    volatile boolean32            retval  = true;

    LOCK_DEFAULT_CONTEXT {

        if (sec_login_pvt_default_context.valid  &&
            sec_login_pvt_handle_equal(handle,
                                       sec_login_pvt_default_context.handle)){

            sec_login_pvt_default_context.valid = false;
            unsetenv(SEC_LOGIN_CCACHE_ENV);

        } else {
            retval = false;
        }

    } UNLOCK_DEFAULT_CONTEXT;

    return retval;
}


PUBLIC  void  sec_login_pvt_purge_context
  (
    sec_login_context_t  *lcp,
    error_status_t       *stp
  )
{
    char  *dfn;
    char  *cfn;
    int   i;


    if (lcp->auth_src == sec_login_auth_src_network) {
        /*
         * if this is the default context, purge default context state info as well
         * do this BEFORE actually purging the credential cache info so we don't
         * pull the rug out from under some other thread trying to reconstruct the
         * the default context.
         */
        (void) sec_login_pvt_purge_default_context(lcp);

        /* Flush the DFS kernel's reference to these credentials */
	if (lcp->v1_1_info.pag != 0) {
	    sec_login_pag_new_tgt(lcp->v1_1_info.pag, 0);
	}

        /* destroy persistent backing store for this, and all previous
         * copies of the context.  We ignore errors because the persistent
         * store will only exist if the context has been set, at least once,
         * as the default context
         */
         for (i = 0; i < KRB_INFO(lcp).num_caches; i++) {
            /* flush the kerberos cache file */
            (void) unlink(KRB_INFO(lcp).prev_caches[i]);

            /* flush both data files */
            sec_util_string_init(&dfn, "", KRB_INFO(lcp).prev_caches[i],
                                 DATAFILE_SUFFIX);
            (void) unlink(dfn);
            free(dfn);
            sec_util_string_init(&dfn, "", KRB_INFO(lcp).prev_caches[i],
				 DB_DATAFILE_SUFFIX);
            (void) unlink(dfn);
            free(dfn);
        }

        /* flush most recent files */
        (void) unlink(DATAFILE_NAME(dfn, KRB_INFO(lcp).cache));
        free(dfn);

        (void) unlink(DB_DATAFILE_NAME(dfn, KRB_INFO(lcp).cache));
        if (dfn) free(dfn);

        /* flush name cache */
        (void) unlink(NAMECACHE_NAME(dfn, KRB_INFO(lcp).cache));
        free(dfn);

	/* just unlink it instead of destroy to keep an in-momory copy around */
	cfn = krb5_cc_get_name(lcp->krb_info.cache);
	(void) unlink(cfn);

	/* krb5_cc_destroy(lcp->krb_info.cache);	
        lcp->krb_info.cache = NULL; */
    }
    rca_binding_cache_purge((sec_login_handle_t *) lcp);
     /* true indicates force cache remove no matter what's the refcount */
    sec_login_pvt_release_context(lcp, true);


}


PUBLIC  void  sec_login_pvt_purge_refresh_context
  (
    sec_login_context_t  *lcp,
    error_status_t       *stp
  )
{
    /* wipe out the contents of the cache */
    if (KRB_INFO(lcp).cache != NULL) {
        krb5_cc_destroy(lcp->krb_info.cache);
        lcp->krb_info.cache = NULL;
    }
    rca_binding_cache_purge(lcp);
    /* true indicates force cache remove no matter what's the refcount */
    sec_login_pvt_release_context(lcp, true);
}


/*
 * key retrieval function for use when keys come pre-crunched from the
 * application. The keyseed is just a (krb5_keyblock *) and we copy it
 * to the output
 */
static krb5_error_code  skey_keyproc
    (
	krb5_context	ctx,
	krb5_const krb5_enctype type,
	krb5_data 		*salt,
	krb5_const_pointer keyseed,
	krb5_keyblock      **key)
{
    if (!valid_keytype(type))
        return KRB5_PROG_ETYPE_NOSUPP;

    return( krb5_copy_keyblock(keyseed, key));
}

extern krb5_error_code pwd_keyproc(krb5_context ctxt,
				   krb5_const krb5_enctype type,
				   krb5_data *salt,
				   krb5_const_pointer keyseed,
				   krb5_keyblock **key);

/* s e c _ l o g i n _ p v t _ g e t _ l r g y _ l o g i n _ i n f o
 *
 * This routine populates the network and unix credentials of a
 * previously allocated login context, from the local registry.
 */
PUBLIC  void  sec_login_pvt_get_lrgy_login_info
  (
    boolean32           override,   /* [in]      */
    sec_login_context_t *lcp,       /* [in, out] */
    error_status_t      *stp        /* [out]     */
  )
{
    sec_lrgy_entry_t    lentry;
    unsigned_char_t     global_name[2*sec_rgy_name_t_size];
    CLEAR_STATUS(stp);

    u_strcpy(global_name, lcp->cell);
    u_strcat(global_name, "/");
    u_strcat(global_name, lcp->principal);

    sec_lrgy_entry_lookup(global_name, &lentry, stp);
    if (GOOD_STATUS(stp)) {
        sec_login_pvt_lrgy_to_lc(override, &lentry, lcp, stp);
    }

    if (GOOD_STATUS(stp)) {
        lcp->auth_src = sec_login_auth_src_local;
        lcp->state = allocated;
        lcp->identity->state = info_certified;
    }
}


/* s e c _ l o g i n _ p v t _ g e t _ l o g i n _ i n f o
 *
 * This routine populates the network and unix credentials of a
 * previously allocated login context.  It authenticates to
 * the registry server if the context state allows, otherwise,
 * the call is made unauthenticated.  If the context state allows
 * authentication, and credentials were previously obtained
 * unauthenticated, the old data is discarded, and authentic data
 * is obtained.
 */
PUBLIC  void  sec_login_pvt_get_login_info
  (
    boolean32               override,       /* [in] */
    sec_login_context_t     *lcp,           /* [in, out] */
    sec_override_fields_t   *overridden,    /* [out] */
    error_status_t          *stp            /* [out] */
  )
{
    sec_rgy_acct_key_t      key_parts;
    sec_rgy_handle_t        rgy_handle = (void *) (-1);
    sec_rgy_sid_t           sid;
    sec_rgy_unix_sid_t      unix_sid;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_acct_admin_t    admin_part;
    sec_rgy_plcy_t          policy;
    sec_rgy_login_name_t    rgy_acct_name;
    signed32                *unix_projlist;
    uuid_t                  *id_projlist;
    signed32                max_groups = SECLOGIN_NGROUPS;
    signed32                num_groups, total_groups;
    sec_rgy_name_t          cell_name;
    uuid_t                  cell_uuid;
    boolean32               get_info;
    boolean32               free_existing_info;
    char                    *cell_pgo_name;
    boolean32               authentic;
    login_info_t            *saved_identity;
    error_status_t          lst;
    error_status_t          ast;
    sec_login_handle_t      slh;
    sec_id_epac_set_t       epac_set;
    sec_id_epac_t           epac;
    sec_id_epac_data_t      epac_data;
    sec_bytes_t             epac_chain;
    sec_attr_t              login_attr;

    CLEAR_STATUS(stp);
    *overridden = sec_override_none;

    if (lcp->auth_src == sec_login_auth_src_local) {
        /* Info already obtained by this point */
        return;
    }

    /*
     * Get info from the network
     */

    check_info_state(lcp, &get_info, &free_existing_info, stp);
    if (BAD_STATUS(stp) || !get_info) {
        return;
    }

    /* CHFts15902: allow login to a remote cell only if the surrogate
     * account for the remote cell is valid in this cell
     */
    if (!lcp->local) {
        sec_rgy_bind_auth_info_t  auth_binding;
        sec_rgy_handle_t          rgy_handle;

        /* bind to registry of local cell */
        auth_binding.info_type = sec_rgy_bind_auth_none;
        sec_rgy_cell_bind((unsigned_char_p_t) "/.:", &auth_binding, &rgy_handle, stp);

        if (GOOD_STATUS(stp)) {
            sec_rgy_login_name_t    rgy_acct_name, name_result;
            sec_rgy_cursor_t        cursor;
            sec_rgy_sid_t           sid;
            sec_rgy_unix_sid_t      unix_sid;
            sec_rgy_acct_key_t      key_part;
            sec_rgy_acct_user_t     user_part;
            sec_rgy_acct_admin_t    admin_part;
            error_status_t          lst;

            /* create principal name of "krbtgt/<remote cell name without /.../>" */
            u_strcat(u_strcpy(rgy_acct_name.pname, KRB_DIR_ROOT), lcp->cell + GLOBAL_DIR_ROOT_LEN);
            rgy_acct_name.gname[0] = '\0';
            rgy_acct_name.oname[0] = '\0';
            sec_rgy_cursor_reset(&cursor);

            /* find account for surrogate principal */
            sec_rgy_acct_lookup(rgy_handle, &rgy_acct_name, &cursor,
                                &name_result, &sid, &unix_sid, &key_part,
                                &user_part, &admin_part, stp);
            if (BAD_STATUS(stp) || !(admin_part.flags & sec_rgy_acct_admin_valid)) {
               /* account not found or not valid - set bad status */
               SET_STATUS(stp, sec_login_s_acct_invalid);
            }

            sec_rgy_site_close(rgy_handle, &lst);
        }

        /* if no valid account was found, or if unable to bind to registry,
         * return with the bad status here
         */
        if (BAD_STATUS(stp)) {
            return;
        }

    }

    if (free_existing_info) {
        saved_identity = lcp->identity;
        allocate_identity(&lcp->identity, stp);
        if (BAD_STATUS(stp)) {
            return;
        }
    }

    if (lcp->state == certified || lcp->state == valid) {
        slh = (sec_login_handle_t) lcp;
    } else {
        slh = (sec_login_handle_t) -1;
    }

    /*
     * Try to authenticate by name to the home cell server using the TGT
     */
    authentic = bind_to_rgy(lcp->cell,
                rpc_c_authn_dce_secret, rpc_c_authz_name,
                slh, &rgy_handle, stp, &ast);

    if (GOOD_STATUS(stp) && !authentic) {
        *stp = ast;
    }

    if (BAD_STATUS(stp)) {
        if (free_existing_info) {
            lcp->identity = saved_identity;
        }
        return;
    }


    /*
     * use principal name in the login context as the key for network login info query
     */
    u_strncpy(rgy_acct_name.pname, lcp->principal, sizeof(sec_rgy_name_t));
    rgy_acct_name.pname[sizeof(sec_rgy_name_t) -1] = '\0';
    *rgy_acct_name.gname = '\0';
    *rgy_acct_name.oname = '\0';

    allocate_projlist_buffers(max_groups, &id_projlist, &unix_projlist, stp);
    if (BAD_STATUS(stp)) {
        error_status_t lst;
        if (free_existing_info) {
            lcp->identity = saved_identity;
        }
        sec_rgy_site_close(rgy_handle, &lst);
        rgy_handle = (void *) (-1);
        return;
    }

#define cleanup_projlists(id_groups, unix_groups) { \
    if ((id_groups) != NULL) { free((char *)(id_groups)); (id_groups) = NULL; }\
    if ((unix_groups )!= NULL) { free( (char *) (unix_groups)); (unix_groups) = NULL; } \
}

    if (override) {
        sec_rgy_login_get_effective(rgy_handle,&rgy_acct_name,&key_parts,&sid,
                                    &unix_sid,&user_part,&admin_part,&policy,
                                    max_groups,&num_groups,id_projlist,
                                    unix_projlist,&total_groups,cell_name,
                                    &cell_uuid,overridden,stp);
    } else {
        sec_rgy_login_get_info(rgy_handle, &rgy_acct_name, &key_parts, &sid,
                               &unix_sid, &user_part, &admin_part, &policy,
                               max_groups, &num_groups, id_projlist,
                               unix_projlist, &total_groups, cell_name,
                               &cell_uuid, stp);
    }
    if (BAD_STATUS(stp)) {
        if (free_existing_info) {
            lcp->identity = saved_identity;
        }
        cleanup_projlists(id_projlist, unix_projlist);
        NET_CREDS(lcp).num_groups = 0;
        sec_rgy_site_close(rgy_handle, &lst);
        rgy_handle = (void *) (-1);
        return;
    }

    /*
     * if we didn't catch the entire groupset on the first time around,
     * reallocate the projlist buffers, and try again.
     */
    if (max_groups < total_groups) {

        cleanup_projlists(id_projlist, unix_projlist);
        allocate_projlist_buffers(total_groups, &id_projlist, &unix_projlist, stp);
        if (BAD_STATUS(stp)) {
            if (free_existing_info) {
                lcp->identity = saved_identity;
            }
            sec_rgy_site_close(rgy_handle, &lst);
            rgy_handle = (void *) (-1);
            return;
        }

        max_groups = total_groups;
        if (override) {
            sec_rgy_login_get_effective(rgy_handle, &rgy_acct_name, &key_parts,
                                        &sid, &unix_sid, &user_part,
                                        &admin_part, &policy, max_groups,
                                        &num_groups, id_projlist, unix_projlist,
                                        &total_groups, cell_name, &cell_uuid,
                                        overridden, stp);
        } else {
            sec_rgy_login_get_info(rgy_handle, &rgy_acct_name, &key_parts, &sid,
                                   &unix_sid, &user_part, &admin_part, &policy,
                                   max_groups, &num_groups, id_projlist,
                                   unix_projlist, &total_groups, cell_name,
                                   &cell_uuid, stp);
        }

        if (BAD_STATUS(stp)) {
            sec_rgy_site_close(rgy_handle, &lst);
            rgy_handle = (void *) (-1);
            if (free_existing_info) {
                lcp->identity = saved_identity;
            }
            cleanup_projlists(id_projlist, unix_projlist);
            NET_CREDS(lcp).num_groups = -1;
            return;
        }
    }

    sec_rgy_site_close(rgy_handle, &lst);
    rgy_handle = (void *) (-1);
    memset(&epac_data, 0, sizeof(sec_id_epac_data_t));

    /* copy the network projlist into the context identity */
    if ( (NET_CREDS(lcp).num_groups = (unsigned16) num_groups) != 0) {

        NET_CREDS(lcp).groups =
            (sec_id_t *) malloc(sizeof(sec_id_t) * NET_CREDS(lcp).num_groups);

        if (NET_CREDS(lcp).groups == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
        } else {
            unsigned16  i;

            for (i=0; i < NET_CREDS(lcp).num_groups; i++) {
                NET_CREDS(lcp).groups[i].uuid = id_projlist[i];
                NET_CREDS(lcp).groups[i].name = NULL;
            }

            /* Build EPAC groupset too */
            epac_data.pa.num_groups = (unsigned16) num_groups;
            epac_data.pa.groups = NET_CREDS(lcp).groups;
        }
    }

    if (BAD_STATUS(stp)) {
        if (free_existing_info) {
            lcp->identity = saved_identity;
        }
        cleanup_projlists(id_projlist, unix_projlist);
        free( (char *) NET_CREDS(lcp).groups);
        NET_CREDS(lcp).groups = NULL;
        return;
    } else {
        free( (char *) id_projlist);
    }

#undef cleanup_projlist

    /*
     * copy cell, principal and group login info to the network credentials.
     */
    NET_CREDS(lcp).principal.uuid = epac_data.pa.principal.uuid = sid.person;
    NET_CREDS(lcp).group.uuid = epac_data.pa.group.uuid = sid.group;
    NET_CREDS(lcp).realm.uuid = epac_data.pa.realm.uuid = cell_uuid;

    u_dynstrcpy(&NET_CREDS(lcp).principal.name, lcp->principal);
    u_dynstrcpy(&NET_CREDS(lcp).realm.name, lcp->cell);
    u_dynstrcpy(&epac_data.pa.principal.name, lcp->principal);
    u_dynstrcpy(&epac_data.pa.realm.name, lcp->cell);

    set_policy(lcp, &admin_part, &user_part, &policy);
    if (free_existing_info) {
        /*
         * copy identity expiration from the old identity - it doesn't
         * come from the registry
         */
        POLICY_INFO(lcp).identity_exp = saved_identity->policy.identity_exp;
    }

    if (!lcp->local) {

        /*
         * This is a "foreign" login, so the unix info diverges from
         * the network info.  Specifically, the user has the name and uid of
         * of their home cell principal as registered in the host cell,
         * a canned gid, and no groupset info. if, for some reason, we can't
         * reach the host cell, we wipe out the unix info entirely,
         * so subsequent sec_login_get_pwent calls will return NULL passwd
         * data.
         */

        krb5_principal     tgt_princ;

        sec_krb_tgs_parsed_name( (char *) NET_CREDS(lcp).realm.name,
                                &tgt_princ, &lst);
        if (GOOD_STATUS(&lst)) {
            sec_dce_pair_from_krb_princ(tgt_princ, NULL, &cell_pgo_name, &lst);
            krb5_free_principal(tgt_princ);
        }

        /*
         * Try to authenticate by name to the host cell server using the TGT
         */
        if (GOOD_STATUS(&lst) && (lcp->state == certified || lcp->state == valid)) {
            slh = (sec_login_handle_t)lcp;
        } else {
            slh = (sec_login_handle_t)(-1);
        }

        authentic = bind_to_rgy(NULL, rpc_c_authn_dce_secret,
                rpc_c_authz_name, slh, &rgy_handle, &lst, &ast);

        if (GOOD_STATUS(&lst) && !authentic) {
            SET_STATUS(&lst, ast);
        }

        if (GOOD_STATUS(&lst)) {
            error_status_t xst;
            sec_rgy_cursor_t   cursor;
	    sec_rgy_login_name_t lname, rlname;
	    sec_rgy_unix_sid_t unix_sid2;

	    strncpy((char *) lname.pname, cell_pgo_name, sec_rgy_name_max_len);
	    /* Make sure the strings are all terminated.  Actual length is
	     * sec_rgy_name_max_len+1, so that first assignment is ok.
	     */
	    lname.pname[sec_rgy_name_max_len] = lname.gname[0] =
		lname.oname[0] = '\0';

            sec_rgy_cursor_reset(&cursor);
	    sec_rgy_acct_lookup(rgy_handle, &lname, &cursor, &rlname, NULL,
		&unix_sid2, NULL, NULL, NULL, &lst);

            if (GOOD_STATUS(&lst)) {
                /* overwrite unix data from user's home cell */
                unix_sid.person = unix_sid2.person;
                unix_sid.group = unix_sid2.group;
	    }
	    else {
		/*
		 * For transitive trust:  default to "<cell-uid>.none"
		 */
		sec_rgy_pgo_item_t item;

		sec_rgy_cursor_reset(&cursor);
		sec_rgy_pgo_get_by_name(rgy_handle,
					sec_rgy_domain_person,
					(unsigned_char_t *) cell_pgo_name,
					&cursor,
					&item,
					&lst);
		if (GOOD_STATUS(&lst)) {
		    unix_sid.person = item.unix_num;
		}

		sec_rgy_cursor_reset(&cursor);
		sec_rgy_pgo_get_by_name(rgy_handle,
					sec_rgy_domain_group,
					(unsigned_char_t *) "none",
					&cursor,
					&item,
					&lst);
		if (GOOD_STATUS(&lst)) {
		    unix_sid.group = item.unix_num;
		}
            }

	    UNIX_CREDS(lcp).num_groups = 0;
	    UNIX_CREDS(lcp).groups = NULL;
	    free( (char *) unix_projlist);  /* don't need it */

	    sec_rgy_site_close(rgy_handle, &xst);
	    rgy_handle = (void *) (-1);
        }

        if (GOOD_STATUS(&lst)) {
            make_passwd_context(&UNIX_CREDS(lcp).pwent,
                                (unsigned_char_t *) cell_pgo_name, &unix_sid,
                                &user_part, &admin_part, &policy, NULL, stp);
        }

    } else {

        /*
         * host and home cell are the same, so use the user's unix info
         * from the home cell login_get_info call.
         */
        make_passwd_context(&UNIX_CREDS(lcp).pwent,
                            NET_CREDS(lcp).principal.name,
                            &unix_sid, &user_part, &admin_part,
                            &policy, NULL, stp);

        UNIX_CREDS(lcp).num_groups = NET_CREDS(lcp).num_groups;
        UNIX_CREDS(lcp).groups = unix_projlist;
    }

    /* Always default to initiator compatibility mode */
    epac_data.compat_mode = sec_id_compat_mode_initiator;

    /* Always request the login set attribute */
    epac_data.num_attrs = 1;
    login_attr.attr_id = login_set_id;
    login_attr.attr_value.attr_encoding = sec_attr_enc_void;
    epac_data.attrs = &login_attr;

    /* Store the EPAC chain (of 1) in the lc */
    sec_id_epac_data_encode((void *(*)(unsigned32))malloc, free, &epac_data,
        &epac.pickled_epac_data.num_bytes, &epac.pickled_epac_data.bytes, stp);
    if (GOOD_STATUS(stp)) {
        epac.seals = NULL;
	epac_set.num_epacs = 1;
	epac_set.epacs = &epac;
        sec_id_epac_set_encode((void *(*)(unsigned32))malloc, free, &epac_set,
            &epac_chain.num_bytes, &epac_chain.bytes, stp);
        if (GOOD_STATUS(stp)) {
	    /* XXX - Could have used pac_to_epac_chain_bytes() routine? */
            EPAC_CHAIN(lcp) = epac_chain;
        }

        sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);
    }
    if (epac_data.pa.principal.name)
	free(epac_data.pa.principal.name);

    if (epac_data.pa.realm.name)
	free(epac_data.pa.realm.name);

    if (BAD_STATUS(stp)) {
        /*flush any newly acquired credentials, and restore the old */
        free_identity(&lcp->identity);
        lcp->identity = saved_identity;
    } else {

        if (free_existing_info) {
            free_identity(&saved_identity);
        }

        switch (lcp->state) {
            case certified:
                lcp->identity->state = info_certified;
                break;
            case  valid:
                lcp->identity->state = info_auth;
                break;
            default:
                lcp->identity->state = info_unauth;
                break;
        }
    }
}


/*
 *
 * This operation may only be called in a privileged process, and the
 * context passed in must be a valid network context
 *
 * The context is certified by performing the folowing steps.
 *
 * 1) acquire a PTGT to the host cell.
 * 2) use the PTGT to acquire a ticket to host machine
 * 3) use the password stored in the local host config database
 *    to decrypt the ticket acquired in (2).
 *
 * Once we have succesfully performed steps 1-3, we know that there
 * os a valid trust path from the user's home cell to the host
 * machine cell
 *
 * NOTE: we MUST use privilege tickets in order to get
 *       trust path evaluation performed by the priv server.
 */
PUBLIC  void  sec_login_pvt_certify_context
  (
    sec_login_context_t  *lcp,
    error_status_t       *stp
  )
{
    char                 *host_cell, *host_name = NULL, *hnp;
    krb5_creds           kcreds;
    krb5_ticket          *ticket = NULL;
    krb5_keyblock        *krb_key;
    extern int           krb5_kdc_default_options;
    sec_passwd_rec_t     *host_key, *kp;
    error_status_t       lst;
    unsigned32           kvno = 0;
    sec_passwd_type_t    keytype = 0;
    unsigned_char_t      *dummy_princ;


    CLEAR_STATUS(stp);

    if (geteuid() != 0) {
        SET_STATUS(stp, sec_login_s_privileged);
        return;
    }

    /*
     * local contexts are certified from the start, because they are
     * constructed from previously certified data protected by local security
     */
    if (lcp->auth_src == sec_login_auth_src_local) {
        lcp->state = certified;
        lcp->identity->state = info_certified;
        return;
    }

    /*
     * try for a PTGT in the host cell
     */
    dce_cf_get_cell_name(&host_cell, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    sec_login_util_get_krb_ptgt((sec_login_handle_t) lcp,
			(unsigned_char_p_t) host_cell,
                        rpc_c_authn_dce_secret, rpc_c_authz_dce, 
                        NULL, stp);
    free(host_cell);
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * use the PTGT aquired above to get a ticket to the
     * host machine principal
     */
    memset(&kcreds, 0, sizeof(kcreds));
    sec_krb_ptgs_parsed_name(NULL, &kcreds.client, stp);
    if (GOOD_STATUS(stp)) {
        dce_cf_prin_name_from_host(NULL, &host_name, stp);
        if (GOOD_STATUS(stp)) {
            if (strncmp(CELL_DIR_ROOT, host_name, CELL_DIR_ROOT_LEN) == 0) {
                hnp = host_name + CELL_DIR_ROOT_LEN;
            } else {
                hnp = host_name;
            }
            sec_krb_princ_from_dce_pair(NULL, hnp, &kcreds.server, stp);
        }
    }
    if (GOOD_STATUS(stp)) {
        /* construct DCE 1.1 stkt authorization data */
	priv_krb_util_construct_v1_stkt_authdata(NULL, NULL, &kcreds.authdata, stp);
    }
    if (GOOD_STATUS(stp)) {
        *stp = krb5_get_credentials_dce(krb5_kdc_default_options, 
			KRB_INFO(lcp).cache, &kcreds, NULL);
    }

    if (BAD_STATUS(stp)) {
        krb5_free_cred_contents(&kcreds);
        free(host_name);
        return;
    }

    /*
     * If we can successfully decode and decrypt the ticket to the
     * host machine principal using the host machine's key, then
     * we're certified
     */

    if (GOOD_STATUS(stp)) {
        *stp = decode_krb5_ticket(&kcreds.ticket, &ticket);
    }

    if (GOOD_STATUS(stp)) {
         kvno = (unsigned32) ticket->enc_part.kvno;
         keytype = krb_to_sec_keytype
                       (krb5_csarray[ticket->enc_part.etype]->system->proto_keytype,
                       stp);
    }

    if (GOOD_STATUS(stp)) {
        sec_key_mgmt_get_key(rpc_c_authn_dce_secret, NULL,
                             (unsigned_char_p_t) host_name,
                              kvno, (void **)&host_key, stp);
        if (GOOD_STATUS(stp)) {
            /*
             * There could be more than one key with this version number,
             * so don't bother trying keys whose types don't match the
             * type of the key used to encrypt the ticket
             */
            boolean32 success = false;
            for(kp = host_key;
                PASSWD_TYPE(kp) != sec_passwd_none && !success; kp++) {
                if (PASSWD_TYPE(kp) == keytype) {
                    krb_key = sec_to_krb_key(kp, stp);
                    if (GOOD_STATUS(stp)) {
                        *stp = krb5_decrypt_tkt_part(krb_key, ticket);
                        success = GOOD_STATUS(stp);
                        krb5_free_keyblock(krb_key);
                    }
                }
            }
            sec_key_mgmt_free_key(host_key, &lst);
        }
    }

    if (GOOD_STATUS(stp)) {
        /*
         * just for laughs (and also because the checksum used by the
         * encryption algorithm is not guaranteed to be collision proof :-)
         * let's just do a few sanity checks on the decrypted ticket
         */
        if (!krb5_principal_compare(ticket->enc_part2->client, kcreds.client)
            || !krb5_principal_compare(ticket->server, kcreds.server)
            || !(ticket->enc_part2->session->keytype == kcreds.keyblock.keytype
            && ticket->enc_part2->session->length == kcreds.keyblock.length
            && memcmp(ticket->enc_part2->session->contents,
                      kcreds.keyblock.contents,
                      kcreds.keyblock.length) == 0)) {
            SET_STATUS(stp, sec_login_s_not_certified);
        }
    }

    if (ticket != NULL) {
        krb5_free_ticket(ticket);
    }

    if (host_name != NULL) {
        free(host_name);
    }

    krb5_free_cred_contents(&kcreds);

    if (GOOD_STATUS(stp)) {
        lcp->state = certified;
        lcp->identity->state = info_certified;
    }

    return;
}


PUBLIC  void  sec_login_pvt_check_policy
  (
    sec_login_context_t   *lcp,          /* [in,out] */
    boolean32             *reset_passwd, /* [out]    */
    error_status_t        *stp
  )
{

    CLEAR_STATUS(stp);
    *reset_passwd = false;

    /* nothing to check in a locally authenticated context */
    if (lcp->auth_src == sec_login_auth_src_local) {
        return;
    }

    if (!FLAG_SET(POLICY_INFO(lcp).pflags, policy_acct_valid)) {
        SET_STATUS(stp, sec_login_s_acct_invalid);
        return;

    }

    *reset_passwd =  !FLAG_SET(POLICY_INFO(lcp).pflags, policy_passwd_valid);
}


PUBLIC  void  sec_login_pvt_create_restricted_context
  (
    sec_login_context_t  *base_lc,
    unsigned32           num_new_groups,
    sec_id_t             *new_groups,
    sec_login_context_t  **restricted_lc,
    error_status_t       *stp
  )
{
    login_info_t         *res_ident;
    boolean32            found;
    unsigned int         i,j;
    error_status_t       lst;
    sec_login_context_t  *rcp;
    krb5_creds           creds, tgt_creds;

    memset(&creds, 0, sizeof(creds));
    memset(&tgt_creds, 0, sizeof(tgt_creds));

    allocate_identity(&res_ident, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    CLEAR_STATUS(stp);

    /* make sure the new groupset is actually a subset of the current groupset */
    if (num_new_groups > 0) {

        /* allocate space for restricted groupset info */
        res_ident->net_creds.groups =
            (sec_id_t *) malloc(sizeof(sec_id_t) * num_new_groups);
        if (res_ident->net_creds.groups == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            free_identity(&res_ident);
            return;
        }
        memset(res_ident->net_creds.groups, 0, sizeof(sec_id_t) * num_new_groups);
        res_ident->net_creds.num_groups = num_new_groups;

        /*
         * If the user and host machine are in the same cell, then there is a
         * corresponding unix number for each network group uuid.  If not, then
         * the user is a "foreign" user, and has no groupset (because unix numbers
         * are not unique across cells).
         */
        if (base_lc->local) {
            res_ident->unix_info.groups =
                (signed32 *) malloc(sizeof(signed32) * num_new_groups);
            if (res_ident->unix_info.groups == NULL) {
                SET_STATUS(stp, sec_login_s_no_memory);
                free_identity(&res_ident);
                return;
            }
            memset(res_ident->unix_info.groups, 0, sizeof(signed32) * num_new_groups);
            res_ident->unix_info.num_groups = num_new_groups;
        }


        for (i = 0; i < num_new_groups && GOOD_STATUS(stp); i++) {

            /* each new_group must be a member of the base groupset */
            found = false;
            for (j = 0; j < NET_CREDS(base_lc).num_groups && !found; j++) {
                if (uuid_equal(&new_groups[i].uuid,
                               &NET_CREDS(base_lc).groups[j].uuid, &lst)) {
                    found = true;
                    res_ident->net_creds.groups[i].uuid = new_groups[i].uuid;
                     /*
                      * only copy unix groupset info if the user and host machine
                      * are in the same cell
                      */
                    if (base_lc->local) {
                        res_ident->unix_info.groups[i] = UNIX_CREDS(base_lc).groups[j];
                    }
                }
            }
            if (!found) {
                SET_STATUS(stp, sec_login_s_groupset_invalid);
            }
        }
    }

    if (BAD_STATUS(stp)) {
        free_identity(&res_ident);
        return;
    }

    res_ident->policy = POLICY_INFO(base_lc);
    res_ident->state = INFO_STATE(base_lc);
    res_ident->net_creds.principal.uuid = NET_CREDS(base_lc).principal.uuid;
    res_ident->net_creds.group.uuid = NET_CREDS(base_lc).group.uuid;
    res_ident->net_creds.realm.uuid = NET_CREDS(base_lc).realm.uuid;
    u_dynstrcpy(&res_ident->net_creds.principal.name, base_lc->principal);
    u_dynstrcpy(&res_ident->net_creds.realm.name, base_lc->cell);

    /* allocate restricted context substructure */
    rcp = (sec_login_context_t *)malloc(sizeof(sec_login_context_t));
    if (rcp == NULL) {     /* can't allocate any space so bail out */
        MEM_FAILURE(stp);
        free_identity(&res_ident);
        return;
    }
    memset(rcp, 0, sizeof(sec_login_context_t));
    rcp->refcount = 0;
    rcp->mark = 0;
    rcp->cache_link_next = NULL;
    rcp->cache_link_prev = NULL;
    rcp->namecache = NULL;
    rcp->namecache_name = NULL;

    rcp->identity = res_ident;
    /* use a new credential cache, and copy the TGT into it later */
    allocate_krb_info(&rcp->krb_info, NULL, stp);
    if (BAD_STATUS(stp))
        goto errout;

    /* Initialize db_mutex */
    init_db_mutex(&rcp->db_mutex, stp);
    if (BAD_STATUS(stp)) {
	goto errout;
    }

    /*
     * fill in the rest of the restricted identity info from the info in the
     * base login_context
     */
    rcp->state = base_lc->state;
    rcp->auth_src = base_lc->auth_src;
    rcp->local = base_lc->local;

    u_strcpy(rcp->cell, base_lc->cell);
    u_strcpy(rcp->principal, base_lc->principal);

    if (UNIX_CREDS(base_lc).pwent) {
        /* copy pwent data */
        UNIX_CREDS(rcp).pwent =
	    (struct sec_passwd *) malloc(sizeof(struct sec_passwd));
        if (UNIX_CREDS(rcp).pwent == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            goto errout;
        }
        if (GOOD_STATUS(stp)) {
            alloc_pwent_string_buffers(UNIX_CREDS(rcp).pwent, stp);
        }
        if (BAD_STATUS(stp))
            goto errout;

        memcpy(UNIX_CREDS(rcp).pwent->pw_name, UNIX_CREDS(base_lc).pwent->pw_name,
               PW_MAX_NAME_LEN + PW_MAX_PASSWD_LEN + PW_MAX_CLASS_LEN +
               PW_MAX_GECOS_LEN + PW_MAX_DIR_LEN + PW_MAX_SHELL_LEN);

        UNIX_CREDS(rcp).pwent->pw_uid = UNIX_CREDS(base_lc).pwent->pw_uid;
        UNIX_CREDS(rcp).pwent->pw_gid = UNIX_CREDS(base_lc).pwent->pw_gid;
        UNIX_CREDS(rcp).pwent->pw_change = UNIX_CREDS(base_lc).pwent->pw_change;
        UNIX_CREDS(rcp).pwent->pw_expire = UNIX_CREDS(base_lc).pwent->pw_expire;
    }

    /*
     * copy the user's home cell TGT to the new context.  Do *NOT* copy
     * any PTGTs as that would defeat the whole purpose of newgroups.
     */
    memset(&creds, 0, sizeof(creds));

    /* first, the client principal - we can get it from the credential cache*/
    *stp = krb5_cc_get_principal(KRB_INFO(base_lc).cache, &creds.client);

    if (GOOD_STATUS(stp)) {
        /* copy intialize the new cache with the client principal */
        *stp = krb5_cc_initialize(KRB_INFO(rcp).cache, creds.client);
    }

    if (GOOD_STATUS(stp)) {
        /* now the Ticket-Granting-Ticket server principal */
        sec_krb_tgs_parsed_name( (char *) rcp->cell, &creds.server, stp);
    }

    if (GOOD_STATUS(stp)) {
        /*
         * Use an endtime of "now" in the TGT lookup, so we don't copy an
         * expired TGT
         */
        *stp =  krb5_timeofday(&creds.times.endtime);
    }

    if (GOOD_STATUS(stp)) {
        *stp = krb5_cc_retrieve_cred(KRB_INFO(base_lc).cache,
                                     0 | KRB5_TC_MATCH_TIMES,
                                     &creds, &tgt_creds);
    }

    if (GOOD_STATUS(stp)) {
        *stp = krb5_cc_store_cred(KRB_INFO(rcp).cache, &tgt_creds);
    }


    if (creds.client != NULL) {
        krb5_free_cred_contents(&creds);
    }
    if (tgt_creds.client != NULL) {
        krb5_free_cred_contents(&tgt_creds);
    }

    if (BAD_STATUS(stp)) {
        goto errout;
    }

    if (INFO_STATE(rcp) == info_certified) {
        char  *cfn;
        /*
         * chown the credential cache and data file with the uid from
         * context unix info.
         * Also, must change the login context owner uid whenever
         * we chown the krb5 cache file.
         */
        cfn = krb5_cc_get_name(rcp->krb_info.cache);
	if (cfn != NULL && UNIX_CREDS(rcp).pwent != NULL && geteuid() == 0) {
	    if (chown(cfn, UNIX_CREDS(rcp).pwent->pw_uid, -1) == -1)
		*stp = errno;
	    rcp->owner_uid = UNIX_CREDS(rcp).pwent->pw_uid;
        }
    }

    lookup_and_cache_enter(rcp);
    *restricted_lc = rcp;
    return;
errout:
    *restricted_lc = NULL;
    sec_login_pvt_free_login_context(rcp);
    return;
}


PUBLIC  void  sec_login_pvt_convert_local
  (
    sec_login_context_t   *lcp,             /* [in, out] */
    error_status_t        *stp
  )
{
    login_info_t          *lid;

    CLEAR_STATUS(stp);

    if (lcp->auth_src == sec_login_auth_src_local) {
        return;
    }

    /*
     * allocate storage for the local identity
     */
    allocate_identity(&lid, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * try to find the principal in the local registry.  For now, this is
     * just /etc/password.
     */
    if (GOOD_STATUS(stp)) {
        get_local_unix_info(lcp->cell, lcp->principal, &lid->unix_info.pwent,
                             &lid->unix_info.num_groups, &lid->unix_info.groups,
                             stp);
    }
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * wipe out any existing network info
     */
    free_identity(&lcp->identity);
    /*
     * get rid of any ticket cache files created previously in the
     * login process
     */
    if (lcp->krb_info.cache != NULL) {
        krb5_cc_destroy(lcp->krb_info.cache);
        lcp->krb_info.cache = NULL;
    }
    free_krb_info(&lcp->krb_info);

    lcp->identity = lid;

    lcp->auth_src = sec_login_auth_src_local;
    lcp->identity->state = info_certified;
    if (lcp->state == valid) {
        /*
         * we can bump it to "certified" because local data is protected by local
         * security
         */
        lcp->state = certified;
    }
}


PUBLIC  void  sec_login_pvt_check_local_passwd
  (
    char                  *user,            /* [in] */
    sec_passwd_rec_t      *passwd,          /* [in] */
    error_status_t        *stp
  )
{
    char                  pw_passwd[sizeof(sec_rgy_unix_passwd_buf_t)];
    char                  pw_gecos[sizeof(sec_rgy_name_t)];
    char                  pw_dir[sizeof(sec_rgy_name_t)];
    char                  pw_shell[sizeof(sec_rgy_name_t)];
    int                   pw_gid;
    int                   pw_uid;
    unsigned16            num_groups;
    signed32              *groups;
    char                  *pwd;

    sec_login_pvt_get_local_login_info(user, (char *)pw_passwd, &pw_uid,
                                      &pw_gid, pw_gecos, pw_dir, pw_shell,
                                      &num_groups, &groups, stp);
    if (GOOD_STATUS(stp)) {
        if (groups != NULL) {
            free((char *) groups);
        }
        pwd = crypt((char*)(PLAIN_PASSWD(passwd)), pw_passwd);
        if (strcmp(pwd, pw_passwd) != 0) {
            SET_STATUS(stp, sec_rgy_passwd_invalid);
        }
    }
}


PRIVATE void copy_creds
  (
    krb5_creds      *in,
    krb5_creds      *out,
    error_status_t  *stp
  )
{
    krb5_data  *ticket;

    *out = *in;

    *stp = krb5_copy_principal(in->client, &out->client);
    if (GOOD_STATUS(stp))
        *stp = krb5_copy_principal(in->server, &out->server);
    if (GOOD_STATUS(stp) && in->keyblock.length != 0)
        *stp = krb5_copy_keyblock_contents(&in->keyblock, &out->keyblock);
    if (GOOD_STATUS(stp) && in->addresses != NULL)
        *stp = krb5_copy_addresses(in->addresses, &out->addresses);
    if (GOOD_STATUS(stp) && in->ticket.length != 0 && in->ticket.data != NULL) {
        *stp = krb5_copy_data(&in->ticket, &ticket);
        if (GOOD_STATUS(stp)) {
            out->ticket = *ticket;
            free((char *)ticket);
        }
    }
    if (GOOD_STATUS(stp) && in->second_ticket.length != 0
    && in->second_ticket.data != NULL) {
        *stp = krb5_copy_data(&in->second_ticket, &ticket);
        if (GOOD_STATUS(stp)) {
            out->second_ticket = *ticket;
            free((char *)ticket);
        }
    }
}



PUBLIC sec_login_context_t * sec_login_pvt_setup_refresh_context
  (
     sec_login_context_t   *lcp,  /* [in] */
     error_status_t        *stp
  )
{
    sec_login_context_t   *rcp;
    krb5_principal        princ;
    error_status_t        lst;

    CLEAR_STATUS(stp);

    if (lcp->auth_src == sec_login_auth_src_local) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                       "Login context is sec_login_auth_src_local\n"));
        SET_STATUS(stp, sec_login_s_internal_error);
        return NULL;
    }

    /* allocate  context substructure */
    rcp = (sec_login_context_t *)malloc(sizeof(sec_login_context_t));
    if (rcp == NULL) {     /* can't allocate any space so bail out */
        MEM_FAILURE(stp);
        return NULL;
    }
    memset(rcp, 0, sizeof(sec_login_context_t));
    rcp->refcount = 0;
    rcp->mark = 0;
    rcp->cache_link_next = NULL;
    rcp->cache_link_prev = NULL;
    rcp->namecache = 0;
    rcp->namecache_name = 0;
#ifdef NOTDEF
    rcp->cred_chain = lcp->cred_chain;
#endif /* NOTDEF FAKE-EPAC */

    /* copy the ticket request info */
    rcp->tkt_request.options = lcp->tkt_request.options;
    rcp->tkt_request.lifetime = lcp->tkt_request.lifetime;
    rcp->tkt_request.renewable_lifetime = lcp->tkt_request.renewable_lifetime;
    rcp->tkt_request.postdated_dormanttime = lcp->tkt_request.postdated_dormanttime;

    allocate_identity(&rcp->identity, stp);
    if (BAD_STATUS(stp))
        goto out;

    INFO_STATE(rcp) = info_none;
    u_strcpy(rcp->cell, lcp->cell);
    u_strcpy(rcp->principal, lcp->principal);

    /* copy the kerberos info */
    allocate_krb_info(&KRB_INFO(rcp), NULL, stp);
    if (BAD_STATUS(stp))
        goto out;

    /* Initialize db mutex */
    init_db_mutex(&rcp->db_mutex, stp);
    if (BAD_STATUS(stp)) {
	goto out;
    }

    /* initialize the cache */
    *stp = krb5_cc_get_principal(KRB_INFO(lcp).cache, &princ);
    if (BAD_STATUS(stp))
        goto out;

    *stp = krb5_cc_initialize(KRB_INFO(rcp).cache, princ);
    krb5_free_principal(princ);

    /* copy pepper contents */
    if (KRB_INFO(lcp).pepper.length > 0) {
        KRB_INFO(rcp).pepper.contents =
            malloc(KRB_INFO(lcp).pepper.length);
        if (KRB_INFO(rcp).pepper.contents == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            goto out;
        } else {
            memcpy(KRB_INFO(rcp).pepper.contents,
                   KRB_INFO(lcp).pepper.contents,
                   KRB_INFO(lcp).pepper.length);
            KRB_INFO(rcp).pepper.length = KRB_INFO(lcp).pepper.length;
        }
    } else {
        KRB_INFO(rcp).pepper.length = 0;
        KRB_INFO(rcp).pepper.contents = NULL;
    }

    /* copy the contents of the tgt */
    if (KRB_REQUEST_INFO(lcp) != NULL) {
	copy_tgt_data(KRB_REQUEST_INFO(lcp), &KRB_REQUEST_INFO(rcp), stp);
	if (BAD_STATUS(stp))
	    goto out;
    }

    rcp->state = setup;
    rcp->auth_src = sec_login_auth_src_network;
    rcp->flags = lcp->flags;
    rcp->local = lcp->local;

    lookup_and_cache_enter(rcp);
    return rcp;

out:
    /* wipe out the contents of the cache */
    if (KRB_INFO(rcp).cache != NULL) {
        krb5_cc_destroy(rcp->krb_info.cache);
        rcp->krb_info.cache = NULL;
    }
    sec_login_pvt_free_login_context(rcp);
    return NULL;
}


PUBLIC  void   sec_login_pvt_synch_refresh_context
  (
    sec_login_context_t   *lcp,  /* [in, out] */
    sec_login_context_t   *rcp,  /* [in] */
    error_status_t        *stp
  )
{
    krb5_creds            creds, tgt_creds;
    unsigned int          i,j;
    boolean32             found;
    error_status_t        lst;
    sec_login_db_entry_t  db_entry;
    char		  *dfn = NULL;
    unsigned32            expiration;

    CLEAR_STATUS(stp);

    /*
     * make sure the credentials haven't changed too drastically since
     * the previous login/refresh.
     *   - principal and primary group uuid must be the same
     *   - we'll accept deletions from the groupset, but not additions
     *     to it.  If the intersection of the old and new groupsets is
     *     empty, the refreshed context will have an empty groupset.
     *     Any additions are simply ignored.
     */

    if (!(uuid_equal(&NET_CREDS(lcp).principal.uuid,
                     &NET_CREDS(rcp).principal.uuid, &lst)
          && uuid_equal(&NET_CREDS(lcp).group.uuid,
            &NET_CREDS(rcp).group.uuid, &lst)
          && uuid_equal(&NET_CREDS(lcp).realm.uuid,
            &NET_CREDS(rcp).realm.uuid, &lst))) {
        SET_STATUS(stp, sec_login_s_refresh_ident_bad);
        return;
    }

    for (i = 0; i < NET_CREDS(lcp).num_groups; i++) {

        found = false;
        for (j = 0; j < NET_CREDS(rcp).num_groups && !found; j++) {
            if (uuid_equal(&NET_CREDS(lcp).groups[i].uuid,
                           &NET_CREDS(lcp).groups[j].uuid, &lst)) {
                found = true;
            }
        }

        if (!found) {

            /*
             * Shift remaining groups down 1 slot in the groupset(s).
             * We only shift the unix groupset if this is a "local"
             * login.  If the user is from a foreign cell, they
             * have no unix groupset info
             */
            for (j = i; j < NET_CREDS(lcp).num_groups - 1; j++) {

                /* don't orphan any string storage */
                if (NET_CREDS(lcp).groups[j].name != NULL && j==i) {
                   u_free(NET_CREDS(lcp).groups[j].name);
                }

                NET_CREDS(lcp).groups[j] = NET_CREDS(lcp).groups[j+1];

                if (lcp->local) {
                    UNIX_CREDS(lcp).groups[j] = UNIX_CREDS(lcp).groups[j+1];
                }
            }

            /*
             * don't advance outer for loop index if we've slid things down
             * a slot
             */
            i--;

            /* there's one less group in the groupset */
            NET_CREDS(lcp).num_groups--;
            if (lcp->local) {
                UNIX_CREDS(lcp).num_groups--;
            }
        }
    }

    /*
     * Zero out any epac_enum entry types in the .data.db file
     * because they are no longer valid - they don't exist in
     * the krb5 creds file anymore.
     */
    memset(&db_entry, 0, sizeof(db_entry));
    db_entry.entry_type = epac_enum;
    sec_login_util_store_db_entries((sec_login_handle_t *) lcp,
		DB_DATAFILE_NAME(dfn, KRB_INFO(lcp).cache),
                1, &db_entry, stp);
    if (dfn)
        free(dfn);
    if (BAD_STATUS(stp)) {
        return;
    }

    /* 
     * Free the TGT data in the login context before we stomp it with 
     * the contents of the refresh context.
     */
    free_krb_request_info(&KRB_REQUEST_INFO(lcp));

    /*
     * Copy the TGT from the refresh context to
     * the callers login context.
     */
    copy_tgt_data(KRB_REQUEST_INFO(rcp), &KRB_REQUEST_INFO(lcp), stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * Swap the newly refreshed data in the refresh context into the
     * existing context.
     */
    sec_login_pvt_ccache_swap(lcp, rcp, stp);
    if (!GOOD_STATUS(stp)) {
        return;
    }

    /*
     * Adjust the local data associated with the login context.
     */
    lcp->state = rcp->state;
    lcp->auth_src = rcp->auth_src;
    lcp->flags = rcp->flags;
    lcp->local = rcp->local;
    INFO_STATE(lcp) = INFO_STATE(rcp);
    update_identity_exp (lcp, stp);

    /* Tell DFS about the new expiration time */
    expiration = POLICY_INFO(lcp).identity_exp;
    sec_login_pag_new_tgt(lcp->v1_1_info.pag, expiration);

    /*
     * synch the backup data file, if any.
     */
    write_context(lcp, true, stp);

}


PUBLIC void sec_login_pvt_get_ecreds(
    sec_login_context_t  *lcp,
    sec_bytes_t          *cred_chain_p,
    error_status_t       *stp)
{
    krb5_creds           kcreds, creds;

    memset(&kcreds, 0, sizeof(kcreds));
    memset(&creds, 0, sizeof(creds));

    /*
     * Look up the home cell PTGT in the credential cache.
     * Call the priv server first to be sure there is one.
     */
    sec_login_util_get_krb_ptgt((sec_login_handle_t) lcp, lcp->cell,
	rpc_c_authn_dce_secret, rpc_c_authz_dce, NULL, stp);

    if (GOOD_STATUS(stp)) {
        sec_krb_ptgs_parsed_name((char *) lcp->cell, &kcreds.client, stp);
    }

    if (GOOD_STATUS(stp)) {
        /* now the Ticket-Granting-Ticket server principal */
        sec_krb_tgs_parsed_name( (char *) lcp->cell, &kcreds.server, stp);
    }

    if (GOOD_STATUS(stp)) {
        *stp = krb5_cc_retrieve_cred(KRB_INFO(lcp).cache,
                                     0 | KRB5_TC_MATCH_TIMES,
                                     &kcreds, &creds);
    }

    if (GOOD_STATUS(stp)) {
	sec_bytes_t  tmp_cred_chain;
	boolean32    found;
        char         *dfn = NULL;

        found = priv_krb_util_fetch_v1_1_ccache_authdata(
			(sec_login_handle_t *) lcp, malloc_shim, free,
			DB_DATAFILE_NAME(dfn, KRB_INFO(lcp).cache),
			lcp->cell, &tmp_cred_chain, NULL, stp);
        if (dfn)
            free(dfn);

	if (GOOD_STATUS(stp)) {
	    if (!found || !tmp_cred_chain.bytes) {
		/* 
		 * no v1_1 creds.  That must mean that our cell is
		 * running pre-1.1 security servers, so get the v1_pac
		 * and convert it to an encoded epac chain..
		 */
		idl_pkl_t     *ppac;
		sec_id_pac_t  tmp_pac;

		priv_krb_util_authdata_to_pac(creds.authdata, &ppac, stp);
		if (GOOD_STATUS(stp) && ppac) {
		    sec_id_pac_t_unpickle(ppac, (void *(*)(unsigned32)) malloc,
					  &tmp_pac, stp);
		    free(ppac);
		    if (GOOD_STATUS(stp)) {
			pac_to_epac_chain_bytes(&tmp_pac, &tmp_cred_chain,
			    0, NULL, sec_id_deleg_type_none, 
			    sec_id_compat_mode_initiator, stp);
			sec_id_pac_util_free(&tmp_pac);
                    }
		}
	    }
	}

	if (GOOD_STATUS(stp)) {
	    *cred_chain_p = tmp_cred_chain;
	}
    }

    /* Clean up after ourselves */
    if (kcreds.client != NULL) {
        krb5_free_cred_contents(&kcreds);
    }
    if (creds.client != NULL) {
        krb5_free_cred_contents(&creds);
    }
}



/* sec_login_pvt_inq_pac
 *
 * Prior to DCE 1.1, a user's credentials consisted of a
 * single v1 PAC, and the DCE privilege service returned
 * a copy of the PAC that included names.  klist
 * used those names to when displaying credentials.
 * At 1.1 things got more complex, for the following
 * reasons.
 *
 * 1. The user may not have any pre-1.1
 *    credentials at all.
 * 
 * An application making use of 1.1 features may
 * chose to disable pre-1.1 compatibility altogether.
 * In that case, an auth RPC to an app server  linked
 * with pre-1.1 libraries will fail in the server 
 * runtime, and an auth RPC to a pre-1.1 app
 * server that has been relinked with, but not
 * ported to, 1.1 libraries will cause the server to
 * see an unauthenticated PAC full of NIL uuids.
 * In this case, We return the same thing that the
 * pre-1.1 server linked with 1.1 libraries would see,
 * *plus* we stuff a "<NIL>" string in the name fields
 * of the cell, principal, and primary group stuctures of
 * the PAC.  This latter step saves applications
 * from performing fruitless sec_id_gen_name() 
 * calls to convert the uuid to a name.
 *
 * Or an application may attempt to utilize 1.1
 * security delegation features in a 1.0.3 security 
 * cell. In that case, the client will be unable to 
 * obtain a pre-1.1 PTGT of any sort (1.1 requests
 * containing ERAs are considered compatible since
 * a 1.1 request containing ERAs succeeds when there
 * are no instances of the specified ERAs attached
 * to the principal in question).  In this case, we
 * propagate the sec_s_v1_1_no_support status back to
 * the client, rather than pretend that the client
 * has any sort of credentials at all.
 *
 * 2. The credential data returned by the 1.1 priv
 *    priv server does not contain names.
 *    
 * The pre-1.1 priv server generated 2 copies
 * of the PAC.  One, containing only uuids,
 * was put into the authorization data of the
 * generated PTGT, and another, containing both
 * uuids and names  was returned in a manner
 * such that the client could extract the identity
 * and store it for later return to callers of
 * sec_login_inquire_net_info().  Since the PAC
 * contained names, there was no need for clients of
 * the sec_login_inquire_net_info() API to call
 * sec_id_parse_name().  Since the 1.1 credentials
 * contain no names, clients of sec_login_inquire_net_info()
 * must use sec_id_gen_name() if they care about names.
 * The klist program does exactly this. 
 *
 * This seems inefficient in the case of cell and 
 * principal names, since we have already generated 
 * those particular names and stored them in the
 * login context during the login context setup
 * using sec_id_parse_name().   Plus, sec_id_gen_name()
 * always returns the primary name associated with the
 * principal.  This could cause confusion for users that
 * log in using an alias, and then see the primary name
 * in klist output.  Since we already have the names
 * we need, why not copy them into the returned PAC?
 *
 * But the cell and principal names in the login context
 * do not always correspond to the cell and principal uuids
 * in the compatibility info.  If delegation is in use, the
 * cell and principal names are names of the intermediary
 * engaging in delegation, but the compatibility PAC, if
 * it exists, is may be for either the intermediary, or the
 * intiator, depending on the compatibility mode in effect.
 * Only if the compatibility mode is "caller" will the
 * PAC be the PAC of the intermediary.  Only in that
 * cae is it appropriate to associate the the names of the
 * intermediary with the uuids in the compatibility PAC.
 * If impersonation is in use, the PAC will always be the 
 * PAC of the initiator and it is never appropriate to 
 * associate the names of the impersonator with the uuids 
 * of the intitiator.  Finally, if delegation is not
 * in use, the compatibility PAC will always correspond
 * to the names in the initiator login context.  If 
 * the compat mode is other that "none" we use the
 * names from the login context in the PAC.
 * 
 * 3. At 1.1, we don't always know what to do if
 *    the PTGT request fails.
 *
 * Prior to 1.1, if the PTGT request failed, we used
 * whatever PAC data was used to seed PTGT requests,
 * since it corresponds to the credentials in effect
 * at the time the principal logged in.
 *
 * Now, if the EPTGT request fails, we can't always
 * do that.  Considerations similar to (2) apply.  If the
 * EPTGT request *would* result in a compatibility PAC,
 * we return the seed info as of old.  If it would not,
 * we return a PAC filled with NIL uuids.  Finally,
 * if the status of the eptgt request is
 * sec_s_v1_1_no_support, we propagate that error back
 * to the client.
 *
 * All of this entails some behavioral changes in the
 * sec_login_inq_net_info() call on 1.1 clients. Under
 * some circumstances, the call may fail completely.
 * Luckily, these circumstances usually occur in 
 * 1.1 intermediary servers, and they aren't likely to
 * be using sec_login_inquire_net_info().  Users are not
 * likely to be using klist on such identities, either.
 */
PUBLIC void sec_login_pvt_inq_pac(
    sec_login_context_t  *lcp,
    sec_id_pac_t         *pac,
    error_status_t       *stp
)
{
    krb5_creds           kcreds, creds;
    boolean32            copy_name_from_lc = false;
    error_status_t       xst;
    sec_bytes_t	         epac_chain;

    memset(&kcreds, 0, sizeof(kcreds));
    memset(&creds, 0, sizeof(creds));
    memset(&epac_chain, 0, sizeof(epac_chain));

    /*
     * Look up the home cell PTGT in the credential cache.
     * Call the priv server first to be sure there is one.
     */
    sec_login_util_get_krb_ptgt((sec_login_handle_t) lcp, lcp->cell,
	rpc_c_authn_dce_secret, rpc_c_authz_dce, &epac_chain, stp);

    if (GOOD_STATUS(stp)) {
        if (epac_chain.bytes != NULL
            && epac_chain.num_bytes != 0) {
            idl_pkl_t *ppac;
            unsigned32 ppac_len;
            /*
             * Try converting the epac chain to a PAC. We do this so
             * we can get the group uuids, etc so that utilities
             * like klist can display them.
             *
             * NOTE!: priv_krb_util_gen_v1_pac *could* return
             * a sec_id_pac_t instead of an idl_pkl_t but it would
             * require some code rejuggling. So there is a needless
             * pickling and unpickling performed.
             */
            priv_krb_util_gen_v1_pac(malloc_shim, free, &epac_chain, 
                                     &ppac, &ppac_len, stp);
            if (GOOD_STATUS(stp) && ppac) {
                sec_id_pac_t_unpickle(ppac, (void *(*)(unsigned32)) malloc,
                                      pac, stp);
                free(ppac);
            } else {
                SET_STATUS(stp, sec_s_no_pac);
            }
        } else {
            /* 
             * No v1.1 creds. This means our cell must be running pre 1.1
             * security servers, so retrieve the PAC from the PTGT authdata in
             * the creds file.
             */
	    sec_krb_ptgs_parsed_name((char *) lcp->cell, &kcreds.client, stp);

            if (GOOD_STATUS(stp)) {
                /* now the Ticket-Granting-Ticket server principal */
                sec_krb_tgs_parsed_name( (char *) lcp->cell, &kcreds.server, stp);
            }

            if (GOOD_STATUS(stp)) {
                *stp = krb5_cc_retrieve_cred(KRB_INFO(lcp).cache,
                                             0 | KRB5_TC_MATCH_TIMES,
                                             &kcreds, &creds);
            }

            if (GOOD_STATUS(stp)) {
                idl_pkl_t *ppac;

                priv_krb_util_authdata_to_pac(creds.authdata, &ppac, stp);
                if (GOOD_STATUS(stp) && ppac) {
                    sec_id_pac_t_unpickle(ppac, (void *(*)(unsigned32)) malloc,
                                          pac, stp);
                    free(ppac);
                } else {
                    SET_STATUS(stp, sec_s_no_pac);
                }
            }
        }
    }

    if (GOOD_STATUS(stp)) {
        if (((DLG_REQ_TYPE(lcp) == sec_id_deleg_type_none)
             && (COMPAT_MODE(lcp) != sec_id_compat_mode_none)) 
	|| ((DLG_REQ_TYPE(lcp) == sec_id_deleg_type_traced)
	   && (COMPAT_MODE(lcp) == sec_id_compat_mode_caller))) 
	{
	    copy_name_from_lc = true;
	}
    }
    
    if (BAD_STATUS(stp) && !STATUS_EQUAL(stp, sec_s_v1_1_no_support)) {

	if (((DLG_REQ_TYPE(lcp) == sec_id_deleg_type_none)
           && (COMPAT_MODE(lcp) != sec_id_compat_mode_none)) 
        || ((DLG_REQ_TYPE(lcp) == sec_id_deleg_type_traced)
           && (COMPAT_MODE(lcp) == sec_id_compat_mode_caller))) 
        {
	    CLEAR_STATUS(stp);
	    sec_id_pac_util_copy(pac, &NET_CREDS(lcp));
	    copy_name_from_lc = true;
	} else if (COMPAT_MODE(lcp) == sec_id_compat_mode_none) {
	    /* compat mode none means no PAC, which means NIL uuids */
	    CLEAR_STATUS(stp);
	    memset(pac, 0, sizeof(*pac));
	    uuid_create_nil(&pac->realm.uuid, &xst);
	    uuid_create_nil(&pac->principal.uuid, &xst);
	    uuid_create_nil(&pac->group.uuid, &xst);
	}
    }

    if (GOOD_STATUS(stp)) {
	if (copy_name_from_lc) {
	    if (!(pac->realm.name || uuid_is_nil(&pac->realm.uuid, &xst))) {
		u_dynstrcpy(&pac->realm.name, lcp->cell);
	    }
	    if (!(pac->principal.name ||  uuid_is_nil(&pac->principal.uuid, &xst))) {
		u_dynstrcpy(&pac->principal.name, lcp->principal);
	    }
	}

	/* 
	 * In some instances, the pre-1.1 PAC may be full of NIL
	 * uuids.  In that case we stick the "<NIL>" string in the
	 * name field to help clients avoid fruitless attempts to 
	 * convert NIL uuids to names.
	 */
	if (!pac->realm.name && uuid_is_nil(&pac->realm.uuid, &xst)) {
	    u_dynstrcpy(&pac->realm.name, "<NIL>");
	}
	if (!pac->principal.name && uuid_is_nil(&pac->principal.uuid, &xst)) {
	    u_dynstrcpy(&pac->principal.name, "<NIL>");
	}
	if (!pac->group.name && uuid_is_nil(&pac->group.uuid, &xst)) {
	    u_dynstrcpy(&pac->group.name, "<NIL>");
	}
    }

    if (kcreds.client != NULL) {
        krb5_free_cred_contents(&kcreds);
    }

    if (creds.client != NULL) {
        krb5_free_cred_contents(&creds);
    }

    if (epac_chain.bytes)
        free(epac_chain.bytes);
}

/*
 * Update our idea of when the identity has expired.
 * Do this in a way so that repeated invocations of this when it *has*
 * expired don't take too long.
 */

PRIVATE void update_identity_exp (lcp, stp)
    sec_login_context_t *lcp;
    error_status_t *stp;
{
    time_t init_exp = lcp->identity->policy.identity_exp;
    struct stat stbuf;
    char *cachename;
    int ret;

    if(lcp->krb_info.cache == NULL)
    {
        *stp = sec_login_s_handle_invalid;
        return;
    }

    cachename = krb5_cc_get_name (lcp->krb_info.cache);
    ret = stat (cachename, &stbuf);
    if (ret < 0)
    {
	*stp = sec_login_s_handle_invalid;
	return;
    }
    /*
     * has the file been touched recently?
     */
    if (stbuf.st_mtime > lcp->modtimes.cachefile)
    {
	krb5_creds kcred, rcred;
	krb5_principal tgt = NULL;
	krb5_principal me = NULL;
	
	lcp->modtimes.cachefile = stbuf.st_mtime;

	memset (&kcred, 0, sizeof(kcred));
	memset (&rcred, 0, sizeof(rcred));
	
	ret = krb5_cc_get_principal (lcp->krb_info.cache, &kcred.client);
	if (ret != 0)
	    goto out;
	
	ret = krb5_tgtname (
	    krb5_princ_realm(kcred.client),
	    krb5_princ_realm(kcred.client),
	    &kcred.server
	);
	
	if (ret != 0)
	    goto out;
	
	ret = krb5_cc_retrieve_cred (lcp->krb_info.cache, KRB5_TC_MATCH_TIMES,
	    &kcred, &rcred);
	
	if (ret != 0)
	    goto out;
	
	lcp->identity->policy.identity_exp = rcred.times.endtime;
	if (init_exp != rcred.times.endtime)
	{
	    /*
	     * update the on-disk context to save proc's started after
	     * us some time.
	     */
	    write_context(lcp, true, stp);
	}
    out:	
	krb5_free_cred_contents(&kcred);
	krb5_free_cred_contents(&rcred);
    }
}



PUBLIC void sec_login_pvt_check_exp(lcp, stp)
    sec_login_context_t *lcp;
    error_status_t *stp;
{
    time_t now;

    time (&now);
    CLEAR_STATUS(stp);

    if (lcp->identity->policy.identity_exp < now)
    {
	update_identity_exp(lcp, stp);
	time (&now);
	if (GOOD_STATUS(stp) && (lcp->identity->policy.identity_exp < now))
	{
	    SET_STATUS(stp, rpc_s_auth_tkt_expired);
	}
    }
}


PUBLIC  void  sec_login_pvt_inq_identity_expiration
  (
    sec_login_context_t  *lcp,
    unsigned32           *exp_time,
    error_status_t       *stp
  )
{
    krb5_creds           kcreds, creds;

    memset(&kcreds, 0, sizeof(kcreds));
    memset(&creds, 0, sizeof(creds));
    *exp_time = 0;
    CLEAR_STATUS(stp);

    sec_login_pvt_check_exp (lcp, stp);
    if (STATUS_EQUAL(stp, rpc_s_auth_tkt_expired)) {
    	CLEAR_STATUS(stp);
    }
    *exp_time = lcp->identity->policy.identity_exp;
}


PUBLIC  void  sec_login_pvt_get_local_pwent
  (
    sec_login_context_t  *lcp,
    error_status_t       *stp
  )
{
    struct sec_passwd    *pwent;
    signed32             *groups;
    unsigned16           num_groups;

    get_local_unix_info(lcp->cell, lcp->principal, &pwent, &num_groups, &groups, stp);

    if (GOOD_STATUS(stp)) {
         free_passwd_context(&lcp->identity->unix_info.pwent);
         lcp->identity->unix_info.pwent = pwent;
    } else {
         free_passwd_context(&pwent);
    }
}


PUBLIC  void  sec_login_pvt_synch_first
  (
    sec_login_context_t  *lcp,
    error_status_t       *stp
  )
{

   /*
    * create a data file for this context. This is the final piece
    * of the puzzle that enables reconstruction of the default
    * machine context.  Until the data file exists, all attempts
    * to reconstruct the machine context will produce a
    * sec_login_s_no_current_context error
    */
   write_context(lcp, false, stp);
}


/* s e c _ l o g i n _ p v t _ u n i q u e _ c r e d _ v a l
 *
 * Generate a semi-unique cred value, to use in searching for unused
 * credential cache names.  Note: this utilizes the same technique as
 * sec_login_pvt_link_ccache, which starts from a pag value to find an
 * unused name to use as a link to the ccache, but we don't have a pag
 * value at the point where this is called, so we're doing it ourselves.
 */
PRIVATE void sec_login_pvt_unique_cred_val ( void )
{
    pthread_mutex_init(&sec_login_ucv__mutex, pthread_mutexattr_default);
    unique_cred_val = getpid();
    unique_cred_val = (unique_cred_val << 16) + ((time(NULL) & 0xFF) <<8);
    sec_login_ucv__inited = true;
}


/* s e c _ l o g i n _ p v t _ c c a c h e _ g e n n a m e
 *
 * Effects:
 * Creates a new file cred cache whose name is guaranteed to be
 * unique.  The name begins with the string SEC_LOGIN_CCACHE TKT_ROOT
 * (from sec_login_file_loc.h).
 *
 * Returns:
 * The filled in krb5_ccache id.
 *
 */
PRIVATE  void  sec_login_pvt_ccache_genname
  (
    char                 **name,
    error_status_t       *stp
  )
{
    int                 fd;
    char                filename[SEC_LOGIN_TKT_NAME_MAX_LENGTH];
    static pthread_once_t   ucv_pthread_init_block = pthread_once_init;

    CLEAR_STATUS(stp);
    if (!sec_login_ucv__inited) {
        pthread_once(&ucv_pthread_init_block, sec_login_pvt_unique_cred_val);
    }

    do {
        PSEM(sec_login_ucv__mutex) {
            unique_cred_val++;
        } VSEM;

        sprintf(filename,SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT,unique_cred_val);
	
        /* Make sure the file name is reserved */
        fd = open(filename, O_CREAT | O_EXCL | O_WRONLY, 0);
    } while(fd == -1 && errno == EEXIST);
    if (fd == -1) {
        SET_STATUS(stp, KRB5_CC_IO);
        return;
    } else {
        /* Ignore user's umask, set mode = 0600 */
        fchmod(fd, S_IREAD | S_IWRITE);
        /* Close before continuing */
        (void) close(fd);
    }

    *name = malloc(1+strlen(filename));
    if (*name == NULL) {
        SET_STATUS(stp, sec_login_s_no_memory);
        unlink(filename);
    } else {
        strcpy(*name, filename);
    }

    return;
}


/* s e c _ l o g i n _ p v t _ c c a c h e _ g e n n e w
 *
 * Effects:
 * Creates a new file cred cache whose name is guaranteed to be
 * unique.  The name begins with the string SEC_LOGIN_CCACHE TKT_ROOT
 * (from sec_login_file_loc.h).
 * The cache is opened and closed to avoid a race condition with multi-threaded
 * use of mktemp.
 * 
 * Returns:
 * The filled in krb5_ccache id.
 *
 */
PUBLIC  void  sec_login_pvt_ccache_gennew
  (
    krb5_ccache          *id,
    error_status_t       *stp
  )
{
    char               *filename;

    sec_login_pvt_ccache_genname(&filename, stp);
    if (GOOD_STATUS(stp)) {
        /*
         * allocate a cache handle for the filename.
         */
        *stp = (*krb5_cc_dfl_ops->resolve) (id, filename);
        free(filename);
    }

    return;
}

/* s e c _ l o g i n _ p v t _ c c a c h e _ s w a p
 *
 * Swap the newly refreshed data in the refresh context into the
 * existing context.  The refresh context will be left referring to
 * an empty ccache file.
 *
 * XXXXX NOTE - THIS IS A VIOLATION OF THE ABSTRACTION:
 *  we should not be manipulating the credential cache files directly,
 *  but locking difficulties in the underlying krb5 code make this the
 *  expedient solution for DCE 1.0.1.  This code should be revisited
 *  for later releases.
 * XXXXX
 */
PRIVATE void sec_login_pvt_ccache_swap
  (
    sec_login_context_t   *lcp,  /* [in, out] */
    sec_login_context_t   *rcp,  /* [in] */
    error_status_t        *stp
  )
{
    char    *filename;
    char    *oldname;
    char    *newname;
    int     fd_temp;
    int     fd_new;
    int     len;
    char    buf[256];

    sec_login_pvt_ccache_genname(&filename, stp);
    if (GOOD_STATUS(stp)) {
        oldname = krb5_cc_get_name(lcp->krb_info.cache);
        newname = krb5_cc_get_name(rcp->krb_info.cache);
        if (oldname == NULL || newname == NULL) {
            if (oldname == NULL) 
                DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                    "krb5_cc_get_name() returned NULL for lcp->krb_info.ccache\n"));

            if (newname == NULL) 
                DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                    "krb5_cc_get_name() returned NULL for rcp->krb_info.ccache\n"));

            SET_STATUS(stp, sec_login_s_internal_error);
            return;
        }

        /*
         * Replace old ccache file with the new one, and provide
         * a copy where the refresh cache thinks it has a cache
         * file.
         */
        fd_temp = open(filename, O_RDWR, 0);
        if (fd_temp == -1) {
            free(filename);
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                          "Oldfile ( %s ) open failed. errno = %d\n",
                          filename, errno));
            SET_STATUS(stp, sec_login_s_internal_error);
            return;
        }
        fd_new = open(newname, O_RDONLY, 0);
        if (fd_new == -1) {
            free(filename);
            close(fd_temp);
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                          "Newfile ( %s ) open failed. errno = %d\n",
                          newname, errno));
            SET_STATUS(stp, sec_login_s_internal_error);
            return;
        }

        do {
            len = read(fd_new, buf, sizeof(buf));
            if (len != -1) {
                if (write(fd_temp, buf, len) != len) {
                    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                        "Write failed. Requested no of bytes ( %d ) not written. errno = %d\n",
                        sizeof(buf), errno));
                    SET_STATUS(stp, sec_login_s_internal_error);
                }
            }
        } while (len == sizeof(buf));

        close(fd_temp);
        close(fd_new);

        rename(newname, oldname);
        rename(filename, newname);
        free(filename);
    }
}

PUBLIC void sec_login_pvt_get_namecache (lcp, cache, filename)
    sec_login_context_t *lcp;
    void **cache;
    char **filename;
{
    unsigned char buf[1024];
    unsigned32 len_used, len_needed;
    error_status_t st;
    char *ncname = NULL, *cname;
    void *cachep = NULL;

    if ((lcp == NULL) && (sec_login_pvt_default_context.valid))
        lcp = sec_login_pvt_get_default_context(&st);

    if (lcp != NULL) {
        /*
         * initialize the context name if it isn't null.
         *
         * !!! this should be broken out into a separate function.
         */
        cname = lcp->context_name;

        if (cname == NULL) {
            sec_login_export_context(lcp, sizeof(buf), buf, &len_used,
                                     &len_needed, &st);
            if (st == error_status_ok) {
                cname = malloc (len_used);
                if (cname != NULL) {
                    memcpy (cname, buf, len_used);
                    lcp->context_name = cname;
                    lcp->context_name_len = len_used;
                }
            }
        }
        /*
         * initialize the namecache name from the context name.
         */
        ncname = lcp->namecache_name;

        if ((ncname == NULL) && (cname != NULL)) {
            char *ptr = strchr(lcp->context_name, ':');

            if (ptr) {
                int len = lcp->context_name_len - (ptr - cname);

                ncname = malloc (len + 4);
                memcpy(ncname, ptr+1, len-1);
                strcat (ncname, NAMECACHE_SUFFIX);
                lcp->namecache_name = ncname;
            }
        }
        cachep = lcp->namecache;
    }

    *cache = cachep;
    *filename = ncname;
}


PUBLIC void sec_login_pvt_set_namecache (lcp, cache)
    sec_login_context_t *lcp;
    void *cache;
{
    error_status_t st;
    if ((lcp == NULL) && (sec_login_pvt_default_context.valid))
        lcp = sec_login_pvt_get_default_context(&st);
    if (lcp != NULL)
        lcp->namecache = cache;
}

/*
 * Open the namecache file.
 *
 * On good status, returns an open file descriptor referencing
 * the namecache file. The caller is responsible for closing
 * this file descriptor.
 */
PUBLIC void sec_login_pvt_open_namecache(
    sec_login_context_t	*lcp,
    char                *ncname,
    int 		oflag,
    int                 *cache_fd,
    error_status_t      *stp
)
{
    int                 outfile;
    int			mode = (lcp != NULL) ? 0600 : 0666;
    CLEAR_STATUS(stp);

    outfile = open(ncname, oflag, mode);
    if (outfile != -1) {
	if (mode == 0666)
	    (void) fchmod (outfile, mode);
        /*
         * Only chown file if we are certified, there is a UNIX password
         * entry for this account, we are root, and this is the first
         * time this file has been opened.
         */ 
        if (lcp != NULL
	    && INFO_STATE(lcp) == info_certified 
	    && UNIX_CREDS(lcp).pwent != NULL
	    && geteuid() == 0
            && KRB_INFO(lcp).num_caches == 0) {
            if (fchown(outfile,
                       (lcp->v1_1_info.pag == SEC_LOGIN_NO_PAG) ?
                       0 : UNIX_CREDS(lcp).pwent->pw_uid, -1) == -1) {
                /* if unable to set ownership, close and delete file */
                if (errno != EPERM) {
                    (void) close(outfile);
                    (void) unlink(ncname);
                    dce_svc_printf(DCE_SVC(sec_svc_handle, "%s%s"),
                                   sec_s_login, svc_c_sev_error | svc_c_action_none,
				   sec_sys_errno_text, "fchown",
                       		   strerror(errno));
        	    SET_STATUS(stp, sec_login_s_internal_error);
                    return;
		}
            }
        }
    } else {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s%s"),
                       sec_s_login, svc_c_sev_error | svc_c_action_none,
       		       sec_sys_errno_text, "open",
		       strerror(errno));
        SET_STATUS(stp, sec_login_s_internal_error);
	return;
    }

    *cache_fd = outfile;
}

PUBLIC void  sec_login_pvt_override_all
  (
    sec_login_context_t     *lcp,
    error_status_t          *stp
  )
{
    sec_rgy_unix_passwd_buf_t   pw_passwd;
    sec_rgy_name_t              pw_gecos;
    sec_rgy_name_t              pw_dir;
    sec_rgy_name_t              pw_shell;
    signed32                    pw_uid, pw_gid;
    sec_override_fields_t       overridden;
    char                        *cellname;

    CLEAR_STATUS(stp);
    pw_gecos[0] = '\0';
    overridden = sec_override_none;
    lcp->auth_src = sec_login_auth_src_overridden;
    pw_uid = pw_gid = sec_rgy_uxid_unknown;
    override_get_login_info((unsigned_char_p_t) lcp->principal,
                            &pw_uid, &pw_gid, pw_passwd, pw_gecos,
                            pw_dir, pw_shell, &overridden, stp);

    /* If no override found, then something may be corrupt
     * or changing out from under us, so don't continue.
     */
    if (STATUS_EQUAL(stp, sec_login_s_ovrd_ent_not_found)||
        STATUS_EQUAL(stp, sec_login_s_no_override_info) ) {
        SET_STATUS(stp, sec_login_s_override_failure);
    }
    /* Caller wanted all fields overridden.  If they're not
     * all overridden, then pass back an error.
     */
    if (GOOD_STATUS(stp) && ( (overridden | sec_override_pw_gecos) != sec_override_pw_all ))
    {
        SET_STATUS(stp, sec_login_s_incomplete_ovrd_ent);
    }
    if (BAD_STATUS(stp)) {
        return;
    }

    /* Can't get cellname from name in this implementation
     * so assume it's a user relative to this cell
     */
    dce_cf_get_cell_name (&cellname, stp);
    if (GOOD_STATUS(stp)) {
        u_strcpy(lcp->cell, cellname);
        free(cellname);
        UNIX_CREDS(lcp).num_groups  = 0;
        POLICY_INFO(lcp).acct_exp       = 0;
        POLICY_INFO(lcp).passwd_exp     = 0;
        POLICY_INFO(lcp).identity_exp   = 0;
        POLICY_INFO(lcp).pflags         = (policy_passwd_valid | policy_acct_valid);

        UNIX_CREDS(lcp).pwent =
	    (struct sec_passwd *) malloc(sizeof(struct sec_passwd));
        if (UNIX_CREDS(lcp).pwent != NULL) {
            alloc_pwent_string_buffers(UNIX_CREDS(lcp).pwent, stp);

            u_strcpy(UNIX_CREDS(lcp).pwent->pw_name, lcp->principal);
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_passwd, pw_passwd);
            UNIX_CREDS(lcp).pwent->pw_class[0] = '\0';
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_gecos,  pw_gecos);
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_dir,    pw_dir);
            u_strcpy(UNIX_CREDS(lcp).pwent->pw_shell,  pw_shell);
            UNIX_CREDS(lcp).pwent->pw_uid       = pw_uid;
            UNIX_CREDS(lcp).pwent->pw_gid       = pw_gid;
            UNIX_CREDS(lcp).pwent->pw_change    = 0;
            UNIX_CREDS(lcp).pwent->pw_expire    = 0;
            INFO_STATE(lcp) = info_unauth;
            lcp->state = semisetup;
        } else {
            SET_STATUS(stp, sec_login_s_no_memory);
        }
    }
}


/* sec_login_pvt_create_raw_context
 *
 * Create a "raw" context using info supplied by the caller, without any verification.
 * The context created has no unix info, or policy data.  It's up to the caller
 * to ensure that the context will perform robustly in all circumstances (i.e.,
 * the caller had better understand, in detail, the implementation of login
 * handles and their use in authenticated RPCs).
 *
 * This routine is provided to support context creation for the user-to-user
 * authentication in master-slave security server replication.
 */
PUBLIC  void  sec_login_pvt_create_raw_context
  (
    char                  *cell,
    char                  *principal,
    sec_id_pac_t          *net_creds,
    krb5_ccache           ccache,
    sec_login_flags_t     flags,
    sec_login_auth_src_t  auth_src,
    context_state_t       state,
    login_info_state_t    info_state,
    boolean32             local,
    policy_info_t         *policy_info,
    uuid_t                *generation_id,
    sec_login_handle_t    *lcap,
    error_status_t        *status
  )
{
    sec_login_context_t  *lcp;

    *lcap = NULL;
    lcp = (sec_login_context_t *)malloc(sizeof(sec_login_context_t));
    if (lcp == NULL) {
        MEM_FAILURE(status);
        return;
    } else {
        CLEAR_STATUS(status);
    }
    memset(lcp, 0, sizeof(sec_login_context_t));

    /* Initialize db mutex */
    init_db_mutex(&lcp->db_mutex, status);
    if (BAD_STATUS(status)) {
	return;
    }

    allocate_identity(&lcp->identity, status);
    if (BAD_STATUS(status)) {
        free( (char *) lcp);
        return;
    }
    sec_login_pvt_init_v1_1_info(NULL, NULL, &lcp->v1_1_info);

    memset(&lcp->krb_info, 0, sizeof(krb_info_t));
    lcp->krb_info.cache = ccache;

    u_strncpy(lcp->cell, cell, sec_rgy_name_t_size);
    lcp->cell[sec_rgy_name_max_len] = '\0';

    u_strncpy(lcp->principal, principal, sec_rgy_name_t_size);
    lcp->principal[sec_rgy_name_max_len] = '\0';

    sec_id_pac_util_copy(&lcp->identity->net_creds, net_creds);

    lcp->identity->state = info_state;
    lcp->state = state;
    lcp->flags = flags;
    lcp->local = local;
    lcp->auth_src = auth_src;
    lcp->refcount = 1;
    if (policy_info != NULL) {
        POLICY_INFO(lcp) = *policy_info;
    }
    lcp->context_name_len = -1;
    lcp->context_name = NULL;
    lcp->generation_id = *generation_id;
    lcp->magic = lcp;

    *lcap = lcp;
}

/* FAKE-EPAC
 *
 * These routines provide support for the temporary mechanism
 * of transmitting EPACS in the authorization data field of
 * keberos tickets.  This routine is referenced from within the
 * FAKE-EPAC code in sec_authn.c. as well, so it's PUBLIC.
 * Eventually, we probably want a sec_cred_util module
 * for routines like this.
 */
PUBLIC void pac_to_epac
  (
    sec_id_pac_t                *pac,
    sec_id_epac_t               *epac,
    unsigned32                  num_attr_keys,
    sec_attr_t                  *attr_keys,
    sec_id_delegation_type_t    dlg_type,
    sec_id_compatibility_mode_t compat_mode,
    error_status_t              *stp
  )
{
    unsigned32          num_left;
    error_status_t      st;
    sec_rgy_handle_t    rgy_h = sec_rgy_default_handle;
    sec_id_epac_data_t  ed;
    sec_id_pa_t         *pa = &ed.pa;
    sec_rgy_name_t      pname;
    int                 i;

    /* convert V1 PAC to EPAC data */
    memset(&ed, 0, sizeof(sec_id_epac_data_t));
    epac->seals = (sec_id_seal_set_t *) NULL;

    ed.deleg_type = dlg_type;
    ed.compat_mode = compat_mode;
    pa->realm = pac->realm;
    pa->principal = pac->principal;
    pa->group           = pac->group;
    pa->num_groups      = pac->num_groups;
    pa->groups          = pac->groups;
    /* Just copy foreign groups into groupsets w/out compression for now */
    pa->num_foreign_groupsets = pac->num_foreign_groups;
    if (pa->foreign_groupsets != 0) {
        pa->foreign_groupsets = (sec_id_foreign_groupset_t *)malloc(
                pa->num_foreign_groupsets * sizeof(sec_id_foreign_groupset_t));
        for (i = 0; i < pac->num_foreign_groups; i++) {
            pa->foreign_groupsets[i].num_groups = 1;
            pa->foreign_groupsets[i].realm = pac->foreign_groups[i].realm;
            pa->foreign_groupsets[i].groups = &(pac->foreign_groups[i].id);
        }
    }
    if (num_attr_keys >0) {
	ed.num_attrs = num_attr_keys;
	ed.attrs = attr_keys;
    } else {
        ed.num_attrs = 0;
        ed.attrs = NULL;
    }

    if (GOOD_STATUS(stp)) {
        /* pickle the EPAC data */
        sec_id_epac_data_encode((void *(*)(unsigned32))malloc, free, &ed,
				&epac->pickled_epac_data.num_bytes,
                                &epac->pickled_epac_data.bytes, stp);
    }
}

/*
 * Converts a V1 PAC to an EPAC CHAIN in sec_bytes format.
 */
PUBLIC void pac_to_epac_chain_bytes
  (
    sec_id_pac_t                *pac,		/* [in]  */
    sec_bytes_t                 *epac_chain,	/* [out] */
    unsigned32                  num_attr_keys,  /* [in]  */
    sec_attr_t                  *attr_keys,	/* [in]  */
    sec_id_delegation_type_t    dlg_type,	/* [in]  */
    sec_id_compatibility_mode_t compat_mode,	/* [in]  */
    error_status_t              *stp		/* [out] */
  )
{
    sec_id_epac_set_t   epac_set;
    sec_id_epac_t	epac;
    sec_bytes_t		tmp_epac_chain;

    CLEAR_STATUS(stp);

    pac_to_epac(pac, &epac, num_attr_keys, attr_keys, dlg_type,
        compat_mode, stp);

    if (GOOD_STATUS(stp)) {
	epac_set.num_epacs = 1;
	epac_set.epacs = &epac;

        sec_id_epac_set_encode(malloc_shim,
            free, &epac_set, &tmp_epac_chain.num_bytes,
            &tmp_epac_chain.bytes, stp);

	sec_encode_epac_free(free, &epac);
    }

    if (GOOD_STATUS(stp)) {
	*epac_chain = tmp_epac_chain;
    }
}

/* Get the epac chain (of length 1) out of the existing login context,
 * decode it down to epac data, generate new epac data from it and the
 * newly specified attributes, then generate a new epac chain (of length 1)
 * from the new epac data, storing it in the new login context.
 */
PRIVATE void sec_login_pvt_insert_attrs(
    sec_login_context_t         *lcp,
    unsigned32                  num_attrs,
    sec_attr_t                  *attrs,
    sec_bytes_t                 *epac_chain,
    error_status_t              *stp
    )
{
    sec_id_epac_t       epac;
    sec_id_epac_set_t   epac_set, nepac_set;
    sec_id_epac_data_t  epac_data, nepac_data;

    /* Decode the original EPAC chain pickle */
    sec_id_epac_set_decode((void *(*)(unsigned32))malloc, free,
        EPAC_CHAIN(lcp).num_bytes, EPAC_CHAIN(lcp).bytes, &epac_set, stp);
    if (BAD_STATUS(stp)) {
	return;
    }

    /* Shouldn't have been able to get this far if already a dlg chain */
    if (epac_set.num_epacs > 1) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                      "epac_set.num_epacs ( %d ) is greater than 1\n", 
                      epac_set.num_epacs));
        SET_STATUS(stp, sec_login_s_internal_error);
	sec_encode_epac_set_free(free, &epac_set);
	return;
    }

    /* Decode the pickled EPAC data */
    sec_id_epac_data_decode((void *(*)(unsigned32))malloc, free, 
			    epac_set.epacs[0].pickled_epac_data.num_bytes,
			    epac_set.epacs[0].pickled_epac_data.bytes,
			    &epac_data, stp);
    if (BAD_STATUS(stp)) {
	sec_encode_epac_set_free(free, &epac_set);
	return;
    }

    /* Copy fixed length portion of epac data to new epac data structure */
    nepac_data = epac_data;

    /* Set the new parameters */
    nepac_data.num_attrs = num_attrs;
    nepac_data.attrs = attrs;

    /* Generate the new epac set */
    sec_id_epac_data_encode((void *(*)(unsigned32))malloc, free, &nepac_data,
			    &epac.pickled_epac_data.num_bytes,
			    &epac.pickled_epac_data.bytes, stp);
    /* Free the old epac set */
    sec_encode_epac_set_free(free, &epac_set);
    if (BAD_STATUS(stp)) {
	sec_encode_epac_data_free(free, &epac_data);
	return;
    }
    epac.seals = NULL;
    epac_set.num_epacs = 1;
    epac_set.epacs = &epac;
    sec_id_epac_set_encode((void *(*)(unsigned32))malloc, free, &epac_set,
        &epac_chain->num_bytes,	&epac_chain->bytes, stp);

    sec_encode_epac_data_free(free, &epac_data);
    sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);
}


/* Get the epac chain (of length 1) out of the existing login context,
 * decode it down to epac data, generate new epac data from it and the
 * newly specified types and restrictions, then generate a new epac chain
 * (of length 1) from the new epac data, storing it in the new login context.
 */
PRIVATE void sec_login_pvt_initiate_chain(
    sec_login_context_t         *lcp,
    sec_id_delegation_type_t    dlg_type,
    sec_id_restriction_set_t    *dlg_rstrs,
    sec_id_restriction_set_t    *tgt_rstrs,
    sec_id_opt_req_t            *opt_rstrs,
    sec_id_opt_req_t            *req_rstrs,
    sec_id_compatibility_mode_t compat_mode,
    sec_bytes_t                 *epac_chain,
    error_status_t              *stp
    )
{
    sec_id_epac_t       epac;
    sec_id_epac_set_t   epac_set;
    sec_id_epac_data_t  epac_data, nepac_data;

    /* Decode the original EPAC chain pickle */
    sec_id_epac_set_decode((void *(*)(unsigned32))malloc, free,
        EPAC_CHAIN(lcp).num_bytes, EPAC_CHAIN(lcp).bytes, &epac_set, stp);
    if (BAD_STATUS(stp)) {
	return;
    }

    /* Shouldn't have been able to get this far if already a dlg chain */
    if (epac_set.num_epacs > 1) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                      "epac_set.num_epacs ( %d ) is greater than 1\n", 
                      epac_set.num_epacs));
        SET_STATUS(stp, sec_login_s_internal_error);
	sec_encode_epac_set_free(free, &epac_set);
	return;
    }

    /* Decode the pickled EPAC data */
    sec_id_epac_data_decode((void *(*)(unsigned32))malloc, free, 
			    epac_set.epacs[0].pickled_epac_data.num_bytes,
			    epac_set.epacs[0].pickled_epac_data.bytes,
			    &epac_data, stp);
    if (BAD_STATUS(stp)) {
	sec_encode_epac_set_free(free, &epac_set);
	return;
    }

    /* Copy fixed length portion of epac data to new epac data structure */
    nepac_data = epac_data;

    /* Set the new parameters */
    nepac_data.deleg_type  = dlg_type;
    nepac_data.compat_mode = compat_mode;
    nepac_data.opt_restrictions = *opt_rstrs;
    nepac_data.req_restrictions = *req_rstrs;
    nepac_data.deleg_restrictions  = *dlg_rstrs;
    nepac_data.target_restrictions = *tgt_rstrs;

    /* Generate the new epac set */
    sec_id_epac_data_encode((void *(*)(unsigned32))malloc, free, &nepac_data,
			    &epac.pickled_epac_data.num_bytes,
			    &epac.pickled_epac_data.bytes, stp);
    /* Free the old epac set */
    sec_encode_epac_set_free(free, &epac_set);
    if (BAD_STATUS(stp)) {
	sec_encode_epac_data_free(free, &epac_data);
	return;
    }
    epac.seals = NULL;
    epac_set.num_epacs = 1;
    epac_set.epacs = &epac;
    sec_id_epac_set_encode((void *(*)(unsigned32))malloc, free, &epac_set,
        &epac_chain->num_bytes,	&epac_chain->bytes, stp);

    sec_encode_epac_data_free(free, &epac_data);
    sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);
}


PRIVATE sec_login_handle_t sec_login_pvt_copy_context(
    sec_login_context_t         *lcp,
    error_status_t              *stp
    )
{
    sec_login_handle_t  nlh = NULL;
    krb5_ccache         ccache = NULL;
    krb5_creds          creds, tgt_creds;
    uuid_t              generation_id;

    memset(&creds, 0, sizeof(creds));
    memset(&tgt_creds, 0, sizeof(tgt_creds));

    if (GOOD_STATUS(stp)) {
	uuid_create(&generation_id, stp);
    }

    /* copy the user's home cell TGT to the new context. */
    if (GOOD_STATUS(stp)) {
        /* first, the client principal - we can get it from the cred cache*/
        *stp = krb5_cc_get_principal(KRB_INFO(lcp).cache, &creds.client);
    }

    if (GOOD_STATUS(stp)) {
        /* now the Ticket-Granting-Ticket server principal */
        sec_krb_tgs_parsed_name( (char *) lcp->cell, &creds.server, stp);
    }

    if (GOOD_STATUS(stp)) {
        /*
         * Use an endtime of "now" in the TGT lookup, so we don't copy an
         * expired TGT
         */
        *stp =  krb5_timeofday(&creds.times.endtime);
    }

    /* Retrieve the TGT from the existing login context credential cache */
    if (GOOD_STATUS(stp)) {
        *stp = krb5_cc_retrieve_cred(KRB_INFO(lcp).cache,
                                     0 | KRB5_TC_MATCH_TIMES,
                                     &creds, &tgt_creds);
    }

    if (GOOD_STATUS(stp)) {
        sec_login_pvt_ccache_gennew(&ccache, stp);
    }

    if (GOOD_STATUS(stp)) {
        /* intialize the new cache with the client principal */
        *stp = krb5_cc_initialize(ccache, creds.client);
    }

    /* Store the TGT in the new ccache */
    if (GOOD_STATUS(stp)) {
        /* done with creds */
        krb5_free_cred_contents(&creds);
        *stp = krb5_cc_store_cred(ccache, &tgt_creds);
    }

    /* Create a new login context from pieces of the existing one */
    if (GOOD_STATUS(stp)) {
        /* done with tgt_creds */
        krb5_free_cred_contents(&tgt_creds);
        sec_login_pvt_create_raw_context((char *)lcp->cell,
            (char *)lcp->principal, &NET_CREDS(lcp), ccache, lcp->flags,
            lcp->auth_src, lcp->state, INFO_STATE(lcp), lcp->local,
            &POLICY_INFO(lcp), &generation_id, &nlh, stp);
    }

    if (GOOD_STATUS(stp)) {
	lookup_and_cache_enter((sec_login_context_p_t) nlh);
        return(nlh);
    }

    /* should only fallthru on bad status */
    krb5_free_cred_contents(&creds);
    krb5_free_cred_contents(&tgt_creds);
    if (ccache != NULL)
	krb5_cc_destroy(ccache);
    return NULL;
}


PUBLIC sec_login_handle_t sec_login_pvt_set_ext_attrs(
    sec_login_context_t         *lcp,
    unsigned32                  num_attrs,
    sec_attr_t                  attrs[],
    error_status_t              *stp
    )
{
    sec_login_handle_t nlh = NULL;
    
    /* Generate a new login context from the existing one */
    nlh = sec_login_pvt_copy_context(lcp, stp);

    /* Set up the epac chain in the new lc, with the specified arguments */
    if (GOOD_STATUS(stp)) {
        sec_login_context_t *nlcp = (sec_login_context_t *)nlh;
        sec_bytes_t epac_chain;
        sec_login_pvt_insert_attrs(lcp, num_attrs, attrs, &epac_chain, stp);
        if (GOOD_STATUS(stp)) {
            EPAC_CHAIN(nlcp) = epac_chain;
	    return(nlh);
        } else {
	    error_status_t xst;
	    sec_login_pvt_purge_context(nlcp, &xst);
	}
    }
    return NULL;
}


PUBLIC sec_login_handle_t sec_login_pvt_become_initiator(
    sec_login_context_t         *lcp,
    sec_id_delegation_type_t    dlg_type,
    sec_id_restriction_set_t    *dlg_rstrs,
    sec_id_restriction_set_t    *tgt_rstrs,
    sec_id_opt_req_t            *opt_rstrs,
    sec_id_opt_req_t            *req_rstrs,
    sec_id_compatibility_mode_t compat_mode,
    error_status_t              *stp
    )
{
    sec_login_handle_t nlh = NULL;
     sec_login_context_t         *nlcp;
   
    /* Generate a new login context from the existing one */
    nlh = sec_login_pvt_copy_context(lcp, stp);

    /* Set up the epac chain in the new lc, with the specified arguments */
    if (GOOD_STATUS(stp)) {
        sec_bytes_t epac_chain;
	sec_login_context_t *nlcp = (sec_login_context_t *)nlh;

        sec_login_pvt_initiate_chain(lcp, dlg_type, dlg_rstrs, tgt_rstrs,
            opt_rstrs, req_rstrs, compat_mode, &epac_chain, stp);
        if (GOOD_STATUS(stp)) {
            EPAC_CHAIN(nlcp) = epac_chain;
	    COMPAT_MODE(nlcp) = compat_mode;
	    DLG_REQ_TYPE(nlcp) = sec__login_c_dlg_req_init;
	    return(nlh);
	} else {
	    error_status_t xst;

	    /* 
	     * use a scratch status parameter so we don't overwrite the 
	     * actual failure status in the process of purging the context
	     */
	    sec_login_pvt_purge_context(nlcp, &xst);
	}
    }
    return NULL;
}

PUBLIC sec_login_handle_t sec_login_pvt_do_delegation_request (
    rpc_authz_cred_handle_t     caller, 
    sec_login_context_t         *lcp,
    sec__login_dlg_req_type_t   dlg_op_requested, 
    sec_id_delegation_type_t    dlg_type_permitted,
    sec_id_restriction_set_t    *dlg_rstrs,
    sec_id_restriction_set_t    *tgt_rstrs,
    sec_id_opt_req_t            *opt_rstrs,
    sec_id_opt_req_t            *req_rstrs,
    sec_id_compatibility_mode_t compat_mode,
    error_status_t              *stp
    )
{
    sec_bytes_t                 dlg_chain;
    sec_login_handle_t          nlh;
    sec_login_context_t         *nlcp = NULL;
    sec_dlg_token_set_t         dlg_token_set;

    nlh = NULL;

    /*
     * This routine can only be used to support
     * real delegation requests
     */
    assert(dlg_op_requested == sec__login_c_dlg_req_traced
           || dlg_op_requested == sec__login_c_dlg_req_imp);
           
    /* Generate a new login context from the existing one */
    nlh = sec_login_pvt_copy_context(lcp, stp);
    if (BAD_STATUS(stp)) {
	return nlh;
    }

    /* Copy the delegate's EPAC chain to the new login context */
    if (GOOD_STATUS(stp)) {
	nlcp = (sec_login_context_t *)nlh;
        EPAC_CHAIN(nlcp).num_bytes = EPAC_CHAIN(lcp).num_bytes;
        EPAC_CHAIN(nlcp).bytes = malloc(EPAC_CHAIN(lcp).num_bytes);
        memcpy(EPAC_CHAIN(nlcp).bytes, EPAC_CHAIN(lcp).bytes,
            EPAC_CHAIN(lcp).num_bytes);
    }

    if (GOOD_STATUS(stp)) {
	sec__cred_get_deleg_chain(&caller, &dlg_chain, stp);
    }
    if (GOOD_STATUS(stp)) {
	sec__cred_get_deleg_token(&caller, &dlg_token_set, stp);
    }

    /* 
     * Write the delegation request info into the login context so it's 
     * available to the priv server client
     */
    if (GOOD_STATUS(stp)) {
	copy_dlg_request_info(dlg_type_permitted, dlg_rstrs, 
			      tgt_rstrs, opt_rstrs, req_rstrs, 
			      &dlg_chain, &dlg_token_set, 
			      &DLG_REQ_INFO(nlcp), stp);
    }

    if (GOOD_STATUS(stp)) {
	DLG_REQ_TYPE(nlcp) = dlg_op_requested;

        /* 
         * compat mode is a little funny because a caller of
         * become_delegate can specify a compat mode, whereas a 
         * caller of become_impersonator can not.  Impersonators 
         * inherit compat mode from their caller.  So compat mode 
         * asignment requires a switch.
         */

	/*
         * Ask the privsvr to build the new delegation chain and
	 * construct a PTGT for it...
	 */	
	switch (dlg_op_requested) {
	case sec__login_c_dlg_req_traced:
            COMPAT_MODE(nlcp) = compat_mode;
	    sec_priv_become_delegate(nlcp, nlcp->cell,
                    rpc_c_authn_dce_secret, rpc_c_authz_dce,
		    dlg_type_permitted,
		    dlg_rstrs, tgt_rstrs, opt_rstrs, req_rstrs, 
		    compat_mode, &dlg_chain, &EPAC_CHAIN(nlcp),
		    &dlg_token_set, NULL, stp);
	    break;

	case sec__login_c_dlg_req_imp:
            COMPAT_MODE(nlcp) = COMPAT_MODE(lcp);
	    sec_priv_become_impersonator(nlcp, nlcp->cell,
                    rpc_c_authn_dce_secret, rpc_c_authz_dce,
		    dlg_type_permitted,
		    dlg_rstrs, tgt_rstrs, opt_rstrs, req_rstrs, 
		    &dlg_chain, &EPAC_CHAIN(nlcp),
		    &dlg_token_set, NULL, stp);
	    break;

	default:
	    /* 
	     * we won't ever get here because the assertion 
	     * at the beginning of this routine made sure that
             * the tag was one of the two cases in the switch
             */
	    break;
	}
    }

    
    /* Translate the not enabled error to the one that clients expect */
    if (STATUS_EQUAL(stp, sec_priv_s_deleg_not_enabled)) {
        SET_STATUS(stp, sec_login_s_deleg_not_enabled);
    }

    if (BAD_STATUS(stp)) {
	error_status_t xst;  /* so we won't overwrite the actual failure
                              * status in the process of purging the context
                              */
	/* 
         * Get rid of the login context resources.
         */
	sec_login_pvt_purge_context(nlcp, &xst);
    }

    return(nlh);
}


/* Get the epac chain (of length 1) out of the existing login context,
 * decode it down to epac data, set the delegation type to none,
 * then generate a new epac chain (of length 1) from the new epac data,
 * storing it in the new login context.
 */
PRIVATE void sec_login_pvt_dlg_type_none(
    sec_login_context_t         *lcp,
    sec_bytes_t                 *epac_chain,
    error_status_t              *stp
    )
{
    sec_id_epac_t       epac;
    sec_id_epac_set_t   epac_set;
    sec_id_epac_data_t  epac_data;

    /* Decode the original EPAC chain pickle */
    sec_id_epac_set_decode((void *(*)(unsigned32))malloc, free,
        EPAC_CHAIN(lcp).num_bytes, EPAC_CHAIN(lcp).bytes, &epac_set, stp);
    if (BAD_STATUS(stp)) {
	return;
    }

    /* Shouldn't have been able to get this far if more than one entry here */
    if (epac_set.num_epacs > 1) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                      "epac_set.num_epacs ( %d ) is greater than 1\n", 
                      epac_set.num_epacs));
        SET_STATUS(stp, sec_login_s_internal_error);
	sec_encode_epac_set_free(free, &epac_set);
	return;
    }

    /* Decode the pickled EPAC data */
    sec_id_epac_data_decode((void *(*)(unsigned32))malloc, free, 
        epac_set.epacs[0].pickled_epac_data.num_bytes,
        epac_set.epacs[0].pickled_epac_data.bytes, &epac_data, stp);
    if (BAD_STATUS(stp)) {
        sec_encode_epac_set_free(free, &epac_set);
        return;
    }

    /* Set delegation type to none */
    epac_data.deleg_type = sec_id_deleg_type_none;

    /* Generate the new epac set */
    sec_id_epac_data_encode((void *(*)(unsigned32))malloc, free, &epac_data, 
			    &epac.pickled_epac_data.num_bytes,
			    &epac.pickled_epac_data.bytes, stp);
    /* Free the old epac set */
    sec_encode_epac_set_free(free, &epac_set);
    if (BAD_STATUS(stp)) {
	sec_encode_epac_data_free(free, &epac_data);
	return;
    }
    epac.seals = NULL;
    epac_set.num_epacs = 1;
    epac_set.epacs = &epac;
    sec_id_epac_set_encode((void *(*)(unsigned32))malloc, free, &epac_set,
        &epac_chain->num_bytes,	&epac_chain->bytes, stp);

    sec_encode_epac_data_free(free, &epac_data);
    sec_encode_buffer_free(free, &epac.pickled_epac_data.bytes);
}


PUBLIC sec_login_handle_t sec_login_pvt_disable_dlg(
    sec_login_context_t         *lcp,
    error_status_t              *stp
    )
{
    sec_login_handle_t nlh = NULL;

    /* Generate a new login context from the existing one */
    nlh = sec_login_pvt_copy_context(lcp, stp);

    /* Set up the epac chain in the new lc, with the specified arguments */
    if (GOOD_STATUS(stp)) {
        sec_login_context_t *nlcp = (sec_login_context_t *)nlh;
        sec_bytes_t epac_chain;
        sec_login_pvt_dlg_type_none(lcp, &epac_chain, stp);
        if (GOOD_STATUS(stp)) {
            EPAC_CHAIN(nlcp) = epac_chain;
	    return(nlh);
        } else {
	    error_status_t xst;
 	    sec_login_pvt_purge_context(nlcp, &xst);
        }
    }
    return NULL;
}


PRIVATE void setup_krb_request_info
  (
    sec_login_context_t  *lcp,
    error_status_t       *stp
  )
{
    CLEAR_STATUS(stp);

    /* free existing request info, if any */
    free_krb_request_info(&KRB_REQUEST_INFO(lcp));
    allocate_krb_request_info(&KRB_REQUEST_INFO(lcp), stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * seed a krb5_cred struct with the following info:
     *
     *   creds.client           = fullname.pname@local_realm
     *   creds.server           = krbtgt/local_realm@local_realm
     *   creds.time.starttime   = 0  (defaults to arrival time at KDC unless the
     *                                KDC_OPT_POSTDATED flag is set, which we
     *                                don't do)
     *   creds.time.endtime     = 0  (defaulted to "infinity" and then restricted
     *                                according to policy by the KDC)
     *   creds.time.renew_till  = 0  (ignored by the KDC unless the
     *                                KDC_OPT_RENEWABLE flag is set, which we
     *                                don't do)
     *
     * We ignore the rest of the fields, hoping that the kerberos client
     * software will fill them in appropriately :-)
     */

    /* first, the client principal = we can get it from the credential cache */
    *stp = krb5_cc_get_principal(lcp->krb_info.cache,
                                 &KRB_REQUEST_INFO(lcp)->creds.client);
    if (BAD_STATUS(stp)) {
        free((char *) KRB_REQUEST_INFO(lcp));
        KRB_REQUEST_INFO(lcp) = NULL;
        return;
    }

    /* now the Ticket-Granting-Ticket server principal */
    sec_krb_tgs_parsed_name( (char *) lcp->cell,
                            &KRB_REQUEST_INFO(lcp)->creds.server, stp);
    if (BAD_STATUS(stp)) {
        krb5_free_principal(KRB_REQUEST_INFO(lcp)->creds.client);
        free_krb_request_info(&KRB_REQUEST_INFO(lcp));
        return;
    }

    *stp = krb5_os_localaddr(&KRB_REQUEST_INFO(lcp)->addrs);
    if (BAD_STATUS(stp)) {
        free_krb_request_info(&KRB_REQUEST_INFO(lcp));
        return;
    }

    KRB_REQUEST_INFO(lcp)->options = 0;
}


PRIVATE void setup_krb_tkt_request_options (
    sec_login_tkt_info_t    *tkt,
    tgt_request_data_t      **tgt,
    error_status_t          *stp
)
{
    krb5_timestamp          now;

    CLEAR_STATUS(stp);

    if (FLAG_SET(tkt->options, sec_login_tkt_allow_postdate)) {
        SET((*tgt)->options, KDC_OPT_ALLOW_POSTDATE);
    }

    if (FLAG_SET(tkt->options, sec_login_tkt_proxiable)) {
        SET((*tgt)->options, KDC_OPT_PROXIABLE);
    }

    if (FLAG_SET(tkt->options, sec_login_tkt_forwardable)) {
        SET((*tgt)->options, KDC_OPT_FORWARDABLE);
    }

    if (FLAG_SET(tkt->options, sec_login_tkt_renewable_ok)) {
        SET((*tgt)->options, KDC_OPT_RENEWABLE_OK);
    }

    *stp = krb5_timeofday(&now);
    if (BAD_STATUS(stp)) return;

    if (FLAG_SET(tkt->options, sec_login_tkt_renewable)) {
        SET((*tgt)->options, KDC_OPT_RENEWABLE);
        (*tgt)->creds.times.renew_till = now + tkt->renewable_lifetime;
    }

    if (FLAG_SET(tkt->options, sec_login_tkt_postdated)) {
        SET((*tgt)->options, KDC_OPT_POSTDATED);
        (*tgt)->creds.times.starttime = now + tkt->postdated_dormanttime;
    }

    if (FLAG_SET(tkt->options, sec_login_tkt_lifetime)) {
        (*tgt)->creds.times.endtime = now + tkt->lifetime;
    }
}

typedef krb5_error_code (*git_key_proc) PROTOTYPE((krb5_context,
						   const krb5_enctype,
						   krb5_data *,
						   krb5_const_pointer,
						   krb5_keyblock **));

/*
 * sec_login_pvt_get_and_process_krb_tgt
 *
 * This routine calls Kerberos routines to request a
 * TGT, decrypt it, and cache it in a credentials file.
 */
PUBLIC  void  sec_login_pvt_get_and_process_krb_tgt (
    sec_login_context_t  *lcp,
    sec_passwd_rec_t     *key,
    boolean32            refresh,
    error_status_t       *stp
)
{
    krb5_keyblock            	ukey;
    krb5_kdc_rep             	*as_reply = NULL;
    int                      	i;
    boolean32                	btm_flag_set;
    krb5_enctype		enctypes[2];
    krb5_preauthtype		patypes[5], *pa;
    volatile git_key_proc 	keyproc;
    void const * volatile 	keyseed;
    krb5_data 			pdata;

    CLEAR_STATUS(stp);

    /* allocate and seed the kerberos request information */
    setup_krb_request_info(lcp, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /* enable special ticket request options */
    if (lcp->tkt_request.options) {
        setup_krb_tkt_request_options(&(lcp)->tkt_request,
                                      &KRB_REQUEST_INFO(lcp),
                                      stp);
        if (BAD_STATUS(stp)) {
            free_krb_request_info(&KRB_REQUEST_INFO(lcp));
            return;
        }
    }

    /* set up kerberos key structure */
    switch (PASSWD_TYPE(key)) {
        default:
            SET_STATUS(stp, sec_login_s_unsupp_passwd_type);
            free_krb_request_info(&KRB_REQUEST_INFO(lcp));
	    return;

        case sec_passwd_des:
	    keyproc = skey_keyproc;
            ukey.keytype = KEYTYPE_DES;
            ukey.length = sec_passwd_c_des_key_size;
            ukey.contents = DES_PASSWD(key);
	    keyseed = &ukey;
            break;

        case sec_passwd_plain:
	    keyseed = &pdata;
	    pdata.data = (char *)PLAIN_PASSWD(key);
	    pdata.length = strlen(pdata.data);
	    keyproc = pwd_keyproc;
            break;
    }

    /* No way to pass bind_to_master flag through Kerberos and back
     * out to krb5rpc.c's sec_krb5rpc_sendto_kdc() so we'll take the
     * ugly way out and increment a global flag here so that any
     * TGT requests that happen until we decrement it will go to the
     * master security server
     */
    btm_flag_set = false;
    if (FLAG_SET(lcp->flags, sec_login_master_rgy)) {
        INC_BTM_FLAG;
        btm_flag_set = true;
    }

    enctypes[0] = ETYPE_DES_CBC_CRC;
    enctypes[1] = 0;

    /*
     * Request preauthentication methods. The order is *extremely*
     * important for the following reason: a pre-DCE 1.2.2 security
     * server only checks the first entry in the padata array; in order
     * to maintain interoperability the first entry must be of type
     * KRB5_PADATA_OSF_DCE or KRB5_PADATA_ENC_UNIX_TIME.
     */
    pa = &patypes[0];
    /*
     * Check if login context is from a machine principal.
     * If this is true, we cannot preauthenticate with
     * OSF_DCE, so we do not include it in the requested preauth types.
     */
    if (!FLAG_SET(lcp->flags, sec_login_machine_princ))
        *(pa++) = KRB5_PADATA_OSF_DCE;
    /*
     * As of DCE 1.2.2, we switched to a different Krb5 preauth
     * timestamps protocol (KRB5_PADATA_ENC_TIMESTAMPS), but we
     * still need to construct the old one for interop with pre-1.2.2
     * security servers.
     */
    *(pa++) = KRB5_PADATA_ENC_UNIX_TIME;
    *(pa++) = KRB5_PADATA_ENC_TIMESTAMP;
    *(pa++) = KRB5_PADATA_ROOT_CERT;	/* public key login (poorly named) */
    *pa = 0;

    *stp = krb5_get_in_tkt(NULL, KRB_REQUEST_INFO(lcp)->options,
			   KRB_REQUEST_INFO(lcp)->addrs,
			   enctypes,
			   patypes,
			   keyproc,
			   keyseed,
			   krb5_kdc_rep_decrypt_proc, 0,
			   &KRB_REQUEST_INFO(lcp)->creds,
			   KRB_INFO(lcp).cache,
			   &as_reply);

    /* Decrement above mentioned bind_to_master flag if necessary */
    if (btm_flag_set) {
        DEC_BTM_FLAG;
    }

    if (BAD_STATUS(stp)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "sec_login_pvt_get_and_process_krb_tgt: status after krb5_get_in_tkt: %d", *stp));
        switch(*stp) {
            free_krb_request_info(&KRB_REQUEST_INFO(lcp));
            case KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN:
            case KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN:
            case KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE:
                SET_STATUS(stp, sec_rgy_object_not_found);
		return;
            case KRB5KDC_ERR_NULL_KEY:
                SET_STATUS(stp, sec_login_s_null_password);
		return;
            case KRB5_PREAUTH_FAILED:
                SET_STATUS(stp, sec_login_s_preauth_failed);
		return;
            case KRB5KRB_AP_ERR_BAD_INTEGRITY:
	    case KRB5KDC_ERR_PREAUTH_FAILED:
	    case KRB5KDC_ERR_PREAUTH_REQUIRED: /* XXX Is this the right translation? */
                SET_STATUS(stp, sec_rgy_passwd_invalid);
		return;
	    case KRB5_KDC_UNREACH:
		/* 
		 * We should not have received this error if krb5_get_in_tkt 
		 * fetched TGT from local registry, so do not try local rgy.
		 */
		SET_STATUS(stp, sec_rgy_server_unavailable);
		return;
	    default:
		return;
	}
    }

    if (GOOD_STATUS(stp)) {
        /*
         * copy identity expiration time from the TGT  endtime
         */
        POLICY_INFO(lcp).identity_exp = as_reply->enc_part2->times.endtime;

        if (as_reply->padata) {
            /*
             * copy the pre-authentication data from the as_reply to the the login
             * context persistent kerberos info.  It will be needed later by the
             * the password update mechanism
             */
            for (i = 0; as_reply->padata[i] != NULL; i++) {

                if (  as_reply->padata[i]->pa_type == KRB5_PADATA_PW_SALT) {
                    KRB_INFO(lcp).pepper = *(as_reply->padata[i]);
                    if(as_reply->padata[i]->length > 0) {
                        KRB_INFO(lcp).pepper.contents  =
                            malloc(as_reply->padata[i]->length);

                        if (KRB_INFO(lcp).pepper.contents == NULL) {
                            SET_STATUS(stp, sec_login_s_no_memory);
                            memset(&KRB_INFO(lcp).pepper, 0, sizeof(KRB_INFO(lcp).pepper));
                        } else {
                            memcpy(KRB_INFO(lcp).pepper.contents,
                                   as_reply->padata[i]->contents,
                                   KRB_INFO(lcp).pepper.length);
                        }
                    } else {
                        KRB_INFO(lcp).pepper.contents = 0;
                    }
                }
            }
        }
    }

    if (GOOD_STATUS(stp)) {
        /*
         * In order for a login using local rgy to work, the TGT reply must
         * be encrypted in the user's key before it is stored in the local registry
         * cache. If the OSF_DCE or PUBLIC_REP preauth protocols are used, the 
	 * TGT will be encrypted in a random key which will be freed from memory. 
         * Therefore, we must re-encrypt the TGT reply in the user's key before 
	 * storing it in the local registry. Unfortunately, we do not know whether 
	 * the user is certifying their identity, nor do we know what key the TGT 
	 * is encrypted in, so we must reencrypt the TGT in the user's key each 
	 * time the user validates to be safe.
         */
	sec_lrgy_encode_kdc_rep(lcp, as_reply, PASSWD_TYPE(key), keyseed, stp);
    }

    if (!GOOD_STATUS(stp)) {
        /*
         * cleanup storage allocated for decrypted/decoded request info,
         * but leave the encrypted as_reply alone, so this routine can be
         * called again and still have something to work on
         */

        /*
         * Save and restore the client and server principals.  They'll
         * be needed again.
         */
        krb5_principal cprinc;
        krb5_principal sprinc;

        cprinc = KRB_REQUEST_INFO(lcp)->creds.client;
        KRB_REQUEST_INFO(lcp)->creds.client = NULL;
        sprinc = KRB_REQUEST_INFO(lcp)->creds.server;
        KRB_REQUEST_INFO(lcp)->creds.server = NULL;

        krb5_free_cred_contents(&KRB_REQUEST_INFO(lcp)->creds);
        memset(&KRB_REQUEST_INFO(lcp)->creds, 0, sizeof(krb5_creds));

        KRB_REQUEST_INFO(lcp)->creds.client = cprinc;
        KRB_REQUEST_INFO(lcp)->creds.server = sprinc;

    }
    /* Done with the temporary storage for the processed as_reply */
    if (as_reply != NULL) {
        krb5_free_kdc_rep(as_reply);
    }
}


/*
 * retrieve pepper from login context or create default if NULL.
 */
PUBLIC char * sec_login_pvt_get_pepper (
    sec_login_context_t *lcp,
    error_status_t      *stp
)
{
    char *pepper = NULL;

    CLEAR_STATUS(stp);

    if (KRB_INFO(lcp).pepper.length != 0) {
        pepper = malloc(KRB_INFO(lcp).pepper.length + 1);
        if (pepper == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            return NULL;
        } else {
            memcpy(pepper, KRB_INFO(lcp).pepper.contents,
                   KRB_INFO(lcp).pepper.length);
            pepper[KRB_INFO(lcp).pepper.length] = '\0';
        }
    } else {
        krb5_data       salt;
        krb5_principal  princ;

        *stp = krb5_cc_get_principal(lcp->krb_info.cache, &princ);
        if (GOOD_STATUS(stp)) {
            *stp = krb5_principal2salt(princ, &salt);
            krb5_free_principal(princ);
        }
        if (GOOD_STATUS(stp)) {
            pepper = malloc(salt.length + 1);
            if (pepper == NULL) {
                SET_STATUS(stp, sec_login_s_no_memory);
            } else {
		memcpy(pepper, salt.data, salt.length);
                pepper[salt.length] = '\0';
            }
            free(salt.data);
        }
    }

    return pepper;
}

/*
 * This routine constructs the trusted portion of
 * preauthentication data required for type
 * KRB5_PADATA_OSF_DCE.
 */
PUBLIC void sec_login_pvt_get_trusted_preauth (
    krb5_principal        client,          /* [in]  */
    krb5_data             **machine_tgt,   /* [out] */
    krb5_data             **enc_data,      /* [out] */
    krb5_keyblock         *random_key,     /* [out] */
    error_status_t        *stp             /* [out] */
)
{
    rpc_binding_handle_t  handle;
    idl_char              *tfilename;
    boolean32             created = false;
    encoded_data_t        *machine_tkt;
    encoded_data_t        *ebuf;
    FILE                  *tfp;
    error_status_t        lst;
    char                  *client_unparsed;
    idl_char              *client_name;
    int                   tfd;

    CLEAR_STATUS(stp);
    *machine_tgt = NULL;
    *enc_data = NULL;

    /*
     * check if superuser, and if so formulate the trusted
     * padata without the help of sec_clientd.
     */
#if 0
    if (getuid() == 0)
        /* not supported yet */ ;
#endif

    /*
     * Bind to sec_clientd.
     */
    sec_login_util_bind_to_sec_clientd(&handle, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /* unparse client principal */
    *stp = krb5_unparse_name(client, &client_unparsed);
    if (BAD_STATUS(stp)) {
        rpc_binding_free(&handle, &lst);
        return;
    }

    /*
     * call sec_clientd to get the trusted preauth data.
     */
    tfilename = NULL;
    client_name = (idl_char *)client_unparsed;
    (*rsec_login_v2_0_c_epv.rsec_login_get_trusted_preauth)
	(handle, client_name, getuid(),
	 &tfilename, &machine_tkt, &ebuf, stp); 
    /* clean up resources associated with handle */
    rpc_binding_free(&handle, &lst);
    if (client_unparsed)
        free(client_unparsed);
    if (BAD_STATUS(stp)) {
        if (tfilename)
	    free(tfilename);
        return;
    }

    /*
     * Read random key from file 
     */
    tfp = fopen((char *) tfilename, "r");
    if (tfp == (FILE *) NULL) {
        free(tfilename);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        *stp = errno;
        return;
    }
    if ((fread((krb5_pointer) &random_key->keytype, 
               sizeof(random_key->keytype), 1, tfp) != 1) ||
        (fread((krb5_pointer) &random_key->length,
               sizeof(random_key->length), 1, tfp) != 1)) {
        /* could be EOF */
        *stp = errno;
        (void) fclose(tfp);
        free(tfilename);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        return;
    }
    if (!(random_key->contents = 
           (krb5_octet *) malloc(random_key->length))) {
        SET_STATUS(stp, sec_login_s_no_memory);
        (void) fclose(tfp);
        free(tfilename);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        return;
    }
    if (fread((krb5_pointer) random_key->contents, 
              sizeof(random_key->contents[0]), random_key->length, 
              tfp) != random_key->length) {
        /* could be EOF */
        *stp = errno;
        (void) fclose(tfp);
        free(tfilename);
        cleanup_key(random_key);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        return;
    }
    (void) fclose(tfp);

    free(tfilename);

    if (!(*machine_tgt = (krb5_data *) malloc(sizeof(krb5_data)))) {
        SET_STATUS(stp, sec_login_s_no_memory);
        cleanup_key(random_key);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        return;
    }
    (*machine_tgt)->length = machine_tkt->num_bytes;
    if (!((*machine_tgt)->data = (char *) malloc(machine_tkt->num_bytes))) {
        SET_STATUS(stp, sec_login_s_no_memory);
        cleanup_key(random_key);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        free(*machine_tgt);
        return;
    }
    memcpy((*machine_tgt)->data, machine_tkt->bytes, machine_tkt->num_bytes);

    if (!(*enc_data = (krb5_data *) malloc(sizeof(krb5_data)))) {
        SET_STATUS(stp, sec_login_s_no_memory);
        cleanup_key(random_key);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        krb5_free_data(*machine_tgt);
        return;
    }
    (*enc_data)->length = ebuf->num_bytes;
    if (!((*enc_data)->data = (char *) malloc(ebuf->num_bytes))) {
        SET_STATUS(stp, sec_login_s_no_memory);
        cleanup_key(random_key);
        rpc_ss_client_free((idl_void_p_t) machine_tkt);
        rpc_ss_client_free((idl_void_p_t) ebuf);
        krb5_free_data(*machine_tgt);
        free(*enc_data);
        return;
    }
    memcpy((*enc_data)->data, ebuf->bytes, ebuf->num_bytes);

    /* done with RPC allocated memory */
    rpc_ss_client_free((idl_void_p_t) machine_tkt);
    rpc_ss_client_free((idl_void_p_t) ebuf);
}

#define GENID_PREFIX "GENID:"
#define GENID_PREFIX_LEN 6
/*
 * Generate an export token from the input
 * login context.  If the token length is
 * less than the input token_buf_size, copy
 * the token into  token_buf, otherwise
 * just return the length
 */

/*
 * This routine is *EXTREMELY* performance criticial; it gets called
 * *TWICE* for every single CDS attribute lookup; each rpc_ns call
 * generates two or more attribute lookups.
 */

void sec_login_pvt_gen_export_token (
    sec_login_context_t  *lc_p,          /* [in] */
    unsigned32           token_buf_size, /* [in] */
    ndr_byte             token_buf[],    /* [out] */
    unsigned32           *token_len_p,   /* [out] */
    error_status_t       *st_p           /* [out] */
)
{
    uuid_t               generation_id;
    char                 *cfn;   /* kerberos credential cache filename */
    unsigned_char_p_t    uuid_str = NULL;
    unsigned32           ccname_len, genid_str_len = 0;
    sec_login_db_entry_t db_entry;
    char                 *dfn;
    error_status_t       lst;
    int			 ret;
    struct stat 	 stb;
    
    if (token_buf_size == 0) 
    {
	/* CDS calls us like this A LOT; so give it a fast path... */
	cfn = krb5_cc_get_name(lc_p->krb_info.cache);
	if (cfn == NULL) {
	    SET_STATUS(st_p, sec_login_s_handle_invalid);
	    return;
	}
	ccname_len = strlen(cfn) +
		strlen(lc_p->krb_info.cache->ops->prefix) + 2;
	ccname_len += GENID_PREFIX_LEN + 1 + 36; /* strlen(uuid) */
	*token_len_p = ccname_len;
	SET_STATUS(st_p, sec_login_s_no_memory);
	return;
    }


    ret = stat(DATAFILE_NAME(dfn, KRB_INFO(lc_p).cache), &stb);
    free(dfn);
    if (ret == -1) {
        SET_STATUS(st_p, sec_login_s_no_current_context);
        if (errno != ENOENT) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, "%s%s"),
			   sec_s_login, svc_c_sev_error | svc_c_action_none,
			   sec_sys_errno_text, "access",
			   strerror(errno));
        }
        return;
    }
    if ((stb.st_mtime != lc_p->exptoken_mtime) ||
	(lc_p->exptoken == NULL))
    {
	if (lc_p->exptoken) {
	    free(lc_p->exptoken);
	    lc_p->exptoken = NULL;
	}
	/* retrieve generation id from context data file */
	db_entry.entry_type = generation_id_enum;
	sec_login_util_fetch_db_entry((sec_login_handle_t *) lc_p,
				      DB_DATAFILE_NAME(dfn, KRB_INFO(lc_p).cache),
				      &db_entry, st_p);
	if (dfn)
	    free(dfn);
	if (GOOD_STATUS(st_p)) {
	    generation_id = db_entry.contents.generation_id;
	    sec_login_db_free_entry_contents(rpc_ss_client_free, &db_entry, -1);
	} else if (STATUS_EQUAL(st_p, db_s_key_not_found) ||
		   STATUS_EQUAL(st_p, db_s_open_failed_enoent)) {
	    /*
	     * we may have picked up a DCE 1.0 login context which
	     * is not aware of the generation id.
	     */
	    uuid_create_nil(&generation_id, &lst);
	    CLEAR_STATUS(st_p);
    	} else if (BAD_STATUS(st_p)) {
	    return;
	}
    
	/* Get the finalized cache name */
	cfn = krb5_cc_get_name(lc_p->krb_info.cache);
	if (cfn == NULL) {
	    SET_STATUS(st_p, sec_login_s_handle_invalid);
	    return;
	}

	/* 
	 * Don't bother putting a NIL uuid into the
	 * export token.  The absence of a generation uuid
	 * results in an assumed NIL uuid in the import logic.
	 */
	if (!uuid_is_nil(&generation_id, st_p) && STATUS_OK(st_p)) {
	    uuid_to_string(&generation_id, &uuid_str, st_p);
	    if (STATUS_OK(st_p)) {
		genid_str_len = strlen((char *) uuid_str) + 1 + GENID_PREFIX_LEN;
	    }
	}

	if (BAD_STATUS(st_p)) {
	    return;
	}
    
	DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		       "sec_login_pvt_gen_export_token: ccache = (%s) - generation uuid = (%s)",
		       cfn, (uuid_str ? ((char *) uuid_str) : "<NIL>")));
	    
	/*
	 * Need enough room for "<TYPETAG>:<NAME>\0"
	 * If the context has a non-NIL generation uuid, then
	 * we need space for  "GENID:<UUID-STRING>\0" *after* the
	 * '\0' character as well.  
	 */
	ccname_len = strlen(cfn) + strlen(lc_p->krb_info.cache->ops->prefix) + 2;
	lc_p->exptoken_mtime = stb.st_mtime;
	lc_p->exptoken_len = ccname_len + genid_str_len;
	lc_p->exptoken = malloc (lc_p->exptoken_len);
	if (lc_p->exptoken == NULL)
	    SET_STATUS(st_p, sec_s_no_memory);
	else {
	    strcpy( (char *) lc_p->exptoken, lc_p->krb_info.cache->ops->prefix);
	    strcat( (char *) lc_p->exptoken, ":");
	    strcat( (char *) lc_p->exptoken, cfn);

	    if (genid_str_len > 0) {
		char *p;
		p = lc_p->exptoken + ccname_len;
		memcpy(p, GENID_PREFIX, GENID_PREFIX_LEN);
		p += GENID_PREFIX_LEN;
		memcpy(p, uuid_str, genid_str_len - GENID_PREFIX_LEN);
	    }
	}
	/* CHFts14684: free uuid_str in both cases */
	if (uuid_str != NULL) {
	    error_status_t  ignore_st;
	    
	    rpc_string_free(&uuid_str, &ignore_st);
	}
    }

    *token_len_p = lc_p->exptoken_len;
    if (*token_len_p > token_buf_size) {
	SET_STATUS(st_p, sec_login_s_no_memory);
    } else {
	memset(token_buf, 0, token_buf_size);
	memcpy(token_buf, lc_p->exptoken, lc_p->exptoken_len);
	SET_STATUS(st_p, error_status_ok);
    }
}

void sec_login_pvt_parse_export_token(
    ndr_char        buf[],             /* [in] */
    unsigned32      buf_len,           /* [in] */
    char            **cname_p,         /* [out] */
    int             *cname_len_p,      /* [out] */
    uuid_t          *generation_id_p,  /* [out] */ 
    error_status_t  *st_p              /* [out] */
)
{
    char *p;

    *cname_p = p = (char *) &buf[0];
    *cname_len_p = strlen(*cname_p) + 1;

    p += *cname_len_p;

    if ((*cname_len_p < buf_len) 
    && (memcmp(p, GENID_PREFIX, GENID_PREFIX_LEN) == 0)) {
	p += GENID_PREFIX_LEN;
        uuid_from_string((unsigned_char_p_t) p, generation_id_p, st_p);
	DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		       "sec_login_pvt_parse_export_token: ccache = (%s) - generation uuid = (%s)",
		       *cname_p, p));
    } else {
	uuid_create_nil(generation_id_p, st_p);
	DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		       "sec_login_pvt_parse_export_token: ccache = (%s) - generation uuid = (%s)",
		       *cname_p, "<NIL>"));
    }
}

/*
 * sec_login_pvt_init_v1_1_info
 *
 * Initialize the fields of the sec_login__v1_1_info_t struct
 * to reasonable initial values.
 */
PUBLIC  void  sec_login_pvt_init_v1_1_info (
    sec_bytes_t             *epac_chain,
    char		    *prev_cfn,
    sec_login__v1_1_info_t  *v1_1_info
)
{
    if (!epac_chain) {
        v1_1_info->epac_chain.num_bytes = 0;
        v1_1_info->epac_chain.bytes = NULL;
    } else {
        v1_1_info->epac_chain = *epac_chain;
    }
    v1_1_info->dlg_req_type = sec__login_c_dlg_req_none;
    v1_1_info->compat_mode = sec_id_compat_mode_initiator;

    memset(&v1_1_info->dlg_req_info, 0, sizeof(v1_1_info->dlg_req_info));

    sec_login_pvt_get_gen_v1_pac_flag(&v1_1_info->gen_v1_pac);

    v1_1_info->pag = 0;
    /*
     * If we inherited this login context from a previous login context,
     * set the pag to the numeric portion of the leaf component of the 
     * credential cache filename.
     */
    if (prev_cfn) {
        sscanf(prev_cfn, SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT, &v1_1_info->pag);
    }
}

/*
 * sec_login_pvt_copy_v1_1_info
 *
 * Copy one sec_login__v1_1_info_t struct to another.
 */
PUBLIC void sec_login_pvt_copy_v1_1_info(
    sec_login__v1_1_info_t      *to_v1_1_info,  	/* [out] */
    sec_login__v1_1_info_t      *from_v1_1_info, 	/* [in]  */
    error_status_t              *stp            	/* [out] */
)
{
    sec_login__v1_1_info_t	*to = to_v1_1_info;

    CLEAR_STATUS(stp);

    if (!from_v1_1_info)
        return;

    memset(to, 0, sizeof(*to));

    to->gen_v1_pac = from_v1_1_info->gen_v1_pac;
    to->pag = from_v1_1_info->pag;
    to->dlg_req_type = from_v1_1_info->dlg_req_type;
    to->compat_mode  = from_v1_1_info->compat_mode;

    if (!from_v1_1_info->epac_chain.bytes) {
        to->epac_chain.num_bytes = 0;
        to->epac_chain.bytes = NULL;
    } else {
        to->epac_chain.num_bytes = from_v1_1_info->epac_chain.num_bytes;
        to->epac_chain.bytes = malloc(to->epac_chain.num_bytes);
        if (!to->epac_chain.bytes) {
            SET_STATUS(stp, sec_login_s_no_memory);
            return;
        }
        memcpy(to->epac_chain.bytes, from_v1_1_info->epac_chain.bytes,
               to->epac_chain.num_bytes);
    }

    copy_dlg_request_info(from_v1_1_info->dlg_req_info.info.dlg_type,
                          &from_v1_1_info->dlg_req_info.info.dlg_rstrs,
                          &from_v1_1_info->dlg_req_info.info.tgt_rstrs,
                          &from_v1_1_info->dlg_req_info.info.opt_rstrs,
                          &from_v1_1_info->dlg_req_info.info.req_rstrs,
                          &from_v1_1_info->dlg_req_info.info.dlg_chain,
                          &from_v1_1_info->dlg_req_info.info.dlg_token_set,
                          &to->dlg_req_info.info, stp);
    if (BAD_STATUS(stp)) {
        if (to->epac_chain.bytes)
            free(to->epac_chain.bytes);
        return;
    }
}

/*
 * sec_login_pvt_get_dce_cf_data
 *
 * Given a key, retrieves the corresponding value from the
 * dce_cf.db file.
 */
PUBLIC void sec_login_pvt_get_dce_cf_data(
    char		*key,		/* [in]  */
    char		**data,		/* [out] */
    error_status_t	*stp		/* [out] */
)
{
    FILE *fp;
    char *config_file = DCE_CONFIG_FILE;

    CLEAR_STATUS(stp);

    *data = NULL;

    fp = fopen(config_file, "r");
    if (fp == (FILE *) NULL) {
        SET_STATUS(stp, errno);
        return;
    }
    dce_cf_find_name_by_key(fp, key, data, stp);

    (void) fclose(fp);
}

/*
 * sec_login_pvt_get_krb5_ccache_vno
 *
 * Retrieves the value of the krb5_ccache_vno key.
 * Default version returned is 1, to preserve binary
 * compatibility.
 *
 * Returns vno in hexadecimal format
 */
PUBLIC void sec_login_pvt_get_krb5_ccache_vno(
    int			*version	/* [out] */
)
{
    char *data;
    error_status_t lst;

/*
 * These match the values defined in krb5/lib/ccache/file/fcc.h
 */
#define SEC_KRB5_FCC_FVNO_1 0x0501          /* krb v5, fcc v1 */
#define SEC_KRB5_FCC_FVNO_2 0x0502          /* krb v5, fcc v2 */
#define SEC_KRB5_FCC_FVNO   0x0503	    /* krb v5, fcc v3 */
#define SEC_KRB5_FCC_DEFAULT_FVNO SEC_KRB5_FCC_FVNO

    /* Assume binary compatibility! */
    *version = SEC_KRB5_FCC_FVNO_1;
    sec_login_pvt_get_dce_cf_data("krb5_ccache_vno", &data, &lst);
    if (GOOD_STATUS(&lst)) {
        *version = atoi(data);
	switch (*version) {
	    case 1:
		*version = SEC_KRB5_FCC_FVNO_1;
		break;
	    case 2:
		*version = SEC_KRB5_FCC_FVNO_2;
		break;
	    default:
                /*
                 * If version is equal to or greater than what we 
		 * support, set to the highest we support.
                 */
                *version = SEC_KRB5_FCC_DEFAULT_FVNO;
	}
	free(data);
    }
}

/*
 * sec_login_pvt_get_gen_v1_pac_flag
 *
 * Retrieves the value of the get_gen_v1_pac key.
 */
PUBLIC void sec_login_pvt_get_gen_v1_pac_flag(
    boolean32		*gen_v1_pac	/* [out] */
)
{
    char *data = NULL;
    error_status_t st;

    *gen_v1_pac = true;

    sec_login_pvt_get_dce_cf_data("gen_v1_pac", &data, &st);
    /*
     * gen_v1_pac is set to true on all cases unless value starts
     * with an 'F' or 'f'.
     */
    if (GOOD_STATUS(&st) &&
        (data != NULL) && 
        ((*data == 'F') || (*data == 'f'))) {
            *gen_v1_pac = false;
    }
    if (data != NULL)
        free(data);
}

/*
 * sec_login_pvt_get_krb5_kt_vno
 *
 * Retrieves the value of the krb5_kt_vno key.
 * Default version returned is 1, to preserve binary
 * compatibility.
 *
 * Returns vno in hexadecimal format
 */
PUBLIC void sec_login_pvt_get_krb5_kt_vno(
    krb5_int16             *version        /* [out] */
)
{
    char *data;
    error_status_t lst;

/*
 * These match the values defined in krb5/lib/keytab/file/ktfile.h
 */
#define SEC_KRB5_KT_VNO_1 0x0501          /* krb v5, keytab v1 */
#define SEC_KRB5_KT_VNO   0x0502          /* krb5 v5, keytab v2 */
#define SEC_KRB5_KT_DEFAULT_VNO SEC_KRB5_KT_VNO

    /* Assume binary compatibility! */
    *version = SEC_KRB5_KT_VNO_1;
    sec_login_pvt_get_dce_cf_data("krb5_kt_vno", &data, &lst);
    if (GOOD_STATUS(&lst)) {
        *version = atoi(data);
        /*
         * If version is greater than what we support, set to
         * the highest we support.
         */
        if (*version >= 2)
            *version = SEC_KRB5_KT_DEFAULT_VNO;
        else
            *version = SEC_KRB5_KT_VNO_1;
        free(data);
    }
}
