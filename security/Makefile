# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.18.5  1996/10/15  20:47:33  arvind
# 	OSF DCE 1.2.2 Drop 4.5
# 	[1996/10/15  20:11:36  arvind]
#
# Revision 1.1.18.4  1996/10/09  14:14:31  arvind
# 	OSF DCE 1.2.2 Drop 4 + fix
# 	[1996/10/09  14:13:37  arvind]
# 
# Revision /main/DCE_1.2.2/6  1996/10/08  18:55 UTC  sommerfeld
# 	CHFts20186: fix build of security/kutils
# 	[1996/10/08  16:10 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/2]
# 
# Revision /main/DCE_1.2.2/5  1996/10/02  20:23 UTC  sommerfeld
# 	do it right this time :-).
# 	[1996/10/02  20:18 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
# 
# Revision /main/DCE_1.2.2/4  1996/10/02  18:28 UTC  cuti
# 	Merge of cuti_pk_fix
# 
# 	Conditionalize  HP build not to use pkss/pkc
# 	[1996/09/10  18:38 UTC  arvind  /main/DCE_1.2.2/arvind_pkss/3]
# 	Revision /main/DCE_1.2.2/cuti_pk_fix/2  1996/10/02  17:48 UTC  cuti
# 	Make HP800 works
# 
# Revision /main/DCE_1.2.2/cuti_pk_fix/1  1996/09/26  16:18 UTC  cuti
# 	Remove pkc for international build
# 
# Revision /main/DCE_1.2.2/2  1996/09/09  21:24 UTC  arvind
# 	PKSS drop from DEC (DCE1.2.2)
# 	[1996/08/30  15:34 UTC  arvind  /main/DCE_1.2.2/arvind_pkss/1]
# 
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 	[1996/06/04  18:49 UTC  psn  /main/DCE122_PK/1]
# 	#
# 	Merge udpated pvtkey stuff
# 	#
# 	Add psm
# 	[1996/05/13  20:44 UTC  cuti  /main/DCE_1.2/DCE12_PK/cuti_pk_new/1]
# 	#
# 
# 	Update OSF copyright year
# 	[1996/03/09  22:42:30  marty]
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/15  20:58 UTC  greg
# 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
# 	[1995/04/22  13:33 UTC  greg  /main/SEC_migrate_merge/1]
# 
# 	build utils before server.  Some idl-derived .h files
# 	are needed during the server/rrs build.
# 	[1995/02/24  15:33 UTC  greg  /main/SEC_migrate/1]
# 
# Revision /main/DCE_1.2.2/1  1996/06/11  17:55 UTC  aha
# 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
# 
# Revision /main/DCE122_PK/1  1996/06/04  18:49 UTC  psn
# 	Update OSF copyright year
# 	[1996/03/09  22:42:30  marty]
# 	#
# 
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/05/15  20:58 UTC  greg
# 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
# 	[1995/04/22  13:33 UTC  greg  /main/SEC_migrate_merge/1]
# 
# 	build utils before server.  Some idl-derived .h files
# 	are needed during the server/rrs build.
# 	[1995/02/24  15:33 UTC  greg  /main/SEC_migrate/1]
# 
# Revision /main/DCE_1.2/DCE12_PK/1  1996/05/15  13:33 UTC  cuti
# 	Merge udpated pvtkey stuff
# 
# 	Add psm
# 	[1996/05/13  20:44 UTC  cuti  /main/DCE_1.2/DCE12_PK/cuti_pk_new/1]
# 	Revision /main/DCE_1.2/1  1996/01/03  19:05 UTC  psn
# 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
# 	[1995/04/22  13:33 UTC  greg  /main/SEC_migrate_merge/1]
# 
# 	build utils before server.  Some idl-derived .h files
# 	are needed during the server/rrs build.
# 	[1995/02/24  15:33 UTC  greg  /main/SEC_migrate/1]
# 
# Revision 1.1.9.6  1994/08/10  19:34:57  burati
# 	Don't build security/helper directory if NO_DFS is defined
# 	[1994/08/10  18:37:42  burati]
# 
# Revision 1.1.9.5  1994/08/09  17:32:35  burati
# 	DFS/EPAC/KRPC/dfsbind changes
# 	[1994/08/09  16:58:03  burati]
# 
# Revision 1.1.9.4  1994/03/25  19:35:37  sekhar
# 	Merged with changes from 1.1.9.3
# 	[1994/03/25  19:35:28  sekhar]
# 
# 	[ OT 10164 ] Security SVC code drop ( sec_svc_bl1 )
# 	             ( added idl & h to SAMIDL_SUBDIRS )
# 	[1994/03/25  19:33:05  sekhar]
# 
# Revision 1.1.9.3  1994/03/09  15:50:39  wray
# 	Add GSSAPI (OT#10019)
# 	[1994/03/09  15:39:34  wray]
# 
# Revision 1.1.9.2  1994/01/13  14:28:04  zeliff
# 	OT9732: Change ordering of audit on EXPINC_SUBDIRS line
# 	[1994/01/13  14:15:13  zeliff]
# 
# Revision 1.1.9.1  1994/01/12  20:51:07  zeliff
# 	OT9726: turn Audit on in the build
# 	[1994/01/12  19:36:53  zeliff]
# 
# Revision 1.1.7.2  1992/12/29  12:36:04  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:32:02  zeliff]
# 
# Revision 1.1.3.3  1992/04/30  21:17:04  john
# 	Revision 1.1.2.2  1992/04/10  14:42:01  john
# 	Added SETUP_SUBDIRS.
# 	[1992/04/16  13:25:03  john]
# 
# Revision 1.1.3.2  1992/04/07  19:17:57  mckeen
# 	Commented out DATA_SUBDIRS
# 	[1992/03/13  15:41:27  mckeen]
# 
# Revision 1.1  1992/01/19  14:55:50  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=9 12/09/91 //littl/prgy/src
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# top level Makefile
#

SETUP_SUBDIRS = krb5
SAMIDL_SUBDIRS = h idl ${AUDIT_SUBDIRS} ${PKC_SUBDIRS}
EXPINC_SUBDIRS = h idl ${AUDIT_SUBDIRS} client server krb5 helper gssapi ${PKC_SUBDIRS}
EXPLIB_SUBDIRS = ${AUDIT_SUBDIRS} utils server client krb5 psm ${KEXPLIB_SUBDIRS} ${DFS_EXPLIB_SUBDIRS} ${PKC_SUBDIRS}
SUBDIRS = ${AUDIT_SUBDIRS} idl server client krb5 h gssapi ${PKC_SUBDIRS}

.ifndef NO_AUDIT
AUDIT_SUBDIRS ?= audit
.endif

.ifndef NO_DFS
.ifndef KRPC_ONLY
DFS_EXPLIB_SUBDIRS = helper
.endif
KEXPLIB_SUBDIRS = kutils
.endif

.if ${TARGET_MACHINE}!="HP800" && defined (USE_DES)
PKC_SUBDIRS=pkc
.endif

.include <${RULES_MK}>
