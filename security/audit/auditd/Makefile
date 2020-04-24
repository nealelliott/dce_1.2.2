# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# (c) Copyright International Business Machines Corp. 1994, 1996
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.7.1  1996/07/08  18:24:34  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:23 UTC  arvind  /main/arvind_audit122/1]
#
# 	Audit for 1.2.2
# 	[1996/03/09  23:27:01  marty  1.1.5.2]
#
# Revision 1.1.5.2  1996/03/09  23:27:01  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:32  marty]
# 
# Revision 1.1.5.1  1995/12/08  17:37:58  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/3  1995/08/18  19:53 UTC  kline_s
# 	Merge fix for CHFts16115
# 
# 	HP revision /main/HPDCE02/3  1995/08/18  19:52 UTC  kline_s
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/2  1995/08/17  21:26 UTC  kline_s
# 	CHFts16115: Remove DES_HIDDEN conditional - only using one protection
# 	level due to singular Domestic/Export release.
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/07/06  19:50 UTC  kline_s
# 	Distinguish between domestic and international (ie. no encryption) builds.
# 
# 	HP revision /main/HPDCE02/1  1994/10/13  13:36 UTC  mothra
# 	add audit_update.h to translate rule for dmn_updt_inform.o
# 	[1995/12/08  17:17:28  root]
# 
# Revision 1.1.2.12  1994/09/23  21:26:10  weisz
# 	CR 11123: remove hardcoded -g from CFLAGS.
# 	[1994/09/23  21:25:39  weisz]
# 
# Revision 1.1.2.11  1994/09/07  15:48:35  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:32  marty]
# 
# Revision 1.1.2.10  1994/07/26  17:27:02  weisz
# 	CR 11425: remove own acl manager and use DCE acl mgr library.
# 	[1994/07/26  17:24:46  weisz]
# 
# Revision 1.1.2.9  1994/06/29  12:16:46  zeliff
# 	ot10155: change CFLAGS assignment operator
# 	[1994/06/29  12:11:43  zeliff]
# 
# Revision 1.1.2.8  1994/03/11  22:48:24  luan
# 	CR 9775 9776: Moved svc stuff to libaudit directory.
# 	[1994/03/11  22:40:43  luan]
# 
# Revision 1.1.2.7  1994/03/02  20:46:13  luan
# 	CR 10035: Changed to use rdaclifv0.idl in security/idl, and
# 	defuncted security/audit/idl/rdaclif.idl.
# 	[1994/03/02  20:42:38  luan]
# 
# Revision 1.1.2.6  1994/02/18  19:35:39  luan
# 	CR 9777: Removed dceaudmac.h from SAMIDL_INCLUDES line
# 	since it is used only in auditd directory.
# 	[1994/02/11  02:52:07  luan]
# 
# Revision 1.1.2.5  1994/01/14  17:56:00  zeliff
# 	OT9744: change IDIR for header files
# 	[1994/01/14  17:50:24  zeliff]
# 
# Revision 1.1.2.4  1994/01/13  18:43:27  zeliff
# 	OT9735: change to .include syntax for ODE2.3 make
# 	[1994/01/13  18:40:20  zeliff]
# 
# Revision 1.1.2.3  1994/01/11  18:28:35  zeliff
# 	OT9716: restructured the way SAMS files are processed
# 	[1994/01/11  16:46:14  zeliff]
# 
# Revision 1.1.2.2  1993/12/22  13:39:45  zeliff
# 	Added include of machdep.mk
# 	[1993/12/22  13:38:21  zeliff]
# 
# Revision 1.1.2.1  1993/12/22  12:38:33  zeliff
# 	Removed references to serviceability library
# 	[1993/12/21  15:22:39  zeliff]
# 
# 	First version of code
# 	[1993/12/21  15:00:57  zeliff]
# 
# $EndLog$
#
# DCE Audit Daemon 
#

VPATH = ../idl:../../idl

# Flags for the NIDL compiler
audit_update.idl_IDLFLAGS =  -keep c_source -cepv 
IDLFLAGS =  -keep c_source 

IDLFILES = audit_update.idl audit_log.idl audit_control.idl rdaclifv0.idl

CFLAGS  += ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../h -I../idl

# Programs to build
PROGRAMS = auditd 

# Files to install
ILIST = auditd

# Directory in which to install the files
IDIR = /bin/

STUBS = audit_control_sstub.o audit_log_sstub.o \
	audit_update_cstub.o rdaclifv0_sstub.o

ACL = dmn_acl_mgr.o

SVC = dmn_main.o dmn_rpc_manager.o dmn_esl_file_write.o \
		dmn_svc.o dmn_updt_inform.o 

auditd_OFILES = ${STUBS} dmn_main.o dmn_esl_file_write.o dmn_rpc_manager.o \
		dmn_updt_inform.o ${ACL} dmn_svc.o

audit_update_cstub.o: audit_update_cstub.c

audit_log_sstub.o: audit_log_sstub.c

audit_control_sstub.o: audit_control_sstub.c

dmn_updt_inform.o: audit_update.idl audit_update.h

rdaclifv0_sstub.o: rdaclifv0_sstub.c
rdaclifv0.idl_IDLFLAGS = -keep c_source -no_mepv
rdaclifv0.idl_ACF = rdaclifv0.acf

LIBS = -laudit -ldce

.include <${RULES_MK}>

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

