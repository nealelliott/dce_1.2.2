# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# @HP_COPYRIGHT@
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.27.5  1996/10/15  20:57:08  arvind
# 	Change for international build
# 	[1996/10/07  19:26 UTC  arvind  /main/DCE_1.2.2/arvind_work/4]
#
# 	CHFts19678: application coredumps when calling rsec_pk_cache_kdc_key
# 	[1996/07/24  18:44 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
#
# 	Add ACF file for rsec_pk_cache interface
# 	[1996/07/18  17:34 UTC  aha  /main/DCE_1.2.2/4]
#
# 	Merge Registry support for KDC private key storage
# 	[1996/07/16  14:11 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/8]
#
# 	Changes for Public Key Login; ensure sec_pk_base.idl is built before
# 	sec_encode.c
# 	[1996/07/15  19:43 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/7]
#
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/10  14:58 UTC  aha  /main/DCE_1.2.2/DCE122_PK/2]
#
# 	Merge to DCE122_PK
# 	[1996/06/07  13:07 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/4]
#
# 	Re-include dependencis of dacl_ifr.o and daclbind.o on
# 	rdaclif_cstub.c and rdaclifv0_cstub.c.
# 	[1996/06/06  17:43 UTC  arvind  /main/DCE_1.2.2/DCE122_PK/aha_pk6/3]
#
# 	Try to fix AIX build.
# 	[1996/06/06  16:37 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/2]
#
# 	Try to fix RIOS build
# 	[1996/06/06  16:13 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/1]
#
# 	Try to fix RIOS build
# 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
#
# 	Add build instructions for rsec_pk_cache_cstub
# 	[1996/04/23  19:18 UTC  aha  /main/DCE_1.2/aha_pk2/1]
#
# 	Additions for public key login
# 	[1996/01/03  19:20 UTC  psn  /main/DCE_1.2/1]
#
# Revision 1.2.27.4  1996/10/03  15:13:47  arvind
# 	CHFts19678: application coredumps when calling rsec_pk_cache_kdc_key
# 	[1996/07/26  20:46 UTC  aha  /main/DCE_1.2.2/5]
# 
# 	don't build sec_pubkey.idl here; it's not needed.
# 	[1996/08/09  17:16 UTC  sommerfeld  /main/DCE_1.2.2/DCE122_PK/sommerfeld_pk_kdc/1]
# 
# 	CHFts19678: application coredumps when calling rsec_pk_cache_kdc_key
# 	[1996/07/24  18:44 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
# 
# 	Add ACF file for rsec_pk_cache interface
# 	[1996/07/18  17:34 UTC  aha  /main/DCE_1.2.2/4]
# 
# 	Merge Registry support for KDC private key storage
# 	[1996/07/16  14:11 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/8]
# 
# 	Changes for Public Key Login; ensure sec_pk_base.idl is built before
# 	sec_encode.c
# 	[1996/07/15  19:43 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/7]
# 
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/10  14:58 UTC  aha  /main/DCE_1.2.2/DCE122_PK/2]
# 
# 	Merge to DCE122_PK
# 	[1996/06/07  13:07 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/4]
# 
# 	Re-include dependencis of dacl_ifr.o and daclbind.o on
# 	rdaclif_cstub.c and rdaclifv0_cstub.c.
# 	[1996/06/06  17:43 UTC  arvind  /main/DCE_1.2.2/DCE122_PK/aha_pk6/3]
# 
# 	Try to fix AIX build.
# 	[1996/06/06  16:37 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/2]
# 
# 	Try to fix RIOS build
# 	[1996/06/06  16:13 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/1]
# 
# 	Try to fix RIOS build
# 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
# 
# 	Add build instructions for rsec_pk_cache_cstub
# 	[1996/04/23  19:18 UTC  aha  /main/DCE_1.2/aha_pk2/1]
# 
# 	Additions for public key login
# 	[1996/01/03  19:20 UTC  psn  /main/DCE_1.2/1]
# 
# Revision 1.2.27.3  1996/08/09  12:12:10  arvind
# 	CHFts19678: application coredumps when calling rsec_pk_cache_kdc_key
# 	[1996/07/24  18:44 UTC  aha  /main/DCE_1.2.2/aha_dbpk1/1]
# 
# 	Add ACF file for rsec_pk_cache interface
# 	[1996/07/18  17:34 UTC  aha  /main/DCE_1.2.2/4]
# 
# 	Merge Registry support for KDC private key storage
# 	[1996/07/16  14:11 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/8]
# 
# 	Changes for Public Key Login; ensure sec_pk_base.idl is built before
# 	sec_encode.c
# 	[1996/07/15  19:43 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/7]
# 
# 	Merge out from DCE122_PK
# 	[1996/06/18  20:56 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/5]
# 
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/11  17:57 UTC  aha  /main/DCE_1.2.2/2]
# 
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/10  14:58 UTC  aha  /main/DCE_1.2.2/DCE122_PK/2]
# 
# 	Merge to DCE122_PK
# 	[1996/06/07  13:07 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/4]
# 
# 	Re-include dependencis of dacl_ifr.o and daclbind.o on
# 	rdaclif_cstub.c and rdaclifv0_cstub.c.
# 	[1996/06/06  17:43 UTC  arvind  /main/DCE_1.2.2/DCE122_PK/aha_pk6/3]
# 
# 	Try to fix AIX build.
# 	[1996/06/06  16:37 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/2]
# 
# 	Try to fix RIOS build
# 	[1996/06/06  16:13 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/1]
# 
# 	Try to fix RIOS build
# 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
# 
# 	Add build instructions for rsec_pk_cache_cstub
# 	[1996/04/23  19:18 UTC  aha  /main/DCE_1.2/aha_pk2/1]
# 
# 	Additions for public key login
# 	[1996/01/03  19:20 UTC  psn  /main/DCE_1.2/1]
# 
# Revision 1.2.27.2  1996/07/08  18:39:16  arvind
# 	no acf file for sec_pk_base.idl...
# 	[1996/06/11  18:58 UTC  sommerfeld  /main/DCE_1.2.2/DCE122_PK/sommerfeld_pk_indir/1]
# 
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/10  14:58 UTC  aha  /main/DCE_1.2.2/DCE122_PK/2]
# 
# 	Merge to DCE122_PK
# 	[1996/06/07  13:07 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/4]
# 
# 	Re-include dependencis of dacl_ifr.o and daclbind.o on
# 	rdaclif_cstub.c and rdaclifv0_cstub.c.
# 	[1996/06/06  17:43 UTC  arvind  /main/DCE_1.2.2/DCE122_PK/aha_pk6/3]
# 
# 	Try to fix AIX build.
# 	[1996/06/06  16:37 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/2]
# 
# 	Try to fix RIOS build
# 	[1996/06/06  16:13 UTC  aha  /main/DCE_1.2.2/DCE122_PK/aha_pk6/1]
# 
# 	Try to fix RIOS build
# 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
# 
# 	Additions for Public Key Login
# 
# 	Add sec_pubkey
# 	[1996/04/26  20:45 UTC  aha  /main/DCE_1.2/DCE12_PK/2]
# 
# 	Add sec_pk files
# 	[1996/04/23  20:02 UTC  aha  /main/DCE_1.2/DCE12_PK/1]
# 
# 	Add build instructions for rsec_pk_cache_cstub
# 	[1996/04/23  19:18 UTC  aha  /main/DCE_1.2/aha_pk2/1]
# 
# 	Additions for public key login
# 	[1996/01/03  19:20 UTC  psn  /main/DCE_1.2/1]
# 
# Revision 1.2.27.1  1996/06/04  22:04:27  arvind
# 	Add sec_krb_u2u.c
# 	[1996/05/06  20:56 UTC  burati  /main/DCE_1.2/3]
# 
# 	merge u2u changes (add sec_krb_u2u.c)
# 	[1996/05/02  15:53 UTC  burati  /main/DCE_1.2/mb_u2u/1]
# 
# 	Remove DES code from gssapi build.
# 	[1995/12/19  16:29 UTC  psn  /main/HPDCE02/jrr_1.2_mothra/1]
# 
# 	Binary Compatibility Merge
# 	[1995/04/03  22:14 UTC  mullan_s  /main/HPDCE02/1]
# 
# 	Extend INCFLAGS for priv_krb_util.o to pick up sec_login_db.h
# 	[1995/03/20  14:54 UTC  mullan_s  /main/mullan_mothra_bin_compat/mullan_mothra_bin_compat2/3]
# 
# 	For priv.c, extend include path to pick up sec_login_db.h
# 	[1995/01/26  20:59 UTC  mullan_s  /main/mullan_mothra_bin_compat/mullan_mothra_bin_compat2/2]
# 
# 	Add rules for sec_login_db.c
# 	[1995/01/11  18:17 UTC  mullan_s  /main/mullan_mothra_bin_compat/1]
# 
# Revision 1.2.24.4  1996/03/09  23:30:23  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:41  marty]
# 
# Revision 1.2.24.3  1995/12/18  17:22:46  psn
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/12/08  16:49 UTC  rps
# 	international build omission
# 	[1995/12/05  01:39 UTC  rps  /main/HPDCE02/rps_moth5/1]
# 
# 	HP revision /main/HPDCE02/2  1995/05/15  21:12 UTC  greg
# 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
# 	[1995/04/22  13:37 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
# 
# 	HP revision /main/HPDCE02/1  1995/04/03  22:14 UTC  mullan_s
# 	Binary Compatibility Merge
# 
# 	HP revision /main/mullan_mothra_bin_compat/mullan_mothra_bin_compat2/3  1995/03/20  14:54 UTC  mullan_s
# 	Extend INCFLAGS for priv_krb_util.o to pick up sec_login_db.h
# 
# 	HP revision /main/mullan_mothra_bin_compat/mullan_mothra_bin_compat2/2  1995/01/26  20:59 UTC  mullan_s
# 	For priv.c, extend include path to pick up sec_login_db.h
# 
# 	HP revision /main/mullan_mothra_bin_compat/1  1995/01/11  18:17 UTC  mullan_s
# 	Add rules for sec_login_db.c
# 	[1995/12/18  17:21:53  psn]
# 
# Revision 1.2.19.15  1994/09/07  15:48:39  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:35  marty]
# 
# Revision 1.2.19.14  1994/08/08  14:23:50  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/08/04  19:24:36  mdf]
# 
# Revision 1.2.19.12  1994/07/17  20:22:31  baum
# 	Added dependency sec_pwd_mgmt.o:rs_pwd_mgmt.h
# 	[1994/07/17  20:12:01  baum]
# 
# Revision 1.2.19.11  1994/07/15  15:03:07  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:24:07  mdf]
# 
# Revision 1.2.19.10  1994/06/28  20:01:00  wray
# 	GSSAPI BL3 (OT 10824)
# 	[1994/06/28  19:25:20  wray]
# 
# Revision 1.2.19.9  1994/06/02  21:17:51  mdf
# 	hp_sec_to_osf_3 drop, merge up with latest.
# 	[1994/05/24  15:20:45  mdf]
# 
# 	hp_sec_to_osf_3 drop
# 
# Revision 1.2.19.8  1994/05/12  18:07:27  ahop
# 	state explicit dependencies for priv.o and sec_login_pvt.o
# 	[1994/05/12  18:04:56  ahop]
# 
# Revision 1.2.19.7  1994/05/11  19:33:46  ahop
# 	hp_sec_to_osf_2 drop
# 	fix: PTGT refresh now takes delegation into account
# 	build sec_login_encode interface
# 	Add rpriv_1_0.idl
# 	build authz_encode_cstub.o
# 	rm sec_attr_trig.idl, sec_attr_base.idl, add IDLINCFLAGS for attr stuff
# 	[1994/04/29  21:49:33  ahop]
# 
# Revision 1.2.19.6  1994/03/25  20:30:06  sekhar
# 	Fixed errors due to merge.
# 	[1994/03/25  20:21:54  sekhar]
# 
# Revision 1.2.19.5  1994/03/25  18:38:17  sekhar
# 	Merged with changes from 1.2.19.4
# 	[1994/03/25  18:38:01  sekhar]
# 
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  18:31:03  sekhar]
# 
# Revision 1.2.19.4  1994/03/23  16:47:32  wray
# 	GSSAPI BL2 (OT#10179)
# 	[1994/03/23  16:44:05  wray]
# 
# 	GSSAPI BL2 (OT#10179)
# 
# Revision 1.2.19.3  1994/03/09  15:50:59  wray
# 	Add GSSAPI (OT#10019)
# 	[1994/03/09  15:39:16  wray]
# 
# Revision 1.2.19.2  1994/03/02  17:56:14  hanfei
# 	era initial drop.
# 	[1994/03/01  15:58:20  hanfei]
# 
# Revision 1.2.19.1  1994/01/28  23:11:27  burati
# 	Add dependency on rdacl stubs to dacl_ifr.o and daclbind.o
# 	so the ACF'd hdr file gets built before it needs to be included
# 	[1994/01/21  21:18:56  burati]
# 
# 	Dlg/EPAC changes - Add rdaclifv0.*, id_encode.*/sec_encode.* (dlg_bl1)
# 	[1994/01/20  02:42:53  burati]
# 
# Revision 1.2.17.1  1993/09/13  15:52:59  delgado
# 	More DFS 486 changes - change TARGET_MACHINE == PMAX to
# 	TARGET_OS == OSF1
# 	[1993/09/13  15:48:14  delgado]
# 
# Revision 1.2.15.2  1993/07/19  19:38:32  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:29:48  zeliff]
# 
# Revision 1.2.13.3  1993/07/16  21:16:00  kissel
# 	Change sense of NO_LATENT_DFS_SUPPORT to LATENT_DFS_SUPPORT
# 	[1993/06/21  14:47:19  kissel]
# 
# Revision 1.2.11.2  1993/06/04  20:20:51  robinson
# 	Update with HP product sources.
# 	[1993/06/04  20:01:10  robinson]
# 
# Revision 1.2.9.8  1993/01/29  18:51:31  burati
# 	Changes for replication client agent
# 	[1993/01/14  23:21:57  burati]
# 
# Revision 1.2.9.7  1992/12/29  16:40:09  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  21:31:17  zeliff]
# 
# Revision 1.2.9.6  1992/12/08  19:02:22  rps
# 	Add rs_bind.
# 	[1992/12/03  01:26:58  rps]
# 
# Revision 1.2.9.5  1992/12/04  21:31:25  burati
# 	CR5978 Move OS dependent sec_login code to machine specific dirs
# 	[1992/12/04  16:46:11  burati]
# 
# Revision 1.2.9.4  1992/11/12  18:08:49  rsarbo
# 	Include i/f's in libdce for Episode ACL
# 	support
# 	[1992/11/12  18:05:13  rsarbo]
# 
# 	turn on DFS ACL's on PMAX
# 	[1992/11/09  14:59:38  rsarbo]
# 
# Revision 1.2.9.3  1992/10/20  19:40:29  rsarbo
# 	update file specific CFLAGS for Episode ACL's.
# 	[1992/10/20  19:40:00  rsarbo]
# 
# Revision 1.2.9.2  1992/10/16  17:47:53  rsarbo
# 	added file-specific CFLAGS dependent on NO_DFS (moved from security.mk)
# 	[1992/10/09  18:19:38  rsarbo]
# 
# Revision 1.2.4.7  1992/06/30  21:54:34  burati
# 	CR4516 HP/UX porting
# 	[1992/06/30  04:43:19  burati]
# 
# Revision 1.2.4.6  1992/06/02  21:18:40  sekhar
# 	Merged Unix interface changes made by frisco
# 	to the mainline.
# 
# 	    ========== Start of Merged changes ===============
# 	    Revision 1.2.7.2  1992/05/22  19:53:22  frisco
# 	    Add Unix interface support
# 	    ========== End of Merged changes ================
# 	[1992/06/02  20:30:18  sekhar]
# 
# Revision 1.2.4.5  1992/05/11  17:51:31  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 	[1992/04/22  15:23:26  mhickey]
# 
# Revision 1.2.5.2  1992/04/15  21:45:56  sommerfeld
# 	Add sec_login_cache.o
# 
# Revision 1.2.4.4  1992/05/08  20:56:22  sommerfeld
# 	Add sec_login_cache.o_INCFLAGS
# 	[1992/04/30  16:51:07  sommerfeld]
# 
# 	Merge in 1.0.1 changes after b9.
# 	[1992/04/17  19:49:12  sommerfeld]
# 
# Revision 1.2.4.3  1992/04/07  19:19:26  mckeen
# 	Added rpladmn.o dependency on rs_rpladmn.h
# 	[1992/03/13  15:47:13  mckeen]
# 
# Revision 1.2.4.2  1992/03/31  22:45:12  burati
# 	CR2419 Add sec_lrgy.o for local registry support
# 	[1992/03/31  01:34:39  burati]
# 
# Revision 1.2  1992/01/19  22:13:53  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#  OSF DCE Version 1.0.2
#
#   Makefile
#
# Copyright (c) Hewlett-Packard Company 1991,1992,1993
# Unpublished work. All Rights Reserved.
#
# DCE Security Services 
# /utils Makefile
#

