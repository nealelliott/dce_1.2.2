# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.18.1  1996/10/03  17:48:38  arvind
# 	Merge from sommerfeld_pk_kdc to aha_pk9
# 	[1996/08/13  21:33 UTC  aha  /main/aha_pk9/1]
#
# 	add -DOSF_DCE because we include krb5.h
# 	[1996/08/09  18:00 UTC  sommerfeld  /main/sommerfeld_pk_kdc/1]
#
# Revision 1.1.15.3  1996/03/09  23:06:10  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:25:00  marty]
# 
# Revision 1.1.15.2  1995/12/08  21:19:07  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:28 UTC  jrr
# 	Add libm.a to the RIOS_LIBS.
# 
# 	HP revision /main/HPDCE02/15  1995/10/10  15:18 UTC  truitt
# 	Merge.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts49/1  1995/10/05  14:56 UTC  truitt
# 	CHFts16529: Get rid of debug level.
# 
# 	HP revision /main/HPDCE02/14  1995/09/29  20:46 UTC  tatsu_s
# 	Submitted the fix for CHFts16512.
# 
# 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/2  1995/09/28  18:22 UTC  tatsu_s
# 	Added -ldcedpvt for libdcecp.
# 
# 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/21  15:25 UTC  tatsu_s
# 	Added -ldcedpvt.
# 
# 	HP revision /main/HPDCE02/13  1995/08/18  19:53 UTC  kline_s
# 	Merge fix for CHFts16115
# 
# 	HP revision /main/HPDCE02/13  1995/08/18  19:52 UTC  kline_s
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/2  1995/08/17  21:29 UTC  kline_s
# 	CHFts16115: Remove DES_HIDDEN conditional - only using one protection
# 	level due to singular Domestic/Export release.
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/07/06  19:50 UTC  kline_s
# 	Distinguish between domestic and international (ie. no encryption) builds.
# 
# 	HP revision /main/HPDCE02/11  1995/07/14  18:32 UTC  truitt
# 	Merge changes.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts39/1  1995/07/14  18:30 UTC  truitt
# 	CHFts15725: Add HP800 directory for optimization purposes.
# 
# 	HP revision /main/HPDCE02/10  1995/05/23  19:00 UTC  truitt
# 	Merge removal of -g from CFLAGS.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts30/1  1995/05/23  18:59 UTC  truitt
# 	CHFts15278: Remove -g from cflags.
# 
# 	HP revision /main/HPDCE02/9  1995/03/07  22:27 UTC  jrr
# 	Allow building of libdcecp.a
# 
# 	HP revision /main/HPDCE02/jrr_dcp_host/1  1995/03/06  15:34 UTC  jrr
# 	Changes to allow building of libdcecp.a when required.
# 
# 	HP revision /main/HPDCE02/8  1994/12/07  18:13 UTC  truitt
# 	Add new clock functionality.
# 
# 	HP revision /main/HPDCE02/7  1994/10/26  17:45 UTC  truitt
# 	Remove link with -ldbm for HP800.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_01/1  1994/10/26  17:42 UTC  truitt
# 	Remove link with -ldbm for HP800.
# 
# 	HP revision /main/HPDCE02/6  1994/10/24  11:02 UTC  root
# 	change LIBS to dcecp_LIBS so libdcecp.sl will build
# 
# 	HP revision /main/HPDCE02/5  1994/10/21  16:52 UTC  pulsinelli
# 	Merge changes for libdcecp.sl work.
# 
# 	HP revision /main/HPDCE02/pulsinelli_dcecp_sl/1  1994/10/19  19:04 UTC  pulsinelli
# 	Make changes necessary for libdcecp.sl
# 
# 	HP revision /main/HPDCE02/4  1994/10/13  14:38 UTC  truitt
# 	add cprpc_hcell.h dependency to CDS_OFILES
# 
# 	HP revision /main/HPDCE02/3  1994/09/22  15:37 UTC  jane
# 	merge from Hpdce02_02
# 	[1995/12/08  17:53:46  root]
# 
# Revision 1.1.7.45  1994/09/14  19:32:25  coren
# 	Added -D_DCECP_TEST to CFLAGS so as to define signal-handling code
# 	needed by TET. (CR 11832)
# 	[1994/09/13  18:27:36  coren]
# 
# Revision 1.1.7.44  1994/09/06  16:22:59  kevins
# 	OT 11694 added rgy_pgo_group.c rgy_pgo_account.c rgy_pgo_utils.c
# 	[1994/09/06  16:20:48  kevins]
# 
# Revision 1.1.7.43  1994/09/01  19:04:37  truitt
# 	Add schema operation and utility files.
# 	[1994/09/01  18:04:21  truitt]
# 
# Revision 1.1.7.42  1994/09/01  13:34:38  rousseau
# 	Add -ansi to at386 compiles (CR 11987)
# 	[1994/09/01  13:33:59  rousseau]
# 
# 	HP revision /main/HPDCE02/2  1994/08/19  21:32 UTC  pulsinelli
# 	Merge from Hpdce02_01
# 
# Revision 1.1.7.41  1994/08/12  20:03:37  kevins
# 	OT 9691 Add registry connect command
# 	[1994/08/12  20:00:12  kevins]
# 
# Revision 1.1.7.40  1994/08/02  20:09:54  kevins
# 	OT 10187 Added cdsutils.a for cdscache stuff.
# 	[1994/08/02  20:09:31  kevins]
# 
# 	OT 10187 Add cdsutils.a for cdscache commands.
# 	[1994/07/25  10:39:01  kevins]
# 
# Revision 1.1.7.39  1994/07/22  21:40:43  pwang
# 	Removed intermediate attr schema stuff of dhd [OT#10262]
# 	[1994/07/22  19:41:29  pwang]
# 
# 	Merge up.
# 	[1994/06/23  15:57:29  rousseau]
# 
# Revision 1.1.7.38  1994/07/14  11:18:46  kevins
# 	Merged with changes from 1.1.7.37
# 	[1994/07/14  11:18:33  kevins]
# 
# 	CR 11219 CDS_Tower encoding fix.
# 	[1994/07/14  11:16:32  kevins]
# 
# Revision 1.1.7.37  1994/07/13  18:00:05  mori_m
# 	CR 11268  libtcl now depends on libdce (messaging facility)
# 	[1994/07/13  17:57:43  mori_m]
# 
# 	Merge up.
# 	[1994/06/23  15:57:29  rousseau]
# 
# Revision 1.1.7.36  1994/07/12  19:26:19  zee
# 	      add cdscp_cellalias.o, cprpc_hcell.idl, stubs.
# 	[1994/07/12  15:54:24  zee]
# 
# Revision 1.1.7.35  1994/07/01  14:25:24  rousseau
# 	Implemented utc subcomponent.
# 	[1994/07/01  14:24:30  rousseau]
# 
# Revision 1.1.7.33  1994/06/22  16:08:10  pwang
# 	Access the attr_schema idl interface
# 	[1994/06/22  15:44:52  pwang]
# 
# 	HP revision /main/HPDCE02/1  1994/07/19  21:22 UTC  mothra
# 	fix building of cprpc_clerk.h and cprpc_server.h
# 
# Revision 1.1.7.32  1994/06/09  16:01:31  devsrc
# 	cr10892 - fix copyright
# 	[1994/06/09  15:46:52  devsrc]
# 
# Revision 1.1.7.31  1994/05/16  20:47:41  rousseau
# 	Fix build problem (CR 10642).
# 	[1994/05/16  20:46:10  rousseau]
# 
# Revision 1.1.7.30  1994/05/13  22:22:38  melman
# 	renamed dcecp's dts.h to dcpdts.h
# 	[1994/05/13  22:18:14  melman]
# 
# Revision 1.1.7.29  1994/05/12  18:29:28  salamone
# 	Added support for sec_admin thru the registry command.
# 	[1994/05/12  18:26:48  salamone]
# 
# Revision 1.1.7.28  1994/05/09  17:59:15  rsalz
# 	Add STRICT_COMPILER_WARNINGS variable (OT CR 10581).
# 	[1994/05/09  17:20:03  rsalz]
# 
# Revision 1.1.7.27  1994/04/29  18:45:47  melman
# 	removed -lsvc again
# 	[1994/04/26  21:48:25  melman]
# 
# Revision 1.1.7.26  1994/04/26  20:03:41  pwang
# 	Added secval object (OT#10262)
# 	[1994/04/26  19:35:33  pwang]
# 
# Revision 1.1.7.25  1994/04/26  15:32:30  salamone
# 	Added "log" command support for DCE serviceability.
# 	[1994/04/26  14:26:59  salamone]
# 
# Revision 1.1.7.24  1994/04/22  19:59:11  pwang
# 	Added dced commands support (OT#10262)
# 	[1994/04/22  19:48:07  pwang]
# 
# Revision 1.1.7.23  1994/04/18  17:00:27  rousseau
# 	Added audit_control.acf
# 	[1994/04/18  17:00:20  rousseau]
# 
# Revision 1.1.7.22  1994/04/07  10:21:59  kevins
# 	CR 10304 Export some header files for cross component dependency issues
# 	[1994/04/07  10:17:39  kevins]
# 
# 	CR 10304 Modifications to help remove cross component dependency build issues
# 	[1994/04/07  10:10:27  kevins]
# 
# 	Merge time
# 	[1994/03/29  12:57:43  kevins]
# 
# Revision 1.1.7.21  1994/03/28  23:00:23  melman
# 	Added rpcentry show and binding utilities
# 	[1994/03/28  22:53:48  melman]
# 
# Revision 1.1.7.20  1994/03/17  21:31:35  rousseau
# 	Reordered libs line to really fix CR 10165.
# 	[1994/03/17  21:31:21  rousseau]
# 
# Revision 1.1.7.19  1994/03/17  20:37:12  rousseau
# 	Remove extra libraries from link line (CR 10165).
# 	[1994/03/17  20:36:54  rousseau]
# 
# Revision 1.1.7.18  1994/02/18  19:41:21  ohara
# 	need libelf and can't pass -g to linker
# 	[1994/02/18  15:49:11  ohara]
# 
# Revision 1.1.7.17  1994/02/04  18:31:28  rousseau
# 	Add RIOS_LIBS for CR 9881.
# 	[1994/02/04  18:30:56  rousseau]
# 
# Revision 1.1.7.16  1994/02/03  18:23:30  kevins
# 	Merged with changes from 1.1.7.15
# 	[1994/02/03  18:23:25  kevins]
# 
# 	Added file rgy_utils.c for the rgy subcomponent
# 	[1994/02/03  18:22:55  kevins]
# 
# Revision 1.1.7.15  1994/02/03  16:31:16  rousseau
# 	Library ordering change for CR 9877.
# 	[1994/02/03  16:30:59  rousseau]
# 
# Revision 1.1.7.14  1994/02/02  23:13:12  rousseau
# 	Changed VPATH to search /usr/include/dce.
# 	[1994/02/02  23:12:52  rousseau]
# 
# Revision 1.1.7.13  1994/02/02  19:53:42  rousseau
# 	Fixed CR 9868 9869 9809
# 	[1994/02/02  19:53:27  rousseau]
# 
# Revision 1.1.7.12  1994/01/23  22:07:34  rousseau
# 	Removed dependency on dtssts.h, and make audit changes.
# 	[1994/01/23  22:07:13  rousseau]
# 
# Revision 1.1.7.11  1994/01/10  19:54:50  rousseau
# 	Fix rios build problems.
# 	[1994/01/10  19:53:32  rousseau]
# 
# Revision 1.1.7.10  1994/01/06  16:50:57  rousseau
# 	Merged with changes from 1.1.7.9
# 	[1994/01/06  16:50:52  rousseau]
# 
# 	Added aud components.
# 	[1994/01/06  16:49:49  rousseau]
# 
# Revision 1.1.7.9  1994/01/05  16:30:23  kevins
# 	Added IDIR for dcecp
# 	[1994/01/05  15:54:45  kevins]
# 
# $EndLog$
#

