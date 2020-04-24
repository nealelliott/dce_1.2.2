# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.2  1996/03/11  02:38:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:36  marty]
#
# Revision 1.1.8.1  1995/12/11  21:56:11  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/10/11  20:40 UTC  bissen
# 	Merge Hpdce02_04 to HPDCE02 branch
# 	[1995/12/11  20:56:02  root]
# 
# Revision 1.1.6.2  1994/09/22  21:27:00  fadden
# 	Add run.hcell and its subdirs
# 	[1994/09/22  21:22:04  fadden]
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  14:48 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/bissen_systest/HPDCE01/3  1994/04/01  17:44 UTC  truitt
# 
# 	HP revision /main/bissen_systest/HPDCE01/truitt_junk/3  1994/04/01  17:12 UTC  truitt
# 	Fix other paths
# 
# 	HP revision /main/bissen_systest/HPDCE01/truitt_junk/2  1994/04/01  16:57 UTC  truitt
# 	fix VPATH
# 
# 	HP revision /main/bissen_systest/HPDCE01/truitt_junk/1  1994/04/01  16:34 UTC  truitt
# 	add cds-specific tet files
# 
# 	HP revision /main/bissen_systest/HPDCE01/1  1994/02/17  21:34  bissen
# 	Insert pithy comment here
# 
# 	HP revision /main/bissen_systest/1  1994/02/15  21:27  bissen
# 	remove OTHERS, ILIST, IDIR, run.cds references
# 
# Revision 1.1.4.2  1993/09/30  20:22:22  qbarry
# 	Added stuff for RUN.CDS.
# 	[1993/09/29  20:16:23  qbarry]
# 
# Revision 1.1.4.1  1993/09/23  15:37:35  cmckeen
# 	Added IDIR line
# 	[1993/09/23  15:37:23  cmckeen]
# 
# Revision 1.1.2.2  1993/03/31  19:54:20  eperkins
# 	Initial version.
# 	[1993/03/31  19:53:59  eperkins]
# 
# $EndLog$

tet_code_IDIR		 = /tet/cds/
tet_scen_IDIR		 = /tet/cds/
tetbuild.cfg_IDIR	 = /tet/cds/
tetclean.cfg_IDIR	 = /tet/cds/
tetexec.cfg_IDIR	 = /tet/cds/

cds_abuse_IDIR		 = /tet/cds/ts/cdsabuse/

dcesx_IDIR		 = /tet/cds/ts/dcesx/
dcesx.tet_IDIR		 = /tet/cds/ts/dcesx/

dcecdsrep_IDIR		 = /tet/cds/ts/dcecdsrep/
dcecdsrep.data_IDIR	 = /tet/cds/ts/dcecdsrep/
dcecdsrep.functions_IDIR = /tet/cds/ts/dcecdsrep/
run.hcell_IDIR	= /tet/systest/cds/bin/

SUBDIRS	= cds.server.4 cds.acl.6 hclcfg001 hclrel001  
SCRIPTS = cds_abuse dcesx.tet \
          dcecdsrep dcecdsrep.data dcecdsrep.functions \
          tet_code tet_scen tetbuild.cfg tetclean.cfg tetexec.cfg

ILIST = ${SCRIPTS} dcesx

IMODE = 777

VPATH = ../../../../directory/${TARGET_MACHINE}:../../../../test/directory/:../../tools\
        :../../../../test/directory/xds:../../../directory/cds/dcesx

PROGRAMS = acl_tester dcesx

acl_tester_OFILES = acl_tester.o acl_util.o

dcesx_OFILES = dcesx.o \
	       dcesx0_cstub.o dcesx0_sstub.o \
	       dcesx1_cstub.o dcesx1_sstub.o \
	       dcesx2_cstub.o dcesx2_sstub.o \
	       dcesx3_cstub.o dcesx3_sstub.o \
	       dcesx4_cstub.o dcesx4_sstub.o \
	       dcesx5_cstub.o dcesx5_sstub.o \
	       dcesx6_cstub.o dcesx6_sstub.o \
	       dcesx7_cstub.o dcesx7_sstub.o \
	       dcesx8_cstub.o dcesx8_sstub.o \
	       dcesx9_cstub.o dcesx9_sstub.o

IDLFLAGS = -keep c_source -cepv -no_mepv

INCFLAGS	= -I../../tools

CFLAGS		= -Dunix -DDEBUG -DDNS_RPC -DDNS_V3API -DDNS_CDS -DDCE_SEC -DDNS_CMA ${${TARGET_MACHINE}_CFLAGS}
LDFLAGS		= ${${TARGET_MACHINE}_LDFLAGS}

.ifdef INSTR
LIBS		= -lxx_ -lmsgq ${${TARGET_MACHINE}_LIBS} -ldce -lm
.else
LIBS		= -lxx_ -lmsgq ${${TARGET_MACHINE}_LIBS} -ldce
.endif

LIBFLAGS	= -L../../tools

IDLFILES	= dcesx0.idl \
	          dcesx1.idl \
	          dcesx2.idl \
	          dcesx3.idl \
	          dcesx4.idl \
	          dcesx5.idl \
	          dcesx6.idl \
	          dcesx7.idl \
	          dcesx8.idl \
	          dcesx9.idl

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

dcesx.o :	dcesx0.h \
                dcesx1.h \
                dcesx2.h \
                dcesx3.h \
                dcesx4.h \
                dcesx5.h \
                dcesx6.h \
                dcesx7.h \
                dcesx8.h \
                dcesx9.h