###########################################################
# define the following "globally" for the libdce.a build.
###########################################################
###  DO NOT PUT ANYTHING IN THIS SECTION EXCEPT FOR OBJECT
###  FILE LISTS!!!!!
###########################################################
#
SEC_OBJS = acct.o \
	   acl_encode_cstub.o \
	   authz_encode_cstub.o \
	   authz_util.o \
	   binding.o \
	   crypt.o \
	   dacl_ifr.o \
	   daclbind.o \
	   dcegssmsg.o \
	   dcegsssvc.o \
	   dcesecmsg.o \
	   dcesecsvc.o \
	   dfs_dce_acl.o \
	   gssapi-supp.o \
	   gssapi.o \
	   id_encode_cstub.o \
	   initstring.o \
	   internal_binding.o \
	   keymgmt.o \
	   krb5rpc.o \
	   krb5rpc_cstub.o \
	   misc.o \
	   override.o \
	   pgo.o \
	   pkl.o \
	   policy.o \
	   priv.o \
	   priv_krb_util.o \
	   rca_pwd.o \
	   rdaclif_cstub.o \
	   rdaclifv0_cstub.o \
	   repadm.o \
	   replist.o \
	   restrictions.o     \
	   roverride_cstub.o \
	   rpriv_1_0_cstub.o \
	   rpriv_cstub.o \
	   rs_acct_cstub.o \
	   rs_attr_cstub.o \
	   rs_attr_schema_cstub.o \
	   rs_attr_srch_cstub.o \
	   rs_bind_cstub.o \
	   rs_login_activity_cstub.o \
	   rs_misc_cstub.o \
	   rs_pgo_cstub.o \
	   rs_plcy_cstub.o \
	   rs_pwd_mgmt_cstub.o \
	   rs_repadm_1_1_cstub.o \
	   rs_repadm_cstub.o \
	   rs_replist_1_0_cstub.o \
	   rs_replist_cstub.o \
	   rs_unix_cstub.o \
	   rsec_cert_cstub.o \
	   rsec_login_cstub.o \
	   rsec_pwd_mgmt_cstub.o \
	   rsecidmap_cstub.o \
	   sec_acl_encode.o \
	   sec_acl_util.o \
	   sec_attr_util.o \
	   sec_attr_trig_cstub.o \
	   sec_authn.o \
	   sec_bind_util.o \
	   sec_certify.o \
	   sec_cred.o \
	   sec_cred_internal.o \
           passwd_encode_cstub.o \
	   sec_encode.o \
	   sec_globals.o \
	   sec_id_pac_util.o \
	   sec_krb_util.o \
	   sec_krb_u2u.o \
	   sec_login.o \
	   sec_login_cache.o \
           sec_login_db.o \
	   sec_login_encode_cstub.o \
	   sec_login_np.o \
	   sec_login_pag.o \
	   sec_login_pvt.o \
	   sec_login_util.o \
	   sec_lrgy.o \
	   sec_pkl.o \
	   sec_pwd_mgmt.o \
	   sec_rgy_attr.o \
	   sec_rgy_attr_sch.o \
	   sec_rgy_attr_srch.o \
	   sec_rgy_login_activity.o \
	   sec_svchandle.o \
	   seca_cn.o \
	   seca_dg.o \
	   seca_gss.o \
	   secidmap.o \
	   seckey_del.o \
	   seckey_get.o \
	   seckey_list.o \
	   seckey_set.o \
	   seckey_util.o \
	   u_str.o \
	   unix.o \
           sec_pk.o \
           rsec_pk_cache_cstub.o \
           sec_pubkey.o