# sams stuff
SAMSFILES		= dcp.sams
SAMIDL_INCLUDES 	= dcedcpmsg.h
SAMS_TARGETS    	= dcedcpmsg.h dcedcpmsg.c dcedcpmac.h \
			  dcedcpsvc.h dcedcpsvc.c dcedcp.cat

VPATH			= sams:../../../export/${target_machine}/usr/include/dce

EXPDIR			= /usr/include/dce/
libdcecp.sl_EXPDIR	= /usr/lib/
libdcecp.a_EXPDIR	= /usr/lib/

# cds stuff
CDS_OFILES		= cdscp.o cdscp_object.o cdscp_link.o \
		          cdscp_utils.o cdscp_clearinghouse.o \
	          	  cdscp_cache.o cdscp_cellalias.o \
			  cprpc_clerk_cstub.o cprpc_server_cstub.o \
			  cprpc_hcell_cstub.o cds_clerkserver_cstub.o

CDS_IDLFILES    	= cprpc_clerk.idl cprpc_server.idl \
			  cds_types.idl cprpc_hcell.idl cds_clerkserver.idl

cds_clerkserver_ACF     =
cprpc_clerk_ACF 	=
cprpc_server_ACF 	=
cprpc_hcell_ACF 	=

# dts stuff
# We need dtss_ctrl_cstub.c for the time control RPC's
# We also depend on dtssts.h which we count on being exported from dts.
DTS_OFILES		= dts_ops.o dts_util.o clock.o dtss_ctrl_cstub.o \
                          rutc_cstub.o
