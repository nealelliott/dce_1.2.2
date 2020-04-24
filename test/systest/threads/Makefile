# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.24.2  1996/03/11  02:48:59  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:09:42  marty]
#
# Revision 1.1.24.1  1995/12/11  22:49:10  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/07/05  17:59 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/3  1994/04/01  19:29 UTC  truitt
# 
# 	HP revision /main/HPDCE01/truitt_junk/1  1994/04/01  19:17 UTC  truitt
# 	cleanup make for threads system tests
# 
# 	HP revision /main/HPDCE01/2  1994/03/18  18:44 UTC  bissen
# 	merge changes
# 
# 	HP revision /main/HPDCE01/bissen_systest/1  1994/03/18  18:42 UTC  bissen
# 	add .data files _IDIR
# 
# 	HP revision /main/HPDCE01/1  1994/01/31  19:15  bissen
# 	merge kk and hpdce01
# 	[1995/12/11  22:23:07  root]
# 
# Revision 1.1.7.3  1993/08/25  14:46:29  giza
# 	Defuncted dcethsig
# 	[1993/08/25  14:45:52  giza]
# 
# Revision 1.1.21.1  1993/10/15  21:15:59  ohara
# 	SVR4 porting changes - 8596
# 	[1993/10/15  16:35:35  ohara]
# 
# Revision 1.1.7.2  1993/07/09  14:44:25  root
# 	Initial King Kong branch
# 	[1993/07/09  14:43:58  root]
# 
# Revision 1.1.5.3  1993/04/23  13:52:09  eheller
# 	Merged Hp_dce1_0_1 changes
# 	[1993/04/16  17:14:20  eheller]
# 
# Revision 1.1.5.2  1993/04/07  11:19:03  mgm
# 	Changes for Gamera, taken from Hp_dce1_0_1
# 	[1993/04/07  11:18:07  mgm]
# 
# Revision 1.1.3.7  1992/11/25  20:04:41  mgm
# 	Added tet* to install
# 	[1992/11/25  20:02:24  mgm]
# 
# Revision 1.1.3.6  1992/11/20  20:54:06  mgm
# 	Added tet_scen to install
# 	[1992/11/20  20:53:42  mgm]
# 
# Revision 1.1.3.5  1992/11/17  13:44:26  mgm
# 	Added rpt.thr and run.thr to install
# 	[1992/11/17  13:44:00  mgm]
# 
# Revision 1.1.3.4  1992/11/11  21:21:04  mgm
# 	Can now install to release area from ODE
# 	[1992/11/11  21:20:33  mgm]
# 
# Revision 1.1.3.3  1992/11/04  19:05:19  mgm
# 	Added SCRIPTS to keep track of drivers
# 	[1992/11/04  19:04:41  mgm]
# 
# Revision 1.1.1.4  1992/06/16  06:49:17  root
# 	    Loading drop Dce1_0_1b19
# 
# Revision 1.1.3.4  1992/06/07  16:38:07  ramb
# 	Changed program names prvb_thd_002 to dceth002_c and
# 	                      prvb_mut_001 to dcethmut_c to be consistent
# 	with the rest of the code.
# 	[1992/06/07  16:37:31  ramb]
# 
# Revision 1.1.3.3  1992/04/15  16:57:24  rps
# 	Use tools libs
# 	[1992/04/15  15:42:12  rps]
# 
# 	Remove -no_mepv from IDLFLAGS
# 	[1992/03/23  20:16:28  rps]
# 
# Revision 1.1.3.2  1992/03/18  01:20:49  rps
# 	Initial version
# 	[1992/03/18  00:10:06  rps]
# 
# $EndLog$

dceth002_IDIR		= /tet/threads/ts/dceth002/
dceth002_c_IDIR		= /tet/threads/ts/dceth002/
dceth002.data_IDIR	= /tet/threads/ts/dceth002/

dcethcac_IDIR		= /tet/threads/ts/dcethcac/
pthreads_CACHING_IDIR	= /tet/threads/ts/dcethcac/

dcethmut_IDIR		= /tet/threads/ts/dcethmut/
dcethmut_c_IDIR		= /tet/threads/ts/dcethmut/
dcethmut.data_IDIR	= /tet/threads/ts/dcethmut/

dcethrpc_IDIR		= /tet/threads/ts/dcethrpc/
thd_client_IDIR		= /tet/threads/ts/dcethrpc/
thd_server_IDIR		= /tet/threads/ts/dcethrpc/
dcethrpc.data_IDIR	= /tet/threads/ts/dcethrpc/

tet_code_IDIR		= /tet/threads/
tet_scen_IDIR		= /tet/threads/
tetbuild.cfg_IDIR	= /tet/threads/
tetclean.cfg_IDIR	= /tet/threads/
tetexec.cfg_IDIR	= /tet/threads/

ILIST		= ${PROGRAMS} ${SCRIPTS}

SCRIPTS		= dceth002 dcethcac dcethmut dcethrpc \
		  tet_scen tet_code tetbuild.cfg tetclean.cfg tetexec.cfg

OTHERS		= dceth002.data dcethmut.data dcethrpc.data

VPATH		= ../../../test/threads/

PROGRAMS	= pthreads_CACHING thd_server thd_client \
	          dcethmut_c dceth002_c

INCFLAGS	= -I../tools

.if ${TARGET_MACHINE}=="SVR4"
CFLAGS          = -Dunix ${${TARGET_MACHINE}_CFLAGS}
.else
CFLAGS		= -g -D_BSD -Dunix ${${TARGET_MACHINE}_CFLAGS} 
.endif

LIBFLAGS	= -L../tools

.if ${TARGET_OS} == "HPUX"
BSD_DEF = -lBSD
.else
BSD_DEF = -lbsd
.endif

.if ${TARGET_MACHINE}=="SVR4"
LIBS           = -lxx_ -lutil ${${TARGET_MACHINE}_LIBS} -ldce
.else
LIBS           = ${BSD_DEF} -lxx_ -lutil ${${TARGET_MACHINE}_LIBS} -ldce
.endif

thd_server_OFILES = thd_server.o thd_sstub.o
thd_client_OFILES = thd_client.o thd_cstub.o
dcethmut_c_OFILES = dcethmut_c.o cts_test.o
dceth002_c_OFILES = dceth002_c.o cts_test.o

IDLFLAGS = -keep c_source
IDLFILES = thd.idl

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>

thd_server.o thd_client.o : thd.h