UTIL_OBJS = args.o commands.o auth_test_util.o

libsecutil.a_OFILES = ${UTIL_OBJS}
libsec_OFILES = ${SEC_OBJS} ${${TARGET_MACHINE}_libsec_OFILES}
libsec.a_OFILES = ${libsec_OFILES}

#
# The rest of the Makefile is "private" for building in the directory
#
.if !defined(LIBDCE_LIBRARY_MAKEFILE)
#
# The following definitions are for shared libraries:
#   OBJECTS must be defined to pull in the common rules for compilation
#   LIBS    must be defined to prevent -ldce from appearing on compile lines
#
OBJECTS =
LIBS    =

# Additional places to look for files
VPATH = ../client/login/${TARGET_MACHINE}:../client/login:../client/acl:../client/rca:../client/key_mgmt:../client/pwd_mgmt:../idl:${TARGET_MACHINE}:../gssapi

# Flags for the IDL compiler
IDLINCFLAGS = -I. -I../idl 
LOGIN_IDLINCFLAGS = -I. -I../client/login -I../idl
ACL_IDLINCFLAGS = -I. -I../client/acl -I../idl
RCA_IDLINCFLAGS = -I. -I../client/rca -I../idl
PWD_IDLINCFLAGS = -I. -I../client/pwd_mgmt -I../idl

