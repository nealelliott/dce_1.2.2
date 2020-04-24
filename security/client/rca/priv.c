/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: priv.c,v $
 * Revision 1.1.16.3  1996/11/13  17:59:54  arvind
 * 	CHFts20712: Remove spurious xfree call which should have been removed
 * 	in previous change (why didn't purify catch this??)
 * 	[1996/11/08  16:07 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_5/1]
 *
 * 	priv_ptgt_exists: always compare epac uuids even if privs argument is NULL.
 * 	[1996/02/22  19:38 UTC  jrr  /main/HPDCE02/13]
 *
 * 	Merge fix for CHFts15637
 * 	[1995/08/10  18:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/3]
 *
 * 	ptgt_exists: ensure ptgt/epac uuids match
 * 	[1995/08/09  14:23 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 *
 * 	Call priv_krb_util_construct_v1_stkt_authdata
 * 	[1995/08/02  17:54 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	sec_priv_ptgt_exists: make sure ptgt uuids match in .db and krb5 creds files
 * 	[1995/05/26  19:38 UTC  mullan_s  /main/HPDCE02/10]
 *
 * 	Merge
 * 	[1995/05/26  18:51 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 *
 * 	sec_priv_store_ptgt: pass epac to sec_login_util_store_ptgtcreds_and_epac
 * 	[1995/05/24  19:27 UTC  mullan_s  /main/HPDCE02/9]
 *
 * 	Merge
 * 	[1995/05/23  19:01 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat10/1]
 *
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 	[1995/05/22  17:05 UTC  mullan_s  /main/HPDCE02/8]
 *
 * 	Merge
 * 	[1995/05/22  16:30 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	Call thread-safe sec_login_util_store_ptgtcreds_and_epac
 * 	in sec_priv_store_ptgt.
 * 	[1995/05/17  19:43 UTC  greg  /main/HPDCE02/7]
 *
 * 	Fix compat bug in sec_priv_get_ptgt_internal().
 * 	[1995/05/17  19:38 UTC  greg  /main/HPDCE02/SEC_migrate_merge/7]
 *
 * 	Fix busted compatibility codei in sec_priv_get_ptgt_internal().
 * 	[1995/05/15  20:59 UTC  greg  /main/HPDCE02/6]
 *
 * 	Merge
 * 	[1995/05/11  17:04 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat7/1]
 *
 * 	In priv_pvt_ptgt_exists, initialize flags to 0.
 * 	[1995/05/10  20:42 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	Merge CHFts15233, CHFts15232.
 * 	[1995/05/10  19:53 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/2]
 *
 * 	When storing a PTGT, first check the krb5 creds for an unexpired PTGT.
 * 	It's possible another process was simultaneously refreshing its PTGT
 * 	and beat us to it.
 * 	[1995/05/10  14:16 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/1]
 *
 * 	Change priv_pvt_ptgt_exists to look in krb5 creds file instead
 * 	of .data.db file for unexpired PTGTs. This allows a krb5 kinit
 * 	to function properly because it only updates the krb5 creds
 * 	file, not the .data.db file.
 * 	[1995/05/08  21:09 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	Merge
 * 	[1995/05/08  19:43 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/2]
 *
 * 	Ensure that creds and epac chain are stored within an
 * 	inter-process and thread safe lock.
 * 	[1995/05/04  21:15 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/1]
 *
 * 	Use sec_svc_handle instead of NULL when calling dce_assert
 * 	[1995/04/14  23:59 UTC  rps  /main/HPDCE02/2]
 *
 * 	Binary Compatibility Merge
 * 	[1994/09/28  21:34:49  burati  1.1.8.11]
 *
 * Revision 1.1.16.2  1996/10/03  14:47:06  arvind
 * 	Deal with API change for type-safety in encode/decode routines.
 * 	[1996/09/16  20:30 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 * 
 * 	last endgame sec fix fron hp -mcg
 * 	[1996/04/16  20:59:47  root]
 * 
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:02 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	priv_ptgt_exists: always compare epac uuids even if privs argument is NULL.
 * 	[1996/02/22  19:38 UTC  jrr  /main/HPDCE02/13]
 * 
 * 	Remove individual "old interface" status checks and replace
 * 	with STATUS_OLD_IF() macro.
 * 	[1996/02/21  16:13 UTC  jrr  /main/HPDCE02/jrr_fallback/1]
 * 
 * 	Merge fix for CHFts15637
 * 	[1995/08/10  18:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/3]
 * 
 * 	ptgt_exists: ensure ptgt/epac uuids match
 * 	[1995/08/09  14:23 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata
 * 	[1995/08/02  17:54 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	sec_priv_ptgt_exists: make sure ptgt uuids match in .db and krb5 creds files
 * 	[1995/05/26  19:38 UTC  mullan_s  /main/HPDCE02/10]
 * 
 * 	Merge
 * 	[1995/05/26  18:51 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 * 
 * 	sec_priv_store_ptgt: pass epac to sec_login_util_store_ptgtcreds_and_epac
 * 	[1995/05/24  19:27 UTC  mullan_s  /main/HPDCE02/9]
 * 
 * 	Merge
 * 	[1995/05/23  19:01 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat10/1]
 * 
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 	[1995/05/22  17:05 UTC  mullan_s  /main/HPDCE02/8]
 * 
 * 	Merge
 * 	[1995/05/22  16:30 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 * 
 * 	Call thread-safe sec_login_util_store_ptgtcreds_and_epac
 * 	in sec_priv_store_ptgt.
 * 	[1995/05/17  19:43 UTC  greg  /main/HPDCE02/7]
 * 
 * 	Fix compat bug in sec_priv_get_ptgt_internal().
 * 	[1995/05/17  19:38 UTC  greg  /main/HPDCE02/SEC_migrate_merge/7]
 * 
 * 	Fix busted compatibility codei in sec_priv_get_ptgt_internal().
 * 	[1995/05/15  20:59 UTC  greg  /main/HPDCE02/6]
 * 
 * 	Merge
 * 	[1995/05/11  17:04 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat7/1]
 * 
 * 	In priv_pvt_ptgt_exists, initialize flags to 0.
 * 	[1995/05/10  20:42 UTC  mullan_s  /main/HPDCE02/4]
 * 
 * 	Merge CHFts15233, CHFts15232.
 * 	[1995/05/10  19:53 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/2]
 * 
 * 	When storing a PTGT, first check the krb5 creds for an unexpired PTGT.
 * 	It's possible another process was simultaneously refreshing its PTGT
 * 	and beat us to it.
 * 	[1995/05/10  14:16 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/1]
 * 
 * 	Change priv_pvt_ptgt_exists to look in krb5 creds file instead
 * 	of .data.db file for unexpired PTGTs. This allows a krb5 kinit
 * 	to function properly because it only updates the krb5 creds
 * 	file, not the .data.db file.
 * 	[1995/05/08  21:09 UTC  mullan_s  /main/HPDCE02/3]
 * 
 * 	Merge
 * 	[1995/05/08  19:43 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/2]
 * 
 * 	Ensure that creds and epac chain are stored within an
 * 	inter-process and thread safe lock.
 * 	[1995/05/04  21:15 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/1]
 * 
 * 	Use sec_svc_handle instead of NULL when calling dce_assert
 * 	[1995/04/14  23:59 UTC  rps  /main/HPDCE02/2]
 * 
 * 	Binary Compatibility Merge
 * 	[1994/09/28  21:34:49  burati  1.1.8.11]
 * 
 * Revision 1.1.16.1  1996/05/17  15:14:27  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/05/16  22:01 UTC  psn
 * 	CHFts16550: memory leak due to mis-defined macro.
 * 	[1995/10/12  22:18 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/11]
 * 
 * 	ptgt_exists: ensure ptgt/epac uuids match
 * 	[1995/08/10  18:19 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/3]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata
 * 	[1995/08/09  14:23 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	sec_priv_ptgt_exists: make sure ptgt uuids match in .db and krb5 creds files
 * 	[1995/08/02  17:54 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	Merge
 * 	[1995/05/26  19:38 UTC  mullan_s  /main/HPDCE02/10]
 * 
 * 	sec_priv_store_ptgt: pass epac to sec_login_util_store_ptgtcreds_and_epac
 * 	[1995/05/26  18:51 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 * 
 * 	Merge
 * 	[1995/05/24  19:27 UTC  mullan_s  /main/HPDCE02/9]
 * 
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 	[1995/05/23  19:01 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat10/1]
 * 
 * 	Merge
 * 	[1995/05/22  17:05 UTC  mullan_s  /main/HPDCE02/8]
 * 
 * 	Call thread-safe sec_login_util_store_ptgtcreds_and_epac
 * 	in sec_priv_store_ptgt.
 * 	[1995/05/22  16:30 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 * 
 * 	Fix compat bug in sec_priv_get_ptgt_internal().
 * 	[1995/05/17  19:43 UTC  greg  /main/HPDCE02/7]
 * 
 * 	Fix busted compatibility codei in sec_priv_get_ptgt_internal().
 * 	[1995/05/17  19:38 UTC  greg  /main/HPDCE02/SEC_migrate_merge/7]
 * 
 * 	Yet another mergeout from HPDCE02 --> SEC_migrate_merge.
 * 	[1995/05/13  17:45 UTC  greg  /main/HPDCE02/SEC_migrate_merge/6]
 * 
 * 	Merge
 * 	[1995/05/11  17:07 UTC  mullan_s  /main/HPDCE02/5]
 * 
 * 	In priv_pvt_ptgt_exists, initialize flags to 0.
 * 	[1995/05/11  17:04 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat7/1]
 * 
 * 	Merge CHFts15233, CHFts15232.
 * 	[1995/05/10  20:42 UTC  mullan_s  /main/HPDCE02/4]
 * 
 * 	When storing a PTGT, first check the krb5 creds for an unexpired PTGT.
 * 	It's possible another process was simultaneously refreshing its PTGT
 * 	and beat us to it.
 * 	[1995/05/10  19:53 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/2]
 * 
 * 	Change priv_pvt_ptgt_exists to look in krb5 creds file instead
 * 	of .data.db file for unexpired PTGTs. This allows a krb5 kinit
 * 	to function properly because it only updates the krb5 creds
 * 	file, not the .data.db file.
 * 	[1995/05/10  14:16 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/1]
 * 
 * 	Fix reference to undeclared "home_cell" in
 * 	sec_priv_become_impersonator()
 * 	[1995/05/13  02:40 UTC  greg  /main/HPDCE02/SEC_migrate_merge/5]
 * 
 * 	merge out from HPDCE02 to SEC_migrate_merge
 * 	[1995/05/10  17:59 UTC  greg  /main/HPDCE02/SEC_migrate_merge/4]
 * 
 * 	Merge
 * 	[1995/05/11  17:07 UTC  mullan_s  /main/HPDCE02/5]
 * 
 * 	In priv_pvt_ptgt_exists, initialize flags to 0.
 * 	[1995/05/11  17:04 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat7/1]
 * 
 * 	Merge CHFts15233, CHFts15232.
 * 	[1995/05/10  20:42 UTC  mullan_s  /main/HPDCE02/4]
 * 
 * 	When storing a PTGT, first check the krb5 creds for an unexpired PTGT.
 * 	It's possible another process was simultaneously refreshing its PTGT
 * 	and beat us to it.
 * 	[1995/05/10  19:53 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/2]
 * 
 * 	Change priv_pvt_ptgt_exists to look in krb5 creds file instead
 * 	of .data.db file for unexpired PTGTs. This allows a krb5 kinit
 * 	to function properly because it only updates the krb5 creds
 * 	file, not the .data.db file.
 * 	[1995/05/10  14:16 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat6/1]
 * 
 * 	Merge
 * 	[1995/05/08  21:09 UTC  mullan_s  /main/HPDCE02/3]
 * 
 * 	Ensure that creds and epac chain are stored within an
 * 	inter-process and thread safe lock.
 * 	[1995/05/08  19:43 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/2]
 * 
 * 	Use sec_svc_handle instead of NULL when calling dce_assert
 * 	[1995/05/04  21:15 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/1]
 * 
 * 	merge up to nightly
 * 	[1995/04/10  17:38 UTC  rps  /main/MOTHRA_panic/3]
 * 
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:02 UTC  mullan_s  /main/HPDCE02
 * 
 * Revision 1.1.8.11  1994/09/28  21:34:49  burati
 * 	CR12179 (slowness) Call new rtn krb5_get_credentials_dce()
 * 	[1994/09/28  21:12:50  burati]
 * 
 * Revision 1.1.8.10  1994/08/25  21:14:31  burati
 * 	Pass tcell to rca_site_bind() in sec_priv_get_eptgt() for site name
 * 	[1994/08/25  21:13:17  burati]
 * 
 * Revision 1.1.8.9  1994/08/18  20:25:21  greg
 * 	A few Fixes for 1.1->pre1.1 compatibility.
 * 	Have successfully logged in and performed a
 * 	klist on a 1.1 client in a 1.0.3 cell.
 * 	[1994/08/17  22:39:09  greg]
 * 
 * 	pre-1.1 client compatibility changes.
 * 	[1994/08/16  18:47:04  greg]
 * 
 * Revision 1.1.8.8  1994/08/04  16:12:35  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/28  17:16:44  mdf]
 * 
 * Revision 1.1.8.7  1994/07/06  20:27:07  sommerfeld
 * 	[OT10607]	band-aid for compat code, which allows you to configure a
 * 	    1.1 client of a 1.0.3 cell.
 * 	[1994/07/06  19:36:12  sommerfeld]
 * 
 * Revision 1.1.8.6  1994/06/10  15:07:12  greg
 * 	Fix double free of local_privs.bytes.
 * 	[1994/06/08  21:23:33  greg]
 * 
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:48:33  greg]
 * 
 * Revision 1.1.8.5  1994/06/02  21:57:37  mdf
 * 	Merged with changes from 1.1.8.4
 * 	[1994/06/02  21:56:37  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/24  15:14:15  mdf]
 * 
 * 	HP revision /main/ODESSA_2/cuti_dlg/1  1994/07/15  14:18 UTC  cuti
 * 	And reverse site_name with cell_name in rca_site_bind call.
 * 
 * Revision 1.1.8.4  1994/06/02  20:21:21  mdf
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/24  15:14:15  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.8.2  1994/05/11  19:03:06  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Substitute fake dlg token with real one
 * 	add memory management function parameters to sec_encode interfaces
 * 	[1994/04/29  20:46:49  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Substitute fake dlg token with real one
 * 	add memory management function parameters to sec_encode interfaces
 * 
 * 	hp_sec_to_osf_2 drop
 * 	Substitute fake dlg token with real one
 * 	add memory management function parameters to sec_encode interfaces
 * 
 * Revision 1.1.8.1  1994/01/28  23:10:29  burati
 * 	Delegation/EPAC changes (dlg_bl1)
 * 	[1994/01/19  22:31:24  burati]
 * 
 * Revision 1.1.5.5  1992/12/29  13:03:17  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:51  zeliff]
 * 
 * Revision 1.1.5.4  1992/12/07  15:53:54  burati
 * 	CR6349 memory leaks: ptgt_creds in priv_pvt_ptgt_exists() and
 * 	                     ptgt_req in sec_priv_get_ptgt()
 * 	[1992/12/07  00:00:06  burati]
 * 
 * Revision 1.1.5.3  1992/11/29  01:04:01  burati
 * 	CR6178 Use correct free routine for ptgt_rep
 * 	[1992/11/27  21:55:45  burati]
 * 
 * Revision 1.1.5.2  1992/09/29  21:16:05  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:19:58  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/14  22:09:28  sekhar
 * 	Pick up the following fix from HP:
 * 
 * 	Narrow exception handling range when communicating with privilege server.
 * 	[1992/08/14  22:07:42  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:47:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1994, 1995
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

/* 
 * Privilege Server Client Agent
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
"$Id: priv.c,v 1.1.16.3 1996/11/13 17:59:54 arvind Exp $";
#endif

#include <un_strng.h>

#include <dce/binding.h>
#include <priv.h>  /* priv svr client agent isn't part of the public API */
#include <rpriv.h>
#include <rpriv_1_0.h>

#include <rca_pvt.h>
#include <rgymacro.h>
#include <u_str.h>
#include <sec_krb_util.h>
#include <sec_pkl.h>
#include <sec_encode.h>
#include <sec_login_db.h>
#include <sec_login_util.h>
#include <sec_svc.h>
#include <priv_krb_util.h>
#include <dce/assert.h>
#include <krb5/krb5.h>
#include <krb5/los-proto.h>
#include <krb5/func-proto.h>
#include <krb5/asn1.h>

#ifdef IDL_PROTOTYPES
#define PROTOTYPE(x) x
#else
#define PROTOTYPE(x) ()
#endif

#define Crpriv_become_delegate(CH,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)\
  (*rpriv_v1_1_c_epv.rpriv_become_delegate)\
    (HANDLE(CH),b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t)

#define Crpriv_get_ptgt(CH,b,c,d,e,f)\
        (*rpriv_v1_0_c_epv.rpriv_get_ptgt) (HANDLE(CH),b,c,d,e,f)

#define Crpriv_get_eptgt(CH,b,c,d,e,f,g,h,i,j,k,l)\
        (*rpriv_v1_1_c_epv.rpriv_get_eptgt)(HANDLE(CH),b,c,d,e,f,g,h,i,j,k,l)

#define Crpriv_become_impersonator(CH,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)\
  (*rpriv_v1_1_c_epv.rpriv_become_impersonator)\
    (HANDLE(CH),b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s)

#define cleanup_authdata(enc_authdata) { if ((enc_authdata).ciphertext.data) {\
        (void) memset((enc_authdata).ciphertext.data, 0,\
             (enc_authdata).ciphertext.length); \
            xfree((enc_authdata).ciphertext.data);}}

static void  sec_priv_get_ptgt_internal(
    sec_login_handle_t     login_context,    /* [in] */
    unsigned_char_p_t      target_cell,      /* [in] */
    unsigned32             authn_svc,        /* [in] */
    unsigned32             authz_svc,        /* [in] */
    boolean32              recurse,          /* [in] */
    error_status_t         *stp              /* [out] */
);

static void  sec_priv_get_eptgt_internal(
    sec_login_handle_t     login_context,    /* [in] */
    unsigned_char_p_t      target_cell,      /* [in] */
    unsigned32             authn_svc,        /* [in] */
    unsigned32             authz_svc,        /* [in] */
    boolean32              recurse,          /* [in] */
    sec_bytes_t            *privs,           /* [out] */
    error_status_t         *stp              /* [out] */
);

static void sec_priv_store_ptgt(
    sec_login_handle_t		login_context,		/* [in]  */
    unsigned_char_p_t		target_cell,		/* [in]  */
    krb5_data                   *enc_eptgt_rep,         /* [in]  */
    krb5_keyblock               *ps_key,                /* [in]  */
    sec_bytes_t                 *epac_chain,            /* [in]  */
    krb5_ccache                 ccache,                 /* [in]  */
    boolean32			gen_v1_pac,		/* [in]  */
    sec_bytes_t			*privs,			/* [out] */
    error_status_t              *stp                    /* [out] */
);

/* p r i v _ p v t _ g e t _ e t y p e
 *
 * Choose an etype for use in encrypting the requisite portions of the
 * priv server request.  As there may be multiple encryption types in use,
 * we do our best to choose one that we know will work.  
 * 
 * For now, we extract the etype from the global cryptosystem info based on
 * the session key type, but that may not work if the mapping from keytype
 * to etype ever becomes one to many.  The kerberos folks haven't sorted
 * this one out, so we don't try either.  Presumably, any changes will be
 * cause a compile time error, and someone can rewrite this macro
 * accordingly. 
 */
#define  priv_pvt_get_etype(creds, etype_p, stp) \
    *(etype_p) = krb5_keytype_array[(creds)->keyblock.keytype]->system->proto_enctype; \
    CLEAR_STATUS((stp));
#define priv_pvt_get_etype_from_key(key, etype_p, stp) \
    *(etype_p) = krb5_keytype_array[(key)->keytype]->system->proto_enctype; \
    CLEAR_STATUS((stp));

/* s e c _ p r i v _ p t g t _ e x i s t s
 *
 * Check  the  credential cache for a valid ptgt for the target cell.
 * 
 * returns true if a valid, current ptgt is found, false otherwise.
 * "not found" status is not passed back to the caller, any other 
 * bad status is.
 */
extern boolean32 sec_priv_ptgt_exists(
    sec_login_handle_t login_context,
    unsigned_char_p_t  target_cell,
    krb5_ccache        ccache,
    sec_bytes_t        *privs, /* [out] */
    error_status_t     *stp    /* [out] */
)
{
    krb5_timestamp  currenttime = 0; /* so we don't find expired credentials */
    krb5_creds      key_creds, *ptgt_creds = NULL;
    krb5_flags      flags = 0;
    boolean32       found = false;
    char	    *dfn = NULL;

    CLEAR_STATUS(stp);

    ptgt_creds = (krb5_creds *) malloc(sizeof(krb5_creds));
    if (ptgt_creds == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        return found;
    }
    memset(ptgt_creds, 0, sizeof(krb5_creds));

    /*
     * if getting the current time won't work, then the process
     * must be completely hosed.  Don't bother trying to recover.
     */
    (void) krb5_timeofday(&currenttime);

    memset(&key_creds, 0, sizeof(krb5_creds));

    /*
     * client is the priv server for the target cell
     */
    sec_krb_ptgs_parsed_name( (char *) target_cell, &key_creds.client, stp);
    if (BAD_STATUS(stp)) {
        if (ptgt_creds)
            free(ptgt_creds);
        return found;
    }

    /*
     * server is the TGS server for the target cell
     */
    sec_krb_tgs_parsed_name( (char *)target_cell, &key_creds.server, stp);
    if (BAD_STATUS(stp)) {
        if (ptgt_creds)
            free(ptgt_creds);
        krb5_free_cred_contents(&key_creds);
        SET_STATUS(stp, sec_priv_s_no_mem);
        return found;
    }

    /* don't want a ticket that's expired. */
    key_creds.times.endtime = currenttime;

    /* try to retrieve a 1.1 client PTGT */
    priv_krb_util_construct_v1_ptgt_authdata(NULL, 0, NULL, NULL,
	&key_creds.authdata, NULL, stp);
    if (BAD_STATUS(stp)) {
        if (ptgt_creds)
            free(ptgt_creds);
        krb5_free_cred_contents(&key_creds);
	return found;
    }

    flags = flags | KRB5_TC_MATCH_TIMES | KRB5_TC_MATCH_AUTHDATA;
    /*
     * try to find an existing PTGT in the cache
     */
    DB_DATAFILE_NAME(dfn, ccache);
    *stp = krb5_cc_retrieve_cred(ccache, flags, &key_creds, ptgt_creds);

    krb5_free_cred_contents(&key_creds);

    if (BAD_STATUS(stp)) {
        /* not found isn't bad */
        if (STATUS_EQUAL(stp, KRB5_CC_NOTFOUND)) {
            CLEAR_STATUS(stp);
        }
    } else {
        sec_bytes_t		ptgt_authbytes, tmp_privs;
        boolean32 		more;

        found = true;
        /*
         * Retrieve the epac chain from the .data.db file
         * If this returns false, something may be seriously
         * wrong, because the epac chain and the PTGT should
         * have the same expiration date. It may be possible
         * that the time expired since we just retrieved the 
         * ticket from the creds file above. In that case, we
         * need a new PTGT anyway.
         */
        memset(&ptgt_authbytes, 0, sizeof(ptgt_authbytes));
        memset(&tmp_privs, 0, sizeof(tmp_privs));
        found = priv_krb_util_fetch_v1_1_ccache_authdata(login_context,
		    malloc_shim, free, dfn,
		    target_cell, &tmp_privs, &ptgt_authbytes, stp);
        /*
         * Compare the ptgt uuid with the epac uuid to ensure they
         * are identical. If not, we need to get a new PTGT to avoid
         * race conditions and mismatched seal errors.
         *
         * NOTE: only check the uuids if we are dealing with a 1.1 server.
         * We know this if we were able to fetch an epac chain.
         */
        if (found && GOOD_STATUS(stp) && (tmp_privs.bytes != NULL) && (tmp_privs.num_bytes > 0)) {
	    krb5_authdata	**epac_authdata = NULL;
	    error_status_t	lst;
	    priv_krb_util_bytes_to_authdata(&ptgt_authbytes, NULL, &epac_authdata, &lst);
	    found = priv_krb_util_compare_authdata_uuids(ptgt_creds->authdata,
			epac_authdata, true, &lst);
	    if (!found)
                DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug2,
                    "sec_priv_ptgt_exists: epac/ptgt uuid mismatch (file:%s)\n", dfn));
	    if (epac_authdata)
	        krb5_free_authdata(epac_authdata);
        }
        if (ptgt_authbytes.bytes)
	    free(ptgt_authbytes.bytes);

	if (found && privs) {
	    privs->bytes = tmp_privs.bytes;
	    privs->num_bytes = tmp_privs.num_bytes;
	} else if (tmp_privs.bytes)
	    free(tmp_privs.bytes);
    }

    krb5_free_creds(ptgt_creds);

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
	"sec_priv_ptgt_exists: Datafile (%s), target_cell (%s), found (%d)\n",
	dfn, target_cell, found));
    if (dfn) free(dfn);
    return found;
}

static   error_status_t  check_delegation_chain(
   sec_bytes_t               *delegation_chain,
   sec_id_delegation_type_t  delegation_type_requested
) 
{
    sec_id_epac_set_t     epac_set;
    sec_id_epac_data_t    epac_data;
    boolean32             epac_data_decoded = false;
    error_status_t        st;

    /* 
     * There has to be something in the delegation chain
     * parameter.
     */
    if (!delegation_chain
    || !delegation_chain->num_bytes
    || !delegation_chain->bytes) {
	return sec_priv_s_corrupt_deleg_chain;
    }
    
    epac_set.num_epacs = 0;
    memset(&epac_data, 0, sizeof(epac_data));

    sec_id_epac_set_decode(malloc_shim, free, 
			   delegation_chain->num_bytes,
			   delegation_chain->bytes,
			   &epac_set, 
			   &st);

    /* there has to be at least one epac in the decoded epac set */
    if (!STATUS_OK(&st) || epac_set.num_epacs == 0) {
	return sec_priv_s_corrupt_deleg_chain;
    }

    /* 
     * decode the last epac in the epac set, check the
     * the delegation type permitted against the delegation
     * type requested and reject if not compatible
     */
    sec_id_epac_data_decode(malloc_shim, free,
	    epac_set.epacs[epac_set.num_epacs-1].pickled_epac_data.num_bytes,
	    epac_set.epacs[epac_set.num_epacs-1].pickled_epac_data.bytes,
	    &epac_data, &st);
    
    if (!STATUS_OK(&st)) { 
	SET_STATUS(&st, sec_priv_s_corrupt_deleg_chain);
    } else {  /* last epac successfully decoded */
	sec_id_delegation_type_t delegation_type_permitted = epac_data.deleg_type;
	
	epac_data_decoded = true;

	switch (delegation_type_permitted) {
	case sec_id_deleg_type_none:
	    SET_STATUS(&st, sec_priv_s_deleg_not_enabled);
	    break;
	case sec_id_deleg_type_traced:
	    /*
	     * new delegate can request "traced" or "none", but
	     * not "impersonation."
	     */
	    if (delegation_type_requested == sec_id_deleg_type_impersonation) {
		SET_STATUS(&st, sec_priv_s_deleg_not_enabled);
	    }
	    break;
	case sec_id_deleg_type_impersonation:
	    /*
	     * new delegate can request "impersonation" or "none", but
	     * not "traced."
	     */
	    if (delegation_type_requested == sec_id_deleg_type_traced) {
		SET_STATUS(&st, sec_priv_s_deleg_not_enabled);
	    }
	    break;
	default:
	    /*
	     * Shouldn't ever happen, but just in case...
	     */
	    SET_STATUS(&st, sec_priv_s_bad_deleg_type);
	}     
    }

    /* cleanup, if necessary */
    if (epac_set.num_epacs > 0) {
	sec_encode_epac_set_free(free, &epac_set);
    }
    if (epac_data_decoded) {
	sec_encode_epac_data_free(free, &epac_data);
    }

    return st;
}
    
/*
 * Perform basic sanity checks on the delegation token
 * and then see if we can find at least one delegation 
 * token in the set that hasn't expired.
 */
static  error_status_t  check_delegation_token_set(
   sec_dlg_token_set_t  *token_set
)
{
    unsigned32      i;
    krb5_timestamp  now;
    error_status_t  st;

    if (!token_set
    || !token_set->num_tokens
    || !token_set->tokens) {
	return sec_priv_s_invalid_dlg_token;
    }
   
    st = krb5_timeofday(&now);
    if (!STATUS_OK(&st)) {
	return st;
    }

    /* assume status is "expired" and clear it if we find
     * a delegation token with any life left in it.
     */
    SET_STATUS(&st, sec_priv_s_deleg_token_exp);
    for (i = 0; i < token_set->num_tokens; i++) {
	if (token_set->tokens[i].expiration > now) {
	    CLEAR_STATUS(&st);
	    break;
	}
    }
    return st;
}


/*
 * s e c _ p r i v _ g e t _ e p t g t _ i n t e r n a l
 */
static void  sec_priv_get_eptgt_internal(
    sec_login_handle_t     login_context,    /* [in] */
    unsigned_char_p_t      target_cell,      /* [in] */
    unsigned32             authn_svc,        /* [in] */
    unsigned32             authz_svc,        /* [in] */
    boolean32              recurse,          /* [in] */
    sec_bytes_t            *privs,           /* [out] */
    error_status_t         *stp              /* [out] */
)
{
    sec_rgy_handle_t       context;
    unsigned_char_p_t      tcell = NULL;
    unsigned_char_p_t      home_cell;
    sec_bytes_t            *req_privs;
    rpriv_pickle_t         *ptgt_req, *ptgt_rep;
    krb5_ccache            ccache;
    krb5_kdc_req           krb_req;
    krb5_data              krb_rep;
    krb5_creds             *ps_creds = NULL;
    krb5_data              *scratch, scratch2;
    krb5_enctype           etype, etypes[1];
    krb5_checksum          ps_cksum;
    extern krb5_cksumtype  krb5_kdc_req_sumtype;
    krb5_cksumtype         sumtype = krb5_kdc_req_sumtype;
    krb5_timestamp         time_now;
    boolean32              local, ptgt_compat = false;
    sec_rgy_bind_auth_info_t bind_auth_info;
    error_status_t         xst;
    sec_bytes_t            granted_privs, local_privs;
    krb5_authdata          **tgt_authdata = NULL;

    memset(&req_privs, 0, sizeof(req_privs));
    memset(&local_privs, 0, sizeof(local_privs));
    memset(&granted_privs, 0, sizeof(granted_privs));

#define cleanup_tcell() {if (tcell != NULL) rpc_string_free(&tcell, &xst); }

    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }

    CLEAR_STATUS(stp);

    /* get handle on the underlying kerberos credential cache */
    ccache = sec_login_util_get_krb_cache(login_context);
    if (ccache == NULL) {
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }

    /*
     * need to canonicalize the cell name input, just in case...
     */
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, target_cell, &tcell, stp);
    if (BAD_STATUS(stp)) {
       return;
    }


    home_cell = sec_login_util_get_cell(login_context);
    if (home_cell == NULL) {
        cleanup_tcell();
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }
    
    local = u_strcmp(tcell, home_cell) == 0;
    
    /* check the cache for an existing valid PTGT */
    if (sec_priv_ptgt_exists(login_context, tcell, ccache, privs, stp)
    || !STATUS_OK(stp)) {
        cleanup_tcell();
        return;
    }

    /* need current time for several reasons: cred lookups, use as nonce */
    *stp = krb5_timeofday(&time_now);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

    ps_creds = (krb5_creds *) malloc(sizeof(krb5_creds));
    if (ps_creds == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup_tcell();
        return;
    }
    memset(ps_creds, 0, sizeof(*ps_creds));

    /*
     * in any case, the server is the priv server for the target cell
     */
    sec_krb_ptgs_parsed_name( (char *)tcell, &ps_creds->server, stp);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell();

    if (local) {

        /*
         * This is a request for a PTGT in the principal's home cell
         * Construct an authenticator for the home cell priv server
         * request.
         *
         * client = principal@home_cell
         */
        *stp = krb5_cc_get_principal(ccache, &ps_creds->client);

	if (STATUS_OK(stp)) {
	    /* extract identity from login context */
	    req_privs = sec_login_util_get_epac(login_context);
	    if (req_privs == NULL) {
		SET_STATUS(stp, sec_login_s_context_invalid);
	    }
	}
    } else {
	/* 
	 * Our caller may have told us not to recurse ... Only the
	 * the delegation routines in this module can do that.
	 */
 	if (recurse) {
	    /*
	     *  Make a recursive call to get an  EPTGT  in the home cell, 
	     * if necessary.
	     */
	    
	    sec_priv_get_eptgt(login_context, home_cell, authn_svc,
			       authz_svc, &local_privs, stp);
	}

        if (STATUS_OK(stp)) {
	    priv_krb_util_construct_v1_ptgt_authdata(NULL, 0, NULL, NULL,
		&tgt_authdata, NULL, stp);
        }

        if (STATUS_OK(stp)) {
	    priv_krb_util_construct_v1_stkt_authdata(NULL, NULL, &ps_creds->authdata, stp);
        }

	/*
	 * if we get this far, then we've got enough state in the underlying
         * kerberos credential cache to acquire/refresh whatever remote TGTs
         * are required to acquire the ticket {PRIV(home), PRIV(foreign)}
         * that we need to construct an authenticator for the priv server
         * request message.
         */

        if (STATUS_OK(stp)) {
	    /* client = priv@home_cell */
	    sec_krb_ptgs_parsed_name( (char *) home_cell,
				     &ps_creds->client, stp);
	    if (BAD_STATUS(stp)) {
		cleanup();
		return;
	    }
	}
    }

#undef cleanup
#define cleanup() \
    if (tgt_authdata) \
        krb5_free_authdata(tgt_authdata); \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    if (local_privs.num_bytes && local_privs.bytes) { \
        free(local_privs.bytes); \
    }

    if (BAD_STATUS(stp)) {
	cleanup();
	return;
    }

    /*
     * We need to bind to a privilege server in the
     * target cell.  But we have a little problem.
     * We need to know whether we're talking to a 1.1
     * or a 1.0 priv server.  Figuring that out is 
     * not so easy.
     * 
     * We'd like to just try binding with the new 
     * rpriv_v1_1_c_ifspec, and treat the failure to do
     * so as an indication that the priv server does not
     * support the v1_1 interface.  Unfortunately, the
     * rca_site_bind() implementation falls back to the
     * local configuration file (pe_site) when all else fails,
     * and as long as the file contains a string binding
     * for the target cell, the bind succeeds.  Typically
     * the pe_site file contains bindings for the local cell
     * but not for any foreign cells.   So an rca_site_bind()
     * to the local cell will usually succeed regardless
     * of interface spec whereas an rca_site_bind() to a
     * foreign cell will typically fail if the server in
     * that cell is not compatible with the specified interface
     * (though it is not clear what the exact status might be).
     *
     * Here's what we do.
     * 
     * First try binding using the rpriv_v1_1_c_ifspec interface
     * spec.  If that fails, assume the server is v1_0.  If it
     * succeeds, assume the server is v1_1 and later, if the v1_1 
     * operation fails with a status of rpc_s_unknown_if, assume
     * the server is v1_0
     * 
     * 02/05/96 - dce1.1 warranty patch changes
     *
     * Ignore all that commentary above (which is left intact
     * soley for historians of the priv client code).  Since the
     * rpriv interface revision is a minor one, and since the 
     * attempt to bind often succeeds anyway (see comments
     * above re: the pe_site file) regardless of what interface may
     * be registered in the namespace, we just bind using the 1.0
     * interface, and figure out whether or not the server
     * supports the new interface when we actually call it.
     */
    bind_auth_info.info_type = sec_rgy_bind_auth_dce;
    bind_auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    bind_auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    bind_auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_name;
    bind_auth_info.tagged_union.dce_info.identity = login_context;
    rca_site_bind(tcell,
		  &bind_auth_info,
		  rca_NSI_cell,
		  rpriv_v1_0_c_ifspec,
		  SEC_PRIV_SERVER_NAME,
		  (char *) tcell,
		  &context,
		  stp);
    
    if (!STATUS_OK(stp)) {
	cleanup();
	return;
    }

#undef cleanup
#define cleanup() \
    if (tgt_authdata) \
        krb5_free_authdata(tgt_authdata); \
    sec_rgy_site_close(context, &xst); \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    if (local_privs.num_bytes && local_privs.bytes) { \
        free(local_privs.bytes); \
    }

    /*
     * retrieve/acquire a valid ticket to the priv server in the target
     * cell from the cache/kdc
     *
     * NOTE: In the local case, we need a TGT to get a service ticket to
     * the priv server - So tgt_authdata is NULL.
     * In the remote case, we need a PTGT to get a service ticket to
     * the remote priv server - So tgt authdata is not NULL.
     */
    ps_creds->times.endtime = 0;
    *stp = krb5_get_credentials_dce(0, ccache, ps_creds, tgt_authdata);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    /* done with tgt_authdata, in any case */
    if (tgt_authdata)
        krb5_free_authdata(tgt_authdata);

#undef cleanup
#define cleanup() \
    sec_rgy_site_close(context, &xst); \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    if (local_privs.num_bytes && local_privs.bytes) { \
        free(local_privs.bytes); \
    }

    if (!local) {
	/* 
	 * In the intercell case, the identity we need is the identity
	 * granted us in the recursive call to our local priv server,
	 * and that should be in the authorization data of the creds
	 * we just obtained to the foreign privilege service.  If the 
	 * identity info we need isn't in the service ticket creds, 
	 * it's because the local priv server couldn't provide them,
	 * so revert immediately to the old protocol
	 */
         if (local_privs.bytes) {
	     req_privs = &local_privs;
         } else {
	     if (sec_login_util_ptgt_compat(login_context, tcell, stp)) {
	         sec_priv_get_ptgt_internal(login_context, target_cell, 
				            authn_svc, authz_svc, 
					    false /* no recursion */, stp);
	     }
	     if (privs) {
	         privs->num_bytes = 0;
		 privs->bytes = NULL;
	     }
	     cleanup();
	     return;
         }
    }

    if (BAD_STATUS(stp)) {
	cleanup();
	return;
    }
	
    /* cons up a krb5_kdc_req with an ap_req authentication header  */

    memset(&krb_req, 0, sizeof(krb_req));

    krb_req.msg_type = KRB5_TGS_REQ;

    /*
     * choose an encryption type for message exchange
     */
    priv_pvt_get_etype(ps_creds, &etype, stp);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    etypes[0] = etype;
    krb_req.etype = etypes;
    krb_req.netypes = 1;

    /*
     * none of those fancy kdc options apply to priv tickets - so leave
     * the kdc_options field zeroed out
     */

    /*
     * client and server fields not used - but the ASN.1 encoding of
     * a KRB5_KDC_REQ insists that the server field contain something
     * real, so we stuff the priv server principal there to make it happy
     * 
     * !!! for architectural cleanliness reasons (symmetry with the
     * kerberos authentication service request), we probably should have
     * put the TGS server name here, but it may be too late to change that.
     */
    krb_req.server = ps_creds->server;

    /*
     * use times from the ticket to the priv server.
     */
    krb_req.from = ps_creds->times.starttime;
    krb_req.till = ps_creds->times.endtime;
    krb_req.rtime = ps_creds->times.renew_till;

    krb_req.nonce = (krb5_int32) time_now;

    /* use adresses from ticket to priv server */
    krb_req.addresses = ps_creds->addresses;

    *stp = encode_krb5_kdc_req_body(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ps_cksum.contents = (krb5_octet *)
        malloc(krb5_cksumarray[sumtype]->checksum_length);
    if (ps_cksum.contents == NULL) {
        krb5_free_data(scratch);
        cleanup();
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }

    *stp = (*(krb5_cksumarray[sumtype]->
                     sum_func))(scratch->data,
                                scratch->length,
                                (krb5_pointer) ps_creds->keyblock.contents,
                                ps_creds->keyblock.length,
                                &ps_cksum);

    /* done with scratch, regardless */
    krb5_free_data(scratch);

    if (BAD_STATUS(stp)) {
        xfree(ps_cksum.contents);
        cleanup();
        return;
    }

#undef cleanup
#define cleanup() \
    sec_rgy_site_close(context, &xst); \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    cleanup_tcell(); \
    if (local_privs.num_bytes && local_privs.bytes) { \
        free(local_privs.bytes); \
    }

    *stp = krb5_mk_req_extended (0L /* no ap options */,
                                &ps_cksum,
                                0L, /* don't need kdc_options for this */
                                0, /* no initial sequence */
                                0, /* no subkey--we're using the session key */
                                0, /* no ccache--already have creds */
                                ps_creds,
                                0, /* don't need authenticator */
                                &scratch2);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    krb_req.padata = (krb5_pa_data **) calloc(2, sizeof(krb5_pa_data *));
    if (krb_req.padata == NULL) {
        cleanup();
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0] = (krb5_pa_data *) malloc(sizeof(krb5_pa_data));
    if (krb_req.padata[0] == NULL) {
        cleanup();
        free((char *)krb_req.padata);
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0]->pa_type = KRB5_PADATA_AP_REQ;
    krb_req.padata[0]->length = scratch2.length;
    krb_req.padata[0]->contents = (krb5_octet *) scratch2.data;

     /*
      * scratch2 is now dedicated to holding data for element2 of our
      * pre-authentication data array.  By freeing the padata, we also
      * free scratch2.data
      */
#undef cleanup
#define cleanup() \
    sec_rgy_site_close(context, &xst); \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    cleanup_tcell(); \
    krb5_free_pa_data(krb_req.padata); \
    if (local_privs.num_bytes && local_privs.bytes) { \
        free(local_privs.bytes); \
    }

    /*
     * Finally, encode the priv server TGS_REQ and stuff it into a pickle
     */
    *stp = encode_krb5_tgs_req(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ptgt_req = malloc (sizeof(rpriv_pickle_t) + scratch->length);
    if (ptgt_req == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup();
        krb5_free_data(scratch);
        return;
    }

    memcpy(ptgt_req->bytes, scratch->data, scratch->length);
    ptgt_req->num_bytes = scratch->length;
    /*
     * done with request data, except for subkey,
     * and session key (in ps_creds)
     */
    xfree(ps_cksum.contents);
    krb5_free_data(scratch);
    krb5_free_pa_data(krb_req.padata);

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    if (local_privs.num_bytes && local_privs.bytes) { \
        free(local_privs.bytes); \
    }

    SETUP_RETRY(rca_op_read, stp) {
        rpriv_attr_request_t    aux_req;
        rpriv_attr_result_t     aux_res;
        unsigned_char_p_t       tkt_req = NULL;
        rpriv_app_tkt_result_t  tkt_res;

	aux_req.num_aux_attr_keys = 0;
	aux_req.aux_attr_keys = NULL;
	Crpriv_get_eptgt(context, authn_svc, authz_svc, req_privs,
			 ptgt_req, &ptgt_rep, &granted_privs, &aux_req,
			 &aux_res, tkt_req, &tkt_res, stp);
	if (STATUS_EQUAL(stp, rpc_s_unknown_if)
	|| STATUS_EQUAL(stp, ept_s_not_registered)) {
	    ABANDON_RETRY;
        }
        NORMAL_CASE;
        RETRY_CASE;
    }
    RETRY_END;

    { error_status_t lst; sec_rgy_site_close(context, &lst); }
    free(ptgt_req);

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    if (local_privs.num_bytes && local_privs.bytes) { \
        free(local_privs.bytes); \
    } \
    if (granted_privs.num_bytes && granted_privs.bytes) { \
        rpc_ss_client_free(granted_privs.bytes); \
    }

    if (STATUS_EQUAL(stp, rpc_s_unknown_if)
    || STATUS_EQUAL(stp, ept_s_not_registered)) {

	/* 
	 * Priv server in the target cell doesn't support 
	 * v1_1, so try the only other protocol we know--the
	 * v1_0 protocol
	 */
	if (sec_login_util_ptgt_compat(login_context, tcell, stp)) {
	    sec_priv_get_ptgt_internal(login_context, target_cell, 
				       authn_svc, authz_svc, 
				       false /* no recursion */, stp);
	}
	if (privs) {
	    privs->num_bytes = 0;
	    privs->bytes = NULL;
	}
	cleanup();
	return;
    }

    if (STATUS_OK(stp)) {
        /* decrypt the encoded krb_rep containing the privilege ticket */
        krb_rep.length = ptgt_rep->num_bytes;
        krb_rep.data = (char *) ptgt_rep->bytes;

	sec_priv_store_ptgt(login_context, target_cell, &krb_rep, 
		&ps_creds->keyblock, &granted_privs, ccache,
		sec_login_util_get_v1_pac_flag(login_context),
		privs, stp);
        /* done with ndr encoded response */
        rpc_ss_client_free((idl_void_p_t) ptgt_rep);
    }

    krb5_free_creds(ps_creds);
    cleanup_tcell();
    if (local_privs.num_bytes && local_privs.bytes) {
        free(local_privs.bytes);
    }
    if (granted_privs.num_bytes && granted_privs.bytes) {
        rpc_ss_client_free(granted_privs.bytes);
    }

#undef cleanup
}


/*
 * s e c _ p r i v _ g e t _ e p t g t
 */
void  sec_priv_get_eptgt(
    sec_login_handle_t     login_context,    /* [in] */
    unsigned_char_p_t      target_cell,      /* [in] */
    unsigned32             authn_svc,        /* [in] */
    unsigned32             authz_svc,        /* [in] */
    sec_bytes_t            *privs,           /* [out] */
    error_status_t         *stp              /* [out] */
) 
{
    sec_priv_get_eptgt_internal(login_context, target_cell,
				authn_svc, authz_svc, true,
				privs, stp);
}


/*
 * s e c _ p r i v _ b e c o m e _ d e l e g a t e
 */
void  sec_priv_become_delegate (
    sec_login_handle_t     	login_context,  /* [in] */
    unsigned_char_p_t      	target_cell,    /* [in] */
    unsigned32             	authn_svc,      /* [in] */
    unsigned32             	authz_svc,      /* [in] */
    sec_id_delegation_type_t    dlg_type,       /* [in] */
    sec_id_restriction_set_t    *dlg_rstrs,     /* [in] */
    sec_id_restriction_set_t    *tgt_rstrs,     /* [in] */
    sec_id_opt_req_t            *opt_rstrs,     /* [in] */
    sec_id_opt_req_t            *req_rstrs,     /* [in] */
    sec_id_compatibility_mode_t compat_mode,    /* [in] */
    sec_bytes_t                 *dlg_chain,     /* [in] */
    sec_bytes_t                 *dlg,           /* [in] */
    sec_dlg_token_set_t         *dlg_token_set, /* [in] */
    sec_bytes_t			*privs,		/* [out] */
    error_status_t         	*stp            /* [out] */
)
{
    sec_rgy_handle_t       context;
    unsigned_char_p_t      tcell = NULL, home_cell;
    rpriv_pickle_t         *ptgt_req, *ptgt_rep;
    krb5_ccache            ccache;
    krb5_kdc_req           krb_req;
    krb5_data              krb_rep;
    krb5_encrypt_block     skey_eblock;
    krb5_creds             *ps_creds = NULL;
    krb5_data              *scratch, scratch2;
    krb5_enctype           etype, etypes[1];
    krb5_checksum          ps_cksum;
    extern krb5_cksumtype  krb5_kdc_req_sumtype;
    krb5_cksumtype         sumtype = krb5_kdc_req_sumtype;
    krb5_timestamp         time_now;
    boolean32              local;
    sec_rgy_bind_auth_info_t bind_auth_info;
    error_status_t         xst;
    sec_bytes_t            granted_privs;
    sec_encrypted_bytes_t  enc_dlg_token;

#define cleanup_tcell() {if (tcell != NULL) rpc_string_free(&tcell, &xst); }

    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }
    
    *stp = check_delegation_chain(dlg_chain, sec_id_deleg_type_traced);
    if (STATUS_OK(stp)) {
	*stp = check_delegation_token_set(dlg_token_set);
    }
    if (!STATUS_OK(stp)) return;
    

    CLEAR_STATUS(stp);

    /* get handle on the underlying kerberos credential cache */
    ccache = sec_login_util_get_krb_cache(login_context);
    if (ccache == NULL) {
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }

    /*
     * need to canonicalize the cell name input, just in case...
     */
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, target_cell, &tcell, stp);
    if (BAD_STATUS(stp)) {
       return;
    }

    home_cell = sec_login_util_get_cell(login_context);
    if (home_cell == NULL) {
        cleanup_tcell();
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }

    local = u_strcmp(tcell, home_cell) == 0;
 
    /* check the cache for an existing valid PTGT */
    if (sec_priv_ptgt_exists(login_context, tcell, ccache, privs, stp)
    || !STATUS_OK(stp)) {
        cleanup_tcell();
        return;
    }

    /* need current time for several reasons: cred lookups, use as nonce */
    *stp = krb5_timeofday(&time_now);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

    ps_creds = (krb5_creds *) malloc(sizeof(krb5_creds));
    if (ps_creds == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup_tcell();
        return;
    }
    memset(ps_creds, 0, sizeof(*ps_creds));

    /*
     * in any case, the server is the priv server for the target cell
     */
    sec_krb_ptgs_parsed_name( (char *)tcell, &ps_creds->server, stp);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell();

    if (local) {

        /*
         * This is a request for a PTGT in the principal's home cell
         * Construct an authenticator for the home cell priv server
         * request.
         *
         * client = principal@home_cell
         */
        *stp = krb5_cc_get_principal(ccache, &ps_creds->client);
        if (BAD_STATUS(stp)) {
            cleanup();
            return;
        }

    } else {
        /*
         * This is a request for a foreign cell EPTGT.  Make a recursive
         * call to get a delegated EPTGT in the home cell, if necessary.
         */
	sec_priv_become_delegate(login_context,
				 home_cell,
				 authn_svc,
				 authz_svc,
				 dlg_type,
				 dlg_rstrs,
				 tgt_rstrs,
				 opt_rstrs,
				 req_rstrs,
				 compat_mode,
				 dlg_chain,
				 dlg,
				 dlg_token_set,
				 NULL,
				 stp);

	if (STATUS_OK(stp)) {
	    /* 
	     * We have a v1_1 EPTGT with delegation enabled.  All
	     * intercell requests, without or without delegation,
	     * are accomplished using the rpriv_get_eptgt() wire
	     * operation, so we can just call sec_priv_get_eptgt()
	     * to accomplish that.   We use the internal version 
	     * so we can disable the recursion that would normally
	     * normally occur in that routine.
	     */
	    sec_priv_get_eptgt_internal(login_context, tcell, authn_svc,
					authz_svc, false, privs, stp);
	}
	cleanup();
	return;
    }

    /*
     * retrieve/acquire a valid ticket to the priv server in the target
     * cell from the cache/kdc
     */
    ps_creds->times.endtime = 0;
    *stp = krb5_get_credentials_dce(0, ccache, ps_creds, NULL);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    /* cons up a krb5_kdc_req with an ap_req authentication header  */
    memset(&krb_req, 0, sizeof(krb_req));
    krb_req.msg_type = KRB5_TGS_REQ;

    /*
     * choose an encryption type for message exchange
     */
    priv_pvt_get_etype(ps_creds, &etype, stp);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    etypes[0] = etype;
    krb_req.etype = etypes;
    krb_req.netypes = 1;

    krb5_use_cstype(&skey_eblock, etype);
    *stp = krb5_process_key(&skey_eblock, &ps_creds->keyblock);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    (void) krb5_finish_key(&skey_eblock);

    /*
     * none of those fancy kdc options apply to priv tickets - so leave
     * leave the kdc_options field zeroed out
     */

    /* client and server fields not used - but the ASN.1 encoding of
     * a KRB5_KDC_REQ insists that the server field contain something
     * real, so we stuff the priv server principal there to make it happy
     */
    krb_req.server = ps_creds->server;

    /*
     * use times from the ticket to the priv server.
     */
    krb_req.from = ps_creds->times.starttime;
    krb_req.till = ps_creds->times.endtime;
    krb_req.rtime = ps_creds->times.renew_till;

    krb_req.nonce = (krb5_int32) time_now;

    /* use adresses from ticket to priv server */
    krb_req.addresses = ps_creds->addresses;

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell(); \

    *stp = encode_krb5_kdc_req_body(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ps_cksum.contents = (krb5_octet *)
        malloc(krb5_cksumarray[sumtype]->checksum_length);
    if (ps_cksum.contents == NULL) {
        krb5_free_data(scratch);
        cleanup();
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }

    *stp = (*(krb5_cksumarray[sumtype]->
                     sum_func))(scratch->data,
                                scratch->length,
                                (krb5_pointer) ps_creds->keyblock.contents,
                                ps_creds->keyblock.length,
                                &ps_cksum);

    /* done with scratch, regardless */
    krb5_free_data(scratch);

    if (BAD_STATUS(stp)) {
        xfree(ps_cksum.contents);
        cleanup();
        return;
    }

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    cleanup_tcell(); \
    (void) krb5_finish_key(&skey_eblock);

    *stp = krb5_mk_req_extended (0L /* no ap options */,
                                &ps_cksum,
                                0L, /* don't need kdc_options for this */
                                0, /* no initial sequence */
                                0, /* no subkey--we're using the session key */
                                0, /* no ccache--already have creds */
                                ps_creds,
                                0, /* don't need authenticator */
                                &scratch2);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    krb_req.padata = (krb5_pa_data **) calloc(2, sizeof(krb5_pa_data *));
    if (krb_req.padata == NULL) {
        cleanup();
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0] = (krb5_pa_data *) malloc(sizeof(krb5_pa_data));
    if (krb_req.padata[0] == NULL) {
        cleanup();
        free((char *)krb_req.padata);
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0]->pa_type = KRB5_PADATA_AP_REQ;
    krb_req.padata[0]->length = scratch2.length;
    krb_req.padata[0]->contents = (krb5_octet *) scratch2.data;

     /*
      * scratch2 is now dedicated to holding data for element2 of our
      * pre-authentication data array.  By freeing the padata, we also
      * free scratch2.data
      */
#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell(); \
    krb5_free_pa_data(krb_req.padata);

    /*
     * Finally, encode the priv server TGS_REQ and stuff it into a pickle
     */
    *stp = encode_krb5_tgs_req(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ptgt_req = malloc (sizeof(rpriv_pickle_t) + scratch->length);
    if (ptgt_req == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup();
        krb5_free_data(scratch);
        return;
    }

    memcpy(ptgt_req->bytes, scratch->data, scratch->length);
    ptgt_req->num_bytes = scratch->length;
    /*
     * done with request data, except for subkey,
     * and session key (in ps_creds)
     */
    xfree(ps_cksum.contents);
    krb5_free_data(scratch);
    krb5_free_pa_data(krb_req.padata);

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell(); \
    if (ptgt_req) free(ptgt_req);

    /* Delegation token */
    enc_dlg_token.etype = etype;
    enc_dlg_token.ekvno = 0;
    priv_krb_util_enc_dlg_token(&skey_eblock, dlg_token_set, &enc_dlg_token,
        stp);
    if (BAD_STATUS(stp)) {
	cleanup();
        return;
    }

    /* bind to a priv server in the appropriate cell - need to use an
     * appropriate internal binding function and bind by priv server
     * interface uuid.  And use real auth RPC.
     *
     * In the case of delegation, we only reach this point in the 
     * local case, because all intercell  requests are made using
     * sec_priv_get_eptgt().
     */
    bind_auth_info.info_type = sec_rgy_bind_auth_dce;
    bind_auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    bind_auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    bind_auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_name;
    bind_auth_info.tagged_union.dce_info.identity = login_context;
    rca_site_bind(tcell,
                    &bind_auth_info,
                    rca_NSI_cell,
                    rpriv_v1_0_c_ifspec,
                    SEC_PRIV_SERVER_NAME,
                    (char *) tcell,
                    &context,
                    stp);

    if (BAD_STATUS(stp)) {
	cleanup();
        return;
    }

    SETUP_RETRY(rca_op_read, stp) {
        rpriv_attr_request_t    aux_req;
        rpriv_attr_result_t     aux_res;
        unsigned_char_p_t       tkt_req = NULL;
        rpriv_app_tkt_result_t  tkt_res;

        /* XXX Dummy/Empty data.  Remove when these params are really used */
	aux_req.num_aux_attr_keys = 0;
        aux_req.aux_attr_keys = NULL;

        Crpriv_become_delegate(context, authn_svc, authz_svc, dlg_type,
            dlg_rstrs, tgt_rstrs, opt_rstrs, req_rstrs, compat_mode,
            dlg_chain, dlg, &enc_dlg_token, ptgt_req, &ptgt_rep,
            &granted_privs, &aux_req, &aux_res, tkt_req, &tkt_res, stp);

	if (STATUS_EQUAL(stp, rpc_s_unknown_if)
	|| STATUS_EQUAL(stp, ept_s_not_registered)) {
	    ABANDON_RETRY;
        }
        NORMAL_CASE;
        RETRY_CASE;
    }
    RETRY_END;

    { error_status_t lst; sec_rgy_site_close(context, &lst); }
    free(ptgt_req);

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell(); \
    if (granted_privs.num_bytes && granted_privs.bytes) { \
        rpc_ss_client_free(granted_privs.bytes); \
    }

    if (STATUS_EQUAL(stp, rpc_s_unknown_if)
    || STATUS_EQUAL(stp, ept_s_not_registered)) {
	/* 
	 * Priv server in the target cell doesn't support 
	 * v1_1, so try the only other protocol we know--the
	 * v1_0 protocol
	 */
	if (sec_login_util_ptgt_compat(login_context, tcell, stp)) {
	    sec_priv_get_ptgt_internal(login_context, target_cell, 
				       authn_svc, authz_svc, 
				       false /* no recursion */, stp);
	}
	cleanup();
	return;
    }

    if (STATUS_OK(stp)) {
        /* decrypt the encoded krb_rep containing the privilege ticket */
        krb_rep.length = ptgt_rep->num_bytes;
        krb_rep.data = (char *) ptgt_rep->bytes;

	sec_priv_store_ptgt(login_context, target_cell, &krb_rep, 
		&ps_creds->keyblock, &granted_privs, ccache,
		sec_login_util_get_v1_pac_flag(login_context),
		privs, stp);
        /* done with ndr encoded response */
        rpc_ss_client_free((idl_void_p_t) ptgt_rep);
    }

    krb5_free_creds(ps_creds);
    cleanup_tcell();
    (void) krb5_finish_key(&skey_eblock);
    if (granted_privs.num_bytes && granted_privs.bytes) {
        rpc_ss_client_free(granted_privs.bytes);
    }
#undef cleanup
}


/*
 * s e c _ p r i v _ b e c o m e _ i m p e r s o n a t o r
 */
void  sec_priv_become_impersonator(
    sec_login_handle_t     	login_context,  /* [in] */
    unsigned_char_p_t      	target_cell,    /* [in] */
    unsigned32             	authn_svc,      /* [in] */
    unsigned32             	authz_svc,      /* [in] */
    sec_id_delegation_type_t    dlg_type,       /* [in] */
    sec_id_restriction_set_t    *dlg_rstrs,     /* [in] */
    sec_id_restriction_set_t    *tgt_rstrs,     /* [in] */
    sec_id_opt_req_t            *opt_rstrs,     /* [in] */
    sec_id_opt_req_t            *req_rstrs,     /* [in] */
    sec_bytes_t                 *dlg_chain,     /* [in] */
    sec_bytes_t                 *impersonator,  /* [in] */
    sec_dlg_token_set_t         *dlg_token_set, /* [in] */
    sec_bytes_t			*privs,		/* [out] */
    error_status_t         	*stp            /* [out] */
)
{
    sec_rgy_handle_t       context;
    unsigned_char_p_t      tcell = NULL, home_cell;
    sec_id_pac_t           *identity;
    rpriv_pickle_t         *ptgt_req, *ptgt_rep;
    krb5_ccache            ccache;
    krb5_kdc_req           krb_req;
    krb5_data              krb_rep;
    krb5_encrypt_block     skey_eblock;
    krb5_creds             *ps_creds = NULL;
    krb5_data              *scratch, scratch2;
    krb5_enctype           etype, etypes[1];
    krb5_checksum          ps_cksum;
    extern krb5_cksumtype  krb5_kdc_req_sumtype;
    krb5_cksumtype         sumtype = krb5_kdc_req_sumtype;
    krb5_timestamp         time_now;
    boolean32              local;
    sec_rgy_bind_auth_info_t bind_auth_info;
    error_status_t         xst;
    sec_bytes_t            granted_privs;
    sec_encrypted_bytes_t  enc_dlg_token;

#define cleanup_tcell() {if (tcell != NULL) rpc_string_free(&tcell, &xst); }

    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }
    
    *stp = check_delegation_chain(dlg_chain, sec_id_deleg_type_impersonation);
    if (STATUS_OK(stp)) {
	*stp = check_delegation_token_set(dlg_token_set);
    }
    if (!STATUS_OK(stp)) return;
    

    CLEAR_STATUS(stp);

    /* get handle on the underlying kerberos credential cache */
    ccache = sec_login_util_get_krb_cache(login_context);
    if (ccache == NULL) {
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }

    /*
     * need to canonicalize the cell name input, just in case...
     */
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, target_cell, &tcell, stp);
    if (BAD_STATUS(stp)) {
       return;
    }

    home_cell = sec_login_util_get_cell(login_context);
    if (home_cell == NULL) {
        cleanup_tcell();
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }

    local = u_strcmp(tcell, home_cell) == 0;

    /* check the cache for an existing valid PTGT */
    if (sec_priv_ptgt_exists(login_context, tcell, ccache, privs, stp)
    || !STATUS_OK(stp)) {
        cleanup_tcell();
        return;
    }

     /* need current time for several reasons: cred lookups, use as nonce */
    *stp = krb5_timeofday(&time_now);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

    ps_creds = (krb5_creds *) malloc(sizeof(krb5_creds));
    if (ps_creds == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup_tcell();
        return;
    }
    memset(ps_creds, 0, sizeof(*ps_creds));

    /*
     * in any case, the server is the priv server for the target cell
     */
    sec_krb_ptgs_parsed_name( (char *)tcell, &ps_creds->server, stp);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell();

    /*
     * A delegation or impersonation request cannot cross a
     * cell boundary, but, once obtained from the local priv server,
     * EPACs containing delegation chains can be used to obtain
     * foreign EPTGTs  via the remote rpriv_get_eptgt() interface.
     *
     * So, an intercell impersonation or delegation requests results
     * in the following wire operations.
     *
     * rpriv_become_delegate() or rpriv_become_impersonator() to 
     * the local priv server to obtain/refresh the local EPTGT with 
     * delegation/impersonation in use, followed by rpriv_get_eptgt() 
     * to the foreign priv server to obtain an EPTGT in the foreign cell.
     */ 
    if (local) {

        /*
         * This is a request for a PTGT in the principal's home cell
         * Construct an authenticator for the home cell priv server
         * request.
         *
         * client = principal@home_cell
         */
        *stp = krb5_cc_get_principal(ccache, &ps_creds->client);
        if (BAD_STATUS(stp)) {
            cleanup();
            return;
        }

    } else {
        /*
         * This is a request for a foreign cell EPTGT.  Make a recursive
         * call to get a delegated EPTGT in the home cell, if necessary.
         */
	sec_priv_become_impersonator(login_context,
				     home_cell,
				     authn_svc,
				     authz_svc,
				     dlg_type,
				     dlg_rstrs,
				     tgt_rstrs,
				     opt_rstrs,
				     req_rstrs,
				     dlg_chain,
				     impersonator,
				     dlg_token_set,
				     NULL,
				     stp);

	if (STATUS_OK(stp)) {
	    /* 
	     * We have a v1_1 EPTGT with delegation enabled.  All
	     * intercell requests, without or without delegation,
	     * are accomplished using the rpriv_get_eptgt() wire
	     * operation, so we can just call sec_priv_get_eptgt()
	     * to accomplish that.   We use the internal version 
	     * so we can disable the recursion that would normally
	     * normally occur in that routine.
	     */
	    sec_priv_get_eptgt_internal(login_context, tcell, authn_svc,
					authz_svc, false, privs, stp);
	}
	cleanup();
	return;
    }

    /*
     * retrieve/acquire a valid ticket to the priv server in the target
     * cell from the cache/kdc
     */
    ps_creds->times.endtime = 0;
    *stp = krb5_get_credentials_dce(0, ccache, ps_creds, NULL);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    /* cons up a krb5_kdc_req with an ap_req authentication header  */
    memset(&krb_req, 0, sizeof(krb_req));
    krb_req.msg_type = KRB5_TGS_REQ;

    /*
     * choose an encryption type for message exchange
     */
    priv_pvt_get_etype(ps_creds, &etype, stp);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    etypes[0] = etype;
    krb_req.etype = etypes;
    krb_req.netypes = 1;

    krb5_use_cstype(&skey_eblock, etype);
    *stp = krb5_process_key(&skey_eblock, &ps_creds->keyblock);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    (void) krb5_finish_key(&skey_eblock);

    /*
     * none of those fancy kdc options apply to priv tickets - so leave
     * leave the kdc_options field zeroed out     */

    /* client and server fields not used - but the ASN.1 encoding of
     * a KRB5_KDC_REQ insists that the server field contain something
     * real, so we stuff the priv server principal there to make it happy
     */
    krb_req.server = ps_creds->server;

    /*
     * use times from the ticket to the priv server.
     */
    krb_req.from = ps_creds->times.starttime;
    krb_req.till = ps_creds->times.endtime;
    krb_req.rtime = ps_creds->times.renew_till;

    krb_req.nonce = (krb5_int32) time_now;

    /* use adresses from ticket to priv server */
    krb_req.addresses = ps_creds->addresses;

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell(); 

    *stp = encode_krb5_kdc_req_body(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ps_cksum.contents = (krb5_octet *)
        malloc(krb5_cksumarray[sumtype]->checksum_length);
    if (ps_cksum.contents == NULL) {
        krb5_free_data(scratch);
        cleanup();
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }

    *stp = (*(krb5_cksumarray[sumtype]->
                     sum_func))(scratch->data,
                                scratch->length,
                                (krb5_pointer) ps_creds->keyblock.contents,
                                ps_creds->keyblock.length,
                                &ps_cksum);

    /* done with scratch, regardless */
    krb5_free_data(scratch);

    if (BAD_STATUS(stp)) {
        xfree(ps_cksum.contents);
        cleanup();
        return;
    }

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    cleanup_tcell(); \
    (void) krb5_finish_key(&skey_eblock);

    *stp = krb5_mk_req_extended (0L /* no ap options */,
                                &ps_cksum,
                                0L, /* don't need kdc_options for this */
                                0, /* no initial sequence */
                                0, /* no subkey--we're using the session key */
                                0, /* no ccache--already have creds */
                                ps_creds,
                                0, /* don't need authenticator */
                                &scratch2);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    krb_req.padata = (krb5_pa_data **) calloc(2, sizeof(krb5_pa_data *));
    if (krb_req.padata == NULL) {
        cleanup();
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0] = (krb5_pa_data *) malloc(sizeof(krb5_pa_data));
    if (krb_req.padata[0] == NULL) {
        cleanup();
        free((char *)krb_req.padata);
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0]->pa_type = KRB5_PADATA_AP_REQ;
    krb_req.padata[0]->length = scratch2.length;
    krb_req.padata[0]->contents = (krb5_octet *) scratch2.data;

     /*
      * scratch2 is now dedicated to holding data for element2 of our
      * pre-authentication data array.  By freeing the padata, we also
      * free scratch2.data
      */
#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell(); \
    krb5_free_pa_data(krb_req.padata);

    /*
     * Finally, encode the priv server TGS_REQ and stuff it into a pickle
     */
    *stp = encode_krb5_tgs_req(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ptgt_req = malloc (sizeof(rpriv_pickle_t) + scratch->length);
    if (ptgt_req == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup();
        krb5_free_data(scratch);
        return;
    }

    memcpy(ptgt_req->bytes, scratch->data, scratch->length);
    ptgt_req->num_bytes = scratch->length;
    /*
     * done with request data, except for subkey,
     * and session key (in ps_creds)
     */
    xfree(ps_cksum.contents);
    krb5_free_data(scratch);
    krb5_free_pa_data(krb_req.padata);

    /* Delegation token */
    enc_dlg_token.etype = etype;
    enc_dlg_token.ekvno = 0;
    priv_krb_util_enc_dlg_token(&skey_eblock, dlg_token_set, &enc_dlg_token,
        stp);
    if (BAD_STATUS(stp)) {
        krb5_free_creds(ps_creds);
        (void) krb5_finish_key(&skey_eblock);
        return;
    }

    /* bind to a priv server in the appropriate cell - need to use an
     * appropriate internal binding function and bind by priv server
     * interface uuid.  And use real auth RPC.
     *
     * In the case of delegation, we only reach this point in the 
     * local case, because all intercell  requests are made using
     * sec_priv_get_eptgt().
     */
    bind_auth_info.info_type = sec_rgy_bind_auth_dce;
    bind_auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
    bind_auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_secret;
    bind_auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_name;
    bind_auth_info.tagged_union.dce_info.identity = login_context;
    rca_site_bind(tcell,
                    &bind_auth_info,
                    rca_NSI_cell,
                    rpriv_v1_0_c_ifspec,
                    SEC_PRIV_SERVER_NAME,
                    (char *) tcell,
                    &context,
                    stp);

    if (BAD_STATUS(stp)) {
        krb5_free_creds(ps_creds);
        (void) krb5_finish_key(&skey_eblock);
        return;
    }

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell();

    SETUP_RETRY(rca_op_read, stp) {
        rpriv_attr_request_t    aux_req;
        rpriv_attr_result_t     aux_res;
        unsigned_char_p_t       tkt_req = NULL;
        rpriv_app_tkt_result_t  tkt_res;

        /* XXX Dummy/Empty data.  Remove when these params are really used */
	aux_req.num_aux_attr_keys = 0;
        aux_req.aux_attr_keys = NULL;

        Crpriv_become_impersonator(context, authn_svc, authz_svc, dlg_type,
            dlg_rstrs, tgt_rstrs, opt_rstrs, req_rstrs, dlg_chain, 
	    impersonator, &enc_dlg_token, ptgt_req, &ptgt_rep,
            &granted_privs, &aux_req, &aux_res, tkt_req, &tkt_res, stp);

	if (STATUS_EQUAL(stp, rpc_s_unknown_if)
	|| STATUS_EQUAL(stp, ept_s_not_registered)) {
	    ABANDON_RETRY;
	}

        NORMAL_CASE;
        RETRY_CASE;
    }
    RETRY_END;

    { error_status_t lst; sec_rgy_site_close(context, &lst); }
    free(ptgt_req);

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    (void) krb5_finish_key(&skey_eblock); \
    cleanup_tcell(); \
    if (granted_privs.num_bytes && granted_privs.bytes) { \
        rpc_ss_client_free(granted_privs.bytes); \
    }

    if (STATUS_EQUAL(stp, rpc_s_unknown_if)
	|| STATUS_EQUAL(stp, ept_s_not_registered)) {
	/* 
	 * Priv server in the target cell doesn't support 
	 * v1_1, so try the only other protocol we know--the
	 * v1_0 protocol
	 */
	if (sec_login_util_ptgt_compat(login_context, tcell, stp)) {
	    sec_priv_get_ptgt_internal(login_context, target_cell, 
				       authn_svc, authz_svc, 
				       false /* no recursion */, stp);
	}
	cleanup();
	return;
    }

    if (STATUS_OK(stp)) {
        /* decrypt the encoded krb_rep containing the privilege ticket */
        krb_rep.length = ptgt_rep->num_bytes;
        krb_rep.data = (char *) ptgt_rep->bytes;

	sec_priv_store_ptgt(login_context, target_cell, &krb_rep, 
		&ps_creds->keyblock, &granted_privs, ccache, 
		sec_login_util_get_v1_pac_flag(login_context),
		privs, stp);
        /* done with ndr encoded response */
        rpc_ss_client_free((idl_void_p_t) ptgt_rep);
    }

    krb5_free_creds(ps_creds);
    cleanup_tcell();
    (void) krb5_finish_key(&skey_eblock);
    if (granted_privs.num_bytes && granted_privs.bytes) {
        rpc_ss_client_free(granted_privs.bytes);
    }
#undef cleanup
}


/*
 * s e c _ p r i v _ g e t _ p t g t
 *
 */
void  sec_priv_get_ptgt(
    sec_login_handle_t     login_context,    /* [in] */
    unsigned_char_p_t      target_cell,      /* [in] */
    unsigned32             authn_svc,        /* [in] */
    unsigned32             authz_svc,        /* [in] */
    error_status_t         *stp              /* [out] */
)
{

    sec_priv_get_ptgt_internal(login_context, target_cell, 
			       authn_svc, authz_svc, true, stp);
}

static void  sec_priv_get_ptgt_internal(
    sec_login_handle_t     login_context,    /* [in] */
    unsigned_char_p_t      target_cell,      /* [in] */
    unsigned32             authn_svc,        /* [in] */
    unsigned32             authz_svc,        /* [in] */
    boolean32              recurse,          /* [out] */
    error_status_t         *stp              /* [out] */
)
{


    sec_rgy_handle_t       context;  
    unsigned_char_p_t      tcell = NULL;
    sec_id_pac_t           *identity;
    rpriv_pickle_t         *ptgt_req, *ptgt_rep;
    krb5_ccache            ccache;
    krb5_kdc_req           krb_req;
    krb5_data              krb_rep;
    krb5_encrypt_block     skey_eblock;
    krb5_creds             *ps_creds = NULL;
    krb5_data              *scratch, scratch2;
    krb5_enctype           etype, etypes[1];
    krb5_checksum          ps_cksum;
    extern krb5_cksumtype  krb5_kdc_req_sumtype;
    krb5_cksumtype         sumtype = krb5_kdc_req_sumtype;
    krb5_timestamp         time_now;
    boolean32              local;
    sec_rgy_bind_auth_info_t bind_auth_info;
    error_status_t         xst;

#define cleanup_tcell() {if (tcell != NULL) rpc_string_free(&tcell, &xst); }

    if (authz_svc != rpc_c_authz_dce) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }

    if (authn_svc != rpc_c_authn_dce_private) {
        SET_STATUS(stp, sec_priv_s_invalid_authz_svc);
        return;
    }

    CLEAR_STATUS(stp);

    /* get handle on the underlying kerberos credential cache */
    ccache = sec_login_util_get_krb_cache(login_context);
    if (ccache == NULL) {
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }

    /*
     * need to canonicalize the cell name input, just in case...
     */
    rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, target_cell, &tcell, stp);
    if (BAD_STATUS(stp)) {
       return;
    }

    /* extract identity from login context */
    identity = sec_login_util_get_pac(login_context);
    if (identity == NULL) {
        cleanup_tcell();
        SET_STATUS(stp, sec_login_s_context_invalid);
        return;
    }
    local = u_strcmp(tcell, identity->realm.name) == 0;

    /* check the cache for an existing valid PTGT */
    if (sec_priv_ptgt_exists(login_context, tcell, ccache, NULL, stp)) {
	cleanup_tcell();
	return;
    }

    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

    /* need current time for several reasons: cred lookups, use as nonce */
    *stp = krb5_timeofday(&time_now);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }

    ps_creds = (krb5_creds *) malloc(sizeof(krb5_creds));
    if (ps_creds == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup_tcell();
        return;
    }
    memset(ps_creds, 0, sizeof(*ps_creds));

    /*
     * in any case, the server is the priv server for the target cell
     */
    sec_krb_ptgs_parsed_name( (char *)tcell, &ps_creds->server, stp);
    if (BAD_STATUS(stp)) {
        cleanup_tcell();
        return;
    }


#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell();

    if (local) {

        /*
         * This is a request for a PTGT in the principal's home cell
         * Construct an authenticator for the home cell priv server
         * request.
         *
         * client = principal@home_cell
         */
        *stp = krb5_cc_get_principal(ccache, &ps_creds->client);
        if (BAD_STATUS(stp)) {
            cleanup();
            return;
        }

    } else { 
	if (recurse) {

	    /*
	     * This is a request for a foreign cell PTGT.  Make a recursive
	     * call to get a ptgt in the home cell, if necessary.
	     */
	    
	    sec_priv_get_ptgt(login_context, identity->realm.name, authn_svc,
			      authz_svc, stp);
	    if (BAD_STATUS(stp)) {
		cleanup();
		return;
	    }
	}

        /*
         * if we get this far, then we've got enough state in the underlying
         * kerberos credential cache to acquire/refresh whatever remote TGTs
         * are required to acquire the ticket {PRIV(home), PRIV(foreign)}
         * that we need to construct an authenticator for the priv server
         * request message.
         */

        /* client = priv@home_cell */
        sec_krb_ptgs_parsed_name( (char *) identity->realm.name,
                                 &ps_creds->client, stp);
        if (BAD_STATUS(stp)) {
            cleanup();
            return;
        }
    }

    /*
     * retrieve/acquire a valid ticket to the priv server in the target
     * cell from the cache/kdc
     */
    ps_creds->times.endtime = 0;
    *stp = krb5_get_credentials_dce(0, ccache, ps_creds, NULL);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    /* cons up a krb5_kdc_req with an ap_req authentication header  */

    memset(&krb_req, 0, sizeof(krb_req));

    krb_req.msg_type = KRB5_TGS_REQ;

    /*
     * choose an encryption type for message exchange
     */
    priv_pvt_get_etype(ps_creds, &etype, stp);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    etypes[0] = etype;
    krb_req.etype = etypes;
    krb_req.netypes = 1;

    krb5_use_cstype(&skey_eblock, etype);
    *stp = krb5_process_key(&skey_eblock, &ps_creds->keyblock);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    (void) krb5_finish_key(&skey_eblock);

    /*
     * none of those fancy kdc options apply to priv tickets - so leave
     * leave the kdc_options field zeroed out
     */

    /* client and server fields not used - but the ASN.1 encoding of
     * a KRB5_KDC_REQ insists that the server field contain something
     * real, so we stuff the priv server principal there to make it happy
     */
    krb_req.server = ps_creds->server;

    /*
     * use times from the ticket to the priv server.
     */
    krb_req.from = ps_creds->times.starttime;
    krb_req.till = ps_creds->times.endtime;
    krb_req.rtime = ps_creds->times.renew_till;

    krb_req.nonce = (krb5_int32) time_now;

    /* use adresses from ticket to priv server */
    krb_req.addresses = ps_creds->addresses;

    /*
     * encrypt the auth data, if any, under the subkey generated above.  Currently
     * there is only authdata associated with a request for a home-cell
     * PTGT
     */
    if (local) {

        idl_pkl_t   *ppac;
        unsigned32  ppac_len;

        sec_id_pac_t_pickle(identity, NULL, (void *(*)(unsigned32)) malloc,
                            (unsigned32) 8, &ppac, &ppac_len, stp);

        if (STATUS_OK(stp)) {
            priv_krb_util_pac_to_enc_authdata(ppac, ppac_len,
                                                (krb5_authdata **) NULL,
                                                &skey_eblock,
                                                &krb_req.authorization_data,
                                                stp);
            free(ppac);
        }
	if (BAD_STATUS(stp)) {
	    cleanup();
	    return;
	}
    }

    /* done with the key, in any event */
    (void) krb5_finish_key(&skey_eblock);

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell(); \
    cleanup_authdata(krb_req.authorization_data);

    *stp = encode_krb5_kdc_req_body(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ps_cksum.contents = (krb5_octet *)
        malloc(krb5_cksumarray[sumtype]->checksum_length);
    if (ps_cksum.contents == NULL) {
        krb5_free_data(scratch);
        cleanup();
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }

    *stp = (*(krb5_cksumarray[sumtype]->
                     sum_func))(scratch->data,
                                scratch->length,
                                (krb5_pointer) ps_creds->keyblock.contents,
                                ps_creds->keyblock.length,
                                &ps_cksum);

    /* done with scratch, regardless */
    krb5_free_data(scratch);

    if (BAD_STATUS(stp)) {
        xfree(ps_cksum.contents);
        cleanup();
        return;
    }


#undef cleanup
#define cleanup() \
    cleanup_authdata(krb_req.authorization_data); \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    cleanup_tcell();

    *stp = krb5_mk_req_extended (0L /* no ap options */,
                                &ps_cksum,
                                0L, /* don't need kdc_options for this */
                                0, /* no initial sequence */
                                0, /* no subkey--we're using the session key */
                                0, /* no ccache--already have creds */
                                ps_creds,
                                0, /* don't need authenticator */
                                &scratch2);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    krb_req.padata = (krb5_pa_data **) calloc(2, sizeof(krb5_pa_data *));
    if (krb_req.padata == NULL) {
        cleanup();
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0] = (krb5_pa_data *) malloc(sizeof(krb5_pa_data));
    if (krb_req.padata[0] == NULL) {
        cleanup();
        free((char *)krb_req.padata);
        xfree(scratch2.data);
        return;
    }

    krb_req.padata[0]->pa_type = KRB5_PADATA_AP_REQ;
    krb_req.padata[0]->length = scratch2.length;
    krb_req.padata[0]->contents = (krb5_octet *) scratch2.data;

     /*
      * scratch2 is now dedicated to holding data for element2 of our
      * pre-authentication data array.  By freeing the padata, we also
      * free scratch2.data
      */
#undef cleanup
#define cleanup() \
    cleanup_authdata(krb_req.authorization_data); \
    krb5_free_creds(ps_creds); \
    xfree(ps_cksum.contents); \
    cleanup_tcell(); \
    krb5_free_pa_data(krb_req.padata);

    /*
     * Finally, encode the priv server TGS_REQ and stuff it into a pickle
     */
    *stp = encode_krb5_tgs_req(&krb_req, &scratch);
    if (BAD_STATUS(stp)) {
        cleanup();
        return;
    }

    ptgt_req = malloc (sizeof(rpriv_pickle_t) + scratch->length);
    if (ptgt_req == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        cleanup();
        krb5_free_data(scratch);
        return;
    }

    memcpy(ptgt_req->bytes, scratch->data, scratch->length);
    ptgt_req->num_bytes = scratch->length;
    /*
     * done with request data, except for subkey,
     * and session key (in ps_creds)
     */
    cleanup_authdata(krb_req.authorization_data);
    xfree(ps_cksum.contents);
    krb5_free_data(scratch);
    krb5_free_pa_data(krb_req.padata);

    /* bind to a priv server in the appropriate cell - need to use an
     * appropriate internal binding function and bind by priv server
     * interface uuid.  
     */
    bind_auth_info.info_type = sec_rgy_bind_auth_none;
    rca_site_bind(tcell, 
                    &bind_auth_info,
                    rca_NSI_cell,
                    rpriv_v1_0_c_ifspec,
                    SEC_RGY_SERVER_NAME,
                    (char *)tcell,
                    &context,
                    stp);

    if (BAD_STATUS(stp)) {
        krb5_free_creds(ps_creds);
        return;
    }

#undef cleanup
#define cleanup() \
    krb5_free_creds(ps_creds); \
    cleanup_tcell();

    SETUP_RETRY(rca_op_read, stp) {

        Crpriv_get_ptgt(context, authn_svc, authz_svc, ptgt_req, &ptgt_rep, stp);

        NORMAL_CASE;
        RETRY_CASE;
    }
    RETRY_END;

    { error_status_t lst; sec_rgy_site_close(context, &lst); }
    free(ptgt_req);

    if (STATUS_OK(stp)) {
        /* decrypt the encoded krb_rep containing the privilege ticket */
        krb_rep.length = ptgt_rep->num_bytes;
        krb_rep.data = (char *) ptgt_rep->bytes;

	sec_priv_store_ptgt(login_context, target_cell, &krb_rep, 
		&ps_creds->keyblock, NULL, ccache, false, NULL, stp);
        /* done with ndr encoded response */
        rpc_ss_client_free((idl_void_p_t) ptgt_rep);
    }

    krb5_free_creds(ps_creds);
    cleanup_tcell();

#undef cleanup

}

/*
 * s e c _ p r i v _ s t o r e _  p t g t
 *
 * Stores PTGT and associated information (epac chain, credentials)
 * If epac_chain is NULL, it assumes this is a ptgt.
 *
 * INPUT:
 *	enc_ptgt_rep: KDC_TGS_REP encoded message containing eptgt/ptgt
 *      ps_key     : priv server session key
 *      epac chain : epac chain. If NULL, the caller is probably
 *	             connected to a 1.0.x server which only returns PACs.
 *      ccache     : the cache file where to store the eptgt/ptgt
 *      gen_v1_pac : a flag indicating whether a V1 PAC should be
 *                   generated and stored in the authdata
 * OUTPUT:
 *	privs: the epac chain to be returned to the caller. This
 *             could either be the epac chain returned by the server
 *             or an epac chain which is stored in the .db file if
 *             another thread or process was simultaneously refreshing
 *             its PTGT and stored it before us. If privs is NULL,
 *	       nothing is returned.
 *	stp : error status
 */
static void sec_priv_store_ptgt(
    sec_login_handle_t		login_context,		/* [in]  */
    unsigned_char_p_t		target_cell,		/* [in]  */
    krb5_data			*enc_ptgt_rep,		/* [in]  */
    krb5_keyblock		*ps_key,		/* [in]  */
    sec_bytes_t			*epac_chain,		/* [in]  */
    krb5_ccache			ccache,			/* [in]  */
    boolean32			gen_v1_pac,		/* [in]  */
    sec_bytes_t			*privs,			/* [out] */
    error_status_t		*stp			/* [out] */
)
{
    krb5_kdc_rep		*dec_ptgt_rep = NULL;
    krb5_principal		ptgt_kdc;
    krb5_creds			*creds = NULL;
    sec_bytes_t			authbytes;
    sec_db_authdata_contents_t	v1_authdata;
    char			*dfn = NULL;
    sec_bytes_t			stored_epac;

    /* check for NULLS! */
    dce_assert(sec_svc_handle, enc_ptgt_rep != NULL);
    dce_assert(sec_svc_handle, ps_key != NULL);

#define clean_kdc_rep() \
    if (dec_ptgt_rep != NULL) { \
        krb5_free_kdc_rep(dec_ptgt_rep); \
    }

    memset(&authbytes, 0, sizeof(authbytes));
    memset(&v1_authdata, 0, sizeof(v1_authdata));
    memset(&stored_epac, 0, sizeof(stored_epac));

    /* decrypt the encoded kdc_tgs_rep containing the privilege ticket */
    *stp = krb5_decode_kdc_rep(enc_ptgt_rep, ps_key,
                               ps_key->keytype,
                               &dec_ptgt_rep);
    if (BAD_STATUS(stp)) {
	return;
    }

    /*XXX need more SANITY CHECKS here !!!! */
    /* See the big block of gunk in get_in_tkt.c for what we should be checking here */
    sec_krb_tgs_parsed_name( (char *)target_cell, &ptgt_kdc, stp);
    if (BAD_STATUS(stp)) {
	clean_kdc_rep();
	return;
    }

    if (krb5_principal_compare(dec_ptgt_rep->enc_part2->server,
				ptgt_kdc) == 0)
    {
	krb5_free_principal(ptgt_kdc);
	SET_STATUS(stp, KRB5_KDCREP_MODIFIED);
	clean_kdc_rep();
	return;
    }
    krb5_free_principal(ptgt_kdc);


    /*
     * put the results in a kerberos v5 credential structure
     */
    creds = (krb5_creds *) malloc (sizeof(krb5_creds));
    if (creds == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        clean_kdc_rep();
        return;
    } else {
        memset(creds, 0, sizeof(krb5_creds));
    }

    /*
     * copy data from ptgt_rep into local credential block
     */
    *stp = krb5_copy_principal(dec_ptgt_rep->client, &creds->client);
    if (BAD_STATUS(stp)) {
        krb5_free_creds(creds);
        clean_kdc_rep();
        return;
    }

    *stp = krb5_copy_principal(dec_ptgt_rep->enc_part2->server,
                               &creds->server);
    if (BAD_STATUS(stp)) {
        krb5_free_creds(creds);
        clean_kdc_rep();
        return;
    }

    *stp = krb5_copy_keyblock_contents(dec_ptgt_rep->enc_part2->session,
                                       &creds->keyblock);
    if (BAD_STATUS(stp)) {
        krb5_free_creds(creds);
        clean_kdc_rep();
        return;
    }

    if (dec_ptgt_rep->enc_part2->caddrs) {
        *stp = krb5_copy_addresses(dec_ptgt_rep->enc_part2->caddrs,
                                   &creds->addresses);
        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            clean_kdc_rep();
            return;
        }
    }

    creds->ticket_flags = dec_ptgt_rep->enc_part2->flags;
    creds->times = dec_ptgt_rep->enc_part2->times;
    creds->is_skey = 0;

    {
        krb5_data *ticket_data;

        *stp = encode_krb5_ticket(dec_ptgt_rep->ticket,
                                  &ticket_data);
        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            clean_kdc_rep();
            return;
        }

        creds->ticket = *ticket_data;
        free( (char *) ticket_data);
    }

    creds->second_ticket.length = 0;
    creds->second_ticket.data = NULL;

    if (epac_chain) {
        /* Generate credential cache authdata from the epac chain */
        priv_krb_util_gen_v1_1_ccache_authdata(epac_chain,
                                               gen_v1_pac,
                                               &authbytes,
                                               &creds->authdata,
                                               stp);
    } else {
        krb5_data	scratch, enc_buf;
        krb5_enc_data	*enc_authdata;
        krb5_encrypt_block skey_eblock;
        krb5_authdata	**clear, *dce_authdata = NULL;
        boolean32	more;
        krb5_enctype	etype;
        error_status_t	lst;

        scratch.length = dec_ptgt_rep->padata[0]->length;
        scratch.data = (char *) dec_ptgt_rep->padata[0]->contents;
        *stp = decode_krb5_enc_data(&scratch, &enc_authdata);
        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            clean_kdc_rep();
            return;
        }

        /* decrypt the authdata in the request */
        if (valid_etype(enc_authdata->etype)) {
            CLEAR_STATUS(stp);
        } else {
            SET_STATUS(stp, KDC_ERR_ETYPE_NOSUPP);
            krb5_free_creds(creds);
            clean_kdc_rep();
            cleanup_authdata(*enc_authdata);
            xfree(enc_authdata);
            return;
        }

        enc_buf.length = enc_authdata->ciphertext.length;
        enc_buf.data = malloc(enc_authdata->ciphertext.length);
        if (enc_buf.data == NULL) {
            SET_STATUS(stp, sec_priv_s_no_mem);
            krb5_free_creds(creds);
            clean_kdc_rep();
            cleanup_authdata(*enc_authdata);
            xfree(enc_authdata);
            return;
        }

        /*
         * choose an encryption type for message exchange
         */
        priv_pvt_get_etype_from_key(ps_key, &etype, stp);
        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            clean_kdc_rep();
            cleanup_authdata(*enc_authdata);
            xfree(enc_authdata);
            free(enc_buf.data);
            return;
        }

        krb5_use_cstype(&skey_eblock, etype);
        *stp = krb5_process_key(&skey_eblock, ps_key);
        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            clean_kdc_rep();
            cleanup_authdata(*enc_authdata);
            xfree(enc_authdata);
            free(enc_buf.data);
            return;
        }

        *stp = krb5_decrypt((krb5_pointer) enc_authdata->ciphertext.data,
                           (krb5_pointer) enc_buf.data,
                           enc_buf.length, &skey_eblock, 0);

        /* done with encyrypted authdata, regardless */
        cleanup_authdata(*enc_authdata);
        xfree(enc_authdata);

        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            free(enc_buf.data);
            clean_kdc_rep();
            return;
        }

        /* enc_buf now has the authorization data, so we decode it */
        *stp = decode_krb5_authdata(&enc_buf, &clear);

        /* done with scratch.data, regardless */
        free(enc_buf.data);
        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            clean_kdc_rep();
            return;
        }

        /*
         * We need to place the DCE ptgt version number in the authdata
         * returned from the 1.0 priv server so future lookups will find
         * the correct PTGT. This requires fetching the DCE authdata,
         * tweaking it a bit, and stuffing it back in. 
         */
        dce_authdata = priv_krb_util_get_dce_authdata(clear, &more);
        if (dce_authdata != NULL
            && dce_authdata->length != 0
            && dce_authdata->contents != NULL
            && !more) {

            krb5_authdata **tmp_authdata;

	    priv_krb_util_construct_v1_ptgt_authdata((idl_pkl_t *)dce_authdata->contents,
		dce_authdata->length, NULL, clear, &tmp_authdata, NULL, stp);
            
            if (STATUS_OK(stp)) {
		krb5_free_authdata(clear);
		clear = tmp_authdata;
	    }
        } else {
	    SET_STATUS(stp, sec_s_no_pac);
        }

        if (BAD_STATUS(stp)) {
            krb5_free_creds(creds);
            clean_kdc_rep();
            krb5_free_authdata(clear);
            return;
	}

        creds->authdata = clear;
    }


    /*
     * store the authdata (the v1 pac, epac chain)
     * in the ".data.db" file. 
     * XXX Optimization : do not store authdata if entry is
     *                    unexpired.
     */
    if (STATUS_OK(stp)) {
        if (epac_chain) {
            v1_authdata.epac_chain.num_bytes = epac_chain->num_bytes;
            v1_authdata.epac_chain.bytes = epac_chain->bytes;
        } else {
            v1_authdata.epac_chain.num_bytes = 0;
            v1_authdata.epac_chain.bytes = NULL;
        }
        v1_authdata.authbytes.num_bytes = authbytes.num_bytes;
        v1_authdata.authbytes.bytes = authbytes.bytes;
        v1_authdata.endtime = creds->times.endtime;
        sec_dce_pair_from_krb_princ(creds->server,
                                    (char **) &v1_authdata.cell,
                                    NULL,
                                    stp);
    }

    if (STATUS_OK(stp)) {
	sec_login_util_store_ptgtcreds_and_epac(login_context,
		DB_DATAFILE_NAME(dfn, ccache), v1_authdata,
		ccache, creds, 
		(privs != NULL) ? &stored_epac : NULL, stp);
        if (dfn)
	    free(dfn);
    }

    if (STATUS_OK(stp) && privs != NULL) {
        /*
         * Check to see if another thread/process stored an
         * epac before us. If so, use it instead. !!! It's
         * extremely important to use the stored epac instead
         * of the epac returned by the server to prevent 
         * "mismatched seal" errors. (An important fact: 2
         * security replicas of the same architecture can each NDR encode
         * an epac consisting of identical information and these 2
         * epacs may not match because of different byte alignment)
         */
        if (stored_epac.bytes && stored_epac.num_bytes) {
	    privs->bytes = stored_epac.bytes;
	    privs->num_bytes = stored_epac.num_bytes;
        } else {
            /*
             * Otherwise, make a copy of the granted_privs using the system
             * memory allocator to insure a consistent memory management
             * scheme.
             */
            privs->bytes = malloc(epac_chain->num_bytes);
            if (privs != NULL) {
                memcpy(privs->bytes, epac_chain->bytes, epac_chain->num_bytes);
                privs->num_bytes = epac_chain->num_bytes;
            } else {
                SET_STATUS(stp, sec_priv_s_no_mem);
	    }
        }
    }

sec_priv_store_ptgt_cleanup:
    if (v1_authdata.cell)
	free(v1_authdata.cell);
    if (authbytes.num_bytes && authbytes.bytes)
        free(authbytes.bytes);
    krb5_free_creds(creds);
    clean_kdc_rep();
}