DTS_IDLFILES    	= dtss_ctrl.idl rutc.idl
dtss_ctrl_ACF   	=

# ACL support
ACL_OFILES		= acl_tclif.o acl_edit.o

# LOG support
LOG_OFILES              = log_tclif.o log_edit.o service_cstub.o
log_edit.o_INCFLAGS     = -I../../dce/utils/svc
LOG_IDLFILES            = service.idl
service_ACF             =

# AUD support
AUD_OFILES		= aud_ops.o aud_util.o audit_control_cstub.o
AUD_IDLFILES		= audit_control.idl
audit_control_ACF       = 

# RGY support
RGY_OFILES              = rgy.o rgy_utils.o rgy_sa_tclif.o rgy_sa_replist.o \
                          rgy_pgo_group.o rgy_pgo_account.o rgy_pgo_utils.o

# SCHEMA support
SCHEMA_OFILES           = schema_ops.o schema_utils.o

# RPC support
RPC_OFILES              = rpccp.o

# DCED (DHD) support
DHD_OFILES		= dhd_util.o dhd_server.o dhd_hostdata.o \
			  dhd_keytab.o dhd_secval.o

# UTC support
UTC_OFILES		= utc_ops.o

# DCECP Utilities
UTIL_OFILES		= util_general.o util_help.o util_login.o \
			  util_signal.o util_var.o util_binding.o \