IDLFLAGS =  -keep c_source -cepv 
IDLFILES = acl_encode.idl \
	   authz_encode.idl \
	   id_encode.idl \
           passwd_encode.idl \
	   krb5rpc.idl \
	   rdaclif.idl \
	   rdaclifv0.idl \
	   roverride.idl \
	   rpriv.idl \
	   rpriv_1_0.idl \
	   rs_acct.idl \
	   rs_attr.idl \
	   rs_attr_schema.idl \
	   rs_attr_srch.idl \
	   rs_bind.idl \
	   rs_login_activity.idl \
	   rs_misc.idl \
	   rs_pgo.idl \
	   rs_plcy.idl \
	   rs_pwd_mgmt.idl \
	   rs_repadm_1_1.idl \
	   rs_repadm.idl \
	   rs_replist_1_0.idl \
	   rs_replist.idl \
	   rs_unix.idl \
	   rsec_cert.idl \
	   rsec_login.idl \
	   rsec_pwd_mgmt.idl \
	   rsecidmap.idl \
	   sec_attr_trig.idl \
	   sec_login_encode.idl \
           sec_pk_base.idl \
           rsec_pk_cache.idl

rpriv_ACF =
rpriv_1_0_ACF =
rsecidmap_ACF =
rs_acct_ACF =
rs_bind_ACF =
rs_misc_ACF =
rs_pgo_ACF =
rs_plcy_ACF =
rs_repadm_1_1_ACF =
rs_repadm_ACF =
rs_replist_1_0_ACF =
rs_replist_ACF =
rdaclif_ACF =
rdaclifv0_ACF =
rsec_cert_ACF =
roverride_ACF =
krb5rpc_ACF =
id_encode_ACF =
authz_encode_ACF =
sec_attr_trig_ACF =
sec_login_encode_ACF =
acl_encode_ACF =
rs_attr_ACF =
rs_attr_schema_ACF =
rs_attr_srch_ACF =
rs_login_activity_ACF =
rsec_login_ACF =
sec_pwd_mgmt_ACF =
rs_pwd_mgmt_ACF =
rsec_pwd_mgmt_ACF =
passwd_encode_ACF =
rsec_pk_cache_ACF =

IDLDATAFILES =	acl_encode.h \
		authz_encode.h \
		id_encode.h \
                passwd_encode.h \
		krb5rpc.h \
		rdaclif.h \
		rdaclifv0.h \
		roverride.h \
		rpriv.h \
		rpriv_1_0.h \
		rs_acct.h \
		rs_attr.h \
		rs_attr_schema.h \
		rs_attr_srch.h \
		rs_bind.h \
		rs_login_activity.h \
		rs_misc.h \
		rs_pgo.h \
		rs_plcy.h \
		rs_pwd_mgmt.h \
		rs_repadm_1_1.h \
		rs_repadm.h \
		rs_replist_1_0.h \
		rs_replist.h \
		rs_unix.h \
		rsec_cert.h \
		rsec_login.h \
		rsec_pwd_mgmt.h \
		rsecidmap.h \
		sec_attr_trig.h \
		sec_login_encode.h \
                sec_pk_base.h \
                rsec_pk_cache.h \
                sec_pubkey.h

id_encode.idl_IDLFLAGS = -keep c_source
authz_encode.idl_IDLFLAGS = -keep c_source
sec_attr_trig.idl_IDLFLAGS = -keep c_source
sec_login_encode.idl_IDLFLAGS = -keep c_source
rsec_pwd_mgmt.idl_IDLFLAGS = -keep c_source
acl_encode.idl_IDLFLAGS = -keep c_source
passwd_encode.idl_IDLFLAGS = -keep c_source

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS} ${NOENCRYPTION}