# common stuff
commands.o_INCFLAGS	= -I../../time/common

IDLFILES        	= ${CDS_IDLFILES} \
			  ${DTS_IDLFILES} \
			  ${AUD_IDLFILES} \
			  ${LOG_IDLFILES}

IDLFLAGS		= -keep c_source
IDLINCFLAGS		= -I../../directory/cds/includes \
			  -I../../directory/cds/stubs \
			  -I../../time/common \
			  -I../../admin/dced/idl \
			  -I../../security/audit/auditd
LIBFLAGS        	= -L../../directory/cds/cache \
                          -L../../directory/cds/library
AT386_LIBS		= -lmach
HP800_LIBS      	= -lM
RIOS_LIBS		= -lkrb5 -lcfg -lodm -lm
SVR4_LIBS		= -lelf_r
dcecp_LIBS		= -ldcecp -lcdscache -lcdsutil -laudit -ltcl -ldcedpvt -ldce \
			  ${${TARGET_MACHINE}_LIBS}

RIOS_CFLAGS		= -Dunix -D_BSD
AT386_CFLAGS		= -ansi

CFLAGS			= ${${TARGET_MACHINE}_CFLAGS} ${STRICT_COMPILER_WARNINGS} -D_DCECP_TEST

# needed for files which include krb5.h
rgy_pgo_account.o_CFLAGS = ${CFLAGS} -DOSF_DCE
commands.o_CFLAGS = ${CFLAGS} -DOSF_DCE