.ifdef DES_HIDDEN
gssapi.o_CFLAGS = ${CFLAGS} -DNOENCRYPTION
.endif

.if ${TARGET_OS} == "HPUX"

.if defined(NO_DFS) && !defined(LATENT_DFS_SUPPORT)
.else
dfs_dce_acl.o_CFLAGS = ${CFLAGS} -DDCE_DFS_PRESENT -Duse_syscall
sec_login_pag.o_CFLAGS = ${CFLAGS} -DDCE_DFS_PRESENT
.endif

.else

.ifndef NO_DFS
.if ${TARGET_OS} == "OSF1"
dfs_dce_acl.o_CFLAGS = ${CFLAGS} -DDCE_DFS_PRESENT -Duse_syscall -DAFS_OSF_ENV
.else
dfs_dce_acl.o_CFLAGS = ${CFLAGS} -DDCE_DFS_PRESENT -Duse_syscall
.endif
sec_login_pag.o_CFLAGS = ${CFLAGS} -DDCE_DFS_PRESENT
.endif

.endif

INCFLAGS = -I../h -I../idl
LOGIN_INCFLAGS =  -I../h -I../client/login -I../idl
ACL_INCFLAGS = -I../h -I../client/acl -I../idl
RCA_INCFLAGS = -I../h -I../client/rca -I../idl
GSS_INCFLAGS = -I../h -I../gssapi -I../idl
PWD_INCFLAGS = -I../h -I../client/rca -I../idl

acct.o_INCFLAGS = ${RCA_INCFLAGS}
acl_encode.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
acl_encode_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
acl_encode_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
authz_encode.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
authz_encode_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
authz_encode_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
binding.o_INCFLAGS = ${RCA_INCFLAGS}
dacl_ifr.o_INCFLAGS = ${ACL_INCFLAGS}
daclbind.o_INCFLAGS = ${ACL_INCFLAGS}
dfs_dce_acl.o_INCFLAGS = ${ACL_INCFLAGS}
gssapi-supp.o_INCFLAGS = ${GSS_INCFLAGS}
gssapi.o_INCFLAGS = ${GSS_INCFLAGS}
id_encode.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
id_encode_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
id_encode_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
internal_binding.o_INCFLAGS = ${RCA_INCFLAGS}
krb5rpc.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
krb5rpc.o_INCFLAGS = ${RCA_INCFLAGS}
krb5rpc_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
krb5rpc_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
misc.o_INCFLAGS = ${RCA_INCFLAGS}
override.o_INCFLAGS = ${RCA_INCFLAGS}
passwd_encode.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
passwd_encode_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
passwd_encode_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
pgo.o_INCFLAGS = ${RCA_INCFLAGS}
policy.o_INCFLAGS = ${RCA_INCFLAGS}
priv.o_INCFLAGS = ${RCA_INCFLAGS}
priv_krb_util.o_INCFLAGS = ${LOGIN_INCFLAGS}
rca_pwd.o_INCFLAGS = ${RCA_INCFLAGS}
rdaclif.h_IDLINCFLAGS = ${ACL_IDLINCFLAGS}
rdaclif_cstub.c_IDLINCFLAGS = ${ACL_IDLINCFLAGS}
rdaclif_cstub.o_INCFLAGS = ${ACL_INCFLAGS}
rdaclifv0.h_IDLINCFLAGS = ${ACL_IDLINCFLAGS}
rdaclifv0_cstub.c_IDLINCFLAGS = ${ACL_IDLINCFLAGS}
rdaclifv0_cstub.o_INCFLAGS = ${ACL_INCFLAGS}
repadm.o_INCFLAGS = ${RCA_INCFLAGS}
replist.o_INCFLAGS = ${RCA_INCFLAGS}
roverride.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
roverride_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
roverride_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rpriv.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rpriv_1_0.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rpriv_1_0_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rpriv_1_0_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rpriv_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rpriv_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_acct.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_acct_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_acct_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_attr.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_attr_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_attr_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_attr_schema.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_attr_schema_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_attr_schema_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_attr_srch.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_attr_srch_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_attr_srch_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_bind.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_bind_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_bind_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_login_activity.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_login_activity_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_login_activity_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_misc.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_misc_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_misc_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_pgo.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_pgo_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_pgo_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_plcy.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_plcy_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_plcy_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_pwd_mgmt.h_IDLINCFLAGS = ${PWD_INCFLAGS}
rs_pwd_mgmt_cstub.c_IDLINCFLAGS = ${PWD_IDLINCFLAGS}
rs_repadm_1_1.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_repadm_1_1_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_repadm_1_1_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_repadm.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_repadm_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_repadm_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_replist.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_replist_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_replist_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_replist_1_0.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_replist_1_0_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_replist_1_0_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rs_unix.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_unix_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rs_unix_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rsec_cert.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rsec_cert_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rsec_cert_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rsec_login.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rsec_login_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rsec_login_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rsec_pk_cache.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rsec_pk_cache_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rsec_pk_cache_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
rsec_pwd_mgmt.h_IDLINCFLAGS = ${PWD_INCFLAGS}
rsec_pwd_mgmt_cstub.c_IDLINCFLAGS = ${PWD_IDLINCFLAGS}
rsecidmap.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
rsecidmap_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
sec_attr_trig_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
sec_attr_trig_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
sec_certify.o_INCFLAGS = ${RCA_INCFLAGS}
sec_encode.o_INCFLAGS = ${INCFLAGS}
sec_login.o_INCFLAGS = ${LOGIN_INCFLAGS}
sec_login_cache.o_INCFLAGS = ${LOGIN_INCFLAGS}
sec_login_db.o_INCFLAGS = ${LOGIN_INCFLAGS}
sec_login_encode.h_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
sec_login_encode_cstub.c_IDLINCFLAGS = ${RCA_IDLINCFLAGS}
sec_login_encode_cstub.o_INCFLAGS = ${RCA_INCFLAGS}
sec_login_np.o_INCFLAGS = ${LOGIN_INCFLAGS}
sec_login_pag.o_INCFLAGS = ${LOGIN_INCFLAGS}
sec_login_pvt.o_INCFLAGS = ${LOGIN_INCFLAGS} -I../client/rca
sec_login_util.o_INCFLAGS = ${LOGIN_INCFLAGS}
sec_pk.o_INCFLAGS = ${RCA_INCFLAGS}
sec_pubkey.o_INCFLAGS = ${RCA_INCFLAGS}
sec_pwd_mgmt.o_INCFLAGS = ${PWD_INCFLAGS}
sec_rgy_attr.o_INCFLAGS = ${RCA_INCFLAGS}
sec_rgy_attr_sch.o_INCFLAGS = ${RCA_INCFLAGS}
sec_rgy_attr_srch.o_INCFLAGS = ${RCA_INCFLAGS}
sec_rgy_login_activity.o_INCFLAGS = ${RCA_INCFLAGS}
seca_gss.o_INCFLAGS = ${GSS_INCFLAGS}
secidmap.o_INCFLAGS = ${RCA_INCFLAGS}
unix.o_INCFLAGS = ${RCA_INCFLAGS}