HFILES			= dcecp.h
INCLUDES 		= dcecp.h dcedcpmsg.h
dcecp_OFILES		= main.o

# Programs to build
PROGRAMS                = dcecp
COMMON_OFILES           = dcpAppInit.o commands.o parseargv.o \
			  dcedcpsvc.o dcedcpmsg.o \
			  ${RGY_OFILES} ${SCHEMA_OFILES} ${ACL_OFILES} \
			  ${LOG_OFILES} ${AUD_OFILES} ${CDS_OFILES} \
			  ${DTS_OFILES} ${DHD_OFILES} ${UTC_OFILES} \
			  ${RPC_OFILES} ${UTIL_OFILES}

# Building libdcecp.*
.if defined(NO_SHARED_LIBRARIES) || defined(USE_STATIC_LIBRARIES)
  LIBRARIES	        = libdcecp.a
  libdcecp.a_OFILES	= ${COMMON_OFILES}
.else
  SHARED_LIBRARIES	= libdcecp.sl
  libdcecp.sl_OFILES	= ${COMMON_OFILES}
.endif
libdcecp.sl_LIBS	= -ldcedpvt -ldce

# DCECP internal scripts
SUBDIRS			= scripts

# Directory in which to install the files
dcecp_IDIR		= /bin/
dcedcp.cat_IDIR 	= /nls/msg/en_US.ASCII/

# Installing libdcecp.*
.if defined(NO_SHARED_LIBRARIES) || defined(USE_STATIC_LIBRARIES)
  libdcecp.a_IDIR       = /usr/lib/
  ILIST                 = dcecp dcedcp.cat libdcecp.a
.else
  libdcecp.sl_IDIR	= /usr/lib/
  ILIST			= dcecp dcedcp.cat libdcecp.sl 
.endif

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

main.o:			util_login.h dcedcpmsg.h
commands.o:		util_login.h rpccp.h cdscp.h dts_proto.h rgy.h \
			rgy_sa_tclif.h aud.h util_help.h dhd.h dcedcpmsg.h
util_general.o:         util_general.h dcedcpmsg.h
util_login.o:		util_login.h dcedcpmsg.h
util_signal.o:		util_signal.h dcedcpmsg.h
util_var.o:		util_var.h dcedcpmsg.h
util_binding.o:		util_binding.h dcedcpmsg.h
rpccp.o:		rpccp.h util_binding.h util_help.h dcedcpmsg.h
${RGY_OFILES}:          rgy.h rgy_sa_tclif.h rgy_sa_replist.h util_help.h \
                        dcedcpmsg.h
${SCHEMA_OFILES}:       rgy.h util_help.h dcedcpmsg.h
${CDS_OFILES}: 		cdscp.h util_help.h cprpc_clerk.h cprpc_server.h \
                        cprpc_hcell.h dcedcpmsg.h
${DTS_OFILES}:		dcp_dts.h dts_proto.h util_login.h util_help.h \
                        rutc.h dcedcpmsg.h
${ACL_OFILES}: 		acl_edit.h util_help.h dcedcpmsg.h
${LOG_OFILES}:          log_edit.h util_help.h dcedcpmsg.h
${AUD_OFILES}:		aud.h util_help.h dcedcpmsg.h
${UTC_OFILES}:		dcp_utc.h util_help.h dcedcpmsg.h