#
# Don't want to build any '.a' here if we are using shared libraries
#
.if ${USE_SHARED_LIBRARIES} == 0
  # Libraries to build
  LIBRARIES = libsecutil.a libsec.a
  EXPLIB_TARGETS                = libsecutil.a export_libsec.a
.else
  LIBRARIES = libsecutil.a
  EXPLIB_TARGETS                = libsecutil.a
  #
  # when we come through this directory building libsecutil.a, we need to 
  # compile with -ldce, so override the nulling of LIBS above.
  #
  args.o_LIBS = -ldce
  commands.o_LIBS = -ldce
  auth_test_util.o_LIBS = -ldce
.endif

EXPSHLIB_TARGETS        = ${libsec_OFILES}

# Directory in which to place exported libraries
EXPDIR = /usr/lib/

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

dacl_ifr.o: rdaclif_cstub.c
dacl_ifr.o: rdaclifv0_cstub.c
daclbind.o: rdaclif_cstub.c
daclbind.o: rdaclifv0_cstub.c
sec_pwd_mgmt.o:	rs_pwd_mgmt.h
repadm.o: rs_repadm.h rs_repadm_1_1.h
replist.o: rs_replist.h  rs_replist_1_0.h
sec_encode.o: sec_pk_base.h
sec_rgy_attr.o:	rs_attr.h
sec_rgy_attr_sch.o:	rs_attr_schema.h
sec_rgy_attr_srch.o:	rs_attr_srch.h
sec_rgy_login_activity.o:       rs_login_activity.h
priv.o:	rpriv_1_0.h
sec_login_pvt.o:	rsec_login.h

# Rules and dependencies
#DUP_FILES= dup_sec_login_util.o dup_sec_login_pvt.o

#${DUP_FILES}: ${.TARGET:S/^dup_//}
#	cp ${.TARGET:S/^dup_//} ${.TARGET}

.endif
